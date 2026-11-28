// engine/machine_ps2.hpp -- the PlayStation 2 lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_ps2_lane();      // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// The lane is one verify/lanes/image_lane: an Emotion Engine ELF (`.elf`), or a disc image (`.iso`) whose BOOT2 program is one, is written back by
// source/console_writer (config/platforms/ps2.json) and rebuilt; one that comes back identical is the original, an edited one has each function run beside the original's in
// CodeBreaker's R5900 interpreter (emulate/ee) over the console's memory: 32 MB of RAM holding the ELF's segments, the scratchpad, the hardware registers
// logged by name, the kernel's system calls logged. A function is run with the registers of the state ($a0-$a3 and $s0 sign-extended into their 64 bits) and an
// 8 KB work area of RAM seeded from the state's `fill`; the comparison is of how it ended, $v0 and $v1 (and their upper 64 bits), $s0-$s7, $sp, HI and LO, the
// FPU's $f0, the RAM and scratchpad it changed, every register access in order, and every system call. The GS, DMA controller, VIFs, IPU and the vector units
// are not run: a function that starts a transfer is compared on what it wrote to their registers and to memory.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

// What the lane is made from (exposed for tests; the table calls the same function).
verify::lanes::ImageLaneSpec ps2_lane_spec();

// Add "ps2" to the machine lane table: R5900 `.elf` programs and `.iso` discs. Safe to call twice.
void register_ps2_lane();

}  // namespace cb::engine
