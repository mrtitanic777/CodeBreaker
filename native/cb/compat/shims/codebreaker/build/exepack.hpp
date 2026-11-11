// codebreaker/build/exepack.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/exepack.hpp for the ports that still use the
// old API (formats/dos, source/dos_writer). No logic: conversion and forwarding. Staged under native/cb/compat/shims/ until
// the exe's build is wired for C++20.
//
// Old API: MESSAGE, FILL, COPY, Fill, Layout, layout, pack.
#pragma once

#include "cb/build/exepack.hpp"
#include "cb/compat/py_bridge.hpp"

namespace codebreaker::build::exepack {

inline const py::bytes MESSAGE = py::bytes("Packed file is corrupt");
inline const int64_t FILL = 0xB0;
inline const int64_t COPY = 0xB2;

// A fill: (offset in the program, length, the byte).
using Fill = std::tuple<int64_t, int64_t, int64_t>;

class Layout {
public:
    py::list<Fill> fills;     // [(offset, length, byte)] in the program, ascending
    py::bytes stub_code;      // the unpacker's code and message: what follows the header words
    int64_t size = 0;         // the program's size in bytes (paragraphs * 16)
    int64_t words = 0;        // 8, or 9 in the later header (a skip count before "RB")

    Layout(const py::list<Fill>& fills_, const py::bytes& stub_code_, int64_t size_, int64_t words_)
        : fills(fills_), stub_code(stub_code_), size(size_), words(words_) {}
};

// How a stored load module was packed. ValueError when it is not EXEPACK (struct.error for a module too short).
inline Layout layout(const py::bytes& module, int64_t cs, int64_t ip) {
    return cb::compat::guard([&] {
        const cb::build::exepack::Layout l = cb::build::exepack::layout(
            cb::compat::from_py(module), cb::compat::int_from_py<std::uint16_t>(cs), cb::compat::int_from_py<std::uint16_t>(ip));
        py::list<Fill> fills;
        for (const auto& f : l.fills) fills.append(Fill{f.offset, f.length, f.byte});
        return Layout(fills, cb::compat::to_py(cb::ByteView(l.stub_code)), l.size, l.words);
    });
}

// The program `image` stored packed, with a header of `words` words. -> (stored load module, (cs, ip, ss, sp) of the file's header)
inline std::tuple<py::bytes, std::tuple<int64_t, int64_t, int64_t, int64_t>> pack(
    const py::bytes& image, const py::list<int64_t>& relocs, const std::tuple<int64_t, int64_t>& entry,
    const std::tuple<int64_t, int64_t>& stack, const py::list<Fill>& fills, const py::bytes& stub_code, int64_t words = 8) {
    return cb::compat::guard([&] {
        namespace cc = cb::compat;
        std::vector<std::uint32_t> relocations;
        for (int64_t r : relocs.vec()) relocations.push_back(cc::int_from_py<std::uint32_t>(r));
        std::vector<cb::build::exepack::Fill> list;
        for (const auto& [offset, length, byte] : fills.vec()) {
            list.push_back({cc::int_from_py<std::uint32_t>(offset), cc::int_from_py<std::uint32_t>(length), cc::int_from_py<std::uint8_t>(byte)});
        }
        const cb::Far16 e{cc::int_from_py<std::uint16_t>(std::get<0>(entry)), cc::int_from_py<std::uint16_t>(std::get<1>(entry))};
        const cb::Far16 s{cc::int_from_py<std::uint16_t>(std::get<0>(stack)), cc::int_from_py<std::uint16_t>(std::get<1>(stack))};
        const cb::build::exepack::Packed p = cb::build::exepack::pack(cc::from_py(image), relocations, e, s, list, cc::from_py(stub_code),
                                                                      cc::int_from_py<unsigned>(words));
        return std::make_tuple(cc::to_py(cb::ByteView(p.module)),
                               std::make_tuple(int64_t{p.registers.cs}, int64_t{p.registers.ip}, int64_t{p.registers.ss},
                                               int64_t{p.registers.sp}));
    });
}

}  // namespace codebreaker::build::exepack
