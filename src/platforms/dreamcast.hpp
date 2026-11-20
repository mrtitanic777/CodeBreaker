// platforms/dreamcast.hpp -- the Sega Dreamcast: one SH-4 (SH7750, little-endian), its programs on a disc: IP.BIN, the 32 KB of boot sectors the BIOS
// loads to $8C008000, and 1ST_READ.BIN, which it loads to $8C010000 (scrambled on a CD-R). Dreamcast programs were mostly C built with GCC for the SH-4
// (KallistiOS homebrew) or Hitachi's compiler, and what comes back is assembly: GNU as syntax, one function after another, which CodeBreaker's own
// assembler rebuilds into the same bytes. The system area and the program built from the source are compared with the disc's, and the functions are run
// in CodeBreaker's own SH-4 (emulate/sh2 as Variant::Sh4) beside the original's.
#pragma once

#include "platforms/package.hpp"

namespace cb::platforms {

Platform dreamcast();

}  // namespace cb::platforms
