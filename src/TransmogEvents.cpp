#include <tga/paramdefs.h>

#include "ModUtils.hpp"
#include "TransmogEvents.hpp"
#include "TransmogShop.hpp"
#include "TransmogVFX.hpp"

using namespace TransmogEvents;
using namespace std;

typedef void AddRemoveItemFn(uint64_t item_type, uint32_t item_id, int32_t quantity);

static AddRemoveItemFn *add_remove_item = nullptr;
static CS::WorldChrManImp **world_chr_man_addr = nullptr;
static map<uint64_t, EquipParamProtector *> *equip_param_protector;

/**
 * If the given item ID in the players inventory is a transmog good, look up the corresponding
 * armor piece and update the transmog VFX to show it.
 */
static EquipParamProtector *try_apply_transmog_item(int32_t item_id)
{
    if (item_id >= item_type_goods_begin && item_id < item_type_goods_end)
    {
        auto transmog_goods_id = item_id - item_type_goods_begin;
        auto transmog_protector_id =
            TransmogShop::get_protector_id_for_transmog_good(transmog_goods_id);
        if (transmog_protector_id != -1)
        {
            return TransmogVFX::set_transmog_protector(transmog_protector_id);
        }
    }

    return nullptr;
}

typedef bool AddInventoryFromShopFn(int32_t *new_item_id, int32_t quantity);

AddInventoryFromShopFn *add_inventory_from_shop = nullptr;
AddInventoryFromShopFn *add_inventory_from_shop_hook = nullptr;

/**
 * Hook the function called when buying and item from a shop to apply transmogs when they're
 * chosen
 */
static bool add_inventory_from_shop_detour(int32_t *new_item_id, int32_t quantity)
{
    auto result = add_inventory_from_shop(new_item_id, quantity);

    // If this is a transmog item, update the VFX to apply it
    auto new_protector = try_apply_transmog_item(*new_item_id);
    if (new_protector != nullptr)
    {
        // Remove any other items of the same category in the player's inventory, so there's only
        // one item for each slot
        for (auto [other_protector_id, other_protector] : *equip_param_protector)
        {
            if (other_protector != new_protector &&
                other_protector->protectorCategory == new_protector->protectorCategory)
            {
                auto other_goods_id =
                    TransmogShop::get_transmog_goods_id_for_protector(other_protector_id);
                add_remove_item(item_type_goods_begin, other_goods_id, -1);
            }
        }
    }

    return result;
}

void TransmogEvents::initialize(CS::ParamMap &params, CS::WorldChrManImp **world_chr_man_addr)
{
    equip_param_protector =
        reinterpret_cast<map<uint64_t, EquipParamProtector *> *>(&params[L"EquipParamProtector"]);

    ::world_chr_man_addr = world_chr_man_addr;

    add_remove_item = ModUtils::scan<AddRemoveItemFn>({
        .aob = "8b 99 90 01 00 00" // mov ebx, [rcx + 0x190] ; param->hostModeCostItemId
               "41 83 c8 ff"       // or r8d, -1
               "8b d3"             // mov edx, ebx
               "b9 00 00 00 40"    // mov ecx, item_type_goods_begin
               "e8 ?? ?? ?? ??",   // call AddRemoveItem
        .offset = 17,
        .relative_offsets = {{1, 5}},
    });

    add_inventory_from_shop_hook = ModUtils::hook(
        {
            .aob = "8b 93 80 00 00 00" // mov edx, [rbx + 0x80]
                   "0f af d1"          // imul edx, ecx
                   "48 8b c8"          // mov rcx, rax
                   "e8 ?? ?? ?? ??",   // call AddInventoryFromShop
            .offset = 12,
            .relative_offsets = {{1, 5}},
        },
        add_inventory_from_shop_detour, add_inventory_from_shop);

    // Hook idea for load in - something initialized in common event?
    // Alternatively, WorldChrMan changes load & teleport. Check what writes to that address.
}

void TransmogEvents::deinitialize()
{
    ModUtils::unhook(add_inventory_from_shop_hook);
}
