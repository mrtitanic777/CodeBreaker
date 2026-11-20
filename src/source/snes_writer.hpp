// source/snes_writer.hpp -- a Super Nintendo cartridge as assembly source that builds back into it.
//
//     image_writer::Written r = snes_writer::reconstruct("game.sfc", "out");
//     r.source, r.config        game.s (ca65 syntax, `.p816`) and game.cfg (ld65)
//     r.same                    the source, built, is the file byte for byte (a copier header included)
//     r.functions               {address: name}; an address is the 24-bit canonical address with the state the function starts in
//                               above it: bit 24 the accumulator is 16 bits wide (M clear), bit 25 the index registers are (X clear), bit 26 native mode
//
// Source syntax: ca65. cc65's assembler has the 65C816 as a CPU (`.p816`), reads the same `$` numbers, the `a:` / `z:` / `f:` size prefixes (the 65816's
// direct-page, absolute and long forms), `.a8` / `.a16` / `.i8` / `.i16` (the register widths that make an immediate operand one byte or two), `.byte` / `.word`
// / `.segment`, and has the linker ld65 whose memory areas place each bank. It is the assembler nearly all SNES homebrew written in assembly uses with
// the 65xx core of this repository (table, encoder, directive layer, ld65 configuration reader) shared with the NES, Atari Lynx and PC Engine writers;
// WLA-DX has the same instructions with a `.MEMORYMAP` / `.ROMBANKMAP` in the source, where ca65 keeps the layout in the configuration file.
//
// The register widths change an instruction's length, so flow tracks them: from the reset vector (emulation mode, 8 bits) through `REP`, `SEP`, and `CLC; XCE`
// (which sets or clears emulation mode when the carry is known there), each instruction is decoded under the widths the path that reached it left. An
// instruction that takes an immediate of the register's width gets a `.a8` / `.a16` / `.i8` / `.i16` before it when the assembler's last setting differs. The
// first path to arrive at an address decides the widths there. Entries are the vectors (RESET in emulation mode; NMI, IRQ, COP, BRK, ABORT in native mode, 8
// bits assumed); calls are followed (JSL, JSR, also the branches and JMP and JML to a constant), and a function is every call target.
//
// Banks are segments `BANKn`, linked at the address the code runs at: LoROM at $nn8000 (the mirror, $00 or $80, that more of the program's long addresses use),
// HiROM at $C0nn0000. The 64-byte internal header is written as its fields (the title, the map mode ... the checksum, the sixteen-bit vectors, a vector that is a
// label as the label). Function starts are `reset`, `nmi`, `irq`, `cop`, `brk`, `abort`, `emu_...` or `sub_<address>`; jump targets `L_<address>`; a data address an
// instruction names `D_<address>`; the chips' registers have their hardware names (INIDISP, VMDATAL, APUIO0, DMAP0, NMITIMEN, MDMAEN ...), the direct page `dp_20`.
// A 16-bit operand that names a label of another bank is written `.loword(label)`.
//
// An instruction whose text does not assemble back into its own bytes is written as `.byte` data, so the source always is the file. The SPC700 sound program
// (the APU's code, which the CPU uploads through $2140-$2143 from ROM) is data. It builds with CodeBreaker's own assembler (build/m6502_asm with the 65816
// encoder) or the cc65 suite (`ca65 game.s` and `ld65 -C game.cfg -o game.sfc game.o`; the memory areas are in file order, each filled to its size).
#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

#include "formats/snes.hpp"
#include "source/image_writer.hpp"

namespace cb::source::snes_writer {

// The bits of a function's address that say the state it starts in.
inline constexpr std::uint32_t kStartsWith16BitAccumulator = 1u << 24;
inline constexpr std::uint32_t kStartsWith16BitIndex = 1u << 25;
inline constexpr std::uint32_t kStartsNative = 1u << 26;
inline constexpr std::uint32_t kAddressMask = 0xFFFFFF;

image_writer::Written write(const formats::snes::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::snes_writer
