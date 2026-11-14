// Tests of cb/formats/gfx.hpp on a synthetic Scaleform file (testdata/gfx/menu.bin: a GFx movie with three external images, two fonts and
// four placements, junk with a stray "GFX" in it, a zlib-compressed CFX movie, and a movie of one 117-glyph font made to exercise the
// TrueType writer). There is no real GFx file on this machine; the expected output is what `codebreaker gfx ...` printed for this one, and
// the expected TrueType files are the ones fontTools wrote through `font_to_ttf`: every table of ours is the same bytes
// (head's two dates and checksum adjustment, which depend on when the font was made, aside).
//
// Behaviour pinned here: a placement's scale is read as the signed fixed-point number it is (an unsigned read would print a
// flipped placement as a huge scale).
#include "cb/formats/gfx.hpp"

#include <format>
#include <map>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/command_test_kit.hpp"

using namespace cb;
namespace gfx = cb::formats::gfx;

namespace {

std::filesystem::path gfx_data(const std::string& name) { return testing::data_path("formats/testdata/gfx/" + name); }
std::string gfx_golden(const char* name) { return fs::read_text(gfx_data(name)); }
Bytes gfx_menu() { return fs::read_file(gfx_data("menu.bin")); }

// The tables of a TrueType file by tag, with head's dates, checksum adjustment and checksum zeroed.
std::map<std::string, Bytes> gfx_tables(ByteView file) {
    std::map<std::string, Bytes> out;
    io::ByteReader r(file, "ttf");
    r.seek(4);
    const unsigned count = r.u16_be();
    for (unsigned i = 0; i < count; ++i) {
        r.seek(12 + 16 * std::size_t{i});
        const std::string tag(as_text(r.bytes(4)));
        r.skip(4);   // the checksum (it follows the data)
        const std::uint32_t offset = r.u32_be(), length = r.u32_be();
        Bytes data(file.begin() + offset, file.begin() + offset + length);
        if (tag == "head") {
            std::fill(data.begin() + 8, data.begin() + 12, std::uint8_t{0});
            std::fill(data.begin() + 20, data.begin() + 36, std::uint8_t{0});
        }
        out[tag] = std::move(data);
    }
    return out;
}

// A minimal movie around a tag stream: the frame size, rate and count, the tags, ShowFrame and End.
Bytes gfx_movie(const std::vector<Bytes>& tags) {
    Bytes body = {0x08, 0x00, 0x00, 0x1E, 0x01, 0x00};   // a frame rectangle of 1 bit a field, the frame rate (30) and a count of 1
    for (const Bytes& t : tags) append(body, t);
    append(body, Bytes{0x40, 0x00, 0x00, 0x00});   // ShowFrame (tag 1, length 0), End (tag 0)
    io::ByteWriter w;
    w.bytes(as_bytes("GFX"));
    w.u8(8);
    w.u32(static_cast<std::uint32_t>(8 + body.size()));
    w.bytes(body);
    return w.take();
}

Bytes gfx_tag(unsigned code, ByteView body) {
    io::ByteWriter w;
    w.u16(static_cast<std::uint16_t>((code << 6) | body.size()));
    w.bytes(body);
    return w.take();
}

}  // namespace

CB_TEST("formats.gfx.movies_are_found_by_their_magic_and_junk_is_not_one", [] {
    const Bytes data = gfx_menu();
    const auto movies = gfx::find_movies(data);
    CB_CHECK_EQ(movies.size(), std::size_t{3});
    CB_CHECK_EQ(movies[0].magic, "GFX");
    CB_CHECK_EQ(movies[0].offset, std::size_t{0x21});
    CB_CHECK_EQ(movies[1].magic, "CFX");      // zlib: the body is inflated
    CB_CHECK_EQ(movies[1].offset, std::size_t{0x239});
    CB_CHECK_EQ(gfx::wrapper_name(data, movies[0].offset), "menu_main");
    CB_CHECK_EQ(gfx::wrapper_name(data, movies[1].offset), "engine_id");
    CB_CHECK(gfx::find_movies(Bytes(200, 0x41)).empty());
    CB_CHECK(gfx::find_movies(as_bytes("GFX\x08\x10\x00\x00\x00 short")).empty());
});

