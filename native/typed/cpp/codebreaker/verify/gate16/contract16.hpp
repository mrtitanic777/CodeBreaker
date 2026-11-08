// codebreaker/verify/gate16/contract16.hpp -- interface of codebreaker.verify.gate16.contract16 (typed C++; implementation pending).
//
// contract16.py -- the calls of each function rebuilt from C beside the original's: per callee, the bytes the caller
// takes off the stack after the call.  The original's callers say how each callee is to be called -- one that removes
// its own arguments is called without the caller taking them off -- and a rebuilt function that takes off what the
// callee already removed (or leaves what it does not) leaves the stack wrong for everything after it.
//
//     mismatches(original, folder, image, names) -> {name: why}
//
// Typing notes:
//   * A callee key is ("near", segment, offset) or ("far", address): `CalleeKey` (kind, a, b) -- for "far" `a` is the
//     address and `b` is 0.  The tests' lambdas read key[1] (a for far).
//   * "bytes taken off after a call" is a count, or "?<operand>" for a count read from a register (compared as text):
//     `TakenOff` = variant<int64_t, py::str>.
//   * `names` is the set of function names to check (the callers pass a dict's keys or a list; iterate once).
//   * The Python caches each described module through pickle (cache "contract16_described"): the .cpp may keep that
//     cache by re-running describe16 instead (the description.txt is rewritten either way).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::verify::gate16::contract16 {

// ("near", seg, off) / ("far", address) of a direct call
struct CalleeKey {
    py::str kind;       // "near" | "far"
    int64_t a = 0;      // far: the address; near: the segment
    int64_t b = 0;      // near: the offset
};
using TakenOff = std::variant<int64_t, py::str>;
// (callee, bytes taken off after it, argument bytes pushed for it -- negative: at least that, None: not known --
// whether the frame's LEAVE follows)
using CallRow = std::tuple<py::str, TakenOff, std::optional<int64_t>, bool>;

// The calls of the code in [lo, hi) of segment `seg` in `img`. name_of(key): the callee's name or None; removes(key):
// the bytes a callee removes itself (its `retf n`), or None.   (used by tests)
py::list<CallRow> _calls(const py::bytes& img, int64_t seg, int64_t lo, int64_t hi,
                         const std::function<std::optional<py::str>(const CalleeKey&)>& name_of,
                         const std::function<std::optional<int64_t>(const CalleeKey&)>& removes =
                             [](const CalleeKey&) { return std::nullopt; });

// {name: why} for each named function whose calls take off different bytes than the original's calls to the same
// callee (map file m.map in `folder`, and the image of the build).  `original`, `image` are paths.
py::dict<py::str, py::str> mismatches(const py::str& original, const py::str& folder, const py::str& image,
                                      const py::list<py::str>& names);

}  // namespace codebreaker::verify::gate16::contract16
