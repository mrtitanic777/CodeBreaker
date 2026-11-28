// engine/machine_x64.hpp -- the 64-bit x86 console lanes, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_ps4_lane();         // "ps4":      PlayStation 4 (AMD Jaguar), an ELF or an unencrypted SELF (formats/orbis)
//     engine::register_ps5_lane();         // "ps5":      PlayStation 5 (Zen 2), the same ELF the payload SDK links
//     engine::register_xboxone_lane();     // "xboxone":  Xbox One / Series, the PE32+ in an .appx / .msix package or loose (formats/xboxone)
//
// A program is written back as NASM source (source/console_writer: config/platforms/ps4.json, ps5.json, xboxone.json, over source/x64_writer) and rebuilt; one that comes back identical is the
// original, an edited one has each function run beside the original's in Unicorn's x86-64 engine (the shared x86 engine CodeBreaker's DOS, Win32 and original Xbox
// lanes run on, in 64-bit mode) over the process's memory: the segments at their addresses (a position-independent program at 0x10000000, a PE at its image
// base), a stack, a scratch area the arguments may point into (filled from the state's `fill`), and a thread block. The loader's work is done as the system's
// would be: relocations applied, and each imported function's slot filled with the address of a stub that logs the call by name and returns 0 (`sceKernelUsleep(...)`,
// `kernel32!Sleep(...)`); the exit functions end the run. A `syscall` instruction is logged by number and, where there is one, its FreeBSD name, and returns 0.
// What is compared is how a function ended, RAX RDX, the callee-saved registers, RSP, the memory it changed, and the logged calls.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec ps4_lane_spec();
verify::lanes::ImageLaneSpec ps5_lane_spec();
verify::lanes::ImageLaneSpec xboxone_lane_spec();

// Add "ps4", "ps5" and "xboxone" to the machine lane table. Each is safe to call twice.
void register_ps4_lane();
void register_ps5_lane();
void register_xboxone_lane();

}  // namespace cb::engine
