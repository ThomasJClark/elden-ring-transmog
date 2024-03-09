#define WIN32_LEAN_AND_MEAN
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <windows.h>

#include "ModUtils.hpp"
#include "ParamUtils.hpp"
#include "PlayerUtils.hpp"
#include "TransmogConfig.hpp"
#include "TransmogMessages.hpp"
#include "TransmogShop.hpp"
#include "TransmogTalkScript.hpp"
#include "TransmogVFX.hpp"

using namespace std;

thread mod_thread;

bool WINAPI DllMain(HINSTANCE dll_instance, uint32_t fdw_reason, void *lpv_reserved)
{
    if (fdw_reason == DLL_PROCESS_ATTACH)
    {
#if _DEBUG
        AllocConsole();
        FILE *stream;
        freopen_s(&stream, "CONOUT$", "w", stdout);
        freopen_s(&stream, "CONOUT$", "w", stderr);
        freopen_s(&stream, "CONIN$", "r", stdin);
#endif

        wchar_t dll_filename[MAX_PATH] = {0};
        if (GetModuleFileNameW(dll_instance, dll_filename, MAX_PATH))
        {
            TransmogConfig::load_config(dll_filename);
        }

        mod_thread = thread([]() {
            try
            {
                ModUtils::initialize();

                ParamUtils::initialize();
                PlayerUtils::initialize();

                cout << "[transmog] Hooking transmog messages..." << endl;
                TransmogMessages::initialize();

                cout << "[transmog] Adding transmog VFX..." << endl;
                TransmogVFX::initialize();

                cout << "[transmog] Adding transmog shops..." << endl;
                TransmogShop::initialize();

                if (TransmogConfig::patch_grace_talk_script)
                {
                    cout << "[transmog] Hooking talk scripts..." << endl;
                    TransmogTalkScript::initialize();
                }

                if (TransmogConfig::initialize_delay)
                {
                    cout << "[transmog] Waiting " << TransmogConfig::initialize_delay
                         << "ms to enable..." << endl;
                    this_thread::sleep_for(chrono::milliseconds(TransmogConfig::initialize_delay));
                }

                ModUtils::enable_hooks();

                cout << "[transmog] Initialized transmog" << endl;
            }
            catch (runtime_error const &e)
            {
                cerr << "[transmog] Error initializing mod: " << e.what() << endl;
                ModUtils::deinitialize();
            }
        });
    }
    else if (fdw_reason == DLL_PROCESS_DETACH && lpv_reserved != nullptr)
    {
        try
        {
            mod_thread.join();
            ModUtils::deinitialize();
        }
        catch (runtime_error const &e)
        {
            cerr << "[transmog] Error deinitializing mod: " << e.what() << endl;
            return false;
        }
    }
    return true;
}