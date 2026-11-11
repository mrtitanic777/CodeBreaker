// Tests of cb/symbols/ordinals. The naming rules run over a stand-in for the corpus (ModuleExports) so that the
// expected answers are plain; the library readers run over real files of the SDK and DDK (testdata/ordinals: two
// 16-bit import libraries, a 32-bit one, a definition file), their expected names being what the Python module read
// from the same files. The readers were also run over all 597 .lib and .def files of the reference folder (227 name
// something) and gave the Python module's answer for every one.
//
// DIVERGES from the Python module, by design: a damaged member of an import library (or a record cut short in a 16-bit
// one) is passed over, where the Python gave up on the whole file; a long symbol name that the string table does not
// end is taken to the end of the member; the shipped file is written with an indent, keys sorted.
#include "cb/symbols/ordinals.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/knowledge/reads.hpp"

namespace ord = cb::symbols::ordinals;

namespace {

// A corpus of a few modules written out.
class FakeExports : public ord::ModuleExports, public ord::ExportCheck {
public:
    std::map<std::string, Table> tables;
    std::map<std::string, std::set<std::uint32_t>> exported;
    std::map<std::pair<std::string, std::uint32_t>, bool> agreement;
    std::set<std::string> imported;
    mutable int table_calls = 0;

    std::optional<Table> table(std::string_view stem) const override {
        ++table_calls;
        const auto it = tables.find(std::string(stem));
        if (it == tables.end()) return std::nullopt;
        return it->second;
    }
    std::optional<std::set<std::uint32_t>> numbers(std::string_view stem) const override {
        const auto it = exported.find(std::string(stem));
        if (it == exported.end()) return std::nullopt;
        return it->second;
    }
    std::optional<bool> arguments_agree(std::string_view stem, std::uint32_t ordinal, std::uint32_t) const override {
        const auto it = agreement.find({std::string(stem), ordinal});
        if (it == agreement.end()) return std::nullopt;
        return it->second;
    }
    std::set<std::string> imported_by_number() const override { return imported; }
};

ord::ShippedTables shipped() {
    ord::ShippedTables t;
    t["MFC30"] = {"mfc30.def", 3, 0, {{800, "??1CString@@QAE@XZ"}, {801, "??0CString@@QAE@XZ"}, {802, "_AfxWinMain@16"}}};
    t["COMCTL32"] = {"comctl32.lib", 0, 0, {{17, "InitCommonControls"}, {18, "NotAnIdentifier!"}}};
    t["KERNEL32"] = {"kernel32.lib", 0, 0, {{99, "FromTheFile"}}};
    return t;
}

std::string spelling(std::string_view name) {
    if (name == "GETDLGITEM") return "GetDlgItem";
    return std::string(name);
}

ord::Ordinals make(const FakeExports& corpus) { return ord::Ordinals(corpus, shipped(), spelling); }

std::filesystem::path data(const char* name) { return cb::testing::data_path(std::string("symbols/testdata/ordinals/") + name); }

using Names = std::map<std::uint32_t, std::string>;
using Library = std::map<std::string, Names>;

}  // namespace

CB_TEST("symbols.ordinals.module_stem", [] {
    CB_CHECK_EQ(ord::module_stem("KERNEL32.dll"), "KERNEL32");
    CB_CHECK_EQ(ord::module_stem("c:\\win\\system\\user.exe"), "USER");
    CB_CHECK_EQ(ord::module_stem("kernel"), "KERNEL");
    CB_CHECK_EQ(ord::module_stem("x/y/Mfc30.DLL"), "MFC30");
    CB_CHECK_EQ(ord::module_stem("keyboard.drv"), "KEYBOARD");
    CB_CHECK_EQ(ord::module_stem("odd.txt"), "ODD.TXT");   // only a module's extensions come off
    CB_CHECK_EQ(ord::module_stem(""), "");
});

