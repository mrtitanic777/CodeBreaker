// source/genesis_writer.hpp -- a Genesis / Mega Drive program as assembly source that builds back into it.
//
//     image_writer::Written r = genesis_writer::reconstruct("game.md", "out");
//     r.source                  game.asm (Motorola syntax: asm68k, vasm)
//     r.same                    the source, built, is the cartridge image byte for byte
//     r.functions               {address: name}
//
// Genesis programs were written in 68000 assembly (most of them; some in C, compiled), and assembly is what comes back. Code is
// what the program's own flow reaches from the reset vector and the exception and interrupt handlers the vector table names
// (formats/genesis); every byte it reaches is written as an instruction, every other byte as data. The vector table is written as
// `dc.l` lines naming the handlers, the header at $100 as data with its fields named. Functions are named for their vector (`reset`,
// `vblank`, `hblank`, ...) or `sub_<address>`, jump targets `L_<address>`, and absolute and PC-relative operands naming the start of
// a line get that line's label -- so the source can be moved and edited, not only rebuilt. The console's ports (VDP, I/O, Z80 bus,
// TMSS) are written by name.
//
// It builds with CodeBreaker's own assembler (build/m68k_asm), or with asm68k / vasm.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/genesis.hpp"
#include "source/image_writer.hpp"

namespace cb::source::genesis_writer {

// The console's registers, by the names the hardware documentation gives them.
const std::map<std::uint32_t, std::string>& hardware();

// The source of `rom` (whose file was named `file_name`) written to `folder` as <stem>.asm, and rebuilt.
image_writer::Written write(const formats::genesis::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a Genesis image).
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::genesis_writer
