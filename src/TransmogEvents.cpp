#include <iostream>
#include <string>
#include <tga/paramdefs.h>

#include "ModUtils.hpp"
#include "TransmogEvents.hpp"
#include "TransmogShop.hpp"
#include "internal/GameDataMan.hpp"

using namespace TransmogEvents;
using namespace std;

typedef int32_t RemoveItemFn(CS::EquipInventoryData *equip_inventory_data, uint32_t index,
                             uint32_t count);

static constexpr uint32_t item_type_goods_begin = 0x40000000;
static constexpr uint32_t item_type_goods_end = 0x50000000;

static CS::GameDataMan **game_data_man_addr = nullptr;
static RemoveItemFn *remove_item = nullptr;

#pragma pack(push, 1)
struct BuyShopItemParamsEntry
{
    byte unk1[0x8];
    int32_t unk2;
    int32_t unk3;
    byte unk4[0x8];
};

struct BuyShopItemParams
{
    byte unk1[0xc];
    ShopLineupParam *shop_lineup_param;
    byte unk2[0x4];
    BuyShopItemParamsEntry **begin;
    BuyShopItemParamsEntry **end;
};
#pragma pack(pop)

// This is called when you buy something!
// it has a 0/1/2/3/4 switch (equipType?)
// buy_shop_lineup
// FUN_140775700(000000013C99E718, true, 000000000014EE68, 00007FF39DA9C094)
void (*FUN_140775700)(void *param_1, bool param_2, void *param_3, void *param_4);
void FUN_140775700_detour(void *param_1, bool param_2, void *param_3, void *param_4)
{
    cout << "FUN_140775700(" << param_1 << ", " << param_2 << ", " << param_3 << ", " << param_4
         << ")" << endl;
    FUN_140775700(param_1, param_2, param_3, param_4);
}

// This is also called when you buy something (by the above function)
void *(*FUN_140775890)(void *param_1, int32_t param_2);
void *FUN_140775890_detour(void *param_1, int32_t param_2)
{
    auto result = FUN_140775890(param_1, param_2);
    cout << "FUN_140775890(" << param_1 << ", " << param_2 << ") -> " << result << endl;
    return result;
}

void TransmogEvents::initialize()
{
    game_data_man_addr = ModUtils::scan<CS::GameDataMan *>({
        .aob = "48 8B 05 ?? ?? ?? ?? 48 85 C0 74 05 48 8B 40 58 C3 C3",
        .relative_offsets = {{0x3, 0x7}},
    });

    remove_item = ModUtils::scan<RemoveItemFn>({
        .aob = "?? 83 ec ?? 8b f2 ?? 8b e9 ?? 85 c0 74",
        .offset = -0xc,
    });

    ModUtils::hook({.offset = 0x775700}, FUN_140775700_detour, FUN_140775700);

    auto game_data_man = *game_data_man_addr;
    if (game_data_man == nullptr || game_data_man->player_game_data == nullptr)
    {
        return;
    }

    auto &inventory_data = game_data_man->player_game_data->equip_game_data.equip_inventory_data;
    for (auto &entry : inventory_data.inventory)
    {
        if (entry.item_id >= item_type_goods_begin && entry.item_id < item_type_goods_end)
        {
            auto goods_id = entry.item_id - item_type_goods_begin;
            cout << "- goods id: " << goods_id << ", quantity: " << entry.quantity << endl;

            auto protector_id = TransmogShop::get_protector_id_for_transmog_good(goods_id);
            cout << "  protector: " << protector_id << endl;
        }
    }
    cout << endl;
}

void TransmogEvents::deinitialize()
{
}
