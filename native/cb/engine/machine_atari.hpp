// cb/engine/machine_atari.hpp -- one call that adds the Atari 2600, 5200, 7800, Lynx and Jaguar lanes to the machine lane table. LaneKit::register_worker_kinds
// makes it, so the host and its worker processes have the lanes before a LaneKit is made; a program that makes a LaneKit without them
// (a test) calls it itself. Safe to call more than once.
#pragma once

namespace cb::engine {

void register_atari_lanes();

}  // namespace cb::engine
