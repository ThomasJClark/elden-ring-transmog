#define WIN32_LEAN_AND_MEAN

#include <algorithm>
#include <stdexcept>
#include <tga/dl_types.h>
#include <tga/param_containers.h>
#include <vector>
#include <windows.h>

#include "Gamehook.hpp"

// https://github.com/soulsmods/DSMapStudio/blob/master/src/StudioCore/Assets/GameOffsets/ER/CoreOffsets.txt
static const std::vector<int> g_param_base_aob = {0x48, 0x8B, 0x0D, -1,   -1,   -1,   -1,
                                                  0x48, 0x85, 0xC9, 0x0F, 0x84, -1,   -1,
                                                  -1,   -1,   0x45, 0x33, 0xC0, 0xBA, 0x90};

static const std::vector<std::pair<ptrdiff_t, ptrdiff_t>> g_param_base_aob_relative_offset = {
    {0x03, 0x07}};

static const size_t g_param_count = 185;

extern "C" struct ParamListEntry
{
    ParamResCap *param_res_cap;
    std::byte unknown[64];
};

extern "C" struct ParamList
{
    std::byte unknown[136];
    ParamListEntry entries[0];
};

void GameHook::initialize(char const *module_name)
{
    HMODULE module_handle = GetModuleHandleA(module_name);

    if (module_handle)
    {
        MEMORY_BASIC_INFORMATION memory_info;
        if (VirtualQuery((void *)module_handle, &memory_info, sizeof(memory_info)) != 0)
        {
            IMAGE_DOS_HEADER *dos_header = (IMAGE_DOS_HEADER *)module_handle;
            IMAGE_NT_HEADERS *nt_headers =
                (IMAGE_NT_HEADERS *)((ULONG64)memory_info.AllocationBase +
                                     (ULONG64)dos_header->e_lfanew);

            if ((dos_header->e_magic == IMAGE_DOS_SIGNATURE) &&
                (nt_headers->Signature == IMAGE_NT_SIGNATURE))
            {
                game_module = std::span((std::byte *)memory_info.AllocationBase,
                                        (size_t)nt_headers->OptionalHeader.SizeOfImage);
            }
        }
    }

    if (game_module.empty())
    {
        throw std::runtime_error("Couldn't find game module");
    }

    auto param_list_addr =
        reinterpret_cast<ParamList **>(scan(g_param_base_aob, g_param_base_aob_relative_offset));

    if (param_list_addr == nullptr)
    {
        throw std::runtime_error("Couldn't find param base address");
    }

    param_list = *param_list_addr;
}

void *GameHook::scan(std::vector<int> aob,
                     std::vector<std::pair<ptrdiff_t, ptrdiff_t>> relative_offsets)
{
    auto start = game_module.begin();
    auto end = game_module.end() - aob.size();

    auto match = std::ranges::find_if(start, end, [&aob](std::byte const &data) {
        return std::all_of(&data, &data + aob.size(), [&data, &aob](std::byte const &value) {
            int comparison_value = aob[&value - &data];
            return comparison_value == -1 || comparison_value == (int)value;
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

std::unique_ptr<GameHook::Param> GameHook::get_param(std::wstring const &name)
{
    auto param_list_entries = param_list->entries;
    for (int i = 0; i <= g_param_count; i++)
    {
        auto param_res_cap = param_list_entries[i].param_res_cap;
        if (name == dlw_c_str(&param_res_cap->param_name))
        {
            auto param_table = param_res_cap->param_header->param_table;
            auto param_table_num_rows = param_table->num_rows;
            auto param_table_rows = param_table->rows;
            auto param_row_base_addr = reinterpret_cast<std::byte *>(param_table);

            auto param = std::make_unique<Param>();
            param->name = name;

            for (int i = 0; i < param_table_num_rows; i++)
            {
                auto row_info = param_table_rows[i];
                param->rows[row_info.row_id] = param_row_base_addr + row_info.param_offset;
            }

            return param;
        }
    }

    throw std::runtime_error("Couldn't find param");
}
