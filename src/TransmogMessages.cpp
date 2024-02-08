#include <algorithm>
#include <cwchar>
#include <map>
#include <string>

#include "TransmogMessages.hpp"
#include "TransmogShop.hpp"
#include "TransmogUtils.hpp"
#include "messages.hpp"

using namespace TransmogMessages;

static const std::uint32_t msgbnd_goods_name = 10;
static const std::uint32_t msgbnd_protector_name = 12;
static const std::uint32_t msgbnd_goods_info = 20;
static const std::uint32_t msgbnd_goods_caption = 24;
static const std::uint32_t msgbnd_protector_caption = 26;
static const std::uint32_t msgbnd_event_text_for_talk = 33;
static const std::uint32_t msgbnd_event_text_for_map = 34;
static const std::uint32_t msgbnd_menu_text = 200;
static const std::uint32_t msgbnd_system_message_win64 = 203;

typedef const char16_t *GetMessageFn(MsgRepository *msg_repository, std::uint32_t unknown,
                                     std::uint32_t bnd_id, std::int32_t msg_id);

static GetMessageFn *get_message_hook;
static GetMessageFn *get_message;

static Messages transmog_messages;

const char16_t *get_message_detour(MsgRepository *msg_repository, std::uint32_t unknown,
                                   std::uint32_t bnd_id, std::int32_t msg_id)
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
    // Determine the player's language preference and retreive the corresponding messages for
    // the mod
    std::u16string_view controller_vibration =
        get_message(msg_repository, 0, msgbnd_system_message_win64, 110010);
    auto messages = std::find_if(transmog_messages_by_lang.begin(), transmog_messages_by_lang.end(),
                                 [controller_vibration](Messages const &messages) {
                                     return controller_vibration ==
                                            messages.system_message_controller_vibration;
                                 });

    if (messages != transmog_messages_by_lang.end())
    {
        transmog_messages = *messages;
    }
    else
    {
        transmog_messages = transmog_messages_engus;
    }

    get_message_hook = TransmogUtils::hook(
        {
            .aob = {0x8B, 0xDA, 0x44, 0x8B, 0xCA, 0x33, 0xD2, 0x48, 0x8B, 0xF9, 0x44, 0x8D, 0x42,
                    0x6F},
            .offset = 0xe,
            .relative_offsets = {{0x1, 0x5}},
        },
        get_message_detour, get_message);
}

const std::u16string_view TransmogMessages::get_protector_name(MsgRepository *msg_repository,
                                                               std::int32_t id)
{
    auto result = get_message_hook(msg_repository, 0, msgbnd_protector_name, id);
    if (result == nullptr)
    {
        return u"";
    }
    return result;
}

void TransmogMessages::deinitialize()
{
    TransmogUtils::unhook(get_message_hook);
}
