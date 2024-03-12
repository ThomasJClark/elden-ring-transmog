#pragma once

#include <cstdint>

namespace TransmogVFX
{
// Fake armor set used as the transformProtector for transmogrification. The params that this ID
// resolves to changes at runtime based on the player's transmog selection.
static constexpr int64_t transmog_set_base_id = 69000000;
static constexpr int64_t transmog_set_alt_base_id = 69001000;

// VFX IDs that apply the transmog visuals. In multiplayer, these IDs are offset by +1, +2, +3,
// etc., so each player has a different VFX param.
static constexpr int64_t transmog_head_base_vfx_id = 6901000;
static constexpr int64_t transmog_body_base_vfx_id = 6902000;

// SpEffects applied to enable the transmog VFX. These are chosen at random in order to avoid
// conflicts in Seamless Co-op.
static constexpr int64_t transmog_vfx_speffect_start_id = 169000000;
static constexpr int64_t transmog_vfx_speffect_end_id = 169500000;

// SpEffects applied to keep track of the player's transmog selections. These aren't used locally
// since we can simply check in memory what those selections are, but they allow us to side channel
// the transmog selections to other players in co-op, since applied SpEffect IDs are automatically
// synced.
static constexpr int64_t transmog_dummy_speffect_start_id = 169500000;
static constexpr int64_t transmog_dummy_speffect_end_id = 169900000;

// Magic SpEffect applied to the player to undo transmogrification. This decouples the VFX from the
// talkscript system, so we can disable talkscript patching and allow modpack authors to dispel
// transmogrification in some other way.
static constexpr int64_t undo_transmog_speffect_id = 169999999;

static constexpr int64_t head_protector_offset = 0;
static constexpr int64_t body_protector_offset = 100;
static constexpr int64_t arms_protector_offset = 200;
static constexpr int64_t legs_protector_offset = 300;

/**
 * Hook the internal get_*_param() functions to return the new params used for the transmog VFX
 */
void initialize();

} // namespace TransmogVFX
