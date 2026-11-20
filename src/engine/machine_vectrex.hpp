// engine/machine_vectrex.hpp -- the Vectrex lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_vectrex_lane();     // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// A cartridge (formats/vectrex) is written back as 6809 source (source/vectrex_writer) and rebuilt; one that comes back identical is the original, an edited one has each
// function run beside the original's in the 6809 interpreter (emulate/m6809) over the Vectrex's memory: the cartridge ROM at $0000 (writes to it are dropped), 1 KB of RAM at
// $C800-$CBFF (seen again at $CC00-$CFFF), the VIA 6522 at $D000-$D7FF as a logged device, and the system ROM at $E000-$FFFF as stubs: every byte an RTS, and each entry into it
// is noted by the routine's name (Wait_Recal ...), so what a function asks of the BIOS is part of what is compared. The VIA answers as an idle machine: the timer 1 flag
// in the interrupt flag register (bit 6 of $D00D) is set on every second read, so a loop that waits for the frame timer ends; no button is down (port A reads $FF), the
// joystick comparator reads low, every other register reads 0. A function is called with the registers of the state and the stack at $CBFF; the comparison is of how it ended,
// A B X Y U S DP CC, the 1 KB of RAM, every VIA access in order and the BIOS entries.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec vectrex_lane_spec();

// Add "vectrex" to the machine lane table. Safe to call twice.
void register_vectrex_lane();

}  // namespace cb::engine
