// source/saturn_writer.hpp -- a Sega Saturn disc's boot sectors and first program as assembly source that builds back into them.
//
//     image_writer::Written r = saturn_writer::reconstruct("game.iso", "out");
//     r.source                  game.s (GNU as syntax, as the Saturn SDK's sh-elf-gcc produces it)
//     r.same                    the source, built, is the disc's 32 KB system area and its first program byte for byte
//     r.functions               {address: name}: IP.BIN's at $06002000 and up, the program's at its load address (usually $06004000)
//
// A Saturn disc runs two programs of the SH-2 that matter here (formats/saturn): IP.BIN, the boot sectors the BIOS loads to $06002000, and
// the first program the disc names (0.BIN), which IP.BIN loads to the address the header gives. One source holds both, each under its own
// `.org` at the address it is loaded to, so a jump, a PC-relative load or an absolute operand names the right place in either: the
// header at $000 written field by field (the hardware ID, the maker, the product number, the areas, the title, the stack pointers, the first
// program's address), the security code and the four area-code entries, the initial program, the padding to 32 KB, and the program.
// Flow runs through each from its entries: the four area-code entries (`ip_area_jp`, `ip_area_tw`, `ip_area_us`, `ip_area_eu`; each a
// `bra` that runs into `ip_init`), and the first program's start (`first_read`). Every byte reached is an instruction, every other byte data;
// the constants PC-relative loads read are named, and the Saturn's registers (the SMPC, the SCU, the VDP1 and VDP2, the CD block, the SCSP
// and the SH7604's own) are written by name.
//
// The source is assembled and compared with the disc's system area followed by the first program: the files after it are not part of it.
// `<stem>.built.bin` is that; `<stem>.built.iso` is a disc of its own that holds them, which the lane runs beside the original.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "formats/saturn.hpp"
#include "source/image_writer.hpp"

namespace cb::source::saturn_writer {

// The machine's registers by physical address: the SMPC, SCU, VDP1, VDP2, CD block and SCSP, and the SH7604's.
const std::map<std::uint32_t, std::string>& hardware();

// The source of `disc` (whose file was named `file_name`) written to `folder` as <stem>.s, and rebuilt.
image_writer::Written write(const formats::saturn::Disc& disc, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a Saturn disc).
image_writer::Written reconstruct(const std::filesystem::path& image_file, const std::filesystem::path& folder);

}  // namespace cb::source::saturn_writer
