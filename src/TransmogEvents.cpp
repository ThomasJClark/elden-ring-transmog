#include <iostream>

#include "ModUtils.hpp"
#include "TransmogEvents.hpp"
#include "internal/GameDataMan.hpp"

using namespace TransmogEvents;
using namespace std;

static CS::GameDataMan **game_data_man_addr = nullptr;

void TransmogEvents::initialize()
{
    if (game_data_man_addr == nullptr)
    {
        game_data_man_addr = ModUtils::scan<CS::GameDataMan *>({
            .aob = "48 8B 05 ?? ?? ?? ?? 48 85 C0 74 05 48 8B 40 58 C3 C3",
            .relative_offsets = {{0x3, 0x7}},
        });
    }

    auto game_data_man = *game_data_man_addr;
    if (game_data_man == nullptr || game_data_man->player_game_data == nullptr)
    {
        return;
    }

    auto &inventory_data = game_data_man->player_game_data->equip_game_data.equip_inventory_data;

    if (inventory_data.inventory.count == 0)
    {
        cout << "inventory: []" << endl;
    }
    else
    {
        cout << "inventory:" << endl;
        for (auto &entry : inventory_data.inventory)
        {
            cout << " - ga_item_id: " << hex << entry.ga_item_id << dec << endl;
            cout << "   item_id: " << hex << entry.item_id << dec << endl;
            cout << "   quantity: " << hex << entry.quantity << dec << endl;
            cout << "   display_id: " << hex << entry.display_id << dec << endl;
        }
    }
}

void TransmogEvents::deinitialize()
{
}
