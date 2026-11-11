#include "cb/verify/gate16/screen.hpp"

#include <algorithm>
#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "zlib.h"

// The shrunk copies are compared with Pillow 12.2's Image.resize (bicubic, the default), run on the same pattern: the
// digests below were printed by Pillow, not by this code, and say that every byte of the shrunk picture is Pillow's.

using namespace cb;
using namespace cb::verify::gate16;

namespace {

// A picture whose pixels follow a formula that has no flat parts (so a filter that differs shows).
Picture pattern(std::uint32_t w, std::uint32_t h) {
    Bytes px;
    for (std::uint32_t y = 0; y < h; ++y)
        for (std::uint32_t x = 0; x < w; ++x) {
            px.push_back(static_cast<std::uint8_t>(x * 7 + y * 3));
            px.push_back(static_cast<std::uint8_t>(x * 5 + y * 11 + 40));
            px.push_back(static_cast<std::uint8_t>((x ^ y) * 9));
        }
    return Picture(w, h, std::move(px));
}

std::string sha1_of(const Picture& p) { return crypto::to_hex(crypto::sha1(p.rgb())); }

Bytes ppm_of(std::uint32_t w, std::uint32_t h, std::uint8_t fill, std::string_view header_extra = "") {
    Bytes out = to_bytes(std::format("P6\n{}# a comment\n{} {}\n255\n", header_extra, w, h));
    out.resize(out.size() + std::size_t{w} * h * 3, fill);
    return out;
}

}  // namespace

CB_TEST("verify.gate16.screen.shrunk_pictures_are_pillows", [] {
    CB_CHECK_EQ(sha1_of(pattern(640, 480).resized(160, 120)), "262e94b108d94d10b3eb7461f1f27f0b79dd3cbc");
    CB_CHECK_EQ(sha1_of(pattern(640, 480).resized(90, 50)), "7c2c17bf6b6cab57387733d66df065e7d692c347");
    CB_CHECK_EQ(sha1_of(pattern(30, 20).resized(60, 40)), "d7df7f938b905d96a5ee1b40645f17daee4f112b");   // stretched
    CB_CHECK_EQ(sha1_of(pattern(64, 48).resized(64, 12)), "8fc0878addde7bf34c75a0e9710a7dc2b258217f");   // one axis
    CB_CHECK_EQ(sha1_of(pattern(64, 48).resized(16, 48)), "fe199319f5aae765cbcafbf088670e88272457ac");
    // the first pixels, as Pillow printed them
    const Picture small = pattern(640, 480).resized(160, 120);
    CB_CHECK(small.at(0, 0) == std::optional<Rgb>(Rgb{16, 66, 18}));
    CB_CHECK(small.at(1, 0) == std::optional<Rgb>(Rgb{43, 85, 46}));
    CB_CHECK(small.at(2, 0) == std::optional<Rgb>(Rgb{72, 106, 89}));
});

CB_TEST("verify.gate16.screen.resize_of_the_same_size_and_of_flat_pictures", [] {
    const Picture p = pattern(16, 16);
    CB_CHECK(p.resized(16, 16).rgb() == p.rgb());
    // a flat picture stays flat whatever the filter's ringing does at the edges
    const Picture flat(40, 30, Bytes(40 * 30 * 3, 77));
    const Picture shrunk = flat.resized(10, 5);
    CB_CHECK(std::all_of(shrunk.rgb().begin(), shrunk.rgb().end(), [](std::uint8_t b) { return b == 77; }));
    CB_CHECK_THROWS_AS(UsageError, flat.resized(0, 5));
    CB_CHECK_EQ(Picture().resized(4, 4).rgb().size(), 48u);   // (an empty picture shrunk: black)
});

CB_TEST("verify.gate16.screen.pixels", [] {
    const Picture p = pattern(8, 4);
    CB_CHECK(p.at(1, 0) == std::optional<Rgb>(Rgb{7, 45, 9}));
    CB_CHECK(!p.at(8, 0).has_value());
    CB_CHECK(!p.at(0, 4).has_value());
    CB_CHECK_EQ((Rgb{10, 20, 30}).sum(), 60u);
    CB_CHECK_THROWS_AS(UsageError, Picture(2, 2, Bytes(5)));
});

