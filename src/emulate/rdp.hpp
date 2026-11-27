// emulate/rdp.hpp -- the Nintendo 64's RDP as the judge sees it: a display list read as its commands. The RDP is the RCP's rasterizer; the RSP
// (or the CPU) hands it a list of 64-bit command words by writing DPC_START and DPC_END. Each command is named and kept with its words, so two
// runs that draw the same thing hand over the same list:
//
//     for (const rdp::Command& c : rdp::commands(memory, start, end)) ...   // c.name "set_color_image", c.words {0x3F18013F00100000}
//
// The commands (the top 6 bits of the first word): the eight triangles (0x08-0x0F: with shade, texture and depth coefficients after the
// edges, 4 to 22 words in all), texture_rectangle and its flip (2 words), the syncs, set_key, set_convert, set_scissor, set_prim_depth,
// set_other_modes, load_tlut, set_tile_size, load_block, load_tile, set_tile, fill_rectangle, the six colours, set_combine and the three
// images. A word the RDP has no command for is "unknown_XX" (the hardware takes it as a no-op).
//
// What is not done: the RDP is not run -- nothing is rasterized into the colour image, so a program is compared on what it asks the RDP to
// draw, not on the pixels.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"

namespace cb::emulate::rdp {

struct Command {
    std::string name;
    std::vector<std::uint64_t> words;
    std::string text() const;   // "fill_rectangle 3651E3BC00000000"
};

// The command a first word starts, and how many words it takes.
std::string_view name_of(std::uint64_t word);
unsigned length_of(std::uint64_t word);

// The commands from `start` to `end` (byte offsets into `memory`, big-endian words; a list that runs past the memory's end stops there).
std::vector<Command> commands(ByteView memory, std::uint32_t start, std::uint32_t end);

}  // namespace cb::emulate::rdp
