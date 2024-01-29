#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <tga/paramdefs.h>

typedef std::map<std::wstring, std::map<std::uint64_t, void *>> ParamMap;

namespace TransmogParams
{
static const std::int64_t transmog_speffect_id = 690001;
static const std::int64_t transmog_head_vfx_id = 690001;
static const std::int64_t transmog_body_vfx_id = 690002;
static const std::int64_t transmog_head_id = 69000000;
static const std::int64_t transmog_body_id = 69000100;
static const std::int64_t transmog_arms_id = 69000200;
static const std::int64_t transmog_legs_id = 69000300;
static const std::int64_t transmog_reinforce_param_id = 0;

/**
 * Hook the internal get_*_param() functions to return the new params used for transmog
 */
void initialize(ParamMap &params);
void deinitialize();

/**
 * Set the given armor piece as the player's appearance
 */
void set_transmog(EquipParamProtector *equip_param_protector);
} // namespace TransmogParams
