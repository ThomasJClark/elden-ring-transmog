#pragma once

#include <filesystem>

namespace TransmogConfig
{
/**
 * Load user preferences from an .ini file
 */
void load_config(const std::filesystem::path &ini_path);

/**
 * Don't require owning an armor piece to unlock it for transmogrification
 */
extern bool include_unobtained_armor;

/**
 * Include cut content armor as transmogrification options (e.g. Brave's Battlewear)
 */
extern bool include_cut_armor;

/**
 * Transmog affects the player's idle pose (e.g. the way your arms spread out while wearing
 * Lionel's Armor)
 */
extern bool transmog_affects_posture;

/**
 * Add the "Tranmsogrify armor" option to the Site of Grace menu. This can be disabled by mod
 * authors to add their entrypoint for transmog.
 */
extern bool patch_grace_talk_script;

/**
 * Delay (in milliseconds) before enabling the mod. This can help workaround a Seamless Co-op
 * compatibility issue where Seamless overrides some hooks if it initializes after this mod.
 */
extern uint32_t initialize_delay;
}; 