CB_TEST("symbols.ordinals.the_modules_own_table_names_it_first", [] {
    FakeExports corpus;
    corpus.tables["USER"] = {{{91, "GETDLGITEM"}, {92, "?Odd@@YAXXZ"}}, "user.exe"};
    corpus.tables["KERNEL32"] = {{{99, "FromTheCorpus"}}, "kernel32.dll"};
    const ord::Ordinals o = make(corpus);
    CB_CHECK(o.linker_name("USER", 91) == std::optional<std::string>("GETDLGITEM"));
    CB_CHECK(o.name("USER", 91) == std::optional<std::string>("GetDlgItem"));    // as the headers spell it
    CB_CHECK(o.name("user.exe", 91) == std::optional<std::string>("GetDlgItem"));   // a file name is a module
    CB_CHECK(o.name("USER", 92) == std::optional<std::string>("?Odd@@YAXXZ"));   // a C++ name is not respelled
    CB_CHECK(o.source("USER", 91) == "user.exe");
    CB_CHECK(o.name("KERNEL32", 99) == std::optional<std::string>("FromTheCorpus"));   // the corpus wins over the shipped file
    CB_CHECK(o.source("KERNEL32", 99) == "kernel32.dll");
    CB_CHECK(!o.name("USER", 99999).has_value());
    CB_CHECK(o.source("USER", 99999) == "");
});

CB_TEST("symbols.ordinals.the_shipped_table_names_what_the_module_leaves_unnamed", [] {
    FakeExports corpus;
    corpus.tables["MFC30"] = {{{1, "OTHER"}}, "mfc30.dll"};
    const ord::Ordinals o = make(corpus);
    CB_CHECK(o.name("MFC30", 800) == std::optional<std::string>("??1CString@@QAE@XZ"));
    CB_CHECK(o.source("MFC30", 800) == "mfc30.def");
    CB_CHECK(o.name("MFC30", 1) == std::optional<std::string>("OTHER"));
    CB_CHECK(o.source("MFC30", 1) == "mfc30.dll");
    CB_CHECK(o.name("COMCTL32", 17) == std::optional<std::string>("InitCommonControls"));
    CB_CHECK(!o.name("COMCTL32", 99).has_value());
    CB_CHECK(!o.name("NOSUCHMODULE", 1).has_value());
});

CB_TEST("symbols.ordinals.table_merges_both_and_respells", [] {
    FakeExports corpus;
    corpus.tables["USER"] = {{{91, "GETDLGITEM"}}, "user.exe"};
    corpus.tables["KERNEL32"] = {{{99, "FromTheCorpus"}, {5, "GETDLGITEM"}}, "kernel32.dll"};
    const ord::Ordinals o = make(corpus);
    CB_CHECK((o.table("USER") == Names{{91, "GetDlgItem"}}));
    CB_CHECK((o.table("KERNEL32") == Names{{5, "GetDlgItem"}, {99, "FromTheCorpus"}}));   // the corpus's name for 99 over the file's
    CB_CHECK((o.table("MFC30") == Names{{800, "??1CString@@QAE@XZ"}, {801, "??0CString@@QAE@XZ"}, {802, "_AfxWinMain@16"}}));
    CB_CHECK(o.table("NOSUCHMODULE").empty());
});

CB_TEST("symbols.ordinals.identifier", [] {
    FakeExports corpus;
    const ord::Ordinals o = make(corpus);
    CB_CHECK(o.identifier("COMCTL32", 17) == std::optional<std::string>("InitCommonControls"));
    CB_CHECK(o.identifier("MFC30", 802) == std::optional<std::string>("AfxWinMain"));   // `_Name@16` without its decoration
    CB_CHECK(!o.identifier("MFC30", 800).has_value());                                  // a C++ name has none
    CB_CHECK(!o.identifier("COMCTL32", 18).has_value());                                // not an identifier
    CB_CHECK(!o.identifier("COMCTL32", 99).has_value());                                // unknown
});

CB_TEST("symbols.ordinals.documented_names", [] {
    const auto d = ord::documented("KERNEL32", 89);
    CB_CHECK(d.has_value());
    CB_CHECK_EQ(d->name, "FT_PrologPrime");
    CB_CHECK(d->where.starts_with("Geoff Chappell"));
    CB_CHECK(ord::documented("kernel32.dll", 17).has_value());
    CB_CHECK(!ord::documented("KERNEL32", 18).has_value());
    CB_CHECK(!ord::documented("USER", 89).has_value());
});

