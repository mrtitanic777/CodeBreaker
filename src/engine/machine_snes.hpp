// engine/machine_snes.hpp -- the Super Nintendo lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_snes_lane();     // once, before a LaneKit is made (the host's start-up does, with the other 65xx consoles)
//
// A cartridge image (formats/snes: `.sfc` / `.smc`, LoROM or HiROM by its internal header) is written back as ca65 source (source/console_writer, config/platforms/snes.json) and rebuilt; one
// that comes back identical is the original, an edited one has each function run beside the original's in the 65C816 interpreter (emulate/w65816) over the
// console's memory: the ROM by its map, the 128 KB of RAM, and the chips' registers as logged devices (PPU, the APU ports, the WRAM port, the joypads, the CPU's
// and the DMA channels', each access under its register's name; DMA runs, HDMA is logged). A function is run in the state it starts in -- native or emulation
// mode, the accumulator and index registers 8 or 16 bits wide, as flow found them at its first instruction (the function's address says: source/console_snes.hpp) --
// with the registers of the state, the program and data banks of its address, and the RAM filled from the state's seed; the comparison is of how it ended, A X Y P S D
// DBR, the RAM and every chip access in order. The SPC700 sound program is data, and the cartridge coprocessors are not modelled.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec snes_lane_spec();

// Add "snes" to the machine lane table: `.sfc` / `.smc` cartridges. Safe to call twice.
void register_snes_lane();

}  // namespace cb::engine
