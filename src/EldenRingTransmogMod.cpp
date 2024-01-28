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

// The selected armor pieces for the player's apperance. When a new armor piece is chosen, its data
// is copied into these structs.
static const std::int64_t transmog_head_id = 69000000;
static EquipParamProtector transmog_head;

static const std::int64_t transmog_body_id = 69000100;
static EquipParamProtector transmog_body;

static const std::int64_t transmog_arms_id = 69000200;
static EquipParamProtector transmog_arms;

static const std::int64_t transmog_legs_id = 69000300;
static EquipParamProtector transmog_legs;

// A reinforce param to returned for every armor set, which is present in Elden Ring even though
// there isn't armor upgrading
static const std::int64_t reinforce_param_protector_id = 0;
static ReinforceParamProtector reinforce_param_protector = {
    .physicsDefRate = 1,
    .magicDefRate = 1,
    .fireDefRate = 1,
    .thunderDefRate = 1,
    .slashDefRate = 1,
    .blowDefRate = 1,
    .thrustDefRate = 1,
    .resistPoisonRate = 1,
    .resistDiseaseRate = 1,
    .resistBloodRate = 1,
    .resistCurseRate = 1,
    .residentSpEffectId1 = 0,
    .residentSpEffectId2 = 0,
    .residentSpEffectId3 = 0,
    .materialSetId = 0,
    .darkDefRate = 1,
    .resistFreezeRate = 1,
    .resistSleepRate = 1,
    .resistMadnessRate = 1,
};

// VFX that transform the player's appearance to the above armor set using the Dark Souls dragon
// transformation system
static const std::uint64_t transmog_head_vfx_id = 690001;
static SpEffectVfxParam transmog_head_vfx = {
    .initSfxId = 523412,
    .transformProtectorId = transmog_head_id,
    .initDmyId = 220,
    .isFullBodyTransformProtectorId = false,
    .isVisibleDeadChr = 1,
};

static const std::uint64_t transmog_body_vfx_id = 690002;
static SpEffectVfxParam transmog_body_vfx = {
    .transformProtectorId = transmog_head_id,
    .isFullBodyTransformProtectorId = true,
    .isVisibleDeadChr = 1,
};

// A speffect applied to the player when transmog is enabled, to show the above VFX
static const std::uint64_t transmog_speffect_id = 690001;
static SpEffectParam transmog_speffect = default_speffect; /* {
    .vfxId1 = transmog_head_vfx_id,
    .vfxId2 = transmog_body_vfx_id,
}; */

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

// struct EquipParamGoodsResult
// {
//     std::int64_t item_id;
//     EquipParamGoods *row;
// };

// typedef void FindEquipParamGoodsEntryFn(EquipParamGoodsResult *result, std::uint32_t row_id);

// static EquipParamGoods *throwing_dagger;

// FindEquipParamGoodsEntryFn *find_equip_param_goods;
// FindEquipParamGoodsEntryFn *find_equip_param_goods_trampoline;

// void find_equip_param_goods_detour(EquipParamGoodsResult *result, std::uint32_t row_id)
// {
//     if (row_id == 1769)
//     {
//         std::cout << row_id << std::endl;
//         result->item_id = 0x300000000 + row_id;
//         result->row = throwing_dagger;
//     }
//     else
//     {
//         find_equip_param_goods_trampoline(result, row_id);
//     }
// }

struct FindEquipParamProtectorResult
{
    std::int32_t equip_param_protector_id;
    std::byte unused1[4];
    EquipParamProtector *equip_param_protector;
    std::int32_t base_equip_param_protector_id;
    std::byte unused2[4];
    std::int64_t reinforce_param_protector_id;
    ReinforceParamProtector *reinforce_param_protector;
    std::uint32_t unknown;
};

struct FindSpEffectParamResult
{
    SpEffectParam *speffect_param;
    std::int32_t speffect_param_id;
    std::byte unknown;
};

