// emulate/cpu/pcode_ops_arm32.hpp -- the ARM descriptions' named operations (the saturation helpers, the VFP's conversions, NEON's lane operations) as host code over
// emulate/cpu/pcode_vector.hpp, bound to an emulator by number (engine/pcode_ops.hpp).
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "emulate/cpu/pcode_ops.hpp"

namespace cb::emulate::pcode_ops {

// What the handlers need of the run they serve: the word of the instruction about to run (the ARM word it means: a Thumb VFP instruction has its halves in ARM order), and the
// VFP's status register's rounding and flush bits as the instruction sees them.
struct Arm32Context {
    std::uint32_t word = 0;
    std::uint32_t fpscr = 0;          // the status register before the instruction
    std::uint32_t fpscr_flags = 0;    // the exception flags the instruction raised (ORed into fpscr by the adapter)
};

// The table of the operations served (built once).
const Table& arm32_table();

// Bind arm32_table() to `emu` (an ARM*.sla emulator) with `context`: the operations of the description it serves now run by number.
Binding bind_arm32(Emu& emu, Arm32Context* context);

// The operations of the description that are named for the architecture and that arm32_table() does not serve (the coprocessor, software-interrupt, barrier, mode and
// hint operations are not listed: the adapter owns them).
std::vector<std::string> arm32_unserved(Emu& emu);

}  // namespace cb::emulate::pcode_ops
