#include "cb/formats/xex.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"

namespace cb::formats::xex {
namespace {

// The header (first 0x2000 bytes, up to the image) of a real title: Pac-Man Championship Edition (Xbox 360 Arcade).
Bytes pacman_header() { return fs::read_file(testing::data_path("formats/testdata/pacman_ce.xex.head")); }

CB_TEST("formats.xex.optional_headers_of_a_real_title", [] {
    const Bytes data = pacman_header();
    OptionalHeaders table(data);
    CB_CHECK_EQ(table.count(), 15u);
    // The entries as the old reader gave them for this file (offset, key, value): the table, in order.
    const OptionalHeader want[] = {
        {24, 767, 1324},          {32, 1023, 1344},         {40, 65792, 2181436344u}, {48, 66049, 2181038080u},
        {56, 66559, 6724},        {64, 98306, 1380},        {72, 99327, 1388},        {80, 131327, 1404},
        {88, 131332, 1600},       {96, 131584, 262144},     {104, 196608, 4},         {112, 262150, 1616},
        {120, 262928, 1640},      {128, 263172, 1704},      {136, 263679, 1720},
    };
    for (const OptionalHeader& w : want) {
        const std::optional<OptionalHeader> got = table.next();
        CB_CHECK(got.has_value());
        if (!got) break;
        CB_CHECK_EQ(got->offset, w.offset);
        CB_CHECK_EQ(got->key, w.key);
        CB_CHECK_EQ(got->value, w.value);
    }
    CB_CHECK(!table.next().has_value());
});

CB_TEST("formats.xex.find_a_key", [] {
    const Bytes data = pacman_header();
    const auto info = find_optional_header(data, kFileFormatInfo);   // 0x3FF = 1023
    CB_CHECK(info.has_value());
    CB_CHECK_EQ(info->value, 1344u);
    CB_CHECK(!find_optional_header(data, 0x12345678).has_value());
});

CB_TEST("formats.xex.a_damaged_table_matters_only_past_the_key_wanted", [] {
    Bytes data = pacman_header();
    data.resize(0x18 + 8 * 3 + 4);   // three whole entries, then half of the fourth
    CB_CHECK(find_optional_header(data, 767).has_value());       // the first entry
    CB_CHECK_THROWS_AS(TruncatedError, (void)find_optional_header(data, 0x12345678));
    OptionalHeaders table(data);
    CB_CHECK(table.next() && table.next() && table.next());
    CB_CHECK_THROWS_AS(TruncatedError, (void)table.next());
    CB_CHECK_THROWS_AS(TruncatedError, (void)OptionalHeaders(ByteView(data).first(0x10)));   // no count
});

CB_TEST("formats.xex.ids", [] {
    const TitleIds t = ids(pacman_header());
    CB_CHECK_EQ(t.media_id, 502788917u);
    CB_CHECK_EQ(t.title_id, 1480657015u);
    CB_CHECK_EQ(ids(Bytes{}).title_id, 0u);                       // not an XEX
    CB_CHECK_EQ(ids(Bytes(64, 0)).media_id, 0u);
    Bytes nothing = pacman_header();
    nothing.resize(0x18);                                          // XEX2 with an empty table...
    nothing[0x14] = nothing[0x15] = nothing[0x16] = nothing[0x17] = 0;
    CB_CHECK_EQ(ids(nothing).title_id, 0u);                        // ...has no execution info
    Bytes cut = pacman_header();
    cut.resize(0x18 + 8 * 3);
    CB_CHECK_THROWS_AS(TruncatedError, (void)ids(cut));            // the table runs out before it says so
});

CB_TEST("formats.xex.header_of_a_real_title", [] {
    const Header h = parse_header(pacman_header());
    CB_CHECK(h.name.has_value());
    CB_CHECK_EQ(*h.name, "pacmanCE.pe");
    CB_CHECK_EQ(*h.base, 2181038080u);
    CB_CHECK_EQ(*h.entry, 2181436344u);
    CB_CHECK((h.imports == std::vector<std::string>{"xam.xex", "xboxkrnl.exe"}));
    CB_CHECK_EQ(h.libraries.size(), 12u);
    CB_CHECK_EQ(h.libraries[0].name, "XAPILIB");
    CB_CHECK_EQ(h.libraries[0].version, "2.0.4929.0");
    CB_CHECK_EQ(h.libraries[11].name, "X3DAUD");
});

CB_TEST("formats.xex.not_an_xex_is_a_format_error", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse_header(Bytes{}));
    CB_CHECK_THROWS_AS(FormatError, (void)parse_header(Bytes(0x100, 0)));
    Bytes cut = pacman_header();
    cut.resize(0x40);
    CB_CHECK_THROWS_AS(FormatError, (void)parse_header(cut));
});