CB_TEST("symbols.ordinals.names_of_imports_once_each_in_order", [] {
    FakeExports corpus;
    corpus.tables["USER"] = {{{91, "GETDLGITEM"}}, "user.exe"};
    const ord::Ordinals o = make(corpus);
    const std::vector<ord::ImportRef> imports = {{"USER", 91}, {"MFC30", 800}, {"USER", 91}, {"USER", 5}, {"MFC30", 800}};
    const auto named = o.names_of(imports);
    CB_CHECK_EQ(named.size(), std::size_t{3});
    CB_CHECK(named[0].import.module == "USER" && named[0].import.ordinal == 91 && named[0].name == std::optional<std::string>("GetDlgItem"));
    CB_CHECK(named[1].import.module == "MFC30" && named[1].name == std::optional<std::string>("??1CString@@QAE@XZ"));
    CB_CHECK(named[2].import.ordinal == 5 && !named[2].name.has_value());
});

CB_TEST("symbols.ordinals.the_corpus_is_asked_once_per_module", [] {
    FakeExports corpus;
    corpus.tables["USER"] = {{{91, "GETDLGITEM"}}, "user.exe"};
    const ord::Ordinals o = make(corpus);
    for (int i = 0; i < 5; ++i) (void)o.name("USER", 91);
    (void)o.name("user.exe", 92);
    CB_CHECK_EQ(corpus.table_calls, 1);
});

CB_TEST("symbols.ordinals.consulting_the_shipped_table_is_a_knowledge_read", [] {
    FakeExports corpus;
    corpus.tables["USER"] = {{{91, "GETDLGITEM"}}, "user.exe"};
    const ord::Ordinals o = make(corpus);
    cb::knowledge::Recording recording;
    (void)o.name("USER", 91);       // the corpus answers: the shipped table is not read
    (void)o.name("MFC30", 800);     // it is
    std::vector<std::string> keys;
    for (const auto& r : recording.reads()->sorted()) keys.push_back(r.text());
    CB_CHECK(keys == (std::vector<std::string>{"windows:ordinal_names:MFC30"}));
});

CB_TEST("symbols.ordinals.shipped_file_round_trip", [] {
    const ord::ShippedTables tables = shipped();
    const std::string text = ord::format_shipped(tables);
    const ord::ShippedTables back = ord::parse_shipped(text);
    CB_CHECK_EQ(back.size(), tables.size());
    CB_CHECK(back.at("MFC30").names == tables.at("MFC30").names);
    CB_CHECK_EQ(back.at("MFC30").from, "mfc30.def");
    CB_CHECK_EQ(back.at("MFC30").checked_yes, 3u);
    // keys sorted as text: "800" before "9"
    ord::ShippedTables sorted;
    sorted["M"] = {"f", 0, 0, {{9, "nine"}, {800, "eight hundred"}}};
    const std::string s = ord::format_shipped(sorted);
    CB_CHECK(s.find("\"800\"") < s.find("\"9\""));

    CB_CHECK_THROWS_AS(cb::ParseError, (void)ord::parse_shipped("["));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)ord::parse_shipped("[]"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)ord::parse_shipped(R"({"M": 3})"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)ord::parse_shipped(R"({"M": {"names": {"x": "y"}}})"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)ord::parse_shipped(R"({"M": {"names": {"1": 2}}})"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)ord::parse_shipped(R"({"M": {"checked": [1]}})"));
    CB_CHECK(ord::parse_shipped(R"({"M": {}})").at("M").names.empty());   // every part is optional
});

CB_TEST("symbols.ordinals.definition_file_of_the_sdk", [] {
    const auto raw = cb::fs::read_text(data("the_dll.def"));
    CB_CHECK((ord::read_definition_file(raw, "the_dll") ==
              Library{{"THE_DLL", {{2, "DLLFunction1"}, {3, "DLLFunction2"}, {4, "DLLFunction3"}, {5, "DLLFunction4"},
                                   {6, "DLLDialogBox"}, {7, "DLLDlgProc"}}}}));
});

