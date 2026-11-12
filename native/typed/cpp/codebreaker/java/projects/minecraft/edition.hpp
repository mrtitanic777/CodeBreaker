// codebreaker/java/projects/minecraft/edition.hpp -- SHIM over cb/java/projects/minecraft/edition.hpp
// (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer.
#pragma once

#include <optional>
#include <string>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/projects/minecraft/edition.hpp"
#include "codebreaker/java/ir.hpp"
#include "py.hpp"

namespace codebreaker::java::projects::minecraft::edition {

using ClassInfo = py::ref<java::ir::JClass>;

namespace shim_ {
namespace cbe = cb::java::projects::minecraft::edition;
inline std::string opt(const std::optional<py::str>& s) { return s ? cb::compat::from_py(*s) : std::string(); }
inline std::optional<py::str> out(const std::optional<cbe::Edition>& e) {
    if (!e) return std::nullopt;
    return cb::compat::to_py(cbe::edition_name(*e));
}
}  // namespace shim_

// Short edition name -> registered target-profile name.
inline const py::dict<py::str, py::str> EDITIONS = [] {
    py::dict<py::str, py::str> d;
    for (auto e : shim_::cbe::kEditions)
        d.set(cb::compat::to_py(shim_::cbe::edition_name(e)), cb::compat::to_py(shim_::cbe::profile_name(e)));
    return d;
}();

inline std::optional<py::str> edition_for_version(const std::optional<py::str>& version_id) {
    return shim_::out(shim_::cbe::edition_for_version(shim_::opt(version_id)));
}

inline std::optional<py::str> detect_edition(const std::optional<py::list<ClassInfo>>& classes) {
    shim_::cbe::CorpusIndex index;
    if (classes) {
        for (const ClassInfo& c : *classes) {
            index.add(cb::compat::from_py(c->name), c->package ? std::optional<std::string>(cb::compat::from_py(*c->package))
                                                               : std::nullopt);
        }
    }
    return shim_::out(shim_::cbe::detect_edition(index));
}

inline std::optional<py::str> profile_name(const std::optional<py::str>& edition) {
    if (!edition) return std::nullopt;
    const auto e = shim_::cbe::parse_edition(cb::compat::from_py(*edition));
    // The Python looked the short name up only (a profile name is not a key of EDITIONS).
    if (!e || shim_::cbe::edition_name(*e) != cb::compat::from_py(*edition)) return std::nullopt;
    return cb::compat::to_py(shim_::cbe::profile_name(*e));
}

inline std::optional<py::str> resolve(const std::optional<py::str>& explicit_ = std::nullopt,
                                      const std::optional<py::str>& mc_version = std::nullopt,
                                      const std::optional<py::list<ClassInfo>>& classes = std::nullopt) {
    return cb::compat::guard([&]() -> std::optional<py::str> {
        shim_::cbe::CorpusIndex index;
        if (classes) {
            for (const ClassInfo& c : *classes)
                index.add(cb::compat::from_py(c->name),
                          c->package ? std::optional<std::string>(cb::compat::from_py(*c->package)) : std::nullopt);
        }
        const auto r = shim_::cbe::resolve(shim_::opt(explicit_), shim_::opt(mc_version), classes ? &index : nullptr);
        if (!r) return std::nullopt;
        return cb::compat::to_py(*r);
    });
}

}  // namespace codebreaker::java::projects::minecraft::edition
