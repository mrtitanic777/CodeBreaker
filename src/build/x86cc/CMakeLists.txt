# cb_wcc_i86, cb_wcc_386: OpenWatcom's C compilers, wcc (16-bit x86 code) and wcc386 (32-bit), built from their own source as
# libraries of codebreaker.exe (docs/BUILTIN_ENGINES_PLAN.md item 5, the route x86link took: the source first, identical by
# construction, then made CodeBreaker's over time). work/x86cc/vendor_cc.py copied the files (each at its path under OpenWatcom's
# bld/: the objects of wccd.dll / wccd386.dll, from their link maps) and each target's generated headers into generated/<t>/,
# with cbnames.h: every global of that compiler renamed wcci86_* / wcc386_*, force-included, so both compilers and wlink link
# into one program. The include folders and defines are OpenWatcom's own build's for an NT x64 host (cc/master.mif,
# cg/mif/rules.mif, cg/intel/master.mif, build/mif/local.mif): the front end's, the code generator's (BY_CG), and those of the
# libraries both use (cfloat, dwarf/dw, owl).
set(X "${CMAKE_CURRENT_LIST_DIR}")
set(CB_WCC_ROOT "${X}")
set(CB_WCC_DEFINES
  _BLDVER=1300 _CYEAR=2026 _BETAVER NDEBUG __FLAT__ _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS _STATIC_CPPLIB __NT__
  WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0601 INCL_MSGTEXT $<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,__NT_X64__,__NT_386__>)

function(cb_wcc target t prefix)
  set(X "${CB_WCC_ROOT}")
  file(STRINGS "${X}/generated/${t}/sources.txt" rel_sources)
  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${X}/generated/${t}/sources.txt")
  set(sources "")
  foreach(rel IN LISTS rel_sources)
    list(APPEND sources "${X}/${rel}")
  endforeach()
  add_library(${target} STATIC ${sources})
  set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  target_compile_definitions(${target} PRIVATE ${CB_WCC_DEFINES})
  if(MSVC)
    target_compile_options(${target} PRIVATE /W0 /utf-8 /permissive- "/FI${X}/generated/${t}/cbnames.h" "/FI${X}/comp_cfg/h/cbcxx.h")
  else()
    # (__SEH__: MinGW's x64 GCC defines it; MSVC and the C build never did, and cc/c/cgen.cpp's try-block code is under it)
    target_compile_options(${target} PRIVATE -w -U__SEH__ "SHELL:-include ${X}/generated/${t}/cbnames.h" "SHELL:-include ${X}/comp_cfg/h/cbcxx.h")
  endif()
  # the front end: cc, fe_misc, the inline assembler (wasm), watcom/c (cc/master.mif)
  set(front "${X}/generated/${t}" "${X}/cc/${t}" "${X}/cc/h" "${X}/cg/intel/${t}/h" "${X}/cg/intel/h" "${X}/cg/h" "${X}/wasm/h"
            "${X}/owl/h" "${X}/dwarf/dw/h" "${X}/comp_cfg/h" "${X}/fe_misc/h" "${X}/watcom/h" "${X}/watcom/c")
  # the code generator (cg/mif/rules.mif, cg/intel/master.mif)
  set(back "${X}/generated/${t}" "${X}/cg/intel/${t}/h" "${X}/cg/intel/h" "${X}/owl/h" "${X}/cg/h" "${X}/cfloat/h" "${X}/dwarf/dw/h"
           "${X}/comp_cfg/h" "${X}/trmem" "${X}/watcom/h" "${X}/watcom/c")
  # the libraries both use (cfloat, dwarf/dw, owl): their own headers
  set(libs "${X}/generated/${t}" "${X}/cfloat/h" "${X}/dwarf/dw/h" "${X}/owl/h" "${X}/watcom/h" "${X}/watcom/c")
  foreach(src IN LISTS sources)
    if(src MATCHES "/cg/")
      set_property(SOURCE "${src}" TARGET_DIRECTORY ${target} PROPERTY INCLUDE_DIRECTORIES ${back})
      set_property(SOURCE "${src}" TARGET_DIRECTORY ${target} APPEND PROPERTY COMPILE_DEFINITIONS BY_CG)
    elseif(src MATCHES "/(cfloat|dwarf|owl)/")
      set_property(SOURCE "${src}" TARGET_DIRECTORY ${target} PROPERTY INCLUDE_DIRECTORIES ${libs})
    else()
      set_property(SOURCE "${src}" TARGET_DIRECTORY ${target} PROPERTY INCLUDE_DIRECTORIES ${front})
    endif()
  endforeach()
  set_property(SOURCE "${X}/cc/c/cmsg.cpp" TARGET_DIRECTORY ${target} APPEND PROPERTY COMPILE_DEFINITIONS __msg_file_prefix=${prefix})
  if(MSVC)
    set_property(SOURCE "${X}/fe_misc/c/intlload.cpp" TARGET_DIRECTORY ${target} APPEND PROPERTY COMPILE_OPTIONS "/FI${X}/cc/h/fesupp.h")
  else()
    set_property(SOURCE "${X}/fe_misc/c/intlload.cpp" TARGET_DIRECTORY ${target} APPEND PROPERTY COMPILE_OPTIONS "-include${X}/cc/h/fesupp.h")
  endif()
endfunction()

# each in a folder of its own: a source's properties (its include folders) belong to the folder its target is made in, and
# the two compilers build the same files for different targets (cg/intel/<t>/h/deftarg.h picks the cpu)
add_subdirectory(targets/i86)
add_subdirectory(targets/386)
