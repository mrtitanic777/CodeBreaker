// source/cdi_writer.hpp -- the CD-RTOS modules of a Philips CD-i disc image (or a module file) as 68000 assembly source that builds back into them.
//
//     image_writer::Written r = cdi_writer::reconstruct("game.iso", "out");
//     r.source           game.asm (Motorola syntax)
//     r.same             the source, built, puts back every placed module's bytes, so the disc (or file) is the original
//     r.functions        {address: name}; an address is where the model places the module (formats/cdi) plus the offset in it
//
// Every executable module is an `org` at its address; its $30-byte header is `dc` lines with the fields named, its code what flow reaches from the
// module's entry (the base plus the execution offset: `main_<name>`), the rest data. The system is called through `TRAP #0` and a word naming the
// call: the pair is written as `TRAP #0` and `dc.w F_Exit` (the OS-9 function codes by name: F_Link, F_Fork, F_Exit, F_Sleep, I_Open, I_Read, I_Write ...),
// and flow goes on after the word. Functions are `main_<module>` and `sub_<address>`, jump targets `L_<address>`; the CD-i's chips (the MCD212 video
// chip at $200000, the slave processor, the real-time clock) have names. A module that is not executable (a data module, a descriptor) stays bytes of
// the disc. The rebuilt disc is kept beside the source as <stem>.built.<ext>.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/cdi.hpp"
#include "source/image_writer.hpp"

namespace cb::source::cdi_writer {

// The system calls by their function codes (the name written after `TRAP #0`), and the chips by address.
const std::map<std::uint32_t, std::string>& system_calls();
const std::map<std::uint32_t, std::string>& hardware();

image_writer::Written write(const formats::cdi::Program& program, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::cdi_writer
