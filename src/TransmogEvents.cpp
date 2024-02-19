#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tga/paramdefs.h>

#include "ModUtils.hpp"
#include "TransmogEvents.hpp"
#include "TransmogMessages.hpp"
#include "TransmogShop.hpp"
#include "TransmogVFX.hpp"
#include "internal/GameDataMan.hpp"

using namespace TransmogEvents;
using namespace std;

typedef int32_t RemoveItemFn(CS::EquipInventoryData *equip_inventory_data, uint32_t index,
                             uint32_t count);

static constexpr uint32_t item_type_goods_begin = 0x40000000;
static constexpr uint32_t item_type_goods_end = 0x50000000;

static CS::GameDataMan *game_data_man = nullptr;
static RemoveItemFn *remove_item = nullptr;
static std::map<uint64_t, void *> *equip_param_protector;

#pragma pack(push, 1)

#pragma pack(pop)

string bytes_to_str(const byte *bytes, size_t count)
{
    if (count == 0)
    {
        return "";
    };

    stringstream s;
    int i = 0;
    while (true)
    {
        s << setfill('0') << setw(2) << hex << right << (int)bytes[i++];
        if (i == count)
            return s.str();
        else if (i % 32 == 0)
            s << endl;
        else if (i % 8 == 0)
            s << "  ";
        else
            s << " ";
    }
}

// much more accurate inventory:
// https://github.com/thefifthmatt/runtimesouls/blob/42801e5873da809b899d6bf902482fcb6adfd536/inventory.h#L77

// FUN_140245a30
int (*add_inventory_equip)(CS::EquipGameData *equip_game_data, uint32_t *ga_item_id,
                           int32_t purchased_quantity, bool param_4, bool param_5);
int add_inventory_equip_detour(CS::EquipGameData *equip_game_data, uint32_t *ga_item_id,
                               int32_t purchased_quantity, bool param_4, bool param_5)
{
    auto inventory_id =
        add_inventory_equip(equip_game_data, ga_item_id, purchased_quantity, param_4, param_5);

    if (*ga_item_id < 0xb0000000 || *ga_item_id >= 0xc0000000)
    {
        return inventory_id;
    }

    auto goods_id = *ga_item_id - 0xb0000000;
    auto protector_id = TransmogShop::get_protector_id_for_transmog_good(goods_id);
    if (protector_id == -1)
    {
        return inventory_id;
    }

    auto &inventory = equip_game_data->equip_inventory_data;

    cout << "inventory.count_info = " << inventory.count_info << endl;
    cout << "inventory.entries = " << inventory.entries << endl;
    cout << "inventory.length = " << inventory.length << endl;
    cout << "inventory.start_id = " << inventory.start_id << endl;

    cout << "index = " << (inventory_id - inventory.start_id) << endl;
    cout << "ga_item_id = 0x" << hex << *ga_item_id << dec << endl;
    cout << "param_4 = " << (param_4 ? "true" : "false") << endl;
    cout << "param_5 = " << (param_5 ? "true" : "false") << endl;
    cout << "inventory_id = " << inventory_id << endl;

    auto protector =
        reinterpret_cast<EquipParamProtector *>(equip_param_protector->at(protector_id));
    if (protector == nullptr)
    {
        cout << "protector = " << protector_id << " <null>" << endl;
    }
    else
    {
        cout << "protector = " << protector_id << " " << protector->equipModelId << endl;
        TransmogVFX::set_transmog(protector);
    }

    cout << endl;
    cout << bytes_to_str((byte *)&inventory, sizeof(inventory)) << endl;
    cout << endl;

    return inventory_id;
}

int FUN_140245db0_detour(CS::EquipGameData *equip_game_data, uint64_t item_id, int32_t quantity,
                         bool param_4, bool param_5)
{
}

bool FUN_140773690_detour(int32_t *item_id, int32_t quantity)
{
}

bool FUN_140773840(int32_t *item_id, int param_2)
{
}

void FUN_140789360(void *param_1, void *param_2)
{
}

void TransmogEvents::initialize(CS::ParamMap &params)
{
    equip_param_protector = &params[L"EquipParamProtector"];

    game_data_man = *ModUtils::scan<CS::GameDataMan *>({
        .aob = "48 8B 05 ?? ?? ?? ?? 48 85 C0 74 05 48 8B 40 58 C3 C3",
        .relative_offsets = {{0x3, 0x7}},
    });

    remove_item = ModUtils::scan<RemoveItemFn>({
        .aob = "?? 83 ec ?? 8b f2 ?? 8b e9 ?? 85 c0 74",
        .offset = -0xc,
    });

    cout << "GameDataMan " << game_data_man << endl;
    cout << "PlayerGameData " << game_data_man->player_game_data << endl;
    cout << "EquipGameData " << &game_data_man->player_game_data->equip_game_data << endl;
    cout << "EquipInventoryData "
         << &game_data_man->player_game_data->equip_game_data.equip_inventory_data << endl;

    // Try hooking methods in CS::EquipInventoryData::vftable?

    // Hook CS::EquipGameData::AddInventoryEquip()
    // TODO AOBs
    ModUtils::hook({.offset = 0x245a30}, add_inventory_equip_detour, add_inventory_equip);

    // auto &inventory_data = game_data_man->player_game_data->equip_game_data.equip_inventory_data;
    // for (auto &entry : inventory_data.inventory)
    // {
    //     if (entry.item_id >= item_type_goods_begin && entry.item_id < item_type_goods_end)
    //     {
    //         auto goods_id = entry.item_id - item_type_goods_begin;
    //         cout << "- goods id: " << goods_id << ", quantity: " << entry.quantity << endl;

    //         auto protector_id = TransmogShop::get_protector_id_for_transmog_good(goods_id);
    //         cout << "  protector: " << protector_id << endl;
    //     }
    // }
    // cout << endl;
}

void TransmogEvents::log()
{
    // auto &inventory_data = game_data_man->player_game_data->equip_game_data.equip_inventory_data;
    // for (auto &entry : inventory_data.inventory)
    // {
    //     if (entry.item_id >= item_type_goods_begin && entry.item_id < item_type_goods_end)
    //     {
    //         int goods_id = entry.item_id - item_type_goods_begin;
    //         cout << &entry.quantity << " " << goods_id << " " << entry.quantity << endl;
    //     }
    // }
    // cout << endl;
}

void TransmogEvents::deinitialize()
{
}
