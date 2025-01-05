cmake_minimum_required(VERSION 3.28.1)

set(CMAKE_GENERATOR_PLATFORM x64)

project(ertransmogrify
  VERSION   "2.3.8"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

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

FetchContent_Declare(mini
  GIT_REPOSITORY        https://github.com/metayeti/mINI.git
  GIT_TAG               0.9.15
  CONFIGURE_COMMAND     ""
  BUILD_COMMAND         "")

set(SPDLOG_DISABLE_DEFAULT_LOGGER ON)
set(SPDLOG_USE_STD_FORMAT ON)
FetchContent_Declare(spdlog
  GIT_REPOSITORY        https://github.com/gabime/spdlog.git
  GIT_TAG               v1.13.0)

set(SWSDK_VERSION "1.59")
set(SWSDK_RELEASE "x64")
FetchContent_Declare(steamworks-sdk
  URL                   https://github.com/julianxhokaxhiu/SteamworksSDKCI/releases/download/${SWSDK_VERSION}/SteamworksSDK-v${SWSDK_VERSION}.0_${SWSDK_RELEASE}.zip
  CONFIGURE_COMMAND     ""
  BUILD_COMMAND         "")

FetchContent_MakeAvailable(
  minhook
  ertga
  pattern16
  mini
  spdlog
  steamworks-sdk)

add_library(ertga INTERFACE)
target_include_directories(ertga INTERFACE ${ertga_SOURCE_DIR}/resources/include)

add_library(pattern16 INTERFACE)
target_include_directories(pattern16 INTERFACE ${pattern16_SOURCE_DIR}/include)

add_library(mini INTERFACE)
target_include_directories(mini INTERFACE ${mini_SOURCE_DIR}/src)

add_library(steamworks-sdk STATIC IMPORTED GLOBAL)
target_include_directories(steamworks-sdk INTERFACE ${steamworks-sdk_SOURCE_DIR}/include)
set_property(TARGET steamworks-sdk APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(steamworks-sdk PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG CXX
        IMPORTED_LOCATION_DEBUG ${steamworks-sdk_SOURCE_DIR}/lib/steam/steam_api64.lib)

add_library(
  ${PROJECT_NAME} SHARED
  src/dllmain.cpp
  src/ertransmogrify_config.cpp
  src/ertransmogrify_config.hpp
  src/ertransmogrify_messages_by_lang.cpp
  src/ertransmogrify_messages.cpp
  src/ertransmogrify_messages.hpp
  src/ertransmogrify_player_state.cpp
  src/ertransmogrify_player_state.hpp
  src/ertransmogrify_shop.cpp
  src/ertransmogrify_shop.hpp
  src/ertransmogrify_talkscript_states.hpp
  src/ertransmogrify_talkscript.cpp
  src/ertransmogrify_talkscript.hpp
  src/ertransmogrify_vfx.cpp
  src/ertransmogrify_vfx.hpp
  src/internal/EzState.hpp
  src/internal/GameDataMan.hpp
  src/internal/GameMan.hpp
  src/internal/List.hpp
  src/internal/WorldChrMan.hpp
  src/utils/modutils.cpp
  src/utils/modutils.hpp
  src/utils/params.cpp
  src/utils/params.hpp
  src/utils/players.cpp
  src/utils/players.hpp)

add_definitions(-DPROJECT_VERSION="${CMAKE_PROJECT_VERSION}")

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/ertransmogrify.ini
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/LICENSE.txt
  COMMAND_EXPAND_LISTS)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
  minhook
  ertga
  pattern16
  mini
  spdlog
  steamworks-sdk)
