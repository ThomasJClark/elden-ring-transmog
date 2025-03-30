#define WIN32_LEAN_AND_MEAN

#include "ertransmogrify_config.hpp"
#include "ertransmogrify_messages.hpp"
#include "ertransmogrify_shop.hpp"
#include "ertransmogrify_talkscript.hpp"
#include "ertransmogrify_vfx.hpp"
#include "utils/players.hpp"

#include <elden-x/params/params.hpp>
#include <elden-x/utils/modutils.hpp>

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <windows.h>

#include <cstdio>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <thread>

namespace fs = std::filesystem;

std::thread mod_thread;

bool WINAPI DllMain(HINSTANCE dll_instance, unsigned int fdw_reason, void *lpv_reserved)
{
    if (fdw_reason == DLL_PROCESS_ATTACH)
    {
        wchar_t dll_filename[MAX_PATH] = {0};
        GetModuleFileNameW(dll_instance, dll_filename, MAX_PATH);
        auto folder = fs::path(dll_filename).parent_path();

        auto logger = std::make_shared<spdlog::logger>("transmog");
        logger->sinks().push_back(make_shared<spdlog::sinks::daily_file_sink_st>(
            (folder / "logs" / "ertransmogrify.log").string(), 0, 0, false, 5));
        logger->flush_on(spdlog::level::warn);
        logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] %^[%l]%$ %v");
        spdlog::set_default_logger(logger);

        SPDLOG_INFO("Transmog version {}", PROJECT_VERSION);

        ertransmogrify::config::load(folder / "ertransmogrify.ini");

        if (ertransmogrify::config::debug)
        {
            AllocConsole();
            FILE *stream;
            freopen_s(&stream, "CONOUT$", "w", stdout);
            freopen_s(&stream, "CONOUT$", "w", stderr);
            freopen_s(&stream, "CONIN$", "r", stdin);
            logger->sinks().push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
            logger->set_level(spdlog::level::trace);
            logger->flush_on(spdlog::level::trace);
            logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] %^[%l]%$ [%s:%#] %v");
        }

        mod_thread = std::thread([]() {
            try
            {
                std::this_thread::sleep_for(std::chrono::seconds(5));
                modutils::initialize();
                er::FD4::find_singletons();
                players::initialize();

                er::CS::SoloParamRepository::wait_for_params();

                SPDLOG_INFO("Hooking transmog messages...");
                ertransmogrify::msg::initialize();

                SPDLOG_INFO("Adding transmog VFX...");
                std::this_thread::sleep_for(std::chrono::seconds(1));
                ertransmogrify::vfx::initialize();

                SPDLOG_INFO("Adding transmog shops...");
                std::this_thread::sleep_for(std::chrono::seconds(1));
                ertransmogrify::shop::initialize();

                if (ertransmogrify::config::patch_grace_talk_script)
                {
                    SPDLOG_INFO("Hooking talk scripts...");
                    ertransmogrify::talkscript::initialize();
                }

                if (ertransmogrify::config::initialize_delay)
                {
                    SPDLOG_INFO("Waiting {}ms to enable...",
                                ertransmogrify::config::initialize_delay);
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(ertransmogrify::config::initialize_delay));
                }

                modutils::enable_hooks();

                SPDLOG_INFO("Initialized transmog");
            }
            catch (std::runtime_error const &e)
            {
                SPDLOG_ERROR("Error initializing mod: {}", e.what());
                modutils::deinitialize();
            }
        });
    }
    else if (fdw_reason == DLL_PROCESS_DETACH && lpv_reserved != nullptr)
    {
        try
        {
            mod_thread.join();
            modutils::deinitialize();
            SPDLOG_INFO("Deinitialized transmog");
        }
        catch (std::runtime_error const &e)
        {
            SPDLOG_ERROR("Error deinitializing mod: {}", e.what());
            spdlog::shutdown();
            return false;
        }
        spdlog::shutdown();
    }
    return true;
}

// Set up a fake ModEngine2 extension simply so it doesn't print a "is not a modengine extension"
// error
static struct dummy_modengine_extension_st
{
    virtual ~dummy_modengine_extension_st() = default;
    virtual void on_attach(){};
    virtual void on_detach(){};
    virtual const char *id()
    {
        return "transmog";
    }
} modengine_extension;

extern "C" __declspec(dllexport) bool modengine_ext_init(void *connector, void **extension)
{
    *extension = &modengine_extension;
    return true;
}
