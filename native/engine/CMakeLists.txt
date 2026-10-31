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
# SQLite: the amalgamation in third_party/sqlite (3.54.1, public domain; its README says where it
# came from), compiled in statically -- nothing outside the project, no DLL beside cbengine.dll.
cmake_minimum_required(VERSION 3.16)
project(codebreaker_engine C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SQLITE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/sqlite)
add_library(sqlite3 STATIC ${SQLITE_SRC}/sqlite3.c)
target_include_directories(sqlite3 PUBLIC ${SQLITE_SRC})
target_compile_definitions(sqlite3 PRIVATE SQLITE_THREADSAFE=1)
if(MSVC)
  target_compile_options(sqlite3 PRIVATE /O2 /W0)
  set_property(TARGET sqlite3 PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

add_library(cbengine SHARED
  util.cc       # SHA-1 / SHA-256, JSON as Python writes it, Python's repr of text, the row buffers
  db.cc         # SQLite: a connection, statements, rows
  facts.cc      # the program database (facts.py)
  engine.cc     # the step registry, a run, its contexts, the worker pool (engine.py)
  verdicts.cc   # the verdict vocabulary (verify/verdicts.py)
  store.cc      # the function store (store/functions.py)
  knowledge.cc  # the knowledge base's database (knowledge/store.py, reads.py's digests)
  api.cc)       # the C interface
target_include_directories(cbengine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cbengine PRIVATE sqlite3 bcrypt)
if(MSVC)
  target_compile_options(cbengine PRIVATE /EHsc /O2 /W3 /utf-8)
  target_compile_definitions(cbengine PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX CBE_BUILD)
  set_property(TARGET cbengine PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")   # no VC runtime DLL needed
endif()
set_target_properties(cbengine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
                                          LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
                                          ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

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
