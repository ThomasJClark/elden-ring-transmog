#include <array>
#include <cstdint>
#include <random>
#include <spdlog/spdlog.h>
#include <tga/param_containers.h>
#include <tga/paramdefs.h>

#include "ertransmogrify_config.hpp"
#include "ertransmogrify_player_state.hpp"
#include "ertransmogrify_shop.hpp"
#include "ertransmogrify_vfx.hpp"
#include "utils/modutils.hpp"
#include "utils/params.hpp"
#include "utils/players.hpp"

using namespace std;
using namespace ertransmogrify;

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

struct FindPostureControlParamProResult
{
    int32_t id;
    byte padding[4];
    PostureControlParam_Pro *row;
};

struct InGameStep;

#pragma pack(pop)

typedef void FindEquipParamProtectorFn(FindEquipParamProtectorResult *result, uint32_t id);
typedef void FindSpEffectParamFn(FindSpEffectParamResult *result, uint32_t id);
typedef void FindSpEffectVfxParamFn(FindSpEffectVfxParamResult *result, uint32_t id);

static FindEquipParamProtectorFn *get_equip_param_protector;
static FindSpEffectParamFn *get_speffect_param;
static FindSpEffectVfxParamFn *get_speffect_vfx_param;

static SpEffectParam dummy_speffect_param;
static ReinforceParamProtector dummy_reinforce_param;

static array<PlayerState, players::net_player_max> player_states;

static void get_equip_param_protector_detour(FindEquipParamProtectorResult *result, uint32_t id)
{
    get_equip_param_protector(result, id);
    if (result->row != nullptr)
    {
        return;
    }

    // Alias set_id/set_alt_id to the protectors chosen for the player's transmog
    for (auto &state : player_states)
    {
        if (state.player == nullptr)
        {
            continue;
        }

        if (id == state.set_id + vfx::head_protector_offset ||
            id == state.set_alt_id + vfx::head_protector_offset)
        {
            result->id = id;
            result->row = state.head_protector;
            result->base_id = id;
            result->reinforce_param_protector_result.id = 0;
            result->reinforce_param_protector_result.row = &dummy_reinforce_param;
            return;
        }

        if (id == state.set_id + vfx::chest_protector_offset ||
            id == state.set_alt_id + vfx::chest_protector_offset)
        {
            result->id = id;
            result->row = state.chest_protector;
            result->base_id = id;
            result->reinforce_param_protector_result.id = 0;
            result->reinforce_param_protector_result.row = &dummy_reinforce_param;
            return;
        }

        if (id == state.set_id + vfx::arms_protector_offset ||
            id == state.set_alt_id + vfx::arms_protector_offset)
        {
            result->id = id;
            result->row = state.arms_protector;
            result->base_id = id;
            result->reinforce_param_protector_result.id = 0;
            result->reinforce_param_protector_result.row = &dummy_reinforce_param;
            return;
        }

        if (id == state.set_id + vfx::legs_protector_offset ||
            id == state.set_alt_id + vfx::legs_protector_offset)
        {
            result->id = id;
            result->row = state.legs_protector;
            result->base_id = id;
            result->reinforce_param_protector_result.id = 0;
            result->reinforce_param_protector_result.row = &dummy_reinforce_param;
            return;
        }
    }
}

static void get_speffect_param_detour(FindSpEffectParamResult *result, uint32_t id)
{
    get_speffect_param(result, id);
    if (result->row != nullptr)
    {
        return;
    }

    // These SpEffect IDs are used to apply the transmog VFX to players. In multiplayer, there's a
    // different effect for each player so they can have different selctions
    for (auto &state : player_states)
    {
        if (state.player == nullptr)
        {
            continue;
        }

        if (id == state.head_speffect_id)
        {
            result->id = state.head_speffect_id;
            result->row = &state.head_speffect;
            result->unknown = byte(0x04);
            return;
        }

        if (id == state.body_speffect_id)
        {
            result->id = state.body_speffect_id;
            result->row = &state.body_speffect;
            result->unknown = byte(0x04);
            return;
        }
    }

    // We just need to ensure these params always exist in order to communicate transmog state
    // between networked players
    if ((id >= vfx::transmog_vfx_speffect_start_id && id < vfx::transmog_vfx_speffect_end_id) ||
        (id >= vfx::transmog_vfx_speffect_start_id && id < vfx::transmog_dummy_speffect_end_id) ||
        id == vfx::undo_transmog_speffect_id)
    {
        result->id = id;
        result->row = &dummy_speffect_param;
        result->unknown = byte(0x04);
        return;
    }
}

