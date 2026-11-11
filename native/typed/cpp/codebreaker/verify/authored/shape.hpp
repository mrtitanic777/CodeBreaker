// codebreaker/verify/authored/shape.hpp -- COMPAT SHIM over cb/verify/authored/shape.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/authored/shape.{hpp,cpp}. This header keeps the old py:: API of the four functions the
// repair step calls (statements, switches, returns, folds) and `_returned`; no logic of its own. Offsets were code points
// and are bytes now (C source is ASCII). Not kept: `_NUM`, `_NAME`, `_close`, `_tail` (internal helpers of the module).
//
// DIVERGES (see the new module's tests): a chain whose constant is octal (a leading 0, which Python's int(x, 0) refused with
// a ValueError) is left alone instead of aborting.
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/verify/authored/shape.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::verify::authored::shape {

// (start, end) of a statement; (start, end, text) of a rewrite
using Span = std::tuple<int64_t, int64_t>;
using Edit = std::tuple<int64_t, int64_t, py::str>;

namespace detail_shim {
namespace nw = cb::verify::authored::shape;
inline py::list<Edit> edits_of(const std::vector<nw::Rewrite>& rewrites) {
    py::list<Edit> out;
    for (const nw::Rewrite& r : rewrites)
        out.append(Edit(static_cast<int64_t>(r.begin), static_cast<int64_t>(r.end), cb::compat::to_py(r.text)));
    return out;
}
}  // namespace detail_shim

// [(start, end)] of the statements between `lo` and `hi`, an if with all its elses as one.
inline py::list<Span> statements(const py::str& shadow, int64_t lo, int64_t hi) {
    py::list<Span> out;
    for (const auto& s : cb::verify::authored::shape::statements(shadow.utf8(), static_cast<std::size_t>(lo), static_cast<std::size_t>(hi)))
        out.append(Span(static_cast<int64_t>(s.begin), static_cast<int64_t>(s.end)));
    return out;
}

// (start, end, text) for each `if (v == A) {..} else if (v == B) {..} [else {..}]` chain: the same as a switch.
inline py::list<Edit> switches(const py::str& body, const py::str& shadow) {
    return detail_shim::edits_of(cb::verify::authored::shape::switches(body.utf8(), shadow.utf8()));
}

// The body with every `r = x;` that only the final `return r;` follows written `return x;`, or None.
inline std::optional<py::str> returns(const py::str& head, const py::list<py::str>& decls, const py::str& body, const py::str& shadow) {
    const auto r = cb::verify::authored::shape::returns(head.utf8(), cb::compat::from_py(decls), body.utf8(), shadow.utf8());
    return r ? std::optional<py::str>(cb::compat::to_py(*r)) : std::nullopt;
}

// The type the function returns, from its heading (the word before its convention and name).
inline std::optional<py::str> _returned(const py::str& head) {
    const auto r = cb::verify::authored::shape::returned_type(head.utf8());
    return r ? std::optional<py::str>(cb::compat::to_py(*r)) : std::nullopt;
}

// (start, end, text) for each `t = f(...);` followed at once by `if (t <compare> N) {`.
inline py::list<Edit> folds(const py::list<py::str>& decls, const py::str& body, const py::str& shadow) {
    return detail_shim::edits_of(cb::verify::authored::shape::folds(cb::compat::from_py(decls), body.utf8(), shadow.utf8()));
}

}  // namespace codebreaker::verify::authored::shape
