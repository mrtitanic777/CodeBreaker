// cb/build/vxdfile.hpp -- a built VxD's LE laid out in a file the way the original's is.
//
//     std::string def = vxdfile::definitions(original_file, lfa);   // the .def its names came from
//     Bytes le = vxdfile::relay(built_file, at, resource);
//
// OpenWatcom's linker writes an LE with its own layout: its pages straight behind the tables. The originals
// were linked by Microsoft's LINK: the pages start on a 512-byte boundary of the file, the non-resident name
// table behind them holds the description and the exports named in the .def (DESCRIPTION, EXPORTS), and a Win95
// VxD carries its version resource behind that, pointed at by the header (at B8h: where in the file, at BCh:
// how long). The description and exports come from the driver's .def, which the linker is given
// (build/driver link), so the name table is the linker's. relay() takes the built LE -- header, loader and
// fix-up sections, pages: all as linked -- and puts it at `at` in such a file: every offset the LE gives from the
// start of the FILE (the pages, the non-resident names, the resource) is set for where it now is.
#pragma once

#include <cstdint>
#include <string>

#include "cb/core/bytes.hpp"

namespace cb::build::vxdfile {

// Pages start on this boundary of the file.
inline constexpr std::uint32_t kPageAlign = 0x200;

// The .def text the original's non-resident name table was made from: DESCRIPTION for the entry with ordinal 0
// (names are Latin-1), then EXPORTS with one `name @ordinal` line each. `lfa` is where the LE header starts in
// `original`. TruncatedError / FormatError when the tables lie.
std::string definitions(ByteView original, std::uint32_t lfa);

// The LE of `built` (a whole VxD file) laid out to start at file offset `at` (a multiple of 16) as Microsoft's
// LINK lays it out: tables, pages from the next 512-byte boundary, the non-resident name table as linked, then
// `resource`. -> the bytes from `at` on. FormatError when the file's own offsets are inconsistent.
Bytes relay(ByteView built, std::uint32_t at, ByteView resource = {});

}  // namespace cb::build::vxdfile
