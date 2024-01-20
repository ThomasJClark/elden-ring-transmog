#pragma once
#define WIN32_LEAN_AND_MEAN

#include <thread>
#include <windows.h>

class BaseMod
{
  public:
    void start();
    void stop();

    virtual void initialize(){};
    virtual void update(){};
    virtual void deinitialize(){};

  private:
    std::atomic<bool> running;
    std::thread thread;
};

#define EXPORT_MOD(ModType)                                                                        \
    static ModType mod;                                                                            \
    BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)                   \
    {                                                                                              \
        if (fdwReason == DLL_PROCESS_ATTACH)                                                       \
            mod.start();                                                                           \
        else if (fdwReason == DLL_PROCESS_DETACH && lpvReserved != nullptr)                        \
            mod.stop();                                                                            \
        return TRUE;                                                                               \
    }
