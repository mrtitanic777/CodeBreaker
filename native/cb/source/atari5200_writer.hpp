// cb/source/atari5200_writer.hpp -- an Atari 5200 cartridge as assembly source that builds back into it.
//
//     image_writer::Written r = atari5200_writer::reconstruct("game.a52", "out");
//     r.source           game.asm, in dasm syntax
//     r.same             the source, built, is the cartridge file byte for byte
//     r.functions        {address: name}
//
// As for the 2600 and 7800 (source/atari2600_writer, source/atari7800_writer), in the syntax of dasm (`dasm game.asm -f3 -ogame.a52`; CodeBreaker's
// own build/dasm_asm reads it too). `ORG` is the address the ROM sits at ($4000 for 32 KB .. $B000 for 4 KB), so the image starts at the first
// byte. Code is what flow reaches from the start address in the last two bytes; every other byte is data. Functions are `start` or `sub_<address>`;
// jump targets `L_<address>`. The GTIA, ANTIC and POKEY registers have their hardware names (COLBK, DMACTL, AUDF1 ...) -- a register named by a
// store takes the write name, by a load the read name (the chips have a different register at the same address). The last four bytes are the
// cartridge header: `.byte flags`, `.word start`.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "cb/formats/atari5200.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::atari5200_writer {

// The registers by the names the hardware documentation gives them, at the address of their first copy: GTIA $C000-$C01F, ANTIC $D400-$D40F,
// POKEY $E800-$E80F (each repeats through its window; the mirrors keep their numbers).
const std::map<std::uint32_t, std::string>& gtia_write();
const std::map<std::uint32_t, std::string>& gtia_read();
const std::map<std::uint32_t, std::string>& antic_write();
const std::map<std::uint32_t, std::string>& antic_read();
const std::map<std::uint32_t, std::string>& pokey_write();
const std::map<std::uint32_t, std::string>& pokey_read();

image_writer::Written write(const formats::atari5200::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::atari5200_writer
