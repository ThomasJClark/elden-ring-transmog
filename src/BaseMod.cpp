#define WIN32_LEAN_AND_MEAN
#include <chrono>
#include <cstdio>
#include <iostream>
#include <windows.h>

#include "BaseMod.hpp"

using namespace std::chrono_literals;

void BaseMod::start()
{
    running = true;
    thread = std::thread([this]() {

#if _DEBUG
        AllocConsole();
        FILE *stream;
        freopen_s(&stream, "CONOUT$", "w", stdout);
        freopen_s(&stream, "CONOUT$", "w", stderr);
        freopen_s(&stream, "CONIN$", "r", stdin);
#endif

        try
        {
            initialize();
        }
        catch (std::runtime_error const &e)
        {
            std::cerr << "Error intializing mod" << std::endl << e.what() << std::endl;
            return;
        }

        while (running)
        {
            std::this_thread::sleep_for(1s);

            try
            {
                update();
            }
            catch (std::runtime_error const &e)
            {
                std::cerr << "Error running mod (" << e.what() << ")" << std::endl;
                return;
            }
        }
    });
}

void BaseMod::stop()
{
    running = false;
    thread.join();
}
