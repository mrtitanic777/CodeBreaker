// cb/platforms/nes.hpp -- the NES (Famicom): 6502 code (the 2A03, without decimal mode) on an iNES cartridge image.
// Its programs were written in assembly, which is what comes back; the cartridge built from that source is compared
// with the original, and its functions are run in CodeBreaker's own 6502.
#pragma once

#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform nes();

}  // namespace cb::platforms
