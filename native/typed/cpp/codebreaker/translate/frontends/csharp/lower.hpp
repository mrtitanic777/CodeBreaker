// codebreaker/translate/frontends/csharp/lower.hpp -- COMPAT SHIM over cb/translate/frontends/csharp/lower.hpp
// (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/frontends/csharp/lower.{hpp,cpp}: Roslyn's bound tree (CsTreeDump's JSON) into the typed
// IR. This header keeps the old calls for the ports that still hold the old IR tree (py::ref nodes) and the dump as an
// ir::Dict: the dict crosses to JSON, the new module lowers it, and the unit it makes is read back into old nodes through its
// L2 document. Only what callers use is kept (load_tree, lower, lower_with_refusals); the old Lowering class and its helpers
// were the Python's internals.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/old_l2_tree.hpp"
#include "cb/compat/old_val_json.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/json.hpp"
#include "cb/translate/frontends/csharp/lower.hpp"
#include "cb/translate/l2.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::frontends::csharp::lower {

namespace ir = codebreaker::translate::ir;
using ir::Dict;

inline const py::str LANG = py::str("csharp");

// A dump's text (one file's CsTreeDump JSON) as a node Dict. ValueError for text that is not JSON.
inline Dict load_tree(const py::str& text) {
    return cb::compat::guard([&] { return std::get<Dict>(cb::compat::from_json_value(cb::json::parse(text.utf8())).v); });
}

// -> (ir.Unit, [(line, kind, why)]): what became Unsupported, and why.
inline std::tuple<py::ref<ir::Unit>, py::list<std::tuple<int64_t, std::optional<py::str>, py::str>>> lower_with_refusals(
    const Dict& tree, const std::optional<py::str>& path = std::nullopt) {
    return cb::compat::guard([&] {
        namespace nf = cb::translate::frontends::csharp;
        cb::Arena arena;
        const nf::Lowered lowered = nf::lower(arena, cb::compat::to_json_value(tree), cb::compat::from_py(path));
        const Dict doc = std::get<Dict>(cb::compat::from_json_value(cb::translate::l2::to_json(*lowered.unit)).v);
        const auto unit = std::dynamic_pointer_cast<ir::Unit>(codebreaker::translate::l2::from_json(doc));
        if (!unit) throw py::TypeError("the lowered unit is not a Unit");
        py::list<std::tuple<int64_t, std::optional<py::str>, py::str>> refused;
        for (const nf::Refusal& r : lowered.refused) {
            refused.append(std::make_tuple(static_cast<int64_t>(r.line), cb::compat::to_py(r.kind), cb::compat::to_py(r.why)));
        }
        return std::make_tuple(unit, refused);
    });
}

// One file's CsTreeDump JSON -> ir.Unit.
inline py::ref<ir::Unit> lower(const Dict& tree, const std::optional<py::str>& path = std::nullopt) {
    return std::get<0>(lower_with_refusals(tree, path));
}

}  // namespace codebreaker::translate::frontends::csharp::lower
