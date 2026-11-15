// cb/engine/machine_intv.hpp -- the Intellivision lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_intv_lane();     // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// A cartridge (formats/intv) is written back as CP1610 source (source/intv_writer) and rebuilt; one that comes back identical is the original, an edited one has each function run
// beside the original's in the CP1610 interpreter (emulate/cp1610) over the Intellivision's memory (word addresses): the STIC at $0000-$007F and the PSG at $01F0-$01FF as logged
// devices (each access named: STIC_DISPLAY_ENABLE, PSG_MIXER ...), 240 bytes of scratch RAM at $0100, 352 words of system RAM at $0200, the graphics RAM at $3800-$39FF (8 bits wide,
// logged), the cartridge's segments as ROM, and the executive ROM at $1000-$1FFF as stubs: every word a `MOVR R5,R7` (a return through R5), each entry into it noted by its
// address (EXEC_1020 ...), so what a function asks of the BIOS is part of what is compared. The PSG's controller ports read $FF (no key down), the other registers 0. A function is
// called with R0..R4 of the state and R5 and the stack holding the return address; the comparison is of how it ended, R0-R4, R6, S Z OV C, the scratch and system RAM and every access
// to the STIC, the PSG and the graphics RAM in order with the BIOS entries.
#pragma once

#include "cb/verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec intv_lane_spec();

// Add "intv" to the machine lane table. Safe to call twice.
void register_intv_lane();

}  // namespace cb::engine
