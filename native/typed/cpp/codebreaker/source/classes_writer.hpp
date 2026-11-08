// codebreaker/source/classes_writer.hpp -- interface of codebreaker.source.classes_writer (typed C++; implementation pending).
//
// classes_writer.py -- the C++ classes of a module, written as class declarations.
//
//     classes_writer::write(module_path, kind, out_dir, stem, name_at, status_at)
//         -> the file written (<stem>_classes.hpp), or None when the module shows no class
//
// Typing notes:
//   * A class is core16::ClassRecord (core16_types.hpp): what analyze::classes::hierarchy returns and what a
//     PowerPC image's virtual tables are made into.
//   * name_at / status_at are callbacks of an address (the module writer's closures).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "codebreaker/core16_types.hpp"
#include "py.hpp"

namespace codebreaker::source::classes_writer {

// [ClassRecord] -- runs of two or more words in a PowerPC image's data that are each the address of one of its
// functions.
py::list<core16::ClassRecord> _powerpc_tables(const py::str& path);

// The import a 32-bit module's function is nothing but a jump to (`jmp [slot]`), as LIBRARY.name or LIBRARY.number
// -- or None.  (used by source.module_writer)
std::optional<py::str> import_behind(const py::str& path, int64_t address);

// The classes of a module, as analyze/classes.py's hierarchy gives them (or the PowerPC tables).
py::list<core16::ClassRecord> found(const py::str& path, const py::str& kind);

// `name_at(address)` -> the name a function has in the written source; `status_at(address)` -> its verdict's
// title, or "". -> the path written, or None.
std::optional<py::str> write(const py::str& path, const py::str& kind, const py::str& out_dir, const py::str& stem,
                             const std::function<py::str(int64_t)>& name_at,
                             const std::function<py::str(int64_t)>& status_at);

}  // namespace codebreaker::source::classes_writer
