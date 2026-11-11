// codebreaker/java/projects/minecraft/profiles/minecraft_base.hpp -- SHIM over
// cb/java/projects/minecraft/profiles/minecraft_base.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py::
// consumer.
#pragma once

#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_base.hpp"
#include "codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp"
#include "py.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_base {

namespace shim_ {
namespace cbb = cb::java::projects::minecraft::profiles::minecraft_base;
inline py::list<std::tuple<py::str, py::str>> to_old(const std::vector<cb::java::profiles::FilterRule>& rules) {
    py::list<std::tuple<py::str, py::str>> out;
    for (const auto& r : rules) out.append(std::make_tuple(cb::compat::to_py(r.pattern), cb::compat::to_py(r.folder)));
    return out;
}
}  // namespace shim_

// Era-invariant re-exports (single source of truth = minecraft_xbox360).
inline const py::str SHARED_DEAD_TYPES = cb::compat::to_py(shim_::cbb::shared_dead_types());
inline const py::list<py::str> SHARED_HEAVY_TYPES = cb::compat::to_py(shim_::cbb::shared_heavy_types());
// The full proven filter taxonomy (a copy: editions must not change the xbox360 table through this one).
inline const py::list<std::tuple<py::str, py::str>> FULL_FILTERS = shim_::to_old(shim_::cbb::full_filters());
// Package roles that are not tied to one era's package layout.
inline const py::dict<py::str, py::str> SHARED_PACKAGE_ROLES = cb::compat::to_py(shim_::cbb::shared_package_roles());

// The .vcxproj/.filters rules for an edition: era-specific rules FIRST, then the shared full taxonomy.
inline py::list<std::tuple<py::str, py::str>> build_filters(const py::list<std::tuple<py::str, py::str>>& era_rules) {
    std::vector<cb::java::profiles::FilterRule> era;
    for (const auto& r : era_rules) era.push_back({cb::compat::from_py(std::get<0>(r)), cb::compat::from_py(std::get<1>(r))});
    return shim_::to_old(shim_::cbb::build_filters(era));
}
inline py::list<std::tuple<py::str, py::str>> build_filters() { return build_filters(py::list<std::tuple<py::str, py::str>>()); }

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_base
