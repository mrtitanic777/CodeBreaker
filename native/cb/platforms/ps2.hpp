// cb/platforms/ps2.hpp -- the PlayStation 2: Emotion Engine (R5900, MIPS III with 128-bit registers and multimedia instructions, little-endian) programs, ps2sdk's `.elf` and disc images (`.iso`) whose BOOT2 program is one. What comes back is assembly: GNU as syntax, the ELF written whole (headers, sections, symbols) and rebuilt byte for byte; its functions are run in CodeBreaker's own R5900 (emulate/ee) beside the original's. The vector units' micro-programs, the IOP's modules, the GS and the DMA controller are not run.
#pragma once

#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform ps2();

}  // namespace cb::platforms
