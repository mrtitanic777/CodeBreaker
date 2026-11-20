// source/msx_writer.hpp -- an MSX cartridge as assembly source that builds back into it.
//
//     image_writer::Written r = msx_writer::reconstruct("game.rom", "out");
//     r.source                  game.asm (sjasmplus / pasmo syntax)
//     r.same                    the source, built, is the cartridge byte for byte
//
// The cartridge (formats/msx) is written through source/z80_writer from its header's entry points, with the MSX's names: the VDP, PSG
// and PPI ports and the BIOS's documented entry points and system variables, as the MSX2 Technical Handbook names them.
#pragma once

#include <filesystem>
#include <string_view>

#include "formats/msx.hpp"
#include "source/image_writer.hpp"
#include "source/z80_writer.hpp"

namespace cb::source::msx_writer {

// What the MSX calls its ports, and the fixed addresses outside a cartridge (BIOS entry points, system variables).
const z80_writer::Machine& machine();

// The source of `program` (read from a file named `file_name`) written to `folder` as <stem>.asm, and rebuilt.
image_writer::Written write(const formats::msx::Program& program, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a cartridge, UnsupportedError for a mapper's).
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::msx_writer
