// source/isa_r4300.hpp -- the Nintendo 64's R4300i as the source writer uses it: the flow decoder the ROM format's function finder asks
// for and the encoder the assembler's directive layer sits over (the R3000A's are in source/isa.hpp; these read and write big-endian words).
#pragma once

#include "build/asm_text.hpp"
#include "formats/code_scan.hpp"

namespace cb::source::isa {

// The R4300i, read at the address it runs at. Like the R3000A's, a jump, a return and an indirect jump end the flow after their delay
// slot; ERET ends it with none.
const formats::Decoder& r4300_decoder();
const build::Encoder& r4300_encoder();

}  // namespace cb::source::isa
