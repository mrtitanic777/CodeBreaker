// platforms/nds.hpp -- the Nintendo DS: a cartridge (.nds) holding a program for each of the console's two processors, the ARM9 (ARMv5TE: ARM and
// Thumb state, CLZ, BLX, saturating arithmetic, coprocessor 15) and the ARM7 (the Game Boy Advance's ARM7TDMI). Its programs were mostly C and C++, built with
// devkitARM's GCC and libnds, and what comes back is assembly: GNU as syntax, both programs in one source (one part each), which the assembler rebuilds into
// the same ROM, header and file system included. The cartridge built from that source is compared with the original, and each function is run in CodeBreaker's
// own ARM interpreter (emulate/arm: System::Nds9 or System::Nds7) beside the original's.
#pragma once

#include "platforms/package.hpp"

namespace cb::platforms {

Platform nds();

}  // namespace cb::platforms
