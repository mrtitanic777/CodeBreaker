#include "cb/exact/modules.hpp"

#include <map>
#include <unordered_set>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/omf.hpp"

using namespace cb;
using namespace cb::exact;
namespace stdfs = std::filesystem;

// testdata/modules/src is the source folder of a small program (the fixture the Python module this one replaced was
// run on: the rows, the library's start, the parts, the combined texts and the address lists below are what it
// made of it, each combined text read through for what a compiler would be given):
//
//     MODULES   main.c 01000 (data 0100, -Ox), util.c 01200 (data 02a0), kit.obj, tables.c (data only),
//               mix.c 01300 with its functions from 01380 in the code segment OVL, last.c 01c00 (data 0300, -Gs)
//     CRTLIB    the library's part begins at 01800

namespace {

stdfs::path src_dir() { return testing::data_path("exact/testdata/modules/src"); }
std::string golden(std::string_view module) { return fs::read_text(testing::data_path("exact/testdata/modules/golden_" + std::string(module) + ".txt")); }

struct FakeToolchain final : modules::Toolchain {
    std::map<std::string, std::string> entries = {{"file_switches", "switches"}, {"segment_switch", "-NT {name}_TEXT"}, {"switches", "-Zi"}};
    mutable std::vector<std::string> sources;
    mutable modules::CompileOptions last_options;
    std::optional<Bytes> object;
    std::optional<std::string> profile(std::string_view key) const override {
        const auto it = entries.find(std::string(key));
        return it == entries.end() ? std::nullopt : std::optional<std::string>(it->second);
    }
    modules::CompileResult compile(ByteView source, const modules::CompileOptions& options) const override {
        sources.emplace_back(as_text(source));
        last_options = options;
        if (!object) return {std::nullopt, "t.c(1) : error C2065: 'x' : undeclared identifier\nfatal error: compilation terminated"};
        return {object, "ok"};
    }
};

struct FakeProject final : modules::Project {
    std::shared_ptr<FakeToolchain> tc = std::make_shared<FakeToolchain>();
    Bytes img;
    SymbolTable syms;
    std::uint32_t dgroup = 0x1C00;
    std::unordered_set<std::uint32_t> relocs;
    std::vector<std::shared_ptr<const modules::Project>> sets;
    std::map<std::string, std::string> config = {{"first_segment", "SEGONE"}};
    stdfs::path src_path = src_dir();

    stdfs::path src() const override { return src_path; }
    stdfs::path path(std::string_view name) const override { return src_path / std::string(name); }
    std::uint32_t code_end() const override { return 0; }
    std::uint32_t own_end() const override { return 0x2000; }
    std::optional<std::string> switches() const override { return std::nullopt; }
    std::optional<std::string> config_text(std::string_view key) const override {
        const auto it = config.find(std::string(key));
        return it == config.end() ? std::nullopt : std::optional<std::string>(it->second);
    }
    std::vector<std::uint32_t> code_segments() const override { return {0, 0x1100}; }
    std::shared_ptr<const modules::Toolchain> toolchain() const override { return tc; }
    std::map<std::string, Bytes> headers() const override { return {{"types.h", to_bytes("typedef int T;\n")}}; }
    std::vector<std::string> include() const override { return {"inc"}; }
    std::vector<std::shared_ptr<const modules::Project>> also_sets() const override { return sets; }
    ByteView image() const override { return img; }
    SymbolTable symbols() const override { return syms; }
    compare::Program program() const override {
        compare::Program p;
        p.dgroup = dgroup;
        p.code_base = [](std::uint32_t) { return 0u; };
        p.relocs = &relocs;
        return p;
    }
};

using Rows = std::vector<modules::Row>;

}  // namespace

CB_TEST("exact.modules.rows_and_where_each_module_ends", [] {
    FakeProject p;
    const Rows rows = modules::rows(p);
    const Rows want = {
        {"main.c", 0x1000, 0x1200, 0x100, "-Ox"},
        {"util.c", 0x1200, 0x1300, 0x2a0, std::nullopt},   // (the part of mix.c from 01300 ends it: parts are rows too)
        {"kit.obj", std::nullopt, std::nullopt, std::nullopt, std::nullopt},
        {"tables.c", std::nullopt, std::nullopt, std::nullopt, std::nullopt},
        {"mix.c", 0x1300, 0x1380, std::nullopt, std::nullopt},
        {"last.c", 0x1c00, 0x2000, 0x300, "-Gs"},           // after the library (01800): to the end of the program's own code
    };
    // (the row of mix.c's part in OVL is not a row of its own)
    CB_CHECK_EQ(rows.size(), want.size());
    for (std::size_t i = 0; i < std::min(rows.size(), want.size()); ++i) CB_CHECK(rows[i] == want[i]);
    CB_CHECK(modules::lib_start(p) == std::optional<std::uint32_t>(0x1800));
    CB_CHECK(modules::starts(p) == (std::vector<std::uint32_t>{0x1000, 0x1200, 0x1300, 0x1380, 0x1c00}));
    const auto parts = modules::parts(p, "mix.c");
    CB_CHECK_EQ(parts.size(), 1u);
    CB_CHECK((parts[0] == modules::Part{0x1380, 0x1c00, "OVL"}));
    CB_CHECK(modules::parts(p, "main.c").empty());
    // the files of 1000..1200 in address order, a function's data before and after it around it
    std::vector<std::string> names;
    for (const auto& f : modules::files_of(src_dir(), 0x1000, 0x1200)) names.push_back(f.filename().string());
    CB_CHECK(names == (std::vector<std::string>{"d01000.c", "f01000.c", "f01010.c", "e01010.c"}));
});

