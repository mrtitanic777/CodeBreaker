// cb/platforms/gba.hpp -- the Nintendo Game Boy Advance: ARM7TDMI code (ARM and Thumb state) in a cartridge ROM. Its programs were mostly C, built
// with devkitARM's GCC (and some assembly), and what comes back is assembly: GNU as syntax, one function after another, which the assembler
// rebuilds into the same ROM. The cartridge built from that source is compared with the original, and its functions are run in CodeBreaker's
// own ARM7TDMI (emulate/arm) beside the original's.
#pragma once

#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform gba();

}  // namespace cb::platforms
