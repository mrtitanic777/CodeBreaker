# CodeBreaker's Xbox 360 executable engine (MIT -- see LICENSE): reads a
# .xex, writes its basefile (the PowerPC image inside), and converts, patches and re-signs it.
#
#   cmake -S native/xex -B native/xex/build -G "Visual Studio 18 2026"
#   cmake --build native/xex/build --config Release --target xex
#
# Python drives it through codebreaker/formats/xex.py and `codebreaker xex <command>`.
cmake_minimum_required(VERSION 3.16)
project(codebreaker_xex CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The engine: every source but the program's entry points.
file(GLOB ENGINE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/crypto/*.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/src/compress/*.cpp)
list(REMOVE_ITEM ENGINE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/selftest.cpp)
add_library(xexcore STATIC ${ENGINE_SRC})
target_include_directories(xexcore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
if(MSVC)
  target_compile_definitions(xexcore PUBLIC _CRT_SECURE_NO_WARNINGS)
  target_compile_options(xexcore PUBLIC /EHsc /utf-8)
endif()

add_executable(xex src/main.cpp src/selftest.cpp)
target_link_libraries(xex PRIVATE xexcore)
set_target_properties(xex PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

# Byte-identity harnesses, run by hand against samples that stay outside the repository:
#   test_lzxenc <basefile> <expected LZX stream|->     test_sign <signed.xex>
foreach(t test_lzxenc test_sign)
  add_executable(${t} EXCLUDE_FROM_ALL tools/${t}.cpp)
  target_link_libraries(${t} PRIVATE xexcore)
  set_target_properties(${t} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
endforeach()
