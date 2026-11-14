// Tests of build/resources: the pieces of an .rc statement, the statements of every kind of resource on real
// templates, the .rc and payload files of real 16-bit and 32-bit modules, reading the compiler's .res layout, comparing
// two sets of resources, and -- when OpenWatcom is named in CB_OPENWATCOM -- the whole round trip through wrc.
//
// What an old harness (recorded outputs on 28 Windows 95 modules)
// established, and these tests reproduce: the .rc and every payload file of 25 of the 28 modules byte for byte (the first
// comment line aside); the statements of 669 dialogs, menus, accelerator tables and version blocks at the three text
// modes (655 identical, the others as below); the counts of 14 end-to-end rebuilds (identical, equivalent, written as
// text, as files, as payloads) and the sizes of their .res files (12 identical, 2 as below); the compare() verdicts on
// mutated copies of the modules.
//
// Notes:
//   * A 32-bit string whose UTF-16 ends in an odd byte (three Windows 95 modules have such a version block, "Microsoft
//     C" with half a character behind it) reads as the text before the odd byte; the
//     text stays Latin-1 and the .rc is written with octal escapes (3 of 28 modules; 14 statements of 669).
//   * read_res(): a 16-bit entry whose body runs past the end of the file is a TruncatedError.
//     A name with no terminator in a 16-bit .res is likewise a TruncatedError. (The 32-bit layout keeps its one clamp -- the last body pushed up to three bytes past the end by the compiler's alignment -- which the compiler's real output needs.)
//   * write_rc(): a resource whose own name has no source form (a zero character in it) is a NoSourceForm error, and
//     rebuild() reports it as `error`.
//   * A lone surrogate cannot reach quoted(): the readers decode text to valid UTF-8 (U+FFFD for the damage), so a
//     surrogate never occurs there.
//   * Payload files are named from the plan in memory: one that another resource of the module has taken with other
//     bytes is numbered. Files left by an earlier round are overwritten, not kept under a new name.
#include "cb/build/resources.hpp"

#include <algorithm>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

namespace br = cb::build::resources;
namespace fr = cb::formats::resources;
namespace stdfs = std::filesystem;
using br::Key;
using br::Text;
using cb::Bytes;

stdfs::path module(const char* name) { return cb::testing::data_path(std::string("formats/testdata/") + name); }

fr::Resource template_of(const char* file, std::uint32_t type, std::uint32_t name) {
    fr::Resource r;
    r.type = type;
    r.name = name;
    r.language = 1033;
    r.data = cb::fs::read_file(cb::testing::data_path(std::string("formats/testdata/resources/") + file));
    return r;
}

Key key(std::uint32_t type, std::uint32_t name, std::optional<std::uint32_t> language = std::nullopt) {
    return {type, name, language};
}
Key key(std::uint32_t type, const char* name, std::optional<std::uint32_t> language = std::nullopt) {
    return {type, std::string(name), language};
}

