#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

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

// TalkESD state for the main "Transmogrify armor" menu
TransmogMenuState transmog_menu_state(69000, transmog_menu_next_state);

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

// TalkESD state that advances to the next state based on the menu selection
TransmogMenuNextState transmog_menu_next_state(69001, transmog_head_state, transmog_body_state,
                                               transmog_arms_state, transmog_legs_state,
                                               disable_transmog_state);

// todo state 7 (return to main menu?)

// TODO class ----
}; // namespace

typedef void ExecuteCommandFn(CSEzStateTalkEvent *self, EzState::ExternalEventTemp *evt);

static map<int, string> esd_command_names = {
    {1, "TalkToPlayer"},
    {5, "RemoveMyAggro"},
    {8, "ForceEndTalk"},
    {9, "ClearTalkProgressData"},
    {10, "ShowShopMessage"},
    {11, "SetEventFlag"},
    {12, "CloseShopMessage"},
    {17, "OpenGenericDialog"},
    {18, "ForceCloseGenericDialog"},
    {19, "AddTalkListData"},
    {20, "ClearTalkListData"},
    {22, "OpenRegularShop"},
    {24, "OpenEnhanceShop"},
    {28, "OpenMagicEquip"},
    {30, "OpenRepository"},
    {31, "OpenSoul"},
    {35, "ClearTalkActionState"},
    {38, "SetUpdateDistance"},
    {39, "ClearPlayerDamageInfo"},
    {42, "StartBonfireAnimLoop"},
    {43, "EndBonfireKindleAnimLoop"},
    {46, "OpenSellShop"},
    {47, "ChangePlayerStat"},
    {48, "OpenEquipmentChangeOfPurposeShop"},
    {49, "CombineMenuFlagAndEventFlag"},
    {52, "PlayerEquipmentQuantityChange"},
    {57, "ShuffleRNGSeed"},
    {58, "SetRNGSeed"},
    {59, "ReplaceTool"},
    {62, "GiveSpEffectToPlayer"},
    {67, "ForceCloseMenu"},
    {68, "SetTalkTime"},
    {71, "ReportConversationEndToHavokBehavior"},
    {76, "OpenConversationChoicesMenu"},
    {80, "StopEventAnimWithoutForcingConversationEnd"},
    {81, "OpenCharaMakeMenu"},
    {82, "OpenChooseQuantityDialog"},
    {83, "ClearQuantityValueOfChooseQuantityDialog"},
    {100, "SetWorkValue"},
    {101, "UpdatePlayerRespawnPoint"},
    {102, "SetMessageTagValue"},
    {103, "TurnCharacterToFaceEntity"},
    {104, "AwardItemLot"},
    {105, "OpenEstusAllotMenu"},
    {108, "EstusAllocationUpdate"},
    {109, "BonfireActivation"},
    {111, "OpenTranspositionShop"},
    {113, "ReallocateAttributes"},
    {123, "RequestAnimation"},
    {130, "OpenPhysickMenu"},
    {131, "AcquireGesture"},
    {135, "OpenDragonCommunionShop"},
    {142, "OpenTailoringShop"},
    {143, "OpenAshOfWarShop"},
    {144, "OpenPuppetShop"},
    {146, "OpenDupeShop"},
    {147, "SetEventFlagValue"},
    {148, "OpenChampionsEquipmentShop"},
    {0, "Unused_DebugEvent"},
    {2, "Unused_InvokeEvent"},
    {3, "Unused_StopAttacking"},
    {4, "Unused_Attack"},
    {6, "Unused_DisplayOneLineHelp"},
    {7, "Unused_TurnToFacePlayer"},
    {13, "Unused_OpenCampMenu"},
    {14, "Unused_CloseCampMenu"},
    {15, "Unused_ChangeTeamType"},
    {16, "Unused_SetDefaultTeamType"},
    {21, "Unused_RequestMoviePlayback"},
    {23, "Unused_OpenRepairShop"},
    {25, "Unused_OpenHumanityMenu"},
    {26, "Unused_OpenMagicShop"},
    {27, "Unused_OpenMiracleShop"},
    {29, "Unused_OpenMiracleEquip"},
    {32, "Unused_CloseMenu"},
    {33, "Unused_SetEventFlagRange"},
    {34, "Unused_OpenDepository"},
    {36, "Unused_ClearTalkDisabledState"},
    {37, "Unused_SetTalkDisableStateMaxDuration"},
    {41, "Unused_StartWarpMenuInit"},
    {50, "Unused_RequestSave"},
    {51, "Unused_ChangeMotionOffsetID"},
    {53, "Unused_RequestUnlockTrophy"},
    {54, "Unused_EnterBonfireEventRange"},
    {55, "Unused_SetAquittalCostMessageTag"},
    {56, "Unused_SubtractAcquittalCostFromPlayerSouls"},
    {60, "Unused_BreakCovenantPledge"},
    {61, "Unused_PlayerRespawn"},
    {63, "Unused_ShowGrandioseTextPresentation"},
    {64, "Unused_AddIzalithRankingPoints"},
    {65, "Unused_OpenItemAcquisitionMenu"},
    {66, "Unused_AcquireGestureOld"},
    {69, "Unused_CollectJustPyromancyFlame"},
    {70, "Unused_OpenArenaRanking"},
    {106, "Unused_AddRankingPoints"},
    {107, "Unused_OpenHollowLevelUpMenu"},
    {112, "Unused_SetBonfireMenuData"},
    {114, "Unused_UndeadMatch"},
    {115, "Unused_OpenBonfireMenu"},
};

