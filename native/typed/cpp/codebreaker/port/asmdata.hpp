// codebreaker/port/asmdata.hpp -- the old API of codebreaker.port.asmdata over py::, kept for the port that is not
// refactored yet (port/__main__), now over native/cb/port/asmdata.hpp (docs/CPP_DESIGN.md 6.3).
//
// The data of an assembler module, as C (part of the port; see port/asm16 for its code):
//
//   messages(text)     a table of numbered messages for the C library's __NMSG_TEXT / __FMSG_TEXT (segments of class
//                      MSG / FAR_MSG: a word, the number; the text; a 0), as C: the table and the two lookups
//   typed(text, ct)    the tables of a data module with pointers in them, as C definitions with initializers
//
// COMPAT(py): deleted with the last py:: consumer (port/__main__).
#pragma once

#include <optional>

#include "py.hpp"

#include "codebreaker/port/asm16.hpp"

namespace codebreaker::port::asmdata {

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
