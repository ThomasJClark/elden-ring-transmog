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
 * Apply the transmog VFX based on the shop items in the player's inventory
 */
void enable_transmog();

/**
 * Remove the transmog VFX from the main character
 */
void disable_transmog();
} // namespace TransmogVFX