CB_TEST("exact.modules.no_list_no_rows", [] {
    FakeProject p;
    p.src_path = testing::data_path("exact/testdata/modules");   // (no MODULES there)
    CB_CHECK(modules::rows(p).empty());
    CB_CHECK(!modules::lib_start(p).has_value());
    CB_CHECK(modules::starts(p).empty());
});

CB_TEST("exact.modules.a_module_is_its_files_put_together", [] {
    FakeProject p;
    const auto types = modules::FunctionTypes::read(src_dir());
    CB_CHECK_EQ(types.all().size(), 2u);
    CB_CHECK_EQ(*types.all().find("f01010"), "char *");
    CB_CHECK_EQ(*types.all().find("f01200"), "unsigned long");
    for (const auto& row : modules::rows(p)) {
        const auto s = modules::source(p, row, types);
        if (row.name == "kit.obj") {
            CB_CHECK(!s.has_value());   // an object of the kit is linked as it is
            continue;
        }
        CB_CHECK(s.has_value());
        if (!s) continue;
        CB_CHECK_EQ(s->text, golden(row.name));
    }
    // the addresses of the functions, and the switches the files ask for
    const Rows rows = modules::rows(p);
    const auto main_source = modules::source(p, rows[0], types);
    CB_CHECK(main_source->addresses == (std::vector<std::uint32_t>{0x1000, 0x1010}));
    CB_CHECK(main_source->switches == (std::set<std::string>{"-Ox"}));
    const auto util = modules::source(p, rows[1], types);
    CB_CHECK(util->switches == (std::set<std::string>{"-Gs"}));
    // mix.c's functions from 01380 are in another code segment: not stand-ins, and a pragma puts them there
    const auto mix = modules::source(p, rows[4], types);
    CB_CHECK(mix->addresses == (std::vector<std::uint32_t>{0x1300}));
    CB_CHECK(mix->text.find("#pragma alloc_text(OVL, f01380)") != std::string::npos);
    CB_CHECK(mix->switches == (std::set<std::string>{"-Zi"}));   // (no file says: the profile's default)
});

CB_TEST("exact.modules.the_combined_text_in_detail", [] {
    // a text with the cases of the combiner each once: header comment, a pass's askings, includes of one file written
    // two ways, an extern twice, a stand-in of the same file's function, a forward declaration of a function that
    // returns a pointer, and a pragma before a body
    fs::ScopedTempDir dir;
    const auto a = dir.path() / "f01000.c", b = dir.path() / "f01010.c";
    fs::write_text(a, "/* switches: -Gs */\r\n/* NOTE: 1, 2 */\r\n#include <stdio.h>\r\n#include \"A.H\"\r\nextern int g;\r\nint f01000()\r\n{\r\n\treturn f01010();\r\n}\r\n\r\n/* SAMEFILE */\r\nint f01099(x)\r\n{\r\n}\r\n");
    fs::write_text(b, "/* switches: -Gs */\n#include <STDIO.H>\n#include <a.h>   /* again */\nextern int g;   /* the same */\nextern long h;\nchar *\nf01010()\n{\n\treturn 0;\n}\n");
    fs::write_text(dir.path() / "f01200.c", "char *\nf01200()\n{\n}\n");
    const auto types = modules::FunctionTypes::read(dir.path());
    const auto c = modules::combine({a, b}, "-Zi", "switches", {{b, "#pragma alloc_text(X, f01010)\n\n"}}, types);
    CB_CHECK_EQ(c.text,
                "#include <stdio.h>\n#include \"A.H\"\nextern int f01099();\nextern int g;\nextern long h;\n\n"
                "int f01000()\n{\n\treturn f01010();\n}\n\n#pragma alloc_text(X, f01010)\n\nchar *\nf01010()\n{\n\treturn 0;\n}\n");
    // (f01010 is called as f01010() in the text already, so no forward declaration is written for it)
    CB_CHECK(c.switches == (std::set<std::string>{"-Gs"}));
    // without a header tag the comments are code, and every function file counts with the default
    const auto plain = modules::combine({a}, "-Zi", std::nullopt, {}, types);
    CB_CHECK(plain.switches == (std::set<std::string>{"-Zi"}));
    CB_CHECK(plain.text.find("/* switches: -Gs */") != std::string::npos);
    CB_CHECK(plain.text.find("NOTE") == std::string::npos);   // an askings comment goes either way
});

