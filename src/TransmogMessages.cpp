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
 * https://partner.steamgames.com/doc/store/localization/languages
 */
static string get_steam_language()
{
    auto steam_api = SteamAPI_SteamApps_v008();
    auto steam_language = SteamAPI_ISteamApps_GetCurrentGameLanguage(steam_api);
    return steam_language != nullptr ? steam_language : "";
}

/**
 * Assigned the list of localized messages based on the player's language preference
 */
static Messages transmog_messages;

/**
 * Set while the transmog menu is open to adjust some of the UI strings, or to -1 while a different
 * shop menu is open
 */
static int8_t active_transmog_shop_protector_category = -1;

typedef const char16_t *GetMessageFn(MsgRepository *msg_repository, uint32_t unknown,
                                     uint32_t bnd_id, int32_t msg_id);

static GetMessageFn *get_message_hook;
static GetMessageFn *get_message;

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

    case msgbnd_line_help:
        if (msg_id == line_help_select_item_for_purchase_id)
        {
            if (active_transmog_shop_protector_category != -1)
            {
                return transmog_messages.line_help_select_transmog_target.c_str();
            }
        }
        break;

    case msgbnd_dialogues:
        if (msg_id == dialogues_purchase_item_for_runes)
        {
            switch (active_transmog_shop_protector_category)
            {
            case TransmogShop::protector_category_head:
                return transmog_messages.dialogues_transmog_head_prompt.c_str();
            case TransmogShop::protector_category_body:
                return transmog_messages.dialogues_transmog_body_prompt.c_str();
            case TransmogShop::protector_category_arms:
                return transmog_messages.dialogues_transmog_arms_prompt.c_str();
            case TransmogShop::protector_category_legs:
                return transmog_messages.dialogues_transmog_legs_prompt.c_str();
            }
        }
        break;
    }

    return get_message(msg_repository, unknown, bnd_id, msg_id);
}

typedef void OpenRegularShopFn(void *, uint64_t, uint64_t);

static OpenRegularShopFn *open_regular_shop_hook;
static OpenRegularShopFn *open_regular_shop;

static void open_regular_shop_detour(void *unk, uint64_t begin_id, uint64_t end_id)
{
    switch (begin_id)
    {
    case TransmogShop::transmog_head_shop_menu_id:
        active_transmog_shop_protector_category = TransmogShop::protector_category_head;
        break;
    case TransmogShop::transmog_body_shop_menu_id:
        active_transmog_shop_protector_category = TransmogShop::protector_category_body;
        break;
    case TransmogShop::transmog_arms_shop_menu_id:
        active_transmog_shop_protector_category = TransmogShop::protector_category_arms;
        break;
    case TransmogShop::transmog_legs_shop_menu_id:
        active_transmog_shop_protector_category = TransmogShop::protector_category_legs;
        break;
    default:
        active_transmog_shop_protector_category = -1;
    }

    open_regular_shop(unk, begin_id, end_id);
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

    // Hook OpenRegularShop() to adjust some UI strings for the transmog shop, but not other
    // shops
    open_regular_shop_hook = ModUtils::hook(
        {
            .offset = 0xe5e350,
        },
        open_regular_shop_detour, open_regular_shop);

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
    ModUtils::unhook(open_regular_shop_hook);
}
