# soulcalibur_hd - ReXGlue Recompiled Project
#
# SDK boilerplate lives in generated/rexglue.cmake.

cmake_minimum_required(VERSION 3.25)
project(soulcalibur_hd LANGUAGES CXX)

option(SOULCALIBUR_OPTIMIZED "Optimize game code while retaining the pinned Debug SDK ABI" OFF)
if(MSVC AND SOULCALIBUR_OPTIMIZED)
  string(REPLACE "/Od" "/O2 /Ob2" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(REXSDK_VERSION "0.9.0.1" CACHE STRING "Pinned ReXGlue package version")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/rexglue-sdk/lib/cmake/rexglue")
    set(rexglue_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/rexglue-sdk/lib/cmake/rexglue" CACHE PATH "Frozen local SDK")
endif()
include(generated/rexglue.cmake)

# Installed SDK snapshot exports UI through its monolithic runtime.
if(NOT TARGET rex::ui)
    add_library(rex::ui ALIAS rex::runtime)
endif()
if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/soulcalibur_hd.toml")
  configure_file(runtime-defaults.toml "${CMAKE_CURRENT_BINARY_DIR}/soulcalibur_hd.toml" COPYONLY)
endif()
# Sources
set(SOULCALIBURHD_SOURCES
    src/main.cpp
    src/host_math.cpp
    interop/jpeg_diagnostics.cpp
    interop/menu_input.cpp
    interop/local_profile.cpp
    interop/frame_diagnostics.cpp
    interop/audio_diagnostics.cpp
)

if(WIN32)
    add_executable(soulcalibur_hd WIN32 ${SOULCALIBURHD_SOURCES})
else()
    add_executable(soulcalibur_hd ${SOULCALIBURHD_SOURCES})
endif()

rexglue_setup_target(soulcalibur_hd GPU_PLUGINS xenos)








if(WIN32)
    target_sources(soulcalibur_hd PRIVATE src/crash_diagnostics.cpp)
endif()
include(CTest)
if(BUILD_TESTING)
    add_executable(host_math_test tests/host_math_test.cpp src/host_math.cpp)
    add_test(NAME host_math_rounding COMMAND host_math_test)
endif()

if(BUILD_TESTING)
    add_executable(keyboard_controller_test tests/keyboard_controller_test.cpp)
    target_include_directories(keyboard_controller_test PRIVATE src)
    target_link_libraries(keyboard_controller_test PRIVATE rex::runtime)
    add_test(NAME keyboard_controller_state COMMAND keyboard_controller_test)
endif()
