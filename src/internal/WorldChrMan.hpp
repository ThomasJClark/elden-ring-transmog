#pragma once

#include <cstddef>
#include <cstdint>
#include <tga/paramdefs.h>

#pragma pack(push, 1)

namespace CS
{
struct EquipMagicData;
struct EquipItemData;
struct ChrIns;

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
    int32_t chest_protector_id;
    int32_t arms_protector_id;
    int32_t legs_protector_id;
    int32_t unk4;
    int32_t accessory_ids[4];
    int32_t unused;
};

struct EquipGameData
{
    std::byte unk1[0x6c];
    ChrAsm chr_asm;
    std::byte unk2[0x18];
    EquipInventoryData equip_inventory_data;
    EquipMagicData *equip_magic_data;
    EquipItemData *equip_item_data;
    std::byte unk3[0x218];
};

struct PlayerGameData
{
    std::byte unk1[0x9c];
    wchar_t *name;
    std::byte unk2[0x20c];
    EquipGameData equip_game_data;
    std::byte unk3[0x360];
};

struct SpEffectEntry
{
    SpEffectParam *param;
    int32_t id;
    std::byte unk1[0x24];
    SpEffectEntry *next;
    std::byte unk2[0x8c];
};

struct SpEffectList
{
    void **vftable;
    SpEffectEntry *head;
};

struct ChrIns
{
    std::byte unk1[0x6C];
    std::byte team_type;
    std::byte unk2[0x10b];
    SpEffectList *speffects;
    std::byte unk3[0x3f0];
};

struct PlayerIns : ChrIns
{
    std::byte unk3[0x10];
    PlayerGameData *player_game_data;
    std::byte unk4[0x1b8];
};

struct NetPlayer
{
    PlayerIns *player;
    std::byte unk[0x8];
};

struct WorldChrManImp
{
    std::byte unk1[0x10EF8];
    NetPlayer *net_players;
    std::byte unk2[0xd608];
    PlayerIns *main_player;
};
}

#pragma pack(pop)
