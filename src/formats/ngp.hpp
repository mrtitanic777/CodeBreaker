// formats/ngp.hpp -- SNK Neo Geo Pocket / Pocket Color cartridge images (`.ngp`, `.ngc`, `.npc`): the ROM that sits at $200000.
//
// The console's CPU is a Toshiba TLCS-900/H (TMP95C061) with 24 address lines; the cartridge ROM is at $200000-$3FFFFF (a second chip at $800000) and the file is the ROM from its
// first byte. The first 64 bytes are the header the BIOS reads:
//
//     $00  28 bytes   "COPYRIGHT BY SNK CORPORATION" or " LICENSED BY SNK CORPORATION" (a leading space: licensed, not first-party)
//     $1C  4 bytes    the program's start address (little-endian, normally inside the header's own ROM: $2000xx)
//     $20  2 bytes    the catalogue number          $22  1 byte  the sub-catalogue / version
//     $23  1 byte     the system it needs: $00 monochrome, $10 colour
//     $24  12 bytes   the title (space padded)      $30  16 bytes  reserved
//
// The BIOS is not part of the file: it is entered through the software interrupts (`SWI`) and keeps its state in work RAM around $6C00-$6FFF; the K1GE/K2GE graphics chip's registers
// are at $8000, the Z80 sound CPU's RAM at $7000 and the TLCS-900's own I/O at $0000-$00FF. (This reader and the register names are all there is so far: the TLCS-900/H core is not written.)
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>

#include "core/bytes.hpp"

namespace cb::formats::ngp {

inline constexpr std::uint32_t kRomBase = 0x200000;
inline constexpr std::size_t kHeaderSize = 64;
inline constexpr std::size_t kMaxSize = 0x400000;

struct Header {
    bool licensed = false;          // " LICENSED BY SNK CORPORATION"
    std::uint32_t start = 0;
    std::uint16_t catalog = 0;
    std::uint8_t sub_catalog = 0;
    bool colour = false;
    std::string title;
};

struct Rom {
    Bytes data;                     // the file: the ROM from $200000
    Header header;
    // The address of a byte of the file, and the file offset of an address (nullopt outside the ROM).
    std::uint32_t address(std::size_t offset) const { return kRomBase + static_cast<std::uint32_t>(offset); }
};

// The registers of the console by address (the TLCS-900's I/O, the BIOS's work-RAM variables, the K1GE graphics chip): names for the source writer and the lane's access log.
const std::map<std::uint32_t, std::string>& hardware();

// FormatError when the file is not an NGP cartridge (the SNK licence text, 64 bytes to 4 MB, a start address inside the ROM).
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// A `.ngp` / `.ngc` / `.npc` that starts with the SNK licence text. Reads the first bytes only.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::ngp
