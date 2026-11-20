// source/vita_writer.hpp -- a PlayStation Vita homebrew program (an unencrypted SELF such as a `.vpk`'s eboot.bin, or the ELF inside it) as assembly source that
// builds back into it.
//
//     image_writer::Written r = vita_writer::reconstruct("eboot.bin", "out");
//     r.source           eboot.s (GNU as syntax, `.syntax unified`, `.thumb`: what VitaSDK's gcc makes of homebrew)
//     r.same             the source, built, is the program's image byte for byte (the loadable segments inflated and laid end to end; compared as the plain ELF
//                        formats::vita::Program::to_elf makes of it, which is also what the lane keeps as `<stem>.built.velf` and runs beside the original)
//     r.functions        {image offset: name}
//
// The image is written whole, an address being the byte's offset in the image. Code is what flow reaches in Thumb state (decompile/arm, `Arch::V7A`) from the module's
// start and stop functions and from what they call; an instruction inside an IT block is written as it decodes, with a comment naming the condition that applies;
// the instructions the tables do not say (most of Thumb-2's 32-bit integer set and NEON) are written as `.word` with the disassembler's text as a comment, so the
// source is the program whatever the analysis guessed. An imported function's stub is named from its NID (`sceCtrlPeekBufferPositive`, or `<library>_<NID>`), so a
// call reads `bl sceCtrlPeekBufferPositive`; a pointer-sized literal that holds an address of the program is written as its label.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

#include "formats/vita.hpp"
#include "source/image_writer.hpp"

namespace cb::source::vita_writer {

image_writer::Written write(const formats::vita::Program& program, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::vita_writer
