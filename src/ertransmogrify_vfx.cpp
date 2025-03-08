#include "ertransmogrify_vfx.hpp"
#include "ertransmogrify_config.hpp"
#include "ertransmogrify_local_player.hpp"
#include "ertransmogrify_net.hpp"
#include "ertransmogrify_shop.hpp"
#include "utils/players.hpp"

#include <elden-x/chr/world_chr_man.hpp>
#include <elden-x/params/param_table.hpp>
#include <elden-x/utils/modutils.hpp>
#include <spdlog/spdlog.h>

#include <chrono>
#include <map>
#include <random>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace ertransmogrify;

unsigned short head_transmog_state_info = 998;
unsigned short body_transmog_state_info = 999;

struct reinforce_param_protector_result_st
{
    long long id;
    er::paramdef::reinforce_param_protector_st *row;
};

struct equip_param_protector_result_st
{
    int id;
    er::paramdef::equip_param_protector_st *row;
    int base_id;
    reinforce_param_protector_result_st reinforce_param_protector_result;
    unsigned int unknown;
};

struct speffect_param_result_st
{
    enum class unkc_en : unsigned char
    {
        unk = 4
    };

    er::paramdef::sp_effect_param_st *row;
    int id;
    unkc_en unkc;
};

struct speffect_vfx_param_result_st
{
    int id;
    er::paramdef::sp_effect_vfx_param_st *row;
    unsigned short unknown;
};

struct posture_control_param_pro_result_st
{
    int id;
    er::paramdef::posture_control_param_pro_st *row;
};

struct InGameStep;

static auto dummy_speffect_param = er::paramdef::sp_effect_param_st{
    .effectEndurance = -1.f,
    .fallDamageRate = 1.f,
    .soulRate = 1.f,
    .equipWeightChangeRate = 1.f,
    .allItemWeightChangeRate = 1.f,
    .soulStealRate = 1.f,
    .lifeReductionRate = 1.f,
    .hpRecoverRate = 1.f,
    .effectTargetSelf = true,
    .effectTargetFriend = true,
    .effectTargetEnemy = true,
    .effectTargetPlayer = true,
    .vowType0 = true,
    .vowType1 = true,
    .vowType2 = true,
    .vowType3 = true,
    .vowType4 = true,
    .vowType5 = true,
    .vowType6 = true,
    .vowType7 = true,
    .vowType8 = true,
    .vowType9 = true,
    .vowType10 = true,
    .vowType11 = true,
    .vowType12 = true,
    .vowType13 = true,
    .vowType14 = true,
    .vowType15 = true,
    .effectTargetOpposeTarget = true,
    .effectTargetFriendlyTarget = true,
    .effectTargetSelfTarget = true,
};

// The category used by transformation effects in the vanilla game (dragon forms and such)
static constexpr unsigned char vfx_play_category_transmog = 8;

struct player_context_st
{
    er::CS::PlayerIns *player;

    // Randomized speffects used to apply the head and body VFX, so networked players can have
    // different params
    long long head_speffect_id{-1};
    long long body_speffect_id{-1};

    // Current IDs of the player's selected transmog armor
    ertransmogrify::vfx::player_state_st state;

    // Fake speffect params returned to apply the above transmog using transformProtectorId
    er::paramdef::sp_effect_param_st head_speffect;
    er::paramdef::sp_effect_param_st body_speffect;
    er::paramdef::sp_effect_vfx_param_st head_vfx{.playCategory = vfx_play_category_transmog,
                                                  .playPriority = 1,
                                                  .isFullBodyTransformProtectorId = false,
                                                  .isVisibleDeadChr = true};
    er::paramdef::sp_effect_vfx_param_st body_vfx{.playCategory = vfx_play_category_transmog,
                                                  .playPriority = 1,
                                                  .isFullBodyTransformProtectorId = false,
                                                  .isVisibleDeadChr = true};
};

static auto player_context_buffer = std::array<player_context_st, 128>{};
static auto player_contexts = std::span{player_context_buffer.data(), 0};

// Patched versions of VFX params. We don't directly edit these in memory to avoid triggering
// Seamless Co-op matchingmaking, which doesn't allow connections between players with different
// params.
static std::map<unsigned int, er::paramdef::sp_effect_vfx_param_st> patched_vfx_params;

