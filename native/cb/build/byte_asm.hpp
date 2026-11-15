// cb/build/byte_asm.hpp -- the directive layer the byte-addressed 8-bit assemblers share (the 8048 of the Odyssey 2, the F8 of the Channel F): it reads a source file, gives each
// label its address in two passes and writes the bytes; the CPU's instruction syntax is the `assemble` function handed in.
//
//     ORG $0400                       where the next bytes go (the first ORG is where the image starts; later ones may only move forward)
//     DB 1, $02, label, "text"        bytes (numbers, labels, strings)            DW $1234, label    16-bit words (the byte order is the CPU's)
//     FILL count, value               count bytes of value
//     NAME EQU value                  a constant (also `NAME = value`)
//     label  or  label:               the address of the next byte
//     LDA #1 ...                      an instruction
//     ; comment
//
// Unwritten bytes between two ORGs are zero.
#pragma once

#include <cstdint>
#include <functional>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/decompile/labels.hpp"

namespace cb::build::byte_asm {

struct Image {
    std::uint32_t base = 0;   // the address of data[0]
    Bytes data;
};

struct Options {
    // The bytes of one instruction line at `pc` (`lenient`: unknown names are 0, nothing is range-checked).
    std::function<Bytes(std::string_view line, std::uint32_t pc, const decompile::NameLabels& labels, bool lenient)> assemble;
    // The value of an expression.
    std::function<std::int64_t(std::string_view expr, const decompile::NameLabels& labels, bool lenient)> eval;
    bool big_endian_words = false;
    // For an instruction whose size depends on a value (an absolute address of 8, 16 or 24 bits): repeat the first pass, a name defined further on standing for its address from the
    // pass before, until no label moves.
    bool converge = false;
};

// AsmError, with the line, for anything that does not assemble.
Image build(std::string_view source, const Options& options);

}  // namespace cb::build::byte_asm
