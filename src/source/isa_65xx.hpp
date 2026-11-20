// source/isa_65xx.hpp -- the 65SC02 (Atari Lynx) as the source writers use it: the flow decoder and the instruction encoder, as
// source/isa.hpp gives them for the NMOS 6502 (m6502_decoder, m6502_encoder); and the HuC6280 (PC Engine) and the 65C816 (SNES) encoders.
#pragma once

#include "build/m6502_asm.hpp"
#include "formats/code_scan.hpp"

namespace cb::source::isa {

const formats::Decoder& m65sc02_decoder();                       // BRA is a jump; JMP (abs,X) is indirect
const build::m6502_asm::Encoder6502& m65sc02_encoder();
const build::m6502_asm::Encoder6502& huc6280_encoder();          // the HuC6280's instruction set (decompile/huc6280)
const build::m6502_asm::Encoder6502& w65816_encoder();           // the 65C816's, with the register widths `.a16` / `.i16` give (decompile/w65816)


}  // namespace cb::source::isa
