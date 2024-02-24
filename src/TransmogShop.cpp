#include <cstdint>
#include <tga/paramdefs.h>
#include <unordered_map>

#include "ModUtils.hpp"
#include "ParamUtils.hpp"
#include "TransmogMessages.hpp"
#include "TransmogShop.hpp"

using namespace TransmogShop;
using namespace std;

static constexpr uint16_t invisible_icon_id = 3142;

#pragma pack(push, 1)
struct FindShopMenuResult
{
    byte shop_type;
    byte padding[3];
    int32_t id;
    ShopLineupParam *row;
};

struct FindShopLineupParamResult
{
    byte shop_type;
    byte padding[3];
    int32_t id;
    ShopLineupParam *row;
};

struct FindEquipParamGoodsResult
{
    int32_t id;
    int32_t unknown;
    EquipParamGoods *row;
};
#pragma pack(pop)

static ShopLineupParam transmog_head_shop_menu = {0};
static ShopLineupParam transmog_body_shop_menu = {0};
static ShopLineupParam transmog_arms_shop_menu = {0};
static ShopLineupParam transmog_legs_shop_menu = {0};

static unordered_map<int32_t, ShopLineupParam> transmog_shop_lineups;
static unordered_map<int32_t, EquipParamGoods> transmog_goods;

static FindShopMenuResult *(*get_shop_menu)(FindShopMenuResult *result, byte shop_type,
                                            int32_t begin_id, int32_t end_id);

FindShopMenuResult *get_shop_menu_detour(FindShopMenuResult *result, byte shop_type,
                                         int32_t begin_id, int32_t end_id)
{
    switch (begin_id)
    {
    case transmog_head_shop_menu_id:
        result->shop_type = (byte)0;
        result->id = transmog_head_shop_menu_id;
        result->row = &transmog_head_shop_menu;
        break;
    case transmog_body_shop_menu_id:
        result->shop_type = (byte)0;
        result->id = transmog_body_shop_menu_id;
        result->row = &transmog_body_shop_menu;
        break;
    case transmog_arms_shop_menu_id:
        result->shop_type = (byte)0;
        result->id = transmog_arms_shop_menu_id;
        result->row = &transmog_arms_shop_menu;
        break;
    case transmog_legs_shop_menu_id:
        result->shop_type = (byte)0;
        result->id = transmog_legs_shop_menu_id;
        result->row = &transmog_legs_shop_menu;
        break;
    default:
        get_shop_menu(result, shop_type, begin_id, end_id);
    }

    return result;
}

static void (*get_shop_lineup_param)(FindShopLineupParamResult *result, byte shop_type, int32_t id);

