// platforms/sega32x.hpp -- the Sega 32X: two SH-2s (big-endian, 16-bit instructions) beside the Genesis's 68000, their program in a 32X cartridge.
// SH-2 programs were mostly C built with GCC for the SH-2, or hand-written assembly, and what comes back is assembly: GNU as syntax, one function after
// another, which CodeBreaker's own assembler rebuilds into the same program (the 68000 side is the Genesis lane's: source/console_writer). The cartridge
// built from the sources is compared with the original, and the functions are run in CodeBreaker's own SH-2 (emulate/sh2) beside the original's.
#pragma once

#include "platforms/package.hpp"

namespace cb::platforms {

Platform sega32x();

}  // namespace cb::platforms
