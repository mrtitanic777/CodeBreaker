// engine/machine_wonderswan.hpp -- the WonderSwan lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_wonderswan_lane();     // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// A cartridge (formats/wonderswan) is written back as NASM source (source/console_writer: config/platforms/wonderswan.json) and rebuilt; one that comes back identical is the original,
// an edited one has each function run beside the original's on Unicorn's x86 engine in 16-bit real mode (the V30MZ is an 80186-class core) over the
// console's memory: 1 MB, the cartridge at the top (formats/wonderswan: the ROM ends at $FFFFF), internal RAM at $0000-$3FFF ($0000-$FFFF on the Color),
// and the I/O ports as logged devices: every `in` and `out` is an access of the run, named by the ports table of the writer. The ports answer as an idle
// console: the line counter (LINE_CURRENT, $02) moves on each read so a wait for a line ends, no key is down (KEY_SCAN $B5 reads 0), every other port reads 0.
// A function is entered at the code segment its reset jump names (a function outside it gets the segment of its address), DS = ES = SS = 0, SP = $3FF0,
// with a return address pushed; the comparison is of how it ended, AX BX CX DX SI DI BP SP and the flags, the RAM it changed, and every port access in
// order. Interrupts and the sound, video and DMA hardware are not run.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec wonderswan_lane_spec();

// Add "wonderswan" to the machine lane table. Safe to call twice.
void register_wonderswan_lane();

}  // namespace cb::engine
