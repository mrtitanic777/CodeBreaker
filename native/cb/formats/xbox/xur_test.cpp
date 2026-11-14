// Tests of cb/formats/xbox/xur.hpp. A one-section XUR5 scene (four strings) is built, its strings read, and it is re-emitted with
// the same strings (byte-identical) and with one replaced; the same is checked here, with the recorded expected lines for that scene
// (testdata/assets/make_asset_goldens), plus a scene with sections after STRN to show them move.
//
// A section's offset is from the start of the file: the Minecraft title's own scenes (cut from its XUI package) show it -- a first section at 56 or 68 in a file whose table ends there.
// (Reading it as from the end of the table would make every real scene read as garbage.)
//
// Behaviour pinned here: a string cut off by the end of its section is FormatError; a string index out of range is
// UsageError; the length of a string in the table is its UTF-16 unit count (not its code-point count, which differs for a character
// outside the Basic Multilingual Plane).
#include "cb/formats/xbox/xur.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/xbox/asset_dump_kit.hpp"

namespace xur_test {

using namespace cb;
using namespace cb::formats::xbox;

static Bytes scene(const std::vector<std::string>& strings, bool with_tail = false) {
    const Bytes strn = xur::build_strn(strings);
    const Bytes tail = to_bytes("TAILDATA");
    io::ByteWriter w;
    w.u32_be(xur::kMagic);
    w.u32_be(5);
    w.u32_be(0);
    w.u16_be(0x0C);
    const std::size_t table = xur::kHeaderSize + (with_tail ? 2 : 1) * xur::kSectionEntrySize;
    w.u32_be(static_cast<std::uint32_t>(table + strn.size() + (with_tail ? tail.size() : 0)));
    w.u16_be(with_tail ? 2 : 1);
    w.u32_be(xur::kStrn);
    w.u32_be(static_cast<std::uint32_t>(table));   // offsets are from the start of the file
    w.u32_be(static_cast<std::uint32_t>(strn.size()));
    if (with_tail) {
        w.u32_be(0x5441494C);   // 'TAIL'
        w.u32_be(static_cast<std::uint32_t>(table + strn.size()));
        w.u32_be(static_cast<std::uint32_t>(tail.size()));
    }
    w.bytes(strn);
    if (with_tail) w.bytes(tail);
    return w.take();
}

CB_TEST("formats.xbox.xur.the_fixture_reads", [] {
    const Bytes file = fs::read_file(fs::from_text(asset_dump::fixture("xur.bin")));
    const std::string diff = asset_dump::difference(asset_dump::golden("xur.bin"), asset_dump::dump_xur("xur.bin", file));
    CB_CHECK(diff.empty());
    if (!diff.empty()) cb::testing::record_failure(__FILE__, __LINE__, "differs from the recorded dump:\n" + diff);
});

CB_TEST("formats.xbox.xur.test_xur", [] {
    const std::vector<std::string> strs = {"XuiCanvas", "file://game:/skin.png", "Press A to continue", ""};
    const Bytes blob = scene(strs);
    CB_CHECK(equal_bytes(blob, fs::read_file(fs::from_text(asset_dump::fixture("xur.bin")))));   // the recorded scene
    const xur::Scene x = xur::Scene::parse(blob);
    CB_CHECK_EQ(x.version, 5u);
    CB_CHECK_EQ(x.sections.size(), 1u);
    CB_CHECK(x.strings() == strs);
    CB_CHECK(equal_bytes(xur::set_strings(blob, strs), blob));
    CB_CHECK_EQ(xur::Scene::parse(xur::set_string(blob, 2, "Press START")).strings()[2], "Press START");
});

CB_TEST("formats.xbox.xur.later_sections_move_with_the_edit", [] {
    const std::vector<std::string> strs = {"one", "two"};
    const Bytes blob = scene(strs, true);
    CB_CHECK(equal_bytes(xur::set_strings(blob, strs), blob));
    const Bytes longer = xur::set_string(blob, 0, "a much longer first string");
    const xur::Scene s = xur::Scene::parse(longer);
    CB_CHECK(s.strings() == (std::vector<std::string>{"a much longer first string", "two"}));
    CB_CHECK_EQ(as_text(*s.section_data(0x5441494C)), "TAILDATA");   // the tail came along
    CB_CHECK_EQ(s.file_size, longer.size());
    const Bytes shorter = xur::set_string(blob, 1, "");
    CB_CHECK_EQ(as_text(*xur::Scene::parse(shorter).section_data(0x5441494C)), "TAILDATA");
    CB_CHECK_EQ(xur::Scene::parse(shorter).file_size, shorter.size());
});

CB_TEST("formats.xbox.xur.text_outside_the_basic_plane_reads_back", [] {
    // U+1F600 is two UTF-16 units; the table counts units
    const std::vector<std::string> strs = {"smile \xF0\x9F\x98\x80", "n\xC3\xA9"};
    const Bytes blob = scene(strs);
    CB_CHECK(xur::Scene::parse(blob).strings() == strs);
    CB_CHECK_EQ(xur::Scene::parse(blob).strings()[0], strs[0]);
});

CB_TEST("formats.xbox.xur.damaged_input_is_a_format_error_or_a_usage_error", [] {
    const Bytes blob = scene({"alpha", "beta"}, true);
    CB_CHECK_THROWS_AS(FormatError, (void)xur::Scene::parse(to_bytes("XUIB")));
    CB_CHECK_THROWS_AS(FormatError, (void)xur::Scene::parse(to_bytes("NOPE0000000000000000000000000000")));
    CB_CHECK_THROWS_AS(TruncatedError, (void)xur::Scene::parse(ByteView(blob).first(xur::kHeaderSize + 12)));   // two sections announced, one in the file
    // a section that runs past the file
    const xur::Scene cut = xur::Scene::parse(ByteView(blob).first(blob.size() - 4));
    CB_CHECK_THROWS_AS(FormatError, (void)cut.section_data(0x5441494C));
    // a string that runs past its section
    Bytes lying = blob;
    lying[xur::kHeaderSize + xur::kSectionEntrySize * 2 + 1] = 0xFF;   // the first string's length
    CB_CHECK_THROWS_AS(TruncatedError, (void)xur::Scene::parse(lying).strings());
    for (std::size_t n = 0; n < blob.size(); n += 3) {   // every prefix is an error or a scene
        try {
            const xur::Scene s = xur::Scene::parse(ByteView(blob).first(n));
            (void)s.strings();
        } catch (const FormatError&) {
        }
    }
    CB_CHECK_THROWS_AS(UsageError, (void)xur::set_string(blob, 2, "x"));   // two strings: 2 is out of range
    // no STRN to edit
    Bytes no_strn = blob;
    no_strn[xur::kHeaderSize] = 'X';
    CB_CHECK_THROWS_AS(UsageError, (void)xur::set_strings(no_strn, std::vector<std::string>{"a"}));
    CB_CHECK(xur::Scene::parse(no_strn).strings().empty());
});

CB_TEST("formats.xbox.xur.the_command_lists_and_edits", [] {
    fs::ScopedTempDir dir("xur");
    const std::string in = asset_dump::fixture("xur.bin"), target = fs::to_text(dir.path() / "e.xur");
    const cli::Cli app("codebreaker", {xur::command()});
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(app.run({"xur", "strings", in}), 0);
        CB_CHECK(cap.out().find("  [  2] Press A to continue") != std::string::npos);
        CB_CHECK(cap.out().find("total: 4 strings") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(app.run({"xur", "info", in}), 0);
        CB_CHECK(cap.out().find("magic       : XUIB  version 5  toolVersion 0xC") != std::string::npos);
        CB_CHECK(cap.out().find("   STRN   offset=0x20     length=106") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(app.run({"xur", "set", in, "0", "Hello", target}), 0);
    }
    CB_CHECK_EQ(xur::Scene::parse(fs::read_file(dir.path() / "e.xur")).strings()[0], "Hello");
});

}  // namespace xur_test
