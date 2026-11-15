// cb/engine/machine_atari.cpp -- the Atari machines' lanes, added to the machine lane table together (engine/machine_lanes.hpp).
#include "cb/engine/machine_atari.hpp"

#include "cb/engine/machine_atari2600.hpp"
#include "cb/engine/machine_atari5200.hpp"
#include "cb/engine/machine_atari7800.hpp"
#include "cb/engine/machine_lynx.hpp"

namespace cb::engine {

void register_atari_lanes() {
    register_atari2600_lane();
    register_atari5200_lane();
    register_atari7800_lane();
    register_lynx_lane();
}

}  // namespace cb::engine
