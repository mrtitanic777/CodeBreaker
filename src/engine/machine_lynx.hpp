// engine/machine_lynx.hpp -- the Atari Lynx lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_lynx_lane();     // once, before a LaneKit is made (the host's start-up does)
//
// The lane is one verify/lanes/image_lane: a `.lnx` cartridge or a BS93 object is written back by source/console_writer (config/platforms/lynx.json) and rebuilt; one that comes
// back identical is the original, an edited one has each function run beside the original's in the 65SC02 interpreter (emulate/m6502, Cmos)
// over the Lynx's memory (emulate/lynx): RAM, Suzy and Mikey logged. The program each file stands for is what the boot ROM leaves in RAM
// (formats/lynx.hpp): the cartridge's first stage is decrypted, and the loaders are run to see what they copy where; the result is kept per file.
// A function is run from the loaded image with the registers of the state, the free RAM filled from the state's seed, and the comparison
// is of how it ended, A X Y P S, the RAM it changed and every Suzy / Mikey access in order.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

// What the lane is made from (exposed for tests; the table calls the same function).
verify::lanes::ImageLaneSpec lynx_lane_spec();

// Add "lynx" to the machine lane table: `.lnx` and BS93 files. Safe to call twice.
void register_lynx_lane();

}  // namespace cb::engine
