# rex_kernel - the vendored RexGlue (Xenia-lineage) kernel + boot core, built as a
# static library. PUBLIC settings (c++23, header-only fmt, include roots) propagate
# to whatever links it - so the recompiled title compiles against the same
# PPCContext and can boot through rex::Runtime.
#
# Provenance: Xenia (Copyright Ben Vanik, BSD-3) adapted for ReXGlue by Tom Clay.
# See LICENSE. Excludes the GPU-seam (we use our own graphics/xe_gpu) and the
# DRM/debug/video stubs. XEX loader + LZX are INCLUDED here (boot loads the raw
# .xex at runtime).
cmake_minimum_required(VERSION 3.20)
project(rex_kernel CXX C)

file(GLOB_RECURSE REX_KERNEL_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(FILTER REX_KERNEL_SOURCES EXCLUDE REGEX "graphics/(command_processor|graphics_system|trace_writer)")
list(FILTER REX_KERNEL_SOURCES EXCLUDE REGEX "xboxkrnl/(cert_monitor|debug_monitor|xboxkrnl_video)")
# the base library (src/core): one platform's files, the compiler's math, no performance counters
if(WIN32)
    list(FILTER REX_KERNEL_SOURCES EXCLUDE REGEX "core/[a-z_0-9]*_posix[.]cpp$")
else()
    list(FILTER REX_KERNEL_SOURCES EXCLUDE REGEX "core/([a-z_0-9]*_win|fiber_win32)[.]cpp$")
endif()
if(WIN32)       # (math_msvc.cpp is the Windows one, whichever compiler: it uses the Win32 intrinsics)
    list(FILTER REX_KERNEL_SOURCES EXCLUDE REGEX "core/math_gcc[.]cpp$")
else()
    list(FILTER REX_KERNEL_SOURCES EXCLUDE REGEX "core/math_msvc[.]cpp$")
endif()
list(FILTER REX_KERNEL_SOURCES EXCLUDE REGEX "core/perf/")
# the exports that need the SDK's audio and input subsystems (not built here): their imports
# stay host stubs
list(FILTER REX_KERNEL_SOURCES EXCLUDE REGEX "xboxkrnl/xboxkrnl_audio|xam/xam_input")

# thirdparty C sources the boot/XEX path needs: libmspack LZX decompressor,
# o1heap (CRT heap), and the crypto (AES + SHA) the XEX loader uses.
set(REX_TP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/mspack/lzxd.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/mspack/system.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/o1heap/o1heap.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/crypto/rijndael-alg-fst.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/aes_128/aes.c")       # (the kernel's crypt exports)

add_library(rex_kernel STATIC ${REX_KERNEL_SOURCES} ${REX_TP_SOURCES})
target_compile_features(rex_kernel PUBLIC cxx_std_23)
target_compile_definitions(rex_kernel PUBLIC FMT_HEADER_ONLY)
target_include_directories(rex_kernel PUBLIC
     "${CMAKE_CURRENT_SOURCE_DIR}/shim"      # shim MUST precede include: overrides
     "${CMAKE_CURRENT_SOURCE_DIR}/include"   # rex/logging, rex/ui etc. (no spdlog/imgui)
     "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fmt/include"
     "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/simde"
     "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty"
     "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/mspack"
     "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/o1heap"
     "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/crypto"
     "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/utfcpp/source"
     "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/disruptorplus/include"
     "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tomlplusplus/include"
     "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cli11/include"
     "${CMAKE_CURRENT_SOURCE_DIR}")             # (for "thirdparty/picosha2/picosha2.h")
# GCC and Clang: the SIMD the base library uses (SSSE3 shuffles, SSE4.1)
if(NOT MSVC)
    target_compile_options(rex_kernel PRIVATE -mssse3 -msse4.1 -fno-char8_t)   # (u8"" is char, as with MSVC)
endif()
# Windows: the crypt exports' random numbers (bcrypt), the base library's sockets, timers and
# shell paths
if(WIN32)
    target_link_libraries(rex_kernel PUBLIC bcrypt ws2_32 winmm shlwapi shell32 ole32)
endif()
