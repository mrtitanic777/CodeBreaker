// source/atari7800_writer.hpp -- an Atari 7800 cartridge as assembly source that builds back into it.
//
//     image_writer::Written r = atari7800_writer::reconstruct("game.a78", "out");
//     r.source           game.asm, in dasm syntax
//     r.same             the source, built, is the cartridge file byte for byte (the 128-byte header included)
//     r.functions        {address: name}; an address is `bank << 16 | address` (the bank is 0 unless the board is a SuperGame)
//
// As for the 2600 (config/platforms/atari2600.json), in the syntax of dasm (`dasm game.asm -f3 -ogame.a78`; CodeBreaker's own build/dasm_asm reads it too).
// The header is the first stretch, written as `.byte` lines (its fields named in comments) at `ORG $0000`; the ROM follows at `ORG $0080` with
// `RORG` the address it runs at. Code is what flow reaches from the NMI, RESET and IRQ vectors; every other byte is data. Functions are `reset`, `nmi`,
// `irq` or `sub_<address>`; jump targets `L_<address>`; the TIA, MARIA and RIOT registers have their hardware names (WSYNC, BACKGRND, CTRL,
// DPPH, SWCHA, INTIM ...). The last six bytes are the vectors, `.word nmi, reset, irq`. A SuperGame board's switchable banks are reached only from
// code that is known to run with that bank selected: a jump from the fixed bank into $8000-$BFFF is not followed (which bank is shown is a run-time fact),
// so those banks are mostly data.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/atari7800.hpp"
#include "source/image_writer.hpp"

namespace cb::source::atari7800_writer {

// The registers by the names the hardware documentation gives them (TIA: by write / read; MARIA: $20-$3F; RIOT: $0280-$0297).
const std::map<std::uint32_t, std::string>& tia_write();
const std::map<std::uint32_t, std::string>& tia_read();
const std::map<std::uint32_t, std::string>& maria();
const std::map<std::uint32_t, std::string>& riot();

image_writer::Written write(const formats::atari7800::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::atari7800_writer
