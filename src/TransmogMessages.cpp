#include <map>
#include <string>

#include "GameMemory.hpp"
#include "TransmogMessages.hpp"

using namespace TransmogMessages;

static const std::uint32_t msgbnd_talk_msg = 1;
static const std::uint32_t msgbnd_blood_msg = 2;
static const std::uint32_t msgbnd_movie_subtitle = 3;
static const std::uint32_t msgbnd_goods_name = 10;
static const std::uint32_t msgbnd_weapon_name = 11;
static const std::uint32_t msgbnd_protector_name = 12;
static const std::uint32_t msgbnd_accessory_name = 13;
static const std::uint32_t msgbnd_magic_name = 14;
static const std::uint32_t msgbnd_npc_name = 18;
static const std::uint32_t msgbnd_place_name = 19;
static const std::uint32_t msgbnd_goods_info = 20;
static const std::uint32_t msgbnd_weapon_info = 21;
static const std::uint32_t msgbnd_protector_info = 22;
static const std::uint32_t msgbnd_accessory_info = 23;
static const std::uint32_t msgbnd_goods_caption = 24;
static const std::uint32_t msgbnd_weapon_caption = 25;
static const std::uint32_t msgbnd_protector_caption = 26;
static const std::uint32_t msgbnd_accessory_caption = 27;
static const std::uint32_t msgbnd_magic_info = 28;
static const std::uint32_t msgbnd_magic_caption = 29;
static const std::uint32_t msgbnd_network_message = 31;
static const std::uint32_t msgbnd_action_button_text = 32;
static const std::uint32_t msgbnd_event_text_for_talk = 33;
static const std::uint32_t msgbnd_event_text_for_map = 34;
static const std::uint32_t msgbnd_gem_name = 35;
static const std::uint32_t msgbnd_gem_info = 36;
static const std::uint32_t msgbnd_gem_caption = 37;
static const std::uint32_t msgbnd_goods_dialog = 41;
static const std::uint32_t msgbnd_arts_name = 42;
static const std::uint32_t msgbnd_arts_caption = 43;
static const std::uint32_t msgbnd_weapon_effect = 44;
static const std::uint32_t msgbnd_gem_effect = 45;
static const std::uint32_t msgbnd_goods_info2 = 46;
static const std::uint32_t msgbnd_menu_text = 200;
static const std::uint32_t msgbnd_line_help = 201;
static const std::uint32_t msgbnd_key_guide = 202;
static const std::uint32_t msgbnd_system_message_win64 = 203;
static const std::uint32_t msgbnd_dialogues = 204;
static const std::uint32_t msgbnd_loading_title = 205;
static const std::uint32_t msgbnd_loading_text = 206;
static const std::uint32_t msgbnd_tutorial_title = 207;
static const std::uint32_t msgbnd_tutorial_body = 208;
static const std::uint32_t msgbnd_text_embed_image_name_win64 = 209;
static const std::uint32_t msgbnd_tos_win64 = 210;

typedef const wchar_t *GetMessageFn(MsgRepository *msg_repository, std::uint32_t unknown,
                                    std::uint32_t bnd_id, std::int32_t msg_id);

static GetMessageFn *get_message_hook;
static GetMessageFn *get_message;

struct Messages
{
    const wchar_t *event_text_for_talk_transmog_armor;
    const wchar_t *event_text_for_talk_transmog_head;
    const wchar_t *event_text_for_talk_transmog_body;
    const wchar_t *event_text_for_talk_transmog_arms;
    const wchar_t *event_text_for_talk_transmog_legs;
    const wchar_t *event_text_for_talk_undo_transmog;
    const wchar_t *event_text_for_map_undo_transmog_alert;
    const wchar_t *menu_text_transmog_head;
    const wchar_t *menu_text_transmog_body;
    const wchar_t *menu_text_transmog_arms;
    const wchar_t *menu_text_transmog_legs;
};

static Messages transmog_messages;

static const Messages transmog_messages_deude = {
    .event_text_for_talk_transmog_armor = L"Transmogrifiziere die rüstung",
    .event_text_for_talk_transmog_head = L"Transmogrifiziere den kopf",
    .event_text_for_talk_transmog_body = L"Transmogrifiziere den körper",
    .event_text_for_talk_transmog_arms = L"Transmogrifiziere die arme",
    .event_text_for_talk_transmog_legs = L"Transmogrifiziere die beine",
    .event_text_for_talk_undo_transmog = L"Machen Sie die Transmogrifizierung rückgängig",
    .event_text_for_map_undo_transmog_alert =
        L"Die nicht transmogrifizierte rüstung wurde in dein inventar zurückgegeben",
    .menu_text_transmog_head = L"Transmogrifiziere den Kopf",
    .menu_text_transmog_body = L"Transmogrifiziere den Körper",
    .menu_text_transmog_arms = L"Transmogrifiziere die Arme",
    .menu_text_transmog_legs = L"Transmogrifiziere die Beine",
};

