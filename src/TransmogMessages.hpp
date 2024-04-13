#pragma once
#include <cstdint>
#include <string>

namespace ertransmogrify
{
namespace msg
{
static constexpr int32_t event_text_for_talk_transmog_armor = 69000000;
static constexpr int32_t event_text_for_talk_transmog_head = 69000010;
static constexpr int32_t event_text_for_talk_transmog_chest = 69000011;
static constexpr int32_t event_text_for_talk_transmog_arms = 69000012;
static constexpr int32_t event_text_for_talk_transmog_legs = 69000013;
static constexpr int32_t event_text_for_talk_undo_transmog = 69000020;
static constexpr int32_t event_text_for_talk_cancel = 15000372;
static constexpr int32_t event_text_for_talk_sort_chest = 15000395;
static constexpr int32_t menu_text_transmog_head = 690000;
static constexpr int32_t menu_text_transmog_chest = 690001;
static constexpr int32_t menu_text_transmog_arms = 690002;
static constexpr int32_t menu_text_transmog_legs = 690003;
static constexpr int32_t line_help_select_item_for_purchase = 231000;
static constexpr int32_t dialogues_purchase_item_for_runes = 231000;

const std::wstring cut_item_prefix = L"[ERROR]";

/**
 * Determine the player's selected language, then hook the internal get_message() function to
 * return strings for the transmog menu
 */
void initialize();

void set_active_transmog_shop_protector_category(int8_t protector_category);

const std::wstring_view get_protector_name(int32_t id);
};
};
