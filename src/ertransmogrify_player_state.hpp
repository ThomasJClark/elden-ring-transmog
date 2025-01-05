#pragma once

#include "ertransmogrify_shop.hpp"

#include <elden-x/chr/player.hpp>
#include <elden-x/paramdef/EQUIP_PARAM_PROTECTOR_ST.hpp>
#include <elden-x/paramdef/SP_EFFECT_PARAM_ST.hpp>
#include <elden-x/paramdef/SP_EFFECT_VFX_PARAM_ST.hpp>

namespace ertransmogrify
{
// Current transmog state for the main player or a networked player
class PlayerState
{
  public:
    from::CS::PlayerIns *player;
    from::CS::PlayerIns *previous_player;
    bool previous_refreshed;

    long long head_speffect_id;
    from::paramdef::SP_EFFECT_PARAM_ST head_speffect;

    long long body_speffect_id;
    from::paramdef::SP_EFFECT_PARAM_ST body_speffect;

    long long head_vfx_id;
    from::paramdef::SP_EFFECT_VFX_PARAM_ST head_vfx;

    long long body_vfx_id;
    from::paramdef::SP_EFFECT_VFX_PARAM_ST body_vfx;

    long long head_protector_id;
    from::paramdef::EQUIP_PARAM_PROTECTOR_ST *head_protector;

    long long chest_protector_id;
    from::paramdef::EQUIP_PARAM_PROTECTOR_ST *chest_protector;

    long long arms_protector_id;
    from::paramdef::EQUIP_PARAM_PROTECTOR_ST *arms_protector;

    long long legs_protector_id;
    from::paramdef::EQUIP_PARAM_PROTECTOR_ST *legs_protector;

    long long set_id;
    long long set_alt_id;

    PlayerState()
        : player(nullptr), head_speffect_id(-1), body_speffect_id(-1), head_vfx_id(-1),
          body_vfx_id(-1), head_protector_id(-1), head_protector(nullptr), chest_protector_id(-1),
          chest_protector(nullptr), arms_protector_id(-1), arms_protector(nullptr),
          legs_protector_id(-1), legs_protector(nullptr), set_id(-1), set_alt_id(-1)
    {
    }

    void refresh_transmog();

    void remove_transmog();

    /**
     * Toggle the set between the default and alternate IDs, to force the game to display the new
     * protectors after patching a VFX param
     */
    inline void toggle_protectors()
    {
        if (head_vfx.transformProtectorId == set_id)
        {
            head_vfx.transformProtectorId = set_alt_id;
            body_vfx.transformProtectorId = set_alt_id;
        }
        else
        {
            head_vfx.transformProtectorId = set_id;
            body_vfx.transformProtectorId = set_id;
        }
    }

    inline void set_vfx_speffect_ids(long long head_or_body_speffect_id)
    {
        if (head_or_body_speffect_id % 2 == 0)
        {
            head_speffect_id = head_or_body_speffect_id;
            body_speffect_id = head_or_body_speffect_id + 1;
        }
        else
        {
            head_speffect_id = head_or_body_speffect_id - 1;
            body_speffect_id = head_or_body_speffect_id;
        }
    }

    inline void clear_transmog_protectors()
    {
        head_protector_id = -1;
        head_protector = nullptr;
        chest_protector_id = -1;
        chest_protector = nullptr;
        arms_protector_id = -1;
        arms_protector = nullptr;
        legs_protector_id = -1;
        legs_protector = nullptr;
    }

    inline bool set_transmog_protector(long long protector_id,
                                       from::paramdef::EQUIP_PARAM_PROTECTOR_ST &protector)
    {
        switch (protector.protectorCategory)
        {
        case shop::protector_category_head:
            if (head_protector_id == protector_id)
                return false;
            head_protector_id = protector_id;
            head_protector = &protector;
            return true;
        case shop::protector_category_chest:
            if (chest_protector_id == protector_id)
                return false;
            chest_protector_id = protector_id;
            chest_protector = &protector;
            return true;
        case shop::protector_category_arms:
            if (arms_protector_id == protector_id)
                return false;
            arms_protector_id = protector_id;
            arms_protector = &protector;
            return true;
        case shop::protector_category_legs:
            if (legs_protector_id == protector_id)
                return false;
            legs_protector_id = protector_id;
            legs_protector = &protector;
            return true;
        default:
            return false;
        }
    }

    inline bool is_head_transmog_enabled()
    {
        return head_protector != nullptr;
    }

    inline bool is_body_transmog_enabled()
    {
        return chest_protector != nullptr || arms_protector != nullptr || legs_protector != nullptr;
    }

    inline bool is_transmog_enabled()
    {
        return is_head_transmog_enabled() || is_body_transmog_enabled();
    }

  private:
    void refresh_transmog_main_player();
    void refresh_transmog_net_player();
    bool should_clear_transmog();
};
}
