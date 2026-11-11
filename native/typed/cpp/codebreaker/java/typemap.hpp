// codebreaker/java/typemap.hpp -- SHIM over cb/java/typemap.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
//
// The old code reads (and the pipeline mutates) module-level names, so the shim keeps those names: the fixed
// tables are filled from the new tables, the profile-sourced ones start empty (the generic profile) and
// `_reseed` takes a profile's data. The new tree has none of this state: a conversion owns a TypeProfile.
#pragma once

#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/typemap.hpp"

namespace codebreaker::java::typemap {

namespace cbt = cb::java::typemap;

// The fields of a codebreaker.java.profiles.Profile that this module reads.
struct Profile {
    py::set<py::str> nullable;
    py::set<py::str> nullable_fields;
    py::set<py::str> nullable_getters;
    py::dict<py::str, py::str> nullable_getter_return_type;
    // (ClassName, fieldName) pairs
    py::set<std::tuple<py::str, py::str>> unsigned_byte_scalar_fields;
    py::set<py::str> default_value_types;
};

namespace shim_ {
inline py::set<py::str> names(const cbt::NameSet& s) {
    py::set<py::str> out;
    for (const std::string& n : s) out.add(cb::compat::to_py(n));
    return out;
}
inline py::dict<py::str, py::str> map(const cbt::NameMap& m) {
    py::dict<py::str, py::str> out;
    for (const auto& [k, v] : m) out.set(cb::compat::to_py(k), cb::compat::to_py(v));
    return out;
}
}  // namespace shim_

inline const py::set<py::str> PRIMITIVES = shim_::names(cbt::primitives());
inline const py::dict<py::str, py::str> JAVA_TO_CPP = shim_::map(cbt::java_to_cpp());
inline py::set<std::tuple<py::str, py::str>> UNSIGNED_BYTE_SCALAR_FIELDS;   // seeded from the generic profile: empty
inline const py::dict<py::str, py::str> PORT_NOTE_TYPES = shim_::map(cbt::port_note_types());
inline const py::set<py::str> CONTAINERS = shim_::names(cbt::containers());
inline const py::dict<py::str, py::str> RAW_CONTAINER_DEFAULT = shim_::map(cbt::raw_container_default());
inline const py::set<py::str> VALUE_STDLIB = shim_::names(cbt::value_stdlib());
inline const py::set<py::str> BOXED_VALUE_TYPES = shim_::names(cbt::boxed_value_types());
inline py::set<py::str> KNOWN_NULLABLE;
inline py::set<py::str> KNOWN_NULLABLE_FIELDS;
inline py::set<py::str> KNOWN_NULLABLE_GETTERS;
inline py::dict<py::str, py::str> NULLABLE_GETTER_RETURN_TYPE;
inline const py::set<py::str> RUNTIME_STATIC_HOLDERS = shim_::names(cbt::runtime_static_holders());
inline const py::set<py::str> RUNTIME_HEADERS = shim_::names(cbt::runtime_headers());
inline const py::set<py::str> JAVART_STATICS = shim_::names(cbt::javart_statics());
inline const py::set<py::str> RUNTIME_ENUM_LIKE = shim_::names(cbt::runtime_enum_like());
inline const py::dict<py::str, py::str> MATH_API = shim_::map(cbt::math_api());
inline const py::dict<py::str, py::str> MATH_API_EMIT = shim_::map(cbt::math_api_emit());
inline py::set<py::str> DEFAULT_VALUE_TYPES;

// Re-derive every profile-sourced global from ``p`` when the active target profile changes (each a fresh copy).
inline void _reseed(const Profile& p) {
    UNSIGNED_BYTE_SCALAR_FIELDS = p.unsigned_byte_scalar_fields;
    KNOWN_NULLABLE = p.nullable;
    KNOWN_NULLABLE_FIELDS = p.nullable_fields;
    KNOWN_NULLABLE_GETTERS = p.nullable_getters;
    NULLABLE_GETTER_RETURN_TYPE = p.nullable_getter_return_type;
    DEFAULT_VALUE_TYPES = p.default_value_types;
}

}  // namespace codebreaker::java::typemap
