#pragma once

#include <cstddef>
#include <cstdint>

#pragma pack(push, 1)

namespace CS
{
struct EquipMagicData;
struct EquipItemData;

struct PlayerInventoryEntry
{
    uint32_t ga_item_id;
    int32_t item_id;
    uint32_t quantity;
    int32_t display_id;
    int32_t unk;
};

struct EquipInventoryData
{
    void **vftable;
    std::byte unk1[0x8];
    PlayerInventoryEntry *entries1;
    uint32_t count_info;
    int32_t start_index;
    std::byte unk2[0x8];
    int32_t todo1;
    std::byte unk3[0x14];
    PlayerInventoryEntry *entries2; // I think this only includes stackable items? idk
    std::byte unk4[0x8];
    PlayerInventoryEntry *key_items; // idk??? why are there three entries lists?
    std::byte unk5[0x28];
    uint32_t length;
    uint32_t todo2;
    std::byte unk6[0x1a];
};

struct ChrAsm
{
    void **vftable;
    std::byte unk1[0x74];
    int32_t left_weapon_id1;
    int32_t right_weapon_id1;
    int32_t left_weapon_id2;
    int32_t right_weapon_id2;
    int32_t left_weapon_id3;
    int32_t right_weapon_id3;
    int32_t arrow_id1;
    int32_t bolt_id1;
    int32_t arrow_id2;
    int32_t bolt_id2;
    int32_t unk2;
    int32_t unk3;
    int32_t head_protector_id;
    int32_t body_protector_id;
    int32_t arms_protector_id;
    int32_t legs_protector_id;
    int32_t unk4;
    int32_t accessory_ids[4];
    std::byte unk5[0x18];
};

struct EquipGameData
{
    std::byte unk1[0x70];
    ChrAsm chr_asm;
    EquipInventoryData equip_inventory_data;
    EquipMagicData *equip_magic_data;
    EquipItemData *equip_item_data;
    std::byte unk6[0x218];
};

struct PlayerGameData
{
    std::byte unk1[0x2B0];
    EquipGameData equip_game_data;
    std::byte unk2[0x360];
};

struct ChrIns
{
    std::byte unk[0x570];
};

struct PlayerIns : ChrIns
{
    std::byte unk1[0x8];
    PlayerGameData *player_game_data;
    std::byte unk2[0x1b8];
};

struct WorldChrManImp
{
    std::byte unk[0x1e508];
    PlayerIns *main_player;
};
} // namespace CS

#pragma pack(pop)
