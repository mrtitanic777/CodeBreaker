# cb/exact/emu/dosexec: the DOS executive -- a DOS program loaded and run on Unicorn (INT 20h/21h, EXEC, the memory arena,
# a DPMI host) -- built as the library cb_dosexec, part of src's one build (included from cb/CMakeLists.txt).
# cb::exact::emu::dos::Executive calls its C interface (cbdos.h) in-process. It calls Win32 directly (the Unicorn it is
# handed is loaded with LoadLibraryW), so it is built on Windows only; elsewhere the executive says it is not there.
# Compiled as it is, under its own flags (not /W4 /WX).
if(WIN32)
  set(DOSEXEC_DIR "${CMAKE_CURRENT_LIST_DIR}")
  add_library(cb_dosexec STATIC
    "${DOSEXEC_DIR}/api.cc"      # the C interface: the file system handed over, a run, its result
    "${DOSEXEC_DIR}/uc.cc"       # Unicorn's entry points, from the unicorn.dll it is handed
    "${DOSEXEC_DIR}/pyfmt.cc"    # the text a run reports (bytes repr, %x, str.upper ...)
    "${DOSEXEC_DIR}/fs.cc"       # paths, the MemFS, disk files
    "${DOSEXEC_DIR}/mcb.cc"      # the DOS memory arena in guest RAM
    "${DOSEXEC_DIR}/exec.cc"     # the executive: load, INT 20h/21h, EXEC, the run loop
    "${DOSEXEC_DIR}/dpmi.cc")    # the DPMI host
  set_target_properties(cb_dosexec PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  target_include_directories(cb_dosexec PRIVATE "${CB_NATIVE_ROOT}" "${CB_THIRD_PARTY}/include")   # (its own headers by their path under src/)
  target_compile_definitions(cb_dosexec PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX UNICODE _UNICODE)
  if(MSVC)
    target_compile_options(cb_dosexec PRIVATE /EHsc /W3 /utf-8)
  else()
    target_compile_options(cb_dosexec PRIVATE ${CB_FLAGS_LIB})
  endif()
  if(CB_ASAN)
    target_compile_options(cb_dosexec PRIVATE /fsanitize=address /Zi)
  endif()
endif()
