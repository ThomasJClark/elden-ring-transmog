#include <filesystem>
#include <iostream>
#include <mini/ini.h>

#include "TransmogConfig.hpp"

using namespace std;

bool TransmogConfig::include_unobtained_armor = true;
bool TransmogConfig::include_cut_armor = true;
bool TransmogConfig::transmog_affects_posture = true;

void TransmogConfig::load_config(const wstring_view &dll_filename)
{
    auto ini_path = filesystem::path(dll_filename).parent_path() / "ertransmogrify.ini";

    wcout << "[transmog] Loading config from " << ini_path << endl;

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
    }

    cout << "[transmog] include_unobtained_armor = " << include_unobtained_armor << endl;
    cout << "[transmog] include_cut_armor = " << include_cut_armor << endl;
    cout << "[transmog] transmog_affects_posture = " << transmog_affects_posture << endl;
}
