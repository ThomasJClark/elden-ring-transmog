#include <cstdio>
#include <iomanip>
#include <span>
#include <thread>
#include <vector>

#include "EldenRingLoadoutMod.h"
#include "GameHook.h"

using namespace std::chrono_literals;

static const std::vector<ptrdiff_t> g_param_inner_path = {0x80, 0x80};

void EldenRingLoadoutMod::start()
{
    running = true;

    game_hook.initialize("eldenring.exe");

    auto params = game_hook.getParams();
    auto equip_param_weapon = params[L"EquipParamWeapon"];
    auto equip_param_protector = params[L"EquipParamProtector"];
    auto equip_param_accesory = params[L"EquipParamAccessory"];
    auto shop_lineup_param = params[L"ShopLineupParam"];

    while (running)
    {

        printf("[%p] %ls (%hu rows)\n", equip_param_weapon->param_header->param_table,
               dlw_c_str(&equip_param_weapon->param_name), equip_param_weapon->param_header->param_table->num_rows);

        printf("[%p] %ls (%hu rows)\n", equip_param_protector->param_header->param_table,
               dlw_c_str(&equip_param_protector->param_name),
               equip_param_protector->param_header->param_table->num_rows);

        printf("[%p] %ls (%hu rows)\n", equip_param_accesory->param_header->param_table,
               dlw_c_str(&equip_param_accesory->param_name), equip_param_accesory->param_header->param_table->num_rows);

        printf("[%p] %ls (%hu rows)\n", shop_lineup_param->param_header->param_table,
               dlw_c_str(&shop_lineup_param->param_name), shop_lineup_param->param_header->param_table->num_rows);

        printf("\n");

        std::this_thread::sleep_for(1s);
    }
}
