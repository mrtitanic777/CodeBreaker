// source/jaguar_writer.hpp -- an Atari Jaguar program (a cartridge or a COFF executable) as assembly source that builds back into it.
//
//     image_writer::Written r = jaguar_writer::reconstruct("game.j64", "out");
//     r.source           game.asm (Motorola syntax: asm68k, vasm, rmac)
//     r.same             the source, built, is the program: a cartridge's ROM byte for byte, a COFF's loaded sections byte for byte
//     r.functions        {address: name}
//
// The program is the 68000's: code is what flow reaches from the start address (a cartridge's long at $800404, a COFF's entry), every other byte is
// data -- the GPU's and DSP's RISC code among it, which a 68000 only copies to their RAM. Every segment is an `org` at the address it loads at
// (a cartridge's ROM at $800000, a COFF section where it says). A cartridge's first $400 bytes (the boot signature) are data and its header at
// $400 is `dc.l` lines, the start address by its label. Functions are `start` or `sub_<address>`, jump targets `L_<address>`, absolute and
// PC-relative operands naming a line's start get its label, and Tom's and Jerry's registers are written by name (VMODE, BG, A1_BASE, B_CMD,
// G_CTRL, D_CTRL, JOYSTICK ...). The rebuilt program is kept beside the source as <stem>.built.<ext>: for a COFF, the original file with the
// sections' bytes replaced by what was built.
//
// It builds with CodeBreaker's own assembler (build/m68k_asm), which reads the same Motorola syntax as asm68k and vasm.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/jaguar.hpp"
#include "source/image_writer.hpp"

namespace cb::source::jaguar_writer {

// Tom's and Jerry's registers, by the names the hardware documentation gives them (jaguar.inc).
const std::map<std::uint32_t, std::string>& hardware();

image_writer::Written write(const formats::jaguar::Program& program, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::jaguar_writer
