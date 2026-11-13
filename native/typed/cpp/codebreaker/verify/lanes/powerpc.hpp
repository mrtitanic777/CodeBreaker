// codebreaker/verify/lanes/powerpc.hpp -- COMPAT SHIM over cb/verify/lanes/powerpc.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/lanes/powerpc*.{hpp,cpp}: the PowerPC lane (`PowerPcLane`, a Kind the reconstruction path is given, filed under
// "powerpc": the module decompiled and rebuilt with the Xbox 360 compiler, each function run beside the original in the PowerPC interpreter and
// compared), over the surroundings the lane kit (engine/lane_kit) makes. Nothing in the old layer judges with the old port any more, so this header
// keeps only what its callers ask of it: the kind's name, per-function verdicts, the revision a verdict means, and the profile verdicts are filed
// under. The module decompiled and made compilable is the lane's `prepare` (verify/authored/repair's glue asks the kit's lane for it).
#pragma once

#include <cstdint>
#include <memory>
#include <tuple>

#include "cb/compat/old_lane_kit.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/verify/lanes/powerpc.hpp"
#include "py.hpp"

namespace codebreaker::verify::lanes::powerpc {

inline const py::str KIND = py::str("powerpc");
inline constexpr bool PER_FUNCTION = true;   // a verdict holds while the function and what it reaches are unchanged
inline constexpr int64_t REV = cb::verify::lanes::PowerPcLane::kRev;
inline constexpr int64_t M32 = 0xFFFFFFFF;

// the argument registers every function is given: r3-r10
inline int64_t ARGS() { return static_cast<int64_t>(cb::verify::lanes::PowerPcLane::kArguments); }

// (digest, settings) the lane's verdicts are filed under (everything but a function's source that a verdict depends on).
inline std::tuple<py::str, py::str> profile(const py::str& module_path) {
    (void)module_path;
    const std::shared_ptr<const cb::verify::lanes::Kind> kind = cb::compat::old_lane_kit().powerpc_lane();
    if (!kind) throw py::RuntimeError("the PowerPC lane is not set up (settings.json: powerpc_tools, powerpc_lib)");
    const cb::verify::lanes::Profile p = cb::compat::guard([&] { return kind->profile({}); });
    return {py::str(p.digest), py::str(p.settings)};
}

}  // namespace codebreaker::verify::lanes::powerpc
