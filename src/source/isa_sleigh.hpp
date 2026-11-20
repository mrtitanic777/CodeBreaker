// source/isa_sleigh.hpp -- any processor's SLEIGH description as the instruction encoder of CodeBreaker's directive layer.
//
// The directive layer (build/asm_text's Encoder, driven by build/mips_asm's two-pass assembler: labels, .org, data, includes) needs
// one thing from a processor: one instruction's text at an address -> its bytes. The SLEIGH assembler (decompile/engine/
// sleigh_assembler) gives that for every description, in the syntax its reader prints, so one assembler serves every processor.
//
//     const build::Encoder& z80 = isa::sleigh_encoder(languages / "z80.sla", {});
//     Bytes rom = build::mips_asm::Assembler(z80).build(source);
#pragma once

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include "build/asm_text.hpp"

namespace cb::source::isa {

// The encoder of one description and starting context (made once per pair, kept for the process).
const build::Encoder& sleigh_encoder(const std::filesystem::path& sla, const std::vector<std::pair<std::string, int>>& context);

}  // namespace cb::source::isa
