// codebreaker/exact/repair.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/exact/repair.{hpp,cpp}; this header
// keeps the old py:: surface (_shifts, candidates, main) for the ports and the dynamic exe that still use it. Conversion and forwarding only: the old
// project is seen by the new module through ledger::detail::NewView. Deleted with its last py:: consumer.
//
// Not carried over: Job, _only, _piece_of, _pascal and _sourced (the new module's own jobs() and mend()).
//
// DIVERGES, for the old callers: source text crosses as Latin-1 (every character a byte, as the sources of the period are), the candidates
// come in the order the new module tries them, and a Pascal piece that cannot be found or compiled is left out of the jobs instead of
// raising (StopIteration, TypeError, IndexError).
#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/repair.hpp"
#include "codebreaker/exact/ledger.hpp"
#include "py.hpp"

namespace codebreaker::exact::repair {

namespace shim_ {
// the source as bytes (the old text is Latin-1 decoded) and back
inline std::string latin1_bytes(const py::str& s) {
    const cb::Bytes b = cb::text::to_latin1(cb::compat::from_py(s));
    return std::string(cb::as_text(b));
}
inline py::str from_latin1_bytes(const std::string& s) { return cb::compat::to_py(cb::text::from_latin1(cb::as_bytes(s))); }
}  // namespace shim_

// what a note names: the shift of the values and the places in the program where they differ -> (shift, offsets), or None
inline std::optional<std::tuple<int64_t, py::list<int64_t>>> _shifts(const py::list<py::str>& notes) {
    std::vector<std::string> n;
    for (const py::str& x : notes) n.push_back(cb::compat::from_py(x));
    const auto s = cb::exact::repair::shifts(n);
    if (!s) return std::nullopt;
    py::list<int64_t> offsets;
    for (std::int64_t o : s->offsets) offsets.append(o);
    return std::make_tuple(s->by, offsets);
}

// the source changed so that the values `mine` become mine + d -> [(what, text)]
inline py::list<std::tuple<py::str, py::str>> candidates(const py::str& text, const py::list<int64_t>& mine, int64_t d) {
    std::vector<std::int64_t> values;
    for (int64_t v : mine) values.push_back(v);
    py::list<std::tuple<py::str, py::str>> out;
    for (const auto& c : cb::exact::repair::candidates(shim_::latin1_bytes(text), values, d))
        out.append(std::make_tuple(cb::compat::to_py(c.what), shim_::from_latin1_bytes(c.text)));
    return out;
}

inline int64_t main(const ledger::Project& prj, const py::list<py::str>& argv, std::ostream& out = std::cout) {
    std::vector<std::string> args;
    for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
    const ledger::detail::NewView view(prj);
    return cb::exact::repair::command(view, args, [&out](std::string_view line) { out << line << "\n"; });
}

}  // namespace codebreaker::exact::repair
