# CodeBreaker's L2 core and C back end in C++ (docs/NATIVE.md step 5): the L2 tree of
# codebreaker/translate/ir.py + l2.py (read, written and validated as Python does) and the C writer
# of codebreaker/translate/backends/c.py. A DLL with a C interface (l2api.h), loaded by
# codebreaker/translate/native_l2.py through ctypes; CB_NATIVE_L2 chooses it.
#
#   (in a vcvars64 shell)
#   cmake -S native/l2 -B native/l2/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build native/l2/build
#   ctest --test-dir native/l2/build
cmake_minimum_required(VERSION 3.16)
project(codebreaker_l2 CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# the L2 core: the node model, JSON as Python's json, the schema (what the translator back ends and
# the porting engine build on: src/l2.hpp, src/value.hpp, src/json.hpp)
add_library(l2core STATIC
  src/value.cc     # a value with Python's semantics (dicts in order, shared containers)
  src/pystr.cc     # Python's str methods, int(), repr()
  src/json.cc      # json.loads / json.dumps as Python's, and the shim's tagged form
  src/l2.cc)       # the L2 node model, from_json / to_json, the schema
target_include_directories(l2core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/include)
if(MSVC)
  target_compile_options(l2core PRIVATE /EHsc /utf-8 /W3)
endif()

# the C back end (backends/c.py), on the core
set(L2C_SRC
  src/pyre.cc      # Python's re: the C back end's patterns as written
  src/cemit.cc     # the core's pretty printer and operator stack (layout)
  src/cback.cc     # expressions, statements, functions
  src/cpasses.cc   # c_text's passes done on the tree
  src/cpasses2.cc  # ... continued: the locals' layout, the 32-bit and PowerPC paths' own
  src/cmodule.cc)  # what the module decides (prototypes, plans, names)
set(HAVE_CBACK OFF)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/cback.cc)
  set(HAVE_CBACK ON)
  add_library(l2c STATIC ${L2C_SRC})
  target_link_libraries(l2c PUBLIC l2core)
  if(MSVC)
    target_compile_options(l2c PRIVATE /EHsc /bigobj /utf-8 /W3)
  endif()
endif()

add_library(cbl2 SHARED src/api.cc)
target_compile_definitions(cbl2 PRIVATE CBL2_EXPORTS)
if(HAVE_CBACK)
  target_link_libraries(cbl2 PRIVATE l2c)
  target_compile_definitions(cbl2 PRIVATE CBL2_CBACK)
else()
  target_link_libraries(cbl2 PRIVATE l2core)
endif()
if(MSVC)
  target_compile_options(cbl2 PRIVATE /EHsc /utf-8)
endif()

enable_testing()
add_executable(l2_tests tests/test_l2.cc)
if(HAVE_CBACK)
  target_link_libraries(l2_tests PRIVATE l2c)
  target_compile_definitions(l2_tests PRIVATE CBL2_CBACK)
else()
  if(HAVE_CBACK)
  target_link_libraries(l2_tests PRIVATE l2c)
  target_compile_definitions(l2_tests PRIVATE CBL2_CBACK)
else()
  target_link_libraries(l2_tests PRIVATE l2core)
endif()
endif()
target_compile_definitions(l2_tests PRIVATE L2_SCHEMA="${CMAKE_CURRENT_SOURCE_DIR}/../../codebreaker/translate/l2.schema.json"
                                            L2_TESTDATA="${CMAKE_CURRENT_SOURCE_DIR}/tests/data")
if(MSVC)
  target_compile_options(l2_tests PRIVATE /EHsc /utf-8)
  target_link_options(l2_tests PRIVATE /STACK:268435456)
endif()
add_test(NAME l2_tests COMMAND l2_tests)
