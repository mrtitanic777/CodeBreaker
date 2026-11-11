# CodeBreaker in C++ (docs/CPP_DESIGN.md section 8): one tree, `native/cb/`, built as static libraries per area
# (cb_core, and cb_formats, cb_analyze ... as each wave lands), a test executable `cb_tests` that runs every
# module's `_test.cpp`, and -- once the commands exist natively -- the one executable `codebreaker`.
#
#   (in a vcvars64 shell, or any shell once cmake/ninja/cl are on PATH)
#   cmake -S native -B state/cxx/cb -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build state/cxx/cb
#   state/cxx/cb/bin/cb_tests.exe
#
# During the transition the program is still built by tools/build_codebreaker_exe.cmd (pyengine-generated CMake).
# That generated project joins this tree with
#   add_subdirectory(<CodeBreaker>/native/cb  <binary dir>/cb)    # defines cb_core ...
#   target_link_libraries(codebreaker PRIVATE cb_core)
# which is what -DCB_WITH_PYTHON=ON describes: the same executable, with the translated Python linked in too.
cmake_minimum_required(VERSION 3.20)
project(codebreaker_native LANGUAGES CXX C)

option(CB_WITH_PYTHON "link the translated Python (pydyn, pyrt) into codebreaker" ON)
option(CB_ASAN "build with /fsanitize=address (the test target is run under it once per module before merge)" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(cb)
