// cb/platforms/dos.hpp -- real-mode DOS programs: MZ executables, and .COM files and device drivers, which have no header
// (the DOS reader tells them apart). Their source is assembly, rebuilt and compared.
//
// What the descriptor says: the 8086 in real mode; the near and far C conventions the decompiler writes; no emulator --
// nothing is run: a program that comes back byte for byte IS the original; OpenWatcom's wasm assembles it, wlink links
// it. (The DOS reader, the writer of the source and the toolchain's paths are the formats, source and build layers'.)
#pragma once

#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform dos();

}  // namespace cb::platforms
