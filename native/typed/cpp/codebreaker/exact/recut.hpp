// codebreaker/exact/recut.hpp -- typed C++ port of codebreaker.exact.recut.
//
// recut.py -- a module's code records cut again, as the original program's order of relocations shows.
//
// The linker lists a program's relocations in the order it meets the fixups that need them: each data record's
// fixups in the order the record lists them, the records in their order. A compiler cut a file's code into
// records by the source's lines and listed a record's fixups as it pleased (Microsoft C: the last first); a
// source put together from pieces does not cut where the original's did. The bytes do not depend on it -- the
// relocation table does (and so EXEPACK's). So for the link, a code segment is written again in records cut
// where the original's relocations show one ended (where the next lies after the one before: within a record
// they run down), each record listing its relocations in the original's order. It proves nothing about the
// source's lines.
//
//     recut::recut(project, raw) -> the object module's bytes (unchanged when its relocations are not the program's)
//     project "recut": true asks the link stage for it
//
// Typing notes:
//   * `prj` is a duck-typed exact.project.Project: Project here is the members this module reads (symbols(),
//     the packed original's relocations, the EXE header and the file the relocation table is read from).
//   * the Python's fixup dicts are exact::standin::Fixup (with `at` set when placed); a header is a list of
//     integers (the Python's tuple of the EXE header's fields).
#pragma once

#include <cstdint>
#include <optional>

#include "codebreaker/exact/compare.hpp"
#include "py.hpp"

namespace codebreaker::exact::recut {

// exact.project.Project, as recut.py reads it
class Project {
  public:
    virtual ~Project() = default;
    virtual compare::Symbols symbols() const = 0;                       // prj.symbols(): name -> (kind, address)
    // prj.packed.relocs when getattr(prj, "packed", None) is truthy, else nullopt
    virtual std::optional<py::list<int64_t>> packed_relocs() const = 0;
    virtual py::list<int64_t> header() const = 0;                       // prj.header (empty or None: no header)
    virtual py::bytes file() const = 0;                                 // prj.file: the original program's file
};

// the program's relocations, image addresses in the order of its table (EXEPACK's when it is packed)
py::list<int64_t> order(const Project& prj);

// the object module's bytes (raw itself when its relocations are not the program's)
py::bytes recut(const Project& prj, const py::bytes& raw);

}  // namespace codebreaker::exact::recut
