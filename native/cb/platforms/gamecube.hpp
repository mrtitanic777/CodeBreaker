// cb/platforms/gamecube.hpp -- the Nintendo GameCube: Gekko (a PowerPC 750 with paired singles, 32-bit, big-endian) code in a DOL executable or an ELF
// (devkitPPC's output). Its homebrew was written in C (devkitPPC's GCC with libogc) or assembly; what comes back is assembly: GNU as syntax, one
// function after another, which the assembler rebuilds into the same file. The file built from that source is compared with the original, and its
// functions are run in CodeBreaker's own PowerPC interpreter (emulate/powerpc, paired singles on) beside the original's, over the console's memory
// with its hardware registers (the command processor, video, the disc, the controllers, audio ...) named and logged, not emulated.
#pragma once

#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform gamecube();

}  // namespace cb::platforms
