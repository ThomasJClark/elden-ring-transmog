#include "ertransmogrify_local_player.hpp"
#include "ertransmogrify_shop.hpp"
#include "utils/players.hpp"

#include <spdlog/spdlog.h>

#include <elden-x/params/param_table.hpp>

ertransmogrify::vfx::player_state_st ertransmogrify::local_player::get_local_player_state(
    er::CS::PlayerIns *player)
{
    ertransmogrify::vfx::player_state_st state;

    // When the local player is given this speffect, remove any transmogs
    if (players::has_speffect(player, ertransmogrify::vfx::undo_transmog_speffect_id))
    {
        players::clear_speffect(player, ertransmogrify::vfx::undo_transmog_speffect_id);
        ertransmogrify::shop::remove_transmog_goods();
        return state;
    }

    // Otherwise, check which dummy transmog items they have in their inventory
    for (auto [protector_id, protector] : er::param::EquipParamProtector)
    {
        int transmog_item_id =
            ertransmogrify::shop::item_type_goods_begin +
            ertransmogrify::shop::get_transmog_goods_id_for_protector(protector_id);

        if (players::has_item_in_inventory(player, transmog_item_id))
        {
            switch (protector.protectorCategory)
            {
            case shop::protector_category_head:
                state.head_protector_id = protector_id;
                break;
            case shop::protector_category_chest:
                state.chest_protector_id = protector_id;
                break;
            case shop::protector_category_arms:
                state.arms_protector_id = protector_id;
                break;
            case shop::protector_category_legs:
                state.legs_protector_id = protector_id;
                break;
            }
        }
    }

    // Body/arms/legs have to be combined in one spffect. If transmog is enabled on some but not
    // all of them, default the others to the player's current armor
    auto has_chest_protector = state.chest_protector_id > 0;
    auto has_arms_protector = state.arms_protector_id > 0;
    auto has_legs_protector = state.legs_protector_id > 0;
    if (has_chest_protector || has_arms_protector || has_legs_protector)
    {
        auto &gear_param_ids = player->game_data->equip_game_data.chr_asm.gear_param_ids;
        if (!has_chest_protector)
        {
            state.chest_protector_id = gear_param_ids.chest_protector_id;
            ertransmogrify::shop::add_transmog_good(gear_param_ids.chest_protector_id);
        }
        if (!has_arms_protector)
        {
            state.arms_protector_id = gear_param_ids.arms_protector_id;
            ertransmogrify::shop::add_transmog_good(gear_param_ids.arms_protector_id);
        }
        if (!has_legs_protector)
        {
            state.legs_protector_id = gear_param_ids.legs_protector_id;
            ertransmogrify::shop::add_transmog_good(gear_param_ids.legs_protector_id);
        }
    }

    return state;
}
