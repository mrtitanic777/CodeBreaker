// codebreaker/build/m68k_asm.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/m68k_asm.hpp for the ports that still use the old
// API (source/genesis_writer, verify/lanes/genesis). No logic: conversion and forwarding. Staged under native/cb/compat/shims/
// until the exe's build is wired for C++20.
//
// Old API: AsmError (the one of build/z80_asm), _strip, build, _size_guess. (_put wrote into a py::bytearray that only build()
// held: not carried over.)
//
// The 68000 instruction syntax is decompile/m68k, which is not refactored yet: its `_value` and `assemble` are called through
// `OldM68kEncoder` -- COMPAT(py): remove when decompile/m68k is refactored (it then provides a cb::build::Encoder itself). The old
// functions are only declared here; the old m68k.cpp defines them. Labels cross the boundary as a py::dict per call, which is
// slow for a large source: this is the transition path.
#pragma once

#include "z80_asm.hpp"   // the shim beside this one: AsmError, as_old

#include "cb/build/m68k_asm.hpp"

namespace codebreaker::decompile::m68k {   // COMPAT(py): defined in decompile/m68k.cpp (not yet refactored)
int64_t _value(const py::str& expr, const py::dict<py::str, int64_t>& labels, int64_t pc);
py::bytes assemble(const py::str& line, int64_t pc, const py::dict<py::str, int64_t>& labels);
}  // namespace codebreaker::decompile::m68k

namespace codebreaker::build::m68k_asm {

using codebreaker::build::z80_asm::AsmError;

// The old m68k::_value/assemble as a cb::build::Encoder: a py::KeyError (an undefined label) is an UndefinedSymbolError, any
// other old exception a ParseError (the line's own text is added by the assembler).
class OldM68kEncoder final : public cb::build::Encoder {
public:
    cb::Result<std::int64_t> value(std::string_view expr, const cb::build::Labels& labels, std::int64_t pc) const override {
        try {
            return codebreaker::decompile::m68k::_value(py::str(std::string(expr)), dict(labels), pc);
        } catch (const py::KeyError& e) {
            return cb::build::UndefinedSymbolError(e.what());
        } catch (const py::Exception& e) {
            return cb::ParseError("", 0, 0, e.what());
        }
    }
    cb::Result<cb::Bytes> assemble(std::string_view line, std::int64_t pc, const cb::build::Labels& labels) const override {
        try {
            return cb::compat::from_py(codebreaker::decompile::m68k::assemble(py::str(std::string(line)), pc, dict(labels)));
        } catch (const py::KeyError& e) {
            return cb::build::UndefinedSymbolError(e.what());
        } catch (const py::Exception& e) {
            return cb::ParseError("", 0, 0, e.what());
        }
    }

private:
    static py::dict<py::str, int64_t> dict(const cb::build::Labels& labels) {
        py::dict<py::str, int64_t> d;
        for (const auto& [name, v] : labels) d.set(py::str(name), v);
        return d;
    }
};

inline py::str _strip(const py::str& line) { return py::str(std::string(cb::build::m68k_asm::strip(line.utf8()))); }

// The bytes from address 0 to the last one written. AsmError ("line N: text (reason)") for a line that does not assemble.
inline py::bytes build(const py::str& source) {
    return z80_asm::as_old([&] {
        static const OldM68kEncoder encoder;
        static const cb::build::m68k_asm::Assembler assembler(encoder);
        const cb::Bytes image = assembler.build(source.utf8());
        return cb::compat::to_py(cb::ByteView(image));
    });
}

// On the first pass, an instruction naming a label not defined yet.
inline py::bytes _size_guess(const py::str& line, int64_t pc, const py::dict<py::str, int64_t>& labels) {
    return z80_asm::as_old([&] {
        static const OldM68kEncoder encoder;
        cb::build::Labels known;
        for (const auto& [name, v] : labels.items()) known[name.utf8()] = v;
        auto r = cb::build::m68k_asm::size_guess(encoder, line.utf8(), pc, known);
        if (!r.ok()) throw py::KeyError(r.error().what());   // (the old one let the encoder's exception through)
        return cb::compat::to_py(cb::ByteView(r.value()));
    });
}

}  // namespace codebreaker::build::m68k_asm
