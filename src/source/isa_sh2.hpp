// source/isa_sh2.hpp -- the SH-2 as the source writers and the cartridge formats use it (the same two views source/isa.hpp gives the other
// processors, kept in a file of its own).
//
//     Decoder   for formats::find_functions: size 2, how control leaves, where it goes. A delay slot is not followed (it is the instruction
//               after the branch, and the walk of the code finds it on its own); `bsr` and `jsr` are calls, `jsr @rn` and `jmp @rn` have no
//               known target.
#pragma once

#include "formats/code_scan.hpp"

namespace cb::source::isa {

const formats::Decoder& sh2_decoder();
// The SH-4 (the Dreamcast's CPU: little-endian words, and the FPU).
const formats::Decoder& sh4_decoder();

}  // namespace cb::source::isa
