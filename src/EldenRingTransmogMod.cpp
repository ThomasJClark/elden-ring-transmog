#include <iomanip>
#include <iostream>
#include <tga/paramdefs.h>
#include <thread>

#include "GameMemory.hpp"
#include "Params.hpp"
#include "TransmogMessages.hpp"
#include "TransmogParams.hpp"

static GameMemory game_memory;
static MsgRepository *msg_repository;

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

        std::cout << "Waiting for params..." << std::endl;
        ParamMap params;
        auto param_list_address = game_memory.scan<ParamList *>({
            .aob = {0x48, 0x8B, 0x0D, -1, -1, -1,   -1,   0x48, 0x85, 0xC9, 0x0F,
                    0x84, -1,   -1,   -1, -1, 0x45, 0x33, 0xC0, 0xBA, 0x90},
            .relative_offsets = {{0x3, 0x7}},
        });
        while (!try_get_params(param_list_address, params))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "Hooking transmog params..." << std::endl;
        TransmogParams::hook(game_memory, params);

#if _DEBUG
        // set up some incredible fashion for testing
        auto equip_param_protector = params[L"EquipParamProtector"];
        TransmogParams::set_transmog(
            reinterpret_cast<EquipParamProtector *>(equip_param_protector[1060000]));
        TransmogParams::set_transmog(
            reinterpret_cast<EquipParamProtector *>(equip_param_protector[140100]));
        TransmogParams::set_transmog(
            reinterpret_cast<EquipParamProtector *>(equip_param_protector[910200]));
        TransmogParams::set_transmog(
            reinterpret_cast<EquipParamProtector *>(equip_param_protector[320300]));
#endif

        std::cout << "Waiting for messages..." << std::endl;
        auto msg_repository_address = game_memory.scan<MsgRepository *>({
            .aob = {0x48, 0x8B, 0x3D, -1, -1, -1, -1, 0x44, 0x0F, 0xB6, 0x30, 0x48, 0x85, 0xFF,
                    0x75},
            .relative_offsets = {{0x3, 0x7}},
        });
        while (*msg_repository_address == nullptr)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "Hooking transmog messages..." << std::endl;
        TransmogMessages::hook(game_memory, *msg_repository_address);

        std::cout << "Initialized transmog" << std::endl;
    });
}

void deinitialize_mod()
{
    game_memory.unhook(item_gib);
    TransmogParams::unhook(game_memory);
    TransmogMessages::unhook(game_memory);
    mod_thread.join();
}
