#include "cb/formats/xbox/texture.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::xbox::texture {
namespace {

using J = nlohmann::json;

std::filesystem::path fixture(std::string_view name) {
    return testing::data_path(std::string("formats/testdata/texture/") + std::string(name));
}

// "b<length>:<sha1>": a byte string as the old harness recorded it
std::string tag(ByteView b) { return std::format("b{}:{}", b.size(), crypto::to_hex(crypto::sha1(b))); }
// ... or "h:<the bytes in hex>" for the tiny ones
std::string hex_tag(ByteView b) { return "h:" + crypto::to_hex(b); }
bool same(ByteView got, const std::string& want) { return want.starts_with("h:") ? hex_tag(got) == want : tag(got) == want; }

// ---- the old reader's answers, on real surfaces -------------------------------------------------------------

CB_TEST("formats.xbox.texture.tiled_offsets_as_the_old_reader_computed_them", [] {
    // 552 (x, y, width in blocks, log2 block bytes) -> byte offset, from the Gildor/UModel formula as the Python ran it
    const J golden = J::parse(fs::read_text(fixture("golden.json")));
    const J& rows = golden["tiled_offset"];
    CB_CHECK(rows.size() > 500);
    for (const J& r : rows) {
        const std::uint64_t got = tiled_offset_2d(r[0], r[1], r[2], r[3]);
        if (got != r[4].get<std::uint64_t>()) {
            testing::record_failure(__FILE__, __LINE__, std::format("tiled_offset_2d({}, {}, {}, {}) = {}, the old reader said {}",
                                                                    r[0].get<int>(), r[1].get<int>(), r[2].get<int>(), r[3].get<int>(), got, r[4].get<std::uint64_t>()));
        }
    }
});

std::string from_hex_text(const std::string& tag_value) {   // "S:64787431" -> "dxt1"
    std::string out;
    for (std::size_t i = 2; i + 1 < tag_value.size(); i += 2) out.push_back(static_cast<char>(std::stoi(tag_value.substr(i, 2), nullptr, 16)));
    return out;
}
// -1 for what does not fit a surface's dimension (the old reader took any Python int; a size is 0..2^32-1 here)
long long int_of(const std::string& t) {
    const auto v = text::parse_int<long long>(std::string_view(t).substr(2));
    return v && *v >= 0 && *v <= 0xFFFFFFFFll ? *v : -1;
}

CB_TEST("formats.xbox.texture.real_surfaces_as_the_old_reader_processed_them", [] {
    const J golden = J::parse(fs::read_text(fixture("golden.json")));
    int checked = 0;
    for (const J& entry : golden["cases"]) {
        const std::vector<std::string> c = entry["case"].get<std::vector<std::string>>();
        const std::string result = entry["result"].get<std::string>();
        const bool error_expected = result.starts_with("[\"ERR\"");   // the old reader raised
        const J want = error_expected ? J() : J::parse(result);
        bool threw = false;
        auto expect_same = [&](ByteView got, const std::string& want_tag = std::string()) {
            if (error_expected) {
                testing::record_failure(__FILE__, __LINE__, c[0] + " of " + c[1] + ": the old reader raised, this one answers");
            } else if (!same(got, want_tag.empty() ? want.get<std::string>() : want_tag)) {
                testing::record_failure(__FILE__, __LINE__, c[0] + " of " + c[1] + " differs from the old answer");
            }
        };
        const std::string& op = c[0];
        auto load = [&](const std::string& f) { return fs::read_file(fixture(f.substr(f.rfind('/') + 1))); };
        auto fmt_of = [&](const std::string& t) { return parse_format(from_hex_text(t)); };
        try {
        if (op == "blocks") {
            const long long w = int_of(c[1]), h = int_of(c[2]);
            if (w < 0 || h < 0) continue;   // (not a size)
            const BlockSize b = blocks(static_cast<std::uint32_t>(w), static_cast<std::uint32_t>(h));
            CB_CHECK_EQ(b.width, want[0].get<std::uint32_t>());
            CB_CHECK_EQ(b.height, want[1].get<std::uint32_t>());
        } else if (op == "dds_header") {
            const auto f = fmt_of(c[3]);
            if (!f) { CB_CHECK(error_expected); threw = true; }   // an unknown format name: no Format at all
            const long long w = int_of(c[1]), h = int_of(c[2]);
            if (!f || w < 0 || h < 0) continue;
            expect_same(dds_header(static_cast<std::uint32_t>(w), static_cast<std::uint32_t>(h), *f));
        } else if (op == "swap16") {
            expect_same(swap16(load(c[1])));
        } else {
            const auto f = fmt_of(c[4]);
            if (!f) { CB_CHECK(error_expected); threw = true; }
            if (!f) continue;
            const Bytes data = load(c[1]);
            if (int_of(c[2]) < 0 || int_of(c[3]) < 0) continue;
            const std::uint32_t w = static_cast<std::uint32_t>(int_of(c[2])), h = static_cast<std::uint32_t>(int_of(c[3]));
            if (op == "tile") {
                expect_same(tile(data, w, h, *f));
            } else if (op == "untile") {
                // DIVERGES: surface data shorter than the tiled layout reaches (a linear file read as tiled). The old reader
                // assigned each short slice into a bytearray, which shortened the result by what was missing; this one keeps
                // the size of the surface and leaves the missing blocks zero. The blocks the data does reach are compared.
                const BlockSize bs = blocks(w, h);
                std::uint64_t reach = 0;
                for (std::uint32_t y = 0; y < bs.height; ++y) {
                    for (std::uint32_t x = 0; x < bs.width; ++x) reach = std::max(reach, tiled_offset_2d(x, y, bs.width, info(*f).log2_block_bytes) + info(*f).block_bytes);
                }
                if (reach > data.size()) {
                    const Bytes got = untile(data, w, h, *f);
                    CB_CHECK_EQ(got.size(), std::size_t{bs.width} * bs.height * info(*f).block_bytes);
                    for (std::uint32_t y = 0; y < bs.height; ++y) {
                        for (std::uint32_t x = 0; x < bs.width; ++x) {
                            const std::uint64_t at = tiled_offset_2d(x, y, bs.width, info(*f).log2_block_bytes);
                            const std::size_t block = info(*f).block_bytes;
                            const std::size_t dst = (std::size_t{y} * bs.width + x) * block;
                            if (at + block <= data.size()) CB_CHECK(std::equal(data.begin() + at, data.begin() + at + block, got.begin() + dst));
                            else if (at >= data.size()) CB_CHECK(std::all_of(got.begin() + dst, got.begin() + dst + block, [](std::uint8_t v) { return v == 0; }));
                        }
                    }
                } else {
                    expect_same(untile(data, w, h, *f));
                }
            } else if (op == "to_dds") {
                expect_same(to_dds(data, w, h, *f));
            } else {
                // decode / decode_flags -> ["IMG", w, h, "RGBA", tag]
                DecodeOptions o;
                if (op == "decode_flags") {
                    o.tiled = int_of(c[5]) != 0;
                    o.swapped = int_of(c[6]) != 0;
                    o.force_opaque = int_of(c[7]) != 0;
                }
                const Image im = decode_dxt(data, w, h, *f, o);
                if (error_expected) {
                    testing::record_failure(__FILE__, __LINE__, "decode of " + c[1] + " " + c[2] + "x" + c[3] + ": the old reader raised, this one decodes");
                    continue;
                }
                CB_CHECK_EQ(im.width, want[1].get<std::uint32_t>());
                CB_CHECK_EQ(im.height, want[2].get<std::uint32_t>());
                expect_same(im.rgba, want[4].get<std::string>());
            }
        }
        } catch (const Error&) {
            if (!error_expected) throw;
            threw = true;
        }
        if (error_expected) CB_CHECK(threw);
        ++checked;
    }
    CB_CHECK(checked > 100);
});

// ---- layout properties ---------------------------------------------------------------------------------------

Bytes counting(std::size_t n) {
    Bytes b(n);
    for (std::size_t i = 0; i < n; ++i) b[i] = static_cast<std::uint8_t>(i * 7 + (i >> 8));
    return b;
}

CB_TEST("formats.xbox.texture.tile_and_untile_are_inverses", [] {
    for (Format f : {Format::Dxt1, Format::Dxt3, Format::Dxt5}) {
        for (auto [w, h] : {std::pair{4u, 4u}, {5u, 3u}, {32u, 32u}, {33u, 17u}, {100u, 40u}, {256u, 64u}}) {
            const BlockSize bs = blocks(w, h);
            const Bytes linear = counting(std::size_t{bs.width} * bs.height * info(f).block_bytes);
            const Bytes tiled = tile(linear, w, h, f);
            CB_CHECK(untile(tiled, w, h, f) == linear);
            CB_CHECK(tiled.size() >= linear.size());
        }
    }
});

CB_TEST("formats.xbox.texture.the_tiled_surface_is_padded_to_macro_tiles", [] {
    // 8 blocks wide is padded to 32: the first row's blocks are in two 32x32 tile columns' worth of layout
    const Bytes linear = counting(8 * 8 * 8);
    const Bytes tiled = tile(linear, 32, 32, Format::Dxt1);
    CB_CHECK(tiled.size() >= tiled_offset_2d(7, 7, 8, 3) + 8);   // the surface reaches past the last block
    CB_CHECK(tiled.size() > linear.size());
    // block (0,0) is at offset 0, and its neighbour at the next 32-byte-per-16 slot (log2 3 = 8 bytes a block)
    CB_CHECK_EQ(tiled_offset_2d(0, 0, 8, 3), 0u);
    CB_CHECK_EQ(tiled_offset_2d(1, 0, 8, 3), 8u);
    CB_CHECK_EQ(tiled_offset_2d(0, 1, 8, 3), 16u);
    CB_CHECK(Bytes(tiled.begin(), tiled.begin() + 8) == Bytes(linear.begin(), linear.begin() + 8));
});

CB_TEST("formats.xbox.texture.blocks_past_the_data_stay_zero", [] {
    const Bytes tiled = counting(100);
    const Bytes linear = untile(tiled, 32, 32, Format::Dxt1);   // 8 x 8 blocks of 8 bytes: needs far more than 100 bytes
    CB_CHECK_EQ(linear.size(), 512u);
    // the part of the surface the data does reach is there, the rest is zeros
        std::size_t zeros = 0;
    for (std::uint8_t b : linear) zeros += b == 0;
    CB_CHECK(zeros > 250);
    CB_CHECK(untile(Bytes{}, 16, 16, Format::Dxt5) == Bytes(16 * 16, 0));   // 4 x 4 blocks of 16 bytes, all missing
    CB_CHECK(untile(Bytes{1, 2, 3}, 0, 0, Format::Dxt1).empty());
    CB_CHECK_EQ(tile(Bytes{}, 4, 4, Format::Dxt1).size(), 8u);
});

CB_TEST("formats.xbox.texture.sizes", [] {
    CB_CHECK_EQ(blocks(0, 0).width, 0u);
    CB_CHECK_EQ(blocks(1, 1).width, 1u);
    CB_CHECK_EQ(blocks(4, 4).width, 1u);
    CB_CHECK_EQ(blocks(5, 8).width, 2u);
    CB_CHECK_EQ(blocks(5, 8).height, 2u);
    CB_CHECK_EQ(blocks(0xFFFFFFFFu, 4).width, 0x40000000u);   // no overflow at the top of the range
    CB_CHECK_EQ(info(Format::Dxt1).block_bytes, 8u);
    CB_CHECK_EQ(info(Format::Dxt5).block_bytes, 16u);
    CB_CHECK_EQ(info(Format::Dxt3).fourcc, "DXT3");
    CB_CHECK(parse_format("dxt3") == Format::Dxt3);
    CB_CHECK(!parse_format("DXT3").has_value());
    CB_CHECK(!parse_format("dxt2").has_value());
    CB_CHECK_THROWS_AS(UnsupportedError, (void)untile(Bytes{}, 0xFFFFFFFFu, 0xFFFFFFFFu, Format::Dxt1));
});

CB_TEST("formats.xbox.texture.swap16", [] {
    CB_CHECK((swap16(Bytes{1, 2, 3, 4}) == Bytes{2, 1, 4, 3}));
    CB_CHECK(swap16(Bytes{}).empty());
    CB_CHECK_THROWS_AS(FormatError, (void)swap16(Bytes{1, 2, 3}));
});

CB_TEST("formats.xbox.texture.dds_header_fields", [] {
    const Bytes h = dds_header(100, 40, Format::Dxt5);
    CB_CHECK_EQ(h.size(), 128u);
    io::ByteReader r(h, "test");
    CB_CHECK_EQ(std::string(as_text(r.bytes(4))), "DDS ");
    CB_CHECK_EQ(r.u32(), 124u);
    CB_CHECK_EQ(r.u32(), 0x81007u);          // caps, height, width, pixel format, linear size
    CB_CHECK_EQ(r.u32(), 40u);
    CB_CHECK_EQ(r.u32(), 100u);
    CB_CHECK_EQ(r.u32(), 25u * 10u * 16u);   // linear size: 25 x 10 blocks of 16 bytes
    r.seek(76);
    CB_CHECK_EQ(r.u32(), 32u);               // pixel format size
    CB_CHECK_EQ(r.u32(), 4u);                // FOURCC
    CB_CHECK_EQ(std::string(as_text(r.bytes(4))), "DXT5");
    r.seek(108);
    CB_CHECK_EQ(r.u32(), 0x1000u);           // caps
    const Bytes body = {1, 2, 3};
    const Bytes dds = to_dds(body, 4, 4, Format::Dxt1);
    CB_CHECK_EQ(dds.size(), 131u);
    CB_CHECK(Bytes(dds.begin() + 128, dds.end()) == body);
});

// ---- decoding, by hand ---------------------------------------------------------------------------------------

// RGB565 of a colour, little-endian
std::pair<std::uint8_t, std::uint8_t> c565(int r5, int g6, int b5) {
    const int v = (r5 << 11) | (g6 << 5) | b5;
    return {static_cast<std::uint8_t>(v & 0xFF), static_cast<std::uint8_t>(v >> 8)};
}

CB_TEST("formats.xbox.texture.a_dxt1_block_by_hand", [] {
    // c0 = pure red (31,0,0), c1 = pure blue (0,0,31); c0 > c1 numerically: four colours. Indices: row 0 = 0,1,2,3
    const auto [r0, r1] = c565(31, 0, 0);
    const auto [b0, b1] = c565(0, 0, 31);
    Bytes block = {r0, r1, b0, b1, 0b11100100, 0, 0, 0};   // texel 0 -> c0, 1 -> c1, 2 -> 2/3 c0 + 1/3 c1, 3 -> 1/3 c0 + 2/3 c1
    const Image im = decode_dxt(block, 4, 4, Format::Dxt1, {.tiled = false, .swapped = false});
    CB_CHECK_EQ(im.width, 4u);
    CB_CHECK_EQ(im.rgba.size(), 64u);
    auto px = [&](int x, int y) { return std::array<int, 4>{im.rgba[(y * 4 + x) * 4], im.rgba[(y * 4 + x) * 4 + 1], im.rgba[(y * 4 + x) * 4 + 2], im.rgba[(y * 4 + x) * 4 + 3]}; };
    CB_CHECK((px(0, 0) == std::array<int, 4>{255, 0, 0, 255}));
    CB_CHECK((px(1, 0) == std::array<int, 4>{0, 0, 255, 255}));
    CB_CHECK((px(2, 0) == std::array<int, 4>{170, 0, 85, 255}));
    CB_CHECK((px(3, 0) == std::array<int, 4>{85, 0, 170, 255}));
    CB_CHECK((px(0, 1) == std::array<int, 4>{255, 0, 0, 255}));   // the other rows use index 0
    // c0 <= c1: three colours and a clear one at index 3
    Bytes punch = {b0, b1, r0, r1, 0b11100100, 0, 0, 0};
    const Image p = decode_dxt(punch, 4, 4, Format::Dxt1, {.tiled = false, .swapped = false});
    CB_CHECK_EQ(p.rgba[2 * 4 + 3], 255);   // index 2: the midpoint, opaque
    CB_CHECK_EQ(p.rgba[2 * 4], 127);       // (0 + 255) / 2
    CB_CHECK_EQ(p.rgba[3 * 4 + 3], 0);     // index 3: transparent black
    CB_CHECK_EQ(p.rgba[3 * 4], 0);
    // the Xbox 360 stores the 16-bit words swapped
    Bytes swapped = block;
    for (std::size_t i = 0; i < 8; i += 2) std::swap(swapped[i], swapped[i + 1]);
    CB_CHECK(decode_dxt(swapped, 4, 4, Format::Dxt1, {.tiled = false, .swapped = true}).rgba ==
             decode_dxt(block, 4, 4, Format::Dxt1, {.tiled = false, .swapped = false}).rgba);
});

CB_TEST("formats.xbox.texture.dxt3_and_dxt5_alpha_by_hand", [] {
    const auto [r0, r1] = c565(31, 63, 31);   // white
    const Bytes colour = {r0, r1, r0, r1, 0, 0, 0, 0};
    // DXT3: 4 bits of alpha a texel (stored 0..15, scaled by 17), the first texel's in the low nibble
    Bytes d3 = {0x0F, 0x80, 0, 0, 0, 0, 0, 0};
    append(d3, colour);
    const Image a3 = decode_dxt(d3, 4, 4, Format::Dxt3, {.tiled = false, .swapped = false});
    CB_CHECK_EQ(a3.rgba[3], 255);        // 0xF * 17
    CB_CHECK_EQ(a3.rgba[7], 0);
    CB_CHECK_EQ(a3.rgba[11], 0);
    CB_CHECK_EQ(a3.rgba[15], 8 * 17);    // the second byte's high nibble
    // DXT5, a0 > a1: eight levels, index 0 -> a0, 1 -> a1, 2 -> (6a0 + a1) / 7 ...
    Bytes d5 = {200, 100, 0b00001000 | 0b10, 0, 0, 0, 0, 0};   // texel 0 -> 2, texel 1 -> 1 (bits 3..5), texel 2 -> 0
    d5[2] = 0b00001010;                                          // 010 for texel 0, 001 for texel 1
    append(d5, colour);
    const Image a5 = decode_dxt(d5, 4, 4, Format::Dxt5, {.tiled = false, .swapped = false});
    CB_CHECK_EQ(a5.rgba[3], (6 * 200 + 100) / 7);
    CB_CHECK_EQ(a5.rgba[7], 100);
    CB_CHECK_EQ(a5.rgba[11], 200);
    // a0 <= a1: six levels plus 0 and 255
    Bytes e5 = {10, 100, 0b00110110, 0b00000001, 0, 0, 0, 0};   // texels: 6, 6 (110 110 -> texel 0 = 110b, texel 1 = 110b), then 7
    append(e5, colour);
    const Image b5 = decode_dxt(e5, 4, 4, Format::Dxt5, {.tiled = false, .swapped = false});
    CB_CHECK_EQ(b5.rgba[3], 0);
    CB_CHECK_EQ(b5.rgba[7], 0);
    // force_opaque ignores the alpha altogether
    const Image o5 = decode_dxt(e5, 4, 4, Format::Dxt5, {.tiled = false, .swapped = false, .force_opaque = true});
    for (std::size_t i = 3; i < o5.rgba.size(); i += 4) CB_CHECK_EQ(o5.rgba[i], 255);
});

CB_TEST("formats.xbox.texture.the_image_is_cut_to_its_size", [] {
    const auto [r0, r1] = c565(31, 0, 0);
    const Bytes one = {r0, r1, r0, r1, 0, 0, 0, 0};
    Bytes two;
    for (int i = 0; i < 4; ++i) append(two, one);       // 2 x 2 blocks
    const Image im = decode_dxt(two, 5, 3, Format::Dxt1, {.tiled = false, .swapped = false});
    CB_CHECK_EQ(im.width, 5u);
    CB_CHECK_EQ(im.height, 3u);
    CB_CHECK_EQ(im.rgba.size(), 60u);
    CB_CHECK_EQ(im.rgba[(2 * 5 + 4) * 4], 255);   // the last pixel is there
    // data that stops short: the blocks it does not hold are transparent black
    const Image cut = decode_dxt(ByteView(two).first(8), 8, 8, Format::Dxt1, {.tiled = false, .swapped = false});
    CB_CHECK_EQ(cut.rgba[3], 255);                 // block 0
    CB_CHECK_EQ(cut.rgba[(0 * 8 + 4) * 4 + 3], 0); // block 1: missing
    CB_CHECK_EQ(decode_dxt(Bytes{}, 0, 0, Format::Dxt1).rgba.size(), 0u);
});

}  // namespace
}  // namespace cb::formats::xbox::texture
