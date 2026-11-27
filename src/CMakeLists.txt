# CodeBreaker in C++ (docs/CPP_DESIGN.md section 8): one tree, `src/`, one build. Each area is a static library
# (cb_core, cb_formats, cb_analyze ...), and each engine inside an area is one too (cb_xex, cb_pysupport, cb_dosexec,
# cb_pyfront, cb_decompiler ...); they link into the one executable `codebreaker` and the test executable `cb_tests`,
# which runs every module's `_test.cpp`. The decompiler engine's programs (decompiler.exe, pcodecheck.exe) are built
# beside them. (native/nasm is NASM, built apart with gcc until x86asm, src/build/x86asm, replaces it.)
#
#   (in a vcvars64 shell, or any shell once cmake/ninja/cl are on PATH)
#   cmake -S src -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
#   cmake --build build
#   build/cb_tests.exe
#
# The program is the `codebreaker` target of this tree; tools/build_codebreaker_exe.cmd builds it and installs it as
# build/codebreaker.exe.
cmake_minimum_required(VERSION 3.20)
project(codebreaker_native LANGUAGES CXX C)

option(CB_ASAN "build with /fsanitize=address (the test target is run under it once per module before merge)" OFF)

# the programs go straight into the build folder (<checkout>/build): build/codebreaker.exe, and the programs and DLLs beside it
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# The static libraries also go into shared ones (cbtrace); ELF needs position-independent code for that.
if(NOT WIN32)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# src: CodeBreaker's idiomatic C++ tree (docs/CPP_DESIGN.md). One static library per area, one test
# executable for all of them. Adding an area = one `cb_area(<dir>)` line below; its `*_test.cpp` files join
# cb_tests automatically.
cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0091 NEW)        # MSVC_RUNTIME_LIBRARY is honoured (static CRT, as the other libraries)

