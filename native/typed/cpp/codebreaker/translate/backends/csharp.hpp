// codebreaker/translate/backends/csharp.hpp -- COMPAT SHIM over cb/translate/backends/csharp.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/backends/csharp.{hpp,cpp}, with Java's library and semantics in csharp_java. This header keeps
// what the old layer calls:
//
//     auto prog = csharp::write(units);        // old IR units -> Program
//     prog->files();                           // {file name: text}, the runtime's files included when used
//     prog->ledger;                            // [(where, kind, why)], after files()
//     prog->write(out_dir);                    // the files into a folder; their paths
//
// An old IR tree is written as its L2 document and read back as the typed tree (cb/compat/old_l2_tree.hpp is the old tree's codec),
// the new Program writes it with the Java library over the API-mapping registry, and a failure comes back as the old exception. The
// old module's Writer and its helper tables were the sibling port's (csharp_java), which is refactored with it; nothing else used them.
#pragma once

#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "cb/compat/old_val_json.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/arena.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/apimap.hpp"
#include "cb/translate/backends/csharp.hpp"
#include "cb/translate/backends/csharp_java.hpp"
#include "cb/translate/l2.hpp"
#include "codebreaker/java/_paths.hpp"
#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/l2.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::translate::backends::csharp {

namespace ir = codebreaker::translate::ir;
using ir::NodeP;

namespace detail_shim {
inline py::str& here_slot() {
    static py::str here;
    return here;
}
// The Java library's registry: the bundled api_maps/ folder, read once.
inline const cb::java::apimap::Registry& java_registry() {
    static const cb::java::apimap::Registry registry = cb::java::apimap::Registry::load(
        cb::fs::from_text(cb::compat::from_py(codebreaker::java::_paths::data_path(py::list<py::str>{py::str("api_maps")}))));
    return registry;
}
}  // namespace detail_shim

// the folder of csharp.py (default: where this source file is); RUNTIME() is dotnet/runtime beside backends
inline void set_here(const py::str& here) { detail_shim::here_slot() = here; }
inline py::str RUNTIME() {
    const py::str here = detail_shim::here_slot().empty() ? py::os::path::dirname(py::os::path::abspath(py::str(__FILE__))) : detail_shim::here_slot();
    return py::os::path::join(py::os::path::join(py::os::path::dirname(here), py::str("dotnet")), py::str("runtime"));
}

// This construct is not written; the method (or unit) becomes a ledgered gap.
struct Decline : py::Exception {
    py::str kind;
    py::str why;
    explicit Decline(const py::str& kind, const std::optional<py::str>& why_ = std::nullopt)
        : py::Exception(why_ && !why_->empty() ? *why_ : kind), kind(kind), why(why_ && !why_->empty() ? *why_ : kind) {}
    const char* type() const override { return "Decline"; }
};

inline py::str ident(const py::str& name) { return cb::compat::to_py(cb::translate::backends::csharp::ident(cb::compat::from_py(name))); }

// The units to write, what they declare, and what came of writing them.
class Program : public py::object {
  public:
    py::list<std::tuple<py::str, py::str, py::str>> ledger;   // (where, kind, why), after files()

    explicit Program(const py::list<NodeP>& old_units) {
        namespace cs = cb::translate::backends::csharp;
        arena_ = std::make_unique<cb::Arena>();
        for (const NodeP& u : old_units) {
            units_.push_back(cb::translate::l2::from_json(*arena_, cb::compat::to_json_value(codebreaker::translate::l2::to_json(u))));
        }
        cs::Options options;
        options.libraries["java"] = cb::translate::backends::csharp_java::factory(detail_shim::java_registry());
        options.runtime_dir = cb::fs::from_text(cb::compat::from_py(RUNTIME()));
        cb::compat::guard([&] { program_ = std::make_unique<cs::Program>(units_, std::move(options)); });
    }

    // {file name: text}, the runtime's files included when used
    py::dict<py::str, py::str> files() {
        return cb::compat::guard([&] {
            py::dict<py::str, py::str> out;
            for (const auto& f : program_->files()) out.set(cb::compat::to_py(f.name), cb::compat::to_py(f.text));
            ledger = py::list<std::tuple<py::str, py::str, py::str>>();
            for (const auto& g : program_->ledger()) ledger.append(std::make_tuple(cb::compat::to_py(g.where), cb::compat::to_py(g.kind), cb::compat::to_py(g.why)));
            return out;
        });
    }

    // writes the files into out_dir; the paths
    py::list<py::str> write(const py::str& out_dir) {
        return cb::compat::guard([&] {
            py::list<py::str> out;
            for (const auto& p : program_->write(cb::fs::from_text(cb::compat::from_py(out_dir)))) out.append(cb::compat::to_py(cb::fs::to_text(p)));
            files();   // (the ledger)
            return out;
        });
    }

  private:
    std::unique_ptr<cb::Arena> arena_;
    std::vector<cb::translate::ir::Node*> units_;
    std::unique_ptr<cb::translate::backends::csharp::Program> program_;
};

// -> Program (call .files() or .write(out_dir))
inline py::ref<Program> write(const py::list<NodeP>& units) { return std::make_shared<Program>(units); }

}  // namespace codebreaker::translate::backends::csharp
