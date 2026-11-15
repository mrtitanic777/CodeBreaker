// cb/formats/vectrex.hpp -- Vectrex cartridge images (`.vec`, `.bin`, `.gam`): the ROM that sits at $0000, up to 32 KB.
//
// The Vectrex's CPU is a Motorola 6809. Its 8 KB system ROM (Mine Storm and the routines a game calls) is at $E000-$FFFF and is not in a cartridge; the VIA 6522 at
// $D000 runs the display, the sound chip and the controllers, and 1 KB of RAM is at $C800-$CBFF (seen again at $CC00-$CFFF). The cartridge ROM is at $0000.
//
// The cartridge starts with the header the BIOS reads: `g GCE 1982` (the copyright text, which must begin `g GCE`) ending with $80, a pointer to the title music (a word),
// then the title lines -- each a height, a width, a relative y and a relative x (signed bytes) and the text ending with $80 -- and a zero byte after the last line.
// The program starts at the byte after that zero: the BIOS jumps there once it has shown the title. A game has no vectors of its own: its interrupts go through
// the BIOS's RAM vectors (the program fills them in). The entry is therefore the one fixed way in. Banked cartridges (more than 32 KB) are not read.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/code_scan.hpp"

namespace cb::formats::vectrex {

inline constexpr std::size_t kMaxSize = 0x8000;

struct TitleLine {
    std::int8_t height = 0, width = 0, y = 0, x = 0;
    std::string text;
};

struct Rom {
    Bytes data;                         // the file: the ROM from $0000
    std::string copyright;              // "g GCE 1982"
    std::uint16_t music = 0;            // the pointer after it
    std::vector<TitleLine> title;       // the title lines
    std::uint32_t entry = 0;            // the address after the header's last zero

    // The 64 KB the CPU sees: the ROM at $0000, the rest zero (RAM, the VIA and the BIOS are not in it).
    Bytes memory() const;
    // Function starts: the entry, then every call target reached from it inside the ROM.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// FormatError when the file is not a Vectrex cartridge (the `g GCE` header, one to 32 KB, a header that ends).
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// A `.vec` / `.bin` / `.gam` of up to 32 KB that starts `g GCE`. Reads the first bytes only.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::vectrex
