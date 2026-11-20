// platforms/n64.hpp -- the Nintendo 64: R4300i (MIPS III, 64-bit registers, big-endian) code in a cartridge ROM (.z64, .v64, .n64). Its
// programs were written in C (libdragon's GCC, Nintendo's SDK) or, for homebrew, in assembly; what comes back is assembly: GNU as syntax, one
// function after another, which the assembler rebuilds into the same ROM. The ROM built from that source is compared with the original, and
// its functions are run in CodeBreaker's own R4300i (emulate/r4300) beside the original's. The RSP and the RDP are not run.
#pragma once

#include "platforms/package.hpp"

namespace cb::platforms {

Platform n64();

}  // namespace cb::platforms
