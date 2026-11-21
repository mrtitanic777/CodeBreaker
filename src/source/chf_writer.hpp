// source/chf_writer.hpp -- a Channel F cartridge as F8 assembly source that builds back into it.
//
//     image_writer::Written r = chf_writer::reconstruct("game.chf", "out");
//     r.source           game.asm (the F8 SLEIGH description's syntax under the one directive layer: `.org 0x0800`, `.byte`, `.space`, `.equ`, labels; the one assembler reads it)
//     r.same             the source, built, is the cartridge file byte for byte
//     r.functions        {address: name}
//
// Code is what flow reaches from $0800 (where the BIOS starts the cartridge): branches, JMP and PI inside the cartridge. A call into the BIOS ($0000-$07FF) stays a call to its address,
// and by its name (`BIOS_DRAWCHAR`, `BIOS_DELAY` ...) where the address is one the documentation names, defined by an `EQU`. A `DCI` whose constant is the address of a line of the
// cartridge is written with that line's label (`DCI D_0A40`). The entry is `start`, other functions `sub_<address>`, branch and jump targets `L_<address>`. An instruction whose
// text would not assemble to its bytes is written as `DB` with the instruction in a comment. Runs of one byte are `FILL count,value`.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

#include "formats/chf.hpp"
#include "source/image_writer.hpp"

namespace cb::source::chf_writer {

image_writer::Written write(const formats::chf::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::chf_writer
