# the translated program: every module's C++ and main.cpp, over native/pyrt (written by pyengine --dynamic)
cmake_minimum_required(VERSION 3.20)
project(pydyn LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
if(MSVC)
  add_compile_options(/W1 /permissive- /utf-8 /EHsc /bigobj)
endif()
add_subdirectory(C:/Users/mrtit/Documents/GitHub/CodeBreaker/.claude/worktrees/typed-1106/dyn_tests/am/pyrt pyrt)
# a whole tree is hundreds of modules: unity batches of about 8000 lines (_unity/, the
# assignment kept in unity.txt) over one precompiled py_object.hpp (each module is its own namespace,
# so batching them is safe); -DPYDYN_UNITY=OFF builds one object per module
option(PYDYN_UNITY "unity batches" ON)
if(PYDYN_UNITY)
  set(PYDYN_SRCS
  _unity/b0.cpp
  )
else()
  set(PYDYN_SRCS
  am.cpp
  )
endif()
add_executable(program
  ${PYDYN_SRCS}
)
target_link_libraries(program PRIVATE pyrt)
target_precompile_headers(program PRIVATE <py_object.hpp>)
if(MSVC)
  target_link_options(program PRIVATE /STACK:268435456)
endif()
