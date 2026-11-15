// cb/platforms/ps1.hpp -- the Sony PlayStation: R3000A (MIPS I, little-endian) code in a PS-X EXE. Its programs were mostly C, built with
// the PlayStation SDK's GCC, and what comes back is assembly: GNU as syntax, one function after another, which the assembler rebuilds
// into the same executable. The executable built from that source is compared with the original, and its functions are run in
// CodeBreaker's own R3000A (emulate/mips) beside the original's.
#pragma once

#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform ps1();

}  // namespace cb::platforms
