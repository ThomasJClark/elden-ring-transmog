#include <cstdint>
#include <iostream>
#include <random>
#include <tga/param_containers.h>
#include <tga/paramdefs.h>

#include "ModUtils.hpp"
#include "ParamUtils.hpp"
#include "TransmogShop.hpp"
#include "TransmogVFX.hpp"
#include "internal/WorldChrMan.hpp"

using namespace TransmogVFX;
using namespace std;

static constexpr int64_t transmog_speffect_id_begin = 169000000;
static constexpr int64_t transmog_speffect_id_end = 170000000;
static constexpr int64_t transmog_head_vfx_id = 6900100;
static constexpr int64_t transmog_body_vfx_id = 6900101;
static constexpr int64_t transmog_set_id = 69000000;
static constexpr int64_t transmog_set_alt_id = 69010000;
static constexpr int64_t transmog_reinforce_param_id = 0;
static constexpr int64_t head_protector_offset = 0;
static constexpr int64_t body_protector_offset = 100;
static constexpr int64_t arms_protector_offset = 200;
static constexpr int64_t legs_protector_offset = 300;
static constexpr int32_t undo_transmog_sfx_id = 8020;

static int64_t transmog_head_speffect_id;
static int64_t transmog_body_speffect_id;

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

struct InGameStep;

#pragma pack(pop)

typedef int32_t ApplySpEffectFn(CS::ChrIns *, uint32_t speffect_id, bool unk);
typedef int32_t ClearSpEffectFn(CS::ChrIns *, uint32_t speffect_id);
typedef void SpawnOneShotVFXOnChrFn(CS::ChrIns *, int32_t dummy_poly_id, int32_t sfx_id, void *unk);
typedef int GetInventoryIdFn(CS::EquipInventoryData *, int32_t *item_id);

static ApplySpEffectFn *apply_speffect = nullptr;
static ClearSpEffectFn *clear_speffect = nullptr;
static SpawnOneShotVFXOnChrFn *spawn_one_shot_sfx_on_chr = nullptr;
static GetInventoryIdFn *get_inventory_id = nullptr;

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

static inline bool is_head_transmog_enabled()
{
    return transmog_head != nullptr;
}

static inline bool is_body_transmog_enabled()
{
    return transmog_body != nullptr || transmog_arms != nullptr | transmog_legs != nullptr;
}

static inline bool is_transmog_enabled()
{
    return is_head_transmog_enabled() || is_body_transmog_enabled();
}

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
    if (id == transmog_head_speffect_id)
    {
        result->id = transmog_head_speffect_id;
        result->row = &transmog_head_speffect;
        result->unknown = byte(0x04);
    }
    else if (id == transmog_body_speffect_id)
    {
        result->id = transmog_body_speffect_id;
        result->row = &transmog_body_speffect;
        result->unknown = byte(0x04);
    }
    else
    {
        get_speffect_param(result, id);
    }
}

static void get_speffect_vfx_param_detour(FindSpEffectVfxParamResult *result, uint32_t id)
{
    if (id == transmog_head_vfx_id)
    {
        result->id = transmog_head_vfx_id;
        result->row = &transmog_head_vfx;
        result->unknown = 1;
    }
    else if (id == transmog_body_vfx_id)
    {
        result->id = transmog_body_vfx_id;
        result->row = &transmog_body_vfx;
        result->unknown = 1;
    }
    else
    {
        get_speffect_vfx_param(result, id);
    }
}

static void (*copy_player_character_data)(CS::ChrIns *, CS::ChrIns *) = nullptr;

/**
 * When the player character is copied onto an NPC (Mimic Tear), apply the relevant transmog VFX to
 * the NPC
 */
static void copy_player_character_data_detour(CS::ChrIns *target, CS::ChrIns *source)
{
    auto world_chr_man = *world_chr_man_addr;
    if (world_chr_man != nullptr && world_chr_man->main_player == source)
    {
        if (is_head_transmog_enabled())
        {
            cout << "Applying head transmog to Mimic Tear" << endl;
            apply_speffect(target, transmog_head_speffect_id, false);
        }

        if (is_body_transmog_enabled())
        {
            cout << "Applying body transmog to Mimic Tear" << endl;
            apply_speffect(target, transmog_body_speffect_id, false);
        }
    }

    copy_player_character_data(target, source);
}

