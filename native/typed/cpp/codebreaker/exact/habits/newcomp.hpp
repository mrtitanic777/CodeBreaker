// codebreaker/exact/habits/newcomp.hpp -- compat shim for cb/exact/habits/newcomp.hpp (docs/CPP_DESIGN.md 6.3). No logic:
// conversion and forwarding. Deleted with its last old-layer consumer (exact/pascal_join).
//
// newcomp.py - is a procedure of MASM 1.10 the kit compiler's code but for the later compiler's habits? (The habits
// and the comparison are described in cb/exact/habits/newcomp.hpp.)
//
//     equivalent(want, got, fixups)  -> (True, [what was found]) or (False, [where it stopped])
//     equivalent3(want, got, fixups) -> the same for INIT86, built by another state of the compiler
//
// The module tables of the original - AMAP (the alignment of the last pair found equivalent: [(offset in the original,
// offset in the compiled)]) and GWALK (where the compiled code's instructions begin) - are kept as the old callers read them: a
// comparison sets them from its answer when it has an alignment (a pair found equivalent, or one that differs only in its
// jump displacements). What follows the entry helper's call comes from cmpp::HDRLEN, as before. DEBUG (the original's
// tracing) is not carried over.
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/habits/newcomp.hpp"
#include "codebreaker/exact/habits/cmpp.hpp"
#include "py.hpp"

namespace codebreaker::exact::habits::newcomp {

inline py::list<int64_t> GWALK;                           // ... and where the compiled code's instructions begin
inline py::list<std::tuple<int64_t, int64_t>> AMAP;       // the alignment of the last pair found equivalent

namespace detail {

namespace nn = cb::exact::habits::newcomp;

inline std::set<std::uint32_t> words_of(const py::set<int64_t>& fx) {
    std::set<std::uint32_t> out;
    for (const int64_t k : fx) out.insert(cb::compat::int_from_py<std::uint32_t>(k));
    return out;
}

inline std::tuple<bool, py::list<py::str>> answer(const nn::Finding& f) {
    if (!f.alignment.empty()) {
        AMAP.clear();
        for (const auto& [a, b] : f.alignment) AMAP.append(std::make_tuple(int64_t(a), int64_t(b)));
        GWALK.clear();
        for (const std::uint32_t g : f.compiled_starts) GWALK.append(int64_t(g));
    }
    return std::make_tuple(f.same, cb::compat::to_py(f.notes));
}

}  // namespace detail

// the instruction text with its numbers as N
inline py::str mask(const py::str& t) { return cb::compat::to_py(detail::nn::mask(cb::compat::from_py(t))); }

// a relative jump or call -> (length of opcode, displacement) or None
inline std::optional<std::tuple<int64_t, int64_t>> rel(const py::bytes& b) {
    const cb::Bytes raw = cb::compat::from_py(b);
    const auto r = detail::nn::relative(raw);
    if (!r) return std::nullopt;
    return std::make_tuple(int64_t(r->opcode_length), r->displacement);
}

// the original's instruction with the registers renamed; None if it cannot be written (SI and DI have no halves)
inline std::optional<py::str> rename(const py::str& t, const py::dict<py::str, py::str>& mp) {
    std::map<std::string, std::string> renaming;
    for (const py::str& k : mp.keys()) renaming[k.utf8()] = mp.at(k).utf8();
    const auto r = detail::nn::rename(t.utf8(), renaming);
    return r ? std::optional<py::str>(cb::compat::to_py(*r)) : std::nullopt;
}

inline std::tuple<bool, py::list<py::str>> equivalent(const py::bytes& want, const py::bytes& got, const py::set<int64_t>& fx) {
    return cb::compat::guard([&] {
        const cb::Bytes w = cb::compat::from_py(want), g = cb::compat::from_py(got);
        return detail::answer(detail::nn::equivalent(w, g, detail::words_of(fx), cmpp::detail::options()));
    });
}

inline std::tuple<bool, py::list<py::str>> equivalent3(const py::bytes& want, const py::bytes& got, const py::set<int64_t>& fx) {
    return cb::compat::guard([&] {
        const cb::Bytes w = cb::compat::from_py(want), g = cb::compat::from_py(got);
        return detail::answer(detail::nn::equivalent3(w, g, detail::words_of(fx), cmpp::detail::options()));
    });
}

}  // namespace codebreaker::exact::habits::newcomp
