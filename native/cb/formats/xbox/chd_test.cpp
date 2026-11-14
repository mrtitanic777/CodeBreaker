// Tests of cb/formats/xbox/chd.hpp. detect.identify takes a "MComprHD" header with version 5 (cb/formats/detect covers that);
// the header reader itself is checked here against the recorded answers for a v5 header made for the purpose
// (testdata/assets/make_asset_goldens: zlib and lzma, 4.7 GB logical, 19584-byte hunks), and against the cases that are refused.
//
// Behaviour pinned here: a version other than 5 is UnsupportedError and a bad magic FormatError (both ChdError); a header cut short is TruncatedError;
// the conversions take the chdman path from the caller (settings or PATH is the application's business).
#include "cb/formats/xbox/chd.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/xbox/asset_dump_kit.hpp"

namespace chd_test {

using namespace cb;
using namespace cb::formats::xbox;

static Bytes header(std::uint32_t version = 5, std::uint32_t c0 = 0x7A6C6962, std::uint32_t c1 = 0x6C7A6D61) {
    io::ByteWriter w;
    w.bytes(as_bytes("MComprHD"));
    w.u32_be(124);
    w.u32_be(version);
    for (const std::uint32_t c : {c0, c1, 0u, 0u}) w.u32_be(c);
    w.u64_be(4700000256ull);
    w.u64_be(0x1234);
    w.u64_be(0x7F00);
    w.u32_be(19584);
    w.u32_be(2048);
    w.zeros(124 - w.size());
    return w.take();
}

CB_TEST("formats.xbox.chd.the_fixture_reads", [] {
    const Bytes file = fs::read_file(fs::from_text(asset_dump::fixture("t.chd")));
    CB_CHECK(equal_bytes(file, header()));
    const std::string diff = asset_dump::difference(asset_dump::golden("t.chd"), asset_dump::dump_chd("t.chd", file));
    CB_CHECK(diff.empty());
    if (!diff.empty()) cb::testing::record_failure(__FILE__, __LINE__, "differs from the recorded dump:\n" + diff);
});

CB_TEST("formats.xbox.chd.a_v5_header", [] {
    const chd::Header h = chd::read_header_file(asset_dump::fixture("t.chd"));
    CB_CHECK_EQ(h.version, 5u);
    CB_CHECK_EQ(h.header_len, 124u);
    CB_CHECK_EQ(h.logical_bytes, 4700000256ull);
    CB_CHECK_EQ(h.hunk_bytes, 19584u);
    CB_CHECK_EQ(h.unit_bytes, 2048u);
    CB_CHECK(h.compressors == (std::vector<std::string>{"zlib", "lzma"}));
    CB_CHECK_EQ(h.map_offset, 0x1234u);
    CB_CHECK_EQ(h.meta_offset, 0x7F00u);
    CB_CHECK(chd::read_header(header(5, 0, 0)).compressors.empty());   // an uncompressed CHD
});

CB_TEST("formats.xbox.chd.damaged_and_other_versions", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)chd::read_header(to_bytes("NOT A CHD FILE, NOT A CHD FILE")));
    CB_CHECK_THROWS_AS(FormatError, (void)chd::read_header(Bytes{}));
    CB_CHECK_THROWS_AS(UnsupportedError, (void)chd::read_header(header(4)));
    CB_CHECK_THROWS_AS(UnsupportedError, (void)chd::read_header(header(3)));
    const Bytes h = header();
    for (std::size_t n = 0; n < 64; ++n) CB_CHECK_THROWS_AS(FormatError, (void)chd::read_header(ByteView(h).first(n)));
    CB_CHECK_NOTHROW((void)chd::read_header(ByteView(h).first(64)));   // the fields end at 64
});

CB_TEST("formats.xbox.chd.conversion_needs_chdman", [] {
    fs::ScopedTempDir dir("chd");
    fs::write_file(dir.path() / "a.iso", Bytes(2048, 1));
    CB_CHECK_THROWS_AS(ToolError, (void)chd::build(std::nullopt, dir.path() / "a.iso", dir.path() / "a.chd"));
    CB_CHECK_THROWS_AS(ToolError, (void)chd::extract(std::nullopt, dir.path() / "a.chd", dir.path() / "b.iso"));
    CB_CHECK_THROWS_AS(ToolError, (void)chd::verify(std::nullopt, dir.path() / "a.chd"));
    CB_CHECK_THROWS_AS(ToolError, (void)chd::verify(dir.path() / "no-such-chdman.exe", dir.path() / "a.chd"));   // cannot be started
});

CB_TEST("formats.xbox.chd.the_command_prints_the_header", [] {
    const cli::Cli app("codebreaker", {chd::command({})});
    testing::CapturedStd cap;
    CB_CHECK_EQ(app.run({"chd", "info", asset_dump::fixture("t.chd")}), 0);
    const std::string out = cap.out();
    CB_CHECK(out.find("CHD v5: ") != std::string::npos);
    CB_CHECK(out.find("logical size : 4700000256 bytes (4.38 GB)") != std::string::npos);
    CB_CHECK(out.find("hunk / unit  : 19584 / 2048 bytes") != std::string::npos);
    CB_CHECK(out.find("compressors  : zlib, lzma") != std::string::npos);
});

}  // namespace chd_test
