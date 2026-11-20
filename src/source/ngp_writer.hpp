// source/ngp_writer.hpp -- a Neo Geo Pocket cartridge as TLCS-900/H assembly source that builds back into it.
//
//     image_writer::Written r = ngp_writer::reconstruct("game.ngp", "out");
//     r.source           game.asm (CodeBreaker's own build/tlcs900_asm reads it: ORG, DB, FILL, EQU, labels and the instruction text of decompile/tlcs900)
//     r.same             the source, built, is the cartridge file byte for byte
//     r.functions        {address: name}   (addresses as the CPU sees them: $200000 + the file offset)
//
// The 64-byte header is written field by field (licence text, the start address as the low two bytes of the `start` label, catalogue, sub-catalogue, colour flag, title, the
// reserved bytes). Code is what flow reaches from the start address: jumps, calls, `jr` / `jrl` / `calr` / `djnz` and a direct `jp` / `call`; and every constant address of
// the cartridge a program stores into the BIOS's interrupt vector table ($6FB8-$6FFF: `ld XWA,0x2012AB` then `ld (0x6FCC),XWA`), which is a function the BIOS calls. Functions are
// `start`, `isr_<vector>`, `sub_<address>`; jump targets `L_<address>`; an absolute address or 32-bit constant that points into the cartridge's data gets a label `D_<address>`.
// A hardware register or BIOS variable (formats/ngp's names: HW_WATCHDOG, K2GE_RAS_V, BIOS_INT_VBL ...) is written by name in an operand that is its address, defined by `EQU`
// lines. An instruction whose text would not assemble to its bytes (a second encoding of the same instruction, an absolute address in a longer form than its text gives) is
// written as `DB` with the instruction in a comment. Runs of one byte are `FILL count,value`.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/ngp.hpp"
#include "source/image_writer.hpp"

namespace cb::source::ngp_writer {

image_writer::Written write(const formats::ngp::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::ngp_writer
