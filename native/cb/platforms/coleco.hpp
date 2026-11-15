// cb/platforms/coleco.hpp -- the ColecoVision: Z80 code on a cartridge (.col). Its games were written in assembly, which is what comes back
// (sjasmplus / pasmo syntax); the cartridge built from that source is compared with the original, and its functions are run in CodeBreaker's
// own Z80 with the console's memory map (BIOS stubbed, RAM at $6000) and its VDP and sound ports logged.
#pragma once

#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform coleco();

}  // namespace cb::platforms