static bool prev_loaded = false;

static void (*in_game_stay_step_load_finish)(InGameStep *) = nullptr;

/**
 * When the main player is reinitialized, set up the transmog VFX again. This happens
 * whenever you go through a loading screen.
 */
static void in_game_stay_step_load_finish_detour(InGameStep *step)
{
    auto world_chr_man = *world_chr_man_addr;
    auto loaded = world_chr_man != nullptr && world_chr_man->main_player != nullptr;
    if (loaded && !prev_loaded)
    {
        refresh_transmog(false);
    }
    prev_loaded = loaded;

    in_game_stay_step_load_finish(step);
}

void TransmogVFX::initialize()
{
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

    world_chr_man_addr = ModUtils::scan<CS::WorldChrManImp *>({
        .aob = "48 8b 05 ?? ?? ?? ??"  // mov rax, [WorldChrMan]
               "48 85 c0"              // test rax, rax
               "74 0f"                 // jz end_label
               "48 39 88 08 e5 01 00", // cmp [rax + 0x1e508], rcx
        .relative_offsets = {{3, 7}},
    });

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

    get_inventory_id = ModUtils::scan<GetInventoryIdFn>({
        .aob = "48 8d 8f 58 01 00 00" // lea rcx, [rdi + 0x158] ; &equipGameData->equipInventoryData
               "e8 ?? ?? ?? ??"       // call GetInventoryId
               "8b d8"                // mov ebx, eax
               "85 c0"                // test eax, eax
               "78 6a",               // js label
        .offset = 7,
        .relative_offsets = {{1, 5}},
    });

    auto in_game_stay_step_vfptr = ModUtils::scan<byte *>({
        .aob = "b9 28 01 00 00"  // mov ecx, 0x128
               "e8 ?? ?? ?? ??"  // call ReserveMemory
               "48 89 44 24 70"  // mov [rsp + local_res18], rax
               "48 85 c0"        // test rax, rax
               "74 0b"           // jz label
               "48 8b c8"        // mov rcx, rax
               "e8 ?? ?? ?? ??", // call ???
        .offset = 23,
        .relative_offsets = {{1, 5}, {29 + 3, 29 + 7}},
    });

    ModUtils::hook(
        {
            .aob = "c7 44 24 30 00 00 00 00" // mov [rsp + 0x30], 0x0
                   "48 8d 54 24 28"          // lea rdx, [rsp + 0x28]
                   "48 8b 8b 80 05 00 00"    // mov rcx, [rbx + 0x580]
                   "e8 ?? ?? ?? ??",         // call PlayerGameData::PopulatePcInfoBuffer
            .offset = -216,
        },
        copy_player_character_data_detour, copy_player_character_data);

    ModUtils::hook({.offset = in_game_stay_step_vfptr[12] - ModUtils::scan<byte>({})},
                   in_game_stay_step_load_finish_detour, in_game_stay_step_load_finish);

    // Hack for seamless co-op: randomize the SpEffect ID so you don't see the transmog VFX applied
    // to other players. This needs testing, and also obviously isn't completely reliable.
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> speffect_id_dist(transmog_speffect_id_begin,
                                                                    transmog_speffect_id_end - 2);
    transmog_head_speffect_id = speffect_id_dist(rng);
    transmog_body_speffect_id = transmog_head_speffect_id + 1;

    cout << "Randomized SpEffect IDs: " << transmog_head_speffect_id << " / "
         << transmog_body_speffect_id << endl;

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

    // TODO test if this is enough to make it client-side only or if the ID randomization is really
    // necessary
    base_speffect.isDisableNetSync = true;

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

void TransmogVFX::refresh_transmog(bool show_sfx)
{
    auto world_chr_man = *world_chr_man_addr;
    if (world_chr_man == nullptr || world_chr_man->main_player == nullptr)
    {
        return;
    }

    auto equip_param_protector = ParamUtils::get_param<EquipParamProtector>(L"EquipParamProtector");

    bool previous_transmog_enabled = is_transmog_enabled();

    // Hack: skip checking the inventory if the player is not the host, because inventory isn't
    // completely copied over for pseudo-multiplayer, and it's not possible for this to have changed
    // since the last time it was checked. Not sure how this affects seamless co-op.
    auto main_player = world_chr_man->main_player;
    if (main_player->team_type != CS::team_type_host)
    {
        cout << "Player isn't host - skipping transmog application" << endl;
    }
    else
    {
        cout << "Applying transmog from inventory..." << endl;

        auto equip_inventory_data =
            &main_player->player_game_data->equip_game_data.equip_inventory_data;

        // Check which transmog goods are in the player's inventory, and update each slot of the
        // transmog VFX with the corresponding protectors.
        transmog_head = nullptr;
        transmog_body = nullptr;
        transmog_arms = nullptr;
        transmog_legs = nullptr;
        for (auto [protector_id, protector] : equip_param_protector)
        {
            int32_t transmog_item_id =
                TransmogShop::item_type_goods_begin +
                TransmogShop::get_transmog_goods_id_for_protector(protector_id);

            auto inventory_id = get_inventory_id(equip_inventory_data, &transmog_item_id);
            if (inventory_id == -1)
            {
                continue;
            }

            switch (protector.protectorCategory)
            {
            case TransmogShop::protector_category_head:
                transmog_head = &protector;
                cout << "Set head transmog to protector " << protector_id << endl;
                break;
            case TransmogShop::protector_category_body:
                transmog_body = &protector;
                cout << "Set body transmog to protector " << protector_id << endl;
                break;
            case TransmogShop::protector_category_arms:
                transmog_arms = &protector;
                cout << "Set arms transmog to protector " << protector_id << endl;
                break;
            case TransmogShop::protector_category_legs:
                transmog_legs = &protector;
                cout << "Set legs transmog to protector " << protector_id << endl;
                break;
            }
        }
    }

    // Body/arms/legs have to be combined in one spffect. If transmog is enabled on some but not
    // all of them, default the others to the player's current armor
    if (is_body_transmog_enabled())
    {
        auto &chr_asm = main_player->player_game_data->equip_game_data.chr_asm;
        if (transmog_body == nullptr)
        {
            transmog_body = &equip_param_protector[chr_asm.body_protector_id];
            TransmogShop::add_transmog_good(chr_asm.body_protector_id);
            cout << "Defaulting body to protector " << chr_asm.body_protector_id << endl;
        }
        if (transmog_arms == nullptr)
        {
            transmog_arms = &equip_param_protector[chr_asm.arms_protector_id];
            TransmogShop::add_transmog_good(chr_asm.arms_protector_id);
            cout << "Defaulting arms to protector " << chr_asm.arms_protector_id << endl;
        }
        if (transmog_legs == nullptr)
        {
            transmog_legs = &equip_param_protector[chr_asm.legs_protector_id];
            TransmogShop::add_transmog_good(chr_asm.legs_protector_id);
            cout << "Defaulting legs to protector " << chr_asm.legs_protector_id << endl;
        }
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

    // Ensure the main player has the transmog effect(s) enabled if they have anything selected
    if (is_head_transmog_enabled())
    {
        apply_speffect(main_player, transmog_head_speffect_id, false);
    }
    else
    {
        clear_speffect(main_player, transmog_head_speffect_id);
    }

    if (is_body_transmog_enabled())
    {
        apply_speffect(main_player, transmog_body_speffect_id, false);
    }
    else
    {
        clear_speffect(main_player, transmog_body_speffect_id);
    }

    // When transmog is enabled or disabled, show a cool effect on the player
    if (show_sfx)
    {
        if (previous_transmog_enabled != is_transmog_enabled())
        {
            spawn_one_shot_sfx_on_chr(world_chr_man->main_player, 900, undo_transmog_sfx_id,
                                      nullptr);
        }
    }
}
