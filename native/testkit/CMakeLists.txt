# testkit: what tests/run_all.py asks of native code -- the dependency map for `--changed` (which suites
# does a changed file reach?) and the compile cache (is this g++/javac run one we have already done?).
#
#   (in a vcvars64 shell)
#   cmake -S native/testkit -B native/testkit/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build native/testkit/build          -> native/testkit/build/bin/testkit.exe
cmake_minimum_required(VERSION 3.16)
project(codebreaker_testkit CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(testkit main.cc)
if(MSVC)
  target_compile_options(testkit PRIVATE /EHsc /O2 /W3 /utf-8)
  target_compile_definitions(testkit PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
  set_property(TARGET testkit PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()
set_target_properties(testkit PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
