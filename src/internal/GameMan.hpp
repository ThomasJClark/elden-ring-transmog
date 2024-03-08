#pragma once

#include <cstddef>
#include <cstdint>

#pragma pack(push, 1)

namespace CS
{
static constexpr std::byte ceremony_type_none = (std::byte)0;

struct GameMan
{
    std::byte unk1[0xbc0];
    std::byte ceremony_type;
    std::byte unk2[0x28f];
};
} // namespace CS

#pragma pack(pop)
