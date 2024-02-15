#include <cstdint>
#include <iostream>

#include "ModUtils.hpp"
#include "TransmogMessages.hpp"
#include "TransmogShop.hpp"
#include "TransmogTalkScript.hpp"
#include "TransmogVFX.hpp"
#include "ezstate/CustomStates.hpp"
#include "ezstate/EzState.hpp"

using namespace std;
using namespace TransmogTalkScript;

static constexpr int32_t event_text_for_talk_sort_chest_id = 15000395;

namespace
{
extern TransmogMenuNextState transmog_menu_next_state;
extern OpenShopState transmog_head_state;
extern OpenShopState transmog_body_state;
extern OpenShopState transmog_arms_state;
extern OpenShopState transmog_legs_state;
extern ApplySpEffectState disable_transmog_state;

// TalkESD state for the main "Transmogrify armor" menu
TransmogMenuState transmog_menu_state(69000, &transmog_menu_next_state);

// TalkESD state that advances to the next state based on the menu selection
TransmogMenuNextState transmog_menu_next_state(69001, &transmog_head_state, &transmog_body_state,
                                               &transmog_arms_state, &transmog_legs_state,
                                               &disable_transmog_state);

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

// TalkESD state that disables transmogrification
// TODO: this *adds* the speffect. figure out how adding/removing it will work.
ApplySpEffectState disable_transmog_state(69006, TransmogVFX::transmog_speffect_id,
                                          &transmog_menu_state);
}; // namespace

// AddTalkListData(69, "Transmogrify armor", -1)
static EzState::IntValue transmog_talk_list_index = 69;
static EzState::IntValue transmog_menu_text_id =
    TransmogMessages::event_text_for_talk_transmog_armor_id;
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
static bool is_add_talk_list_data_call(EzState::Call *call, int32_t message_id)
{
    return call->command == EzState::Commands::add_talk_list_data && call->args.count == 3 &&
           *reinterpret_cast<const int32_t *>(call->args.elements[1].data + 1) == message_id;
}

/**
 * Return true if the given EzState transition goes to a state that opens the storage chest
 */
static bool is_sort_chest_transition(EzState::Transition *transition)
{
    auto target_state = transition->target_state;
    return target_state != nullptr && target_state->entry_commands.count != 0 &&
           target_state->entry_commands.elements[0].command == EzState::Commands::open_repository;
}

/**
 * Patch the site of grace menu to contain a "Transmogrify armor" option
 */
static void patch_site_of_grace_menu(EzState::StateGroup *state_group)
{
    EzState::State *add_menu_state = nullptr;
    EzState::State *menu_transition_state = nullptr;

    int command_index = -1;
    int transition_index = -1;

    // Look for a state that adds a "Sort chest" menu option, and a state that opens the storage
    // chest.
    for (int i = 0; i < state_group->states.count; i++)
    {
        auto state = &state_group->states.elements[i];

        for (int j = 0; j < state->entry_commands.count; j++)
        {
            auto call = &state->entry_commands.elements[j];
            if (is_add_talk_list_data_call(call, event_text_for_talk_sort_chest_id))
            {
                add_menu_state = state;
                command_index = j + 1;
            }
            else if (is_add_talk_list_data_call(
                         call, TransmogMessages::event_text_for_talk_transmog_armor_id))
            {
                cout << "Not patching state group x" << (0x7fffffff - state_group->id)
                     << ", already patched" << endl;
                return;
            }
        }

        for (int j = 0; j < state->transitions.count; j++)
        {
            auto transition = state->transitions.elements[j];
            if (is_sort_chest_transition(transition))
            {
                menu_transition_state = state;
                transition_index = j + 1;
            }
        }
    }

    // If this is the site of grace menu, patch it
    if (add_menu_state != nullptr && menu_transition_state != nullptr)
    {
        // Add "Transmogrify armor" menu option
        auto &commands = add_menu_state->entry_commands;

        copy(commands.elements, commands.elements + commands.count, patched_commands);
        copy(commands.elements + command_index, commands.elements + commands.count,
             patched_commands + command_index + 1);

        patched_commands[command_index] = main_menu_transmog_command;

        commands.elements = patched_commands;
        commands.count++;

        // Add a transition to the "Transmogrify armor" menu
        auto &transitions = menu_transition_state->transitions;

        copy(transitions.elements, transitions.elements + transitions.count, patched_transitions);
        copy(transitions.elements + transition_index, transitions.elements + transitions.count,
             patched_transitions + transition_index + 1);

        patched_transitions[transition_index] = &main_menu_transmog_transition;

        transitions.elements = patched_transitions;
        transitions.count++;

        // When closing the transmog menu, return to the main site of grace menu
        transmog_menu_next_state.set_return_state(state_group->initial_state);

        cout << "Patched state group x" << (0x7fffffff - state_group->id) << endl;
    }
}

static EzState::StateGroup *state_group = nullptr;

static void (*ezstate_enter_state)(EzState::State *state, EzState::MachineImpl *machine, void *unk);
static void (*ezstate_enter_state_hook)(EzState::State *state, EzState::MachineImpl *machine,
                                        void *unk);
static void ezstate_enter_state_detour(EzState::State *state, EzState::MachineImpl *machine,
                                       void *unk)
{
    auto state_group = machine->state_group;
    if (state == state_group->initial_state)
    {
        patch_site_of_grace_menu(state_group);
    }

    ezstate_enter_state(state, machine, unk);
}

void TransmogTalkScript::initialize()
{
    // TODO aob instead of hardcoded offsets
    ezstate_enter_state_hook =
        ModUtils::hook({.offset = 0x20426a0}, ezstate_enter_state_detour, ezstate_enter_state);
}

void TransmogTalkScript::deinitialize()
{
    ModUtils::unhook(ezstate_enter_state_hook);
}
