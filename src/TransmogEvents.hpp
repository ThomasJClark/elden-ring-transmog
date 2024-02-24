#pragma once

#include "internal/WorldChrMan.hpp"

namespace TransmogEvents
{
static constexpr uint32_t item_type_goods_begin = 0x40000000;
static constexpr uint32_t item_type_goods_end = 0x50000000;

/**
 * Manages the transmog VFX according to the player's selections in the transmog shop
 */
void initialize(CS::WorldChrManImp **world_chr_man_addr);
}; // namespace TransmogEvents