/**
 * Utility to alias fake EquipParamProtector IDs to armor pieces chosen by the player
 */
static int get_real_protector_id(int transmog_protector_id)
{
    for (auto &context : player_contexts)
    {
        if (transmog_protector_id ==
            context.head_vfx.transformProtectorId + vfx::head_protector_offset)
            return context.state.head_protector_id;
        else if (transmog_protector_id ==
                 context.body_vfx.transformProtectorId + vfx::chest_protector_offset)
            return context.state.chest_protector_id;
        else if (transmog_protector_id ==
                 context.body_vfx.transformProtectorId + vfx::arms_protector_offset)
            return context.state.arms_protector_id;
        else if (transmog_protector_id ==
                 context.body_vfx.transformProtectorId + vfx::legs_protector_offset)
            return context.state.legs_protector_id;
    }
    return -1;
};

static void (*get_equip_param_protector)(equip_param_protector_result_st *, unsigned int);
static void get_equip_param_protector_detour(equip_param_protector_result_st *result,
                                             unsigned int id)
{
    get_equip_param_protector(result, id);
    if (result->row != nullptr)
    {
        return;
    }

    auto real_protector_id = get_real_protector_id(id);
    if (real_protector_id > 0)
    {
        get_equip_param_protector(result, real_protector_id);
        if (result->row)
        {
            result->id = id;
            result->base_id = id;
        }
    }
}

static void (*get_speffect_param)(speffect_param_result_st *, unsigned int);
static void get_speffect_param_detour(speffect_param_result_st *result, unsigned int id)
{
    get_speffect_param(result, id);
    if (result->row != nullptr)
    {
        return;
    }

    if (id >= ertransmogrify::vfx::transmog_vfx_speffect_start_id &&
        id < ertransmogrify::vfx::transmog_vfx_speffect_end_id)
    {
        // Even if this isn't a known transmog SpEffect, if it's within the correct range return
        // something so it can be applied to net players before their transmog state has synced.
        result->id = id;
        result->row = &dummy_speffect_param;
        result->unkc = speffect_param_result_st::unkc_en::unk;

        // These SpEffect IDs are used to apply the transmog VFX to players. In multiplayer, there's
        // a different effect for each player so they can have different selections
        for (auto &context : player_contexts)
        {
            if (id == context.head_speffect_id)
            {
                result->row = &context.head_speffect;
                return;
            }

            if (id == context.body_speffect_id)
            {
                result->row = &context.body_speffect;
                return;
            }
        }
    }

    if (id == ertransmogrify::vfx::undo_transmog_speffect_id)
    {
        result->id = id;
        result->row = &dummy_speffect_param;
        result->unkc = speffect_param_result_st::unkc_en::unk;
        return;
    }
}

static void (*get_speffect_vfx_param)(speffect_vfx_param_result_st *, unsigned int);
static void get_speffect_vfx_param_detour(speffect_vfx_param_result_st *result, unsigned int id)
{
    auto patched_param = patched_vfx_params.find(id);
    if (patched_param != patched_vfx_params.end())
    {
        result->id = patched_param->first;
        result->row = &patched_param->second;
        result->unknown = 1;
        return;
    }

    get_speffect_vfx_param(result, id);
    if (result->row != nullptr)
    {
        return;
    }

    // These VFX params are used to apply the transmog VFX to the player
    for (auto &context : player_contexts)
    {
        if (id == context.head_speffect.vfxId)
        {
            result->id = id;
            result->row = &context.head_vfx;
            result->unknown = 1;
            return;
        }

        if (id == context.body_speffect.vfxId)
        {
            result->id = id;
            result->row = &context.body_vfx;
            result->unknown = 1;
            return;
        }
    }
}

typedef int GetPostureControlInnerFn(posture_control_param_pro_result_st *, signed char, int);

static GetPostureControlInnerFn *get_posture_control_inner = nullptr;

/**
 * Override the player's resting posture to be based on the chest transmog instead of the actual
 * equipped armor. This technically has a mechanical effect (the player's hurtbox) but it looks
 * goofy if this isn't tied to the selected chest transmog.
 */