CB_TEST("symbols.ordinals.definition_file_forms", [] {
    const std::string text =
        "; a comment @ 99\r\n"
        "NAME other\r\n"
        "  Func1 @1\r\n"
        "Func2=InternalName  @ 2 NONAME\r\n"
        "Func3@3\r\n"                       // no blank before '@': not an export line
        "Func4  @x\r\n"                     // no number
        "Func1  @10 ; the first line for a number wins, and a number once named is kept\r\n"
        "Dup @1\r\n"
        "Func5 @4294967296\r\n"             // beyond 32 bits: not an export
        "Last\t@\t7\n";
    CB_CHECK((ord::read_definition_file(text, "fromfile") == Library{{"FROMFILE", {{1, "Func1"}, {2, "Func2"}, {7, "Last"}, {10, "Func1"}}}}));
    // LIBRARY names the module, in any case; a later LIBRARY line renames it
    CB_CHECK((ord::read_definition_file("library   Mine ; x\nA @1\n", "file") == Library{{"MINE", {{1, "A"}}}}));
    CB_CHECK((ord::read_definition_file("LIBRARY One\nA @1\nLIBRARY Two\nB @2\n", "file") == Library{{"TWO", {{1, "A"}, {2, "B"}}}}));
    CB_CHECK(ord::read_definition_file("LIBRARY X\nEXPORTS\n", "file").empty());   // nothing exported: no module
    CB_CHECK(ord::read_definition_file("", "file").empty());
    CB_CHECK(ord::read_definition_file("Lone\rA @5\rB @6", "f") == (Library{{"F", {{5, "A"}, {6, "B"}}}}));   // old line ends
});

CB_TEST("symbols.ordinals.import_library_16_bit", [] {
    const Library commctrl = ord::read_import_library(cb::fs::read_file(data("commctrl16.lib")));
    CB_CHECK_EQ(commctrl.size(), std::size_t{1});
    const Names& names = commctrl.at("COMMCTRL");
    CB_CHECK_EQ(names.size(), std::size_t{26});
    CB_CHECK_EQ(names.at(2), "MENUHELP");
    CB_CHECK_EQ(names.at(17), "INITCOMMONCONTROLS");
    CB_CHECK_EQ(names.at(53), "IMAGELIST_ADDICON");
    CB_CHECK_EQ(names.at(161), "DESTROYPROPERTYSHEETPAGE");

    const Library shell = ord::read_import_library(cb::fs::read_file(data("shell16.lib")));
    CB_CHECK((shell == Library{{"SHELL",
                                {{9, "DRAGACCEPTFILES"}, {11, "DRAGQUERYFILE"}, {12, "DRAGFINISH"}, {13, "DRAGQUERYPOINT"},
                                 {20, "SHELLEXECUTE"}, {21, "FINDEXECUTABLE"}, {34, "EXTRACTICON"}, {37, "DOENVIRONMENTSUBST"},
                                 {38, "FINDENVIRONMENTSTRING"}, {40, "EXTRACTICONEX"}, {98, "Shl3216_ThunkData16"},
                                 {99, "Shl1632_ThunkData16"}, {101, "DllEntryPoint"}}}}));
});

CB_TEST("symbols.ordinals.import_library_32_bit", [] {
    // the linker's own symbols, as the library holds them
    CB_CHECK((ord::read_import_library(cb::fs::read_file(data("cap32.lib"))) ==
              Library{{"CAP", {{2, "__penter"}, {3, "_StartCAP@0"}, {4, "_StopCAP@0"}, {5, "_DumpCAP@0"}}}}));
});

