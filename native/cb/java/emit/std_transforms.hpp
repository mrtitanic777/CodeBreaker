// cb/java/emit/std_transforms.hpp -- C++ standard-revision text transforms for emitted code.
//
// Applied as a final pass over rendered .h/.cpp text. Each transform is gated on the target C++ standard's year,
// so C++03 output is untouched.
//
// Transforms:
//   C++11+  NULL -> nullptr  (outside string/char literals)
//   C++11+  verbose iterator types in for-loops and declarations -> auto
//   C++11+  static const int X = N  ->  constexpr int X = N  (in enum structs)
//
// Each transform annotates the FIRST occurrence in a file with an inline comment naming the standard that
// introduced it, so the developer sees at a glance what was modernised.
#pragma once

#include <functional>
#include <string>
#include <string_view>

namespace cb::java::emit::std_transforms {

// Apply the transforms the target standard (its year: 2003, 2011, 2014, ...) allows to emitted code. Before 2011
// the text is returned unchanged.
std::string apply_std_transforms(std::string_view text, int cpp_year);

// Enum-constant references that collide with C/C++ macros: the declaration is suffixed with `_` (naming::enum_member),
// but a reference `EnumX::NULL` reaches emission through several paths (method bodies, static-field initialisers,
// enum-constant arguments), so every one is normalised here in one always-on pass (at every C++ level). Only an
// upper-case macro name after `::` is an enum constant, so this is safe. Applying it twice changes nothing more.
std::string sanitize_reserved_members(std::string_view text);

// The text with `f` applied to each piece that lies outside string and character literals; the literals are
// kept as they are. (A literal is "..." or '...' with backslash escapes; an unclosed quote is not a literal.)
std::string map_outside_literals(std::string_view text, const std::function<std::string(std::string_view)>& f);

// Find the first line containing `pattern` whose remainder (after the pattern) holds no `//` comment, and append
// `annotation` to it as an inline comment (two spaces after its trailing whitespace is trimmed). A text without
// such a line is returned unchanged, and so is any text for an empty pattern (which finds nothing).
std::string annotate_first(std::string_view text, std::string_view pattern, std::string_view annotation);

}  // namespace cb::java::emit::std_transforms
