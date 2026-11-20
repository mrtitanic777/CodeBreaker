// formats/atari5200.hpp -- Atari 5200 cartridge images (`.a52`, or a raw `.bin` / `.rom`): the ROM and nothing else.
//
// Where the program sits. The cartridge window is $4000-$BFFF; a ROM ends at its top ($BFFF), so 32 KB fills $4000-$BFFF, 16 KB sits at $8000, 8 KB
// at $A000 and 4 KB at $B000 (a 16 KB cartridge of one chip, which the console also shows at $4000, is the same bytes). The last four bytes are
// the cartridge header the BIOS reads: two flag bytes (the second says whether the BIOS title screen is skipped) and the program's start address,
// a little-endian word at $BFFE. The 5200 has no interrupt vectors of its own in the cartridge: the BIOS vectors interrupts through RAM ($0200 and
// up), which the program fills in, so the start address is the one fixed entry. A 40 KB banked cartridge (Bounty Bob Strikes Back) is not modelled.
//
// A raw `.bin` is a 5200 cartridge when its size is one of the four and the start address lies inside the ROM's own window; the Atari 2600's
// images, which have the same sizes, have their vectors at $F000 and up.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "formats/code_scan.hpp"

namespace cb::formats::atari5200 {

struct Rom {
    Bytes data;                   // the ROM, as it was
    std::uint32_t base = 0;       // the address its first byte sits at
    std::uint8_t flag_a = 0;      // $BFFC
    std::uint8_t flag_b = 0;      // $BFFD: bit 7 set = the BIOS skips its title screen
    std::uint16_t start = 0;      // $BFFE: where the program starts

    // The 64 KB the CPU sees: the ROM at `base`, the rest zero (RAM and the chips are not in it).
    Bytes memory() const;
    // The offset in `data` of `address`, or nullopt where there is no ROM.
    std::optional<std::uint32_t> offset(std::uint32_t address) const;
    // Function starts: the start address, then every JSR target reached from it.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// FormatError if the size is not 4, 8, 16 or 32 KB or the start address is outside the ROM.
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// A `.a52` of a valid size; or a `.bin` / `.rom` of a valid size whose start address is inside the ROM. Reads the file's last four bytes only.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::atari5200
