// Tests of the pieces of `codebreaker port` that are not the port itself: the compilers it runs and reads, the source tree, the
// assembler modules, the check rounds, PORT.md, the sanitized runs' key.
//
// DIVERGES from codebreaker/port/__main__.py:
//   * the module is several (tool_compilers, tool_tree, tool_asm, tool_build, tool_report, tool_run, sanitize_run); the Python's
//     process-wide state (the far-names cache hung on a function, the data-definition results hung on a function) is the
//     objects' (`CallArgTypes`, `DataDefinitions`);
//   * a text file is read with line ends made `\n` as Python's text mode did, and written with `\n`;
//   * the compilers' `diag` patterns are matched with cb::Regex (`(?P<name>` works; \w \d \s are ASCII);
//   * the memory watch stops the process with exit code 3 as before but writes no thread dump (there is no faulthandler);
//   * a check or build result is kept in cb::base::cache (shared kinds "port-checks", "port-objects"), the old files are not read.

#include <fstream>

#include "cb/base/cache.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/port/sanitize_run.hpp"
#include "cb/port/tool_asm.hpp"
#include "cb/port/tool_build.hpp"
#include "cb/port/tool_compilers.hpp"
#include "cb/port/tool_run.hpp"
#include "cb/port/tool_report.hpp"
#include "cb/port/tool_tree.hpp"

using namespace cb;
using namespace cb::port;
using namespace cb::port::tool;

namespace {

std::filesystem::path tool_checkout() { return std::filesystem::path(CB_SOURCE_DIR).parent_path(); }

// A compiler that is `cmd /c type FILE`: its "complaints" are the file's own lines, so a test writes the complaints it wants.
Compiler type_compiler() {
    Compiler c;
    c.check = {"cmd", "/c", "type", "{src}"};
    c.diag = R"(^(?<file>[^:\n]+?):(?<line>\d+):(?<col>\d+): (?<kind>error|warning): (?<text>.*)$)";
    c.diag_regex = std::make_shared<const Regex>(c.diag);
    return c;
}

cache::Cache tool_test_cache(const std::filesystem::path& dir) {
    cache::Config config;
    config.state_dir = dir / "state";
    config.shared_dir = dir / "shared";
    return cache::Cache(config);
}

}  // namespace

// ---- compilers ---------------------------------------------------------------------------------------------------------------

CB_TEST("port.tool.compilers_of_the_real_file", [] {
    const Compilers c = load_compilers(tool_checkout() / "codebreaker" / "port" / "compilers.json");
    CB_CHECK(c.contains("gcc") && c.contains("clang") && c.contains("msvc") && c.contains("ml64"));
    CB_CHECK(!c.contains("about"));
    CB_CHECK(std::find(c.at("gcc").build.begin(), c.at("gcc").build.end(), "{srcs}") != c.at("gcc").build.end());
    CB_CHECK(!c.at("msvc").setup.empty());
    CB_CHECK(c.at("gcc").setup.empty());
    CB_CHECK(c.at("gcc").diag_regex != nullptr);
    CB_CHECK(c.at("ml64").diag_regex == nullptr);   // it has no pattern: it is only found
});

