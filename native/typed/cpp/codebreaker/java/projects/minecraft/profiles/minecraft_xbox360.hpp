// codebreaker/java/projects/minecraft/profiles/minecraft_xbox360.hpp -- SHIM over
// cb/java/projects/minecraft/profiles/minecraft_xbox360.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py::
// consumer. The era profile ports (alpha, beta, ...) still build their own records from these tables.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_xbox360.hpp"
#include "py.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_xbox360 {

// The Python class is a bag of target data; every field defaults to empty. (The registry's own record is
// codebreaker::java::profiles::Profile; minecraft::package converts this one to it.)
struct Profile {
    py::str name;
    py::list<py::str> static_init_order;
    py::dict<py::str, py::str> package_roles;
    py::list<py::str> heavy_types;                                // a tuple of names
    py::list<std::tuple<py::str, py::str>> vcxproj_filters;       // (basename regex, filter path)
    std::optional<py::str> dead_types;                            // anchored regex, or None
    std::optional<py::str> optional_types;                        // anchored regex, or None
    py::set<py::str> nullable;
    py::set<py::str> nullable_fields;
    py::set<py::str> nullable_getters;
    py::dict<py::str, py::str> nullable_getter_return_type;
    py::set<py::str> unsigned_byte_scalar_fields;
    py::set<py::str> default_value_types;
    py::list<py::str> heavy_types_high;                           // () by default
};

namespace shim_ {
namespace cbx = cb::java::projects::minecraft::profiles::minecraft_xbox360;
inline py::list<std::tuple<py::str, py::str>> filters() {
    py::list<std::tuple<py::str, py::str>> out;
    for (const auto& f : cbx::vcxproj_filters()) out.append(std::make_tuple(cb::compat::to_py(f.pattern), cb::compat::to_py(f.folder)));
    return out;
}
}  // namespace shim_

// Proven, ctor-MEDIATED static-init dependencies used to SEED the topological order.
inline const py::list<py::str> STATIC_INIT_ORDER = cb::compat::to_py(shim_::cbx::static_init_order());
// Package -> one-line role, for the signatures.md codebase map.
inline const py::dict<py::str, py::str> PACKAGE_ROLES = cb::compat::to_py(shim_::cbx::package_roles());
// Types heavy enough that per-frame heap allocation of them is a hotpath-rule flag.
inline const py::list<py::str> HEAVY_TYPES = cb::compat::to_py(shim_::cbx::heavy_types());
// Registry arrays whose slots are NULL until an id is registered.
inline const py::set<py::str> NULLABLE = cb::compat::to_py(shim_::cbx::nullable());
// Fields that are legitimately NULL at points in the lifecycle.
inline const py::set<py::str> NULLABLE_FIELDS = cb::compat::to_py(shim_::cbx::nullable_fields());
// Getters that can return NULL.
inline const py::set<py::str> NULLABLE_GETTERS = cb::compat::to_py(shim_::cbx::nullable_getters());
// Return (pointer) type of each pointer-returning nullable getter.
inline const py::dict<py::str, py::str> NULLABLE_GETTER_RETURN_TYPE = cb::compat::to_py(shim_::cbx::nullable_getter_return_type());
// DEAD external desktop/GUI types (the anchored prune regex).
inline const py::str DEAD_TYPES = cb::compat::to_py(shim_::cbx::dead_types());
// Visual Studio .vcxproj/.filters classification: (basename regex, filter path). First match wins.
inline const py::list<std::tuple<py::str, py::str>> VCXPROJ_FILTERS = shim_::filters();

inline const Profile PROFILE = Profile{
    py::str("minecraft_xbox360"),
    STATIC_INIT_ORDER,
    PACKAGE_ROLES,
    HEAVY_TYPES,
    VCXPROJ_FILTERS,
    DEAD_TYPES,
    std::nullopt,                                  // optional_types
    NULLABLE,
    NULLABLE_FIELDS,
    NULLABLE_GETTERS,
    NULLABLE_GETTER_RETURN_TYPE,
    py::set<py::str>(),                            // unsigned_byte_scalar_fields=set()
    py::set<py::str>(),                            // default_value_types=set()
    py::list<py::str>(),                           // heavy_types_high = ()
};

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_xbox360
