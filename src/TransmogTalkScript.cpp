#include <cstdint>
#include <iostream>
#include <map>
#include <string>

#include "ModUtils.hpp"
#include "TransmogMessages.hpp"
#include "TransmogTalkScript.hpp"

struct CSEzStateTalkEvent;

struct EzStateExternalEventTemp;

struct EzStateExternalEventTempArg
{
    std::int32_t value;
    std::byte padding1[4];
    std::int32_t type;
    std::byte padding2[4];
};

struct EzStateExternalEventTempVtable
{
    void *(*FUN_141ffaf70)(EzStateExternalEventTemp *param_1, void *param_2);
    void (*FUN_141ffb060)(EzStateExternalEventTemp *param_1, int param_2, void *param_3,
                          void *param_4);
    int (*GetCommand)(EzStateExternalEventTemp *param_1);
    int (*GetArgCount)(EzStateExternalEventTemp *param_1);
    EzStateExternalEventTempArg *(*GetArg)(EzStateExternalEventTemp *param_1, int param_2);
};

struct EzStateExternalEventTemp
{
    EzStateExternalEventTempVtable *vftable;
    std::int32_t command;
    std::int32_t unk1;
    std::int32_t unk2;
    std::int32_t unk3;
    EzStateExternalEventTempArg args[10];
    std::byte unk4[808];
    std::int32_t arg_count;
};

// struct EsdResCap
// {
//     void **vftable;
// };

// struct EsdRepository
// {
//     void **vftable;
//     std::byte unk1[0x28];
//     std::uint16_t unk2; // 0x100000000;
//     std::byte unk3[0x38];
//     void *unk4;
//     std::byte unk5[0xc];
//     std::uint32_t esd_res_cap_count;
//     EsdResCap **esd_res_caps;
// };

/*
1. TGA > Misc/WIP > Dependencies > FD4Singleton Finder & Symbol Registerer
2. EsdRepository
EsdRepository 7FF661122C48
*/

static std::map<int, std::string> esd_command_names = {
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

static void log_evt(EzStateExternalEventTemp *evt)
{
    auto command_id = evt->vftable->GetCommand(evt);
    auto arg_count = evt->vftable->GetArgCount(evt);

    if (command_id != 62 && command_id != 119 && command_id != 123 && command_id != 138 &&
        command_id != 139)
    {
        if (esd_command_names.contains(command_id))
        {
            std::cout << esd_command_names[command_id];
        }
        else
        {
            std::cout << "UNKNOWN_ESD_COMMAND_" << command_id << std::dec;
        }
        std::cout << "(";
        for (int i = 1; i < arg_count; i++)
        {
            auto arg = evt->vftable->GetArg(evt, i);
            std::cout << "[" << arg->type << "]" << arg->value << (i < arg_count - 1 ? ", " : "");
        }
        std::cout << ")" << std::endl;
    }
}

typedef void ExecuteCommandFn(CSEzStateTalkEvent *self, EzStateExternalEventTemp *evt);

ExecuteCommandFn *execute_command_hook;
ExecuteCommandFn *execute_command;
static void execute_command_detour(CSEzStateTalkEvent *self, EzStateExternalEventTemp *evt)
{
    log_evt(evt);
    execute_command(self, evt);

    if (evt->vftable->GetCommand(evt) == 19 /* AddTalkListData */
        && evt->vftable->GetArg(evt, 2)->value == 15000395 /* Sort chest */)
    {
        auto evt2 = *evt;
        evt2.args[0].value = 199;
        evt2.args[1].value = TransmogMessages::event_text_for_talk_transmog_armor_id;
        log_evt(&evt2);
        execute_command(self, &evt2);
    }
}

/**
 * WIP.
 *
 * Working:
 *   - Inserting commands into existing event scripts
 *
 * Not working:
 *   - State transitions
 *   - New states
 */
void TransmogTalkScript::initialize()
{
    execute_command_hook =
        // TODO aob instead of hardcoded offset
        ModUtils::hook({.offset = 0xe65800}, execute_command_detour, execute_command);
}

void TransmogTalkScript::deinitialize()
{
    ModUtils::unhook(execute_command_hook);
}
