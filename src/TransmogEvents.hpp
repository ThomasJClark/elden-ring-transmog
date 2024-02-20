#pragma once

#include "internal/Params.hpp"

namespace TransmogEvents
{
static constexpr uint32_t item_type_goods_begin = 0x40000000;
static constexpr uint32_t item_type_goods_end = 0x50000000;

/**
 * Manages the transmog VFX according to the player's selections in the transmog shop
 */
void initialize(CS::ParamMap &params);
void log();
void deinitialize();
}; // namespace TransmogEvents
