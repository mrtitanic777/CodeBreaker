#include "cb/exact/data.hpp"

#include <map>
#include <unordered_set>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::exact;
namespace stdfs = std::filesystem;

// testdata/data/p0 is the program P0.EXE of the Microsoft C 3.00 sources the old exact.data was run on (the
// Microsoft-Assemblers-Compilers research, msc-3.0/recon/p0): its src folder (MODULES and the fADDR.c files), the image
// and symbols and headers of the program (fixture.txt), and the answers the real MSC 3.00 gave when the module's
// functions were compiled alone (c<N>.obj / c<N>.txt, looked up by the crc and length of the source, which proves the
// source text that is handed to the compiler is the Python's). golden.json is what the Python module made of each row:
// the walk and the drafted files, each read through for what P0's data is (strings of its functions, the sccsid
// stamps, the tables of flags and keywords, the g_ADDR names of what nothing names).
//
// DIVERGES: the draft of SVARSTCK.OBJ (a row for an object of the kit, which has code but no f files) raised IndexError;
// here its data file is named after the module (SVARSTCK-data.c) as a module without functions's is. The headers are
// read in name order, as the Python's dict and glob gave them on the machine it ran on. A struct a table names that no
// header defines gets "{ 0 }" for its value (the Python raised KeyError).

namespace {

struct FixtureToolchain final : modules::Toolchain {
    struct Recorded {
        stdfs::path object;   // empty: the compiler refused
        stdfs::path console;
    };
    std::map<std::string, std::string> entries;
    std::map<std::string, Recorded> recorded;
    bool refuse = false;
    std::optional<std::string> profile(std::string_view key) const override {
        const auto it = entries.find(std::string(key));
        return it == entries.end() ? std::nullopt : std::optional<std::string>(it->second);
    }
    modules::CompileResult compile(ByteView source, const modules::CompileOptions&) const override {
        if (refuse) return {std::nullopt, "the compiler said no\n"};
        const std::string key = std::format("{:08x}_{}", crypto::crc32(source), source.size());
        const auto it = recorded.find(key);
        CB_CHECK(it != recorded.end());
        if (it == recorded.end()) return {std::nullopt, "not recorded"};
        modules::CompileResult r;
        r.console = fs::read_text(it->second.console);
        if (!it->second.object.empty()) r.object = fs::read_file(it->second.object);
        return r;
    }
};

struct Fixture final : data::Project {
    stdfs::path dir = testing::data_path("exact/testdata/data/p0");
    stdfs::path source_dir = dir / "src";
    std::shared_ptr<FixtureToolchain> tc = std::make_shared<FixtureToolchain>();
    std::map<std::string, Bytes> header_map;
    std::vector<std::string> include_dirs;
    std::optional<std::string> switch_text;
    Bytes img;
    SymbolTable syms;
    std::uint32_t dgroup = 0;
    std::string order = "after_next_function";
    std::unordered_set<std::uint32_t> relocs;

    Fixture() {
        std::string fixture = fs::read_text(dir / "fixture.txt");
        for (std::string_view line : text::lines(fixture)) {
            if (line.empty()) continue;
            const auto f = text::split(line, '\t');
            const std::string_view k = f[0];
            if (k == "switches") switch_text = std::string(f[1].substr(1));
            else if (k == "tc_p") tc->entries[std::string(f[1])] = std::string(f[2]);
            else if (k == "order") order = std::string(f[1]);
            else if (k == "header") header_map[std::string(f[1])] = fs::read_file(dir / std::string(f[2]));
            else if (k == "image") img = fs::read_file(dir / std::string(f[1]));
            else if (k == "dgroup") dgroup = *text::parse_int<std::uint32_t>(f[1]);
            else if (k == "sym") {
                const SymKind kind = f[2] == "C" ? SymKind::Code : f[2] == "D" ? SymKind::Data : f[2] == "S" ? SymKind::Segment : SymKind::Unknown;
                syms.insert_or_assign(std::string(f[1]), SymPlace{kind, *text::parse_int<std::int64_t>(f[3])});
            } else if (k == "compile") {
                tc->recorded[std::string(f[1])] = {f[2] == "-" ? stdfs::path() : dir / std::string(f[2]), dir / std::string(f[3])};
            }
        }
        include_dirs = {fs::to_text(dir / "include")};
    }