CB_TEST("port.tool.the_users_settings_change_and_add_compilers", [] {
    fs::ScopedTempDir tmp("cb-tool");
    fs::write_text(tmp.path() / "compilers.json", R"({"about": "x", "gcc": {"find": ["gcc"], "check": ["{cc}", "{src}"], "diag": "^(?<file>.*)$"}})");
    fs::write_text(tmp.path() / "settings.json",
                   R"({"port": {"compilers": {"gcc": {"find": ["mygcc"]}, "tcc": {"check": ["tcc", "{src}"], "diag": "^(?<file>x)$"}}}})");
    const Compilers c = load_compilers(tmp.path() / "compilers.json", tmp.path() / "settings.json");
    CB_CHECK((c.at("gcc").find == std::vector<std::string>{"mygcc"}));       // changed
    CB_CHECK((c.at("gcc").check == std::vector<std::string>{"{cc}", "{src}"}));   // the rest kept
    CB_CHECK((c.at("tcc").check == std::vector<std::string>{"tcc", "{src}"}));    // added
    // settings that are not JSON, or absent, change nothing
    fs::write_text(tmp.path() / "settings.json", "{ not json");
    CB_CHECK_EQ(load_compilers(tmp.path() / "compilers.json", tmp.path() / "settings.json").size(), std::size_t{1});
    CB_CHECK_EQ(load_compilers(tmp.path() / "compilers.json", tmp.path() / "nothing.json").size(), std::size_t{1});
    // a pattern that is none, a file that is missing
    fs::write_text(tmp.path() / "bad.json", R"({"gcc": {"diag": "(unclosed"}})");
    CB_CHECK_THROWS_AS(cb::FormatError, (void)load_compilers(tmp.path() / "bad.json"));
    CB_CHECK_THROWS_AS(cb::Error, (void)load_compilers(tmp.path() / "missing.json"));
});

CB_TEST("port.tool.expand_fills_the_templates", [] {
    Values v;
    v.cc = "C:/gcc.exe";
    v.inc = ".";
    v.out = "program.exe";
    v.src = "a.c";
    v.srcs = {"a.c", "b.c", "dosrt.c"};
    const std::vector<std::string> tpl = {"{cc}", "-I{inc}", "-o", "{out}", "{srcs}", "{src}", "-D{unknown}"};
    const std::vector<std::string> want = {"C:/gcc.exe", "-I.", "-o", "program.exe", "a.c", "b.c", "dosrt.c", "a.c", "-D{unknown}"};
    CB_CHECK(expand(tpl, v) == want);
    CB_CHECK(expand({}, v).empty());
});

CB_TEST("port.tool.diagnostics_of_each_compilers_words", [] {
    const Compilers c = load_compilers(tool_checkout() / "codebreaker" / "port" / "compilers.json");
    const std::string gcc =
        "stray.c:1:1: note: a note with nothing before it\n"
        "prog.c:3:5: error: 'x' undeclared (first use in this function)\n"
        "prog.c:3:5: note: each undeclared identifier is reported only once\n"
        "x.c:7:2: warning: passing argument 1 of 'f' makes pointer from integer\n"
        "x.c:7:2: note: expected 'char *' but argument is of type 'int'\n"
        "In file included from prog.c:1:\n";
    const std::vector<Diagnostic> d = diagnostics(c.at("gcc"), gcc);
    CB_CHECK_EQ(d.size(), std::size_t{2});
    CB_CHECK_EQ(d[0].file, "prog.c");   // the file's own name
    CB_CHECK_EQ(d[0].line, std::size_t{3});
    CB_CHECK_EQ(d[0].col, std::size_t{5});
    CB_CHECK_EQ(d[0].kind, "error");
    CB_CHECK_EQ(d[0].text, "'x' undeclared (first use in this function)");
    CB_CHECK_EQ(d[0].notes.size(), std::size_t{1});
    CB_CHECK_EQ(d[1].kind, "warning");
    CB_CHECK_EQ(d[1].notes[0], "expected 'char *' but argument is of type 'int'");

    // a complaint inside a macro: where the macro is used
    const auto m = diagnostics(c.at("gcc"), "a.c:10:3: error: bad\na.c:2:9: note: in expansion of macro 'M'\n");
    CB_CHECK_EQ(m.size(), std::size_t{1});
    CB_CHECK(m[0].macro_at.has_value());
    CB_CHECK_EQ(m[0].macro_at->line, std::size_t{10});
    CB_CHECK_EQ(m[0].line, std::size_t{2});
    CB_CHECK_EQ(m[0].col, std::size_t{9});

    const auto cl = diagnostics(c.at("clang"), "C:\\src\\a.c:12:5: error: use of undeclared identifier 'q'\n");
    CB_CHECK_EQ(cl.size(), std::size_t{1});
    CB_CHECK_EQ(cl[0].file, "a.c");
    CB_CHECK_EQ(cl[0].line, std::size_t{12});

    const auto ms = diagnostics(c.at("msvc"), "C:\\src\\a.c(12,5): error C2065: 'x': undeclared identifier\nb.c(3): warning C4013: 'f' undefined\n");
    CB_CHECK_EQ(ms.size(), std::size_t{2});
    CB_CHECK_EQ(ms[0].file, "a.c");
    CB_CHECK_EQ(ms[0].col, std::size_t{5});
    CB_CHECK_EQ(ms[0].text, "'x': undeclared identifier");
    CB_CHECK_EQ(ms[1].col, std::size_t{0});   // no column said
    CB_CHECK(diagnostics(c.at("ml64"), "a.c:1:1: error: x").empty());   // no pattern, no complaints
});

CB_TEST("port.tool.finding_a_compiler", [] {
    fs::ScopedTempDir tmp("cb-tool");
    fs::write_text(tmp.path() / "vs" / "17" / "bin" / "cl.exe", "x");
    fs::write_text(tmp.path() / "vs" / "19" / "bin" / "cl.exe", "x");
    fs::write_text(tmp.path() / "vs" / "18" / "bin" / "other.exe", "x");
    const std::string pattern = fs::to_text(tmp.path()) + "/vs/*/bin/cl.exe";
    const std::vector<std::filesystem::path> all = glob_reverse(pattern);
    CB_CHECK_EQ(all.size(), std::size_t{2});
    CB_CHECK(fs::to_text(all[0]).find("/19/") != std::string::npos);   // the newest first
    const std::vector<std::string> names = {"definitely-not-a-program-on-any-path", pattern};
    const auto found = find_compiler(names);
    CB_CHECK(found.has_value() && fs::to_text(*found).find("/19/") != std::string::npos);
    CB_CHECK(!find_compiler(std::vector<std::string>{"definitely-not-a-program-on-any-path", fs::to_text(tmp.path()) + "/nothing/*.exe"}).has_value());
    // a name the user gave comes first; a path that is a file is that file
    const std::string direct = fs::to_text(tmp.path() / "vs" / "17" / "bin" / "cl.exe");
    CB_CHECK(fs::to_text(*find_compiler(names, direct)).find("/17/") != std::string::npos);
    CB_CHECK(glob_reverse(fs::to_text(tmp.path()) + "/vs/1?/bin/cl.exe").size() == 2);
});

CB_TEST("port.tool.a_command_runs_with_the_compilers_folder_on_the_path", [] {
    fs::ScopedTempDir tmp("cb-tool");
    const Compiler c = type_compiler();
    fs::write_text(tmp.path() / "t.txt", "hello\n");
    const Run r = run(c, {"cmd", "/c", "type", "t.txt"}, tmp.path(), tmp.path() / "bin" / "gcc.exe");
    CB_CHECK(r.ok());
    CB_CHECK(r.output().find("hello") != std::string::npos);
    CB_CHECK(!run(c, {"cmd", "/c", "type", "nothing.txt"}, tmp.path(), tmp.path() / "bin" / "gcc.exe").ok());
});

// ---- the tree ------------------------------------------------------------------------------------------------------------------

CB_TEST("port.tool.read_tree", [] {
    fs::ScopedTempDir tmp("cb-tool");
    fs::write_text(tmp.path() / "b.c", "int b;\r\nint c;\r\n");
    fs::write_text(tmp.path() / "a.H", "#define A 1\r");
    fs::write_text(tmp.path() / "notes.txt", "not C");
    fs::write_text(tmp.path() / "src" / "b.c", "int b2;\n");   // src/ over the folder
    fs::write_text(tmp.path() / "src" / "z.c", "int z;\n");
    fs::write_text(tmp.path() / "asm" / "m.asm", "PUBLIC _x\r\n");
    fs::write_text(tmp.path() / "src" / "n.ASM", "END\n");
    const SourceTree t = read_tree(tmp.path());
    std::vector<std::string> names;
    for (const auto& [n, text] : t.c) names.push_back(n);
    CB_CHECK((names == std::vector<std::string>{"a.H", "b.c", "z.c"}));   // b.c keeps its place, src/'s text
    CB_CHECK_EQ(t.c.at("b.c"), "int b2;\n");
    CB_CHECK_EQ(t.c.at("a.H"), "#define A 1\n");                          // a lone CR is a line end too
    CB_CHECK_EQ(t.assembler.at("m.asm"), "PUBLIC _x\n");
    CB_CHECK_EQ(t.assembler.size(), std::size_t{2});
    CB_CHECK(t.assembler_paths.at("m.asm") == tmp.path() / "asm" / "m.asm");
    CB_CHECK(read_tree(tmp.path() / "absent").c.empty());
    CB_CHECK_EQ(normalize_newlines("a\r\nb\rc\n\r\nd"), "a\nb\nc\n\nd");
});

CB_TEST("port.tool.symbols_and_bss_lists", [] {
    fs::ScopedTempDir tmp("cb-tool");
    fs::write_text(tmp.path() / "src" / "SYMBOLS", "# the recovery's\n_main D 0010\nbuf   D 0020  # a buffer\nbad D zz\nfew D\nfn C 0100\n");
    const auto s = read_symbols(tmp.path());
    CB_CHECK_EQ(s.size(), std::size_t{2});
    CB_CHECK_EQ(s.at("main"), std::uint64_t{0x10});   // the C name has no leading underscore
    CB_CHECK_EQ(s.at("buf"), std::uint64_t{0x20});
    CB_CHECK(read_symbols(tmp.path() / "absent").empty());

    fs::write_text(tmp.path() / "BSS", "range 0100 0200\nhole 0120 10\n_a 0100\nb 0110\nc 0150\n# nothing\n");
    const auto b = read_bss(tmp.path());
    CB_CHECK_EQ(b.size(), std::size_t{3});
    CB_CHECK_EQ(b.at("a"), std::uint64_t{0x10});    // to the next variable
    CB_CHECK_EQ(b.at("b"), std::uint64_t{0x10});    // cut short by the hole that begins first
    CB_CHECK_EQ(b.at("c"), std::uint64_t{0xB0});    // to the end of the range
    fs::write_text(tmp.path() / "BSS", "x 0010\n");
    CB_CHECK_EQ(read_bss(tmp.path()).at("x"), std::uint64_t{2});   // no range: a word
});

CB_TEST("port.tool.data_places_beside_an_assembler_module", [] {
    fs::ScopedTempDir tmp("cb-tool");
    fs::write_text(tmp.path() / "SYMBOLS", "b D 0020\na D 0010\nx C 0030\nz D 12g\nq D 0004 extra\n");
    const auto places = data_places(tmp.path() / "m.asm");
    CB_CHECK_EQ(places.size(), std::size_t{2});
    CB_CHECK_EQ(places[0].second, "a");   // sorted by address
    CB_CHECK_EQ(places[1].first, std::uint32_t{0x20});
    CB_CHECK(data_places(tmp.path() / "sub" / "m.asm").empty());
});

// ---- assembler modules -----------------------------------------------------------------------------------------------------------

namespace {

pre_ansi::Profile asm_profile() {
    pre_ansi::Profile p;
    p.libc = {"printf", "malloc", "strlen"};
    p.platform_calls = {"dos_open"};
    p.runtime_defines = {"fopen", "exit", "malloc", "write"};
    return p;
}

}  // namespace

CB_TEST("port.tool.asm_roles", [] {
    const pre_ansi::Profile profile = asm_profile();
    // makes public what the runtime defines
    const Role runtime = asm_role("PUBLIC _exit, __cexit\n", profile);
    CB_CHECK(runtime.kind == AsmKind::Runtime);
    CB_CHECK((runtime.publics == std::vector<std::string>{"__cexit", "_exit"}));
    CB_CHECK(asm_role("PUBLIC _main\n", profile).kind == AsmKind::Runtime);
    // fopen, fprintf and malloc the program may define itself: not the runtime's by that alone
    const std::string own_malloc =
        "_TEXT SEGMENT BYTE PUBLIC 'CODE'\nPUBLIC _malloc\n_malloc PROC NEAR\n mov ax,1\n ret\n_malloc ENDP\n_TEXT ENDS\nEND\n";
    CB_CHECK(asm_role(own_malloc, profile).kind == AsmKind::Runtime);   // (and its name is the C library's: no own code)
    // the start-up code
    CB_CHECK(asm_role("_TEXT SEGMENT\nstart:\n ret\n_TEXT ENDS\nEND start\n", profile).kind == AsmKind::Runtime);
    // data alone
    const std::string data = "_DATA SEGMENT WORD PUBLIC 'DATA'\nPUBLIC _tab\n_tab LABEL WORD\n DW 1,2,3\n_DATA ENDS\nEND\n";
    CB_CHECK(asm_role(data, profile).kind == AsmKind::Data);
    // the program's own code
    const std::string code =
        "_TEXT SEGMENT BYTE PUBLIC 'CODE'\nPUBLIC _fast, BIGCALL\n_fast PROC NEAR\n mov ax,1\n ret\n_fast ENDP\n_TEXT ENDS\nEND\n";
    const Role own = asm_role(code, profile);
    CB_CHECK(own.kind == AsmKind::Code);
    CB_CHECK((own.publics == std::vector<std::string>{"BIGCALL", "_fast"}));
    CB_CHECK_EQ(asm_kind_name(AsmKind::Code), "code");
    // code whose every public name is the library's
    const std::string lib =
        "_TEXT SEGMENT BYTE PUBLIC 'CODE'\nPUBLIC _strlen\n_strlen PROC NEAR\n mov ax,1\n ret\n_strlen ENDP\n_TEXT ENDS\nEND\n";
    CB_CHECK(asm_role(lib, profile).kind == AsmKind::Runtime);
});

CB_TEST("port.tool.c_declared_is_what_the_c_expects_of_the_assembly", [] {
    const std::vector<std::string> texts = {
        "#define NEAR near\n#define FAR far pascal\nextern int NEAR foo();\nextern char * bar();\nextern long baz();\nextern unsigned qux();\n"
        "extern void v();\nextern struct s *sp();\nextern int FAR calls(int);\n/* extern int hidden(); */\n"};
    const auto d = c_declared(texts);
    CB_CHECK_EQ(d.at("foo"), "int16_t");
    CB_CHECK_EQ(d.at("bar"), "char *");
    CB_CHECK_EQ(d.at("baz"), "int32_t");
    CB_CHECK_EQ(d.at("qux"), "uint16_t");
    CB_CHECK_EQ(d.at("v"), "void");
    CB_CHECK_EQ(d.at("sp"), "struct s *");
    CB_CHECK(!d.contains("calls"));    // it has arguments declared
    CB_CHECK(!d.contains("hidden"));   // in a comment
    // the first declaration wins
    CB_CHECK_EQ(c_declared(std::vector<std::string>{"extern int f();\n", "extern char f();\n"}).at("f"), "int16_t");
});

CB_TEST("port.tool.the_old_runtimes_straight_routines_are_inlined", [] {
    OrderedMap<std::string, std::string> runtime;
    runtime.insert_or_assign("rt.asm",
                             "PUBLIC cret, __other, straight\n"
                             "cret PROC NEAR\n lea sp,[bp-4]\n pop si\n pop di\n pop bp\n ret\ncret ENDP\n"
                             "__other PROC NEAR\n call somewhere\n ret\n__other ENDP\n"
                             "hidden PROC NEAR\n ret\nhidden ENDP\n"
                             "straight:\n ; a comment\n xor ax,ax\n ret\n");
    const auto tails = runtime_tails(runtime);
    CB_CHECK_EQ(tails.size(), std::size_t{2});   // __other calls, hidden is not public
    CB_CHECK((tails.at("cret") == std::vector<std::string>{"\tlea sp,[bp-4]", "\tpop si", "\tpop di", "\tpop bp", "\tret"}));
    CB_CHECK((tails.at("straight") == std::vector<std::string>{"\txor ax,ax", "\tret"}));

    std::vector<std::string> notes;
    const std::string code = "_foo PROC NEAR\n mov ax,1\n jmp cret\n_foo ENDP\n jmp short straight  ; again\n jmp elsewhere\n";
    const std::string made = inline_tails(code, tails, notes, "m.asm");
    CB_CHECK_EQ(made,
                "_foo PROC NEAR\n mov ax,1\n\t; (JMP cret - the old runtime's: its instructions here)\n\tlea sp,[bp-4]\n\tpop si\n\tpop di\n\tpop bp\n\tret\n"
                "_foo ENDP\n\t; (JMP straight - the old runtime's: its instructions here)\n\txor ax,ax\n\tret\n jmp elsewhere\n");
    CB_CHECK_EQ(notes.size(), std::size_t{1});
    CB_CHECK_EQ(notes[0], "m.asm: 2 jump(s) into the old runtime's own ends (cret): their instructions in place");
    // nothing to inline: the text as it is
    notes.clear();
    CB_CHECK_EQ(inline_tails(code, {}, notes, "m.asm"), code);
    CB_CHECK_EQ(inline_tails("jmp elsewhere\n", tails, notes, "m.asm"), "jmp elsewhere\n");
    CB_CHECK(notes.empty());
});

CB_TEST("port.tool.data_alone_is_defined_in_c_with_the_programs_types", [] {
    const std::vector<std::string> c = {"int16_t tab[3];\nchar c;\n"};
    const asm16::CTypes ct(c, c);
    const std::string module =
        "_DATA SEGMENT WORD PUBLIC 'DATA'\nPUBLIC _tab, _c, _unknown\n"
        "_tab LABEL WORD\n DW 1,2,3\n"
        "_c LABEL BYTE\n DB 7\n"
        "_unknown LABEL BYTE\n DB 4 DUP (0)\n"
        "_DATA ENDS\nEND\n";
    const DataDefinitions d = asm_data_defs(module, ct);
    CB_CHECK_EQ(d.definitions.size(), std::size_t{3});
    CB_CHECK_EQ(d.definitions[0], "int16_t tab[3];");
    CB_CHECK_EQ(d.definitions[1], "char c;");
    CB_CHECK_EQ(d.definitions[2], "char unknown[4];\t\t/* (no C declaration says its type) */");
    CB_CHECK_EQ(d.sizes.at("tab"), std::uint64_t{6});
    CB_CHECK_EQ(d.sizes.at("unknown"), std::uint64_t{4});
    CB_CHECK(d.one_place.empty() && d.aliases.empty());
});

CB_TEST("port.tool.two_names_of_one_place_are_one_variable", [] {
    const std::vector<std::string> c = {"int16_t word;\nint16_t *ptr;\n"};
    const asm16::CTypes ct(c, c);
    const std::string module =
        "_DATA SEGMENT WORD PUBLIC 'DATA'\nPUBLIC _word, _ptr\n_word LABEL WORD\n_ptr LABEL WORD\n DW 0\n_DATA ENDS\nEND\n";
    const DataDefinitions d = asm_data_defs(module, ct);
    CB_CHECK_EQ(d.one_place.size(), std::size_t{1});
    CB_CHECK_EQ(d.one_place.at("word"), "ptr");   // the pointer's name is the one kept
});

// ---- the checks --------------------------------------------------------------------------------------------------------------------

CB_TEST("port.tool.check_keys_follow_the_includes", [] {
    const Compiler spec = type_compiler();
    OrderedMap<std::string, std::string> t;
    t.insert_or_assign("a.c", "#include \"a.h\"\nint a;\n");
    t.insert_or_assign("a.h", "#include \"b.h\"\n");
    t.insert_or_assign("b.h", "int b;\n");
    t.insert_or_assign("c.c", "int c;\n");
    const std::filesystem::path cc = "C:/gcc/bin/gcc.exe";
    const std::string key_a = check_key(spec, cc, "a.c", t);
    CB_CHECK_EQ(key_a, check_key(spec, cc, "a.c", t));
    CB_CHECK(key_a != check_key(spec, cc, "c.c", t));
    CB_CHECK(key_a != check_key(spec, "C:/other/gcc.exe", "a.c", t));
    OrderedMap<std::string, std::string> changed_deep = t;
    changed_deep.insert_or_assign("b.h", "int b2;\n");
    CB_CHECK(key_a != check_key(spec, cc, "a.c", changed_deep));                       // a header it reaches changed
    CB_CHECK_EQ(check_key(spec, cc, "c.c", t), check_key(spec, cc, "c.c", changed_deep));   // one it does not reach did not matter
    Compiler other = spec;
    other.check.push_back("-Wall");
    CB_CHECK(key_a != check_key(other, cc, "a.c", t));
});

CB_TEST("port.tool.changed_files_are_the_changed_and_those_that_reach_a_changed_header", [] {
    OrderedMap<std::string, std::string> before;
    before.insert_or_assign("a.c", "#include \"a.h\"\n");
    before.insert_or_assign("a.h", "#include \"b.h\"\n");
    before.insert_or_assign("b.h", "int b;\n");
    before.insert_or_assign("c.c", "int c;\n");
    before.insert_or_assign("d.c", "#include \"c2.h\"\n");
    before.insert_or_assign("c2.h", "int x;\n");
    const std::vector<std::string> all = {"a.c", "c.c", "d.c"};
    CB_CHECK(changed_files(before, {}, all) == all);   // nothing before: every file
    CB_CHECK(changed_files(before, before, all).empty());
    OrderedMap<std::string, std::string> now = before;
    now.insert_or_assign("b.h", "int b2;\n");
    CB_CHECK((changed_files(now, before, all) == std::vector<std::string>{"a.c"}));   // through a.h
    now = before;
    now.insert_or_assign("c.c", "int c3;\n");
    CB_CHECK((changed_files(now, before, all) == std::vector<std::string>{"c.c"}));
    now = before;
    now.insert_or_assign("new.h", "int n;\n");   // a new header nobody includes
    CB_CHECK(changed_files(now, before, all).empty());
    now = before;
    now.insert_or_assign("d.c", "#include \"new.h\"\n#include \"c2.h\"\n");
    CB_CHECK((changed_files(now, before, all) == std::vector<std::string>{"d.c"}));
});

CB_TEST("port.tool.a_check_is_kept_by_what_it_depended_on", [] {
    fs::ScopedTempDir tmp("cb-tool");
    cache::Cache cache = tool_test_cache(tmp.path());
    const Compiler spec = type_compiler();
    fs::write_text(tmp.path() / "x.c", "x.c:1:1: error: bad thing\nx.c:2:3: warning: odd\n");
    OrderedMap<std::string, std::string> texts;
    texts.insert_or_assign("x.c", "x.c:1:1: error: bad thing\nx.c:2:3: warning: odd\n");
    bool ran = false;
    const auto first = check_file(cache, spec, "C:/gcc/bin/gcc.exe", tmp.path(), "x.c", texts, ran);
    CB_CHECK(ran);
    CB_CHECK_EQ(first.size(), std::size_t{2});
    // as the last time: the answer kept, the compiler not run (the file is gone from the disk to prove it)
    std::filesystem::remove(tmp.path() / "x.c");
    const auto second = check_file(cache, spec, "C:/gcc/bin/gcc.exe", tmp.path(), "x.c", texts, ran);
    CB_CHECK(!ran);
    CB_CHECK_EQ(second.size(), std::size_t{2});
    CB_CHECK_EQ(second[1].text, "odd");
    CB_CHECK_EQ(second[1].col, std::size_t{3});
    // another text is another check
    texts.insert_or_assign("x.c", "different\n");
    CB_CHECK(check_file(cache, spec, "C:/gcc/bin/gcc.exe", tmp.path(), "x.c", texts, ran).empty());
    CB_CHECK(ran);
});

CB_TEST("port.tool.a_build_with_one_command", [] {
    fs::ScopedTempDir tmp("cb-tool");
    cache::Cache cache = tool_test_cache(tmp.path());
    const Compiler spec = type_compiler();
    fs::write_text(tmp.path() / "a.c", "int a;\n");
    const std::vector<std::string> tpl = {"cmd", "/c", "copy", "/y", "{srcs}", "{out}"};
    const BuildResult ok = build(cache, spec, "C:/gcc/bin/gcc.exe", tpl, tmp.path(), {"a.c"}, "program.exe", 2);
    CB_CHECK(ok.ok);
    CB_CHECK(std::filesystem::is_regular_file(tmp.path() / "program.exe"));
    const BuildResult bad = build(cache, spec, "C:/gcc/bin/gcc.exe", tpl, tmp.path(), {"missing.c"}, "other.exe", 2);
    CB_CHECK(!bad.ok);
    CB_CHECK(!bad.log.empty());
});

CB_TEST("port.tool.objects_are_whole_or_they_are_not_kept", [] {
    fs::ScopedTempDir tmp("cb-tool");
    CB_CHECK(!object_is_whole(tmp.path() / "none.o"));
    fs::write_text(tmp.path() / "small.o", "abc");
    CB_CHECK(!object_is_whole(tmp.path() / "small.o"));
    fs::write_file(tmp.path() / "zeros.o", Bytes(200, 0));
    CB_CHECK(!object_is_whole(tmp.path() / "zeros.o"));
    Bytes real(200, 0);
    real[3] = 0x4c;
    fs::write_file(tmp.path() / "real.o", real);
    CB_CHECK(object_is_whole(tmp.path() / "real.o"));
});

// ---- the loop of check rounds --------------------------------------------------------------------------------------------------------

namespace {

// A port whose C files are what a test says; `mend` removes the lines the compiler complained of.
struct RoundsPort {
    std::filesystem::path out;
    std::map<std::string, std::string> files;
    int mends = 0;
    bool keep_complaining = false;
    bool refuse = false;
    std::vector<std::string> said;

    Files write() {
        Files f;
        for (const auto& [n, t] : files) {
            fs::write_text(out / n, t);
            f.insert_or_assign(n, t);
        }
        return f;
    }
    bool mend(const Files& texts, const std::vector<Diagnostic>& diags) {
        ++mends;
        if (refuse) return false;
        if (keep_complaining) return true;   // says it changed something and changes nothing
        bool changed = false;
        for (const Diagnostic& d : diags) {
            std::string& t = files[d.file];
            const std::string line = d.file + ":" + std::to_string(d.line) + ":" + std::to_string(d.col) + ": " + d.kind + ": " + d.text + "\n";
            if (const std::size_t at = t.find(line); at != std::string::npos) {
                t.erase(at, line.size());
                changed = true;
            }
        }
        (void)texts;
        return changed;
    }
    CheckHooks hooks() {
        return CheckHooks{[this] { return write(); }, [this](const Files& t, const std::vector<Diagnostic>& d) { return mend(t, d); },
                          [this](const std::string& s) { said.push_back(s); }};
    }
};

}  // namespace

CB_TEST("port.tool.check_rounds_go_until_the_compiler_has_nothing_to_say", [] {
    fs::ScopedTempDir tmp("cb-tool");
    cache::Cache cache = tool_test_cache(tmp.path());
    const Compiler spec = type_compiler();
    RoundsPort port{tmp.path()};
    port.files["a.c"] = "a.c:1:1: error: one\na.c:2:1: error: two\n";
    port.files["b.c"] = "fine\n";
    port.files["x.h"] = "not checked\n";
    const CheckResult r = run_check_rounds(cache, {{&spec, "C:/gcc/bin/gcc.exe", "gcc"}}, tmp.path(), port.hooks(), 2);
    CB_CHECK_EQ(r.rounds, 2);   // the first round found two errors, the second none
    CB_CHECK(r.diagnostics.empty());
    CB_CHECK_EQ(port.mends, 2);   // the second round had nothing to mend, and said so
    CB_CHECK_EQ(port.said.size(), std::size_t{2});
    CB_CHECK(port.said[0].starts_with("port: round 1 (gcc): 2 of 2 files checked, 2 errors, 0 warnings"));
    CB_CHECK(port.said[1].starts_with("port: round 2 (gcc): 1 of 2 files checked, 0 errors, 0 warnings"));   // b.c was not checked again
    CB_CHECK(r.texts.contains("x.h"));
});

CB_TEST("port.tool.check_rounds_stop_when_the_rules_have_nothing_more", [] {
    fs::ScopedTempDir tmp("cb-tool");
    cache::Cache cache = tool_test_cache(tmp.path());
    const Compiler spec = type_compiler();
    RoundsPort port{tmp.path()};
    port.keep_complaining = true;
    port.files["a.c"] = "a.c:1:1: error: forever\n";
    const CheckResult r = run_check_rounds(cache, {{&spec, "C:/gcc/bin/gcc.exe", "gcc"}}, tmp.path(), port.hooks(), 2);
    CB_CHECK_EQ(r.rounds, 3);   // the same said three times: twice more than the first
    CB_CHECK_EQ(r.diagnostics.size(), std::size_t{1});
    // a mend that changes nothing ends it at once
    RoundsPort refuses{tmp.path()};
    refuses.refuse = true;
    refuses.files["s.c"] = "s.c:1:1: error: cannot mend\n";
    const CheckResult c = run_check_rounds(cache, {{&spec, "C:/gcc/bin/gcc.exe", "gcc"}}, tmp.path(), refuses.hooks(), 1);
    CB_CHECK_EQ(c.rounds, 1);
    CB_CHECK_EQ(c.diagnostics.size(), std::size_t{1});
});

CB_TEST("port.tool.a_second_compilers_errors_only_count", [] {
    fs::ScopedTempDir tmp("cb-tool");
    cache::Cache cache = tool_test_cache(tmp.path());
    const Compiler spec = type_compiler();
    RoundsPort port{tmp.path()};
    port.files["w.c"] = "w.c:1:1: warning: only a warning\n";
    // the first compiler mends its warning; the second (the same here) is asked after and says nothing of errors
    const CheckResult r = run_check_rounds(cache, {{&spec, "C:/gcc/bin/gcc.exe", "gcc"}, {&spec, "C:/clang/bin/clang.exe", "clang"}}, tmp.path(),
                                           port.hooks(), 2);
    CB_CHECK(r.diagnostics.empty() || r.diagnostics.front().kind == "warning");
    CB_CHECK(std::any_of(port.said.begin(), port.said.end(), [](const std::string& s) { return s.find("(clang)") != std::string::npos; }));
});

// ---- the command ----------------------------------------------------------------------------------------------------------------------

CB_TEST("port.tool.options", [] {
    const Options o = parse_options({"C:/src/prog", "C:/out/port", "--compiler", "clang", "--cases", "cases.json", "--asm16", "masm.exe"});
    CB_CHECK_EQ(o.compiler, "clang");
    CB_CHECK_EQ(o.runtime, "dos_c_early");
    CB_CHECK(!o.cc.has_value());
    CB_CHECK_EQ(fs::to_text(*o.cases), "cases.json");
    CB_CHECK_EQ(fs::to_text(o.out), "C:/out/port");
    CB_CHECK_EQ(parse_options({"a", "b", "--runtime", "dos_lattice", "--cc", "C:/gcc.exe", "--types", "t.json"}).runtime, "dos_lattice");
    CB_CHECK_THROWS_AS(cb::UsageError, (void)parse_options({"only-one"}));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)parse_options({"a", "b", "--compiler"}));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)parse_options({"a", "b", "--nonsense", "x"}));
    // the port is written beside the source, never into it
    CB_CHECK_THROWS_AS(cb::UsageError, (void)parse_options({"C:/src/prog", "C:/src/prog/port"}));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)parse_options({"C:/src/prog", "C:/SRC/prog"}));
    (void)parse_options({"C:/src/prog", "C:/src/prog2"});   // a sibling whose name begins alike is fine
    CB_CHECK(usage().starts_with("usage: codebreaker port SOURCE_DIR OUT_DIR"));
});

