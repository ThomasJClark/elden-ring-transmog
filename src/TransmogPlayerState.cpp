#include <iostream>
#include <vector>

#include "ParamUtils.hpp"
#include "PlayerUtils.hpp"
#include "TransmogPlayerState.hpp"
#include "TransmogVFX.hpp"
#include "internal/GameMan.hpp"

using namespace std;

static constexpr int32_t transmog_sfx_id = 8020;

static inline int64_t get_transmog_speffect_id_for_protector(int64_t protector_id)
{
    if (protector_id == -1)
    {
        return -1;
    }

    return TransmogVFX::transmog_dummy_speffect_start_id + protector_id / 100;
}

static inline int64_t get_protector_id_for_transmog_speffect(int64_t speffect_id)
{
    if (speffect_id >= TransmogVFX::transmog_dummy_speffect_start_id &&
        speffect_id < TransmogVFX::transmog_dummy_speffect_end_id)
    {
        return 100 * (speffect_id - TransmogVFX::transmog_dummy_speffect_start_id);
    }

    return -1;
}

void TransmogPlayerState::refresh_transmog(bool show_sfx)
{
    if (player == nullptr)
    {
        clear_transmog_protectors();
        return;
    }

    bool previous_is_transmog_enabled = is_transmog_enabled();

    if (player == PlayerUtils::get_main_player())
    {
        refresh_transmog_main_player();
    }
    else
    {
        refresh_transmog_net_player();
    }

    // When transmog is enabled or disabled, show a cool effect on the player
    if (show_sfx && (is_transmog_enabled() != previous_is_transmog_enabled))
    {
        PlayerUtils::spawn_one_shot_sfx_on_chr(player, 900, transmog_sfx_id, nullptr);
    };
}

/**
 * Refresh transmog for the main player based on goods in their inventory
 */
void TransmogPlayerState::refresh_transmog_main_player()
{
    auto equip_param_protector = ParamUtils::get_param<EquipParamProtector>(L"EquipParamProtector");

    bool any_changed = false;

    // Skip checking the inventory if the player is in a ceremony (i.e. pseudo-multiplayer), because
    // inventory isn't completely copied over, and it's not possible for this to have changed since
    // the last time it was checked.
    if (PlayerUtils::get_ceremony_type() == PlayerUtils::ceremony_type_none)
    {
        if (should_clear_transmog())
        {
            if (is_transmog_enabled())
            {
                // If the player was given the undo transmog SpEffect, don't set any protector slots
                // and remove any transmog goods.
                clear_transmog_protectors();
                TransmogShop::remove_transmog_goods();
                cout << "[transmog] Removed main player transmog goods" << endl;
            }
        }
        else
        {
            // Otherwise, set the protector slots based on the transmog goods in the player's
            // inventory
            for (auto [protector_id, protector] : equip_param_protector)
            {
                int32_t transmog_item_id =
                    TransmogShop::item_type_goods_begin +
                    TransmogShop::get_transmog_goods_id_for_protector(protector_id);

                if (PlayerUtils::has_item_in_inventory(player, transmog_item_id))
                {
                    if (set_transmog_protector(protector_id, protector))
                    {
                        cout << "[transmog] Set main player transmog protector " << protector_id
                             << endl;
                        any_changed = true;
                    }
                }
            }
        }
    }

    // Body/arms/legs have to be combined in one spffect. If transmog is enabled on some but not
    // all of them, default the others to the player's current armor
    if (is_body_transmog_enabled())
    {
        auto &chr_asm = player->player_game_data->equip_game_data.chr_asm;
        if (body == nullptr)
        {
            body_id = chr_asm.body_protector_id;
            body = &equip_param_protector[chr_asm.body_protector_id];
            TransmogShop::add_transmog_good(chr_asm.body_protector_id);
            cout << "[transmog] Defaulting main player body to protector " << body_id << endl;
        }
        if (arms == nullptr)
        {
            arms_id = chr_asm.arms_protector_id;
            arms = &equip_param_protector[chr_asm.arms_protector_id];
            TransmogShop::add_transmog_good(chr_asm.arms_protector_id);
            cout << "[transmog] Defaulting main player arms to protector " << arms_id << endl;
        }
        if (legs == nullptr)
        {
            legs_id = chr_asm.legs_protector_id;
            legs = &equip_param_protector[chr_asm.legs_protector_id];
            TransmogShop::add_transmog_good(chr_asm.legs_protector_id);
            cout << "[transmog] Defaulting main player legs to protector " << legs_id << endl;
        }
    }

    if (any_changed)
    {
        toggle_protectors();
    }

    // Apply SpEffects for the selected transmog protectors, to indicate the selections to other
    // networked players
    if (head_id != -1)
    {
        PlayerUtils::apply_speffect(player, get_transmog_speffect_id_for_protector(head_id), false);
    }
    if (body_id != -1)
    {
        PlayerUtils::apply_speffect(player, get_transmog_speffect_id_for_protector(body_id), false);
    }
    if (arms_id != -1)
    {
        PlayerUtils::apply_speffect(player, get_transmog_speffect_id_for_protector(arms_id), false);
    }
    if (legs_id != -1)
    {
        PlayerUtils::apply_speffect(player, get_transmog_speffect_id_for_protector(legs_id), false);
    }

    // Remove any existing SpEffects for protectors that aren't selected
    auto cleared_speffects = vector<int32_t>();
    for (auto entry = player->speffects->head; entry != nullptr; entry = entry->next)
    {
        auto protector_id = get_protector_id_for_transmog_speffect(entry->id);
        if (protector_id != -1)
        {
            if (protector_id != head_id && protector_id != body_id && protector_id != arms_id &&
                protector_id != legs_id)
            {
                cleared_speffects.push_back(entry->id);
            }
        }
    }
    for (auto speffect_id : cleared_speffects)
        PlayerUtils::clear_speffect(player, speffect_id);

    // Ensure the player has the transmog VFX if they have anything selected
    if (is_head_transmog_enabled())
    {
        PlayerUtils::apply_speffect(player, head_speffect_id, false);
    }
    else
    {
        PlayerUtils::clear_speffect(player, head_speffect_id);
    }
    if (is_body_transmog_enabled())
    {
        PlayerUtils::apply_speffect(player, body_speffect_id, false);
    }
    else
    {
        PlayerUtils::clear_speffect(player, body_speffect_id);
    }
}

