#pragma once

#include "internal/Params.hpp"

namespace TransmogEvents
{
/**
 * Manages the transmog VFX according to the player's selections in the transmog shop
 */
void initialize(CS::ParamMap &params);
void log();
void deinitialize();
}; // namespace TransmogEvents
