// platforms/wii.hpp -- the Nintendo Wii: Broadway (the Gekko's successor, the same PowerPC 750 instructions) code in a homebrew DOL executable or
// an ELF. A program is told from the GameCube's by what it touches: the second memory (MEM2) or the Hollywood registers. The functions are run in
// CodeBreaker's own PowerPC interpreter over the Wii's memory with the GameCube's hardware blocks, the mailbox with the ARM processor (IPC) and the
// Hollywood registers named and logged.
#pragma once

#include "platforms/package.hpp"

namespace cb::platforms {

Platform wii();

}  // namespace cb::platforms
