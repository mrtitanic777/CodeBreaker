#include "cb/java/console/png_rgba.hpp"

#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/java/console/test_support.hpp"

namespace {

using namespace cb;
using namespace cb::java::console;
namespace stdfs = std::filesystem;

std::string hex(ByteView v) { return crypto::to_hex(v); }

Bytes png_file(const std::string& name) { return fs::read_file(test::data("assets/png/" + name + ".png")); }

// The golden lines have the form "png NAME: ct=N needs=B changed=B WxH PIXELS" and "bad NAME: ...".
std::string field(const std::string& line, const std::string& key) {
    const std::size_t at = line.find(key);
    const std::size_t from = at + key.size();
    return line.substr(from, line.find(' ', from) - from);
}

CB_TEST("java.console.png.every_kind_decodes_as_the_original_did", [] {
    int cases = 0;
    const std::string golden_text = test::golden("assets.png.golden.txt");
    for (std::string_view line : text::lines(golden_text)) {
        if (!line.starts_with("png ")) continue;
        const std::string l(line);
        const std::string name = l.substr(4, l.find(':') - 4);
        const Bytes raw = png_file(name);
        const bool needs = field(l, "needs=") == "True";
        CB_CHECK_EQ(png::needs_rgba(raw), needs);
        CB_CHECK_EQ(static_cast<int>(*png::color_type(raw)), std::stoi(field(l, "ct=")));

        const png::Converted c = png::normalize_to_rgba(raw);
        CB_CHECK_EQ(c.changed, field(l, "changed=") == "True");
        // the pixels of the converted picture (an RGBA input is passed through as it is) are the ones the original gave
        const auto parts = text::split(line, ' ');
        const std::string size(parts[parts.size() - 2]), pixels(parts.back());
        const png::Image img = png::decode_rgba(c.png);
        CB_CHECK_EQ(hex(img.rgba), pixels);
        CB_CHECK_EQ(std::format("{}x{}", img.width, img.height), size);
        // the input read directly gives the same pixels as the converted one
        CB_CHECK_EQ(hex(png::decode_rgba(raw).rgba), hex(img.rgba));
        if (c.changed) {
            // an 8-bit RGBA PNG: IHDR bit depth 8, colour type 6, no interlace
            CB_CHECK_EQ(static_cast<int>(*png::color_type(c.png)), 6);
            CB_CHECK_EQ(static_cast<int>(c.png[24]), 8);
            CB_CHECK_EQ(static_cast<int>(c.png[28]), 0);
            CB_CHECK(!png::needs_rgba(c.png));
            CB_CHECK(png::normalize_to_rgba(c.png).changed == false);   // converting twice changes nothing more
        } else {
            CB_CHECK(equal_bytes(c.png, raw));
        }
        ++cases;
    }
    CB_CHECK_EQ(cases, 23);
});

CB_TEST("java.console.png.unreadable_pngs", [] {
    int cases = 0;
    const std::string golden_text = test::golden("assets.png.golden.txt");
    for (std::string_view line : text::lines(golden_text)) {
        if (!line.starts_with("bad ")) continue;
        const std::string l(line);
        const std::string name = l.substr(4, l.find(':') - 4);
        const Bytes raw = png_file("bad_" + name);
        CB_CHECK_EQ(png::needs_rgba(raw), l.find("needs=True") != std::string::npos);
        if (l.find("raises") != std::string::npos) {
            CB_CHECK_THROWS_AS(cb::FormatError, (void)png::normalize_to_rgba(raw));
            CB_CHECK_THROWS_AS(cb::FormatError, (void)png::decode_rgba(raw));
        } else {
            // not a PNG (or too short to say): passed through, as the original did
            CB_CHECK(!png::normalize_to_rgba(raw).changed);
            CB_CHECK(equal_bytes(png::normalize_to_rgba(raw).png, raw));
        }
        ++cases;
    }
    CB_CHECK_EQ(cases, 6);
    CB_CHECK(!png::color_type({}).has_value());
    CB_CHECK_THROWS_AS(cb::FormatError, (void)png::decode_rgba({}));
});

CB_TEST("java.console.png.damage_is_reported_not_guessed", [] {
    const Bytes good = png_file("rgb8");
    CB_CHECK_NOTHROW((void)png::decode_rgba(good));
    // every prefix of the file is either a clean error or (for the tail after the data) the picture; nothing crashes
    for (std::size_t n = 0; n < good.size(); n += 3) {
        const Bytes cut(good.begin(), good.begin() + static_cast<std::ptrdiff_t>(n));
        try {
            (void)png::decode_rgba(cut);
        } catch (const cb::FormatError&) {
        }
    }
    // a header that lies about the size: the data stream is shorter than the image needs
    Bytes big = good;
    big[16] = 0x00; big[17] = 0x00; big[18] = 0x10; big[19] = 0x00;   // width 4096
    CB_CHECK_THROWS_AS(cb::FormatError, (void)png::decode_rgba(big));   // (the header CRC no longer matches either)
    // a chunk type that is not IHDR where the header must be
    Bytes bad_header = good;
    bad_header[12] = 'X';
    CB_CHECK_THROWS_AS(cb::FormatError, (void)png::decode_rgba(bad_header));
    // a width of zero
    Bytes zero = good;
    zero[19] = 0;
    CB_CHECK_THROWS_AS(cb::FormatError, (void)png::decode_rgba(zero));
});

CB_TEST("java.console.png.encode_decode_round_trip", [] {
    png::Image img;
    img.width = 3;
    img.height = 2;
    for (unsigned i = 0; i < 3 * 2 * 4; ++i) img.rgba.push_back(static_cast<std::uint8_t>(i * 11));
    const Bytes encoded = png::encode_rgba(img);
    CB_CHECK_EQ(static_cast<int>(*png::color_type(encoded)), 6);
    const png::Image back = png::decode_rgba(encoded);
    CB_CHECK_EQ(back.width, 3u);
    CB_CHECK_EQ(back.height, 2u);
    CB_CHECK(back.rgba == img.rgba);
    img.rgba.pop_back();
    CB_CHECK_THROWS_AS(cb::InternalError, (void)png::encode_rgba(img));    // pixels that do not fit the size are a bug in the caller
});

}  // namespace
