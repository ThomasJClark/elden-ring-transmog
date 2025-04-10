#pragma once

#include "ertransmogrify_vfx.hpp"

#include <steam/steamclientpublic.h>

namespace ertransmogrify {
namespace net {

/**
 * Send the local player's transmog selections to every connected player in the current session
 */
void send_messages(const ertransmogrify::vfx::player_state_st &);

/**
 * Check for new messages from the Steamworks API containing the transmog state of other
 * connected players
 */
void receive_messages();

/**
 * Get the transmog state sent by another player
 */
const ertransmogrify::vfx::player_state_st &get_net_player_state(CSteamID);

}
}