/**
 * Refresh transmog for a networked player based on SpEffects applied by the other player's game
 */
void TransmogPlayerState::refresh_transmog_net_player()
{
    auto equip_param_protector = ParamUtils::get_param<EquipParamProtector>(L"EquipParamProtector");

    clear_transmog_protectors();

    bool undo_transmog = false;
    auto transmog_speffect_ids = vector<int32_t>();

    for (auto entry = player->speffects->head; entry != nullptr; entry = entry->next)
    {
        // When this SpEffect ID is added, remove all transmog effects from this player
        if (entry->id == TransmogVFX::undo_transmog_speffect_id)
        {
            undo_transmog = true;
        }
        // Search for the dummy SpEffect IDs that indicate the player's transmog selections
        else if (entry->id >= TransmogVFX::transmog_dummy_speffect_start_id &&
                 entry->id < TransmogVFX::transmog_dummy_speffect_end_id)
        {
            undo_transmog = false;
            transmog_speffect_ids.push_back(entry->id);
        }
        // Search for the VFX SpEffect IDs chosen by that player's game
        else if (entry->id >= TransmogVFX::transmog_vfx_speffect_start_id &&
                 entry->id < TransmogVFX::transmog_vfx_speffect_end_id)
        {
            undo_transmog = false;
            set_vfx_speffect_ids(entry->id);
            entry->param = entry->id == head_speffect_id ? &head_speffect : &body_speffect;
        }
    }

    if (undo_transmog)
    {
        clear_transmog_protectors();

        PlayerUtils::clear_speffect(player, head_speffect_id);
        PlayerUtils::clear_speffect(player, body_speffect_id);

        for (auto speffect_id : transmog_speffect_ids)
        {
            PlayerUtils::clear_speffect(player, speffect_id);
        }
    }
    else
    {
        for (auto speffect_id : transmog_speffect_ids)
        {
            auto protector_id = get_protector_id_for_transmog_speffect(speffect_id);
            if (protector_id != -1)
            {
                if (protector_id != head_id && protector_id != body_id && protector_id != arms_id &&
                    protector_id != legs_id)
                {
                    PlayerUtils::clear_speffect(player, speffect_id);
                }
            }
        }

        bool any_changed = false;
        for (auto speffect_id : transmog_speffect_ids)
        {
            auto protector_id = get_protector_id_for_transmog_speffect(speffect_id);
            if (protector_id != -1 &&
                set_transmog_protector(protector_id, equip_param_protector[protector_id]))
            {
                any_changed = true;
            }
        }

        if (any_changed)
        {
            toggle_protectors();
        }
    }

    PlayerUtils::clear_speffect(player, TransmogVFX::undo_transmog_speffect_id);
}

/**
 * Check if the disable transmog SpEffect was applied more recently than a transmog was enabled.
 * This is useful for net players, since we don't have a callback for when they interact with the
 * transmog shop.
 */
bool TransmogPlayerState::should_clear_transmog()
{
    bool should_clear = false;
    for (auto entry = player->speffects->head; entry != nullptr; entry = entry->next)
    {
        if (entry->id == TransmogVFX::undo_transmog_speffect_id)
        {
            should_clear = true;
        }
        else if (entry->id >= TransmogVFX::transmog_dummy_speffect_start_id &&
                 entry->id < TransmogVFX::transmog_dummy_speffect_end_id)
        {
            should_clear = false;
        }
    }
    return should_clear;
}