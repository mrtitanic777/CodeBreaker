// codebreaker/java/rules.hpp -- SHIM over cb/java/rules.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py::
// consumer.
//
// The old code reads the heavy types from state that the active-profile switch re-seeds (`_reseed`) and the nullable
// getters/fields from the typemap module's globals; the shim keeps that state and hands it to the new scan as the
// profile the scan reads. Only the entry points the converter and the `analyze` command call are here.
#pragma once

#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/profiles/package.hpp"
#include "cb/java/rules.hpp"
#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/typemap.hpp"
#include "py.hpp"

namespace codebreaker::java::rules {

// A type that is expensive or owns a resource and must never be copied by value.
inline py::list<py::str>& heavy_types_() {
    static py::list<py::str> v;
    return v;
}
inline py::list<py::str>& heavy_high_() {
    static py::list<py::str> v;
    return v;
}

// Re-derive the heavy types when the active target profile changes (profiles.on_change).
inline void _reseed(const py::list<py::str>& heavy_types, const py::list<py::str>& heavy_types_high) {
    heavy_types_() = heavy_types.copy();
    heavy_high_() = heavy_types_high.copy();
}

// The profile the old state describes: the heavy types and the nullable getters/fields.
inline cb::java::profiles::Profile current_profile_() {
    cb::java::profiles::Profile p;
    p.heavy_types = cb::compat::from_py(heavy_types_());
    p.heavy_types_high = cb::compat::from_py(heavy_high_());
    for (const py::str& n : typemap::KNOWN_NULLABLE_GETTERS) p.type.nullable_getters.insert(cb::compat::from_py(n));
    for (const py::str& n : typemap::KNOWN_NULLABLE_FIELDS) p.type.nullable_fields.insert(cb::compat::from_py(n));
    return p;
}

// Run every structural rule over the converted tree, appending Events. Returns the count.
inline int64_t scan_rules(ledger::Ledger& led, const py::str& output_dir, const std::optional<py::str>& platform = std::nullopt) {
    return cb::compat::guard([&] {
        const cb::java::profiles::Profile profile = current_profile_();
        cb::java::ledger::Ledger found;
        const std::size_t n = cb::java::rules::scan_rules(
            found, cb::fs::from_text(cb::compat::from_py(output_dir)),
            {.platform = platform ? cb::compat::from_py(*platform) : std::string(), .profile = &profile});
        for (const cb::java::ledger::Event& e : found.events()) led.events.append(ledger::from_new(e));
        return static_cast<int64_t>(n);
    });
}

}  // namespace codebreaker::java::rules
