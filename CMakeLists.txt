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

FetchContent_Declare(pattern16
  GIT_REPOSITORY        https://github.com/Dasaav-dsv/Pattern16.git
  GIT_TAG               728eac543cee7e7bf6fda445d7db06e3dc8a61d0
  CONFIGURE_COMMAND     ""
  BUILD_COMMAND         "")

FetchContent_MakeAvailable(minhook ertga pattern16)

add_library(ertga INTERFACE)
target_include_directories(ertga INTERFACE ${ertga_SOURCE_DIR}/resources/include)

add_library(pattern16 INTERFACE)
target_include_directories(pattern16 INTERFACE ${pattern16_SOURCE_DIR}/include)

add_library(steamapi STATIC IMPORTED GLOBAL)
set_property(TARGET steamapi APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(steamapi PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG CXX
        IMPORTED_LOCATION_DEBUG steam_api64.lib)

add_library(EldenRingTransmogMod SHARED
  src/ModUtils.hpp
  src/ModUtils.cpp
  src/Transmog.hpp
  src/Transmog.cpp
  src/TransmogEvents.hpp
  src/TransmogEvents.cpp
  src/TransmogMessages.hpp
  src/TransmogMessages.cpp
  src/TransmogShop.hpp
  src/TransmogShop.cpp
  src/TransmogTalkScript.hpp
  src/TransmogTalkScript.cpp
  src/TransmogVFX.hpp
  src/TransmogVFX.cpp
  src/dllmain.cpp
  src/ezstate/EzState.hpp
  src/ezstate/CustomStates.hpp
  src/messages.hpp
  src/params/Params.hpp)

target_link_libraries(EldenRingTransmogMod
  minhook
  ertga
  pattern16
  ${CMAKE_SOURCE_DIR}/steam_api64.lib)
