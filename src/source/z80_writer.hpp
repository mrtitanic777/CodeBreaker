// source/z80_writer.hpp -- a flat Z80 program (one block of memory at one address) as assembly source that builds back into it.
// The console writer's z80-block shape (source/console_writer: the Spectrum, the MSX, the ColecoVision, the CPC) and the Neo Geo's sound CPU write through it: they give the block, its entry points and the machine's names.
//
//     z80_writer::Block block{.image = bytes, .origin = 0x4000, .entries = {{0x4010, "init"}}};
//     image_writer::Written r = z80_writer::write(block, machine, "out", "game");
//     r.source                  <stem>.asm (in the machine's dialect, sjasmplus unless it says another: source/z80_source)
//     r.same                    the source, built, is the block byte for byte
//     r.functions               {address: name}
//     r.built_image             the block as the source builds it
//
// Code is what the program's own flow reaches from its entry points; every byte it reaches is written as an instruction, every other
// byte as data. Entry points keep the names given, called functions are `sub_<address>`, jump targets `L_<address>`, and pointers
// loaded into HL DE IX IY (or read and written through) that name the start of a line get that line's label, so the source can be
// moved and edited, not only rebuilt. Ports and fixed addresses (system variables, ROM routines) are written by the names the
// machine's documentation gives them.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "decompile/z80.hpp"
#include "source/image_writer.hpp"

namespace cb::source::z80_writer {

// The block of memory a program is: its bytes, the address of the first, the entry points and a note for the head of the source.
struct Block {
    ByteView image;
    std::uint32_t origin = 0;
    image_writer::Names entries;            // address -> name
    std::vector<std::string> header;        // lines of the head comment (each becomes `; line`)
};

// What a machine calls things: its I/O ports, and the fixed addresses outside the block (system variables, ROM routines).
struct Machine {
    image_writer::Names ports;
    image_writer::Names names;
    std::string dialect = "sjasmplus";      // the dialect the source is written in (config/dialects/<dialect>.json)
};

// What flow reaches from the entries inside the block: the code (by address), the jump targets, the function starts. A called
// address is a function start, except a RST's.
image_writer::Walked<decompile::z80::Insn> walk(const Block& block);

// The source of the block written to `folder` as <stem>.asm, and rebuilt.
image_writer::Written write(const Block& block, const Machine& machine, const std::filesystem::path& folder, std::string_view stem);

}  // namespace cb::source::z80_writer
