// codebreaker/exact/habits/oldcomp.hpp -- compat shim for cb/exact/habits/oldcomp.hpp (docs/CPP_DESIGN.md 6.3). No logic:
// conversion and forwarding. Deleted with its last old-layer consumer.
#pragma once

#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/habits/oldcomp.hpp"
#include "py.hpp"

namespace codebreaker::exact::habits::oldcomp {

// -> (True, [what was found]) or (False, [where it stopped])
inline std::tuple<bool, py::list<py::str>> equivalent(const py::bytes& want, const py::bytes& got, const py::set<int64_t>& fx) {
    std::set<std::size_t> fixups;
    for (const int64_t j : fx) {
        if (j >= 0) fixups.insert(static_cast<std::size_t>(j));   // (a negative offset is none of the compiled bytes)
    }
    const cb::Bytes w = cb::compat::from_py(want);
    const cb::Bytes g = cb::compat::from_py(got);
    const cb::exact::habits::oldcomp::Verdict v = cb::exact::habits::oldcomp::equivalent(w, g, fixups);
    return {v.equivalent, cb::compat::to_py(v.equivalent ? v.found : std::vector<std::string>{v.stopped})};
}

}  // namespace codebreaker::exact::habits::oldcomp
