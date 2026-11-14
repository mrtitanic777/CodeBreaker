# CodeBreaker in C++ (docs/CPP_DESIGN.md section 8): one tree, `native/cb/`, built as static libraries per area
# (cb_core, and cb_formats, cb_analyze ... as each wave lands), a test executable `cb_tests` that runs every
# module's `_test.cpp`, and -- once the commands exist natively -- the one executable `codebreaker`.
#
#   (in a vcvars64 shell, or any shell once cmake/ninja/cl are on PATH)
#   cmake -S native -B state/cxx/cb -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build state/cxx/cb
#   state/cxx/cb/bin/cb_tests.exe
#
# The program is the `codebreaker` target of this tree; tools/build_codebreaker_exe.cmd builds it and installs it as
# state/cxx/codebreaker/build/codebreaker.exe.
cmake_minimum_required(VERSION 3.20)
project(codebreaker_native LANGUAGES CXX C)

option(CB_ASAN "build with /fsanitize=address (the test target is run under it once per module before merge)" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(cb)
