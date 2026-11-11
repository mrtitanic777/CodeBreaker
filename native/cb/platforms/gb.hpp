// cb/platforms/gb.hpp -- the Game Boy and Game Boy Color: SM83 code on a cartridge image. Their programs were written
// in assembly, which is what comes back (RGBDS syntax); the cartridge built from that source is compared with the
// original, and its functions are run in CodeBreaker's own SM83.
#pragma once

#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform gb();

}  // namespace cb::platforms
