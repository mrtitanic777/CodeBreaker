// cb/build/m68k_asm.hpp -- CodeBreaker's own 68000 assembler, for the source the Genesis lane writes.
//
//     Assembler m68k(encoder);                     // the 68000 instruction syntax (decompile/m68k)
//     Bytes image = m68k.build(source_text);       // the bytes from address 0 to the last one written
//
// It reads the part of the Motorola assemblers' language (asm68k, SNASM68K, vasm's mot syntax) that
// source/genesis_writer writes, so the source also builds with those:
//
//     org $addr                       where the next bytes go
//     dc.b / dc.w / dc.l              bytes, words and longs, big-endian: numbers, labels, "strings"
//     dcb.b n,value                   n bytes of value (dcb.w / dcb.l: words, longs)
//     NAME equ value                  constants
//     label:                          the address of the next byte
//     even                            pad to an even address
//     ; comment  (or * at the start of a line)
//
// Unwritten bytes between two org's are zero. Source text is bytes (asm_text.hpp): a "string" in dc.b is exactly the
// bytes between its quotes. The directive layer sits over an `Encoder`, so it is tested without the decompiler's
// instruction tables, which own the 68000's syntax. An assembler holds no state between builds.
#pragma once

#include <cstdint>
#include <string_view>

#include "cb/build/asm_text.hpp"

namespace cb::build::m68k_asm {

class Assembler {
public:
    // The encoder is borrowed and must outlive the assembler.
    explicit Assembler(const Encoder& encoder) : isa_(&encoder) {}

    // The image `source` describes. AsmError, with the line, for anything that does not assemble (including a
    // branch whose size differs between the two passes, and bytes outside the 64 MiB an image may span).
    Bytes build(std::string_view source) const;

private:
    const Encoder* isa_;
};

// `line` with its comment removed, trimmed at the end: a line whose first non-blank character is `*` is all comment.
std::string_view strip(std::string_view line);

// An instruction's size on the first pass, when a label it names is not defined yet: assembled with every unknown
// name standing for a nearby address (pc + 4; the source gives each operand's size: BRA.S / .W, (x).W / (x).L), so
// the bytes have the size the instruction will have. `labels` are the names defined so far. The error is the
// encoder's own when the line fails for another reason. (Public for the tests.)
Result<Bytes> size_guess(const Encoder& encoder, std::string_view line, std::int64_t pc, const Labels& labels);

} // namespace cb::build::m68k_asm
