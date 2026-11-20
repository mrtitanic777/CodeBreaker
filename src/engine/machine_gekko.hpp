// engine/machine_gekko.hpp -- the Nintendo GameCube, Wii and Wii U lanes, added to the machine lane table (engine/machine_lanes.hpp) without
// touching the lane kit.
//
//     engine::register_gekko_lanes();      // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// Each lane is one verify/lanes/image_lane: a program (a DOL, an ELF, an RPX) is written back by source/gekko_writer and rebuilt; one that comes
// back identical is the original, an edited one has each function run beside the original's in CodeBreaker's own PowerPC interpreter
// (emulate/powerpc) with Gekko's paired singles on, over the program as the loader leaves it. A function starts with the argument registers
// of the state (r3-r10, f1-f3), the small data area bases the program's start-up code sets, and a work area at 0x60000000 filled the way the
// state says; the comparison is of how it ended, r3, r4, f1 and its second half, r1 and the registers a function must keep, the memory it
// changed, and every access to a hardware register in order (formats/gekko_map: the GameCube's command processor, video, disc, serial, external
// and audio blocks, the Wii's mailbox and Hollywood, the Wii U's Latte window) -- named and logged, not emulated.
#pragma once

#include "verify/lanes/image_lane.hpp"

namespace cb::engine {

// What a lane is made from (exposed for tests; the table calls the same function): `console` is "gamecube", "wii" or "wiiu".
verify::lanes::ImageLaneSpec gekko_lane_spec(std::string_view console);

// Add "gamecube", "wii" and "wiiu" to the machine lane table. Safe to call twice.
void register_gekko_lanes();

}  // namespace cb::engine
