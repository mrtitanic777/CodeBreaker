// Tests of cb/formats/xbox/bink.hpp. A "BIKi" file (header: size 1000000, 300 frames, largest 8192, 1280x720 at
// 30/1, one audio track) is written and its header read; the same file is checked here against recorded answers (testdata/assets/make_asset_goldens). XMA's half of that
// check is in cb/xna/xact.
//
// Behaviour pinned here: a header shorter than its 44 bytes is TruncatedError; the video flags are a number, shown as 0x%08X.
#include "cb/formats/xbox/bink.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/xbox/asset_dump_kit.hpp"

namespace bink_test {

using namespace cb;
using namespace cb::formats::xbox;

static Bytes header(std::string_view magic, std::uint32_t den = 1, std::uint32_t num = 30) {
    io::ByteWriter w;
    w.bytes(as_bytes(magic));
    for (const std::uint32_t v : {1000000u, 300u, 8192u, 300u, 1280u, 720u, num, den, 0u, 1u}) w.u32(v);
    w.zeros(20);
    return w.take();
}

CB_TEST("formats.xbox.bink.the_fixture_reads", [] {
    const Bytes file = fs::read_file(fs::from_text(asset_dump::fixture("t.bik")));
    CB_CHECK(equal_bytes(file, header("BIKi")));
    const std::string diff = asset_dump::difference(asset_dump::golden("t.bik"), asset_dump::dump_bink("t.bik", file));
    CB_CHECK(diff.empty());
    if (!diff.empty()) cb::testing::record_failure(__FILE__, __LINE__, "differs from the recorded dump:\n" + diff);
});

CB_TEST("formats.xbox.bink.test_xma_and_bink_header", [] {
    const bink::Header h = bink::read_header_file(asset_dump::fixture("t.bik"));
    CB_CHECK_EQ(h.width, 1280u);
    CB_CHECK_EQ(h.height, 720u);
    CB_CHECK_EQ(h.fps, 30.0);
    CB_CHECK_EQ(h.audio_tracks, 1u);
    CB_CHECK_EQ(h.generation, 1);
    CB_CHECK_EQ(h.file_size, 1000008u);   // the field counts from after itself
});

CB_TEST("formats.xbox.bink.generations_and_frame_rates", [] {
    for (const char* magic : {"BIKb", "BIKd", "BIKf", "BIKg", "BIKh", "BIKi", "BIKk"}) {
        CB_CHECK(bink::is_bink(header(magic)));
        CB_CHECK_EQ(bink::read_header(header(magic)).generation, 1);
    }
    for (const char* magic : {"KB2a", "KB2j", "KB2z"}) CB_CHECK_EQ(bink::read_header(header(magic)).generation, 2);
    CB_CHECK(!bink::is_bink(header("BIKa")));    // not one of the Bink 1 revisions
    CB_CHECK(!bink::is_bink(header("KB2A")));    // the Bink 2 letter is lower case
    CB_CHECK(!bink::is_bink(to_bytes("BI")));
    CB_CHECK_EQ(bink::read_header(header("BIKi", 1001, 30000)).fps, 29.97);   // to three places
    CB_CHECK_EQ(bink::read_header(header("BIKi", 0, 30)).fps, 0.0);           // no denominator
    CB_CHECK_EQ(bink::read_header(header("KB2j")).codec, "KB2j");
});

CB_TEST("formats.xbox.bink.damaged_input_is_a_format_error", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)bink::read_header(header("NOPE")));
    CB_CHECK_THROWS_AS(FormatError, (void)bink::read_header(Bytes{}));
    const Bytes h = header("BIKi");
    for (std::size_t n = 0; n < 44; ++n) CB_CHECK_THROWS_AS(FormatError, (void)bink::read_header(ByteView(h).first(n)));
    CB_CHECK_NOTHROW((void)bink::read_header(ByteView(h).first(44)));
});

CB_TEST("formats.xbox.bink.decode_needs_ffmpeg", [] {
    fs::ScopedTempDir dir("bink");
    fs::write_file(dir.path() / "a.bik", header("BIKi"));
    CB_CHECK_THROWS_AS(ToolError, (void)bink::decode(dir.path() / "a.bik", dir.path() / "a.wav", std::nullopt));
    // a tool that cannot be started, and one that fails (cmd with a command that does not exist)
    CB_CHECK_THROWS_AS(ToolError, (void)bink::decode(dir.path() / "a.bik", dir.path() / "a.wav", dir.path() / "no-such-ffmpeg.exe"));
});

CB_TEST("formats.xbox.bink.the_command_prints_the_summary", [] {
    const cli::Cli app("codebreaker", {bink::command({})});
    testing::CapturedStd cap;
    CB_CHECK_EQ(app.run({"bink", "info", asset_dump::fixture("t.bik")}), 0);
    const std::string out = cap.out();
    CB_CHECK(out.find("Bink1 (BIKi): ") != std::string::npos);
    CB_CHECK(out.find("  1280 x 720  @ 30.000 fps   300 frames   1 audio track(s)") != std::string::npos);
    CB_CHECK(out.find("  file size 1000008 bytes  (largest frame 8192)  flags 0x00000000") != std::string::npos);
});

}  // namespace bink_test
