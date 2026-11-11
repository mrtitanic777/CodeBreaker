// DIVERGES from Pillow, the codec this replaces (the .rgba files are Pillow's Image.open(...).convert('RGBA') -- but for grey16,
// written from the samples' high bytes, and grey2_key_hand, written by hand; every other variant agrees pixel for pixel):
//   * a 16-bit grey keeps its high byte; Pillow clips the 16-bit value to 255. (RGB16, grey+alpha16 and RGBA16 agree: high byte.)
//   * a grey colour key (tRNS) is compared with the RAW sample, as the PNG specification says (grey2_key_hand); Pillow compares
//     it with the scaled sample for 1, 2 and 4 bits.
//   * a damaged picture is a FormatError (bad signature, chunk or zlib data cut short, CRC mismatch, palette index out of range)
//     where Pillow raised its own errors; a header naming an unknown compression, filter or interlace method is an
//     UnsupportedError.

#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/rgba_png.hpp"

namespace {

using namespace cb;
using namespace cb::java::rgba_png;

// testdata/png/<name>.png are written by hand (every colour type and depth, interlaced or not, tRNS keys and palette
// alpha, a different row filter on every row, the data split over two IDAT chunks); <name>.rgba holds what Pillow's
// Image.open(...).convert('RGBA') makes of each -- the oracle the Java lane used before this codec.
Bytes png(const std::string& name) { return fs::read_file(testing::data_path("java/testdata/png/" + name + ".png")); }
Bytes expected(const std::string& name) { return fs::read_file(testing::data_path("java/testdata/png/" + name + ".rgba")); }

CB_TEST("java.rgba_png.decode_matches_pillow", [] {
    struct Case {
        const char* name;
        std::uint32_t w, h;
    };
    const Case cases[] = {
        {"grey1", 13, 5},          {"grey2", 9, 6},           {"grey4", 11, 4},          {"grey8", 10, 7},
        {"grey16", 6, 5},          {"grey8_key", 8, 6},       {"grey2_key_hand", 8, 1},      {"rgb8", 7, 6},
        {"rgb16", 5, 4},           {"rgb8_key", 6, 6},        {"palette1", 17, 3},       {"palette2", 10, 5},
        {"palette4", 9, 9},        {"palette8", 16, 16},      {"greyalpha8", 8, 5},      {"greyalpha16", 4, 4},
        {"rgba8", 9, 8},           {"rgba16", 5, 5},          {"rgba8_interlaced", 11, 9}, {"rgb8_interlaced_tiny", 3, 2},
        {"palette4_interlaced", 13, 10}, {"grey1_interlaced", 19, 7}, {"rgba8_1x1", 1, 1}, {"rgba8_wide", 40, 1},
        {"rgba8_tall", 1, 33},
    };
    for (const Case& c : cases) {
        const Image img = decode(png(c.name));
        CB_CHECK_EQ(img.width, c.w);
        CB_CHECK_EQ(img.height, c.h);
        const Bytes want = expected(c.name);
        CB_CHECK_EQ(want.size(), std::size_t{c.w} * c.h * 4);
        // compare as a whole, then name the first pixel that differs
        if (img.rgba != want) {
            for (std::size_t i = 0; i < want.size(); ++i) {
                if (img.rgba[i] != want[i]) {
                    CB_CHECK_EQ(std::string(c.name) + " byte " + std::to_string(i) + " = " + std::to_string(img.rgba[i]),
                                std::string(c.name) + " byte " + std::to_string(i) + " = " + std::to_string(want[i]));
                    break;
                }
            }
        }
    }
});

CB_TEST("java.rgba_png.encode_roundtrip", [] {
    const Image src = decode(png("rgba8_interlaced"));
    const Bytes file = encode(src);
    // a PNG: signature, IHDR (8-bit RGBA, no interlace), one IDAT, IEND
    CB_CHECK_EQ(file[1], std::uint8_t{'P'});
    CB_CHECK_EQ(file[12], std::uint8_t{'I'});
    CB_CHECK_EQ(file[13], std::uint8_t{'H'});
    CB_CHECK_EQ(file[24], std::uint8_t{8});    // bit depth
    CB_CHECK_EQ(file[25], std::uint8_t{6});    // colour type RGBA
    CB_CHECK_EQ(file[28], std::uint8_t{0});    // not interlaced
    const Image back = decode(file);
    CB_CHECK_EQ(back.width, src.width);
    CB_CHECK_EQ(back.height, src.height);
    CB_CHECK(back.rgba == src.rgba);
    CB_CHECK(encode(decode(png("palette8"))) == encode(decode(png("palette8"))));      // deterministic
    // sizes at the edge
    CB_CHECK(decode(encode(blank(1, 1))).rgba == Bytes(4, 0));
    const Image wide = decode(encode(decode(png("rgba8_wide"))));
    CB_CHECK(wide.width == 40 && wide.height == 1);
});

CB_TEST("java.rgba_png.damaged_input", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)decode({}));
    CB_CHECK_THROWS_AS(FormatError, (void)decode(to_bytes("GIF89a....")));
    const Bytes good = png("rgba8");
    // every prefix that stops before the IEND chunk is an error of the format: never a crash, never a wrong picture
    for (std::size_t n = 0; n + 12 < good.size(); ++n) {
        CB_CHECK_THROWS_AS(FormatError, (void)decode(ByteView(good).first(n)));
    }
    // (a file that lost only its IEND still has all its pixels)
    CB_CHECK(decode(ByteView(good).first(good.size() - 12)).rgba == expected("rgba8"));

    Bytes flipped = good;
    flipped[flipped.size() - 20] ^= 0x40;            // a byte of the IDAT data: its CRC no longer matches
    CB_CHECK_THROWS_AS(FormatError, (void)decode(flipped));
    Bytes lying = good;
    lying[33] = 0xFF;                                // the first IDAT's length now runs past the end
    CB_CHECK_THROWS_AS(FormatError, (void)decode(lying));
});