CB_TEST("port.tool.the_platform_layer", [] {
    fs::ScopedTempDir tmp("cb-tool");
    fs::write_text(tmp.path() / "platform" / "dosrt.h", "/* h */");
    fs::write_text(tmp.path() / "platform" / "dosrt.c", "/* c */");
    write_platform_layer(tmp.path() / "out", tmp.path() / "platform", {"dos_lattice", true, std::nullopt, true});
    CB_CHECK_EQ(fs::read_text(tmp.path() / "out" / "dosrt.h"), "/* h */");
    CB_CHECK_EQ(fs::read_text(tmp.path() / "out" / "dosrt_config.h"),
                "/* dosrt_config.h -- the old runtime's ways (made by CodeBreaker's port from the profile dos_lattice) */\n"
                "#define DOSRT_TEXT_MODE 1\n#define DOSRT_CONSOLE_TEXT 1\n#define DOSRT_EXIT_STATUS 1\n");
    // binary files by default, but the console text kept; no exit status
    write_platform_layer(tmp.path() / "out", tmp.path() / "platform", {"p", false, true, false});
    CB_CHECK_EQ(fs::read_text(tmp.path() / "out" / "dosrt_config.h"),
                "/* dosrt_config.h -- the old runtime's ways (made by CodeBreaker's port from the profile p) */\n"
                "#define DOSRT_TEXT_MODE 0\n#define DOSRT_CONSOLE_TEXT 1\n#define DOSRT_EXIT_STATUS 0\n");
    CB_CHECK_THROWS_AS(cb::Error, write_platform_layer(tmp.path() / "o2", tmp.path() / "nowhere", {}));
});

