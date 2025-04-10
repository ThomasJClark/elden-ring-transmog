#include "ertransmogrify_local_player.hpp"
#include "ertransmogrify_shop.hpp"
#include "utils/players.hpp"

#include <spdlog/spdlog.h>

#include <elden-x/params/param_table.hpp>

static constexpr int transmog_sfx_id = 8020;

ertransmogrify::vfx::player_state_st ertransmogrify::local_player::get_local_player_state(
    er::CS::PlayerIns *player) {
    if (!player) {
        return ertransmogrify::vfx::player_state_st{};
    }

    auto &gear_param_ids = player->game_data->equip_game_data.chr_asm.gear_param_ids;

    auto state = ertransmogrify::vfx::player_state_st{
        .head_protector_id = gear_param_ids.head_protector_id,
        .chest_protector_id = gear_param_ids.chest_protector_id,
        .arms_protector_id = gear_param_ids.arms_protector_id,
        .legs_protector_id = gear_param_ids.legs_protector_id};

    // When the local player is given this speffect, remove any transmogs
    if (players::has_speffect(player, ertransmogrify::vfx::undo_transmog_speffect_id)) {
        players::clear_speffect(player, ertransmogrify::vfx::undo_transmog_speffect_id);
        ertransmogrify::shop::remove_transmog_goods();

        // Play a cool effect when a player dispels transmog
        players::spawn_one_shot_sfx_on_chr(player, 905, transmog_sfx_id, nullptr);
    }
    // Otherwise, check which dummy transmog items they have in their inventory
    else {
        for (auto [protector_id, protector] : er::param::EquipParamProtector) {
            int transmog_item_id =
                ertransmogrify::shop::item_type_goods_begin +
                ertransmogrify::shop::get_transmog_goods_id_for_protector(protector_id);

            if (players::has_item_in_inventory(player, transmog_item_id)) {
                switch (protector.protectorCategory) {
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
    }

    return state;
}
