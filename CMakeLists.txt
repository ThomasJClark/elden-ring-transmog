cmake_minimum_required(VERSION 3.28.1)

project(TomEldenRingMods
  VERSION   0.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(minhook
  GIT_REPOSITORY        https://github.com/TsudaKageyu/minhook.git
  GIT_TAG               f5485b8454544c2f034c78f8f127c1d03dea3636)

FetchContent_Declare(ertga
  GIT_REPOSITORY        https://github.com/The-Grand-Archives/Elden-Ring-CT-TGA.git
  GIT_TAG               v1.7.1
  CONFIGURE_COMMAND     ""
  BUILD_COMMAND         "")

FetchContent_MakeAvailable(minhook ertga)

add_library(ertga INTERFACE)
target_include_directories(ertga INTERFACE ${ertga_SOURCE_DIR}/resources/include)

add_library(EldenRingLoadoutMod SHARED
  src/EldenRingLoadoutMod.cpp
  src/BaseMod.cpp
  src/BaseMod.hpp
  src/GameHook.cpp
  src/GameHook.hpp
  src/WorldChrMan.hpp)
target_link_libraries(EldenRingLoadoutMod minhook ertga)
  
add_library(ConvergenceSpellScalingMod SHARED
  src/ConvergenceSpellScalingMod.cpp
  src/BaseMod.cpp
  src/BaseMod.hpp
  src/GameHook.cpp
  src/GameHook.hpp)  
target_link_libraries(ConvergenceSpellScalingMod minhook ertga)
