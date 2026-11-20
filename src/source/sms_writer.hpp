// source/sms_writer.hpp -- a Master System / Game Gear program as assembly source that builds back into it.
//
//     image_writer::Written r = sms_writer::reconstruct("game.sms", "out");
//     r.source                  game.asm (the Z80 SLEIGH description's syntax, source/z80_source: .org, .equ, .byte)
//     r.same                    the source, built, is the cartridge image byte for byte
//     r.functions               {address: name}
//
// These programs were written in assembly, so assembly is what comes back. Code is what the program's own flow reaches from the
// reset, interrupt and NMI vectors in the pages mapped at power-on (formats/sms); every byte it reaches is written as an
// instruction, every other byte as data. Function starts are named for their vector (`reset`, `irq`, `nmi`) or `sub_<address>`,
// jump targets `L_<address>`, and absolute words naming the start of a line get that line's label, so the source can be moved and
// edited, not only rebuilt. The console's ports and the mapper's registers are written by name.
//
// It builds with CodeBreaker's one assembler (`assemble --cpu z80 --sleigh`): the instructions are the Z80 description's text, a page
// begins at its `.org` (the slot it is seen in), and the file is the pages in order.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/sms.hpp"
#include "source/image_writer.hpp"

namespace cb::source::sms_writer {

// The console's I/O ports, by the names the hardware documentation gives them (the Game Gear's own are added for it).
const std::map<std::uint32_t, std::string>& ports();
const std::map<std::uint32_t, std::string>& game_gear_ports();
// The mapper's registers, written into RAM space.
const std::map<std::uint32_t, std::string>& mapper_registers();

// The slot (0, 1 or 2) a page is shown in at power-on: pages 0 and 1 in slots 0 and 1, every later page in slot 2.
std::uint32_t slot_of(std::size_t page);

// The source of `rom` (whose file was named `file_name`) written to `folder` as <stem>.asm, and rebuilt.
image_writer::Written write(const formats::sms::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a cartridge image).
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::sms_writer
