// codebreaker/formats/xex_imports.hpp -- typed C++ of codebreaker.formats.xex_imports
//
// xex_imports.py -- an Xbox 360 module's imports, one per line: library, ordinal, name, kind, slot, thunk.
//
//     python -m codebreaker xex-imports <module.xex>
//     xex_imports::imports(path)       [(library, ordinal, name, kind, slot address, thunk address or None)]
//
// Works for any module, a title or a system one (any load address). Names come from
// symbols/console_ordinals.json; an ordinal missing there is named <library>_<ordinal>.
//
// Each imported function has two records: a 4-byte slot (filled by the loader) and a 16-byte code
// thunk (which the recompiler turns into its `__imp__Name` function). An imported variable has only
// the slot. The import header (0x000103FF) is: the total size, the size of the string table, the
// number of strings, the library names (each padded to 4 bytes), then import blocks until the total
// is used -- and a library can have several blocks (dashboard 17559's dash.xex has two for xam).
// A block names its library by index in its byte at 0x25 (early devkit dashboards -- 2255, 2258,
// 2832 -- set other bits in the byte before it), then lists the addresses of its records; a
// record's word in the image has the ordinal in its low 16 bits and, for a thunk, a non-zero top byte.
//
// Typing notes:
//   * imports() returns a list of 6-tuples; slot and thunk are std::optional (the Python's None).
//   * _names() is the table knowledge.table("xbox360", "ordinal_names") holds: the authored reader of
//     knowledge/sources.py applied to symbols/console_ordinals.json (through the base when the process has
//     one open, else straight from the file -- the rows are the same).  A name in the table that is not a
//     string (the file has none) is a TypeError here.
//   * main(argv): Python's main takes argv; the exit status is the returned value.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xex_imports {

inline constexpr int64_t IMPORT_HEADER = 0x000103FF;

// {library: {ordinal: name}} -- the libraries whose names do not start with "_"
py::dict<py::str, py::dict<int64_t, py::str>> _names();

using Import = std::tuple<py::str, int64_t, py::str, py::str, std::optional<int64_t>, std::optional<int64_t>>;

// [(library, ordinal, name, kind, slot address, thunk address or None)]
py::list<Import> imports(const py::str& path);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xex_imports
