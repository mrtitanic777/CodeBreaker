// cb/formats/nes.hpp -- NES cartridge images (iNES and NES 2.0, `.nes`).
//
// The file is a 16-byte header ("NES" 1A, PRG size in 16 KB units, CHR size in 8 KB units, two flag bytes holding
// the mapper number's nibbles, mirroring, battery and trainer), an optional 512-byte trainer, the PRG, then the
// CHR. NES 2.0 marks itself in byte 7 (bits 2-3 = 2) and widens the mapper number and the sizes.
//
// Where the program sits. On NROM (mapper 0) 32 KB of PRG fill $8000-$FFFF, and 16 KB sit at $C000 and repeat at
// $8000. Other boards switch banks; as most of them do at power-on, the last 16 KB bank is taken to be fixed at
// $C000 and the first at $8000 -- the program's start and its vectors are there -- and the other banks are listed
// by number, their addresses not known without running the board.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/code_scan.hpp"

namespace cb::formats::nes {

inline constexpr std::size_t kHeaderSize = 16;
inline constexpr std::size_t kTrainerSize = 512;
inline constexpr std::size_t kPrgUnit = 0x4000;   // 16 KB
inline constexpr std::size_t kChrUnit = 0x2000;   // 8 KB
inline constexpr std::size_t kAddressSpace = 0x10000;

enum class Mirroring { Horizontal, Vertical, FourScreen };
std::string_view to_string(Mirroring m);   // "horizontal", "vertical", "four-screen"

// The three 6502 interrupt vectors at $FFFA, as the CPU reads them at power-on.
struct Vectors {
    std::uint16_t nmi = 0;
    std::uint16_t reset = 0;
    std::uint16_t irq = 0;
};

struct Rom {
    Bytes header;            // the 16 header bytes as they were
    bool nes2 = false;
    std::uint16_t mapper = 0;     // the board's number (0 = NROM: no bank switching)
    std::uint8_t submapper = 0;   // NES 2.0 only
    Mirroring mirroring = Mirroring::Horizontal;
    bool battery = false;         // the cartridge saves
    Bytes trainer;           // 512 bytes or none
    Bytes prg;               // program ROM
    Bytes chr;               // character (tile) ROM
    Bytes extra;             // anything after the CHR: some dumps carry a title
    Vectors vectors;
    std::uint32_t code_at = 0;    // the address PRG's first byte sits at ($C000 for a 16 KB PRG, else $8000)

    // The 16 KB PRG banks, in order (views into `prg`).
    std::size_t bank_count() const { return prg.size() / kPrgUnit; }
    ByteView bank(std::size_t index) const;

    // The 64 KB the CPU sees at power-on: RAM zero, PRG mapped (see the notes above).
    Bytes memory() const;

    // Where in PRG the byte the CPU reads at `address` is: an offset, or nullopt (below $8000).
    std::optional<std::uint32_t> mapped(std::uint32_t address) const;

    // Function starts in the fixed address space: the three vectors, then every JSR target reached by following the
    // code from them (fall through, branches, JMP, JSR). `decode` is the 6502 decoder.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// Parse an iNES / NES 2.0 image. FormatError if it is not one or the PRG (or a declared trainer) is cut short.
Rom parse(ByteView data);

// Read and parse a file. IoError if it cannot be read.
Rom read(const std::filesystem::path& path);

// Does the file start with the iNES magic? (Reads four bytes.)
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::nes
