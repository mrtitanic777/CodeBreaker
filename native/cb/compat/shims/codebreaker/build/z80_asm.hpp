// codebreaker/build/z80_asm.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/z80_asm.hpp for the ports that still use the
// old API (build/sm83_asm, build/m68k_asm, source/sms_writer, source/z80_writer). No logic: conversion and forwarding.
// Staged under native/cb/compat/shims/ until the exe's build is wired for C++20.
//
// Old API: AsmError, _strip, _items, build, build_file, build_flat, _size_guess. (_put wrote into a py::bytearray the callers
// never held: not carried over.)
//
// The Z80 instruction syntax is decompile/z80, which is not refactored yet: its `value` and `assemble` are called through
// `OldZ80Encoder` -- COMPAT(py): remove when decompile/z80 is refactored (it then provides a cb::build::Encoder itself). The
// old functions are only declared here; the old z80.cpp defines them. Labels cross the boundary as a py::dict per call, which
// is slow for a large source: this is the transition path.
#pragma once

#include "cb/build/z80_asm.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace codebreaker::decompile::z80 {   // COMPAT(py): defined in decompile/z80.cpp (not yet refactored)
int64_t value(const py::str& expr, const py::dict<py::str, int64_t>& labels, int64_t pc);
py::bytes assemble(const py::str& line, int64_t pc, const py::dict<py::str, int64_t>& labels);
}  // namespace codebreaker::decompile::z80

namespace codebreaker::build::z80_asm {

// class AsmError(ValueError)
struct AsmError : py::ValueError {
    explicit AsmError(const std::string& m) : py::ValueError(m) {}
    explicit AsmError(const char* m) : py::ValueError(m) {}
    explicit AsmError(const py::str& m) : py::ValueError(m.utf8()) {}
    const char* type() const override { return "AsmError"; }
};

// The old z80::value/assemble as a cb::build::Encoder: a py::KeyError (an undefined label) is an UndefinedSymbolError, any
// other old exception a ParseError (the line's own text is added by the assembler).
class OldZ80Encoder final : public cb::build::Encoder {
public:
    cb::Result<std::int64_t> value(std::string_view expr, const cb::build::Labels& labels, std::int64_t pc) const override {
        try {
            return codebreaker::decompile::z80::value(py::str(std::string(expr)), dict(labels), pc);
        } catch (const py::KeyError& e) {
            return cb::build::UndefinedSymbolError(e.what());
        } catch (const py::Exception& e) {
            return cb::ParseError("", 0, 0, e.what());
        }
    }
    cb::Result<cb::Bytes> assemble(std::string_view line, std::int64_t pc, const cb::build::Labels& labels) const override {
        try {
            return cb::compat::from_py(codebreaker::decompile::z80::assemble(py::str(std::string(line)), pc, dict(labels)));
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

// The shim's assembler: the new directive layer over the old encoder.
inline const cb::build::z80_asm::Assembler& assembler() {
    static const OldZ80Encoder encoder;
    static const cb::build::z80_asm::Assembler a(encoder);
    return a;
}

// An AsmError of the new layer is the old AsmError, with the message the old one built ("line N: text (reason)").
template <class F>
decltype(auto) as_old(F&& f) {
    try {
        return std::forward<F>(f)();
    } catch (const cb::build::AsmError& e) {
        throw AsmError(std::string(e.what()));
    } catch (const cb::Error& e) {
        cb::compat::rethrow_as_py(e);
    }
}

inline py::str _strip(const py::str& line) {
    return py::str(std::string(cb::text::rtrim(cb::build::strip_comment(line.utf8()))));
}

// Comma-separated items of a data directive (strings kept whole).
inline py::list<py::str> _items(const py::str& text) {
    py::list<py::str> out;
    const std::string utf8 = text.utf8();
    for (const std::string_view item : cb::build::split_items(utf8, true)) out.append(py::str(std::string(item)));
    return out;
}

inline py::bytes build_file(const py::str& path) {
    return as_old([&] {
        const cb::Bytes rom = assembler().build_file(cb::fs::from_text(path.utf8()));
        return cb::compat::to_py(cb::ByteView(rom));
    });
}

inline py::bytes build(const py::str& source, const py::str& folder = py::str(".")) {
    return as_old([&] {
        const cb::Bytes rom = assembler().build(source.utf8(), cb::fs::from_text(folder.utf8()));
        return cb::compat::to_py(cb::ByteView(rom));
    });
}

// A flat program in the syntax of sjasmplus, pasmo and the Spectrum and MSX assemblers.
// -> (address of the first byte, the bytes up to the last one written; gaps zero).
inline std::tuple<int64_t, py::bytes> build_flat(const py::str& source, const py::str& folder = py::str(".")) {
    return as_old([&] {
        const auto flat = assembler().build_flat(source.utf8(), cb::fs::from_text(folder.utf8()));
        return std::make_tuple(int64_t{flat.start}, cb::compat::to_py(cb::ByteView(flat.bytes)));
    });
}

// An instruction's size on the first pass, when a label it names is not defined yet.
inline py::bytes _size_guess(const py::str& line, int64_t pc) {
    return as_old([&] {
        static const OldZ80Encoder encoder;
        auto r = cb::build::z80_asm::size_guess(encoder, line.utf8(), pc);
        if (!r.ok()) throw py::KeyError(r.error().what());   // (the old one let the encoder's KeyError through)
        return cb::compat::to_py(cb::ByteView(r.value()));
    });
}

}  // namespace codebreaker::build::z80_asm
