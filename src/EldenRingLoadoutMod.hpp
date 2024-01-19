#pragma once

#include <atomic>

#include "GameHook.hpp"

class EldenRingLoadoutMod
{
  public:
    void start();
    std::atomic<bool> running = true;

  private:
    GameHook gameHook;
};
