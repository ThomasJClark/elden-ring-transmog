#pragma once

#include <cstddef>
#include <cstdint>
#include <initializer_list>

#pragma pack(push, 1)

namespace EzState
{
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

struct StateGroup;
struct State;
struct Call;

struct MachineImpl
{
    void **vftable;
    std::byte unk1[0x20];
    StateGroup *state_group;
    std::byte unk2[0x110];
};

struct StateGroup
{
    int32_t id;
    std::byte pad[4];
    List<State> states;
    State *initial_state;
};

struct Transition
{
    State *target_state;
    List<Call> pass_commands;
    List<Transition *> sub_transitions;
    List<std::byte> evaluator;

    template <size_t evaluator_chars>
    inline Transition(State *target_state, const char (&evaluator_string)[evaluator_chars],
                      List<Call> pass_commands = {})
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

struct Command
{
    int32_t bank;
    int32_t id;

    bool operator==(Command const &other)
    {
        return bank == other.bank && id == other.id;
    }
};

namespace Commands
{
constexpr Command show_shop_message = {.bank = 1, .id = 0xa};
constexpr Command close_shop_message = {.bank = 1, .id = 0xc};
constexpr Command add_talk_list_data = {.bank = 1, .id = 0x13};
constexpr Command clear_talk_list_data = {.bank = 1, .id = 0x14};
constexpr Command open_regular_shop = {.bank = 1, .id = 0x16};
constexpr Command open_repository = {.bank = 1, .id = 0x1e};
constexpr Command give_speffect_to_player = {.bank = 1, .id = 0x3e};
constexpr Command return_value = {.bank = 7, .id = -1};
}; // namespace Commands

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

struct Call
{
    Command command;
    List<CommandArg> args;
};

struct State
{
    int32_t id;
    std::byte pad[4];
    List<Transition *> transitions;
    List<Call> entry_commands;
    List<Call> exit_commands;
    List<Call> while_commands;
};
}; // namespace EzState

#pragma pack(pop)