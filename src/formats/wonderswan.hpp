// formats/wonderswan.hpp -- Bandai WonderSwan and WonderSwan Color cartridges (`.ws`, `.wsc`).
//
// The CPU is an NEC V30MZ (an 80186-class x86 in real mode: 20-bit addresses from segment:offset). The cartridge's ROM is the whole file; its last 16 bytes
// are the footer the console reads, which is also the reset code: the CPU starts at $FFFF0, where the footer's first five bytes are a far jump
// (`EA offset segment`) into the program. The ROM ends at $FFFFF: a file of up to 896 KB is mapped from $100000 minus its size, so $20000 is
// the lowest address a cartridge shows; a larger ROM shows its last 896 KB and the rest is switched in through the bank registers (ports $C0-$C3,
// not modelled). The footer:
//
//     $0-$4  EA lo hi lo hi   JMPF to the start             $5    maintenance flag (zero)        $6   developer id
//     $7     0 mono, 1 colour                               $8    game id                        $9   version
//     $A     ROM size code                                  $B    save type (SRAM or EEPROM)     $C   flags: bit 0 vertical, bit 2 ...
//     $D     real-time clock (1: present)                   $E-$F checksum: the sum of every byte but the last two, little-endian
//
// Internal RAM is 16 KB at $00000 ($0000-$3FFF; 64 KB on the Color, where $4000-$FFFF holds the colour palettes and tile data too) and
// the I/O ports are 256 (`in` and `out`); both are the machine model's (engine/machine_wonderswan). The program's code is the 8086 set plus the
// 80186 additions.
#pragma once

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <vector>

#include "core/bytes.hpp"
#include "formats/code_scan.hpp"

namespace cb::formats::wonderswan {

inline constexpr std::size_t kFooter = 16;
inline constexpr std::uint32_t kEnd = 0x100000;            // the address after the last byte of the ROM
inline constexpr std::uint32_t kMaxMapped = 0xE0000;       // the most of a ROM the CPU sees at once ($20000-$FFFFF)

struct Rom {
    Bytes data;                         // the file
    std::uint16_t reset_offset = 0, reset_segment = 0;   // the footer's far jump
    std::uint8_t maintenance = 0, developer = 0, color = 0, game_id = 0, version = 0, size_code = 0, save = 0, flags = 0, rtc = 0;
    std::uint16_t checksum = 0;
    bool checksum_ok = false;

    bool is_color() const { return color != 0; }
    bool vertical() const { return (flags & 1) != 0; }
    // The address of the first mapped byte, and how many bytes are mapped.
    std::uint32_t base() const { return kEnd - mapped(); }
    std::uint32_t mapped() const { return static_cast<std::uint32_t>(std::min<std::size_t>(data.size(), kMaxMapped)); }
    // Where the program starts as a linear address, and the segment its code runs in.
    std::uint32_t entry() const { return ((static_cast<std::uint32_t>(reset_segment) << 4) + reset_offset) & 0xFFFFF; }
    std::uint32_t code_base() const { return static_cast<std::uint32_t>(reset_segment) << 4; }

    // The 1 MB the CPU sees: the mapped ROM at its place, the rest zero (RAM and the ports are not in it).
    Bytes memory() const;
    // The file offset of a linear address, or nullopt where no ROM is mapped.
    std::optional<std::uint32_t> offset(std::uint32_t address) const;
    // Function starts as linear addresses: the entry, then every near call target reached from it, all in the code segment.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// FormatError when the file is not a power-of-two multiple of 64 KB with a far jump at the start of its footer.
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// A `.ws` or `.wsc` whose footer has the far jump. Reads the last 16 bytes.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::wonderswan