void get_shop_lineup_param_detour(FindShopLineupParamResult *result, byte shop_type, int32_t id)
{
    if (shop_type == byte(0) && id >= transmog_head_shop_menu_id &&
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

static void (*get_equip_param_goods)(FindEquipParamGoodsResult *result, int32_t id);

void get_equip_param_goods_detour(FindEquipParamGoodsResult *result, int32_t id)
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

void TransmogShop::initialize(MsgRepository *msg_repository)
{
    // Add a shop to "buy" armor pieces for each category. Note: these params control the title
    // and and icon for the shop, but otherwise aren't used for determining shop inventory.
    transmog_head_shop_menu.menuTitleMsgId = TransmogMessages::MenuText::transmog_head;
    transmog_head_shop_menu.menuIconId = 5;

    transmog_body_shop_menu.menuTitleMsgId = TransmogMessages::MenuText::transmog_body;
    transmog_body_shop_menu.menuIconId = 5;

    transmog_arms_shop_menu.menuTitleMsgId = TransmogMessages::MenuText::transmog_arms;
    transmog_arms_shop_menu.menuIconId = 5;

    transmog_legs_shop_menu.menuTitleMsgId = TransmogMessages::MenuText::transmog_legs;
    transmog_legs_shop_menu.menuIconId = 5;

    // Hook get_shop_menu() to return the above shop menus. The player can select an appearance
    // by buying an item from one of these shops for $0, since this is easier than making a custom
    // menu.
    ModUtils::hook(
        {
            .aob = "45 8b 4e 14"     // mov r9d,dword ptr [r14 + 0x14]
                   "45 8b 46 10"     // mov r8d,dword ptr [r14 + 0x10]
                   "33 d2"           // xor edx,edx
                   "48 8d 4d 18"     // lea rcx=>local_1d0,[rbp + 0x18]
                   "e8 ?? ?? ?? ??", // call GetShopMenu
            .offset = 14,
            .relative_offsets = {{1, 5}},
        },
        get_shop_menu_detour, get_shop_menu);

    // Add goods and shop entries for every armor piece the player can buy
    for (auto [protector_id, protector_row] :
         ParamUtils::get_param<EquipParamProtector>(L"EquipParamProtector"))
    {
        // Skip invalid/cut items
        auto protector_name = TransmogMessages::get_protector_name(msg_repository, protector_id);
        if (protector_name.empty() || protector_name.starts_with(u"[ERROR]"))
        {
            continue;
        }

        auto goods_id = get_transmog_goods_id_for_protector(protector_id);
        transmog_goods[goods_id] = {
            .refId_default = -1,
            .sfxVariationId = -1,
            .weight = 1,
            .replaceItemId = -1,
            .appearanceReplaceItemId = -1,
            .yesNoDialogMessageId = -1,
            .potGroupId = -1,
            .iconId =
                is_invisible_protector_id(protector_id) ? invisible_icon_id : protector_row.iconIdM,
            .compTrophySedId = -1,
            .trophySeqId = -1,
            .maxNum = 1,
            .goodsType = 1,
            .refId_1 = -1,
            .refVirtualWepId = -1,
            .castSfxId = -1,
            .fireSfxId = -1,
            .effectSfxId = -1,
            .showLogCondType = 1,
            .showDialogCondType = 2,
            .sortGroupId = 255,
            .isUseNoAttackRegion = 1,
            .aiUseJudgeId = -1,
            .reinforceGoodsId = -1,
            .reinforceMaterialId = -1,
        };

        int32_t shop_lineup_param_id = -1;
        switch (protector_row.protectorCategory)
        {
        case protector_category_head:
            shop_lineup_param_id = transmog_head_shop_menu_id + protector_id / 100;
            break;
        case protector_category_body:
            shop_lineup_param_id = transmog_body_shop_menu_id + protector_id / 100;
            break;
        case protector_category_arms:
            shop_lineup_param_id = transmog_arms_shop_menu_id + protector_id / 100;
            break;
        case protector_category_legs:
            shop_lineup_param_id = transmog_legs_shop_menu_id + protector_id / 100;
            break;
        }

        transmog_shop_lineups[shop_lineup_param_id] = {
            .equipId = static_cast<int32_t>(goods_id),
            .value = -1,
            .mtrlId = -1,
            .sellQuantity = -1,
            .equipType = 3,
            .setNum = 1,
            .iconId = -1,
            .nameMsgId = -1,
        };
    }

    // Hook get_equip_param_goods() to return the above items
    ModUtils::hook(
        {
            .aob = "41 8d 50 03"        // lea edx, [r8 + 3]
                   "e8 ?? ?? ?? ??"     // call SoloParamRepositoryImp::GetParamResCap
                   "48 85 c0"           // test rax rax
                   "0f 84 ?? ?? ?? ??", // jz end_lbl
            .offset = -106,
        },
        get_equip_param_goods_detour, get_equip_param_goods);

    // Hook get_shop_lineup_param() to return the above shop entries
    ModUtils::hook(
        {
            .aob = "48 8d 15 ?? ?? ?? ??" // lea rdx, [shop_lineup_param_indexes]
                   "45 33 c0"             // xor r8d, r8d
                   "?? ?? ??"             // ???
                   "e8 ?? ?? ?? ??"       // call SoloParamRepositoryImp::GetParamResCap
                   "48 85 c0"             // test rax, rax
                   "74 ??",               // jz end_lbl
            .offset = -129,
        },
        get_shop_lineup_param_detour, get_shop_lineup_param);
}
