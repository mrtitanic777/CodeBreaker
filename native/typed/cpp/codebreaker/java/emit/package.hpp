// codebreaker/java/emit/package.hpp -- typed C++ from codebreaker/java/emit/__init__.py (the package `codebreaker.java.emit`).
//
// emit/ -- Module 4: IR + resolved types -> correct C++03, by construction.
//
// Deliberately split into separate files so this never becomes another 10k-line
// monolith:
//   naming.py      identifier conventions (s_ statics, bare instance/value fields,
//                  include-guard macros)
//   header.py      render the .h (guard, includes, forward decls, class body)
//   source.py      render the .cpp (static defs, ctors w/ safety, method defs); the bodies come
//                  from the translator's back end (codebreaker/translate/backends/cpp03.py)
//
// The emitter takes a TypeResolver (Module 3) and an optional Ledger (so every
// PORT / GUESS / STUB is reported as it is emitted, not discovered later).
//
// Typing notes:
//   * The package re-exports render_header (emit.header), render_source (emit.source) and apply_std_transforms
//     (emit.std_transforms): here they are using-declarations of those typed ports' functions, so
//     `emit::package::render_header(...)` is the call the Python's `emit.render_header(...)` is.  `__all__` is the list of
//     the three names.
#pragma once

#include "codebreaker/java/emit/header.hpp"
#include "codebreaker/java/emit/source.hpp"
#include "codebreaker/java/emit/std_transforms.hpp"
#include "py.hpp"

namespace codebreaker::java::emit::package {

using codebreaker::java::emit::header::render_header;
using codebreaker::java::emit::source::render_source;
using codebreaker::java::emit::std_transforms::apply_std_transforms;

extern const py::list<py::str> __all__;

}  // namespace codebreaker::java::emit::package
