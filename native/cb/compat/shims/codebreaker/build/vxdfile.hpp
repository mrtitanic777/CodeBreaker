// codebreaker/build/vxdfile.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/vxdfile.hpp for the ports that still use the
// old API (source/driver_writer, source/dos_writer). No logic: conversion and forwarding. Staged under native/cb/compat/shims/
// until the exe's build is wired for C++20.
//
// Old API: PAGE_ALIGN, _u, definitions, relay.
#pragma once

#include "cb/build/fields.hpp"
#include "cb/build/vxdfile.hpp"
#include "cb/compat/py_bridge.hpp"

namespace codebreaker::build::vxdfile {

inline const int64_t PAGE_ALIGN = cb::build::vxdfile::kPageAlign;

// The 32-bit little-endian word of `d` at `at` (struct.error when it does not fit).
inline int64_t _u(const py::bytes& d, int64_t at) {
    return cb::compat::guard([&] { return static_cast<int64_t>(cb::build::u32_at(cb::compat::from_py(d), cb::compat::int_from_py<std::uint64_t>(at), "vxdfile")); });
}

// The .def text the original's non-resident name table was made from. -> str
inline py::str definitions(const py::bytes& original, int64_t lfa) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(cb::build::vxdfile::definitions(cb::compat::from_py(original), cb::compat::int_from_py<std::uint32_t>(lfa)));
    });
}

// The LE of `built` (a whole VxD file) laid out to start at file offset `at` (a multiple of 16) as Microsoft's LINK lays it
// out. -> the bytes from `at` on.
inline py::bytes relay(const py::bytes& built, int64_t at, const py::bytes& resource = py::bytes()) {
    return cb::compat::guard([&] {
        const cb::Bytes out = cb::build::vxdfile::relay(cb::compat::from_py(built), cb::compat::int_from_py<std::uint32_t>(at),
                                                        cb::compat::from_py(resource));
        return cb::compat::to_py(cb::ByteView(out));
    });
}

}  // namespace codebreaker::build::vxdfile
