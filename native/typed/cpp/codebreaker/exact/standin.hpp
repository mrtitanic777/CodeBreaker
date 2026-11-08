// codebreaker/exact/standin.hpp -- typed C++ of codebreaker.exact.standin
//
// standin.py -- a source file's object module for the link of the whole program, with stand-ins.
//
// A module is compiled whole. A function of it that is not yet the program's would be of another length
// and push everything behind it out of place; so, for the link, its code in the object module is
// replaced by THE PROGRAM'S BYTES for that function - a stand-in, with no words for the linker to fill
// (they are filled already) - and the functions behind it are moved, the module's records written
// again. What the linked program then proves is everything else: the order of the files, the data,
// the place of every variable, the library's part, the header and the relocation table. A stand-in
// proves nothing about its function; the ledger is what counts the functions.
//
// Where a project has several toolchains, a function another toolchain's object of the same file
// gives is taken from that object (its names renumbered) before the program's bytes are.
//
//     auto [raw, stand] = standin::rebuild(prj, raw, addrs, end, title, alts, decode);
//         addrs   where the module's functions stand in the program, in order; end: where its code ends
//         stand   the names of the functions that are stand-ins
//
// Typing notes:
//   * `prj` is the interface `Project` below: the members of exact.project.Project that standin.py reads
//     (image, symbols(), relocs(), packed.relocs, loadseg, dgroup, code_base() for compare.function).
//   * The Python's dicts of a fix-up are the struct `Fixup`; the thread table kept from record to record
//     (`thr`) is `Threads`.
//   * compare.function takes the 16-bit decoder as a parameter in the typed compare port; rebuild() passes the
//     one it is given.  x86.disasm uses x86's own decoder: install it with x86::set_decoder first.
//   * `title` is std::optional<str> (None: the module is not renamed); `alts` is a list of object modules (bytes).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/exact/compare.hpp"
#include "py.hpp"

namespace codebreaker::exact::standin {

using Symbols = compare::Symbols;

// exact.project.Project, as standin.py uses it
class Project {
  public:
    virtual ~Project() = default;
    virtual py::bytes image() const = 0;                                // prj.image
    virtual Symbols symbols() const = 0;                                // prj.symbols()
    virtual int64_t loadseg() const = 0;                                // prj.loadseg
    virtual int64_t dgroup() const = 0;                                 // prj.dgroup
    virtual int64_t code_base(int64_t a) const = 0;                     // prj.code_base(a)
    virtual const py::set<int64_t>& relocs() const = 0;                 // prj.relocs()
    // prj.packed.relocs when the project has a packed original (getattr(prj, "packed", None) is truthy), else nullopt
    virtual std::optional<py::list<int64_t>> packed_relocs() const = 0;
};

// one fix-up of a FIXUPP record (a dict in the Python); `seg` and `at` are set when the fix-up is placed
struct Fixup {
    int64_t M = 0;
    int64_t loc = 0;
    int64_t off = 0;
    int64_t fm = 0;
    std::optional<int64_t> fi;
    int64_t tm = 0;
    std::optional<int64_t> ti;
    int64_t P = 0;
    int64_t disp = 0;
    std::optional<int64_t> seg;
    std::optional<int64_t> at;
};

// {(is a target thread, thread number): (method, index or None)}
using Threads = py::dict<std::tuple<int64_t, int64_t>, std::tuple<int64_t, std::optional<int64_t>>>;

py::bytes _ix(int64_t n);
std::tuple<int64_t, int64_t> _idx(const py::bytes& r, int64_t q);

// a FIXUPP record's fixups, the threads resolved -> [Fixup]; thr is kept from record to record
py::list<Fixup> fixups(const py::bytes& r, const Threads& thr);

// the fixups (their places as offsets in the segment) written for a data record that begins at base
py::bytes fixrec(const py::list<Fixup>& fs, int64_t base);

// -> (the object module again, the names of the functions that are stand-ins)
std::tuple<py::bytes, py::list<py::str>> rebuild(const Project& prj, const py::bytes& raw, const py::list<int64_t>& addrs, int64_t end,
                                                 const std::optional<py::str>& title, const py::list<py::bytes>& alts,
                                                 const compare::Decoder& decode);

}  // namespace codebreaker::exact::standin
