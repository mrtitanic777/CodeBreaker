# CodeBreaker's run recorder: the hooks of a 32-bit function's run under Unicorn (what
# codebreaker/emulate/emulator.py trace_function records), in C++ -- no Python per instruction.
# A DLL with a C interface (cbtrace.h), loaded by codebreaker/emulate/native_trace.py through
# ctypes.
#
#   (in a vcvars64 shell)
#   cmake -S native/tracer -B native/tracer/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build native/tracer/build
#
# It is compiled against the headers of the Unicorn and Capstone that Python's `unicorn` and
# `capstone` packages carry, and links neither: at run time it calls the very unicorn.dll and
# capstone.dll Python loaded (found by path), so a native run runs and decodes exactly as the
# Python one.  UNICORN_DIR / CAPSTONE_DIR override where those packages are.
cmake_minimum_required(VERSION 3.16)
project(codebreaker_tracer CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
if(NOT UNICORN_DIR)
  execute_process(COMMAND ${Python3_EXECUTABLE} -c "import unicorn, os; print(os.path.dirname(unicorn.__file__))"
                  OUTPUT_VARIABLE UNICORN_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()
if(NOT CAPSTONE_DIR)
  execute_process(COMMAND ${Python3_EXECUTABLE} -c "import capstone, os; print(os.path.dirname(capstone.__file__))"
                  OUTPUT_VARIABLE CAPSTONE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()
message(STATUS "unicorn: ${UNICORN_DIR}")
message(STATUS "capstone: ${CAPSTONE_DIR}")

add_library(cbtrace SHARED
  api.cc        # the C interface: set up, attach, the record handed back
  libs.cc       # Unicorn and Capstone, by the DLLs Python loaded
  decode.cc     # instruction text, straight runs, pushes before a call, branch decode
  recorder.cc   # per instruction / per block, settle
  memory.cc     # reads and writes, pages mapped on demand, interrupts, putting pages back
  imports.cc    # the stand-in for an import entered
  branches.cc   # the branch log
  driver_rec.cc # the flat ring-0 driver recorder (driver.py run)
  w16_rec.cc    # the 16-bit recorder (windows16.py trace_function16)
  shadow.cc)    # the path explorer's symbolic shadow (was emulate/shadow.py)
target_include_directories(cbtrace PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${UNICORN_DIR}/include ${CAPSTONE_DIR}/include)
if(MSVC)
  target_compile_options(cbtrace PRIVATE /EHsc /O2 /W3)
  target_compile_definitions(cbtrace PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
  set_property(TARGET cbtrace PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")   # no VC runtime DLL needed
endif()
set_target_properties(cbtrace PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
                                         LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