CB_TEST("exact.modules.a_module_is_compiled_with_the_sets_options", [] {
    FakeProject p;
    const auto types = modules::FunctionTypes::read(src_dir());
    const Rows rows = modules::rows(p);
    // (an object of the kit gives nothing)
    CB_CHECK(!modules::compile_module(p, rows[2], types).object.has_value());
    // the compiler rejects: its console comes back
    const auto bad = modules::compile_module(p, rows[0], types);
    CB_CHECK(!bad.object.has_value());
    CB_CHECK(bad.console.find("C2065") != std::string::npos);
    CB_CHECK(bad.addresses == (std::vector<std::uint32_t>{0x1000, 0x1010}));
    // accepted: the module's whole text and the project's headers and include folders
    p.tc->object = Bytes{1, 2, 3};
    const auto good = modules::compile_module(p, rows[0], types);
    CB_CHECK(good.object.has_value() && *good.object == (Bytes{1, 2, 3}));
    CB_CHECK_EQ(p.tc->sources.back(), golden("main.c"));
    CB_CHECK(p.tc->last_options.headers.contains("types.h"));
    CB_CHECK(p.tc->last_options.include == (std::vector<std::string>{"inc"}));
    CB_CHECK(!p.tc->last_options.switches.has_value());
    // a whole file (tables.c) is compiled by the set whose pattern names it, with that set's toolchain
    struct Extra final : modules::Project {
        std::shared_ptr<FakeToolchain> tc = std::make_shared<FakeToolchain>();
        stdfs::path src() const override { return {}; }
        stdfs::path path(std::string_view) const override { return {}; }
        std::uint32_t code_end() const override { return 0; }
        std::uint32_t own_end() const override { return 0; }
        std::optional<std::string> switches() const override { return "-Extra"; }
        std::optional<std::string> config_text(std::string_view key) const override { return key == "sources" ? std::optional<std::string>("TABL.*\\.c") : std::nullopt; }
        std::vector<std::uint32_t> code_segments() const override { return {}; }
        std::shared_ptr<const modules::Toolchain> toolchain() const override { return tc; }
        std::map<std::string, Bytes> headers() const override { return {}; }
        std::vector<std::string> include() const override { return {"other"}; }
        std::vector<std::shared_ptr<const modules::Project>> also_sets() const override { return {}; }
        ByteView image() const override { return {}; }
        SymbolTable symbols() const override { return {}; }
        compare::Program program() const override { return {}; }
    };
    auto extra = std::make_shared<Extra>();
    extra->tc->object = Bytes{9};
    p.sets = {extra};
    const auto found = modules::set_of(p, "Tables.C");   // (case does not matter)
    CB_CHECK(found.own == extra);
    CB_CHECK(modules::set_of(p, "main.c").own == nullptr);
    const auto whole = modules::compile_module(p, rows[3], types);
    CB_CHECK(whole.object.has_value() && *whole.object == Bytes{9});
    CB_CHECK(extra->tc->last_options.include == (std::vector<std::string>{"other"}));
    CB_CHECK(extra->tc->last_options.switches == std::optional<std::string>("-Extra"));
});

CB_TEST("exact.modules.first_segment_names_the_code_before_the_next_segment", [] {
    FakeProject p;
    CB_CHECK(modules::first_segment(p, 0x1000) == std::optional<std::string>("SEGONE"));   // before 0x1100, where the next begins
    CB_CHECK(!modules::first_segment(p, 0x1200).has_value());
    CB_CHECK(!modules::first_segment(p, std::nullopt).has_value());
    p.config.clear();
    CB_CHECK(!modules::first_segment(p, 0x1000).has_value());
});

