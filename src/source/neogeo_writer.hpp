// source/neogeo_writer.hpp -- a Neo Geo P-ROM as assembly source that builds back into it.
//
//     image_writer::Written r = neogeo_writer::reconstruct("game-p1.p1", "out");
//     r.source                  game-p1.asm (Motorola syntax: asm68k, vasm)
//     r.same                    the source, built, is the P-ROM byte for byte
//     r.functions               {address: name}
//
// The program is the 68000's: what flow reaches from the four callbacks the header's jump table gives the BIOS (`user`, `player_start`,
// `demo_end`, `coin_sound`), and from the vectors the P-ROM holds itself (the vertical blank `vblank`, the raster `timer`, ...). Every byte it
// reaches is written as an instruction, every other byte as data; the vector table is `dc.l` lines, the header at $100 data with its fields
// named. The console's registers (the LSPC's VRAM ports, the controller and system ports, the palette and fix-layer switches), the BIOS's
// entry points and the BIOS's work-RAM variables are written by name. The P-ROM's second megabyte is written at $200000 as data (the window a
// bank switch fills); banks beyond the first are not written and the result then says so.
//
// A word-swapped file and a `.neo` container come back as their P-ROM in the 68000's order: what is rebuilt and compared is that.
//
// The sound program, the M-ROM (the Z80's, in a `.neo` container or a file of its own), is written through source/z80_writer: the fixed
// 32 KB the Z80 runs from $0000 (`reset`, `irq` at $38, `nmi` at $66, then what they reach), the YM2610's and the 68000-link ports by name,
// as <stem>.m1.asm in the Z80 SLEIGH description's syntax, and rebuilt. Its banks (the rest of a 128 KB M-ROM) are not written. A `.neo` container's
// result says in its notes when the sound program did not rebuild; the Z80's functions are not in `functions` (their addresses are the Z80's)
// and are not judged: there is no Neo Geo sound machine in the Z80 lanes' models.
// It builds with CodeBreaker's own assembler (build/m68k_asm), or with asm68k / vasm.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/neogeo.hpp"
#include "source/image_writer.hpp"

namespace cb::source::neogeo_writer {

// The console's registers, the BIOS's calls and its variables, by the names the hardware documentation gives them.
const std::map<std::uint32_t, std::string>& hardware();

// The source of `rom` (whose file was named `file_name`) written to `folder` as <stem>.asm, and rebuilt.
image_writer::Written write(const formats::neogeo::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a Neo Geo P-ROM). A `.neo` container's M-ROM is written
// too, beside it (<stem>.m1.asm).
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

// The Z80's sound program `sound` (a file named `file_name`, or a `.neo`'s M-ROM) written to `folder` as <stem>.m1.asm, and rebuilt: `same` is true
// when the source builds the first 32 KB (all of a program that size or smaller) back. The function names are `reset`, `irq`, `nmi` and `sub_<address>`.
image_writer::Written write_sound(ByteView sound, std::string_view file_name, const std::filesystem::path& folder);

// The same for a raw M-ROM file (a `.m1`).
image_writer::Written reconstruct_sound(const std::filesystem::path& sound_file, const std::filesystem::path& folder);

}  // namespace cb::source::neogeo_writer
