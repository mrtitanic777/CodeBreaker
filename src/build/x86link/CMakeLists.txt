# cb_wlink: CodeBreaker's linker for OpenWatcom's object files, a library of codebreaker.exe (docs/X86LINK_DESIGN.md).
# It began as OpenWatcom's wlink, built from its own source (toolchains/openwatcom, branch `codebreaker`: the objects
# wlinkd.dll is linked from) and proven byte-identical to wlink.exe on two corpora; it is now typed C++ in the module
# folders of the design (phase 5), each module the job of the wlink files it came from:
#   link/        the state of one link (Link, the run's state, the flags, formats' data, the symbol and object types)
#   base/        memory, carvers, virtual memory, hash tables, string tables, rings, the file layer
#   messages/    the messages and their table (compiled in: Msg_Get reads messages/msgtable.cpp)
#   directives/  the directive language
#   objects/     reading OMF and ORL (COFF/ELF) objects and libraries
#   symbols/     the symbol table, the two passes, library search, communal data, imports and exports
#   layout/      segment and group placement, overlays, address allocation
#   fixups/      fixups, relocations, the TOC
#   debuginfo/   Watcom, CodeView, HLL and DWARF debugging information
#   formats/     the output file and each executable format (DOS, DOS/16M, OS/2 NE/LX, LE/VxD, PE, Novell, Phar Lap,
#                QNX, ELF, raw, RDOS)
#   map/         `option map`
#   driver/      the passes and the IDE entry the program calls (build/x86link/x86link.cpp)
#   support/     OpenWatcom's libraries it uses (orl, wres, three of dwarf/dw, the rc resource merger, watcom/c, the
#                demangler) and two headers OpenWatcom's build generates
# The linker's own headers are included by their path under src/ ("build/x86link/<module>/<file>"); the support
# libraries' by their folders. The defines are OpenWatcom's build's for an NT x64 host (build/mif/local.mif, cproj.mif,
# wl/master.mif).
set(X "${CMAKE_CURRENT_LIST_DIR}")
set(S "${X}/support")

set(CB_WLINK_SOURCES)
foreach(m link base messages directives objects symbols layout fixups debuginfo formats map driver)
  file(GLOB ms CONFIGURE_DEPENDS "${X}/${m}/*.cpp")
  list(APPEND CB_WLINK_SOURCES ${ms})
endforeach()
list(FILTER CB_WLINK_SOURCES EXCLUDE REGEX "(_test|_main)\\.cpp$")
list(REMOVE_ITEM CB_WLINK_SOURCES "${X}/link/run_state.cpp")   # (in cb_wlink_support: the libraries keep their state in it too)
# OpenWatcom builds these as libraries of their own (orl, wres, three of dwarf/dw, lib_misc): no INSIDE_WLINK
file(GLOB CB_WLINK_LIBS CONFIGURE_DEPENDS
  "${S}/lib_misc/c/*.cpp" "${S}/dwarf/dw/c/*.cpp" "${S}/orl/c/*.cpp" "${S}/orl/omf/c/*.cpp" "${S}/orl/elf/c/*.cpp" "${S}/orl/coff/c/*.cpp"
  "${S}/wres/c/*.cpp")
# the linker's own build compiles these with it (INSIDE_WLINK)
file(GLOB CB_WLINK_WITH CONFIGURE_DEPENDS "${S}/watcom/c/*.cpp" "${S}/rc/rc/c/*.cpp")

set(CB_WLINK_INCLUDES
  "${S}/generated" "${S}/dwarf/dw/h" "${S}/orl/h" "${S}/orl/omf/h" "${S}/orl/elf/h" "${S}/orl/coff/h"
  "${S}/rc/rc/h" "${S}/wres/h" "${S}/lib_misc/h" "${S}/watcom/c" "${S}/trmem" "${S}/watcom/h"
  "${X}/../..")   # (src/: the linker's headers by their path under it)
set(CB_WLINK_DEFINES
  _BLDVER=1300 _CYEAR=2026 _BETAVER NDEBUG __FLAT__ _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS __NT__ WIN32_LEAN_AND_MEAN
  _WIN32_WINNT=0x0601 $<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,__NT_X64__,__NT_386__>)

add_library(cb_wlink_support STATIC ${CB_WLINK_LIBS} "${X}/link/run_state.cpp")   # (the state of a run: every part keeps its own in it)
add_library(cb_wlink STATIC ${CB_WLINK_SOURCES} ${CB_WLINK_WITH})
target_link_libraries(cb_wlink PUBLIC cb_wlink_support)
foreach(t cb_wlink_support cb_wlink)
  set_target_properties(${t} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  target_include_directories(${t} PRIVATE ${CB_WLINK_INCLUDES})
  target_compile_definitions(${t} PRIVATE ${CB_WLINK_DEFINES})
  if(MSVC)
    target_compile_options(${t} PRIVATE /W0 /utf-8 /permissive- /EHsc "/FI${X}/link/cbcxx.h")
  else()
    target_compile_options(${t} PRIVATE -w -include "${X}/link/cbcxx.h")
  endif()
endforeach()
target_compile_definitions(cb_wlink PRIVATE INSIDE_WLINK CHAIN_CALLBACK CB_WLINK_MSG_TABLE)
