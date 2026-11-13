// codebreaker/verify/lanes/driver.hpp -- COMPAT SHIM over cb/verify/lanes/driver.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/lanes/driver.{hpp,cpp} now: the driver lane (judging a rebuilt virtual device driver entry point by entry point in
// the 32-bit ring-0 emulator), a `Kind` the caller builds over the platform's packager, the driver builder and the names of the services. Nothing in the
// old layer called the port (the module is reached by name from `__main__`'s command table and by the Python), so this header keeps only what the
// old callers could name without a lane to ask: the kind's name and the revision a verdict means. No logic.
#pragma once

#include <cstdint>

#include "cb/verify/lanes/driver.hpp"
#include "py.hpp"

namespace codebreaker::verify::lanes::driver {

inline const py::str KIND = py::str("le");
inline constexpr int64_t REV = cb::verify::lanes::DriverLane::kRev;

}  // namespace codebreaker::verify::lanes::driver
