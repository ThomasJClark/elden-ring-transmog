#include <iostream>
#include <thread>

#include "ModUtils.hpp"
#include "ParamUtils.hpp"
#include "Transmog.hpp"
#include "TransmogMessages.hpp"
#include "TransmogShop.hpp"
#include "TransmogTalkScript.hpp"
#include "TransmogVFX.hpp"
#include "internal/WorldChrMan.hpp"

using namespace std;

thread mod_thread;

void Transmog::initialize()
{
    cout << "Initializing mod..." << endl;
    ModUtils::initialize();

    mod_thread = thread([]() {
        ParamUtils::initialize();

        auto world_chr_man_addr = ModUtils::scan<CS::WorldChrManImp *>({
            .aob = "48 8b 05 ?? ?? ?? ??"  // mov rax, [WorldChrMan]
                   "48 85 c0"              // test rax, rax
                   "74 0f"                 // jz end_label
                   "48 39 88 08 e5 01 00", // cmp [rax + 0x1e508], rcx
            .relative_offsets = {{3, 7}},
        });

        cout << "Hooking transmog messages..." << endl;
        TransmogMessages::initialize();

        cout << "Adding transmog VFX..." << endl;
        TransmogVFX::initialize(world_chr_man_addr);

        cout << "Adding transmog shops..." << endl;
        TransmogShop::initialize();

        cout << "Hooking talk scripts..." << endl;
        TransmogTalkScript::initialize();

        ModUtils::enable_hooks();

        cout << "Initialized transmog" << endl;
    });
}

void Transmog::deinitialize()
{
    mod_thread.join();
    ModUtils::deinitialize();
}
