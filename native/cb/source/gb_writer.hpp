// cb/source/gb_writer.hpp -- a Game Boy program as assembly source that builds back into it.
//
//     image_writer::Written r = gb_writer::reconstruct("game.gb", "out");
//     r.source                  game.asm (RGBDS syntax)
//     r.same                    the source, built, is the cartridge image byte for byte
//     r.functions               {address: name}
//
// Game Boy programs were written in assembly, so assembly is what comes back. Code is what the program's own flow reaches from the
// entry at $0100 and the interrupt vectors in the two banks mapped at power-on (formats/gb); every byte it reaches is written as an
// instruction, every other byte as data. The cartridge header ($0104-$014F) is written as data with its fields named in comments.
// Functions are named for their vector (`entry`, `vblank`, `stat`, `timer`, `serial`, `joypad`) or `sub_<address>`, jump targets
// `L_<address>`; the I/O registers by the names Pan Docs gives them (rLCDC, rSTAT, ...), as hardware.inc does.
//
// It builds with CodeBreaker's own assembler (build/sm83_asm) or with RGBDS (`rgbasm -o game.o game.asm && rgblink -o game.gb
// game.o`).
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "cb/formats/gb.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::gb_writer {

// The I/O registers, as hardware.inc names them.
const std::map<std::uint32_t, std::string>& registers();
// The bank controller's registers (writes into ROM space), as hardware.inc names them.
const std::map<std::uint32_t, std::string>& mbc_registers();

// The source of `rom` (whose file was named `file_name`) written to `folder` as <stem>.asm, and rebuilt.
image_writer::Written write(const formats::gb::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a Game Boy image).
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::gb_writer
