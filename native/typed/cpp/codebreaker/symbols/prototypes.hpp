// codebreaker/symbols/prototypes.hpp -- typed C++ port of codebreaker.symbols.prototypes.
//
// prototypes.py -- how the Windows headers declare a function: result, convention, parameters.
//
//     auto p = prototypes::of("GetDlgItem");     // {returns "HWND", convention "stdcall", marker "WINAPI",
//                                                //  parameters [["HWND","hDlg"],["int","nIDDlgItem"]], header "winuser.h"}
//     prototypes::of("GETDLGITEM", 16);          // the 16-bit declaration (found whatever the capitals)
//     prototypes::declaration("LZRead", 16);     // "int WINAPI LZRead(HFILE, void FAR *, int)"
//     prototypes::spelling("GETDLGITEM");        // "GetDlgItem"    (16-bit name tables are all capitals)
//
// symbols/api_arguments.py answers one question about an import -- how many bytes of arguments it removes --
// from the exporting module's code. This is the rest of the declaration, for reading: what the function
// returns and what each parameter is.
//
// Read from the headers reconstructions are compiled against (OpenWatcom's h/nt and h/win), so a declaration
// given here is one the compiler accepts. Those headers leave parameters unnamed; where Microsoft's own
// headers of the time are at hand (the corpus's reference folder) and declare the same number of parameters,
// their parameter names are added, and functions only they declare are taken from them. The headers are read
// once; the table is kept in the knowledge base (platforms win32 and win16, kind "prototype": knowledge/)
// and made again when a header folder changes.
//
// Reading the headers is C++ (native/symbols/prototypes.cc, cbsymbols.dll); symbols/native.hpp is its shim,
// and this is the table over it.
//
// Notes on the port:
//   * A prototype is a record of five fields (the Python's dict with those keys): struct Prototype.
//   * The table is {"32": {name: prototype}, "16": {...}, "spelling": {NAME: name}}: struct Table.
//   * _folders() is {"32": [folders], "16": [folders]}: struct Folders.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::symbols::prototypes {

constexpr int64_t REV = 6;

// One declaration: how a header declares a function.
struct Prototype {
    py::str returns;
    py::str convention;
    py::str marker;                                      // WINAPI, WINAPIV, ...
    py::list<std::tuple<py::str, py::str>> parameters;   // (type, name)
    py::str header;
};

// {"32": [folders], "16": [folders]} -- the compiler's headers first, then Microsoft's.
struct Folders {
    py::list<py::str> w32, w16;
};

// {"32": {name: prototype}, "16": {...}, "spelling": {NAME: name}}
struct Table {
    py::dict<py::str, Prototype> w32, w16;
    py::dict<py::str, py::str> spelling;
};

Folders _folders();

py::str _stamp(const Folders& folders);

// The table, made once (and kept in the knowledge base).
const Table& table();

// The prototype of a Windows function, or None. Capitals do not matter. `bits` 32 or 16 (KeyError for
// another, as the Python's table[str(bits)] is).
std::optional<Prototype> of(const py::str& name, int64_t bits = 32);

// The name as the headers write it (`GETDLGITEM` -> `GetDlgItem`, `_WSPRINTF` -> `wsprintf`); unchanged
// when they do not.
py::str spelling(const py::str& name);

// The declaration as one line of C, or None.
std::optional<py::str> declaration(const py::str& name, int64_t bits = 32);

}  // namespace codebreaker::symbols::prototypes
