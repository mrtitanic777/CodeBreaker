// source/atari2600_writer.hpp -- an Atari 2600 cartridge as assembly source that builds back into it.
//
//     image_writer::Written r = atari2600_writer::reconstruct("game.a26", "out");
//     r.source           game.asm, in dasm syntax
//     r.same             the source, built, is the cartridge image byte for byte
//     r.functions        {address: name}; an address is `bank << 16 | address in the window`
//
// 2600 programs were written in assembly, so assembly is what comes back, in the syntax of dasm -- the assembler the community's homebrew uses
// (`dasm game.asm -f3 -ogame.a26`; CodeBreaker's own, build/dasm_asm, reads the same file). Each 4 KB bank is its own stretch:
//
//         ORG $1000          where the bank is in the file
//         RORG $F000         where the 6507 sees it
//
// Code is what flow reaches from each bank's reset and IRQ vectors; every other byte is data. Function starts are named `reset`, `irq` or
// `sub_<address>` (`sub_<bank><address>` with banks), jump targets `L_<address>`, and an absolute operand naming the start of a line gets that
// line's label, so the source can be edited and moved. The TIA and RIOT registers have their hardware names (WSYNC, COLUBK, INTIM ...); an
// absolute-addressed zero-page address is written `LDA.w` so the bytes come back the same. The last four bytes of each bank are the vectors,
// written as `.word reset, irq`.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/atari2600.hpp"
#include "source/image_writer.hpp"

namespace cb::source::atari2600_writer {

// The console's registers by the names the hardware documentation gives them. The TIA's names differ between reading and writing an address.
const std::map<std::uint32_t, std::string>& tia_write();
const std::map<std::uint32_t, std::string>& tia_read();
const std::map<std::uint32_t, std::string>& riot();   // $0280 .. $0297

image_writer::Written write(const formats::atari2600::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::atari2600_writer
