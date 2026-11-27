// source/gba_writer.hpp -- a Game Boy Advance cartridge as assembly source that builds back into it.
//
//     image_writer::Written r = gba_writer::reconstruct("game.gba", "out");
//     r.source                  game.s (GNU as syntax, `.syntax unified`, as devkitARM's gcc produces it)
//     r.same                    the source, built, is the cartridge byte for byte
//     r.functions               {address: name}
//
// GBA programs were written in C (compiled by GCC for the ARM7TDMI, mostly in Thumb state) and assembly, and what comes back is
// assembly: every function in the order it lies in the ROM, with `.arm` or `.thumb` before the instructions of the state it runs in, and
// the literal pools that follow a function's code as `.word` lines under a label of their own. Code is what the program's own flow
// reaches from the entry branch at $08000000 and from the functions it calls -- the processor's state is part of an address (its lowest
// bit), a branch stays in its state, and the state changes only through `bx`, so a `bx r0` is followed to where it goes when the register
// was just loaded from a literal pool or made from the pc (`ldr r0, =func+1`; `add r0, pc, #1`; `mov lr, pc` before it makes the call
// come back). A function that is only called through a pointer is found where one begins after another's end (the bytes after a return
// and its literal pool, when they read as a function: every halfword or word an instruction, ending in a return), or where a pointer to
// it is -- a word of a literal pool, or in a table, with the pointer's lowest bit saying Thumb -- and its bytes read as a function.
// Every byte it reaches is written as an instruction, every other byte as data: strings as `.ascii`/`.asciz`, words as `.word` (a word
// that holds an address in the ROM is its label: jump tables, pointer tables and literal pools read as such), runs of one value as
// `.space`/`.fill`, the rest as `.byte`.
//
// An instruction is written as text only when the text assembles back to the instruction's own bytes (decompile/arm: the assembler's
// round trip); the few encodings the assembler would write differently (`adds r0, r0, #3` in its 3-bit form) stay as `.hword`/`.word`
// with the instruction as a comment, so the source always builds the ROM it came from.
//
// Functions are named `arm_<address>` and `thumb_<address>` after the state they run in (`entry` for the header's branch), the address
// being the function's own; the key of a Thumb function in the result has its lowest bit set, as a pointer to it does. Jump targets are
// `L_<address>`, data `D_<address>`. The hardware's registers have the names libgba and libtonc give them (REG_DISPCNT, REG_DMA3CNT ...,
// the table in `hardware()`), other addresses outside the ROM are `var_<address>` equates. The first 192 bytes are the cartridge header,
// written field by field (the logo, the title, the game and maker codes, the fixed byte, the checksum).
//
// It builds with CodeBreaker's own assembler (the one assembler). The instructions are GNU as's, so a toolchain's `as` takes them; the `.org`
// that says where the ROM is mapped is CodeBreaker's (the one assembler).
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/gba.hpp"
#include "source/image_writer.hpp"

namespace cb::source::gba_writer {

// The console's registers, by address ($04000000-$040003FE), by the names libgba gives them.
const std::map<std::uint32_t, std::string>& hardware();

// The source of `rom` (whose file was named `file_name`) written to `folder` as <stem>.s, and rebuilt.
image_writer::Written write(const formats::gba::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a GBA cartridge).
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::gba_writer