namespace {

// an object of two functions of the program (f01000, f01010), one the symbols know (other), one nothing knows
// (mystery), and four bytes of data
Bytes small_object() {
    OrderedMap<std::string, std::int64_t> publics;
    publics.insert_or_assign("_f01000", 0);
    publics.insert_or_assign("_f01010", 6);
    publics.insert_or_assign("_other", 12);
    publics.insert_or_assign("_mystery", 13);
    const Bytes code = {0x55, 0x8B, 0xEC, 0x5D, 0xC3, 0x90, 0xB8, 0x01, 0x00, 0xC3, 0x90, 0x90, 0xC3, 0xCC};
    return formats::omf::raw_object("small", code, publics, Bytes{0x11, 0x22, 0x33, 0x44});
}

FakeProject program_for_small_object() {
    FakeProject p;
    p.img.assign(0x2100, 0);
    const Bytes at1000 = {0x55, 0x8B, 0xEC, 0x5D, 0xC3, 0x90, 0xB8, 0x02, 0x00, 0xC3, 0x90, 0x90};   // f01010 has 02 where the object has 01
    std::copy(at1000.begin(), at1000.end(), p.img.begin() + 0x1000);
    p.img[0x1100] = 0xC3;
    const Bytes data = {0x11, 0x22, 0x33, 0x45};   // the last byte differs
    std::copy(data.begin(), data.end(), p.img.begin() + 0x1C00 + 0x100);
    p.syms.insert_or_assign("_other", SymPlace{SymKind::Code, 0x1100});
    p.syms.insert_or_assign("_data_thing", SymPlace{SymKind::Data, 0x40});
    p.tc->object = small_object();
    return p;
}

}  // namespace

CB_TEST("exact.modules.check_compares_every_function_and_the_data", [] {
    FakeProject p = program_for_small_object();
    const auto types = modules::FunctionTypes::read(src_dir());
    const modules::Row row = modules::rows(p).front();   // main.c: data at 0100
    const auto r = modules::check(p, row, types);
    CB_CHECK(!r.kit_object && !r.error);
    CB_CHECK(r.object.has_value());
    CB_CHECK_EQ(r.functions.size(), 4u);
    CB_CHECK_EQ(r.functions[0].name, "_f01000");
    CB_CHECK(r.functions[0].verdict == modules::Verdict::Same);
    CB_CHECK(r.functions[0].address == std::optional<std::uint32_t>(0x1000));
    CB_CHECK_EQ(r.functions[0].length, 6u);
    CB_CHECK(r.functions[1].verdict == modules::Verdict::Differs);
    CB_CHECK_EQ(r.functions[1].differing_bytes, 5u);   // (the bytes the comparer lists, as the Python did: 0, 1, 3, 4, 5)
    CB_CHECK(r.functions[2].verdict == modules::Verdict::Same);   // a name SYMBOLS knows as code, at its place
    CB_CHECK(r.functions[2].address == std::optional<std::uint32_t>(0x1100));
    CB_CHECK(r.functions[3].verdict == modules::Verdict::NoPlace);
    CB_CHECK(!r.functions[3].address.has_value());
    CB_CHECK_EQ(r.data_length, 4u);
    CB_CHECK(r.data_differences == (std::vector<std::string>{"0103: 44, the program's 45"}));
    // a data start not given: the data is not checked
    modules::Row without_data = row;
    without_data.data_start = std::nullopt;
    const auto nd = modules::check(p, without_data, types);
    CB_CHECK_EQ(nd.data_length, 0u);
    CB_CHECK(nd.data_differences.empty());
    // an object given is checked as it is, and not compiled again
    const std::size_t compiles = p.tc->sources.size();
    (void)modules::check(p, row, types, small_object());
    CB_CHECK_EQ(p.tc->sources.size(), compiles);
    // an object of the kit: nothing to check; a module that does not compile: its console
    CB_CHECK(modules::check(p, modules::rows(p)[2], types).kit_object);
    p.tc->object.reset();
    const auto failed = modules::check(p, row, types);
    CB_CHECK(failed.error.has_value() && failed.error->find("C2065") != std::string::npos);
    CB_CHECK(failed.functions.empty());
});

CB_TEST("exact.modules.the_command_prints_a_line_for_each_module", [] {
    FakeProject p = program_for_small_object();
    testing::CapturedStd cap;
    CB_CHECK_EQ(modules::run(p, {"main", "-v"}), 0);
    const std::string out = cap.out();
    CB_CHECK(out.find("main.c         01000    4 functions,   2 identical; data 0100..0104 1 differ") != std::string::npos);
    CB_CHECK(out.find("     _f01010      01010 differs 5") != std::string::npos);
    CB_CHECK(out.find("     _mystery     ? no place") != std::string::npos);
    CB_CHECK(out.find("     data 0103: 44, the program's 45") != std::string::npos);
    CB_CHECK(out.find("4 functions in the modules, 2 identical") != std::string::npos);
    CB_CHECK(out.find("util.c") == std::string::npos);   // (only what was asked for)
});
