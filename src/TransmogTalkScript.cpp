#include <cstdint>
#include <iostream>

#include "ModUtils.hpp"
#include "ParamUtils.hpp"
#include "TransmogMessages.hpp"
#include "TransmogShop.hpp"
#include "TransmogTalkScript.hpp"
#include "TransmogVFX.hpp"
#include "ezstate/CustomStates.hpp"
#include "internal/EzState.hpp"

using namespace std;
using namespace TransmogTalkScript;

namespace
{
extern TransmogMenuNextState transmog_menu_next_state;
extern OpenShopState transmog_head_state;
extern OpenShopState transmog_body_state;
extern OpenShopState transmog_arms_state;
extern OpenShopState transmog_legs_state;
extern OpenShopState transmog_eyes_state;
extern PassState disable_transmog_state;

// TalkESD state for the main "Transmogrify armor" menu
TransmogMenuState transmog_menu_state(69000, &transmog_menu_next_state);

// TalkESD state that advances to the next state based on the menu selection
TransmogMenuNextState transmog_menu_next_state(69001, &transmog_head_state, &transmog_body_state,
                                               &transmog_arms_state, &transmog_legs_state,
                                               &disable_transmog_state, &transmog_eyes_state);

// TalkESD states to open the menu for each protector category
OpenShopState transmog_head_state(69002, TransmogShop::transmog_head_shop_menu_id,
                                  TransmogShop::transmog_head_shop_menu_id +
                                      TransmogShop::transmog_shop_max_size - 1,
                                  &transmog_menu_state);

OpenShopState transmog_body_state(69003, TransmogShop::transmog_body_shop_menu_id,
                                  TransmogShop::transmog_body_shop_menu_id +
                                      TransmogShop::transmog_shop_max_size - 1,
                                  &transmog_menu_state);

OpenShopState transmog_arms_state(69004, TransmogShop::transmog_arms_shop_menu_id,
                                  TransmogShop::transmog_arms_shop_menu_id +
                                      TransmogShop::transmog_shop_max_size - 1,
                                  &transmog_menu_state);

OpenShopState transmog_legs_state(69005, TransmogShop::transmog_legs_shop_menu_id,
                                  TransmogShop::transmog_legs_shop_menu_id +
                                      TransmogShop::transmog_shop_max_size - 1,
                                  &transmog_menu_state);

OpenShopState transmog_eyes_state(69006, TransmogShop::transmog_eyes_shop_menu_id,
                                  TransmogShop::transmog_eyes_shop_menu_id +
                                      TransmogShop::transmog_shop_max_size - 1,
                                  &transmog_menu_state);

// TalkESD state that disables transmogrification
PassState disable_transmog_state(69007, &transmog_menu_state);
}; // namespace

// AddTalkListData(69, "Transmogrify armor", -1)
static EzState::IntValue transmog_talk_list_index = 69;
static EzState::IntValue transmog_menu_text_id = TransmogMessages::EventTextForTalk::transmog_armor;
static EzState::IntValue unk = -1;
static EzState::CommandArg transmog_arg_list[] = {transmog_talk_list_index, transmog_menu_text_id,
                                                  unk};
static EzState::Call main_menu_transmog_command(EzState::Commands::add_talk_list_data,
                                                transmog_arg_list);

// GetTalkListEntryResult() == 69
static EzState::Transition main_menu_transmog_transition(&transmog_menu_state,
                                                         "\x57\x84\x82\x45\x00\x00\x00\x95\xa1");

static EzState::Call patched_commands[100];
static EzState::Transition *patched_transitions[100];

/**
 * Return true if the given EzState call is AddTalkListData(??, message_id, ??)
 */
static bool is_add_talk_list_data_call(EzState::Call &call, int32_t message_id)
{
    return call.command == EzState::Commands::add_talk_list_data && call.args.count == 3 &&
           *reinterpret_cast<const int32_t *>(call.args[1].data + 1) == message_id;
}

