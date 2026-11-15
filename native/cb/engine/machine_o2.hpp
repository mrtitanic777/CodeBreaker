// cb/engine/machine_o2.hpp -- the Odyssey 2 lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_o2_lane();     // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// A cartridge (formats/o2) is written back as 8048 source (source/o2_writer) and rebuilt; one that comes back identical is the original, an edited one has each function run beside
// the original's in the 8048 interpreter (emulate/i8048) over the console's memory: the cartridge at $0400 of the program memory, the BIOS at $0000-$03FF as stubs (every byte a RET,
// each entry noted by its address: BIOS_01B0 ...), 64 bytes of internal RAM, and the data space (MOVX, the video chip's registers and the cartridge's RAM, 256 bytes that hold what is
// written; VDC_STATUS reads as an idle display, 0 and $FF alternately). Every port, expander, bus and data access is a logged device access named by what it touches (P1, P2, BUS,
// DBB, P4..P7, T0, T1, INT, VDC_CONTROL ...). No key is down (the ports read $FF), T0 reads 0 and T1 alternates, so a loop that waits for the video chip's flag ends. A function is
// called with A and R0..R3 of the state; the comparison is of how it ended, A, PSW, the registers and RAM, the port latches, the data space and every access in order.
#pragma once

#include "cb/verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec o2_lane_spec();

// Add "o2" to the machine lane table. Safe to call twice.
void register_o2_lane();

}  // namespace cb::engine
