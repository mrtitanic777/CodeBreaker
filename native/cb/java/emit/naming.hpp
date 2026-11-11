// cb/java/emit/naming.hpp -- identifier conventions for emitted C++.
//
// Clean, consistent rules (the legacy converter mixed `s_` statics with the odd bare exception like
// `blocksList`; we normalise):
//   * static fields   -> `s_` prefix
//   * instance fields -> bare (decompiled locals are var1/var2, so no collision)
//   * leading underscores are stripped to avoid reserved `s__name` / `__name`
#pragma once

#include <string>
#include <string_view>

namespace cb::java::emit::naming {

// `s_` + the name without its leading underscores.
std::string static_field_name(std::string_view name);
std::string instance_field_name(std::string_view name);
// The emitted name of a field of a class.
std::string field_name(std::string_view name, bool is_static);

// 'Vec3D' -> 'VEC3D_H'. Every character that is not an ASCII letter or digit becomes one '_' (a multi-byte UTF-8
// character is one character, so it becomes one '_', not one per byte).
std::string guard_macro(std::string_view class_name);

// Enum-constant names (and member names) that collide with C/C++ macros or keywords: Windows, the CRT and xnamath
// define NULL/TRUE/FALSE/..., and an enum member literally named NULL would expand to a constant ("C2059 syntax
// error 'constant'"). They get a `_` suffix at both the declaration and every reference, so `EnumX::NULL` is
// emitted as `EnumX::NULL_`.
bool is_reserved_member(std::string_view name);

// The name to declare and reference an enum constant by: a reserved name gets the `_` suffix, any other is
// unchanged. Call it at declaration and reference sites alike. A result is never itself reserved, so applying
// it twice changes nothing more (`NULL_` stays `NULL_`).
std::string enum_member(std::string_view name);

}  // namespace cb::java::emit::naming
