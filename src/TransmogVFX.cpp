#include <cstdint>
#include <iostream>
#include <tga/param_containers.h>
#include <tga/paramdefs.h>

#include "ModUtils.hpp"
#include "ParamUtils.hpp"
#include "TransmogShop.hpp"
#include "TransmogVFX.hpp"

using namespace TransmogVFX;
using namespace std;

static constexpr int64_t head_protector_offset = 0;
static constexpr int64_t body_protector_offset = 100;
static constexpr int64_t arms_protector_offset = 200;
static constexpr int64_t legs_protector_offset = 300;
static constexpr int32_t undo_transmog_sfx_id = 8020;

#pragma pack(push, 1)
struct FindReinforceParamProtectorResult
{
    int64_t id;
    ReinforceParamProtector *row;
};

struct FindEquipParamProtectorResult
{
    int32_t id;
    byte padding1[4];
    EquipParamProtector *row;
    int32_t base_id;
    byte padding2[4];
    FindReinforceParamProtectorResult reinforce_param_protector_result;
    uint32_t unknown;
};

struct FindSpEffectParamResult
{
    SpEffectParam *row;
    int32_t id;
    byte unknown;
};

struct FindSpEffectVfxParamResult
{
    int32_t id;
    byte padding[4];
    SpEffectVfxParam *row;
    uint16_t unknown;
};

#pragma pack(pop)

typedef int32_t ApplySpEffectFn(CS::PlayerIns *, uint32_t speffect_id, bool unk);
typedef int32_t ClearSpEffectFn(CS::PlayerIns *, uint32_t speffect_id);
typedef void SpawnOneShotVFXOnChrFn(CS::ChrIns *, int32_t dummy_poly_id, int32_t sfx_id, void *unk);

static ApplySpEffectFn *apply_speffect;
static ClearSpEffectFn *clear_speffect;
static SpawnOneShotVFXOnChrFn *spawn_one_shot_sfx_on_chr;

typedef void FindEquipParamProtectorFn(FindEquipParamProtectorResult *result, uint32_t id);
typedef void FindSpEffectParamFn(FindSpEffectParamResult *result, uint32_t id);
typedef void FindSpEffectVfxParamFn(FindSpEffectVfxParamResult *result, uint32_t id);

static FindEquipParamProtectorFn *get_equip_param_protector;
static FindSpEffectParamFn *get_speffect_param;
static FindSpEffectVfxParamFn *get_speffect_vfx_param;

static EquipParamProtector *transmog_head = nullptr;
static EquipParamProtector *transmog_body = nullptr;
static EquipParamProtector *transmog_arms = nullptr;
static EquipParamProtector *transmog_legs = nullptr;
static ReinforceParamProtector transmog_reinforce_param = {0};
static SpEffectParam transmog_head_speffect = {0};
static SpEffectParam transmog_body_speffect = {0};
static SpEffectVfxParam transmog_head_vfx = {0};
static SpEffectVfxParam transmog_body_vfx = {0};

static CS::WorldChrManImp **world_chr_man_addr;

static void get_equip_param_protector_detour(FindEquipParamProtectorResult *result, uint32_t id)
{
    switch (id)
    {
    case transmog_set_id + head_protector_offset:
    case transmog_set_alt_id + head_protector_offset:
        result->id = id;
        result->row = transmog_head;
        result->base_id = id;
        result->reinforce_param_protector_result.id = transmog_reinforce_param_id;
        result->reinforce_param_protector_result.row = &transmog_reinforce_param;
        break;
    case transmog_set_id + body_protector_offset:
    case transmog_set_alt_id + body_protector_offset:
        result->id = id;
        result->row = transmog_body;
        result->base_id = id;
        result->reinforce_param_protector_result.id = transmog_reinforce_param_id;
        result->reinforce_param_protector_result.row = &transmog_reinforce_param;
        break;
    case transmog_set_id + arms_protector_offset:
    case transmog_set_alt_id + arms_protector_offset:
        result->id = id;
        result->row = transmog_arms;
        result->base_id = id;
        result->reinforce_param_protector_result.id = transmog_reinforce_param_id;
        result->reinforce_param_protector_result.row = &transmog_reinforce_param;
        break;
    case transmog_set_id + legs_protector_offset:
    case transmog_set_alt_id + legs_protector_offset:
        result->id = id;
        result->row = transmog_legs;
        result->base_id = id;
        result->reinforce_param_protector_result.id = transmog_reinforce_param_id;
        result->reinforce_param_protector_result.row = &transmog_reinforce_param;
        break;
    default:
        get_equip_param_protector(result, id);
    }
}

static void get_speffect_param_detour(FindSpEffectParamResult *result, uint32_t id)
{
    switch (id)
    {
    case transmog_head_speffect_id:
        result->id = transmog_head_speffect_id;
        result->row = &transmog_head_speffect;
        result->unknown = byte(0x04);
        break;
    case transmog_body_speffect_id:
        result->id = transmog_body_speffect_id;
        result->row = &transmog_body_speffect;
        result->unknown = byte(0x04);
        break;
    default:
        get_speffect_param(result, id);
    }
}

