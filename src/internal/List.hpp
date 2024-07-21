#pragma once

#include <cstdint>

#pragma pack(push, 1)

namespace CS
{
template <typename T> struct List
{
    T *elements;
    unsigned int count;
    unsigned char pad[4];

    inline List() : count(0), elements(nullptr)
    {
    }

    inline List(T *elements, unsigned int count) : elements(elements), count(count)
    {
    }

    template <size_t count> inline List(T (&t)[count]) : elements(t), count(count)
    {
    }

    inline T &operator[](size_t index)
    {
        return elements[index];
    }

    inline T *begin()
    {
        return elements;
    }

    inline T *end()
    {
        return elements + count;
    }
};
};

#pragma pack(pop)
