#pragma once

#include <cstddef>
#include <cstdint>
#include <initializer_list>

#pragma pack(push, 1)

namespace EzState
{
struct MachineImpl;

template <typename T> struct List
{
    T *elements;
    uint32_t count;
    std::byte pad[4];

    inline List() : count(0), elements(nullptr)
    {
    }

    inline List(T *elements, uint32_t count) : elements(elements), count(count)
    {
    }

    template <size_t count> inline List(T (&t)[count]) : elements(t), count(count)
    {
    }
};

struct State;
struct Command;

struct Transition
{
    State *target_state;
    List<Command> pass_commands;
    List<Transition *> sub_transitions;
    List<std::byte> evaluator;

    template <size_t evaluator_chars>
    inline Transition(State &target_state, const char (&evaluator_string)[evaluator_chars])
        : target_state(&target_state),
          evaluator(reinterpret_cast<std::byte *>(const_cast<char *>(evaluator_string)),
                    evaluator_chars - 1)
    {
    }
};

struct IntValue
{
    std::byte start = std::byte(0x82);
    int32_t value;
    std::byte end = std::byte(0xa1);

    inline IntValue(int32_t value) : value(value)
    {
    }
};

struct CommandArg
{
    const std::byte *data;
    uint32_t size;
    std::byte pad[4];

    template <typename T>
    inline CommandArg(const T &value)
        : data(reinterpret_cast<const std::byte *>(&value)), size(sizeof(value))
    {
    }
};

static constexpr int32_t show_shop_message_command_id = 0xa;
static constexpr int32_t close_shop_message_command_id = 0xc;
static constexpr int32_t add_talk_list_data_command_id = 0x13;
static constexpr int32_t clear_talk_list_data_command_id = 0x14;
static constexpr int32_t open_regular_shop_command_id = 0x16;
static constexpr int32_t open_repository_command_id = 0x1e;
static constexpr int32_t give_speffect_to_player_command_id = 0x3e;

struct Command
{
    int32_t bank = 0x1;
    int32_t id;
    List<CommandArg> args;
};

struct State
{
    int32_t id;
    std::byte pad[4];
    List<Transition *> transitions;
    List<Command> entry_commands;
    List<Command> exit_commands;
    List<Command> while_commands;
};

}; // namespace EzState

#pragma pack(pop)