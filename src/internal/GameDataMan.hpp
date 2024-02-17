#pragma once

#include <cstddef>
#include <cstdint>

#include "List.hpp"

#pragma pack(push, 1)

namespace CS
{
struct PlayerInventoryEntry
{
    std::uint32_t ga_item_id;
    std::uint32_t item_id;
    std::uint32_t quantity;
    std::int32_t display_id;
    std::int32_t unk;
};

struct EquipInventoryData
{
    std::byte unk1[0x10];
    List<PlayerInventoryEntry> inventory;
    std::byte unk2[0xf4];
};

struct EquipGameData
{
    std::byte unk1[0x158];
    EquipInventoryData equip_inventory_data;
    std::byte unk2[0x228];
};
struct PlayerGameData
{
    std::byte unk1[0x2B0];
    EquipGameData equip_game_data;
    std::byte unk2[0x360];
};

struct GameDataMan
{
    std::byte unk[0x8];
    PlayerGameData *player_game_data;
};
} // namespace CS

#pragma pack(pop)
