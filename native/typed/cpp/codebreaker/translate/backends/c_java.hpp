// codebreaker/translate/backends/c_java.hpp -- COMPAT SHIM over cb/translate/backends/c_java.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
//
// The module is native/cb/translate/backends/c_java*.{hpp,cpp}: the C back end for Java programs, over the typed IR (an arena of nodes). The
// old program and function writers (`Program`, `Method`) do not cross to the old IR tree, so they are gone; what is left is what the old
// callers (the Java judge, translate/javajudge) use: `program` and `build` over a list of old trees (handed to the new module as their L2
// documents, in an arena of its own), the compiler's name and flags, and the runtime's folder. A decline is the old Decline.
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/old_val_json.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/arena.hpp"
#include "cb/core/fs.hpp"
#include "cb/translate/backends/c_java.hpp"
#include "cb/translate/l2.hpp"
#include "codebreaker/translate/backends/cpp03.hpp"
#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/l2.hpp"
#include "py.hpp"

namespace codebreaker::translate::backends::c_java {

namespace ir = codebreaker::translate::ir;
namespace nc = cb::translate::backends::c_java;

using cpp03::Decline;

inline const py::str CLANG = py::str(std::string(nc::kClang));
inline const py::list<py::str> FLAGS = [] {
    py::list<py::str> flags;
    for (const std::string_view f : nc::kFlags) flags.append(py::str(std::string(f)));
    return flags;
}();

// the folder of the C runtime
inline py::str RUNTIME() { return cb::compat::to_py(cb::fs::to_text(nc::default_runtime_dir())); }

namespace shim_detail {

// the old trees as typed nodes (their L2 documents), made in `arena`
inline std::vector<cb::translate::ir::Node*> typed(cb::Arena& arena, const py::list<ir::NodeP>& old_units) {
    std::vector<cb::translate::ir::Node*> out;
    for (const ir::NodeP& u : old_units) out.push_back(cb::translate::l2::from_json(arena, cb::compat::to_json_value(l2::to_json(u))));
    return out;
}

}  // namespace shim_detail

// The program's C text, or raise Decline.
inline py::str program(const py::list<ir::NodeP>& units, const std::optional<py::str>& main = std::nullopt) {
    cb::Arena arena;
    std::vector<cb::translate::ir::Node*> typed_units = shim_detail::typed(arena, units);
    try {
        return cb::compat::to_py(nc::program(arena, typed_units, main ? std::optional<std::string>(main->utf8()) : std::nullopt));
    } catch (const nc::Decline& e) {
        throw Decline(py::str(cb::compat::to_py(std::string(e.what()))));
    }
}

// Write the program as C and compile it with clang (else the host's gcc) against the runtime. -> (exe or None, why)
inline std::tuple<std::optional<py::str>, py::str> build(const py::list<ir::NodeP>& units, const py::str& out) {
    cb::Arena arena;
    std::vector<cb::translate::ir::Node*> typed_units = shim_detail::typed(arena, units);
    const nc::Built built = nc::build(arena, typed_units, cb::fs::from_text(out.utf8()));
    return {built.exe ? std::optional<py::str>(cb::compat::to_py(cb::fs::to_text(*built.exe))) : std::nullopt, cb::compat::to_py(built.why)};
}

}  // namespace codebreaker::translate::backends::c_java
