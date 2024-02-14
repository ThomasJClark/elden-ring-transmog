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

struct ExternalEventTempArg
{
    int32_t value;
    std::byte pad1[4];
    int32_t type;
    std::byte pad2[4];
};

struct ExternalEventTemp;
struct ExternalEventTempVtable
{
    void *(*FUN_141ffaf70)(ExternalEventTemp *param_1, void *param_2);
    void (*FUN_141ffb060)(ExternalEventTemp *param_1, int param_2, void *param_3, void *param_4);
    int (*GetCommand)(ExternalEventTemp *param_1);
    int (*GetArgCount)(ExternalEventTemp *param_1);
    ExternalEventTempArg *(*GetArg)(ExternalEventTemp *param_1, int param_2);
};

struct ExternalEventTemp
{
    ExternalEventTempVtable *vftable;
    int32_t command;
    int32_t unk1;
    int32_t unk2;
    int32_t unk3;
    ExternalEventTempArg args[10];
    std::byte unk4[808];
    int32_t arg_count;
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

struct CSEzStateTalkEvent;
struct MachineImpl
{
    void **vftable;
    std::byte unk1[0xc0];
    CSEzStateTalkEvent **events_begin;
    CSEzStateTalkEvent **events_end;
    std::byte unk2[0x50];
    uint32_t status;
};
}; // namespace EzState

#pragma pack(pop)