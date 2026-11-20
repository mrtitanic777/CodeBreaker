// formats/atari2600.hpp -- Atari 2600 cartridge images (`.a26`, `.bin`): raw ROM, no header.
//
// A cartridge is 2 KB or 4 KB (one bank) or 8, 16 or 32 KB (2, 4 or 8 banks of 4 KB, which the programs switch by touching
// "hotspot" addresses at the top of the 4 KB window). The 6507 has 13 address lines, so the cartridge's window is $1000-$1FFF and every
// address with bit 12 set shows it: programs are written for $F000 (or $1000). The last four bytes of a bank are the vectors: reset at
// $FFC, IRQ/BRK at $FFE (the 6507 has no NMI pin). A 2 KB cartridge fills $F800-$FFFF and is mirrored below it.
//
//     size     scheme   hotspots (read or write of the address selects the bank)
//     2 K, 4 K none
//     8 K      F8       $FF8 -> bank 0, $FF9 -> bank 1
//     16 K     F6       $FF6 .. $FF9 -> banks 0 .. 3
//     32 K     F4       $FF4 .. $FFB -> banks 0 .. 7
//
// The scheme is taken from the size (the usual one for it). Other schemes (E0, 3F, FE, FA and the cartridges with extra RAM) are not
// recognised, so a file of another size is not taken as a cartridge.
//
// A file is told as a cartridge by its size, its extension (.a26 always; .bin only if the reset code is plausible) and the reset vector of
// its banks pointing into the window at code that decodes.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "formats/code_scan.hpp"

namespace cb::formats::atari2600 {

enum class Scheme { None, F8, F6, F4 };
std::string_view to_string(Scheme scheme);   // "none", "F8", "F6", "F4"

struct Vectors {
    std::uint16_t reset = 0;
    std::uint16_t irq = 0;
};

struct Rom {
    Bytes data;                   // the file as it is
    Scheme scheme = Scheme::None;
    std::uint32_t bank_size = 0;  // 2048 or 4096
    std::uint32_t window = 0;     // where a bank's first byte shows: $F000 or $1000 (+ $800 for 2 KB), by the reset vector
    std::vector<Vectors> vectors; // each bank's

    std::size_t bank_count() const { return data.size() / bank_size; }
    ByteView bank(std::size_t index) const;
    // The bank the first hotspot of the scheme selects: the one the program is taken to start in (the last, as the usual ones do).
    std::size_t start_bank() const { return bank_count() - 1; }

    // The addresses that select a bank (0x1FF8 ...), and what a touch of `address` (any with bit 12 set and these low 12 bits) selects.
    std::uint32_t first_hotspot() const;   // 0xFF8 (F8), 0xFF6 (F6), 0xFF4 (F4); 0 for none
    std::optional<std::size_t> bank_of_hotspot(std::uint32_t address) const;

    // The 64 KB the CPU sees with `bank` selected: the bank at every address with bit 12 set (a 2 KB cartridge twice in each 4 KB), zero
    // below. RAM and the chips are not in it.
    Bytes memory(std::size_t bank) const;

    // Where a bank-relative address is, as flow sees it: the address in the window (a mirror folded in), or nothing outside the cartridge.
    std::optional<std::uint32_t> fold(std::uint32_t address) const;
    // The offset in `data` of a window address of `bank`.
    std::uint32_t offset(std::size_t bank, std::uint32_t folded) const { return static_cast<std::uint32_t>(bank * bank_size + ((folded - window) & (bank_size - 1))); }

    // Function starts of every bank, as (bank << 16 | address): the vectors, then every JSR target reached from them. `decode` is the 6502's.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// Parse a raw cartridge. FormatError for a size that is not one of the schemes' or a reset vector outside the window.
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// Is the file a 2600 cartridge (see the top)? Reads the file, up to 32 KB.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::atari2600
