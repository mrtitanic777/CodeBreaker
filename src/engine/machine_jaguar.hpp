// engine/machine_jaguar.hpp -- the Atari Jaguar lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_jaguar_lane();     // once, before a LaneKit is made (register_atari_lanes() does it with the other Atari machines)
//
// A program (formats/jaguar: a cartridge at $800000, or a COFF executable's sections) is written back as Motorola-syntax source
// (source/console_writer, config/platforms/jaguar.json) and rebuilt; one that comes back identical is the original, an edited one has each function run beside the original's in the
// 68000 interpreter (emulate/m68k) over the Jaguar's memory: 2 MB of DRAM at $000000, the program where it loads, the GPU's RAM at $F03000 and the
// DSP's at $F1B000 as plain memory, and the rest of Tom's registers ($F00000-$F0FFFF) and Jerry's ($F10000-$F1FFFF) as logged devices. The chips answer
// as an idle console: the raster counters (HC, VC) count on each read so a wait for a line ends, the controllers read no button down ($FFFF), every
// other register reads 0. The comparison is of how a function ended, D0-D7, A0-A6, the condition codes, the DRAM it changed (the first 128 KB and
// the top 64 KB, where stacks live), the GPU's and DSP's RAM, and every device access in order. The GPU and DSP programs are not run.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec jaguar_lane_spec();

// Add "jaguar" to the machine lane table. Safe to call twice.
void register_jaguar_lane();

}  // namespace cb::engine
