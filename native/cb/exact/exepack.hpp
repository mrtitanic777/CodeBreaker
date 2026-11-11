// cb/exact/exepack.hpp -- what a file packed by EXEPACK (or LINK /EXEPACK) keeps of the program it packed.
//
// The packed file's code segment begins with EXEPACK's header: the program's own entry and stack (real CS:IP, SS:SP),
// the length it unpacks to, the signature "RB". After the unpacker's code and its message ("Packed file is corrupt")
// come the program's relocations: sixteen groups, one for each 64K of the image, a count and that many offsets - in
// the order the linker listed them.
#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::exact::exepack {

struct Packed {
    std::uint16_t ip = 0, cs = 0;        // the program's entry (paragraphs from the image's start)
    std::uint16_t mem = 0;               // EXEPACK's own field: the memory the unpacker wants
    std::uint16_t size = 0;              // the packed image's size field
    std::uint16_t sp = 0, ss = 0;        // the program's stack
    std::uint16_t dest = 0;              // the unpacked length in paragraphs
    std::vector<std::uint32_t> relocs;   // [image address] of each relocation, in the table's order
};

// The packed program's header and relocations, or nullopt when the file is not EXEPACK-packed. Throws
// TruncatedError when the relocation table runs off the end of the file.
std::optional<Packed> read(ByteView file);

}  // namespace cb::exact::exepack
