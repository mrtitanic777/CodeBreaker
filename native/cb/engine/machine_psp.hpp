// cb/engine/machine_psp.hpp -- the PSP lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_psp_lane();      // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// The lane is one verify/lanes/image_lane: an unencrypted PSP program (`.elf`, `.prx`, or an `EBOOT.PBP` holding one) is written back by source/psp_writer and
// rebuilt; one that comes back identical is the original, an edited one has each function run beside the original's in CodeBreaker's Allegrex interpreter
// (emulate/allegrex) over the console's memory: 32 MB of RAM holding the module's segments, video memory, the hardware blocks logged by name, and every call to an
// imported function logged by its name. The comparison is of how a function ended, $v0 and $v1, $s0-$s7, $sp, HI and LO, the FPU's $f0, the RAM it changed, every
// hardware access in order, and every call. The graphics engine and the VFPU are not run (a function using the VFPU ends "not run" in both programs). An
// encrypted `~PSP` module is recorded as not handled: the lane does not take it.
#pragma once

#include "cb/verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec psp_lane_spec();

// Add "psp" to the machine lane table. Safe to call twice.
void register_psp_lane();

}  // namespace cb::engine