CB_TEST("symbols.ordinals.import_library_damage", [] {
    CB_CHECK(ord::read_import_library({}).empty());
    const cb::Bytes text = cb::to_bytes("this is not a library at all");
    CB_CHECK(ord::read_import_library(text).empty());
    // cut anywhere: an answer (maybe fewer names), never a crash or a read past the end
    for (const char* file : {"cap32.lib", "commctrl16.lib"}) {
        const cb::Bytes whole = cb::fs::read_file(data(file));
        std::size_t most = 0;
        for (std::size_t n = 0; n <= whole.size(); n += (n < 400 ? 1 : 13)) {
            const Library cut = ord::read_import_library(cb::ByteView(whole).subspan(0, n));
            std::size_t found = 0;
            for (const auto& [module, names] : cut) found += names.size();
            most = std::max(most, found);
        }
        CB_CHECK(most >= 4);
    }
    // every byte of an archive's members damaged in turn
    cb::Bytes archive = cb::fs::read_file(data("cap32.lib"));
    for (std::size_t i = 8; i < archive.size(); i += 7) {
        const std::uint8_t keep = archive[i];
        archive[i] = static_cast<std::uint8_t>(keep ^ 0xFF);
        (void)ord::read_import_library(archive);
        archive[i] = keep;
    }
});

CB_TEST("symbols.ordinals.library_files_in_order", [] {
    cb::fs::ScopedTempDir tmp("cb-ord");
    const auto put = [&](const char* rel) { cb::fs::write_text(tmp.path() / rel, "x"); };
    put("lib/b.lib");
    put("lib/a.def");
    put("lib/z/inner.lib");
    put("lib/Alpha/skipped.lib");
    put("lib/ppc/skipped.lib");
    put("lib/MIPS/skipped.lib");
    put("lib/m/deeper/last.def");
    put("alone.lib");
    const std::vector<std::filesystem::path> paths = {tmp.path() / "lib", tmp.path() / "alone.lib"};
    std::vector<std::string> got;
    for (const auto& p : ord::library_files(paths)) got.push_back(cb::fs::to_text(std::filesystem::relative(p, tmp.path())));
    CB_CHECK(got == (std::vector<std::string>{"lib/a.def", "lib/b.lib", "lib/m/deeper/last.def", "lib/z/inner.lib", "alone.lib"}));
});

CB_TEST("symbols.ordinals.collect", [] {
    cb::fs::ScopedTempDir tmp("cb-ord");
    cb::fs::copy_tree(cb::testing::data_path("symbols/testdata/ordinals"), tmp.path() / "libs");

    FakeExports corpus;
    // COMMCTRL is in the corpus: it exports 2..8, 17 and 20, and names its number 5 itself
    corpus.tables["COMMCTRL"] = {{{5, "DRAWSTATUSTEXT_FROM_MODULE"}}, "commctrl.dll"};
    corpus.exported["COMMCTRL"] = {2, 3, 4, 5, 6, 7, 8, 17, 20};
    // SHELL is in the corpus and exports every number its library names
    corpus.exported["SHELL"] = {9, 11, 12, 13, 20, 21, 34, 37, 38, 40, 98, 99, 101};
    // CAP is not in the corpus but something imports it by number: its names are kept (and made plain) when the code
    // agrees with those it can check. THE_DLL is neither in the corpus nor imported: not kept.
    corpus.imported = {"CAP"};
    corpus.agreement[{"CAP", 3}] = true;
    corpus.agreement[{"CAP", 4}] = true;
    corpus.agreement[{"CAP", 5}] = true;

    std::vector<std::string> log;
    const std::vector<std::filesystem::path> paths = {tmp.path() / "libs"};
    const ord::ShippedTables out = ord::collect(paths, corpus, corpus, [&](std::string_view line) { log.emplace_back(line); });

    CB_CHECK_EQ(out.size(), std::size_t{3});
    const ord::ShippedModule& cap = out.at("CAP");
    CB_CHECK_EQ(cap.from, "cap32.lib");
    CB_CHECK_EQ(cap.checked_yes, 3u);   // `__penter` has no stack size in its name: unchecked
    CB_CHECK_EQ(cap.checked_no, 0u);
    CB_CHECK((cap.names == Names{{2, "_penter"}, {3, "StartCAP"}, {4, "StopCAP"}, {5, "DumpCAP"}}));   // without the linker's decoration
    // COMMCTRL: the names of the numbers the module exports and does not name itself (5 is named by the module; the
    // numbers it does not export, 13 and up, belong to another release)
    const ord::ShippedModule& commctrl = out.at("COMMCTRL");
    CB_CHECK_EQ(commctrl.from, "commctrl16.lib");
    CB_CHECK((commctrl.names == Names{{2, "MENUHELP"}, {3, "SHOWHIDEMENUCTL"}, {4, "GETEFFECTIVECLIENTRECT"}, {6, "CREATESTATUSWINDOW"},
                                       {7, "CREATETOOLBAR"}, {8, "CREATEMAPPEDBITMAP"}, {17, "INITCOMMONCONTROLS"}, {20, "CREATETOOLBAREX"}}));
    CB_CHECK_EQ(out.at("SHELL").names.size(), std::size_t{13});   // 16-bit names carry no decoration to check
    CB_CHECK(!out.contains("THE_DLL"));
    CB_CHECK_EQ(log.size(), std::size_t{3});   // a line for each table kept, by module
    CB_CHECK_EQ(log[0], "CAP            4 names from cap32.lib      (3 checked against the code, 0 dropped)");
    CB_CHECK(log[1].starts_with("COMMCTRL       8 names from commctrl16.lib"));
});

