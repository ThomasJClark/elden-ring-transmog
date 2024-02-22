#include <iostream>

#include "ModUtils.hpp"
#include "TransmogEvents.hpp"
#include "TransmogShop.hpp"
#include "TransmogVFX.hpp"
#include "internal/WorldChrMan.hpp"

using namespace TransmogEvents;
using namespace std;

static CS::WorldChrManImp **world_chr_man_addr = nullptr;

typedef int32_t RemoveItemFn(CS::EquipInventoryData *, uint32_t, uint32_t);
typedef int32_t ApplySpEffectFn(CS::PlayerIns *, uint32_t, bool unk);
typedef int32_t ClearSpEffectFn(CS::PlayerIns *, uint32_t);

static RemoveItemFn *remove_item = nullptr;
static ApplySpEffectFn *apply_speffect = nullptr;
static ClearSpEffectFn *clear_speffect = nullptr;

/**
 * When the player buys a transmog good, look up the corresponding armor piece and update
 * the transmog VFX to show it.
 *
 * TODO:
 *   - remove existing items of the same category
 *   - call this function on load with the initial inventory
 */
void try_apply_transmog_item(int32_t item_id)
{
    auto world_chr_man = *world_chr_man_addr;
    if (world_chr_man == nullptr || world_chr_man->main_player == nullptr)
    {
        return;
    }

    if (item_id < item_type_goods_begin || item_id >= item_type_goods_end)
    {
        return;
    }

    auto transmog_protector_id =
        TransmogShop::get_protector_id_for_transmog_good(item_id - item_type_goods_begin);
    if (transmog_protector_id == -1)
    {
        return;
    }

    // Update the transmog VFX to show this protector
    TransmogVFX::set_transmog_protector(transmog_protector_id);

    // Ensure the main player has the transmog speffect
    apply_speffect(world_chr_man->main_player, TransmogVFX::transmog_speffect_id, false);
}

bool (*add_inventory_from_shop)(int32_t *, int32_t) = nullptr;
bool (*add_inventory_from_shop_hook)(int32_t *, int32_t) = nullptr;

bool add_inventory_from_shop_detour(int32_t *item_id, int32_t quantity)
{
    try_apply_transmog_item(*item_id);
    return add_inventory_from_shop(item_id, quantity);
}

void TransmogEvents::initialize(CS::ParamMap &params)
{
    world_chr_man_addr = ModUtils::scan<CS::WorldChrManImp *>({
        .aob = "48 8b 05 ?? ?? ?? ??"  // mov rax, [WorldChrMan]
               "48 85 c0"              // test rax, rax
               "74 0f"                 // jz end_label
               "48 39 88 08 e5 01 00", // cmp [rax + 0x1e508], rcx
        .relative_offsets = {{3, 7}},
    });

    remove_item = ModUtils::scan<RemoveItemFn>({
        .aob = "?? 83 ec ?? 8b f2 ?? 8b e9 ?? 85 c0 74",
        .offset = -12,
    });

    // Locate both ChrIns::ApplyEffect() and ChrIns::ClearSpEffect() from this snippet that manages
    // speffect 4270 (Disable Grace Warp)
    string enable_disable_grace_warp_aob = "45 33 c0"        // xor r8d, r8d
                                           "ba ae 10 00 00"  // mov edx, 4270 ; Disable Grace Warp
                                           "48 8b cf"        // mov rcx, rdi
                                           "e8 ?? ?? ?? ??"  // call ChrIns::ApplyEffect
                                           "eb 16"           // jmp end_label
                                           "e8 ?? ?? ?? ??"  // call HasSpEffect
                                           "84 c0"           // test al, al
                                           "74 0d"           // jz end_label
                                           "ba ae 10 00 00"  // mov edx, 4270 ; Disable Grace Warp
                                           "48 8b cf"        // mov rcx, rdi
                                           "e8 ?? ?? ?? ??"; // call ChrIns::ClearSpEffect

    ptrdiff_t disable_enable_grace_warp_offset =
        ModUtils::scan<byte>({.aob = enable_disable_grace_warp_aob}) -
        ModUtils::scan<byte>({.offset = 0});

    apply_speffect = ModUtils::scan<ApplySpEffectFn>({
        .offset = disable_enable_grace_warp_offset + 11,
        .relative_offsets = {{1, 5}},
    });

    clear_speffect = ModUtils::scan<ClearSpEffectFn>({
        .offset = disable_enable_grace_warp_offset + 35,
        .relative_offsets = {{1, 5}},
    });

    // TODO: AOB. either eldenring.exe+73690 or eldenring.exe+73840 could work here
    add_inventory_from_shop_hook = ModUtils::hook(
        {.offset = 0x773840}, add_inventory_from_shop_detour, add_inventory_from_shop);

    // Hook idea for load in - something initialized in common event?
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
    ModUtils::unhook(add_inventory_from_shop_hook);
}
