#pragma once

#include <map>
#include <memory>
#include <span>
#include <string>
#include <vector>

struct ParamList;

/**
 * Scans the main game executable memory
 */
class GameHook
{
  public:
    void initialize(char const *module_name);
    void *scan(std::vector<int> aob,
               std::vector<std::pair<ptrdiff_t, ptrdiff_t>> relative_offsets = {});

    struct Param
    {
        std::wstring name;
        std::map<uint64_t, void *> rows;
    };

    std::unique_ptr<Param> get_param(std::wstring const &name);

  private:
    std::span<std::byte> game_module;
    ParamList *param_list;
};
