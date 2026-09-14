cmake_minimum_required(VERSION 3.16)
project(XEXMagic LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Single output directory per configuration, so the exe always lands somewhere
# predictable instead of build/Release vs build/Debug.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
foreach(cfg IN ITEMS DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg} ${CMAKE_BINARY_DIR}/bin)
endforeach()

# ---------------------------------------------------------------------------
# third_party
# ---------------------------------------------------------------------------

# mspack - LZX decompression (XEX2 compressed images, IW5 fastfile bodies).
add_library(mspack STATIC
    third_party/mspack/lzxd.c
    third_party/mspack/system.c
)
target_include_directories(mspack PUBLIC third_party/mspack)

# crypto - AES/Rijndael for XEX2 key decryption.
add_library(crypto STATIC
    third_party/crypto/rijndael-alg-fst.c
)
target_include_directories(crypto PUBLIC third_party/crypto)

# Vendored code is not ours to keep warning-clean.
foreach(lib mspack crypto)
    if(MSVC)
        target_compile_options(${lib} PRIVATE /W0)
    else()
        target_compile_options(${lib} PRIVATE -w)
    endif()
    set_target_properties(${lib} PROPERTIES FOLDER "third_party")
endforeach()

# ---------------------------------------------------------------------------
# xexmagic
# ---------------------------------------------------------------------------

# Grouped by role rather than listed flat, so the Solution Explorer mirrors how
# the tool is actually structured.
set(SRC_CONTAINER
    src/lzx.cc
    src/xex_loader.cc
    src/coff_lib.cc
    src/apply_patch.cc
)
# Nexia's generated PPC opcode tables, copied unmodified so they can be
# re-synced. They cover the whole ISA including the Xenon VMX128 encodings,
# which the hand-written decoder in ppc_disasm.cc does not. The compat/
# directory supplies the few Xenia headers they include.
set(SRC_PPC_TABLES
    src/ppc/ppc_opcode_info.cc
    src/ppc/ppc_opcode_table_gen.cc
    src/ppc/ppc_opcode_lookup_gen.cc
    src/ppc/ppc_opcode_disasm.cc
    src/ppc/ppc_opcode_disasm_gen.cc
)

set(SRC_ANALYSIS
    src/ppc_disasm.cc
    src/ppc_decompiler.cc
    src/il_builder.cc
    src/il_validator.cc
    src/var_allocator.cc
    src/xenon_cycles.cc
    src/decompiler_c.cc
    src/progress.cc
    src/version.cc
    src/app_paths.cc
)
set(SRC_CODEGEN
    src/codegen.cc
    src/codegen_rt360.cc
)
set(SRC_SYMBOLS
    src/msvc_demangle.cc
    src/import_names.cc
    src/import_annotate.cc
)
set(SRC_RUNTIME
    src/ppc_interp.cc
    src/kernel_exports.cc
)
set(SRC_UI
    src/web_server.cc
    src/sdk_config.cc
)
set(SRC_APP
    src/main.cc
)

