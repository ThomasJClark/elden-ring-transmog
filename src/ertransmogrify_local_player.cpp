#include "ertransmogrify_local_player.hpp"
#include "ertransmogrify_shop.hpp"
#include "utils/players.hpp"

#include <spdlog/spdlog.h>

#include <elden-x/params.hpp>

#include <utility>

using namespace std;

static constexpr int transmog_sfx_id = 8020;

/**
 * Get the player's displayed armor before transmog is applied
 */
static pair<ertransmogrify::vfx::player_state_st, bool> get_default_player_state(
    er::CS::PlayerIns *player) {
    bool ignore_arms_transmog = false;

    auto &gear_param_ids = player->game_data->equip_game_data.chr_asm.gear_param_ids;

    // If the player isn't using transmog, default to their actual armor.
    auto state = ertransmogrify::vfx::player_state_st{
        .head_protector_id = gear_param_ids.head_protector_id,
        .chest_protector_id = gear_param_ids.chest_protector_id,
        .arms_protector_id = gear_param_ids.arms_protector_id,
        .legs_protector_id = gear_param_ids.legs_protector_id};

    // If any other transform effects are present other than transmog (e.g. the DLC transformation
    // items), override the armor.
    for (auto entry = player->special_effects->head; entry; entry = entry->next) {
        if (entry->param_id >= ertransmogrify::vfx::transmog_vfx_speffect_start_id &&
            entry->param_id < ertransmogrify::vfx::transmog_vfx_speffect_end_id) {
            continue;
        }

        if (entry->param->vfxId != -1) {
            auto &vfx = er::param::SpEffectVfxParam[entry->param->vfxId].first;
            if (vfx.transformProtectorId != -1) {
                if (vfx.isFullBodyTransformProtectorId) {
                    state.chest_protector_id =
                        vfx.transformProtectorId + ertransmogrify::vfx::chest_protector_offset;
                    state.arms_protector_id =
                        vfx.transformProtectorId + ertransmogrify::vfx::arms_protector_offset;
                    state.legs_protector_id =
                        vfx.transformProtectorId + ertransmogrify::vfx::legs_protector_offset;
                } else if (vfx.isArmsTransformProtectorId) {
                    state.arms_protector_id =
                        vfx.transformProtectorId + ertransmogrify::vfx::arms_protector_offset;

                    // Super special case: arm transforms are used for the singular purpose of
                    // hiding armor while beast claws or DLC fists are equipped, and should take
                    // priority over transmog selections.
                    ignore_arms_transmog = true;
                } else {
                    state.head_protector_id =
                        vfx.transformProtectorId + ertransmogrify::vfx::head_protector_offset;
                }
            }
        }
    }

    return {state, ignore_arms_transmog};
}

ertransmogrify::vfx::player_state_st ertransmogrify::local_player::get_local_player_state(
    er::CS::PlayerIns *player) {
    if (!player) {
        return ertransmogrify::vfx::player_state_st{};
    }

    auto [state, ignore_arms_transmog] = get_default_player_state(player);

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
                        if (!ignore_arms_transmog) {
                            state.arms_protector_id = protector_id;
                        }
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
