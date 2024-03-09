#pragma once

#include <string>

namespace TransmogConfig
{
/**
 * Load user preferences from a ertransmogrify.ini file located in the same folder as the DLL
 */
void load_config(const std::wstring_view &dll_path);

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
}; // namespace TransmogConfig