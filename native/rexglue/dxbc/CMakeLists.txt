# rex_dxbc - the vendored ReXGlue/Xenia DXBC shader translator, built as a static
# library with its own (c++23 + header-only fmt) settings so the host project can
# stay on its own C++ standard and only sees the clean rex_dxbc_translate.h API.
#
# Provenance: Xenia (Copyright Ben Vanik, BSD-3), adapted for ReXGlue by Tom Clay.
# See LICENSE in this directory. Only rex_dxbc_translate.* + the shims are ours.
cmake_minimum_required(VERSION 3.20)
project(rex_dxbc CXX)

set(REX_DXBC_SOURCES
    src/graphics/pipeline/shader/dxbc.cpp
    src/graphics/pipeline/shader/dxbc_translator.cpp
    src/graphics/pipeline/shader/dxbc_translator_alu.cpp
    src/graphics/pipeline/shader/dxbc_translator_fetch.cpp
    src/graphics/pipeline/shader/dxbc_translator_memexport.cpp
    src/graphics/pipeline/shader/dxbc_translator_om.cpp
    src/graphics/pipeline/shader/translator.cpp
    src/graphics/pipeline/shader/translator_disasm.cpp
    src/graphics/pipeline/shader/shader.cpp
    src/graphics/xenos.cpp
    src/graphics/register_file.cpp
    src/graphics/registers.cpp
    src/graphics/flags.cpp
    src/graphics/format/ucode.cpp
    src/ui/graphics_util.cpp
    src/core/string_buffer.cpp
    thirdparty/dxbc/DXBCChecksum.cpp
    shim/ui_shim.cpp
    shim/mem_swap_shim.cpp
    shim/link_shims.cpp
    rex_dxbc_translate.cpp
)

add_library(rex_dxbc STATIC ${REX_DXBC_SOURCES})
target_compile_features(rex_dxbc PRIVATE cxx_std_23)
target_compile_definitions(rex_dxbc PRIVATE FMT_HEADER_ONLY)
# CodeBreaker: beside the kernel core (added first), its base library provides what the link
# shims stand in for
if(TARGET rex_kernel)
    target_compile_definitions(rex_dxbc PRIVATE REX_DXBC_WITH_KERNEL)
endif()

# PUBLIC: the clean, dependency-free header (so the host's xe_translators.cpp can
# #include "rex_dxbc_translate.h" and light up the __has_include seam).
target_include_directories(rex_dxbc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# PRIVATE: the vendored SDK headers + shims + header-only fmt (never leak out).
target_include_directories(rex_dxbc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/shim
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/fmt/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)
