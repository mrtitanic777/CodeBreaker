// cb/source/o2_writer.hpp -- an Odyssey 2 cartridge as 8048 assembly source that builds back into it.
//
//     image_writer::Written r = o2_writer::reconstruct("game.o2", "out");
//     r.source           game.asm (`ORG $0400`, DB, FILL, labels, 8048 mnemonics; CodeBreaker's own build/i8048_asm reads it)
//     r.same             the source, built, is the cartridge file byte for byte
//     r.functions        {address: name}
//
// Code is what flow reaches from $0400 (where the BIOS starts the program): jumps, calls and conditional jumps inside the cartridge; a call into the BIOS ($0000-$03FF) stays a
// call to its address. A `JMP` or `CALL` is taken to stay in the memory bank of its own address. The entry is `start`, other functions `sub_<address>`, jump targets `L_<address>`.
// An instruction whose text would not assemble to its bytes is written as `DB` with the instruction in a comment. Runs of one byte are `FILL count,value`.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

#include "cb/formats/o2.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::o2_writer {

image_writer::Written write(const formats::o2::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::o2_writer
