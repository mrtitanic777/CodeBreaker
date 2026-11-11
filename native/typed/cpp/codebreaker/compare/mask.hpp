// codebreaker/compare/mask.hpp -- SHIM over cb/compare/mask.hpp (docs/CPP_DESIGN.md 6.3): the old types in, the new
// functions called, the old types out. No logic; deleted with its last py:: consumer.
//
// Old callers hold a mask as py::bytes (byte flags) or py::list<int64_t> (flags, or the bits left out of each word).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "cb/compare/mask.hpp"
#include "cb/compat/py_bridge.hpp"

namespace codebreaker::compare::mask {

using Mask = std::variant<py::bytes, py::list<int64_t>>;
using MaybeMask = std::optional<Mask>;

namespace shim {
// A mask as byte flags (a list's items: non-zero is left out).
inline cb::Bytes flags(const MaybeMask& m) {
    cb::Bytes out;
    if (!m) return out;
    if (const py::bytes* b = std::get_if<py::bytes>(&*m)) return cb::compat::from_py(*b);
    for (int64_t v : std::get<py::list<int64_t>>(*m)) out.push_back(v != 0 ? 1 : 0);
    return out;
}
// A mask as the bits left out of each word.
inline std::vector<std::uint64_t> bits(const MaybeMask& m) {
    std::vector<std::uint64_t> out;
    if (!m) return out;
    if (const py::bytes* b = std::get_if<py::bytes>(&*m)) {
        for (unsigned char c : b->raw()) out.push_back(c);
    } else {
        for (int64_t v : std::get<py::list<int64_t>>(*m)) out.push_back(static_cast<std::uint64_t>(v));
    }
    return out;
}
inline std::vector<std::uint64_t> words(const py::list<int64_t>& l) {
    std::vector<std::uint64_t> w;
    for (int64_t v : l) w.push_back(static_cast<std::uint64_t>(v));
    return w;
}
template <class Container>
std::vector<std::int64_t> offsets(const Container& c) {
    std::vector<std::int64_t> out;
    for (int64_t v : c) out.push_back(v);
    return out;
}
inline py::bytearray array(const cb::Bytes& b) { return py::bytearray(cb::compat::to_py(cb::ByteView(b))); }
inline std::vector<cb::Bytes> held(const py::list<MaybeMask>& masks) {
    std::vector<cb::Bytes> out;
    for (const MaybeMask& m : masks) out.push_back(flags(m));
    return out;
}
inline std::vector<cb::compare::mask::ByteMask> views(const std::vector<cb::Bytes>& held) {
    std::vector<cb::compare::mask::ByteMask> out;
    for (const cb::Bytes& b : held) out.emplace_back(b);
    return out;
}
}  // namespace shim

inline py::bytearray of(const py::list<int64_t>& offsets, int64_t size) {
    return shim::array(cb::compare::mask::of(shim::offsets(offsets), static_cast<std::size_t>(size)));
}
inline py::bytearray of(const py::set<int64_t>& offsets, int64_t size) {
    return shim::array(cb::compare::mask::of(shim::offsets(offsets), static_cast<std::size_t>(size)));
}
inline py::bytes blank(const py::bytes& code, py::list<int64_t> offsets) {
    return cb::compat::to_py(cb::ByteView(cb::compare::mask::blank(cb::compat::from_py(code), shim::offsets(offsets))));
}
inline py::bytes blank(const py::bytes& code, const py::set<int64_t>& offsets) {
    return cb::compat::to_py(cb::ByteView(cb::compare::mask::blank(cb::compat::from_py(code), shim::offsets(offsets))));
}

inline bool same(const py::bytes& a, const py::bytes& b, const py::list<MaybeMask>& masks) {
    auto held = shim::held(masks);
    return cb::compare::mask::same(cb::compat::from_py(a), cb::compat::from_py(b), shim::views(held));
}

inline py::list<int64_t> differing(const py::bytes& a, const py::bytes& b, const py::list<MaybeMask>& masks) {
    auto held = shim::held(masks);
    py::list<int64_t> out;
    for (std::size_t i : cb::compare::mask::differing(cb::compat::from_py(a), cb::compat::from_py(b), shim::views(held))) {
        out.append(static_cast<int64_t>(i));
    }
    return out;
}

inline bool same_words(const py::list<int64_t>& a, const py::list<int64_t>& b, const py::list<int64_t>& ignore) {
    return cb::compare::mask::same_words(shim::words(a), shim::words(b), shim::words(ignore));
}

inline std::tuple<py::str, int64_t, int64_t, int64_t> distance(const py::bytes& a, const py::bytes& b,
                                                               const MaybeMask& ma = std::nullopt,
                                                               const MaybeMask& mb = std::nullopt, int64_t unit = 1,
                                                               const py::str& byteorder = py::str("little")) {
    namespace m = cb::compare::mask;
    cb::Bytes ba = cb::compat::from_py(a), bb = cb::compat::from_py(b);
    m::DistanceResult r;
    if (unit == 1) {
        cb::Bytes fa = shim::flags(ma), fb = shim::flags(mb);
        r = m::distance(ba, bb, fa, fb);
    } else {
        if (byteorder != py::str("little") && byteorder != py::str("big")) {
            throw py::ValueError("byteorder must be either 'little' or 'big'");
        }
        auto wa = shim::bits(ma), wb = shim::bits(mb);
        try {
            r = m::distance_words(ba, bb, static_cast<std::size_t>(unit),
                                  byteorder == py::str("big") ? m::ByteOrder::Big : m::ByteOrder::Little, wa, wb);
        } catch (const cb::UsageError& e) {
            throw py::ValueError(e.what());
        }
    }
    return {py::str(std::string(m::to_text(r.verdict))), static_cast<int64_t>(r.differing),
            static_cast<int64_t>(r.compared), r.first ? static_cast<int64_t>(*r.first) : -1};
}

}  // namespace codebreaker::compare::mask
