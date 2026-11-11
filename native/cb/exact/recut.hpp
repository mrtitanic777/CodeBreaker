// cb/exact/recut.hpp -- a module's code records cut again, as the original program's order of relocations shows.
//
// The linker lists a program's relocations in the order it meets the fixups that need them: each data record's
// fixups in the order the record lists them, the records in their order. A compiler cut a file's code into records
// by the source's lines and listed a record's fixups as it pleased (Microsoft C: the last first); a source put
// together from pieces does not cut where the original's did. The bytes do not depend on it -- the relocation table
// does (and so EXEPACK's). So for the link, a code segment is written again in records cut where the original's
// relocations show one ended (where the next lies after the one before: within a record they run down), each
// record listing its relocations in the original's order. It proves nothing about the source's lines.
//
// The project asks the link stage for it with `"recut": true`.
#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/exact/symbols.hpp"

namespace cb::exact::recut {

// Where an EXE's relocation table is, as the header says: how many entries, and the file offset of the first.
struct RelocTable {
    std::uint32_t count = 0;
    std::uint32_t offset = 0;
};

// What recut reads of the project (exact/project, refactored later, fills this in).
struct Original {
    const SymbolTable& symbols;                                // the project's SYMBOLS
    std::optional<std::vector<std::uint32_t>> packed_relocs;   // when the program is EXEPACK-packed: its relocations, in order
    std::optional<RelocTable> table;                           // the EXE header's table; absent when the file has no header
    ByteView file;                                             // the original program's file
};

// The program's relocations as image addresses (segment * 16 + offset), in the order of its table (EXEPACK's when it
// is packed). TruncatedError/FormatError when the header's table lies outside the file.
std::vector<std::uint32_t> relocation_order(const Original& original);

// The object module's bytes, its code segments cut again; `raw` itself (copied) when its relocations are not the
// program's or nothing in it can be cut to fit. FormatError/TruncatedError for a damaged module.
Bytes recut(const Original& original, ByteView raw);

}  // namespace cb::exact::recut
