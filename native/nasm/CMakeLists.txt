# CodeBreaker's assembler: NASM (BSD-2-Clause, see LICENSE and AUTHORS) with a MASM 6.x
# compatibility mode (`--masm`, docs/MASM-COMPATIBILITY.md). Only the assembler engine and the
# output formats CodeBreaker uses are here: bin, obj (OMF) and win32/win64 (COFF).
#
#   cmake -S native/nasm -B native/nasm/build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
#   cmake --build native/nasm/build --target nasm
#
# Built with gcc (MinGW-w64 on Windows), the compiler NASM is written for; config/config.h is the
# configuration NASM's configure script works out for it, kept so no autoconf is needed.
#
# The tables NASM generates with Perl (instructions, registers, tokens, directives, warnings,
# standard macros) are kept generated in the tree, so a build needs no Perl. The standard macros
# are regenerated whenever a .mac file changes and Perl is on the PATH (macros/macros.pl).
cmake_minimum_required(VERSION 3.16)
project(codebreaker_nasm C)

set(S ${CMAKE_CURRENT_SOURCE_DIR})
set(NASM_SRC
    asm/nasm.c
    # assembler
    asm/error.c asm/floats.c asm/directiv.c asm/pragma.c asm/assemble.c asm/labels.c
    asm/parser.c asm/preproc.c asm/quote.c asm/listing.c asm/eval.c asm/exprlib.c
    asm/exprdump.c asm/stdscan.c asm/getbool.c asm/strfunc.c asm/segalloc.c asm/rdstrnum.c
    asm/srcfile.c asm/directbl.c asm/pptok.c asm/tokhash.c asm/uncompress.c asm/warnings.c
    macros/macros.c
    # output formats
    output/outform.c output/outlib.c output/nulldbg.c output/nullout.c
    output/outbin.c output/outobj.c output/outcoff.c output/codeview.c
    # library
    stdlib/snprintf.c stdlib/vsnprintf.c stdlib/strlcpy.c stdlib/strnlen.c
    nasmlib/ver.c nasmlib/alloc.c nasmlib/asprintf.c nasmlib/crc32b.c nasmlib/crc64.c
    nasmlib/md5c.c nasmlib/string.c nasmlib/nctype.c nasmlib/file.c nasmlib/fileio.c
    nasmlib/mmap.c nasmlib/realpath.c nasmlib/path.c nasmlib/ilog2.c nasmlib/numstr.c
    nasmlib/rlimit.c nasmlib/zerobuf.c nasmlib/bsi.c nasmlib/rbtree.c nasmlib/hashtbl.c
    nasmlib/raa.c nasmlib/saa.c nasmlib/strlist.c nasmlib/perfhash.c nasmlib/badenum.c
    nasmlib/readnum.c
    common/common.c common/errstubs.c
    x86/insnsa.c x86/insnsb.c x86/insnsn.c x86/regs.c x86/regvals.c x86/regflags.c x86/iflag.c
    zlib/adler32.c zlib/crc32.c zlib/infback.c zlib/inffast.c zlib/inflate.c zlib/inftrees.c
    zlib/zutil.c)

find_program(PERL perl)
if(PERL)
  file(GLOB MAC_FILES ${S}/macros/*.mac ${S}/output/*.mac)
  add_custom_command(
    OUTPUT ${S}/macros/macros.c
    COMMAND ${PERL} -I${S}/perllib -I${S} ${S}/macros/macros.pl ${S}/version.mac ${MAC_FILES}
    DEPENDS ${MAC_FILES} ${S}/macros/macros.pl ${S}/asm/pptok.ph ${S}/version.mac
    WORKING_DIRECTORY ${S}
    COMMENT "standard macros from the .mac files")
endif()

add_executable(nasm ${NASM_SRC})
target_include_directories(nasm PRIVATE ${S} ${S}/include ${S}/x86 ${S}/asm ${S}/output ${S}/zlib)
target_compile_definitions(nasm PRIVATE HAVE_CONFIG_H OF_ONLY OF_BIN OF_OBJ OF_WIN32 OF_WIN64
                                        OF_DEFAULT=of_bin)
if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
  message(FATAL_ERROR "NASM is built with gcc (MinGW-w64 on Windows): -G \"MinGW Makefiles\"")
endif()
target_compile_options(nasm PRIVATE -O2 -std=c23 -U__STRICT_ANSI__ -fwrapv -ftrivial-auto-var-init=zero
                                    -fno-common -w)
target_link_options(nasm PRIVATE -static)
set_target_properties(nasm PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
