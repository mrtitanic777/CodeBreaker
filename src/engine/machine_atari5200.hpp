// engine/machine_atari5200.hpp -- the Atari 5200 lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_atari5200_lane();     // once, before a LaneKit is made (register_atari_lanes() does it with the other Atari machines)
//
// A cartridge (formats/atari5200) is written back as dasm source (source/console_writer, config/platforms/atari5200.json) and rebuilt; one that comes back identical is the
// original, an edited one has each function run beside the original's in the 6502 interpreter (emulate/m6502) over the 5200's memory: 16 KB of RAM at
// $0000-$3FFF, the ROM at the top of the cartridge window, and as logged devices the GTIA ($C000-$CFFF, repeating every 32 bytes), ANTIC ($D400-$D7FF,
// every 16) and POKEY ($E800-$EFFF, every 16) and the BIOS ROM window ($F800-$FFFF, which holds no BIOS here: every fetch from it reads RTS, so a
// call into the BIOS returns, and the access is logged). The chips answer as an idle console: the triggers and console keys read not pressed, no
// key code, the pots at rest, ANTIC's VCOUNT counting on each read, POKEY's RANDOM a fixed sequence. The comparison is of how a function ended,
// A X Y P S, the 16 KB of RAM and every device access in order.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec atari5200_lane_spec();

// Add "atari5200" to the machine lane table. Safe to call twice.
void register_atari5200_lane();

}  // namespace cb::engine
