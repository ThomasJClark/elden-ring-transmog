#include <iomanip>
#include <iostream>
#include <tga/paramdefs.h>
#include <thread>

#include "Params.hpp"
#include "Transmog.hpp"
#include "TransmogMessages.hpp"
#include "TransmogParams.hpp"
#include "TransmogUtils.hpp"

std::thread mod_thread;

void Transmog::initialize()
{
    mod_thread = std::thread([]() {
        std::cout << "Initializing mod..." << std::endl;
        TransmogUtils::initialize();

        std::cout << "Waiting for params..." << std::endl;
        ParamMap params;
        auto param_list_address = TransmogUtils::scan<ParamList *>({
            .aob = {0x48, 0x8B, 0x0D, -1, -1, -1,   -1,   0x48, 0x85, 0xC9, 0x0F,
                    0x84, -1,   -1,   -1, -1, 0x45, 0x33, 0xC0, 0xBA, 0x90},
            .relative_offsets = {{0x3, 0x7}},
        });
        while (!try_get_params(param_list_address, params))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "Hooking transmog params..." << std::endl;
        TransmogParams::initialize(params);

        std::cout << "Waiting for messages..." << std::endl;
        auto msg_repository_address = TransmogUtils::scan<MsgRepository *>({
            .aob = {0x48, 0x8B, 0x3D, -1, -1, -1, -1, 0x44, 0x0F, 0xB6, 0x30, 0x48, 0x85, 0xFF,
                    0x75},
            .relative_offsets = {{0x3, 0x7}},
        });
        while (*msg_repository_address == nullptr)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "Hooking transmog messages..." << std::endl;
        TransmogMessages::initialize(*msg_repository_address);

        std::cout << "Initialized transmog" << std::endl;

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
    });
}

void Transmog::deinitialize()
{
    TransmogParams::deinitialize();
    TransmogMessages::deinitialize();
    TransmogUtils::deinitialize();
    mod_thread.join();
}
