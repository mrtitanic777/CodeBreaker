// cb/formats/coleco.hpp -- ColecoVision cartridge images (`.col`, or a `.rom`/`.bin` with the same header).
//
// The console's BIOS (8 KB at $0000) checks the cartridge at $8000: the bytes $AA $55 (the title screen is shown first) or $55 $AA (the game
// starts at once), then five words -- the sprite-name table, the sprite-order table, the work buffer, the controller map and the game's start
// address -- then seven three-byte vectors the BIOS jumps through (RST $08 .. RST $38; the last, RST $38, is the maskable interrupt), the NMI
// vector (the VDP's vertical blank) and the title text. RAM is 1 KB at $6000, repeated up to $7FFF. A cartridge is up to 32 KB (the $8000-$FFFF
// window); larger ones switch banks (a mapper), which this lane does not follow: they are refused (UnsupportedError) rather than half-read.
//
// The BIOS is not in the cartridge, so it is not run: its documented entry points are known by name (`bios_names()`), and a call into it is
// logged and returns (emulate/z80_machines).
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::coleco {

inline constexpr std::array<std::string_view, 3> kExtensions = {".col", ".rom", ".bin"};
inline constexpr std::uint32_t kOrigin = 0x8000;
inline constexpr std::size_t kMaxSize = 0x8000;

struct Header {
    bool title_screen = false;            // $AA $55 (the BIOS shows its title screen); false for $55 $AA
    std::uint16_t sprite_names = 0;
    std::uint16_t sprite_order = 0;
    std::uint16_t work_buffer = 0;
    std::uint16_t controllers = 0;
    std::uint16_t start = 0;
    std::string title;                    // the printable text at $8024 ("LINE 2/LINE 1/YEAR")
};

struct Entry {
    std::uint16_t address = 0;
    std::string name;                     // "start", "rst08" .. "rst30", "irq" (RST $38) or "nmi"
};

struct Program {
    Header header;
    Bytes image;                          // the cartridge, as it sits from $8000
    // The start address, then each vector that holds a jump, return or interrupt return (a vector the game does not use is zeros or $FF).
    std::vector<Entry> entries;
};

// FormatError if there is no $AA55 / $55AA header; UnsupportedError if the image is over 32 KB.
Program parse(ByteView data);
Program read(const std::filesystem::path& path);

// A `.col` file (or `.rom` / `.bin` of at most 32 KB) with the header.
bool probe(const std::filesystem::path& path);

// The BIOS's documented entry points ($1F61-$1FFD) and the system RAM it keeps ($7000-$73FF), by the names its listing gives them.
const std::map<std::uint32_t, std::string>& bios_names();

}  // namespace cb::formats::coleco
