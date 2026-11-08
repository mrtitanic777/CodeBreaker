// codebreaker/exact/aout.hpp -- C++ port of codebreaker.exact.aout.
//
// aout.py -- a Version 7 a.out object file for the 8086 (XENIX's C compiler), read as omf.read reads OMF.
//
// The file: 16 bytes of header (0407h/0107h magic, text, data, bss and symbol sizes, the entry, a
// word, and whether there is relocation), the text, the data, then - unless the last header word
// says none - one relocation word for every word of the text and of the data (0: none), then the
// symbols (8 characters, a type word, a value word).
//
// A relocation word: bit 0 set, the word is relative to the place after it (a call or jump); bits
// 1-3 what it names (1 the text, 2 the data, 3 the bss, 4 an external symbol, bits 4-14 its number);
// bit 15 set, the operand begins one byte on (the 8086 has operands at odd places too).
//
//     o = aout.read(data)       o.seg o.segs o.filled o.pubs o.ext o.comm o.fix o.raw   (as omf.read)
//         o.fix kinds: 'off' a word that holds a place, 'rel' a place relative to the word's end;
//         targets ('seg', '_TEXT'|'_DATA'|'_BSS') or ('ext', name). A word that names the data or the
//         bss, and a symbol's value, count from that segment's own start (the linker adds the piece's
//         place; LINK 2.4x relinked from these objects shows it). A relative word's own value counts
//         from the start of the text, so its displacement is set to make it count as an OMF
//         self-relative fixup's would.
//
// Typing notes:
//   The Python's `Obj` (an attribute bag) is the struct `Obj`.  Its fixups are tuples
//   (segment, offset, kind, target, displacement); a target is a (kind, name) pair of texts, kind being
//   "seg" or "ext".  `groups` is always empty here (an OMF object fills it: {group: [segment names]}).
#pragma once

#include "py.hpp"

namespace codebreaker::exact::aout {

extern const py::list<int64_t> MAGICS;

bool is_aout(const py::bytes& data);

using Target = std::tuple<py::str, py::str>;                                     // ("seg", "_TEXT") or ("ext", name)
using Fixup = std::tuple<py::str, int64_t, py::str, Target, int64_t>;           // (segment, offset, kind, target, disp)
using SegInfo = std::tuple<py::str, py::str, int64_t, int64_t>;                 // (name, class, size, align)
using Sym = std::tuple<py::str, int64_t, int64_t>;                              // (name, type, value)
using Pub = std::tuple<py::str, py::str, int64_t>;                              // (name, segment, value)

struct Obj {
    py::dict<py::str, py::bytes> seg;
    py::dict<py::str, int64_t> filled;
    py::list<SegInfo> segs;
    py::dict<py::str, py::list<py::str>> groups;
    py::list<Sym> syms;
    py::list<Pub> pubs;
    py::list<py::str> ext;
    py::dict<py::str, std::tuple<int64_t, bool>> comm;
    py::list<Fixup> fix;
    py::bytes raw;
};

Obj read(const py::bytes& data);

}  // namespace codebreaker::exact::aout
