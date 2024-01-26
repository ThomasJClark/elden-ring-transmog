#pragma once

#include <cstdint>
#include <vector>

struct ScanArgs
{
    std::vector<int> aob;
    std::ptrdiff_t step = 1;
    std::ptrdiff_t offset = 0;
    std::vector<std::pair<ptrdiff_t, ptrdiff_t>> relative_offsets = {};
};

template <typename T> T *scan(std::span<std::byte> game_module, ScanArgs const &args)
{
    const std::vector<int> &aob = args.aob;
    const std::ptrdiff_t &step = args.step;
    const std::ptrdiff_t &offset = args.offset;
    const std::vector<std::pair<ptrdiff_t, ptrdiff_t>> &relative_offsets = args.relative_offsets;

    auto match = game_module.begin();
    auto end = game_module.end() - args.aob.size();

    while (true)
    {
        if (match >= end)
        {

            return nullptr;
        }

        auto matches = true;
        for (int i = 0; i < args.aob.size(); i++)
        {
            if (args.aob[i] != -1 && args.aob[i] != (int)match[i])
            {
                matches = false;
                break;
            }
        }

        if (matches)
        {
            break;
        }

        match += args.step;
    }

    match += args.offset;

    for (auto [first, second] : args.relative_offsets)
    {
        ptrdiff_t offset = *reinterpret_cast<std::uint32_t *>(&match[first]) + second;
        match += offset;
    }

    return &match[0];
}
