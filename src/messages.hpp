#pragma once

#include <map>
#include <string>

struct Messages
{
    std::u16string event_text_for_talk_transmog_armor;
    std::u16string event_text_for_talk_transmog_head;
    std::u16string event_text_for_talk_transmog_body;
    std::u16string event_text_for_talk_transmog_arms;
    std::u16string event_text_for_talk_transmog_legs;
    std::u16string event_text_for_talk_undo_transmog;
    std::u16string event_text_for_map_undo_transmog_alert;
    std::u16string menu_text_transmog_head;
    std::u16string menu_text_transmog_body;
    std::u16string menu_text_transmog_arms;
    std::u16string menu_text_transmog_legs;
    std::u16string protector_name_invisible;
};

static const std::map<std::string, Messages> transmog_messages_by_lang = {
    {"english",
     {
         .event_text_for_talk_transmog_armor = u"Transmogrify armor",
         .event_text_for_talk_transmog_head = u"Transmogrify head",
         .event_text_for_talk_transmog_body = u"Transmogrify body",
         .event_text_for_talk_transmog_arms = u"Transmogrify arms",
         .event_text_for_talk_transmog_legs = u"Transmogrify legs",
         .event_text_for_talk_undo_transmog = u"Untransmogrify equipped armor",
         .event_text_for_map_undo_transmog_alert =
             u"Untransmogrified armor has been returned to your inventory",
         .menu_text_transmog_head = u"Transmogrify Head",
         .menu_text_transmog_body = u"Transmogrify Body",
         .menu_text_transmog_arms = u"Transmogrify Arms",
         .menu_text_transmog_legs = u"Transmogrify Legs",
         .protector_name_invisible = u"Invisible",
     }},
    {"german",
     {
         .event_text_for_talk_transmog_armor = u"Transmogrifiziere die rüstung",
         .event_text_for_talk_transmog_head = u"Transmogrifiziere den kopf",
         .event_text_for_talk_transmog_body = u"Transmogrifiziere den körper",
         .event_text_for_talk_transmog_arms = u"Transmogrifiziere die arme",
         .event_text_for_talk_transmog_legs = u"Transmogrifiziere die beine",
         .event_text_for_talk_undo_transmog = u"Machen Sie die Transmogrifizierung rückgängig",
         .event_text_for_map_undo_transmog_alert =
             u"Die nicht transmogrifizierte rüstung wurde in dein inventar zurückgegeben",
         .menu_text_transmog_head = u"Transmogrifiziere den Kopf",
         .menu_text_transmog_body = u"Transmogrifiziere den Körper",
         .menu_text_transmog_arms = u"Transmogrifiziere die Arme",
         .menu_text_transmog_legs = u"Transmogrifiziere die Beine",
         .protector_name_invisible = u"Unsichtbar",
     }},
    {"french",
     {
         .event_text_for_talk_transmog_armor = u"Transmogrifier l'armure",
         .event_text_for_talk_transmog_head = u"Transmogrifier la tête",
         .event_text_for_talk_transmog_body = u"Transmogrifier le corps",
         .event_text_for_talk_transmog_arms = u"Transmogrifier les bras",
         .event_text_for_talk_transmog_legs = u"Transmogrifier les jambes",
         .event_text_for_talk_undo_transmog = u"Annuler la transmogrification de l'armure équipée",
         .event_text_for_map_undo_transmog_alert =
             u"L'armure non transmogrifiée a, été retournée dans votre inventaire",
         .menu_text_transmog_head = u"Transmogrifier la Tête",
         .menu_text_transmog_body = u"Transmogrifier le Corps",
         .menu_text_transmog_arms = u"Transmogrifier les Bras",
         .menu_text_transmog_legs = u"Transmogrifier les Jambes",
         .protector_name_invisible = u"Invisible",
     }},
    {"italian",
     {
         .event_text_for_talk_transmog_armor = u"Trasmogrifica l'armatura",
         .event_text_for_talk_transmog_head = u"Trasmogrifica la testa",
         .event_text_for_talk_transmog_body = u"Trasmogrifica il corpo",
         .event_text_for_talk_transmog_arms = u"Trasmogrifica le braccia",
         .event_text_for_talk_transmog_legs = u"Trasmogrifica le gambe",
         .event_text_for_talk_undo_transmog =
             u"Disfai la trasmogrificazione dell'armatura equipaggiata",
         .event_text_for_map_undo_transmog_alert =
             u"L'armatura non trasmogrificata è stata restituita al tuo inventario",
         .menu_text_transmog_head = u"Trasmogrifica la Testa",
         .menu_text_transmog_body = u"Trasmogrifica il Corpo",
         .menu_text_transmog_arms = u"Trasmogrifica le Braccia",
         .menu_text_transmog_legs = u"Trasmogrifica le Gambe",
         .protector_name_invisible = u"Invisibile",
     }},
    {"polish",
     {
         .event_text_for_talk_transmog_armor = u"Transmogryfikuj zbroje",
         .event_text_for_talk_transmog_head = u"Transmogryfikuj głowe",
         .event_text_for_talk_transmog_body = u"Transmogryfikuj ciało",
         .event_text_for_talk_transmog_arms = u"Transmogryfikuj ramiona",
         .event_text_for_talk_transmog_legs = u"Transmogryfikuj nogi",
         .event_text_for_talk_undo_transmog = u"Odtransmogryfikuj założoną zbroje",
         .event_text_for_map_undo_transmog_alert =
             u"Odtransmogryfikowana zbroja wróciła do twojego ekwipunku",
         .menu_text_transmog_head = u"Transmogryfikuj Głowe",
         .menu_text_transmog_body = u"Transmogryfikuj Ciało",
         .menu_text_transmog_arms = u"Transmogryfikuj Ramiona",
         .menu_text_transmog_legs = u"Transmogryfikuj Nogi",
         .protector_name_invisible = u"Niewidzialny",
     }},
    {"brazilian",
     {
         .event_text_for_talk_transmog_armor = u"Transmogrifique a armadura",
         .event_text_for_talk_transmog_head = u"Transmogrifique a cabeça",
         .event_text_for_talk_transmog_body = u"Transmogrifique o corpo",
         .event_text_for_talk_transmog_arms = u"Transmogrifique os braços",
         .event_text_for_talk_transmog_legs = u"Transmogrifique as pernas",
         .event_text_for_talk_undo_transmog = u"Desfazer a transmogrificação da armadura equipada",
         .event_text_for_map_undo_transmog_alert =
             u"A armadura não transmogrificada foi devolvida ao seu inventário",
         .menu_text_transmog_head = u"Transmogrifique a Cabeça",
         .menu_text_transmog_body = u"Transmogrifique o Corpo",
         .menu_text_transmog_arms = u"Transmogrifique os Braços",
         .menu_text_transmog_legs = u"Transmogrifique as Pernas",
         .protector_name_invisible = u"Invisível",
     }},
    {"latam",
     {
         .event_text_for_talk_transmog_armor = u"Transmogrificar la armadura",
         .event_text_for_talk_transmog_head = u"Transmogrificar la cabeza",
         .event_text_for_talk_transmog_body = u"Transmogrificar el cuerpo",
         .event_text_for_talk_transmog_arms = u"Transmogrificar los brazos",
         .event_text_for_talk_transmog_legs = u"Transmogrificar las piernas",
         .event_text_for_talk_undo_transmog = u"Deshacer la transmogrificación",
         .event_text_for_map_undo_transmog_alert =
             u"La armadura no transmogrificada ha sido devuelta a tu inventario",
         .menu_text_transmog_head = u"Transmogrificar la Cabeza",
         .menu_text_transmog_body = u"Transmogrificar el Cuerpo",
         .menu_text_transmog_arms = u"Transmogrificar los Brazos",
         .menu_text_transmog_legs = u"Transmogrificar las Piernas",
         .protector_name_invisible = u"Invisible",
     }},
    {"spanish",
     {
         .event_text_for_talk_transmog_armor = u"Transmogrificar la armadura",
         .event_text_for_talk_transmog_head = u"Transmogrificar la cabeza",
         .event_text_for_talk_transmog_body = u"Transmogrificar el cuerpo",
         .event_text_for_talk_transmog_arms = u"Transmogrificar los brazos",
         .event_text_for_talk_transmog_legs = u"Transmogrificar las piernas",
         .event_text_for_talk_undo_transmog = u"Deshacer la transmogrificación",
         .event_text_for_map_undo_transmog_alert =
             u"La armadura no transmogrificada ha sido devuelta a tu inventario",
         .menu_text_transmog_head = u"Transmogrificar la Cabeza",
         .menu_text_transmog_body = u"Transmogrificar el Cuerpo",
         .menu_text_transmog_arms = u"Transmogrificar los Brazos",
         .menu_text_transmog_legs = u"Transmogrificar las Piernas",
         .protector_name_invisible = u"Invisible",
     }},
    {"schinese",
     {
         .event_text_for_talk_transmog_armor = u"幻化装备",
         .event_text_for_talk_transmog_head = u"幻化头部",
         .event_text_for_talk_transmog_body = u"幻化胴体",
         .event_text_for_talk_transmog_arms = u"幻化手臂",
         .event_text_for_talk_transmog_legs = u"幻化腿部",
         .event_text_for_talk_undo_transmog = u"移除已穿戴装备的幻化",
         .event_text_for_map_undo_transmog_alert = u"被移除幻化的装备已回到装备库中",
         .menu_text_transmog_head = u"幻化头部装备",
         .menu_text_transmog_body = u"幻化胴体装备",
         .menu_text_transmog_arms = u"幻化手臂装备",
         .menu_text_transmog_legs = u"幻化腿部装备",
         .protector_name_invisible = u"无形",
     }},
    {"tchinese",
     {
         .event_text_for_talk_transmog_armor = u"幻化裝備",
         .event_text_for_talk_transmog_head = u"幻化頭部",
         .event_text_for_talk_transmog_body = u"幻化胴體",
         .event_text_for_talk_transmog_arms = u"幻化手臂",
         .event_text_for_talk_transmog_legs = u"幻化腿部",
         .event_text_for_talk_undo_transmog = u"移除已穿戴裝備的幻化",
         .event_text_for_map_undo_transmog_alert = u"被移除幻化的裝備已回到裝備庫中",
         .menu_text_transmog_head = u"幻化頭部裝備",
         .menu_text_transmog_body = u"幻化胴體裝備",
         .menu_text_transmog_arms = u"幻化手臂裝備",
         .menu_text_transmog_legs = u"幻化腿部裝備",
         .protector_name_invisible = u"無形",
     }},
};
