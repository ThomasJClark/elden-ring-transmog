#include <iostream>
#include <tga/paramdefs.h>
#include <thread>

#include "AOBs.hpp"
#include "DefaultParams.hpp"
#include "GameDataMan.hpp"
#include "GameMemory.hpp"
#include "MsgRepository.hpp"
#include "Params.hpp"
#include "TransmogI18n.hpp"

static GameMemory game_memory;
static ParamMap params;
static MsgRepository *msg_repository;

// Unused IDs we can yoink
static const std::uint64_t transmog_speffect_id = 501220; // Deathsbane Jerkey
static const std::uint64_t transmog_head_vfx_id = 5366;   // Deathsbane Jerkey VFX
static const std::uint64_t transmog_body_vfx_id = 5200;   // Unknown VFX
static const std::int64_t transmog_head_id = 912000;      // Unknown protector
static const std::int64_t transmog_body_id = 912100;      // Unknown protector
static const std::int64_t transmog_arms_id = 912200;      // Unknown protector
static const std::int64_t transmog_legs_id = 912300;      // Unknown protector

static SpEffectParam *transmog_speffect;

static SpEffectVfxParam *transmog_head_vfx;
static SpEffectVfxParam *transmog_body_vfx;

static EquipParamProtector *transmog_head;
static EquipParamProtector *transmog_body;
static EquipParamProtector *transmog_arms;
static EquipParamProtector *transmog_legs;

struct ItemInfo
{
    std::uint32_t id;
    std::uint32_t quantity;
    std::uint32_t relay_value;
    std::uint32_t ash_of_war;
};

struct ItemInfoList
{
    uint32_t count;
    ItemInfo items[10];
};

typedef const std::int32_t *ItemGibFn(void *unknown1, ItemInfoList *item_info_list, void *unknown2,
                                      std::int32_t unknown3);

ItemGibFn *item_gib;
ItemGibFn *item_gib_trampoline;

const std::int32_t *item_gib_detour(void *unknown1, ItemInfoList *item_info_list, void *unknown2,
                                    std::int32_t unknown3)
{
    if (item_info_list == nullptr)
    {
        std::cout << "item_gib: null" << std::endl;
    }
    else
    {
        std::cout << "item_gib:" << std::endl;
        for (int i = 0; i < item_info_list->count; i++)
        {
            std::cout << "  - id: " << item_info_list->items[i].id << std::endl;
            std::cout << "    quantity: " << item_info_list->items[i].quantity << std::endl;
            std::cout << "    relay_value: " << item_info_list->items[i].relay_value << std::endl;
            std::cout << "    ash_of_war: " << item_info_list->items[i].ash_of_war << std::endl;
        }
    }
    std::cout << std::endl;

    return item_gib_trampoline(unknown1, item_info_list, unknown2, unknown3);
}

std::thread mod_thread;

void initialize_mod()
{
    mod_thread = std::thread([]() {
        std::cout << "Initializing mod..." << std::endl;
        game_memory.initialize();

        std::cout << "Hooking item_gib..." << std::endl;
        item_gib = game_memory.hook({.aob = {0x8B, 0x02, 0x83, 0xF8, 0x0A}, .offset = -0x52},
                                    item_gib_detour, item_gib_trampoline);

        std::cout << "Hooking get_message..." << std::endl;
        get_message = game_memory.hook({.aob = {0x8B, 0xDA, 0x44, 0x8B, 0xCA, 0x33, 0xD2, 0x48,
                                                0x8B, 0xF9, 0x44, 0x8D, 0x42, 0x6F},
                                        .offset = 0xe,
                                        .relative_offsets = {{0x1, 0x5}}},
                                       get_message_detour, get_message_trampoline);

        std::cout << "Waiting for params..." << std::endl;
        auto param_list_address = game_memory.scan(param_list_aob);
        while (!try_get_params(param_list_address, params))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "Patching params..." << std::endl;
        auto equip_param_protector = params[L"EquipParamProtector"];
        auto speffect_vfx_param = params[L"SpEffectVfxParam"];
        auto speffect_param = params[L"SpEffectParam"];

        // Add placeholder protectors for the player's head, body, arms, and legs. We'll patch
        // these params with whatever armor pieces the player picks.
        transmog_head =
            reinterpret_cast<EquipParamProtector *>(equip_param_protector[transmog_head_id]);
        *transmog_head = *reinterpret_cast<EquipParamProtector *>(equip_param_protector[1060000]);

        transmog_body =
            reinterpret_cast<EquipParamProtector *>(equip_param_protector[transmog_body_id]);
        *transmog_body = *reinterpret_cast<EquipParamProtector *>(equip_param_protector[140100]);

        transmog_arms =
            reinterpret_cast<EquipParamProtector *>(equip_param_protector[transmog_arms_id]);
        *transmog_arms = *reinterpret_cast<EquipParamProtector *>(equip_param_protector[910200]);

        transmog_legs =
            reinterpret_cast<EquipParamProtector *>(equip_param_protector[transmog_legs_id]);
        *transmog_legs = *reinterpret_cast<EquipParamProtector *>(equip_param_protector[320300]);

        // Add a VFX that transforms the player's head into the chosen transmog helm
        transmog_head_vfx =
            reinterpret_cast<SpEffectVfxParam *>(speffect_vfx_param[transmog_head_vfx_id]);
        *transmog_head_vfx = default_speffect_vfx;
        transmog_head_vfx->transformProtectorId = transmog_head_id;
        transmog_head_vfx->isFullBodyTransformProtectorId = false;
        transmog_head_vfx->initSfxId = 523412;
        transmog_head_vfx->initDmyId = 220;

        // Add a VFX that transforms the player's body, arms, and legs into the rest of the
        // chosen transmog pieces
        transmog_body_vfx =
            reinterpret_cast<SpEffectVfxParam *>(speffect_vfx_param[transmog_body_vfx_id]);
        *transmog_body_vfx = default_speffect_vfx;
        transmog_body_vfx->transformProtectorId = transmog_head_id;
        transmog_body_vfx->isFullBodyTransformProtectorId = true;

        // Initialize a SpEffect that applies both transmog VFX
        transmog_speffect = reinterpret_cast<SpEffectParam *>(speffect_param[transmog_speffect_id]);
        *transmog_speffect = default_speffect;
        transmog_speffect->vfxId1 = transmog_head_vfx_id;
        transmog_speffect->vfxId2 = transmog_body_vfx_id;

        std::cout << "Patched transmog params" << std::endl;

        std::cout << "Waiting for messages..." << std::endl;
        auto msg_repository_address = game_memory.scan(msg_repository_aob);
        while (*msg_repository_address == nullptr)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        setup_transmog_messages(*msg_repository_address);

        std::cout << "Set up messages" << std::endl;
    });
}

void deinitialize_mod()
{
    game_memory.unhook(get_message);
    game_memory.unhook(item_gib);
    mod_thread.join();
}
