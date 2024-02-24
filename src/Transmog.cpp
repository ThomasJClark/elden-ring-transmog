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
#include "internal/WorldChrMan.hpp"

using namespace std;

thread mod_thread;
CS::ParamMap params;

void Transmog::initialize()
{
    cout << "Initializing mod..." << endl;
    ModUtils::initialize();

    mod_thread = thread([]() {
        cout << "Waiting for params..." << endl;
        auto param_list_address = ModUtils::scan<CS::ParamList *>({
            .aob = "48 8B 0D ?? ?? ?? ?? 48 85 C9 0F 84 ?? ?? ?? ?? 45 33 C0 BA 90",
            .relative_offsets = {{3, 7}},
        });
        while (!CS::try_get_params(param_list_address, params))
        {
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        cout << "Waiting for messages..." << endl;
        auto msg_repository_address = ModUtils::scan<MsgRepository *>({
            .aob = "48 8B 3D ?? ?? ?? ?? 44 0F B6 30 48 85 FF 75",
            .relative_offsets = {{3, 7}},
        });
        while (*msg_repository_address == nullptr)
        {
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        auto msg_repository = *msg_repository_address;

        auto world_chr_man_addr = ModUtils::scan<CS::WorldChrManImp *>({
            .aob = "48 8b 05 ?? ?? ?? ??"  // mov rax, [WorldChrMan]
                   "48 85 c0"              // test rax, rax
                   "74 0f"                 // jz end_label
                   "48 39 88 08 e5 01 00", // cmp [rax + 0x1e508], rcx
            .relative_offsets = {{3, 7}},
        });

        cout << "Hooking transmog messages..." << endl;
        TransmogMessages::initialize(msg_repository);

        cout << "Adding transmog VFX..." << endl;
        TransmogVFX::initialize(params, world_chr_man_addr);

        cout << "Adding transmog shops..." << endl;
        TransmogShop::initialize(params, msg_repository);

        cout << "Hooking talk scripts..." << endl;
        TransmogTalkScript::initialize();

        cout << "Hooking transmog events..." << endl;
        TransmogEvents::initialize(params, world_chr_man_addr);

        ModUtils::enable_hooks();

        cout << "Initialized transmog" << endl;
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
