// cb/engine/machine_chf.hpp -- the Channel F lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_chf_lane();     // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// A cartridge (formats/chf) is written back as F8 source (source/chf_writer) and rebuilt; one that comes back identical is the original, an edited one has each function run beside the
// original's in the F8 interpreter (emulate/f8) over the console's memory: the cartridge ROM at $0800 (writes dropped), 2 KB of cartridge RAM at $2800-$2FFF (state), and the BIOS at
// $0000-$07FF as stubs (every byte a POP, so a PI into it returns; each entry is noted by the routine's name or address: BIOS_DRAWCHAR, BIOS_0123 ...). Every IN, OUT, INS and OUTS is a
// logged port access (PORT_0 ..); the ports read $FF (no key down). A function is called with A and R0..R3 of the state and PC1 holding the return address; the comparison is of how it
// ended, A, W, ISAR, DC0, DC1, the scratchpad, the cartridge RAM, every port access in order and the BIOS entries.
#pragma once

#include "cb/verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec chf_lane_spec();

// Add "chf" to the machine lane table. Safe to call twice.
void register_chf_lane();

}  // namespace cb::engine
