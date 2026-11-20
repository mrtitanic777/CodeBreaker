// source/n64_writer.hpp -- a Nintendo 64 ROM as assembly source that builds back into it.
//
//     image_writer::Written r = n64_writer::reconstruct("game.z64", "out");
//     r.source                  game.s (GNU as syntax, as the SDK's GCC and the homebrew assemblers' programs read)
//     r.same                    the source, built, is the ROM byte for byte (in the byte order the file had)
//     r.functions               {address: name}
//
// What a cartridge holds, and how it is written:
//
//     $0000-$003F   the header, field by field: the PI configuration word, clock rate, entry point (a function's label when it is one),
//                   release, the two CRCs (kept as data, and compared with the ones CodeBreaker computes from the program: the comment
//                   on each line says whether they match; the rebuild keeps them as they are, so it stays the ROM it was), the name, the
//                   category, game code, country and version
//     $0040-$0FFF   the boot code (IPL3) as data: `.word` lines, strings as text. It is the console's own program, run from SP memory
//                   before anything of the game, and is no part of what a game's source is
//     $1000-        the program, at its load address ($80001000 and the like): what the boot code copies to RDRAM (up to 1 MB). Code is
//                   what the flow from the entry point reaches, as in the PlayStation's writer (decompile/mips, the R4300 variant:
//                   branch likely, doubleword and FPU instructions; ERET ends a flow with no delay slot); a function that is only
//                   called through a pointer is found where one begins after another's end, or where a pointer to it is -- built in the
//                   code (`lui` with `addiu`, `daddiu` or `ori`, or a load or store's offset) or held in a table. Every byte it reaches is
//                   an instruction, every other byte data: strings, words (a word that holds an address in the program is that label),
//                   runs of one value as `.space`/`.fill`, the rest `.byte`
//     past the program's first megabyte   data, at its cartridge address ($B0101000 and up): what the game reads through the PI
//
// Functions are `entry` (the load address), `sub_<address>`; jump targets `L_<address>`, data `D_<address>`; an address the code builds is
// the pair's `%hi(name)` / `%lo(name)` over a label, an `.equ var_<address>` for RDRAM, or the register's name (MI_INTR_MASK, VI_ORIGIN,
// PI_STATUS, SP_STATUS ... the table in `hardware()`).
//
// It builds with CodeBreaker's own assembler (build/mips_asm, big-endian); the instructions are GNU as's. A `.v64` or `.n64` file's source is
// the ROM in the console's byte order, and the rebuilt file is made in the file's order again.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/n64.hpp"
#include "source/image_writer.hpp"

namespace cb::source::n64_writer {

// The console's registers, by physical address, by the names the hardware documentation gives them.
const std::map<std::uint32_t, std::string>& hardware();

// The source of `rom` (whose file was named `file_name`) written to `folder` as <stem>.s, and rebuilt.
image_writer::Written write(const formats::n64::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a ROM).
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::n64_writer
