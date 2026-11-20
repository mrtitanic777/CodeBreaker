// build/tlcs900_asm.hpp -- CodeBreaker's own TLCS-900/H assembler, for the source the Neo Geo Pocket lane writes: the directive layer of build/byte_asm over decompile/tlcs900's
// instructions.
//
//     byte_asm::Image image = tlcs900_asm::build(source_text);      // ORG, DB, DW (little-endian), FILL, EQU, labels; the bytes from the first ORG
//
// An instruction whose size depends on its value (an absolute address in 8, 16 or 24 bits) is sized by repeating the first pass until no label moves (byte_asm's `converge`).
#pragma once

#include <string_view>

#include "build/byte_asm.hpp"
#include "decompile/asm_value.hpp"
#include "decompile/tlcs900.hpp"

namespace cb::build::tlcs900_asm {

inline byte_asm::Image build(std::string_view source) {
    byte_asm::Options o;
    o.assemble = [](std::string_view line, std::uint32_t pc, const decompile::NameLabels& labels, bool lenient) { return decompile::tlcs900::assemble(line, pc, labels, lenient); };
    o.eval = [](std::string_view expr, const decompile::NameLabels& labels, bool lenient) { return decompile::asm_value::eval(expr, labels, lenient); };
    o.converge = true;
    return byte_asm::build(source, o);
}

}  // namespace cb::build::tlcs900_asm
