#pragma once

#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <tga/dl_types.h>
#include <tga/param_containers.h>

static const size_t g_param_count = 186;

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

typedef std::map<std::wstring, std::map<uint64_t, void *>> ParamMap;

static bool try_get_params(ParamList **param_list_address, ParamMap &params)
{
    auto param_list = *param_list_address;
    if (param_list == nullptr)
    {
        return false;
    }

    auto param_list_entries = param_list->entries;
    for (int i = 0; i < g_param_count; i++)
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

        auto &param = params[dlw_c_str(&param_res_cap->param_name)] = {};
        for (int i = 0; i < param_table_num_rows; i++)
        {
            auto row_info = param_table_rows[i];
            param[row_info.row_id] = param_row_base_addr + row_info.param_offset;
        }
    }
    return true;
}