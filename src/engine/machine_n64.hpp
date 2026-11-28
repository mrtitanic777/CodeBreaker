// engine/machine_n64.hpp -- the Nintendo 64 lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_n64_lane();      // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// The lane is one verify/lanes/image_lane: a ROM (`.z64`, `.v64`, `.n64`) is written back by source/console_writer (config/platforms/n64.json) and rebuilt; one that comes back
// identical is the original, an edited one has each function run beside the original's in CodeBreaker's own R4300i interpreter (emulate/r4300)
// over the console's memory: 8 MB of RDRAM holding the program as the boot code leaves it, SP memory, the cartridge, the RCP's registers logged.
// A function is run with the registers of the state (the argument registers and S0 sign-extended into their 64 bits) and a work area of RDRAM
// seeded from the state's `fill`; the comparison is of how it ended, V0 V1, S0-S7, SP, HI and LO, the FPU's F0, the RDRAM and SP memory it
// changed, and every register access in order. A function that starts the RSP has its microcode run to its BREAK (emulate/rsp: what the task
// leaves in DMEM and RDRAM is part of the memory compared, and each task's start, length and ending is compared), and every display list
// handed to the RDP is compared as its commands (emulate/rdp; the RDP is not run: no pixels).
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

// What the lane is made from (exposed for tests; the table calls the same function).
verify::lanes::ImageLaneSpec n64_lane_spec();

// Add "n64" to the machine lane table: `.z64`, `.v64` and `.n64` ROMs. Safe to call twice.
void register_n64_lane();

}  // namespace cb::engine
