#include <iostream>

#include "BaseMod.hpp"
#include "GameHook.hpp"

class EldenRingLoadoutMod : public BaseMod
{
  public:
    void initialize()
    {
        game_hook.initialize("eldenring.exe");
    }

    void update()
    {
        std::string cmd;
        std::cin >> cmd;

        if (cmd == "p")
        {
            auto player = game_hook.get_player(0);
            if (player)
            {
                auto character_data = player->character_data;
                std::wcout << character_data->name << " " << character_data->head_armor
                           << std::endl;
            }
            else
            {
                std::cout << "player is null" << std::endl;
            }
        }
        else if (cmd == "s")
        {
            auto player = game_hook.get_player(0);
            if (player)
            {
                auto character_data = player->character_data;
                if (character_data->head_armor == 1060000)
                {
                    character_data->head_armor = 760000;
                }
                else
                {
                    character_data->head_armor = 1060000;
                }
            }
            else
            {
                std::cout << "player is null" << std::endl;
            }
        }
    }

    void deinitialize()
    {
    }

  private:
    GameHook game_hook;
};

EXPORT_MOD(EldenRingLoadoutMod);
