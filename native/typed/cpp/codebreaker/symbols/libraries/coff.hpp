// codebreaker/symbols/libraries/coff.hpp -- compat shim for cb/symbols/libraries/coff.hpp (docs/CPP_DESIGN.md 6.3). The old
// records in, the new reader called, the old records out; header-only. Deleted with its last old-layer consumer
// (symbols/runtime_library, verify/authored/match).
//
//     coff::members(data)          (label, bytes) of each object in an archive
//     Coff(data)                   one object: .machine .sections .symbols .relocations(section)
//     coff::functions(data)        [Function] of one object (x86 or PowerPC), for the library matcher
//
//   * The Python's dict-shaped section and symbol records are `Section` and `Symbol`; `symbols` has None (nullopt) in the
//     slots of auxiliary records, as before. The Coff constructor raises StructError where the headers run past the data.
//   * A function's mask is a bytearray for x86 and a list of ints (the bits left out of each word) for PowerPC.
//   * The generators' helpers (_x86, _powerpc, _field) have no callers and are not carried over.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/symbols/libraries/coff.hpp"
#include "py.hpp"

namespace codebreaker::symbols::libraries::coff {

inline constexpr int64_t I386 = cb::symbols::libraries::coff::kI386;
inline constexpr int64_t POWERPC = cb::symbols::libraries::coff::kPowerPc;
// IMAGE_REL_PPC_* (the low byte of a relocation's type)
inline constexpr int64_t REL_ADDR32 = 0x2, REL_ADDR24 = 0x3, REL_ADDR16 = 0x4, REL_ADDR14 = 0x5;
inline constexpr int64_t REL_REL24 = 0x6, REL_REL14 = 0x7, REL_ADDR32NB = 0xA;
inline constexpr int64_t REL_SECREL = 0xB, REL_REFHI = 0x10, REL_REFLO = 0x11, REL_PAIR = 0x12;
inline constexpr int64_t REL_SECRELLO = 0x13, REL_SECRELHI = 0x14, REL_GPREL = 0x15;

// One function of a library: its symbol, its bytes, its mask and its offset in its section.
struct Function {
    py::str name;
    py::bytes code;
    std::variant<py::bytearray, py::list<int64_t>> mask;
    int64_t at = 0;
};

// (label, bytes) of each object in a COFF archive (`!<arch>`); the linker's own members left out.
inline py::list<std::tuple<py::str, py::bytes>> members(const py::bytes& data) {
    const cb::Bytes raw = cb::compat::from_py(data);
    py::list<std::tuple<py::str, py::bytes>> out;
    for (const auto& m : cb::symbols::libraries::coff::members(raw)) {
        out.append(std::make_tuple(cb::compat::to_py(m.label), cb::compat::to_py(m.body)));
    }
    return out;
}

// A section table entry.
struct Section {
    int64_t size = 0;
    int64_t ptr = 0;
    int64_t relptr = 0;
    int64_t nrel = 0;
    int64_t chars = 0;
};

// A symbol table entry.
struct Symbol {
    py::str name;
    int64_t value = 0;
    int64_t sec = 0;    // signed: the section number (0 undefined, -1 absolute, -2 debug)
    int64_t type = 0;
    int64_t cls = 0;
};

// (offset in the section, symbol index, type)
using Relocation = std::tuple<int64_t, int64_t, int64_t>;

class Coff {
public:
    py::bytes data;
    int64_t machine = 0;
    int64_t symptr = 0;
    py::list<Section> sections;
    py::list<std::optional<Symbol>> symbols;

    explicit Coff(const py::bytes& d) : data(d), raw_(cb::compat::from_py(d)) {
        cb::compat::guard([&] {
            const auto obj = cb::symbols::libraries::coff::Object::parse(raw_);
            machine = obj.machine();
            symptr = obj.symbol_table_offset();
            for (const auto& s : obj.sections()) sections.append(Section{s.size, s.ptr, s.relptr, s.nrel, s.chars});
            for (const auto& s : obj.symbols()) {
                if (s) symbols.append(Symbol{cb::compat::to_py(s->name), s->value, s->sec, s->type, s->cls});
                else symbols.append(std::nullopt);
            }
        });
    }

    // [(offset in the section, symbol index, type)]
    py::list<Relocation> relocations(const Section& sec) const {
        return cb::compat::guard([&] {
            cb::symbols::libraries::coff::Section s;
            s.size = static_cast<std::uint32_t>(sec.size);
            s.ptr = static_cast<std::uint32_t>(sec.ptr);
            s.relptr = static_cast<std::uint32_t>(sec.relptr);
            s.nrel = static_cast<std::uint16_t>(sec.nrel);
            s.chars = static_cast<std::uint32_t>(sec.chars);
            const auto obj = cb::symbols::libraries::coff::Object::parse(raw_);
            py::list<Relocation> out;
            for (const auto& r : obj.relocations(s)) out.append(Relocation{r.offset, r.symbol, r.type});
            return out;
        });
    }

private:
    cb::Bytes raw_;
};

// [Function] of one object: each external function of its code sections (x86), or each function a symbol starts
// (PowerPC); [] for an object of another machine.
inline py::list<Function> functions(const py::bytes& data) {
    return cb::compat::guard([&] {
        const cb::Bytes raw = cb::compat::from_py(data);
        py::list<Function> out;
        for (const auto& f : cb::symbols::libraries::coff::functions(raw)) {
            Function o;
            o.name = cb::compat::to_py(f.name);
            o.code = cb::compat::to_py(cb::ByteView(f.code));
            if (f.ignore.empty()) o.mask = py::bytearray(cb::compat::to_py(cb::ByteView(f.mask)));
            else o.mask = cb::compat::to_py(f.ignore);
            o.at = f.at;
            out.append(o);
        }
        return out;
    });
}

}  // namespace codebreaker::symbols::libraries::coff
