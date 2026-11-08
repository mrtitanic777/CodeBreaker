// codebreaker/exact/pascal_join.hpp -- C++17 port of codebreaker.exact.pascal_join.
//
// pascal_join.py -- a Pascal program written in pieces, joined into its compilands, compiled, compared.
//
// A compiland of an MS-Pascal program is one source (a PROGRAM or a MODULE) holding all its procedures;
// the reconstruction keeps it in pieces so that each procedure can be written and checked alone:
//
//     <dir>/GLOBALS.pas    types and variables          <dir>/EXTERNS.pas   procedures of other compilands
//     <dir>/FORWARDS.pas   forward declarations         <dir>/MAIN.pas      the compiland's own block
//     <dir>/pADDR.pas      the procedure P<ADDR> (ADDR: its place in the program); one nested in it is
//                          <dir>/pADDR.d/pNEST.pas, pulled in by a line {+pNEST}
//     a heading marked {STUB} (something to call) or {TODO} (not written yet) is not compared
//
// joined in address order, compiled by the project's toolchain (exact/mspascal.py), each procedure held
// to the program at its place. The compiler of 1981 runs out of memory on a large compiland: it is
// compiled in "parts" runs, each with the procedures of one run in full and the others as empty bodies
// (their headings: something to call).
//
// project.json:
//     "compilands": {NAME: {"dir", "kind": PROGRAM|MODULE, "main": its own block's place, "parts": runs}}
//     "procedures": a JSON file {"0xADDR": {"size", "unit"}} - every procedure of the program, its compiland
//                   ("unit": the compiland's number, "units": {NAME: number})
//     "dgroup": the image offset of DS:0 (it may be below the image: negative)
//
//     pascal_join::run(project) -> rows as ledger.run's
//
// Typing notes (the Python duck-types its `prj`, which has no C++ port; these are the stand-ins):
//   * Project is the members of exact.project.Project that pascal_join reads (dir, cfg, image, symbols(),
//     toolchain()) and of what compare.function reads of it (dgroup, loadseg, code_base(), relocs(): the
//     compare::Project `compare`), plus `decode`, the 16-bit decoder compare::function takes.
//   * cfg is the dict project.json holds, reduced to the keys pascal_join reads: the records Config and
//     CompilandCfg (a key the dict lacks is an empty optional / the Python's default).
//   * toolchain() gives the object whose compile(source bytes, name="T.PAS") answers (the object's bytes, or
//     None, and the console text) and whose helper_data (default 3) cmpp.HDRLEN is set from.
//   * "equivalent" names a function "module:name" that is imported by name in the Python; here
//     it is looked up in `equivalents` (a registry of the project) and then in the habits comparers that have
//     C++ ports (codebreaker.exact.habits.newcomp:equivalent / equivalent3, ...oldcomp:equivalent). A name found
//     in neither gives the ModuleNotFoundError text of the failed import as its "why".
//   * "main" of a compiland is an int or a str (int(text, 0)): std::variant.
//   * A row of run() is (label, procedure name or None, address or None, bytes, verdict, notes).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/exact/compare.hpp"
#include "py.hpp"

namespace codebreaker::exact::pascal_join {

extern const py::str NL;

// what a compiland's entry of project.json's "compilands" holds
struct CompilandCfg {
    py::str dir;
    py::str kind;                                   // "PROGRAM" or "MODULE"
    std::variant<int64_t, py::str> main = int64_t(0);   // its own block's place
    int64_t parts = 1;
    std::optional<py::list<py::str>> over;          // "over": overlay folders (else the project's)
    std::optional<py::list<py::str>> dirs;          // "dirs": else [dir]
    std::optional<int64_t> unit;
    bool stackck = false;
    std::optional<py::str> equivalent;
};

struct Config {
    py::str root = py::str(".");
    py::list<py::str> over;
    std::optional<py::str> map;
    std::optional<py::str> kinds;
    std::optional<py::str> procedures;              // required to run (KeyError when absent)
    py::dict<py::str, int64_t> units;
    std::optional<py::str> equivalent;
    py::dict<py::str, CompilandCfg> compilands;     // in project.json's order
};

struct Toolchain {
    // compile(source, name) -> (the object's bytes or None, the console text)
    std::function<std::tuple<std::optional<py::bytes>, py::str>(const py::bytes&, const py::str&)> compile;
    int64_t helper_data = 3;                        // the bytes after the entry helper's call
};

// an equivalence comparer: (want, got, offsets the linker fills) -> (same, why)
using Equivalent = std::function<std::tuple<bool, py::list<py::str>>(const py::bytes&, const py::bytes&, const py::set<int64_t>&)>;

struct Project {
    py::str dir;
    Config cfg;
    py::bytes image;
    std::function<Toolchain()> toolchain;
    std::function<codebreaker::exact::compare::Symbols()> symbols;
    codebreaker::exact::compare::Project compare;
    codebreaker::exact::compare::Decoder decode;
    py::dict<py::str, Equivalent> equivalents;      // "module:name" -> comparer, tried before the ported habits
};

// {"over": [...], ...} the cfg a Compiland reads of the project, plus its own entry
class Compiland {
  public:
    const Project& prj;
    py::str name;
    CompilandCfg u;
    py::list<py::str> dirs;
    py::str src;
    py::str headline;
    py::str empty;
    py::dict<py::str, py::str> map;                 // (pieces of another program's: where each is in this one)
    bool over;
    // EDITS (FILE|old|new, FILE|+|a line; * every piece, *~ a regular expression) and REMOVED, lowest overlay first
    py::list<std::tuple<py::str, py::str, py::str>> edits;
    py::set<py::str> removed;
    py::dict<int64_t, std::optional<py::str>> kinds;   // (each procedure with the stack check or without)

    Compiland(const Project& prj_, const py::str& name_, const CompilandCfg& u_);

    int64_t addr_of(const py::str& h) const;
    py::str where(const py::str& f) const;
    py::list<py::str> pieces() const;
    // the metacommand that gives the procedure of piece f the stack check it has in the program
    py::str check(const py::str& f) const;
    py::str expand(const py::str& text, const py::str& d, const std::optional<py::str>& own = std::nullopt) const;
    py::str piece(const py::str& f) const;
    py::str rd(const py::str& f) const;
    py::dict<py::str, py::str> result_types() const;
    py::str stub(const py::str& f, const py::dict<py::str, py::str>& rt) const;
    py::list<py::list<py::str>> parts() const;
    py::str source(const py::list<py::str>& part) const;
    py::list<py::str> names_of(const py::list<py::str>& part) const;
};

using Row = std::tuple<py::str, std::optional<py::str>, std::optional<int64_t>, int64_t, py::str, py::list<py::str>>;

py::dict<int64_t, std::tuple<std::optional<int64_t>, std::optional<int64_t>>> _procedures(const Project& prj);   // {address: (size, unit)}

py::list<Row> run(const Project& prj, bool verbose = false);

}  // namespace codebreaker::exact::pascal_join
