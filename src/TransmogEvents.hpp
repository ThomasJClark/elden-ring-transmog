#pragma once

#include "internal/WorldChrMan.hpp"

namespace TransmogEvents
{
/**
 * Manages the transmog VFX according to the player's selections in the transmog shop
 */
void initialize(CS::WorldChrManImp **world_chr_man_addr);
}; // namespace TransmogEvents
