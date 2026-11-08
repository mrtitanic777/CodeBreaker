// codebreaker/build/vxdfile.hpp -- C++17 port of codebreaker.build.vxdfile.
//
// vxdfile.py -- a built VxD's LE laid out in a file the way the original's is.
//
//     py::str text = vxdfile::definitions(original, lfa);   the .def its names came from (DESCRIPTION, EXPORTS)
//     py::bytes le = vxdfile::relay(built, at, resource);
//
// OpenWatcom's linker writes an LE with its own layout: its pages straight behind the tables. The
// originals were linked by Microsoft's LINK: the pages start on a 512-byte
// boundary of the file, the non-resident name table behind them holds the description and the
// exports named in the .def (DESCRIPTION, EXPORTS), and a Win95 VxD carries its version resource
// behind that, pointed at by the header (at B8h: where in the file, at BCh: how long). The
// description and exports come from the driver's .def, which the linker is given (build/driver.py
// link), so the name table is the linker's. relay()
// takes the built LE -- header, loader and fix-up sections, pages: all as linked -- and puts it
// at `at` in such a file: every offset the LE gives from the start of the FILE (the pages, the
// non-resident names, the resource) is set for where it now is.
#pragma once

#include "py.hpp"

namespace codebreaker::build::vxdfile {

extern const int64_t PAGE_ALIGN;

// The 32-bit little-endian word of `d` at `at` (struct.error when it does not fit).
int64_t _u(const py::bytes& d, int64_t at);

// The .def text the original's non-resident name table was made from. -> str
py::str definitions(const py::bytes& original, int64_t lfa);

// The LE of `built` (a whole VxD file) laid out to start at file offset `at` (a multiple of
// 16) as Microsoft's LINK lays it out: tables, pages from the next 512-byte boundary, the
// non-resident name table as linked, then `resource`. -> the bytes from `at` on.
py::bytes relay(const py::bytes& built, int64_t at, const py::bytes& resource = py::bytes());

}  // namespace codebreaker::build::vxdfile
