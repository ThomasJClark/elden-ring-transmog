#pragma once

#include <map>
#include <span>
#include <string>
#include <vector>

#include "params.h"

/**
 * Scans the main game executable memory
 */
class GameHook
{
  public:
    void initialize(char const *module_name);

    void *scan(std::vector<int> aob, std::vector<std::pair<ptrdiff_t, ptrdiff_t>> relative_offsets = {});

    std::map<std::wstring, ParamResCap *> getParams();

  public:
    std::span<std::byte> game_module;
};
