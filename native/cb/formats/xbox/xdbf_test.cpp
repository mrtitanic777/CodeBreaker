// Tests of cb/formats/xbox/xdbf.hpp. The inputs are the ones tests/test_xbox_formats.py builds (test_gpd_unlock); the expected lines are what the Python's
// xdbf module (through formats.dll) answers for the same bytes (testdata/assets/make_asset_goldens.py makes them), and the file the test builds
// itself is checked against what that Python test asserts.
//
// DIVERGES from the Python: the strings of an achievement are split on 16-bit boundaries (the Python split on any two zero bytes, which cuts a
// character whose low byte is zero followed by one whose high byte is zero); an entry outside the file is FormatError (the Python clamped it).
// `payload` answers the current payload, edits included (the Python's answered the file's bytes).
#include "cb/formats/xbox/xdbf.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/xbox/asset_dump_kit.hpp"
#include "cb/formats/xbox/utf16.hpp"

namespace xdbf_test {

using namespace cb;
using namespace cb::formats::xbox;

// An achievement record as the Python test makes it.
static Bytes record(std::uint32_t id, std::uint32_t score, const std::string& name, const std::string& unlocked = "Unlocked",
                    const std::string& locked = "Locked") {
    io::ByteWriter w;
    w.u32_be(0);   // the size field (the Python overwrites it with the body's length; nothing reads it)
    w.u32_be(id);
    w.u32_be(5);
    w.u32_be(score);
    w.u32_be(0);
    w.u64_be(0);
    for (const std::string& s : {name, unlocked, locked}) {
        w.bytes(to_utf16be(s));
        w.u16_be(0);
    }
    return w.take();
}

static Bytes build_file(const std::vector<std::pair<std::uint64_t, Bytes>>& achievements) {
    xdbf::Database db;
    db.version = 0x10000;
    db.entry_table_len = static_cast<std::uint32_t>(achievements.size());
    db.free_table_len = 1;
    for (const auto& [id, payload] : achievements) db.entries.push_back({1, id, payload});
    return db.build();
}

CB_TEST("formats.xbox.xdbf.the_fixture_reads_as_the_pythons_does", [] {
    const Bytes file = fs::read_file(fs::from_text(asset_dump::fixture("gpd.bin")));
    const std::string diff = asset_dump::difference(asset_dump::golden("gpd.bin"), asset_dump::dump_xdbf("gpd.bin", file));
    CB_CHECK(diff.empty());
    if (!diff.empty()) cb::testing::record_failure(__FILE__, __LINE__, "differs from the Python:\n" + diff);
});

CB_TEST("formats.xbox.xdbf.unlock_and_rebuild_as_test_gpd_unlock", [] {
    const Bytes gpd = build_file({{1, record(1, 10, "First Blood")}, {2, record(2, 25, "Marathon")}});
    xdbf::Database x = xdbf::Database::parse(gpd);
    CB_CHECK(x.unlock_achievement(1, true));
    const xdbf::Database y = xdbf::Database::parse(x.build());
    std::map<std::uint32_t, xdbf::Achievement> a;
    for (const xdbf::Achievement& r : y.achievements()) a[r.id] = r;
    CB_CHECK(a[1].unlocked && !a[2].unlocked);
    const auto payload = y.payload(1, 1);
    CB_CHECK(payload.has_value() && payload->size() >= 0x1C);
    io::ByteReader r(*payload);
    r.seek(0x14);
    CB_CHECK_EQ(r.u64_be(), xdbf::kUnlockFiletime);
    CB_CHECK_EQ(a[1].name, "First Blood");
    CB_CHECK_EQ(a[2].score, 25u);
    CB_CHECK_EQ(a[1].unlocked_desc, "Unlocked");
    CB_CHECK_EQ(a[2].locked_desc, "Locked");
    CB_CHECK(!x.unlock_achievement(99, true));   // none with that id
    CB_CHECK(x.unlock_achievement(1, false));    // and back
    CB_CHECK(!xdbf::Database::parse(x.build()).achievements()[0].unlocked);
});

CB_TEST("formats.xbox.xdbf.strings_are_split_on_unit_boundaries", [] {
    // U+3000 (bytes 30 00) then 'A' (00 41) puts two zero bytes in a row that are not a terminator
    const std::string name = "\xE3\x80\x80" "A";
    const Bytes gpd = build_file({{1, record(7, 5, name, "d", "l")}});
    const std::vector<xdbf::Achievement> a = xdbf::Database::parse(gpd).achievements();
    CB_CHECK_EQ(a.size(), 1u);
    CB_CHECK_EQ(a[0].name, name);
    CB_CHECK_EQ(a[0].unlocked_desc, "d");
    CB_CHECK_EQ(a[0].locked_desc, "l");
});

CB_TEST("formats.xbox.xdbf.a_short_or_unterminated_record", [] {
    CB_CHECK(!xdbf::decode_achievement(Bytes(0x1B, 0)).has_value());
    Bytes b(0x1C, 0);
    append(b, to_utf16be("Only a name"));   // no terminator, no descriptions
    const auto a = xdbf::decode_achievement(b);
    CB_CHECK(a.has_value());
    CB_CHECK_EQ(a->name, "Only a name");
    CB_CHECK_EQ(a->unlocked_desc, "");
    CB_CHECK_EQ(a->locked_desc, "");
});

CB_TEST("formats.xbox.xdbf.set_payload_and_build_keep_order_and_capacity", [] {
    const Bytes file = fs::read_file(fs::from_text(asset_dump::fixture("gpd.bin")));
    xdbf::Database db = xdbf::Database::parse(file);
    db.set_payload(5, 0x1234, to_bytes("hello"));   // new: at the end
    db.set_payload(1, 2, to_bytes("changed"));      // replaced in place
    const xdbf::Database again = xdbf::Database::parse(db.build());
    CB_CHECK_EQ(again.entries.size(), 3u);
    CB_CHECK(again.entries[1].payload == to_bytes("changed"));
    CB_CHECK_EQ(again.entries[2].ns, 5);
    CB_CHECK_EQ(again.entries[2].id, 0x1234u);
    CB_CHECK(again.payload(5, 0x1234).has_value());
    CB_CHECK(!again.payload(5, 0x1235).has_value());
    CB_CHECK_EQ(again.entry_table_len, 3u);   // grew to hold them
    CB_CHECK_EQ(db.counts().at(1), 2u);
    // an unchanged database writes the file it read
    CB_CHECK(equal_bytes(xdbf::Database::parse(file).build(), file));
});

CB_TEST("formats.xbox.xdbf.damaged_input_is_a_format_error", [] {
    const Bytes file = fs::read_file(fs::from_text(asset_dump::fixture("gpd.bin")));
    CB_CHECK_THROWS_AS(FormatError, (void)xdbf::Database::parse(to_bytes("NOPE0000000000000000000000")));
    CB_CHECK_THROWS_AS(TruncatedError, (void)xdbf::Database::parse(Bytes{}));
    // every prefix of the file is an error or a database, and never a crash
    for (std::size_t n = 0; n < file.size(); ++n) {
        try {
            (void)xdbf::Database::parse(ByteView(file).first(n));
        } catch (const FormatError&) {
        }
    }
    CB_CHECK_THROWS_AS(FormatError, (void)xdbf::Database::parse(ByteView(file).first(file.size() - 1)));   // the last payload is cut short
    Bytes many = file;   // a count of 0xFFFFFFFF entries cannot be in the file
    for (std::size_t i = 12; i < 16; ++i) many[i] = 0xFF;
    CB_CHECK_THROWS_AS(TruncatedError, (void)xdbf::Database::parse(many));
});

CB_TEST("formats.xbox.xdbf.extract_writes_one_file_per_entry", [] {
    fs::ScopedTempDir dir("xdbf");
    const xdbf::Database db = xdbf::Database::parse(fs::read_file(fs::from_text(asset_dump::fixture("gpd.bin"))));
    CB_CHECK_EQ(db.extract_all(dir.path() / "out"), 2u);
    CB_CHECK(fs::read_file(dir.path() / "out" / "achievement_0000000000000001.bin") == db.entries[0].payload);
    CB_CHECK_EQ(xdbf::namespace_name(5), "string");
    CB_CHECK_EQ(xdbf::namespace_name(99), "ns99");
});

CB_TEST("formats.xbox.xdbf.the_command_prints_the_listing", [] {
    const std::string gpd = asset_dump::fixture("gpd.bin");
    const cli::Cli app("codebreaker", {xdbf::command()});
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(app.run({"xdbf", "ach", gpd}), 0);
        const std::string out = cap.out();
        CB_CHECK(out.find("First Blood") != std::string::npos);
        CB_CHECK(out.find("total: 2 achievements, 0 unlocked, 35G") != std::string::npos);
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(app.run({"xdbf", "info", gpd}), 0);
        const std::string out = cap.out();
        CB_CHECK(out.find("version     : 0x10000") != std::string::npos);
        CB_CHECK(out.find("entries     : 2 (table 2)  free: 1") != std::string::npos);
        CB_CHECK(out.find("achievement    2") != std::string::npos);
    }
    {
        fs::ScopedTempDir dir("xdbf");
        const std::string target = fs::to_text(dir.path() / "u.gpd");
        testing::CapturedStd cap;
        CB_CHECK_EQ(app.run({"xdbf", "unlock-all", gpd, target}), 0);
        const xdbf::Database db = xdbf::Database::parse(fs::read_file(dir.path() / "u.gpd"));
        CB_CHECK(db.achievements()[0].unlocked && db.achievements()[1].unlocked);
        CB_CHECK(cap.out().find("unlocked 2 achievements") != std::string::npos);
    }
});

}  // namespace xdbf_test
