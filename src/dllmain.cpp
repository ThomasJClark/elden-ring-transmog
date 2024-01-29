
#define WIN32_LEAN_AND_MEAN
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <windows.h>

#include "Transmog.hpp"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
#if _DEBUG
        AllocConsole();
        FILE *stream;
        freopen_s(&stream, "CONOUT$", "w", stdout);
        freopen_s(&stream, "CONOUT$", "w", stderr);
        freopen_s(&stream, "CONIN$", "r", stdin);
#endif
        try
        {
            Transmog::initialize();
        }
        catch (std::runtime_error const &e)
        {
            std::cerr << "Error initializing mod: " << e.what() << std::endl;
        }
    }
    else if (fdwReason == DLL_PROCESS_DETACH && lpvReserved != nullptr)
    {
        try
        {
            Transmog::deinitialize();
        }
        catch (std::runtime_error const &e)
        {
            std::cerr << "Error deinitializing mod: " << e.what() << std::endl;
        }
    }
    return TRUE;
}