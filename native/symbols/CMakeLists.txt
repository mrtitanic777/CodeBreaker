# CodeBreaker's symbol knowledge in C++: what codebreaker/symbols/ computed (demangling first).
# A DLL with a C interface (cbsymbols.h), loaded by codebreaker/symbols/native.py through ctypes.
#
#   (in a vcvars64 shell)
#   cmake -S native/symbols -B native/symbols/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build native/symbols/build
cmake_minimum_required(VERSION 3.16)
project(codebreaker_symbols CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(cbsymbols SHARED
  api.cc        # the C interface: a question, its text, the answer as JSON
  demangle.cc     # Microsoft's decorated names (was symbols/demangle.py)
  prototypes.cc)  # the API's prototypes from the C headers (was symbols/prototypes.py's reader)
target_include_directories(cbsymbols PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
if(MSVC)
  target_compile_options(cbsymbols PRIVATE /EHsc /O2 /W3 /utf-8)
  target_compile_definitions(cbsymbols PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
  set_property(TARGET cbsymbols PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")   # no VC runtime DLL needed
endif()
set_target_properties(cbsymbols PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
                                           LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
