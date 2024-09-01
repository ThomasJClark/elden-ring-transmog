#include <spdlog/spdlog.h>
#include <vector>

#include "ertransmogrify_player_state.hpp"
#include "ertransmogrify_vfx.hpp"
#include "internal/GameMan.hpp"
#include "utils/params.hpp"
#include "utils/players.hpp"

using namespace ertransmogrify;

static constexpr int transmog_sfx_id = 8020;

static inline long long get_transmog_speffect_id_for_protector(long long protector_id)
{
    if (protector_id == -1)
    {
        return -1;
    }

    return vfx::transmog_dummy_speffect_start_id + protector_id / 100;
}

static inline long long get_protector_id_for_transmog_speffect(long long speffect_id)
{
    if (speffect_id >= vfx::transmog_dummy_speffect_start_id &&
        speffect_id < vfx::transmog_dummy_speffect_end_id)
    {
        return 100 * (speffect_id - vfx::transmog_dummy_speffect_start_id);
    }

    return -1;
}

void PlayerState::refresh_transmog()
{
    if (player == nullptr || player->player_game_data == nullptr || player->speffects == nullptr)
    {
        previous_refreshed = false;
        return;
    }

    bool previous_is_transmog_enabled = is_transmog_enabled();

    if (player == players::get_main_player())
    {
        refresh_transmog_main_player();
    }
    else
    {
        refresh_transmog_net_player();
    }

    // Clear stale SpEffects from transmogs that are no longer applied
    auto cleared_speffects = std::vector<int>();
    for (auto entry = player->speffects->head; entry != nullptr; entry = entry->next)
    {
        auto protector_id = get_protector_id_for_transmog_speffect(entry->id);
        if (protector_id != -1)
        {
            if (protector_id != head_protector_id && protector_id != chest_protector_id &&
                protector_id != arms_protector_id && protector_id != legs_protector_id)
            {
                cleared_speffects.push_back(entry->id);
            }
        }
    }

    for (auto speffect_id : cleared_speffects)
    {
        players::clear_speffect(player, speffect_id);
    }

    // Ensure the player has the head/body transmog VFX if they have anything selected in those
    // slots
    if (is_head_transmog_enabled() && head_speffect_id > 0)
    {
        players::apply_speffect(player, head_speffect_id, false);
    }
    else
    {
        players::clear_speffect(player, head_speffect_id);
    }

    if (is_body_transmog_enabled() && body_speffect_id > 0)
    {
        players::apply_speffect(player, body_speffect_id, false);
    }
    else
    {
        players::clear_speffect(player, body_speffect_id);
    }

    // When transmog is enabled or disabled, show a cool effect on the player. Skip this
    // when refreshing for the first time, so there isn't an effect when loading into the game
    // with transmog already enabled.
    if (previous_refreshed && is_transmog_enabled() != previous_is_transmog_enabled)
    {
        players::spawn_one_shot_sfx_on_chr(player, 905, transmog_sfx_id, nullptr);
    };

    previous_player = player;
    previous_refreshed = true;
}

void PlayerState::remove_transmog()
{
    clear_transmog_protectors();
    players::clear_speffect(player, head_speffect_id);
    players::clear_speffect(player, body_speffect_id);
    if (head_protector_id != -1)
    {
        players::clear_speffect(player, get_transmog_speffect_id_for_protector(head_protector_id));
    }
    if (chest_protector_id != -1)
    {
        players::clear_speffect(player, get_transmog_speffect_id_for_protector(chest_protector_id));
    }
    if (arms_protector_id != -1)
    {
        players::clear_speffect(player, get_transmog_speffect_id_for_protector(arms_protector_id));
    }
    if (legs_protector_id != -1)
    {
        players::clear_speffect(player, get_transmog_speffect_id_for_protector(legs_protector_id));
    }
}

/**
 * Refresh transmog for the main player based on goods in their inventory
 */
