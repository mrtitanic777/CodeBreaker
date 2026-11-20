// build/a64_asm.hpp -- CodeBreaker's own AArch64 assembler, for the source the Switch lane writes.
//
//     a64_asm::Image image = a64_asm::build(source_text);   // the bytes from the first address written to the last
//
// The directives follow GNU as's for AArch64 (the instruction syntax is decompile/a64's):
//
//     .org 0x1000                      where the next bytes go (the first .org is where the image starts, 0 when there is none; later ones may only move forward)
//     .byte 1, 0x02, label, "text"     bytes            .hword / .short 0x1234, label     16-bit little-endian      .word / .long / .int 0x12345678, label    32-bit
//     .quad / .xword 0x1234, label+8   64-bit           .ascii "text"   .asciz / .string "text" (with the zero)
//     .fill count, size, value         `count` units of `size` bytes (1, 2, 4 or 8) holding value (size and value may be left out: size 1, value 0)
//     .space count[, value]  .zero count     count bytes (of value)           .balign n   .p2align n   .align n (a power of two, as GNU as's AArch64 reads it)  pad with zeros
//     .incbin "file"                   the bytes of a file (looked for beside the source, in `include_dir`)
//     .equ NAME, value                 a constant (also .set)             label:   the address of the next byte
//     adrp x0, label / add x0, x0, :lo12:label     an instruction (decompile/a64)
//     // comment                       (or a line whose first character is #)
//
// Unwritten bytes between two .orgs are zero. Two passes: the first gives each instruction its size (4) and each label its address.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

#include "core/bytes.hpp"

namespace cb::build::a64_asm {

struct Image {
    std::uint64_t base = 0;   // the address of data[0]
    Bytes data;
};

// AsmError, with the line, for anything that does not assemble.
Image build(std::string_view source, const std::filesystem::path& include_dir = {});

}  // namespace cb::build::a64_asm
