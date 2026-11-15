// cb/source/psp_writer.hpp -- a PSP program (an executable `.elf`, a relocatable module `.prx`, or an `EBOOT.PBP` holding one) as assembly source that builds
// back into it.
//
//     image_writer::Written r = psp_writer::reconstruct("EBOOT.PBP", "out");
//     r.source                  EBOOT.s (the Allegrex's MIPS32 as GNU as reads it; the VFPU's instructions in CodeBreaker's syntax, see decompile/mips_ext.hpp)
//     r.same                    the source, built, is the file byte for byte (a PBP: the whole package, its PARAM.SFO and icons as data)
//
// The layout is mips_elf_writer's. What is the console's: the Allegrex's instructions (`ext` `ins` `seb` `wsbh` `clz` `max` `madd` ..., the VFPU's loads, stores and
// arithmetic) and the names of what a module imports and exports. An import stub is a function named for its NID -- `sceDisplaySetMode` when the NID is one of
// pspsdk's published ones (formats/psp), `<library>_<NID>` otherwise; a module's own exports (module_start, and what its export table lists) are named the same
// way. A module's relocations are kept as the data they are, so the rebuilt file is the original. An encrypted `~PSP` module is not handled.
#pragma once

#include <filesystem>
#include <string_view>

#include "cb/formats/psp.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::psp_writer {

// The source of `program` (whose file was named `file_name`) written to `folder` as <stem>.s, and rebuilt.
image_writer::Written write(const formats::psp::Program& program, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (FormatError when it is none of the formats, IoError when it cannot be read).
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::psp_writer
