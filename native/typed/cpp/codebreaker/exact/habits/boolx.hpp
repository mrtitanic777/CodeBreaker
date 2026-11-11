// codebreaker/exact/habits/boolx.hpp -- compat shim for cb/exact/habits/boolx.hpp (docs/CPP_DESIGN.md 6.3). No logic:
// conversion and forwarding. Deleted with its last old-layer consumer (exact/habits/newcomp).
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/habits/boolx.hpp"
#include "py.hpp"

namespace codebreaker::exact::habits::boolx {

using Instruction = std::tuple<int64_t, py::str>;   // (offset, text)

// same()'s answer: (True, (else target of the original, else target of the compiled) or None) or (False, why).
struct SameResult {
    bool same = false;
    std::optional<std::tuple<int64_t, int64_t>> elses;   // (when same)
    py::str why;                                         // (when not)
};

// iw, ig: the stretches [lo, hi) of the original and the compiled code; initw, initg: what SI and DI hold at their
// start ({"si": ..., "di": ...}; absent: their own start values)
inline SameResult same(const py::list<Instruction>& iw, int64_t lw, int64_t hw, const py::list<Instruction>& ig, int64_t lg, int64_t hg,
                       const std::optional<py::dict<py::str, py::str>>& initw = std::nullopt,
                       const std::optional<py::dict<py::str, py::str>>& initg = std::nullopt) {
    using cb::compat::int_from_py;
    namespace bx = cb::exact::habits::boolx;
    const auto code = [](const py::list<Instruction>& l) {
        std::vector<bx::Instruction> out;
        for (const Instruction& i : l) out.push_back({int_from_py<std::uint32_t>(std::get<0>(i)), std::get<1>(i).utf8()});
        return out;
    };
    const auto bases = [](const std::optional<py::dict<py::str, py::str>>& init) {
        bx::Bases b;
        if (init) {
            for (const auto& kv : init->items()) {
                const std::string name = std::get<0>(kv).utf8();
                if (name == "si") b.si = std::get<1>(kv).utf8();
                else if (name == "di") b.di = std::get<1>(kv).utf8();
            }
        }
        return b;
    };
    const std::vector<bx::Instruction> original = code(iw);
    const std::vector<bx::Instruction> compiled = code(ig);
    const bx::Verdict v = bx::same({original, int_from_py<std::uint32_t>(lw), int_from_py<std::uint32_t>(hw), bases(initw)},
                                   {compiled, int_from_py<std::uint32_t>(lg), int_from_py<std::uint32_t>(hg), bases(initg)});
    SameResult r;
    r.same = v.same;
    if (v.else_targets) r.elses = std::make_tuple<int64_t, int64_t>(v.else_targets->original, v.else_targets->compiled);
    r.why = cb::compat::to_py(v.why);
    return r;
}

}  // namespace codebreaker::exact::habits::boolx
