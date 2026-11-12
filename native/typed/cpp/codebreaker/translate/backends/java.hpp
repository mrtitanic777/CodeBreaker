// codebreaker/translate/backends/java.hpp -- COMPAT SHIM over cb/translate/backends/java.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/backends/java.{hpp,cpp} (the faithful tier, the pointer analysis and the program), with the
// idiomatic tier in java_objects and the C library mapping in java_libc. This header keeps the one entry point the old layer
// calls:
//
//     prog = java::program(units, "Hello", tier)     // old IR units -> Program: .source (Java text), .tier, .notes
//
// An old IR tree is written as its L2 document and read back as the typed tree (cb/compat/old_l2_tree.hpp is the old tree's codec),
// the program is written by the new module with the idiomatic tier and the library mapping over the API-mapping registry, and a
// refusal comes back as the old `Refused`. The old module's internals (Faithful, FnWriter, the expression and precedence helpers)
// were the sibling ports' (java_objects, java_libc), which are refactored with it; nothing else used them.
#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "cb/compat/old_val_json.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/arena.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/apimap.hpp"
#include "cb/translate/backends/java.hpp"
#include "cb/translate/backends/java_libc.hpp"
#include "cb/translate/backends/java_objects.hpp"
#include "cb/translate/l2.hpp"
#include "codebreaker/java/_paths.hpp"
#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/l2.hpp"
#include "py.hpp"

namespace codebreaker::translate::backends::java {

namespace ir = codebreaker::translate::ir;
using ir::NodeP;

// The back end does not write this program (the reason says why).
struct Refused : py::Exception {
    explicit Refused(const py::str& m) : py::Exception(m) {}
    explicit Refused(const char* m) : py::Exception(m) {}
    const char* type() const override { return "Refused"; }
};

struct Program {
    py::str name;
    py::str source;
    py::str tier;
    py::list<py::str> notes;
    std::optional<py::str> refused;

    Program(const py::str& name, const py::str& source, const py::str& tier, const py::list<py::str>& notes,
            const std::optional<py::str>& refused = std::nullopt)
        : name(name), source(source), tier(tier), notes(notes), refused(refused) {}
};

namespace detail_shim {
// The C library mapping's registry: the bundled api_maps/ folder, read once (as the old module's cache did).
inline const cb::java::apimap::Registry& libc_registry() {
    static const cb::java::apimap::Registry registry = cb::java::apimap::Registry::load(
        cb::fs::from_text(cb::compat::from_py(codebreaker::java::_paths::data_path(py::list<py::str>{py::str("api_maps")}))));
    return registry;
}
}  // namespace detail_shim

// L2 units of one C/C++ program -> Program (Java source of class `name`). Raises Refused.
// tier: 'auto' (idiomatic where the analysis allows it, else faithful), 'idiomatic' or 'faithful'.
inline Program program(const py::list<NodeP>& units, const py::str& name, const py::str& tier = py::str("auto")) {
    namespace nj = cb::translate::backends::java;
    nj::Tier chosen = nj::Tier::Auto;
    if (tier == py::str("idiomatic")) chosen = nj::Tier::Idiomatic;
    else if (tier == py::str("faithful")) chosen = nj::Tier::Faithful;
    else if (tier != py::str("auto")) throw py::ValueError(py::str("unknown tier ") + tier);
    return cb::compat::guard([&] {
        cb::Arena arena;
        std::vector<cb::translate::ir::Node*> nodes;
        for (const NodeP& u : units) {
            nodes.push_back(cb::translate::l2::from_json(arena, cb::compat::to_json_value(codebreaker::translate::l2::to_json(u))));
        }
        static const cb::translate::backends::java_libc::LibC library(detail_shim::libc_registry());
        try {
            const nj::Program p = nj::program(
                nodes, cb::compat::from_py(name),
                {.tier = chosen, .idiomatic = cb::translate::backends::java_objects::factory(&library), .library = &library});
            return Program(cb::compat::to_py(p.name), cb::compat::to_py(p.source), cb::compat::to_py(nj::to_text(p.tier)),
                           cb::compat::to_py(p.notes));
        } catch (const nj::Refused& r) {
            throw Refused(r.what());
        }
    });
}

}  // namespace codebreaker::translate::backends::java
