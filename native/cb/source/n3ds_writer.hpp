// cb/source/n3ds_writer.hpp -- a Nintendo 3DS program (a `.3dsx`, or the ARM ELF devkitARM links it from) as assembly source that builds back into it.
//
//     image_writer::Written r = n3ds_writer::reconstruct("app.3dsx", "out");
//     r.source           app.s (GNU as syntax, `.syntax unified`, ARM state: what devkitARM's gcc makes of homebrew)
//     r.same             the source, built, is the file byte for byte
//     r.functions        {file offset: name}
//
// The file is written whole, in file order, an address being the byte's offset in the file (a 3DSX: its header, relocation headers, code, read-only data,
// data, the relocation tables and the icon and RomFS after them; an ELF: its headers, segments, symbols and debug information). Code is what flow reaches
// (decompile/arm, `Arch::V6K`: the ARMv6K integer set and the VFP) from the entry point, the symbols of an ELF, and the code a 3DSX's relocation table
// says is pointed at (a jump table's cases, a callback, a virtual function); the rest is data: strings as `.ascii`, runs as `.fill`, bytes. A word the
// relocation table patches (a literal-pool address, a vtable entry) is written as its target's label (`D_<offset>`, `sub_<offset>`) plus the difference
// between the loaded address and the file offset, so the source says what the pointer points at; an instruction whose text would not assemble back to its
// bits stays a `.word` with the text as a comment. A software interrupt is followed by a comment naming the Horizon kernel's call (`@ svcSendSyncRequest`).
//
// It builds with CodeBreaker's own assembler (build/arm_asm, ARMv6K). The rebuilt file is kept beside the source as <stem>.built.3dsx or .built.elf.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "cb/formats/n3ds.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::n3ds_writer {

// The system addresses programs load into registers (the kernel's configuration page, the thread-local storage, the I/O and GPU register windows).
const std::map<std::uint32_t, std::string>& hardware();
// The name of a Horizon kernel call by its SVC number ("SendSyncRequest" for 0x32), or empty.
std::string_view svc_name(std::uint32_t number);

image_writer::Written write(const formats::n3ds::Program& program, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::n3ds_writer
