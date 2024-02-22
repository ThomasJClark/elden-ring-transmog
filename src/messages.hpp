#pragma once

#include <map>
#include <string>

struct Messages
{
    std::u16string transmog_armor;
    std::u16string transmog_head;
    std::u16string transmog_body;
    std::u16string transmog_arms;
    std::u16string transmog_legs;
    std::u16string undo_transmog;
    std::u16string transmog_head_title;
    std::u16string transmog_body_title;
    std::u16string transmog_arms_title;
    std::u16string transmog_legs_title;
    std::u16string invisible;
    std::u16string select_transmog_target;
    std::u16string transmog_head_to;
    std::u16string transmog_body_to;
    std::u16string transmog_arms_to;
    std::u16string transmog_legs_to;
};

static const std::map<std::string, Messages> transmog_messages_by_lang = {
    {"english",
     {
         .transmog_armor = u"Transmogrify armor",
         .transmog_head = u"Transmogrify head",
         .transmog_body = u"Transmogrify body",
         .transmog_arms = u"Transmogrify arms",
         .transmog_legs = u"Transmogrify legs",
         .undo_transmog = u"Dispel transmogrification",
         .transmog_head_title = u"Transmogrify Head",
         .transmog_body_title = u"Transmogrify Body",
         .transmog_arms_title = u"Transmogrify Arms",
         .transmog_legs_title = u"Transmogrify Legs",
         .invisible = u"Invisible",
         .select_transmog_target = u"Select target for transmogrification",
         .transmog_head_to = u"Transmogrify head to <?itemName?>?",
         .transmog_body_to = u"Transmogrify body to <?itemName?>?",
         .transmog_arms_to = u"Transmogrify arms to <?itemName?>?",
         .transmog_legs_to = u"Transmogrify legs to <?itemName?>?",
     }},
    {"german",
     {
         .transmog_armor = u"Rüstung transmogrifizieren",
         .transmog_head = u"Kopf transmogrifizieren",
         .transmog_body = u"Körper transmogrifizieren",
         .transmog_arms = u"Arme transmogrifizieren",
         .transmog_legs = u"Beine transmogrifizieren",
         .undo_transmog = u"Transmogrifikation deaktivieren",
         .transmog_head_title = u"Kopf transmogrifizieren",
         .transmog_body_title = u"Körper transmogrifizieren",
         .transmog_arms_title = u"Arme transmogrifizieren",
         .transmog_legs_title = u"Beine transmogrifizieren",
         .invisible = u"Unsichtbar",
         .select_transmog_target = u"Ziel für die Transmogrifizierung auswählen",
         .transmog_head_to = u"Transmogrifizieren kopf zu <?itemName?>?",
         .transmog_body_to = u"Transmogrifizieren körper zu <?itemName?>?",
         .transmog_arms_to = u"Transmogrifizieren arme zu <?itemName?>?",
         .transmog_legs_to = u"Transmogrifizieren beine zu <?itemName?>?",
     }},
    {"french",
     {
         .transmog_armor = u"Transmogrifier l'armure",
         .transmog_head = u"Transmogrifier la tête",
         .transmog_body = u"Transmogrifier le corps",
         .transmog_arms = u"Transmogrifier les bras",
         .transmog_legs = u"Transmogrifier les jambes",
         .undo_transmog = u"Dissiper la transmogrification",
         .transmog_head_title = u"Transmogrifier la tête",
         .transmog_body_title = u"Transmogrifier le corps",
         .transmog_arms_title = u"Transmogrifier les bras",
         .transmog_legs_title = u"Transmogrifier les jambes",
         .invisible = u"Invisible",
         .select_transmog_target = u"Sélectionner la cible de la transmogrification",
         .transmog_head_to = u"Transmogrifier la tête en <?itemName?> ?",
         .transmog_body_to = u"Transmogrifier le corps en <?itemName?> ?",
         .transmog_arms_to = u"Transmogrifier les bras en <?itemName?> ?",
         .transmog_legs_to = u"Transmogrifier les jambes en <?itemName?> ?",
     }},
    {"italian",
     {
         .transmog_armor = u"Transmogrifica l'armatura",
         .transmog_head = u"Transmogrifica della testa",
         .transmog_body = u"Transmogrifica del corpo",
         .transmog_arms = u"Transmogrifica delle braccia",
         .transmog_legs = u"Transmogrifica gambe",
         .undo_transmog = u"Dissipi la transmogrificazione",
         .transmog_head_title = u"Transmogrifica della testa",
         .transmog_body_title = u"Transmogrifica del corpo",
         .transmog_arms_title = u"Transmogrifica delle braccia",
         .transmog_legs_title = u"Transmogrifica gambe",
         .invisible = u"Invisibile",
         .select_transmog_target = u"Seleziona il bersaglio della transmogrificazione",
         .transmog_head_to = u"Transmogrifica la testa in <?itemName?>?",
         .transmog_body_to = u"Transmogrifica il corpo in <?itemName?>?",
         .transmog_arms_to = u"Transmogrifica le braccia in <?itemName?>?",
         .transmog_legs_to = u"Transmogrifica le gambe in <?itemName?>?",
     }},
    {"polish",
     {
         .transmog_armor = u"Transmogryfikuj zbroje",
         .transmog_head = u"Transmogryfikuj głowe",
         .transmog_body = u"Transmogryfikuj ciało",
         .transmog_arms = u"Transmogryfikuj ramiona",
         .transmog_legs = u"Transmogryfikuj nogi",
         .undo_transmog = u"Dispel transmogrification", // TODO
         .transmog_head_title = u"Transmogryfikuj głowe",
         .transmog_body_title = u"Transmogryfikuj ciało",
         .transmog_arms_title = u"Transmogryfikuj ramiona",
         .transmog_legs_title = u"Transmogryfikuj nogi",
         .invisible = u"Niewidzialny",
         .select_transmog_target = u"Select target for transmogrification", // TODO
         .transmog_head_to = u"Transmogrify head to <?itemName?>?",         // TODO
         .transmog_body_to = u"Transmogrify body to <?itemName?>?",         // TODO
         .transmog_arms_to = u"Transmogrify arms to <?itemName?>?",         // TODO
         .transmog_legs_to = u"Transmogrify legs to <?itemName?>?",         // TODO
     }},
    {"brazilian",
     {
         .transmog_armor = u"Transmogrificar armadura",
         .transmog_head = u"Transmogrificar cabeça",
         .transmog_body = u"Transmogrificar corpo",
         .transmog_arms = u"Transmogrificar braços",
         .transmog_legs = u"Transmogrificar pernas",
         .undo_transmog = u"Dissipar transmogrificação",
         .transmog_head_title = u"Transmogrificar Cabeça",
         .transmog_body_title = u"Transmogrificar Corpo",
         .transmog_arms_title = u"Transmogrificar Braços",
         .transmog_legs_title = u"Transmogrificar Pernas",
         .invisible = u"Invisível",
         .select_transmog_target = u"Selecione o alvo da transmogrificação",
         .transmog_head_to = u"Transmogrificar cabeça para <?itemName?>?",
         .transmog_body_to = u"Transmogrificar corpo para <?itemName?>?",
         .transmog_arms_to = u"Transmogrificar braços para <?itemName?>?",
         .transmog_legs_to = u"Transmogrificar pernas para <?itemName?>?",
     }},
    {"latam",
     {
         .transmog_armor = u"Transmogrificar armadura",
         .transmog_head = u"Transmogrificar cabeza",
         .transmog_body = u"Transmogrificar cuerpo",
         .transmog_arms = u"Transmogrificar brazos",
         .transmog_legs = u"Transmogrificar piernas",
         .undo_transmog = u"Disipar transmogrificación",
         .transmog_head_title = u"Transmogrificar cabeza",
         .transmog_body_title = u"Transmogrificar cuerpo",
         .transmog_arms_title = u"Transmogrificar brazos",
         .transmog_legs_title = u"Transmogrificar piernas",
         .invisible = u"Invisible",
         .select_transmog_target = u"Selecciona el objetivo de la transmogrificación",
         .transmog_head_to = u"Transmogrificar cabeza a <?itemName?>?",
         .transmog_body_to = u"Transmogrificar cuerpo a <?itemName?>?",
         .transmog_arms_to = u"Transmogrificar brazos a <?itemName?>?",
         .transmog_legs_to = u"Transmogrificar piernas a <?itemName?>?",
     }},
    {"spanish",
     {
         .transmog_armor = u"Transmogrificar armadura",
         .transmog_head = u"Transmogrificar cabeza",
         .transmog_body = u"Transmogrificar cuerpo",
         .transmog_arms = u"Transmogrificar brazos",
         .transmog_legs = u"Transmogrificar piernas",
         .undo_transmog = u"Disipar transmogrificación",
         .transmog_head_title = u"Transmogrificar cabeza",
         .transmog_body_title = u"Transmogrificar cuerpo",
         .transmog_arms_title = u"Transmogrificar brazos",
         .transmog_legs_title = u"Transmogrificar piernas",
         .invisible = u"Invisible",
         .select_transmog_target = u"Selecciona el objetivo de la transmogrificación",
         .transmog_head_to = u"Transmogrificar cabeza a <?itemName?>?",
         .transmog_body_to = u"Transmogrificar cuerpo a <?itemName?>?",
         .transmog_arms_to = u"Transmogrificar brazos a <?itemName?>?",
         .transmog_legs_to = u"Transmogrificar piernas a <?itemName?>?",
     }},
    {"schinese",
     {
         .transmog_armor = u"幻化装备",
         .transmog_head = u"幻化头部",
         .transmog_body = u"幻化胴体",
         .transmog_arms = u"幻化手臂",
         .transmog_legs = u"幻化腿部",
         .undo_transmog = u"Dispel transmogrification", // TODO
         .transmog_head_title = u"幻化头部装备",
         .transmog_body_title = u"幻化胴体装备",
         .transmog_arms_title = u"幻化手臂装备",
         .transmog_legs_title = u"幻化腿部装备",
         .invisible = u"无形",
         .select_transmog_target = u"Select target for transmogrification", // TODO
         .transmog_head_to = u"Transmogrify head to <?itemName?>?",         // TODO
         .transmog_body_to = u"Transmogrify body to <?itemName?>?",         // TODO
         .transmog_arms_to = u"Transmogrify arms to <?itemName?>?",         // TODO
         .transmog_legs_to = u"Transmogrify legs to <?itemName?>?",         // TODO
     }},
    {"tchinese",
     {
         .transmog_armor = u"幻化裝備",
         .transmog_head = u"幻化頭部",
         .transmog_body = u"幻化胴體",
         .transmog_arms = u"幻化手臂",
         .transmog_legs = u"幻化腿部",
         .undo_transmog = u"Dispel transmogrification", // TODO
         .transmog_head_title = u"幻化頭部裝備",
         .transmog_body_title = u"幻化胴體裝備",
         .transmog_arms_title = u"幻化手臂裝備",
         .transmog_legs_title = u"幻化腿部裝備",
         .invisible = u"無形",
         .select_transmog_target = u"Select target for transmogrification", // TODO
         .transmog_head_to = u"Transmogrify head to <?itemName?>?",         // TODO
         .transmog_body_to = u"Transmogrify body to <?itemName?>?",         // TODO
         .transmog_arms_to = u"Transmogrify arms to <?itemName?>?",         // TODO
         .transmog_legs_to = u"Transmogrify legs to <?itemName?>?",         // TODO
     }},
};
