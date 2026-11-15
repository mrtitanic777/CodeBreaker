// cb/source/dreamcast_writer.hpp -- a Sega Dreamcast disc's IP.BIN and first program as assembly source that builds back into them.
//
//     image_writer::Written r = dreamcast_writer::reconstruct("game.cdi", "out");
//     r.source                  game.s (GNU as syntax, as the Dreamcast's sh-elf-gcc produces it)
//     r.same                    the source, built, is IP.BIN and the (descrambled) first program byte for byte
//     r.functions               {address: name}: physical addresses, IP.BIN's from $0C008300, the program's from $0C010000
//
// A Dreamcast disc runs two programs of the SH-4 that matter here (formats/dreamcast): IP.BIN, the 32 KB of boot sectors the BIOS loads to
// $8C008000, and 1ST_READ.BIN, which IP.BIN's bootstrap loads to $8C010000. One source holds both, each under its own `.org` at the physical
// address it is loaded to ($0C008000 and $0C010000; the program names them with the cache bits of its choice: a longword that holds $8C010100 is
// written `sub_0C010100 + 0x80000000`). IP.BIN's header is written field by field (the hardware ID, the maker, the device information, the areas,
// the peripherals, the product number, the version, the release date, the boot file, the software maker, the title); the rest of IP.BIN and the
// program are walked from their entries (the bootstrap at IP.BIN + $300, and the program's first instruction), every byte reached is an instruction
// and every other byte data. The SH-4 is little-endian here: words and longwords are written low byte first, the FPU instructions are decoded, and
// the machine's registers (the Holly's system and PowerVR registers, the G1 and G2 buses, the Maple, the GD-ROM, the AICA and the SH-4's own) are
// written by name.
//
// A scrambled 1ST_READ.BIN (homebrew on a CD-R) is written descrambled, which is the program the machine runs; the disc the lane runs beside the
// original (`<stem>.built.iso`) holds it scrambled again, so a rebuilt disc is the same disc. `<stem>.built.bin` is the system area followed by the
// descrambled program: the bytes the source is compared with.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>

#include "cb/formats/dreamcast.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::dreamcast_writer {

// The machine's registers by physical address (the SH-4's on-chip registers by their P4 addresses, $FF000000 and up).
const std::map<std::uint32_t, std::string>& hardware();

// The source of `disc` (whose file was named `file_name`) written to `folder` as <stem>.s, and rebuilt.
image_writer::Written write(const formats::dreamcast::Disc& disc, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not a Dreamcast disc).
image_writer::Written reconstruct(const std::filesystem::path& image_file, const std::filesystem::path& folder);

// A disc of 2048-byte sectors that holds `image` (IP.BIN, then the program): the system area, an ISO 9660 volume whose root directory has the one
// file `name`, and the program in the sectors after (scrambled when `scrambled`). The reader (formats/dreamcast) finds the same two parts in it.
// A `volume_base` numbers the volume's sectors from there, as a GD-ROM's high-density track or a second session does.
Bytes minimal_disc(ByteView image, const std::string& name, bool scrambled, std::uint32_t volume_base = 0);

}  // namespace cb::source::dreamcast_writer
