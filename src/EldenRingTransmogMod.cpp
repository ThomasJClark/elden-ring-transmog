#include <iomanip>
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

struct EquipParamGoodsResult
{
    std::int64_t item_id;
    EquipParamGoods *row;
};

typedef void FindEquipParamGoodsEntryFn(EquipParamGoodsResult *result, std::uint32_t row_id);

static EquipParamGoods *throwing_dagger;

FindEquipParamGoodsEntryFn *find_equip_param_goods;
FindEquipParamGoodsEntryFn *find_equip_param_goods_trampoline;

void find_equip_param_goods_detour(EquipParamGoodsResult *result, std::uint32_t row_id)
{
    if (row_id == 1769)
    {
        std::cout << row_id << std::endl;
        result->item_id = 0x300000000 + row_id;
        result->row = throwing_dagger;
    }
    else
    {
        find_equip_param_goods_trampoline(result, row_id);
    }
}

typedef void *LookupParamFn(void *param_repo, std::int32_t param_index, std::uint32_t unknown);

LookupParamFn *lookup_param;
LookupParamFn *lookup_param_trampoline;

std::int64_t param_accesses[0xba] = {0L};

void *lookup_param_detour(void *param_repo, std::int32_t param_index, std::uint32_t unknown)
{
    if (param_index == 0x01)
    {
        /* EquipParamProtector */
        param_accesses[param_index]++;
    }
    else if (param_index == 0x03)
    {
        /* EquipParamGoods */
        param_accesses[param_index]++;
    }
    else if (param_index == 0x10)
    {
        /* SpEffectParam */
        param_accesses[param_index]++;
    }
    else if (param_index == 0x11)
    {
        /* SpEffectVfxParam */
        param_accesses[param_index]++;
    }
    else if (param_index == 0x1b)
    {
        /* ShopLineupParam */
        param_accesses[param_index]++;
    }

    return lookup_param_trampoline(param_repo, param_index, unknown);
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

        std::cout << "Hooking find_equip_param_goods..." << std::endl;
        find_equip_param_goods = game_memory.hook<FindEquipParamGoodsEntryFn>(
            {.aob =
                 {
                     0x45, 0x33, 0xC0, 0x41, 0x8D, 0x50, 0x03, 0xE8, -1,   -1, -1,
                     -1,   0x48, 0x85, 0xC0, 0x0F, 0x84, -1,   -1,   -1,   -1, 0x48,
                     0x8B, 0x80, 0x80, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x90,
                 },
             .offset = -0x67},
            find_equip_param_goods_detour, find_equip_param_goods_trampoline);

        std::cout << "Hooking lookup_param..." << std::endl;
        lookup_param = game_memory.hook<LookupParamFn>(
            {.aob = {0x81, 0xfa, 0xba, 0x00, 0x00, 0x00, 0x7d, 0x24, 0x48, 0x63, 0xd2}},
            lookup_param_detour, lookup_param_trampoline);

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

        auto equip_param_goods = params[L"EquipParamGoods"];
        throwing_dagger = reinterpret_cast<EquipParamGoods *>(equip_param_goods[1700]);
        std::cout << "throwing dagger " << throwing_dagger << std::endl;

        std::cout << "Patched transmog params" << std::endl;

        std::cout << "Waiting for messages..." << std::endl;
        auto msg_repository_address = game_memory.scan(msg_repository_aob);
        while (*msg_repository_address == nullptr)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        setup_transmog_messages(*msg_repository_address);

        std::cout << "Set up messages" << std::endl;

        for (;;)
        {
            for (int i = 0; i < sizeof(param_accesses) / sizeof(param_accesses[0]); i++)
            {
                if (param_accesses[i])
                {
                    std::cout << i << " " << param_accesses[i] << std::endl;
                }
            }
            std::cout << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

void deinitialize_mod()
{
    game_memory.unhook(get_message);
    game_memory.unhook(item_gib);
    game_memory.unhook(find_equip_param_goods);
    mod_thread.join();
}
