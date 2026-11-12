// cb/source/nes_writer.hpp -- a NES program as assembly source that builds back into it.
//
//     image_writer::Written r = nes_writer::reconstruct("game.nes", "out");
//     r.source, r.config        game.s (ca65 syntax) and game.cfg (ld65), beside the CHR as game.chr
//     r.same                    the source, built, is the cartridge image byte for byte
//     r.functions               {address: name}
//
// NES programs were written in assembly almost without exception, so assembly is what comes back (docs/PLATFORM_DESCRIPTOR.md).
// Code is what the program's own flow reaches from its three vectors (formats/nes); every byte it reaches is written as an
// instruction, every other byte as data. Function starts are named for their vector (`reset`, `nmi`, `irq`) or `sub_<address>`,
// branch and jump targets `L_<address>`, and an absolute operand naming the start of a line gets that line's label -- so the
// source can be moved and edited, not only rebuilt. The last six bytes of the bank that holds them are the vectors, written as
// `.word nmi, reset, irq`.
//
// It builds with CodeBreaker's own assembler (build/m6502_asm) or with the cc65 suite (`ca65 game.s && ld65 -C game.cfg -o game.nes
// game.o`).
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "cb/formats/nes.hpp"
#include "cb/source/image_writer.hpp"

namespace cb::source::nes_writer {

// The console's registers, by the names the hardware documentation gives them.
const std::map<std::uint32_t, std::string>& hardware();

// One stretch of the program ROM as the CPU sees it.
struct Bank {
    std::string segment;       // "BANK0": the ca65 segment and the ld65 area it loads into
    std::uint32_t base = 0;    // the CPU address of its first byte
    std::uint32_t offset = 0;  // where it begins in the PRG
    std::uint32_t size = 0;
};

// A 16 KB PRG sits at $C000; one that fits the two fixed slots at $8000; with a mapper, each 16 KB bank on its own, the last one
// fixed at $C000 (the others as they are at power-on).
std::vector<Bank> banks(const formats::nes::Rom& rom);

// The source of `rom` (whose file was named `file_name`) written to `folder` as <stem>.s, <stem>.cfg and <stem>.chr, and rebuilt.
image_writer::Written write(const formats::nes::Rom& rom, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is not an iNES image).
image_writer::Written reconstruct(const std::filesystem::path& rom_file, const std::filesystem::path& folder);

}  // namespace cb::source::nes_writer
