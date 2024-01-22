#pragma once
#pragma pack(1)

#include <map>
#include <memory>
#include <span>
#include <string>
#include <vector>

#include "MsgRepository.hpp"
#include "WorldChrMan.hpp"

struct ParamList;

/**
 * Scans the main game executable memory
 */
class GameHook
{
  public:
    void initialize(char const *module_name);
    void *scan(std::vector<int> aob, std::ptrdiff_t offset = 0,
               std::vector<std::pair<ptrdiff_t, ptrdiff_t>> relative_offsets = {});

    struct Param
    {
        std::wstring name;
        std::map<uint64_t, void *> rows;
    };

    std::unique_ptr<Param> get_param(std::wstring const &name);
    Player *get_player(int index);
    void hook_get_message(GetMessageFn *get_message_override);
    void unhook_get_message();
    GetMessageFn *get_message_original;

  private:
    std::span<std::byte> game_module;
    ParamList **param_list_address;
    WorldChrMan **world_chr_man_address;
    MsgRepository **msg_repository_address;
    GetMessageFn *get_message;
};
