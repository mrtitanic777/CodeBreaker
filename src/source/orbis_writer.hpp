// source/orbis_writer.hpp -- a PlayStation 4 or PlayStation 5 program as NASM source that builds back into it.
//
//     image_writer::Written r = orbis_writer::reconstruct("hello_world.elf", "out");
//     r.source           hello_world.asm (NASM, `bits 64`)
//     r.same             the source, built with `nasm -f bin`, is the ELF byte for byte (for a SELF: the ELF it holds, laid out as its headers say)
//     r.functions        {virtual address: name}
//
// The ELF (formats/orbis) is written whole by source/x64_writer: its loaded segments are NASM sections placed at their virtual addresses, so a label is the address
// the program runs at. Code is what flow reaches from the entry point, the initialisers, every function the symbol tables name and every code address a
// relocation stores in data (a vtable, a callback table). Functions keep the names the symbols give; each import's GOT slot is `got_<name>` (`got_<NID>` where the
// list of well-known functions does not name the NID), and a PLT stub that jumps through a slot is named for the function it reaches, so a call reads `call puts`.
// The ELF header, the program headers and the dynamic section are `db` rows under a comment that says what they are.
#pragma once

#include <filesystem>
#include <string_view>

#include "formats/orbis.hpp"
#include "source/image_writer.hpp"

namespace cb::source::orbis_writer {

image_writer::Written write(const formats::orbis::Program& program, std::string_view file_name, const std::filesystem::path& folder);
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder);

}  // namespace cb::source::orbis_writer
