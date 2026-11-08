// codebreaker/build/mzfile.hpp -- C++ from codebreaker.build.mzfile.
//
// mzfile.py -- an MZ program's file written the way the original's is laid out.
//
//     auto tpl = mzfile::template_(original_header);       the header's own data, build outputs blanked
//     auto data = mzfile::compose(tpl, module, relocs, cs, ip, ss, sp, min_alloc);
//
// A linker writes a header of its own: the fields, the relocation table, padding. Many of the
// originals carry more in theirs -- MS-DOS 7's utilities keep their messages ("Incorrect MS-DOS
// version", "Extended Error %1"), their version block (VS_FIXEDFILEINFO) and their parse tables
// in the header, behind the relocation table. That data is part of the program as written, so it
// is kept as a file beside the source (<stem>.hdr): the original header with what the BUILD
// decides blanked -- the sizes, the relocation count and table, the minimum memory, the stack and
// the entry point -- and the rest kept: the header's size, the maximum memory and checksum words
// (linker options of the original's build), where the relocation table goes, and the data.
// compose() fills the blanks from what was built.
#pragma once

#include "py.hpp"

namespace codebreaker::build::mzfile {

// (offset, width) of each header field the build decides (Python: _BUILT).
extern const py::list<std::tuple<int64_t, int64_t>> BUILT_;

// The original header with the build's outputs blanked. (`template` is a C++ keyword: template_.)
// Raises py::StructError when the header is shorter than 0x1A bytes.
py::bytes template_(const py::bytes& header);

// The file: the template's header with the blanks filled -- `relocs` [(offset, segment)]
// written at the template's relocation table place -- then the load module.
// ValueError when the relocation table does not fit where the template has room for it.
py::bytes compose(const py::bytes& tpl, const py::bytes& module, const py::list<std::tuple<int64_t, int64_t>>& relocs,
                  int64_t cs, int64_t ip, int64_t ss, int64_t sp, int64_t min_alloc, bool full_last = false);

}  // namespace codebreaker::build::mzfile
