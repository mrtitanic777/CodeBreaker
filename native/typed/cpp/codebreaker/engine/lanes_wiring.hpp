// codebreaker/engine/lanes_wiring.hpp -- COMPAT SHIM over cb/engine/lanes_wiring.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/engine/lanes_wiring.{hpp,cpp}: the platforms that have a lane, handed to the engine's steps. What the old port also did -- fill the
// old reconstruct registry with the "ne" and "pe" lanes and install the hooks that sent a module to the engine's steps -- is the lane kit's now
// (engine/lane_kit: its lanes are the reconstruction path's Kinds, and verify/reconstruct's shim asks it which lane a file is). Nothing in the old layer
// calls the old port, so this header keeps nothing. No logic.
#pragma once

#include "cb/engine/lanes_wiring.hpp"
