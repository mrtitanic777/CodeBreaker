// codebreaker/java/emit/std_transforms.hpp -- C++17 port of codebreaker.java.emit.std_transforms.
//
// std_transforms.py -- C++ standard-revision text transforms for emitted code.
//
// Applied as a final pass over rendered .h/.cpp text. Each transform is gated on
// the target CppStandard year so C++03 output is untouched.
//
// Transforms:
//   C++11+  NULL -> nullptr  (outside string/char literals)
//   C++11+  verbose iterator types in for-loops and declarations -> auto
//   C++11+  static const int X = N  ->  constexpr int X = N  (in enum structs)
//
// Each transform annotates the FIRST occurrence in a file with an inline comment
// identifying which C++ standard introduced it, so the developer knows at a glance.
#pragma once

#include <optional>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::emit::std_transforms {

// The target C++ standard revision (transpile.CppStandard); only `year` is read here.
struct CppStandard {
    int64_t year = 0;
};

// (The module-level re.compile(...) patterns, made on first use.)
const py::re::Pattern& _NULL_WORD();
const py::re::Pattern& _ITER_FOR();
const py::re::Pattern& _ITER_DECL();
const py::re::Pattern& _STATIC_CONST_INT();
const py::re::Pattern& _RESERVED_REF();

// Apply re.sub only to parts of s outside string/char literals.
py::str _sub_outside_literals(const py::re::Pattern& rx, const py::str& repl, const py::str& s);

// Find the first line containing `pattern` and append `annotation` as an
// inline comment, but only if that line doesn't already have one.
py::str _annotate_first(const py::str& text, const py::str& pattern, const py::str& annotation);

// Suffix `EnumX::NULL` -> `EnumX::NULL_` to match the sanitized declaration. Idempotent
// (the `\b` after the name does not match the already-suffixed `NULL_`). Applied at all C++ levels.
py::str sanitize_reserved_members(const py::str& text);

// Apply C++ standard-specific text transforms to emitted code.
py::str apply_std_transforms(const py::str& text, const std::optional<CppStandard>& cpp_std);

}  // namespace codebreaker::java::emit::std_transforms
