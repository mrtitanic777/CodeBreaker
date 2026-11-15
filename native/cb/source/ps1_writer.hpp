// cb/source/ps1_writer.hpp -- a PlayStation program as assembly source that builds back into it.
//
//     image_writer::Written r = ps1_writer::reconstruct("game.exe", "out");
//     r.source                  game.s (GNU as syntax, as the SDK's GCC and PSn00bSDK produce it)
//     r.same                    the source, built, is the executable byte for byte
//     r.functions               {address: name}
//
// PlayStation programs were written in C and compiled with GCC for the R3000A, and what comes back is assembly: every function, in the
// order it lies in the executable, with `.set noreorder` and each delay slot on a line of its own. Code is what the program's own flow
// reaches from the entry point in the header (decompile/mips knows which instruction ends a flow: a jump or return, after its delay
// slot) and from the functions it calls; a function that is only called through a pointer is found where one begins after another's
// end (the bytes after a jump and its delay slot, when they read as a function: every word an instruction, ending in a jump or return)
// or where a pointer to it is -- built in the code (`lui`/`addiu`) or held in a table -- and its bytes read as a function.
// Every byte it reaches is written as an instruction, every other byte as data: strings as `.ascii`/`.asciz`, words as `.word` (a word
// that holds an address in the program is that label: jump tables and pointer tables read as such), runs of one value as `.space`/`.fill`,
// the rest as `.byte`.
//
// Functions are named `entry` (the header's start address) or `sub_<address>`, jump targets `L_<address>`, data `D_<address>`. An address
// the code builds with a pair (`lui` then `addiu`, `ori`, or a load or store's offset) is written as the pair's `%hi(name)` and `%lo(name)`,
// so a function can move and the program still means the same: a label in the program, an `.equ` named `var_<address>` for a variable
// outside the file's text (RAM past the program), or the hardware register's name (GPU_GP0, I_STAT, DMA2_CHCR ... the table in
// `hardware()`). The file's first 2048 bytes are the PS-X EXE header, written field by field.
//
// It builds with CodeBreaker's own assembler (build/mips_asm). The instructions are GNU as's, so a toolchain's `as` takes them; the
// header and the `.org` that says where the program loads are CodeBreaker's (build/mips_asm.hpp).
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "cb/formats/psx.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::ps1_writer {

// The console's registers, by physical address, by the names the hardware documentation gives them.
const std::map<std::uint32_t, std::string>& hardware();

// The source of `exe` (whose file was named `file_name`) written to `folder` as <stem>.s, and rebuilt.
image_writer::Written write(const formats::psx::Exe& exe, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a PS-X EXE).
image_writer::Written reconstruct(const std::filesystem::path& exe_file, const std::filesystem::path& folder);

}  // namespace cb::source::ps1_writer
