# The CodeBreaker SDK's samples, built against the installed SDK (find_package(CodeBreaker)), never against the source tree.
#
#   cmake -S samples -B build/samples -G Ninja -DCMAKE_PREFIX_PATH=<the SDK's install prefix>
#   cmake --build build/samples
#
# Each sample is one folder with one main.c or main.cpp and a README, and also builds on its own (cmake -S samples/HelloBinary ...).
# tools\sdk\samples.cmd installs the SDK, builds them all and runs each against tests/sdk/golden: a sample that rots fails the gate.
cmake_minimum_required(VERSION 3.20)
project(CodeBreakerSamples LANGUAGES C CXX)
find_package(CodeBreaker 0.1 REQUIRED)
if(MSVC)
  add_compile_options(/W4 /WX /utf-8)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)   # (fopen, strtoul: the samples are portable C)
else()
  add_compile_options(-Wall -Wextra)
endif()
add_subdirectory(framework)
foreach(sample HelloBinary HelloDecompile DisasmAnyCpu)
  add_subdirectory(${sample})
endforeach()
