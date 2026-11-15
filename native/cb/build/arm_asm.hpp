// cb/build/arm_asm.hpp -- CodeBreaker's own ARM7TDMI assembler, for the source the Game Boy Advance lane writes.
//
//     Assembler arm(encoder);                      // the ARM and Thumb instruction syntax (decompile/arm)
//     Bytes file = arm.build(source_text);         // the bytes written, in the order they were written
//
// It reads the part of GNU as's language (the assembler of devkitARM, arm-none-eabi-as, `.syntax unified`) that source/gba_writer writes,
// so the source's instructions also build with it:
//
//     .arm / .code 32           the instructions that follow are ARM (4 bytes, at a multiple of four);  .thumb / .code 16  Thumb (2 or 4
//                               bytes, at a multiple of two). The lowest bit of the address the encoder is given is the state.
//     .word / .4byte / .long / .int   words (little-endian): numbers, labels, expressions
//     .half / .hword / .short / .2byte   halfwords;  .byte  bytes
//     .ascii / .asciz / .asciiz text (with \n \t \r \0 \\ \" and \ooo octal), without and with the terminating zero
//     .space n[, fill]          n bytes (zero, or fill);  .fill n, size, value   n items of `size` bytes
//     .align n                  zeros up to a multiple of 2^n;  .p2align n likewise;  .balign n  up to a multiple of n bytes
//     .equ name, value          constants (also `.set name, value`)
//     name = value
//     label:                    the address of the next byte (a plain address: a label in Thumb code has no state bit; write `label + 1`
//                               for a Thumb function's address, as a pointer to it is held)
//     .org address              the address the next byte has (see below)
//     @ comment                 (also a line that begins with `#`)
//
// The file is the bytes in the order they are written. `.org` does not move the file: it says what address the next byte is seen at,
// which is what labels and branches use -- a Game Boy Advance cartridge begins at file offset 0, and `.org 0x08000000` says it is seen at
// the address the console maps it to. `.globl .global .text .data .bss .section .type .size .thumb_func .syntax .fpu
// .eabi_attribute .file .extern .local .comm .ltorg` are accepted and ignored (a flat image has no sections or symbols; there are no
// literal pools to place: a PC-relative load names the word it reads).
//
// The directive layer sits over an `Encoder`, so it is tested without the decompiler's instruction tables. An assembler holds no
// state between builds.
#pragma once

#include <cstdint>
#include <string_view>

#include "cb/build/asm_text.hpp"

namespace cb::build::arm_asm {

class Assembler {
public:
    // The encoder is borrowed and must outlive the assembler. `armv5` is the ARMv5TE encoder (decompile/arm: `Arch::V5TE`) that a `.cpu arm946e-s`
    // (or `.arch armv5te`) line switches to, and `.cpu arm7tdmi` back from; without one every instruction is the ARM7TDMI's.
    explicit Assembler(const Encoder& encoder, const Encoder* armv5 = nullptr) : isa_(&encoder), v5_(armv5) {}

    // The file `source` describes. AsmError, with the line, for anything that does not assemble (including an instruction whose size
    // differs between the two passes, and a file past 16 MiB).
    Bytes build(std::string_view source) const;

private:
    const Encoder* isa_;
    const Encoder* v5_;
};

// `line` with its `@` comment removed (not one inside a string), a line that begins with `#` empty, trimmed at the end.
std::string_view strip(std::string_view line);

// An instruction's size on the first pass, when a label it names is not defined yet: assembled with every unknown name standing for a
// word-aligned place just ahead, so the bytes have the size the instruction will have. `pc` carries the state in its lowest bit. (Public for
// the tests.)
Result<Bytes> size_guess(const Encoder& encoder, std::string_view line, std::int64_t pc, const Labels& labels);

}  // namespace cb::build::arm_asm
