// build/cp1610_asm.hpp -- CodeBreaker's own CP1610 assembler, for the source the Intellivision lane writes.
//
//     Bytes file = cp1610_asm::build(source_text);     // the .rom file (or the raw file) the source describes
//
// The source language is as1600's, with the directives the writer needs to say how the file is laid out:
//
//     SEGMENT $50,$68                 a segment of pages $50 to $68 (the words $5000-$68FF) begins; the address counter is at its first word
//     ORG $5100                       where the next word goes (inside the segment)
//     DECLE 1, $02, label ...         words (numbers up to 16 bits, labels)           FILL count, value     count words of value
//     ROMTAIL $00, $10 ...            the bytes that follow the segments in a .rom file (the Intellicart's tables)
//     FORMAT RAW                      the output is the raw file (big-endian words from the first segment's start, no table, no CRC)
//     NAME EQU value                  constants (also `NAME = value`)
//     label  or  label:               the word address of the next word
//     MVII #$1234,R0 ...              an instruction (decompile/cp1610); an instruction after SDBD is double-byte
//     ; comment
//
// Words nothing is written to are zero. The CRC of each segment is worked out. Two passes: the first gives each instruction its size and each label its address.
#pragma once

#include <string_view>

#include "core/bytes.hpp"

namespace cb::build::cp1610_asm {

// AsmError, with the line, for anything that does not assemble.
Bytes build(std::string_view source);

}  // namespace cb::build::cp1610_asm
