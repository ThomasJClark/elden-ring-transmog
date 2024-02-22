#define WIN32_LEAN_AND_MEAN

#include <MinHook.h>
#include <Pattern16.h>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <windows.h>

#include "ModUtils.hpp"

using namespace std;

static span<byte> memory;

void ModUtils::initialize()
{
    HMODULE module_handle = GetModuleHandleA("eldenring.exe");
    if (!module_handle)
    {
        throw runtime_error("Failed to get handle for eldenring.exe process");
    }

    MEMORY_BASIC_INFORMATION memory_info;
    if (VirtualQuery((void *)module_handle, &memory_info, sizeof(memory_info)) == 0)
    {
        throw runtime_error("Failed to get virtual memory information");
    }

    IMAGE_DOS_HEADER *dos_header = (IMAGE_DOS_HEADER *)module_handle;
    IMAGE_NT_HEADERS *nt_headers =
        (IMAGE_NT_HEADERS *)((ULONG64)memory_info.AllocationBase + (ULONG64)dos_header->e_lfanew);

    if ((dos_header->e_magic == IMAGE_DOS_SIGNATURE) &&
        (nt_headers->Signature == IMAGE_NT_SIGNATURE))
    {
        memory = {(byte *)memory_info.AllocationBase, nt_headers->OptionalHeader.SizeOfImage};
    }

    auto mh_status = MH_Initialize();
    if (mh_status != MH_OK)
    {
        throw runtime_error(string("Error initializing MinHook: ") + MH_StatusToString(mh_status));
    }
}

void ModUtils::deinitialize()
{
    MH_Uninitialize();
}

void *ModUtils::scan(const ScanArgs &args)
{
    auto match =
        args.aob.empty()
            ? &memory.front()
            : reinterpret_cast<byte *>(Pattern16::scan(&memory.front(), memory.size(), args.aob));
    if (match != nullptr)
    {
        match += args.offset;

        for (auto [first, second] : args.relative_offsets)
        {
            ptrdiff_t offset = *reinterpret_cast<int32_t *>(&match[first]) + second;
            match += offset;
        }

        return match;
    }

    return nullptr;
}

void ModUtils::hook(void *function, void *detour, void **trampoline)
{
    auto mh_status = MH_CreateHook(function, detour, trampoline);
    if (mh_status != MH_OK)
    {
        throw runtime_error(string("Error creating hook: ") + MH_StatusToString(mh_status));
    }
    mh_status = MH_EnableHook(function);
    if (mh_status != MH_OK)
    {
        throw runtime_error(string("Error enabling hook: ") + MH_StatusToString(mh_status));
    }
}

void ModUtils::unhook(void *function)
{
    MH_RemoveHook(function);
}