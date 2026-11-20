// engine/machine_atari7800.hpp -- the Atari 7800 lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_atari7800_lane();     // once, before a LaneKit is made (register_atari_lanes() does it with the other Atari machines)
//
// A cartridge (formats/atari7800) is written back as dasm source (source/atari7800_writer) and rebuilt; one that comes back identical is the
// original, an edited one has each function run beside the original's in the 6502 interpreter (emulate/m6502) over the 7800's memory: the TIA
// ($00-$1F) and MARIA ($20-$3F) and the RIOT ($0280-$02FF) as logged devices, the 4 KB of RAM at $1800-$27FF with its zero-page and stack windows
// ($0040-$00FF is $2040-$20FF, $0140-$01FF is $2140-$21FF), and the ROM at the top of memory (a SuperGame board's bank select, a write to
// $8000-$FFFF, switches the window at $8000 and is logged). The chips answer as an idle console: MARIA's status register alternates its vertical-blank
// bit on each read (so a loop that waits for either edge ends), the joystick fire inputs read not pressed, the RIOT's timer has run out. The comparison is of
// how a function ended, A X Y P S, the 4 KB of RAM and every device access in order. The 2600 mode of the machine is not modelled.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec atari7800_lane_spec();

// Add "atari7800" to the machine lane table. Safe to call twice.
void register_atari7800_lane();

}  // namespace cb::engine
