#include <mini/ini.h>
#include <spdlog/spdlog.h>

#include "ertransmogrify_config.hpp"

namespace fs = std::filesystem;
using namespace ertransmogrify;

bool config::include_unobtained_armor = true;
bool config::include_cut_armor = true;
bool config::include_dlc_armor = true;
bool config::patch_grace_talk_script = true;
unsigned int config::initialize_delay = 0;

void config::load(const fs::path &ini_path)
{
    SPDLOG_INFO("Loading config from {}", ini_path.string());

    mINI::INIFile file(ini_path.string());
    mINI::INIStructure ini;
    if (file.read(ini) && ini.has("ertransmogrify"))
    {
        auto &config = ini["ertransmogrify"];

        if (config.has("include_unobtained_armor"))
            include_unobtained_armor = config["include_unobtained_armor"] != "false";

        if (config.has("include_cut_armor"))
            include_cut_armor = config["include_cut_armor"] != "false";

        if (config.has("include_dlc_armor"))
            include_dlc_armor = config["include_dlc_armor"] != "false";

        if (config.has("patch_grace_talk_script"))
            patch_grace_talk_script = config["patch_grace_talk_script"] != "false";

        if (config.has("initialize_delay"))
            initialize_delay = stoi(config["initialize_delay"], nullptr, 10);
    }

    SPDLOG_INFO("include_unobtained_armor = {} ", include_unobtained_armor);
    SPDLOG_INFO("include_cut_armor = {} ", include_cut_armor);
    SPDLOG_INFO("include_dlc_armor = {} ", include_dlc_armor);
    SPDLOG_INFO("patch_grace_talk_script = {} ", patch_grace_talk_script);
    SPDLOG_INFO("initialize_delay = {} ", initialize_delay);
}
