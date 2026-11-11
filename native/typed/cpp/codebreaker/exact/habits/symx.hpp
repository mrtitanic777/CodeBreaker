// codebreaker/exact/habits/symx.hpp -- compat shim for cb/exact/habits/symx.hpp (docs/CPP_DESIGN.md 6.3). No logic:
// conversion and forwarding. Deleted with its last old-layer consumer (exact/habits/newcomp).
#pragma once

#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/habits/symx.hpp"
#include "py.hpp"

namespace codebreaker::exact::habits::symx {

// one instruction: (text, its bytes)
using Ins = std::tuple<py::str, py::bytes>;

// w, g: [(text, bytes)]; fixed: {index of an instruction in g: the numbers in it that a linker fills in}
// -> (True, '') or (False, why)
inline std::tuple<bool, py::str> same(const py::list<Ins>& w, const py::list<Ins>& g, const py::dict<int64_t, py::set<int64_t>>& fixed,
                                      bool regs_too = false, bool flags_too = false) {
    using cb::compat::from_py;
    namespace sym = cb::exact::habits::symx;
    const auto stretch = [](const py::list<Ins>& l) {
        std::vector<sym::Instruction> out;
        for (const Ins& i : l) out.push_back({from_py(std::get<0>(i)), from_py(std::get<1>(i))});
        return out;
    };
    sym::LinkerWords words;
    for (const auto& kv : fixed.items()) {
        std::set<std::int64_t>& numbers = words[cb::compat::int_from_py<std::size_t>(std::get<0>(kv))];
        for (const int64_t n : std::get<1>(kv)) numbers.insert(n);
    }
    const sym::Verdict v = sym::same(stretch(w), stretch(g), words, {.registers = regs_too, .flags = flags_too});
    return {v.same, cb::compat::to_py(v.why)};
}

}  // namespace codebreaker::exact::habits::symx
