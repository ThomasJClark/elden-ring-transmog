#pragma once
#include <cstdint>
#include <string>

namespace TransmogMessages
{
namespace EventTextForTalk
{
static constexpr int32_t transmog_armor = 69000000;
static constexpr int32_t transmog_head = 69000010;
static constexpr int32_t transmog_body = 69000011;
static constexpr int32_t transmog_arms = 69000012;
static constexpr int32_t transmog_legs = 69000013;
static constexpr int32_t undo_transmog = 69000020;
static constexpr int32_t cancel = 15000372;
static constexpr int32_t sort_chest = 15000395;
} // namespace EventTextForTalk

namespace MenuText
{
static constexpr int32_t transmog_head = 690000;
static constexpr int32_t transmog_body = 690001;
static constexpr int32_t transmog_arms = 690002;
static constexpr int32_t transmog_legs = 690003;
} // namespace MenuText

namespace LineHelp
{
static constexpr int32_t select_item_for_purchase = 231000;
}

namespace Dialogues
{
static constexpr int32_t purchase_item_for_runes = 231000;
}

const std::u16string cut_item_prefix = u"[ERROR]";

/**
 * Determine the player's selected language, then hook the internal get_message() function to
 * return strings for the transmog menu
 */
void initialize();

const std::u16string_view get_protector_name(int32_t id);
}; // namespace TransmogMessages
