#include <iostream>
#include <tga/paramdefs.h>
#include <thread>

#include "ModUtils.hpp"
#include "Transmog.hpp"
#include "TransmogEvents.hpp"
#include "TransmogMessages.hpp"
#include "TransmogShop.hpp"
#include "TransmogTalkScript.hpp"
#include "TransmogVFX.hpp"
#include "internal/Params.hpp"

using namespace std;

thread mod_thread;

void Transmog::initialize()
{
    cout << "Initializing mod..." << endl;
    ModUtils::initialize();

    mod_thread = thread([]() {
        cout << "Waiting for params..." << endl;
        CS::ParamMap params;
        auto param_list_address = ModUtils::scan<CS::ParamList *>({
            .aob = "48 8B 0D ?? ?? ?? ?? 48 85 C9 0F 84 ?? ?? ?? ?? 45 33 C0 BA 90",
            .relative_offsets = {{0x3, 0x7}},
        });
        while (!CS::try_get_params(param_list_address, params))
        {
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        cout << "Waiting for messages..." << endl;
        auto msg_repository_address = ModUtils::scan<MsgRepository *>({
            .aob = "48 8B 3D ?? ?? ?? ?? 44 0F B6 30 48 85 FF 75",
            .relative_offsets = {{0x3, 0x7}},
        });
        while (*msg_repository_address == nullptr)
        {
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        auto msg_repository = *msg_repository_address;

        cout << "Hooking transmog messages..." << endl;
        TransmogMessages::initialize(msg_repository);

        cout << "Adding transmog VFX..." << endl;
        TransmogVFX::initialize(params);

        cout << "Adding transmog shops..." << endl;
        TransmogShop::initialize(params, msg_repository);

        cout << "Hooking talk scripts..." << endl;
        TransmogTalkScript::initialize();

        cout << "Hooking transmog events..." << endl;
        TransmogEvents::initialize();

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
    TransmogTalkScript::deinitialize();
    TransmogEvents::deinitialize();
    ModUtils::deinitialize();
}
