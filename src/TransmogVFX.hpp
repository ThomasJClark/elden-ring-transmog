#pragma once

#include <cstdint>
#include <tga/paramdefs.h>

namespace TransmogVFX
{
static constexpr int64_t transmog_speffect_id_begin = 169000000;
static constexpr int64_t transmog_speffect_id_end = 169999999;
static constexpr int64_t undo_transmog_speffect_id = 169999999;
static constexpr int64_t transmog_head_vfx_id = 6900100;
static constexpr int64_t transmog_body_vfx_id = 6900101;
static constexpr int64_t transmog_set_id = 69000000;
static constexpr int64_t transmog_set_alt_id = 69010000;
static constexpr int64_t transmog_reinforce_param_id = 0;
static constexpr int64_t head_protector_offset = 0;
static constexpr int64_t body_protector_offset = 100;
static constexpr int64_t arms_protector_offset = 200;
static constexpr int64_t legs_protector_offset = 300;
static constexpr int32_t undo_transmog_sfx_id = 8020;

/**
 * Hook the internal get_*_param() functions to return the new params used for the transmog VFX
 */
void initialize();

/**
 * Apply the transmog VFX based on the shop items in the player's inventory
 */
void refresh_transmog(bool show_sfx = true);
} // namespace TransmogVFX
