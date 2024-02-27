#include <iostream>
#include <thread>

#include "ModUtils.hpp"
#include "ParamUtils.hpp"
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
        ParamUtils::initialize();

        cout << "Hooking transmog messages..." << endl;
        TransmogMessages::initialize();

        cout << "Adding transmog VFX..." << endl;
        TransmogVFX::initialize();

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
