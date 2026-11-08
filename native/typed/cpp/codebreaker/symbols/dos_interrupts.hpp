// codebreaker/symbols/dos_interrupts.hpp -- C++ from codebreaker.symbols.dos_interrupts.
#pragma once

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::symbols::dos_interrupts {

// dos_interrupts.py -- names for the services a real-mode program asks of DOS and the BIOS.
//
//     Service s = dos_interrupts::name(0x21, 0x3D);        s.name "DOS_OpenFile"  s.arguments "AL = access mode, ..."
//     Service s = dos_interrupts::name(0x2F, 0x16, 0x00);  s.name "Multiplex_WindowsEnhancedModeCheck"
//     s.exact        false when only the interrupt (or only its group) could be named
//     text = dos_interrupts::annotate(text, interrupts)    C text with each call's service noted
//
// A program calls the system with `int n`; which service it wants is a number in AH (for some a
// second one in AL, for the mouse the whole of AX). The table (dos_interrupts.json, written for
// this project from the documented interfaces: interrupt -> AH -> name and register arguments,
// with an "al" table for services that take a subfunction) covers DOS (21h, 20h-2Fh), the BIOS
// (10h-1Ah), the multiplex interrupt (2Fh), the mouse (33h) and expanded memory (67h).
//
// The decompiler shows an interrupt as `swi(0x21)` without the registers, so the service number
// comes from the machine code (codebreaker.analyze.dos_functions finds the constant loaded before
// each `int`). annotate() writes what is known in front of each function, and inline where every
// call of that interrupt in the function asks for the same service.

// A service: its name, its register arguments, and whether the service itself was named.
class Service {
public:
    py::str name;
    py::str arguments;
    bool exact = false;

    Service(const py::str& name, const py::str& arguments, bool exact);
    py::str __repr__() const;
};

// One service of an interrupt: (name, register arguments), a two-item list in the JSON.
using Hit = std::tuple<py::str, py::str>;

// One interrupt's row of dos_interrupts.json (a record: the keys the table uses).
struct Entry {
    py::str name;                                        // "name"
    py::str arguments;                                   // "arguments" ("" when absent: entry.get("arguments", ""))
    bool has_services = false;                           // "services" in entry
    py::dict<py::str, Hit> services;                     // "services": {AH as two hex digits: hit}
    std::optional<py::str> select;                       // "select": "AX" for the mouse
    py::dict<py::str, py::dict<py::str, Hit>> al;        // "al": {AH: {AL: hit}}
};

// One `int` instruction of codebreaker.analyze.dos_functions (not ported yet): the fields these
// functions read of it.
struct Interrupt {
    int64_t number = 0;
    std::optional<int64_t> ah;
    std::optional<int64_t> al;
    int64_t address = 0;
};

// Where dos_interrupts.json is read from (the Python asks the knowledge base for the "dos"/"interrupt"
// table, which is that file's rows).
extern py::str TABLE_PATH;

// The table of a given file, parsed (ValueError when it is not the table's shape).
py::dict<int64_t, Entry> load_table(const py::str& path);

// {interrupt number: entry of dos_interrupts.json}, read once.
py::dict<int64_t, Entry> table();

// The service an `int number` asks for, given what is known of AH and AL. Always answers: an
// interrupt outside the table is `int_NNh`, a service outside it `<group>_AH_NNh`; `exact` says
// whether the service itself was named.
Service name(int64_t number, std::optional<int64_t> ah = std::nullopt, std::optional<int64_t> al = std::nullopt);

// One line for an Interrupt of dos_functions: `int 21h AH=3Dh  DOS_OpenFile(AL = ...)`.
py::str describe(const Interrupt& interrupt);

// A function's C text with its interrupt calls named. `interrupts` are those of that function, in
// address order. A comment in front lists each; `swi(0xNN)` gets its service inline only where all
// the function's calls of that interrupt ask for the same one.
py::str annotate(const py::str& text, const py::list<Interrupt>& interrupts);

// python -m codebreaker dos-interrupts [<interrupt> [<AH> [<AL>]]]
// Name a service (numbers in hex), or with no arguments say how many are in the table.
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::symbols::dos_interrupts