static int (*get_posture_control)(er::CS::ChrAsm *&, signed char, int, int);
static int get_posture_control_detour(er::CS::ChrAsm *&chr_asm, signed char unk1,
                                      int posture_control_group, int unk2)
{
    auto get_player_context = [chr_asm]() {
        // This unused field is used to link Mimic Tears' ChrAsm to a PlayerState
        if (chr_asm->gear_param_ids.unused4 >= 0 &&
            chr_asm->gear_param_ids.unused4 < player_contexts.size())
        {
            return player_contexts.begin() + chr_asm->gear_param_ids.unused4;
        }

        // Otherwise, check if this ChrAsm is exactly equal to one of the players. This function
        // is passed a copy of ChrAsm, not a pointer to the one in the player's PlayerIns.
        for (auto context = player_contexts.begin(); context != player_contexts.end(); context++)
        {
            if (context->player && memcmp(&context->player->game_data->equip_game_data.chr_asm,
                                          chr_asm, sizeof(er::CS::ChrAsm)) == 0)
            {
                return context;
            }
        }

        return player_contexts.end();
    };

    // If this ChrAsm either belongs to a player with transmog enabled, or a Mimic Tear copied from
    // such a player, apply the posture control from the transmogrified chest.
    auto context = get_player_context();

    if (context != player_contexts.end() && context->player &&
        context->state.chest_protector_id > 0)
    {
        auto &chest_protector =
            er::param::EquipParamProtector[context->state.chest_protector_id].first;
        auto posture_control_id = 100 * posture_control_group + chest_protector.postureControlId;
        posture_control_param_pro_result_st posture_control_result = {
            .id = posture_control_id,
            .row = &er::param::PostureControlParam_Pro[posture_control_id].first,
        };

        return get_posture_control_inner(&posture_control_result, unk1, unk2);
    }

    return get_posture_control(chr_asm, unk1, posture_control_group, unk2);
}

/**
 * When a player character is copied onto an NPC (Mimic Tear), apply the relevant transmog VFX to
 * the NPC
 */
static void (*copy_player_character_data)(er::CS::PlayerIns *, er::CS::PlayerIns *);
static void copy_player_character_data_detour(er::CS::PlayerIns *target, er::CS::PlayerIns *source)
{
    copy_player_character_data(target, source);

    int i = 0;
    for (auto &context : player_contexts)
    {
        if (context.player == source)
        {
            if (context.state.head_protector_id > 0 && context.head_speffect_id > 0)
            {
                players::apply_speffect(target, context.head_speffect_id, false);
            }

            if ((context.state.chest_protector_id > 0 || context.state.arms_protector_id > 0 ||
                 context.state.legs_protector_id > 0) &&
                context.body_speffect_id > 0)
            {
                players::apply_speffect(target, context.body_speffect_id, false);
            }

            // Store a value in this unused field in order to link this Mimic Tear's ChrAsm to the
            // player. The posture adjustment function, which doesn't have access to the PlayerIns
            // object, checks this to associate the Mimic Tear with a PlayerState.
            auto &chr_asm = target->game_data->equip_game_data.chr_asm;
            chr_asm.gear_param_ids.unused4 = i;
            return;
        }
        i++;
    }
}

