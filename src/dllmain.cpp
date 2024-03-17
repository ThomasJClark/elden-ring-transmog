#define WIN32_LEAN_AND_MEAN
#include <cstdio>
#include <filesystem>
#include <memory>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
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

void setup_logger(const filesystem::path &logs_path)
{
    auto logger = make_shared<spdlog::logger>("transmog");
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] %^[%l]%$ %v");
    logger->sinks().push_back(
        make_shared<spdlog::sinks::daily_file_sink_st>(logs_path.string(), 0, 0, false, 5));
    logger->flush_on(spdlog::level::info);

#if _DEBUG
    AllocConsole();
    FILE *stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
    freopen_s(&stream, "CONIN$", "r", stdin);
    logger->sinks().push_back(make_shared<spdlog::sinks::stdout_color_sink_st>());
    logger->set_level(spdlog::level::trace);
#endif

    spdlog::set_default_logger(logger);
}

bool WINAPI DllMain(HINSTANCE dll_instance, uint32_t fdw_reason, void *lpv_reserved)
{
    if (fdw_reason == DLL_PROCESS_ATTACH)
    {
        wchar_t dll_filename[MAX_PATH] = {0};
        GetModuleFileNameW(dll_instance, dll_filename, MAX_PATH);
        auto folder = filesystem::path(dll_filename).parent_path();

        setup_logger(folder / "logs" / "ertransmogrify.log");

#ifdef PROJECT_VERSION
        spdlog::info("Transmog version {}", PROJECT_VERSION);
#endif

        TransmogConfig::load_config(folder / "ertransmogrify.ini");

        mod_thread = thread([]() {
            try
            {
                ModUtils::initialize();

                ParamUtils::initialize();
                PlayerUtils::initialize();

                spdlog::info("Hooking transmog messages...");
                TransmogMessages::initialize();

                spdlog::info("Adding transmog VFX...");
                TransmogVFX::initialize();

                spdlog::info("Adding transmog shops...");
                TransmogShop::initialize();

                if (TransmogConfig::patch_grace_talk_script)
                {
                    spdlog::info("Hooking talk scripts...");
                    TransmogTalkScript::initialize();
                }

                if (TransmogConfig::initialize_delay)
                {
                    spdlog::info("Waiting {}ms to enable...", TransmogConfig::initialize_delay);
                    this_thread::sleep_for(chrono::milliseconds(TransmogConfig::initialize_delay));
                }

                ModUtils::enable_hooks();

                spdlog::info("Initialized transmog");
            }
            catch (runtime_error const &e)
            {
                spdlog::error("Error initializing mod: {}", e.what());
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
            spdlog::info("Deinitialized transmog");
        }
        catch (runtime_error const &e)
        {
            spdlog::error("Error deinitializing mod: {}", e.what());
            spdlog::shutdown();
            return false;
        }
        spdlog::shutdown();
    }
    return true;
}