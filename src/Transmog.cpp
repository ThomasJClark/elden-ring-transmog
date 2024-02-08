#include <iomanip>
#include <iostream>
#include <tga/paramdefs.h>
#include <thread>

#include "ModUtils.hpp"
#include "Params.hpp"
#include "Transmog.hpp"
#include "TransmogMessages.hpp"
#include "TransmogShop.hpp"
#include "TransmogTalkScript.hpp"
#include "TransmogVFX.hpp"

using namespace std;

thread mod_thread;

void Transmog::initialize()
{
    cout << "Initializing mod..." << endl;
    ModUtils::initialize();

    mod_thread = thread([]() {
        cout << "Waiting for params..." << endl;
        ParamMap params;
        auto param_list_address = ModUtils::scan<ParamList *>({
            .aob = {0x48, 0x8B, 0x0D, -1, -1, -1,   -1,   0x48, 0x85, 0xC9, 0x0F,
                    0x84, -1,   -1,   -1, -1, 0x45, 0x33, 0xC0, 0xBA, 0x90},
            .relative_offsets = {{0x3, 0x7}},
        });
        while (!try_get_params(param_list_address, params))
        {
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        cout << "Waiting for messages..." << endl;
        auto msg_repository_address = ModUtils::scan<MsgRepository *>({
            .aob = {0x48, 0x8B, 0x3D, -1, -1, -1, -1, 0x44, 0x0F, 0xB6, 0x30, 0x48, 0x85, 0xFF,
                    0x75},
            .relative_offsets = {{0x3, 0x7}},
        });
        while (*msg_repository_address == nullptr)
        {
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        cout << "Hooking transmog messages..." << endl;
        TransmogMessages::initialize(*msg_repository_address);

        cout << "Adding transmog VFX..." << endl;
        TransmogVFX::initialize(params);

        cout << "Adding transmog shops..." << endl;
        TransmogShop::initialize(params, *msg_repository_address);

        // cout << "Hooking talk scripts..." << endl;
        // TransmogTalkScript::initialize();

        cout << "Initialized transmog" << endl;

#if _DEBUG
        // set up some incredible fashion for testing
        TransmogVFX::set_transmog(
            reinterpret_cast<EquipParamProtector *>(params[L"EquipParamProtector"][1060000]));
        TransmogVFX::set_transmog(
            reinterpret_cast<EquipParamProtector *>(params[L"EquipParamProtector"][140100]));
        TransmogVFX::set_transmog(
            reinterpret_cast<EquipParamProtector *>(params[L"EquipParamProtector"][910200]));
        TransmogVFX::set_transmog(
            reinterpret_cast<EquipParamProtector *>(params[L"EquipParamProtector"][320300]));

        // make boiled crab activate transmog for testing
        auto boiled_crab_good =
            reinterpret_cast<EquipParamGoods *>(params[L"EquipParamGoods"][820]);
        boiled_crab_good->refId_default = TransmogVFX::transmog_speffect_id;
        boiled_crab_good->goodsUseAnim = 62;
#endif
    });
}

void Transmog::deinitialize()
{
    mod_thread.join();
    TransmogMessages::deinitialize();
    TransmogVFX::deinitialize();
    TransmogShop::deinitialize();
    // TransmogTalkScript::deinitialize();
    ModUtils::deinitialize();
}
