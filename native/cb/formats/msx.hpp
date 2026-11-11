// cb/formats/msx.hpp -- MSX cartridge images (`.rom`, `.mx1`, `.mx2`).
//
// A cartridge starts "AB", then four addresses: INIT (called at power-on), STATEMENT (the BASIC CALL extension),
// DEVICE, and TEXT (a BASIC program in the ROM); zero where unused. A cartridge sits in page 1 ($4000) -- 32 KB
// ones fill pages 1 and 2 -- or, rarely, at $0000 or $8000; where is told by its INIT address. Cartridges over
// 48 KB switch banks through a mapper (Konami, ASCII ...), which this lane does not follow yet: they are refused
// (UnsupportedError) rather than half-read.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::msx {

inline constexpr std::array<std::string_view, 3> kExtensions = {".rom", ".mx1", ".mx2"};

// The cartridge header's four entry addresses (zero = unused).
struct Header {
    std::uint16_t init = 0;
    std::uint16_t statement = 0;
    std::uint16_t device = 0;
    std::uint16_t text = 0;
};

// A named entry point inside the image.
struct Entry {
    std::uint16_t address = 0;
    std::string_view name;     // "init", "statement" or "device"
};

struct Program {
    std::uint32_t origin = 0;  // the address the image's first byte sits at
    Bytes image;
    Header header;
    // The header's INIT, STATEMENT and DEVICE entries that point into the image, in that order; two names for one
    // address keep the first.
    std::vector<Entry> entries;
};

// FormatError if there is no "AB" header (at the start, or at $4000 of a 48 KB image); UnsupportedError if the
// image is over 48 KB (a mapper).
Program parse(ByteView data);
Program read(const std::filesystem::path& path);

// A `.rom`/`.mx1`/`.mx2` file with "AB" at its start or at $4000.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::msx
