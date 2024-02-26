#pragma once

#include <cstdint>
#include <tga/paramdefs.h>

#include "internal/WorldChrMan.hpp"

namespace TransmogVFX
{
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
