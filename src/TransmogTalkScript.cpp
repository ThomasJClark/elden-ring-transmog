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

namespace
{
extern TransmogMenuNextState transmog_menu_next_state;
extern OpenShopState transmog_head_state;
extern OpenShopState transmog_body_state;
extern OpenShopState transmog_arms_state;
extern OpenShopState transmog_legs_state;
extern ApplySpEffectState disable_transmog_state;

// TalkESD state for the main "Transmogrify armor" menu
TransmogMenuState transmog_menu_state(69000, transmog_menu_next_state);

// TalkESD state that advances to the next state based on the menu selection
TransmogMenuNextState transmog_menu_next_state(69001, transmog_head_state, transmog_body_state,
                                               transmog_arms_state, transmog_legs_state,
                                               disable_transmog_state);

// TalkESD states to open the menu for each protector category
OpenShopState transmog_head_state(69002, TransmogShop::transmog_head_shop_menu_id,
                                  TransmogShop::transmog_head_shop_menu_id +
                                      TransmogShop::transmog_shop_max_size - 1,
                                  transmog_menu_state);

OpenShopState transmog_body_state(69003, TransmogShop::transmog_body_shop_menu_id,
                                  TransmogShop::transmog_body_shop_menu_id +
                                      TransmogShop::transmog_shop_max_size - 1,
                                  transmog_menu_state);

OpenShopState transmog_arms_state(69004, TransmogShop::transmog_arms_shop_menu_id,
                                  TransmogShop::transmog_arms_shop_menu_id +
                                      TransmogShop::transmog_shop_max_size - 1,
                                  transmog_menu_state);

OpenShopState transmog_legs_state(69005, TransmogShop::transmog_legs_shop_menu_id,
                                  TransmogShop::transmog_legs_shop_menu_id +
                                      TransmogShop::transmog_shop_max_size - 1,
                                  transmog_menu_state);

// TalkESD state that disables transmogrification
// TODO: this *adds* the speffect. figure out how adding/removing it will work.
ApplySpEffectState disable_transmog_state(69006, TransmogVFX::transmog_speffect_id,
                                          transmog_menu_state);

//  EzState::Transition cancel_transition = {
//     DisableTransmog::state,
//     "\x41\xa1",
// };

// todo state 7 (return to main menu?)
}; // namespace

static EzState::IntValue unk = -1;
static EzState::IntValue transmog_talk_list_index = 69;
static EzState::IntValue transmog_menu_text_id =
    TransmogMessages::event_text_for_talk_transmog_armor_id;
static EzState::CommandArg transmog_arg_list[] = {transmog_talk_list_index, transmog_menu_text_id,
                                                  unk};

// AddTalkListData(69, "Transmogrify armor", -1)
static EzState::Command main_menu_transmog_command = {
    .id = EzState::CommandId::add_talk_list_data,
    .args = transmog_arg_list,
};

// GetTalkListEntryResult() == 69
static EzState::Transition main_menu_transmog_transition = {transmog_menu_state,
                                                            "\xaf\x85\x95\xa1"};

static EzState::Command patched_site_of_grace_talk_list_commands[100];

static EzState::Transition *patched_site_of_grace_talk_list_transitions[100];

/**
 * Patch a TalkESD state to include adding a "Transmogrify armor" menu option, if it's a menu
 * that should have this option and it's not already patched.
 *
 * This is done in a hook instead of during initialization because the game reloads talk scripts
 * during loading screens, rather than loading them all once during startup.
 *
 * TODO: test e2e
 * TODO: test grace detection on Reforged and The Convergence
 * TODO: patch fia's mirror
 */
static bool patch_menu_state(EzState::State *state)
{
    auto &commands = state->entry_commands;

    bool is_site_of_grace_menu = false;
    for (int i = 0; i < commands.count; i++)
    {
        auto &command = commands.elements[i];
        if (command.id == EzState::CommandId::add_talk_list_data && command.args.count == 3)
        {
            auto &message_id =
                *reinterpret_cast<const int32_t *>(command.args.elements[1].data + 1);

            // Already patched - don't change anything
            if (message_id == TransmogMessages::event_text_for_talk_transmog_armor_id)
            {
                return false;
            }

            if (message_id == 15000395) // Sort chest
            {
                is_site_of_grace_menu = true;
            }
        }
    }
    if (!is_site_of_grace_menu)
    {
        // Doesn't have a "Sort chest" menu option - don't change anything
        return false;
    }

    // Patch the commands for this state to include a "Transmogrify armor" menu option
    // TODO make immediately after sort chest
    copy(commands.elements, commands.elements + commands.count,
         patched_site_of_grace_talk_list_commands);

    patched_site_of_grace_talk_list_commands[commands.count] = main_menu_transmog_command;

    commands.elements = patched_site_of_grace_talk_list_commands;
    commands.count++;

    return true;
}

/**
 * Patch the TalkESD state that transitions to one of the top-level site of grace menus to include
 * a transition to the transmog menu.
 */
static bool patch_menu_transition_state(EzState::State *state)
{
    auto &transitions = state->transitions;

    int index = -1;
    for (int i = 0; i < transitions.count; i++)
    {
        auto transition = transitions.elements[i];

        // Already patched - don't change anything
        if (transition == &main_menu_transmog_transition)
        {
            return false;
        }

        auto &target_state = transition->target_state;
        if (target_state != nullptr && target_state->entry_commands.count != 0 &&
            target_state->entry_commands.elements[0].bank == 1 &&
            target_state->entry_commands.elements[0].id == EzState::CommandId::open_repository)
        {
            index = i;
        }
    }
    if (index == -1)
    {
        // Doesn't have transition to "Sort chest" action - don't change anything
        return false;
    }

    // Patch the transition for this state
    copy(transitions.elements, transitions.elements + index + 1,
         patched_site_of_grace_talk_list_transitions);

    copy(transitions.elements + index + 1, transitions.elements + transitions.count,
         patched_site_of_grace_talk_list_transitions + index + 2);

    patched_site_of_grace_talk_list_transitions[index + 1] = &main_menu_transmog_transition;

    transitions.elements = patched_site_of_grace_talk_list_transitions;
    transitions.count++;

    return true;
}

static void (*ezstate_enter_state)(EzState::State *state, EzState::MachineImpl *machine, void *unk);
static void (*ezstate_enter_state_hook)(EzState::State *state, EzState::MachineImpl *machine,
                                        void *unk);
static void ezstate_enter_state_detour(EzState::State *state, EzState::MachineImpl *machine,
                                       void *unk)
{
    if (patch_menu_state(state))
    {
        cout << "Patched menu state" << endl;
    }
    else if (patch_menu_transition_state(state))
    {
        cout << "Patched menu transition state" << endl;
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