static const Messages transmog_messages_engus = {
    .event_text_for_talk_transmog_armor = L"Transmogrify armor",
    .event_text_for_talk_transmog_head = L"Transmogrify head",
    .event_text_for_talk_transmog_body = L"Transmogrify body",
    .event_text_for_talk_transmog_arms = L"Transmogrify arms",
    .event_text_for_talk_transmog_legs = L"Transmogrify legs",
    .event_text_for_talk_undo_transmog = L"Untransmogrify equipped armor",
    .event_text_for_map_undo_transmog_alert =
        L"Untransmogrified armor has been returned to your inventory",
    .menu_text_transmog_head = L"Transmogrify Head",
    .menu_text_transmog_body = L"Transmogrify Body",
    .menu_text_transmog_arms = L"Transmogrify Arms",
    .menu_text_transmog_legs = L"Transmogrify Legs",
};

static const Messages transmog_messages_frafr = {
    .event_text_for_talk_transmog_armor = L"Transmogrifier l'armure",
    .event_text_for_talk_transmog_head = L"Transmogrifier la tête",
    .event_text_for_talk_transmog_body = L"Transmogrifier le corps",
    .event_text_for_talk_transmog_arms = L"Transmogrifier les bras",
    .event_text_for_talk_transmog_legs = L"Transmogrifier les jambes",
    .event_text_for_talk_undo_transmog =
        L"Annuler la transmogrification de l'armure équipée", // TODO fix
    .event_text_for_map_undo_transmog_alert =
        L"L'armure non transmogrifiée a, été retournée dans votre inventaire",
    .menu_text_transmog_head = L"Transmogrifier la Tête",
    .menu_text_transmog_body = L"Transmogrifier le Corps",
    .menu_text_transmog_arms = L"Transmogrifier les Bras",
    .menu_text_transmog_legs = L"Transmogrifier les Jambes",
};

static const Messages transmog_messages_itait = {
    .event_text_for_talk_transmog_armor = L"Trasmogrifica l'armatura",
    .event_text_for_talk_transmog_head = L"Trasmogrifica la testa",
    .event_text_for_talk_transmog_body = L"Trasmogrifica il corpo",
    .event_text_for_talk_transmog_arms = L"Trasmogrifica le braccia",
    .event_text_for_talk_transmog_legs = L"Trasmogrifica le gambe",
    .event_text_for_talk_undo_transmog = L"Disfai la trasmogrificazione dell'armatura equipaggiata",
    .event_text_for_map_undo_transmog_alert =
        L"L'armatura non trasmogrificata è stata restituita al tuo inventario",
    .menu_text_transmog_head = L"Trasmogrifica la Testa",
    .menu_text_transmog_body = L"Trasmogrifica il Corpo",
    .menu_text_transmog_arms = L"Trasmogrifica le Braccia",
    .menu_text_transmog_legs = L"Trasmogrifica le Gambe",
};

static const Messages transmog_messages_polpl = {
    .event_text_for_talk_transmog_armor = L"Transmogryfikuj zbroje",
    .event_text_for_talk_transmog_head = L"Transmogryfikuj głowe",
    .event_text_for_talk_transmog_body = L"Transmogryfikuj ciało",
    .event_text_for_talk_transmog_arms = L"Transmogryfikuj ramiona",
    .event_text_for_talk_transmog_legs = L"Transmogryfikuj nogi",
    .event_text_for_talk_undo_transmog = L"Odtransmogryfikuj założoną zbroje",
    .event_text_for_map_undo_transmog_alert =
        L"Odtransmogryfikowana zbroja wróciła do twojego ekwipunku",
    .menu_text_transmog_head = L"Transmogryfikuj Głowe",
    .menu_text_transmog_body = L"Transmogryfikuj Ciało",
    .menu_text_transmog_arms = L"Transmogryfikuj Ramiona",
    .menu_text_transmog_legs = L"Transmogryfikuj Nogi",
};

static const Messages transmog_messages_porbr = {
    .event_text_for_talk_transmog_armor = L"Transmogrifique a armadura",
    .event_text_for_talk_transmog_head = L"Transmogrifique a cabeça",
    .event_text_for_talk_transmog_body = L"Transmogrifique o corpo",
    .event_text_for_talk_transmog_arms = L"Transmogrifique os braços",
    .event_text_for_talk_transmog_legs = L"Transmogrifique as pernas",
    .event_text_for_talk_undo_transmog = L"Desfazer a transmogrificação da armadura equipada",
    .event_text_for_map_undo_transmog_alert =
        L"A armadura não transmogrificada foi devolvida ao seu inventário",
    .menu_text_transmog_head = L"Transmogrifique a Cabeça",
    .menu_text_transmog_body = L"Transmogrifique o Corpo",
    .menu_text_transmog_arms = L"Transmogrifique os Braços",
    .menu_text_transmog_legs = L"Transmogrifique as Pernas",
};

