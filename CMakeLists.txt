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

set(SHARED_SRC
  src/ExportMod.hpp
  src/GameHook.cpp
  src/GameHook.hpp
  src/ItemGib.hpp
  src/MsgRepository.hpp
  src/WorldChrMan.hpp)
  
add_library(ConvergenceSpellScalingMod SHARED
  src/ConvergenceSpellScalingMod.cpp
  ${SHARED_SRC})  
target_link_libraries(ConvergenceSpellScalingMod minhook ertga)

add_library(EldenRingTransmogMod SHARED
  src/EldenRingTransmogMod.cpp
  ${SHARED_SRC})
target_link_libraries(EldenRingTransmogMod minhook ertga)
