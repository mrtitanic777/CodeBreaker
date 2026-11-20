// platforms/psp.hpp -- the PSP: Allegrex (MIPS32 with the VFPU, little-endian) programs, unencrypted: pspsdk's `.elf`, `.prx` and `EBOOT.PBP`. What comes back is assembly, the file written whole and rebuilt byte for byte; its functions are run in CodeBreaker's own Allegrex (emulate/allegrex) beside the original's, imports logged by name. Encrypted `~PSP` modules are not handled; the VFPU is decoded but not run.
#pragma once

#include "platforms/package.hpp"

namespace cb::platforms {

Platform psp();

}  // namespace cb::platforms
