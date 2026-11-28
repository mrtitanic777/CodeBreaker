// engine/machine_vb.hpp -- the Virtual Boy lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_vb_lane();     // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// A cartridge (formats/vb) is written back as V810 source (source/console_writer, config/platforms/vb.json) and rebuilt; one that comes back identical is the original, an edited one has each function run
// beside the original's in the V810 interpreter (emulate/v810) over the console's memory (27 address lines): the VIP's video memory at $00000000 and its character memory at
// $00078000 (logged), the VIP registers at $0005F800, the VSU at $01000000 and the hardware control registers at $02000000 (all logged devices, each access named: VIP_DPCTRL,
// VSU_S1FQL, HW_SCR ...), 64 KB of work RAM at $05000000 and 8 KB of cartridge RAM at $06000000 (both repeated over their 16 MB), and the cartridge ROM at $07000000 (repeated;
// a write to it is dropped). The VIP answers as an idle display: its interrupt-pending, display-status and drawing-status registers read all ones on every second read and
// zero between, so a loop that waits for a frame or for the drawing to finish ends; the control registers read 0 (no button is down). A function is called with the
// registers of the state (r6..r9, the arguments of the C calling convention) and the stack at the top of work RAM; the comparison is of how it ended, r1-r31, PSW,
// work RAM and cartridge RAM, and every access to the video memory, the VIP, the VSU and the hardware registers in order.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec vb_lane_spec();

// Add "vb" to the machine lane table. Safe to call twice.
void register_vb_lane();

}  // namespace cb::engine
