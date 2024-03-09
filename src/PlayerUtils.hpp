#pragma once

#include <cstdint>

#include "internal/WorldChrMan.hpp"

namespace PlayerUtils
{
void initialize();
CS::PlayerIns *get_main_player();
bool has_item_in_inventory(CS::PlayerIns *, int32_t item_id);
}; // namespace PlayerUtils