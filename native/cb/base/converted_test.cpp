#include "cb/base/converted.hpp"

#include <algorithm>
#include <format>
#include <memory>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::base;
namespace stdfs = std::filesystem;

namespace {

// A clock that ticks: 2026-01-01T00:00:01, :02 ... (what the golden data was made with).
converted::Clock ticking() {
    auto n = std::make_shared<int>(0);
    return [n] { return std::format("2026-01-01T00:00:{:02}", ++*n); };
}

std::string native(const stdfs::path& p) {
    std::string s = fs::to_text(p);
    std::replace(s.begin(), s.end(), '/', '\\');
    return s;
}

// What the golden files were made from: the places a run used are <IN> and <OUT>, as plain text and as JSON text.
std::string masked(std::string text, const stdfs::path& in, const stdfs::path& out) {
    const auto doubled = [](const std::string& s) { return text::replace_all(s, "\\", "\\\\"); };
    text = text::replace_all(text, doubled(native(in)), "<IN>");
    text = text::replace_all(text, native(in), "<IN>");
    text = text::replace_all(text, doubled(native(out)), "<OUT>");
    return text::replace_all(text, native(out), "<OUT>");
}

std::string golden(std::string_view name) { return fs::read_text(testing::data_path(std::string("base/testdata/converted/") + std::string(name))); }

Bytes bytes_of(std::string_view s) { return to_bytes(s); }

}  // namespace

CB_TEST("base.converted.clean_name", [] {
    // from the cases of the Python's harness
    CB_CHECK_EQ(converted::clean_name(""), "binary");
    CB_CHECK_EQ(converted::clean_name("x"), "x");
    CB_CHECK_EQ(converted::clean_name("hello world"), "hello_world");
    CB_CHECK_EQ(converted::clean_name("a/b\\c"), "a_b_c");
    CB_CHECK_EQ(converted::clean_name("..."), "binary");
    CB_CHECK_EQ(converted::clean_name("._-"), "-");
    CB_CHECK_EQ(converted::clean_name("a..b"), "a..b");
    CB_CHECK_EQ(converted::clean_name(" lead"), "lead");
    CB_CHECK_EQ(converted::clean_name("tail "), "tail");
    CB_CHECK_EQ(converted::clean_name("a  b"), "a_b");
    CB_CHECK_EQ(converted::clean_name("!!!"), "binary");
    CB_CHECK_EQ(converted::clean_name("ok_name-1.2"), "ok_name-1.2");
    CB_CHECK_EQ(converted::clean_name("____"), "binary");
    CB_CHECK_EQ(converted::clean_name(".-.-"), "-.-");
    CB_CHECK_EQ(converted::clean_name("tab\there"), "tab_here");
    CB_CHECK_EQ(converted::clean_name("xxx\ny"), "xxx_y");
    // letters outside ASCII are runs of other bytes: one '_', which an end then cuts
    CB_CHECK_EQ(converted::clean_name("\xC3\x9C" "n\xC3\xAF"), "n");
    CB_CHECK_EQ(converted::clean_name("\xC4\x80\xC4\x81z"), "z");
    CB_CHECK_EQ(converted::clean_name("a\xC3\x9C" "b"), "a_b");
});

CB_TEST("base.converted.sha1_of_streams_the_file", [] {
    fs::ScopedTempDir dir("cbconv");
    fs::write_file(dir.path() / "empty.bin", {});
    fs::write_file(dir.path() / "abc.bin", bytes_of("abc"));
    CB_CHECK_EQ(converted::sha1_of(dir.path() / "empty.bin"), "da39a3ee5e6b4b0d3255bfef95601890afd80709");
    CB_CHECK_EQ(converted::sha1_of(dir.path() / "abc.bin"), "a9993e364706816aba3e25717850c26c9cd0d89d");
    // more than one block of the stream: the digest of a megabyte and a half of zeros
    Bytes big(3u << 19, 0);
    fs::write_file(dir.path() / "big.bin", big);
    CB_CHECK_EQ(converted::sha1_of(dir.path() / "big.bin"), "95823c334fce55968e8d2827ccd1cf77cee19abd");
    CB_CHECK_THROWS_AS(IoError, converted::sha1_of(dir.path() / "missing.bin"));
});

CB_TEST("base.converted.kind_of_says_what_detection_says", [] {
    CB_CHECK_EQ(converted::kind_of(testing::data_path("formats/testdata/lz32.dll")), "PE executable");
    CB_CHECK_EQ(converted::kind_of(testing::data_path("formats/testdata/bios.vxd")), "virtual device driver (LE)");
    CB_CHECK_EQ(converted::kind_of(testing::data_path("formats/testdata/setver.exe")), "DOS program (MZ)");
    CB_CHECK_EQ(converted::kind_of(testing::data_path("formats/testdata/choice.com")), "");   // not recognised
    CB_CHECK_EQ(converted::kind_of(testing::data_path("formats/testdata/no_such_file.bin")), "");
});

