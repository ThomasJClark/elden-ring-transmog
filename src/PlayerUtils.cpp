#pragma once

#include "PlayerUtils.hpp"
#include "ModUtils.hpp"

using namespace std;
using namespace PlayerUtils;

static CS::WorldChrManImp **world_chr_man_addr;

typedef int GetInventoryIdFn(CS::EquipInventoryData *, int32_t *item_id);
static GetInventoryIdFn *get_inventory_id = nullptr;

void PlayerUtils::initialize()
{
    world_chr_man_addr = ModUtils::scan<CS::WorldChrManImp *>({
        .aob = "48 8b 05 ?? ?? ?? ??"  // mov rax, [WorldChrMan]
               "48 85 c0"              // test rax, rax
               "74 0f"                 // jz end_label
               "48 39 88 08 e5 01 00", // cmp [rax + 0x1e508], rcx
        .relative_offsets = {{3, 7}},
    });

    get_inventory_id = ModUtils::scan<GetInventoryIdFn>({
        .aob = "48 8d 8f 58 01 00 00" // lea rcx, [rdi + 0x158] ;
                                      // &equipGameData->equipInventoryData
               "e8 ?? ?? ?? ??"       // call GetInventoryId
               "8b d8"                // mov ebx, eax
               "85 c0"                // test eax, eax
               "78 6a",               // js label
        .offset = 7,
        .relative_offsets = {{1, 5}},
    });
}

CS::PlayerIns *PlayerUtils::get_main_player()
{
    auto world_chr_man = *world_chr_man_addr;
    if (world_chr_man != nullptr)
    {
        return world_chr_man->main_player;
    }

    return nullptr;
}

bool PlayerUtils::has_item_in_inventory(CS::PlayerIns *player, int32_t item_id)
{
    if (player == nullptr)
    {
        return false;
    }

    auto &equip_game_data = player->player_game_data->equip_game_data;
    return get_inventory_id(&equip_game_data.equip_inventory_data, &item_id) != -1;
}
