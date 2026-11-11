// codebreaker/exact/linkemu.hpp -- C++17 port of codebreaker.exact.linkemu.
//
// linkemu.py -- a linker's work done here, as it did it, for a linker that cannot be run (or is the program itself).
//
//     link24(objects, symbols, log=None) -> the run file's bytes
//
// link24: Microsoft's LINK 2.4x as XENIX C built it (runfile.c), for a small-model program:
//
//   the segments   in the order the modules first name them (crt0's: _TEXT, then the group DGROUP - CRT,
//                  _DATA, _BSS, c_common, STACK); each module's piece of a segment begins where its alignment
//                  says, the group on a paragraph. Names are the linker's without regard to case.
//   the communal   variables (an external with a size; the greatest asked for): c_common. The linker placed
//   variables      them in the order of its table of names, a hash of the names - and most of the names are
//                  not known. So their order is taken from SYMBOLS, the record of where each was found (each
//                  must begin where the one before ends).
//   the run file   a header of 512 bytes with the relocations (the paragraph of DGROUP, each a place in its
//                  segment: the code's or DGROUP's), the segments as far as there is data, the checksum that
//                  makes the file's words add up to FFFF.
//
// objects: [(name, object bytes, odd)] - odd: the code ends on an odd byte (the compiler's filler is not there:
// the module after it is byte aligned). Either an OMF module or a Version 7 a.out (exact/aout.py reads both alike).
//
// Typing notes:
//   * A Piece is shared by the module that holds it and by the lists link24 builds over the modules (the
//     Python's object): py::ref<Piece>.
//   * A fix is (segment, offset, how, target, addend); a target is (kind, name): ("sym", name),
//     ("seg", segment) or ("grp", "") -- the Python's 1-tuple ('grp',) has an empty name here.
//   * A load()ed module is the tuple (pieces, defined, common, fixes, start); link24's `mods` are the
//     records Module (the name first, then those five).
//   * `real` of load is None or a dict: std::optional of the (shared) dict.
//   * `symbols` is {name: (kind, address)} (project.symbols()); `log` is a callable of one text, empty for None.
//   * The Python sets `link24.mods` and `link24.dgroup` (where each module's pieces went: a map); here they
//     are the variables link24_mods and link24_dgroup.
//   * SystemExit(text) is py::SystemExit.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::exact::linkemu {

extern const py::dict<int64_t, int64_t> ALIGN;

// W = lambda b, k: struct.unpack_from("<H", b, k)[0]
int64_t W(const py::bytes& b, int64_t k);

struct Piece {
    py::str seg;
    int64_t align;
    py::bytes data;
    int64_t length;
    bool hasdata;
    std::optional<int64_t> at;   // None until placed
    bool stack = false;
    Piece(const py::str& seg_, int64_t align_, const py::bytes& data_, int64_t length_, bool hasdata_)
        : seg(seg_), align(align_), data(data_), length(length_), hasdata(hasdata_) {}
};

using PieceRef = py::ref<Piece>;
using Pieces = py::dict<py::str, PieceRef>;                          // {segment: Piece}
using Defined = py::dict<py::str, std::tuple<py::str, int64_t>>;     // {name: (segment, offset)}
using Common = py::dict<py::str, int64_t>;                           // {name: length}
using Target = std::tuple<py::str, py::str>;                         // ("sym", name) | ("seg", segment) | ("grp", "")
using Fix = std::tuple<py::str, int64_t, py::str, Target, int64_t>;  // (segment, offset, how, target, addend): how "abs" | "rel" | "seg"
using Fixes = py::list<Fix>;
using Start = std::optional<std::tuple<py::str, int64_t>>;
using Loaded = std::tuple<Pieces, Defined, Common, Fixes, Start>;

struct Module {
    py::str name;
    Pieces pieces;
    Defined defined;
    Common common;
    Fixes fixes;
    Start start;
};

// (pieces {segment: Piece}, defined {name: (segment, offset)}, common {name: length}, fixes, start)
// a fix: (segment, offset, how, target, addend) - how 'abs' (the address), 'rel' (from the end of the word),
// 'seg' (a paragraph); target ('sym', name) or ('seg', segment) or ('grp',)
Loaded load(const py::bytes& raw, bool odd = false, const std::optional<py::dict<py::str, py::str>>& real = std::nullopt);

using Object = std::tuple<py::str, py::bytes, bool>;                 // (name, object bytes, odd)
using Symbols = py::dict<py::str, std::tuple<py::str, int64_t>>;     // {name: (kind, address)}

extern py::list<Module> link24_mods;   // (where each module's pieces went: a map)
extern int64_t link24_dgroup;

py::bytes link24(const py::list<Object>& objects, const Symbols& symbols,
                 const std::function<void(const py::str&)>& log = nullptr);

}  // namespace codebreaker::exact::linkemu
