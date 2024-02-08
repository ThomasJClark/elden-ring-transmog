#include <cstdint>
#include <iomanip>
#include <iostream>
#include <tga/param_containers.h>
#include <tga/paramdefs.h>
#include <unordered_map>

#include "TransmogMessages.hpp"
#include "TransmogShop.hpp"
#include "TransmogUtils.hpp"

using namespace TransmogShop;

#pragma pack(push, 1)
struct FindShopMenuResult
{
    std::byte shop_type;
    std::byte padding[3];
    std::int32_t id;
    ShopLineupParam *row;
};

struct FindShopLineupParamResult
{
    std::byte shop_type;
    std::byte padding[3];
    std::int32_t id;
    ShopLineupParam *row;
};

struct FindEquipParamGoodsResult
{
    std::int32_t id;
    std::int32_t unknown;
    EquipParamGoods *row;
};
#pragma pack(pop)

typedef FindShopMenuResult *FindShopMenuFn(FindShopMenuResult *result, std::byte shop_type,
                                           std::int32_t begin_id, std::int32_t end_id);
typedef void FindShopLineupParamFn(FindShopLineupParamResult *result, std::byte shop_type,
                                   std::int32_t id);
typedef void FindEquipParamGoodsFn(FindEquipParamGoodsResult *result, std::int32_t id);

static FindShopMenuFn *get_shop_menu_hook;
static FindShopMenuFn *get_shop_menu;
static FindShopLineupParamFn *get_shop_lineup_param;
static FindShopLineupParamFn *get_shop_lineup_param_hook;
static FindEquipParamGoodsFn *get_equip_param_goods;
static FindEquipParamGoodsFn *get_equip_param_goods_hook;

static ShopLineupParam transmog_head_shop_menu = {0};
static ShopLineupParam transmog_body_shop_menu = {0};
static ShopLineupParam transmog_arms_shop_menu = {0};
static ShopLineupParam transmog_legs_shop_menu = {0};

static std::unordered_map<std::int32_t, ShopLineupParam> transmog_shop_lineups;
static std::unordered_map<std::int32_t, EquipParamGoods> transmog_goods;

FindShopMenuResult *get_shop_menu_detour(FindShopMenuResult *result, std::byte shop_type,
                                         std::int32_t begin_id, std::int32_t end_id)
{
    switch (begin_id)
    {
    case transmog_head_shop_menu_id:
        result->shop_type = (std::byte)0;
        result->id = transmog_head_shop_menu_id;
        result->row = &transmog_head_shop_menu;
        break;
    case transmog_body_shop_menu_id:
        result->shop_type = (std::byte)0;
        result->id = transmog_body_shop_menu_id;
        result->row = &transmog_body_shop_menu;
        break;
    case transmog_arms_shop_menu_id:
        result->shop_type = (std::byte)0;
        result->id = transmog_arms_shop_menu_id;
        result->row = &transmog_arms_shop_menu;
        break;
    case transmog_legs_shop_menu_id:
        result->shop_type = (std::byte)0;
        result->id = transmog_legs_shop_menu_id;
        result->row = &transmog_legs_shop_menu;
        break;
    default:
        get_shop_menu(result, shop_type, begin_id, end_id);
    }

    return result;
}

void get_shop_lineup_param_detour(FindShopLineupParamResult *result, std::byte shop_type,
                                  std::int32_t id)
{
    if (shop_type == std::byte(0) && id >= transmog_head_shop_menu_id &&
        id < transmog_legs_shop_menu_id + transmog_shop_max_size)
    {
        auto entry = transmog_shop_lineups.find(id);
        if (entry != transmog_shop_lineups.end())
        {
            result->shop_type = shop_type;
            result->id = id;
            result->row = &entry->second;
            return;
        }
        else
        {
            result->row = nullptr;
            return;
        }
    }

    get_shop_lineup_param(result, shop_type, id);
}

bool logged = false;

void get_equip_param_goods_detour(FindEquipParamGoodsResult *result, std::int32_t id)
{
    auto transmog_good = transmog_goods.find(id);
    if (transmog_good != transmog_goods.end())
    {
        result->id = transmog_good->first;
        result->row = &transmog_good->second;
        result->unknown = 3;
    }
    else
    {
        get_equip_param_goods(result, id);
    }
}

