// codebreaker/symbols/prototypes.hpp -- compat shim for cb/symbols/prototypes.hpp (docs/CPP_DESIGN.md 6.3). The old types
// in, the new reader called, the old types out; the old knowledge base (platforms win32 and win16, kind "prototype")
// still keeps the table. Deleted with its last old-layer consumer.
//
//     auto p = prototypes::of("GetDlgItem");     // {returns "HWND", convention "stdcall", marker "WINAPI",
//                                                //  parameters [["HWND","hDlg"],["int","nIDDlgItem"]], header "winuser.h"}
//     prototypes::of("GETDLGITEM", 16);          // the 16-bit declaration (found whatever the capitals)
//     prototypes::declaration("LZRead", 16);     // "int WINAPI LZRead(HFILE, void FAR *, int)"
//     prototypes::spelling("GETDLGITEM");        // "GetDlgItem"    (16-bit name tables are all capitals)
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

// The prototype of a Windows function, or None. Capitals do not matter. `bits` 32 or 16 (KeyError for another).
std::optional<Prototype> of(const py::str& name, int64_t bits = 32);

// The name as the headers write it (`GETDLGITEM` -> `GetDlgItem`, `_WSPRINTF` -> `wsprintf`); unchanged when they do not.
py::str spelling(const py::str& name);

// The declaration as one line of C, or None.
std::optional<py::str> declaration(const py::str& name, int64_t bits = 32);

}  // namespace codebreaker::symbols::prototypes
