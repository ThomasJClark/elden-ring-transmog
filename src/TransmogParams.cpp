#include <cstdint>
#include <iomanip>
#include <iostream>
#include <tga/paramdefs.h>

#include "GameMemory.hpp"
#include "TransmogParams.hpp"

using namespace TransmogParams;

struct FindReinforceParamProtectorResult
{
    std::int64_t id;
    ReinforceParamProtector *row;
};

struct FindEquipParamProtectorResult
{
    std::int32_t id;
    std::byte unused1[4];
    EquipParamProtector *row;
    std::int32_t base_id;
    std::byte unused2[4];
    FindReinforceParamProtectorResult reinforce_param_protector_result;
    std::uint32_t unknown;
};

struct FindSpEffectParamResult
{
    SpEffectParam *row;
    std::int32_t id;
    std::byte unknown;
};

struct FindSpEffectVfxParamResult; // TODD

struct FindEquipParamGoodsResult; // TODD

typedef void FindEquipParamProtectorFn(FindEquipParamProtectorResult *result, std::uint32_t id);
typedef void FindSpEffectParamFn(FindSpEffectParamResult *result, std::uint32_t id);
typedef void FindSpEffectVfxParamFn(FindSpEffectVfxParamResult *result, std::uint32_t id);
typedef void FindEquipParamGoodsFn(FindEquipParamGoodsResult *result, std::uint32_t id);

static FindEquipParamProtectorFn *find_equip_param_protector_hook;
static FindEquipParamProtectorFn *find_equip_param_protector;
static FindSpEffectParamFn *find_speffect_param_hook;
static FindSpEffectParamFn *find_speffect_param;
static FindSpEffectVfxParamFn *find_speffect_vfx_param_hook;
static FindSpEffectVfxParamFn *find_speffect_vfx_param;
static FindEquipParamGoodsFn *find_equip_param_goods_hook;
static FindEquipParamGoodsFn *find_equip_param_goods;

static EquipParamProtector transmog_head = {0};
static EquipParamProtector transmog_body = {0};
static EquipParamProtector transmog_arms = {0};
static EquipParamProtector transmog_legs = {0};
static ReinforceParamProtector transmog_reinforce_param = {0};
static SpEffectParam transmog_speffect = {0};
static SpEffectVfxParam transmog_head_vfx = {0};
static SpEffectVfxParam transmog_body_vfx = {0};

/**
 * Override find_equip_param_protector) to return the head/body/arms/legs pieces selected by
 * the player for transmogrification
 */
static void find_equip_param_protector_detour(FindEquipParamProtectorResult *result,
                                              std::uint32_t id)
{
    switch (id)
    {
    case transmog_head_id:
        std::cout << "find_equip_param_protector_detour " << id << std::endl;
        result->id = transmog_head_id;
        result->row = &transmog_head;
        result->base_id = transmog_head_id;
        result->reinforce_param_protector_result.id = transmog_reinforce_param_id;
        result->reinforce_param_protector_result.row = &transmog_reinforce_param;
        break;
    case transmog_body_id:
        std::cout << "find_equip_param_protector_detour " << id << std::endl;
        result->id = transmog_body_id;
        result->row = &transmog_body;
        result->base_id = transmog_body_id;
        result->reinforce_param_protector_result.id = transmog_reinforce_param_id;
        result->reinforce_param_protector_result.row = &transmog_reinforce_param;
        break;
    case transmog_arms_id:
        std::cout << "find_equip_param_protector_detour " << id << std::endl;
        result->id = transmog_arms_id;
        result->row = &transmog_arms;
        result->base_id = transmog_arms_id;
        result->reinforce_param_protector_result.id = transmog_reinforce_param_id;
        result->reinforce_param_protector_result.row = &transmog_reinforce_param;
        break;
    case transmog_legs_id:
        std::cout << "find_equip_param_protector_detour " << id << std::endl;
        result->id = transmog_legs_id;
        result->row = &transmog_legs;
        result->base_id = transmog_legs_id;
        result->reinforce_param_protector_result.id = transmog_reinforce_param_id;
        result->reinforce_param_protector_result.row = &transmog_reinforce_param;
        break;
    default:
        find_equip_param_protector(result, id);
    }
}

/**
 * Override find_speffect_param() to return the transmogrification effect
 */
static void find_speffect_param_detour(FindSpEffectParamResult *result, std::uint32_t id)
{
    switch (id)
    {
    case transmog_speffect_id:
        std::cout << "find_speffect_param_detour " << id << std::endl;
        result->id = transmog_speffect_id;
        result->row = &transmog_speffect;
        result->unknown = std::byte(0x04);
        break;
    default:
        find_speffect_param(result, id);
    }
}

/**
 * Override find_speffect_vfx_param() to return the transmogrification VFX
 *
 * TODO never called, need better AOB
 */
static void find_speffect_vfx_param_detour(FindSpEffectVfxParamResult *result, std::uint32_t id)
{
    if (id == transmog_head_vfx_id || id == transmog_body_vfx_id)
    {
        std::byte buffer1[64];
        std::byte buffer2[64];
        for (int i = 0; i < sizeof(buffer1); i++)
        {
            buffer1[i] = std::byte(0x00);
            buffer2[i] = std::byte(0xff);
        }

        find_speffect_vfx_param((FindSpEffectVfxParamResult *)buffer1, id);
        find_speffect_vfx_param((FindSpEffectVfxParamResult *)buffer2, id);

        std::cout << id << std::endl;
        for (int i = 0; i < sizeof(buffer1); i++)
        {
            if (buffer1[i] == buffer2[i])
            {
                std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)buffer1[i];
            }
            else
            {
                std::cout << "??";
            }
            if (i % 16 == 15)
                std::cout << std::endl;
            else if (i % 8 == 7)
                std::cout << "  ";
            else
                std::cout << " ";
        }
        std::cout << std::dec << std::endl;
    }

    find_speffect_vfx_param(result, id);
}