CB_TEST("base.converted.default_name_is_the_file_name_unless_generic", [] {
    fs::ScopedTempDir dir("cbconv");
    const auto put = [&](const char* rel, std::string_view content) {
        const stdfs::path p = dir.path() / rel;
        fs::write_file(p, bytes_of(content));
        return p;
    };
    CB_CHECK_EQ(converted::default_name(put("a/hello.bin", "x")), "hello");
    CB_CHECK_EQ(converted::default_name(put("My Game! (v2).exe", "x")), "My_Game_v2");
    CB_CHECK_EQ(converted::default_name(put("a.b.c.exe", "x")), "a.b.c");
    // a generic name (any case) takes its folder's
    CB_CHECK_EQ(converted::default_name(put("Fancy Installer/setup.exe", "x")), "Fancy_Installer");
    CB_CHECK_EQ(converted::default_name(put("upper/Setup.EXE", "x")), "upper");
    CB_CHECK_EQ(converted::default_name(put("zork_folder/default.bin", "x")), "zork_folder");
    CB_CHECK_EQ(converted::default_name(put("main/main.dat", "x")), "main");
});

CB_TEST("base.converted.default_name_of_a_generic_xex_is_the_name_its_header_gives", [] {
    fs::ScopedTempDir dir("cbconv");
    const Bytes head = fs::read_file(testing::data_path("formats/testdata/pacman_ce.xex.head"));
    fs::write_file(dir.path() / "xexgame" / "default.xex", head);
    CB_CHECK_EQ(converted::default_name(dir.path() / "xexgame" / "default.xex"), "pacmanCE");   // header: pacmanCE.pe
    // not a generic name: the file's own, whatever the header says
    fs::write_file(dir.path() / "xexgame" / "pac.xex", head);
    CB_CHECK_EQ(converted::default_name(dir.path() / "xexgame" / "pac.xex"), "pac");
    // a generic name whose content is no XEX takes the folder's
    fs::write_file(dir.path() / "other" / "default.xex", bytes_of("not an xex"));
    CB_CHECK_EQ(converted::default_name(dir.path() / "other" / "default.xex"), "other");
    // an XEX2 magic and nothing more names nothing
    fs::write_file(dir.path() / "cut" / "default.xex", bytes_of("XEX2"));
    CB_CHECK_EQ(converted::default_name(dir.path() / "cut" / "default.xex"), "cut");
});

CB_TEST("base.converted.a_project_is_made_once_and_recorded_like_the_python_did", [] {
    fs::ScopedTempDir temp("cbconv");
    const stdfs::path out = temp.path();
    const stdfs::path in = out / "in";
    const stdfs::path game = in / "My Game! (v2).exe";
    fs::write_file(game, bytes_of(std::string_view("MZ\x90\x00 hello, converted\n", 22)));
    converted::Store store(out / "Converted", ticking());

    converted::Project p = store.project(game);
    CB_CHECK_EQ(p.name(), "My_Game_v2");
    for (const std::string_view sub : converted::kFolders) CB_CHECK(stdfs::is_directory(p.root() / sub));
    CB_CHECK(fs::read_file(p.original() / "My Game! (v2).exe") == fs::read_file(game));
    CB_CHECK(p.original_file().has_value());
    CB_CHECK(fs::same_path(*p.original_file(), p.original() / "My Game! (v2).exe"));

    p.record("decompile", {p.src() / "game.c", p.reports(), out / "elsewhere" / "report.txt"}, "build: wcc -zq game.c\nrun: game.exe");
    p.record("recompile", {p.build() / "game.exe"});
    p.record("analyze", {}, "no outputs, only a note");

    // the same bytes again are the same project (and nothing is stamped)
    const converted::Project again = store.project(game);
    CB_CHECK(fs::same_path(again.root(), p.root()));

    // the same name with other bytes is <name>_2
    const stdfs::path other = in / "other" / "My Game! (v2).exe";
    fs::write_file(other, bytes_of("different bytes"));
    const converted::Project second = store.project(other);
    CB_CHECK_EQ(second.name(), "My_Game_v2_2");

    // the files are the text the Python wrote for the same run (golden data made by the Python; <IN>/<OUT> stand for the places)
    CB_CHECK_EQ(masked(fs::read_text(p.manifest_path()), in, out), golden("game.project.json"));
    CB_CHECK_EQ(masked(fs::read_text(p.root() / "README.txt"), in, out), golden("game.README.txt"));
    CB_CHECK_EQ(masked(fs::read_text(second.manifest_path()), in, out), golden("game2.project.json"));
    CB_CHECK_EQ(masked(fs::read_text(second.root() / "README.txt"), in, out), golden("game2.README.txt"));
});