CB_TEST("formats.xex.probe", [] {
    fs::ScopedTempDir dir("xex-test");
    fs::write_file(dir.path() / "a.xex", pacman_header());
    fs::write_file(dir.path() / "b.xex", Bytes{'X', 'E', 'X', '1'});
    fs::write_file(dir.path() / "c.xex", Bytes{'X', 'E'});
    CB_CHECK(probe(dir.path() / "a.xex"));
    CB_CHECK(!probe(dir.path() / "b.xex"));
    CB_CHECK(!probe(dir.path() / "c.xex"));
    CB_CHECK(!probe(dir.path() / "missing.xex"));
    CB_CHECK_EQ(read_header(dir.path() / "a.xex").imports.size(), 2u);
});

// A stand-in for the xex program: `extract <in> <out>` copies the file (a batch file; CreateProcess runs it).
Tools fake_tools(const fs::ScopedTempDir& dir, bool working) {
    const std::filesystem::path script = dir.path() / "xex.cmd";
    // (cmd's copy wants backslashes; the program is handed forward slashes, which the real one takes as they are)
    fs::write_text(script,
                   working ? "@echo off\r\nset \"I=%~2\"\r\nset \"O=%~3\"\r\nset \"I=%I:/=\\%\"\r\nset \"O=%O:/=\\%\"\r\n"
                             "if \"%1\"==\"extract\" copy /b \"%I%\" \"%O%\" >nul\r\nexit /b 0\r\n"
                           : "@echo off\r\necho no key for this title 1>&2\r\nexit /b 3\r\n");
    return {script, dir.path() / "unpacked"};
}

CB_TEST("formats.xex.basefile_is_extracted_once_and_named_by_contents", [] {
    fs::ScopedTempDir dir("xex-test");
    const std::filesystem::path xex = dir.path() / "Pac-Man CE.XEX";
    fs::write_file(xex, pacman_header());
    const Tools tools = fake_tools(dir, true);
    const std::filesystem::path image = basefile(tools, xex);
    CB_CHECK(image.parent_path() == tools.unpacked_dir);
    const std::string name = fs::to_text(image.filename());
    CB_CHECK(name.starts_with("pac-man ce-"));
    CB_CHECK(name.ends_with(".exe"));
    CB_CHECK_EQ(name.size(), std::string("pac-man ce-").size() + 12 + 4);
    CB_CHECK(fs::read_file(image) == pacman_header());
    // the facts of the header, beside it
    const auto libs = import_libraries(image);
    CB_CHECK(libs.has_value());
    CB_CHECK((*libs == std::vector<std::string>{"xam.xex", "xboxkrnl.exe"}));
    // a second call finds it
    fs::write_file(image, Bytes{1, 2, 3});
    CB_CHECK(basefile(tools, xex) == image);
    CB_CHECK_EQ(fs::file_size(image), 3u);
    // a changed file is another basefile
    Bytes changed = pacman_header();
    changed[0x1FFF] ^= 0xFF;
    fs::write_file(xex, changed);
    CB_CHECK(basefile(tools, xex) != image);
});

CB_TEST("formats.xex.basefile_failures", [] {
    fs::ScopedTempDir dir("xex-test");
    const std::filesystem::path xex = dir.path() / "a.xex";
    fs::write_file(xex, pacman_header());
    // the program is not there
    CB_CHECK_THROWS_AS(NotFoundError, (void)basefile(Tools{dir.path() / "nope.exe", dir.path() / "u"}, xex));
    // the program fails: its message is carried
    try {
        (void)basefile(fake_tools(dir, false), xex);
        CB_CHECK(false);
    } catch (const ToolError& e) {
        CB_CHECK_EQ(e.tool_exit_code(), 3u);
        CB_CHECK(e.output().find("no key") != std::string::npos);
    }
});

