// source/segacd_writer.hpp -- a Sega CD / Mega-CD disc's boot sectors as assembly source that builds back into them.
//
//     image_writer::Written r = segacd_writer::reconstruct("game.iso", "out");
//     r.source                  game.asm (Motorola syntax: asm68k, vasm)
//     r.same                    the source, built, is the disc's 32 KB system area byte for byte
//     r.functions               {address: name}: the main CPU's at $FF0000 and up, the sub CPU's below $80000
//
// A Sega CD runs two 68000 programs from the boot sectors (formats/segacd): the main CPU's security code and IP, loaded at $FF0000, and
// the sub CPU's SP, loaded at $6000 of its PRG-RAM. One source holds both, each under its own `org` at the address it runs at, so a
// jump or an absolute operand names the right place in either; the disc header, the code and the padding between are the other `org`
// blocks (the header's fields named; padding addresses are only where the assembler puts bytes that are not code). Flow runs through
// each program from its entries: the security code and the IP's entry on the main CPU, the four entries of the SP's jump table on the
// sub CPU (`sp_init`, `sp_main`, `sp_int2`, `sp_user`). Every byte reached is an instruction, every other byte data. The Genesis's ports,
// the gate array, the CD BIOS's entry points (`CDBIOS`, `WAITVSYNC` ...) are written by name.
//
// The source is assembled and the blocks are put back at their disc offsets: the system area, which is what is compared. Files on the disc
// after the system area are not part of it.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/segacd.hpp"
#include "source/image_writer.hpp"

namespace cb::source::segacd_writer {

// The ports of both CPUs: the Genesis's, the gate array as each CPU sees it, the CD BIOS's calls.
const std::map<std::uint32_t, std::string>& hardware();

// The source of `disc` (whose file was named `file_name`) written to `folder` as <stem>.asm, and rebuilt (<stem>.built.iso is the system area).
image_writer::Written write(const formats::segacd::Disc& disc, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a Sega CD disc).
image_writer::Written reconstruct(const std::filesystem::path& image_file, const std::filesystem::path& folder);

}  // namespace cb::source::segacd_writer