CB_TEST("base.converted.the_manifest_reads_back_typed", [] {
    fs::ScopedTempDir temp("cbconv");
    const stdfs::path game = temp.path() / "hello.bin";
    fs::write_file(game, bytes_of("hello world"));
    converted::Store store(temp.path() / "Converted", ticking());
    const converted::Project p = store.project(game);
    p.record("decompile", {p.src() / "a.c"}, "n");
    const converted::Manifest m = p.manifest();
    CB_CHECK_EQ(m.name.value_or("?"), "hello");
    CB_CHECK_EQ(m.file.value_or("?"), "hello.bin");
    CB_CHECK_EQ(m.sha1.value_or("?"), "2aae6c35c94fcfb415dbe95f408b9ce91ee846ed");
    CB_CHECK_EQ(m.size.value_or(0), std::uint64_t{11});
    CB_CHECK_EQ(m.kind.value_or("?"), "");
    CB_CHECK_EQ(m.steps.size(), std::size_t{1});
    CB_CHECK_EQ(m.steps[0].command, "decompile");
    CB_CHECK_EQ(m.steps[0].when, "2026-01-01T00:00:01");
    CB_CHECK_EQ(m.steps[0].outputs.size(), std::size_t{1});
    CB_CHECK_EQ(m.steps[0].outputs[0], "src\\a.c");
    CB_CHECK_EQ(m.steps[0].notes, "n");
});

CB_TEST("base.converted.recording_a_command_again_replaces_its_step", [] {
    fs::ScopedTempDir temp("cbconv");
    const stdfs::path game = temp.path() / "hello.bin";
    fs::write_file(game, bytes_of("hello world"));
    converted::Store store(temp.path() / "Converted", ticking());
    const converted::Project p = store.project(game);
    p.record("decompile", {p.src() / "a.c"}, "first");
    p.record("recompile");
    p.record("decompile", {p.src() / "b.c"}, "second");
    const converted::Manifest m = p.manifest();
    CB_CHECK_EQ(m.steps.size(), std::size_t{2});
    CB_CHECK_EQ(m.steps[0].command, "recompile");
    CB_CHECK_EQ(m.steps[1].command, "decompile");
    CB_CHECK_EQ(m.steps[1].notes, "second");
    CB_CHECK_EQ(m.steps[1].outputs[0], "src\\b.c");
    CB_CHECK_EQ(m.steps[1].when, "2026-01-01T00:00:03");
});

CB_TEST("base.converted.an_output_beside_the_project_is_not_inside_it", [] {
    // DIVERGES: the Python tested the folder as a text prefix, so Converted/foo_2/x counted as inside Converted/foo and was
    // recorded as ..\foo_2\x; it is recorded as the absolute path it is
    fs::ScopedTempDir temp("cbconv");
    const stdfs::path game = temp.path() / "foo.bin";
    fs::write_file(game, bytes_of("one"));
    converted::Store store(temp.path() / "Converted", ticking());
    const converted::Project p = store.project(game);
    const stdfs::path sibling = p.root().parent_path() / (p.name() + "_2") / "x.txt";
    p.record("c", {sibling, p.root() / "assets" / "deep" / "y.txt", p.root()});
    const converted::Manifest m = p.manifest();
    CB_CHECK_EQ(m.steps[0].outputs.size(), std::size_t{3});
    CB_CHECK_EQ(m.steps[0].outputs[0], native(stdfs::absolute(sibling).lexically_normal()));
    CB_CHECK_EQ(m.steps[0].outputs[1], "assets\\deep\\y.txt");
    CB_CHECK_EQ(m.steps[0].outputs[2], ".");   // the folder itself
});

CB_TEST("base.converted.a_named_project_and_another_base_folder", [] {
    fs::ScopedTempDir temp("cbconv");
    const stdfs::path game = temp.path() / "hello.bin";
    fs::write_file(game, bytes_of("hello world"));
    converted::Store store(temp.path() / "Converted", ticking());
    const converted::Project a = store.project(game, "My Name!");
    CB_CHECK_EQ(a.name(), "My_Name");
    // found again by the bytes alone or with the same name; another name makes another folder
    CB_CHECK(fs::same_path(store.project(game).root(), a.root()));
    CB_CHECK_EQ(store.project(game, "My Name!").name(), "My_Name");
    CB_CHECK_EQ(store.project(game, "Else").name(), "Else");
    // another base folder is a world of its own
    const stdfs::path base = temp.path() / "elsewhere";
    const converted::Project b = store.project(game, std::nullopt, base);
    CB_CHECK(fs::same_path(b.root(), base / "hello"));
    CB_CHECK(stdfs::is_directory(base / "hello" / "original"));
});

