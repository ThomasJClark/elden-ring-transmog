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
void refresh_transmog(bool show_sfx = true);
} // namespace TransmogVFX
