// Tests of the module writer: the words that say a status, the names a module's functions are written with, the files written from a real
// function store (a SQLite file in a temporary folder) with a stand-in readable pass, a driver's assembly file, and the scoreboard. The C file
// is held to a golden file that was read through for correctness (testdata/writers/module.golden.c).

#include "source/module_writer.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace cb::source::module_writer {
namespace {

store::Best best_of(store::Class cls, std::string reason = "", std::string limits = "", store::WeakLimits weak = {}) {
    store::Best b;
    b.cls = cls;
    b.candidate.reason = std::move(reason);
    b.candidate.limits = std::move(limits);
    b.weak = weak;
    return b;
}

CB_TEST("source.module_writer.a_status_says_how_much_was_exercised_why_it_is_weak_and_what_differed", [] {
    CB_CHECK_EQ(describe(best_of(store::Class::Verified, "", "coverage=100,runs=4")), "100% of it exercised");
    CB_CHECK_EQ(describe(best_of(store::Class::Verified)), "");
    CB_CHECK_EQ(describe(best_of(store::Class::Weak, "", "coverage=40", {store::WeakLimit::LowCoverage, store::WeakLimit::Truncated})),
                std::string(store::explanation(store::WeakLimit::LowCoverage)) + "; " + std::string(store::explanation(store::WeakLimit::Truncated)) + " (40% ran)");
    CB_CHECK_EQ(describe(best_of(store::Class::Weak, "", "", {store::WeakLimit::FaultOnly})), std::string(store::explanation(store::WeakLimit::FaultOnly)));
    CB_CHECK_EQ(describe(best_of(store::Class::Unverified, "ret-differs")), "returns a different value");
    CB_CHECK_EQ(describe(best_of(store::Class::NotJudged, "a reason nobody has words for")), "a reason nobody has words for");
    CB_CHECK_EQ(describe(best_of(store::Class::NoCompile, std::string(200, 'e'))).size(), 90u);
    CB_CHECK_EQ(describe(best_of(store::Class::NoSource, std::string(200, 'e'))).size(), 80u);
    CB_CHECK_EQ(describe(best_of(store::Class::Verified, "", "coverage=oops")), "");   // a limit that is not a number says nothing
    CB_CHECK_EQ(std::string(why("stack-differs")), "removes a different number of bytes from the stack (convention or argument count)");
    CB_CHECK_EQ(std::string(why("retail: call-into-nothing")), "the original calls through a pointer the loader fills in (a thunk entry)");
    CB_CHECK_EQ(std::string(why("anything else")), "anything else");
});

CB_TEST("source.module_writer.an_import_is_written_by_its_real_name_where_that_name_is_free", [] {
    const std::map<std::string, std::string> found = {{"USER_91", "GetDlgItem"}, {"USER_92", "SetFocus"}, {"KERNEL_5", "SetFocus"}, {"X_1", "Foo"}, {"Y_2", "Y_2"}};
    // SetFocus is two imports' (and so neither's); Foo is a function of the module's; Y_2 is the name of another import by number
    CB_CHECK((unique_import_names(found, {"Foo"}) == std::map<std::string, std::string>{{"USER_91", "GetDlgItem"}}));
    CB_CHECK_EQ(unique_import_names(found, {}).size(), 2u);   // GetDlgItem, and Foo now that the module has no function of that name
    CB_CHECK(unique_import_names({}, {}).empty());
});

CB_TEST("source.module_writer.a_console_import_is_named_from_the_libraries_numbers", [] {
    CB_CHECK(parse_console_import("import0_ord_16") && parse_console_import("import0_ord_16")->library == 0 && parse_console_import("import0_ord_16")->number == "16");
    CB_CHECK_EQ(parse_console_import("import12_ord_7")->library, 12u);
    for (const char* bad : {"", "import_ord_1", "import0_ord_", "import0ord_1", "importX_ord_1", "import0_ord_1x", "xport0_ord_1", "import0_ord_1 "}) CB_CHECK(!parse_console_import(bad));
    const std::vector<std::string> symbols = {"import0_ord_16", "import0_ord_17", "import1_ord_3", "import1_ord_4", "import2_ord_1", "other", "import0_ord_99", "import1_ord_5"};
    const std::vector<std::map<std::string, std::string>> libraries = {
        {{"16", "XamGetSystemVersion"}, {"17", "not-an-identifier!"}, {"99", ""}}, {{"3", "NetDll_socket"}, {"4", "NetDll_socket"}, {"5", "Taken"}}};
    // 17 is no C name; 3 and 4 give the same name (so neither gets it); the third library is not there; 5's name is a function of the module's
    const auto names = console_import_names(symbols, libraries, {"Taken"});
    CB_CHECK((names == std::map<std::string, std::string>{{"import0_ord_16", "XamGetSystemVersion"}}));
});

CB_TEST("source.module_writer.spell_replaces_whole_identifiers_only", [] {
    const std::map<std::string, std::string> names = {{"USER_91", "GetDlgItem"}, {"sub_00401010", "CreateThing"}};
    CB_CHECK_EQ(spell("a = USER_91(x); USER_912(y); XUSER_91(z); sub_00401010();", names), "a = GetDlgItem(x); USER_912(y); XUSER_91(z); CreateThing();");
    CB_CHECK_EQ(spell("no names: USER_91", {}), "no names: USER_91");
    CB_CHECK_EQ(spell("", names), "");
    CB_CHECK_EQ(spell("USER_91", names), "GetDlgItem");   // the whole text is one name
    CB_CHECK_EQ(spell("0xUSER_91 USER_91.x", names), "0xUSER_91 GetDlgItem.x");   // `.` ends a word, a digit before it does not
});

CB_TEST("source.module_writer.a_recovered_name_replaces_a_placeholder_where_it_is_free", [] {
    const std::vector<std::string> addrs = {"A", "B", "C", "D", "E"};
    const std::map<std::string, std::string> candidate = {{"A", "sub_A"}, {"B", "DllMain"}, {"C", "sub_C"}, {"D", "sub_D"}};   // E has none: sub_E
    const std::map<std::string, std::string> recovered = {{"A", "Alpha"}, {"B", "Beta"}, {"C", "Alpha"}, {"D", "Imported"}, {"E", "Epsilon"}};
    const Names n = plan_names(
        addrs, [&](const std::string& a) { return candidate.contains(a) ? candidate.at(a) : std::string(); }, {{"USER_9", "Imported"}}, {{"B", "Exported"}}, recovered);
    // A gets Alpha; B has a real name already; C would be Alpha again; D's name is an import's; E has no name, so it is sub_E
    CB_CHECK((n.rename == std::map<std::string, std::string>{{"sub_A", "Alpha"}, {"sub_E", "Epsilon"}}));
    CB_CHECK((n.all == std::map<std::string, std::string>{{"USER_9", "Imported"}, {"sub_A", "Alpha"}, {"sub_E", "Epsilon"}}));
});

// ---- the files ----------------------------------------------------------------------------------------------------

constexpr std::string_view kModule = "m0d";
constexpr std::string_view kProfile = "prof1";

// The readable pass of the tests: it marks what it was given.
class MarkingPass final : public ReadablePass {
public:
    explicit MarkingPass(const translate::l2::Made& stored) : texts_(stored.texts()) {}
    std::string function(const std::string& address) override { return "/* readable */ " + texts_.at(address); }
    bool results_unused(const std::string& name) const override { return name == "sub_00401010"; }
    std::string calls_hint(std::string_view text) const override { return text.find("CreateThing") != std::string_view::npos ? "it makes the thing" : ""; }
    std::string rename_globals(const std::string& header) const override { return header + "\n/* globals renamed */\n"; }
    bool constants_used() const override { return true; }
    std::string definitions() const override { return "#define WM_USER 0x400\n"; }
    std::string struct_definitions() const override { return "struct S;\n"; }

private:
    OrderedMap<std::string, std::string> texts_;
};

void fill(store::FunctionStore& store) {
    store.add_module(kModule, "demo.dll", "C:/demo.dll", "pe");
    store.add_profile(kProfile, "settings");
    const std::vector<std::string> addrs = {"00401000", "00401010", "00401020", "00401030", "00401040", "00401050", "00401060"};
    store.add_functions(kModule, addrs);
    std::vector<store::Record> rows;
    rows.push_back({.addr = "00401000", .name = "DllMain", .source = "int DllMain(void) { return USER_91(1); }", .verdict = "equivalent", .limits = "coverage=100"});
    rows.push_back({.addr = "00401010", .name = "sub_00401010", .source = "void sub_00401010(void) { sub_00401020(); }", .verdict = "divergent", .reason = "ret-differs"});
    rows.push_back({.addr = "00401020", .name = "sub_00401020", .source = "this is not C: sub_00401010();", .verdict = "no-compile", .reason = "error C2143: syntax error"});
    rows.push_back({.addr = "00401040", .name = "sub_00401040", .verdict = "no-source", .reason = "the decompiler gave nothing"});
    rows.push_back({.addr = "00401050", .name = "_memcpy", .source = "void *_memcpy(void *a) { return a; }", .verdict = "equivalent", .limits = "coverage=100"});   // identified, and verified
    store.record_many(kModule, "core", "r1", kProfile, rows);
    store.set_identified(kModule, {{"00401030", {"_strlen", "LIBC"}}, {"00401050", {"_memcpy", "LIBC"}}});
    store.put_text(kModule, "core", "header", "typedef int HANDLE_T;\nint g_data;\n");
}

Prepared prepared() {
    Prepared p;
    p.name = "demo.dll";
    p.stem = "demo";
    p.module = std::string(kModule);
    p.profile = std::string(kProfile);
    p.date = "2026-11-12";
    p.source_engine = "core";
    p.flat = true;
    p.exported = {{"00401000", "_DllMain@12"}};
    p.found_imports = {{"USER_91", "GetDlgItem"}};
    p.recovered = {{"00401010", "CreateThing"}};
    p.globals_note = [](const std::string&) { return std::string("\n/* globals: g_data, read by 2 */\n"); };
    p.address_key = [](core16::Va a) { return std::format("{:08x}", a); };
    p.readable = [](const translate::l2::Made& stored) { return std::unique_ptr<ReadablePass>(std::make_unique<MarkingPass>(stored)); };
    return p;
}

std::string golden(std::string_view name) {
    return text::replace_all(fs::read_text(testing::data_path("source/testdata/writers/" + std::string(name))), "\r\n", "\n");
}

// The text the writer wrote against the golden file. (CB_UPDATE_GOLDEN=1 writes the file instead, to be read through before it is checked in.)
void check_golden(std::string_view name, const std::string& actual) {
    if (fs::get_env("CB_UPDATE_GOLDEN")) fs::write_file(testing::data_path("source/testdata/writers/" + std::string(name)), as_bytes(actual));
    CB_CHECK_EQ(actual, golden(name));
}

CB_TEST("source.module_writer.a_module_is_written_as_c_with_every_function_under_its_status", [] {
    fs::ScopedTempDir dir("cb_modw");
    store::FunctionStore store(dir.path() / "functions.sqlite", dir.path() / "db");
    fill(store);
    std::vector<std::string> log;
    const Written w = write_module(prepared(), store, dir.path() / "out", 0, [&](std::string_view l) { log.emplace_back(l); });
    CB_CHECK(w.file == dir.path() / "out" / "demo.c");
    CB_CHECK_EQ(w.total, 7u);
    // counted by status: 00401000 verified; 00401010 unverified; 00401020 does not compile; 00401030 identified (no row);
    // 00401040 no source (a row, but nothing written); 00401050 identified AND verified, which is counted as verified; 00401060 no row
    CB_CHECK_EQ(w.counts.at(store::Class::Verified), 2);
    CB_CHECK_EQ(w.counts.at(store::Class::Unverified), 1);
    CB_CHECK_EQ(w.counts.at(store::Class::NoCompile), 1);
    CB_CHECK_EQ(w.counts.at(store::Class::Identified), 1);
    CB_CHECK_EQ(w.counts.at(store::Class::NoSource), 2);
    CB_CHECK_EQ(w.counts.at(store::Class::Weak), 0);
    check_golden("module.golden.c", fs::read_text(w.file));
    check_golden("module.golden.h", fs::read_text(dir.path() / "out" / "demo.h"));
    CB_CHECK(!std::filesystem::exists(dir.path() / "out" / "demo_classes.hpp"));   // no class, no file
    CB_CHECK_EQ(log.size(), 1u);
    CB_CHECK(log[0].ends_with(": VERIFIED 2, IDENTIFIED (runtime library) 1, UNVERIFIED 1, DOES NOT COMPILE 1, NO SOURCE 2"));
});

CB_TEST("source.module_writer.the_golden_names_what_it_should", [] {
    // Read for what it says: the import by number is written by its real name wherever it stands; the placeholder is the recovered name;
    // the status of each function is above it; the identified function with no source is said and not counted a failure.
    const std::string c = golden("module.golden.c");
    for (const char* line :
         {"/* [VERIFIED] DllMain  @ 00401000 -- 100% of it exercised; exported as _DllMain@12:", "/* readable */ int DllMain(void) { return GetDlgItem(1); }",
          "CreateThing  @ 00401010", "it makes the thing", "no caller here uses what it returns", "returns a different value", "DOES NOT COMPILE",
          "[IDENTIFIED (runtime library)] _strlen  @ 00401030 -- LIBC; not measured on the current verifier", "[NO SOURCE] sub_00401040  @ 00401040 -- the decompiler gave nothing", "[NO SOURCE] @ 00401060 -- not measured on the current verifier",
          "/* (no reconstruction) */"}) {
        CB_CHECK(c.find(line) != std::string::npos);
    }
    CB_CHECK(c.find("USER_91") == std::string::npos);   // (the unreplaced name does not appear)
    const std::string h = golden("module.golden.h");
    for (const char* line : {"/* demo.dll -- types, declarations and globals used by the functions in demo.c.", "typedef int HANDLE_T;", "/* imports by number:", " *   GetDlgItem",
                             "USER.91", "/* globals: g_data, read by 2 */", "/* globals renamed */", "/* SDK constants written into demo.c", "#define WM_USER 0x400", "struct S;"}) {
        CB_CHECK(h.find(line) != std::string::npos);
    }
});

CB_TEST("source.module_writer.the_classes_are_written_beside_the_c_with_each_function_under_its_name", [] {
    fs::ScopedTempDir dir("cb_modw");
    store::FunctionStore store(dir.path() / "functions.sqlite", dir.path() / "db");
    fill(store);
    Prepared p = prepared();
    core16::ClassRecord c;
    c.vtable = 0x403000;
    c.name = "CDemo";
    c.methods = {0x401000, 0x401010, 0x401030, 0x401040, 0x401234};   // verified, renamed, identified, no source, and one that only jumps to an import
    c.slots = 5;
    c.installed_by = {0x401010};
    p.classes = {c};
    p.import_behind = [](core16::Va a) -> std::optional<std::string> { return a == 0x401234 ? std::optional<std::string>("USER32.GetDlgItem") : std::nullopt; };
    std::vector<std::string> log;
    write_module(p, store, dir.path() / "out", 0, [&](std::string_view l) { log.emplace_back(l); });
    const std::string text = fs::read_text(dir.path() / "out" / "demo_classes.hpp");
    CB_CHECK(text.find("virtual int4 vf_0();     // DllMain  [VERIFIED]") != std::string::npos);
    CB_CHECK(text.find("virtual int4 vf_1();     // CreateThing  [UNVERIFIED]") != std::string::npos);   // as the file names it
    CB_CHECK(text.find("virtual int4 vf_2();     // _strlen") != std::string::npos);                   // identified: named, no verdict of its own
    CB_CHECK(text.find("virtual int4 vf_3();     // sub_00401040") != std::string::npos);
    CB_CHECK(text.find("virtual int4 vf_4();     // USER32.GetDlgItem") != std::string::npos);        // a function that is only a jump through an import
    CB_CHECK(text.find("//   constructed or destroyed in: CreateThing") != std::string::npos);
    CB_CHECK_EQ(log.size(), 2u);
    CB_CHECK(log[0].ends_with("demo_classes.hpp: the module's C++ classes"));
});

CB_TEST("source.module_writer.a_long_module_is_split_into_files", [] {
    fs::ScopedTempDir dir("cb_modw");
    store::FunctionStore store(dir.path() / "functions.sqlite", dir.path() / "db");
    fill(store);
    const Written w = write_module(prepared(), store, dir.path() / "out", 4);   // seven functions, four to a file
    CB_CHECK(w.file == dir.path() / "out" / "demo_1.c");
    CB_CHECK(std::filesystem::exists(dir.path() / "out" / "demo_2.c"));
    CB_CHECK(!std::filesystem::exists(dir.path() / "out" / "demo.c"));
});

CB_TEST("source.module_writer.a_module_with_no_header_text_has_no_header_file", [] {
    fs::ScopedTempDir dir("cb_modw");
    store::FunctionStore store(dir.path() / "functions.sqlite", dir.path() / "db");
    store.add_module(kModule, "demo.dll", "", "pe");
    store.add_functions(kModule, std::vector<std::string>{"00401000"});
    const Written w = write_module(prepared(), store, dir.path() / "out");
    CB_CHECK(!std::filesystem::exists(dir.path() / "out" / "demo.h"));
    CB_CHECK(fs::read_text(w.file).find("[NO SOURCE]") != std::string::npos);
    CB_CHECK_EQ(w.total, 1u);
});

CB_TEST("source.module_writer.a_drivers_assembly_is_one_file_with_the_status_at_its_head", [] {
    fs::ScopedTempDir dir("cb_modw");
    store::FunctionStore store(dir.path() / "functions.sqlite", dir.path() / "db");
    store.add_module(kModule, "drv.vxd", "", "le");
    store.add_profile(kProfile, "settings");
    store.add_functions(kModule, std::vector<std::string>{"00000010", "00000020"});
    store.record_many(kModule, "core", "r1", kProfile,
                      std::vector<store::Record>{{.addr = "00000010", .name = "Handler", .source = "x", .verdict = "equivalent", .limits = "coverage=100", .detail = "ran the control messages"},
                                                  {.addr = "00000020", .name = "Other", .source = "y", .verdict = "divergent", .reason = "ret-differs"}});
    store.put_text(kModule, "core", "header", "        .386p\n        mov eax, 1\n");
    Prepared p = prepared();
    p.name = "drv.vxd";
    p.stem = "drv";
    p.lane = translate::backends::module_file::Lane::Le;
    std::vector<std::string> log;
    const Written w = write_assembly(p, store, dir.path() / "out", [&](std::string_view l) { log.emplace_back(l); });
    CB_CHECK(w.file == dir.path() / "out" / "drv.asm");
    CB_CHECK_EQ(w.total, 2u);
    const std::string text = fs::read_text(w.file);
    CB_CHECK(text.find("\r\n") != std::string::npos && text.find("\n") == text.find("\r\n") + 1);   // CRLF, as an assembler on this system reads it
    CB_CHECK(text.find("drv.vxd -- reconstructed source.") != std::string::npos);
    CB_CHECK(text.find("2 entry points or functions:") != std::string::npos);
    CB_CHECK(text.find("ran the control messages") != std::string::npos);                       // what the judge said of the module as a whole
    CB_CHECK(text.find("00000020  ") != std::string::npos && text.find("UNVERIFIED") != std::string::npos);   // a function not verified is listed
    CB_CHECK(text.find("mov eax, 1") != std::string::npos);
    CB_CHECK(log.size() == 1 && log[0].ends_with(": VERIFIED 1, UNVERIFIED 1"));
});

CB_TEST("source.module_writer.the_scoreboard_lists_what_was_measured", [] {
    ScoreboardInput in;
    in.generated = "2026-11-12 09:30";
    in.settings = "pe abc, ne def";
    store::ClassCount a, b;
    for (const store::Class c : store::kClassScored) a[c] = b[c] = 0;
    a[store::Class::Verified] = 3;
    a[store::Class::Unverified] = 1;
    b[store::Class::Weak] = 2;
    b[store::Class::NoSource] = 2;
    in.modules = {{"a.dll", "pe", a}, {"b.exe", "ne", b}};
    in.corpus_32 = 10;
    in.corpus_16 = 4;
    const std::string text = scoreboard(in);
    CB_CHECK(text.starts_with("# Reconstruction scoreboard\n\nGenerated 2026-11-12 09:30 by `codebreaker scoreboard` (verifier settings `pe abc, ne def`).\n\n"));
    CB_CHECK(text.find("| a.dll | pe | 4 | 3 (75%) | 0 | 0 | 1 | 0 | 0 | 0 | not tested | not tested |\n") != std::string::npos);
    CB_CHECK(text.find("| b.exe | ne | 4 | 0 (0%) | 2 | 0 | 0 | 0 | 0 | 2 | not tested | not tested |\n") != std::string::npos);
    CB_CHECK(text.find("| **all measured** | | 8 | 3 (38%) | 2 | 0 | 1 | 0 | 0 | 2 | | |\n") != std::string::npos);   // 37.5% rounds to even, as printf does
    CB_CHECK(text.find("|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---|---|") != std::string::npos);
    CB_CHECK(text.ends_with("2 modules measured so far (the corpus has 10 32-bit, 4 16-bit).\n"));
    in.modules.clear();
    const std::string empty = scoreboard(in);
    CB_CHECK(empty.find("all measured") == std::string::npos);   // nothing measured: no total
    CB_CHECK(empty.ends_with("0 modules measured so far (the corpus has 10 32-bit, 4 16-bit).\n"));
});

}  // namespace
}  // namespace cb::source::module_writer