/**
 * Return true if the given EzState transition goes to a state that opens the storage chest
 */
static bool is_sort_chest_transition(const EzState::Transition *transition)
{
    auto target_state = transition->target_state;
    return target_state != nullptr && target_state->entry_commands.count != 0 &&
           target_state->entry_commands[0].command == EzState::Commands::open_repository;
}

static EzState::StateGroup *state_group = nullptr;

static void (*ezstate_enter_state)(EzState::State *state, EzState::MachineImpl *machine, void *unk);

/**
 * Patch the site of grace menu to contain a "Transmogrify armor" option
 */
static void ezstate_enter_state_detour(EzState::State *state, EzState::MachineImpl *machine,
                                       void *unk)
{
    auto state_group = machine->state_group;
    if (state == state_group->initial_state)
    {
        EzState::State *add_menu_state = nullptr;
        EzState::Call *call_iter = nullptr;

        EzState::State *menu_transition_state = nullptr;
        EzState::Transition **transition_iter = nullptr;

        // Look for a state that adds a "Sort chest" menu option, and a state that opens the storage
        // chest.
        for (auto &state : state_group->states)
        {
            for (auto &call : state.entry_commands)
            {
                if (is_add_talk_list_data_call(call,
                                               TransmogMessages::EventTextForTalk::sort_chest))
                {
                    add_menu_state = &state;
                    call_iter = &call + 1;
                }
                else if (is_add_talk_list_data_call(
                             call, TransmogMessages::EventTextForTalk::transmog_armor))
                {
                    cout << "Not patching state group x" << (0x7fffffff - state_group->id)
                         << ", already patched" << endl;
                    return;
                }
            }

            for (auto &transition : state.transitions)
            {
                if (is_sort_chest_transition(transition))
                {
                    menu_transition_state = &state;
                    transition_iter = &transition + 1;
                }
            }
        }

        // If this is the site of grace menu, patch it
        if (add_menu_state != nullptr && menu_transition_state != nullptr)
        {
            // Add "Transmogrify armor" menu option
            auto &commands = add_menu_state->entry_commands;

            int command_index = call_iter - commands.begin();
            copy(commands.begin(), call_iter, patched_commands);
            copy(call_iter, commands.end(), patched_commands + command_index + 1);
            patched_commands[command_index] = main_menu_transmog_command;

            commands.elements = patched_commands;
            commands.count++;

            // Add a transition to the "Transmogrify armor" menu
            auto &transitions = menu_transition_state->transitions;

            int transition_index = transition_iter - transitions.begin();
            copy(transitions.begin(), transition_iter, patched_transitions);
            copy(transition_iter, transitions.end(), patched_transitions + transition_index + 1);
            patched_transitions[transition_index] = &main_menu_transmog_transition;

            transitions.elements = patched_transitions;
            transitions.count++;

            // When closing the transmog menu, return to the main site of grace menu
            transmog_menu_next_state.set_return_state(state_group->initial_state);

            cout << "Patched state group x" << (0x7fffffff - state_group->id) << endl;
        }
    }

    if (state == &disable_transmog_state)
    {
        TransmogVFX::disable_transmog();
        TransmogShop::remove_transmog_goods();
    }

    ezstate_enter_state(state, machine, unk);
}

void TransmogTalkScript::initialize()
{
    ModUtils::hook(
        {
            .aob = "80 7e 18 00"     // cmp byte ptr [rsi + 0x18], 0x0
                   "74 15"           // jz end_label
                   "4c 8d 44 24 40"  // lea r8=>local_4c8, [rsp + 0x40]
                   "48 8b d6"        // mov rdx,rsi
                   "48 8b 4e 20"     // mov rcx,qword ptr [rsi + 0x20]
                   "e8 ?? ?? ?? ??", // call EzState::EnterState
            .offset = 18,
            .relative_offsets = {{1, 5}},
        },
        ezstate_enter_state_detour, ezstate_enter_state);
}
