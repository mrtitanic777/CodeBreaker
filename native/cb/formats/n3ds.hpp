// cb/formats/n3ds.hpp -- Nintendo 3DS homebrew: a `.3dsx` executable, or the ARM ELF devkitARM links it from (an `.elf` loaded at $00100000).
//
// A 3DSX file is the homebrew launcher's format: a header (the magic "3DSX", the header's size and the size of one relocation header, a version and flags, then
// the sizes of the code, read-only data, data and bss segments; a header of 44 bytes adds the offset and size of the SMDH icon block and the offset of the
// RomFS), the three relocation headers (the number of absolute and relative relocations of each segment), the code, read-only data and data segments (the
// data one without its bss), then the relocation tables (each entry two 16-bit numbers: words to skip and words to patch) and what the header's offsets name
// after them. The image is linked at address 0 and the loader adds the base ($00100000) to each absolute relocation, so a stored pointer is the offset
// of its target in the image; the segments follow each other, each rounded up to a page.
//
// The processor is the ARM11 MPCore (ARMv6K) in the user mode the homebrew runs in: ARM state (Thumb appears), the VFP, and the Horizon kernel's SVCs. An
// encrypted `.cia` or `.3ds` is not handled: it is recognised by its header and refused.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/mips_elf.hpp"

namespace cb::formats::n3ds {

inline constexpr std::uint32_t kBase = 0x00100000;   // where the loader puts the image
inline constexpr std::uint32_t kPage = 0x1000;
inline constexpr std::uint16_t kMachineArm = 40;

enum class Kind { X3dsx, Elf };

// A loaded piece: where it is in the file, where it is loaded, and what it is.
struct Segment {
    std::uint32_t file_offset = 0, file_size = 0;
    std::uint32_t address = 0;       // loaded at (kBase included)
    std::uint32_t memory_size = 0;   // with the bss
    bool code = false;
    std::string name;
};

struct Header {
    std::uint16_t header_size = 0, relocation_header_size = 0;
    std::uint32_t version = 0, flags = 0, code_size = 0, rodata_size = 0, data_size = 0, bss_size = 0;
    std::uint32_t smdh_offset = 0, smdh_size = 0, romfs_offset = 0;
};

struct Program {
    Kind kind = Kind::X3dsx;
    Bytes file;
    Header header;                          // a 3DSX's
    std::vector<Segment> segments;
    std::uint32_t entry = 0;                // the file offset the program starts at
    std::vector<std::uint32_t> pointers;    // file offsets of the words the relocation tables patch as absolute (a 3DSX), in order
    std::map<std::uint32_t, std::string> names;   // file offset -> symbol (an ELF's symbol table)
    std::optional<mips_elf::Elf> elf;

    // The file offset of a loaded address (kBase included), or nothing.
    std::optional<std::uint32_t> offset_of(std::uint32_t address) const;
    // The address a file offset is loaded at.
    std::optional<std::uint32_t> address_of(std::uint32_t offset) const;
    bool in_code(std::uint32_t offset) const;
};

// FormatError when the bytes are neither (an encrypted container says so).
Program parse(ByteView data);
Program read(const std::filesystem::path& path);
// Is it a `.3dsx`, or an ARM ELF loaded at $00100000? Reads the first kilobyte.
bool probe(const std::filesystem::path& path);
// The kind of container an encrypted one is ("CIA", "3DS (NCSD)") when the bytes are one, for `info`.
std::optional<std::string> encrypted_container(ByteView head);

}  // namespace cb::formats::n3ds
