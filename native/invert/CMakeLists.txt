# CodeBreaker's compiler models in C++: codebreaker/invert/ ported by hand, piece by piece (first:
# flow16, the control-flow graph and the structurer the 16-bit models share).  A DLL with a C
# interface (cbinvert.h), loaded by codebreaker/invert/native_invert.py through ctypes.
#
#   (in a vcvars64 shell)
#   cmake -S native/invert -B native/invert/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build native/invert/build
cmake_minimum_required(VERSION 3.16)
project(codebreaker_invert CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(cbinvert SHARED
  api.cc       # the C interface: text in, JSON out, the Python exception kinds
  flow16.cc    # invert/flow16.py: blocks, edges, switches, dominators, the structure tree
  cs.cc        # Capstone: the capstone.dll of Python's `capstone` package, loaded by path
  msc6.cc      # invert/msc6.py: the Microsoft C 6.0a model
  msc6_helpers.cc    # msc6_calls, segment_thunk, whole_function, msc6_function._mask
  msc6_small.cc      # msc6_function.invert_cleanup, msc6_walk, msc6_templates
  msc6_registers.cc  # msc6_registers.invert_reg, invert_loop
  pyval.cc         # Python values (tuples, lists, dicts) and their JSON form for the shim
  msvc2.cc           # msvc2_forward, msvc2, msvc2_registers: the MSVC 2.0 32-bit models
  early_c.cc         # early_c: the MC 2.03 / 3.00 register tracker, recover, recover_cfg
  early_c_text.cc    # early_c's text passes, detect_copies, to_c
  ${CMAKE_CURRENT_SOURCE_DIR}/../l2/src/pyre.cc    # Python's re (native/l2's pyre, with what it needs)
  ${CMAKE_CURRENT_SOURCE_DIR}/../l2/src/pystr.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/../l2/src/value.cc)
# capstone.h: the headers Python's `capstone` package carries (the same version as its DLL)
if(NOT CAPSTONE_INCLUDE)
  execute_process(COMMAND python -c "import capstone, os; print(os.path.join(os.path.dirname(capstone.__file__), 'include'))"
                  OUTPUT_VARIABLE CAPSTONE_INCLUDE OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()
message(STATUS "capstone headers: ${CAPSTONE_INCLUDE}")
target_include_directories(cbinvert PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../l2/src ${CAPSTONE_INCLUDE})
if(MSVC)
  target_compile_options(cbinvert PRIVATE /EHsc /O2 /W3 /utf-8)
  target_compile_definitions(cbinvert PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
  set_property(TARGET cbinvert PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()
set_target_properties(cbinvert PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
                                          LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
