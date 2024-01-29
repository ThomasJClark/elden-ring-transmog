#pragma once
#define WIN32_LEAN_AND_MEAN

#include <MinHook.h>
#include <algorithm>
#include <cstdint>
#include <memory>
#include <span>
#include <stdexcept>
#include <vector>
#include <windows.h>

class GameMemory
{
  private:
    std::span<std::byte> memory;

  public:
    void initialize()
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
        IMAGE_NT_HEADERS *nt_headers = (IMAGE_NT_HEADERS *)((ULONG64)memory_info.AllocationBase +
                                                            (ULONG64)dos_header->e_lfanew);

        if ((dos_header->e_magic == IMAGE_DOS_SIGNATURE) &&
            (nt_headers->Signature == IMAGE_NT_SIGNATURE))
        {
            memory = {(std::byte *)memory_info.AllocationBase,
                      nt_headers->OptionalHeader.SizeOfImage};
        }

        auto mh_status = MH_Initialize();
        if (mh_status != MH_OK)
        {
            throw std::runtime_error(std::string("Error initializing MinHook: ") +
                                     MH_StatusToString(mh_status));
        }
    }

    void deinitialize()
    {
        MH_Uninitialize();
    }

    template <typename ReturnType, typename AlignmentType = std::byte> struct ScanArgs
    {
        const std::vector<int> aob;
        const std::ptrdiff_t step = 1;
        const std::ptrdiff_t offset = 0;
        const std::vector<std::pair<ptrdiff_t, ptrdiff_t>> relative_offsets = {};
    };

    /**
     * Scans a span of memory for a matching array of bytes, then apply an optional offset and list
     * of relative offsets
     */
    template <typename ReturnType, typename AlignmentType = std::byte>
    ReturnType *scan(const ScanArgs<ReturnType, AlignmentType> &args)
    {
        const auto &aob = args.aob;
        const auto step = args.step;
        const auto offset = args.offset;
        const auto &relative_offsets = args.relative_offsets;

        for (auto match = &memory.front(); match < &memory.back() - aob.size(); match += step)
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

                return reinterpret_cast<ReturnType *>(match);
            }
        }

        return nullptr;
    }

    /**
     * Scans for a function in the game, and hooks it with an override using MinHook
     */
    template <typename FunctionType, typename AlignmentType = std::byte>
    FunctionType *hook(const ScanArgs<FunctionType, AlignmentType> &args, FunctionType &detour,
                       FunctionType *&trampoline)
    {
        auto original = scan(args);
        if (original == nullptr)
        {
            throw std::runtime_error(std::string("Error looking up original function"));
        }

        auto mh_status = MH_CreateHook(original, &detour, (void **)&trampoline);
        if (mh_status != MH_OK)
        {
            throw std::runtime_error(std::string("Error creating hook: ") +
                                     MH_StatusToString(mh_status));
        }
        mh_status = MH_EnableHook(original);
        if (mh_status != MH_OK)
        {
            throw std::runtime_error(std::string("Error enabling hook: ") +
                                     MH_StatusToString(mh_status));
        }
        return original;
    }

    template <typename FunctionType> void unhook(FunctionType *hook)
    {
        MH_RemoveHook(hook);
    }
};
