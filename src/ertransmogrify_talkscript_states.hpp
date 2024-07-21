#pragma once

#include "ertransmogrify_messages.hpp"
#include "internal/EzState.hpp"

#pragma pack(push, 1)

/**
 * TalkESD state that applies a speffect to the player and immediately exits
 */
class ApplySpEffectState : public EzState::State
{
  private:
    EzState::IntValue speffect_id_arg;
    EzState::CommandArg arg_list[1] = {speffect_id_arg};
    EzState::Call entry_commands[1] = {{EzState::Commands::give_speffect_to_player, arg_list}};
    EzState::Transition pass_transition;
    EzState::Transition *transitions[1] = {&pass_transition};

  public:
    ApplySpEffectState(int id, int speffect_id, EzState::State *main_menu_state)
        : EzState::State({.id = id, .transitions = transitions, .entry_commands = entry_commands}),
          speffect_id_arg(speffect_id), pass_transition(main_menu_state, "\x41\xa1")
    {
    }
};

/**
 * TalkESD state that opens a regular shop, and exits when the shop menu is closed
 */
class OpenShopState : public EzState::State
{
  private:
    EzState::IntValue begin_arg;
    EzState::IntValue end_arg;
    EzState::CommandArg arg_list[2] = {begin_arg, end_arg};
    EzState::Call entry_commands[1] = {{EzState::Commands::open_regular_shop, arg_list}};
    EzState::Transition close_shop_transition;
    EzState::Transition *transitions[1] = {&close_shop_transition};

  public:
    OpenShopState(int id, int begin_shop_lineup_id, int end_up_lineup_id,
                  EzState::State *main_menu_state)
        : EzState::State({.id = id, .transitions = transitions, .entry_commands = entry_commands}),
          begin_arg(begin_shop_lineup_id), end_arg(end_up_lineup_id),
          close_shop_transition(main_menu_state,
                                // CheckSpecificPersonMenuIsOpen(5, 0) == 1
                                "\x7b"
                                "\x82\x05\x00\x00\x00"
                                "\x82\x00\x00\x00\x00"
                                "\x86"
                                "\x82\x01\x00\x00\x00"
                                "\x95"
                                // CheckSpecificPersonGenericDialogIsOpen(0) == 0
                                "\x7a"
                                "\x82\x00\x00\x00\x00"
                                "\x85"
                                "\x82\x00\x00\x00\x00"
                                "\x95"
                                // &&
                                "\x98"
                                // == 0
                                "\x82\x00\x00\x00\x00"
                                "\x95"
                                "\xa1")
    {
    }
};

/**
 * TalkESD state with "Transmogrify head", "Transmogrify chest", etc. options
 */
class TransmogMenuState : public EzState::State
{
  private:
    EzState::IntValue unk = -1;

    EzState::IntValue transmog_head_talk_list_index = 1;
    EzState::IntValue transmog_head_menu_text_id =
        ertransmogrify::msg::event_text_for_talk_transmog_head;
    EzState::CommandArg transmog_head_arg_list[3] = {transmog_head_talk_list_index,
                                                     transmog_head_menu_text_id, unk};

    EzState::IntValue transmog_chest_talk_list_index = 2;
    EzState::IntValue transmog_chest_menu_text_id =
        ertransmogrify::msg::event_text_for_talk_transmog_chest;
    EzState::CommandArg transmog_chest_arg_list[3] = {transmog_chest_talk_list_index,
                                                      transmog_chest_menu_text_id, unk};

    EzState::IntValue transmog_arms_talk_list_index = 3;
    EzState::IntValue transmog_arms_menu_text_id =
        ertransmogrify::msg::event_text_for_talk_transmog_arms;
    EzState::CommandArg transmog_arms_arg_list[3] = {transmog_arms_talk_list_index,
                                                     transmog_arms_menu_text_id, unk};

    EzState::IntValue transmog_legs_talk_list_index = 4;
    EzState::IntValue transmog_legs_menu_text_id =
        ertransmogrify::msg::event_text_for_talk_transmog_legs;
    EzState::CommandArg transmog_legs_arg_list[3] = {transmog_legs_talk_list_index,
                                                     transmog_legs_menu_text_id, unk};

    EzState::IntValue disable_transmog_talk_list_index = 5;
    EzState::IntValue disable_transmog_menu_text_id =
        ertransmogrify::msg::event_text_for_talk_undo_transmog;
    EzState::CommandArg disable_transmog_arg_list[3] = {disable_transmog_talk_list_index,
                                                        disable_transmog_menu_text_id, unk};

