#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#if _DEBUG
#include <cstdio>
#define ENABLE_DEBUG_CONSOLE                                                                       \
    AllocConsole();                                                                                \
    FILE *stream;                                                                                  \
    freopen_s(&stream, "CONOUT$", "w", stdout);                                                    \
    freopen_s(&stream, "CONOUT$", "w", stderr);                                                    \
    freopen_s(&stream, "CONIN$", "r", stdin);
#endif

#define export_mod(initialize, deinitialize)                                                       \
    BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)                   \
    {                                                                                              \
        if (fdwReason == DLL_PROCESS_ATTACH)                                                       \
        {                                                                                          \
            ENABLE_DEBUG_CONSOLE;                                                                  \
            initialize_mod();                                                                      \
        }                                                                                          \
        else if (fdwReason == DLL_PROCESS_DETACH && lpvReserved != nullptr)                        \
        {                                                                                          \
            deinitialize();                                                                        \
        }                                                                                          \
        return TRUE;                                                                               \
    }
