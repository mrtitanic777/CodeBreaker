// codebreaker/build/stubjoin.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/stubjoin.hpp for the ports that still use
// the old API (source/dos_writer). No logic: conversion and forwarding. Staged under native/cb/compat/shims/ until the
// exe's build is wired for C++20.
//
// Old API: join (and the helpers _u, _align, _pe, _ne, which only join used: not carried over).
#pragma once

#include "cb/build/stubjoin.hpp"
#include "cb/compat/py_bridge.hpp"

namespace codebreaker::build::stubjoin {

// `stub`: the DOS program (an MZ file, its new-header field at 3Ch to be set); `built`: the Windows module as linked. -> the file.
inline py::bytes join(const py::bytes& stub, const py::bytes& built, std::optional<int64_t> at = std::nullopt) {
    return cb::compat::guard([&] {
        std::optional<std::uint32_t> place;
        if (at && *at > 0) place = cb::compat::int_from_py<std::uint32_t>(*at);   // (an at of zero or less was ignored)
        try {
            const cb::Bytes out = cb::build::stubjoin::join(cb::compat::from_py(stub), cb::compat::from_py(built), place);
            return cb::compat::to_py(cb::ByteView(out));
        } catch (const cb::UnsupportedError& e) {
            throw py::ValueError(e.what());   // "not a PE or NE module", "resources aligned differently": the old ValueErrors
        }
    });
}

}  // namespace codebreaker::build::stubjoin