CB_TEST("verify.gate16.screen.ppm", [] {
    const Bytes ok = ppm_of(4, 3, 0x40);
    const std::optional<Picture> p = Picture::try_from_ppm(ok);
    CB_CHECK(p.has_value());
    CB_CHECK_EQ(p->width(), 4u);
    CB_CHECK_EQ(p->height(), 3u);
    CB_CHECK(p->at(3, 2) == std::optional<Rgb>(Rgb{0x40, 0x40, 0x40}));
    // a comment inside the header, and white space of every kind
    CB_CHECK(Picture::try_from_ppm(to_bytes("P6 \t 1\r\n1  255\n\x01\x02\x03")).has_value());
    // damage: every prefix of a good file is nothing (a screendump still being written), never a crash
    for (std::size_t n = 0; n < ok.size(); ++n)
        CB_CHECK(!Picture::try_from_ppm(ByteView(ok).first(n)).has_value());
    CB_CHECK(!Picture::try_from_ppm(to_bytes("P5\n1 1\n255\n\x01")).has_value());       // not P6
    CB_CHECK(!Picture::try_from_ppm(to_bytes("P6\n0 1\n255\n")).has_value());           // no pixels
    CB_CHECK(!Picture::try_from_ppm(to_bytes("P6\n1 1\n65535\n\0\0\0\0\0\0")).has_value());   // 16 bits a channel
    CB_CHECK(!Picture::try_from_ppm(to_bytes("P6\n99999 99999\n255\n")).has_value());   // a size that is no screen
    CB_CHECK(!Picture::try_from_ppm(to_bytes("P6\nx y\n255\n")).has_value());
    CB_CHECK(!Picture::try_from_ppm(to_bytes("P6\n1234567890123 1\n255\n")).has_value());
    CB_CHECK(!Picture::try_from_ppm({}).has_value());
});

CB_TEST("verify.gate16.screen.png", [] {
    const Picture p = pattern(20, 7);
    const Bytes png = p.to_png();
    CB_CHECK(equal_bytes(ByteView(png).first(8), Bytes{0x89, 'P', 'N', 'G', '\r', '\n', 0x1A, '\n'}));
    // walk the chunks: IHDR (20x7, 8-bit RGB), one IDAT, IEND; each CRC right
    std::size_t at = 8;
    std::vector<std::string> kinds;
    Bytes idat;
    while (at + 12 <= png.size()) {
        const std::uint32_t length = png[at] << 24 | png[at + 1] << 16 | png[at + 2] << 8 | png[at + 3];
        const std::string kind(as_text(ByteView(png).subspan(at + 4, 4)));
        const ByteView body = ByteView(png).subspan(at + 8, length);
        const std::uint32_t crc = png[at + 8 + length] << 24 | png[at + 9 + length] << 16 | png[at + 10 + length] << 8 |
                                  png[at + 11 + length];
        CB_CHECK_EQ(crypto::crc32(ByteView(png).subspan(at + 4, 4 + length)), crc);
        kinds.push_back(kind);
        if (kind == "IHDR") {
            CB_CHECK_EQ(body[3], 20);
            CB_CHECK_EQ(body[7], 7);
            CB_CHECK_EQ(body[8], 8);    // bits
            CB_CHECK_EQ(body[9], 2);    // RGB
        }
        if (kind == "IDAT")
            idat.assign(body.begin(), body.end());
        at += 12 + length;
    }
    CB_CHECK(kinds == (std::vector<std::string>{"IHDR", "IDAT", "IEND"}));
    CB_CHECK_EQ(at, png.size());
    // the image data is the rows, each behind a 0 filter byte
    Bytes raw(7 * (1 + 20 * 3));
    z_stream zs{};
    CB_CHECK_EQ(inflateInit(&zs), Z_OK);
    zs.next_in = idat.data();
    zs.avail_in = static_cast<uInt>(idat.size());
    zs.next_out = raw.data();
    zs.avail_out = static_cast<uInt>(raw.size());
    CB_CHECK_EQ(inflate(&zs, Z_FINISH), Z_STREAM_END);
    CB_CHECK_EQ(zs.total_out, raw.size());
    inflateEnd(&zs);
    for (std::size_t y = 0; y < 7; ++y) {
        CB_CHECK_EQ(raw[y * 61], 0);
        CB_CHECK(equal_bytes(ByteView(raw).subspan(y * 61 + 1, 60), ByteView(p.rgb()).subspan(y * 60, 60)));
    }
});
