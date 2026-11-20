# cb_wlink: OpenWatcom's linker, wlink, built from its own source (C, now compiled as C++: cb/cbcxx.h) as a library of codebreaker.exe (docs/X86LINK_DESIGN.md,
# route A: the source first, byte-identical by construction, then refactored into CodeBreaker's C++ over time).
#
# The files are OpenWatcom's (toolchains/openwatcom, branch `codebreaker`; work/x86link/vendor.py copied them, each at its
# path under bld/), the objects wlinkd.dll is linked from (bld/wl/ntx64.dll/wlinkd.map), now C++ (docs/X86LINK_DESIGN.md phase 4a): the linker (wl/c), and of its
# support libraries only what it uses (orl, wres, three of dwarf/dw, the rc resource merger, watcom/c helpers). Two headers
# OpenWatcom's build generates are kept made (generated/). Messages are compiled in (cb/msgtable.c, CB_WLINK_MSG_TABLE)
# instead of read from the program's string table. The program calls it through the IDE entry wlinkd.dll exports
# (wl/c/ideentry.c: IDEInitDLL, IDEPassInitInfo, IDERunYourSelf, IDEFiniDLL): build/x86link/x86link.cpp.
# The defines and include folders are OpenWatcom's build's for an NT x64 host (build/mif/local.mif, cproj.mif, wl/master.mif).
set(W "${CMAKE_CURRENT_LIST_DIR}")

set(CB_WLINK_SOURCES
  autogrp carve wcomdef
  dbgall dbgcv dbghll dbgdwarf dbginfo debug distrib dosstub global hash ideentry impexp libr libsupp linkutil
  load16m loaddos loadelf loadelf2 loadfile loadflat loadnov loados2 loadpe loadphar loadqnx loadraw loadrdv
  lsymtab mapio mixcache msg objio obj2supp objcalc objfree objnode objomf objorl objpass1 objpass2 objstrip omfreloc
  overlays ovlsupp permdata procfile reloc ring ring2 salloc spillio strtab symmem symtrace toc virtmem wlink wlmem wlnkmsg ntio)
list(TRANSFORM CB_WLINK_SOURCES PREPEND "${W}/wl/c/")
list(TRANSFORM CB_WLINK_SOURCES APPEND ".cpp")
# the directive language, typed C++ in build/x86link/directives (docs/X86LINK_DESIGN.md phase 4b): wlink's cmd*.c
file(GLOB CB_WLINK_DIRECTIVES CONFIGURE_DEPENDS "${W}/../directives/*.cpp")
list(APPEND CB_WLINK_SOURCES ${CB_WLINK_DIRECTIVES})
# OpenWatcom builds these as libraries of their own (orl, wres, three of dwarf/dw, lib_misc): no INSIDE_WLINK
file(GLOB CB_WLINK_LIBS CONFIGURE_DEPENDS
  "${W}/lib_misc/c/*.cpp" "${W}/dwarf/dw/c/*.cpp" "${W}/orl/c/*.cpp" "${W}/orl/omf/c/*.cpp" "${W}/orl/elf/c/*.cpp" "${W}/orl/coff/c/*.cpp"
  "${W}/wres/c/*.cpp")
# the linker's own build compiles these with it (INSIDE_WLINK)
file(GLOB CB_WLINK_WITH CONFIGURE_DEPENDS "${W}/watcom/c/*.cpp" "${W}/rc/rc/c/*.cpp")

set(CB_WLINK_INCLUDES
  "${W}/generated" "${W}/cb" "${W}/wl/h" "${W}/dwarf/dw/h" "${W}/orl/h" "${W}/orl/omf/h" "${W}/orl/elf/h" "${W}/orl/coff/h"
  "${W}/rc/rc/h" "${W}/wres/h" "${W}/lib_misc/h" "${W}/watcom/c" "${W}/trmem" "${W}/watcom/h" "${W}/wl/res"
  "${W}/../../..")   # (src/: the directive layer's headers by their path under it)
set(CB_WLINK_DEFINES
  _BLDVER=1300 _CYEAR=2026 _BETAVER NDEBUG __FLAT__ _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS __NT__ WIN32_LEAN_AND_MEAN
  _WIN32_WINNT=0x0601 $<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,__NT_X64__,__NT_386__>)

add_library(cb_wlink_support STATIC ${CB_WLINK_LIBS} "${W}/cb/run_state.cpp")   # (the state of a run: every part of wlink keeps its own in it)
add_library(cb_wlink STATIC ${CB_WLINK_SOURCES} ${CB_WLINK_WITH} "${W}/cb/msgtable.cpp")
target_link_libraries(cb_wlink PUBLIC cb_wlink_support)
foreach(t cb_wlink_support cb_wlink)
  set_target_properties(${t} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  # the include path in OpenWatcom's order (wl/h before rc/rc/h: wl's rcrtns.h is the one the linker's build uses)
  target_include_directories(${t} PRIVATE ${CB_WLINK_INCLUDES})
  target_compile_definitions(${t} PRIVATE ${CB_WLINK_DEFINES})
  if(MSVC)
    target_compile_options(${t} PRIVATE /W0 /utf-8 /permissive- /EHsc "/FI${W}/cb/cbcxx.h")
  else()
    target_compile_options(${t} PRIVATE -w -include "${W}/cb/cbcxx.h")
  endif()
endforeach()
target_compile_definitions(cb_wlink PRIVATE INSIDE_WLINK CHAIN_CALLBACK CB_WLINK_MSG_TABLE)