bool contains(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

std::string text_of(const cb::Result<std::vector<std::string>>& r) { return cb::text::join(r.value(), "\n"); }

// ---- the pieces of a statement ------------------------------------------------------------------------------------

CB_TEST("build.resources.quoted_writes_the_escapes_the_compiler_reads_back", [] {
    CB_CHECK_EQ(br::quoted("", Text::Wide).value(), std::string("\"\""));
    CB_CHECK_EQ(br::quoted("plain", Text::Narrow).value(), std::string("\"plain\""));
    CB_CHECK_EQ(br::quoted("say \"hi\"", Text::Wide).value(), std::string("\"say \"\"hi\"\"\""));       // a quote is doubled
    CB_CHECK_EQ(br::quoted("back\\slash", Text::Wide).value(), std::string("\"back\\\\slash\""));
    CB_CHECK_EQ(br::quoted("tab\there\nline\rend", Text::Wide).value(), std::string("\"tab\\there\\nline\\rend\""));
    // control characters and DEL are three-digit octal, a fixed width so a digit after one cannot join it
    CB_CHECK_EQ(br::quoted("\x01\x1f\x7f", Text::Wide).value(), std::string("\"\\001\\037\\177\""));
    CB_CHECK_EQ(br::quoted("digit after \x01" "1", Text::Narrow).value(), std::string("\"digit after \\0011\""));
    CB_CHECK_EQ(br::quoted("a\"\"b\\\\c", Text::Narrow).value(), std::string("\"a\"\"\"\"b\\\\\\\\c\""));
    CB_CHECK_EQ(br::quoted(std::string(300, 'x'), Text::Wide).value().size(), std::size_t{302});
});

CB_TEST("build.resources.quoted_reaches_latin1_by_octal_and_beyond_it_by_utf8", [] {
    const std::string cafe = "caf\xC3\xA9 \xC3\xBF";   // "café ÿ"
    CB_CHECK_EQ(br::quoted(cafe, Text::Narrow).value(), std::string("\"caf\\351 \\377\""));
    CB_CHECK_EQ(br::quoted(cafe, Text::Wide).value(), std::string("\"caf\\351 \\377\""));
    CB_CHECK_EQ(br::quoted(cafe, Text::Unicode).value(), "\"" + cafe + "\"");   // the source is UTF-8 there
    CB_CHECK_EQ(br::quoted("\xC2\x80\xC2\x9F\xC2\xA0", Text::Wide).value(), std::string("\"\\200\\237\\240\""));
    // beyond Latin-1: Wide and Narrow cannot say it (write_rc starts over in UTF-8), Unicode can
    for (const char* beyond : {"\xC4\x80 beyond", "\xE2\x82\xAC euro", "snow \xE2\x98\x83 man", "\xF0\x9F\x98\x80 face"}) {
        for (Text mode : {Text::Narrow, Text::Wide}) {
            const auto r = br::quoted(beyond, mode);
            CB_CHECK(!r.ok());
            CB_CHECK_EQ(std::string(r.error().kind()), std::string("BeyondLatin1"));
        }
        CB_CHECK_EQ(br::quoted(beyond, Text::Unicode).value(), "\"" + std::string(beyond) + "\"");
    }
});

CB_TEST("build.resources.quoted_refuses_what_the_compiler_cannot_take", [] {
    // a string ends at a zero character, however it is written: no source form in any mode
    for (Text mode : {Text::Narrow, Text::Wide, Text::Unicode}) {
        const auto r = br::quoted(std::string("zero\0inside", 11), mode);
        CB_CHECK(!r.ok());
        CB_CHECK_EQ(std::string(r.error().kind()), std::string("NoSourceForm"));
    }
    // the compiler counts a string table's UTF-8 strings in bytes, not characters: beyond ASCII, no source form there
    const auto counted = br::quoted("\xE2\x82\xAC", Text::Unicode, true);
    CB_CHECK(!counted.ok());
    CB_CHECK_EQ(std::string(counted.error().kind()), std::string("NoSourceForm"));
    CB_CHECK_EQ(br::quoted("ascii only", Text::Unicode, true).value(), std::string("\"ascii only\""));
    CB_CHECK_EQ(br::quoted("caf\xC3\xA9", Text::Wide, true).value(), std::string("\"caf\\351\""));   // octal is counted right
});

// ---- the statements of the kinds that have a source form ----------------------------------------------------------

CB_TEST("build.resources.a_dialog_is_written_as_a_dialog_statement", [] {
    const fr::Resource r = template_of("shell32_dialog_1069.bin", fr::kDialog, 1069);
    const std::string expected =
        "1069 DIALOG 0, 0, 188, 52\n"
        "STYLE 0x80C800C4L\n"
        "CAPTION \"Computer Name\"\n"
        "FONT 8, \"MS Sans Serif\"\n"
        "BEGIN\n"
        "  CONTROL \"&Named:\", 12810, \"STATIC\", 0x5002000CL, 6, 8, 25, 8\n"
        "  CONTROL \"\", 14096, \"COMBOBOX\", 0x50210042L, 38, 7, 150, 90\n"
        "END";
    CB_CHECK_EQ(text_of(br::dialog_text(r, Text::Wide)), expected);
    CB_CHECK_EQ(text_of(br::dialog_text(r, Text::Unicode)), expected);
});

CB_TEST("build.resources.a_dialog_with_a_menu_and_no_controls", [] {
    const fr::Resource r = template_of("shell32_dialog_1078.bin", fr::kDialog, 1078);
    CB_CHECK_EQ(text_of(br::dialog_text(r, Text::Wide)),
                std::string("1078 DIALOG 0, 0, 400, 250\nSTYLE 0x92CF0044L\nMENU 357\nFONT 8, \"MS Sans Serif\"\nBEGIN\nEND"));
});

CB_TEST("build.resources.a_dialog_with_text_beyond_latin1_needs_the_utf8_source", [] {
    const fr::Resource r = template_of("comctl32_dialog_1006.bin", fr::kDialog, 1006);
    const auto wide = br::dialog_text(r, Text::Wide);
    CB_CHECK(!wide.ok());
    CB_CHECK_EQ(std::string(wide.error().kind()), std::string("BeyondLatin1"));
    const std::string unicode = text_of(br::dialog_text(r, Text::Unicode));
    CB_CHECK(contains(unicode, "FONT 9, \"\xE6\x96\xB0\xE7\xB4\xB0\xE6\x98\x8E\xE9\xAB\x94\""));   // 新細明體
    CB_CHECK(contains(unicode, "  CONTROL \"\xE7\xA2\xBA\xE5\xAE\x9A\", 1, \"BUTTON\", 0x50030001L, 4, 122, 50, 14"));
    // a control class that is not a standard atom is written as its name
    CB_CHECK(contains(unicode, "\"SysTabControl32\", 0x50030200L, 4, 4, 212, 114"));
});

CB_TEST("build.resources.an_extended_menu_is_written_with_its_types_and_states", [] {
    const fr::Resource r = template_of("shell32_menu_226_ext.bin", fr::kMenu, 226);
    CB_CHECK_EQ(text_of(br::menu_text(r, Text::Wide)),
                std::string("226 MENUEX\nBEGIN\n  POPUP \"\", 0, 0x0L, 0x0L, 0\n  BEGIN\n    MENUITEM \"R&estore\", 112, 0x0L, 0x0L\n  END\nEND"));
});

CB_TEST("build.resources.an_accelerator_table_names_its_keys_and_options", [] {
    const fr::Resource r = template_of("shell32_accelerator_2.bin", fr::kAccelerator, 2);
    CB_CHECK_EQ(text_of(br::accelerators_text(r, Text::Wide)),
                std::string("2 ACCELERATORS\nBEGIN\n  116, 142, VIRTKEY, NOINVERT\n  46, 132, VIRTKEY, NOINVERT\n"
                            "  46, 132, VIRTKEY, NOINVERT, SHIFT\nEND"));
});

CB_TEST("build.resources.a_version_block_is_written_in_utf8_when_its_text_needs_it", [] {
    const fr::Resource r = template_of("comctl32_version_1.bin", fr::kVersion, 1);
    CB_CHECK(!br::version_text(r, Text::Wide).ok());   // Chinese text: beyond Latin-1
    const std::string unicode = text_of(br::version_text(r, Text::Unicode));
    CB_CHECK(unicode.starts_with("1 VERSIONINFO\nFILEVERSION 4, 70, 0, 1146\nPRODUCTVERSION 4, 70, 0, 1146\nFILEFLAGSMASK 0x3FL\n"));
    CB_CHECK(contains(unicode, "    BLOCK \"080404B0\"\n"));
    CB_CHECK(contains(unicode, "      VALUE \"InternalName\", \"COMMCTRL\"\n"));
    CB_CHECK(contains(unicode, "    VALUE \"Translation\", 0x804, 0x4B0\n"));
    CB_CHECK(unicode.ends_with("  END\nEND"));
});

CB_TEST("build.resources.a_16_bit_version_block_has_no_source_form", [] {
    // the compiler writes a 16-bit block with lengths that do not add up: it is carried as it is
    fr::Resource r = template_of("comctl32_version_1.bin", fr::kVersion, 1);
    const auto narrow = br::version_text(r, Text::Narrow);
    CB_CHECK(!narrow.ok());
    CB_CHECK_EQ(std::string(narrow.error().kind()), std::string("NoSourceForm"));
});

CB_TEST("build.resources.damaged_bytes_are_a_format_error_not_a_crash", [] {
    fr::Resource r = template_of("shell32_dialog_1069.bin", fr::kDialog, 1069);
    r.data.resize(20);   // cut inside the template header
    const auto cut = br::dialog_text(r, Text::Wide);
    CB_CHECK(!cut.ok());
    CB_CHECK_THROWS_AS(cb::FormatError, (void)cut.value());
    fr::Resource m = template_of("shell32_menu_226_ext.bin", fr::kMenu, 226);
    m.data = {0xFF, 0xFF, 0, 0};   // a template version no one wrote
    CB_CHECK(!br::menu_text(m, Text::Wide).ok());
    fr::Resource a = template_of("shell32_accelerator_2.bin", fr::kAccelerator, 2);
    a.data.assign(3, 0);
    CB_CHECK_EQ(text_of(br::accelerators_text(a, Text::Wide)), std::string("2 ACCELERATORS\nBEGIN\nEND"));   // no whole entry
});

// ---- writing the .rc ---------------------------------------------------------------------------------------------------

CB_TEST("build.resources.the_rc_of_a_16_bit_module_and_its_payload_files", [] {
    cb::fs::ScopedTempDir dir("cb-res");
    const br::Module m = br::read_module(module("charmap.exe"));
    CB_CHECK(m.text == Text::Narrow);
    CB_CHECK_EQ(m.resources.size(), std::size_t{8});
    const br::RcFiles rc = br::write_rc(m, dir.path());
    CB_CHECK(rc.text == Text::Narrow);
    CB_CHECK(rc.rc == dir.path() / "charmap.rc");
    const std::string expected =
        "/* charmap.rc -- the resources of charmap.exe. Generated by `codebreaker resources`. */\n"
        "111 ICON PRELOAD MOVEABLE DISCARDABLE IMPURE \"res/group_icon_111.ico\"\n"
        "110 BITMAP LOADONCALL MOVEABLE PURE \"res/bitmap_110.bmp\"\n"
        "\"CHARMAP\" DIALOG PRELOAD MOVEABLE IMPURE 9, 33, 355, 124\n"
        "STYLE 0x84CA0044L\n"
        "CAPTION \"Character Map\"\n"
        "CLASS \"MyDlgClass\"\n"
        "FONT 8, \"MS SANS SERIF\"\n"
        "BEGIN\n"
        "  CONTROL \"&Font:\", 110, \"STATIC\", 0x50000000L, 7, 9, 17, 8\n"
        "  CONTROL \"\", 105, \"COMBOBOX\", 0x50210313L, 32, 7, 114, 67\n"
        "  CONTROL \"Ch&aracters to copy:\", 109, \"STATIC\", 0x50000000L, 155, 9, 65, 8\n"
        "  CONTROL \"\", 104, \"EDIT\", 0x50810180L, 223, 7, 66, 14\n"
        "  CONTROL \"Close\", 101, \"BUTTON\", 0x50010000L, 298, 7, 50, 14\n"
        "  CONTROL \"&Select\", 103, \"BUTTON\", 0x50010001L, 298, 25, 50, 14\n"
        "  CONTROL \"&Copy\", 102, \"BUTTON\", 0x50010000L, 298, 43, 50, 14\n"
        "END\n"
        "1 16 LOADONCALL MOVEABLE PURE \"res/version_1.bin\"\n"
        "STRINGTABLE PRELOAD MOVEABLE DISCARDABLE PURE\n"
        "BEGIN\n"
        "  96, \"&What's This?\"\n"
        "  97, \"Character Map\"\n"
        "  98, \"&Help Topics\"\n"
        "  99, \"&About Character Map\"\n"
        "  101, \"Closes the Character Map window.\"\n"
        "  102, \"Copies contents of the Characters to Copy box to the Clipboard.\"\n"
        "  103, \"Places selected character in the Characters to Copy box.\"\n"
        "  104, \"Accumulates characters to be copied to the Clipboard.\"\n"
        "  105, \"Lists fonts available for character selection.\"\n"
        "  108, \"Shows available characters in the selected font.\"\n"
        "  112, \"windows.hlp>proc4\"\n"
        "  113, \"Rich Text Format\"\n"
        "  114, \"Keystroke: \"\n"
        "  115, \"Spacebar\"\n"
        "  116, \"Ctrl+\"\n"
        "  117, \"Ctrl+Alt+\"\n"
        "  118, \"Shift+Ctrl+Alt+\"\n"
        "  119, \"Alt+0\"\n"
        "END\n";
    CB_CHECK_EQ(cb::fs::read_text(rc.rc), expected);
    // an icon group and its two member images are one .ico; a bitmap one .bmp; the version block is carried as it is
    CB_CHECK_EQ(cb::fs::read_file(dir.path() / "res/group_icon_111.ico").size(), std::size_t{1094});
    CB_CHECK_EQ(cb::fs::read_file(dir.path() / "res/bitmap_110.bmp").size(), std::size_t{606});   // (the file header is added)
    CB_CHECK_EQ(cb::fs::read_file(dir.path() / "res/version_1.bin").size(), std::size_t{496});
    CB_CHECK_EQ(rc.how.text.size(), std::size_t{3});     // the dialog and the two string tables
    CB_CHECK_EQ(rc.how.files.size(), std::size_t{4});    // group + its 2 members, bitmap
    CB_CHECK_EQ(rc.how.fallback.size(), std::size_t{1}); // the version block
    CB_CHECK_EQ(rc.how.opaque.size(), std::size_t{0});
});

CB_TEST("build.resources.the_rc_of_a_32_bit_module_names_its_language_and_merges_its_string_tables", [] {
    cb::fs::ScopedTempDir dir("cb-res");
    const br::Module m = br::read_module(module("xcopy32.exe"));
    CB_CHECK(m.text == Text::Wide);
    const br::RcFiles rc = br::write_rc(m, dir.path());
    CB_CHECK(rc.text == Text::Wide);
    const std::string text = cb::fs::read_text(rc.rc);
    CB_CHECK(text.starts_with("/* xcopy32.rc -- the resources of xcopy32.exe. Generated by `codebreaker resources`. */\n"
                              "LANGUAGE 0x09, 0x01\n1 VERSIONINFO\nFILEVERSION 4, 0, 0, 1111\n"));
    CB_CHECK(contains(text, "      VALUE \"LegalCopyright\", \"Copyright \\251 Microsoft Corp. 1994-1995\"\n"));
    CB_CHECK(contains(text, "      VALUE \"ProductName\", \"Microsoft\\256 Windows\\256 Operating System\"\n"));
    CB_CHECK(contains(text, "    VALUE \"Translation\", 0x409, 0x4E4\n"));
    // two string resources of one language make one STRINGTABLE (the compiler numbers the 16 strings of each by id)
    CB_CHECK_EQ(cb::text::split(text, "STRINGTABLE").size(), std::size_t{2});
    CB_CHECK(contains(text, "  1, \"Insufficient memory\"\n"));
    CB_CHECK(contains(text, "  27, \" /C           Continues copying even if errors occur.%n"));
    CB_CHECK(text.ends_with("/N           Copy using the generated short names.\"\nEND\n"));
    // in a 32-bit module the memory flags are not written
    CB_CHECK(!contains(text, "PRELOAD") && !contains(text, "MOVEABLE"));
    CB_CHECK_EQ(rc.how.text.size(), std::size_t{3});
});

CB_TEST("build.resources.every_resource_can_be_carried_as_a_payload_instead", [] {
    cb::fs::ScopedTempDir dir("cb-res");
    const br::Module m = br::read_module(module("charmap.exe"));
    std::set<Key> all;
    for (const fr::Resource& r : m.resources) all.insert(r.key());
    const br::RcFiles rc = br::write_rc(m, dir.path(), all);
    CB_CHECK_EQ(rc.how.fallback.size(), std::size_t{8});
    CB_CHECK(rc.how.text.empty() && rc.how.files.empty() && rc.how.opaque.empty());
    const std::string text = cb::fs::read_text(rc.rc);
    CB_CHECK(contains(text, "\"CHARMAP\" 5 PRELOAD MOVEABLE IMPURE \"res/dialog_charmap.bin\"\n"));
    CB_CHECK(contains(text, "111 14 PRELOAD MOVEABLE DISCARDABLE IMPURE \"res/group_icon_111.bin\"\n"));
    CB_CHECK(contains(text, "1 3 "));   // the member icons are payloads of their own
    CB_CHECK(cb::fs::read_file(dir.path() / "res/dialog_charmap.bin") == m.resources[2].data);
    // one image group carried as it is takes all the groups with it (the compiler numbers the members)
    const br::RcFiles one = br::write_rc(m, dir.path() / "one", {m.resources[0].key()});
    CB_CHECK_EQ(one.how.fallback.size(), std::size_t{2});   // the group, and the 16-bit version block as before
    CB_CHECK_EQ(one.how.opaque.size(), std::size_t{2});     // and its two member images, now loose resources
    CB_CHECK_EQ(one.how.files.size(), std::size_t{1});      // the bitmap
    CB_CHECK_EQ(one.how.text.size(), std::size_t{3});
});

CB_TEST("build.resources.a_resource_beyond_latin1_makes_the_whole_source_utf8", [] {
    // a module whose resource is called "한글" (a name beyond Latin-1): the .rc is started over as UTF-8
    br::Module m;
    m.path = "korean.dll";
    m.text = Text::Wide;
    fr::Resource named;
    named.type = fr::kRcData;
    named.name = std::string("\xED\x95\x9C\xEA\xB8\x80");
    named.language = 1042;
    named.data = {1, 2, 3};
    fr::Resource plain;
    plain.type = fr::kRcData;
    plain.name = std::string("PLAIN");
    plain.language = 1042;
    plain.data = {4};
    m.resources = {plain, named};
    cb::fs::ScopedTempDir dir("cb-res");
    const br::RcFiles rc = br::write_rc(m, dir.path());
    CB_CHECK(rc.text == Text::Unicode);
    const std::string text = cb::fs::read_text(rc.rc);
    CB_CHECK(contains(text, "LANGUAGE 0x12, 0x01\n"));   // 1042 = 0x412: language 0x12, sublanguage 1
    CB_CHECK(contains(text, "\"\xED\x95\x9C\xEA\xB8\x80\" 10 \"res/rcdata__"));   // written as it is, in UTF-8
    CB_CHECK(contains(text, "\"PLAIN\" 10 \"res/rcdata_plain_0412.bin\"\n"));
    CB_CHECK_EQ(rc.how.opaque.size(), std::size_t{2});
    // the file the plan was made twice for left no payload behind from the first try
    const std::size_t files = static_cast<std::size_t>(std::distance(stdfs::directory_iterator(dir.path() / "res"), stdfs::directory_iterator()));
    CB_CHECK_EQ(files, std::size_t{2});
});

CB_TEST("build.resources.string_tables_beyond_ascii_are_payloads_in_a_utf8_source", [] {
    // the compiler counts a UTF-8 string table's strings in bytes: such a table is carried as it is
    br::Module m;
    m.path = "t.dll";
    m.text = Text::Unicode;
    fr::Resource table;
    table.type = fr::kString;
    table.name = std::uint32_t{1};
    table.language = 1033;
    // 16 length-prefixed UTF-16 strings: the first is "€1" (beyond ASCII), the rest empty
    table.data = {2, 0, 0xAC, 0x20, '1', 0};
    table.data.resize(table.data.size() + 15 * 2, 0);
    fr::Resource ascii = table;
    ascii.name = std::uint32_t{2};
    ascii.data = {2, 0, 'h', 0, 'i', 0};
    ascii.data.resize(ascii.data.size() + 15 * 2, 0);
    m.resources = {table, ascii};
    cb::fs::ScopedTempDir dir("cb-res");
    const br::RcFiles rc = br::write_rc(m, dir.path());
    CB_CHECK_EQ(rc.how.fallback.size(), std::size_t{1});
    CB_CHECK_EQ(rc.how.text.size(), std::size_t{1});
    const std::string text = cb::fs::read_text(rc.rc);
    CB_CHECK(contains(text, "1 6 \"res/string_1_0409.bin\"\n"));
    CB_CHECK(contains(text, "STRINGTABLE\nBEGIN\n  16, \"hi\"\nEND\n"));
});

CB_TEST("build.resources.a_name_with_no_source_form_is_a_no_source_form_error", [] {
    br::Module m;
    m.path = "zero.dll";
    m.text = Text::Wide;
    fr::Resource r;
    r.type = fr::kRcData;
    r.name = std::string("a\0b", 3);
    r.language = 1033;
    r.data = {1};
    m.resources = {r};
    cb::fs::ScopedTempDir dir("cb-res");
    CB_CHECK_THROWS_AS(br::NoSourceForm, (void)br::write_rc(m, dir.path()));
});

CB_TEST("build.resources.payloads_with_one_name_and_different_bytes_are_numbered", [] {
    // two names that make the same file stem: "a b" and "a_b"
    br::Module m;
    m.path = "stem.dll";
    m.text = Text::Wide;
    for (const char* name : {"a b", "a_b"}) {
        fr::Resource r;
        r.type = fr::kRcData;
        r.name = std::string(name);
        r.language = 1033;
        r.data = {static_cast<std::uint8_t>(name[1])};
        m.resources.push_back(r);
    }
    cb::fs::ScopedTempDir dir("cb-res");
    const br::RcFiles rc = br::write_rc(m, dir.path());
    const std::string text = cb::fs::read_text(rc.rc);
    CB_CHECK(contains(text, "\"a b\" 10 \"res/rcdata_a_b_0409.bin\"\n"));
    CB_CHECK(contains(text, "\"a_b\" 10 \"res/rcdata_a_b_0409_2.bin\"\n"));
    CB_CHECK_EQ(cb::fs::read_file(dir.path() / "res/rcdata_a_b_0409_2.bin")[0], std::uint8_t{'_'});
});

CB_TEST("build.resources.read_module_reads_pe_and_ne_and_nothing_else", [] {
    CB_CHECK_EQ(br::read_module(module("comcat.dll")).resources.size(), std::size_t{1});
    CB_CHECK(br::read_module(module("comcat.dll")).text == Text::Wide);
    CB_CHECK_EQ(br::read_module(module("resources/SETUP.EXE")).resources.size(), std::size_t{3});
    CB_CHECK(br::read_module(module("extract.exe")).resources.empty());   // (a CAB extractor of DOS vintage)
    CB_CHECK(br::read_module(module("find.exe")).resources.empty());
    CB_CHECK_THROWS_AS(cb::IoError, (void)br::read_module(module("no-such-file.exe")));
});

// ---- reading the compiler's .res layout ----------------------------------------------------------------------------

void put16(Bytes& b, std::uint32_t v) { b.push_back(static_cast<std::uint8_t>(v)); b.push_back(static_cast<std::uint8_t>(v >> 8)); }
void put32(Bytes& b, std::uint32_t v) { put16(b, v & 0xFFFF); put16(b, v >> 16); }

Bytes name32(const fr::Ident& v) {
    Bytes b;
    if (const auto* n = std::get_if<std::uint32_t>(&v)) {
        put16(b, 0xFFFF);
        put16(b, *n);
    } else {
        for (std::uint8_t c : cb::text::to_utf16le(std::get<std::string>(v))) b.push_back(c);
        put16(b, 0);
    }
    return b;
}

Bytes entry32(const fr::Ident& type, const fr::Ident& name, const Bytes& data, std::uint16_t flags = 0x30,
              std::uint16_t lang = 1033, bool pad = true) {
    Bytes names = name32(type);
    for (std::uint8_t c : name32(name)) names.push_back(c);
    const std::uint32_t head_len = ((8 + static_cast<std::uint32_t>(names.size()) + 3) & ~3u) + 16;
    Bytes b;
    put32(b, static_cast<std::uint32_t>(data.size()));
    put32(b, head_len);
    b.insert(b.end(), names.begin(), names.end());
    while (b.size() % 4 != 0) b.push_back(0);
    put32(b, 0);
    put16(b, flags);
    put16(b, lang);
    put32(b, 0);
    put32(b, 0);
    b.insert(b.end(), data.begin(), data.end());
    while (pad && b.size() % 4 != 0) b.push_back(0);
    return b;
}

Bytes entry16(const fr::Ident& type, const fr::Ident& name, const Bytes& data, std::uint16_t flags = 0x1030) {
    Bytes b;
    for (const fr::Ident* v : {&type, &name}) {
        if (const auto* n = std::get_if<std::uint32_t>(v)) {
            b.push_back(0xFF);
            put16(b, *n);
        } else {
            for (char c : std::get<std::string>(*v)) b.push_back(static_cast<std::uint8_t>(c));
            b.push_back(0);
        }
    }
    put16(b, flags);
    put32(b, static_cast<std::uint32_t>(data.size()));
    b.insert(b.end(), data.begin(), data.end());
    return b;
}

Bytes marker32() {
    Bytes b;
    put32(b, 0);
    put32(b, 32);
    for (std::uint32_t w : {0xFFFFu, 0u, 0xFFFFu, 0u}) put16(b, w);
    put32(b, 0);
    put16(b, 0);
    put16(b, 0);
    put32(b, 0);
    put32(b, 0);
    return b;
}

Bytes cat(std::initializer_list<Bytes> parts) {
    Bytes out;
    for (const Bytes& p : parts) out.insert(out.end(), p.begin(), p.end());
    return out;
}

Bytes bytes_of(std::string_view s) { return Bytes(s.begin(), s.end()); }

CB_TEST("build.resources.read_res_reads_a_32_bit_layout_and_skips_the_marker", [] {
    const Bytes file = cat({marker32(), entry32(10u, std::string("DATA"), bytes_of("hello world!")),
                            entry32(6u, 1u, Bytes(32, 0)), entry32(5u, std::string("caf\xC3\xA9"), Bytes(40, 7))});
    const auto found = br::read_res(file, true);
    CB_CHECK_EQ(found.size(), std::size_t{3});
    CB_CHECK(found[0].key() == key(10, "DATA", 1033));
    CB_CHECK_EQ(found[0].flags, std::uint16_t{0x30});
    CB_CHECK(found[0].data == bytes_of("hello world!"));
    CB_CHECK(found[1].key() == key(6, 1, 1033));
    CB_CHECK_EQ(found[1].data.size(), std::size_t{32});
    CB_CHECK(found[2].key() == key(5, "caf\xC3\xA9", 1033));   // the name is UTF-16 in the file, UTF-8 here
    CB_CHECK_EQ(found[2].data.size(), std::size_t{40});
}) ;

CB_TEST("build.resources.read_res_finds_entries_the_compiler_did_not_pad", [] {
    // the compiler leaves some bodies unpadded: the next entry starts anywhere (up to 16 bytes on), its fixed header
    // part moved to the next boundary. What is read is what the reader always read: the first entry whole, the second
    // with the fixed part taken from the nearest boundary (its flags and language are then not the writer's).
    const Bytes file = cat({marker32(), entry32(10u, std::string("ODD"), bytes_of("abc"), 0x30, 1033, false),
                            entry32(10u, std::string("EVEN"), bytes_of("abcd"))});
    const auto found = br::read_res(file, true);
    CB_CHECK_EQ(found.size(), std::size_t{2});
    CB_CHECK(found[0].key() == key(10, "ODD", 1033));
    CB_CHECK_EQ(found[0].flags, std::uint16_t{48});
    CB_CHECK_EQ(found[0].data.size(), std::size_t{3});
    CB_CHECK(found[1].key() == key(10, "EVEN", 4));
    CB_CHECK_EQ(found[1].flags, std::uint16_t{2304});
    CB_CHECK_EQ(found[1].data.size(), std::size_t{3});
});

CB_TEST("build.resources.read_res_stops_where_a_32_bit_layout_stops_making_sense", [] {
    // cut inside the last entry, a file too short for any entry, and noise: nothing, or what came before
    const Bytes whole = cat({marker32(), entry32(10u, std::string("DATA"), bytes_of("hello world!"))});
    CB_CHECK(br::read_res(cb::ByteView(whole).first(whole.size() - 5), true).empty());   // the cut entry is not an entry
    CB_CHECK(br::read_res(Bytes(10, 0), true).empty());
    CB_CHECK(br::read_res(Bytes{}, true).empty());
    CB_CHECK(br::read_res(Bytes(300, 'B'), true).empty());
    const auto marker_only = br::read_res(marker32(), true);
    CB_CHECK(marker_only.empty());
});

CB_TEST("build.resources.read_res_reads_a_16_bit_layout", [] {
    const Bytes file = cat({entry16(10u, std::string("DATA"), bytes_of("hello")), Bytes{0, 0, 0}, entry16(6u, 1u, Bytes(6, 0)),
                            entry16(5u, 1u, bytes_of("abcdefgh"))});
    const auto found = br::read_res(file, false);
    CB_CHECK_EQ(found.size(), std::size_t{3});
    CB_CHECK(found[0].key() == key(10, "DATA"));
    CB_CHECK_EQ(found[0].flags, std::uint16_t{0x1030});
    CB_CHECK(found[0].data == bytes_of("hello"));
    CB_CHECK(found[1].key() == key(6, 1));        // (the three zero bytes between entries are padding)
    CB_CHECK_EQ(found[1].data.size(), std::size_t{6});
    CB_CHECK(found[2].key() == key(5, 1));
    CB_CHECK(found[2].data == bytes_of("abcdefgh"));
    CB_CHECK(!found[0].language.has_value());     // a 16-bit resource has no language
    CB_CHECK(br::read_res(Bytes{}, false).empty());
    CB_CHECK(br::read_res(Bytes(40, 0), false).empty());
    CB_CHECK(br::read_res(Bytes{0x0a, 0x41, 0x42, 0x43}, false).empty());   // too short to hold an entry
});

CB_TEST("build.resources.read_res_16_bit_damage_is_a_truncated_error", [] {
    // a body that runs past the end is damage, and an error
    const Bytes cut = cat({entry16(10u, std::string("DATA"), bytes_of("hello")), entry16(6u, 1u, Bytes(6, 0))});
    CB_CHECK_THROWS_AS(cb::TruncatedError, (void)br::read_res(cb::ByteView(cut).first(cut.size() - 4), false));
    CB_CHECK_THROWS_AS(cb::TruncatedError, (void)br::read_res(Bytes(120, 'B'), false));   // a name with no terminator
});

// ---- comparing two sets of resources ---------------------------------------------------------------------------

Bytes flip(Bytes data, std::uint8_t mask) {
    if (!data.empty()) data[0] ^= mask;
    return data;
}

std::vector<fr::Resource> flipped(std::vector<fr::Resource> rs, std::uint8_t mask) {
    for (fr::Resource& r : rs) r.data = flip(r.data, mask);
    return rs;
}

br::Rebuilt compared(const std::vector<fr::Resource>& original, const std::vector<fr::Resource>& other, bool wide) {
    br::Rebuilt r;
    br::compare(original, other, wide, r);
    return r;
}

bool keys_are(const std::vector<Key>& got, std::initializer_list<Key> expected) {
    return got.size() == expected.size() && std::equal(got.begin(), got.end(), expected.begin());
}

// The verdicts are the recorded ones on the same mutations of the same modules.
CB_TEST("build.resources.compare_a_16_bit_module_with_changed_copies_of_itself", [] {
    const std::vector<fr::Resource> orig = br::read_module(module("charmap.exe")).resources;
    // 8 resources, of which 2 icon images belong to the group: 6 are compared
    auto same = compared(orig, orig, false);
    CB_CHECK_EQ(same.total, std::size_t{6});
    CB_CHECK_EQ(same.identical, std::size_t{6});
    CB_CHECK(same.same());
    CB_CHECK(same.different.empty() && same.missing.empty() && same.extra.empty());

    const std::vector<fr::Resource> without_first(orig.begin() + 1, orig.end());   // the icon group is gone
    auto dropped = compared(orig, without_first, false);
    CB_CHECK_EQ(dropped.identical, std::size_t{5});
    CB_CHECK(keys_are(dropped.missing, {key(14, 111)}));
    CB_CHECK(keys_are(dropped.extra, {key(3, 1), key(3, 2)}));   // its member images are loose resources now

    auto first_flipped = orig;
    first_flipped[0].data = flip(first_flipped[0].data, 0xFF);   // the group's first byte: the same images, so equivalent
    auto flip0 = compared(orig, first_flipped, false);
    CB_CHECK_EQ(flip0.identical, std::size_t{5});
    CB_CHECK_EQ(flip0.equivalent, std::size_t{1});
    CB_CHECK(flip0.different.empty());

    auto all = compared(orig, flipped(orig, 1), false);
    CB_CHECK_EQ(all.identical, std::size_t{0});
    CB_CHECK_EQ(all.equivalent, std::size_t{1});   // the version block still decodes to the same thing
    CB_CHECK(keys_are(all.different, {key(14, 111), key(2, 110), key(5, "CHARMAP"), key(6, 7), key(6, 8)}));

    auto twice = orig;
    twice.push_back(orig[0]);
    auto dup0 = compared(orig, twice, false);
    CB_CHECK(keys_are(dup0.different, {key(14, 111)}));   // two of one key: the compiler wrote something else too
    CB_CHECK_EQ(dup0.identical, std::size_t{5});

    const std::vector<fr::Resource> reversed(orig.rbegin(), orig.rend());
    auto rev = compared(orig, reversed, false);
    CB_CHECK_EQ(rev.identical, std::size_t{6});   // order does not matter

    auto empty = compared(orig, {}, false);
    CB_CHECK_EQ(empty.missing.size(), std::size_t{6});
    CB_CHECK_EQ(empty.total, std::size_t{6});
});

CB_TEST("build.resources.compare_a_32_bit_module_with_changed_copies_of_itself", [] {
    const std::vector<fr::Resource> orig = br::read_module(module("xcopy32.exe")).resources;
    CB_CHECK_EQ(compared(orig, orig, true).identical, std::size_t{3});
    auto dropped = compared(orig, std::vector<fr::Resource>(orig.begin() + 1, orig.end()), true);
    CB_CHECK(keys_are(dropped.missing, {key(6, 1, 1033)}));
    CB_CHECK_EQ(dropped.identical, std::size_t{2});
    auto first_flipped = orig;
    first_flipped[0].data = flip(first_flipped[0].data, 0xFF);   // a string table with a different length: not the same
    auto f0 = compared(orig, first_flipped, true);
    CB_CHECK(keys_are(f0.different, {key(6, 1, 1033)}));
    auto all = compared(orig, flipped(orig, 1), true);
    CB_CHECK_EQ(all.identical, std::size_t{0});
    CB_CHECK_EQ(all.equivalent, std::size_t{1});   // the version block: the flipped length byte decodes the same
    CB_CHECK(keys_are(all.different, {key(6, 1, 1033), key(6, 2, 1033)}));
    auto empty = compared(orig, {}, true);
    CB_CHECK_EQ(empty.missing.size(), std::size_t{3});
});

CB_TEST("build.resources.names_are_compared_without_regard_to_case", [] {
    fr::Resource a;
    a.type = fr::kRcData;
    a.name = std::string("Data");
    a.language = 1033;
    a.data = {1, 2};
    fr::Resource b = a;
    b.name = std::string("DATA");
    const auto r = compared({a}, {b}, true);
    CB_CHECK_EQ(r.identical, std::size_t{1});
    CB_CHECK(r.missing.empty() && r.extra.empty());
    b.language = 1031;   // another language is another resource
    const auto other = compared({a}, {b}, true);
    CB_CHECK_EQ(other.missing.size(), std::size_t{1});
    CB_CHECK_EQ(other.extra.size(), std::size_t{1});
});

// ---- the result and the compiler ----------------------------------------------------------------------------------------

CB_TEST("build.resources.the_result_says_what_came_back", [] {
    br::Rebuilt r;
    r.total = 12;
    r.identical = 7;
    r.equivalent = 5;
    r.as_text = 10;
    r.as_files = 6;
    CB_CHECK(r.same());
    CB_CHECK_EQ(r.line(),
                std::string("12 resources: 7 identical, 5 equivalent, 0 different, 0 missing  (written as text 10, as image "
                            "files 6, opaque payload 0, fallback payload 0)"));
    r.extra.push_back(key(3, 1));
    CB_CHECK(!r.same());
    CB_CHECK(contains(r.line(), "0 missing, 1 that the original does not have  (written"));
    r.extra.clear();
    r.different.push_back(key(5, 1));
    CB_CHECK(!r.same());
    r.different.clear();
    r.error = "the module has no resources";
    CB_CHECK(!r.same());
    CB_CHECK_EQ(r.line(), std::string("FAILED: the module has no resources"));
});

CB_TEST("build.resources.rebuild_of_a_module_without_resources_says_so", [] {
    cb::fs::ScopedTempDir dir("cb-res");
    const br::Rebuilt r = br::rebuild(cb::build::watcom::OpenWatcom(stdfs::path()), module("find.exe"), dir.path());
    CB_CHECK_EQ(r.error, std::string("the module has no resources"));
    CB_CHECK(!r.same());
    CB_CHECK(!r.rc.has_value());
});

CB_TEST("build.resources.without_a_resource_compiler_the_compile_is_a_tool_error", [] {
    cb::fs::ScopedTempDir dir("cb-res");
    cb::fs::write_text(dir.path() / "x.rc", "");
    CB_CHECK_THROWS_AS(cb::ToolError, (void)br::compile_rc(cb::build::watcom::OpenWatcom(dir.path() / "no-openwatcom"), dir.path() / "x.rc", Text::Wide));
    CB_CHECK_THROWS_AS(cb::ToolError, (void)br::rebuild(cb::build::watcom::OpenWatcom(dir.path() / "no-openwatcom"), module("charmap.exe"), dir.path() / "out"));
});

// With CB_OPENWATCOM (the OpenWatcom tree) set, a real module goes through the whole round trip: written, compiled by
// wrc, read back and compared.
CB_TEST("build.resources.a_16_bit_module_survives_the_round_trip_through_wrc", [] {
    const auto root = cb::fs::get_env("CB_OPENWATCOM");
    if (!root) return;   // (not set up: nothing to run)
    const cb::build::watcom::OpenWatcom ow{cb::fs::from_text(*root)};
    cb::fs::ScopedTempDir dir("cb-res");
    const br::Rebuilt r = br::rebuild(ow, module("charmap.exe"), dir.path());
    CB_CHECK_EQ(r.error, std::string());
    CB_CHECK(r.same());
    CB_CHECK_EQ(r.total, std::size_t{6});
    CB_CHECK(r.rc && r.res && r.link_res);
    CB_CHECK(stdfs::is_regular_file(*r.res) && stdfs::is_regular_file(*r.link_res));
    CB_CHECK_EQ(r.as_text + r.as_files + r.opaque + r.fallback, std::size_t{8});   // (a group and its members count once each)
    CB_CHECK_EQ(r.fallback, std::size_t{1});                                          // the 16-bit version block
    // what the compiler wrote reads back as what was written
    const auto back = br::read_res(cb::fs::read_file(*r.res), false);
    CB_CHECK_EQ(back.size(), std::size_t{8});
});

CB_TEST("build.resources.a_32_bit_module_survives_the_round_trip_through_wrc", [] {
    const auto root = cb::fs::get_env("CB_OPENWATCOM");
    if (!root) return;
    const cb::build::watcom::OpenWatcom ow{cb::fs::from_text(*root)};
    cb::fs::ScopedTempDir dir("cb-res");
    const br::Rebuilt r = br::rebuild(ow, module("xcopy32.exe"), dir.path());
    CB_CHECK_EQ(r.error, std::string());
    CB_CHECK(r.same());
    CB_CHECK_EQ(r.total, std::size_t{3});
    CB_CHECK_EQ(r.fallback, std::size_t{0});
    CB_CHECK_EQ(r.as_text, std::size_t{3});
    CB_CHECK(r.link_res.has_value());
});

CB_TEST("build.resources.a_resource_the_compiler_rejects_is_carried_as_a_payload", [] {
    const auto root = cb::fs::get_env("CB_OPENWATCOM");
    if (!root) return;
    const cb::build::watcom::OpenWatcom ow{cb::fs::from_text(*root)};
    // a 16-bit module's version block is the example the compiler gets wrong; SETUP.EXE has one and an icon group
    cb::fs::ScopedTempDir dir("cb-res");
    const br::Rebuilt r = br::rebuild(ow, module("resources/SETUP.EXE"), dir.path());
    CB_CHECK_EQ(r.error, std::string());
    CB_CHECK(r.same());
    CB_CHECK_EQ(r.fallback, std::size_t{1});
    CB_CHECK_EQ(r.not_source.size(), std::size_t{1});
    CB_CHECK(r.not_source[0] == key(fr::kVersion, 1));
});

// ---- the command ---------------------------------------------------------------------------------------------------------

CB_TEST("build.resources.the_command_declares_its_arguments", [] {
    br::Environment env;
    const cb::cli::Command c = br::command(env);
    CB_CHECK_EQ(c.name, std::string("resources"));
    const cb::cli::Cli app("codebreaker", {c});
    CB_CHECK_THROWS_AS(cb::UsageError, (void)app.parse({"resources"}));   // a module is required
    const auto parsed = app.parse({"resources", "notepad.exe", "winmine.exe", "--out", "d"});
    CB_CHECK_EQ(parsed.args.values("module").size(), std::size_t{2});
    CB_CHECK_EQ(parsed.args.value("out").value_or(""), std::string("d"));
});

CB_TEST("build.resources.the_command_names_a_module_it_cannot_find", [] {
    br::Environment env;
    env.retail = [](std::string_view) { return std::nullopt; };
    const cb::cli::Cli app("codebreaker", {br::command(env)});
    cb::testing::CapturedStd captured;
    const int code = app.run({"resources", "nosuchmodule.dll"});
    CB_CHECK_EQ(code, 1);
    CB_CHECK_EQ(captured.out(), std::string("nosuchmodule.dll: no such module in the corpus\n"));
});

CB_TEST("build.resources.the_command_prints_a_line_and_the_rc_it_wrote", [] {
    const auto root = cb::fs::get_env("CB_OPENWATCOM");
    if (!root) return;
    cb::fs::ScopedTempDir dir("cb-res");
    br::Environment env;
    env.open_watcom = cb::build::watcom::OpenWatcom(cb::fs::from_text(*root));
    env.retail = [&](std::string_view name) -> std::optional<stdfs::path> {
        return name == "charmap.exe" ? std::optional<stdfs::path>(module("charmap.exe")) : std::nullopt;
    };
    const cb::cli::Cli app("codebreaker", {br::command(env)});
    cb::testing::CapturedStd captured;
    const int code = app.run({"resources", "charmap.exe", "--out", cb::fs::to_text(dir.path() / "out")});
    const std::string out = captured.out();
    CB_CHECK_EQ(code, 0);
    CB_CHECK(out.starts_with("charmap.exe: 6 resources: "));
    CB_CHECK(contains(out, "charmap.rc\n"));
    CB_CHECK(stdfs::is_regular_file(dir.path() / "out" / "charmap.rc"));
});

}  // namespace
