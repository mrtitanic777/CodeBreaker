// cb/source/isa.hpp -- the processors of the console and home-computer lanes, as the source writers use them.
//
// The decompiler's tables (decompile/m6502, z80, sm83, m68k, mips, arm) know a CPU's instructions; two other layers want them in their own
// terms, and this is the one place that says how:
//
//     Decoders.  The cartridge formats (formats/nes, sms, gb, genesis) find function starts by following code, and ask for a
//                `formats::Decoder`: size, how control leaves, where it goes. A conditional return and a HALT fall through to
//                the next instruction (a HALT resumes after an interrupt); a Game Boy STOP and a 68000 STOP end the flow; a
//                68000 TRAP is a branch (its handler is not a function start).
//
//     Encoders.  build/z80_asm, m68k_asm and m6502_asm are directive layers over an `Encoder`: one instruction's text in, its
//                bytes out, with a name that has no value yet told apart from text that is not an instruction, because the first
//                pass of a two-pass assembler meets names defined further on.
//
//     build::z80_asm::Assembler assembler(isa::z80_encoder());
//     std::vector<std::uint32_t> starts = rom.functions(isa::m6502_decoder());
#pragma once

#include "cb/build/asm_text.hpp"
#include "cb/build/m6502_asm.hpp"
#include "cb/formats/code_scan.hpp"

namespace cb::source::isa {

// Does an instruction's pattern ("LD (nn),A") hold `part`?
inline bool pattern_has(std::string_view pattern, std::string_view part) { return pattern.find(part) != std::string_view::npos; }

const formats::Decoder& m6502_decoder();
const formats::Decoder& z80_decoder();
const formats::Decoder& sm83_decoder();
const formats::Decoder& m68k_decoder();
const formats::Decoder& mips_decoder();       // the R3000A, read at the address it runs at (a jump's target keeps the address's top four bits)
const formats::Decoder& arm_decoder();        // the ARM7TDMI: an address carries the state in its lowest bit (odd: Thumb), a target too
const formats::Decoder& arm9_decoder();       // the ARM946E-S (ARMv5TE): the above, with BLX, CLZ and the other ARMv5TE instructions

const build::Encoder& z80_encoder();
const build::Encoder& m68k_encoder();
const build::Encoder& mips_encoder();
const build::Encoder& arm_encoder();           // ... and the state an instruction is assembled in is the address's lowest bit
const build::Encoder& arm9_encoder();          // ... the ARMv5TE set (decompile/arm: Arch::V5TE)
const build::m6502_asm::Encoder6502& m6502_encoder();

}  // namespace cb::source::isa
