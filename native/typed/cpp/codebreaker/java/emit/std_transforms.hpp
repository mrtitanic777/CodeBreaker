// codebreaker/java/emit/std_transforms.hpp -- SHIM over cb/java/emit/std_transforms.hpp (docs/CPP_DESIGN.md 6.3);
// deleted with its last py:: consumer. (The port's compiled-pattern accessors _NULL_WORD() ... had no consumer and
// are not carried.)
#pragma once

#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/emit/std_transforms.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::emit::std_transforms {

// The target C++ standard revision (transpile.CppStandard); only `year` is read here.
struct CppStandard {
    int64_t year = 0;
};

// Apply re.sub only to the parts of s outside string/char literals: `rx` is the old layer's pattern, so the
// substitution itself stays here; the literal masking is the new module's.
inline py::str _sub_outside_literals(const py::re::Pattern& rx, const py::str& repl, const py::str& s) {
    return cb::compat::to_py(cb::java::emit::std_transforms::map_outside_literals(
        cb::compat::from_py(s), [&](std::string_view piece) { return cb::compat::from_py(rx.sub(repl, cb::compat::to_py(piece))); }));
}

inline py::str _annotate_first(const py::str& text, const py::str& pattern, const py::str& annotation) {
    return cb::compat::to_py(cb::java::emit::std_transforms::annotate_first(
        cb::compat::from_py(text), cb::compat::from_py(pattern), cb::compat::from_py(annotation)));
}

inline py::str sanitize_reserved_members(const py::str& text) {
    return cb::compat::to_py(cb::java::emit::std_transforms::sanitize_reserved_members(cb::compat::from_py(text)));
}

// A year past what int can hold cannot be a C++ revision; it is clamped, not wrapped.
inline py::str apply_std_transforms(const py::str& text, const std::optional<CppStandard>& cpp_std) {
    const int year = !cpp_std ? 0 : cpp_std->year > 100000 ? 100000 : cpp_std->year < 0 ? 0 : static_cast<int>(cpp_std->year);
    return cb::compat::to_py(cb::java::emit::std_transforms::apply_std_transforms(cb::compat::from_py(text), year));
}

}  // namespace codebreaker::java::emit::std_transforms
