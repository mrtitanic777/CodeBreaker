// engine/machine_vita.hpp -- the PlayStation Vita lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_vita_lane();      // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// An unencrypted Vita SELF or its ELF (formats/vita) is written back as GNU as source (source/console_writer: config/platforms/vita.json) and rebuilt; one that comes back identical is the original,
// an edited one has each function run beside the original's in CodeBreaker's ARM interpreter (emulate/arm, `System::Vita`: Thumb state, the ARMv7-A branches, IT,
// CBZ and the VFP; the 32-bit Thumb-2 integer instructions and NEON are decoded for their length and text but not run) over the process's memory (the segments
// at their addresses, the rest paged RAM). A call into an imported function stub is logged by name (`sceCtrlPeekBufferPositive(...)`) and returns 0; the exit
// functions end the run. The comparison is of how a function ended, R0 R1 R4-R11 SP, the memory it changed and the imported calls.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec vita_lane_spec();

// Add "vita" to the machine lane table. Safe to call twice.
void register_vita_lane();

}  // namespace cb::engine
