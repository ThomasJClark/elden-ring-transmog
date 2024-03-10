#pragma once

#include <cstdint>
#include <tga/paramdefs.h>

#include "internal/WorldChrMan.hpp"

namespace TransmogVFX
{
static constexpr int64_t transmog_set_base_id = 69000000;
static constexpr int64_t transmog_set_alt_base_id = 69001000;
static constexpr int64_t transmog_head_speffect_base_id = 169001000;
static constexpr int64_t transmog_body_speffect_base_id = 169002000;
static constexpr int64_t undo_transmog_speffect_id = 169999999;
static constexpr int64_t transmog_head_base_vfx_id = 6901000;
static constexpr int64_t transmog_body_base_vfx_id = 6902000;

static constexpr int64_t head_protector_offset = 0;
static constexpr int64_t body_protector_offset = 100;
static constexpr int64_t arms_protector_offset = 200;
static constexpr int64_t legs_protector_offset = 300;

static constexpr int32_t undo_transmog_sfx_id = 8020;

struct TransmogVFXState
{
    CS::PlayerIns *player;

    int64_t head_speffect_id;
    SpEffectParam head_speffect;

    int64_t body_speffect_id;
    SpEffectParam body_speffect;

    int64_t head_vfx_id;
    SpEffectVfxParam head_vfx;

    int64_t body_vfx_id;
    SpEffectVfxParam body_vfx;

    int64_t set_id;
    int64_t set_alt_id;
    EquipParamProtector *head;
    EquipParamProtector *body;
    EquipParamProtector *arms;
    EquipParamProtector *legs;

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
};

/**
 * Hook the internal get_*_param() functions to return the new params used for the transmog VFX
 */
void initialize();

/**
 * Apply the transmog VFX based on the shop items in the given player's inventory
 */
void refresh_transmog(TransmogVFXState &, int i, bool show_sfx = true);
} // namespace TransmogVFX
