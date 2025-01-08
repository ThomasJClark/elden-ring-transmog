#pragma once

#include "ertransmogrify_vfx.hpp"

#include <elden-x/chr/player.hpp>

namespace ertransmogrify
{
namespace local_player
{

/**
 * Get the transmog state selected by the local player
 */
ertransmogrify::vfx::player_state_st get_local_player_state(from::CS::PlayerIns *);

}
}
