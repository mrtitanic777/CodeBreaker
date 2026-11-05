# pyrt -- the runtime of pyengine's dynamic mode: py::var (include/py_object.hpp), Python's object
# model, builtins and the library modules CodeBreaker uses, in C++. Generated C++ links pyrt.lib.
#
#   (vcvars64)  cmake -S native/pyrt -B native/pyrt/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#               cmake --build native/pyrt/build
#               ctest --test-dir native/pyrt/build
cmake_minimum_required(VERSION 3.20)
project(pyrt LANGUAGES C CXX)

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
set(PYFRONT ${CMAKE_CURRENT_SOURCE_DIR}/../pyfront)
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
  src/lib_db.cc       # sqlite3 (over native/engine's vendored SQLite), pickle
  src/lib_ast.cc      # ast (over native/pyfront), inspect
  src/lib_extra.cc    # the C halves of standard modules translated from CPython (gettext, _random, _csv, zlib ...)
  src/lib_ctypes.cc   # ctypes: calls straight into DLLs (structures, pointers, arrays, callbacks)
  src/lib_pytest.cc   # pytest: the API the test files use, and the runner (-m pytest)
  src/lib_xml.cc      # pyexpat: the XML parser xml.etree / xml.dom.minidom (CPython's Python) call
  src/lib_pil.cc      # PIL.Image: the surface CodeBreaker uses (PNG, PPM; modes L LA P RGB RGBA)
  src/lib_crypto.cc   # cryptography (hashes, HMAC, AES / 3DES / ARC4, RSA numbers) and DES over bcrypt
  src/match.cc        # what match statements ask of a subject (sequence, mapping and class patterns)
  src/lib_async.cc    # sys's async generator hooks; the networking modules' registration
  src/lib_net.cc      # _socket and select over Winsock, _overlapped (IOCP) and _winapi: under CPython's socket, selectors, asyncio
  src/lib_mcp.cc      # the mcp package's surface CodeBreaker's MCP server uses (MCPServer over stdio, Client, ToolAnnotations): mcp 2.3's wire behaviour
  ${L2}/src/pystr.cc         # (pyre's code point helpers)
  ${PYENGINE}/src/big.cc     # Python's unbounded int (shared with pyengine)
  ${L2}/src/pyre.cc)         # Python-compatible regular expressions (shared with native/l2)
target_include_directories(pyrt PUBLIC include PRIVATE src ${PYENGINE}/src ${L2}/src ${PYFRONT}/include ${PYFRONT}/src)
# SQLite: the amalgamation native/engine vendors (third_party/sqlite), for the sqlite3 module
set(SQLITE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../engine/third_party/sqlite)
add_library(pyrt_sqlite3 STATIC ${SQLITE_SRC}/sqlite3.c)
target_include_directories(pyrt_sqlite3 PUBLIC ${SQLITE_SRC})
target_compile_definitions(pyrt_sqlite3 PRIVATE SQLITE_THREADSAFE=1)
if(MSVC)
  target_compile_options(pyrt_sqlite3 PRIVATE /O2 /W0)
endif()
set_target_properties(pyrt_sqlite3 PROPERTIES LINKER_LANGUAGE CXX)
# the parser (native/pyfront), for the ast module
add_library(pyrt_pyfront STATIC ${PYFRONT}/src/lexer.cpp ${PYFRONT}/src/literals.cpp ${PYFRONT}/src/unicode.cpp ${PYFRONT}/src/parser.cpp ${PYFRONT}/src/dump.cpp)
target_include_directories(pyrt_pyfront PUBLIC ${PYFRONT}/include)
set_target_properties(pyrt_pyfront PROPERTIES CXX_STANDARD 20)
# zlib 1.3.1, the copy native/decompiler carries (its crc32: src/zcrc32.c), for the zlib module
set(ZLIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../decompiler/core/zlib)
add_library(pyrt_zlib STATIC ${ZLIB_SRC}/adler32.c ${ZLIB_SRC}/deflate.c ${ZLIB_SRC}/inflate.c ${ZLIB_SRC}/inffast.c
            ${ZLIB_SRC}/inftrees.c ${ZLIB_SRC}/trees.c ${ZLIB_SRC}/zutil.c src/zcrc32.c)
target_include_directories(pyrt_zlib PUBLIC ${ZLIB_SRC})
if(MSVC)
  target_compile_options(pyrt_zlib PRIVATE /O2 /W0)
endif()
target_link_libraries(pyrt PUBLIC pyrt_sqlite3 pyrt_pyfront pyrt_zlib bcrypt shell32 ws2_32 mswsock)

enable_testing()
add_executable(pyrt_tests tests/pyrt_tests.cpp)
target_link_libraries(pyrt_tests PRIVATE pyrt)
if(MSVC)
  target_link_options(pyrt_tests PRIVATE /STACK:268435456)
endif()
add_test(NAME pyrt_core COMMAND pyrt_tests)
# coroutines, async generators, async for / with and the match helpers, in the shapes the emitter writes
add_executable(pyrt_async_tests tests/pyrt_async_tests.cpp)
target_link_libraries(pyrt_async_tests PRIVATE pyrt)
if(MSVC)
  target_link_options(pyrt_async_tests PRIVATE /STACK:268435456)
endif()
add_test(NAME pyrt_async COMMAND pyrt_async_tests)
