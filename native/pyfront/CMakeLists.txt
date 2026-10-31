# pyfront -- a Python 3 parser in C++: source -> an AST shaped like CPython's `ast`.
#
#   cmake -S native/pyfront -B native/pyfront/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build native/pyfront/build
#   ctest --test-dir native/pyfront/build
cmake_minimum_required(VERSION 3.20)
project(pyfront LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
if(MSVC)
  add_compile_options(/W4 /permissive- /utf-8 /EHsc)
endif()

set(PYFRONT_SOURCES src/lexer.cpp src/literals.cpp src/unicode.cpp src/parser.cpp src/dump.cpp src/capi.cpp)

# The library: static for C++ consumers (native/l2 and the inference stage link this) ...
add_library(pyfront STATIC ${PYFRONT_SOURCES})
target_include_directories(pyfront PUBLIC include)

# ... and a DLL exporting the C ABI (pyfront/pyfront.h).
add_library(pyfront_c SHARED ${PYFRONT_SOURCES})
target_include_directories(pyfront_c PUBLIC include)
target_compile_definitions(pyfront_c PRIVATE PYFRONT_BUILD)
set_target_properties(pyfront_c PROPERTIES OUTPUT_NAME pyfront ARCHIVE_OUTPUT_NAME pyfront_dll)

add_executable(pyfront_dump tools/pyfront_dump.cpp)
target_link_libraries(pyfront_dump PRIVATE pyfront)

enable_testing()
add_executable(pyfront_tests tests/pyfront_tests.cpp)
target_link_libraries(pyfront_tests PRIVATE pyfront)
add_test(NAME pyfront_grammar COMMAND pyfront_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/cases)
