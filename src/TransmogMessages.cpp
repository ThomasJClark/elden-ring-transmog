#include <iostream>
#include <string>

#include "ModUtils.hpp"
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
static const uint32_t msgbnd_event_text_for_map = 34;
static const uint32_t msgbnd_menu_text = 200;
static const uint32_t msgbnd_system_message_win64 = 203;

struct ISteamApps;
extern "C" __declspec(dllimport) ISteamApps *__cdecl SteamAPI_SteamApps_v008();
extern "C" __declspec(dllimport) const
    char *__cdecl SteamAPI_ISteamApps_GetCurrentGameLanguage(ISteamApps *);

/**
 * Return the player's selected language using the Steamworks SDK
 *
 * https://partner.steamgames.com/doc/store/localization/languages
 */
static string get_steam_language()
{
    auto steam_api = SteamAPI_SteamApps_v008();
    auto steam_language = SteamAPI_ISteamApps_GetCurrentGameLanguage(steam_api);
    return steam_language != nullptr ? steam_language : "";
}

typedef const char16_t *GetMessageFn(MsgRepository *msg_repository, uint32_t unknown,
                                     uint32_t bnd_id, int32_t msg_id);

static GetMessageFn *get_message_hook;
static GetMessageFn *get_message;

static Messages transmog_messages;

const char16_t *get_message_detour(MsgRepository *msg_repository, uint32_t unknown, uint32_t bnd_id,
                                   int32_t msg_id)
{
    switch (bnd_id)
    {
    case msgbnd_event_text_for_talk:
        switch (msg_id)
        {
        case event_text_for_talk_transmog_armor_id:
            return transmog_messages.event_text_for_talk_transmog_armor.c_str();
        case event_text_for_talk_transmog_head_id:
            return transmog_messages.event_text_for_talk_transmog_head.c_str();
        case event_text_for_talk_transmog_body_id:
            return transmog_messages.event_text_for_talk_transmog_body.c_str();
        case event_text_for_talk_transmog_arms_id:
            return transmog_messages.event_text_for_talk_transmog_arms.c_str();
        case event_text_for_talk_transmog_legs_id:
            return transmog_messages.event_text_for_talk_transmog_legs.c_str();
        case event_text_for_talk_undo_transmog_id:
            return transmog_messages.event_text_for_talk_undo_transmog.c_str();
        }
        break;

    case msgbnd_menu_text:
        switch (msg_id)
        {
        case menu_text_transmog_head_id:
            return transmog_messages.menu_text_transmog_head.c_str();
        case menu_text_transmog_body_id:
            return transmog_messages.menu_text_transmog_body.c_str();
        case menu_text_transmog_arms_id:
            return transmog_messages.menu_text_transmog_arms.c_str();
        case menu_text_transmog_legs_id:
            return transmog_messages.menu_text_transmog_legs.c_str();
        }
        break;

    case msgbnd_event_text_for_map:
        if (msg_id == event_text_for_map_undo_transmog_alert_id)
        {
            return transmog_messages.event_text_for_map_undo_transmog_alert.c_str();
        }
        break;

    case msgbnd_goods_name: {
        auto protector_id = TransmogShop::get_protector_id_for_transmog_good(msg_id);
        if (protector_id > 0)
        {
            if (TransmogShop::is_invisible_protector_id(protector_id))
            {
                return transmog_messages.protector_name_invisible.c_str();
            }
            else
            {
                return get_message(msg_repository, unknown, msgbnd_protector_name, protector_id);
            }
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
            return u"";
        }
        break;
    }
    }

    return get_message(msg_repository, unknown, bnd_id, msg_id);
}

void TransmogMessages::initialize(MsgRepository *msg_repository)
{
    // Hook MsgRepositoryImp::LookupEntry() to return message strings used by the mod
    get_message_hook = ModUtils::hook(
        {
            .aob = "8B DA 44 8B CA 33 D2 48 8B F9 44 8D 42 6F",
            .offset = 0xe,
            .relative_offsets = {{0x1, 0x5}},
        },
        get_message_detour, get_message);

    // Pick the messages to use based on the player's selected language for the game in Steam
    auto language = get_steam_language();

    auto messages_iterator = transmog_messages_by_lang.find(language);
    if (messages_iterator == transmog_messages_by_lang.end())
    {
        cout << "Detected game language = " << language << " (not supported)" << endl;
        transmog_messages = transmog_messages_by_lang.at("english");
    }
    else
    {
        cout << "Detected game language = " << language << endl;
        transmog_messages = messages_iterator->second;
    }

    // For Elden Ring Reforged, add icons to match other menu text
    u16string_view calibrations_ver = get_message_hook(msg_repository, 0, msgbnd_menu_text, 401322);
    if (calibrations_ver.find(u"ELDEN RING Reforged") != string::npos)
    {
        cout << "Detected ELDEN RING Reforged - enabling menu icons" << endl;

        auto prepend_icon = [](u16string &str, u16string const &icon) {
            str = u"<img src='img://" + icon + u"' height='32' width='32' vspace='-16'/> " + str;
        };

        prepend_icon(transmog_messages.event_text_for_talk_transmog_armor,
                     u"SB_ERR_Grace_AlterGarments.png");
        prepend_icon(transmog_messages.event_text_for_talk_transmog_head, u"SB_ERR_A_Mind");
        prepend_icon(transmog_messages.event_text_for_talk_transmog_body, u"SB_ERR_A_Vigor");
        prepend_icon(transmog_messages.event_text_for_talk_transmog_arms, u"SB_ERR_A_Strength");
        prepend_icon(transmog_messages.event_text_for_talk_transmog_legs, u"SB_ERR_A_Endurance");
        prepend_icon(transmog_messages.event_text_for_talk_undo_transmog,
                     u"SB_ERR_Grace_AlterGarments.png");
    }
}

const u16string_view TransmogMessages::get_protector_name(MsgRepository *msg_repository, int32_t id)
{
    auto result = get_message(msg_repository, 0, msgbnd_protector_name, id);
    if (result == nullptr)
    {
        return u"";
    }
    return result;
}

void TransmogMessages::deinitialize()
{
    ModUtils::unhook(get_message_hook);
}
