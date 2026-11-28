// engine/machine_ps3.hpp -- the PlayStation 3 lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_ps3_lane();      // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// A PS3 homebrew ELF or unencrypted SELF (formats/ps3) is written back as GNU as source (source/console_writer: config/platforms/ps3.json) and rebuilt; one that comes back identical is the
// original, an edited one has each function run beside the original's in the PowerPC interpreter the Xbox 360 and GameCube lanes share (emulate/powerpc: 64-bit
// registers, the 64-bit rotates and doubleword loads and stores) over the process's memory (the segments at their addresses, r2 set to the program's TOC). The VMX
// instructions are decoded for their length and text, and the interpreter stops at them as unsupported. A call into an imported function lands on its unpatched
// stub (the loader fills the descriptors), so a function that calls out ends its run there the same way in the original and the rebuilt program. The comparison is
// of how a function ended, r3, r4, f1, r1, r14-r31 and the memory it changed. SPU code stays data.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec ps3_lane_spec();

// Add "ps3" to the machine lane table. Safe to call twice.
void register_ps3_lane();

}  // namespace cb::engine
