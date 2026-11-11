// The shim at the old header path of formats.xbox.texture: the old py:: surface over cb/formats/xbox/texture.
#include "codebreaker/formats/xbox/texture.hpp"

#include "cb/core/testing.hpp"

namespace tex = codebreaker::formats::xbox::texture;

CB_TEST("compat.texture.old_surface_over_the_new_module", [] {
    CB_CHECK_EQ(tex::FORMATS.items().size(), 3u);
    CB_CHECK_EQ(tex::tiled_offset_2d(0, 0, 32, 4), 0);
    CB_CHECK_EQ(tex::tiled_offset_2d(1, 0, 32, 4), 32);   // the same answers the old reader gave
    CB_CHECK_EQ(tex::tiled_offset_2d(31, 31, 32, 3), 8056);
    const auto [bw, bh] = tex::_blocks(9, 4);
    CB_CHECK_EQ(bw, 3);
    CB_CHECK_EQ(bh, 1);
    std::string linear(8 * 4, '\0');
    for (std::size_t i = 0; i < linear.size(); ++i) linear[i] = static_cast<char>(i * 5);
    const py::bytes tiled = tex::tile(py::bytes(linear), 8, 8, py::str("dxt1"));
    CB_CHECK(tex::untile(tiled, 8, 8, py::str("dxt1")) == py::bytes(linear));
    CB_CHECK_EQ(tex::dds_header(8, 8, py::str("dxt5")).size(), 128);
    CB_CHECK_EQ(tex::to_dds(py::bytes("abc"), 4, 4, py::str("dxt1")).size(), 131);
    CB_CHECK(tex::swap16(py::bytes("abcd")) == py::bytes("badc"));
    const tex::Image im = tex::decode_dxt(py::bytes(std::string(8, '\0')), 4, 4, py::str("dxt1"), false, false, false);
    CB_CHECK_EQ(im.width, 4);
    CB_CHECK_EQ(im.pixels.size(), 64);
});

CB_TEST("compat.texture.errors_are_the_old_exceptions", [] {
    CB_CHECK_THROWS_AS(py::KeyError, (void)tex::untile(py::bytes("x"), 4, 4, py::str("dxt9")));
    CB_CHECK_THROWS_AS(py::ValueError, (void)tex::swap16(py::bytes("abc")));
});
