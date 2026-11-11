// cb/platforms/genesis.hpp -- the Sega Genesis / Mega Drive: 68000 code on a cartridge image (the Z80 beside it runs
// sound code the 68000 copies in). Its programs were mostly written in assembly, which is what comes back (Motorola
// syntax); the cartridge built from that source is compared with the original, and its functions are run in
// CodeBreaker's own 68000.
#pragma once

#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform genesis();

}  // namespace cb::platforms