CB_TEST("java.rgba_png.malformed_headers_and_data", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)decode(png("bad_filter")));              // row filter type 9
    CB_CHECK_THROWS_AS(FormatError, (void)decode(png("bad_palette_index")));       // an index past the palette
    CB_CHECK_THROWS_AS(FormatError, (void)decode(png("bad_combination")));         // RGB with 4-bit samples
    CB_CHECK_THROWS_AS(FormatError, (void)decode(png("zero_width")));
    CB_CHECK_THROWS_AS(FormatError, (void)decode(png("short_data")));              // the zlib data holds too few rows
    CB_CHECK_THROWS_AS(FormatError, (void)decode(png("no_data")));                 // no IDAT at all
    // 65536 x 65536 pixels is refused from the header, before anything is allocated
    CB_CHECK_THROWS_AS(FormatError, (void)decode(png("huge")));
    // methods this reader does not know are a valid file it cannot handle, not a damaged one
    CB_CHECK_THROWS_AS(UnsupportedError, (void)decode(png("bad_interlace_method")));
    CB_CHECK_THROWS_AS(UnsupportedError, (void)decode(png("bad_compression_method")));
    CB_CHECK_THROWS_AS(UnsupportedError, (void)decode(png("bad_filter_method")));
});

Image numbered(std::uint32_t w, std::uint32_t h) {
    Image img = blank(w, h);
    for (std::uint32_t y = 0; y < h; ++y) {
        for (std::uint32_t x = 0; x < w; ++x) {
            std::uint8_t* p = img.pixel(x, y);
            p[0] = static_cast<std::uint8_t>(x);
            p[1] = static_cast<std::uint8_t>(y);
            p[2] = 7;
            p[3] = 255;
        }
    }
    return img;
}

