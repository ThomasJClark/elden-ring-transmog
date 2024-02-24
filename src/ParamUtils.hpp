#pragma once

#include <cstdint>
#include <iterator>
#include <sstream>
#include <string>
#include <tga/param_containers.h>

namespace ParamUtils
{
void initialize();

struct ParamListEntry
{
    ParamResCap *param_res_cap;
    std::byte unknown[64];
};

struct ParamList
{
    std::byte unknown[136];
    ParamListEntry entries[186];
};

extern ParamList **param_list_address;

template <typename T> class ParamTableSequence
{
  private:
    ParamTable *param_table;

  public:
    class Iterator
    {
        ParamTable *table;
        ParamRowInfo *row;

      public:
        Iterator(ParamTable *table, ParamRowInfo *row) : table(table), row(row)
        {
        }

        Iterator &operator++()
        {
            row++;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator retval = *this;
            ++(*this);
            return retval;
        }

        bool operator==(Iterator other) const
        {
            return table == other.table && row == other.row;
        }

        bool operator!=(Iterator other) const
        {
            return !(*this == other);
        }

        std::pair<uint64_t, T &> operator*()
        {
            auto base_addr = reinterpret_cast<std::byte *>(table);
            return std::pair<uint64_t, T &>(row->row_id,
                                            *reinterpret_cast<T *>(base_addr + row->param_offset));
        }

        using difference_type = int64_t;
        using value_type = std::pair<uint64_t, T &>;
        using pointer = const std::pair<uint64_t, T &> *;
        using reference = const std::pair<uint64_t, T &> &;
        using iterator_category = std::output_iterator_tag;
    };

    ParamTableSequence(ParamTable *param_table) : param_table(param_table)
    {
    }

    T &operator[](uint64_t id)
    {
        for (auto [row_id, row] : *this)
        {
            if (row_id == id)
            {
                return row;
            }
        }

        std::stringstream ss;
        ss << "Row " << id << " not found";
        throw std::runtime_error(ss.str());
    }

    Iterator begin()
    {
        return Iterator(param_table, &param_table->rows[0]);
    }

    Iterator end()
    {
        return Iterator(param_table, &param_table->rows[param_table->num_rows]);
    }

    size_t size()
    {
        return param_table->num_rows;
    }
};

/**
 * Returns a type-safe STL-style container with the given param
 */
template <typename T> ParamTableSequence<T> get_param(std::wstring name)
{
    auto param_list = *param_list_address;
    if (param_list != nullptr)
    {
        for (int i = 0; i < sizeof(param_list->entries) / sizeof(param_list->entries[0]); i++)
        {
            auto param_res_cap = param_list->entries[i].param_res_cap;
            if (param_res_cap != nullptr)
            {
                std::wstring_view param_name = dlw_c_str(&param_res_cap->param_name);
                if (param_name == name)
                {
                    return ParamTableSequence<T>(param_res_cap->param_header->param_table);
                }
            }
        }
    }

    throw std::runtime_error("Param not found");
}
}; // namespace ParamUtils
