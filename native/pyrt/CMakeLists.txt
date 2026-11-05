# pyrt -- the runtime of pyengine's dynamic mode: py::var (include/py_object.hpp), Python's object
# model, builtins and the library modules CodeBreaker uses, in C++. Generated C++ links pyrt.lib.
#
#   (vcvars64)  cmake -S native/pyrt -B native/pyrt/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#               cmake --build native/pyrt/build
#               ctest --test-dir native/pyrt/build
cmake_minimum_required(VERSION 3.20)
project(pyrt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
if(MSVC)
  add_compile_options(/W3 /permissive- /utf-8 /EHsc /bigobj /Zc:__cplusplus)
  add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS)
endif()

set(L2 ${CMAKE_CURRENT_SOURCE_DIR}/../l2)
set(PYENGINE ${CMAKE_CURRENT_SOURCE_DIR}/../pyengine)
add_library(pyrt STATIC
  src/object.cc       # types, truth, attribute lookup and descriptors, classes, super, isinstance
  src/call.cc         # argument binding, functions, natives, bound methods, calling a class
  src/exc.cc          # the exception hierarchy
  src/module.cc       # modules and import
  src/ops.cc          # operators, comparisons, hashing, repr
  src/str.cc          # str, format specs, str.format, %-formatting
  src/bytes.cc        # bytes, bytearray, codecs
  src/containers.cc   # list, tuple, range, slice, iteration, items, len, sort
  src/dict.cc         # dict, set, frozenset
  src/builtins.cc     # the builtin types and functions, the runtime's start
  src/gen.cc          # generators on fibers
  src/io.cc           # open, files, StringIO / BytesIO, sys
  src/lib_os.cc       # os, os.path, shutil, glob, fnmatch, tempfile, subprocess, time, stat, errno
  src/lib_re.cc       # re over native/l2's pyre
  src/lib_misc.cc     # json, struct, hashlib, collections, functools, itertools, math, string, textwrap, copy ...
  src/lib_more.cc     # enum, dataclasses, contextlib, threading, heapq, operator, importlib, typing ...; the GIL
  ${L2}/src/pystr.cc         # (pyre's code point helpers)
  ${PYENGINE}/src/big.cc     # Python's unbounded int (shared with pyengine)
  ${L2}/src/pyre.cc)         # Python-compatible regular expressions (shared with native/l2)
target_include_directories(pyrt PUBLIC include PRIVATE src ${PYENGINE}/src ${L2}/src)
target_link_libraries(pyrt PUBLIC bcrypt shell32)

enable_testing()
add_executable(pyrt_tests tests/pyrt_tests.cpp)
target_link_libraries(pyrt_tests PRIVATE pyrt)
if(MSVC)
  target_link_options(pyrt_tests PRIVATE /STACK:268435456)
endif()
add_test(NAME pyrt_core COMMAND pyrt_tests)