file(GLOB SRC_HEADERS CONFIGURE_DEPENDS src/*.h)

add_executable(xexmagic
    ${SRC_CONTAINER}
    ${SRC_PPC_TABLES}
    ${SRC_ANALYSIS}
    ${SRC_CODEGEN}
    ${SRC_SYMBOLS}
    ${SRC_RUNTIME}
    ${SRC_UI}
    ${SRC_APP}
    ${SRC_HEADERS}
)

source_group("Container"  FILES ${SRC_CONTAINER})
source_group("PPC Tables" FILES ${SRC_PPC_TABLES})
source_group("Analysis"   FILES ${SRC_ANALYSIS})
source_group("Codegen"    FILES ${SRC_CODEGEN})
source_group("Symbols"    FILES ${SRC_SYMBOLS})
source_group("Interpreter" FILES ${SRC_RUNTIME})
source_group("Web UI"     FILES ${SRC_UI})
source_group("App"        FILES ${SRC_APP})
source_group("Headers"    FILES ${SRC_HEADERS})

target_include_directories(xexmagic PRIVATE src third_party third_party/capstone src/ppc/compat)
target_link_libraries(xexmagic PRIVATE mspack crypto)

if(WIN32)
    # dbghelp: UnDecorateSymbolName for MSVC-mangled symbols in .lib imports.
    target_link_libraries(xexmagic PRIVATE dbghelp)
endif()

if(MSVC)
    target_compile_options(xexmagic PRIVATE /W3)
    target_compile_definitions(xexmagic PRIVATE _CRT_SECURE_NO_WARNINGS)
    # web_server.cc embeds the UI in large string literals; without /bigobj the
    # object file exceeds the COFF section limit.
    target_compile_options(xexmagic PRIVATE /bigobj)
else()
    target_compile_options(xexmagic PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

# ---------------------------------------------------------------------------
# tests
# ---------------------------------------------------------------------------

add_executable(test_il
    test/test_il.cc
    ${SRC_CONTAINER}
    ${SRC_ANALYSIS}
    ${SRC_CODEGEN}
    ${SRC_SYMBOLS}
    ${SRC_PPC_TABLES}
)
target_include_directories(test_il PRIVATE src third_party third_party/capstone src/ppc/compat)
target_link_libraries(test_il PRIVATE mspack crypto)
if(WIN32)
    target_link_libraries(test_il PRIVATE dbghelp)
endif()
if(MSVC)
    target_compile_definitions(test_il PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(test_il PRIVATE /bigobj)
endif()
set_target_properties(test_il PROPERTIES FOLDER "tests")

enable_testing()
add_test(NAME il COMMAND test_il)

# Open the solution on the tool, not on ALL_BUILD.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
             PROPERTY VS_STARTUP_PROJECT xexmagic)

# Data the tool reads at runtime; kept beside the binary so it works from the
# build directory without extra arguments.
add_custom_command(TARGET xexmagic POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/extra_seeds.txt
            $<TARGET_FILE_DIR:xexmagic>/extra_seeds.txt
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/src/rt360_runtime.h
            $<TARGET_FILE_DIR:xexmagic>/rt360_runtime.h
    COMMENT "Staging runtime data next to xexmagic"
)

# ---------------------------------------------------------------------------
# win95dec -- the Win95 x86 disassembler+decompiler CLI (the x86 port).
# NE loader + x86 decode + x86->IL lift -> reused DecompileToC back-end.
# ---------------------------------------------------------------------------
# x86-only: WIN95DEC_NO_PPC compiles out decompiler_c's PPC-opcode annotation
# path, so the 5 PPC opcode-table .cc files are no longer needed here.
add_executable(win95dec
    src/win95/ne_loader.cc
    src/win95/x86_decode.cc
    src/win95/x86_lift.cc
    src/win95/capstone_decode.cc
    src/win95/test_decomp.cc
    src/decompiler_c.cc
    src/var_allocator.cc
)
target_include_directories(win95dec PRIVATE src third_party third_party/capstone src/ppc/compat)
target_compile_definitions(win95dec PRIVATE WIN95DEC_NO_PPC)
target_link_libraries(win95dec PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/capstone/capstone.lib)
if(MSVC)
    target_compile_definitions(win95dec PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(win95dec PRIVATE /bigobj)
endif()

# ---------------------------------------------------------------------------
# win95pedec -- the same engine over 32-bit PE (kernel32/gdi32/... the 638 PE
# class). PE loader + shared x86 decode/lift (32-bit ABI) -> DecompileToC.
# ---------------------------------------------------------------------------
add_executable(win95pedec
    src/win95/pe_loader.cc
    src/win95/x86_decode.cc
    src/win95/x86_lift.cc
    src/win95/capstone_decode.cc
    src/win95/pe_decomp.cc
    src/decompiler_c.cc
    src/var_allocator.cc
)
target_include_directories(win95pedec PRIVATE src third_party third_party/capstone src/ppc/compat)
target_compile_definitions(win95pedec PRIVATE WIN95DEC_NO_PPC)
target_link_libraries(win95pedec PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/capstone/capstone.lib)
if(MSVC)
    target_compile_definitions(win95pedec PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(win95pedec PRIVATE /bigobj)
endif()

# ---------------------------------------------------------------------------
# win95ledec -- the same engine over LE (VxD / *.386, the 223 ring-0 drivers).
# LE loader + shared x86 decode/lift (32-bit flat) -> DecompileToC.
# ---------------------------------------------------------------------------
add_executable(win95ledec
    src/win95/le_loader.cc
    src/win95/x86_decode.cc
    src/win95/x86_lift.cc
    src/win95/capstone_decode.cc
    src/win95/le_decomp.cc
    src/decompiler_c.cc
    src/var_allocator.cc
)
target_include_directories(win95ledec PRIVATE src third_party third_party/capstone src/ppc/compat)
target_compile_definitions(win95ledec PRIVATE WIN95DEC_NO_PPC)
target_link_libraries(win95ledec PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/capstone/capstone.lib)
if(MSVC)
    target_compile_definitions(win95ledec PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(win95ledec PRIVATE /bigobj)
endif()

# ---------------------------------------------------------------------------
# win95mzdec -- the same engine over plain DOS/MZ 16-bit executables (~92).
# MZ loader + shared x86 decode/lift (16-bit real mode) -> DecompileToC.
# ---------------------------------------------------------------------------
add_executable(win95mzdec
    src/win95/mz_loader.cc
    src/win95/x86_decode.cc
    src/win95/x86_lift.cc
    src/win95/capstone_decode.cc
    src/win95/mz_decomp.cc
    src/decompiler_c.cc
    src/var_allocator.cc
)
target_include_directories(win95mzdec PRIVATE src third_party third_party/capstone src/ppc/compat)
target_compile_definitions(win95mzdec PRIVATE WIN95DEC_NO_PPC)
target_link_libraries(win95mzdec PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/capstone/capstone.lib)
if(MSVC)
    target_compile_definitions(win95mzdec PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(win95mzdec PRIVATE /bigobj)
endif()

# ---------------------------------------------------------------------------
# capstone_probe -- prove the bundled Capstone 4.0.2 static lib (from
# PEExplorerV2, MIT+BSD) links + decodes 16/32-bit x86 in our build. If good,
# Capstone becomes an accurate decode backend for the lifter.
# ---------------------------------------------------------------------------
add_executable(capstone_probe src/win95/capstone_probe.cc)
target_include_directories(capstone_probe PRIVATE third_party/capstone)
target_link_libraries(capstone_probe PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/capstone/capstone.lib)
if(MSVC)
    target_compile_definitions(capstone_probe PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
