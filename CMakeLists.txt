cmake_minimum_required(VERSION 3.28.1)

project(
  TomEldenRingMods
  VERSION 0.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_library(EldenRingLoadoutMod
  SHARED
  src/EldenRingLoadoutMod.cpp
  src/BaseMod.cpp
  src/BaseMod.hpp
  src/GameHook.cpp
  src/GameHook.hpp)

add_library(ConvergenceSpellScalingMod
  SHARED
  src/ConvergenceSpellScalingMod.cpp
  src/BaseMod.cpp
  src/BaseMod.hpp
  src/GameHook.cpp
  src/GameHook.hpp)

target_link_libraries(EldenRingLoadoutMod)
target_link_libraries(ConvergenceSpellScalingMod)
