// codebreaker/exact/ledger.hpp -- typed C++ port of codebreaker.exact.ledger.
//
// ledger.py -- every one-function source of a project compiled by its toolchain and put against the program.
//
//     python -m codebreaker exact PROJECT ledger [--learn] [-v] [--write]
//
// Each src/fADDR.c is compiled on its own; its first function is held to the program at ADDR, the others at
// their places in SYMBOLS. A project with several toolchains has each file compiled by each; a function is
// identical when one of them gives its bytes, and the ledger says which did.
// --learn: the places identical functions show are added to SYMBOLS (a name is only learnt from a file all of
// whose placed functions are identical under one toolchain). --write: src/LEDGER is written (otherwise only
// the counts are printed).
//
//     ledger::run(project, learn=false) -> [(file, function, address, length, verdict, notes, by)]
//         verdict: identical | differs | no place | does not compile
//         by:      the labels of the toolchains that give the function's bytes
//
// Typing notes (the Python duck-types its `prj` and `tc`):
//   * `prj` is the interface Project below: the members of exact.project.Project that ledger.py reads (src, dir,
//     exe, path(), cfg.get() for the string keys it asks for, headers(), include, switches, starts(), whole(),
//     image, code_end, toolchains(), symbols(), save_symbols(), sources(), sets()), and what compare.function
//     takes of a project (dgroup, loadseg, code_base(), relocs()) with the 16-bit decoder (platforms.decoder(16),
//     the module-global `_CS` of compare.py) as decoder(). A Toolchain is exact::modules::Toolchain (its
//     compile()). The Python's project with a "compilands" entry hands itself to pascal_join; here it hands the
//     pascal_join::Project that is its view (pascal_project()).
//   * A row is (file, function or None, address or None, length, verdict, notes); a finished row has `by`
//     (the toolchains' labels) after the notes, as a 7-tuple (Row7). `by` is a list of optional text: the
//     Python puts the project's "toolchain" setting there for a compilands project, which may be None.
//   * the files are read as bytes; a console text splits into lines as str.splitlines() does.
//   * not ported: the `__name__` block (there is none): main(prj, argv) is the entry the command calls; it
//     returns 0.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/exact/compare.hpp"
#include "codebreaker/exact/modules.hpp"
#include "codebreaker/exact/pascal_join.hpp"
#include "py.hpp"

namespace codebreaker::exact::ledger {

using Symbols = compare::Symbols;
using Toolchain = modules::Toolchain;

// exact.project.Project, as ledger.py uses it
class Project {
  public:
    virtual ~Project() = default;
    virtual py::str src() const = 0;                                    // prj.src
    virtual py::str dir() const = 0;                                    // prj.dir
    virtual py::str exe() const = 0;                                    // prj.exe: the program's file
    virtual py::str path(const py::str& name) const = 0;                // prj.path(name)
    virtual std::optional<py::str> cfg_str(const py::str& key) const = 0;   // prj.cfg.get(key) (a string entry): "named_by", "toolchain"
    virtual bool has_compilands() const = 0;                            // bool(prj.cfg.get("compilands"))
    virtual pascal_join::Project pascal_project() const = 0;            // the project as pascal_join reads it
    virtual py::dict<py::str, py::bytes> headers() const = 0;           // prj.headers()
    virtual py::list<py::str> include() const = 0;                      // prj.include
    virtual std::optional<py::str> switches() const = 0;                // prj.switches
    virtual py::list<int64_t> starts() const = 0;                       // prj.starts() (ascending)
    virtual bool whole(const py::str& path) const = 0;                  // prj.whole(path)
    virtual py::bytes image() const = 0;                                // prj.image
    virtual int64_t code_end() const = 0;                               // prj.code_end (0: none)
    virtual py::list<std::tuple<py::str, std::shared_ptr<Toolchain>>> toolchains() const = 0;   // prj.toolchains(): (label, toolchain)
    virtual Symbols symbols() const = 0;                                // prj.symbols()
    virtual void save_symbols(const Symbols& syms) const = 0;           // prj.save_symbols(s)
    // prj.sources(): (address or None, source file)
    virtual py::list<std::tuple<std::optional<int64_t>, py::str>> sources() const = 0;
    // prj.sets(): the project's source sets (the project itself, then those of "also"); a set is a Project
    virtual py::list<std::shared_ptr<const Project>> sets() const = 0;
    // for compare.function
    virtual int64_t dgroup() const = 0;
    virtual int64_t loadseg() const = 0;
    virtual int64_t code_base(int64_t a) const = 0;
    virtual const py::set<int64_t>& relocs() const = 0;
    virtual compare::Decoder decoder() const = 0;
};

using By = py::list<std::optional<py::str>>;
// (file, function, address, length, verdict, notes)
using Row = std::tuple<py::str, std::optional<py::str>, std::optional<int64_t>, int64_t, py::str, py::list<py::str>>;
// ... and by
using Row7 = std::tuple<py::str, std::optional<py::str>, std::optional<int64_t>, int64_t, py::str, py::list<py::str>, By>;

// one source file compiled by one toolchain and held to the program; `learnt` gets the places of the functions
// found identical (when all of the file's placed functions are)
py::list<Row> check_file(const Project& prj, const Toolchain& tc, const py::str& path, std::optional<int64_t> at, const Symbols& syms,
                         Symbols& learnt);

// after a line /* SAMEFILE */: empty stand-ins of functions of the program's same file, which a far function
// calls another way than one of another file (push cs, call near) - they are compiled and not compared
py::set<py::str> _stubs(const py::bytes& src);

// the function goes on to the next start (or to a pad of zeros before it: a segment's end)
bool _reaches(const Project& prj, const py::list<int64_t>& starts, int64_t a, int64_t ln);

// one file's rows under each toolchain [(label, rows)] -> its rows, a function identical when one gave it
py::list<Row7> _merge(const py::list<std::tuple<py::str, py::list<Row>>>& per);

py::list<Row7> run(const Project& prj, bool learn = false, bool verbose = false);

void write(const Project& prj, const py::list<Row7>& rows);

int64_t main(const Project& prj, const py::list<py::str>& argv);

}  // namespace codebreaker::exact::ledger
