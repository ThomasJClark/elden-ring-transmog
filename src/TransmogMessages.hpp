#pragma once
#include <cstdint>
#include <string>

struct MsgRepository;

namespace TransmogMessages
{
static const int32_t event_text_for_talk_transmog_armor_id = 69000000;
static const int32_t event_text_for_talk_transmog_head_id = 69000010;
static const int32_t event_text_for_talk_transmog_body_id = 69000011;
static const int32_t event_text_for_talk_transmog_arms_id = 69000012;
static const int32_t event_text_for_talk_transmog_legs_id = 69000013;
static const int32_t event_text_for_talk_undo_transmog_id = 69000020;
static const int32_t event_text_for_talk_cancel_id = 15000372;
static const int32_t event_text_for_map_undo_transmog_alert_id = 690000;
static const int32_t menu_text_transmog_head_id = 690000;
static const int32_t menu_text_transmog_body_id = 690001;
static const int32_t menu_text_transmog_arms_id = 690002;
static const int32_t menu_text_transmog_legs_id = 690003;
static const int32_t line_help_select_item_for_purchase_id = 231000;
static const int32_t dialogues_purchase_item_for_runes = 231000;

/**
 * Determine the player's selected language, then hook the internal get_message() function to
 * return strings for the transmog menu
 */
void initialize(MsgRepository *msg_repository);

const std::u16string_view get_protector_name(MsgRepository *msg_repository, int32_t id);

void deinitialize();
}; // namespace TransmogMessages
