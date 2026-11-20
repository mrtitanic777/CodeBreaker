// platforms/saturn.hpp -- the Sega Saturn: two SH-2s (the master runs the program first), its programs on a disc: IP.BIN, the boot sectors the BIOS
// loads to $06002000, and the first program it loads to $06004000. Saturn programs were mostly C built with GCC for the SH-2 (or Hitachi's compiler),
// and what comes back is assembly: GNU as syntax, one function after another, which CodeBreaker's own assembler rebuilds into the same bytes. The
// system area and the program built from the source are compared with the disc's, and the functions are run in CodeBreaker's own SH-2 (emulate/sh2)
// beside the original's.
#pragma once

#include "platforms/package.hpp"

namespace cb::platforms {

Platform saturn();

}  // namespace cb::platforms
