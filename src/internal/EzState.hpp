#pragma once

#include <cstddef>

#include "List.hpp"

#pragma pack(push, 1)

namespace EzState
{
struct StateGroup;
struct State;
struct Call;

struct MachineImpl
{
    void **vftable;
    unsigned char unk1[0x20];
    StateGroup *state_group;
    unsigned char unk2[0x110];
};

struct StateGroup
{
    int id;
    unsigned char pad[4];
    CS::List<State> states;
    State *initial_state;
};

struct Transition
{
    State *target_state;
    CS::List<Call> pass_commands;
    CS::List<Transition *> sub_transitions;
    CS::List<unsigned char> evaluator;

    template <size_t evaluator_chars>
    inline Transition(State *target_state, const char (&evaluator_string)[evaluator_chars],
                      CS::List<Call> pass_commands = {})
        : target_state(target_state),
          evaluator(reinterpret_cast<unsigned char *>(const_cast<char *>(evaluator_string)),
                    evaluator_chars - 1),
          pass_commands(pass_commands)
    {
    }
};

struct IntValue
{
    unsigned char start{0x82};
    int value;
    unsigned char end{0xa1};

    inline IntValue(int value) : value(value)
    {
    }
};

struct Command
{
    int bank;
    int id;

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
};

struct CommandArg
{
    const unsigned char *data;
    unsigned int size;
    unsigned char pad[4];

    template <typename T>
    inline CommandArg(const T &value)
        : data(reinterpret_cast<const unsigned char *>(&value)), size(sizeof(value))
    {
    }
};

struct Call
{
    Command command;
    CS::List<CommandArg> args;
};

struct State
{
    int id;
    unsigned char pad[4];
    CS::List<Transition *> transitions;
    CS::List<Call> entry_commands;
    CS::List<Call> exit_commands;
    CS::List<Call> while_commands;
};
};

#pragma pack(pop)