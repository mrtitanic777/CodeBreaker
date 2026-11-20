// formats/pce.hpp -- PC Engine / TurboGrafx-16 HuCard images (`.pce`, `.tg16`).
//
// A HuCard image is the card's ROM in 8 KB banks, with nothing in front of it -- or with a 512-byte copier header (the file's size is then 512 past
// a multiple of 8 KB), which is kept as it was. There is no header to name the program: the ROM is its own description.
//
// Where the program sits. The HuC6280 sees 64 KB through eight mapping registers (MPR0 to MPR7), one per 8 KB page of its address space, each
// naming a physical bank. At reset MPR7 alone is known: it holds bank $00, the card's first 8 KB, at $E000-$FFFF, and the six vectors are the
// last bytes of it -- IRQ2 and BRK $FFF6, IRQ1 (the VDC) $FFF8, TIMER $FFFA, NMI $FFFC, RESET $FFFE. Every other page is set by the program (TAM).
// The physical banks are numbered: $00-$7F the card's ROM (a card smaller than 1 MB repeats), $F8 the console's 8 KB of RAM (the program puts it at
// $2000, so page zero is $2000-$20FF and the stack $2100-$21FF), $FF the hardware page, which holds the chips' registers:
//
//     $0000 VDC (video)    $0400 VCE (palette)    $0800 PSG (sound)    $0C00 timer    $1000 I/O (joypads)    $1400 interrupt control
//
// This reader does not undo the bit-reversal the US TurboGrafx-16 cards carry in some dumps, nor model the mappers of the largest cards
// (Street Fighter II' and its 2.5 MB), nor the CD-ROM system.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>

#include "core/bytes.hpp"

namespace cb::formats::pce {

inline constexpr std::size_t kBankSize = 0x2000;
inline constexpr std::size_t kHeaderSize = 512;
inline constexpr std::uint8_t kRamBank = 0xF8;
inline constexpr std::uint8_t kHardwareBank = 0xFF;
inline constexpr std::uint32_t kResetBase = 0xE000;   // where bank 0 sits at reset
inline constexpr std::uint16_t kZeroPage = 0x2000;
inline constexpr std::uint16_t kStack = 0x2100;

struct Vectors {
    std::uint16_t irq2 = 0;    // also BRK
    std::uint16_t irq1 = 0;    // the VDC
    std::uint16_t timer = 0;
    std::uint16_t nmi = 0;
    std::uint16_t reset = 0;
};

struct Rom {
    Bytes header;               // the 512 copier-header bytes as they were, or none
    Bytes data;                 // the ROM: whole 8 KB banks
    Vectors vectors;

    std::size_t bank_count() const { return data.size() / kBankSize; }
    // The bank as a physical bank number $00-$7F reaches it (a card smaller than 1 MB repeats: a power-of-two size wraps), or nullopt where the
    // card has none (a size that is not a power of two does not repeat).
    std::optional<std::size_t> rom_bank(std::uint32_t physical) const;
    ByteView bank(std::size_t index) const;
    // The byte at `offset` of the bank mapped at `physical`, or nullopt.
    std::optional<std::uint8_t> at(std::uint32_t physical, std::uint32_t offset) const;
};

// Parse an image (FormatError if its size is not 8 KB banks, with or without the header, or its RESET vector is not in $E000-$FFFF).
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// Is this file a HuCard image: the extension `.pce`, `.tg16` or `.sgx`, a size that is whole banks (with or without the header), and a RESET vector
// into the top page. Reads the header and the vectors.
bool probe(const std::filesystem::path& path);

// The chips' registers by the names the hardware documentation gives them, by their address in the hardware page ($0000-$17FF as the program
// writes it, when MPR0 holds $FF). A VDC and a timer register are one name for a read and another for a write.
const std::map<std::uint32_t, std::string>& registers_read();
const std::map<std::uint32_t, std::string>& registers_write();

// The name of the register at `offset` of the hardware page (the chips repeat their registers over their 1 KB: VDC $0000-$03FF every four bytes,
// VCE every eight, PSG every sixteen), or "IO_xxxx" for an offset no chip answers.
std::string register_name(std::uint16_t offset, bool write);

// The VDC's own registers, the ones ST0 selects and ST1/ST2 (or a store to $0002/$0003) write: "MAWR" for 0, "CR" for 5 ... ("R13" for a number the
// chip has no register at).
std::string vdc_register_name(unsigned index);

}  // namespace cb::formats::pce
