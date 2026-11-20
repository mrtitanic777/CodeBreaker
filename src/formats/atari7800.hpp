// formats/atari7800.hpp -- Atari 7800 cartridge images (`.a78`): a 128-byte header and the ROM.
//
// The header (A78 header version 1 to 4, the community's): byte 0 the version, bytes 1-16 "ATARI7800" padded with spaces, 17-48 the title,
// 49-52 the ROM size (big-endian), 53-54 the cartridge type flags (big-endian), 55 and 56 the controllers, 57 the TV standard, 58 the save
// device, and from 100 to 127 the text "ACTUAL CART DATA STARTS HERE"; the ROM follows at 128.
//
// Where the program sits. The ROM ends at the top of the address space: 16 KB at $C000, 32 KB at $8000, 48 KB at $4000 (a size that is a
// multiple of 4 KB below 48 KB fills the top of it). Its last six bytes are the vectors NMI $FFFA, RESET $FFFC, IRQ $FFFE. A SuperGame board
// (type flag 2, ROM of 64 KB or more in 16 KB banks) has the last bank fixed at $C000 and one other selectable at $8000 by a write to $8000-$FFFF;
// at power-on the first is taken to show. Other boards (the Activision's, the absolute's, banked RAM, POKEY, the YM2151) are not modelled.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "formats/code_scan.hpp"

namespace cb::formats::atari7800 {

inline constexpr std::size_t kHeaderSize = 128;
inline constexpr std::size_t kBankSize = 0x4000;
inline constexpr std::uint16_t kTypeSuperGame = 0x0002;

struct Vectors {
    std::uint16_t nmi = 0;
    std::uint16_t reset = 0;
    std::uint16_t irq = 0;
};

struct Rom {
    Bytes header;                 // the 128 bytes as they were
    std::uint8_t version = 0;
    std::string title;
    std::uint32_t declared_size = 0;   // the header's size field
    std::uint16_t cart_type = 0;
    std::uint8_t controller1 = 0, controller2 = 0, tv = 0, save = 0;
    Bytes data;                   // the ROM
    bool supergame = false;
    std::uint32_t base = 0;       // the address the first byte of a linear ROM sits at
    Vectors vectors;

    std::size_t bank_count() const { return supergame ? data.size() / kBankSize : 1; }
    // The bank that stays at $C000 (SuperGame), or 0.
    std::size_t fixed_bank() const { return supergame ? bank_count() - 1 : 0; }

    // The 64 KB the CPU sees with `bank` selected at $8000 (SuperGame; the fixed bank is always at $C000). RAM and the chips are not in it.
    Bytes memory(std::size_t bank = 0) const;

    // The offset in `data` of `address` as `bank` shows it, or nullopt where there is no ROM.
    std::optional<std::uint32_t> offset(std::size_t bank, std::uint32_t address) const;

    // Function starts as (bank << 16 | address): the vectors, then every JSR target reached from them.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// Parse a header and ROM. FormatError if the header is not one, the ROM is cut short, or is of a size no board here has.
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// Does the file start as an A78 header (version byte, "ATARI7800" at 1)? Reads 128 bytes.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::atari7800
