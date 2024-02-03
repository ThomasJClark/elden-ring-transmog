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

add_library(EldenRingTransmogMod SHARED
  src/Params.hpp
  src/Transmog.hpp
  src/Transmog.cpp
  src/TransmogMessages.hpp
  src/TransmogMessages.cpp
  src/TransmogParams.hpp
  src/TransmogParams.cpp
  src/TransmogTalkScript.hpp
  src/TransmogTalkScript.cpp
  src/TransmogUtils.hpp
  src/TransmogUtils.cpp
  src/dllmain.cpp)

target_link_libraries(EldenRingTransmogMod minhook ertga)
