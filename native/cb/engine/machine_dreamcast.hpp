// cb/engine/machine_dreamcast.hpp -- the Sega Dreamcast lane (SH-4), added to the machine lane table (engine/machine_lanes.hpp) without touching
// the lane kit.
//
//     engine::register_dreamcast_lane();      // once, before a LaneKit is made (LaneKit::register_worker_kinds does it)
//
// One verify/lanes/image_lane: the disc's IP.BIN and first program are written back as source (source/dreamcast_writer) and rebuilt; one that
// comes back identical is the original, an edited one has each function run beside the original's in CodeBreaker's own SH-4 interpreter
// (emulate/sh2 as Variant::Sh4, with emulate/sh4's FPU) over the machine's memory. The comparison is of how the run ended, R0 R1 and FR0 (the
// results), R8-R15 and FR12-FR15 (what a caller keeps), PR GBR MACH MACL SR FPSCR and FPUL, the memory it changed, and every access to the
// machine's registers, every BIOS call and every cache operation in order.
//
// A function (addresses are physical: $0C008300 is IP.BIN's bootstrap, $0C010000 the first program) runs over the Dreamcast's map:
//   * main RAM, 16 MB at $0C000000, holding IP.BIN and the program where the machine loads them, the BIOS's system-call vectors at $0C0000B0 (system
//     information, ROM font, flash ROM, GD-ROM and the menu: each points at a stub of the BIOS ROM that returns 0, and every entry of a stub is
//     logged with its selector); the stack is at its top;
//   * the PowerVR's video memory (8 MB at $04000000), the AICA's wave RAM (2 MB at $00800000), the BIOS ROM and flash at $00000000 and $00200000 (stubs);
//   * the Holly's registers at $005F6800-$005F9FFF (the system ASIC, the Maple bus, the G1 bus with the GD-ROM's ATA registers, the G2 bus, the
//     PowerVR2's core, display and tile accelerator), the AICA's channels and common registers at $00700000, its clock at $00710000, the tile
//     accelerator's FIFOs at $10000000, and the G2 external area at $01000000, all logged. The ones a program waits on answer the way a machine in
//     the middle of a frame does: the PowerVR's ID and revision, SPG_STATUS with the vertical blank coming and going, the system interrupt status
//     showing the V-blank in and out, the GD-ROM ready, the Maple DMA done;
//   * the SH-4's own registers ($FF000000 up: the cache, MMU, bus controller, DMAC, timers, serial ports) hold what is written and are logged, the
//     store queues ($E0000000) flush to the address QACR0/QACR1 and the low bits give when a PREF names them.
#pragma once

#include "cb/verify/lanes/image_lane.hpp"

namespace cb::engine {

// What the lane is made from (exposed for tests; the table calls the same function).
verify::lanes::ImageLaneSpec dreamcast_lane_spec();

// Add "dreamcast" to the machine lane table. Safe to call twice.
void register_dreamcast_lane();

}  // namespace cb::engine
