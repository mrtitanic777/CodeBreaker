// cb/platforms/sms.hpp -- the Sega Master System and Game Gear (and the SG-1000 before them): Z80 code on a cartridge
// image. Their programs were written in assembly, which is what comes back (WLA-DX syntax); the cartridge built from
// that source is compared with the original, and its functions are run in CodeBreaker's own Z80.
#pragma once

#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform sms();

}  // namespace cb::platforms
