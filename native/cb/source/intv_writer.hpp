// cb/source/intv_writer.hpp -- an Intellivision cartridge as CP1610 assembly source that builds back into it.
//
//     image_writer::Written r = intv_writer::reconstruct("game.rom", "out");
//     r.source           game.asm (as1600's syntax; CodeBreaker's own build/cp1610_asm reads it too, with the SEGMENT / ROMTAIL / FORMAT directives it adds)
//     r.same             the source, built, is the cartridge file byte for byte
//     r.functions        {word address: name}
//
// Every segment is `SEGMENT first,last` and `ORG`, its words code or `DECLE` data. Code is what flow reaches from the program's start (the third pointer of the header at $5000):
// branches, `J`, `JSR`, and an `MVII #address,R7`. An SDBD and the instruction it prefixes are two lines. The start is `start`, other functions `sub_<address>`, branch and jump
// targets `L_<address>`. An immediate that is the address of a line of a segment (`MVII #D_5200,R4`) is written as that label. A direct address that is a register of the STIC
// ($0000-$003F: the MOB registers, display enable, the colour stack, the border) or of the PSG ($01F0-$01FF, with the two controller ports) is written by its name, defined by an
// `EQU`. The words that are not code are `DECLE` rows and `FILL count,value` runs; the Intellicart's tables after the last segment are `ROMTAIL` bytes.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "cb/formats/intv.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::intv_writer {

// The STIC and PSG registers by word address.
const std::map<std::uint32_t, std::string>& hardware();

image_writer::Written write(const formats::intv::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::intv_writer
