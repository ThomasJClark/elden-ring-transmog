#pragma once
#include <cstdint>
#include <map>
#include <string>

#include "MsgRepository.hpp"

struct TransmogMessages
{
    const wchar_t *transmogrify_armor_menu_item;
    const wchar_t *transmogrify_head_menu_item;
    const wchar_t *transmogrify_body_menu_item;
    const wchar_t *transmogrify_arms_menu_item;
    const wchar_t *transmogrify_legs_menu_item;
    const wchar_t *untransmogrify_menu_item;
    const wchar_t *transmogrify_head_title;
    const wchar_t *transmogrify_body_title;
    const wchar_t *transmogrify_arms_title;
    const wchar_t *transmogrify_legs_title;
    const wchar_t *untransmogrify_alert;
    const wchar_t *invisible_protector;
};

struct TransmogMessagesByLang
{
    const TransmogMessages deude;
    const TransmogMessages engus;
    const TransmogMessages frafr;
    const TransmogMessages itait;
    const TransmogMessages polpl;
    const TransmogMessages porbr;
    const TransmogMessages spaar;
};

static const TransmogMessagesByLang transmog_messages_by_lang = {
    .deude = {.transmogrify_armor_menu_item = L"Transmogrifiziere die rüstung",
              .transmogrify_head_menu_item = L"Transmogrifiziere den kopf",
              .transmogrify_body_menu_item = L"Transmogrifiziere den körper",
              .transmogrify_arms_menu_item = L"Transmogrifiziere die arme",
              .transmogrify_legs_menu_item = L"Transmogrifiziere die beine",
              .untransmogrify_menu_item = L"Machen Sie die Transmogrifizierung rückgängig",
              .transmogrify_head_title = L"Transmogrifiziere den Kopf",
              .transmogrify_body_title = L"Transmogrifiziere den Körper",
              .transmogrify_arms_title = L"Transmogrifiziere die Arme",
              .transmogrify_legs_title = L"Transmogrifiziere die Beine",
              .untransmogrify_alert =
                  L"Die nicht transmogrifizierte rüstung wurde in dein inventar zurückgegeben",
              .invisible_protector = L"Unsichtbar"},
    .engus = {.transmogrify_armor_menu_item = L"Transmogrify armor",
              .transmogrify_head_menu_item = L"Transmogrify head",
              .transmogrify_body_menu_item = L"Transmogrify body",
              .transmogrify_arms_menu_item = L"Transmogrify arms",
              .transmogrify_legs_menu_item = L"Transmogrify legs",
              .untransmogrify_menu_item = L"Untransmogrify equipped armor",
              .transmogrify_head_title = L"Transmogrify Head",
              .transmogrify_body_title = L"Transmogrify Body",
              .transmogrify_arms_title = L"Transmogrify Arms",
              .transmogrify_legs_title = L"Transmogrify Legs",
              .untransmogrify_alert = L"Untransmogrified armor has been returned to your inventory",
              .invisible_protector = L"Invisible"},
    .frafr = {.transmogrify_armor_menu_item = L"Transmogrifier l'armure",
              .transmogrify_head_menu_item = L"Transmogrifier la tête",
              .transmogrify_body_menu_item = L"Transmogrifier le corps",
              .transmogrify_arms_menu_item = L"Transmogrifier les bras",
              .transmogrify_legs_menu_item = L"Transmogrifier les jambes",
              .untransmogrify_menu_item =
                  L"Annuler la transmogrification de l'armure équipée", // TODO fix
              .transmogrify_head_title = L"Transmogrifier la Tête",
              .transmogrify_body_title = L"Transmogrifier le Corps",
              .transmogrify_arms_title = L"Transmogrifier les Bras",
              .transmogrify_legs_title = L"Transmogrifier les Jambes",
              .untransmogrify_alert =
                  L"L'armure non transmogrifiée a, été retournée dans votre inventaire",
              .invisible_protector = L"Invisible"},
    .itait = {.transmogrify_armor_menu_item = L"Trasmogrifica l'armatura",
              .transmogrify_head_menu_item = L"Trasmogrifica la testa",
              .transmogrify_body_menu_item = L"Trasmogrifica il corpo",
              .transmogrify_arms_menu_item = L"Trasmogrifica le braccia",
              .transmogrify_legs_menu_item = L"Trasmogrifica le gambe",
              .untransmogrify_menu_item =
                  L"Disfai la trasmogrificazione dell'armatura equipaggiata",
              .transmogrify_head_title = L"Trasmogrifica la Testa",
              .transmogrify_body_title = L"Trasmogrifica il Corpo",
              .transmogrify_arms_title = L"Trasmogrifica le Braccia",
              .transmogrify_legs_title = L"Trasmogrifica le Gambe",
              .untransmogrify_alert =
                  L"L'armatura non trasmogrificata è stata restituita al tuo inventario",
              .invisible_protector = L"Invisibile"},
    .polpl = {.transmogrify_armor_menu_item = L"Transmogryfikuj zbroje",
              .transmogrify_head_menu_item = L"Transmogryfikuj głowe",
              .transmogrify_body_menu_item = L"Transmogryfikuj ciało",
              .transmogrify_arms_menu_item = L"Transmogryfikuj ramiona",
              .transmogrify_legs_menu_item = L"Transmogryfikuj nogi",
              .untransmogrify_menu_item = L"Odtransmogryfikuj założoną zbroje",
              .transmogrify_head_title = L"Transmogryfikuj Głowe",
              .transmogrify_body_title = L"Transmogryfikuj Ciało",
              .transmogrify_arms_title = L"Transmogryfikuj Ramiona",
              .transmogrify_legs_title = L"Transmogryfikuj Nogi",
              .untransmogrify_alert = L"Odtransmogryfikowana zbroja wróciła do twojego ekwipunku",
              .invisible_protector = L"Niewidzialny"},
    .porbr = {.transmogrify_armor_menu_item = L"Transmogrifique a armadura",
              .transmogrify_head_menu_item = L"Transmogrifique a cabeça",
              .transmogrify_body_menu_item = L"Transmogrifique o corpo",
              .transmogrify_arms_menu_item = L"Transmogrifique os braços",
              .transmogrify_legs_menu_item = L"Transmogrifique as pernas",
              .untransmogrify_menu_item = L"Desfazer a transmogrificação da armadura equipada",
              .transmogrify_head_title = L"Transmogrifique a Cabeça",
              .transmogrify_body_title = L"Transmogrifique o Corpo",
              .transmogrify_arms_title = L"Transmogrifique os Braços",
              .transmogrify_legs_title = L"Transmogrifique as Pernas",
              .untransmogrify_alert =
                  L"A armadura não transmogrificada foi devolvida ao seu inventário",
              .invisible_protector = L"Invisível"},
    .spaar = {.transmogrify_armor_menu_item = L"Transmogrificar la armadura",
              .transmogrify_head_menu_item = L"Transmogrificar la cabeza",
              .transmogrify_body_menu_item = L"Transmogrificar el cuerpo",
              .transmogrify_arms_menu_item = L"Transmogrificar los brazos",
              .transmogrify_legs_menu_item = L"Transmogrificar las piernas",
              .untransmogrify_menu_item = L"Deshacer la transmogrificación",
              .transmogrify_head_title = L"Transmogrificar la Cabeza",
              .transmogrify_body_title = L"Transmogrificar el Cuerpo",
              .transmogrify_arms_title = L"Transmogrificar los Brazos",
              .transmogrify_legs_title = L"Transmogrificar las Piernas",
              .untransmogrify_alert =
                  L"La armadura no transmogrificada ha sido devuelta a tu inventario",
              .invisible_protector = L"Invisible"},
};

