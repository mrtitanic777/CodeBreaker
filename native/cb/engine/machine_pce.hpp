// cb/engine/machine_pce.hpp -- the PC Engine / TurboGrafx-16 lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_pce_lane();     // once, before a LaneKit is made (the host's start-up does, with the other 65xx consoles)
//
// A HuCard image (formats/pce) is written back as ca65 source (source/pce_writer) and rebuilt; one that comes back identical is the original, an
// edited one has each function run beside the original's in the HuC6280 interpreter (emulate/huc6280) over the console's memory: the card's ROM in
// 8 KB banks, the 8 KB of RAM, the eight mapping registers, and the chips' registers as logged devices (VDC, VCE, PSG, timer, joypad port, interrupt
// control, each access under its register's name). A function is run with the mapping its own bank implies (the bank its address is in, at the page
// its address is in; the RAM at $2000, the chips at $0000, the other pages mapping banks 2 to 6), the registers of the state, and the RAM filled from the
// state's seed; the comparison is of how it ended, A X Y P S, the RAM, the mapping registers and every chip access in order.
#pragma once

#include "cb/verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec pce_lane_spec();

// Add "pce" to the machine lane table: `.pce` / `.tg16` HuCards. Safe to call twice.
void register_pce_lane();

}  // namespace cb::engine
