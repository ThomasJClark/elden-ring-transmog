#pragma once

#include <cstddef>
#include <cstdint>

#pragma pack(push, 1)

namespace CS
{
struct GameMan
{
    std::byte unk1[0xbf0];
    std::byte ceremony_type;
};
}

#pragma pack(pop)
