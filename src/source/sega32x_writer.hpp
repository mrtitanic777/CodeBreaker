// source/sega32x_writer.hpp -- a Sega 32X cartridge as source that builds back into it.
//
//     image_writer::Written r = sega32x_writer::reconstruct("game.32x", "out");
//     r.source                  game.sh2.s (the SH-2 program, GNU as syntax); the 68000 side is game.asm
//     r.same                    both sources, built, put the cartridge back byte for byte
//     r.functions               {address: name}: the SH-2s' at $06000000 and up, the 68000's below $400000
//
// A 32X cartridge is two programs in one ROM (formats/sega32x): the Genesis's 68000 code, which source/genesis_writer writes as Motorola
// syntax (`<stem>.asm`, the whole ROM: the SH-2 program is data in it), and the SH-2 program the add-on's two processors run, which
// source/sh2_writer writes as GNU as source (`<stem>.sh2.s`): the vector tables, then code and data at $06000000, where the 32X copies it.
// Each is assembled by CodeBreaker's own assemblers and compared with its bytes; the ROM is put back together from the two (the 68000
// side's rebuilt ROM with the SH-2 program's rebuilt bytes over the program's place) and compared with the file: `<stem>.built.32x`.
//
// The two SH-2s share the program: the master starts at the header's first address, the slave at the second. Functions are `master_entry`
// and `slave_entry` for the starts, `sub_<address>`, and the 32X's registers (the system registers, the communication ports, PWM, the VDP's)
// and the SH7604's own (the divide unit, the DMA controller, the timer ...) are written by name.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/sega32x.hpp"
#include "source/image_writer.hpp"

namespace cb::source::sega32x_writer {

// The add-on's registers as the SH-2s see them, by address ($00004000 and up), and the SH7604's own.
const std::map<std::uint32_t, std::string>& hardware();

// The sources of `rom` (whose file was named `file_name`) written to `folder`, and the ROM rebuilt from them.
image_writer::Written write(const formats::sega32x::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a 32X cartridge).
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::sega32x_writer
