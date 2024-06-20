#pragma once

#include "players.hpp"
#include "../internal/GameMan.hpp"
#include "modutils.hpp"

using namespace std;

static CS::WorldChrManImp **world_chr_man_addr = nullptr;
static CS::GameMan **game_man_addr = nullptr;

typedef int GetInventoryIdFn(CS::EquipInventoryData *, int32_t *item_id);
static GetInventoryIdFn *get_inventory_id = nullptr;

players::ApplySpEffectFn *players::apply_speffect = nullptr;
players::ClearSpEffectFn *players::clear_speffect = nullptr;
players::SpawnOneShotVFXOnChrFn *players::spawn_one_shot_sfx_on_chr = nullptr;

void players::initialize()
{
    world_chr_man_addr = modutils::scan<CS::WorldChrManImp *>({
        .aob = "48 8b 05 ?? ?? ?? ??"  // mov rax, [WorldChrMan]
               "48 85 c0"              // test rax, rax
               "74 0f"                 // jz end_label
               "48 39 88 08 e5 01 00", // cmp [rax + 0x1e508], rcx
        .relative_offsets = {{3, 7}},
    });
    if (!world_chr_man_addr)
    {
        throw runtime_error("Failed to find WorldChrMan base");
    }

    game_man_addr = modutils::scan<CS::GameMan *>({
        .aob = "48 8B 05 ?? ?? ?? ??" // mov rax, [GameDataMan]
               "80 B8 ?? ?? ?? ?? 0D 0F 94 C0 C3",
        .relative_offsets = {{3, 7}},
    });
    if (!game_man_addr)
    {
        throw runtime_error("Failed to find GameDataMan base");
    }

    get_inventory_id = modutils::scan<GetInventoryIdFn>({
        .aob = "48 8d 8f 58 01 00 00" // lea rcx, [rdi + 0x158] ;
                                      // &equipGameData->equipInventoryData
               "e8 ?? ?? ?? ??"       // call GetInventoryId
               "8b d8"                // mov ebx, eax
               "85 c0"                // test eax, eax
               "78 6a",               // js label
        .offset = 7,
        .relative_offsets = {{1, 5}},
    });
    if (!get_inventory_id)
    {
        throw runtime_error("Failed to find GetInventoryId");
    }

    // Locate both ChrIns::ApplyEffect() and ChrIns::ClearSpEffect() from this snippet that manages
    // speffect 4270 (Disable Grace Warp)
    auto disable_enable_grace_warp_address = modutils::scan<byte>({
        .aob = "45 33 c0"        // xor r8d, r8d
               "ba ae 10 00 00"  // mov edx, 4270 ; Disable Grace Warp
               "48 8b cf"        // mov rcx, rdi
               "e8 ?? ?? ?? ??"  // call ChrIns::ApplyEffect
               "eb 16"           // jmp end_label
               "e8 ?? ?? ?? ??"  // call HasSpEffect
               "84 c0"           // test al, al
               "74 0d"           // jz end_label
               "ba ae 10 00 00"  // mov edx, 4270 ; Disable Grace Warp
               "48 8b cf"        // mov rcx, rdi
               "e8 ?? ?? ?? ??", // call ChrIns::ClearSpEffect});
    });
    if (!disable_enable_grace_warp_address)
    {
        throw runtime_error("Failed to find ChrIns::ApplyEffect and ChrIns::ClearSpEffect");
    }

    apply_speffect = modutils::scan<ApplySpEffectFn>({
        .address = disable_enable_grace_warp_address + 11,
        .relative_offsets = {{1, 5}},
    });

    clear_speffect = modutils::scan<ClearSpEffectFn>({
        .address = disable_enable_grace_warp_address + 35,
        .relative_offsets = {{1, 5}},
    });

    spawn_one_shot_sfx_on_chr = modutils::scan<SpawnOneShotVFXOnChrFn>({
        .aob = "45 8b 46 04"    // mov r8d, [r14 + 0x4]
               "41 8b 16"       // mov edx, [r14]
               "48 8b 0b"       // mov rcx, [rbx]
               "e8 ?? ?? ?? ??" // call EMEVD::SpawnOneShotSFXOnChr
               "48 8d 5b 08"    // lea rbx, [rbx + 0x8]
               "48 ff c7"       // inc rdi
               "48 3b fe"       // cmp rdi, rsi
               "75 e5",         // jnz start
        .offset = 10,
        .relative_offsets = {{1, 5}},
    });
    if (!spawn_one_shot_sfx_on_chr)
    {
        throw runtime_error("Failed to find EMEVD::SpawnOneShotSFXOnChr");
    }
}

CS::PlayerIns *players::get_main_player()
{
    auto world_chr_man = *world_chr_man_addr;
    if (world_chr_man != nullptr)
    {
        return world_chr_man->main_player;
    }

    return nullptr;
}

CS::NetPlayer *players::get_net_players()
{
    auto world_chr_man = *world_chr_man_addr;
    if (world_chr_man != nullptr)
    {
        return world_chr_man->net_players;
    }

    return nullptr;
}

bool players::has_item_in_inventory(CS::PlayerIns *player, int32_t item_id)
{
    if (player == nullptr)
    {
        return false;
    }

    auto &equip_game_data = player->player_game_data->equip_game_data;
    return get_inventory_id(&equip_game_data.equip_inventory_data, &item_id) != -1;
}

bool players::has_speffect(CS::PlayerIns *player, int32_t speffect_id)
{
    if (player == nullptr)
    {
        return false;
    }

    for (auto entry = player->speffects->head; entry != nullptr; entry = entry->next)
    {
        if (entry->id == speffect_id)
        {
            return true;
        }
    }

    return false;
}

byte players::get_ceremony_type()
{
    auto game_man = *game_man_addr;
    if (game_man == nullptr)
    {
        return byte(0);
    }

    return game_man->ceremony_type;
}