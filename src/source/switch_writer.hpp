// source/switch_writer.hpp -- a Nintendo Switch program (a homebrew `.nro`, a `.nso`, or the AArch64 ELF devkitA64 links) as GNU as AArch64 source that builds back into it.
//
//     image_writer::Written r = switch_writer::reconstruct("app.nro", "out");
//     r.source           app.s (GNU as syntax: what devkitA64's gcc makes of homebrew)
//     r.same             the source, built, is the program: for a NRO the file byte for byte, for a NSO or an ELF the loaded image (the container is not written back)
//     r.functions        {address: name}   (an address is an offset in the image: a NRO's file offset)
//
// The image is written whole, in address order. Code is what flow reaches (decompile/a64) from the entry point, the dynamic symbols, the code the relocation tables say is
// pointed at (a callback, a virtual function, an init-array entry) and, past those, the unreached stretches of the text segment that begin like a function (a prologue of
// `stp x29, x30, [sp, #-N]!`, `sub sp, sp, #N` ...); the rest is data: strings as `.ascii` / `.asciz`, runs as `.fill`, words and bytes. An `adrp` and the instruction that
// completes its address (`add x0, x0, #lo12`, `ldr x1, [x0, #lo12]`) are written with the name of what they address: `adrp x0, D_0012A0` and `add x0, x0, :lo12:D_0012A0`.
// A word the dynamic relocations patch (R_AARCH64_RELATIVE: a vtable entry, a function pointer in the global offset table) is written as `.quad <target's label>`, and the
// relocation table's own entries as `.quad` of the label of the word they patch, the relocation type and the label of what it points at. A NRO's header is written field by
// field, its MOD0 block as words, and its asset section (icon, control data, RomFS) is kept in <stem>.assets.bin beside the source and written as `.incbin`.
// An instruction whose text would not assemble to its bits stays a `.word` with the text as a comment, and a `svc` is followed by a comment naming the Horizon kernel call.
//
// It builds with CodeBreaker's own assembler (build/a64_asm). The rebuilt image is kept beside the source as <stem>.built.nro (or .built.bin for the loaded image of a NSO or ELF).
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

#include "formats/nro.hpp"
#include "source/image_writer.hpp"

namespace cb::source::switch_writer {

image_writer::Written write(const formats::nro::Program& program, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::switch_writer
