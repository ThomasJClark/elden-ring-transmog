#pragma once

#include <atomic>

#include "GameHook.h"

class EldenRingLoadoutMod
{
  public:
    void start();
    std::atomic<bool> running;

  private:
    GameHook game_hook;
};
