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
    inline Transition(State *target_state, const char (&evaluator_string)[evaluator_chars],
                      List<Command> pass_commands = {})
        : target_state(target_state),
          evaluator(reinterpret_cast<std::byte *>(const_cast<char *>(evaluator_string)),
                    evaluator_chars - 1),
          pass_commands(pass_commands)
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

enum class CommandId : int32_t
{
    // Bank 1
    show_shop_message = 0xa,
    close_shop_message = 0xc,
    add_talk_list_data = 0x13,
    clear_talk_list_data = 0x14,
    open_regular_shop = 0x16,
    open_repository = 0x1e,
    give_speffect_to_player = 0x3e,

    // Bank 7
    return_value = -1,
};

struct Command
{
    int32_t bank = 0x1;
    CommandId id;
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