void TransmogShop::initialize(ParamMap &params, MsgRepository *msg_repository)
{
    auto equip_param_protector = params[L"EquipParamProtector"];

    // Add a shop to "buy" armor pieces for each category. Note: these params control the title
    // and and icon for the shop, but otherwise aren't used for determining shop inventory.
    transmog_head_shop_menu.menuTitleMsgId = TransmogMessages::menu_text_transmog_head_id;
    transmog_head_shop_menu.menuIconId = 5;

    transmog_body_shop_menu.menuTitleMsgId = TransmogMessages::menu_text_transmog_body_id;
    transmog_body_shop_menu.menuIconId = 5;

    transmog_arms_shop_menu.menuTitleMsgId = TransmogMessages::menu_text_transmog_arms_id;
    transmog_arms_shop_menu.menuIconId = 5;

    transmog_legs_shop_menu.menuTitleMsgId = TransmogMessages::menu_text_transmog_legs_id;
    transmog_legs_shop_menu.menuIconId = 5;

    // Hook get_shop_menu() to return the above shop menus. The player can select an appearance
    // by buying an item from one of these shops for $0, since this is easier than making a custom
    // menu.
    get_shop_menu_hook = TransmogUtils::hook(
        {
            .aob =
                {// mov r9d,dword ptr [r14 + 0x14]
                 0x45, 0x8b, 0x4e, 0x14,
                 // mov r8d,dword ptr [r14 + 0x10]
                 0x45, 0x8b, 0x46, 0x10,
                 // xor edx,edx
                 0x33, 0xd2,
                 // lea rcx=>local_1d0,[rbp + 0x18]
                 0x48, 0x8d, 0x4d, 0x18,
                 // call GetShopMenu
                 0xe8, -1, -1, -1, -1},
            .offset = 0xe,
            .relative_offsets = {{0x1, 0x5}},
        },
        get_shop_menu_detour, get_shop_menu);

    auto glass_shard = reinterpret_cast<EquipParamGoods *>(params[L"EquipParamGoods"][10000]);

    // Add goods and shop entries for every armor piece the player can buy
    for (auto entry : equip_param_protector)
    {
        auto protector_id = entry.first;
        auto protector_row = reinterpret_cast<EquipParamProtector *>(entry.second);

        // Skip invalid/cut items
        auto protector_name = TransmogMessages::get_protector_name(msg_repository, protector_id);
        if (protector_name.empty() || protector_name.starts_with(u"[ERROR]"))
        {
            continue;
        }

        auto shop_lineup_param_id =
            get_transmog_shop_param_id(protector_id, protector_row->protectorCategory);

        auto goods_id = get_transmog_goods_id_for_protector(protector_id);

        transmog_shop_lineups[shop_lineup_param_id] = {
            .equipId = static_cast<std::int32_t>(goods_id),
            .value = 40,
            .mtrlId = -1,
            .sellQuantity = -1,
            .equipType = 3,
            .setNum = 1,
            .iconId = -1,
            .nameMsgId = -1,
        };

        // TODO: initialize this to a placeholder good
        auto good = *glass_shard;
        if (is_invisible_protector_id(protector_id))
        {
            good.iconId = 3142;
        }
        else
        {
            good.iconId = protector_row->iconIdM;
        }
        transmog_goods[goods_id] = good;
    }

    // Hook get_equip_param_goods() to return the above items
    get_equip_param_goods_hook = TransmogUtils::hook(
        {
            .aob =
                {// lea edx, [r8 + 3]
                 0x41, 0x8d, 0x50, 0x03,
                 // call SoloParamRepositoryImp::GetParamResCap
                 0xe8, -1, -1, -1, -1,
                 // test rax, rax
                 0x48, 0x85, 0xc0,
                 // jz end_lbl
                 0x0f, 0x84, -1, -1, -1, -1},
            .offset = -0x6a,
        },
        get_equip_param_goods_detour, get_equip_param_goods);

    // TODO: AOB
    // Hook get_shop_lineup_param() to return the above shop entries
    get_shop_lineup_param_hook = TransmogUtils::hook(
        {.offset = 0xd156c0}, get_shop_lineup_param_detour, get_shop_lineup_param);
}

void TransmogShop::deinitialize()
{
    TransmogUtils::unhook(get_shop_menu_hook);
    TransmogUtils::unhook(get_shop_lineup_param_hook);
    TransmogUtils::unhook(get_equip_param_goods_hook);
}
