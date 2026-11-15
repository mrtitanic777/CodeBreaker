// cb/source/ps3_writer.hpp -- a PlayStation 3 homebrew program (a PSL1GHT ELF, or an unencrypted SELF holding one) as assembly source that builds back into it.
//
//     image_writer::Written r = ps3_writer::reconstruct("app.elf", "out");
//     r.source           app.s (GNU as syntax: the 64-bit PowerPC instructions of the PPU, `.long` words for the VMX instructions the table does not say)
//     r.same             the source, built, is the program's image byte for byte (the loadable segments end to end; compared as the plain ELF
//                        formats::ps3::Program::to_elf makes of it, which is also what the lane keeps as `<stem>.built.elf` and runs beside the original)
//     r.functions        {image offset: name}
//
// The image is written whole, an address being the byte's offset in the image. Code is what flow reaches from the entry function and from the function descriptors
// the data holds (a doubleword that is the address of code followed by the program's TOC: the `.opd` of the PowerPC64 ABI), so a function only called through a
// pointer is found; every other byte is data (strings as `.ascii`, runs as `.fill`, bytes). A descriptor's code address is written as its function's label plus the
// difference between the loaded address and the offset. An imported function's descriptor is named from its NID (`ioPadInit`, or `<library>_<NID>`), an `sc` is
// followed by a comment naming the lv2 system call (`# lv2 sys_ppu_thread_exit`, from the `li r11, N` before it), and a vector (VMX) instruction is a `.long` with
// the disassembler's text as a comment. SPU programs inside the image stay data.
//
// It builds with CodeBreaker's own assembler (build/mips_asm's directive layer, big-endian, over decompile/ppc with its 64-bit rows).
#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

#include "cb/formats/ps3.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::ps3_writer {

image_writer::Written write(const formats::ps3::Program& program, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::ps3_writer
