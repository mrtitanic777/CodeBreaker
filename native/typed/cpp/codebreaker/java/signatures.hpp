// codebreaker/java/signatures.hpp -- typed C++ port of codebreaker.java.signatures.
//
// signatures.py -- generate signatures.md, a single-file map of the whole game
// codebase (every class, grouped by subsystem/package, with inheritance, fields and
// method signatures, showing BOTH the Java spelling and the resolved C++03 type).
//
// Drives the existing CodeBreaker front-end (parse -> IR) and type resolver, so the map
// stays faithful to what the converter actually sees.
//
//     python -m codebreaker java signatures <java_dir> [--out signatures.md]
//
// Typing notes:
//   * The package-role descriptions come from the active target profile. The Python freezes them into
//     `_PACKAGE_ROLES` and re-derives them when profiles.on_change fires; the profile registry is
//     transpile's (java/transpile.hpp: active_profile()), whose switch is exactly what fires on_change, so
//     _PACKAGE_ROLES() reads the active profile's package_roles when asked (the same dict, always).
//   * Python's `except Exception as e: ... str(e)` is `catch (const py::Exception&)` with e.what().
//   * run() prints to standard output as the Python does and returns nothing.
#pragma once

#include <cstdint>

#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/types.hpp"
#include "py.hpp"

namespace codebreaker::java::signatures {

extern const py::str DEFAULT_PKG;   // '(default package)'

// Package -> role descriptions of the active target profile.
py::dict<py::str, py::str> _PACKAGE_ROLES();

py::str _package_role(const py::str& pkg, const py::list<py::str>& class_names);
py::str _kind_word(const ir::JClass& cls);
py::str _inheritance(const ir::JClass& cls);
py::str _field_line(const ir::Field& field, const types::TypeResolver& resolver);
py::str _method_line(const ir::Method& method, const types::TypeResolver& resolver);

void run(const py::str& java_dir, const py::str& out_path);

}  // namespace codebreaker::java::signatures
