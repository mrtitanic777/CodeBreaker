// cb/platforms/msx.hpp -- the MSX: Z80 code on a cartridge (.rom). Its programs were written in assembly, which is what
// comes back (sjasmplus / pasmo syntax); the cartridge built from that source is compared with the original, and its
// functions are run in CodeBreaker's own Z80.
//
// The Nes, Sms, Gb and Genesis descriptors are built the same way, with `platforms::machine()`.
#pragma once

#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform msx();

}  // namespace cb::platforms
