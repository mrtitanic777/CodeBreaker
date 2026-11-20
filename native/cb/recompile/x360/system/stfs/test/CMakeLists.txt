# Standalone tests of the STFS library (build.bat builds and runs them).
cmake_minimum_required(VERSION 3.20)
project(stfs_test CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("${CMAKE_CURRENT_LIST_DIR}/../stfs.cmake")

add_executable(stfs_test stfs_test.cpp ${X360_STFS_SOURCES})
# log.h: this folder's stand-in for the runtime's.
target_include_directories(stfs_test PRIVATE "${CMAKE_CURRENT_LIST_DIR}" ${X360_STFS_INCLUDE_DIR})

target_link_libraries(stfs_test PRIVATE bcrypt)
