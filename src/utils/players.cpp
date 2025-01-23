#pragma once

#include "players.hpp"

#include <elden-x/chr/world_chr_man.hpp>
#include <elden-x/utils/modutils.hpp>

typedef int GetInventoryIdFn(er::CS::EquipInventoryData *, int *item_id);
static GetInventoryIdFn *get_inventory_id = nullptr;

players::ApplySpEffectFn *players::apply_speffect = nullptr;
players::ClearSpEffectFn *players::clear_speffect = nullptr;
players::SpawnOneShotVFXOnChrFn *players::spawn_one_shot_sfx_on_chr = nullptr;

void players::initialize()
{
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
        throw std::runtime_error("Failed to find GetInventoryId");
    }

    // Locate both ChrIns::ApplyEffect() and ChrIns::ClearSpEffect() from this snippet that manages
    // speffect 4270 (Disable Grace Warp)
    auto disable_enable_grace_warp_address = modutils::scan<unsigned char>({
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
               "e8 ?? ?? ?? ??", // call ChrIns::ClearSpEffect
    });
    if (!disable_enable_grace_warp_address)
    {
        throw std::runtime_error("Failed to find ChrIns::ApplyEffect and ChrIns::ClearSpEffect");
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
        throw std::runtime_error("Failed to find EMEVD::SpawnOneShotSFXOnChr");
    }
}

bool players::has_item_in_inventory(er::CS::PlayerIns *player, int item_id)
{
    if (player == nullptr)
    {
        return false;
    }

    auto &equip_game_data = player->game_data->equip_game_data;
    return get_inventory_id(&equip_game_data.equip_inventory_data, &item_id) != -1;
}

bool players::has_speffect(er::CS::PlayerIns *player, int speffect_id)
{
    if (player == nullptr)
    {
        return false;
    }

    for (auto entry = player->special_effects->head; entry != nullptr; entry = entry->next)
    {
        if (entry->param_id == speffect_id)
        {
            return true;
        }
    }

    return false;
}
