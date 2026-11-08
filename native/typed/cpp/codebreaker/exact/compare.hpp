// codebreaker/exact/compare.hpp -- typed C++ from codebreaker.exact.compare.
//
// compare.py -- a compiled function against the program's bytes at its place.
//
// Every byte must be the program's, except the words a linker fills in; those must come to the
// program's word when the place of what they name is put in. Names whose place is known are in the
// project's SYMBOLS; a name met for the first time is learnt from the program (the place its word
// implies), and the next function that names it is held to that. A word naming the file's own data
// must point, in the program's data segment, at what the file's data has there.
//
// The program's code may be in several segments (the project's code_segments: their image offsets);
// an offset within one wraps at 64K. Its segment words are those of a load at the project's loadseg.
//
//     bad, notes = compare::function(obj, image, name, offset, length, at, project, symbols, learnt,
//                                    decode, text="_TEXT", where=None)
//         text   the object's code segment the function is in
//         where  where(x): the image address of the segment's offset x (each function of the file at its
//                own place); by default the file's code lies together from `at`
//         bad    offsets (in the function) of bytes that differ
//         notes  what is wrong with the linker's words; a note starting with "?" is a warning only
//
// Stand-ins (the Python takes these as duck-typed objects of modules with no C++ port yet):
//   Obj      an OMF/a.out object as omf.read / aout.read return it (the members compare.py reads: seg, fix, pubs)
//   Project  the members of exact.project.Project compare.py reads (dgroup, loadseg, code_base(), relocs())
//   Decoder  platforms.decoder(16).disasm(code, address): the 16-bit x86 instructions of a buffer
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::exact::compare {

// What a fixup names: ("ext", name) an external, ("seg", segment) a segment's start, or (anything, segment).
using Target = std::tuple<py::str, py::str>;
// One fixup: (segment, offset of the word in it, kind "rel"|"ptr"|"base"|"off", target, displacement).
using Fixup = std::tuple<py::str, int64_t, py::str, Target, int64_t>;
// One public: (name, segment, offset).
using Public = std::tuple<py::str, py::str, int64_t>;
// {name: (kind "C" | "D" | "S" | "?", address)}
using Symbols = py::dict<py::str, std::tuple<py::str, int64_t>>;

struct Obj {
    py::dict<py::str, py::bytes> seg;
    py::list<Fixup> fix;
    py::list<Public> pubs;
};

struct Project {
    int64_t dgroup = 0;
    int64_t loadseg = 0;
    std::function<int64_t(int64_t)> code_base;                // the image offset of the code segment `a` is in
    std::function<const py::set<int64_t>&()> relocs;          // the places in the load image the loader adds the load segment to
};

// One decoded instruction (capstone's address, size, op_str and bytes).
struct Insn {
    int64_t address = 0;
    int64_t size = 0;
    py::str op_str;
    py::bytes bytes;
};
using Decoder = std::function<py::list<Insn>(const py::bytes&, int64_t)>;

using Where = std::function<int64_t(int64_t)>;

// the decoded-instruction cache of one call (compare.py's `cache` dict, whose only key is "ins")
struct Cache {
    std::optional<py::list<std::tuple<int64_t, int64_t, py::str>>> ins;
};

// -> (offsets of differing bytes, notes)
std::tuple<py::list<int64_t>, py::list<py::str>> function(
    const Obj& o, const py::bytes& img, const py::str& name, int64_t off, int64_t ln, int64_t at, const Project& prj,
    const Symbols& syms, const Symbols& learn, const Decoder& decode, const py::str& text = py::str("_TEXT"),
    const std::optional<Where>& where = std::nullopt);

// the instruction holding the word at fo addresses memory through a base or index register
bool _indexed(const py::bytes& code, int64_t off, int64_t ln, int64_t fo, Cache& cache, const Decoder& decode);

// bytes that differ alone (an immediate) and each by the same amount: a constant or an enumeration with
// members more or fewer in the program's source than in this one.  -> the note, or None
std::optional<py::str> _shift(const py::bytes& code, const py::bytes& img, int64_t off, int64_t at, const py::list<int64_t>& bad);

// near calls and jumps from the function to another of its file have no fixup (the compiler wrote them):
// with the file's functions at places of their own, each must reach its callee's place
void _calls_within(const py::bytes& code, const py::bytes& img, int64_t off, int64_t ln, int64_t at, int64_t base,
                   const Where& where, const py::set<int64_t>& skip, const Decoder& decode);

bool identical(const py::list<int64_t>& bad, const py::list<py::str>& notes);

}  // namespace codebreaker::exact::compare
