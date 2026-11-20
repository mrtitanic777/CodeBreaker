// source/wonderswan_writer.hpp -- a WonderSwan / WonderSwan Color cartridge as NASM source that builds back into it.
//
//     image_writer::Written r = wonderswan_writer::reconstruct("game.ws", "out");
//     r.source           game.asm (NASM, `bits 16`)
//     r.same             the source, built with `nasm -f bin`, is the cartridge file byte for byte
//     r.functions        {linear address: name}
//
// The program is the V30MZ's, in the code segment the footer's reset jump names: code is what flow reaches from the start address (a footer's
// `jmpf cs:ip`) -- jumps, branches and near calls inside that segment's 64 KB -- and every other byte is data (`db` rows). The ROM is written as up to three NASM
// sections, the bytes before the code segment, the segment itself with `vstart` so that a label is the offset the CPU uses, and the bytes after it, the
// last 16 (the footer, with the reset jump) among them. Each instruction is written as text the way Capstone reads it, in NASM's spelling, and what NASM
// does not give back byte for byte (the other encoding of a register move, a long form where a short one fits ...) is written as `db` with the
// instruction beside it, found out in rounds against the assembler (source/flat_rounds). Functions are `start` and `sub_<address>` (the linear address,
// five digits), jump targets `L_<address>`; the I/O ports an `in` or `out` names have the hardware's names (DISPLAY_CTRL, SPR_BASE, KEY_SCAN, INT_ENABLE,
// BANK_ROM0 ...), written as equates. Far jumps and calls into other segments are not followed.
//
// x86asm, CodeBreaker's own assembler, builds the source, in this process.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/code_scan.hpp"
#include "formats/wonderswan.hpp"
#include "source/image_writer.hpp"

namespace cb::source::wonderswan_writer {

// The I/O ports by number, by the names the WonderSwan hardware documentation gives them.
const std::map<std::uint32_t, std::string>& ports();

// The x86 instruction table the format's code scan reads: Capstone's 16-bit decoder over a memory image, for code running in the segment at `code_base`.
formats::Decoder decoder(std::uint32_t code_base);

image_writer::Written write(const formats::wonderswan::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::wonderswan_writer