CB_TEST("base.converted.a_damaged_manifest_is_an_empty_one_and_unknown_keys_survive", [] {
    fs::ScopedTempDir temp("cbconv");
    const stdfs::path folder = temp.path() / "Converted" / "hand";
    converted::Project p(folder, ticking());   // the folders only; no project.json yet
    CB_CHECK(!stdfs::exists(p.manifest_path()));
    CB_CHECK(p.manifest().steps.empty());
    CB_CHECK(!p.original_file().has_value());

    fs::write_text(p.manifest_path(), "{ this is not json");
    CB_CHECK(!p.manifest().name.has_value());
    fs::write_text(p.manifest_path(), "[1, 2]");
    CB_CHECK(!p.manifest().name.has_value());

    // a hand-made one: a key we do not know, a step with a key we do not know, a step that is no object, a wrong-typed size
    fs::write_text(p.manifest_path(),
                   R"({"name": "hand", "file": "x.bin", "size": "big", "owner": {"who": "me", "n": [1, 2]},
                       "steps": [{"command": "old", "when": "t", "outputs": [], "flags": 3}, 7]})");
    const converted::Manifest m = p.manifest();
    CB_CHECK_EQ(m.name.value_or("?"), "hand");
    CB_CHECK(!m.size.has_value());   // not an integer: kept as it was, not read
    CB_CHECK_EQ(m.steps.size(), std::size_t{1});   // DIVERGES: a step that is no object is dropped (the Python raised AttributeError)
    CB_CHECK(fs::same_path(*p.original_file(), p.original() / "x.bin"));

    p.record("new", {}, "n");
    const std::string text = fs::read_text(p.manifest_path());
    CB_CHECK(text.find("\"owner\"") != std::string::npos);
    CB_CHECK(text.find("\"who\": \"me\"") != std::string::npos);
    CB_CHECK(text.find("\"size\": \"big\"") != std::string::npos);
    CB_CHECK(text.find("\"flags\": 3") != std::string::npos);
    const converted::Manifest again = p.manifest();
    CB_CHECK_EQ(again.steps.size(), std::size_t{2});
    CB_CHECK_EQ(again.steps[0].command, "old");
    CB_CHECK_EQ(again.steps[1].command, "new");
    // the README names what is known, and says "?" and "unknown" for what is not
    const std::string readme = fs::read_text(p.root() / "README.txt");
    CB_CHECK(readme.starts_with("hand\r\n====\r\n\r\nWhat CodeBreaker made of x.bin.\r\n"));
    CB_CHECK(readme.find("  Kind:    unknown\r\n  Size:    ? bytes\r\n") != std::string::npos);
});

CB_TEST("base.converted.text_that_is_not_ascii_is_kept_in_the_manifest", [] {
    fs::ScopedTempDir temp("cbconv");
    const stdfs::path game = temp.path() / "hello.bin";
    fs::write_file(game, bytes_of("x"));
    converted::Store store(temp.path() / "Converted", ticking());
    const converted::Project p = store.project(game);
    p.record("c", {}, "caf\xC3\xA9 \xE2\x82\xAC");   // as the Python's ensure_ascii wrote it: é €
    CB_CHECK(fs::read_text(p.manifest_path()).find("caf\\u00e9 \\u20ac") != std::string::npos);
    CB_CHECK_EQ(p.manifest().steps[0].notes, "caf\xC3\xA9 \xE2\x82\xAC");
    CB_CHECK(fs::read_text(p.root() / "README.txt").find("    caf\xC3\xA9 \xE2\x82\xAC\r\n") != std::string::npos);
});

CB_TEST("base.converted.errors", [] {
    fs::ScopedTempDir temp("cbconv");
    converted::Store store(temp.path() / "Converted", ticking());
    CB_CHECK_THROWS_AS(IoError, store.project(temp.path() / "missing.bin"));
    CB_CHECK(!stdfs::exists(temp.path() / "Converted" / "missing"));   // nothing is made for a file that is not there
});

CB_TEST("base.converted.the_clock_is_local_time_to_the_second", [] {
    const std::string now = converted::local_time_now();
    CB_CHECK_EQ(now.size(), std::size_t{19});
    CB_CHECK_EQ(now[4], '-');
    CB_CHECK_EQ(now[10], 'T');
    CB_CHECK_EQ(now[13], ':');
});
