#include "cb/recompile/config.hpp"

#include <format>
#include <map>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
namespace cfg = cb::recompile::config;

// DIVERGES from the Python: the first line of the configuration names "recompile/config", not the .py file; an address of a switch
// file past 32 bits is a ParseError (TOML allows any integer); a [[switch]] with no case labels, and an XenonAnalyse base not in the
// code, are passed over (the Python raised, or read from the wrong end of the code).

namespace {
namespace cfgtest {

constexpr std::uint32_t kCodeVa = 0x82100000;   // where the made program of config_code.bin loads (testdata/gen_config_golden.py)

// What the generator wrote: the image's facts, then the reference's answers as "[name]" sections ("| "-led lines are text).
struct Golden {
    cfg::Title title;
    std::map<std::string, std::vector<std::string>> sections;   // lookup by name only
};

std::vector<std::string> unbar(const std::vector<std::string>& lines) {
    std::vector<std::string> out;
    for (const std::string& l : lines) out.push_back(l.size() >= 2 ? l.substr(2) : std::string());
    return out;
}

std::uint32_t hex(std::string_view s) { return *text::parse_int<std::uint32_t>(s, 16); }

const Golden& golden() {
    static const Golden g = [] {
        Golden out;
        out.title.image.code = fs::read_file(testing::data_path("recompile/testdata/config_code.bin"));
        out.title.image.code_begin = kCodeVa;
        out.title.image.code_end = kCodeVa + static_cast<std::uint32_t>(out.title.image.code.size());
        std::vector<std::string>* current = nullptr;
        const std::string body = fs::read_text(testing::data_path("recompile/testdata/config.golden.txt"));
        for (const std::string_view line : text::lines(body)) {
            if (line.empty() && !current) continue;
            if (line.starts_with('[')) {
                current = &out.sections[std::string(line.substr(1, line.size() - 2))];
                continue;
            }
            if (current) {
                current->emplace_back(line);
                continue;
            }
            if (line.starts_with('#')) continue;
            const auto w = text::split_ws(line);
            if (w[0] == "extent") out.title.image.extents[hex(w[1])] = hex(w[2]);
            else if (w[0] == "import") out.title.image.imports.insert(hex(w[1]));
            else if (w[0] == "run") out.title.run_names[hex(w[1])] = std::string(w[2]);
            else if (w[0] == "entry") out.title.image.entry = hex(w[1]);
        }
        return out;
    }();
    return g;
}

recompile::disassemble::Disassembler& dis() {
    static recompile::disassemble::Disassembler d;
    return d;
}

std::vector<std::string> address_lines(const cfg::Addresses& a) {
    std::vector<std::string> out;
    for (const auto& [key, address] : a) out.push_back(std::format("{} {:08X}", key, address));
    return out;
}

std::vector<std::string> extra_lines(const std::vector<cfg::ExtraFunction>& extra) {
    std::vector<std::string> out;
    for (const cfg::ExtraFunction& f : extra) out.push_back(std::format("{:08X} {:X}", f.address, f.size));
    return out;
}

std::uint32_t at(std::uint32_t offset) { return kCodeVa + offset; }

cfg::SwitchTables gap_switch() { return {{at(0x504), {at(0x510), at(0x520), at(0x530)}}}; }
cfg::SwitchTables both_switches() {
    cfg::SwitchTables t = gap_switch();
    t.insert_or_assign(at(0x114), std::vector<std::uint32_t>{at(0x150), at(0x160), at(0x1AC)});
    return t;
}
cfg::SwitchTables both_switches_reversed() {
    return {{at(0x114), {at(0x150), at(0x160), at(0x1AC)}}, {at(0x504), {at(0x510), at(0x520), at(0x530)}}};
}

std::string joined(const std::vector<std::string>& lines) { return text::join(lines, "\n") + "\n"; }

}  // namespace cfgtest

using namespace cfgtest;

// ---- the helpers and the jump buffer --------------------------------------------------------------------------------------

CB_TEST("recompile.config.helpers_by_name_and_by_pattern", [] {
    const Golden& g = golden();
    const cfg::Addresses found = cfg::helpers(g.title, dis());
    CB_CHECK(address_lines(found) == g.sections.at("helpers"));
    // the run names give the gpr/fpr helpers; the vector runs are found by their code (the lookalikes of 0x340 and 0x360 are not);
    // the names of the entries after the first (__savegprlr_15) are not helpers
    CB_CHECK_EQ(found.size(), std::size_t{7});
    CB_CHECK_EQ(found.at("savevmx_14_address"), at(0x300));
    CB_CHECK_EQ(found.at("restvmx_14_address"), at(0x320));
    CB_CHECK(!found.contains("savevmx_64_address"));
    CB_CHECK_EQ(found.at("savegprlr_14_address"), at(0x200));
});

CB_TEST("recompile.config.helpers_of_an_image_with_nothing", [] {
    cfg::Title empty;
    empty.image.code = Bytes(64, 0);
    empty.image.code_begin = kCodeVa;
    empty.image.code_end = kCodeVa + 64;
    CB_CHECK(cfg::helpers(empty, dis()).empty());
    CB_CHECK(cfg::jump_buffers(empty).empty());
    cfg::Title none;   // not even code
    CB_CHECK(cfg::helpers(none, dis()).empty());
});

CB_TEST("recompile.config.jump_buffers", [] {
    const Golden& g = golden();
    const cfg::Addresses found = cfg::jump_buffers(g.title);
    CB_CHECK(address_lines(found) == g.sections.at("jump_buffers"));
    CB_CHECK_EQ(found.at("setjmp_address"), at(0x400));     // no .pdata: the first word after the padding
    CB_CHECK_EQ(found.at("longjmp_address"), at(0x430));    // inside a .pdata function: its start
});

CB_TEST("recompile.config.a_longjmp_without_its_setjmp_is_not_given", [] {
    // longjmp's loads alone (the lookalike's base register aside): no setjmp_address, so no longjmp_address either
    cfg::Title t = golden().title;
    for (std::size_t i = 0; i < 4; ++i) t.image.code[0x400 + i] = 0;   // the std r1,0x90(r3) of setjmp
    CB_CHECK(cfg::jump_buffers(t).empty());
});

CB_TEST("recompile.config.start_before_and_end_of_run", [] {
    const Golden& g = golden();
    const ByteView code = g.title.image.code;
    const auto& starts = g.sections.at("start_before");
    const auto& ends = g.sections.at("end_of_run");
    CB_CHECK_EQ(starts.size(), code.size() / 4);
    for (std::size_t i = 0; i < starts.size(); ++i) {
        const auto s = text::split_ws(starts[i]);
        const auto e = text::split_ws(ends[i]);
        CB_CHECK_EQ(cfg::start_before(code, kCodeVa, hex(s[0])), hex(s[1]));
        CB_CHECK_EQ(cfg::end_of_run(code, kCodeVa, hex(e[0])), hex(e[1]));
    }
    // with no blr, bctr or branch after it a run ends where the code does
    CB_CHECK_EQ(cfg::end_of_run(code, kCodeVa, at(0x7FC)), at(0x800));
    CB_CHECK_EQ(cfg::end_of_run(ByteView(), kCodeVa, kCodeVa), kCodeVa);
});

// ---- the functions .pdata leaves out ---------------------------------------------------------------------------------------

CB_TEST("recompile.config.extra_functions", [] {
    const Golden& g = golden();
    const recompile::discover::Discovery found = recompile::discover::discover(g.title.image, dis());
    CB_CHECK(extra_lines(cfg::extra_functions(g.title, found.functions, dis())) == g.sections.at("extra_none"));
    CB_CHECK(extra_lines(cfg::extra_functions(g.title, found.functions, dis(), gap_switch())) == g.sections.at("extra_gap"));
    CB_CHECK(extra_lines(cfg::extra_functions(g.title, found.functions, dis(), both_switches())) == g.sections.at("extra_both"));
    CB_CHECK(extra_lines(cfg::extra_functions(g.title, found.functions, dis(), both_switches_reversed())) == g.sections.at("extra_both_reversed"));
});

CB_TEST("recompile.config.extra_functions_meaning", [] {
    const Golden& g = golden();
    const recompile::discover::Discovery found = recompile::discover::discover(g.title.image, dis());
    const std::vector<cfg::ExtraFunction> none = cfg::extra_functions(g.title, found.functions, dis());
    const auto has = [](const std::vector<cfg::ExtraFunction>& v, std::uint32_t a) {
        return std::ranges::any_of(v, [a](const cfg::ExtraFunction& f) { return f.address == a; });
    };
    CB_CHECK(has(none, at(0x100)));                       // called from F1 and not in .pdata
    CB_CHECK(has(none, at(0x180)) && has(none, at(0x1C0)));   // only tail-jumped to: found by following the branches
    CB_CHECK(!has(none, at(0x200)) && !has(none, at(0x240)));   // the save and restore runs are XenonRecomp's own
    CB_CHECK(!has(none, at(0x40)));                      // .pdata's
    // the switch in the gap becomes a function from the gap's start to its last case, and 0x520, called but a case label, is no start
    const std::vector<cfg::ExtraFunction> gap = cfg::extra_functions(g.title, found.functions, dis(), gap_switch());
    CB_CHECK(has(none, at(0x520)) && !has(gap, at(0x520)));
    CB_CHECK(has(gap, at(0x500)));
    for (const cfg::ExtraFunction& f : gap)
        if (f.address == at(0x500)) CB_CHECK_EQ(f.size, 0x38u);
    // a switch with no case labels is passed over
    cfg::SwitchTables empty;
    empty.insert_or_assign(at(0x504), std::vector<std::uint32_t>{});
    CB_CHECK(cfg::extra_functions(g.title, found.functions, dis(), empty) == none);
    // functions in order of address, none twice
    for (std::size_t i = 1; i < gap.size(); ++i) CB_CHECK(gap[i - 1].address < gap[i].address);
});

// ---- the configuration ------------------------------------------------------------------------------------------------------

CB_TEST("recompile.config.make", [] {
    const Golden& g = golden();
    const std::filesystem::path xex = testing::data_path("recompile/testdata/title.xex");   // (not there: only its folder is)
    // the switch tables are read from the file beside the .xex
    cfg::MakeOptions beside;
    beside.switch_tables = "config_switches.toml";
    beside.file_path = "game.xex";
    CB_CHECK_EQ(cfg::make(g.title, dis(), xex, beside), joined(unbar(g.sections.at("make_beside"))));
    // no such file: no switches (backslashes of a path become slashes)
    cfg::MakeOptions none;
    none.switch_tables = "no_such_switches.toml";
    none.out_dir = "out\\ppc";
    none.file_path = "game.xex";
    CB_CHECK_EQ(cfg::make(g.title, dis(), xex, none), joined(unbar(g.sections.at("make_no_file"))));
    // the hooks are copied as they are (trimmed)
    cfg::MakeOptions hooks = beside;
    hooks.hooks = "\n[[midasm_hook]]\nname = \"x\"\naddress = 0x82100000\n\n";
    CB_CHECK_EQ(cfg::make(g.title, dis(), xex, hooks), joined(unbar(g.sections.at("make_hooks"))));
    // the switches given are the ones used, whatever the folder holds
    cfg::MakeOptions given = beside;
    given.switch_tables = "no_such_switches.toml";
    given.switches = both_switches();
    CB_CHECK_EQ(cfg::make(g.title, dis(), xex, given),
                text::replace_all(joined(unbar(g.sections.at("make_beside"))), "config_switches.toml", "no_such_switches.toml"));
});

CB_TEST("recompile.config.make_names_the_xex_when_no_file_path_is_given", [] {
    const std::string config = cfg::make(golden().title, dis(), "C:\\games\\x\\default.xex");
    CB_CHECK(config.find("file_path = \"C:/games/x/default.xex\"\n") != std::string::npos);
    CB_CHECK(config.find("out_directory_path = \"ppc\"\n") != std::string::npos);
    CB_CHECK(config.find("switch_table_file_path = \"switch_tables.toml\"\n") != std::string::npos);
    CB_CHECK(config.starts_with("# XenonRecomp configuration, written by CodeBreaker"));
    for (const cfg::RegisterSetting& s : cfg::kRegisterSettings)
        CB_CHECK(config.find(std::format("\n{} = {}\n", s.key, s.value ? "true" : "false")) != std::string::npos);
});

// ---- the switch-table file ---------------------------------------------------------------------------------------------------

CB_TEST("recompile.config.read_switch_tables", [] {
    // what tomllib read of the same texts (gen_config_golden.py prints them): the finder's file, XenonAnalyse's format with
    // the spellings TOML allows, and a file with other tables in it
    const cfg::SwitchTables finder = cfg::read_switch_tables(testing::data_path("recompile/testdata/config_switches.toml"));
    CB_CHECK(finder == both_switches());   // (in the order of the file)
    CB_CHECK_EQ(finder.entries().front().first, at(0x504));

    const std::vector<cfg::SwitchEntry> analyse = cfg::parse_switch_entries(
        fs::read_text(testing::data_path("recompile/testdata/config_analysed.toml")));
    CB_CHECK_EQ(analyse.size(), std::size_t{4});
    CB_CHECK_EQ(analyse[0].base, at(0x500));
    CB_CHECK_EQ(analyse[0].reg.value_or(99), 11u);
    CB_CHECK_EQ(analyse[0].default_target, at(0x540));
    CB_CHECK_EQ(analyse[1].default_target, 0u);   // (no default: 0)

    const std::vector<cfg::SwitchEntry> spellings = cfg::parse_switch_entries(
        "[[switch]]\nbase = 0x82100504\nr = 11\ndefault = 0x82100540\nlabels = [0x82100510, 0x82100520]\n\n"
        "[[switch]]\nbase = 1_000\nr = 3\nlabels = [ 0x10 , 0b101,0o17,\n  42 # a comment\n , ]\n");
    CB_CHECK_EQ(spellings.size(), std::size_t{2});
    CB_CHECK_EQ(spellings[0].base, 0x82100504u);
    CB_CHECK(spellings[0].labels == (std::vector<std::uint32_t>{0x82100510, 0x82100520}));
    CB_CHECK_EQ(spellings[1].base, 1000u);
    CB_CHECK(spellings[1].labels == (std::vector<std::uint32_t>{16, 5, 15, 42}));
    CB_CHECK_EQ(spellings[1].reg.value_or(99), 3u);

    const std::vector<cfg::SwitchEntry> other = cfg::parse_switch_entries(
        "title = \"x\"\n[main]\nfile = 'y'\nok = true\nf = 1.5\n[[switch]]\nbase = 16\nlabels = []\n[tail]\nz = [1, 2]\n");
    CB_CHECK_EQ(other.size(), std::size_t{1});
    CB_CHECK_EQ(other[0].base, 16u);
    CB_CHECK(other[0].labels.empty());
    CB_CHECK(cfg::parse_switch_entries("").empty());
    CB_CHECK(cfg::parse_switch_entries("# only a comment\n").empty());
    CB_CHECK(cfg::switch_tables_of("[[switch]]\nbase = 5\nlabels = [1]\n[[switch]]\nbase = 5\nlabels = [2, 3]\n").size() == 1);   // (a base met again keeps its place, takes the last labels)
});

CB_TEST("recompile.config.read_switch_tables_errors", [] {
    CB_CHECK_THROWS_AS(ParseError, (void)cfg::parse_switch_entries("[[switch]]\nbase = \n"));
    CB_CHECK_THROWS_AS(ParseError, (void)cfg::parse_switch_entries("[[switch]]\nlabels = [1]\n"));       // no base
    CB_CHECK_THROWS_AS(ParseError, (void)cfg::parse_switch_entries("[[switch]]\nbase = 1\n"));           // no labels
    CB_CHECK_THROWS_AS(ParseError, (void)cfg::parse_switch_entries("[[switch]\nbase = 1\nlabels = []\n"));
    CB_CHECK_THROWS_AS(ParseError, (void)cfg::parse_switch_entries("[[switch]]\nbase = 1\nlabels = [1, 2\n"));
    CB_CHECK_THROWS_AS(ParseError, (void)cfg::parse_switch_entries("[[switch]]\nbase = 1 labels = []\n"));
    CB_CHECK_THROWS_AS(ParseError, (void)cfg::parse_switch_entries("[[switch]]\nbase = 1\nlabels = [\"x\n"));
    CB_CHECK_THROWS_AS(ParseError, (void)cfg::parse_switch_entries("[[switch]]\nbase = 1\nlabels = [nonsense]\n"));
    CB_CHECK_THROWS_AS(ParseError, (void)cfg::parse_switch_entries("= 5\n"));
    // DIVERGES: TOML allows any integer; an address here is 32 bits
    CB_CHECK_THROWS_AS(ParseError, (void)cfg::parse_switch_entries("[[switch]]\nbase = 0x1FFFFFFFF\nlabels = []\n"));
    CB_CHECK_THROWS_AS(ParseError, (void)cfg::parse_switch_entries("[[switch]]\nbase = 1\nlabels = [-1]\n"));
    try {
        (void)cfg::parse_switch_entries("[[switch]]\nbase = 1\nlabels = [\n  zz,\n]\n", "t.toml");
        CB_CHECK(false);
    } catch (const ParseError& e) {
        CB_CHECK_EQ(e.file(), "t.toml");
        CB_CHECK_EQ(e.line(), 4u);
    }
    CB_CHECK_THROWS_AS(IoError, (void)cfg::read_switch_tables(testing::data_path("recompile/testdata/no_such_switches.toml")));
});

CB_TEST("recompile.config.xenon_analyse_tables_are_added_for_the_bctrs_the_finder_missed", [] {
    const Golden& g = golden();
    recompile::switches::CodeImage image;
    image.base = kCodeVa;
    image.data = g.title.image.code;
    image.code_begin = kCodeVa;
    image.code_end = g.title.image.code_end;
    recompile::switches::SwitchTable found;   // the finder's one: the gap's switch
    found.bctr = at(0x504);
    found.index_reg = 11;
    found.kind = recompile::switches::TableKind::Absolute;
    found.labels = {at(0x510), at(0x520), at(0x530)};
    found.guard = at(0x500);
    const cfg::Merged merged = cfg::merge_xenon_analyse({found}, image, fs::read_text(testing::data_path("recompile/testdata/config_analysed.toml")));
    CB_CHECK_EQ(merged.found, std::size_t{1});
    CB_CHECK_EQ(merged.added, std::size_t{1});
    CB_CHECK_EQ(merged.tables.size(), std::size_t{2});
    // the file the Python wrote for the same two tables
    const auto& lines = g.sections.at("write_switch_tables");
    CB_CHECK_EQ(lines.front(), "counts 1 1");
    const std::vector<std::string> want(lines.begin() + 1, lines.end());
    CB_CHECK_EQ(recompile::switches::format_file(merged.tables), joined(unbar(want)) + "\n");   // (the generator kept the lines, less the file's last blank one)
    // (its entry made from XenonAnalyse's: the sequence's start as its bctr, guard 0)
    CB_CHECK_EQ(merged.tables[1].bctr, at(0x110));
    CB_CHECK(merged.tables[1].kind == recompile::switches::TableKind::XenonAnalyse);
    // nothing to add: nothing added
    const cfg::Merged same = cfg::merge_xenon_analyse({found}, image, "");
    CB_CHECK_EQ(same.added, std::size_t{0});
    CB_CHECK_EQ(same.tables.size(), std::size_t{1});
    // an entry with no register is a fault in the file
    CB_CHECK_THROWS_AS(ParseError, (void)cfg::merge_xenon_analyse({}, image, "[[switch]]\nbase = 0x82100110\nlabels = [1]\n"));
});

CB_TEST("recompile.config.write_switch_tables", [] {
    const Golden& g = golden();
    recompile::switches::CodeImage image;
    image.base = kCodeVa;
    image.data = g.title.image.code;
    image.code_begin = kCodeVa;
    image.code_end = g.title.image.code_end;
    fs::ScopedTempDir dir("cb-config");
    const cfg::WrittenTables plain = cfg::write_switch_tables(dir.path() / "a.toml", image);
    CB_CHECK_EQ(plain.added, std::size_t{0});
    const std::string finder_only = fs::read_text(dir.path() / "a.toml");
    CB_CHECK_EQ(finder_only, recompile::switches::format_file(recompile::switches::find(image).tables));
    const cfg::WrittenTables more = cfg::write_switch_tables(dir.path() / "b.toml", image, fs::read_text(testing::data_path("recompile/testdata/config_analysed.toml")));
    CB_CHECK_EQ(more.found, plain.found);
    CB_CHECK(more.added >= 1);
    // what is written reads back as the tables it holds
    CB_CHECK_EQ(cfg::read_switch_tables(dir.path() / "b.toml").size(), more.found + more.added);
});

}  // namespace
