# cb/decompile/engine: CodeBreaker's decompiler engine -- decompiler.cc and pcodecheck.cc (ours) around the decompiler core
# in core/ (C++, Apache-2.0 -- see core/LICENSE and core/NOTICE; CodeBreaker's fork, docs/GHIDRA_FORK.md), and the
# language files in languages/. Part of src's one build (included from cb/CMakeLists.txt):
#
#   cb_decompiler      the core, a static library
#   decompiler.exe     a module's description in, each function's C out (cb::decompile starts it)
#   pcodecheck.exe     one instruction's p-code run from a given state (cb::emulate::pcode starts it)
#   sleigh.exe         the SLEIGH compiler: languages/src/*.slaspec -> languages/*.sla (built on request)
#
# The programs are built beside codebreaker.exe and cb_tests.exe (the build's bin folder), which is where cb looks for
# them. Compiled as it is, under its own flags (not /W4 /WX).
set(DECOMPILER_DIR "${CMAKE_CURRENT_LIST_DIR}")
file(GLOB CB_DECOMPILER_ALL CONFIGURE_DEPENDS "${DECOMPILER_DIR}/core/cpp/*.cc")
# Not part of the library: the Ghidra-GUI pipe protocol, the SLEIGH compiler, test and console mains, and the loaders that
# need GNU BFD.
set(CB_DECOMPILER_SKIP
    ghidra_arch inject_ghidra ghidra_translate loadimage_ghidra typegrp_ghidra database_ghidra
    ghidra_context cpool_ghidra ghidra_process comment_ghidra string_ghidra signature_ghidra
    slgh_compile slghparse slghscan
    consolemain sleighexample test
    bfd_arch loadimage_bfd analyzesigs codedata)
set(CB_DECOMPILER_SOURCES "")
foreach(f ${CB_DECOMPILER_ALL})
  get_filename_component(n ${f} NAME_WE)
  if(NOT n IN_LIST CB_DECOMPILER_SKIP)
    list(APPEND CB_DECOMPILER_SOURCES ${f})
  endif()
endforeach()

list(APPEND CB_DECOMPILER_SOURCES "${DECOMPILER_DIR}/sleigh_reader.cc")   # (ours: the SLEIGH translator as CodeBreaker's one instruction reader)
list(APPEND CB_DECOMPILER_SOURCES "${DECOMPILER_DIR}/sleigh_assembler.cc")   # (ours: the inverse of the reader, CodeBreaker's one assembler engine)
add_library(cb_decompiler STATIC ${CB_DECOMPILER_SOURCES})
set_target_properties(cb_decompiler PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(cb_decompiler PUBLIC "${CB_NATIVE_ROOT}")   # (its headers by their path under src/: decompile/engine/core/cpp/...)
target_compile_definitions(cb_decompiler PUBLIC NO_GZIP _CRT_SECURE_NO_WARNINGS)
if(WIN32)
  target_compile_definitions(cb_decompiler PUBLIC _WINDOWS WINDOWS)
endif()
target_link_libraries(cb_decompiler PUBLIC cb_zlib)   # (the zlib of cb/third_party/zlib)
if(MSVC)
  target_compile_options(cb_decompiler PRIVATE /w /EHsc /bigobj)
else()
  target_compile_options(cb_decompiler PRIVATE ${CB_FLAGS_QUIET})
endif()

# The programs. The core registers its printers and architectures through static objects, so the whole library is linked
# into each (a linker that pulls only referenced members would drop them).
add_executable(decompiler "${DECOMPILER_DIR}/decompiler.cc")
add_executable(pcodecheck "${DECOMPILER_DIR}/pcodecheck.cc")
add_executable(sleigh EXCLUDE_FROM_ALL "${DECOMPILER_DIR}/core/cpp/slgh_compile.cc" "${DECOMPILER_DIR}/core/cpp/slghparse.cc"
               "${DECOMPILER_DIR}/core/cpp/slghscan.cc")
foreach(t decompiler pcodecheck sleigh)
  set_target_properties(${t} PROPERTIES CXX_STANDARD 17 MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  target_include_directories(${t} PRIVATE "${CB_NATIVE_ROOT}")   # (elsewhere the library is linked by flags, which carry no includes)
  if(MSVC)
    target_compile_options(${t} PRIVATE /EHsc /bigobj /w)
  else()
    target_compile_options(${t} PRIVATE ${CB_FLAGS_QUIET})
  endif()
endforeach()
target_link_libraries(sleigh PRIVATE cb_decompiler)
# The assembler's round-trip metric on real code (not a test of the suite): sleigh_asmcheck <file.sla> <binary> <offset> <base> <count> [ctx=val ...]
add_executable(sleigh_asmcheck EXCLUDE_FROM_ALL "${DECOMPILER_DIR}/sleigh_asmcheck.cc")
set_target_properties(sleigh_asmcheck PROPERTIES CXX_STANDARD 17 MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_include_directories(sleigh_asmcheck PRIVATE "${CB_NATIVE_ROOT}")
if(MSVC)
  target_compile_options(sleigh_asmcheck PRIVATE /EHsc /bigobj /w)
else()
  target_compile_options(sleigh_asmcheck PRIVATE ${CB_FLAGS_QUIET})
endif()
target_link_libraries(sleigh_asmcheck PRIVATE cb_decompiler)
foreach(t decompiler pcodecheck)
  if(MSVC)
    target_link_libraries(${t} PRIVATE cb_decompiler)
    target_link_options(${t} PRIVATE "/WHOLEARCHIVE:$<TARGET_FILE:cb_decompiler>")   # (a path: lld-link takes no bare name)
  else()
    target_link_libraries(${t} PRIVATE -Wl,--whole-archive cb_decompiler -Wl,--no-whole-archive cb_zlib)
  endif()
endforeach()
