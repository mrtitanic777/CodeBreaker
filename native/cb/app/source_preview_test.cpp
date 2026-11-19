// Tests of the Source card's preview (source_preview.hpp): the order of the output folder's files, the syntax colouring and the reading of a file.
#include "cb/app/source_preview.hpp"

#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace cb::app::preview {
namespace {

std::vector<std::string> sp_paths(const std::vector<FileEntry>& files) {
    std::vector<std::string> out;
    for (const FileEntry& f : files) out.push_back(f.path);
    return out;
}

// The text a span covers.
std::string sp_text(const std::string& line, const Span& s) { return line.substr(s.start, s.length); }

CB_TEST("app.source_preview.the_main_source_is_listed_first_then_sources_then_build_scripts", [] {
    fs::ScopedTempDir dir{"cb_preview_list"};
    for (const char* name : {"lz32.inc", "build.cmd", "lz32.asm", "lz32.c", "lz32.def", "README.txt", "lz32.rc"}) fs::write_text(dir.path() / name, "x");
    fs::write_text(dir.path() / "sub" / "helper.c", "x");
    const std::vector<std::string> expected = {"lz32.c", "lz32.asm", "lz32.def", "lz32.inc", "lz32.rc", "sub/helper.c", "build.cmd", "README.txt"};
    CB_CHECK(sp_paths(list_files(dir.path(), "lz32")) == expected);
    CB_CHECK(sp_paths(list_files(dir.path(), "LZ32")) == expected);   // (the stem is matched without regard to case)
    // a translation's main source is its own language's: .cpp before .asm, whatever the other files are
    fs::ScopedTempDir other{"cb_preview_list2"};
    for (const char* name : {"Bank.asm", "Bank.cpp", "cs_runtime.hpp"}) fs::write_text(other.path() / name, "x");
    CB_CHECK(sp_paths(list_files(other.path(), "Bank")) == (std::vector<std::string>{"Bank.cpp", "Bank.asm", "cs_runtime.hpp"}));
    CB_CHECK(list_files(dir.path() / "nowhere", "lz32").empty());
});

CB_TEST("app.source_preview.what_the_main_source_is_called_for_the_Recent_chip", [] {
    CB_CHECK_EQ(chip_for_files({{"a.c", 1}}), "C");
    CB_CHECK_EQ(chip_for_files({{"a.cpp", 1}}), "C++");
    CB_CHECK_EQ(chip_for_files({{"a.asm", 1}}), "Assembly");
    CB_CHECK_EQ(chip_for_files({{"a.cs", 1}}), "C#");
    CB_CHECK_EQ(chip_for_files({{"readme", 1}}), "Source");
    CB_CHECK_EQ(chip_for_files({}), "Source");
});

CB_TEST("app.source_preview.the_language_comes_from_the_extension", [] {
    CB_CHECK(lang_for("lz32.c") == Lang::C && lang_for("lz32.h") == Lang::C);
    CB_CHECK(lang_for("a.cpp") == Lang::Cpp && lang_for("a.hpp") == Lang::Cpp && lang_for("A.CC") == Lang::Cpp);
    CB_CHECK(lang_for("Bank.cs") == Lang::CSharp);
    CB_CHECK(lang_for("lz32.asm") == Lang::Asm && lang_for("x.inc") == Lang::Asm && lang_for("x.s") == Lang::Asm);
    CB_CHECK(lang_for("build.cmd") == Lang::Plain && lang_for("x.py") == Lang::Plain && lang_for("noext") == Lang::Plain);
    CB_CHECK(is_script_file("build.cmd") && is_script_file("Makefile") && is_script_file("notes.txt"));
    CB_CHECK(!is_script_file("lz32.c"));
});

CB_TEST("app.source_preview.C_colours_keywords_numbers_strings_comments_and_directives", [] {
    const std::vector<std::string> lines = {"#include <windows.h>", "    return 0x1F; // done", "    s = \"a\\\"b\"; c = 'x';", "if (n >= 3.5f) {}"};
    const auto spans = highlight(Lang::C, lines);
    CB_CHECK(spans.size() == lines.size());
    // "#include" is one keyword; "<windows.h>" stays plain
    CB_CHECK(spans[0].size() == 1 && spans[0][0].kind == Tok::Keyword && sp_text(lines[0], spans[0][0]) == "#include");
    // return, the number and the comment
    CB_CHECK(spans[1].size() == 3);
    CB_CHECK(spans[1][0].kind == Tok::Keyword && sp_text(lines[1], spans[1][0]) == "return");
    CB_CHECK(spans[1][1].kind == Tok::Number && sp_text(lines[1], spans[1][1]) == "0x1F");
    CB_CHECK(spans[1][2].kind == Tok::Comment && sp_text(lines[1], spans[1][2]) == "// done");
    // a string with an escaped quote is one span; a character literal another
    CB_CHECK(spans[2].size() == 2 && spans[2][0].kind == Tok::String && sp_text(lines[2], spans[2][0]) == "\"a\\\"b\"");
    CB_CHECK(sp_text(lines[2], spans[2][1]) == "'x'");
    // a float with a suffix is one number
    CB_CHECK(spans[3].size() == 2 && spans[3][0].kind == Tok::Keyword && sp_text(lines[3], spans[3][1]) == "3.5f");
    // a keyword inside an identifier is not one
    const auto inner = highlight(Lang::C, {"int_value = interval;"});
    CB_CHECK(inner[0].empty());
});

CB_TEST("app.source_preview.a_block_comment_carries_over_lines_and_C_plus_plus_has_its_own_keywords", [] {
    const std::vector<std::string> lines = {"a /* start", "middle", "end */ b", "class X {};", "x = nullptr;"};
    const auto spans = highlight(Lang::Cpp, lines);
    CB_CHECK(spans[0].size() == 1 && sp_text(lines[0], spans[0][0]) == "/* start");
    CB_CHECK(spans[1].size() == 1 && spans[1][0].kind == Tok::Comment && spans[1][0].length == 6);
    CB_CHECK(spans[2].size() == 1 && sp_text(lines[2], spans[2][0]) == "end */");
    CB_CHECK(spans[3].size() == 1 && sp_text(lines[3], spans[3][0]) == "class");
    CB_CHECK(spans[4].size() == 1 && sp_text(lines[4], spans[4][0]) == "nullptr");
    // plain C does not know "class"
    CB_CHECK(highlight(Lang::C, {"class X;"})[0].empty());
    // C# knows its own
    const auto cs = highlight(Lang::CSharp, {"public static void Main() { var x = null; }"});
    CB_CHECK(cs[0].size() == 5);
});

CB_TEST("app.source_preview.assembly_colours_instructions_registers_numbers_and_comments", [] {
    const std::vector<std::string> lines = {"        mov     eax, 0FFh        ; load", "        db      \"hi\", 0x10", "label:", "$FF"};
    const auto spans = highlight(Lang::Asm, lines);
    CB_CHECK(spans[0].size() == 4);
    CB_CHECK(sp_text(lines[0], spans[0][0]) == "mov" && spans[0][0].kind == Tok::Keyword);
    CB_CHECK(sp_text(lines[0], spans[0][1]) == "eax" && spans[0][1].kind == Tok::Keyword);
    CB_CHECK(sp_text(lines[0], spans[0][2]) == "0FFh" && spans[0][2].kind == Tok::Number);
    CB_CHECK(spans[0][3].kind == Tok::Comment);
    CB_CHECK(spans[1].size() == 3 && spans[1][1].kind == Tok::String && spans[1][2].kind == Tok::Number);
    CB_CHECK(spans[2].empty());   // a label is plain
    CB_CHECK(spans[3].size() == 1 && spans[3][0].kind == Tok::Number);
    // other files are plain
    CB_CHECK(highlight(Lang::Plain, {"return 1; // x"})[0].empty());
});

CB_TEST("app.source_preview.a_file_is_read_as_lines_without_tabs_or_returns", [] {
    const Preview p = make_preview("a\tb\r\nsecond\n\nlast", 18);
    CB_CHECK(p.lines == (std::vector<std::string>{"a    b", "second", "", "last"}));
    CB_CHECK(!p.truncated && !p.binary && p.note.empty());
    // a byte-order mark is not shown, and a final newline is not an empty line
    CB_CHECK(make_preview("\xEF\xBB\xBFx\n", 6).lines == (std::vector<std::string>{"x"}));
    CB_CHECK(make_preview("", 0).lines.empty());
});

CB_TEST("app.source_preview.a_long_file_shows_its_first_part_with_a_note_and_a_binary_one_none", [] {
    const Preview cut = make_preview("one\ntwo\n", 3 * 1024 * 1024);
    CB_CHECK(cut.truncated);
    CB_CHECK(cut.note.find("Showing the first") == 0 && cut.note.find("3.0 MB") != std::string::npos);
    const std::string binary("MZ\0\0\x90", 5);
    const Preview b = make_preview(binary, 12 * 1024);
    CB_CHECK(b.binary && b.lines.empty());
    CB_CHECK_EQ(b.note, "Binary file, 12 KB.");
    CB_CHECK_EQ(size_words(812), "812 bytes");
    CB_CHECK_EQ(size_words(128 * 1024), "128 KB");
    CB_CHECK_EQ(size_words(3584 * 1024), "3.5 MB");
    // from disk: at most kPreviewBytes are read
    fs::ScopedTempDir dir{"cb_preview_read"};
    fs::write_text(dir.path() / "big.c", std::string(kPreviewBytes + 5000, 'x'));
    const Preview big = read_preview(dir.path() / "big.c");
    CB_CHECK(big.truncated && !big.note.empty());
    CB_CHECK(read_preview(dir.path() / "missing.c").note.find("can't be read") != std::string::npos);
});

}  // namespace
}  // namespace cb::app::preview