static void get_speffect_vfx_param_detour(FindSpEffectVfxParamResult *result, uint32_t id)
{
    get_speffect_vfx_param(result, id);
    if (result->row != nullptr)
    {
        return;
    }

    // These VFX params are used to apply a dragon head / torso transformation to the player
    for (auto &state : player_states)
    {
        if (state.player == nullptr)
        {
            continue;
        }

        if (id == state.head_vfx_id)
        {
            result->id = id;
            result->row = &state.head_vfx;
            result->unknown = 1;
            return;
        }

        if (id == state.body_vfx_id)
        {
            result->id = id;
            result->row = &state.body_vfx;
            result->unknown = 1;
            return;
        }
    }
}

typedef int32_t GetPostureControlInnerFn(FindPostureControlParamProResult *, int8_t, int32_t);
typedef int32_t GetPostureControlFn(CS::ChrAsm **, int8_t, int32_t, int32_t);

static GetPostureControlInnerFn *get_posture_control_inner = nullptr;
static GetPostureControlFn *get_posture_control = nullptr;

/**
 * Override the player's resting posture to be based on the chest transmog instead of the actual
 * equipped armor. This technically has a mechanical effect (the player's hurtbox) but it looks
 * goofy if this isn't tied to the selected chest transmog.
 */
static int32_t get_posture_control_detour(CS::ChrAsm **chr_asm_ptr, int8_t unk1,
                                          int32_t posture_control_group, int32_t unk2)
{
    auto &chr_asm = **chr_asm_ptr;

    auto get_player_state = [chr_asm]() {
        // This unused field is used to link Mimic Tears' ChrAsm to a PlayerState
        if (chr_asm.unused >= 0 && chr_asm.unused < player_states.size())
        {
            return player_states.begin() + chr_asm.unused;
        }

        // Otherwise, check if this ChrAsm is exactly equal to one of the players. This function
        // is passed a copy of ChrAsm, not a pointer to the one in the player's PlayerIns.
        for (auto state = player_states.begin(); state != player_states.end(); state++)
        {
            if (state->player != nullptr &&
                memcmp(&state->player->player_game_data->equip_game_data.chr_asm, &chr_asm,
                       sizeof(CS::ChrAsm)) == 0)
            {
                return state;
            }
        }

        return player_states.end();
    };

    // If this ChrAsm either belongs to a player with transmog enabled, or a Mimic Tear copied from
    // such a player, apply the posture control from the transmogrified chest.
    auto state = get_player_state();
    if (state != player_states.end() && state->player != nullptr &&
        state->is_body_transmog_enabled())
    {
        auto posture_control_id =
            100 * posture_control_group + state->chest_protector->postureControlId;
        auto posture_control_param =
            params::get_param<PostureControlParam_Pro>(L"PostureControlParam_Pro");
        FindPostureControlParamProResult posture_control_result = {
            .id = posture_control_id,
            .row = &posture_control_param[posture_control_id],
        };

        return get_posture_control_inner(&posture_control_result, unk1, unk2);
    }

    return get_posture_control(chr_asm_ptr, unk1, posture_control_group, unk2);
}

static void (*copy_player_character_data)(CS::PlayerIns *, CS::PlayerIns *) = nullptr;

/**
 * When a player character is copied onto an NPC (Mimic Tear), apply the relevant transmog VFX to
 * the NPC
 */
static void copy_player_character_data_detour(CS::PlayerIns *target, CS::PlayerIns *source)
{
    copy_player_character_data(target, source);

    for (int i = 0; i < player_states.size(); i++)
    {
        auto &state = player_states[i];
        if (state.player == source)
        {
            if (state.is_head_transmog_enabled())
            {
                spdlog::info("Applying head transmog to Mimic Tear");
                players::apply_speffect(target, state.head_speffect_id, false);
            }

            if (state.is_body_transmog_enabled())
            {
                spdlog::info("Applying body transmog to Mimic Tear");
                players::apply_speffect(target, state.body_speffect_id, false);
            }

            // Store a value in this unused field in order to link this Mimic Tear's ChrAsm to the
            // player. The posture adjustment function, which doesn't have access to the PlayerIns
            // object, checks this to associate the Mimic Tear with a PlayerState.
            target->player_game_data->equip_game_data.chr_asm.unused = i;
        }
    }
}

static void (*in_game_stay_step_load_finish)(InGameStep *) = nullptr;

static uint64_t update_count = 0;
static uint64_t update_interval = 2; // TODO: 1 might be fine

/**
 * Update the main player and each network player's transmog state as part of the main game loop
 */
