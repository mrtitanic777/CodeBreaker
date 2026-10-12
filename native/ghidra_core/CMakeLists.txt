# The Ghidra decompiler core (C++, Apache-2.0 -- see LICENSE / NOTICE), built as a library inside
# CodeBreaker with our own front end. No Java, no Ghidra application: the same engine the Ghidra
# GUI drives over a pipe, linked directly and driven by cbdecomp.cc.
cmake_minimum_required(VERSION 3.16)
project(cb_ghidra_core CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB GH_ALL ${CMAKE_CURRENT_SOURCE_DIR}/cpp/*.cc)
# Not part of the library: the Ghidra-GUI pipe protocol, the SLEIGH compiler, test and console
# mains, and the loaders that need GNU BFD.
set(GH_SKIP
    ghidra_arch inject_ghidra ghidra_translate loadimage_ghidra typegrp_ghidra database_ghidra
    ghidra_context cpool_ghidra ghidra_process comment_ghidra string_ghidra signature_ghidra
    slgh_compile slghparse slghscan
    consolemain sleighexample test
    bfd_arch loadimage_bfd analyzesigs codedata)
set(GH_SRC "")
foreach(f ${GH_ALL})
  get_filename_component(n ${f} NAME_WE)
  list(FIND GH_SKIP ${n} idx)
  if(idx EQUAL -1)
    list(APPEND GH_SRC ${f})
  endif()
endforeach()
file(GLOB ZLIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/zlib/*.c)

add_library(ghidra_decomp STATIC ${GH_SRC} ${ZLIB_SRC})
target_include_directories(ghidra_decomp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cpp)
target_compile_definitions(ghidra_decomp PUBLIC LOCAL_ZLIB NO_GZIP _WINDOWS WINDOWS _CRT_SECURE_NO_WARNINGS)
if(MSVC)
  target_compile_options(ghidra_decomp PRIVATE /w /EHsc /bigobj)
endif()

set(W95 ${CMAKE_CURRENT_SOURCE_DIR}/../decompiler/src/win95)
add_executable(cbdecomp cbdecomp.cc ${W95}/pe_loader.cc)
target_include_directories(cbdecomp PRIVATE ${W95} ${W95}/..)
target_link_libraries(cbdecomp PRIVATE ghidra_decomp)
# The core registers its printers and architectures through static objects; a linker that only
# pulls referenced members out of a library would drop them.
if(MSVC)
  target_link_options(cbdecomp PRIVATE /WHOLEARCHIVE:ghidra_decomp)
endif()
if(MSVC)
  target_compile_options(cbdecomp PRIVATE /EHsc /bigobj)
endif()
set_target_properties(cbdecomp PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
