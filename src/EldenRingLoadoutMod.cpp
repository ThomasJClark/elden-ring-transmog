#include <iostream>

#include "BaseMod.hpp"
#include "GameHook.hpp"

class EldenRingLoadoutMod : public BaseMod
{
  public:
    void initialize()
    {
        std::cout << "*** initializing loadout mod" << std::endl;
    }

    void update()
    {
        std::cout << "*** updating loadout mod" << std::endl;
    }

    void deinitialize()
    {
    }

  private:
    GameHook game_hook;
};

EXPORT_MOD(EldenRingLoadoutMod);
