#include "ertransmogrify_talkscript.hpp"
#include "ertransmogrify_config.hpp"
#include "ertransmogrify_messages.hpp"
#include "ertransmogrify_shop.hpp"
#include "ertransmogrify_vfx.hpp"
#include "utils/talkscript_utils.hpp"

#include <elden-x/ezstate/ezstate.hpp>
#include <elden-x/ezstate/talk_commands.hpp>
#include <elden-x/utils/modutils.hpp>

#include <spdlog/spdlog.h>

using namespace ertransmogrify;

namespace {
extern talkscript_menu_state transmog_menu_state;

auto transmog_option =
    talkscript_menu_option{69, msg::event_text_for_talk_transmog_armor, &transmog_menu_state};

auto transmog_head_menu_state = open_regular_shop_state{
    shop::transmog_head_shop_menu_id,
    shop::transmog_head_shop_menu_id + shop::transmog_shop_max_size - 1, &transmog_menu_state};

auto transmog_chest_menu_state = open_regular_shop_state{
    shop::transmog_chest_shop_menu_id,
    shop::transmog_chest_shop_menu_id + shop::transmog_shop_max_size - 1, &transmog_menu_state};

auto transmog_arms_menu_state = open_regular_shop_state{
    shop::transmog_arms_shop_menu_id,
    shop::transmog_arms_shop_menu_id + shop::transmog_shop_max_size - 1, &transmog_menu_state};

auto transmog_legs_menu_state = open_regular_shop_state{
    shop::transmog_legs_shop_menu_id,
    shop::transmog_legs_shop_menu_id + shop::transmog_shop_max_size - 1, &transmog_menu_state};

auto undo_transmog_state =
    give_speffect_to_player_state{vfx::undo_transmog_speffect_id, &transmog_menu_state};

talkscript_menu_state transmog_menu_state = {{
    {1, ertransmogrify::msg::event_text_for_talk_transmog_head, &transmog_head_menu_state},
    {2, ertransmogrify::msg::event_text_for_talk_transmog_chest, &transmog_chest_menu_state},
    {3, ertransmogrify::msg::event_text_for_talk_transmog_arms, &transmog_arms_menu_state},
    {4, ertransmogrify::msg::event_text_for_talk_transmog_legs, &transmog_legs_menu_state},
    {5, ertransmogrify::msg::event_text_for_talk_undo_transmog, &undo_transmog_state},
    {99, ertransmogrify::msg::event_text_for_talk_cancel, nullptr, true},
}};

}
static auto patched_events = std::array<er::ezstate::event, 100>{};
static auto patched_transitions = std::array<er::ezstate::transition *, 100>{};

/**
 * Return true if the given EzState event is AddTalkListData(??, message_id, ??)
 */
static bool is_add_talk_list_data_event(er::ezstate::event &event, int message_id) {
    return event.command == er::talk_command::add_talk_list_data && event.args.size() == 3 &&
           get_ezstate_int_value(event.args[1]) == message_id;
}

/**
 * Return true if the given EzState event is the site of grace "Sort chest" menu option
 */
static bool is_sort_chest_event(er::ezstate::event &event) {
    if (event.command == er::talk_command::add_talk_list_data) {
        auto message_id = get_ezstate_int_value(event.args[1]);
        return message_id == ertransmogrify::msg::event_text_for_talk_sort_chest;
    }

    if (event.command == er::talk_command::add_talk_list_data_if ||
        event.command == er::talk_command::add_talk_list_data_alt) {
        auto message_id = get_ezstate_int_value(event.args[2]);
        return message_id == ertransmogrify::msg::event_text_for_talk_sort_chest;
    }

    return false;
}

static bool is_grace_state_group(er::ezstate::state_group *state_group) {
    for (auto &state : state_group->states) {
        for (auto &event : state.entry_events) {
            if (is_sort_chest_event(event)) {
                return true;
            }
        }
    }
    return false;
}

/**
 * Return true if the given EzState transition goes to a state that opens the storage chest
 */
