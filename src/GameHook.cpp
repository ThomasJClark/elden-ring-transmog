#define WIN32_LEAN_AND_MEAN

#include <algorithm>
#include <stdexcept>
#include <vector>
#include <windows.h>

#include "Gamehook.hpp"

// https://github.com/soulsmods/DSMapStudio/blob/master/src/StudioCore/Assets/GameOffsets/ER/CoreOffsets.txt
static const std::vector<int> paramBaseAob = {0x48, 0x8B, 0x0D, -1,   -1,   -1,   -1,
                                              0x48, 0x85, 0xC9, 0x0F, 0x84, -1,   -1,
                                              -1,   -1,   0x45, 0x33, 0xC0, 0xBA, 0x90};
static const std::vector<std::pair<ptrdiff_t, ptrdiff_t>> paramBaseAobRelativeOffset = {
    {0x03, 0x07}};

extern "C"
{
    struct ParamRowInfo
    {
        uint64_t id;             // ID of param row
        uint64_t paramOffset;    // Offset of pointer to param data relative to parent table
        uint64_t paramEndOffset; // Seems to point to end of ParamTable struct
    };

    struct ParamTable
    {
        uint8_t unknown1[0x0A];
        uint16_t numRows; // Number of rows in param table
        uint8_t unknown2[0x04];
        uint64_t paramTypeOffset; // Offset of param type string from the beginning of this struct
        uint8_t unknown3[0x28];
        ParamRowInfo rows[0]; // Array of row information structs
    };

    struct ParamHeader
    {
        uint8_t unknown1[0x80];
        ParamTable *paramTable;
    };

    struct DLWString
    {
        union {
            wchar_t inPlace[8];
            wchar_t *ptr;
        } str;
        uint64_t length;
        uint64_t capacity;
    };

    struct ParamResCap
    {
        void **vftable_ptr;
        uint8_t unknown1[0x10];
        DLWString paramName;
        uint8_t unknown2[0x48];
        ParamHeader *paramHeader;
    };

    struct ParamBankEntry
    {
        ParamResCap *paramResCap;
        std::byte unknown[64];
    };

    struct ParamBank
    {
        std::byte unknown[136];
        ParamBankEntry entries[0];
    };

    inline wchar_t *dlw_c_str(DLWString *s)
    {
        return (s->capacity > 7) ? s->str.ptr : s->str.inPlace;
    }
}

void GameHook::initialize(char const *moduleName)
{
    HMODULE moduleHandle = GetModuleHandleA(moduleName);

    if (moduleHandle)
    {
        MEMORY_BASIC_INFORMATION memoryInfo;
        if (VirtualQuery((void *)moduleHandle, &memoryInfo, sizeof(memoryInfo)) != 0)
        {
            IMAGE_DOS_HEADER *dosHeader = (IMAGE_DOS_HEADER *)moduleHandle;
            IMAGE_NT_HEADERS *ntHeaders = (IMAGE_NT_HEADERS *)((ULONG64)memoryInfo.AllocationBase +
                                                               (ULONG64)dosHeader->e_lfanew);

            if ((dosHeader->e_magic == IMAGE_DOS_SIGNATURE) &&
                (ntHeaders->Signature == IMAGE_NT_SIGNATURE))
            {
                gameModule = std::span((std::byte *)memoryInfo.AllocationBase,
                                       (size_t)ntHeaders->OptionalHeader.SizeOfImage);
            }
        }
    }

    if (gameModule.empty())
    {
        throw std::runtime_error("Couldn't find game module");
    }

    auto paramBankAddr =
        reinterpret_cast<ParamBank **>(scan(paramBaseAob, paramBaseAobRelativeOffset));

    if (paramBankAddr == nullptr)
    {
        throw std::runtime_error("Couldn't find param base address");
    }

    paramBank = *paramBankAddr;
}

void *GameHook::scan(std::vector<int> aob,
                     std::vector<std::pair<ptrdiff_t, ptrdiff_t>> relative_offsets)
{
    auto start = gameModule.begin();
    auto end = gameModule.end() - aob.size();

    auto match = std::ranges::find_if(start, end, [&aob](std::byte const &data) {
        return std::all_of(&data, &data + aob.size(), [&data, &aob](std::byte const &value) {
            int comparisonValue = aob[&value - &data];
            return comparisonValue == -1 || comparisonValue == (int)value;
        });
    });

    if (match == end)
    {
        return nullptr;
    }

    for (auto [first, second] : relative_offsets)
    {
        ptrdiff_t offset = *reinterpret_cast<uint32_t *>(&match[first]) + second;
        match += offset;
    }

    return &match[0];
}

std::unique_ptr<GameHook::Param> GameHook::getParam(std::wstring const &name)
{
    auto paramBankEntries = paramBank->entries;
    for (int i = 0; i <= paramCount; i++)
    {
        auto paramResCap = paramBankEntries[i].paramResCap;
        if (name == dlw_c_str(&paramResCap->paramName))
        {
            auto paramTable = paramResCap->paramHeader->paramTable;
            auto paramTableNumRows = paramTable->numRows;
            auto paramTableRows = paramTable->rows;
            auto paramRowBaseAddr = reinterpret_cast<std::byte *>(paramTable);

            auto param = std::make_unique<Param>();
            param->name = name;

            for (int i = 0; i < paramTableNumRows; i++)
            {
                auto rowInfo = paramTableRows[i];
                param->rows[rowInfo.id] = paramRowBaseAddr + rowInfo.paramOffset;
            }

            return param;
        }
    }
    throw std::runtime_error("Couldn't find param");
}