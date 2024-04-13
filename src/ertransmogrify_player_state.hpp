#pragma once

#include <cstdint>
#include <tga/paramdefs.h>

#include "ertransmogrify_shop.hpp"
#include "internal/WorldChrMan.hpp"

namespace ertransmogrify
{
// Current transmog state for the main player or a networked player
class PlayerState
{
  public:
    CS::PlayerIns *player;
    CS::PlayerIns *previous_player;
    bool previous_refreshed;

    int64_t head_speffect_id;
    SpEffectParam head_speffect;

    int64_t body_speffect_id;
    SpEffectParam body_speffect;

    int64_t head_vfx_id;
    SpEffectVfxParam head_vfx;

    int64_t body_vfx_id;
    SpEffectVfxParam body_vfx;

    int64_t head_protector_id;
    EquipParamProtector *head_protector;

    int64_t chest_protector_id;
    EquipParamProtector *chest_protector;

    int64_t arms_protector_id;
    EquipParamProtector *arms_protector;

    int64_t legs_protector_id;
    EquipParamProtector *legs_protector;

    int64_t set_id;
    int64_t set_alt_id;

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

    inline void set_vfx_speffect_ids(int64_t head_or_body_speffect_id)
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

    inline bool set_transmog_protector(int64_t protector_id, EquipParamProtector &protector)
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
