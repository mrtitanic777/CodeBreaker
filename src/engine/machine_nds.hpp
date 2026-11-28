// engine/machine_nds.hpp -- the Nintendo DS lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_nds_lane();      // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// The lane is one verify/lanes/image_lane: a `.nds` ROM is written back by source/console_writer (config/platforms/nds.json) (one source, a part for each of the console's two
// processors) and rebuilt; one that comes back identical is the original, an edited one has each function run beside the original's in
// CodeBreaker's own ARM interpreter (emulate/arm) over the DS's memory (System::Nds9 for a function of the ARM9 program, System::Nds7 for the
// ARM7's: the address says which): main RAM holding the program, the TCMs, shared WRAM, video memory, I/O registers and coprocessor 15 logged,
// BIOS calls logged. A function is run with the registers of the state and the free main RAM filled from the state's seed; the comparison is of
// how it ended, R0 R1 R4-R11 SP, the memory it changed, every register access in order, and the BIOS calls.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

// What the lane is made from (exposed for tests; the table calls the same function).
verify::lanes::ImageLaneSpec nds_lane_spec();

// Add "nds" to the machine lane table: `.nds` ROMs. Safe to call twice.
void register_nds_lane();

}  // namespace cb::engine
