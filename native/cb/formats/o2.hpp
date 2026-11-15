// cb/formats/o2.hpp -- Magnavox Odyssey 2 / Philips Videopac cartridge images (`.o2`): the bytes that sit in the 8048's program memory from $0400.
//
// The console's CPU is an Intel 8048 with 12 address lines. Its own 1 KB of BIOS ROM is at $0000-$03FF (not in a cartridge file); the cartridge's 1-3 KB are at $0400-$0FFF and the
// BIOS starts the program there once it has run. A larger cartridge is bank-switched (SEL MB0 / MB1 and the P1 lines) and is not read. The Intel 8244/8245 video chip and the
// controllers are reached through MOVX and the ports: the video chip's registers are at the data addresses $A0 (control), $A1 (status), $A2 (collision) and $A3 (colour), with the
// character, quad and sprite tables below them.
//
// A cartridge file has no header or magic: a file is taken to be one by its extension (`.o2`), and a size that is a whole number of 256-byte pages up to 3 KB.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>

#include "cb/core/bytes.hpp"

namespace cb::formats::o2 {

inline constexpr std::uint32_t kBase = 0x400;
inline constexpr std::size_t kMaxSize = 0xC00;

struct Rom {
    Bytes data;                 // the file: the program memory from $0400
    std::uint32_t address(std::size_t offset) const { return kBase + static_cast<std::uint32_t>(offset); }
};

// The video chip's registers by data-space address (for the lane's access log).
const std::map<std::uint32_t, std::string>& vdc();

// FormatError when the file is not one (empty, over 3 KB, not whole pages).
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// A `.o2` file that parses. Reads the file's size only.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::o2
