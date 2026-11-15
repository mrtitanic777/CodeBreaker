// cb/engine/machine_atari2600.hpp -- the Atari 2600 lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_atari2600_lane();     // once, before a LaneKit is made (register_atari_lanes() does it with the other Atari machines)
//
// The lane is one verify/lanes/image_lane: a cartridge (formats/atari2600) is written back as dasm source (source/atari2600_writer) and rebuilt; one
// that comes back identical is the original, an edited one has each function run beside the original's in the 6502 interpreter (emulate/m6502) over the
// 2600's memory: the 6507's 13 address lines, the 128 bytes of RAM and their mirrors (the stack is in them), the TIA and RIOT as logged devices
// (their registers answer as an idle console: no collisions, no fire button pressed, the timer run out) and the cartridge's bank switches (a touch of
// a hotspot selects a bank and is logged). A function runs in the bank it is in. The comparison is of how it ended, A X Y P S, the 128 bytes of RAM and
// every TIA, RIOT and bank-switch access in order.
#pragma once

#include "cb/verify/lanes/image_lane.hpp"

namespace cb::engine {

// What the lane is made from (exposed for tests; the table calls the same function).
verify::lanes::ImageLaneSpec atari2600_lane_spec();

// Add "atari2600" to the machine lane table. Safe to call twice.
void register_atari2600_lane();

}  // namespace cb::engine
