#pragma once

#include <map>
#include <string>

namespace ertransmogrify {
namespace msg {
struct Messages {
    std::wstring transmog_armor;
    std::wstring transmog_head;
    std::wstring transmog_chest;
    std::wstring transmog_arms;
    std::wstring transmog_legs;
    std::wstring undo_transmog;
    std::wstring transmog_head_title;
    std::wstring transmog_chest_title;
    std::wstring transmog_arms_title;
    std::wstring transmog_legs_title;
    std::wstring invisible;
    std::wstring select_transmog_target;
    std::wstring transmog_head_to;
    std::wstring transmog_chest_to;
    std::wstring transmog_arms_to;
    std::wstring transmog_legs_to;
};

extern const std::map<std::string, Messages> messages_by_lang;

static constexpr int event_text_for_talk_transmog_armor = 69000000;
static constexpr int event_text_for_talk_transmog_head = 69000010;
static constexpr int event_text_for_talk_transmog_chest = 69000011;
static constexpr int event_text_for_talk_transmog_arms = 69000012;
static constexpr int event_text_for_talk_transmog_legs = 69000013;
static constexpr int event_text_for_talk_undo_transmog = 69000020;
static constexpr int event_text_for_talk_cancel = 15000372;
static constexpr int event_text_for_talk_sort_chest = 15000395;
static constexpr int menu_text_transmog_head = 690000;
static constexpr int menu_text_transmog_chest = 690001;
static constexpr int menu_text_transmog_arms = 690002;
static constexpr int menu_text_transmog_legs = 690003;
static constexpr int line_help_select_item_for_purchase = 231000;
static constexpr int dialogues_purchase_item_for_runes = 231000;

const std::wstring cut_item_prefix = L"[ERROR]";

/**
 * Determine the player's selected language, then hook the internal get_message() function to
 * return strings for the transmog menu
 */
void initialize();

void set_active_transmog_shop_protector_category(signed char protector_category);

/**
 * Returns the name of the armor with the given ID, and true if it's a DLC item (which is
 * determined) by checking if the name is in the DLC messages or not.
 */
const std::pair<std::wstring_view, bool> get_protector_data(int id);
};
};
