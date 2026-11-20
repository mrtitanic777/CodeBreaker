// platforms/cpc.hpp -- the Amstrad CPC: Z80 programs on a disk (.dsk), in a snapshot (.sna) or as an AMSDOS file (.bin). They were written in
// assembly, which is what comes back (sjasmplus / pasmo syntax); the program built from that source is compared with the original, and its
// functions are run in CodeBreaker's own Z80 with the firmware jumpblock stubbed and its calls and its gate array / CRTC / PPI ports logged.
#pragma once

#include "platforms/package.hpp"

namespace cb::platforms {

Platform cpc();

}  // namespace cb::platforms
