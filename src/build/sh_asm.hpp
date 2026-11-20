// build/sh_asm.hpp -- CodeBreaker's own SuperH assembler, for the source the Sega 32X and Saturn lanes write (SH-2, big-endian).
//
//     Assembler sh(encoder);                       // the SH instruction syntax (decompile/sh2)
//     Bytes file = sh.build(source_text);          // the bytes written, in the order they were written
//
// It reads the part of GNU as's language (the assembler of the Saturn and 32X homebrew toolchains: sh-elf-as) that source/sh2_writer
// writes, so the source's instructions also build with it:
//
//     .long / .int / .4byte     longwords (big-endian): numbers, labels, expressions
//     .word / .short / .2byte / .hword   words of 16 bits (on the SH `.word` is two bytes);  .byte  bytes
//     .ascii / .asciz / .string text (with \n \t \r \0 \\ \" and \ooo octal), without and with the terminating zero
//     .space n[, fill] / .skip  n bytes (zero, or fill);  .fill n, size, value   n items of `size` bytes
//     .align n                  zeros up to a multiple of 2^n (the SH's ELF targets count in powers of two);  .p2align n likewise;
//                               .balign n up to a multiple of n bytes
//     .equ name, value          constants (also `.set name, value`; and `name = value`)
//     label:                    the address of the next byte
//     .org address              the address the next byte has (see below)
//     ! comment
//
// The file is the bytes in the order they are written. `.org` does not move the file: it says what address the next byte is seen at, which
// is what labels, branches and PC-relative loads use -- a Saturn disc's boot sectors are written at address 0, then `.org 0x06004000`
// begins the program at the address it is loaded to. `.globl .text .data .section .type .size .file .extern .local .comm .arch .cpu` are
// accepted and ignored (a flat image has no sections or symbols). An instruction must be at an even address.
//
// The directive layer sits over an `Encoder`, so it is tested without the decompiler's instruction tables. An assembler holds no state
// between builds.
#pragma once

#include <cstdint>
#include <string_view>

#include "build/asm_text.hpp"

namespace cb::build::sh_asm {

class Assembler {
public:
    // The encoder is borrowed and must outlive the assembler.
    // `little_endian`: `.long`, `.word` and `.fill` write the low byte first (the SH-4 in the Dreamcast's mode; the encoder's instructions must too).
    explicit Assembler(const Encoder& encoder, bool little_endian = false) : isa_(&encoder), little_(little_endian) {}

    // The file `source` describes. AsmError, with the line, for anything that does not assemble (including a file past 16 MiB).
    Bytes build(std::string_view source) const;

private:
    const Encoder* isa_;
    bool little_;
};

// `line` with its `!` comment removed (not one inside a string), trimmed at the end.
std::string_view strip(std::string_view line);

}  // namespace cb::build::sh_asm
