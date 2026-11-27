// source/lynx_writer.hpp -- an Atari Lynx program as assembly source that builds back into it.
//
//     image_writer::Written r = lynx_writer::reconstruct("game.lnx", "out", booter);
//     r.source, r.config        game.s (ca65 syntax, the 65SC02) and game.cfg (ld65)
//     r.same                    the source, built, is the file byte for byte
//     r.functions               {address: name}
//
// A Lynx program is its loaded image (formats/lynx.hpp): what the boot ROM and the cartridge's own loaders copy from the file to RAM.
// Each stretch the loaders copy is one ca65 segment, linked at the RAM address it loads to; its bytes are instructions where flow from
// the loaders' entries reaches them, data elsewhere. Everything else in the file -- the 64-byte `.lnx` header, the encrypted first stage
// (the boot ROM decrypts it; it is written as the bytes it is), the directory, padding, unloaded blocks -- is bytes of its own segments
// in file order, so the source is the whole file. Function starts are `start` (the first entry the loaders jump to) or `sub_<address>`,
// jump targets `L_<address>`, and Suzy's and Mikey's registers carry their documented names (`SPRGO`, `DISPCTL`, `GREEN0` ...).
//
// A BS93 object (the homebrew loader format) is its ten header bytes, one segment at the load address and whatever follows the block.
//
// Which stretches a `.lnx` loads is found by playing the boot ROM, which is the emulator's business (engine/lynx_boot.hpp, not linked here):
// the caller gives the `Booter`, which calls `Program::set_loaded`. Without one a cartridge has no code: it is written as data.
//
// It builds with CodeBreaker's own assembler (build/m6502_asm, with the 65SC02 encoder) or with the cc65 suite (`ca65 game.s` and
// `ld65 -C game.cfg -o game.lnx game.o`; the memory areas are in file order, each filled to its size).
#pragma once

#include <filesystem>
#include <functional>
#include <string_view>

#include "formats/lynx.hpp"
#include "source/image_writer.hpp"

namespace cb::source::lynx_writer {

// Finds what a cartridge loads and calls `program.set_loaded(segments, entries)`.
using Booter = std::function<void(formats::lynx::Program&)>;

image_writer::Written write(const formats::lynx::Program& program, std::string_view file_name, const std::filesystem::path& folder);

// The same for a file (IoError when it cannot be read, FormatError when it is neither a `.lnx` nor a BS93 object); `booter`, if given, is
// asked about a cartridge first.
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder, const Booter& booter = {});

}  // namespace cb::source::lynx_writer
