// codebreaker/symbols/libraries/coff.hpp -- typed C++ port of codebreaker.symbols.libraries.coff.
//
// coff.py -- the functions of a COFF object or `!<arch>` archive: each one's name, bytes and mask.
//
//     coff::members(data)          (label, bytes) of each object in an archive
//     Coff(data)                   one object: .machine .sections .symbols .relocations(section)
//     coff::functions(data)        [Function] of one object (x86 or PowerPC), for the library matcher
//
// The 32-bit tools' libraries -- Visual C++'s C runtime and MFC, the Xbox 360 XDK -- are archives
// of COFF objects. What the linker fills in is listed by each code section's relocation records,
// so nothing is guessed: on x86 the bytes of the field (two or four), on PowerPC the bits of the
// instruction word the relocation's type fills (the 24-bit distance of a `bl`, a 16-bit half of
// an address), the opcode and registers still compared.
//
// Typing notes:
//   * members() is a generator in the Python; here it returns the whole list (the generator's early
//     `return` on a malformed header just ends the list). An archive member size beyond int64 is read
//     as a saturated number (a huge positive size ends the walk as Python's does); a size field below
//     int64's range raises OverflowError (Python would walk off backwards).
//   * Coff.sections / Coff.symbols hold dicts of fixed keys in the Python: the records Section and
//     Symbol here; the symbol list has None in the slots of auxiliary records: std::optional<Symbol>.
//   * Function is codebreaker.symbols.libraries.Function (a namedtuple "name code mask at" of the
//     package's __init__, which has no C++ port yet): this is its stand-in. Its mask is a bytearray
//     for x86 and a list of ints (the bits left out of each word) for PowerPC: a std::variant.
//   * functions()'s helpers _x86 / _powerpc are generators in the Python; here they return lists.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::symbols::libraries::coff {

extern const int64_t I386;
extern const int64_t POWERPC;
// IMAGE_REL_PPC_* (the low byte of a relocation's type)
extern const int64_t REL_ADDR32, REL_ADDR24, REL_ADDR16, REL_ADDR14;
extern const int64_t REL_REL24, REL_REL14, REL_ADDR32NB;
extern const int64_t REL_SECREL, REL_REFHI, REL_REFLO, REL_PAIR;
extern const int64_t REL_SECRELLO, REL_SECRELHI, REL_GPREL;

// One function of a library: its symbol, its bytes, its mask and its offset in its section.
struct Function {
    py::str name;
    py::bytes code;
    std::variant<py::bytearray, py::list<int64_t>> mask;
    int64_t at = 0;
};

// (label, bytes) of each object in a COFF archive (`!<arch>`); the linker's own members left out.
py::list<std::tuple<py::str, py::bytes>> members(const py::bytes& data);

// A section table entry (the Python's dict size/ptr/relptr/nrel/chars).
struct Section {
    int64_t size = 0;
    int64_t ptr = 0;
    int64_t relptr = 0;
    int64_t nrel = 0;
    int64_t chars = 0;
};

// A symbol table entry (the Python's dict name/value/sec/type/cls).
struct Symbol {
    py::str name;
    int64_t value = 0;
    int64_t sec = 0;    // signed: the section number (0 undefined, -1 absolute, -2 debug)
    int64_t type = 0;
    int64_t cls = 0;
};

// (offset in the section, symbol index, type)
using Relocation = std::tuple<int64_t, int64_t, int64_t>;

// A COFF object: its headers, section table, symbols (None in the slots of auxiliary records)
// and relocations. The structures are little-endian whatever the machine (the Xbox 360's code is
// big-endian, its objects' headers are not).
// The constructor raises StructError where the headers run past the data, as struct.unpack_from does.
class Coff {
public:
    py::bytes data;
    int64_t machine = 0;
    int64_t symptr = 0;
    py::list<Section> sections;
    py::list<std::optional<Symbol>> symbols;

    explicit Coff(const py::bytes& data);

    // [(offset in the section, symbol index, type)]
    py::list<Relocation> relocations(const Section& sec) const;
};

// [Function] of one object: each external function of its code sections (x86), or each
// function a symbol starts (PowerPC); [] for an object of another machine.
py::list<Function> functions(const py::bytes& data);

// (name, code, byte mask, offset) for the external functions of a 32-bit x86 object.
py::list<Function> _x86(const Coff& obj);

// The bits of an instruction word a relocation of this type fills (None: the whole word).
std::optional<int64_t> _field(int64_t typ);

// (name, code, [bits left out of each word], offset in its section) for every function in a
// PowerPC object's code sections.
py::list<Function> _powerpc(const Coff& obj);

}  // namespace codebreaker::symbols::libraries::coff
