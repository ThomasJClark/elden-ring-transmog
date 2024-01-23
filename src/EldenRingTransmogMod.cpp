#include <iostream>
#include <tga/paramdefs.h>
#include <thread>

#include "ExportMod.hpp"
#include "GameHook.hpp"
#include "TransmogUtils.hpp"

static GameHook game_hook;

static const std::uint64_t transmog_speffect_id = 501220; // Deathsbane Jerkey (unused)
static const std::uint64_t transmog_head_vfx_id = 5366;   // Deathsbane Jerkey VFX (unused)
static const std::uint64_t transmog_body_vfx_id = 5200;   // Unknown VFX (unused?)
static const std::int64_t transmog_head_id = 912000;      // Unused?
static const std::int64_t transmog_body_id = 912100;      // Unused?
static const std::int64_t transmog_arms_id = 912200;      // Unused?
static const std::int64_t transmog_legs_id = 912300;      // Unused?

// SpEffect that applies the transmog VFX
static SpEffectParam *transmog_speffect;

// VFX that abuse the dragon head and body system to make a placeholder armor set appear on the
// player
static SpEffectVfxParam *transmog_head_vfx;
static SpEffectVfxParam *transmog_body_vfx;

// Protectors used as placeholders for the transmog VFX
static EquipParamProtector *transmog_head;
static EquipParamProtector *transmog_body;
static EquipParamProtector *transmog_arms;
static EquipParamProtector *transmog_legs;

static wchar_t const *get_message_override(MsgRepository *msg_repository, std::uint32_t unknown,
                                           MsgBndId bnd_id, std::int32_t msg_id)
{
    return game_hook.get_message_original(msg_repository, unknown, bnd_id, msg_id);
}

std::uint64_t get_transmog_good_id_for_protector_id(std::uint64_t protector_id)
{
    return 6900000 + protector_id / 100;
}

std::uint64_t get_protector_id_for_transmog_good_id(std::uint64_t transmog_good_id)
{
    return 100 * (transmog_good_id - 6900000);
}

std::thread mod_thread;

static void initialize_mod()
{
    game_hook.initialize("eldenring.exe");
    game_hook.hook_get_message(get_message_override);

    mod_thread = std::thread([]() {
        std::cout << "Waiting for params" << std::endl;

        // Wait until params are initialized
        while (!game_hook.try_initialize_params())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "Initializing transmog" << std::endl;

        auto speffect_param = game_hook.params[L"SpEffectParam"];
        auto speffect_vfx_param = game_hook.params[L"SpEffectVfxParam"];
        auto equip_param_protector = game_hook.params[L"EquipParamProtector"];

        transmog_speffect =
            reinterpret_cast<SpEffectParam *>(speffect_param->rows[transmog_speffect_id]);
        transmog_head_vfx =
            reinterpret_cast<SpEffectVfxParam *>(speffect_vfx_param->rows[transmog_head_vfx_id]);
        transmog_body_vfx =
            reinterpret_cast<SpEffectVfxParam *>(speffect_vfx_param->rows[transmog_body_vfx_id]);
        transmog_head =
            reinterpret_cast<EquipParamProtector *>(equip_param_protector->rows[transmog_head_id]);
        transmog_body =
            reinterpret_cast<EquipParamProtector *>(equip_param_protector->rows[transmog_body_id]);
        transmog_arms =
            reinterpret_cast<EquipParamProtector *>(equip_param_protector->rows[transmog_arms_id]);
        transmog_legs =
            reinterpret_cast<EquipParamProtector *>(equip_param_protector->rows[transmog_legs_id]);

        *transmog_speffect = base_speffect;
        transmog_speffect->vfxId1 = transmog_head_vfx_id;
        transmog_speffect->vfxId2 = transmog_body_vfx_id;

        *transmog_head_vfx = base_speffect_vfx;
        transmog_head_vfx->transformProtectorId = transmog_head_id;
        transmog_head_vfx->isFullBodyTransformProtectorId = 0;
        transmog_head_vfx->initSfxId = 523412;
        transmog_head_vfx->initDmyId = 220;

        *transmog_body_vfx = base_speffect_vfx;
        transmog_body_vfx->transformProtectorId = transmog_head_id;
        transmog_body_vfx->isFullBodyTransformProtectorId = true;

        std::cout << "transmog initialized" << std::endl;

        *transmog_head =
            *reinterpret_cast<EquipParamProtector *>(equip_param_protector->rows[1060000]);
        *transmog_body =
            *reinterpret_cast<EquipParamProtector *>(equip_param_protector->rows[140100]);
        *transmog_arms =
            *reinterpret_cast<EquipParamProtector *>(equip_param_protector->rows[910200]);
        *transmog_legs =
            *reinterpret_cast<EquipParamProtector *>(equip_param_protector->rows[320300]);
    });
}

static void deinitialize_mod()
{
    game_hook.unhook_get_message();
    mod_thread.join();
}

export_mod(initialize_mod, deinitialize_mod);