static bool update_player_context(player_context_st &context,
                                  const ertransmogrify::vfx::player_state_st &new_state, int index)
{
    if (new_state.head_protector_id != context.state.head_protector_id ||
        new_state.chest_protector_id != context.state.chest_protector_id ||
        new_state.arms_protector_id != context.state.arms_protector_id ||
        new_state.legs_protector_id != context.state.legs_protector_id)
    {
        // Toggle between the main and alternate set IDs when the protectors change, so the game
        // looks up the params again. Both of these IDs alias to the same armor set which is
        // dynamically updated.
        int diff = ertransmogrify::vfx::transmog_set_alt_base_id -
                   ertransmogrify::vfx::transmog_set_base_id;
        if (context.head_vfx.transformProtectorId >= ertransmogrify::vfx::transmog_set_alt_base_id)
        {
            context.head_vfx.transformProtectorId -= diff;
            context.body_vfx.transformProtectorId -= diff;
        }
        else
        {
            context.head_vfx.transformProtectorId += diff;
            context.body_vfx.transformProtectorId += diff;
        }

        auto is_head_applied = new_state.head_protector_id > 0;
        auto is_body_applied = new_state.chest_protector_id > 0 ||
                               new_state.arms_protector_id > 0 || new_state.legs_protector_id > 0;

        // Set the VFX for the speffects applied to the player if they're applied. We could also
        // just add and remove the speffects to toggle the VFX, but speffect removal is not synced
        // in co-op.
        context.head_speffect.vfxId =
            is_head_applied ? ertransmogrify::vfx::transmog_head_base_vfx_id + index : -1;
        context.body_speffect.vfxId =
            is_body_applied ? ertransmogrify::vfx::transmog_body_base_vfx_id + index : -1;

        // Check for any protectors that apply purely cosmetic VFX, such as Midras Gaze in
        // The Convergence. We should apply these effects when the protectors are chosen as
        // transmogs, but we shouldn't include any effects that have mechanical benefits.
        auto get_cosmetic_vfx_id = [](std::initializer_list<int> protector_ids) {
            for (auto protector_id : protector_ids)
            {
                if (protector_id == -1)
                    continue;

                auto &protector = er::param::EquipParamProtector[protector_id].first;
                for (auto sp_effect_id :
                     {protector.residentSpEffectId, protector.residentSpEffectId2,
                      protector.residentSpEffectId3})
                {
                    if (sp_effect_id == -1)
                        continue;

                    auto &sp_effect = er::param::SpEffectParam[sp_effect_id].first;
                    for (auto vfx_id :
                         {sp_effect.vfxId, sp_effect.vfxId1, sp_effect.vfxId2, sp_effect.vfxId3,
                          sp_effect.vfxId4, sp_effect.vfxId5, sp_effect.vfxId6, sp_effect.vfxId7})
                    {
                        if (vfx_id == -1)
                            continue;

                        auto &vfx = er::param::SpEffectVfxParam[vfx_id].first;
                        if (!vfx.isSilence)
                            return vfx_id;
                    }
                }
            }
            return -1;
        };

        context.head_speffect.vfxId1 = get_cosmetic_vfx_id({
            new_state.head_protector_id,
        });

        context.body_speffect.vfxId1 = get_cosmetic_vfx_id({
            new_state.chest_protector_id,
            new_state.arms_protector_id,
            new_state.legs_protector_id,
        });

        context.state = new_state;

        return true;
    }

    return false;
}

/**
 * Returns true if the local player's transmog shouldn't be displayed on other player's screens
 * and vice versa
 */
static bool is_client_side_only()
{
    auto result = ertransmogrify::config::client_side_only;

    // F8 temporarily inverts this setting so you can peek at other players' actual armor
    if (GetAsyncKeyState(VK_F8) & 0x8000)
    {
        return !result;
    }

    return result;
}

static void update_player_contexts()
{
    static auto empty_state = ertransmogrify::vfx::player_state_st{};

    static auto clock = std::chrono::steady_clock{};
    static auto next_net_update_time = std::chrono::steady_clock::time_point{};

    auto world_chr_man = er::CS::WorldChrManImp::instance();
    if (!world_chr_man)
    {
        return;
    }

    player_contexts = std::span{player_contexts.data(), world_chr_man->player_chr_set.capacity()};

    auto &local_player_context = player_contexts[0];
    local_player_context.player = world_chr_man->main_player;

    // Update the local player VFX based on their transmog selections
    auto state = ertransmogrify::local_player::get_local_player_state(world_chr_man->main_player);
    auto any_changed = update_player_context(local_player_context, state, 0);

    // Apply or remove the transmog SpEffects on the main player based on their selections
    if (local_player_context.player && any_changed)
    {
        if (local_player_context.state.head_protector_id > 0)
        {
            players::apply_speffect(local_player_context.player,
                                    local_player_context.head_speffect_id, false);
        }
        else
        {
            players::clear_speffect(local_player_context.player,
                                    local_player_context.head_speffect_id);
        }

        if (local_player_context.state.chest_protector_id > 0 ||
            local_player_context.state.arms_protector_id > 0 ||
            local_player_context.state.legs_protector_id > 0)
        {
            players::apply_speffect(local_player_context.player,
                                    local_player_context.body_speffect_id, false);
        }
        else
        {
            players::clear_speffect(local_player_context.player,
                                    local_player_context.body_speffect_id);
        }
    }

    static bool prev_client_side_only = false;
    bool client_side_only = is_client_side_only();
    if (client_side_only != prev_client_side_only)
    {
        any_changed = true;
        prev_client_side_only = client_side_only;
    }
    if (client_side_only)
    {
        state = empty_state;
    }

    // Send the main player's transmog state to other players in the session when it changes,
    // and also periodically just in case
    auto now = clock.now();
    if (any_changed || now >= next_net_update_time)
    {
        next_net_update_time = now + std::chrono::seconds(5);
        ertransmogrify::net::send_messages(state);
    }

    ertransmogrify::net::receive_messages();
    for (int i = 1; i < player_contexts.size(); i++)
    {
        auto &player_context = player_context_buffer[i];
        player_context.player = world_chr_man->player_chr_set.at(i);
        if (!player_context.player || !player_context.player->session_holder.network_session)
        {
            continue;
        }

        // Search for the net player's transmog speffect IDs
        for (auto entry = player_context.player->special_effects->head; entry != nullptr;
             entry = entry->next)
        {
            if (entry->param_id >= vfx::transmog_vfx_speffect_start_id &&
                entry->param_id < vfx::transmog_vfx_speffect_end_id)
            {
                if (entry->param_id % 2 == 0)
                {
                    entry->param = &player_context.head_speffect;
                    player_context.head_speffect_id = entry->param_id;
                }
                else
                {
                    entry->param = &player_context.body_speffect;
                    player_context.body_speffect_id = entry->param_id;
                }
            }
        }

        // Update the networked player VFX based on the state previously sent over the
        // network
        auto &state = client_side_only
                          ? empty_state
                          : ertransmogrify::net::get_net_player_state(
                                player_context.player->session_holder.network_session->steam_id);

        update_player_context(player_context, state, i);
    }
}