/**
 * Override find_equip_param_goods() to return items shown in the transmog shop menu
 *
 * TODO
 */
static void find_equip_param_goods_detour(FindEquipParamGoodsResult *result, std::uint32_t id)
{
    find_equip_param_goods(result, id);
}

void TransmogParams::hook(GameMemory &game_memory, ParamMap &params)
{
    auto equip_param_protector = params[L"EquipParamProtector"];
    auto reinforce_param_protector = params[L"ReinforceParamProtector"];
    auto speffect_param = params[L"SpEffectParam"];

    // Initialize to bare head/body/arms/legs
    transmog_head = *reinterpret_cast<EquipParamProtector *>(equip_param_protector[10000]);
    transmog_body = *reinterpret_cast<EquipParamProtector *>(equip_param_protector[10100]);
    transmog_arms = *reinterpret_cast<EquipParamProtector *>(equip_param_protector[10200]);
    transmog_legs = *reinterpret_cast<EquipParamProtector *>(equip_param_protector[10300]);

    // Initialize to reinforce level +0 (doesn't matter though because the armor is never equipped)
    transmog_reinforce_param =
        *reinterpret_cast<ReinforceParamProtector *>(reinforce_param_protector[0]);

    // Initialize the speffect from speffect 2 (basically a no-op effect), and override the VFX
    transmog_speffect = *reinterpret_cast<SpEffectParam *>(speffect_param[2]);
    transmog_speffect.vfxId = transmog_head_vfx_id;
    transmog_speffect.vfxId1 = transmog_body_vfx_id;
    transmog_speffect.effectEndurance = -1;
    transmog_speffect.effectTargetSelf = true;
    transmog_speffect.effectTargetFriend = true;
    transmog_speffect.effectTargetEnemy = true;
    transmog_speffect.effectTargetPlayer = true;
    transmog_speffect.effectTargetOpposeTarget = true;
    transmog_speffect.effectTargetFriendlyTarget = true;
    transmog_speffect.effectTargetSelfTarget = true;

    // Add two VFX for transforming the player's head and the rest of their body.
    // transformProtectorId should always be the ID of the head armor, and if
    // isFullBodyTransformProtectorId is true the VFX will instead apply the three other armor
    // pieces.
    transmog_head_vfx.transformProtectorId = transmog_head_id;
    transmog_head_vfx.isFullBodyTransformProtectorId = false;
    transmog_head_vfx.isVisibleDeadChr = true;
    transmog_head_vfx.initSfxId = 523412;
    transmog_head_vfx.initDmyId = 220;

    transmog_body_vfx.transformProtectorId = transmog_head_id;
    transmog_body_vfx.isFullBodyTransformProtectorId = true;
    transmog_body_vfx.isVisibleDeadChr = true;

    find_equip_param_protector_hook = game_memory.hook<>(
        {
            .aob =
                {// lea edx, [r8 + 1]
                 0x41, 0x8d, 0x50, 0x01,
                 // call get_param
                 0xe8, -1, -1, -1, -1,
                 // test rax, rax
                 0x48, 0x85, 0xc0,
                 // jz end_lbl
                 0x0f, 0x84, -1, -1, -1, -1},
            .offset = -0x96,
        },
        find_equip_param_protector_detour, find_equip_param_protector);

    find_speffect_param_hook = game_memory.hook<>(
        {
            .aob =
                {// lea edx, [r8 + 15]
                 0x41, 0x8d, 0x50, 0x0f,
                 // call get_param
                 0xe8, -1, -1, -1, -1,
                 // test rax, rax
                 0x48, 0x85, 0xc0,
                 // jz end_lbl
                 0x0f, 0x84, -1, -1, -1, -1},
            .offset = -0x72,
        },
        find_speffect_param_detour, find_speffect_param);

    find_speffect_vfx_param_hook = game_memory.hook<>(
        {
            .aob =
                {// lea edx, [r8 + 16]
                 0x41, 0x8d, 0x50, 0x10,
                 // call get_param
                 0xe8, -1, -1, -1, -1,
                 // test rax, rax
                 0x48, 0x85, 0xc0,
                 // jz end_lbl
                 0x0f, 0x84, -1, -1, -1, -1},
            .offset = -0x6e,
        },
        find_speffect_vfx_param_detour, find_speffect_vfx_param);

    // find_equip_param_goods_hook = game_memory.hook<>(
    //     {.aob = {0x41, 0x8d, 0x50, 0x03, 0xe8, -1, -1, -1, -1, 0x48, 0x85, 0xc0},
    //      .offset = -0xb0},
    //     find_equip_param_goods_detour, find_equip_param_goods);
}

void TransmogParams::unhook(GameMemory &game_memory)
{
    game_memory.unhook(find_equip_param_protector_hook);
    game_memory.unhook(find_speffect_param_hook);
    game_memory.unhook(find_speffect_vfx_param_hook);
    // game_memory.unhook(find_equip_param_goods_hook);
}

void TransmogParams::set_transmog(EquipParamProtector *equip_param_protector)
{
    if (equip_param_protector->protectorCategory == 0)
    {
        transmog_head = *equip_param_protector;
    }
    else if (equip_param_protector->protectorCategory == 1)
    {
        transmog_body = *equip_param_protector;
    }
    else if (equip_param_protector->protectorCategory == 2)
    {
        transmog_arms = *equip_param_protector;
    }
    else if (equip_param_protector->protectorCategory == 3)
    {
        transmog_legs = *equip_param_protector;
    }
}