// codebreaker/build/m6502_asm.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/m6502_asm.hpp for the ports that still use
// the old API (source/nes_writer). No logic: conversion and forwarding. Staged under native/cb/compat/shims/ until the exe's
// build is wired for C++20.
//
// Old API: Area, config, build. (_AREA, _number, _split_args, _strip_comment were helpers of the old build(): not carried over.)
//
// The 6502 instruction syntax is decompile/m6502, which is not refactored yet: its `assemble` and `BRANCHES` are called through
// `OldMos6502Encoder` -- COMPAT(py): remove when decompile/m6502 is refactored. The old names are only declared here.
#pragma once

#include "cb/build/m6502_asm.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"

namespace codebreaker::decompile::m6502 {   // COMPAT(py): defined in decompile/m6502.cpp (not yet refactored)
extern const py::set<py::str> BRANCHES;
py::bytes assemble(const py::str& line, int64_t pc, const py::dict<py::str, int64_t>& labels);
}  // namespace codebreaker::decompile::m6502

namespace codebreaker::build::m6502_asm {

// one MEMORY area: (name, start, size, fill)
using Area = std::tuple<py::str, int64_t, int64_t, bool>;

// (memory areas [(name, start, size, fill)], {segment: area})
inline std::tuple<py::list<Area>, py::dict<py::str, py::str>> config(const py::str& text) {
    return cb::compat::guard([&] {
        const cb::build::m6502_asm::LinkConfig c = cb::build::m6502_asm::parse_config(text.utf8());
        py::list<Area> areas;
        for (const auto& a : c.areas) areas.append(Area{py::str(a.name), a.start, a.size, a.fill});
        py::dict<py::str, py::str> segments;
        for (const auto& [segment, area] : c.segment_area) segments.set(py::str(segment), py::str(area));
        return std::make_tuple(areas, segments);
    });
}

class OldMos6502Encoder final : public cb::build::m6502_asm::Encoder6502 {
public:
    cb::Result<cb::Bytes> assemble(std::string_view line, std::uint32_t pc, const cb::build::Labels& labels) const override {
        py::dict<py::str, int64_t> d;
        for (const auto& [name, v] : labels) d.set(py::str(name), v);
        try {
            return cb::compat::from_py(codebreaker::decompile::m6502::assemble(py::str(std::string(line)), pc, d));
        } catch (const py::KeyError& e) {
            return cb::build::UndefinedSymbolError(e.what());
        } catch (const py::Exception& e) {
            return cb::ParseError("", 0, 0, e.what());
        }
    }
    bool is_branch(std::string_view mnemonic) const override {
        return codebreaker::decompile::m6502::BRANCHES.contains(py::str(std::string(mnemonic)));
    }
};

// -> the linked image (bytes). Raises ValueError with the line on an error.
inline py::bytes build(const py::str& source, const py::str& config_text, const py::str& folder = py::str(".")) {
    static const OldMos6502Encoder encoder;
    static const cb::build::m6502_asm::Assembler assembler(encoder);
    try {
        const cb::Bytes image = assembler.build(source.utf8(), config_text.utf8(), cb::fs::from_text(folder.utf8()));
        return cb::compat::to_py(cb::ByteView(image));
    } catch (const cb::Error& e) {
        cb::compat::rethrow_as_py(e);   // an AsmError is a ParseError: the old build raised ValueError with the line
    }
}

}  // namespace codebreaker::build::m6502_asm
