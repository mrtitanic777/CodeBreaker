// codebreaker/verify/lanes/win16.hpp -- COMPAT SHIM over cb/verify/lanes/win16.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/lanes/win16.{hpp,cpp}: the 16-bit Windows lane (`Win16Lane`, a Kind the reconstruction path is given, filed under "ne"), over
// the surroundings the lane kit (engine/lane_kit) makes. Nothing in the old layer calls the old port's judge, its inputs or its build any more (the engine's
// 16-bit steps, the gate and the module builder are native/cb now), so this header keeps only what the old callers could name without a lane to ask: the kind's
// name and the revision a verdict means. No logic.
#pragma once

#include <cstdint>

#include "cb/verify/lanes/win16.hpp"
#include "py.hpp"

namespace codebreaker::verify::lanes::win16 {

inline const py::str KIND = py::str("ne");
inline constexpr bool PER_FUNCTION = true;   // a verdict holds while the function and what it reaches are unchanged
inline constexpr int64_t REV = cb::verify::lanes::Win16Lane::kRev;

}  // namespace codebreaker::verify::lanes::win16
