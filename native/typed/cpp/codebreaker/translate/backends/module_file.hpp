// codebreaker/translate/backends/module_file.hpp -- C++17 port of codebreaker.translate.backends.module_file.
//
// module_file.py -- L2 to the source file people read: a module's functions, each under its status,
// the status at the head.
//
//     auto files = module_file::c(doc, facts, split);   // [(file name, text)]: <stem>.c, or <stem>_1.c ... when split
//     py::str text = module_file::asm_(doc, facts);     // a driver's or a real-mode program's one assembly file
//
// (`asm` is a C++ keyword: the function is asm_, as the back end's namespace is.)
//
// The back end source/module_writer.py writes through. The module is one L2 tree
// (docs/IR_UNIFICATION.md): for C, a Module whose units are its functions in address order --
//
//     MethodDecl      a function with a reconstruction: `text` its C, `comments` [its status line]
//     Comment         a function with none: its status line alone
//
// -- and for a driver or a real-mode program a Module of one MachineFragment, its whole source, which
// the assembly back end (backends/asm.py) writes. What is said at the head is not the tree's but the
// binary's: the per-binary facts the function store and the build give (`facts`):
//
//     name, stem          the module's file name and its stem
//     date, digest        when, and under which verifier settings
//     order, titles       the status classes in the order they are counted, and each one's title
//     counts, total       functions in each class, and all of them
//     kind                the lane (pe, ne, powerpc, le, dos)
//     whole               the whole module's build, or None: linked, error, exports_same, exports_total,
//                         rejected, absent, built (the image's path)
//     real                how many imports by number are written with their real names
//     header              whether <stem>.h is written (the file includes it)
//     how                 (assembly) what the judge said of the module as a whole
//     rows                (assembly) [(address, name, title, what)] each function not verified
//
// The Python documents and facts are dicts with fixed keys, so here they are records: a Facts, a
// Whole, a Row, and for the C document a Unit and a Document of its own (the C units carry
// `comments`, which the assembly back end's units do not). The assembly file takes the assembly
// back end's own asm_::Document.
#pragma once

#include <optional>
#include <tuple>

#include "codebreaker/translate/backends/asm.hpp"
#include "py.hpp"

namespace codebreaker::translate::backends::module_file {

// from codebreaker.translate.backends import asm as _asm
namespace backend_asm = ::codebreaker::translate::backends::asm_;

// facts["whole"]: how the whole module built (present = a non-empty dict)
struct Whole {
    bool linked = false;
    int64_t exports_same = 0;
    int64_t exports_total = 0;
    py::str error;
    int64_t rejected = 0;
    int64_t absent = 0;
    py::str built;
};

// one of facts["rows"]: (address, name, title, what)
struct Row {
    py::str va;
    py::str name;
    py::str title;
    py::str what;
};

// the per-binary facts
struct Facts {
    py::str name;
    py::str stem;
    py::str date;
    py::str digest;
    py::list<py::str> order;
    py::dict<py::str, py::str> titles;
    py::dict<py::str, int64_t> counts;
    int64_t total = 0;
    py::str kind;
    std::optional<Whole> whole;
    int64_t real = 0;
    bool header = false;
    std::optional<py::str> how;
    py::list<Row> rows;
};

// One unit of the C document (a dict in the Python); an absent key is std::nullopt.
struct Unit {
    std::optional<py::str> node;
    std::optional<py::list<py::str>> comments;
    std::optional<py::str> text;
};

// The C document: a Module whose units are the functions.
struct Document {
    std::optional<py::str> node;
    py::list<Unit> units;
};

// The status lines of the classes, each `lead` + title, count and share.
py::list<py::str> _counts(const Facts& facts, const py::str& lead);

// The head of the C file: the status, how the module builds, what the labels mean.
py::list<py::str> banner(const Facts& facts);

// One function's part of the file: its status line as a comment, then its C.
py::str unit(const Unit& u);

// The module's C, as files: [(file name, text)] -- one, or parts of at most `split` functions
// each (the status in the first).
py::list<std::tuple<py::str, py::str>> c(const Document& doc, const Facts& facts, int64_t split = 0);

// The head of a driver's or a real-mode program's assembly file, as comments.
py::list<py::str> asm_banner(const Facts& facts);

// A driver's or a real-mode program's one assembly file: the status at its head as comments,
// then the module's source (`; (no source could be written)` when it has none).
py::str asm_(const backend_asm::Document& doc, const Facts& facts);

}  // namespace codebreaker::translate::backends::module_file
