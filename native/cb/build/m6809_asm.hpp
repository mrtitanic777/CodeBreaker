// cb/build/m6809_asm.hpp -- CodeBreaker's own 6809 assembler, for the source the Vectrex lane writes.
//
//     Bytes image = m6809_asm::build(source_text);     // the bytes from address 0 to the last one written
//
// It reads the part of the 6809 assemblers' language (lwasm, as9) that source/vectrex_writer writes, so the source also builds with them (FILL aside):
//
//     ORG $C800                       where the next bytes go
//     FCB 1, $02, "A"+$80 ...         bytes (numbers, labels, one-character strings)       FDB $1234, label      words, big-endian
//     FCC "text"                      the bytes of a string (any delimiter that is not in the text: `"`, `/`)
//     FILL count, value               count bytes of value (CodeBreaker's; lwasm has no such directive)       RMB n   n zero bytes
//     NAME EQU value                  constants (also `NAME = value`)
//     label  or  label:               the address of the next byte (a label starts in the first column)
//     LDA #$12 ...                    an instruction (decompile/m6809: `<` direct, a bare address extended, `,X` indexed forms)
//     ; comment                       (or `*` in the first column)
//
// Unwritten bytes between two ORGs are zero. Two passes: the first gives each instruction its size (the table's addressing modes have fixed sizes except
// indexed offsets, which the source says with `<` and `>` where the encoding is not the shortest) and each label its address. An assembler holds no state between builds.
#pragma once

#include <string_view>

#include "cb/core/bytes.hpp"

namespace cb::build::m6809_asm {

// AsmError, with the line, for anything that does not assemble.
Bytes build(std::string_view source);

}  // namespace cb::build::m6809_asm
