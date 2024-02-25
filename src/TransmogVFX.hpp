#pragma once

#include <cstdint>
#include <tga/paramdefs.h>

#include "internal/WorldChrMan.hpp"

namespace TransmogVFX
{
static constexpr int64_t transmog_head_speffect_id = 690001;
static constexpr int64_t transmog_body_speffect_id = 690002;
static constexpr int64_t transmog_head_vfx_id = 690001;
static constexpr int64_t transmog_body_vfx_id = 690002;
static constexpr int64_t transmog_set_id = 69000000;
static constexpr int64_t transmog_set_alt_id = 69010000;
static constexpr int64_t transmog_reinforce_param_id = 0;
static constexpr int64_t head_protector_offset = 0;
static constexpr int64_t body_protector_offset = 100;
static constexpr int64_t arms_protector_offset = 200;
static constexpr int64_t legs_protector_offset = 300;

/**
 * Hook the internal get_*_param() functions to return the new params used for the transmog VFX
 */
void initialize(CS::WorldChrManImp **);

/**
 * Set the dynamic transmog params to their initial state
 */
void reset_transmog();

/**
 * Set the given armor piece as the player's appearance
 */
EquipParamProtector *set_transmog_protector(int64_t equip_param_protector_id);

/**
 * Remove the transmog VFX from the main character
 */
void disable_transmog();
} // namespace TransmogVFX
