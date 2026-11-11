// codebreaker/java/console/platform_equivalents.hpp -- SHIM over cb/java/console/platform_equivalents.hpp
// (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer.
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/console/platform_equivalents.hpp"
#include "py.hpp"

namespace codebreaker::java::console::platform_equivalents {

// One entry of the map (the Python's dict with these keys; every entry in the table has xdk, lib and status).
struct Equiv {
    py::str xdk;
    py::str lib;
    py::str status;                                   // 'real' | 'stub' | 'dead'
    std::optional<py::str> note;
    std::optional<py::dict<py::str, py::str>> methods;
};

namespace shim_ {
namespace cbpe = cb::java::console::platform_equivalents;

inline Equiv to_old(const cbpe::Equiv& e) {
    Equiv out{cb::compat::to_py(e.xdk), cb::compat::to_py(e.lib), cb::compat::to_py(cbpe::status_name(e.status)), std::nullopt,
              std::nullopt};
    if (!e.note.empty()) out.note = cb::compat::to_py(e.note);
    if (!e.methods.empty()) {
        py::dict<py::str, py::str> methods;
        for (const cbpe::MethodEquiv& m : e.methods) methods.set(cb::compat::to_py(m.method), cb::compat::to_py(m.detail));
        out.methods = methods;
    }
    return out;
}

inline py::dict<py::str, py::dict<py::str, Equiv>> build() {
    py::dict<py::str, py::dict<py::str, Equiv>> out;
    for (const cbpe::Subsystem& sub : cbpe::subsystems()) {
        py::dict<py::str, Equiv> entries;
        for (const cbpe::Equiv& e : sub.entries) entries.set(cb::compat::to_py(e.basename), to_old(e));
        out.set(cb::compat::to_py(sub.name), entries);
    }
    return out;
}
}  // namespace shim_

// subsystem -> ordered {basename: entry}
inline const py::dict<py::str, py::dict<py::str, Equiv>> EQUIV = shim_::build();

// Return the equivalent for an emitted class basename, or None.
inline std::optional<Equiv> lookup(const py::str& basename) {
    const cb::java::console::platform_equivalents::Equiv* e = cb::java::console::platform_equivalents::lookup(cb::compat::from_py(basename));
    if (e == nullptr) return std::nullopt;
    return shim_::to_old(*e);
}

// 'xdk' | 'handwritten' | 'dead'
inline py::str kind(const Equiv& e) {
    namespace cbpe = cb::java::console::platform_equivalents;
    // (a status other than the three the table uses was never "dead": it is read as a live one)
    const cbpe::Status status = cbpe::parse_status(cb::compat::from_py(e.status)).value_or(cbpe::Status::Real);
    return cb::compat::to_py(cbpe::kind_name(cbpe::kind(status, cb::compat::from_py(e.lib))));
}

// The one-line C++ comment dropped next to a stub; '' if we have no mapping.
inline py::str annotation(const py::str& basename) {
    return cb::compat::to_py(cb::java::console::platform_equivalents::annotation(cb::compat::from_py(basename)));
}

// Flatten to (subsystem, basename, entry) for the coverage report.
inline py::list<std::tuple<py::str, py::str, Equiv>> all_entries() {
    py::list<std::tuple<py::str, py::str, Equiv>> out;
    for (const auto& sub_items : EQUIV.items()) {
        for (const auto& name_entry : std::get<1>(sub_items).items()) {
            out.append(std::make_tuple(std::get<0>(sub_items), std::get<0>(name_entry), std::get<1>(name_entry)));
        }
    }
    return out;
}

}  // namespace codebreaker::java::console::platform_equivalents