static void in_game_stay_step_load_finish_detour(InGameStep *step)
{
    if (update_count++ % update_interval == 0)
    {
        auto main_player = players::get_main_player();
        auto net_players = players::get_net_players();

        auto &main_player_state = player_states[0];
        main_player_state.player = main_player;
        main_player_state.refresh_transmog();

        if (main_player != nullptr && net_players != nullptr)
        {
            for (int i = 1; i < players::net_player_max; i++)
            {
                auto &state = player_states[i];
                auto player = net_players == nullptr ? nullptr : net_players[i].player;
                auto prev_player = state.player;
                state.player = player;
                state.refresh_transmog();

                if (prev_player == nullptr && state.player != nullptr)
                {
                    spdlog::info("Net player joined - forcing refresh of main player transmog");
                    player_states[0].remove_transmog();
                }
            }
        }
    }

    in_game_stay_step_load_finish(step);
}

void vfx::initialize()
{
    // Hook get_equip_param_protector() to return the above protectors and reinforce params. These
    // protectors are never equipped, but they're referenced by the transmog VFX params.
    modutils::hook(
        {
            .aob = "41 8d 50 01"        // lea edx, [r8 + 1]
                   "e8 ?? ?? ?? ??"     // call SoloParamRepositoryImp::GetParamResCap
                   "48 85 c0"           // test rax, rax
                   "0f 84 ?? ?? ?? ??", // jz end_lbl
            .offset = -150,
        },
        get_equip_param_protector_detour, get_equip_param_protector);

    // Hook get_speffect_param() to return the above speffect
    modutils::hook(
        {
            .aob = "41 8d 50 0f"        // lea edx, [r8 + 15]
                   "e8 ?? ?? ?? ??"     // call SoloParamRepositoryImp::GetParamResCap
                   "48 85 c0"           // test rax, rax
                   "0f 84 ?? ?? ?? ??", // jz end_lbl
            .offset = -114,
        },
        get_speffect_param_detour, get_speffect_param);

    // Hook get_speffect_vfx_param() to return the above VFX params
    modutils::hook(
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
    auto disable_enable_grace_warp_address = modutils::scan<byte>({
        .aob = "45 33 c0"        // xor r8d, r8d
               "ba ae 10 00 00"  // mov edx, 4270 ; Disable Grace Warp
               "48 8b cf"        // mov rcx, rdi
               "e8 ?? ?? ?? ??"  // call ChrIns::ApplyEffect
               "eb 16"           // jmp end_label
               "e8 ?? ?? ?? ??"  // call HasSpEffect
               "84 c0"           // test al, al
               "74 0d"           // jz end_label
               "ba ae 10 00 00"  // mov edx, 4270 ; Disable Grace Warp
               "48 8b cf"        // mov rcx, rdi
               "e8 ?? ?? ?? ??", // call ChrIns::ClearSpEffect
    });

    auto in_game_stay_step_vfptr = modutils::scan<void *>({
        .aob = "33 ed"                 // xor ebp, ebp
               "48 89 ab b8 00 00 00"  // mov qword ptr [rbx + 0xb8], rbp
               "48 89 ab c0 00 00 00"  // mov qword ptr [rbx + 0xc0], rbp
               "48 89 ab c8 00 00 00"  // mov qword ptr [rbx + 0xc8], rbp
               "48 89 ab d0 00 00 00"  // mov qword ptr [rbx + 0xd0], rbp
               "48 89 ab d8 00 00 00"  // mov qword ptr [rbx + 0xd8], rbp
               "48 89 ab e0 00 00 00"  // mov qword ptr [rbx + 0xe0], rbp
               "48 89 ab e8 00 00 00"  // mov qword ptr [rbx + 0xe8], rbp
               "48 89 ab f0 00 00 00"  // mov qword ptr [rbx + 0xf0], rbp
               "48 89 ab f8 00 00 00"  // mov qword ptr [rbx + 0xf8], rbp
               "48 89 ab 00 01 00 00"  // mov qword ptr [rbx + 0x100], rbp
               "48 89 ab 08 01 00 00"  // mov qword ptr [rbx + 0x108], rbp
               "40 88 ab 10 01 00 00", // mov byte ptr [rbx + 0x110], bpl
        .offset = -10,
        .relative_offsets = {{3, 7}},
    });

    if (config::transmog_affects_posture)
    {
        auto get_posture_control_original = modutils::scan<void>({
            .aob = "0f b6 80 27 01 00 00" // movzx eac, [rax + 0x127]
                   "?? ?? ?? ?? ??"       // ...
                   "6b d6 64"             // imul edx, esi, 100
                   "48 8d 4c 24 20"       // lea rcx, postureControlResult.id
                   "4c 89 74 24 28"       // mov postureControlResult.row, r14
                   "03 d0"                // add edx, eax
                   "89 54 24 20",         // mov postureControlResult.id, edx
            .offset = -127,
        });

        // First Person Souls has a memory hack that changes this function. If we can't find it,
        // let's assume another mod is doing something more important with it.
        if (get_posture_control_original != nullptr)
        {
            modutils::hook({.address = get_posture_control_original}, get_posture_control_detour,
                           get_posture_control);

            get_posture_control_inner = modutils::scan<GetPostureControlInnerFn>({
                .address = (byte *)get_posture_control_original + 175,
                .relative_offsets = {{1, 5}},
            });
        }
        else
        {
            spdlog::warn("Couldn't find GetPostureControl(), skipping posture fix");
        }
    }

    modutils::hook(
        {
            .aob = "c7 44 24 30 00 00 00 00" // mov [rsp + 0x30], 0x0
                   "48 8d 54 24 28"          // lea rdx, [rsp + 0x28]
                   "48 8b 8b 80 05 00 00"    // mov rcx, [rbx + 0x580]
                   "e8 ?? ?? ?? ??",         // call PlayerGameData::PopulatePcInfoBuffer
            .offset = -216,
        },
        copy_player_character_data_detour, copy_player_character_data);

    modutils::hook({.address = in_game_stay_step_vfptr[4]}, in_game_stay_step_load_finish_detour,
                   in_game_stay_step_load_finish);

    // Initialize to reinforce level +0 (doesn't matter though because the armor is never equipped)
    dummy_reinforce_param =
        params::get_param<ReinforceParamProtector>(L"ReinforceParamProtector")[0];

    // Initialize the speffects from speffect 2 (basically a no-op effect), overiding the VFX and
    // a few other properties
    dummy_speffect_param = params::get_param<SpEffectParam>(L"SpEffectParam")[2];
    dummy_speffect_param.effectEndurance = -1;
    dummy_speffect_param.effectTargetSelf = true;
    dummy_speffect_param.effectTargetFriend = true;
    dummy_speffect_param.effectTargetEnemy = true;
    dummy_speffect_param.effectTargetPlayer = true;
    dummy_speffect_param.effectTargetOpposeTarget = true;
    dummy_speffect_param.effectTargetFriendlyTarget = true;
    dummy_speffect_param.effectTargetSelfTarget = true;
    dummy_speffect_param.vfxId = -1;

    for (int i = 0; i < players::net_player_max; i++)
    {
        auto &state = player_states[i];

        state.player = nullptr;

        // Initialize the transmog set to empty. If the player selects a transmog, these will point
        // to the selected protector params.
        state.set_id = transmog_set_base_id + 10000 * i;
        state.set_alt_id = transmog_set_alt_base_id + 10000 * i;

        state.clear_transmog_protectors();

        // Add two VFX for transforming the player's head and the rest of their body into the above
        // armor set.
        state.head_vfx_id = transmog_head_base_vfx_id + i;
        state.head_vfx.transformProtectorId = state.set_id;
        state.head_vfx.isFullBodyTransformProtectorId = false;
        state.head_vfx.isVisibleDeadChr = true;
        state.head_vfx.materialParamId = -1;

        state.body_vfx_id = transmog_body_base_vfx_id + i;
        state.body_vfx.transformProtectorId = state.set_id;
        state.body_vfx.isFullBodyTransformProtectorId = true;
        state.body_vfx.isVisibleDeadChr = true;
        state.body_vfx.materialParamId = -1;

        // Add two SpEffects that enable the above VFX
        if (i == 0)
        {
            // For the main player, pick two random IDs (an even and odd one) to avoid conflicts in
            // Seamless Co-op
            random_device dev;
            mt19937 rng(dev());
            uniform_int_distribution<mt19937::result_type> speffect_id_dist(
                transmog_vfx_speffect_start_id, transmog_vfx_speffect_end_id - 1);

            state.set_vfx_speffect_ids(speffect_id_dist(rng));
            spdlog::info("SpEffect IDs = {} {}", state.head_speffect_id, state.body_speffect_id);
        }
        else
        {
            // For networked players, the SpEffects will be chosen in the other player's game
            state.head_speffect_id = -1;
            state.body_speffect_id = -1;
        }

        state.head_speffect = dummy_speffect_param;
        state.head_speffect.vfxId = state.head_vfx_id;

        state.body_speffect = dummy_speffect_param;
        state.body_speffect.vfxId = state.body_vfx_id;
    }
}
