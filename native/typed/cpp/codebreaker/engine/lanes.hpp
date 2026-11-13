// codebreaker/engine/lanes.hpp -- COMPAT SHIM over cb/engine/lanes.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/engine/lanes.{hpp,cpp}: every lane's reconstruct-and-judge as steps of the engine, and the 32-bit module build, over the typed Kind.
// The old port was reached only by the old module builder and the old reconstruction path, which hand a lane's work to the lane kit (engine/lane_kit)
// now, so nothing in the old layer calls it and this header keeps nothing. No logic.
#pragma once

#include "cb/engine/lanes.hpp"