CB_TEST("port.tool.the_build_script", [] {
    const Compilers c = load_compilers(tool_checkout() / "codebreaker" / "port" / "compilers.json");
    CB_CHECK_EQ(build_script("gcc", c.at("gcc"), "gcc.exe", "program.exe", {"a.c", "b.c", "dosrt.c", "ported64.obj"}),
                "#!/bin/sh\n# the port built by gcc (made by CodeBreaker's port)\ncd \"$(dirname \"$0\")\"\n"
                "gcc.exe -std=c11 -O1 -fsigned-char -fwrapv -Wall -I. -o program.exe a.c b.c dosrt.c ported64.obj\n");
    // no compiler found: its name stands in
    CB_CHECK(build_script("clang", c.at("clang"), "", "program.exe", {"a.c"}).find("\nclang -D_CRT_SECURE_NO_WARNINGS") != std::string::npos);
});

CB_TEST("port.tool.the_memory_watch_decides_by_what_is_used", [] {
    fs::ScopedTempDir tmp("cb-tool");
    int asked = 0;
    // (the first ask is the guard's own probe: nothing there, no thread to stop the test process)
    MemoryGuard guard(tmp.path(), 1.0, [&]() -> std::optional<std::uint64_t> {
        return ++asked == 1 ? std::nullopt : std::optional<std::uint64_t>(2ull << 30);
    });
    CB_CHECK(guard.over_limit());
    MemoryGuard under(tmp.path(), 4.0, []() -> std::optional<std::uint64_t> { return std::nullopt; });
    CB_CHECK(!under.over_limit());
    CB_CHECK(MemoryGuard::committed_bytes().value_or(0) > 1'000'000);   // this process holds some memory
});