    stdfs::path src() const override { return source_dir; }
    stdfs::path path(std::string_view name) const override { return source_dir / std::string(name); }
    std::uint32_t code_end() const override { return 0; }
    std::uint32_t own_end() const override { return 15201; }
    std::optional<std::string> switches() const override { return switch_text; }
    std::optional<std::string> config_text(std::string_view) const override { return std::nullopt; }
    std::vector<std::uint32_t> code_segments() const override { return {0}; }
    std::shared_ptr<const modules::Toolchain> toolchain() const override { return tc; }
    std::map<std::string, Bytes> headers() const override { return header_map; }
    std::vector<std::string> include() const override { return include_dirs; }
    std::vector<std::shared_ptr<const modules::Project>> also_sets() const override { return {}; }
    ByteView image() const override { return img; }
    SymbolTable symbols() const override { return syms; }
    compare::Program program() const override {
        compare::Program p;
        p.dgroup = dgroup;
        p.code_base = [](std::uint32_t) { return 0u; };
        p.relocs = &relocs;
        return p;
    }
    std::string data_order() const override { return order; }
};

Json golden() { return json::load(testing::data_path("exact/testdata/data/p0/golden.json")); }

Bytes from_hex(const std::string& h) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < h.size(); i += 2) out.push_back(static_cast<std::uint8_t>(*text::parse_int<unsigned>(h.substr(i, 2), 16)));
    return out;
}

}  // namespace

CB_TEST("exact.data.a_c_string", [] {
    CB_CHECK_EQ(data::c_string(as_bytes("")), "\"\"");
    CB_CHECK_EQ(data::c_string(as_bytes("@(#)debug.c:1.9")), "\"@(#)debug.c:1.9\"");
    CB_CHECK_EQ(data::c_string(from_hex("7b0900ff613b633b2061")), "\"{\\t\\000\\377a;c; a\"");
    CB_CHECK_EQ(data::c_string(from_hex("7b7b7d63ff636380620d7d22610dff200dff0a6261802022007d")),
                "\"{{}c\\377cc\\200b\\r}\\\"a\\r\\377 \\r\\377\\nba\\200 \\\"\\000}\"");
    CB_CHECK_EQ(data::c_string(from_hex("775c11943017")), "\"w\\\\\\021\\2240\\027\"");
    CB_CHECK_EQ(data::c_string(from_hex("7f")), "\"\\177\"");   // (DEL is not printable)
});

CB_TEST("exact.data.what_a_declaration_takes", [] {
    data::Structs structs;
    structs["rect"] = {{"int", 0, {}, "x"}, {"int", 0, {}, "y"}, {"long", 0, {}, "area"}, {"char", 0, {4}, "name"}, {"char", 1, {}, "label"}};
    structs["mix"] = {{"int", 0, {}, "a"}, {"char", 0, {3}, "b"}, {"long", 0, {}, "c"}};
    structs["nest"] = {{"struct mix", 0, {}, "m"}, {"char", 0, {}, "c"}};
    const auto size = [&](const std::string& type, const std::string& declarator) {
        return data::declared_size({{type, declarator}}, structs);
    };
    CB_CHECK_EQ(*size("int", "x"), 2u);
    CB_CHECK_EQ(*size("char", "*p"), 2u);
    CB_CHECK_EQ(*size("char", "s[8]"), 8u);
    CB_CHECK_EQ(*size("char", "*t[4]"), 8u);
    CB_CHECK_EQ(*size("long", "l"), 4u);
    CB_CHECK_EQ(*size("unsigned", "u"), 2u);
    CB_CHECK_EQ(*size("unsigned char", "c"), 1u);
    CB_CHECK_EQ(*size("FILE", "*fp"), 2u);
    CB_CHECK_EQ(*size("int", "(*f)()"), 2u);
    CB_CHECK_EQ(*size("int", "(*tab[3])()"), 6u);
    CB_CHECK_EQ(*size("double", "d[2][2]"), 32u);
    CB_CHECK_EQ(*size("int", "m[2][3]"), 12u);
    CB_CHECK(!size("int", "f(int)"));                  // (a function)
    CB_CHECK(!size("int", "tab[]"));                   // (an array of no count: its run says)
    CB_CHECK(!size("int", "m[ ]"));
    CB_CHECK(!data::declared_size({}, structs));
    // structs: members on the alignment of their size up to a word, the whole to its widest member
    CB_CHECK_EQ(*size("struct mix", "m"), 10u);        // 2 + 3 (+1) + 4
    CB_CHECK_EQ(*size("struct mix", "m[3]"), 30u);
    CB_CHECK_EQ(*size("struct mix", "*mp"), 2u);
    CB_CHECK_EQ(*size("struct rect", "r"), 14u);       // 2 + 2 + 4 + 4 + 2
    CB_CHECK_EQ(*size("struct nest", "n"), 12u);       // 10 + 1 (+1)
    CB_CHECK_EQ(*size("struct nothere", "n"), 0u);     // (a struct no header defines)
    // of a name declared more than one way, the first by (type, declarator)
    CB_CHECK_EQ(*data::declared_size({{"int", "a"}, {"char", "a"}}, structs), 1u);
    CB_CHECK_EQ(*data::declared_size({{"long", "a"}, {"int", "a[3]"}}, structs), 6u);
    CB_CHECK_EQ(*data::declared_size({{"char", "*p"}, {"char", "*p"}}, structs), 2u);
    CB_CHECK_EQ(*data::declared_size({{"int", "f(int)"}, {"long", "l"}}, structs), 4u);
});

