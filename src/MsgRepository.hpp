#pragma once
#pragma pack(1)

#include <cstddef>
#include <cstdint>

enum class MsgBndId : std::uint32_t
{
    talk_msg = 1,
    blood_msg = 2,
    movie_subtitle = 3,
    goods_name = 10,
    weapon_name = 11,
    protector_name = 12,
    accessory_name = 13,
    magic_name = 14,
    npc_name = 18,
    place_name = 19,
    goods_info = 20,
    weapon_info = 21,
    protector_info = 22,
    accessory_info = 23,
    goods_caption = 24,
    weapon_caption = 25,
    protector_caption = 26,
    accessory_caption = 27,
    magic_info = 28,
    magic_caption = 29,
    network_message = 31,
    action_button_text = 32,
    event_text_for_talk = 33,
    event_text_for_map = 34,
    gem_name = 35,
    gem_info = 36,
    gem_caption = 37,
    goods_dialog = 41,
    arts_name = 42,
    arts_caption = 43,
    weapon_effect = 44,
    gem_effect = 45,
    goods_info2 = 46,
    menu_text = 200,
    line_help = 201,
    key_guide = 202,
    system_message_win64 = 203,
    dialogues = 204,
    loading_title = 205,
    loading_text = 206,
    tutorial_title = 207,
    tutorial_body = 208,
    text_embed_image_name_win64 = 209,
    tos_win64 = 210,
};

extern "C" struct MsgRepositoryCategory
{
    std::byte unknown1;
    std::uint8_t big_endian;
    std::uint8_t version;
    std::byte unknown2;
    std::uint32_t file_size;
    std::byte unknown3[4];
    std::uint32_t group_count;
    std::uint32_t string_count;
    std::byte unknown4[4];
    std::ptrdiff_t *string_offsets;
    std::byte unknown5[8];
};

extern "C" struct MsgRepository
{
    void **vftable_ptr;
    MsgRepositoryCategory *categories;
    std::uint32_t version_count;
    std::uint32_t category_capacity;
    std::byte unknown[8];
};

typedef const wchar_t *GetMessageFn(MsgRepository *msg_repository, uint32_t unknown,
                                    MsgBndId bnd_id, std::int32_t msg_id);