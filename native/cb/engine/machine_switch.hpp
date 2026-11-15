// cb/engine/machine_switch.hpp -- the Nintendo Switch lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_switch_lane();      // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// A homebrew `.nro`, a `.nso` or the AArch64 ELF devkitA64 links (formats/nro) is written back as GNU as source (source/switch_writer) and rebuilt; one that comes back
// identical is the original, an edited one has each function run beside the original's in CodeBreaker's AArch64 interpreter (emulate/a64). The memory is the process's as the
// homebrew loader leaves it: the segments at $08000000 (the dynamic R_AARCH64_RELATIVE relocations applied, the bss zero), a stack, the thread-local storage that
// TPIDRRO_EL0 and TPIDR_EL0 point at, the loader's configuration entries, and a work area of 64 KB the starting states hand out pointers into. A function is called with x0-x3 as the
// state gives them (the first state is what the loader gives `_start`: x0 the configuration entries, x1 = -1), x19 and the work area seeded from `fill`. A `svc` is a Horizon
// kernel call: logged by its name and x0-x3 (and, for SendSyncRequest, the IPC command and the service it names), answered by emulate/a64's default handler; ExitProcess,
// ExitThread and Break end the run. The comparison is of how a function ended, x0, x1, x19-x28, SP, the memory it changed and the kernel calls.
//
// A rebuilt NSO or ELF is the loaded image alone (switch_writer says so), not a file that parses; the lane then runs it as one flat region (everything writable).
#pragma once

#include "cb/verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec switch_lane_spec();

// Add "switch" to the machine lane table. Safe to call twice.
void register_switch_lane();

}  // namespace cb::engine
