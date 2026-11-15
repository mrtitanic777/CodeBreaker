// cb/source/gekko_writer.hpp -- a Nintendo GameCube, Wii or Wii U program (a DOL, an ELF, an RPX) as assembly source that builds back into it.
//
//     image_writer::Written r = gekko_writer::reconstruct("game.dol", "out");
//     r.source                  game.s (GNU as syntax: powerpc-eabi-as -mregnames, as devkitPPC's assembler reads it)
//     r.same                    the source, built, is the program byte for byte (a Wii U RPX: the decompressed file, formats/ppc_elf)
//     r.functions               {address: name}
//
// What a program holds, and how it is written (all of it is in the one source, in file order, each part at the address it is loaded at):
//
//     a DOL      its 256-byte header as words: the sections' file offsets, their load addresses (the labels `text0`, `data3` ...), their
//                sizes (`text0_end - text0`), the bss area and the entry point (a function's label); then each section
//     an ELF     the ELF header field by field, the program header table and the section header table a row to a line; each section
//                (code for the executable ones, data for the others) at its address; what no section holds (padding, the symbol and
//                string tables, debugging information) as data
//     an RPX     the same, from the decompressed file
//
// Code is what the flow from the entry point reaches (decompile/ppc: no delay slots, every instruction four bytes): a `bl` target is a
// function, so is the code after a return that begins as a function does (`stwu r1,-N(r1)`) and the code a pointer in the program names;
// the blocks after an unconditional branch that a pointer names are a jump table's cases. Every byte flow reaches is an instruction, every
// other byte data: strings, words (a word that holds an address in the program is that label), runs of one value as `.space`/`.fill`, the
// rest `.byte`. A function that an ELF's symbol table names has that name; others are `sub_<address>`, jump targets `L_<address>`,
// data `D_<address>`.
//
// An address the code builds (a `lis` with an `addi`, `ori` or a load or store's offset) is written `name@ha` and `name@l`, over a label of the
// program, an `.equ var_<address>` for memory the program does not hold (its bss), or the name of a hardware register
// (formats/gekko_map: VI_DPV, DI_STATUS, IPC_PPCCTRL ...). Every instruction's text is assembled and compared with its word; one that would
// not come back is written as `.long`, so the source is the program whatever the analysis guessed.
//
// It builds with CodeBreaker's own assembler (build/mips_asm's directive layer over decompile/ppc, big-endian).
#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

#include "cb/formats/ppc_program.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::gekko_writer {

// The source of `program` (whose file was named `file_name`) written to `folder` as <stem>.s, and rebuilt.
image_writer::Written write(const formats::ppc_program::Program& program, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is none of the three formats).
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::gekko_writer
