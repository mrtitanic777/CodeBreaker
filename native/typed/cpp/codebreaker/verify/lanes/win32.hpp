// codebreaker/verify/lanes/win32.hpp -- COMPAT SHIM over cb/verify/lanes/win32.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/lanes/win32*.{hpp,cpp}: the 32-bit Windows lane (`Win32Lane`, a Kind the reconstruction path is given, filed under "pe": the
// module decompiled and rebuilt with OpenWatcom, each function run beside the original in the emulator and compared), over the surroundings the lane kit
// (engine/lane_kit) makes. Nothing in the old layer judges with the old port any more, so this header keeps only what its callers ask of it:
//
//   * the kind's name, per-function verdicts and the revision a verdict means;
//   * `functions` (win32_functions.hpp), the profile the lane's verdicts are filed under, the key of a function ("%x" of its address) and `prepare` --
//     the module decompiled and made compilable as the old `reconstruct::Prepared` (what verify/authored/repair's glue reads: the texts, the order, the
//     keys, and the old Module32 as its `lane`).
//
// `prepare` is the old decompiler's (decompile/decompiler.hpp), which is a shim over the new one, with the signature the old lane gave the module.
#pragma once

#include <cstdint>
#include <memory>
#include <tuple>

#include "cb/compat/old_lane_kit.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/verify/lanes/win32.hpp"
#include "codebreaker/decompile/c_text.hpp"
#include "codebreaker/decompile/decompiler.hpp"
#include "codebreaker/verify/lanes/win32_functions.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "py.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::verify::lanes::win32 {

inline const py::str KIND = py::str("pe");
inline constexpr bool PER_FUNCTION = true;   // a verdict holds while the function and what it reaches are unchanged
inline constexpr int64_t REV = cb::verify::lanes::Win32Lane::kRev;

// the arguments every function is given (its real count is not known)
inline int64_t ARITY() { return static_cast<int64_t>(cb::verify::lanes::Win32Lane::kArity); }

// (digest, settings) the lane's verdicts are filed under (everything but a function's source that a verdict depends on).
inline std::tuple<py::str, py::str> settings_profile(const py::str& lane) {
    (void)lane;   // ("pe")
    const std::shared_ptr<const cb::verify::lanes::Kind> kind = cb::compat::old_lane_kit().win32_lane();
    if (!kind) throw py::RuntimeError("the 32-bit lane is not set up (settings.json: openwatcom)");
    const cb::verify::lanes::Profile p = cb::compat::guard([&] { return kind->profile({}); });
    return {py::str(p.digest), py::str(p.settings)};
}

class PELane {
  public:
    py::list<int64_t> functions(const py::str& path) const { return win32::functions(KIND, path); }
    py::str key(int64_t va) const { return py::percent("%x", va); }
};

inline py::dict<py::str, PELane> LANES = [] {
    py::dict<py::str, PELane> d;
    d.set(KIND, PELane());
    return d;
}();

// The module decompiled and made compilable, as the old lane prepared it.
inline reconstruct::Prepared prepare(const py::str& module_path, const py::str& work) {
    const PELane& lane = LANES.at(KIND);
    const py::list<int64_t> addrs = lane.functions(module_path);
    auto m = std::make_shared<decompile::c_text::Module32>(decompile::decompiler::decompile(module_path, addrs, work));
    reconstruct::Prepared out;
    out.header = m->header();
    out.texts = m->texts();
    out.order = m->order();
    for (const auto& kv : m->by_va().items()) out.key.set(std::get<1>(kv), lane.key(std::get<0>(kv)));
    for (int64_t va : addrs) out.all_keys.append(lane.key(va));
    // The module is one translation unit: any change to it may change any function's build.
    py::str body("");
    for (const py::str& n : out.order) body = body + out.texts.at(n);
    const py::str nul(std::u32string(1, U'\0'));
    out.sig = py::hashlib::sha1((py::percent("%s", py::to_str(decompile::decompiler::REV)) + nul + out.header + nul + body).encode("utf-8", "replace")).hexdigest();
    out.lane = m;
    return out;
}

}  // namespace codebreaker::verify::lanes::win32