GetMessageFn *get_message;
GetMessageFn *get_message_trampoline;

TransmogMessages transmog_messages = transmog_messages_by_lang.engus;

/**
 * Figure out the current language based on a message that's probably not changed by any mods
 */
void setup_transmog_messages(MsgRepository *msg_repository)
{
    auto controller_vibration =
        get_message_trampoline(msg_repository, 0, MsgBndId::system_message_win64, 110010);

    if (controller_vibration == std::wstring(L"Controller-Vibration"))
    {
        transmog_messages = transmog_messages_by_lang.deude;
    }
    else if (controller_vibration == std::wstring(L"Vibrations de la manette"))
    {
        transmog_messages = transmog_messages_by_lang.frafr;
    }
    else if (controller_vibration == std::wstring(L"Vibrazione del controller"))
    {
        transmog_messages = transmog_messages_by_lang.itait;
    }
    else if (controller_vibration == std::wstring(L"Wibracje kontrolera"))
    {
        transmog_messages = transmog_messages_by_lang.polpl;
    }
    else if (controller_vibration == std::wstring(L"Vibração do controle"))
    {
        transmog_messages = transmog_messages_by_lang.porbr;
    }
    else if (controller_vibration == std::wstring(L"Vibración del control") ||
             controller_vibration == std::wstring(L"Vibración del mando"))
    {
        transmog_messages = transmog_messages_by_lang.spaar;
    }
}

/**
 * Override the internal get_message() function to return transmog-specific messages
 */
const wchar_t *get_message_detour(MsgRepository *msg_repository, uint32_t unknown, MsgBndId bnd_id,
                                  std::int32_t msg_id)
{
    if (bnd_id == MsgBndId::event_text_for_talk)
    {
        if (msg_id == 69000000)
            return transmog_messages.transmogrify_armor_menu_item;
        else if (msg_id == 69000010)
            return transmog_messages.transmogrify_head_menu_item;
        else if (msg_id == 69000011)
            return transmog_messages.transmogrify_body_menu_item;
        else if (msg_id == 69000012)
            return transmog_messages.transmogrify_arms_menu_item;
        else if (msg_id == 69000013)
            return transmog_messages.transmogrify_legs_menu_item;
        else if (msg_id == 69000020)
            return transmog_messages.untransmogrify_menu_item;
    }
    else if (bnd_id == MsgBndId::menu_text)
    {
        if (msg_id == 690000)
            return transmog_messages.transmogrify_head_title;
        else if (msg_id == 690001)
            return transmog_messages.transmogrify_body_title;
        else if (msg_id == 690002)
            return transmog_messages.transmogrify_arms_title;
        else if (msg_id == 690003)
            return transmog_messages.transmogrify_legs_title;
    }
    else if (bnd_id == MsgBndId::event_text_for_map)
    {
        if (msg_id == 690000)
            return transmog_messages.untransmogrify_alert;
    }

    return get_message_trampoline(msg_repository, unknown, bnd_id, msg_id);
}
