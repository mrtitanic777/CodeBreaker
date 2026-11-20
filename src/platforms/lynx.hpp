// platforms/lynx.hpp -- the Atari Lynx: 65SC02 code in a `.lnx` cartridge image or a BS93 object. Its programs were written in assembly (or
// compiled with cc65) and what comes back is assembly: ca65 syntax, the segments the cartridge's loaders copy to RAM, which the assembler
// rebuilds into the same file. The cartridge's encrypted first stage is decrypted to find the loaders; the functions are run in CodeBreaker's own
// 65SC02 (emulate/m6502) over the Lynx's memory beside the original's.
#pragma once

#include "platforms/package.hpp"

namespace cb::platforms {

Platform lynx();

}  // namespace cb::platforms
