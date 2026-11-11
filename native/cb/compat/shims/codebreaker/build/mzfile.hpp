// codebreaker/build/mzfile.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/mzfile.hpp for the ports that still use the
// old API (source/dos_writer). No logic: conversion and forwarding. Staged under native/cb/compat/shims/ until the exe's
// build is wired for C++20.
//
// Old API: BUILT_, template_, compose. Errors reach the old code as the old layer raised them (a short header is
// py::StructError, a table that does not fit py::ValueError).
#pragma once

#include "cb/build/mzfile.hpp"
#include "cb/compat/py_bridge.hpp"

namespace codebreaker::build::mzfile {

namespace detail {
// a header word: a value that does not fit is what struct.pack raised (struct.error)
inline std::uint16_t u16(int64_t v) {
    if (v < 0 || v > 0xFFFF) throw py::StructError("ushort format requires 0 <= number <= 65535");
    return static_cast<std::uint16_t>(v);
}
}  // namespace detail

// (offset, width) of each header field the build decides (Python: _BUILT).
inline const py::list<std::tuple<int64_t, int64_t>> BUILT_ = py::list<std::tuple<int64_t, int64_t>>{
    {0x02, 2}, {0x04, 2}, {0x06, 2}, {0x0A, 2}, {0x0E, 2}, {0x10, 2}, {0x14, 2}, {0x16, 2}};

// The original header with the build's outputs blanked. (`template` is a C++ keyword: template_.)
inline py::bytes template_(const py::bytes& header) {
    return cb::compat::guard([&] {
        const cb::Bytes tpl = cb::build::mzfile::make_template(cb::compat::from_py(header));
        return cb::compat::to_py(cb::ByteView(tpl));
    });
}

// The file: the template's header with the blanks filled -- `relocs` [(offset, segment)] -- then the load module.
inline py::bytes compose(const py::bytes& tpl, const py::bytes& module, const py::list<std::tuple<int64_t, int64_t>>& relocs,
                         int64_t cs, int64_t ip, int64_t ss, int64_t sp, int64_t min_alloc, bool full_last = false) {
    return cb::compat::guard([&] {
        std::vector<cb::build::mzfile::Relocation> list;
        for (const auto& [offset, segment] : relocs.vec()) {
            list.push_back({detail::u16(offset), detail::u16(segment)});
        }
        const cb::build::mzfile::StartState start{detail::u16(cs), detail::u16(ip),
                                                  detail::u16(ss), detail::u16(sp)};
        const cb::Bytes file = cb::build::mzfile::compose(cb::compat::from_py(tpl), cb::compat::from_py(module), list, start,
                                                          detail::u16(min_alloc), full_last);
        return cb::compat::to_py(cb::ByteView(file));
    });
}

}  // namespace codebreaker::build::mzfile