CB_TEST("symbols.ordinals.collect_refuses_a_table_the_code_disagrees_with", [] {
    cb::fs::ScopedTempDir tmp("cb-ord");
    cb::fs::copy_tree(cb::testing::data_path("symbols/testdata/ordinals"), tmp.path() / "libs");
    FakeExports corpus;
    corpus.imported = {"CAP"};
    corpus.agreement[{"CAP", 3}] = false;
    corpus.agreement[{"CAP", 4}] = false;
    corpus.agreement[{"CAP", 5}] = true;
    std::vector<std::string> log;
    const std::vector<std::filesystem::path> paths = {tmp.path() / "libs"};
    const ord::ShippedTables out = ord::collect(paths, corpus, corpus, [&](std::string_view line) { log.emplace_back(line); });
    CB_CHECK(!out.contains("CAP"));   // 2 wrong of 3 checked: more than one in twenty
    CB_CHECK_EQ(log.size(), std::size_t{1});
    CB_CHECK_EQ(log[0], "CAP        NOT USED: cap32.lib names 1 exports rightly and 2 wrongly");
    // one wrong in twenty-one is tolerated
    FakeExports many;
    many.imported = {"M"};
    std::string def = "LIBRARY M\n";
    for (int i = 1; i <= 21; ++i) def += "F" + std::to_string(i) + "_Stack@4 @" + std::to_string(i) + "\n";
    cb::fs::write_text(tmp.path() / "m.def", def);
    for (std::uint32_t i = 1; i <= 21; ++i) many.agreement[{"M", i}] = i != 5;
    const std::vector<std::filesystem::path> one = {tmp.path() / "m.def"};
    const ord::ShippedTables kept = ord::collect(one, many, many);
    CB_CHECK_EQ(kept.at("M").names.size(), std::size_t{20});
    CB_CHECK(!kept.at("M").names.contains(5));
    // two wrong of twenty-one are not
    many.agreement[{"M", 6}] = false;
    CB_CHECK(ord::collect(one, many, many).empty());
});

CB_TEST("symbols.ordinals.collect_earlier_files_win_and_bad_files_are_skipped", [] {
    cb::fs::ScopedTempDir tmp("cb-ord");
    cb::fs::write_text(tmp.path() / "a.def", "LIBRARY M\nOne @1\nTwo @2\n");
    cb::fs::write_text(tmp.path() / "b.def", "LIBRARY M\nOther @1\nThree @3\n");   // M is taken
    cb::fs::write_text(tmp.path() / "c.lib", "not a library");
    cb::fs::write_text(tmp.path() / "d.txt", "LIBRARY X\nNo @1\n");               // neither .def nor .lib
    FakeExports corpus;
    corpus.imported = {"M"};
    const std::vector<std::filesystem::path> paths = {tmp.path()};
    const ord::ShippedTables out = ord::collect(paths, corpus, corpus);
    CB_CHECK_EQ(out.size(), std::size_t{1});
    CB_CHECK((out.at("M").names == Names{{1, "One"}, {2, "Two"}}));
    CB_CHECK_EQ(out.at("M").from, "a.def");
});
