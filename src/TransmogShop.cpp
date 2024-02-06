#include <cstdint>
#include <iomanip>
#include <iostream>
#include <tga/param_containers.h>
#include <tga/paramdefs.h>

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
#pragma pack(pop)

typedef FindShopMenuResult *FindShopMenuFn(FindShopMenuResult *result, std::byte shop_type,
                                           std::int32_t begin_id, std::int32_t end_id);
typedef void FindShopLineupParamFn(FindShopLineupParamResult *result, std::byte shop_type,
                                   std::int32_t id);

static FindShopMenuFn *get_shop_menu_hook;
static FindShopMenuFn *get_shop_menu;
static FindShopLineupParamFn *get_shop_lineup_param;
static FindShopLineupParamFn *get_shop_lineup_param_hook;

static ShopLineupParam transmog_head_shop_menu = {0};
static ShopLineupParam transmog_body_shop_menu = {0};
static ShopLineupParam transmog_arms_shop_menu = {0};
static ShopLineupParam transmog_legs_shop_menu = {0};

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

// TODO seems to work for goods only?
ShopLineupParam testslp1 = {
    .equipId = 1700,
    .value = 40,
    .mtrlId = -1,
    .eventFlag_forStock = 150070,
    .eventFlag_forRelease = 0,
    .sellQuantity = -1,
    .equipType = 3,
    .costType = 0,
    .setNum = 1,
    .value_Add = 0,
    .value_Magnification = 1,
    .iconId = -1,
    .nameMsgId = -1,
    .menuTitleMsgId = -1,
    .menuIconId = -1,
};

ShopLineupParam testslp2 = {
    .equipId = 140100,
    .value = 0,
    .mtrlId = -1,
    .sellQuantity = -1,
    .equipType = 1,
    .setNum = 1,
    .value_Add = 0,
    .value_Magnification = 1,
    .iconId = -1,
    .nameMsgId = -1,
};

ShopLineupParam testslp3 = {
    .equipId = 910200,
    .value = 0,
    .mtrlId = -1,
    .sellQuantity = -1,
    .equipType = 1,
    .setNum = 1,
    .value_Add = 0,
    .value_Magnification = 1,
    .iconId = -1,
    .nameMsgId = -1,
};

ShopLineupParam testslp4 = {
    .equipId = 320300,
    .value = 0,
    .mtrlId = -1,
    .sellQuantity = -1,
    .equipType = 1,
    .setNum = 1,
    .value_Add = 0,
    .value_Magnification = 1,
    .iconId = -1,
    .nameMsgId = -1,
};

void get_shop_lineup_param_detour(FindShopLineupParamResult *result, std::byte shop_type,
                                  std::int32_t id)
{
    if (id == transmog_head_shop_menu_id)
    {
        result->shop_type = shop_type;
        result->id = id;
        result->row = &testslp1;
        return;
    }
    // if (id >= transmog_head_shop_menu_id)
    // {
    //     if (id < transmog_head_shop_menu_id + transmog_shop_max_size)
    //     {
    //         auto index = id - transmog_head_shop_menu_id;
    //         if (index == 0)
    //         {
    //             std::cout << "result" << std::endl;
    //             result->shop_type = shop_type;
    //             result->id = id;
    //             result->row = &testslp1;
    //             return;
    //         }
    //     }
    //     else if (id < transmog_body_shop_menu_id + transmog_shop_max_size)
    //     {
    //         auto index = id - transmog_body_shop_menu_id;
    //         if (index == 0)
    //         {
    //             result->shop_type = shop_type;
    //             result->id = id;
    //             result->row = &testslp1;
    //             return;
    //         }
    //     }
    //     else if (id < transmog_arms_shop_menu_id + transmog_shop_max_size)
    //     {
    //         auto index = id - transmog_arms_shop_menu_id;
    //         if (index == 0)
    //         {
    //             result->shop_type = shop_type;
    //             result->id = id;
    //             result->row = &testslp1;
    //             return;
    //         }
    //     }
    //     else if (id < transmog_legs_shop_menu_id + transmog_shop_max_size)
    //     {
    //         auto index = id - transmog_legs_shop_menu_id;
    //         if (index == 0)
    //         {
    //             result->shop_type = shop_type;
    //             result->id = id;
    //             result->row = &testslp1;
    //             return;
    //         }
    //     }
    // }

    get_shop_lineup_param(result, shop_type, id);
}

void TransmogShop::initialize(ParamMap &params)
{
    auto equip_param_protector = params[L"EquipParamProtector"];
    auto reinforce_param_protector = params[L"ReinforceParamProtector"];
    auto speffect_param = params[L"SpEffectParam"];

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

    // Add shop entries for every armor piece the player can buy
    // TODO: these should map to goods instead of the original armor sets.

    get_shop_lineup_param_hook = TransmogUtils::hook(
        {.offset = 0xd156c0}, get_shop_lineup_param_detour, get_shop_lineup_param);
}

void TransmogShop::deinitialize()
{
    TransmogUtils::unhook(get_shop_menu_hook);
    TransmogUtils::unhook(get_shop_lineup_param_hook);
}