// ---- PORT.md -------------------------------------------------------------------------------------------------------------------------------

CB_TEST("port.tool.the_report", [] {
    ReportInput in;
    in.source = "C:/recovered/lib";
    in.compiler = "gcc";
    in.compiler_path = "C:/msys64/mingw64/bin/gcc.exe";
    in.left_out.insert_or_assign("stdio.h", "the old library's header");
    in.left_out.insert_or_assign("fopen.c", "defines what the C library defines");
    in.roles["rt.asm"] = {AsmKind::Runtime, {"_exit", "_main"}};
    in.roles["data.asm"] = {AsmKind::Data, {}};
    in.roles["fast.asm"] = {AsmKind::Code, {"_fast"}};
    pre_ansi::note(in.notes, "pointers kept whole", "a.c");
    pre_ansi::note(in.notes, "far words dropped", "a.c");
    pre_ansi::note(in.notes, "far words dropped", "b.c");
    for (const char* f : {"a.c", "b.c", "c.c", "d.c", "e.c"}) pre_ansi::note(in.notes, "int widths", f);
    in.assembler_notes = {"fast.asm: ported as assembly language"};
    in.sanitizer_notes = {"round 1: no fault"};
    in.behaviour.insert_or_assign("binmode", "files open binary by default");
    in.diagnostics = {{"a.c", 1, 1, "error", "'x' undeclared", {}, std::nullopt},
                      {"b.c", 2, 2, "warning", "'y' undeclared", {}, std::nullopt},
                      {"b.c", 3, 3, "warning", "unused variable 'z'", {}, std::nullopt}};
    in.rounds = 3;
    in.built = false;
    in.build_log = "link failed";
    const std::string text = render_report(in);
    CB_CHECK_EQ(text,
                "# Port of `lib`\n\n"
                "Made by CodeBreaker's port from `C:/recovered/lib` (read only), for **gcc** (`C:/msys64/mingw64/bin/gcc.exe`).\n\n"
                "**Not built.** 3 rounds of the compiler's complaints mended.\n\n"
                "## Left out: the old runtime\n\n| file | why |\n|---|---|\n"
                "| fopen.c | defines what the C library defines |\n| stdio.h | the old library's header |\n"
                "| data.asm | data alone (nothing public) |\n"
                "| fast.asm | the program's own assembly language (_fast): ported as assembly language (below) |\n"
                "| rt.asm | assembler routines of the old runtime (_exit, _main): the C library and the platform layer stand in |\n\n"
                "## Changes\n\n| change | how many | where (first) |\n|---|---|---|\n"
                "| int widths | 5 | a.c, b.c, c.c, d.c ... |\n| far words dropped | 2 | a.c, b.c |\n| pointers kept whole | 1 | a.c |\n\n"
                "## The program's assembly language\n\n- fast.asm: ported as assembly language\n\n"
                "## Run under AddressSanitizer (the verify cases)\n\n- round 1: no fault\n\n"
                "## Behaviour that may differ\n\n- **binmode**: files open binary by default\n"
                "- **int** is 16 bits in the original and 32 here: arithmetic that relied on 16-bit wrap-around differs "
                "(structure members keep 16 bits); the verifier (verify.py) is what shows it.\n\n"
                "## What the compiler still says (3)\n\n- 2 x '..' undeclared\n- 1 x unused variable '..'\n\n"
                "## Build\n\n```\nlink failed\n```\n");
});
