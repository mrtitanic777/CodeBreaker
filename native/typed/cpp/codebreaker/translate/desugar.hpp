// codebreaker/translate/desugar.hpp -- COMPAT SHIM over cb/translate/desugar.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/desugar.{hpp,cpp}: the lowering pass over the typed tree. This header keeps the old
// call for the back ends that still hold the old IR tree (py::ref nodes): the units cross to the typed tree as their L2
// documents, the pass runs there, and the lowered units are written back into the old nodes (each Unit keeps its identity, so
// whoever holds the list sees the result), as the Python's `run(units, keep)` rewrote them in place. `keep` names the forms the
// back end writes itself, as before.
#pragma once

#include <cstdint>
#include <vector>

#include "cb/compat/old_val_json.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/translate/desugar.hpp"
#include "cb/translate/l2.hpp"
#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/l2.hpp"
#include "py.hpp"

namespace codebreaker::translate::desugar {

namespace ir = codebreaker::translate::ir;

inline const py::list<py::str> FORMS = {py::str("foreach"), py::str("switchexpr"), py::str("stringswitch"), py::str("pattern")};

// Lower the modern forms in every method of `units` (in place). -> how many were lowered.
inline int64_t run(const py::list<ir::NodeP>& units, const py::list<py::str>& keep = py::list<py::str>()) {
    namespace newl2 = cb::translate::l2;
    namespace nd = cb::translate::desugar;
    return cb::compat::guard([&]() -> int64_t {
        std::vector<nd::Form> kept;
        for (const py::str& name : keep) {
            const std::string n = name.utf8();
            if (n == "foreach") kept.push_back(nd::Form::ForEach);
            else if (n == "switchexpr") kept.push_back(nd::Form::SwitchExpr);
            else if (n == "stringswitch") kept.push_back(nd::Form::StringSwitch);
            else if (n == "pattern") kept.push_back(nd::Form::Pattern);
        }
        cb::Arena arena;
        std::vector<cb::translate::ir::Node*> nodes;
        for (const ir::NodeP& u : units) nodes.push_back(newl2::from_json(arena, cb::compat::to_json_value(l2::to_json(u))));
        const std::size_t lowered = nd::run(arena, nodes, kept);
        if (lowered == 0) return 0;   // nothing changed: the old tree stays as it is
        for (std::size_t i = 0; i < nodes.size(); ++i) {
            const auto old_unit = std::dynamic_pointer_cast<ir::Unit>(units[static_cast<int64_t>(i)]);
            const auto fresh = std::dynamic_pointer_cast<ir::Unit>(l2::from_json(std::get<ir::Dict>(cb::compat::from_json_value(newl2::to_json(*nodes[i])).v)));
            if (old_unit && fresh) *old_unit = *fresh;
        }
        return static_cast<int64_t>(lowered);
    });
}

}  // namespace codebreaker::translate::desugar
