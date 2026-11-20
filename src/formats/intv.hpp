// formats/intv.hpp -- Mattel Intellivision cartridge images: the Intellicart / jzIntv `.rom` file, and a raw `.int` file of big-endian words.
//
// The Intellivision's CPU (a General Instrument CP1610) addresses 16-bit words; a cartridge's ROM is 10 bits wide ("decles") and sits at $5000 and above. A `.rom` file holds
// segments of whole 256-word pages:
//
//     $A8, the number of segments N, N's complement                      (3 bytes)
//     N times:  first page, last page (the high bytes of the first and last address),
//               the words (two bytes each, high byte first),
//               a CRC-16 (polynomial $1021, start value $FFFF) of the segment record from its first page byte up to here, high byte first
//     then the Intellicart's tables (memory attributes and the like), kept as they are
//
// A cartridge starts with a header at $5000: six pointers (each two words, low byte then high byte: the MOB picture base, the process table, the program's start, the background,
// the GRAM image, the title and date), then a flags word. The BIOS runs the program from the third pointer once it has shown the title. A raw `.int` file is the words from $5000 on.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"

namespace cb::formats::intv {

inline constexpr std::uint32_t kHeaderAddress = 0x5000;

// CRC-16/CCITT-FALSE.
std::uint16_t crc16(ByteView data);

struct Segment {
    std::uint32_t start = 0;                   // the word address of the first word (a multiple of 256)
    std::vector<std::uint16_t> words;          // whole pages
    std::uint16_t crc = 0;                     // as stored
    std::uint32_t end() const { return start + static_cast<std::uint32_t>(words.size()); }   // one past the last
};

struct Rom {
    bool raw = false;                          // a raw file: big-endian words at $5000, no segment table
    std::vector<Segment> segments;
    Bytes tail;                                // what follows the last segment of a .rom

    const Segment* segment_at(std::uint32_t address) const;
    std::optional<std::uint16_t> word_at(std::uint32_t address) const;
    // The program's start: the third header pointer, when the header is there and the pointer is inside a segment.
    std::optional<std::uint32_t> entry() const;
    // The title the header points at (printable characters after the date word), or empty.
    std::string title() const;
    // The file, as it was read (stored CRCs kept) or as the writer builds it (`recompute_crc`).
    Bytes serialize(bool recompute_crc = false) const;
    std::size_t word_count() const;
};

// FormatError when the file is not an Intellivision cartridge image.
Rom parse(ByteView data, bool raw = false);
Rom read(const std::filesystem::path& path);

// A `.rom` file with the Intellicart header whose segment table is consistent with its size, or a `.int` file whose header points into itself.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::intv
