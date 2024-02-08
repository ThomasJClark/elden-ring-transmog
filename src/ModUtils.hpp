#pragma once
#define WIN32_LEAN_AND_MEAN

#include <stdexcept>
#include <vector>

namespace ModUtils
{

void initialize();
void deinitialize();

void *scan(const std::vector<int> &aob, std::ptrdiff_t alignment, std::ptrdiff_t offset,
           const std::vector<std::pair<ptrdiff_t, ptrdiff_t>> relative_offsets);

void hook(void *function, void *detour, void **trampoline);

void unhook(void *function);

template <typename ReturnType> struct ScanArgs
{
    const std::vector<int> aob;
    const std::size_t alignment = 1;
    const std::ptrdiff_t offset = 0;
    const std::vector<std::pair<ptrdiff_t, ptrdiff_t>> relative_offsets = {};
};

template <typename ReturnType> inline ReturnType *scan(const ScanArgs<ReturnType> &args)
{
    return reinterpret_cast<ReturnType *>(
        scan(args.aob, args.alignment, args.offset, args.relative_offsets));
}

template <typename FunctionType>
inline FunctionType *hook(const ScanArgs<FunctionType> &args, FunctionType &detour,
                          FunctionType *&trampoline)
{
    auto function = scan(args);
    if (function == nullptr)
    {
        throw std::runtime_error("Failed to find original function address");
    }
    hook(reinterpret_cast<void *>(function), reinterpret_cast<void *>(&detour),
         reinterpret_cast<void **>(&trampoline));
    return function;
}

}; // namespace ModUtils