CB_TEST("exact.data.the_externs_the_functions_declare", [] {
    fs::ScopedTempDir tmp("dataext");
    fs::write_text(tmp.path() / "f00001.c",
                   "/* f00001 */\n"
                   "extern int a;\n"
                   "extern char *p, q[10];\n"
                   "extern int f();\n"
                   "extern int (*fp)();\n"
                   "extern int (*tab[3])();\n"
                   "extern unsigned long ul;\n"
                   "extern struct foo sf, *sp;\n"
                   "extern /* a comment */ int z;\n"
                   "  extern int indented;\n"
                   "extern int arr[]; extern char zz;\n"
                   "int notextern;\n");
    fs::write_text(tmp.path() / "f00002.c", "extern int a;\nextern long a;\nextern union u un;\n");
    fs::write_text(tmp.path() / "g00003.c", "extern int ignored;\n");
    const data::Declarations d = data::externs(tmp.path());
    const auto only = [&](const std::string& name) { return d.at(name).begin()->declarator; };
    CB_CHECK_EQ(d.size(), std::size_t(10));
    CB_CHECK_EQ(d.at("a").size(), std::size_t(2));       // (int a, long a: both kept)
    CB_CHECK_EQ(only("p"), "*p");
    CB_CHECK_EQ(only("q"), "q[10]");
    CB_CHECK_EQ(d.at("q").begin()->type, "char");
    CB_CHECK_EQ(only("tab"), "(*tab[3])()");
    CB_CHECK_EQ(d.at("ul").begin()->type, "unsigned long");
    CB_CHECK_EQ(d.at("sf").begin()->type, "struct foo");
    CB_CHECK_EQ(d.at("sp").begin()->declarator, "*sp");
    CB_CHECK_EQ(d.at("z").begin()->type, "int");
    CB_CHECK_EQ(only("arr"), "arr[]; extern char zz");   // (the line is read as one declaration: as it always was)
    CB_CHECK_EQ(d.at("un").begin()->type, "union u");
    CB_CHECK(!d.contains("f") && !d.contains("fp") && !d.contains("indented") && !d.contains("notextern") && !d.contains("ignored"));
    // only the files given
    const data::Declarations some = data::externs(tmp.path(), std::vector<stdfs::path>{tmp.path() / "f00002.c"});
    CB_CHECK_EQ(some.size(), std::size_t(2));
    CB_CHECK(some.contains("a") && some.contains("un"));
    CB_CHECK_THROWS_AS(IoError, data::externs(tmp.path(), std::vector<stdfs::path>{tmp.path() / "nothere.c"}));
});

