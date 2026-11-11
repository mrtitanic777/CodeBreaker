// cb/build/mzfile.hpp -- an MZ program's file written the way the original's is laid out.
//
//     Bytes tpl  = mzfile::make_template(original_header);        // the header's own data, build outputs blanked
//     Bytes file = mzfile::compose(tpl, module, relocs, start, min_alloc);
//
// A linker writes a header of its own: the fields, the relocation table, padding. Many of the originals carry
// more in theirs -- MS-DOS 7's utilities keep their messages ("Incorrect MS-DOS version", "Extended Error %1"),
// their version block (VS_FIXEDFILEINFO) and their parse tables in the header, behind the relocation table.
// That data is part of the program as written, so it is kept as a file beside the source (<stem>.hdr): the
// original header with what the BUILD decides blanked -- the sizes, the relocation count and table, the minimum
// memory, the stack and the entry point -- and the rest kept: the header's size, the maximum memory and checksum
// words (linker options of the original's build), where the relocation table goes, and the data.
// compose() fills the blanks from what was built.
#pragma once

#include <cstdint>
#include <span>

#include "cb/core/bytes.hpp"

namespace cb::build::mzfile {

// One relocation: the word at segment:offset of the load module is a segment value to be fixed up.
struct Relocation {
    std::uint16_t offset = 0;
    std::uint16_t segment = 0;
};

// The registers a program starts with, relative to the load module.
struct StartState {
    std::uint16_t cs = 0, ip = 0;
    std::uint16_t ss = 0, sp = 0;
};

// The original header with the build's outputs blanked (every field the build decides, and the relocation
// table). TruncatedError when the header is shorter than the 0x1A-byte fixed part; FormatError when its
// relocation table lies outside it.
Bytes make_template(ByteView header);

// The file: the template's header with the blanks filled -- `relocs` written at the template's relocation table
// place -- then the load module. `full_last` writes a last page that is exactly full as 512 (some linkers did)
// rather than 0. FormatError when the relocation table does not fit where the template has room for it, or a
// size does not fit its header field.
Bytes compose(ByteView tpl, ByteView module, std::span<const Relocation> relocs, const StartState& start,
              std::uint16_t min_alloc, bool full_last = false);

}  // namespace cb::build::mzfile
