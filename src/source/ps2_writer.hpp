// source/ps2_writer.hpp -- a PlayStation 2 program (an Emotion Engine `.elf`, or the BOOT2 program of a disc image) as assembly source that builds back
// into it.
//
//     image_writer::Written r = ps2_writer::reconstruct("homebrew.elf", "out");
//     r.source                  homebrew.s (GNU as syntax for the R5900: the instructions of mips-as with `-march=r5900`; the layout directives are CodeBreaker's)
//     r.same                    the source, built, is the ELF byte for byte
//
// The layout is mips_elf_writer's (the ELF's headers, sections and what no section holds, in file order). What is the console's: the R5900 (128-bit LQ and SQ,
// the multimedia instructions, the three-operand multiplies, COP2's VU0 moves and macro instructions) and the Emotion Engine's hardware registers by their
// manuals' names (GS_CSR, GIF_STAT, D2_CHCR, I_STAT, T0_COUNT ...: formats/ps2.hpp). The VU0 and VU1 micro-programs are data (they are sent by DMA, not run
// by the main processor), and so is every IOP (I/O processor) module a program carries.
//
// A disc image is read for its SYSTEM.CNF's BOOT2 program; the source written is that ELF's, and the file rebuilt is that ELF.
#pragma once

#include <filesystem>
#include <string_view>

#include "formats/ps2.hpp"
#include "source/image_writer.hpp"

namespace cb::source::ps2_writer {

// The source of `program` (whose file was named `file_name`) written to `folder` as <stem>.s, and rebuilt.
image_writer::Written write(const formats::ps2::Program& program, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (FormatError when it is none of the formats, IoError when it cannot be read).
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::ps2_writer