CB_TEST("exact.data.the_structs_the_headers_define", [] {
    fs::ScopedTempDir tmp("datastr");
    fs::write_text(tmp.path() / "b.h", "struct pair { unsigned int a; unsigned short b; long l; unsigned c; };\n");
    fs::write_text(tmp.path() / "a.h",
                   "/* struct hidden { int h; }; */\n"
                   "struct node { struct node *next; char *name; int kind; char tag[4]; };\n"
                   "struct inl { int one; char two[0x2], three, *four[3]; };\n"
                   "typedef struct tagged { int y; } TAGGED;\n"
                   "struct dup { int first; };\n");
    fs::write_text(tmp.path() / "c.h", "struct dup { char second; };\nstruct big { char a[16]; int n[N]; };\n");
    Fixture p;
    p.header_map.clear();
    p.header_map["own.h"] = to_bytes("struct own { int x; };\nstruct dup { long own; };\n");
    p.include_dirs = {fs::to_text(tmp.path()), fs::to_text(tmp.path() / "nodir")};
    const data::StructTable t = data::parse_structs(p);
    const auto field = [&](const std::string& s, std::size_t i) { return t.structs.at(s).at(i); };
    CB_CHECK_EQ(t.structs.at("node").size(), std::size_t(4));
    CB_CHECK_EQ(field("node", 0).base, "struct node");
    CB_CHECK_EQ(field("node", 0).stars, 1u);
    CB_CHECK_EQ(field("node", 3).name, "tag");
    CB_CHECK((field("node", 3).dims == std::vector<std::uint32_t>{4}));
    CB_CHECK_EQ(field("pair", 0).base, "unsigned int");
    CB_CHECK_EQ(field("pair", 3).base, "unsigned");
    CB_CHECK_EQ(field("pair", 3).name, "c");
    CB_CHECK((field("inl", 1).dims == std::vector<std::uint32_t>{2}));
    CB_CHECK_EQ(field("inl", 3).name, "four");
    CB_CHECK_EQ(field("inl", 3).stars, 1u);
    CB_CHECK_EQ(field("big", 0).dims.at(0), 16u);
    CB_CHECK(field("big", 1).dims.empty());              // ([N] is no number)
    CB_CHECK(!t.structs.contains("hidden") && !t.structs.contains("tagged"));
    // the last definition of a name is the one kept (include folders in order of their files), its home the first
    CB_CHECK_EQ(t.structs.at("dup").at(0).name, "second");
    CB_CHECK_EQ(t.home.at("dup"), "own.h");
    CB_CHECK_EQ(t.home.at("node"), "a.h");
    CB_CHECK_EQ(t.home.at("pair"), "b.h");
    p.header_map.clear();
    p.include_dirs.clear();
    CB_CHECK(data::parse_structs(p).structs.empty());
});

CB_TEST("exact.data.p0_where_each_modules_data_stands", [] {
    Fixture p;
    const Json g = golden();
    const auto rows = modules::rows(p);
    CB_CHECK_EQ(rows.size(), g["rows"].size());
    for (const modules::Row& row : rows) {
        const Json& want = g["walk"][row.name];
        const data::Walk w = data::walk(p, row);
        CB_CHECK_EQ(w.data_start, want["dstart"].get<std::uint32_t>());
        CB_CHECK_EQ(w.data_end, want["dend"].get<std::uint32_t>());
        CB_CHECK_EQ(w.runs.size(), want["runs"].size());
        for (std::size_t i = 0; i < w.runs.size() && i < want["runs"].size(); ++i) {
            const Json& r = want["runs"][i];
            CB_CHECK_EQ(w.runs[i].lo, r[0].get<std::uint32_t>());
            CB_CHECK_EQ(w.runs[i].hi, r[1].get<std::uint32_t>());
            CB_CHECK_EQ(w.runs[i].after, r[2].get<std::string>());
            CB_CHECK_EQ(w.runs[i].kind == data::RunKind::Strings ? "strings" : "vars", r[3].get<std::string>());
        }
        CB_CHECK_EQ(w.literals.size(), want["strs"].size());
        for (std::size_t i = 0; i < w.literals.size() && i < want["strs"].size(); ++i) {
            const Json& l = want["strs"][i];
            CB_CHECK_EQ(w.literals[i].offset, l[0].get<std::uint32_t>());
            CB_CHECK(w.literals[i].bytes == from_hex(l[1].get<std::string>()));
            CB_CHECK_EQ(w.literals[i].function.value_or("(none)"), l[2].is_null() ? std::string("(none)") : l[2].get<std::string>());
        }
        CB_CHECK_EQ(w.functions.size(), want["fn"].size());
        for (std::size_t i = 0; i < w.functions.size() && i < want["fn"].size(); ++i) {
            CB_CHECK_EQ(w.functions[i].name, want["fn"][i][0].get<std::string>());
            CB_CHECK_EQ(w.functions[i].address, want["fn"][i][1].get<std::uint32_t>());
        }
    }
});

