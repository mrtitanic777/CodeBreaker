#include "cb/exact/pascal_join.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/omf.hpp"

using namespace cb;
using namespace cb::exact;
namespace stdfs = std::filesystem;

// testdata/pascal is a small program in pieces (MAIN, four procedures, one nested in P01010, a stack-check file); the
// golden_*.pas texts are what the Python module this one replaced joined from it, read through for what the 1981
// compiler is given: the used variables only, the others' headings as stubs, the stack-check metacommands.

namespace {

stdfs::path data() { return testing::data_path("exact/testdata/pascal"); }
std::string golden(const char* name) { return fs::read_text(data() / name); }

struct FakeToolchain final : pascal_join::Toolchain {
    mutable std::vector<std::string> sources;
    std::optional<Bytes> object;
    std::uint32_t helper = 3;
    pascal_join::CompileResult compile(ByteView source, std::string_view file_name) const override {
        sources.emplace_back(as_text(source));
        CB_CHECK_EQ(file_name, "T.PAS");
        if (!object) return {std::nullopt, "line 4: error 12\nfatal: out of memory"};
        return {object, ""};
    }
    std::uint32_t helper_data() const override { return helper; }
};

pascal_join::Project base_project(const FakeToolchain& tc, int parts = 2) {
    pascal_join::Project p;
    p.dir = data();
    p.config.root = ".";
    p.config.kinds = "kinds.json";
    p.config.procedures = "procedures.json";
    p.config.units["MAIN"] = 1;
    pascal_join::CompilandConfig u;
    u.dir = "main";
    u.kind = pascal_join::CompilandKind::Program;
    u.main_at = 0x1100;
    u.parts = parts;
    p.config.compilands.insert_or_assign("MAIN", u);
    p.toolchain = &tc;
    p.program.code_base = [](std::uint32_t) { return 0u; };
    return p;
}

}  // namespace

CB_TEST("exact.pascal_join.pieces_parts_and_sources", [] {
    FakeToolchain tc;
    const auto p = base_project(tc);
    const pascal_join::Compiland c(p, "MAIN", p.config.compilands.begin()->second);
    CB_CHECK(c.pieces() == (std::vector<std::string>{"p01000.pas", "p01010.pas", "p01020.pas", "p01030.pas"}));
    const auto parts = c.parts();
    CB_CHECK_EQ(parts.size(), 2u);
    CB_CHECK(parts[0] == (std::vector<std::string>{"p01000.pas", "p01010.pas"}));
    CB_CHECK(parts[1] == (std::vector<std::string>{"p01020.pas", "p01030.pas", "MAIN.pas"}));   // MAIN.pas last of the last run
    CB_CHECK_EQ(c.source(parts[0]), golden("golden_part0.pas"));
    CB_CHECK_EQ(c.source(parts[1]), golden("golden_part1.pas"));
    // a nested procedure is compared too; a {STUB} or {TODO} heading is not
    CB_CHECK(c.names_of(parts[0]) == (std::vector<std::string>{"01000", "01010", "01011"}));
    CB_CHECK(c.names_of(parts[1]) == (std::vector<std::string>{"MAIN"}));
    // one run: everything in full
    auto whole = base_project(tc, 1);
    const pascal_join::Compiland one(whole, "MAIN", whole.config.compilands.begin()->second);
    CB_CHECK_EQ(one.parts().size(), 1u);
    CB_CHECK_EQ(one.source(one.parts()[0]), golden("golden_whole.pas"));
    CB_CHECK_EQ(c.address_of("01010"), 0x1010u);
});