static void get_speffect_vfx_param_detour(FindSpEffectVfxParamResult *result, uint32_t id)
{
    switch (id)
    {
    case transmog_head_vfx_id:
        result->id = transmog_head_vfx_id;
        result->row = &transmog_head_vfx;
        result->unknown = 1;
        break;
    case transmog_body_vfx_id:
        result->id = transmog_body_vfx_id;
        result->row = &transmog_body_vfx;
        result->unknown = 1;
        break;
    default:
        get_speffect_vfx_param(result, id);
    }
}

void TransmogVFX::initialize(CS::WorldChrManImp **world_chr_man_addr)
{
    ::world_chr_man_addr = world_chr_man_addr;

    // Hook get_equip_param_protector() to return the above protectors and reinforce params. These
    // protectors are never equipped, but they're referenced by the transmog VFX params.
    ModUtils::hook(
        {
            .aob = "41 8d 50 01"        // lea edx, [r8 + 1]
                   "e8 ?? ?? ?? ??"     // call SoloParamRepositoryImp::GetParamResCap
                   "48 85 c0"           // test rax, rax
                   "0f 84 ?? ?? ?? ??", // jz end_lbl
            .offset = -150,
        },
        get_equip_param_protector_detour, get_equip_param_protector);

    // Hook get_speffect_param() to return the above speffect
    ModUtils::hook(
        {
            .aob = "41 8d 50 0f"        // lea edx, [r8 + 15]
                   "e8 ?? ?? ?? ??"     // call SoloParamRepositoryImp::GetParamResCap
                   "48 85 c0"           // test rax, rax
                   "0f 84 ?? ?? ?? ??", // jz end_lbl
            .offset = -114,
        },
        get_speffect_param_detour, get_speffect_param);

    // Hook get_speffect_vfx_param() to return the above VFX params
    ModUtils::hook(
        {
            .aob = "41 8d 50 10"    // lea edx, [r8 + 16]
                   "e8 ?? ?? ?? ??" // call SoloParamRepositoryImp::GetParamResCap
                   "48 85 c0"       // test rax, rax
                   "74 ??",         // jz end_lbl
            .offset = -106,
        },
        get_speffect_vfx_param_detour, get_speffect_vfx_param);

    // Locate both ChrIns::ApplyEffect() and ChrIns::ClearSpEffect() from this snippet that manages
    // speffect 4270 (Disable Grace Warp)
    string enable_disable_grace_warp_aob = "45 33 c0"        // xor r8d, r8d
                                           "ba ae 10 00 00"  // mov edx, 4270 ; Disable Grace Warp
                                           "48 8b cf"        // mov rcx, rdi
                                           "e8 ?? ?? ?? ??"  // call ChrIns::ApplyEffect
                                           "eb 16"           // jmp end_label
                                           "e8 ?? ?? ?? ??"  // call HasSpEffect
                                           "84 c0"           // test al, al
                                           "74 0d"           // jz end_label
                                           "ba ae 10 00 00"  // mov edx, 4270 ; Disable Grace Warp
                                           "48 8b cf"        // mov rcx, rdi
                                           "e8 ?? ?? ?? ??"; // call ChrIns::ClearSpEffect

    ptrdiff_t disable_enable_grace_warp_offset =
        ModUtils::scan<byte>({.aob = enable_disable_grace_warp_aob}) -
        ModUtils::scan<byte>({.offset = 0});

    apply_speffect = ModUtils::scan<ApplySpEffectFn>({
        .offset = disable_enable_grace_warp_offset + 11,
        .relative_offsets = {{1, 5}},
    });

    clear_speffect = ModUtils::scan<ClearSpEffectFn>({
        .offset = disable_enable_grace_warp_offset + 35,
        .relative_offsets = {{1, 5}},
    });

    spawn_one_shot_sfx_on_chr = ModUtils::scan<SpawnOneShotVFXOnChrFn>({
        .aob = "45 8b 46 04"    // mov r8d, [r14 + 0x4]
               "41 8b 16"       // mov edx, [r14]
               "48 8b 0b"       // mov rcx, [rbx]
               "e8 ?? ?? ?? ??" // call EMEVD::SpawnOneShotSFXOnChr
               "48 8d 5b 08"    // lea rbx, [rbx + 0x8]
               "48 ff c7"       // inc rdi
               "48 3b fe"       // cmp rdi, rsi
               "75 e5",         // jnz start
        .offset = 10,
        .relative_offsets = {{1, 5}},
    });

    reset_transmog();
}

