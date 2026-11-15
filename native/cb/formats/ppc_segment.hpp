// cb/formats/ppc_segment.hpp -- what the PowerPC console program formats (formats/dol, formats/ppc_elf) share: a piece of the
// program image and where it lies in the file and in memory.
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace cb::formats::ppc_segment {

// One stretch of a program that is loaded into memory.
struct Segment {
    std::string name;                // "text0", "data2", ".text", ".rodata"
    std::uint32_t address = 0;       // where it is loaded (virtual, cached: 0x80003100)
    std::size_t offset = 0;          // where its bytes are in the file
    std::size_t size = 0;            // how many bytes the file holds
    std::size_t memory_size = 0;     // how many bytes it takes in memory (a zero-filled tail past `size`)
    bool code = false;               // the program's instructions (a DOL's text sections, an ELF's executable sections)
};

}  // namespace cb::formats::ppc_segment
