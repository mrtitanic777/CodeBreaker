// cb/engine/machine_threedo.hpp -- the 3DO lane, added to the machine lane table (engine/machine_lanes.hpp) without touching the lane kit.
//
//     engine::register_threedo_lane();      // once, before a LaneKit is made (LaneKit::register_worker_kinds does)
//
// A 3DO disc image (the program is its LaunchMe, an AIF) or a bare AIF (formats/threedo) is written back as GNU as source (source/threedo_writer) and
// rebuilt; one that comes back identical is the original, an edited one has each function run beside the original's in CodeBreaker's ARM interpreter
// (emulate/arm) as a big-endian processor (`Bus::set_big_endian`: the memory holds each word reversed, a byte or halfword access goes to its lane).
// The map is the 3DO's as far as a program reaches it: 2 MB of DRAM from address 0 here is 4 MB of the interpreter's main RAM at $02000000, the
// image loaded there (it is position-independent until it relocates itself; a function's address is its offset in the image) and, as logged devices,
// Madam ($03300000) and Clio ($03400000), whose vertical counter moves on each read. Software interrupts are the operating system's calls (the folios),
// logged by number with R0-R3. The comparison is of how a function ended, R0 R1 R4-R11 SP, the main RAM it changed, every register access in order and
// the operating-system calls. The processor's 26-bit mode (the flags in the pc) is not modelled.
#pragma once

#include "cb/verify/lanes/image_lane.hpp"

namespace cb::engine {

verify::lanes::ImageLaneSpec threedo_lane_spec();

// Add "threedo" to the machine lane table. Safe to call twice.
void register_threedo_lane();

}  // namespace cb::engine
