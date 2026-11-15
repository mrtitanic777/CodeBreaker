// cb/engine/machine_ngp.hpp -- the Neo Geo Pocket lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_ngp_lane();     // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// A cartridge (formats/ngp) is written back as TLCS-900/H source (source/ngp_writer) and rebuilt; one that comes back identical is the original, an edited one has each function run
// beside the original's in the TLCS-900/H interpreter (emulate/tlcs900) over the console's memory (24 address lines): the CPU's own I/O at $000000-$0000FF (a logged device: timers,
// watchdog, the Z80 mailbox), 12 KB of work RAM at $004000, the Z80's 4 KB at $007000, the K2GE's registers at $008000-$0081FF (a logged device that gives back what was written, except
// that the raster line K2GE_RAS_V and the status register K2GE_STATUS read as a display that is drawing then in VBlank on every second read, so a loop that waits for a frame ends),
// its palette, sprite, tile and map memory at $008200-$00BFFF (logged), the cartridge at $200000 (repeated over its 2 MB, a write to it is dropped) and the BIOS ROM at
// $FF0000-$FFFFFF: every byte `ret`, with the BIOS's table of function addresses at $FFFE00 pointing at one `ret` each, so a call through the table comes back, and is logged by
// vector number. `swi n` is stubbed and logged too (the number, the function number in RW3 and the registers). A function is called with XWA, XBC, XDE, XHL of the state, the four
// numbers pushed as words on the stack (the first at (XSP+4)), register bank 3 current, and the stack at $006C00; the comparison is of how it ended, every register of every bank, SR,
// work RAM, the Z80's RAM, every access to the I/O, the K2GE and its memory in order, and the BIOS calls.
#pragma once

#include "cb/verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec ngp_lane_spec();

// Add "ngp" to the machine lane table. Safe to call twice.
void register_ngp_lane();

}  // namespace cb::engine
