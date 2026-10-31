# pyengine -- the porting engine's core in C++: scopes, whole-package type inference and lowering of
# Python (parsed by native/pyfront) into L2 (native/l2's documents), with a ledger of refusals.
#
#   (vcvars64)  cmake -S native/pyengine -B native/pyengine/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#               cmake --build native/pyengine/build
#               ctest --test-dir native/pyengine/build
cmake_minimum_required(VERSION 3.20)
project(pyengine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(MSVC)
  add_compile_options(/W3 /permissive- /utf-8 /EHsc /bigobj)
endif()

# the parser (native/pyfront) and the L2 core's values and JSON (native/l2), built here from source
set(PYFRONT ${CMAKE_CURRENT_SOURCE_DIR}/../pyfront)
set(L2 ${CMAKE_CURRENT_SOURCE_DIR}/../l2)
add_library(pyfront_lib STATIC ${PYFRONT}/src/lexer.cpp ${PYFRONT}/src/literals.cpp ${PYFRONT}/src/unicode.cpp
            ${PYFRONT}/src/parser.cpp ${PYFRONT}/src/dump.cpp)
target_include_directories(pyfront_lib PUBLIC ${PYFRONT}/include)
add_library(l2core_lib STATIC ${L2}/src/value.cc ${L2}/src/pystr.cc ${L2}/src/json.cc ${L2}/src/l2.cc)
target_include_directories(l2core_lib PUBLIC ${L2}/src ${L2}/include)

add_library(pyengine STATIC
  src/big.cc        # Python's unbounded ints, for interval bounds
  src/pytypes.cc    # the inference's types and integer intervals
  src/astutil.cc    # the child walker, ast.walk, the parent map, ast helpers
  src/scopes.cc     # names to bindings: module, class, function, comprehension scopes
  src/program.cc    # modules, symbols, imports, containers, the join
  src/infer.cc      # the abstract interpreter, solving to a fixpoint
  src/methods.cc    # builtins, builtin types' methods, the standard library rows
  src/ext.cc        # the extensions: re, files, os, closures, variants, specialization
  src/ir.cc         # the L2 nodes and their JSON
  src/lower.cc      # typed functions into L2; the module units; the ledger
  src/engine.cc)    # the whole run: load, solve, translate, write L2 and the coverage ledger
target_include_directories(pyengine PUBLIC src include)
target_link_libraries(pyengine PUBLIC pyfront_lib l2core_lib)

add_library(pyengine_c SHARED src/capi.cc)
target_link_libraries(pyengine_c PRIVATE pyengine)
target_compile_definitions(pyengine_c PRIVATE PYENGINE_BUILD)
set_target_properties(pyengine_c PROPERTIES OUTPUT_NAME pyengine_api)

add_executable(pyengine_cli tools/pyengine_main.cpp)
target_link_libraries(pyengine_cli PRIVATE pyengine)
set_target_properties(pyengine_cli PROPERTIES OUTPUT_NAME pyengine)
if(MSVC)
  target_link_options(pyengine_cli PRIVATE /STACK:268435456)
endif()

enable_testing()
add_executable(pyengine_tests tests/pyengine_tests.cpp)
target_link_libraries(pyengine_tests PRIVATE pyengine)
if(MSVC)
  target_link_options(pyengine_tests PRIVATE /STACK:268435456)
endif()
foreach(t scopes inference specialization closures generators variants regex files)
  add_test(NAME pyengine_${t} COMMAND pyengine_tests ${t})
endforeach()
