// formats/snes.hpp -- Super Nintendo cartridge images (`.sfc`, `.smc`).
//
// The file is the cartridge's ROM, behind a 512-byte copier header on a dump made by a copier (the file's size is then 512 past a multiple of 32 KB;
// the header is kept as it was). The ROM carries its own description, the *internal header*, 64 bytes at $7FC0 (LoROM) or $FFC0 (HiROM) of the ROM:
//
//     +$00  title, 21 bytes of ASCII          +$15  map mode (bit 0 set: HiROM; bit 4: FastROM)    +$16  chipset
//     +$17  ROM size, a power of two in KB     +$18  RAM size    +$19  country    +$1A  developer    +$1B  version
//     +$1C  checksum complement (two bytes)    +$1E  checksum (two bytes); the two add to $FFFF
//     +$24  the native-mode vectors COP BRK ABORT NMI -- IRQ, and +$34 the emulation-mode ones COP -- ABORT NMI RESET IRQ/BRK (16 bits each)
//
// Which of the two places holds the header is how the ROM is mapped: both candidates are scored (the checksum pair, the map mode agreeing with the place,
// a title of printable characters, a RESET vector into $8000-$FFFF) and the better one chosen. The CPU starts in emulation mode at the RESET vector, in bank 0.
//
// Where the program sits. LoROM: bank n of 32 KB is the ROM's n-th 32 KB at $8000-$FFFF of banks $00-$7D and, repeated, $80-$FF (so $00:8000 and
// $80:8000 are the same byte). HiROM: bank n of 64 KB is the n-th 64 KB at banks $C0-$FF (and $40-$7D) in full, and its upper half at $8000-$FFFF of banks
// $00-$3F and $80-$BF. A ROM smaller than the space it is mapped to repeats (a power-of-two size wraps). Banks $7E-$7F are the console's 128 KB of RAM
// and the first 8 KB of banks $00-$3F / $80-$BF are the same RAM's start; $2100-$21FF and $4200-$43FF there are the chips' registers.
//
// ExHiROM, the coprocessors (SuperFX, SA-1, DSP, Cx4, S-DD1) and the SPC700 sound program are not modelled: the sound program is data.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>

#include "core/bytes.hpp"

namespace cb::formats::snes {

inline constexpr std::size_t kCopierHeaderSize = 512;
inline constexpr std::size_t kInternalHeaderSize = 64;
inline constexpr std::uint32_t kLoRomHeader = 0x7FC0;
inline constexpr std::uint32_t kHiRomHeader = 0xFFC0;
inline constexpr std::size_t kLoRomBank = 0x8000;
inline constexpr std::size_t kHiRomBank = 0x10000;
inline constexpr std::size_t kWramSize = 0x20000;

enum class Mapping { LoRom, HiRom };
std::string_view to_string(Mapping m);   // "LoROM", "HiROM"

// The sixteen-bit vectors of one mode.
struct Vectors {
    std::uint16_t cop = 0, brk = 0, abort = 0, nmi = 0, reset = 0, irq = 0;
};

struct Rom {
    Bytes copier_header;                // 512 bytes or none
    Bytes data;                         // the ROM
    Mapping mapping = Mapping::LoRom;
    std::uint32_t header_offset = 0;    // of the internal header in `data`
    std::string title;
    std::uint8_t map_mode = 0, chipset = 0, rom_size_code = 0, ram_size_code = 0, country = 0, developer = 0, version = 0;
    std::uint16_t complement = 0, checksum = 0;
    bool checksum_ok = false;           // the pair adds to $FFFF
    Vectors native;                     // COP BRK ABORT NMI IRQ (RESET is not used in native mode)
    Vectors emulation;                  // COP ABORT NMI RESET IRQ/BRK (the BRK slot holds nothing)

    std::size_t bank_size() const { return mapping == Mapping::LoRom ? kLoRomBank : kHiRomBank; }
    std::size_t bank_count() const { return (data.size() + bank_size() - 1) / bank_size(); }
    bool fast() const { return (map_mode & 0x10) != 0; }

    // The offset in `data` of the byte the CPU reads at the 24-bit `address`, or nullopt where there is no ROM.
    std::optional<std::uint32_t> offset(std::uint32_t address) const;
    // The address `offset` is seen at in the canonical mirror: LoROM bank = offset / 32 KB (plus $80 when `high`), HiROM bank = $C0 + offset / 64 KB.
    std::uint32_t address_of(std::uint32_t offset, bool high) const;
};

// Parse an image. FormatError if it is empty, smaller than a header, or no internal header is plausible.
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// Is this file a cartridge image: the extension `.sfc`, `.smc`, `.swc` or `.fig`, and an internal header that scores as one? Reads the header places.
bool probe(const std::filesystem::path& path);

// The chips' registers by the names the hardware documentation gives them, by their 16-bit address in banks $00-$3F: the PPU's $2100-$213F, the APU
// ports $2140-$2143, the WRAM port $2180-$2183, the joypad ports $4016-$4017, the CPU's $4200-$421F and each DMA channel's $4300-$437F.
const std::map<std::uint32_t, std::string>& registers();

// The name at `address` (a 16-bit address, or a 24-bit one: the bank is ignored), or empty when it is not a register.
std::string register_name(std::uint32_t address);

}  // namespace cb::formats::snes