void TransmogVFX::reset_transmog()
{
    cout << "Resetting transmog params" << endl;

    // Initialize to bare head/body/arms/legs
    auto equip_param_protector = ParamUtils::get_param<EquipParamProtector>(L"EquipParamProtector");
    transmog_head = &equip_param_protector[TransmogShop::bare_head_protector_id];
    transmog_body = &equip_param_protector[TransmogShop::bare_body_protector_id];
    transmog_arms = &equip_param_protector[TransmogShop::bare_arms_protector_id];
    transmog_legs = &equip_param_protector[TransmogShop::bare_legs_protector_id];

    // Initialize to reinforce level +0 (doesn't matter though because the armor is never equipped)
    transmog_reinforce_param =
        ParamUtils::get_param<ReinforceParamProtector>(L"ReinforceParamProtector")[0];

    // Initialize the speffects from speffect 2 (basically a no-op effect), ovrriding the VFX and
    // a few other properties
    auto base_speffect = ParamUtils::get_param<SpEffectParam>(L"SpEffectParam")[2];
    base_speffect.effectEndurance = -1;
    base_speffect.effectTargetSelf = true;
    base_speffect.effectTargetFriend = true;
    base_speffect.effectTargetEnemy = true;
    base_speffect.effectTargetPlayer = true;
    base_speffect.effectTargetOpposeTarget = true;
    base_speffect.effectTargetFriendlyTarget = true;
    base_speffect.effectTargetSelfTarget = true;

    transmog_head_speffect = base_speffect;
    transmog_head_speffect.vfxId = transmog_head_vfx_id;

    transmog_body_speffect = base_speffect;
    transmog_body_speffect.vfxId = transmog_body_vfx_id;

    // Add two VFX for transforming the player's head and the rest of their body.
    // transformProtectorId should always be the ID of the head armor, and if
    // isFullBodyTransformProtectorId is true the VFX will instead apply the three other armor
    // pieces.
    transmog_head_vfx.transformProtectorId = transmog_set_id;
    transmog_head_vfx.isFullBodyTransformProtectorId = false;
    transmog_head_vfx.isVisibleDeadChr = true;
    transmog_head_vfx.materialParamId = -1;

    transmog_body_vfx.transformProtectorId = transmog_set_id;
    transmog_body_vfx.isFullBodyTransformProtectorId = true;
    transmog_body_vfx.isVisibleDeadChr = true;
    transmog_body_vfx.materialParamId = -1;
}

EquipParamProtector *TransmogVFX::set_transmog_protector(int64_t equip_param_protector_id)
{

    FindEquipParamProtectorResult protector_result;
    get_equip_param_protector(&protector_result, equip_param_protector_id);
    if (protector_result.row == nullptr)
    {
        cout << "Protector " << equip_param_protector_id << " doesn't exist" << endl;
        return nullptr;
    }

    auto equip_param_protector = protector_result.row;

    // Set the appropriate slot in the transmog protector set to point to the new armor piece
    switch (protector_result.row->protectorCategory)
    {
    case TransmogShop::protector_category_head:
        transmog_head = protector_result.row;
        cout << "Set head transmog to protector " << protector_result.id << endl;
        break;
    case TransmogShop::protector_category_body:
        transmog_body = protector_result.row;
        cout << "Set body transmog to protector " << protector_result.id << endl;
        break;
    case TransmogShop::protector_category_arms:
        transmog_arms = protector_result.row;
        cout << "Set arms transmog to protector " << protector_result.id << endl;
        break;
    case TransmogShop::protector_category_legs:
        transmog_legs = protector_result.row;
        cout << "Set legs transmog to protector " << protector_result.id << endl;
        break;
    }

    // Toggle the set between the default and alternate IDs to force the game to pick up the new
    // protector
    if (transmog_head_vfx.transformProtectorId == transmog_set_id)
    {
        transmog_head_vfx.transformProtectorId = transmog_set_alt_id;
        transmog_body_vfx.transformProtectorId = transmog_set_alt_id;
    }
    else
    {
        transmog_head_vfx.transformProtectorId = transmog_set_id;
        transmog_body_vfx.transformProtectorId = transmog_set_id;
    }

    // Ensure the main player has the transmog speffect
    auto world_chr_man = *world_chr_man_addr;
    if (protector_result.row->protectorCategory == TransmogShop::protector_category_head)
    {
        apply_speffect(world_chr_man->main_player, TransmogVFX::transmog_head_speffect_id, false);
    }
    else
    {
        apply_speffect(world_chr_man->main_player, TransmogVFX::transmog_body_speffect_id, false);
    }

    // Show a one-shot SFX on the player depending on the armor category
    switch (protector_result.row->protectorCategory)
    {
    case TransmogShop::protector_category_head:
        break;
    case TransmogShop::protector_category_body:
        break;
    case TransmogShop::protector_category_arms:
        break;
    case TransmogShop::protector_category_legs:
        break;
    }

    return equip_param_protector;
}

void TransmogVFX::disable_transmog()
{
    auto world_chr_man = *world_chr_man_addr;
    clear_speffect(world_chr_man->main_player, transmog_head_speffect_id);
    clear_speffect(world_chr_man->main_player, transmog_body_speffect_id);
    spawn_one_shot_sfx_on_chr(world_chr_man->main_player, 900, undo_transmog_sfx_id, nullptr);
}