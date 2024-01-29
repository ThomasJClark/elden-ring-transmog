#define WIN32_LEAN_AND_MEAN

#include <MinHook.h>
#include <algorithm>
#include <cstdint>
#include <memory>
#include <span>
#include <stdexcept>
#include <windows.h>

#include "TransmogUtils.hpp"

static std::span<std::byte> memory;

void TransmogUtils::initialize()
{
    HMODULE module_handle = GetModuleHandleA("eldenring.exe");
    if (!module_handle)
    {
        throw std::runtime_error("Failed to get handle for eldenring.exe process");
    }

    MEMORY_BASIC_INFORMATION memory_info;
    if (VirtualQuery((void *)module_handle, &memory_info, sizeof(memory_info)) == 0)
    {
        throw std::runtime_error("Failed to get virtual memory information");
    }

    IMAGE_DOS_HEADER *dos_header = (IMAGE_DOS_HEADER *)module_handle;
    IMAGE_NT_HEADERS *nt_headers =
        (IMAGE_NT_HEADERS *)((ULONG64)memory_info.AllocationBase + (ULONG64)dos_header->e_lfanew);

    if ((dos_header->e_magic == IMAGE_DOS_SIGNATURE) &&
        (nt_headers->Signature == IMAGE_NT_SIGNATURE))
    {
        memory = {(std::byte *)memory_info.AllocationBase, nt_headers->OptionalHeader.SizeOfImage};
    }

    auto mh_status = MH_Initialize();
    if (mh_status != MH_OK)
    {
        throw std::runtime_error(std::string("Error initializing MinHook: ") +
                                 MH_StatusToString(mh_status));
    }
}

void TransmogUtils::deinitialize()
{
    MH_Uninitialize();
}

void *TransmogUtils::scan(const std::vector<int> &aob, std::ptrdiff_t alignment,
                          std::ptrdiff_t offset,
                          const std::vector<std::pair<ptrdiff_t, ptrdiff_t>> relative_offsets)
{
    for (auto match = &memory.front(); match < &memory.back() - aob.size(); match += alignment)
    {
        if (std::all_of(aob.begin(), aob.end(), [&aob, &match](const auto &b) {
                return b == -1 || b == (int)match[&b - &aob[0]];
            }))
        {
            match += offset;

            for (auto [first, second] : relative_offsets)
            {
                ptrdiff_t offset = *reinterpret_cast<std::uint32_t *>(&match[first]) + second;
                match += offset;
            }

            return match;
        }
    }

    return nullptr;
}

void TransmogUtils::hook(void *function, void *detour, void **trampoline)
{
    auto mh_status = MH_CreateHook(function, detour, trampoline);
    if (mh_status != MH_OK)
    {
        throw std::runtime_error(std::string("Error creating hook: ") +
                                 MH_StatusToString(mh_status));
    }
    mh_status = MH_EnableHook(function);
    if (mh_status != MH_OK)
    {
        throw std::runtime_error(std::string("Error enabling hook: ") +
                                 MH_StatusToString(mh_status));
    }
}

void TransmogUtils::unhook(void *function)
{
    MH_RemoveHook(function);
}