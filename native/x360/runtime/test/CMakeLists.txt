# A test of the runtime's XEX reading (src/xex.cpp's ReadImage, the path XAM's data reads take: xam.xex's online schema
# and country table, huduiskin.xex's common strings): a module's image decoded into host memory, written out so it can
# be held against the xex engine's basefile of the same file (tests/test_x360.py, test_runtime_read_image).
#
#   cmake -S native/x360/runtime/test -B <dir> -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=<clang-cl>
#   cmake --build <dir>
#   <dir>/xex_image_test <module.xex> <image out>
cmake_minimum_required(VERSION 3.20)
project(xex_image_test CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

get_filename_component(X360_RUNTIME "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
include("${X360_RUNTIME}/../../xex/reader.cmake")
# xex.cpp as the runtime compiles it, with the guest memory it places modules in (Load; ReadImage does not use it)
add_executable(xex_image_test xex_image_test.cpp "${X360_RUNTIME}/src/xex.cpp" "${X360_RUNTIME}/src/memory.cpp"
    "${X360_RUNTIME}/src/pool.cpp" ${XEX_READER_SOURCES})
target_include_directories(xex_image_test PRIVATE "${X360_RUNTIME}/src" "${X360_RUNTIME}/ppc_shared"
    "${XEX_READER_INCLUDE_DIR}")
target_compile_options(xex_image_test PRIVATE -fno-strict-aliasing /EHsc /utf-8)
target_compile_definitions(xex_image_test PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS)
