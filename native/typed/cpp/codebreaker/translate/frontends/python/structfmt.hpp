// codebreaker/translate/frontends/python/structfmt.hpp -- COMPAT SHIM over cb/translate/frontends/python/structfmt.hpp
// (docs/CPP_DESIGN.md 6.3). The module is native/cb/translate/frontends/python/structfmt.{hpp,cpp}; this header
// keeps the old py:: API for translate/frontends/python/methods.cpp. No logic of its own.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/translate/frontends/python/structfmt.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::translate::frontends::python::structfmt {

// one item: (code, size, kind, lo, hi). `hi` is unsigned because 'Q' reaches 2**64 - 1, past int64.
using Item = std::tuple<py::str, int64_t, py::str, int64_t, uint64_t>;

namespace detail_shim {
namespace nw = cb::translate::frontends::python::structfmt;
inline Item to_old(const nw::Item& i) {
    return Item(py::str(std::string(1, i.code)), cb::compat::to_py(i.size),
                cb::compat::to_py(nw::kind_word(i.kind)), i.lo, i.hi);
}
inline nw::Item from_old(const Item& i) {
    nw::Item n;
    n.code = std::get<0>(i).utf8().at(0);
    n.size = static_cast<std::uint64_t>(std::get<1>(i));
    const std::string kind = std::get<2>(i).utf8();
    n.kind = kind == "pad"     ? nw::Kind::Pad
             : kind == "bytes" ? nw::Kind::Bytes
             : kind == "bool"  ? nw::Kind::Bool
             : kind == "float" ? nw::Kind::Float
                               : nw::Kind::Int;
    n.lo = std::get<3>(i);
    n.hi = std::get<4>(i);
    return n;
}
inline std::vector<nw::Item> from_old(const py::list<Item>& items) {
    std::vector<nw::Item> out;
    for (const Item& i : items) out.push_back(from_old(i));
    return out;
}
}  // namespace detail_shim

// -> None when the format is refused.
inline std::optional<py::list<Item>> parse(const py::str& fmt) {
    const auto items = cb::translate::frontends::python::structfmt::parse(fmt.utf8());
    if (!items) return std::nullopt;
    py::list<Item> out;
    for (const auto& i : *items) out.append(detail_shim::to_old(i));
    return out;
}

inline int64_t size(const py::list<Item>& items) {
    return cb::compat::to_py(cb::translate::frontends::python::structfmt::size(detail_shim::from_old(items)));
}

// the items that are values (pads are not)
inline py::list<Item> values(const py::list<Item>& items) {
    py::list<Item> out;
    for (const auto& i : cb::translate::frontends::python::structfmt::values(detail_shim::from_old(items)))
        out.append(detail_shim::to_old(i));
    return out;
}

}  // namespace codebreaker::translate::frontends::python::structfmt
