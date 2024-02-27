#pragma once

#include <cstdint>
#include <tga/paramdefs.h>

namespace TransmogVFX
{
/**
 * Hook the internal get_*_param() functions to return the new params used for the transmog VFX
 */
void initialize();

/**
 * Apply the transmog VFX based on the shop items in the player's inventory
 */
void enable_transmog();

/**
 * Remove the transmog VFX from the main character
 */
void disable_transmog();
} // namespace TransmogVFX