typedef void FindEquipParamProtectorFn(FindEquipParamProtectorResult *result, std::uint32_t id);
typedef void FindEquipParamGoodsFn(std::byte *result, std::uint32_t id);
typedef void FindSpEffectParamFn(FindSpEffectParamResult *result, std::uint32_t id);
typedef void FindSpEffectVfxParamFn(std::byte *result, std::uint32_t id);

FindEquipParamProtectorFn *find_equip_param_protector;
FindEquipParamGoodsFn *find_equip_param_goods;
FindSpEffectParamFn *find_speffect_param;
FindSpEffectVfxParamFn *find_speffect_vfx_param;

FindEquipParamProtectorFn *find_equip_param_protector_trampoline;
FindEquipParamGoodsFn *find_equip_param_goods_trampoline;
FindSpEffectParamFn *find_speffect_param_trampoline;
FindSpEffectVfxParamFn *find_speffect_vfx_param_trampoline;

void find_equip_param_protector_detour(FindEquipParamProtectorResult *result, std::uint32_t id)
{
    switch (id)
    {
    case transmog_head_id:
        result->equip_param_protector_id = transmog_head_id;
        result->equip_param_protector = &transmog_head;
        result->base_equip_param_protector_id = transmog_head_id;
        result->reinforce_param_protector_id = reinforce_param_protector_id;
        result->reinforce_param_protector = &reinforce_param_protector;
        break;
    case transmog_body_id:
        result->equip_param_protector_id = transmog_body_id;
        result->equip_param_protector = &transmog_body;
        result->base_equip_param_protector_id = transmog_body_id;
        result->reinforce_param_protector_id = reinforce_param_protector_id;
        result->reinforce_param_protector = &reinforce_param_protector;
        break;
    case transmog_arms_id:
        result->equip_param_protector_id = transmog_arms_id;
        result->equip_param_protector = &transmog_arms;
        result->base_equip_param_protector_id = transmog_arms_id;
        result->reinforce_param_protector_id = reinforce_param_protector_id;
        result->reinforce_param_protector = &reinforce_param_protector;
        break;
    case transmog_legs_id:
        result->equip_param_protector_id = transmog_legs_id;
        result->equip_param_protector = &transmog_legs;
        result->base_equip_param_protector_id = transmog_legs_id;
        result->reinforce_param_protector_id = reinforce_param_protector_id;
        result->reinforce_param_protector = &reinforce_param_protector;
        break;
    default:
        find_equip_param_protector_trampoline(result, id);
    }
}

void find_equip_param_goods_detour(std::byte *data, std::uint32_t id)
{
    find_equip_param_goods_trampoline(data, id);
}

void find_speffect_param_detour(FindSpEffectParamResult *result, std::uint32_t id)
{
    switch (id)
    {
    case transmog_speffect_id:
        result->speffect_param_id = transmog_speffect_id;
        result->speffect_param = &transmog_speffect;
        result->unknown = std::byte(0x04);
        break;
    default:
        find_speffect_param_trampoline(result, id);
    }
}

