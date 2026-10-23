# The Xbox 360 runtime as one static library, built once for every title compiled with the same register settings
# (x360.cmake: x360_title configures and builds it in native/x360/build/runtime-<key>, key = the settings, the
# compiler and -march). A title's program is then its recompiled code, its import stubs and this library.
#
#   cmake -S native/x360/runtime_lib -B <dir> -G Ninja -DCMAKE_BUILD_TYPE=Release -DX360_SETTINGS_DIR=<dir>
#         -DX360_MARCH=haswell -DCMAKE_CXX_COMPILER=<clang-cl> -DCMAKE_C_COMPILER=<clang-cl>
#   cmake --build <dir> --target x360_runtime
#
# X360_SETTINGS_DIR holds x360_ppc_settings.h: the PPCContext layout the runtime is compiled for.
cmake_minimum_required(VERSION 3.20)
project(x360_runtime CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (NOT X360_SETTINGS_DIR)
    message(FATAL_ERROR "X360_SETTINGS_DIR is required (the folder holding x360_ppc_settings.h)")
endif()
set(X360_MARCH "haswell" CACHE STRING "-march of the build")

get_filename_component(X360_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
set(X360_RUNTIME "${X360_ROOT}/runtime")
set(X360_PPC_SHARED "${X360_RUNTIME}/ppc_shared")
set(X360_XENON "${X360_ROOT}/../xenonrecomp")

file(GLOB_RECURSE runtime_sources CONFIGURE_DEPENDS "${X360_RUNTIME}/src/*.cpp")
include("${X360_ROOT}/system/xui/xui.cmake")
include("${X360_ROOT}/system/live/live.cmake")
include("${X360_ROOT}/system/stfs/stfs.cmake")
include("${X360_ROOT}/system/guide/guide.cmake")
include("${X360_ROOT}/system/transition/transition.cmake")
include("${X360_ROOT}/system/pads/pads.cmake")
add_library(x360_runtime STATIC ${runtime_sources} ${X360_XUI_SOURCES} ${X360_LIVE_SOURCES} ${X360_STFS_SOURCES}
    ${X360_GUIDE_SOURCES} ${X360_TRANSITION_SOURCES} ${X360_PADS_SOURCES})
target_include_directories(x360_runtime PRIVATE "${X360_RUNTIME}/src" "${X360_PPC_SHARED}" "${X360_SETTINGS_DIR}"
    "${X360_XENON}/XenonUtils" "${X360_XENON}/thirdparty/simde" "${X360_XUI_INCLUDE_DIR}" ${X360_LIVE_INCLUDE_DIRS}
    "${X360_STFS_INCLUDE_DIR}" "${X360_GUIDE_INCLUDE_DIR}" "${X360_TRANSITION_INCLUDE_DIR}" "${X360_PADS_INCLUDE_DIR}")
target_compile_options(x360_runtime PRIVATE -fno-strict-aliasing -march=${X360_MARCH} /fp:strict /Z7)
target_compile_definitions(x360_runtime PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS)
