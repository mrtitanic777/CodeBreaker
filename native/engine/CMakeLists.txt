# CodeBreaker's engine and store in C++: the program database (codebreaker/engine/facts.py), the
# incremental engine's core -- the step registry, the staleness rule, the scheduler and its worker
# pool (engine/engine.py) --, the function store (store/functions.py), the verdict vocabulary
# (verify/verdicts.py) and the knowledge base's database (knowledge/store.py, the digests of
# knowledge/reads.py), all on SQLite's C API.  A DLL with a C interface (cbengine.h), loaded by
# codebreaker/engine/native_engine.py through ctypes; steps still written in Python are called
# back through it.
#
#   (in a vcvars64 shell)
#   cmake -S native/engine -B native/engine/build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build native/engine/build
#   ctest --test-dir native/engine/build
#
# SQLite: the header and the DLL of an installed SQLite (MSYS2's UCRT64 build by default: its
# libsqlite3-0.dll needs only KERNEL32 and the UCRT, so MSVC code can call it).  SQLITE_DIR names
# another (a folder with include/sqlite3.h and bin/<the DLL>).  The import library is made here
# from the DLL's exports; the DLL is copied beside cbengine.dll.
cmake_minimum_required(VERSION 3.16)
project(codebreaker_engine CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT SQLITE_DIR)
  set(SQLITE_DIR "C:/msys64/ucrt64")
endif()
if(NOT SQLITE_DLL_NAME)
  set(SQLITE_DLL_NAME "libsqlite3-0.dll")
endif()
set(SQLITE_DLL "${SQLITE_DIR}/bin/${SQLITE_DLL_NAME}")
if(NOT EXISTS "${SQLITE_DIR}/include/sqlite3.h" OR NOT EXISTS "${SQLITE_DLL}")
  message(FATAL_ERROR "SQLite not found: ${SQLITE_DIR}/include/sqlite3.h and ${SQLITE_DLL} (set SQLITE_DIR)")
endif()
message(STATUS "sqlite: ${SQLITE_DIR} (${SQLITE_DLL_NAME})")
# the header alone (the folder beside it holds that toolchain's own C headers, not MSVC's)
file(COPY ${SQLITE_DIR}/include/sqlite3.h DESTINATION ${CMAKE_BINARY_DIR}/sqlite_include)

# the import library, from the DLL's exports
find_program(DUMPBIN dumpbin REQUIRED)
find_program(LIBEXE lib REQUIRED)
execute_process(COMMAND ${DUMPBIN} /nologo /exports ${SQLITE_DLL} OUTPUT_VARIABLE _exports)
string(REGEX MATCHALL "[ \t]+[0-9]+[ \t]+[0-9A-Fa-f]+[ \t]+[0-9A-Fa-f]+[ \t]+(sqlite3[A-Za-z0-9_]*)" _lines "${_exports}")
set(_def "LIBRARY ${SQLITE_DLL_NAME}\nEXPORTS\n")
foreach(_l ${_lines})
  string(REGEX REPLACE ".*[ \t](sqlite3[A-Za-z0-9_]*)$" "\\1" _name "${_l}")
  string(APPEND _def "  ${_name}\n")
endforeach()
file(WRITE ${CMAKE_BINARY_DIR}/sqlite3.def "${_def}")
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/sqlite3.lib
                   COMMAND ${LIBEXE} /nologo /def:${CMAKE_BINARY_DIR}/sqlite3.def /machine:x64
                           /out:${CMAKE_BINARY_DIR}/sqlite3.lib
                   DEPENDS ${CMAKE_BINARY_DIR}/sqlite3.def)
add_custom_target(sqlite_implib DEPENDS ${CMAKE_BINARY_DIR}/sqlite3.lib)

add_library(cbengine SHARED
  util.cc       # SHA-1 / SHA-256, JSON as Python writes it, Python's repr of text, the row buffers
  db.cc         # SQLite: a connection, statements, rows
  facts.cc      # the program database (facts.py)
  engine.cc     # the step registry, a run, its contexts, the worker pool (engine.py)
  verdicts.cc   # the verdict vocabulary (verify/verdicts.py)
  store.cc      # the function store (store/functions.py)
  knowledge.cc  # the knowledge base's database (knowledge/store.py, reads.py's digests)
  api.cc)       # the C interface
add_dependencies(cbengine sqlite_implib)
target_include_directories(cbengine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR}/sqlite_include)
target_link_libraries(cbengine PRIVATE ${CMAKE_BINARY_DIR}/sqlite3.lib bcrypt)
if(MSVC)
  target_compile_options(cbengine PRIVATE /EHsc /O2 /W3 /utf-8)
  target_compile_definitions(cbengine PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX CBE_BUILD)
  set_property(TARGET cbengine PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")   # no VC runtime DLL needed
endif()
set_target_properties(cbengine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
                                          LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
                                          ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
add_custom_command(TARGET cbengine POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SQLITE_DLL} ${CMAKE_BINARY_DIR}/bin/)

# the tests: the C interface alone, no Python
enable_testing()
add_executable(cbengine_test test/engine_test.cc)
target_include_directories(cbengine_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cbengine_test PRIVATE cbengine)
if(MSVC)
  target_compile_options(cbengine_test PRIVATE /EHsc /W3 /utf-8)
  target_compile_definitions(cbengine_test PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
  set_property(TARGET cbengine_test PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()
set_target_properties(cbengine_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_test(NAME cbengine_test COMMAND cbengine_test ${CMAKE_BINARY_DIR}/testdata
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
