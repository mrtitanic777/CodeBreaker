// msvc2.h -- the MSVC 2.0 (win95 C2.EXE, 32-bit) models: invert/msvc2_forward.py (emit, to_c,
// frame_layout), invert/msvc2.py (invert) and invert/msvc2_registers.py (interp).
#pragma once

#include <string>

namespace cbinv {

// which 0: msvc2.invert   ("C <hex>", "M <offsets, comma-separated>" or no M line for mask=None)
//       1: msvc2_registers.interp ("C <hex>", "N <nargs>")
//       2: msvc2_forward.emit (the node, as pyval JSON) -> hex or null
//       3: msvc2_forward.to_c (the node) -> a JSON string
//       4: msvc2_forward.frame_layout (a pyval list of (size, is_array) tuples) -> [framesize, [[i, off]..]]
std::string msvc2_call(const std::string &in, int which);

}  // namespace cbinv
