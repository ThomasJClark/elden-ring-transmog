#include <iostream>

#include "ModUtils.hpp"
#include "TransmogEvents.hpp"

using namespace TransmogEvents;
using namespace std;

#pragma pack(push, 1)
struct PlayerInventoryEntry
{
    uint32_t ga_item_id;
    uint32_t item_id;
    uint32_t quantity;
    int32_t display_id;
    int32_t unk; // -1
};

struct EquipInventoryData
{
    byte unk1[0x10];
    PlayerInventoryEntry *inventory;
    int32_t inventory_count;
    byte unk2[0xf4];
};

struct EquipGameData
{
    byte unk1[0x158];
    EquipInventoryData equip_inventory_data;
    byte unk2[0x228];
};
struct PlayerGameData
{
    byte unk1[0x2B0];
    EquipGameData equip_game_data;
    byte unk2[0x360];
};

struct GameDataMan
{
    byte unk[0x8];
    PlayerGameData *player_game_data;
};
#pragma pack(pop)

GameDataMan **game_data_man_addr = nullptr;

void TransmogEvents::initialize()
{
    if (game_data_man_addr == nullptr)
    {
        game_data_man_addr = ModUtils::scan<GameDataMan *>({
            .aob = "48 8B 05 ?? ?? ?? ?? 48 85 C0 74 05 48 8B 40 58 C3 C3",
            .relative_offsets = {{0x3, 0x7}},
        });
    }

    auto game_data_man = *game_data_man_addr;
    if (game_data_man == nullptr || game_data_man->player_game_data == nullptr)
    {
        return;
    }

    auto &inventory_data = game_data_man->player_game_data->equip_game_data.equip_inventory_data;

    // local item = getAddress(inventoryList + i * 0x14)
    // local itemId = readInteger(item + 4)
    // local itemType = get_type(itemId)
    // itemId = itemId - itemType[1]
    // local quantity = readInteger(item + 8)
    // if itemId &lt;= 0x5FFFFFFF and itemId ~= nil and quantity ~= nil then
    //     print(string.format("#%d | Type: %s | ID: %d | Quantity: %d", i, itemType[2], itemId,
    //     quantity))
    // end

    if (inventory_data.inventory_count == 0)
    {
        cout << "inventory: []" << endl;
    }
    else
    {
        cout << "inventory:" << endl;
        for (int i = 0; i < inventory_data.inventory_count; i++)
        {
            auto &entry = inventory_data.inventory[i];
            cout << " - ga_item_id: " << hex << entry.ga_item_id << dec << endl;
            cout << "   item_id: " << hex << entry.item_id << dec << endl;
            cout << "   quantity: " << hex << entry.quantity << dec << endl;
            cout << "   display_id: " << hex << entry.display_id << dec << endl;
        }
    }
}

void TransmogEvents::deinitialize()
{
}
// 7ff705354d20
// 3a80061