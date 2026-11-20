// platforms/segacd.hpp -- the Sega CD / Mega-CD: two 68000 programs on a disc's boot sectors, the main CPU's (security code and IP) and
// the sub CPU's (SP, in PRG-RAM, which calls the CD BIOS). Both come back as assembly (Motorola syntax) in one source, rebuilt with
// CodeBreaker's own assembler and compared with the original's boot area; their functions are run in CodeBreaker's own 68000 over the main
// CPU's memory map (the Genesis's, with the gate array and word RAM) or the sub CPU's (PRG-RAM, word RAM, the gate array, PCM), the sub CPU's
// BIOS calls logged.
#pragma once

#include "platforms/package.hpp"

namespace cb::platforms {

Platform segacd();

}  // namespace cb::platforms