typedef void ExecuteCommandFn(CSEzStateTalkEvent *self, EzState::ExternalEventTemp *evt);

ExecuteCommandFn *execute_command_hook;
ExecuteCommandFn *execute_command;
static void execute_command_detour(CSEzStateTalkEvent *self, EzState::ExternalEventTemp *evt)
{
    execute_command(self, evt);
}

std::string bytes_to_str(const byte *bytes, size_t count)
{
    stringstream s;
    for (int i = 0; i < count; i++)
    {
        s << setfill('0') << setw(2) << hex << right << (int)bytes[i];
        if (i != count - 1)
            s << " ";
    }
    return s.str();
}

void log_ezstate_command_list(EzState::List<EzState::Command> const &command_list)
{
    if (command_list.count != 0)
    {
        cout << endl;
        for (int i = 0; i < command_list.count; i++)
        {
            auto &command = command_list.elements[i];

            cout << "    - ";

            if (command.bank == 1 && esd_command_names.contains(command.id))
            {
                cout << esd_command_names[command.id];
            }
            else
            {
                cout << "cmd_" << command.bank << "_" << command.id << dec;
            }
            cout << "(";
            for (int i = 0; i < command.args.count; i++)
            {
                auto arg = command.args.elements[i];
                cout << bytes_to_str(arg.data, arg.size);
                if (i < command.args.count - 1)
                    cout << ", ";
            }
            cout << ")" << endl;
        }
    }
    else
    {
        cout << "<empty>" << endl;
    }
}

void log_ezstate_state(EzState::State const &state)
{
    cout << "state:" << endl;
    cout << "  id: " << state.id << endl;
    cout << "  transitions:" << endl;
    for (int i = 0; i < state.transitions.count; i++)
    {
        auto transition = state.transitions.elements[i];

        if (transition->target_state != nullptr)
            cout << "    - target_state: " << transition->target_state->id << endl;
        else
            cout << "    - target_state: null" << endl;

        if (transition->pass_commands.count != 0)
        {
            cout << "      pass:" << endl;
            for (int i = 0; i < transition->pass_commands.count; i++)
                cout << "        - command " << transition->pass_commands.elements[i].id << "["
                     << transition->pass_commands.elements[i].bank << "]" << endl;
        }
        else
            cout << "      pass: <empty>" << endl;

        cout << "      evaluator: "
             << bytes_to_str(transition->evaluator.elements, transition->evaluator.count) << endl;
    }

    cout << "  entry: ";
    log_ezstate_command_list(state.entry_commands);

    cout << "  while: ";
    log_ezstate_command_list(state.while_commands);

    cout << "  exit: ";
    log_ezstate_command_list(state.exit_commands);
}

