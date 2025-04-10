#include "ertransmogrify_messages.hpp"
#include "ertransmogrify_shop.hpp"

#include <spdlog/spdlog.h>
#include <steam/isteamapps.h>
#include <elden-x/utils/modutils.hpp>

#include <chrono>
#include <string>
#include <thread>

using namespace ertransmogrify;

static const unsigned int msgbnd_goods_name = 10;
static const unsigned int msgbnd_protector_name = 12;
static const unsigned int msgbnd_goods_info = 20;
static const unsigned int msgbnd_goods_caption = 24;
static const unsigned int msgbnd_protector_caption = 26;
static const unsigned int msgbnd_event_text_for_talk = 33;
static const unsigned int msgbnd_menu_text = 200;
static const unsigned int msgbnd_line_help = 201;
static const unsigned int msgbnd_system_message_win64 = 203;
static const unsigned int msgbnd_dialogues = 204;
static const unsigned int msgbnd_dlc_protector_name = 313;
static const unsigned int msgbnd_dlc_goods_name = 319;

namespace CS {
struct MsgRepository;
}
static CS::MsgRepository *msg_repository = nullptr;

/**
 * Assigned the list of localized messages based on the player's language preference
 */
static msg::Messages transmog_messages;

/**
 * Set while the transmog menu is open to adjust some of the UI strings, or to -1 while a different
 * shop menu is open
 */
static signed char active_transmog_shop_protector_category = -1;

static std::wstring_view get_goods_name(int id);

static const wchar_t *(*get_message)(CS::MsgRepository *msg_repository,
                                     unsigned int unknown,
                                     unsigned int bnd_id,
                                     int msg_id);

const wchar_t *get_message_detour(CS::MsgRepository *msg_repository,
                                  unsigned int unknown,
                                  unsigned int bnd_id,
                                  int msg_id) {
    switch (bnd_id) {
        case msgbnd_event_text_for_talk:
            switch (msg_id) {
                case msg::event_text_for_talk_transmog_armor:
                    return transmog_messages.transmog_armor.c_str();
                case msg::event_text_for_talk_transmog_head:
                    return transmog_messages.transmog_head.c_str();
                case msg::event_text_for_talk_transmog_chest:
                    return transmog_messages.transmog_chest.c_str();
                case msg::event_text_for_talk_transmog_arms:
                    return transmog_messages.transmog_arms.c_str();
                case msg::event_text_for_talk_transmog_legs:
                    return transmog_messages.transmog_legs.c_str();
                case msg::event_text_for_talk_undo_transmog:
                    return transmog_messages.undo_transmog.c_str();
            }
            break;

        case msgbnd_menu_text:
            switch (msg_id) {
                case msg::menu_text_transmog_head:
                    return transmog_messages.transmog_head_title.c_str();
                case msg::menu_text_transmog_chest:
                    return transmog_messages.transmog_chest_title.c_str();
                case msg::menu_text_transmog_arms:
                    return transmog_messages.transmog_arms_title.c_str();
                case msg::menu_text_transmog_legs:
                    return transmog_messages.transmog_legs_title.c_str();
            }
            break;

        case msgbnd_goods_name: {
            auto protector_id = shop::get_protector_id_for_transmog_good(msg_id);
            if (protector_id > 0) {
                // Show bare head/chest/arms/legs as "Invisible"
                if (shop::is_invisible_protector_id(protector_id)) {
                    return transmog_messages.invisible.c_str();
                }

                // Show DLC transformation options as the name of the item that activates them,
                // e.g. "Priestess Heart"
                if (shop::dlc_transformation_goods_by_protector_id.contains(protector_id)) {
                    auto goods_id = shop::dlc_transformation_goods_by_protector_id.at(protector_id);
                    return get_goods_name(goods_id).data();
                }

                auto [protector_name, protector_is_dlc] = msg::get_protector_data(protector_id);

                // Remove the "[ERROR]" prefix from cut items in the transmog shop
                if (protector_name.starts_with(msg::cut_item_prefix)) {
                    protector_name = protector_name.substr(msg::cut_item_prefix.size());
                }

                return protector_name.data();
            }
            break;
        }

        case msgbnd_goods_caption: {
            auto protector_id = shop::get_protector_id_for_transmog_good(msg_id);
            if (protector_id > 0) {
                return L"";
            }
            break;
        }

        case msgbnd_goods_info: {
            auto protector_id = shop::get_protector_id_for_transmog_good(msg_id);
            if (protector_id > 0) {
                return L"";
            }
            break;
        }

        case msgbnd_line_help:
            if (msg_id == msg::line_help_select_item_for_purchase) {
                if (active_transmog_shop_protector_category != -1) {
                    return transmog_messages.select_transmog_target.c_str();
                }
            }
            break;

        case msgbnd_dialogues:
            if (msg_id == msg::dialogues_purchase_item_for_runes) {
                switch (active_transmog_shop_protector_category) {
                    case shop::protector_category_head:
                        return transmog_messages.transmog_head_to.c_str();
                    case shop::protector_category_chest:
                        return transmog_messages.transmog_chest_to.c_str();
                    case shop::protector_category_arms:
                        return transmog_messages.transmog_arms_to.c_str();
                    case shop::protector_category_legs:
                        return transmog_messages.transmog_legs_to.c_str();
                }
            }
            break;
    }

    return get_message(msg_repository, unknown, bnd_id, msg_id);
}

// Set a flag to adjust some UI strings for the transmog shop, but not other shops
void msg::set_active_transmog_shop_protector_category(signed char protector_category) {
    active_transmog_shop_protector_category = protector_category;
}

void msg::initialize() {
    auto msg_repository_address = modutils::scan<CS::MsgRepository *>({
        .aob = "48 8B 3D ?? ?? ?? ?? 44 0F B6 30 48 85 FF 75",
        .relative_offsets = {{3, 7}},
    });

    SPDLOG_INFO("Waiting for messages...");
    while (!(msg_repository = *msg_repository_address)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Hook MsgRepositoryImp::LookupEntry() to return message strings used by the mod
    modutils::hook(
        {
            .aob = "8B DA 44 8B CA 33 D2 48 8B F9 44 8D 42 6F",
            .offset = 14,
            .relative_offsets = {{1, 5}},
        },
        get_message_detour, get_message);

    // Pick the messages to use based on the player's selected language for the game in Steam
    std::string language = SteamApps()->GetCurrentGameLanguage();

    auto messages_iterator = messages_by_lang.find(language);
    if (messages_iterator == messages_by_lang.end()) {
        SPDLOG_WARN("Detected game language = {} (not supported, defaulting to English)", language);
        transmog_messages = messages_by_lang.at("english");
    } else {
        SPDLOG_INFO("Detected game language = {}", language);
        transmog_messages = messages_iterator->second;
    }
}

static std::wstring_view get_goods_name(int id) {
    auto result = get_message(msg_repository, 0, msgbnd_goods_name, id);
    if (result != nullptr) {
        return result;
    }

    result = get_message(msg_repository, 0, msgbnd_dlc_goods_name, id);
    if (result != nullptr) {
        return result;
    }

    return L"";
}

const std::pair<std::wstring_view, bool> msg::get_protector_data(int id) {
    auto result = get_message(msg_repository, 0, msgbnd_protector_name, id);
    if (result != nullptr) {
        return {result, false};
    }

    result = get_message(msg_repository, 0, msgbnd_dlc_protector_name, id);
    if (result != nullptr) {
        return {result, true};
    }

    return {L"", false};
}
