#pragma once

#include <map>
#include <memory>
#include <span>
#include <string>
#include <vector>

struct ParamBank;

/**
 * Scans the main game executable memory
 */
class GameHook
{
  public:
    void initialize(char const *moduleName);
    void *scan(std::vector<int> aob,
               std::vector<std::pair<ptrdiff_t, ptrdiff_t>> relativeOffsets = {});

    struct Param
    {
        std::wstring name;
        std::map<uint64_t, void *> rows;
    };

    std::unique_ptr<Param> getParam(std::wstring const &name);

  private:
    std::span<std::byte> gameModule;
    ParamBank *paramBank;

    static const size_t paramCount = 185;
};