CB_TEST("exact.pascal_join.a_map_places_pieces_of_another_program", [] {
    fs::ScopedTempDir dir;
    fs::make_dirs(dir.path() / "main");
    fs::write_text(dir.path() / "main" / "pabcde.pas", "PROCEDURE PABCDE;\nBEGIN\nEND;\n");
    fs::write_text(dir.path() / "main" / "p00010.pas", "PROCEDURE P00010;\nBEGIN\nEND;\n");
    fs::write_text(dir.path() / "map.json", R"({"MAIN": {"ABCDE": "00500", "00010": "01010"}})");
    FakeToolchain tc;
    pascal_join::Project p;
    p.dir = dir.path();
    p.config.map = "map.json";
    pascal_join::CompilandConfig u;
    u.dir = "main";
    const pascal_join::Compiland c(p, "MAIN", u);
    CB_CHECK(c.has_map());
    CB_CHECK(c.maps("abcde"));   // (a piece's number in any case)
    CB_CHECK_EQ(c.address_of("00010"), 0x1010u);
    CB_CHECK_EQ(c.address_of("abcde"), 0x500u);
    CB_CHECK_EQ(c.address_of("beef0"), 0xbeef0u);   // not in the map: its own number
    // the pieces go in the order of the places they have here
    CB_CHECK(c.pieces() == (std::vector<std::string>{"pabcde.pas", "p00010.pas"}));
});

CB_TEST("exact.pascal_join.overlays_edits_and_removed_pieces", [] {
    fs::ScopedTempDir dir;
    fs::make_dirs(dir.path() / "ov" / "main");
    fs::make_dirs(dir.path() / "main");
    fs::write_text(dir.path() / "main" / "GLOBALS.pas", "VAR\n  G_0010: WORD; G_0012: WORD;\n");
    fs::write_text(dir.path() / "main" / "p01000.pas", "PROCEDURE P01000;\nBEGIN\n  G_0010 := 1; G_0012 := 2\nEND;\n");
    fs::write_text(dir.path() / "main" / "p01010.pas", "PROCEDURE P01010;\nBEGIN\nEND;\n");
    fs::write_text(dir.path() / "ov" / "main" / "REMOVED", "p01010.pas\n");
    fs::write_text(dir.path() / "ov" / "main" / "EDITS", "*|1;|7;\n*~|G_0012 := (\\d)|G_0012 := \\1\\1\nGLOBALS.pas|WORD|INTEGER\nGLOBALS.pas|+|  G_0014: BYTE;\nbad line\n");
    pascal_join::Project p;
    p.dir = dir.path();
    p.config.over = {"ov"};
    pascal_join::CompilandConfig u;
    u.dir = "main";
    const pascal_join::Compiland c(p, "MAIN", u);
    CB_CHECK(c.pieces() == (std::vector<std::string>{"p01000.pas"}));   // the overlay removed p01010
    const std::string src = c.source(c.parts()[0]);
    CB_CHECK(src.find("G_0010 := 7;") != std::string::npos);                 // a plain edit of every piece
    // a pattern edit, in the project's pattern: the \1 of the file is a group
    CB_CHECK(src.find("G_0012 := 22") != std::string::npos);
    // the edits of GLOBALS.pas, one declaration a line again; the added G_0014 is used by nothing, so it is left out
    CB_CHECK(src.find("VAR\n  G_0010: INTEGER;\n  G_0012: INTEGER;\n") != std::string::npos);
    CB_CHECK(src.find("G_0014") == std::string::npos);
});