static bool is_sort_chest_transition(const er::ezstate::transition *transition) {
    auto target_state = transition->target_state;
    return target_state && !target_state->entry_events.empty() &&
           target_state->entry_events[0].command == er::talk_command::open_repository;
}

/**
 * Patch the site of grace menu to contain a "Transmogrify armor" option
 */
static bool patch_state_group(er::ezstate::state_group *state_group) {
    er::ezstate::state *add_menu_state = nullptr;
    er::ezstate::state *menu_transition_state = nullptr;

    int event_index = -1;
    int transition_index = -1;

    // Look for a state that adds a "Sort chest" menu option, and a state that opens the storage
    // chest.
    for (auto &state : state_group->states) {
        for (int i = 0; i < state.entry_events.size(); i++) {
            auto &event = state.entry_events[i];
            if (is_sort_chest_event(event)) {
                add_menu_state = &state;
                event_index = i;
            } else if (event.command == er::talk_command::add_talk_list_data) {
                auto message_id = get_ezstate_int_value(event.args[1]);
                if (message_id == ertransmogrify::msg::event_text_for_talk_transmog_armor) {
                    SPDLOG_DEBUG("Not patching state group x{}, already patched",
                                 0x7fffffff - state_group->id);
                    return false;
                }
            }
        }

        for (int i = 0; i < state.transitions.size(); i++) {
            if (is_sort_chest_transition(state.transitions[i])) {
                menu_transition_state = &state;
                transition_index = i;
                break;
            }
        }
    }

    if (event_index == -1 || transition_index == -1) {
        return false;
    }

    SPDLOG_INFO("Patching state group x{}", 0x7fffffff - state_group->id);

    // Add an "Apply dyes" menu option
    auto &events = add_menu_state->entry_events;
    std::copy(events.begin(), events.end(), patched_events.begin());
    patched_events[events.size()] = {er::talk_command::add_talk_list_data, transmog_option.args};
    events = {patched_events.data(), events.size() + 1};

    // Add a transition to the "Apply dyes" menu
    auto &transitions = menu_transition_state->transitions;
    std::copy(transitions.begin(), transitions.begin() + transition_index,
              patched_transitions.begin());
    std::copy(transitions.begin() + transition_index, transitions.end(),
              patched_transitions.begin() + transition_index + 1);
    patched_transitions[transition_index] = &transmog_option.transition;
    transitions = {patched_transitions.data(), transitions.size() + 1};

    return true;
}

static void (*ezstate_enter_state)(er::ezstate::state *state,
                                   er::ezstate::machine *machine,
                                   void *unk);

static void ezstate_enter_state_detour(er::ezstate::state *state,
                                       er::ezstate::machine *machine,
                                       void *unk) {
    if (ertransmogrify::config::patch_grace_talk_script) {
        if (is_grace_state_group(machine->state_group)) {
            if (state == machine->state_group->initial_state &&
                patch_state_group(machine->state_group)) {
                transmog_menu_state.opts.back().transition.target_state =
                    machine->state_group->initial_state;
            }
        }
    }

    for (auto &event : state->entry_events) {
        if (event.command == er::talk_command::open_chara_make_menu) {
            vfx::is_in_chr_make_menu = true;
            break;
        } else if (event.command == er::talk_command::clear_talk_list_data) {
            vfx::is_in_chr_make_menu = false;
            break;
        }
    }

    ezstate_enter_state(state, machine, unk);
}

void talkscript::initialize() {
    modutils::hook(
        {
            .aob = "80 7e 18 00"      // cmp byte ptr [rsi + 0x18], 0x0
                   "74 15"            // jz end_label
                   "4c 8d 44 24 40"   // lea r8=>local_4c8, [rsp + 0x40]
                   "48 8b d6"         // mov rdx,rsi
                   "48 8b 4e 20"      // mov rcx,qword ptr [rsi + 0x20]
                   "e8 ?? ?? ?? ??",  // call EzState::EnterState
            .offset = 18,
            .relative_offsets = {{1, 5}},
        },
        ezstate_enter_state_detour, ezstate_enter_state);
}
