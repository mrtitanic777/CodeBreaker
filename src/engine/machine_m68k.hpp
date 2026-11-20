// engine/machine_m68k.hpp -- the Neo Geo and Sega CD lanes, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_m68k_lanes();     // once, before a LaneKit is made (LaneKit::register_worker_kinds does it)
//
// Each lane is one verify/lanes/image_lane: the program is written back as Motorola-syntax source (source/neogeo_writer, source/segacd_writer)
// and rebuilt; one that comes back identical is the original, an edited one has each function run beside the original's in the 68000 interpreter
// (emulate/m68k) over the machine's memory, and the comparison is of how it ended, D0-D7, A0-A6, the condition codes, the RAM it changed and
// every access to the machine's devices in order.
//
// Neo Geo: the P-ROM at $000000 (its second megabyte at $200000), work RAM at $100000-$10FFFF, palette RAM at $400000, the memory card at
// $800000 and the system ports at $300000-$3FFFFF as logged devices: the controllers and DIP switches read idle, the LSPC's status counts raster
// lines on each read (so a wait for the vertical blank ends), its VRAM is modelled (address, modulo and data ports) and compared. The BIOS at
// $C00000 is stubs: every word is an RTS, and each of the BIOS's calls (the 6-byte jump table at $C00400) first writes D0 to a logged port, so
// a call to SYSTEM_IO, FIX_CLEAR ... shows in the comparison.
//
// Sega CD: a function of the main CPU's program (entries from $FF0000) runs over the Genesis's memory map with the gate array at $A12000 logged
// (the sub CPU's flag byte alternates on each read, so a handshake loop ends), word RAM at $200000 and the BIOS area ($000000-$01FFFF) as RTSs;
// a function of the sub CPU's program (below $80000) runs over its PRG-RAM with the CD BIOS's jump table at $5F0A-$5F3A stubbed the same way
// (each call logs D0.W), word RAM at $080000, the PCM at $FF0000 and the gate array at $FF8000 as logged devices.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

// What the lanes are made from (exposed for tests; the table calls the same functions).
verify::lanes::ImageLaneSpec neogeo_lane_spec();
verify::lanes::ImageLaneSpec segacd_lane_spec();

// Add "neogeo" and "segacd" to the machine lane table. Safe to call twice.
void register_m68k_lanes();

}  // namespace cb::engine
