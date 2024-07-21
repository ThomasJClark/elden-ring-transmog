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
    unsigned int ga_item_id;
    int item_id;
    unsigned int quantity;
    int display_id;
    int unk;
};

struct EquipInventoryData
{
    void **vftable;
    unsigned char unk1[0x8];
    PlayerInventoryEntry *entries1;
    unsigned int count_info;
    int start_index;
    unsigned char unk2[0x8];
    int todo1;
    unsigned char unk3[0x14];
    PlayerInventoryEntry *entries2; // I think this only includes stackable items? idk
    unsigned char unk4[0x8];
    PlayerInventoryEntry *key_items; // idk??? why are there three entries lists?
    unsigned char unk5[0x28];
    unsigned int length;
    unsigned int todo2;
    unsigned char unk6[0x1a];
};

struct ChrAsm
{
    void **vftable;
    unsigned char unk1[0x74];
    int left_weapon_id1;
    int right_weapon_id1;
    int left_weapon_id2;
    int right_weapon_id2;
    int left_weapon_id3;
    int right_weapon_id3;
    int arrow_id1;
    int bolt_id1;
    int arrow_id2;
    int bolt_id2;
    int unk2;
    int unk3;
    int head_protector_id;
    int chest_protector_id;
    int arms_protector_id;
    int legs_protector_id;
    int unk4;
    int accessory_ids[4];
    int unused;
};

struct EquipGameData
{
    unsigned char unk1[0x6c];
    ChrAsm chr_asm;
    unsigned char unk2[0x18];
    EquipInventoryData equip_inventory_data;
    EquipMagicData *equip_magic_data;
    EquipItemData *equip_item_data;
    unsigned char unk3[0x218];
};

struct PlayerGameData
{
    unsigned char unk1[0x9c];
    wchar_t *name;
    unsigned char unk2[0x20c];
    EquipGameData equip_game_data;
    unsigned char unk3[0x360];
};

struct SpEffectEntry
{
    SpEffectParam *param;
    int id;
    unsigned char unk1[0x24];
    SpEffectEntry *next;
    unsigned char unk2[0x8c];
};

struct SpEffectList
{
    void **vftable;
    SpEffectEntry *head;
};

struct ChrIns
{
    unsigned char unk1[0x6C];
    unsigned char team_type;
    unsigned char unk2[0x10b];
    SpEffectList *speffects;
    unsigned char unk3[0x3f0];
};

struct PlayerIns : ChrIns
{
    unsigned char unk3[0x10];
    PlayerGameData *player_game_data;
    unsigned char unk4[0x1b8];
};

struct NetPlayer
{
    PlayerIns *player;
    unsigned char unk[0x8];
};

struct WorldChrManImp
{
    unsigned char unk1[0x10EF8];
    NetPlayer *net_players;
    unsigned char unk2[0xd608];
    PlayerIns *main_player;
};
}

#pragma pack(pop)