CB_TEST("formats.gfx.images_fonts_and_placements_are_read_from_the_tags", [] {
    const auto movies = gfx::find_movies(gfx_menu());
    const auto images = gfx::external_images(movies[0].body);
    CB_CHECK_EQ(images.size(), std::size_t{3});
    CB_CHECK_EQ(images[0].name, "menu_bg.dds");
    CB_CHECK_EQ(images[0].width, 256u);
    CB_CHECK_EQ(images[1].height, 64u);
    const auto fonts = gfx::fonts(movies[0].body);
    CB_CHECK_EQ(fonts.size(), std::size_t{2});
    CB_CHECK_EQ(fonts[0].name, "Segoe-Bold");
    CB_CHECK(fonts[0].bold && fonts[0].layout);
    CB_CHECK_EQ(fonts[0].version, 3);
    CB_CHECK_EQ(fonts[0].glyphs, 4u);
    const auto placed = gfx::placements(movies[0].body);
    CB_CHECK_EQ(placed.size(), std::size_t{4});
    CB_CHECK(placed[1].matrix && placed[1].matrix->scale_x == 0.5 && placed[1].matrix->scale_y == 1.5 && placed[1].matrix->translate_x == -75.0);
    CB_CHECK(!placed[3].matrix.has_value() || placed[3].character_id == 9u);   // (a placement of a character that has no image)
});

CB_TEST("formats.gfx.glyph_outlines_are_moves_lines_and_curves", [] {
    // the font 5's glyph 'A' and 'B' (the second has two contours and curves)
    const auto movies = gfx::find_movies(gfx_menu());
    for (const gfx::Tag& tag : gfx::parse_tags(movies[0].body)) {
        if (tag.code != 75 || tag.body[0] != 5) continue;
        io::ByteReader r(tag.body, "font");
        r.seek(4);
        const unsigned name_length = r.u8();
        r.skip(name_length + 2);
        const std::size_t offsets_at = r.pos();
        r.seek(offsets_at);
        const std::uint32_t first = r.u32(), second = r.u32();
        const auto a = gfx::glyph_contours(tag.body, offsets_at + first);
        CB_CHECK_EQ(a.size(), std::size_t{1});
        CB_CHECK_EQ(a[0].size(), std::size_t{8});
        CB_CHECK((a[0][0] == gfx::PathOp{gfx::PathOp::Kind::Move, 1000, -14000, 0, 0}));
        CB_CHECK((a[0][1] == gfx::PathOp{gfx::PathOp::Kind::Line, 7000, 0, 0, 0}));
        const auto b = gfx::glyph_contours(tag.body, offsets_at + second);
        CB_CHECK_EQ(b.size(), std::size_t{2});
        CB_CHECK((b[0][2] == gfx::PathOp{gfx::PathOp::Kind::Curve, 6500, -10500, 6500, -14000}));
        return;
    }
    CB_CHECK(false);
});

CB_TEST("formats.gfx.a_flipped_placement_has_a_negative_scale", [] {
    // PlaceObject2 with a matrix: scale (-1.0, 1.0) in 20-bit fixed point, translation (4, 8) twips
    Bytes matrix = {0};
    std::uint64_t acc = 0;
    unsigned used = 0;
    const auto put = [&](std::uint64_t v, unsigned n) {
        for (int k = static_cast<int>(n) - 1; k >= 0; --k) {
            acc = (acc << 1) | ((v >> k) & 1u);
            if (++used == 8) {
                matrix.push_back(static_cast<std::uint8_t>(acc));
                acc = 0;
                used = 0;
            }
        }
    };
    matrix.clear();
    put(1, 1);
    put(20, 5);
    put((std::uint64_t{1} << 20) - 65536, 20);   // -1.0
    put(65536, 20);                                // 1.0
    put(0, 1);
    put(5, 5);
    put(4, 5);
    put(8, 5);
    while (used != 0) put(0, 1);
    Bytes place = {0x06, 1, 0, 7, 0};   // flags (matrix, character), depth 1, character 7
    append(place, matrix);
    const Bytes image = [] {
        io::ByteWriter w;
        w.u16(7);
        w.u16(0);
        w.u16(2);
        w.u16(2);
        w.u8(1);
        w.bytes(as_bytes("x"));
        return w.take();
    }();
    const Bytes movie = gfx_movie({gfx_tag(1001, image), gfx_tag(26, place)});
    const auto found = gfx::find_movies(movie);
    CB_CHECK_EQ(found.size(), std::size_t{1});
    const auto placed = gfx::placements(found[0].body);
    CB_CHECK_EQ(placed.size(), std::size_t{1});
    CB_CHECK(placed[0].matrix.has_value());
    if (!placed[0].matrix) return;
    CB_CHECK_EQ(placed[0].matrix->scale_x, -1.0);
    CB_CHECK_EQ(placed[0].matrix->scale_y, 1.0);
    CB_CHECK_EQ(placed[0].matrix->translate_x, 0.2);   // 4 twips
    // a tag too short to read is left out, not an error
    const auto cut = gfx::find_movies(gfx_movie({gfx_tag(26, Bytes{0x06, 1, 0})}));
    CB_CHECK(gfx::placements(cut[0].body).empty());
});

