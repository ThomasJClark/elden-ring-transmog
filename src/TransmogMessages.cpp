#include <chrono>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>

#include "ModUtils.hpp"
#include "TransmogConfig.hpp"
#include "TransmogMessages.hpp"
#include "TransmogShop.hpp"
#include "messages.hpp"

using namespace TransmogMessages;
using namespace std;

static const uint32_t msgbnd_goods_name = 10;
static const uint32_t msgbnd_protector_name = 12;
static const uint32_t msgbnd_goods_info = 20;
static const uint32_t msgbnd_goods_caption = 24;
static const uint32_t msgbnd_protector_caption = 26;
static const uint32_t msgbnd_event_text_for_talk = 33;
static const uint32_t msgbnd_menu_text = 200;
static const uint32_t msgbnd_line_help = 201;
static const uint32_t msgbnd_system_message_win64 = 203;
static const uint32_t msgbnd_dialogues = 204;

struct ISteamApps;
extern "C" __declspec(dllimport) ISteamApps *__cdecl SteamAPI_SteamApps_v008();
extern "C" __declspec(dllimport) const
    char *__cdecl SteamAPI_ISteamApps_GetCurrentGameLanguage(ISteamApps *);

/**
 * Return the player's selected language using the Steamworks SDK
 *
 * https://partner.steamgames.com/doc/api/ISteamApps#GetCurrentGameLanguage
 */
static string get_steam_language()
{
    auto steam_api = SteamAPI_SteamApps_v008();
    auto steam_language = SteamAPI_ISteamApps_GetCurrentGameLanguage(steam_api);
    return steam_language != nullptr ? steam_language : "";
}

namespace CS
{
struct MsgRepository;
}
static CS::MsgRepository *msg_repository = nullptr;

/**
 * Assigned the list of localized messages based on the player's language preference
 */
static Messages transmog_messages;

/**
 * Set while the transmog menu is open to adjust some of the UI strings, or to -1 while a different
 * shop menu is open
 */
static int8_t active_transmog_shop_protector_category = -1;

static const wchar_t *(*get_message)(CS::MsgRepository *msg_repository, uint32_t unknown,
                                     uint32_t bnd_id, int32_t msg_id);

const wchar_t *get_message_detour(CS::MsgRepository *msg_repository, uint32_t unknown,
                                  uint32_t bnd_id, int32_t msg_id)
{
    switch (bnd_id)
    {
    case msgbnd_event_text_for_talk:
        if (TransmogConfig::patch_grace_talk_script)
        {
            switch (msg_id)
            {
            case EventTextForTalk::transmog_armor:
                return transmog_messages.transmog_armor.c_str();
            case EventTextForTalk::transmog_head:
                return transmog_messages.transmog_head.c_str();
            case EventTextForTalk::transmog_chest:
                return transmog_messages.transmog_chest.c_str();
            case EventTextForTalk::transmog_arms:
                return transmog_messages.transmog_arms.c_str();
            case EventTextForTalk::transmog_legs:
                return transmog_messages.transmog_legs.c_str();
            case EventTextForTalk::undo_transmog:
                return transmog_messages.undo_transmog.c_str();
            }
        }
        break;

    case msgbnd_menu_text:
        switch (msg_id)
        {
        case MenuText::transmog_head:
            return transmog_messages.transmog_head_title.c_str();
        case MenuText::transmog_chest:
            return transmog_messages.transmog_chest_title.c_str();
        case MenuText::transmog_arms:
            return transmog_messages.transmog_arms_title.c_str();
        case MenuText::transmog_legs:
            return transmog_messages.transmog_legs_title.c_str();
        }
        break;

    case msgbnd_goods_name: {
        auto protector_id = TransmogShop::get_protector_id_for_transmog_good(msg_id);
        if (protector_id > 0)
        {
            // Show bare head/chest/arms/legs as "Invisible"
            if (TransmogShop::is_invisible_protector_id(protector_id))
            {
                return transmog_messages.invisible.c_str();
            }

            auto protector_name = get_protector_name(protector_id);

            // Remove the "[ERROR]" prefix from cut items in the transmog shop
            if (protector_name.starts_with(cut_item_prefix))
            {
                protector_name = protector_name.substr(cut_item_prefix.size());
            }

            return protector_name.data();
        }
        break;
    }

    case msgbnd_goods_caption: {
        auto protector_id = TransmogShop::get_protector_id_for_transmog_good(msg_id);
        if (protector_id > 0)
        {
            return get_message(msg_repository, unknown, msgbnd_protector_caption, protector_id);
        }
        break;
    }

    case msgbnd_goods_info: {
        auto protector_id = TransmogShop::get_protector_id_for_transmog_good(msg_id);
        if (protector_id > 0)
        {
            return L"";
        }
        break;
    }

    case msgbnd_line_help:
        if (msg_id == LineHelp::select_item_for_purchase)
        {
            if (active_transmog_shop_protector_category != -1)
            {
                return transmog_messages.select_transmog_target.c_str();
            }
        }
        break;

    case msgbnd_dialogues:
        if (msg_id == Dialogues::purchase_item_for_runes)
        {
            switch (active_transmog_shop_protector_category)
            {
            case TransmogShop::protector_category_head:
                return transmog_messages.transmog_head_to.c_str();
            case TransmogShop::protector_category_chest:
                return transmog_messages.transmog_chest_to.c_str();
            case TransmogShop::protector_category_arms:
                return transmog_messages.transmog_arms_to.c_str();
            case TransmogShop::protector_category_legs:
                return transmog_messages.transmog_legs_to.c_str();
            }
        }
        break;
    }

    return get_message(msg_repository, unknown, bnd_id, msg_id);
}

// Set a flag to adjust some UI strings for the transmog shop, but not other shops
void TransmogMessages::set_active_transmog_shop_protector_category(int8_t protector_category)
{
    active_transmog_shop_protector_category = protector_category;
}

void TransmogMessages::initialize()
{
    auto msg_repository_address = ModUtils::scan<CS::MsgRepository *>({
        .aob = "48 8B 3D ?? ?? ?? ?? 44 0F B6 30 48 85 FF 75",
        .relative_offsets = {{3, 7}},
    });

    spdlog::info("Waiting for messages...");
    while (!(msg_repository = *msg_repository_address))
    {
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    // Hook MsgRepositoryImp::LookupEntry() to return message strings used by the mod
    ModUtils::hook(
        {
            .aob = "8B DA 44 8B CA 33 D2 48 8B F9 44 8D 42 6F",
            .offset = 14,
            .relative_offsets = {{1, 5}},
        },
        get_message_detour, get_message);

    // Pick the messages to use based on the player's selected language for the game in Steam
    auto language = get_steam_language();

    auto messages_iterator = transmog_messages_by_lang.find(language);
    if (messages_iterator == transmog_messages_by_lang.end())
    {
        spdlog::warn("Detected game language = {} (not supported, defaulting to English)",
                     language);
        transmog_messages = transmog_messages_by_lang.at("english");
    }
    else
    {
        spdlog::info("Detected game language = {}", language);
        transmog_messages = messages_iterator->second;
    }
}

const wstring_view TransmogMessages::get_protector_name(int32_t id)
{
    auto result = get_message(msg_repository, 0, msgbnd_protector_name, id);
    if (result == nullptr)
    {
        return L"";
    }
    return result;
}
