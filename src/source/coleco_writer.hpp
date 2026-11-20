// source/coleco_writer.hpp -- a ColecoVision cartridge as assembly source that builds back into it.
//
//     image_writer::Written r = coleco_writer::reconstruct("game.col", "out");
//     r.source                  game.asm (the Z80 SLEIGH description's syntax: .org, .equ, .byte)
//     r.same                    the source, built, is the cartridge byte for byte
//
// The cartridge (formats/coleco) is written through source/z80_writer from the game's start address and the vectors the BIOS jumps through
// (`start`, `rst08` .. `rst30`, `irq`, `nmi`), with the console's names: the VDP, sound chip and controller ports and the BIOS's entry points and
// system RAM, as the BIOS listing names them. The header (the $AA55 / $55AA, the five table pointers, the title) is data, described in the head
// comment.
#pragma once

#include <filesystem>
#include <string_view>

#include "formats/coleco.hpp"
#include "source/image_writer.hpp"
#include "source/z80_writer.hpp"

namespace cb::source::coleco_writer {

// What the ColecoVision calls its ports, and the fixed addresses outside a cartridge (BIOS entry points, system RAM).
const z80_writer::Machine& machine();

// The source of `program` (read from a file named `file_name`) written to `folder` as <stem>.asm, and rebuilt.
image_writer::Written write(const formats::coleco::Program& program, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a cartridge, UnsupportedError for a mapper's).
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::coleco_writer
