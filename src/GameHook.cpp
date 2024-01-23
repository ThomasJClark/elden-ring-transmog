#define WIN32_LEAN_AND_MEAN

#include <MinHook.h>
#include <algorithm>
#include <stdexcept>
#include <tga/dl_types.h>
#include <tga/param_containers.h>
#include <vector>
#include <windows.h>

#include "Gamehook.hpp"

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

    auto param_list_aob = {0x48, 0x8B, 0x0D, -1, -1, -1,   -1,   0x48, 0x85, 0xC9, 0x0F,
                           0x84, -1,   -1,   -1, -1, 0x45, 0x33, 0xC0, 0xBA, 0x90};
    param_list_address = reinterpret_cast<ParamList **>(scan(param_list_aob, 0, {{0x03, 0x07}}));
    if (param_list_address == nullptr)
    {
        throw std::runtime_error("Couldn't find param base address");
    }

    auto world_chr_man_aob = {0x48, 0x8B, 0x05, -1,   -1,   -1,   -1,  0x48,
                              0x85, 0xC0, 0x74, 0x0F, 0x48, 0x39, 0x88};
    world_chr_man_address =
        reinterpret_cast<WorldChrMan **>(scan(world_chr_man_aob, 0, {{0x03, 0x07}}));
    if (world_chr_man_address == nullptr)
    {
        throw std::runtime_error("Couldn't find WorldChrMan address");
    }

    auto msg_repository_aob = {0x48, 0x8B, 0x3D, -1,   -1,   -1,   -1,  0x44,
                               0x0F, 0xB6, 0x30, 0x48, 0x85, 0xFF, 0x75};
    msg_repository_address =
        reinterpret_cast<MsgRepository **>(scan(msg_repository_aob, 0, {{0x03, 0x07}}));
    if (msg_repository_address == nullptr)
    {
        throw std::runtime_error("Couldn't find MsgRepository address");
    }

    auto get_message_aob = {0x8B, 0xDA, 0x44, 0x8B, 0xCA, 0x33, 0xD2,
                            0x48, 0x8B, 0xF9, 0x44, 0x8D, 0x42, 0x6F};
    get_message = reinterpret_cast<GetMessageFn *>(scan(get_message_aob, 14, {{0x01, 0x05}}));
    if (get_message == nullptr)
    {

        throw std::runtime_error("Couldn't find getMessage() address");
    }

    auto mh_status = MH_Initialize();
    if (mh_status != MH_OK)
    {
        throw std::runtime_error(MH_StatusToString(mh_status));
    }
}

void *GameHook::scan(std::vector<int> aob, ptrdiff_t offset,
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

    match += offset;

    for (auto [first, second] : relative_offsets)
    {
        ptrdiff_t offset = *reinterpret_cast<std::uint32_t *>(&match[first]) + second;
        match += offset;
    }

    return &match[0];
}

bool GameHook::try_initialize_params()
{
    auto param_list = *param_list_address;
    if (param_list == nullptr)
    {
        return false;
    }
    auto param_list_entries = param_list->entries;
    for (int i = 0; i <= g_param_count; i++)
    {
        auto param_res_cap = param_list_entries[i].param_res_cap;
        if (param_res_cap == nullptr)
        {
            return false;
        }

        auto param_table = param_res_cap->param_header->param_table;
        auto param_table_num_rows = param_table->num_rows;
        auto param_table_rows = param_table->rows;
        auto param_row_base_addr = reinterpret_cast<std::byte *>(param_table);

        auto param = std::make_shared<Param>(dlw_c_str(&param_res_cap->param_name));
        for (int i = 0; i < param_table_num_rows; i++)
        {
            auto row_info = param_table_rows[i];
            param->rows[row_info.row_id] = param_row_base_addr + row_info.param_offset;
        }
        params[param->name] = param;
    }
    return true;
}

ParamHeader *GameHook::get_param_header(std::wstring const &name)
{
    auto param_list = *param_list_address;
    if (param_list == nullptr)
    {
        return nullptr;
    }

    auto param_list_entries = param_list->entries;
    for (int i = 0; i <= g_param_count; i++)
    {
        auto param_res_cap = param_list_entries[i].param_res_cap;
        if (param_res_cap != nullptr && name == dlw_c_str(&param_res_cap->param_name))
        {
            return param_res_cap->param_header;
        }
    }

    return nullptr;
}

Player *GameHook::get_player(int index)
{
    auto world_chr_man = *world_chr_man_address;
    if (world_chr_man == nullptr)
    {
        return nullptr;
    }

    auto players = world_chr_man->players;
    if (players == nullptr)
    {
        return nullptr;
    }

    return players[index];
}

template <typename F> void hook(F *function, F *override_function, F **original_function)
{
    auto status =
        MH_CreateHook(function, override_function, reinterpret_cast<void **>(original_function));
    if (status != MH_OK)
    {
        throw std::runtime_error(std::string("MH_CreateHook: ") + MH_StatusToString(status));
    }

    status = MH_EnableHook(function);
    if (status != MH_OK)
    {
        throw std::runtime_error(std::string("MH_EnableHook: ") + MH_StatusToString(status));
    }
}

template <typename F> static void unhook(F *function)
{
    auto status = MH_DisableHook(function);
    if (status != MH_OK)
    {
        throw std::runtime_error(std::string("MH_DisableHook: ") + MH_StatusToString(status));
    }

    status = MH_RemoveHook(function);
    if (status != MH_OK)
    {
        throw std::runtime_error(std::string("MH_RemoveHook: ") + MH_StatusToString(status));
    }
}

void GameHook::hook_get_message(GetMessageFn get_message_override)
{
    hook(get_message, get_message_override, &get_message_original);
}

void GameHook::unhook_get_message()
{
    unhook(get_message);
}