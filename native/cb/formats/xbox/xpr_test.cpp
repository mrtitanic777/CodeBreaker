// Tests of cb/formats/xbox/xpr.hpp. A 64x32 DXT1 surface (a recognisable ramp) wrapped in an XPR0
// bundle, read back (format, dimensions, pixels), turned into a DDS whose last bytes are the pixels, and an XPR2 header with its two sizes. The same checks are
// made here, with the recorded expected lines for those bundles (testdata/assets/make_asset_goldens).
//
// Behaviour pinned here: a bundle whose pixel data starts past the end of the file is FormatError; `--selftest` is these tests;
// build_xpr0 of a size that is not a power of two is a UsageError.
#include "cb/formats/xbox/xpr.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/xbox/asset_dump_kit.hpp"

namespace xpr_test {

using namespace cb;
using namespace cb::formats::xbox;

static Bytes ramp() {
    Bytes pixels(64 * 32 / 2);
    for (std::size_t i = 0; i < pixels.size(); ++i) pixels[i] = static_cast<std::uint8_t>((i * 7) & 0xFF);
    return pixels;
}

CB_TEST("formats.xbox.xpr.the_fixtures_read", [] {
    for (const char* name : {"xpr0.bin", "xpr2.bin"}) {
        const Bytes file = fs::read_file(fs::from_text(asset_dump::fixture(name)));
        const std::string diff = asset_dump::difference(asset_dump::golden(name), asset_dump::dump_xpr(name, file));
        CB_CHECK(diff.empty());
        if (!diff.empty()) cb::testing::record_failure(__FILE__, __LINE__, std::string(name) + " differs from the recorded dump:\n" + diff);
    }
});

CB_TEST("formats.xbox.xpr.the_selfcheck", [] {
    const Bytes pixels = ramp();
    const Bytes blob = xpr::build_xpr0(pixels, 64, 32, 0x0C);
    CB_CHECK(equal_bytes(blob, fs::read_file(fs::from_text(asset_dump::fixture("xpr0.bin")))));   // the recorded bundle
    const xpr::Bundle info = xpr::parse(blob);
    CB_CHECK(info.container == xpr::Container::Xpr0);
    CB_CHECK(info.dds_format.has_value() && *info.dds_format == texture::Format::Dxt1);
    CB_CHECK_EQ(info.width, 64u);
    CB_CHECK_EQ(info.height, 32u);
    CB_CHECK_EQ(info.mipmaps, 1u);
    CB_CHECK(!info.cubemap);
    CB_CHECK(equal_bytes(info.pixels, pixels));
    const Bytes dds = xpr::to_dds(info);
    CB_CHECK_EQ(as_text(ByteView(dds).first(4)), "DDS ");
    CB_CHECK(equal_bytes(ByteView(dds).last(pixels.size()), pixels));
    // an XPR2 header: magic, two sizes, 8 bytes of padding, then the pixels
    io::ByteWriter x2;
    x2.bytes(as_bytes("XPR2"));
    x2.u32_be(0x14);
    x2.u32_be(static_cast<std::uint32_t>(pixels.size()));
    x2.zeros(8);
    x2.bytes(pixels);
    const xpr::Bundle i2 = xpr::parse(x2.data());
    CB_CHECK(i2.container == xpr::Container::Xpr2);
    CB_CHECK_EQ(i2.data_offset, 0x14u);
    CB_CHECK(equal_bytes(i2.pixels, pixels));
    CB_CHECK_EQ(i2.data_size, pixels.size());
});

CB_TEST("formats.xbox.xpr.formats_and_flags", [] {
    struct Case {
        std::uint32_t xfmt;
        const char* name;
        std::optional<texture::Format> dds;
    };
    const Case cases[] = {{0x0E, "DXT3", texture::Format::Dxt3}, {0x0F, "DXT5", texture::Format::Dxt5}, {0x06, "A8R8G8B8", std::nullopt}, {0x5A, "0x5A", std::nullopt}};
    for (const Case& c : cases) {
        const xpr::Bundle b = xpr::parse(xpr::build_xpr0(ramp(), 128, 16, c.xfmt, 3));
        CB_CHECK_EQ(b.format_name, std::string(c.name));
        CB_CHECK(b.dds_format == c.dds);
        CB_CHECK_EQ(b.width, 128u);
        CB_CHECK_EQ(b.height, 16u);
        CB_CHECK_EQ(b.mipmaps, 3u);
        if (!c.dds) CB_CHECK_THROWS_AS(UnsupportedError, (void)xpr::to_dds(b));   // no DDS path for it
    }
    CB_CHECK_THROWS_AS(UsageError, (void)xpr::build_xpr0(ramp(), 100, 32));
    // a cube map bit; a total of zero means "to the end"; a header size of zero means the minimum
    Bytes blob = xpr::build_xpr0(ramp(), 64, 32);
    blob[0x18] |= 0x4;
    for (std::size_t i = 4; i < 12; ++i) blob[i] = 0;
    const xpr::Bundle b = xpr::parse(blob);
    CB_CHECK(b.cubemap);
    CB_CHECK_EQ(b.data_offset, 0x1Cu);
    CB_CHECK_EQ(b.pixels.size(), ramp().size());
    CB_CHECK_EQ(b.total, blob.size());
});

CB_TEST("formats.xbox.xpr.an_xpr2_needs_its_dimensions", [] {
    const xpr::Bundle b = xpr::parse(fs::read_file(fs::from_text(asset_dump::fixture("xpr2.bin"))));
    CB_CHECK_THROWS_AS(UsageError, (void)xpr::to_dds(b));
    CB_CHECK_THROWS_AS(UsageError, (void)xpr::to_dds(b, 64, std::nullopt));
    const Bytes dds = xpr::to_dds(b, 64, 32);   // a DXT5 DDS (its exact bytes are the golden of the fixtures)
    CB_CHECK_EQ(as_text(ByteView(dds).first(4)), "DDS ");
    CB_CHECK_EQ(as_text(ByteView(dds).subspan(84, 4)), "DXT5");
});

CB_TEST("formats.xbox.xpr.damaged_input_is_a_format_error", [] {
    const Bytes blob = fs::read_file(fs::from_text(asset_dump::fixture("xpr0.bin")));
    CB_CHECK_THROWS_AS(FormatError, (void)xpr::parse(to_bytes("XPR1 not a bundle at all......")));
    CB_CHECK_THROWS_AS(FormatError, (void)xpr::parse(Bytes{}));
    CB_CHECK_THROWS_AS(FormatError, (void)xpr::parse(ByteView(blob).first(0x1B)));   // an XPR0 header is 0x1C bytes
    CB_CHECK_THROWS_AS(FormatError, (void)xpr::parse(to_bytes("XPR2\0\0\0\x14")));
    for (std::size_t n = 0; n < 0x40; ++n) {
        try {
            (void)xpr::parse(ByteView(blob).first(n));
        } catch (const FormatError&) {
        }
    }
    Bytes beyond = blob;   // pixel data that starts past the end of the file
    beyond[8] = 0xFF;
    beyond[9] = 0xFF;
    CB_CHECK_THROWS_AS(FormatError, (void)xpr::parse(beyond));
    // a total that is smaller than the file cuts the pixels there; a larger one reads to the end
    Bytes cut = blob;
    cut[4] = 0x1C + 16;
    cut[5] = 0;
    CB_CHECK_EQ(xpr::parse(cut).pixels.size(), 16u);
    cut[5] = 0xFF;
    CB_CHECK_EQ(xpr::parse(cut).pixels.size(), blob.size() - 0x1C);
});

CB_TEST("formats.xbox.xpr.the_command_writes_the_dds", [] {
    fs::ScopedTempDir dir("xpr");
    const std::string in = asset_dump::fixture("xpr0.bin"), target = fs::to_text(dir.path() / "t.dds");
    const cli::Cli app("codebreaker", {xpr::command()});
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(app.run({"xpr", "info", in}), 0);
        CB_CHECK(cap.out().find("  format_name  DXT1") != std::string::npos);
        CB_CHECK(cap.out().find("  width        64") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(app.run({"xpr", "dds", in, target}), 0);
        CB_CHECK(cap.out().find("wrote " + target + " (1152 bytes, DXT1 64x32)") != std::string::npos);
    }
    CB_CHECK_EQ(fs::file_size(dir.path() / "t.dds"), 1152u);
});

}  // namespace xpr_test
