#pragma once
#pragma pack(1)

#include <cstddef>
#include <cstdint>

extern "C" struct InventoryEntry
{
    std::byte unknown1[0x4];
    std::int32_t item_id;
    std::int32_t quantity;
    std::byte unknown2[0x8];
};

extern "C" struct EquipInventoryData
{
    std::byte unknown1[0x10];
    InventoryEntry *inventory_list;
    std::uint32_t inventory_count;
};

extern "C" struct PlayerGameData
{
    std::byte unknown1[0x408];
    EquipInventoryData equip_inventory_data;
};

extern "C" struct GameDataMan
{
    std::byte unknown1[0x8];
    PlayerGameData *player_game_data;
};
