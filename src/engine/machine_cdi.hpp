// engine/machine_cdi.hpp -- the Philips CD-i lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_cdi_lane();     // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// A disc image or module file (formats/cdi) is written back as Motorola-syntax source (source/cdi_writer), module by module, and rebuilt; one that
// comes back identical is the original, an edited one has each function run beside the original's in the 68000 interpreter (emulate/m68k) over the
// player's memory: the executable modules where the model places them (from $10000), free RAM at $100000-$1FFFFF (the data pointer A6 starts at $108000, the
// stack at $1FFF00), and as logged devices the MCD212 video chip and the slave processor ($200000-$4FFFFF). CD-RTOS is not run: every `TRAP #0` and the call
// word after it is made a call to a stub that logs the call number (F_Exit, I_Write ...) and returns, so what a function asks of the system is part of
// what is compared, with the registers (D0-D7, A0-A6), the condition codes, the RAM it changed and every device access in order.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec cdi_lane_spec();

// Add "cdi" to the machine lane table. Safe to call twice.
void register_cdi_lane();

}  // namespace cb::engine
