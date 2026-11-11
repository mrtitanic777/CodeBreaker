// codebreaker/symbols/libraries/package.hpp -- typed C++ port of codebreaker/symbols/libraries/__init__.py.
//
// libraries -- finding a library's functions in a program that was linked with it.
//
//     auto [kind, found] = libraries::functions(path);   every function of a library or object file:
//                                                        kind 16, 32 (x86) or "powerpc"; found [Function]
//     libraries::fits(function, code, code_mask)         whether `code` starts with the library function
//     Index index(words);                                a program's code, to search for functions in
//     index.find(words, ignore)                          every place a function of words is
//
// A linked program's library functions are the library's bytes except where the linker filled something in --
// an address, the distance of a call -- and the library's own relocation records say where that is. So a library
// function is told by its bytes outside those places (its mask, compare/mask): where a program has the same, it
// has that function, and nobody wrote it.
//
// The readers, one per kind of file, do nothing but read (symbols/libraries/omf, symbols/libraries/coff).
//
// Typing notes:
//   * Function is the namedtuple "name code mask at". Its mask is a byte mask (x86 and 16-bit OMF: one flag per
//     byte) or, for PowerPC, the bits left out of each word (a list of ints): a std::variant, as compare::mask's.
//   * functions() answers (kind, [Function]); kind is 16 or 32 (int) or "powerpc" (str): std::variant<int64_t, str>.
//     coff::Function / omf::Function (the readers' own records) are converted to this one.
//   * Index holds ints (the words of PowerPC code, the only units the callers use); the Python's units may be any
//     hashable run (the bytes of code too), here int64_t.
//   * `fits`'s code_mask is the optional byte mask of `code` (the Python's bytes / bytearray / None); an empty one
//     is as none, as `if code_mask` is.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"

namespace codebreaker::symbols::libraries {

using Mask = std::variant<py::bytes, py::list<int64_t>>;
using Kind = std::variant<int64_t, py::str>;

// One function of a library: its symbol, its bytes, its mask (a byte mask for x86; for PowerPC the bits left out of
// each word) and its offset in its section.
struct Function {
    py::str name;
    py::bytes code;
    Mask mask;
    int64_t at = 0;

    // (a namedtuple compares field by field)
    friend bool operator==(const Function& a, const Function& b) {
        return a.name == b.name && a.code == b.code && a.mask == b.mask && a.at == b.at;
    }
};

// (kind, [Function]) for every function in a library or object file: kind 16 for an OMF file, else the machine of
// its COFF objects -- 32 (x86) or "powerpc".
std::tuple<Kind, py::list<Function>> functions(const py::str& path);

// {machine: kind} of the COFF machines the reader knows
const py::dict<int64_t, Kind>& _KIND();

// Whether `code` (with `code_mask`, the places it has that a loader filled in) starts with the library function: as
// long at least, and the same at every byte neither leaves out.
bool fits(const Function& function, const py::bytes& code, const std::optional<py::bytes>& code_mask = std::nullopt);

// A program's code as a run of units (the words of PowerPC code), indexed by each pair of neighbours: a function is
// looked for where its rarest pair of unmasked units is (a prologue's pair is everywhere; a pair the program does
// not have at all rules it out at once).
class Index {
  public:
    explicit Index(const py::list<int64_t>& units);

    py::list<int64_t> units;
    py::dict<std::tuple<int64_t, int64_t>, py::list<int64_t>> pairs;

    // Every place, in order, where the units `sig` are, outside the bits `ignore` leaves out of each; [] when no two
    // neighbours of it are whole (nothing to look it up by).
    py::list<int64_t> find(const py::list<int64_t>& sig, const py::list<int64_t>& ignore) const;
};

}  // namespace codebreaker::symbols::libraries
