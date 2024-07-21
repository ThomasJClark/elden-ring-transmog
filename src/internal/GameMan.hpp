#pragma once

#include <cstddef>
#include <cstdint>

#pragma pack(push, 1)

namespace CS
{
struct GameMan
{
    unsigned char unk1[0xbf0];
    unsigned char ceremony_type;
};
}

#pragma pack(pop)
