# CodeBreaker's decompiler: decompiler.cc (ours) around the Ghidra decompiler core in core/
# (C++, Apache-2.0 -- see core/LICENSE and core/NOTICE), linked in as a library. No Java, no
# Ghidra application.
#
#   cmake -S native/decompiler -B native/decompiler/build -G "Visual Studio 18 2026"
#   cmake --build native/decompiler/build --config Release --target decompiler --parallel 6
cmake_minimum_required(VERSION 3.16)
project(codebreaker_decompiler CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB CORE_ALL ${CMAKE_CURRENT_SOURCE_DIR}/core/cpp/*.cc)
# Not part of the library: the Ghidra-GUI pipe protocol, the SLEIGH compiler, test and console
# mains, and the loaders that need GNU BFD.
set(CORE_SKIP
    ghidra_arch inject_ghidra ghidra_translate loadimage_ghidra typegrp_ghidra database_ghidra
    ghidra_context cpool_ghidra ghidra_process comment_ghidra string_ghidra signature_ghidra
    slgh_compile slghparse slghscan
    consolemain sleighexample test
    bfd_arch loadimage_bfd analyzesigs codedata)
set(CORE_SRC "")
foreach(f ${CORE_ALL})
  get_filename_component(n ${f} NAME_WE)
  list(FIND CORE_SKIP ${n} idx)
  if(idx EQUAL -1)
    list(APPEND CORE_SRC ${f})
  endif()
endforeach()
file(GLOB ZLIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/core/zlib/*.c)

add_library(core STATIC ${CORE_SRC} ${ZLIB_SRC})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core/cpp)
target_compile_definitions(core PUBLIC LOCAL_ZLIB NO_GZIP _WINDOWS WINDOWS _CRT_SECURE_NO_WARNINGS)
if(MSVC)
  target_compile_options(core PRIVATE /w /EHsc /bigobj)
endif()

add_executable(decompiler decompiler.cc)
target_link_libraries(decompiler PRIVATE core)
if(MSVC)
  # The core registers its printers and architectures through static objects; a linker that
  # only pulls referenced members out of a library would drop them.
  target_link_options(decompiler PRIVATE /WHOLEARCHIVE:core)
  target_compile_options(decompiler PRIVATE /EHsc /bigobj)
endif()
set_target_properties(decompiler PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