// wip vfx
void find_speffect_vfx_param_detour(std::byte *data, std::uint32_t id)
{
    if (id == 5370)
    {
        std::byte buffer1[64];
        std::byte buffer2[64];
        for (int i = 0; i < sizeof(buffer1); i++)
        {
            buffer1[i] = std::byte(0x00);
            buffer2[i] = std::byte(0xff);
        }

        find_speffect_vfx_param_trampoline(buffer1, id);
        find_speffect_vfx_param_trampoline(buffer2, id);

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

    find_speffect_vfx_param_trampoline(data, id);
}

void hook_transmog_params()
{
    std::cout << "Hooking find_equip_param_protector..." << std::endl;
    find_equip_param_protector = game_memory.hook<>(
        {.aob = {0x41, 0x8d, 0x50, 0x01, 0xe8, -1, -1, -1, -1, 0x48, 0x85, 0xc0}, .offset = -0x96},
        find_equip_param_protector_detour, find_equip_param_protector_trampoline);

    // std::cout << "Hooking find_equip_param_goods..." << std::endl;
    // find_equip_param_goods = game_memory.hook<>(
    //     {.aob = {0x41, 0x8d, 0x50, 0x03, 0xe8, -1, -1, -1, -1, 0x48, 0x85, 0xc0},
    //      .offset = -0xb0},
    //     find_equip_param_goods_detour, find_equip_param_goods_trampoline);

    std::cout << "Hooking find_speffect_param..." << std::endl;
    find_speffect_param = game_memory.hook<>(
        {.aob = {0x41, 0x8d, 0x50, 0x0f, 0xe8, -1, -1, -1, -1, 0x48, 0x85, 0xc0}, .offset = -0x72},
        find_speffect_param_detour, find_speffect_param_trampoline);

    std::cout << "Hooking find_speffect_vfx_param..." << std::endl;
    find_speffect_vfx_param = game_memory.hook<>(
        {.aob = {0x41, 0x8d, 0x50, 0x10, 0xe8, -1, -1, -1, -1, 0x48, 0x85, 0xc0}, .offset = -0x6e},
        find_speffect_vfx_param_detour, find_speffect_vfx_param_trampoline);
}

void unhook_transmog_params()
{
    game_memory.unhook(find_equip_param_protector);
    // game_memory.unhook(find_equip_param_goods);
    game_memory.unhook(find_speffect_param);
    game_memory.unhook(find_speffect_vfx_param);
}

std::thread mod_thread;

void initialize_mod()
{
    mod_thread = std::thread([]() {
        std::cout << "Initializing mod..." << std::endl;
        game_memory.initialize();

        hook_transmog_params();

        std::cout << "Hooking item_gib..." << std::endl;
        item_gib = game_memory.hook({.aob = {0x8B, 0x02, 0x83, 0xF8, 0x0A}, .offset = -0x52},
                                    item_gib_detour, item_gib_trampoline);

        std::cout << "Hooking get_message..." << std::endl;
        get_message = game_memory.hook({.aob = {0x8B, 0xDA, 0x44, 0x8B, 0xCA, 0x33, 0xD2, 0x48,
                                                0x8B, 0xF9, 0x44, 0x8D, 0x42, 0x6F},
                                        .offset = 0xe,
                                        .relative_offsets = {{0x1, 0x5}}},
                                       get_message_detour, get_message_trampoline);

        // std::cout << "Hooking find_equip_param_goods..." << std::endl;
        // find_equip_param_goods = game_memory.hook<FindEquipParamGoodsEntryFn>(
        //     {.aob =
        //          {
        //              0x45, 0x33, 0xC0, 0x41, 0x8D, 0x50, 0x03, 0xE8, -1,   -1, -1,
        //              -1,   0x48, 0x85, 0xC0, 0x0F, 0x84, -1,   -1,   -1,   -1, 0x48,
        //              0x8B, 0x80, 0x80, 0x00, 0x00, 0x00, 0x48, 0x8B, 0x90,
        //          },
        //      .offset = -0x67},
        //     find_equip_param_goods_detour, find_equip_param_goods_trampoline);

        std::cout << "Waiting for params..." << std::endl;
        auto param_list_address = game_memory.scan(param_list_aob);
        while (!try_get_params(param_list_address, params))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Add placeholder protectors for the player's head, body, arms, and legs. We'll patch
        // these params with whatever armor pieces the player picks.
        auto equip_param_protector = params[L"EquipParamProtector"];
        transmog_head = *reinterpret_cast<EquipParamProtector *>(equip_param_protector[1060000]);
        transmog_body = *reinterpret_cast<EquipParamProtector *>(equip_param_protector[140100]);
        transmog_arms = *reinterpret_cast<EquipParamProtector *>(equip_param_protector[910200]);
        transmog_legs = *reinterpret_cast<EquipParamProtector *>(equip_param_protector[320300]);

        std::cout << "Waiting for messages..." << std::endl;
        auto msg_repository_address = game_memory.scan(msg_repository_aob);
        while (*msg_repository_address == nullptr)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        setup_transmog_messages(*msg_repository_address);

        std::cout << "Finished initializing" << std::endl;
    });
}

void deinitialize_mod()
{
    game_memory.unhook(get_message);
    game_memory.unhook(item_gib);
    unhook_transmog_params();
    mod_thread.join();
}
