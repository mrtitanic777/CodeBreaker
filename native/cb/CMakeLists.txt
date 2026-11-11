# native/cb: CodeBreaker's idiomatic C++ tree (docs/CPP_DESIGN.md). One static library per area, one test
# executable for all of them. Adding an area = one `cb_area(<dir>)` line below; its `*_test.cpp` files join
# cb_tests automatically.
cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0091 NEW)        # MSVC_RUNTIME_LIBRARY is honoured (static CRT, as the other libraries)

get_filename_component(CB_NATIVE_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
set(CB_THIRD_PARTY "${CB_NATIVE_ROOT}/third_party")

# The flags of docs/CPP_DESIGN.md 3.1, applied to every cb_* target.
function(cb_apply_flags target)
  target_compile_features(${target} PUBLIC cxx_std_20)
  set_target_properties(${target} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  target_include_directories(${target} PUBLIC "${CB_NATIVE_ROOT}" "${CB_THIRD_PARTY}" "${CB_THIRD_PARTY}/include")
  target_compile_definitions(${target} PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN UNICODE _UNICODE)
  target_compile_options(${target} PRIVATE /permissive- /utf-8 /EHsc /W4 /WX /Zc:__cplusplus /Zc:char8_t- /bigobj)
  if(CB_ASAN)
    target_compile_options(${target} PRIVATE /fsanitize=address /Zi)
    target_link_options(${target} PUBLIC /INCREMENTAL:NO)
  endif()
endfunction()

# cb_area(<dir>): static library cb_<dir> from <dir>/*.cpp (not *_test.cpp, not *_main.cpp); the area's tests
# are collected for cb_tests.
set_property(GLOBAL PROPERTY CB_TEST_SOURCES "")
set_property(GLOBAL PROPERTY CB_AREAS "")
function(cb_area dir)
  file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/${dir}/*.cpp")
  file(GLOB_RECURSE tests CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/${dir}/*_test.cpp")
  list(FILTER sources EXCLUDE REGEX "(_test|_main)\\.cpp$")
  list(FILTER sources EXCLUDE REGEX "/testdata/")   # test inputs, not the area's code
  list(FILTER tests EXCLUDE REGEX "/testdata/")
  add_library(cb_${dir} STATIC ${sources})
  cb_apply_flags(cb_${dir})
  set_property(GLOBAL APPEND PROPERTY CB_AREAS cb_${dir})
  get_property(all_tests GLOBAL PROPERTY CB_TEST_SOURCES)
  set_property(GLOBAL PROPERTY CB_TEST_SOURCES ${all_tests} ${tests})
endfunction()

# CodeBreaker's one XEX reader (native/xex/reader.cmake: the parser, the basefile's decryption and decompression):
# shared with the xex program and the recompiled-game runtime, so it is compiled as it is, under its own flags (not
# /W4 /WX). cb::formats::xex wraps its parser; the tests of cb::formats::lzx use its decoder as the independent oracle.
include("${CB_NATIVE_ROOT}/xex/reader.cmake")
add_library(cb_xex_reader STATIC ${XEX_READER_SOURCES})
set_target_properties(cb_xex_reader PROPERTIES CXX_STANDARD 20 MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(cb_xex_reader PUBLIC "${XEX_READER_INCLUDE_DIR}")
target_compile_options(cb_xex_reader PRIVATE /utf-8 /EHsc /W3)
if(CB_ASAN)
  target_compile_options(cb_xex_reader PRIVATE /fsanitize=address /Zi)
endif()

# zlib 1.3.1 (MSZIP): the copy native/decompiler carries (as native/formats compiles it), with crc32() from
# native/formats/core/zcrc32.c; a C library compiled as it is.
enable_language(C)
file(GLOB CB_ZLIB_SOURCES CONFIGURE_DEPENDS "${CB_NATIVE_ROOT}/decompiler/core/zlib/*.c")
add_library(cb_zlib STATIC ${CB_ZLIB_SOURCES} "${CB_NATIVE_ROOT}/formats/core/zcrc32.c")
set_target_properties(cb_zlib PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(cb_zlib PUBLIC "${CB_NATIVE_ROOT}/decompiler/core/zlib")
target_compile_options(cb_zlib PRIVATE /O2 /W0)
target_compile_definitions(cb_zlib PRIVATE _CRT_SECURE_NO_WARNINGS)

cb_area(core)
target_link_libraries(cb_core PUBLIC bcrypt)
# The SQLite amalgamation (third-party C, its own flags). It stays where the engine keeps it until native/engine
# moves into this tree (docs/CPP_DESIGN.md section 8), when it moves to third_party/ with nlohmann.
set(CB_SQLITE_DIR "${CB_NATIVE_ROOT}/engine/third_party/sqlite")
add_library(cb_sqlite3 STATIC "${CB_SQLITE_DIR}/sqlite3.c")
target_include_directories(cb_sqlite3 PUBLIC "${CB_SQLITE_DIR}")
target_compile_definitions(cb_sqlite3 PRIVATE SQLITE_THREADSAFE=1 _CRT_SECURE_NO_WARNINGS)
target_compile_options(cb_sqlite3 PRIVATE /W0 /O2)
set_target_properties(cb_sqlite3 PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

cb_area(base)
target_link_libraries(cb_base PUBLIC cb_core PRIVATE cb_sqlite3)
cb_area(build)
target_link_libraries(cb_build PUBLIC cb_core cb_base)
cb_area(knowledge)
target_link_libraries(cb_knowledge PUBLIC cb_core)
cb_area(jobs)
target_link_libraries(cb_jobs PUBLIC cb_core)
cb_area(platforms)
target_link_libraries(cb_platforms PUBLIC cb_core)

# The host the worker tests start: a program with a few kinds of share (jobs/worker_probe_main.cpp).
add_executable(cb_worker_probe jobs/worker_probe_main.cpp)
cb_apply_flags(cb_worker_probe)
target_link_libraries(cb_worker_probe PRIVATE cb_jobs cb_core)

cb_area(verify)
target_link_libraries(cb_verify PUBLIC cb_core)

cb_area(app)
target_link_libraries(cb_app PUBLIC cb_core)

cb_area(java)
target_link_libraries(cb_java PUBLIC cb_core)

# The shims at the old header paths (native/typed/cpp/codebreaker/...) are tested too: their test files include the
# old layer, so they build at /W3 without /WX like compat/.
file(GLOB CB_SHIM_TESTS CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/java/shims_test.cpp")
set_source_files_properties(${CB_SHIM_TESTS} PROPERTIES COMPILE_OPTIONS "/W3;/WX-")

cb_area(decompile)
target_link_libraries(cb_decompile PUBLIC cb_core)

cb_area(analyze)
target_link_libraries(cb_analyze PUBLIC cb_core)

cb_area(exact)
target_link_libraries(cb_exact PUBLIC cb_core)

cb_area(compare)
target_link_libraries(cb_compare PUBLIC cb_core)
cb_area(emulate)
target_link_libraries(cb_emulate PUBLIC cb_core)

cb_area(formats)
target_link_libraries(cb_formats PUBLIC cb_core cb_xex_reader cb_zlib)

# compat/py_bridge.hpp (header only, used by the shims at the old header paths): only its test is built here. The
# test includes the old py:: layer, which is not warning-clean, so this one file builds at /W3 without /WX.
file(GLOB CB_COMPAT_TESTS CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/compat/*_test.cpp")
set_source_files_properties(${CB_COMPAT_TESTS} PROPERTIES COMPILE_OPTIONS "/W3;/WX-"
  INCLUDE_DIRECTORIES "${CB_NATIVE_ROOT}/ported/include;${CB_NATIVE_ROOT}/typed/cpp;${CB_NATIVE_ROOT}/l2/src")
get_property(CB_ALL_TESTS_SO_FAR GLOBAL PROPERTY CB_TEST_SOURCES)
set_property(GLOBAL PROPERTY CB_TEST_SOURCES ${CB_ALL_TESTS_SO_FAR} ${CB_COMPAT_TESTS})
# The shims that have a translation unit of their own (they call old ports that cannot be linked into cb_tests) are compiled, not
# run: this keeps them honest until their last consumer is refactored. At /W3 without /WX, like the compat tests.
add_library(cb_shim_check OBJECT
  "${CB_NATIVE_ROOT}/typed/cpp/codebreaker/base/corpus.cpp"
  "${CB_NATIVE_ROOT}/typed/cpp/codebreaker/base/tidy_state.cpp"
  "${CB_NATIVE_ROOT}/typed/cpp/codebreaker/build/functions.cpp")
cb_apply_flags(cb_shim_check)
target_compile_options(cb_shim_check PRIVATE /W3 /WX-)
target_include_directories(cb_shim_check PRIVATE "${CB_NATIVE_ROOT}/typed/cpp" "${CB_NATIVE_ROOT}/ported/include" "${CB_NATIVE_ROOT}/l2/src")
target_link_libraries(cb_shim_check PUBLIC cb_base cb_build)

# where DynLib looks last: the source tree's prebuilt DLLs, so a test run from the build folder finds them even
# before the copy below (see native/third_party/README.md)
set_source_files_properties(core/dynlib.cpp PROPERTIES COMPILE_DEFINITIONS CB_THIRD_PARTY_BIN="${CB_THIRD_PARTY}/bin")

# The prebuilt unicorn/capstone/libz3 DLLs go next to every executable that loads them.
set(CB_PREBUILT_DLLS "${CB_THIRD_PARTY}/bin/unicorn.dll" "${CB_THIRD_PARTY}/bin/capstone.dll" "${CB_THIRD_PARTY}/bin/libz3.dll")
function(cb_copy_prebuilt_dlls target)
  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CB_PREBUILT_DLLS} "$<TARGET_FILE_DIR:${target}>")
endfunction()

# ---- cbtrace.dll: the recorder's C interface (emulate/tracer/cbtrace.h), for the callers that still reach it through
# ctypes or LoadLibrary (the old ports and the dynamic exe). The same sources as cb_emulate's tracer, exported. It is
# handed engines made by the caller's own Unicorn and so calls that DLL (tracer/libs.hpp use_libraries). Not built
# under ASan: the sanitizer runtime does not belong in a DLL the Python process loads.
if(NOT CB_ASAN)
  file(GLOB CB_TRACER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/emulate/tracer/*.cpp")
  add_library(cbtrace SHARED ${CB_TRACER_SOURCES})
  cb_apply_flags(cbtrace)
  target_link_libraries(cbtrace PRIVATE cb_core)
endif()

# ---- the test executable: every area's tests, compiled straight into it (a static library would let the
# linker drop test objects nothing references) -------------------------------------------------------------
get_property(CB_ALL_TESTS GLOBAL PROPERTY CB_TEST_SOURCES)
add_executable(cb_tests core/testing_main.cpp ${CB_ALL_TESTS} cb.manifest)
cb_apply_flags(cb_tests)
target_compile_definitions(cb_tests PRIVATE CB_SOURCE_DIR="${CB_NATIVE_ROOT}")
target_include_directories(cb_tests PRIVATE "${CB_NATIVE_ROOT}/typed/cpp" "${CB_NATIVE_ROOT}/ported/include" "${CB_NATIVE_ROOT}/l2/src")   # for the shims' tests
# Every area library (cb_area records it in CB_AREAS), so a new area needs no edit here.
get_property(CB_ALL_AREAS GLOBAL PROPERTY CB_AREAS)
target_link_libraries(cb_tests PRIVATE ${CB_ALL_AREAS})

target_link_libraries(cb_tests PRIVATE cb_formats cb_core)
# the compat tests include the shims at their old header paths, which include the old py:: layer by its own include root
target_include_directories(cb_tests PRIVATE "${CB_NATIVE_ROOT}/ported/include" "${CB_NATIVE_ROOT}/typed/cpp")
cb_copy_prebuilt_dlls(cb_tests)
add_dependencies(cb_tests cb_worker_probe)
set_source_files_properties(core/testing.cpp PROPERTIES COMPILE_DEFINITIONS CB_SOURCE_DIR="${CB_NATIVE_ROOT}")

enable_testing()
add_test(NAME cb_tests COMMAND cb_tests)