static const Messages transmog_messages_spaar = {
    .event_text_for_talk_transmog_armor = L"Transmogrificar la armadura",
    .event_text_for_talk_transmog_head = L"Transmogrificar la cabeza",
    .event_text_for_talk_transmog_body = L"Transmogrificar el cuerpo",
    .event_text_for_talk_transmog_arms = L"Transmogrificar los brazos",
    .event_text_for_talk_transmog_legs = L"Transmogrificar las piernas",
    .event_text_for_talk_undo_transmog = L"Deshacer la transmogrificación",
    .event_text_for_map_undo_transmog_alert =
        L"La armadura no transmogrificada ha sido devuelta a tu inventario",
    .menu_text_transmog_head = L"Transmogrificar la Cabeza",
    .menu_text_transmog_body = L"Transmogrificar el Cuerpo",
    .menu_text_transmog_arms = L"Transmogrificar los Brazos",
    .menu_text_transmog_legs = L"Transmogrificar las Piernas",
};

/**
 * Detect the player's selected language based on the "Controller Vibration" string. There
 * might be a way to do this through the Steam API, but this works.
 */
static Messages const &autodetect_transmog_messages(MsgRepository *msg_repository)
{
    auto controller_vibration = get_message(msg_repository, 0, msgbnd_system_message_win64, 110010);
    if (controller_vibration == std::wstring(L"Controller-Vibration"))
    {
        return transmog_messages_deude;
    }
    else if (controller_vibration == std::wstring(L"Vibrations de la manette"))
    {
        return transmog_messages_frafr;
    }
    else if (controller_vibration == std::wstring(L"Vibrazione del controller"))
    {
        return transmog_messages_itait;
    }
    else if (controller_vibration == std::wstring(L"Wibracje kontrolera"))
    {
        return transmog_messages_polpl;
    }
    else if (controller_vibration == std::wstring(L"Vibração do controle"))
    {
        return transmog_messages_porbr;
    }
    else if (controller_vibration == std::wstring(L"Vibración del control") ||
             controller_vibration == std::wstring(L"Vibración del mando"))
    {
        return transmog_messages_spaar;
    }
    else
    {
        return transmog_messages_engus;
    }
}

const wchar_t *get_message_detour(MsgRepository *msg_repository, std::uint32_t unknown,
                                  std::uint32_t bnd_id, std::int32_t msg_id)
{
    if (bnd_id == msgbnd_event_text_for_talk)
    {
        if (msg_id == event_text_for_talk_transmog_armor_id)
        {
            return transmog_messages.event_text_for_talk_transmog_armor;
        }
        else if (msg_id == event_text_for_talk_transmog_head_id)
        {
            return transmog_messages.event_text_for_talk_transmog_head;
        }
        else if (msg_id == event_text_for_talk_transmog_body_id)
        {
            return transmog_messages.event_text_for_talk_transmog_body;
        }
        else if (msg_id == event_text_for_talk_transmog_arms_id)
        {
            return transmog_messages.event_text_for_talk_transmog_arms;
        }
        else if (msg_id == event_text_for_talk_transmog_legs_id)
        {
            return transmog_messages.event_text_for_talk_transmog_legs;
        }
        else if (msg_id == event_text_for_talk_undo_transmog_id)
        {
            return transmog_messages.event_text_for_talk_undo_transmog;
        }
    }
    else if (bnd_id == msgbnd_event_text_for_map)
    {
        if (msg_id == event_text_for_map_undo_transmog_alert_id)
        {
            return transmog_messages.event_text_for_map_undo_transmog_alert;
        }
    }
    else if (bnd_id == msgbnd_menu_text)
    {
        if (msg_id == menu_text_transmog_head_id)
        {
            return transmog_messages.menu_text_transmog_head;
        }
        else if (msg_id == menu_text_transmog_body_id)
        {
            return transmog_messages.menu_text_transmog_body;
        }
        else if (msg_id == menu_text_transmog_arms_id)
        {
            return transmog_messages.menu_text_transmog_arms;
        }
        else if (msg_id == menu_text_transmog_legs_id)
        {
            return transmog_messages.menu_text_transmog_legs;
        }
    }

    return get_message(msg_repository, unknown, bnd_id, msg_id);
}

void TransmogMessages::hook(GameMemory &game_memory, MsgRepository *msg_repository)
{
    get_message_hook = game_memory.hook<>(
        {
            .aob = {0x8B, 0xDA, 0x44, 0x8B, 0xCA, 0x33, 0xD2, 0x48, 0x8B, 0xF9, 0x44, 0x8D, 0x42,
                    0x6F},
            .offset = 0xe,
            .relative_offsets = {{0x1, 0x5}},
        },
        get_message_detour, get_message);

    transmog_messages = autodetect_transmog_messages(msg_repository);
}

void TransmogMessages::unhook(GameMemory &game_memory)
{
}
