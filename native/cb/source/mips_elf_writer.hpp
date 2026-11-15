// cb/source/mips_elf_writer.hpp -- a little-endian MIPS ELF program (the PlayStation 2's `.elf`, the PSP's `.elf`, `.prx` and `EBOOT.PBP`) as assembly source
// that builds back into it. The two consoles' writers (ps2_writer, psp_writer) are this one with their processor, hardware and names.
//
//     image_writer::Written r = mips_elf_writer::write(machine, file, elf, elf_offset, "game.elf", "out");
//     r.source                  game.s (GNU as syntax: mips-as / psp-as read the instructions, `.org` and the data directives are CodeBreaker's)
//     r.same                    the source, built, is the file byte for byte
//     r.functions               {address: name}
//
// What a file holds, and how it is written (all of it is in the one source, in file order, each part at the address it is loaded at):
//
//     the ELF header     field by field: the identification bytes, the type and machine, the entry point (a function's label when it is one), the
//                        tables' offsets, the flags and the counts
//     the program and section header tables   a row to a line, with the program header's loaded address and sizes as numbers
//     each loaded piece  code (the executable sections, or the executable segments of a file with no sections) and data, at its address: `.org <address>`
//     everything else    the symbol and string tables, relocations (a PSP module's `.rel` sections and its relocation segment), debug information and
//                        a container's other files (an EBOOT.PBP's header, PARAM.SFO, icons) as data, so a rebuilt file is the original whatever was kept
//
// Code is what the flow reaches (decompile/mips, the R5900 or Allegrex variant: branch likely, a delay slot written as a line of its own after every jump)
// from the entry point, the functions the program exports, the symbols of the symbol table and the imports' stubs, and from what they call; a function
// only called through a pointer is found where one begins after another's end, or where a pointer to it is -- built in the code (`lui` with `addiu`,
// `ori` or a load or store's offset) or held in a table (a jump table's cases are the code after a jump that a pointer names). Every byte it reaches is
// an instruction, every other byte data (strings, words that hold an address of the program as that label, runs of one value, bytes). An instruction whose
// text would not assemble back to its word is written as `.word`, so the source is the program whatever the analysis guessed.
//
// A function the symbol table names has that name (sanitised), the entry point is `entry`, a PSP import its function's name when its NID is known
// (`sceDisplaySetMode`) or `<library>_<NID>`, and the rest `sub_<address>`; jump targets are `L_<address>`, data `D_<address>`. An address the code builds is
// `%hi(name)` and `%lo(name)` over a label, an `.equ` for hardware (the console's register names) and for RAM the program does not hold (`var_<address>`).
//
// It builds with CodeBreaker's own assembler (build/mips_asm).
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <string_view>

#include "cb/build/asm_text.hpp"
#include "cb/decompile/mips.hpp"
#include "cb/formats/mips_elf.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::mips_elf_writer {

// What a console says to the writer: its processor, its hardware, and the names its container gives functions.
struct Machine {
    decompile::mips::Variant variant = decompile::mips::Variant::R5900;
    const build::Encoder* encoder = nullptr;                                        // the assembler's instruction layer for the variant
    std::string description;                                                        // for the source's head ("PlayStation 2 ELF")
    std::function<std::optional<std::string>(std::uint32_t)> hardware;              // the name of the hardware register at an address, if it is one
    std::function<bool(std::uint32_t)> in_ram;                                      // is the address RAM (it is named `var_<address>` unless the program holds it)
    std::map<std::uint32_t, std::string> names;                                     // functions and data the container names, by address
    std::string extension = ".elf";                                                 // of the rebuilt file
};

// The source of the program (`file`: the file's bytes; `elf`: the ELF inside it, which starts at `elf_offset`, 0 for a bare ELF) written to `folder`
// as <stem>.s, and rebuilt.
image_writer::Written write(const Machine& machine, ByteView file, const formats::mips_elf::Elf& elf, std::size_t elf_offset, std::string_view file_name,
                            const std::filesystem::path& folder);

}  // namespace cb::source::mips_elf_writer
