// codebreaker/port/asmdata.hpp -- typed C++ port of codebreaker.port.asmdata.
//
// The data of an assembler module, as C (part of the port; see port/asm16 for its code).
//
// A module of data alone is the old linker's business: its bytes were placed, and the names in them were offsets.
// Here they are C - each table given the C program's types where it declares them (and where a pointer of a
// declared one points: that type too), its words that held an offset a pointer now, initialized as the bytes were:
//
//   image(text)        the module's segments: their bytes, which words were the offset of a name (relocations), the
//                      labels' places, each segment's class
//   messages(text)     a table of numbered messages for the C library's __NMSG_TEXT / __FMSG_TEXT (segments of class
//                      MSG / FAR_MSG: a word, the number; the text; a 0), as C: the table and the two lookups
//   typed(text, ct)    the tables of a data module with pointers in them, as C definitions with initializers
//
// Typing notes:
//   * A Seg is shared (std::shared_ptr<Seg>): image() hands back the same objects it filled. Its `data` is a
//     py::bytearray, `reloc` {offset of a word: (symbol, displacement)}, `labels` {name: offset}, `items` the
//     (offset, size) of each DB/DW/DD item in order; `cls` is the segment's class or None.
//   * typed() leaves its notes in `typed_notes` (the Python keeps them as the function attribute typed.notes).
//   * A word of more than 8 bytes cannot be held by an int64_t: it raises OverflowError when its value does not fit
//     (no real data module has one). A word of 8 bytes is read as an unsigned number held in an int64_t.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "py.hpp"

#include "codebreaker/port/asm16.hpp"

namespace codebreaker::port::asmdata {

struct Seg {
    py::str name;
    std::optional<py::str> cls;
    py::bytearray data;
    py::dict<int64_t, std::tuple<py::str, std::optional<int64_t>>> reloc;   // (offset of a word: (symbol, displacement; None for `sym+text`))
    py::dict<py::str, int64_t> labels;                          // (name: offset)
    py::list<std::tuple<int64_t, int64_t>> items;               // ((offset, size) of each DB/DW/DD item, in order)
    Seg(const py::str& name_, const std::optional<py::str>& cls_) : name(name_), cls(cls_) {}
};
using SegP = std::shared_ptr<Seg>;

// a DW/DD item: (number, None) or (displacement, symbol); the displacement is None for `sym+text` that is no number
std::tuple<std::optional<int64_t>, std::optional<py::str>> _value(const py::str& v, const py::dict<py::str, py::str>& equ);

// a DB item: its bytes
py::bytes _bytes(const py::str& item);

// {segment name: Seg} of a module's data
py::dict<py::str, SegP> image(const py::str& text);

// bytes as a C string literal (its 0 left to C)
py::str c_string(const py::bytes& b);

extern const py::dict<py::str, int64_t> BASE_SIZES;

py::set<py::str> publics(const py::str& text);

// what typed() could not do, in its last call (the Python's typed.notes)
extern py::list<py::str> typed_notes;

// the C of a data module whose words hold addresses (a table of chains, of names): each table typed - by the
// C program's declaration of its name, or by what points to it - and initialized as its bytes were. None when
// the module has no addresses in it
std::optional<py::str> typed(const py::str& text, const asm16::CTypes& ct);

// the C of a module of numbered messages (segments of class MSG and FAR_MSG), or None: a table of each - the
// near ones __NMSG_TEXT's, the far ones __FMSG_TEXT's (their numbers are their own: the same number in both is
// two messages)
std::optional<py::str> messages(const py::str& text);

}  // namespace codebreaker::port::asmdata
