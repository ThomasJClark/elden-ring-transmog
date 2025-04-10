#pragma once

#include <filesystem>

namespace ertransmogrify {
namespace config {
/**
 * Load user preferences from an .ini file
 */
void load(const std::filesystem::path &ini_path);

/**
 * Don't require owning an armor piece to unlock it for transmogrification
 */
extern bool include_unobtained_armor;

/**
 * Include cut content armor as transmogrification options (e.g. Brave's Battlewear)
 */
extern bool include_cut_armor;

/**
 * Include Shadow of the Erdtree armor pieces as transmogrification options. This can be disabled
 * to avoid spoilers.
 */
extern bool include_dlc_armor;

/**
 * Add the "Tranmsogrify armor" option to the Site of Grace menu. This can be disabled by mod
 * authors to add their entrypoint for transmog.
 *
 * Note that this isn't in the default ertransmogrify.ini, because it's not intended to be changed
 * by mod players. Everyone who needs to use this already knows about it.
 */
extern bool patch_grace_talk_script;

/**
 * Delay (in milliseconds) before enabling the mod. This can help workaround a Seamless Co-op
 * compatibility issue where Seamless overrides some hooks if it initializes after this mod.
 */
extern unsigned int initialize_delay;

/**
 * Disables networking, for PVP reasons.
 */
extern bool client_side_only;

/**
 * Enables debug console
 */
extern bool debug;
};
};