get_filename_component(CB_NATIVE_ROOT "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
set(CB_THIRD_PARTY "${CMAKE_CURRENT_LIST_DIR}/third_party")

# What differs by platform: the libraries sockets and process queries come from, the manifest a Windows executable carries,
# and the prebuilt engine libraries that go next to every executable (src/third_party/README.md).
if(WIN32)
  set(CB_SOCKET_LIBS ws2_32)
  set(CB_PROCESS_LIBS psapi)
  set(CB_MANIFEST cb.manifest)
  set(CB_PREBUILT_LIBS "${CB_THIRD_PARTY}/bin/unicorn.dll" "${CB_THIRD_PARTY}/bin/capstone.dll" "${CB_THIRD_PARTY}/bin/libz3.dll")
else()
  set(CB_SOCKET_LIBS "")
  set(CB_PROCESS_LIBS "")
  set(CB_MANIFEST "")
  set(CB_PREBUILT_LIBS "")
  # The engine libraries the executables load from beside themselves: libraries built for this machine in CB_LINUX_DEPS (Capstone 5,
  # whose records the headers in third_party/include follow, is not a distribution package: docs/LINUX.md), else third_party/bin.
  set(CB_LINUX_DEPS "${CB_NATIVE_ROOT}/../work/linux-deps/prefix/lib" CACHE PATH "Where libcapstone.so (5.0.x) and the like were built to")
  foreach(lib libunicorn.so libcapstone.so libz3.so)
    foreach(dir "${CB_LINUX_DEPS}" "${CB_THIRD_PARTY}/bin")
      if(EXISTS "${dir}/${lib}")
        list(APPEND CB_PREBUILT_LIBS "${dir}/${lib}")
        break()
      endif()
    endforeach()
  endforeach()
endif()

# Compiler flags per compiler family (docs/CPP_DESIGN.md 3.1). MSVC and clang-cl take cl's flags, GCC and Clang the GNU
# ones. Warnings are errors only under MSVC, the compiler whose warnings the code is held to; the others show theirs.
if(MSVC)
  set(CB_FLAGS_STRICT /permissive- /utf-8 /EHsc /W4 /Zc:__cplusplus /Zc:char8_t- /bigobj)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    list(APPEND CB_FLAGS_STRICT /WX)
  endif()
  set(CB_FLAGS_LIB /permissive- /utf-8 /EHsc /W3)
  set(CB_FLAGS_QUIET /W0)
else()
  set(CB_FLAGS_STRICT -finput-charset=UTF-8 -fexec-charset=UTF-8 -Wall -Wextra -fno-char8_t)
  set(CB_FLAGS_LIB -finput-charset=UTF-8 -fexec-charset=UTF-8)
  set(CB_FLAGS_QUIET -w)
  if(MINGW)
    list(APPEND CB_FLAGS_STRICT -Wa,-mbig-obj)   # (as /bigobj: more sections than COFF's default allows)
    list(APPEND CB_FLAGS_LIB -Wa,-mbig-obj)
  endif()
endif()

# The flags of docs/CPP_DESIGN.md 3.1, applied to every cb_* target.
function(cb_apply_flags target)
  target_compile_features(${target} PUBLIC cxx_std_20)
  set_target_properties(${target} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  target_include_directories(${target} PUBLIC "${CB_NATIVE_ROOT}" "${CB_THIRD_PARTY}" "${CB_THIRD_PARTY}/include")
  if(WIN32)
    target_compile_definitions(${target} PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN UNICODE _UNICODE)
  endif()
  target_compile_options(${target} PRIVATE ${CB_FLAGS_STRICT})
  if(CB_ASAN)
    target_compile_options(${target} PRIVATE /fsanitize=address /Z7)   # /Z7: debug info in each object, so the shared precompiled header works
    target_link_options(${target} PUBLIC /INCREMENTAL:NO /DEBUG)   # /DEBUG: a .pdb, so the sanitizer names the functions in its stacks
  endif()
  cb_speed_up(${target})
endfunction()

# Build speed (2026-11-11: a cold build was the fleet's bottleneck). CB_UNITY compiles each target's sources in
# batches of CB_UNITY_BATCH as one translation unit; a source with its own compile options or include folders is left
# out by CMake, and one that cannot share a unit (file-local names that collide) is marked SKIP_UNITY_BUILD_INCLUSION.
# CB_PCH parses the standard headers every file uses once (core/pch.hpp) and reuses that one header in every target.
option(CB_UNITY "compile src in unity batches" ON)
set(CB_UNITY_BATCH 8 CACHE STRING "sources per unity batch")
option(CB_PCH "precompile the standard headers of src" ON)
# Sources that cannot share a unity unit: they define file-local helpers with the same names as a neighbour's. A source can also say so itself with a "// cb-unity: alone" comment.
set(CB_UNITY_ALONE
  port/old_c_passes.cpp port/union_views.cpp source/vxd/symbols.cpp knowledge/value.cpp)
# The tests that cannot share a unity unit with their neighbours (file-scope using-directives, same-named helpers): each its own unit.
list(APPEND CB_UNITY_ALONE
  analyze/origin_test.cpp
  analyze/ppc_calls_test.cpp
  build/arm_asm_test.cpp
  build/x86asm/encode_test.cpp
  build/x86asm/preproc_test.cpp
  decompile/arm_test.cpp
  decompile/frame16_test.cpp
  decompile/m6502_test.cpp
  decompile/shape16_test.cpp
  decompile/z80_test.cpp
  emulate/emulator_test.cpp
  emulate/windows16_test.cpp
  engine/win16_test.cpp
  exact/draft/forms_test.cpp
  exact/draft/shapes_test.cpp
  exact/draft/text_test.cpp
  exact/fixupp_test.cpp
  exact/recut_test.cpp
  exact/seed_test.cpp
  formats/fonts_test.cpp
  formats/inspect_test.cpp
  formats/omf_test.cpp
  formats/xbox/crypto/xecrypt_test.cpp
  formats/xbox/nand_test.cpp
  invert/command_test.cpp
  invert/early_c_forward_test.cpp
  java/apimap_test.cpp
  java/projects/minecraft/controller_nav_test.cpp
  java/projects/minecraft/keybind_mark_test.cpp
  port/asmdata_test.cpp
  port/old_c_text_test.cpp
  port/real_program_test.cpp
  port/sanitize_run_test.cpp
  recompile/builders/memory_test.cpp
  recompile/config_test.cpp
  source/ne_asm/segdiff_test.cpp
  source/placement_test.cpp
  symbols/demangle_test.cpp
  symbols/libraries/omf_test.cpp
  symbols/libraries/package_test.cpp
  translate/backends/c_test.cpp
  translate/backends/java_libc_test.cpp
  translate/dotnet/package_test.cpp
  translate/l2_test.cpp
  verify/concolic_test.cpp
  verify/gate16/contract16_test.cpp
  verify/teeth_test.cpp
  translate/backends/java_objects_test.cpp
)
function(cb_unity target sources)
  if(NOT CB_UNITY)
    return()
  endif()
  # Each source goes into the unit its path hashes to, not the unit its place in the list gives: adding or removing a file then recompiles only
  # its own unit, not every unit after it (2026-11-19). The number of units is the power of two at or above count / CB_UNITY_BATCH, so it
  # changes (and every unit of the target is made again) only when a target doubles or halves.
  list(LENGTH sources count)
  math(EXPR want "(${count} + ${CB_UNITY_BATCH} - 1) / ${CB_UNITY_BATCH}")
  set(units 1)
  while(units LESS want)
    math(EXPR units "${units} * 2")
  endwhile()
  set_target_properties(${target} PROPERTIES UNITY_BUILD ON UNITY_BUILD_MODE GROUP)
  foreach(src IN LISTS sources)
    file(RELATIVE_PATH rel "${CMAKE_CURRENT_FUNCTION_LIST_DIR}" "${src}")
    file(READ "${src}" text)
    # windows.h defines macros (far, near, pascal, min, LoadLibrary...) that would leak into the next file of the unit
    # (or the source says so itself: a "cb-unity: alone" comment)
    # (unicorn.h includes windows.h on Windows, so a source using the emulator's table is alone too)
    if(rel IN_LIST CB_UNITY_ALONE OR text MATCHES "<[Ww]indows\\.h>" OR text MATCHES "unicorn_api\\.hpp|<unicorn/" OR text MATCHES "cb-unity: alone")
      set_source_files_properties("${src}" PROPERTIES SKIP_UNITY_BUILD_INCLUSION ON)
    else()
      string(MD5 hash "${rel}")
      string(SUBSTRING "${hash}" 0 6 hash)
      math(EXPR unit "0x${hash} % ${units}")
      set_source_files_properties("${src}" PROPERTIES UNITY_GROUP "u${unit}")
    endif()
  endforeach()
endfunction()
function(cb_speed_up target)
  if(CB_PCH)
    if(target STREQUAL "cb_core")
      target_precompile_headers(cb_core PRIVATE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/core/pch.hpp")
    elseif(target STREQUAL "cb_tests")   # (its own: the test files are compiled at /Od, and a precompiled header is used only with the options it was made with)
      target_precompile_headers(cb_tests PRIVATE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/core/pch.hpp")
    elseif(TARGET cb_core)
      target_precompile_headers(${target} REUSE_FROM cb_core)
    endif()
  endif()
endfunction()

# cb_area(<dir>): static library cb_<dir> from <dir>/*.cpp (not *_test.cpp, not *_main.cpp); the area's tests
# are collected for cb_tests.
set_property(GLOBAL PROPERTY CB_TEST_SOURCES "")
set_property(GLOBAL PROPERTY CB_AREAS "")
# The engines that live inside an area but are built by their own rules (their own CMakeLists.txt, flags and programs:
# the decompiler, the DOS runner, the Python parser, the XEX tools, the Xbox 360 runtime, XenonRecomp, NASM...). An
# area's library never takes their sources; each is added below with add_subdirectory or compiled by its own rules.
set(CB_ENGINE_DIRS
  decompile/engine exact/emu/dosexec translate/support translate/frontends/python/pyfront formats/xex
  recompile/x360 recompile/xenonrecomp recompile/rexglue app/memcap third_party build/x86link build/x86cc)
function(cb_area dir)
  file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/${dir}/*.cpp")
  file(GLOB_RECURSE tests CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/${dir}/*_test.cpp")
  list(FILTER sources EXCLUDE REGEX "(_test|_main)\\.cpp$")
  list(FILTER sources EXCLUDE REGEX "/testdata/")   # test inputs, not the area's code
  list(FILTER tests EXCLUDE REGEX "/testdata/")
  foreach(engine IN LISTS CB_ENGINE_DIRS)
    list(FILTER sources EXCLUDE REGEX "^${CMAKE_CURRENT_LIST_DIR}/${engine}/")
    list(FILTER tests EXCLUDE REGEX "^${CMAKE_CURRENT_LIST_DIR}/${engine}/")
  endforeach()
  add_library(cb_${dir} STATIC ${sources})
  cb_apply_flags(cb_${dir})
  cb_unity(cb_${dir} "${sources}")
  set_property(GLOBAL APPEND PROPERTY CB_AREAS cb_${dir})
  get_property(all_tests GLOBAL PROPERTY CB_TEST_SOURCES)
  set_property(GLOBAL PROPERTY CB_TEST_SOURCES ${all_tests} ${tests})
  set_property(GLOBAL APPEND PROPERTY CB_AREA_TESTS_${dir} ${tests})   # for cb_tests_<area> (end of this file)
endfunction()

# CodeBreaker's XEX engine (formats/xex: the parser, the basefile's decryption and decompression, the conversions), the
# library cb_xex: cb::formats::xex parses, writes basefiles and runs `codebreaker xex` with it; the tests of
# cb::formats::lzx use its decoder as the independent oracle.
include("${CMAKE_CURRENT_LIST_DIR}/formats/xex/CMakeLists.txt")

# zlib 1.3.1 (MSZIP; third_party/zlib, with crc32() from its zcrc32.c): the program's and the decompiler engine's; a C
# library compiled as it is.
enable_language(C)
if(WIN32)
  enable_language(RC)   # (codebreaker.exe's icon and version information: app/codebreaker.rc)
endif()
file(GLOB CB_ZLIB_SOURCES CONFIGURE_DEPENDS "${CB_THIRD_PARTY}/zlib/*.c")
add_library(cb_zlib STATIC ${CB_ZLIB_SOURCES})
set_target_properties(cb_zlib PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(cb_zlib PUBLIC "${CB_THIRD_PARTY}/zlib")
target_compile_options(cb_zlib PRIVATE ${CB_FLAGS_QUIET})
target_compile_definitions(cb_zlib PRIVATE _CRT_SECURE_NO_WARNINGS)

cb_area(core)
if(WIN32)
  target_link_libraries(cb_core PUBLIC bcrypt ws2_32 winhttp)   # (winhttp: core/http, the downloads of a Minecraft version)
else()
  find_package(Threads REQUIRED)
  target_link_libraries(cb_core PUBLIC Threads::Threads ${CMAKE_DL_LIBS})   # (std::thread; dlopen for the engine libraries)
  find_package(OpenSSL REQUIRED)
  target_link_libraries(cb_core PUBLIC OpenSSL::Crypto)   # (core/crypto_cipher_posix.cpp: random bytes, AES/DES, RSA; Windows uses CNG)
endif()
if(NOT MSVC)   # (GCC, on Linux and MinGW alike; the MSVC linker, and clang-cl's, searches every library whole)
  # The static libraries reach into one another in a cycle; GNU ld reads each archive once, in command-line order, so the
  # executables' libraries are linked as one group (every archive is searched again until nothing is left undefined).
  set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> -Wl,--start-group <LINK_LIBRARIES> -Wl,--end-group")
  # (MinGW's CMake puts the libraries into the response file with the objects, which leaves the group above empty: the objects
  # alone go there, the libraries stay on the command line inside it)
  set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_LIBRARIES 0)
endif()
# core16_types.hpp: the records of the core16 cycle (header only; its test sits beside it, not in an area)
get_property(CB_TESTS_BEFORE_CORE16 GLOBAL PROPERTY CB_TEST_SOURCES)
set_property(GLOBAL PROPERTY CB_TEST_SOURCES ${CB_TESTS_BEFORE_CORE16} "${CMAKE_CURRENT_LIST_DIR}/core16_types_test.cpp")
# The SQLite amalgamation (third_party/sqlite; third-party C, its own flags).
set(CB_SQLITE_DIR "${CB_THIRD_PARTY}/sqlite")
add_library(cb_sqlite3 STATIC "${CB_SQLITE_DIR}/sqlite3.c")
target_include_directories(cb_sqlite3 PUBLIC "${CB_SQLITE_DIR}")
target_compile_definitions(cb_sqlite3 PRIVATE SQLITE_THREADSAFE=1 _CRT_SECURE_NO_WARNINGS)
target_compile_options(cb_sqlite3 PRIVATE ${CB_FLAGS_QUIET})
set_target_properties(cb_sqlite3 PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

cb_area(base)
target_link_libraries(cb_base PUBLIC cb_core cb_formats PRIVATE cb_sqlite3)
cb_area(build)
target_link_libraries(cb_build PUBLIC cb_core cb_base cb_formats cb_symbols)
target_link_libraries(cb_build PUBLIC cb_core cb_base cb_formats cb_platforms cb_source cb_symbols)
target_link_libraries(cb_build PUBLIC cb_core cb_base cb_formats)
target_link_libraries(cb_build PUBLIC cb_decompile)   # (the one assembler lives in decompile/engine)
cb_area(knowledge)
target_link_libraries(cb_knowledge PUBLIC cb_core cb_base cb_symbols)   # (cb_base: the SQLite wrapper; cb_symbols: the readers Knowledge gives their memories to -- static libraries may be cyclic)
# The program database (the fact store, over cb::base::sqlite).
cb_area(engine)
target_link_libraries(cb_engine PUBLIC cb_core cb_base cb_knowledge cb_verify cb_jobs cb_decompile cb_build cb_emulate cb_symbols cb_analyze cb_formats cb_platforms cb_translate cb_source cb_java)   # (the lane kit builds every lane's surroundings from these -- static libraries may be cyclic)   # (cb_verify: the verdict and class vocabulary)
cb_area(jobs)
target_link_libraries(cb_jobs PUBLIC cb_core cb_knowledge)
cb_area(platforms)
target_link_libraries(cb_platforms PUBLIC cb_core cb_formats)
# What CodeBreaker made of every program it was given (reach); the platform registry, the media readers and the
# function store's tally reach it through Services.
cb_area(coverage)
target_link_libraries(cb_coverage PUBLIC cb_core cb_formats cb_platforms cb_base cb_engine cb_verify cb_store)   # (ledger: the corpus, the function store, the verdict vocabulary; cb_store: where the store of a state folder is -- static libraries may be cyclic)

# memcap <MB> <program> [args...]: runs a program in a job object with a commit limit (app/memcap; Windows). Built on request.
if(WIN32)
  add_executable(memcap EXCLUDE_FROM_ALL app/memcap/memcap.cpp)
  set_target_properties(memcap PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  target_compile_options(memcap PRIVATE ${CB_FLAGS_LIB})   # (a standalone Win32 program: its own flags, narrow Win32 calls)
endif()

# The host the worker tests start: a program with a few kinds of share (jobs/worker_probe_main.cpp).
add_executable(cb_worker_probe jobs/worker_probe_main.cpp)
cb_apply_flags(cb_worker_probe)
target_link_libraries(cb_worker_probe PRIVATE cb_jobs cb_core)

cb_area(verify)

cb_area(app)
target_link_libraries(cb_app PUBLIC cb_core cb_base cb_formats cb_build cb_symbols cb_analyze cb_source cb_store cb_coverage cb_exact cb_recompile cb_java cb_xna cb_engine cb_jobs cb_ui cb_invert cb_knowledge cb_verify cb_translate)
if(WIN32)
  target_compile_definitions(cb_app PRIVATE _WIN32_WINNT=0x0A00 WINVER=0x0A00)   # (Windows 10: the per-monitor DPI functions; MinGW's headers default lower)
  target_link_libraries(cb_app PUBLIC d2d1 dwrite dwmapi windowscodecs ole32 oleaut32 shell32 uuid user32 advapi32)   # (the window: Direct2D, DirectWrite, DWM, WIC, the file dialog, drag and drop)
  if(MSVC)   # (MSVC and clang-cl: the notification is a Windows toast, made with the Windows Runtime; MinGW's is a balloon: window_shell_win.cpp)
    target_link_libraries(cb_app PUBLIC runtimeobject)
  endif()
endif()

# (zlib for the jar and PNG readers of java/console is the cb_zlib above)
cb_area(java)
target_link_libraries(cb_java PUBLIC cb_core cb_base cb_translate cb_decompile cb_formats PRIVATE cb_zlib)   # (cb_base: the configuration the tools are made from; cb_decompile: the Minecraft decompiler of `java decompile`; cb_formats: the imports of a built PC program, whose runtime DLLs are copied beside it -- static libraries may be cyclic)


# The decompiler engine (decompile/engine): cb_decompiler, and decompiler.exe and pcodecheck.exe, which cb::decompile and
# cb::emulate::pcode start (they are built beside the program and its tests).
include("${CMAKE_CURRENT_LIST_DIR}/decompile/engine/CMakeLists.txt")
cb_area(decompile)
target_link_libraries(cb_decompile PUBLIC cb_core cb_base cb_formats cb_platforms cb_analyze cb_build cb_translate cb_symbols cb_knowledge)   # (cb_symbols, cb_knowledge, cb_base: environment.cpp reads the argument sizes, ordinal and driver tables the base keeps; cb_platforms: the Capstone decoder; cb_analyze: the DOS functions; cb_build: the relocation places of a data segment; cb_translate: the L2 library client and the module model c_text writes)

cb_area(analyze)
target_link_libraries(cb_analyze PUBLIC cb_core cb_base cb_formats cb_knowledge cb_symbols cb_decompile cb_jobs cb_emulate cb_recompile)   # (cb_recompile: function discovery and the disassembler of the ppc-calls command; cb_formats: the PE reader image.hpp takes; cb_base, cb_knowledge: the fact table; cb_symbols: the convention facts)

# The DOS executive (exact/emu/dosexec): cb_dosexec, on Windows, which cb::exact::emu::dos calls in-process.
include("${CMAKE_CURRENT_LIST_DIR}/exact/emu/dosexec/CMakeLists.txt")
cb_area(exact)
if(TARGET cb_dosexec)
  target_link_libraries(cb_exact PUBLIC cb_dosexec)
  target_compile_definitions(cb_exact PRIVATE CB_DOSEXEC)
endif()
target_link_libraries(cb_exact PUBLIC cb_core cb_base cb_formats cb_java cb_invert)   # (cb_invert: the early-C model the draft stage reads the code with, draft/early_c_model)   # (omf_object: an OMF module as the lanes' object record; cb_java: the zip reader of the pre-release Pascal kit, command.cpp -- static libraries may be cyclic)

cb_area(compare)
target_link_libraries(cb_compare PUBLIC cb_core cb_base cb_formats cb_platforms cb_analyze cb_decompile cb_jobs)   # (the readers and the corpus, the decoder, the function starts of each kind, the pool of the product comparison)
cb_area(emulate)
target_link_libraries(cb_emulate PUBLIC cb_core cb_decompile cb_formats cb_symbols cb_knowledge)   # (the tracer's module loader reads PE files, prototypes and the win32 model)

cb_area(formats)
target_link_libraries(cb_formats PUBLIC cb_core cb_xex cb_zlib cb_knowledge)   # (cb_knowledge: the consoles' register and firmware names are descriptions, config/knowledge/<platform>/registers.json -- static libraries may be cyclic)
include("${CMAKE_CURRENT_LIST_DIR}/formats/embed_formats.cmake")   # config/formats/*.fmt built into the program (formats/format_builtin)
target_sources(cb_formats PRIVATE "${CMAKE_BINARY_DIR}/formats_embedded.cpp")
include("${CMAKE_CURRENT_LIST_DIR}/formats/embed_nids.cmake")   # config/nids/*.nids built into the program (formats/nid_data: the consoles' function-ID tables)
target_sources(cb_formats PRIVATE "${CMAKE_BINARY_DIR}/nids_embedded.cpp")
add_executable(format_check EXCLUDE_FROM_ALL formats/format_check_main.cpp)   # the format engine's tree of a file, diffed against the hand-written reader
cb_apply_flags(format_check)
target_link_libraries(format_check PRIVATE cb_formats cb_core)

# XenonRecomp and XenonAnalyse (recompile/xenonrecomp, CodeBreaker's fork) as libraries: cb::recompile::xenon_engine calls
# their mains in-process. Their own build (its third-party libraries, its flags); the programs build on request only.
add_subdirectory(recompile/xenonrecomp "${CMAKE_BINARY_DIR}/xenonrecomp")
# The linker (build/x86link: began as OpenWatcom's wlink, from its own source; Windows: its file layer is Win32)
if(WIN32)
  add_subdirectory(build/x86link "${CMAKE_BINARY_DIR}/wlink")
  # OpenWatcom's C compilers from their own source (build/x86cc: wcc and wcc386, their globals renamed per target)
  add_subdirectory(build/x86cc "${CMAKE_BINARY_DIR}/x86cc")
endif()
# x86link: the linker as CodeBreaker calls it (build/x86link/x86link.cpp), and its proof against the captured links
# (replay.cpp; the test is replay_test.cpp, and x86link_replay runs it alone: x86link_replay <corpus> <scratch>)
add_library(cb_x86link STATIC build/x86link/x86link.cpp build/x86link/replay.cpp)
cb_apply_flags(cb_x86link)
target_link_libraries(cb_x86link PUBLIC cb_core)
if(WIN32)
  target_link_libraries(cb_x86link PRIVATE cb_wlink)
endif()
target_link_libraries(cb_build PUBLIC cb_x86link)
# x86cc: the C compilers as CodeBreaker calls them (build/x86cc/x86cc.cpp), and their proof against the captured compiles
# (replay.cpp; the test is replay_test.cpp, and x86cc_replay runs it alone: x86cc_replay <corpus> <scratch>)
add_library(cb_x86cc STATIC build/x86cc/x86cc.cpp build/x86cc/replay.cpp)
cb_apply_flags(cb_x86cc)
target_link_libraries(cb_x86cc PUBLIC cb_core)
if(WIN32)
  target_link_libraries(cb_x86cc PRIVATE cb_wcc_i86 cb_wcc_386)
endif()
target_link_libraries(cb_build PUBLIC cb_x86cc)
set_property(GLOBAL APPEND PROPERTY CB_TEST_SOURCES "${CMAKE_CURRENT_LIST_DIR}/build/x86cc/replay_test.cpp")
add_executable(x86cc_replay EXCLUDE_FROM_ALL build/x86cc/replay_main.cpp)
cb_apply_flags(x86cc_replay)
target_link_libraries(x86cc_replay PRIVATE cb_x86cc)
set_property(GLOBAL APPEND PROPERTY CB_TEST_SOURCES "${CMAKE_CURRENT_LIST_DIR}/build/x86link/replay_test.cpp")
add_executable(x86link_replay EXCLUDE_FROM_ALL build/x86link/replay_main.cpp)
cb_apply_flags(x86link_replay)
target_link_libraries(x86link_replay PRIVATE cb_x86link)
cb_area(recompile)
target_link_libraries(cb_recompile PRIVATE LibXenonRecomp LibXenonAnalyseMain)
target_link_libraries(cb_recompile PUBLIC cb_core cb_base cb_platforms cb_formats cb_decompile)   # (cb_base: the converted projects of the recompile-project command)

# The recompiler's own pieces (builders, disassembler, the title project) and the source writers (placement, assembly,
# the NE assembly tools).
cb_area(source)
target_link_libraries(cb_source PUBLIC cb_core cb_base cb_formats cb_decompiler)   # (cb_decompiler: the SLEIGH reader of source/processors)

cb_area(invert)
target_link_libraries(cb_invert PUBLIC cb_core cb_base cb_formats cb_platforms cb_build cb_decompile cb_symbols cb_exact)   # (cb_platforms: the 16-bit decoder the models decode with)

cb_area(port)
target_link_libraries(cb_port PUBLIC cb_core cb_base cb_verify cb_translate)
if(TARGET cb_dosexec)
  target_link_libraries(cb_port PUBLIC cb_dosexec)   # (port/verify runs the originals on the DOS executive)
  target_compile_definitions(cb_port PRIVATE CB_DOSEXEC)
endif()

cb_area(symbols)
target_link_libraries(cb_symbols PUBLIC cb_core cb_base cb_formats cb_knowledge cb_compare cb_platforms cb_source cb_store)   # (cb_source: the NE disassembler of debug_build)

cb_area(xna)
target_link_libraries(cb_xna PUBLIC cb_core cb_base cb_formats cb_java cb_decompile cb_dotnet)   # (cb_base: the Converted project; cb_formats: PE and media images; cb_java: the PNG codec of the content converters; cb_decompile: the Xbox effect as HLSL -- static libraries may be cyclic)

# .NET: the ECMA-335 metadata reader (dotnet/metadata) and the decompiler ported from ILSpy (dotnet/decompiler):
# docs/DOTNET_DECOMPILER_PLAN.md.
cb_area(dotnet)
target_link_libraries(cb_dotnet PUBLIC cb_core cb_formats)

# The translator's back ends, front ends and the whole-program judge; the interface's helpers (the window is
# app/window_*: docs/CPP_DESIGN.md section 9).
# pyfront: the Python parser the translator's Python front end reads programs with (translate/frontends/python/pyfront).
include("${CMAKE_CURRENT_LIST_DIR}/translate/frontends/python/pyfront/CMakeLists.txt")
# pysupport: Python's value semantics (py::V, str, json, re) and the L2 schema check, which the back ends on the typed tree use (translate/support).
include("${CMAKE_CURRENT_LIST_DIR}/translate/support/CMakeLists.txt")

cb_area(translate)
target_link_libraries(cb_translate PUBLIC cb_core cb_pyfront cb_pysupport cb_java cb_verify cb_dotnet)   # (cb_dotnet: the CIL lane lowers the .NET decompiler's ILAst, translate/frontends/cil; cb_java: the API-mapping registry java_libc and csharp_java read; cb_java links cb_translate back: static libraries may be cyclic)
cb_area(ui)
target_link_libraries(cb_ui PUBLIC cb_core)

target_link_libraries(cb_verify PUBLIC cb_core cb_base cb_formats cb_platforms cb_emulate cb_jobs cb_decompile PRIVATE cb_zlib ${CB_SOCKET_LIBS})

# The native program the decompiler's tests start in place of decompiler.exe (decompile/fake_decompiler_main.cpp), built beside cb_tests.
add_executable(cb_fake_decompiler decompile/fake_decompiler_main.cpp)
cb_apply_flags(cb_fake_decompiler)

# The machine setup16's tests start in place of QEMU (verify/gate16/fake_qemu_main.cpp), built beside cb_tests.
add_executable(cb_fake_qemu verify/gate16/fake_qemu_main.cpp)
cb_apply_flags(cb_fake_qemu)
target_link_libraries(cb_fake_qemu PRIVATE cb_core ${CB_SOCKET_LIBS})   # (cb_core: the socket and the environment)

target_link_libraries(cb_source PUBLIC cb_core cb_base cb_formats cb_platforms cb_analyze cb_symbols cb_translate)   # (cb_platforms: the one Capstone decoder, for the VxD disassembly; cb_analyze, cb_symbols, cb_translate: the DOS writer reads the functions, the interrupts' names and builds an L2 assembly -- static libraries may be cyclic)

# Where the function store is, opening it, and what `codebreaker versions` asks of it (the Ledger over the corpus, the store and the bodies).
cb_area(store)
target_link_libraries(cb_store PUBLIC cb_core cb_base cb_engine cb_verify cb_compare cb_knowledge)

target_link_libraries(cb_source PUBLIC cb_core cb_base cb_formats cb_platforms cb_analyze cb_symbols cb_translate cb_build cb_decompile cb_compare cb_engine cb_verify cb_exact)   # (cb_platforms: the one Capstone decoder, for the VxD disassembly; cb_analyze, cb_symbols, cb_translate: the DOS writer reads the functions, the interrupts' names and builds an L2 assembly -- static libraries may be cyclic)

target_link_libraries(cb_ui PUBLIC cb_core cb_base cb_engine cb_verify cb_coverage cb_symbols cb_analyze cb_decompile cb_compare cb_store cb_knowledge cb_app cb_platforms cb_source cb_java cb_formats ${CB_SOCKET_LIBS})   # (the ledger, the function store and the verdict vocabulary the service answers from; ws2_32: its client; cb_symbols: the demangler and the names of the MCP server; cb_analyze: its disassembly; cb_decompile, cb_engine: its decompiler; cb_compare, cb_store: the version comparison; cb_app: the command table; cb_platforms, cb_java: what a file is; cb_source: the source tree -- static libraries may be cyclic)
target_link_libraries(cb_verify PUBLIC cb_core cb_base cb_formats cb_platforms cb_emulate cb_jobs cb_decompile cb_engine PRIVATE cb_zlib ${CB_SOCKET_LIBS})   # (cb_engine: the function store reconstruct fills -- static libraries may be cyclic)


# where DynLib looks last: the source tree's prebuilt DLLs, so a test run from the build folder finds them even
# before the copy below (see src/third_party/README.md)
set_source_files_properties(core/dynlib.cpp PROPERTIES COMPILE_DEFINITIONS CB_THIRD_PARTY_BIN="${CB_THIRD_PARTY}/bin")

# The prebuilt unicorn/capstone/libz3 DLLs go next to every executable that loads them.
function(cb_copy_prebuilt_dlls target)
  if(NOT CB_PREBUILT_LIBS)
    return()
  endif()
  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CB_PREBUILT_LIBS} "$<TARGET_FILE_DIR:${target}>")
endfunction()

# ---- cbtrace.dll: the recorder's C interface (emulate/tracer/cbtrace.h), for the callers that still reach it through
# ctypes or LoadLibrary (the Python callers that remain). The same sources as cb_emulate's tracer, exported. It is
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
add_executable(cb_tests core/testing_main.cpp ${CB_ALL_TESTS} ${CB_MANIFEST})
cb_apply_flags(cb_tests)
target_compile_definitions(cb_tests PRIVATE CB_SOURCE_DIR="${CB_NATIVE_ROOT}")
cb_unity(cb_tests "${CB_ALL_TESTS}")
# The test files themselves are compiled without optimization: the code they test is in the area libraries (/O2); a test body
# runs once, and the optimizer is most of what compiling it costs.
if(MSVC)
  target_compile_options(cb_tests PRIVATE /Od)
endif()
# Every area library (cb_area records it in CB_AREAS), so a new area needs no edit here.
get_property(CB_ALL_AREAS GLOBAL PROPERTY CB_AREAS)
target_link_libraries(cb_tests PRIVATE ${CB_ALL_AREAS})

target_link_libraries(cb_tests PRIVATE cb_formats cb_core)
# A running program cannot be overwritten but can be renamed: before codebreaker.exe and cb_tests.exe are linked, a running
# one is moved aside (tools/replace_running.cmd), so a build never fails because the service or a test run holds it.
function(cb_replace_running target)
  if(WIN32)
    add_custom_command(TARGET ${target} PRE_LINK COMMAND cmd /c "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../tools/replace_running.cmd" "$<TARGET_FILE:${target}>" VERBATIM)
  endif()
endfunction()
cb_replace_running(cb_tests)
cb_copy_prebuilt_dlls(cb_tests)
# the kit's worker host (see above): the libraries every area links, and the DLLs next to it
add_executable(cb_lane_worker EXCLUDE_FROM_ALL engine/lane_worker_main.cpp)
cb_apply_flags(cb_lane_worker)
target_link_libraries(cb_lane_worker PRIVATE cb_engine cb_core)
cb_copy_prebuilt_dlls(cb_lane_worker)
# observe's worker host (analyze/observe_worker_main.cpp): the program the tests of running a module's functions in worker processes start.
add_executable(cb_observe_worker EXCLUDE_FROM_ALL analyze/observe_worker_main.cpp)
cb_apply_flags(cb_observe_worker)
target_link_libraries(cb_observe_worker PRIVATE cb_analyze cb_core)
cb_copy_prebuilt_dlls(cb_observe_worker)
# The MCP server as a program of its own over the standard services (the proof harness of the tools; `codebreaker mcp` serves the same).
add_executable(cb_mcp_stdio EXCLUDE_FROM_ALL ui/mcp_stdio_main.cpp)
cb_apply_flags(cb_mcp_stdio)
target_link_libraries(cb_mcp_stdio PRIVATE cb_ui cb_core)
cb_copy_prebuilt_dlls(cb_mcp_stdio)
# The service as a program of its own (`codebreaker serve` is the same command): the proof harness of ui/serve.
add_executable(cb_serve EXCLUDE_FROM_ALL ui/serve_main.cpp)
cb_apply_flags(cb_serve)
target_link_libraries(cb_serve PRIVATE cb_ui cb_core)
cb_copy_prebuilt_dlls(cb_serve)
# The source writers' commands (vxd, ne-asm, driver-source, dos-source, dos-build, driver-judge) as a program of their own: the proof harness of
# source/commands.hpp (the application registers the same commands).
add_executable(cb_source_commands EXCLUDE_FROM_ALL source/commands_main.cpp)
cb_apply_flags(cb_source_commands)
target_link_libraries(cb_source_commands PRIVATE cb_source cb_core)
cb_copy_prebuilt_dlls(cb_source_commands)
add_dependencies(cb_tests cb_worker_probe cb_fake_qemu cb_fake_decompiler cb_lane_worker cb_observe_worker decompiler pcodecheck)
if(NOT WIN32)
  # (the tests name these helper programs with .exe, as on Windows; an ELF file takes any name)
  set_target_properties(cb_worker_probe cb_fake_qemu cb_fake_decompiler cb_lane_worker cb_observe_worker PROPERTIES SUFFIX ".exe")
endif()
set_source_files_properties(core/testing.cpp PROPERTIES COMPILE_DEFINITIONS CB_SOURCE_DIR="${CB_NATIVE_ROOT}")

# cmdrun: runs the command layers of formats, build and analyze with the surroundings the application will give them, to compare each with the
# Python on real inputs (app/cmdrun_main.cpp). Not built by default.
add_executable(cb_cmdrun EXCLUDE_FROM_ALL app/cmdrun_main.cpp)
cb_apply_flags(cb_cmdrun)
target_link_libraries(cb_cmdrun PRIVATE cb_formats cb_build cb_analyze cb_source cb_core)
cb_copy_prebuilt_dlls(cb_cmdrun)

# codebreaker: THE program (tools/build_codebreaker_exe.cmd installs it as build/codebreaker.exe, the path every tool
# uses). main is cb::app's command table; it hosts its own workers (app/host_hook.cpp); with no command it is the Windows app (app/window_*.cpp).
if(WIN32)
  add_executable(codebreaker app/codebreaker_main.cpp app/allocator_main.cpp app/codebreaker.rc ${CB_MANIFEST})   # (the icon and version information are app/codebreaker.rc; the small-block allocator reserves address space with VirtualAlloc: Windows only, malloc elsewhere)
else()
  add_executable(codebreaker app/codebreaker_main.cpp)
endif()
cb_apply_flags(codebreaker)
target_link_libraries(codebreaker PRIVATE cb_app cb_core)
cb_copy_prebuilt_dlls(codebreaker)
add_dependencies(codebreaker decompiler pcodecheck)   # (the programs it starts, beside it)

# exe_gate: runs the real codebreaker.exe on the command cases of tests/exe_gate and compares goldens (tools\gate.cmd).
add_executable(cb_exe_gate EXCLUDE_FROM_ALL app/exe_gate_main.cpp app/gate_changed.cpp)
cb_apply_flags(cb_exe_gate)
target_link_libraries(cb_exe_gate PRIVATE cb_core)

# toolrun: the `crypto` tools and the commands the switch-over moves off the old extension (xbox-disc, xex-basic, xenon-config, switch-tables,
# corpus-executables), with the settings and the corpus the application gives them, to compare each with the Python and time it (app/toolrun_main.cpp).
add_executable(cb_toolrun EXCLUDE_FROM_ALL app/toolrun_main.cpp)
cb_apply_flags(cb_toolrun)
target_link_libraries(cb_toolrun PRIVATE cb_formats cb_recompile cb_base cb_core)
cb_copy_prebuilt_dlls(cb_toolrun)

# pipeline: the reconstruction commands (decompile, module, build, measure, teeth, sweep, match, score16, repair, place) with the surroundings the application
# will give them, to compare each with the Python on real inputs (source/pipeline_main.cpp). Not built by default.
add_executable(cb_pipeline EXCLUDE_FROM_ALL source/pipeline_main.cpp)
cb_apply_flags(cb_pipeline)
target_link_libraries(cb_pipeline PRIVATE cb_source cb_engine cb_core)
cb_copy_prebuilt_dlls(cb_pipeline)
# regress: the milestone regression (all-15 Setup boot, the Setup chain, the slow cases) as a program of its own, the proof harness of verify/regress.hpp
# (the application registers the same command). Not built by default.
add_executable(cb_regress EXCLUDE_FROM_ALL verify/regress_main.cpp)
cb_apply_flags(cb_regress)
target_link_libraries(cb_regress PRIVATE cb_verify cb_engine cb_core)
cb_copy_prebuilt_dlls(cb_regress)
# analyze_run: `analyze` of store/analyze_command on a command line, to compare its report with the Python's (store/analyze_main.cpp). Not built by default.
add_executable(cb_analyze_run EXCLUDE_FROM_ALL store/analyze_main.cpp)
cb_apply_flags(cb_analyze_run)
target_link_libraries(cb_analyze_run PRIVATE cb_store cb_engine cb_analyze cb_core)
cb_copy_prebuilt_dlls(cb_analyze_run)
# cb_cmds: the proof harness of the program-reading and recompiling commands (symbols/cmds_main.cpp): classes, xdk-match, recompile,
# recompile-project, xenon-recompile, java and xna with the Environment the command table will give them. Not built by default.
add_executable(cb_cmds EXCLUDE_FROM_ALL symbols/cmds_main.cpp)
cb_apply_flags(cb_cmds)
target_link_libraries(cb_cmds PRIVATE cb_symbols cb_recompile cb_java cb_xna cb_core)
cb_copy_prebuilt_dlls(cb_cmds)

# cb_state_cmds: the proof harness of the commands that read and write the state folder (names, knowledge, coverage, progress, facts, origin,
# driver-services) with the Environment the command table will give them (app/state_cmds_main.cpp). Not built by default.
add_executable(cb_state_cmds EXCLUDE_FROM_ALL app/state_cmds_main.cpp)
cb_apply_flags(cb_state_cmds)
target_link_libraries(cb_state_cmds PRIVATE cb_ui cb_symbols cb_knowledge cb_engine cb_analyze cb_coverage cb_store cb_verify cb_core)
cb_copy_prebuilt_dlls(cb_state_cmds)

enable_testing()
add_test(NAME cb_tests COMMAND cb_tests)

# cb_tests_<area>: one area's tests alone, built on request (tools\build_cb.cmd area <name>). A change to one area needs
# only that area's library, what it links, and its own tests -- not every area's tests (40% of a full build) nor the
# tests of other areas. The merge check still builds and runs the whole cb_tests.
get_property(CB_AREAS_FOR_AREA_TESTS GLOBAL PROPERTY CB_AREAS)
foreach(lib IN LISTS CB_AREAS_FOR_AREA_TESTS)
  string(REGEX REPLACE "^cb_" "" area "${lib}")
  get_property(area_tests GLOBAL PROPERTY CB_AREA_TESTS_${area})
  if(NOT area_tests)
    continue()
  endif()
  add_executable(cb_tests_${area} EXCLUDE_FROM_ALL core/testing_main.cpp ${area_tests} ${CB_MANIFEST})
  cb_apply_flags(cb_tests_${area})
  target_compile_definitions(cb_tests_${area} PRIVATE CB_SOURCE_DIR="${CB_NATIVE_ROOT}")
  target_link_libraries(cb_tests_${area} PRIVATE ${lib} cb_core)
  cb_copy_prebuilt_dlls(cb_tests_${area})
  if(area STREQUAL "jobs")
    add_dependencies(cb_tests_${area} cb_worker_probe)
  elseif(area STREQUAL "engine")
    add_dependencies(cb_tests_${area} cb_lane_worker)
  elseif(area STREQUAL "analyze")
    add_dependencies(cb_tests_${area} cb_observe_worker)
  elseif(area STREQUAL "verify")
    add_dependencies(cb_tests_${area} cb_fake_qemu)
  elseif(area STREQUAL "decompile")
    add_dependencies(cb_tests_${area} cb_fake_decompiler)
  endif()
endforeach()

# Windows libraries, linked by name so that every compiler links them (MSVC's #pragma comment(lib) is MSVC's alone).
target_link_libraries(cb_port PUBLIC ${CB_PROCESS_LIBS})
target_link_libraries(cb_ui PUBLIC ${CB_SOCKET_LIBS})

cb_replace_running(codebreaker)

# The .NET port's work in progress (dotnet/**/*.cxx, tests *_test.cxx): built only where -DCB_DOTNET_WIP=ON (work/ilspy-port's private
# build), so a piece not yet finished never breaks another build. A finished piece is renamed .cpp and joins cb_area(dotnet) above.
option(CB_DOTNET_WIP "build the .NET port's work in progress (dotnet/**/*.cxx)" OFF)
if(CB_DOTNET_WIP)
  file(GLOB_RECURSE cb_dotnet_wip CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/dotnet/*.cxx")
  set(cb_dotnet_wip_tests ${cb_dotnet_wip})
  list(FILTER cb_dotnet_wip EXCLUDE REGEX "_test\.cxx$")
  list(FILTER cb_dotnet_wip_tests INCLUDE REGEX "_test\.cxx$")
  # each its own object (not in a unity batch), so one can be compile-checked alone (work/ilspy-port/compile_obj.cmd)
  set_source_files_properties(${cb_dotnet_wip} ${cb_dotnet_wip_tests} PROPERTIES SKIP_UNITY_BUILD_INCLUSION ON)
  target_sources(cb_dotnet PRIVATE ${cb_dotnet_wip})
  if(TARGET cb_tests_dotnet)
    target_sources(cb_tests_dotnet PRIVATE ${cb_dotnet_wip_tests})
  endif()
endif()
