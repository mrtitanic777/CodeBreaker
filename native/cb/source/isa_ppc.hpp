// cb/source/isa_ppc.hpp -- the PowerPC 750 family (the GameCube's Gekko, the Wii's Broadway, the Wii U's Espresso) as the source writers use
// it: the encoder the assembler's directive layer (build/mips_asm, with big-endian data directives) sits over. The instructions are
// decompile/ppc's.
#pragma once

#include "cb/build/asm_text.hpp"

namespace cb::source::isa {

const build::Encoder& ppc_encoder();

}  // namespace cb::source::isa
