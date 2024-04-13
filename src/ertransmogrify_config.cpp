#include <mini/ini.h>
#include <spdlog/spdlog.h>

#include "ertransmogrify_config.hpp"

using namespace std;
using namespace ertransmogrify;

bool config::include_unobtained_armor = true;
bool config::include_cut_armor = true;
bool config::transmog_affects_posture = true;
bool config::patch_grace_talk_script = true;
uint32_t config::initialize_delay = 0;

void config::load(const filesystem::path &ini_path)
{
    spdlog::info("Loading config from {}", ini_path.string());

    mINI::INIFile file(ini_path.string());
    mINI::INIStructure ini;
    if (file.read(ini) && ini.has("ertransmogrify"))
    {
        auto &config = ini["ertransmogrify"];

        if (config.has("include_unobtained_armor"))
            include_unobtained_armor = config["include_unobtained_armor"] != "false";

        if (config.has("include_cut_armor"))
            include_cut_armor = config["include_cut_armor"] != "false";

        if (config.has("transmog_affects_posture"))
            transmog_affects_posture = config["transmog_affects_posture"] != "false";

        if (config.has("patch_grace_talk_script"))
            patch_grace_talk_script = config["patch_grace_talk_script"] != "false";

        if (config.has("initialize_delay"))
            initialize_delay = stoi(config["initialize_delay"], nullptr, 10);
    }

    spdlog::info("include_unobtained_armor = {} ", include_unobtained_armor);
    spdlog::info("include_cut_armor = {} ", include_cut_armor);
    spdlog::info("transmog_affects_posture = {} ", transmog_affects_posture);
    spdlog::info("patch_grace_talk_script = {} ", patch_grace_talk_script);
    spdlog::info("initialize_delay = {} ", initialize_delay);
}
