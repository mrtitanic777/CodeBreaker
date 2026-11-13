// codebreaker/verify/gate16/isolated16.hpp -- COMPAT SHIM over cb/verify/gate16/isolated16.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/gate16/isolated16.{hpp,cpp}: `Isolated16` runs each function of a rebuilt 16-bit module beside the original's on its own, over
// the surroundings the lane kit (engine/lane_kit) makes -- the runner's models and argument sizes, the p-code lifters, the tables of the original's decompiled
// module. This header keeps what the old callers asked of the port: `check`, with its answers as py:: words. No logic.
//
// Not carried over: the worker processes the old port shared the work out over (the shares are threads now), the `_tables` / `_functions` helpers of the
// engine's old 16-bit steps (engine/win16 is native/cb/engine/win16 now: it is handed the tables as a document), `guided_inputs` and `worker_main`
// (nothing in the old layer called them).
#pragma once

#include <optional>

#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/old_lane_kit.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/verify/gate16/isolated16.hpp"
#include "py.hpp"

namespace codebreaker::verify::gate16::isolated16 {

// One result of check(): (name, verdict, why).
struct Checked {
    py::str name;
    py::str verdict;
    py::str why;
};

// Each named function (one the build has as C) beside the original's. `image`: the built module (default: the file of the folder named as the original);
// `chain`: the module's other functions run for real on both sides.
inline py::list<Checked> check(const py::str& original, const py::str& folder, const py::list<py::str>& names,
                               const std::optional<py::str>& image = std::nullopt, bool chain = false) {
    namespace ng = ::cb::verify::gate16;
    std::vector<std::string> wanted;
    for (const py::str& n : names) wanted.push_back(n.utf8());
    ng::Isolated16Options options;
    if (image) options.image = ::cb::compat::legacy::path_of(*image);
    options.chain = chain;
    ::cb::engine::LaneKit& kit = ::cb::compat::old_lane_kit();
    const ng::Isolated16 gate(kit.isolated16_spec());
    const std::vector<ng::Isolated16Answer> answers = ::cb::compat::guard([&] {
        return gate.check(::cb::compat::legacy::path_of(original), ::cb::compat::legacy::path_of(folder), wanted, options);
    });
    py::list<Checked> out;
    for (const ng::Isolated16Answer& a : answers) out.append(Checked{py::str(a.name), py::str(std::string(::cb::verify::word(a.verdict))), py::str(a.why)});
    return out;
}

}  // namespace codebreaker::verify::gate16::isolated16
