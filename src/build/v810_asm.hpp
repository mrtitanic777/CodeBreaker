// build/v810_asm.hpp -- CodeBreaker's own V810 assembler, for the source the Virtual Boy lane writes.
//
//     v810_asm::Image image = v810_asm::build(source_text);   // the bytes from the first address written to the last
//
// The directives follow the GNU assembler's (which also builds the instruction syntax of decompile/v810, `.fill` apart):
//
//     .org 0x07000000                 where the next bytes go (the first .org is where the image starts; later ones may only move forward)
//     .byte 1, 0x02, label, "text"    bytes (numbers, labels, strings)            .hword 0x1234, label    16-bit little-endian       .word 0x12345678, label   32-bit
//     .ascii "text"                   the bytes of a string                       .fill count, value      count bytes of value
//     .equ NAME, value                a constant (also .set)
//     label:                          the address of the next byte
//     movhi hi(label),r0,r1 ...       an instruction (decompile/v810)
//     ; comment                       (or # in the first column)
//
// Unwritten bytes between two .orgs are zero. Two passes: the first gives each instruction its size (fixed by the opcode) and each label its address.
#pragma once

#include <cstdint>
#include <string_view>

#include "core/bytes.hpp"

namespace cb::build::v810_asm {

struct Image {
    std::uint32_t base = 0;   // the address of data[0]
    Bytes data;
};

// AsmError, with the line, for anything that does not assemble.
Image build(std::string_view source);

}  // namespace cb::build::v810_asm
