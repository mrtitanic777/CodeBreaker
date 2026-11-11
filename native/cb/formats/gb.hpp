// cb/formats/gb.hpp -- Game Boy and Game Boy Color cartridge images (`.gb`, `.gbc`).
//
// The cartridge header is at $0100-$014F: the entry (usually NOP / JP), Nintendo's logo (the boot ROM checks it),
// the title, the Color and Super Game Boy flags, the cartridge type (which bank controller, RAM, battery), the ROM
// and RAM sizes, the header checksum (the boot ROM checks it) and the global checksum (nothing checks it). Bank 0
// is always at $0000-$3FFF; the controller switches the bank at $4000-$7FFF, which holds bank 1 at power-on --
// other banks are listed by number, their addresses known only by running the program.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/code_scan.hpp"

namespace cb::formats::gb {

inline constexpr std::size_t kBank = 0x4000;
inline constexpr std::array<std::string_view, 2> kExtensions = {".gb", ".gbc"};
inline constexpr std::uint32_t kEntry = 0x0100;
inline constexpr std::size_t kHeaderEnd = 0x150;

// Nintendo's logo bitmap as it sits at $0104.
extern const std::array<std::uint8_t, 48> kLogo;

// The interrupt vectors; each is a table slot of 8 bytes in the first bank.
struct InterruptVector {
    std::string_view name;
    std::uint32_t address;
};
inline constexpr std::array<InterruptVector, 5> kInterruptVectors = {{
    {"vblank", 0x40}, {"stat", 0x48}, {"timer", 0x50}, {"serial", 0x58}, {"joypad", 0x60},
}};

// The header's Game Boy Color flag ($0143).
enum class ColorSupport { None, Also, Only };

struct Rom {
    Bytes data;
    bool logo_ok = false;                 // the logo at $0104 is Nintendo's
    ColorSupport color = ColorSupport::None;
    std::string title;                    // the header's title (latin-1 decoded, trailing NULs and blanks dropped)
    bool sgb = false;                     // Super Game Boy support
    std::uint8_t cartridge_type = 0;      // $0147
    std::uint8_t header_checksum = 0;     // $014D
    bool header_checksum_ok = false;      // the checksum the boot ROM computes over $0134-$014C matches it

    // The memory bank controller the cartridge type names ("ROM", "MBC1", "MBC3+TIMER+BATTERY" ...), or
    // "type $XX" for a code that is not listed.
    std::string mbc() const;

    // The 16 KB banks (views into `data`; the last may be shorter).
    std::size_t bank_count() const { return (data.size() + kBank - 1) / kBank; }
    ByteView bank(std::size_t index) const;

    // The 64 KB the CPU sees at power-on: bank 0 at $0000, bank 1 at $4000, the rest zero.
    Bytes memory() const;

    // Function starts in the power-on address space ($0000-$7FFF): the entry at $0100, each interrupt vector that holds
    // code (unused ones are commonly $FF or a lone RETI), then every CALL and RST target reached by following the code
    // from them. `decode` is the SM83 decoder.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// FormatError if the image is too short to hold the header.
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// A `.gb`/`.gbc` file of at least 32 KB, or any file that has the logo at $0104.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::gb
