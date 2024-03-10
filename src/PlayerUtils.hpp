#pragma once

#include <cstdint>

#include "internal/WorldChrMan.hpp"

namespace PlayerUtils
{
static constexpr size_t net_player_max = 6;

void initialize();
CS::PlayerIns *get_main_player();
CS::NetPlayer *get_net_players();
bool has_item_in_inventory(CS::PlayerIns *, int32_t item_id);
}; // namespace PlayerUtils