CB_TEST("java.rgba_png.crop", [] {
    const Image src = numbered(8, 6);
    const Image inner = crop(src, 2, 1, 5, 3);
    CB_CHECK(inner.width == 3 && inner.height == 2);
    CB_CHECK_EQ(inner.pixel(0, 0)[0], std::uint8_t{2});
    CB_CHECK_EQ(inner.pixel(0, 0)[1], std::uint8_t{1});
    CB_CHECK_EQ(inner.pixel(2, 1)[0], std::uint8_t{4});
    CB_CHECK_EQ(inner.pixel(2, 1)[1], std::uint8_t{2});
    // a box that overhangs the image: the missing pixels are transparent black
    const Image over = crop(src, 6, 4, 10, 8);
    CB_CHECK(over.width == 4 && over.height == 4);
    CB_CHECK_EQ(over.pixel(1, 1)[0], std::uint8_t{7});
    CB_CHECK_EQ(over.pixel(1, 1)[3], std::uint8_t{255});
    CB_CHECK_EQ(over.pixel(2, 0)[3], std::uint8_t{0});                 // x = 8 is outside
    CB_CHECK_EQ(over.pixel(0, 2)[3], std::uint8_t{0});                 // y = 6 is outside
    const Image before = crop(src, -2, -1, 2, 1);
    CB_CHECK_EQ(before.pixel(0, 0)[3], std::uint8_t{0});
    CB_CHECK_EQ(before.pixel(2, 1)[0], std::uint8_t{0});               // source (0, 0)
    CB_CHECK_EQ(before.pixel(2, 1)[3], std::uint8_t{255});
    // an empty or inverted box
    CB_CHECK(crop(src, 3, 3, 3, 5).width == 0);
    CB_CHECK(crop(src, 5, 1, 2, 4).width == 0);
});

CB_TEST("java.rgba_png.resize_nearest", [] {
    // 2x2 -> 4x4: every pixel becomes a 2x2 block
    const Image two = numbered(2, 2);
    const Image up = resize_nearest(two, 4);
    CB_CHECK(up.width == 4 && up.height == 4);
    for (std::uint32_t y = 0; y < 4; ++y) {
        for (std::uint32_t x = 0; x < 4; ++x) {
            CB_CHECK_EQ(up.pixel(x, y)[0], static_cast<std::uint8_t>(x / 2));
            CB_CHECK_EQ(up.pixel(x, y)[1], static_cast<std::uint8_t>(y / 2));
        }
    }
    // 16x16 sprite -> 64x64 tile: blocks of 4x4 (the achievement tile size)
    const Image sprite = numbered(16, 16);
    const Image tile = resize_nearest(sprite, 64);
    CB_CHECK(tile.width == 64 && tile.height == 64);
    CB_CHECK_EQ(tile.pixel(63, 63)[0], std::uint8_t{15});
    CB_CHECK_EQ(tile.pixel(3, 4)[0], std::uint8_t{0});
    CB_CHECK_EQ(tile.pixel(3, 4)[1], std::uint8_t{1});
    CB_CHECK_EQ(tile.pixel(4, 3)[0], std::uint8_t{1});
    // scaling down takes the pixel in the middle of each step: 4 -> 2 picks columns 1 and 3
    const Image down = resize_nearest(numbered(4, 4), 2);
    CB_CHECK_EQ(down.pixel(0, 0)[0], std::uint8_t{1});
    CB_CHECK_EQ(down.pixel(1, 1)[0], std::uint8_t{3});
    CB_CHECK_EQ(down.pixel(1, 1)[1], std::uint8_t{3});
    // the same size is a copy; an odd factor stays in range
    CB_CHECK(resize_nearest(sprite, 16).rgba == sprite.rgba);
    const Image odd = resize_nearest(numbered(5, 5), 7);
    CB_CHECK(odd.pixel(6, 6)[0] <= 4);
    CB_CHECK_THROWS_AS(UsageError, (void)resize_nearest(sprite, 0));
    CB_CHECK(resize_nearest(blank(0, 0), 3).rgba == Bytes(36, 0));
});

}  // namespace
