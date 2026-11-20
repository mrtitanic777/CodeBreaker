// formats/threedo.hpp -- 3DO (Opera) programs: a disc image holding the Opera filesystem, and the ARM Image Format (AIF) executable a disc launches.
//
// Everything on a 3DO disc is big-endian, the ARM60 included: instruction words and data alike.
//
// A 3DO disc image (`.iso`, 2048-byte sectors) starts with the Opera volume header: record type 1, five bytes $5A, version 1, then (big-endian) the
// volume's comment and label (32 bytes each), its id, the block size (2048), the block count, and the root directory: its id, the number of blocks it
// takes, its block size and the blocks its copies start at. A directory block holds a header (next and previous block, flags, the first free byte, the
// offset of the first entry, 20) and entries: flags (the low byte 2 a file, 6 or 7 a directory; bit 30 ends the block's entries, bit 31 the
// directory's), id, type (a four-character code), block size, byte count, block count, burst, gap, a 32-byte name, the last copy and the blocks the
// copies start at.
//
// What the console launches is the file `LaunchMe` in the root: an AIF executable (ARM's image format, which the 3DO's toolchain makes). Its header
// has (as big-endian words) the exit SWI at word 4, the sizes of the read-only, read-write and zero-initialised areas at words 5, 6 and 8, and the entry at word 3 as a BL
// (the self-relocation code is at word 1, the zero-initialisation at word 2). The image is position-independent until it relocates itself, so
// it is read as loaded at address 0: an address is an offset in the image, and the entry is where word 3's BL goes. The processor is an ARM60 (ARMv3,
// ARM state only; the 26-bit mode's flags in the pc are not modelled). A bare AIF (`LaunchMe`, `.aif`) is read as such.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "formats/code_scan.hpp"

namespace cb::formats::threedo {

inline constexpr std::size_t kSector = 2048;
inline constexpr std::size_t kAifHeader = 0x40;   // what the header words fill; the self-relocation code follows

// A file of the Opera filesystem.
struct Entry {
    std::string name;
    std::uint32_t flags = 0;
    std::uint32_t type = 0;           // the four-character code
    std::uint32_t block_size = 0;
    std::uint32_t byte_count = 0;
    std::uint32_t first_block = 0;    // where the first copy starts, in blocks
    bool directory() const { return (flags & 0xFF) == 6 || (flags & 0xFF) == 7; }
};

struct Aif {
    std::uint32_t entry = 0;          // the address the BL at word 3 goes to (the image is at 0); 0 where word 3 is a NOP
    std::uint32_t read_only = 0, read_write = 0, debug = 0, zero_init = 0;
    std::uint32_t base = 0;           // word 10
    std::uint32_t mode = 0;           // word 12
};

enum class Kind { Disc, Aif };

struct Program {
    Kind kind = Kind::Aif;
    Bytes file;                       // the file as it was (the disc image, or the bare AIF)
    std::uint32_t image_offset = 0;   // where the AIF starts in `file`
    std::uint32_t image_size = 0;
    std::string label;                // the disc's volume label
    std::string name = "LaunchMe";
    std::vector<Entry> root;          // the root directory
    Aif aif;

    ByteView image() const { return ByteView(file).subspan(image_offset, image_size); }
    // The image as the instruction tables and the interpreter read it: each aligned word with its bytes reversed (the ARM60 runs big-endian,
    // CodeBreaker's ARM tables are little-endian), the size rounded up to whole words.
    Bytes native() const;
    // Function starts: the entry, then every call target reached from it.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// FormatError when the bytes are not a disc with a LaunchMe, or an AIF.
Program parse(ByteView data);
Program read(const std::filesystem::path& path);

// Is the file an Opera disc image (the volume header: 01 5A 5A 5A 5A 5A 01 at the start), or a bare AIF named LaunchMe or `.aif`? Reads 2 KB.
bool probe(const std::filesystem::path& path);

// Does the sector start as an Opera volume header?
bool is_volume_header(ByteView sector);

}  // namespace cb::formats::threedo
