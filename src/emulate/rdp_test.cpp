// Tests of emulate/rdp: a display list read as its commands, the triangles' lengths by their coefficients, and an unknown or cut-short word.
#include "emulate/rdp.hpp"

#include "core/testing.hpp"

namespace cb::emulate::rdp {
namespace {

void put(Bytes& b, std::uint64_t w) {
    for (int s = 56; s >= 0; s -= 8) b.push_back(static_cast<std::uint8_t>(w >> s));
}

}  // namespace

CB_TEST("emulate.rdp.a_display_list_is_read_as_its_commands", [] {
    Bytes list;
    put(list, 0x3F18013F00100000ull);   // set_color_image: 16-bit, 320 wide, at $100000
    put(list, 0x2D00000000500398ull);   // set_scissor
    put(list, 0x3700000000010001ull);   // set_fill_color
    put(list, 0x3651E3BC00000000ull);   // fill_rectangle
    put(list, 0x2400000000000000ull);   // texture_rectangle: two words
    put(list, 0x0000000000000000ull);
    put(list, 0x2900000000000000ull);   // sync_full
    const std::vector<Command> c = commands(list, 0, static_cast<std::uint32_t>(list.size()));
    CB_CHECK_EQ(c.size(), 6u);
    CB_CHECK_EQ(c[0].name, "set_color_image");
    CB_CHECK_EQ(c[3].text(), "fill_rectangle 3651E3BC00000000");
    CB_CHECK_EQ(c[4].name, "texture_rectangle");
    CB_CHECK_EQ(c[4].words.size(), 2u);
    CB_CHECK_EQ(c[5].name, "sync_full");
    // a list from the middle, and one that ends where it is cut
    CB_CHECK_EQ(commands(list, 16, 24).size(), 1u);
    CB_CHECK_EQ(commands(list, 0, 4).size(), 0u);
});

CB_TEST("emulate.rdp.triangles_take_their_coefficients_and_unknown_words_are_named", [] {
    CB_CHECK_EQ(length_of(0x0800000000000000ull), 4u);    // fill: the edges
    CB_CHECK_EQ(length_of(0x0C00000000000000ull), 12u);   // shade
    CB_CHECK_EQ(length_of(0x0A00000000000000ull), 12u);   // texture
    CB_CHECK_EQ(length_of(0x0F00000000000000ull), 22u);   // shade, texture and depth
    CB_CHECK_EQ(name_of(0x0F00000000000000ull), "shade_texture_zbuffer_triangle");
    Bytes list;
    put(list, 0x0100000000000000ull);
    CB_CHECK_EQ(commands(list, 0, 8).at(0).name, "unknown_01");
});

}  // namespace cb::emulate::rdp
