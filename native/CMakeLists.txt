# CodeBreaker in C++ (docs/CPP_DESIGN.md section 8): one tree, `native/cb/`, one build. Each area is a static library
# (cb_core, cb_formats, cb_analyze ...), and each engine inside an area is one too (cb_xex, cb_l2, cb_dosexec,
# cb_pyfront, cb_decompiler ...); they link into the one executable `codebreaker` and the test executable `cb_tests`,
# which runs every module's `_test.cpp`. The decompiler engine's programs (decompiler.exe, pcodecheck.exe) are built
# beside them. (native/nasm is NASM, built apart with gcc until x86asm, native/cb/build/x86asm, replaces it.)
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
# The static libraries also go into shared ones (cbtrace); ELF needs position-independent code for that.
if(NOT WIN32)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

add_subdirectory(cb)
