# CodeBreaker's analyses in C++: what codebreaker/analyze/ computed (the image first).
# A DLL with a C interface (cbanalyze.h), loaded by codebreaker/analyze/native.py through ctypes.
# Compiled against the headers of the Capstone that Python's `capstone` package carries, and
# linked to none: at run time it calls the very capstone.dll Python loaded (found by path), so an
# instruction decodes to the same text it does in Python (as native/tracer does).
#
#   (in a vcvars64 shell)
#   cmake -S native/analyze -B native/analyze/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build native/analyze/build
cmake_minimum_required(VERSION 3.16)
project(codebreaker_analyze CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
if(NOT CAPSTONE_DIR)
  execute_process(COMMAND ${Python3_EXECUTABLE} -c "import capstone, os; print(os.path.dirname(capstone.__file__))"
                  OUTPUT_VARIABLE CAPSTONE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()
message(STATUS "capstone: ${CAPSTONE_DIR}")

add_library(cbanalyze SHARED
  api.cc        # the C interface: an image handed over once, questions about it as JSON
  image.cc      # the image's decoding: functions, jump tables, discovery (was analyze/image.py)
  call_graph.cc  # calls, references, reach (was analyze/call_graph.py's _build)
  conventions.cc) # each function's convention, arguments, result, shape (was analyze/conventions.py's _functions)
target_include_directories(cbanalyze PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CAPSTONE_DIR}/include)
if(MSVC)
  target_compile_options(cbanalyze PRIVATE /EHsc /O2 /W3 /utf-8)
  target_compile_definitions(cbanalyze PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
  set_property(TARGET cbanalyze PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")   # no VC runtime DLL needed
endif()
set_target_properties(cbanalyze PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
                                           LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