CB_TEST("exact.data.p0_the_data_files_drafted", [] {
    Fixture p;
    const Json g = golden();
    const modules::FunctionTypes types = modules::FunctionTypes::read(p.src());
    std::size_t files = 0;
    for (const modules::Row& row : modules::rows(p)) {
        const std::vector<data::DataFile> got = data::draft(p, row, types);
        if (row.name == "SVARSTCK.OBJ") {   // DIVERGES: the Python raised IndexError here
            CB_CHECK_EQ(got.size(), std::size_t(1));
            CB_CHECK_EQ(got.at(0).name, "SVARSTCK-data.c");
            continue;
        }
        const Json& want = g["draft"][row.name];
        CB_CHECK_EQ(got.size(), want.size());
        for (std::size_t i = 0; i < got.size() && i < want.size(); ++i) {
            CB_CHECK_EQ(got[i].name, want[i][0].get<std::string>());
            CB_CHECK_EQ(got[i].text, want[i][1].get<std::string>());
            ++files;
        }
    }
    CB_CHECK(files > 10);
});

CB_TEST("exact.data.p0_the_command", [] {
    Fixture p;
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(data::run(p, {"debug.c"}), 0);
        CB_CHECK_EQ(cap.out(),
                    "debug.c: data 0052..013e, 5 literals of its functions\n"
                    "   0052..0062   16 bytes after (start)    strings\n"
                    "   007d..0080    3 bytes after _f00002    vars\n"
                    "   00fd..0100    3 bytes after _f00081    vars\n"
                    "   013d..013e    1 bytes after _f000c6    strings\n");
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(data::run(p, {"--draft", "debug.c"}), 0);
        CB_CHECK_EQ(cap.out(), "== d00002.c\nstatic char *sccsid = \"@(#)debug.c:1.9\";\n\n== d00081.c\nint g_00fe = 0;\n\n");
    }
    {   // a module named without its extension; one nothing has; a flag it does not know
        testing::CapturedStd cap;
        CB_CHECK_EQ(data::run(p, {"debug"}), 0);
        const std::string by_stem = cap.out();
        CB_CHECK(by_stem.starts_with("debug.c: data 0052..013e"));
    }
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(data::run(p, {"nosuchmodule"}), 0);
        CB_CHECK_EQ(cap.out(), "");
    }
    {   // every module, the kit's objects left out
        testing::CapturedStd cap;
        CB_CHECK_EQ(data::run(p, {"-x"}), 0);
        const std::string all = cap.out();
        CB_CHECK(all.find("p0scanner.c: data ") != std::string::npos);
        CB_CHECK(all.find("SVARSTCK") == std::string::npos);
    }
});

CB_TEST("exact.data.write_keeps_what_is_there", [] {
    Fixture p;
    fs::ScopedTempDir tmp("datawrite");
    fs::copy_tree(p.source_dir, tmp.path());
    p.source_dir = tmp.path();
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(data::run(p, {"--write", "debug.c"}), 0);
        CB_CHECK(cap.out().find("(exists") == std::string::npos);
    }
    CB_CHECK_EQ(fs::read_text(tmp.path() / "d00002.c"), "static char *sccsid = \"@(#)debug.c:1.9\";\n");
    fs::write_text(tmp.path() / "d00081.c", "int mine;\n");
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(data::run(p, {"--write", "debug.c"}), 0);
        CB_CHECK(cap.out().find("   (exists: not written)\n") != std::string::npos);
    }
    CB_CHECK_EQ(fs::read_text(tmp.path() / "d00081.c"), "int mine;\n");   // (not overwritten)
});

CB_TEST("exact.data.what_stops_a_walk_or_a_draft", [] {
    Fixture p;
    const auto rows = modules::rows(p);
    modules::Row no_data = rows.at(0);
    no_data.data_start.reset();
    CB_CHECK_THROWS_AS(data::DataError, data::walk(p, no_data));
    p.tc->refuse = true;
    try {
        (void)data::walk(p, rows.at(0));
        CB_CHECK(false);
    } catch (const data::DataError& e) {
        CB_CHECK(std::string(e.what()).starts_with("debug.c's functions do not compile alone:\nthe compiler said no"));
    }
    p.tc->refuse = false;
    p.order = "before_functions";
    CB_CHECK_THROWS_AS(data::DataError, data::draft(p, rows.at(0)));
    p.order = "after_next_function";
    p.dgroup = 0xFFFFFF;   // (a data segment past the end of the program)
    CB_CHECK_THROWS_AS(data::DataError, data::walk(p, rows.at(0)));
});
