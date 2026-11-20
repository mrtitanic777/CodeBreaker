// platforms/neogeo.hpp -- the Neo Geo (AES / MVS): 68000 code in a cartridge's P-ROM (the Z80 sound program in the M-ROM is a separate
// chip). Its programs were written in assembly or in C compiled for the 68000; what comes back is assembly (Motorola syntax), rebuilt with
// CodeBreaker's own assembler and compared with the original, and its functions are run in CodeBreaker's own 68000 over the Neo Geo's
// memory map (work RAM, palette RAM, the LSPC, the system ports, the BIOS's calls logged).
#pragma once

#include "platforms/package.hpp"

namespace cb::platforms {

Platform neogeo();

}  // namespace cb::platforms