    EzState::IntValue cancel_talk_list_index = 99;
    EzState::IntValue cancel_menu_text_id = ertransmogrify::msg::event_text_for_talk_cancel;
    EzState::CommandArg cancel_arg_list[3] = {cancel_talk_list_index, cancel_menu_text_id, unk};

    EzState::IntValue generic_dialog_shop_message = 0;
    EzState::CommandArg show_shop_message_arg_list[1] = {generic_dialog_shop_message};

    EzState::Call entry_commands[9] = {
        // CloseShopMessage()
        {EzState::Commands::close_shop_message},
        // ClearTalkListData()
        {EzState::Commands::clear_talk_list_data},
        // AddTalkListData(1, "Transmogrify head", -1)
        {EzState::Commands::add_talk_list_data, transmog_head_arg_list},
        // AddTalkListData(2, "Transmogrify chest", -1)
        {EzState::Commands::add_talk_list_data, transmog_chest_arg_list},
        // AddTalkListData(3, "Transmogrify arms", -1)
        {EzState::Commands::add_talk_list_data, transmog_arms_arg_list},
        // AddTalkListData(4, "Transmogrify legs", -1)
        {EzState::Commands::add_talk_list_data, transmog_legs_arg_list},
        // AddTalkListData(5, "Untransmogrify equipped armor", -1)
        {EzState::Commands::add_talk_list_data, disable_transmog_arg_list},
        // AddTalkListData(99, "Cancel", -1)
        {EzState::Commands::add_talk_list_data, cancel_arg_list},
        // ShowShopMessage(1)
        {EzState::Commands::show_shop_message, show_shop_message_arg_list},
    };
    EzState::Transition next_menu_transition;
    EzState::Transition *transitions[1] = {&next_menu_transition};

  public:
    TransmogMenuState(int id, EzState::State *next_state)
        : EzState::State({.id = id, .transitions = transitions, .entry_commands = entry_commands}),
          next_menu_transition(next_state,
                               // CheckSpecificPersonMenuIsOpen(1, 0) == 1
                               "\x7b"
                               "\x82\x01\x00\x00\x00"
                               "\x82\x00\x00\x00\x00"
                               "\x86"
                               "\x82\x01\x00\x00\x00"
                               "\x95"
                               // CheckSpecificPersonGenericDialogIsOpen(0) == 0
                               "\x7a"
                               "\x82\x00\x00\x00\x00"
                               "\x85"
                               "\x82\x00\x00\x00\x00"
                               "\x95"
                               // &&
                               "\x98"
                               // == 0
                               "\x82\x00\x00\x00\x00"
                               "\x95"
                               "\xa1")
    {
    }
};

/**
 * TalkESD state that advances to one of the transmog shop states based on the selected menu
 * entry
 */
class TransmogMenuNextState : public EzState::State
{
  private:
    EzState::Transition select_transmog_head_transition;
    EzState::Transition select_transmog_chest_transition;
    EzState::Transition select_transmog_arms_transition;
    EzState::Transition select_transmog_legs_transition;
    EzState::Transition select_disable_transmog_transition;
    EzState::Transition return_transition;
    EzState::Transition *transitions[6] = {
        &select_transmog_head_transition,    &select_transmog_chest_transition,
        &select_transmog_arms_transition,    &select_transmog_legs_transition,
        &select_disable_transmog_transition, &return_transition};

  public:
    TransmogMenuNextState(int id, EzState::State *transmog_head_state,
                          EzState::State *transmog_chest_state, EzState::State *transmog_arms_state,
                          EzState::State *transmog_legs_state,
                          EzState::State *disable_transmog_state)

        : EzState::State({.id = id, .transitions = transitions, .entry_commands = entry_commands}),
          // GetTalkListEntryResult() == 1
          select_transmog_head_transition(transmog_head_state, "\x57\x84\xa7\x41\x95\xa1"),
          // GetTalkListEntryResult() == 2
          select_transmog_chest_transition(transmog_chest_state, "\xaf\x42\x95\xa1"),
          // GetTalkListEntryResult() == 3
          select_transmog_arms_transition(transmog_arms_state, "\xaf\x43\x95\xa1"),
          // GetTalkListEntryResult() == 4
          select_transmog_legs_transition(transmog_legs_state, "\xaf\x44\x95\xa1"),
          // GetTalkListEntryResult() == 5
          select_disable_transmog_transition(disable_transmog_state, "\xaf\x45\x95\xa1"),
          return_transition(nullptr, "\x41\xa1")
    {
    }

    void set_return_state(EzState::State *state)
    {
        return_transition.target_state = state;
    }
};

#pragma pack(pop)