CB_TEST("formats.xex.import_libraries_of_an_image_without_a_sidecar", [] {
    fs::ScopedTempDir dir("xex-test");
    fs::write_file(dir.path() / "x.exe", Bytes{1});
    CB_CHECK(!import_libraries(dir.path() / "x.exe").has_value());
    fs::write_text(dir.path() / "x.exe.json", "not json");
    CB_CHECK(!import_libraries(dir.path() / "x.exe").has_value());
    fs::write_text(dir.path() / "x.exe.json", "{\"imports\": [\"a.xex\"]}");
    CB_CHECK((*import_libraries(dir.path() / "x.exe") == std::vector<std::string>{"a.xex"}));
    fs::write_text(dir.path() / "x.exe.json", "{\"imports\": 5}");
    CB_CHECK(!import_libraries(dir.path() / "x.exe").has_value());
});

CB_TEST("formats.xex.write_basic_puts_the_image_in_one_block", [] {
    fs::ScopedTempDir dir("xex-test");
    const std::filesystem::path xex = dir.path() / "a.xex";
    Bytes file = pacman_header();
    file.insert(file.end(), 100, 0x77);   // some "compressed image" bytes after the header (the fake program copies all of it)
    fs::write_file(xex, file);
    const Tools tools = fake_tools(dir, true);
    const std::filesystem::path out = dir.path() / "basic.xex";
    write_basic(tools, xex, out);
    const Bytes made = fs::read_file(out);
    const Bytes image = fs::read_file(basefile(tools, xex));
    CB_CHECK_EQ(made.size(), 0x2000 + image.size());
    CB_CHECK(Bytes(made.begin() + 0x2000, made.end()) == image);
    // the header is as it was, except the 16 bytes of the file format info
    const std::uint32_t info = 1344;
    for (std::size_t i = 0; i < 0x2000; ++i) {
        if (i >= info && i < info + 16) continue;
        if (made[i] != file[i]) {
            CB_CHECK_EQ(i, std::size_t{0});   // fails, naming the offset
            break;
        }
    }
    io::ByteReader r(made, "test");
    r.seek(info);
    CB_CHECK_EQ(r.u32_be(), 16u);     // size: one block descriptor
    CB_CHECK_EQ(r.u16_be(), 0);       // no zero run
    CB_CHECK_EQ(r.u16_be(), 1);       // one block
    CB_CHECK_EQ(r.u32_be(), image.size());
    CB_CHECK_EQ(r.u32_be(), 0u);
});

CB_TEST("formats.xex.write_basic_errors", [] {
    fs::ScopedTempDir dir("xex-test");
    const Tools tools = fake_tools(dir, true);
    fs::write_file(dir.path() / "junk.xex", Bytes(100, 0));
    CB_CHECK_THROWS_AS(FormatError, write_basic(tools, dir.path() / "junk.xex", dir.path() / "o.xex"));
    // an XEX2 whose table has no file format info
    Bytes bare(0x20, 0);
    bare[0] = 'X'; bare[1] = 'E'; bare[2] = 'X'; bare[3] = '2';
    bare[11] = 0x20;     // data offset
    fs::write_file(dir.path() / "bare.xex", bare);
    CB_CHECK_THROWS_AS(FormatError, write_basic(tools, dir.path() / "bare.xex", dir.path() / "o.xex"));
});

CB_TEST("formats.xex.run_passes_the_arguments", [] {
    fs::ScopedTempDir dir("xex-test");
    fs::write_text(dir.path() / "prog.cmd", "@echo off\r\nexit /b %1\r\n");
    const Tools tools{dir.path() / "prog.cmd", dir.path() / "u"};
    CB_CHECK_EQ(run(tools, {"7"}), 7);
    CB_CHECK_EQ(run(tools, {"0"}), 0);
    CB_CHECK_THROWS_AS(NotFoundError, (void)run(Tools{dir.path() / "none.cmd", {}}, {}));
});

}  // namespace
}  // namespace cb::formats::xex
