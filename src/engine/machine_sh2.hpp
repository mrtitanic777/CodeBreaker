// engine/machine_sh2.hpp -- the Sega 32X and Saturn lanes (SH-2), added to the machine lane table (engine/machine_lanes.hpp) without
// touching the lane kit.
//
//     engine::register_sh2_lanes();      // once, before a LaneKit is made (LaneKit::register_worker_kinds does it)
//
// Each lane is one verify/lanes/image_lane: the program is written back as source (source/console_writer: config/platforms/saturn.json) and rebuilt;
// one that comes back identical is the original, an edited one has each function run beside the original's in CodeBreaker's own SH-2
// interpreter (emulate/sh2) over the machine's memory. The comparison is of how the run ended, R0 and R1, R8-R15, PR GBR MACH MACL and SR,
// the memory it changed in the machine's RAMs, and every access to the machine's registers in order.
//
// Sega 32X: a function of the SH-2 program (addresses from $02000000) runs over the SH-2s' map: the cartridge at $02000000, the frame buffer
// at $04000000 (compared), SDRAM at $06000000 holding the program, the 32X's system registers at $4000 (the communication ports read back
// what was written; the others read zero), its VDP registers at $4100 (the frame buffer control register shows the vertical blank and flips
// its frame-swap bit on each read, so a wait for either ends), the palette at $4200, and the SH7604's own registers (the divide unit divides);
// the 32X boot ROM at $0000 is stubs (every word an RTS or NOP). A function of the 68000 side (addresses below $01000000) runs in the 68000
// interpreter over the Genesis's memory map with the adapter's registers at $A15100 and the VDP logged.
//
// Saturn: a function of IP.BIN or the first program runs over the Saturn's map: the high work RAM at $06000000 holding both programs, the
// low work RAM at $00200000, the VDP1's and VDP2's video RAM and the colour RAM, the sound RAM (all compared), the BIOS at $0000 as stubs, and
// the SMPC, the SCU, the VDP1 and VDP2 registers, the CD block, the SCSP registers and the A-bus as logged devices: the VDP2 status register
// shows the vertical blank and drops it again, the SCU's interrupt status shows the V-blank in and out, the VDP1 reports its drawing done,
// the SMPC is ready, the CD block has every flag set.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

// What the lanes are made from (exposed for tests; the table calls the same functions).
verify::lanes::ImageLaneSpec sega32x_lane_spec();
verify::lanes::ImageLaneSpec saturn_lane_spec();

// Add "sega32x" and "saturn" to the machine lane table. Safe to call twice.
void register_sh2_lanes();

}  // namespace cb::engine