static EzState::Command patched_site_of_grace_talk_list_commands[100];
static EzState::IntValue unk = -1;
static EzState::IntValue transmog_talk_list_index = 69;
static EzState::IntValue transmog_menu_text_id =
    TransmogMessages::event_text_for_talk_transmog_armor_id;
static EzState::CommandArg transmog_arg_list[] = {transmog_talk_list_index, transmog_menu_text_id,
                                                  unk};

/**
 * Patch a TalkESD state to include adding a "Transmogirfy armor" menu option, if it's a menu
 * that should have this option and it's not already patched.
 *
 * This is done in a hook instead of during initialization because I believe the game reads talk
 * scripts from the disk when reloading, and I have no idea how to patch that routine.
 *
 * TODO: test grace detection on Reforged and The Convergence
 * TODO: state transition
 */
static bool patch_menu_state(EzState::State *state)
{
    bool is_site_of_grace_menu = false;

    for (int i = 0; i < state->entry_commands.count; i++)
    {
        auto &command = state->entry_commands.elements[i];
        if (command.id == EzState::add_talk_list_data_command_id && command.args.count == 3)
        {
            auto &message_id =
                *reinterpret_cast<const int32_t *>(command.args.elements[1].data + 1);

            if (message_id == 15000395) // Sort chest
            {
                is_site_of_grace_menu = true;
            }
            else if (message_id == TransmogMessages::event_text_for_talk_transmog_armor_id)
            {
                // Already patched - don't change anything
                return false;
            }
        }
    }

    if (is_site_of_grace_menu)
    {
        copy(state->entry_commands.elements,
             state->entry_commands.elements + state->entry_commands.count,
             patched_site_of_grace_talk_list_commands);

        patched_site_of_grace_talk_list_commands[state->entry_commands.count++] = {
            .id = EzState::add_talk_list_data_command_id,
            .args = transmog_arg_list,
        };

        state->entry_commands.elements = patched_site_of_grace_talk_list_commands;

        return true;
    }

    return false;
}

static void (*FUN_1420426a0)(EzState::State *state, EzState::MachineImpl *machine, void *unk);
static void (*FUN_1420426a0_hook)(EzState::State *state, EzState::MachineImpl *machine, void *unk);
static void FUN_1420426a0_detour(EzState::State *state, EzState::MachineImpl *machine, void *unk)
{
    if (patch_menu_state(state))
    {
        cout << "Patched menu state" << endl;
        log_ezstate_state(*state);
    }

    FUN_1420426a0(state, machine, unk);
}

void TransmogTalkScript::initialize()
{
    // TODO aob instead of hardcoded offsets
    execute_command_hook =
        ModUtils::hook({.offset = 0xe65800}, execute_command_detour, execute_command);
    FUN_1420426a0_hook = ModUtils::hook({.offset = 0x20426a0}, FUN_1420426a0_detour, FUN_1420426a0);

    log_ezstate_state(transmog_menu_state);
    log_ezstate_state(transmog_menu_next_state);
    log_ezstate_state(transmog_head_state);
    log_ezstate_state(transmog_body_state);
    log_ezstate_state(transmog_arms_state);
    log_ezstate_state(transmog_legs_state);
    log_ezstate_state(disable_transmog_state);
}

void TransmogTalkScript::deinitialize()
{
    ModUtils::unhook(execute_command_hook);
    ModUtils::unhook(FUN_1420426a0_hook);
}
