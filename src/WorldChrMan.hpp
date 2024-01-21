#pragma once
#pragma pack(1)

#include <cstddef>
#include <cstdint>

extern "C" struct CharacterData
{
    std::byte unknown1[0x10];
    std::uint32_t hp;
    std::uint32_t max_hp;
    std::byte unknown2[0x8];
    std::uint32_t max_stamina;
    std::byte unknown3[0xc];
    std::uint32_t max_fp;
    std::byte unknown4[0x8];
    std::uint32_t vigor;
    std::uint32_t mind;
    std::uint32_t endurance;
    std::uint32_t strength;
    std::uint32_t dexterity;
    std::uint32_t intelligence;
    std::uint32_t faith;
    std::uint32_t arcane;
    std::byte unknown5[0xc];
    std::uint32_t level;
    std::byte unknown6[0x2c];
    std::int32_t character_type;
    wchar_t name[16];
    std::byte unknown7[0x26];
    std::uint8_t upgrade_level;
    std::byte unknown8[0x2b9];
    std::int32_t right_weapon1;
    std::int32_t left_weapon1;
    std::int32_t right_weapon2;
    std::int32_t left_weapon2;
    std::int32_t right_weapon3;
    std::int32_t left_weapon3;
    std::int32_t arrow1;
    std::int32_t bolt1;
    std::int32_t arrow2;
    std::int32_t bolt2;
    std::byte unknown[0x8];
    std::int32_t head_armor;
    std::int32_t body_armor;
    std::int32_t arm_armor;
    std::int32_t leg_armor;
    std::byte unknown9[0x4];
    std::int32_t talismans[5];
};

extern "C" struct Player
{
    std::byte unknown[0x580];
    CharacterData *character_data;
};

extern "C" struct WorldChrMan
{
    std::byte unknown[0x10EF8];
    Player **players;
};
