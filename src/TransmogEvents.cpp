#include <iostream>
#include <tga/paramdefs.h>

#include "ModUtils.hpp"
#include "ParamUtils.hpp"
#include "TransmogEvents.hpp"
#include "TransmogShop.hpp"
#include "TransmogVFX.hpp"

using namespace TransmogEvents;
using namespace std;

static CS::WorldChrManImp **world_chr_man_addr = nullptr;

typedef int GetInventoryIdFn(CS::EquipInventoryData *, int32_t *item_id);
static GetInventoryIdFn *get_inventory_id = nullptr;

namespace CS
{
struct InGameStep;
}; // namespace CS

static bool prev_loaded = false;

static void (*in_game_stay_step_load_finish)(CS::InGameStep *) = nullptr;

void in_game_stay_step_load_finish_detour(CS::InGameStep *step)
{
    // The main CS::PlayerIns is reinitialized every time you go through a loading screen. When this
    // happens, read the player's inventory to update the transmog VFX, since it might be a
    // different player or the game may have been completely restarted.
    auto world_chr_man = *world_chr_man_addr;
    auto loaded = world_chr_man != nullptr && world_chr_man->main_player != nullptr;
    if (prev_loaded != loaded)
    {
        if (loaded)
        {
            TransmogVFX::reset_transmog();

            auto equip_inventory_data =
                &world_chr_man->main_player->player_game_data->equip_game_data.equip_inventory_data;

            for (auto [protector_id, protector] :
                 ParamUtils::get_param<EquipParamProtector>(L"EquipParamProtector"))
            {
                int32_t transmog_item_id =
                    TransmogShop::item_type_goods_begin +
                    TransmogShop::get_transmog_goods_id_for_protector(protector_id);
                auto inventory_id = get_inventory_id(equip_inventory_data, &transmog_item_id);
                if (inventory_id != -1)
                {
                    TransmogVFX::set_transmog_protector(protector_id);
                }
            }
        }
        else
        {
            TransmogVFX::reset_transmog();
        }
        prev_loaded = loaded;
    }

    in_game_stay_step_load_finish(step);
}

void TransmogEvents::initialize(CS::WorldChrManImp **world_chr_man_addr)
{
    ::world_chr_man_addr = world_chr_man_addr;

    // TODO AOB
    get_inventory_id = ModUtils::scan<GetInventoryIdFn>({
        .offset = 0x24b490,
    });

    // TODO AOB
    ModUtils::hook({.offset = 0xabc830}, in_game_stay_step_load_finish_detour,
                   in_game_stay_step_load_finish);
}