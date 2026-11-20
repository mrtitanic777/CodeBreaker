// source/isa_mips_le.hpp -- the PlayStation 2's R5900 and the PSP's Allegrex as the source writers use them: the flow decoder a format's function
// finder asks for and the encoder the assembler's directive layer sits over (the R3000A's are in source/isa.hpp, the R4300i's in source/isa_r4300.hpp).
// All four read and write little-endian words, except the R4300i's.
#pragma once

#include "build/asm_text.hpp"
#include "formats/code_scan.hpp"

namespace cb::source::isa {

// Read at the address the code runs at. A jump, a return and an indirect jump end the flow after their delay slot.
const formats::Decoder& r5900_decoder();
const build::Encoder& r5900_encoder();
const formats::Decoder& allegrex_decoder();
const build::Encoder& allegrex_encoder();

}  // namespace cb::source::isa
