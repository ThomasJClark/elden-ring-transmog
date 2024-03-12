#pragma once

#include <cstdint>
#include <tga/paramdefs.h>

#include "TransmogShop.hpp"
#include "internal/WorldChrMan.hpp"

// Current transmog state for the main player or a networked player
class TransmogPlayerState
{
  public:
    CS::PlayerIns *player;

    int64_t head_speffect_id;
    SpEffectParam head_speffect;

    int64_t body_speffect_id;
    SpEffectParam body_speffect;

    int64_t head_vfx_id;
    SpEffectVfxParam head_vfx;

    int64_t body_vfx_id;
    SpEffectVfxParam body_vfx;

    int64_t head_id;
    EquipParamProtector *head;

    int64_t body_id;
    EquipParamProtector *body;

    int64_t arms_id;
    EquipParamProtector *arms;

    int64_t legs_id;
    EquipParamProtector *legs;

    int64_t set_id;
    int64_t set_alt_id;

    TransmogPlayerState()
        : player(nullptr), head_speffect_id(-1), body_speffect_id(-1), head_vfx_id(-1),
          body_vfx_id(-1), head_id(-1), head(nullptr), body_id(-1), body(nullptr), arms_id(-1),
          arms(nullptr), legs_id(-1), legs(nullptr), set_id(-1), set_alt_id(-1)
    {
    }

    void refresh_transmog(bool show_sfx = true);

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
        head_id = -1;
        head = nullptr;
        body_id = -1;
        body = nullptr;
        arms_id = -1;
        arms = nullptr;
        legs_id = -1;
        legs = nullptr;
    }

    inline bool set_transmog_protector(int64_t protector_id, EquipParamProtector &protector)
    {
        switch (protector.protectorCategory)
        {
        case TransmogShop::protector_category_head:
            if (head_id == protector_id)
                return false;
            head_id = protector_id;
            head = &protector;
            return true;
        case TransmogShop::protector_category_body:
            if (body_id == protector_id)
                return false;
            body_id = protector_id;
            body = &protector;
            return true;
        case TransmogShop::protector_category_arms:
            if (arms_id == protector_id)
                return false;
            arms_id = protector_id;
            arms = &protector;
            return true;
        case TransmogShop::protector_category_legs:
            if (legs_id == protector_id)
                return false;
            legs_id = protector_id;
            legs = &protector;
            return true;
        default:
            return false;
        }
    }

    inline bool is_head_transmog_enabled()
    {
        return head != nullptr;
    }

    inline bool is_body_transmog_enabled()
    {
        return body != nullptr || arms != nullptr | legs != nullptr;
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
