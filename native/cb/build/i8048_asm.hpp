// cb/build/i8048_asm.hpp -- CodeBreaker's own 8048 assembler, for the source the Odyssey 2 lane writes: the directive layer of build/byte_asm over decompile/i8048's instructions.
//
//     byte_asm::Image image = i8048_asm::build(source_text);      // ORG, DB, DW (little-endian), FILL, EQU, labels; the bytes from the first ORG
#pragma once

#include <string_view>

#include "cb/build/byte_asm.hpp"
#include "cb/decompile/asm_value.hpp"
#include "cb/decompile/i8048.hpp"

namespace cb::build::i8048_asm {

inline byte_asm::Image build(std::string_view source) {
    byte_asm::Options o;
    o.assemble = [](std::string_view line, std::uint32_t pc, const decompile::NameLabels& labels, bool lenient) { return decompile::i8048::assemble(line, pc, labels, lenient); };
    o.eval = [](std::string_view expr, const decompile::NameLabels& labels, bool lenient) { return decompile::asm_value::eval(expr, labels, lenient); };
    return byte_asm::build(source, o);
}

}  // namespace cb::build::i8048_asm