/**
 * Update the main player and each network player's transmog state as part of the main game loop
 */
static void (*in_game_stay_step_load_finish)(InGameStep *) = nullptr;
static void in_game_stay_step_load_finish_detour(InGameStep *step)
{
    update_player_contexts();
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
    auto disable_enable_grace_warp_address = modutils::scan<unsigned char>({
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
            .address = (unsigned char *)get_posture_control_original + 175,
            .relative_offsets = {{1, 5}},
        });
    }
    else
    {
        SPDLOG_WARN("Couldn't find GetPostureControl(), skipping posture fix");
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

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> speffect_id_dist(
        transmog_vfx_speffect_start_id, transmog_vfx_speffect_end_id - 1);
    auto base_vfx_speffect_id = speffect_id_dist(rng);

    int i = 0;
    for (auto &context : player_context_buffer)
    {
        context.player = nullptr;

        // Add two VFX for transforming the player's head and the rest of their body into the above
        // armor set.
        context.head_vfx.transformProtectorId =
            (int)(ertransmogrify::vfx::transmog_set_base_id + 10000 * i);
        context.body_vfx.transformProtectorId =
            (int)(ertransmogrify::vfx::transmog_set_base_id + 10000 * i);
        context.body_vfx.isFullBodyTransformProtectorId = true;

        // For the main player, pick two random IDs (an even and odd one) to avoid conflicts in
        // Seamless Co-op. For networked players, the SpEffects will be chosen in the other
        // player's game
        if (i == 0)
        {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> speffect_id_dist(
                transmog_vfx_speffect_start_id, transmog_vfx_speffect_end_id - 1);

            auto id = speffect_id_dist(rng);
            if (id % 2 == 0)
            {
                context.head_speffect_id = id;
                context.body_speffect_id = id + 1;
            }
            else
            {
                context.head_speffect_id = id - 1;
                context.body_speffect_id = id;
            }
        }

        context.head_speffect = dummy_speffect_param;
        context.head_speffect.stateInfo = head_transmog_state_info;

        context.body_speffect = dummy_speffect_param;
        context.body_speffect.stateInfo = body_transmog_state_info;

        i++;
    }

    // Decrease the priority of DLC transformation effects, so transmog can be used to override
    // them
    for (auto [vfx_id, vfx] : er::param::SpEffectVfxParam)
    {
        if (vfx.transformProtectorId != -1 && vfx.isFullBodyTransformProtectorId &&
            vfx.playCategory == vfx_play_category_transmog)
        {
            (patched_vfx_params[vfx_id] = vfx).playPriority = 100;
        }
    }
}