CB_TEST("exact.pascal_join.run_holds_each_procedure_to_the_program", [] {
    OrderedMap<std::string, std::int64_t> publics;
    publics.insert_or_assign("P01000", 0);
    publics.insert_or_assign("P01010", 6);
    publics.insert_or_assign("MAIN", 12);
    const Bytes code = {0x55, 0x8B, 0xEC, 0x5D, 0xC3, 0x90, 0xB8, 0x01, 0x00, 0xC3, 0x90, 0x90, 0x90, 0x90, 0x90, 0xC3};
    FakeToolchain tc;
    tc.object = formats::omf::raw_object("t", code, publics);
    auto p = base_project(tc);
    Bytes image(0x1200, 0);
    std::copy(code.begin(), code.begin() + 6, image.begin() + 0x1000);
    const Bytes other = {0xB8, 0x02, 0x00, 0xC3, 0x90, 0x90};   // P01010 with 02 where the compiler has 01
    std::copy(other.begin(), other.end(), image.begin() + 0x1010);
    std::copy(code.begin() + 12, code.end(), image.begin() + 0x1100);
    p.image = image;
    p.config.compilands.find("MAIN")->equivalent = std::nullopt;

    testing::CapturedStd cap;
    const auto rows = pascal_join::run(p, true);
    const std::string out = cap.out();
    using V = pascal_join::Verdict;
    // two runs, each compiled: the first holds P01000, P01010 (P01011 is nested: not in the compiled code), the second MAIN
    CB_CHECK_EQ(tc.sources.size(), 2u);
    CB_CHECK_EQ(rows.size(), 4u);
    CB_CHECK_EQ(rows[0].label, "MAIN p01000.pas");
    CB_CHECK_EQ(*rows[0].procedure, "P01000");
    CB_CHECK(rows[0].verdict == V::Identical && rows[0].address == std::optional<std::uint32_t>(0x1000) && rows[0].length == 6);
    CB_CHECK(rows[1].verdict == V::Differs && *rows[1].procedure == "P01010");
    CB_CHECK(rows[2].verdict == V::NoPlace && *rows[2].procedure == "P01011");
    CB_CHECK(rows[2].notes == (std::vector<std::string>{"not in the compiled code"}));
    CB_CHECK_EQ(rows[3].label, "MAIN p01020.pas");
    CB_CHECK_EQ(*rows[3].procedure, "MAIN");
    CB_CHECK(rows[3].verdict == V::Identical && rows[3].address == std::optional<std::uint32_t>(0x1100));
    CB_CHECK(out.find("MAIN p01000.pas        P01000     01000 identical") != std::string::npos);
    CB_CHECK(out.find("P01010     01010 differs") != std::string::npos);
    CB_CHECK_EQ(std::string(pascal_join::verdict_text(V::Equivalent)), "equivalent");

    // a size the program's table gives that is not the compiled length is noted
    // an equivalence comparer may excuse a difference: it is told the program's bytes, the compiled ones, and the
    // offsets the linker fills, and the helper's data length
    std::vector<std::uint32_t> told;
    p.equivalents["habits:fake"] = [&](ByteView want, ByteView got, const std::set<std::uint32_t>& holes, std::uint32_t helper) {
        told = {static_cast<std::uint32_t>(want.size()), static_cast<std::uint32_t>(got.size()), static_cast<std::uint32_t>(holes.size()), helper, want[1], got[1]};
        return pascal_join::Equivalence{true, {"constants differ", "habit 7"}};
    };
    p.config.compilands.find("MAIN")->equivalent = "habits:fake";
    tc.helper = 5;
    const auto again = pascal_join::run(p);
    CB_CHECK(again[1].verdict == V::Equivalent);
    CB_CHECK(again[1].notes == (std::vector<std::string>{"the same code but for the habits of another compiler state: constants differ; habit 7"}));
    CB_CHECK(told == (std::vector<std::uint32_t>{6, 6, 0, 5, 2, 1}));
    CB_CHECK(again[0].verdict == V::Identical);   // (only a difference is asked)
    // a comparer that is not registered excuses nothing, and says so
    p.config.compilands.find("MAIN")->equivalent = "habits:missing";
    const auto unknown = pascal_join::run(p);
    CB_CHECK(unknown[1].verdict == V::Differs);
});

CB_TEST("exact.pascal_join.a_compile_that_fails_is_a_row_with_the_last_line_of_its_console", [] {
    FakeToolchain tc;   // no object
    auto p = base_project(tc);
    const auto rows = pascal_join::run(p);
    CB_CHECK_EQ(rows.size(), 2u);   // one per run
    CB_CHECK(rows[0].verdict == pascal_join::Verdict::DoesNotCompile);
    CB_CHECK(rows[0].notes == (std::vector<std::string>{"fatal: out of memory"}));
    CB_CHECK(!rows[0].procedure && !rows[0].address && rows[0].length == 0);
});

CB_TEST("exact.pascal_join.the_programs_procedures", [] {
    FakeToolchain tc;
    const auto p = base_project(tc);
    const auto procs = pascal_join::procedures(p);
    CB_CHECK_EQ(procs.size(), 3u);
    CB_CHECK(procs.at(0x1000).size == std::optional<std::uint32_t>(6));
    CB_CHECK(procs.at(0x1100).unit == std::optional<std::int64_t>(1));
    auto missing = base_project(tc);
    missing.config.procedures = "nonesuch.json";
    CB_CHECK_THROWS_AS(NotFoundError, (void)pascal_join::procedures(missing));
});
