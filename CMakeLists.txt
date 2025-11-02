cmake_minimum_required(VERSION 3.28.1)

set(CMAKE_GENERATOR_PLATFORM x64)

project(ertransmogrify
  VERSION   "2.4.12"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

include(FetchContent)

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

set(ER_WITH_HOOKS ON)
FetchContent_Declare(elden-x
  GIT_REPOSITORY        https://github.com/ThomasJClark/elden-x.git
  GIT_TAG               c1e1e84fc6b4d8601bbd5d6ed0e7cb8a092e9ce9)

add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)  

FetchContent_MakeAvailable(
  mini
  spdlog
  steamworks-sdk
  elden-x)

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
  src/ertransmogrify_local_player.cpp
  src/ertransmogrify_messages_by_lang.cpp
  src/ertransmogrify_messages.cpp
  src/ertransmogrify_net.cpp
  src/ertransmogrify_shop.cpp
  src/ertransmogrify_talkscript.cpp
  src/ertransmogrify_vfx.cpp
  src/utils/players.cpp)

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
  mini
  spdlog
  steamworks-sdk
  elden-x)
