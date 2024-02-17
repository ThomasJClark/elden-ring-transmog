#pragma once

#include <cstdint>

namespace CS
{
template <typename T> struct List
{
    T *elements;
    uint32_t count;
    std::byte pad[4];

    inline List() : count(0), elements(nullptr)
    {
    }

    inline List(T *elements, uint32_t count) : elements(elements), count(count)
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
}; // namespace CS