// codebreaker/java/emit/naming.hpp -- C++ from codebreaker.java.emit.naming.
//
// naming.py -- identifier conventions for emitted C++.
//
// Clean, CONSISTENT rules (the legacy converter mixed `s_` statics with the odd
// bare exception like `blocksList`; we normalize):
//   * static fields  -> `s_` prefix
//   * instance fields -> bare (decompiled locals are var1/var2, so no collision)
//   * leading underscores stripped to avoid reserved `s__name` / `__name`
#pragma once

#include "py.hpp"

namespace codebreaker::java::emit::naming {

// What field_name() reads of a field: the Python takes any object with `.name` and `.is_static`.
struct Field {
    py::str name;
    bool is_static = false;
};

// Enum-constant names that collide with C/C++ macros or keywords (Windows/CRT/xnamath define these).
// A member literally named NULL/TRUE/FALSE expands to a constant -> "C2059 syntax error 'constant'".
// We suffix `_` at BOTH the declaration and the reference so `EnumX::NULL` becomes `EnumX::NULL_`.
// (Python: _RESERVED_MEMBER; a leading underscore and capital is reserved in C++.)
extern py::set<py::str> RESERVED_MEMBER_;

py::str static_field_name(const py::str& name);
py::str instance_field_name(const py::str& name);
py::str field_name(const Field& field);
// 'Vec3D' -> 'VEC3D_H'.
py::str guard_macro(const py::str& class_name);
// Sanitize an enum-constant identifier that collides with a C/C++ macro or keyword.
// Idempotent and applied consistently at declaration and reference sites.
py::str enum_member(const py::str& name);

}  // namespace codebreaker::java::emit::naming
