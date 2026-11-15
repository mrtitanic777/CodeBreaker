// cb/source/vectrex_writer.hpp -- a Vectrex cartridge as 6809 assembly source that builds back into it.
//
//     image_writer::Written r = vectrex_writer::reconstruct("game.vec", "out");
//     r.source           game.asm (6809 syntax: lwasm, as9; CodeBreaker's own build/m6809_asm reads it too)
//     r.same             the source, built, is the cartridge file byte for byte
//     r.functions        {address: name}
//
// The header is written field by field (`FCC "g GCE 1982"`, `FCB $80`, the music pointer, each title line, the closing zero). Code is what flow reaches from the
// entry (the byte after the header): branches, jumps and calls inside the ROM; a call into the system ROM ($E000-$FFFF) stays a call by the BIOS routine's name (Wait_Recal,
// Intensity_a, Moveto_d, Draw_VLc, Print_Str_d, Read_Btns, Joy_Digital ...), and the VIA 6522's registers at $D000 have their names (VIA_port_b, VIA_t1_cnt_hi, VIA_int_flags
// ...); they are written as `NAME EQU $addr` lines. Every other byte is data. Functions are `start` and `sub_<address>`, jump targets `L_<address>`. An indexed offset the
// instruction encodes in more bits than the shortest is written with `<` or `>`, direct addresses with `<`, so what is written assembles to the same bytes; an instruction
// that would not is written as `FCB` with the instruction in a comment. Runs of one byte are `FILL count, value`.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "cb/formats/code_scan.hpp"
#include "cb/formats/vectrex.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::vectrex_writer {

// The BIOS routines by address, and the VIA's registers.
const std::map<std::uint32_t, std::string>& bios();
const std::map<std::uint32_t, std::string>& via();

// The 6809 instruction table as the format's code scan reads it.
formats::Decoder decoder();

image_writer::Written write(const formats::vectrex::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::vectrex_writer
