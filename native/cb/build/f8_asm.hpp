// cb/build/f8_asm.hpp -- CodeBreaker's own F8 assembler, for the source the Channel F lane writes: the directive layer of build/byte_asm over decompile/f8's instructions.
//
//     byte_asm::Image image = f8_asm::build(source_text);      // ORG, DB, DW (big-endian), FILL, EQU, labels; the bytes from the first ORG
#pragma once

#include <string_view>

#include "cb/build/byte_asm.hpp"
#include "cb/decompile/asm_value.hpp"
#include "cb/decompile/f8.hpp"

namespace cb::build::f8_asm {

inline byte_asm::Image build(std::string_view source) {
    byte_asm::Options o;
    o.assemble = [](std::string_view line, std::uint32_t pc, const decompile::NameLabels& labels, bool lenient) { return decompile::f8::assemble(line, pc, labels, lenient); };
    o.eval = [](std::string_view expr, const decompile::NameLabels& labels, bool lenient) { return decompile::asm_value::eval(expr, labels, lenient); };
    o.big_endian_words = true;
    return byte_asm::build(source, o);
}

}  // namespace cb::build::f8_asm
