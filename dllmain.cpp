#define WIN32_LEAN_AND_MEAN

#include <chrono>
#include <thread>
#include <windows.h>

#include "EldenRingLoadoutMod.h"

using namespace std::chrono_literals;

static std::thread modThread;
static EldenRingLoadoutMod mod;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        modThread = std::thread([]() {
            std::this_thread::sleep_for(15s);
            mod.start();
        });
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        mod.running = false;
        modThread.join();
    }

    return TRUE;
}
