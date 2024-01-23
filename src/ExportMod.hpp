#pragma once
#define WIN32_LEAN_AND_MEAN

#include <stdexcept>
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
            try                                                                                    \
            {                                                                                      \
                initialize_mod();                                                                  \
            }                                                                                      \
            catch (std::runtime_error const &e)                                                    \
            {                                                                                      \
                std::cerr << "Error initializing mod " << e.what() << std::endl;                   \
            }                                                                                      \
        }                                                                                          \
        else if (fdwReason == DLL_PROCESS_DETACH && lpvReserved != nullptr)                        \
        {                                                                                          \
            try                                                                                    \
            {                                                                                      \
                deinitialize_mod();                                                                \
            }                                                                                      \
            catch (std::runtime_error const &e)                                                    \
            {                                                                                      \
                std::cerr << "Error deinitializing mod " << e.what() << std::endl;                 \
            }                                                                                      \
        }                                                                                          \
        return TRUE;                                                                               \
    }
