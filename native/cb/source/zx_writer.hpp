// cb/source/zx_writer.hpp -- a ZX Spectrum program as assembly source that builds back into it.
//
//     image_writer::Written r = zx_writer::reconstruct("game.z80", "out");
//
// The program block (formats/zx: a snapshot's RAM above the screen, a tape's code block) is written through source/z80_writer with the
// Spectrum's names: its ULA port, the 48K ROM's documented entry points and the system variables, as the Spectrum manual and The Complete
// Spectrum ROM Disassembly name them.
#pragma once

#include <filesystem>
#include <string_view>

#include "cb/formats/zx.hpp"
#include "cb/source/image_writer.hpp"
#include "cb/source/z80_writer.hpp"

namespace cb::source::zx_writer {

// What the Spectrum calls its ports, and the fixed addresses outside the block (ROM routines, system variables).
const z80_writer::Machine& machine();

// The source of `program` (read from a file named `file_name`) written to `folder` as <stem>.asm, and rebuilt.
image_writer::Written write(const formats::zx::Program& program, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a snapshot or a tape).
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::zx_writer
