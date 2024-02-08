cmake_minimum_required(VERSION 3.28.1)

project(EldenRingTransmogMod
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

add_library(steamapi STATIC IMPORTED GLOBAL)
set_property(TARGET steamapi APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(steamapi PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG CXX
        IMPORTED_LOCATION_DEBUG steam_api64.lib)

add_library(EldenRingTransmogMod SHARED
  src/Params.hpp
  src/Transmog.hpp
  src/Transmog.cpp
  src/TransmogMessages.hpp
  src/TransmogMessages.cpp
  src/TransmogShop.hpp
  src/TransmogShop.cpp
  src/TransmogTalkScript.hpp
  src/TransmogTalkScript.cpp
  src/TransmogUtils.hpp
  src/TransmogUtils.cpp
  src/TransmogVFX.hpp
  src/TransmogVFX.cpp
  src/dllmain.cpp
  src/messages.hpp)

target_link_libraries(EldenRingTransmogMod
  minhook
  ertga
  ${CMAKE_SOURCE_DIR}/steam_api64.lib)