CB_TEST("formats.gfx.the_commands_print_the_recorded_output", [] {
    const std::string menu = fs::to_text(gfx_data("menu.bin"));
    for (const char* name : {"scan", "images", "fonts", "placements"})
        CB_CHECK_EQ(testing::run_command(gfx::command(), {name, menu}), gfx_golden((std::string(name) + ".txt").c_str()));
    const fs::ScopedTempDir dir{"cb-gfx"};
    const std::string empty = fs::to_text(dir.path() / "nothing.bin");
    fs::write_file(dir.path() / "nothing.bin", Bytes(100, 0));
    CB_CHECK_EQ(testing::run_command(gfx::command(), {"scan", empty}), "no GFx movies in " + empty + "\nexit=0\n");
});

CB_TEST("formats.gfx.truetype_fonts_have_the_tables_fonttools_wrote", [] {
    const Bytes data = gfx_menu();
    const std::pair<unsigned, const char*> fonts[] = {{5, "font5.ttf"}, {6, "font6.ttf"}, {9, "font9.ttf"}};
    for (const auto& [id, golden] : fonts) {
        const gfx::TrueType ttf = gfx::font_to_ttf(data, id);
        const auto ours = gfx_tables(ttf.file);
        const auto theirs = gfx_tables(fs::read_file(gfx_data(golden)));
        CB_CHECK_EQ(ours.size(), theirs.size());
        for (const auto& [tag, bytes] : theirs) {
            CB_CHECK(ours.contains(tag));
            if (ours.contains(tag)) CB_CHECK_EQ(ours.at(tag) == bytes, true);
            if (ours.contains(tag) && ours.at(tag) != bytes) CB_CHECK_EQ(tag, "(a table that differs)");
        }
        CB_CHECK_EQ(ttf.file.size(), fs::read_file(gfx_data(golden)).size());
    }
    // a family given replaces the font's name in the name table
    const gfx::TrueType named = gfx::font_to_ttf(data, 6, std::string("My Family"));
    CB_CHECK_EQ(named.name, "Plain");
    CB_CHECK(gfx_tables(named.file) == gfx_tables(fs::read_file(gfx_data("font6_family.ttf"))));
    // the whole file adds up as TrueType asks: the words sum to 0xB1B0AFBA
    std::uint32_t sum = 0;
    for (std::size_t i = 0; i < named.file.size(); i += 4)
        sum += (std::uint32_t{named.file[i]} << 24) | (std::uint32_t{named.file[i + 1]} << 16) | (std::uint32_t{named.file[i + 2]} << 8) | named.file[i + 3];
    CB_CHECK_EQ(sum, 0xB1B0AFBAu);
});

CB_TEST("formats.gfx.font_command_writes_the_file_and_a_missing_font_is_an_error", [] {
    const fs::ScopedTempDir dir{"cb-gfx"};
    const std::string menu = fs::to_text(gfx_data("menu.bin")), out = fs::to_text(dir.path() / "f.ttf");
    CB_CHECK_EQ(testing::run_command(gfx::command(), {"font-ttf", menu, "5", out}), std::format("'Segoe-Bold': 4 glyphs -> {}\nexit=0\n", out));
    CB_CHECK(gfx_tables(fs::read_file(dir.path() / "f.ttf")) == gfx_tables(fs::read_file(gfx_data("font5.ttf"))));
    CB_CHECK_THROWS_AS(FormatError, gfx::font_to_ttf(gfx_menu(), 77));
});