void PlayerState::refresh_transmog_main_player()
{
    auto equip_param_protector = params::get_param<EquipParamProtector>(L"EquipParamProtector");

    bool any_changed = false;
    bool has_chest_protector = false;
    bool has_arms_protector = false;
    bool has_legs_protector = false;

    // Skip checking the inventory if the player is in a ceremony (i.e. pseudo-multiplayer), because
    // inventory isn't completely copied over, and it's not possible for this to have changed since
    // the last time it was checked.
    auto ceremony_type = players::get_ceremony_type();
    if (ceremony_type == players::ceremony_type_none)
    {
        if (player != previous_player)
        {
            spdlog::info("Loaded main player - clearing previous transmog");
            clear_transmog_protectors();
        }

        if (should_clear_transmog())
        {
            if (is_transmog_enabled())
            {
                spdlog::info("Main player - undoing transmog");
                // If the player was given the undo transmog SpEffect, don't set any protector slots
                // and remove any transmog goods.
                clear_transmog_protectors();
                shop::remove_transmog_goods();
            }
        }
        else
        {
            // Otherwise, set the protector slots based on the transmog goods in the player's
            // inventory
            for (auto [protector_id, protector] : equip_param_protector)
            {
                int transmog_item_id = shop::item_type_goods_begin +
                                       shop::get_transmog_goods_id_for_protector(protector_id);

                if (players::has_item_in_inventory(player, transmog_item_id))
                {
                    if (set_transmog_protector(protector_id, protector))
                    {
                        spdlog::info("Set main player transmog protector {}", protector_id);
                        any_changed = true;
                    }

                    switch (protector.protectorCategory)
                    {
                    case shop::protector_category_chest:
                        has_chest_protector = true;
                        break;
                    case shop::protector_category_arms:
                        has_arms_protector = true;
                        break;
                    case shop::protector_category_legs:
                        has_legs_protector = true;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        spdlog::debug("Ceremony type {} is active, skipping transmog update", (int)ceremony_type);
    }

    // Body/arms/legs have to be combined in one spffect. If transmog is enabled on some but not
    // all of them, default the others to the player's current armor
    if (has_chest_protector || has_arms_protector || has_legs_protector)
    {
        auto &chr_asm = player->player_game_data->equip_game_data.chr_asm;
        if (!has_chest_protector)
        {
            chest_protector_id = chr_asm.chest_protector_id;
            spdlog::info("Defaulting main player chest to protector {}", chest_protector_id);
            chest_protector = &equip_param_protector[chest_protector_id];
            shop::add_transmog_good(chr_asm.chest_protector_id);
        }
        if (!has_arms_protector)
        {
            arms_protector_id = chr_asm.arms_protector_id;
            spdlog::info("Defaulting main player arms to protector {}", arms_protector_id);
            arms_protector = &equip_param_protector[arms_protector_id];
            shop::add_transmog_good(chr_asm.arms_protector_id);
        }
        if (!has_legs_protector)
        {
            legs_protector_id = chr_asm.legs_protector_id;
            spdlog::info("Defaulting main player legs to protector {}", legs_protector_id);
            legs_protector = &equip_param_protector[legs_protector_id];
            shop::add_transmog_good(chr_asm.legs_protector_id);
        }
    }

    if (any_changed)
    {
        toggle_protectors();
    }

    // Apply SpEffects for the selected transmog protectors, to indicate the selections to other
    // networked players
    if (head_protector_id != -1)
    {
        players::apply_speffect(player, get_transmog_speffect_id_for_protector(head_protector_id),
                                false);
    }
    if (chest_protector_id != -1)
    {
        players::apply_speffect(player, get_transmog_speffect_id_for_protector(chest_protector_id),
                                false);
    }
    if (arms_protector_id != -1)
    {
        players::apply_speffect(player, get_transmog_speffect_id_for_protector(arms_protector_id),
                                false);
    }
    if (legs_protector_id != -1)
    {
        players::apply_speffect(player, get_transmog_speffect_id_for_protector(legs_protector_id),
                                false);
    }
}

/**
 * Refresh transmog for a networked player based on SpEffects applied by the other player's game
 */
void PlayerState::refresh_transmog_net_player()
{
    auto equip_param_protector = params::get_param<EquipParamProtector>(L"EquipParamProtector");

    bool undo_transmog = false;
    auto transmog_speffect_ids = std::vector<int>();

    for (auto entry = player->speffects->head; entry != nullptr; entry = entry->next)
    {
        // When this SpEffect ID is added, remove all transmog effects from this player
        if (entry->id == vfx::undo_transmog_speffect_id)
        {
            undo_transmog = true;
        }
        // Search for the dummy SpEffect IDs that indicate the player's transmog selections
        else if (entry->id >= vfx::transmog_dummy_speffect_start_id &&
                 entry->id < vfx::transmog_dummy_speffect_end_id)
        {
            undo_transmog = false;
            transmog_speffect_ids.push_back(entry->id);
        }
        // Search for the VFX SpEffect IDs chosen by that player's game
        else if (entry->id >= vfx::transmog_vfx_speffect_start_id &&
                 entry->id < vfx::transmog_vfx_speffect_end_id)
        {
            undo_transmog = false;
            set_vfx_speffect_ids(entry->id);
            entry->param = entry->id == head_speffect_id ? &head_speffect : &body_speffect;
        }
    }

    if (undo_transmog)
    {
        spdlog::info("Net player - undoing transmog");
        remove_transmog();
    }
    else
    {
        bool any_changed = false;
        for (auto speffect_id : transmog_speffect_ids)
        {
            auto protector_id = get_protector_id_for_transmog_speffect(speffect_id);
            if (protector_id != -1 &&
                set_transmog_protector(protector_id, equip_param_protector[protector_id]))
            {
                spdlog::info("Set net player transmog protector {}", protector_id);
                any_changed = true;
            }
        }

        if (any_changed || player != previous_player)
        {
            toggle_protectors();
        }
    }

    players::clear_speffect(player, vfx::undo_transmog_speffect_id);
}

/**
 * Check if the disable transmog SpEffect was applied more recently than a transmog was enabled.
 * This is useful for net players, since we don't have a callback for when they interact with the
 * transmog shop.
 */
bool PlayerState::should_clear_transmog()
{
    bool should_clear = false;
    for (auto entry = player->speffects->head; entry != nullptr; entry = entry->next)
    {
        if (entry->id == vfx::undo_transmog_speffect_id)
        {
            should_clear = true;
        }
        else if (entry->id >= vfx::transmog_dummy_speffect_start_id &&
                 entry->id < vfx::transmog_dummy_speffect_end_id)
        {
            should_clear = false;
        }
    }
    return should_clear;
}
