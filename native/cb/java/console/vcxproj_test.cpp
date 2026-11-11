#include "cb/java/console/vcxproj.hpp"

#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/java/console/test_support.hpp"

namespace {

using namespace cb;
using namespace cb::java::console;
namespace stdfs = std::filesystem;
using vcxproj::FilterRule;
using vcxproj::FilterRules;

// The rules the small goldens were made with (gen_golden.py RULES).
const std::vector<FilterRule>& small_rules() {
    static const std::vector<FilterRule> rules = {
        {"^(Debug|HeapTrack)$", "Debug"}, {"^(stdafx|Common)$", "Runtime"}, {"^Block$", "Blocks\\Base"},
        {"^LiquidBlock$", "Blocks\\Liquid"}, {"^Gui\\w*", "Gui"}, {"^A&B$", "Amp & <Co>"}, {"^Zed$", "Zed"}};
    return rules;
}

// The parent maps of gen_golden.py PMAPS.
std::map<std::string, std::string> parent_map(const std::string& name) {
    if (name == "chain") return {{"Fluid", "LiquidBlock"}, {"LiquidBlock", "Block"}};
    if (name == "cycle") return {{"X", "Y"}, {"Y", "Z"}, {"Z", "X"}};
    if (name == "self") return {{"Q", "Q"}};
    if (name == "emptyparent") return {{"E", ""}};
    if (name == "deep") return {{"D1", "D2"}, {"D2", "D3"}, {"D3", "D4"}, {"D4", "GuiButton"}};
    if (name == "long") {
        std::map<std::string, std::string> m;
        for (int i = 0; i < 30; ++i) m[std::format("L{}", i)] = std::format("L{}", i + 1);
        m["L30"] = "Entity";
        return m;
    }
    return {};   // none, empty
}

// "name|pmap: filter" lines, one per name and parent map, as the goldens have them.
std::string classify_lines(const FilterRules& rules, const std::string& golden_text) {
    std::string out;
    for (std::string_view line : text::lines(golden_text)) {
        const std::size_t bar = line.find('|');
        const std::size_t colon = line.find(": ", bar);
        const std::string name(line.substr(0, bar));
        const std::string pmap(line.substr(bar + 1, colon - bar - 1));
        out += std::format("{}|{}: {}\n", name, pmap, rules.classify(name, parent_map(pmap)));
    }
    return out;
}

CB_TEST("java.console.vcxproj.classify_matches_the_original", [] {
    const std::string golden = test::golden("vcx.classify.golden.txt");
    CB_CHECK_EQ(classify_lines(FilterRules(small_rules()), golden), golden);
});

CB_TEST("java.console.vcxproj.the_real_profile_rules_classify_as_before", [] {
    // the rules of the Minecraft Xbox 360 profile, as the Python held them: every pattern compiles here and sorts
    // the same names into the same folders (a long name list over every kind of parent map)
    std::vector<FilterRule> rules;
    const std::string golden_text = test::golden("vcx.rules.golden.tsv");
    for (std::string_view line : text::lines(golden_text)) {
        const std::size_t tab = line.find('\t');
        rules.push_back({std::string(line.substr(0, tab)), std::string(line.substr(tab + 1))});
    }
    CB_CHECK(rules.size() > 40);
    const FilterRules compiled(rules);
    CB_CHECK_EQ(compiled.size(), rules.size());
    const std::string golden = test::golden("vcx.classify_real.golden.txt");
    CB_CHECK_EQ(classify_lines(compiled, golden), golden);
});

CB_TEST("java.console.vcxproj.rules_behaviour", [] {
    const FilterRules none;
    CB_CHECK_EQ(none.size(), std::size_t{0});
    CB_CHECK_EQ(none.classify("Block"), "Game (misc)");              // the generic profile has no rules
    CB_CHECK_EQ(none.classify("Block", {{"Block", "Base"}}), "Game (misc)");
    // first match wins; a pattern is anchored at the start only
    const FilterRules two({{"^A", "FirstA"}, {"^A", "SecondA"}, {"Mid", "Middle"}});
    CB_CHECK_EQ(two.classify("Abc"), "FirstA");
    CB_CHECK_EQ(two.classify("xMidx"), "Game (misc)");               // "Mid" must be at the start
    CB_CHECK_EQ(two.classify("Midway"), "Middle");
    // a class with no rule of its own takes its nearest ancestor's folder, even through a long chain
    const std::map<std::string, std::string> chain = {{"Z", "Y"}, {"Y", "X"}, {"X", "Abc"}};
    CB_CHECK_EQ(two.classify("Z", chain), "FirstA");
    // a cycle or a self-reference ends instead of looping
    CB_CHECK_EQ(two.classify("P", {{"P", "Q"}, {"Q", "P"}}), "Game (misc)");
    CB_CHECK_EQ(two.classify("S", {{"S", "S"}}), "Game (misc)");
    // a bad pattern is reported when the rules are built, not met later
    CB_CHECK_THROWS_AS(cb::ParseError, FilterRules({{"^(unclosed", "x"}}));
    // copies share the compiled rules
    FilterRules copy = two;
    CB_CHECK_EQ(copy.classify("Abc"), "FirstA");
});

CB_TEST("java.console.vcxproj.guids_and_title_ids", [] {
    std::string got;
    const std::string golden_text = test::golden("vcx.guid.golden.txt");
    for (std::string_view line : text::lines(golden_text)) {
        const std::size_t colon = line.find(": ");
        got += std::format("{}: {}\n", line.substr(0, colon), vcxproj::filter_guid(line.substr(0, colon)));
    }
    CB_CHECK_EQ(got, test::golden("vcx.guid.golden.txt"));

    CB_CHECK_EQ(*vcxproj::normalize_title_id("FFFE1234"), "FFFE1234");
    CB_CHECK_EQ(*vcxproj::normalize_title_id("0xabcdef12"), "ABCDEF12");
    CB_CHECK_EQ(*vcxproj::normalize_title_id(" 0X1f "), "0000001F");
    CB_CHECK_EQ(*vcxproj::normalize_title_id("0"), "00000000");
    for (const char* bad : {"", "0x", "123456789", "xyz", "12 34", "-1", "0x-1", "1.0", "\xC3\xA9"})
        CB_CHECK(!vcxproj::normalize_title_id(bad));
});

CB_TEST("java.console.vcxproj.title_id_matches_the_original", [] {
    // what the original said of each platform.config.json in testdata/vcx/tNN, by the kind of note it gave
    std::string got;
    const std::string golden_text = test::golden("vcx.title.golden.txt");
    for (std::string_view line : text::lines(golden_text)) {
        const std::string tag(line.substr(0, line.find(':')));
        const vcxproj::TitleId t = vcxproj::title_id_for(test::data("vcx/" + tag));
        std::string kind = t.note.empty()                                 ? "none"
                           : t.note.starts_with("title id: UNSET")        ? "UNSET"
                           : t.note.find("could not read") != std::string::npos ? "UNREADABLE"
                           : t.note.find("not 1-8 hex") != std::string::npos    ? "INVALID"
                                                                                : "?" + t.note;
        got += std::format("{}: id={} note={}\n", tag, t.id ? *t.id : std::string("None"), kind);
    }
    std::string golden = test::golden("vcx.title.golden.txt");
    // DIVERGES: the original raised AttributeError for a config that is JSON but not an object ([1,2], "str", null, 12);
    // a note that it could not be read is what the caller gets now.
    for (const char* t : {"t18", "t19", "t35", "t36"})
        golden = text::replace_all(golden, std::format("{}: EXC AttributeError", t), std::format("{}: id=None note=UNREADABLE", t));
    // DIVERGES: a UTF-8 byte-order mark in front of the JSON was an error in the original (Python's json refuses it);
    // editors write one, so it is accepted here.
    golden = text::replace_all(golden, "t20: id=None note=UNREADABLE", "t20: id=0000ABCD note=none");
    // (the generator printed a Title ID folder with no config as an empty note; the kinds above call that "none")
    golden = text::replace_all(golden, "t41: id=None note=\n", "t41: id=None note=none\n");
    CB_CHECK_EQ(got, golden);

    CB_CHECK_EQ(vcxproj::title_id_for(test::data("vcx/t05")).id.value_or(""), "00012345");       // a number counts as its digits
    const auto unset = vcxproj::title_id_for(test::data("vcx/t06"));
    CB_CHECK(unset.note.find("re-run vcxproj") != std::string::npos);
    const auto bad = vcxproj::title_id_for(test::data("vcx/t10"));
    CB_CHECK_EQ(bad.note, "title id: 'xyz' in platform.config.json is not 1-8 hex digits -- ignored");
    CB_CHECK_EQ(vcxproj::title_id_for(test::data("vcx/t51")).note,
                "title id: 12345678901234567890 in platform.config.json is not 1-8 hex digits -- ignored");
});

CB_TEST("java.console.vcxproj.parent_map_matches_the_original", [] {
    std::string got;
    for (const char* d : {"pm1", "pm_empty", "nonexistent"}) {
        got += std::string("parent_map ") + d + "\n";
        for (const auto& [child, base] : vcxproj::build_parent_map(test::data(std::string("vcx/") + d))) got += "  " + child + " => " + base + "\n";
    }
    CB_CHECK_EQ(got, test::golden("vcx.parent_map.golden.txt"));
    // the first declaration wins, and one text gives the same answer however it is cut into lines
    std::map<std::string, std::string> m;
    vcxproj::add_class_bases("class A : public B {};\r\nclass A : public C {};\r\n  class D :\r\n  E {};", m);
    CB_CHECK_EQ(m.size(), std::size_t{2});
    CB_CHECK_EQ(m["A"], "B");
    CB_CHECK_EQ(m["D"], "E");
    // DIVERGES: names are ASCII identifiers (the Python's \w also took accented letters, which no C++ class has)
    std::map<std::string, std::string> unicode;
    vcxproj::add_class_bases("class \xC3\x9Cn : public Base {};\nclass Ok : public Base_9 {};\n", unicode);
    CB_CHECK_EQ(unicode.size(), std::size_t{1});
    CB_CHECK_EQ(unicode["Ok"], "Base_9");
});

CB_TEST("java.console.vcxproj.enumerate_sources_matches_the_original", [] {
    std::string got;
    for (const char* d : {"es1", "es_empty", "nonexistent"}) {
        const auto s = vcxproj::enumerate_sources(test::data(std::string("vcx/") + d));
        got += std::format("enum {}\n  cpps={}\n  hs={}\n", d, text::join(s.cpps, "|"), text::join(s.headers, "|"));
    }
    CB_CHECK_EQ(got, test::golden("vcx.enum_sources.golden.txt"));
});

// Copy a fixture folder of testdata to a temporary one (the tests write into folders).
stdfs::path fixture_copy(const fs::ScopedTempDir& tmp, const std::string& name) {
    const stdfs::path to = tmp.path() / name;
    fs::copy_tree(test::data("vcx/" + name), to);
    return to;
}

CB_TEST("java.console.vcxproj.filters_match_the_original", [] {
    fs::ScopedTempDir tmp("cbvcx");
    const FilterRules rules(small_rules());
    // the same four cases the generator made, in its form: a heading, then the file
    auto split = [](const std::string& s) {
        std::vector<std::string> v;
        if (!s.empty()) v = [&] { std::vector<std::string> out; for (auto p : text::split(s, '|')) out.emplace_back(p); return out; }();
        return v;
    };
    struct Case {
        const char* dir;
        const char* name;
        std::vector<std::string> cpps, hs;
    };
    const std::vector<std::string> all_cpps = split("stdafx.cpp|Block.cpp|LiquidBlock.cpp|Fluid.cpp|Debug.cpp|A&B.cpp|Many.dots.here.cpp|Zed.cpp|"
                                                    "GuiMainMenu.cpp|xuimenu\\Ui.cpp|xuimenu\\Gfx\\Draw.cpp|Debug\\skipped.cpp|obj\\skipped.cpp");
    const std::vector<std::string> all_hs = split("stdafx.h|Block.h|Fluid.h|LiquidBlock.h|Debug.h|A&B.h|Many.dots.here.h|KernelConfig.h|GuiMainMenu.h|"
                                                  "xuimenu\\Ui.h|xuimenu\\stdafx.h|Release\\skipped.h");
    const Case cases[] = {{"wf1", "Proj", all_cpps, all_hs},
                          {"wf2", "Empty", {}, {}},
                          {"wf3", "Q&A", {"Block.cpp", "Debug.cpp"}, {"Block.h"}},
                          {"wf5", "Dir", {"sub\\deep\\er\\x.cpp", "Block.cpp"}, {"Block.h", "a\\b\\Block.h"}}};
    std::string got;
    for (const Case& c : cases) {
        stdfs::path dir = tmp.path() / c.dir;
        if (stdfs::exists(test::data(std::string("vcx/") + c.dir))) dir = fixture_copy(tmp, c.dir);
        else fs::make_dirs(dir);
        const auto written = vcxproj::write_filters(dir, c.name, rules, {c.cpps, c.hs});
        got += std::format("write_filters {} {} -> {} folders={}\n{}", c.dir, c.name, fs::to_text(written.path.filename()),
                           written.folders, test::lf(fs::read_text(written.path)));
        if (&c != &cases[3]) got += "\n";
    }
    CB_CHECK_EQ(got, test::golden("vcx.write_filters.golden.txt"));
    // written with CRLF, as Visual Studio writes it
    const std::string raw = fs::read_text(tmp.path() / "wf3" / "Q&A.vcxproj.filters");
    CB_CHECK(raw.find("\r\n") != std::string::npos);
    std::size_t lone_lf = 0;   // every line end is a pair
    for (std::size_t i = 0; i < raw.size(); ++i)
        if (raw[i] == '\n' && (i == 0 || raw[i - 1] != '\r')) ++lone_lf;
    CB_CHECK_EQ(lone_lf, std::size_t{0});
});

// What the golden writes of one regenerated project, as its elements: the heading, the status text, then each project
// file and a closing "  --". (The generator joined the elements with a line break.)
void run_elements(const stdfs::path& dir, const std::string& case_name, const vcxproj::Report& r, bool filters,
                  std::vector<std::string>& out) {
    out.push_back(std::format("run {} filters={}", case_name, filters ? "True" : "False"));
    std::string summary = r.summary() + "\n";
    // (the wording of a JSON read error is the parser's; the golden has Python's)
    const std::string phrase = "could not read platform.config.json (";
    if (const std::size_t paren = summary.find(phrase); paren != std::string::npos) {
        const std::size_t from = paren + phrase.size();
        summary.replace(from, summary.find(')', from) - from, "Expecting value: line 1 column 14 (char 13)");
    }
    out.push_back("  stdout: " + text::replace_all(summary, "\n", "\\n"));
    std::vector<std::string> files;
    for (const auto& e : stdfs::recursive_directory_iterator(dir))
        if (e.is_regular_file() && (e.path().extension() == ".vcxproj" || e.path().extension() == ".filters"))
            files.push_back(fs::to_text(e.path().lexically_relative(dir)));
    std::sort(files.begin(), files.end());
    for (const std::string& f : files) out.push_back("  file " + f + ":\n" + test::lf(fs::read_text(dir / f)));
    out.push_back("  --");
}

CB_TEST("java.console.vcxproj.run_matches_the_original", [] {
    fs::ScopedTempDir tmp("cbvcx");
    const FilterRules rules(small_rules());
    struct Case {
        const char* name;
        bool filters;
    };
    std::vector<std::string> elements;
    for (const Case& c : {Case{"ra", true}, Case{"rb", true}, Case{"rc", true}, Case{"rd", false}, Case{"re", false},
                          Case{"rf", true}, Case{"rg", false}, Case{"rh", true}}) {
        const stdfs::path dir = fixture_copy(tmp, std::string("run_") + c.name);
        vcxproj::Options options;
        options.write_filters = c.filters;
        run_elements(dir, c.name, vcxproj::run(dir, rules, options), c.filters, elements);
    }
    CB_CHECK_EQ(text::join(elements, "\n") + "\n", test::golden("vcx.run.golden.txt"));
});

CB_TEST("java.console.vcxproj.run_details", [] {
    fs::ScopedTempDir tmp("cbvcx");
    const FilterRules rules(small_rules());
    const stdfs::path dir = fixture_copy(tmp, "run_ra");
    vcxproj::Report report = vcxproj::run(dir, rules);
    const std::string once = fs::read_text(dir / "Game.vcxproj");
    CB_CHECK(once.find("\r\n") != std::string::npos);                       // Visual Studio's line ends
    CB_CHECK(report.stdafx_block_found);
    CB_CHECK_EQ(report.cpp_count, std::size_t{11});
    CB_CHECK(report.filters.has_value() && report.filters->folders == 11);
    CB_CHECK(once.find("<ClCompile Include=\"xuimenu\\Ui.cpp\">") != std::string::npos);   // a subfolder's own PCH setting
    CB_CHECK(once.find("<PrecompiledHeader>NotUsing</PrecompiledHeader>") != std::string::npos);
    CB_CHECK(once.find("<Target Name=\"DeployContent\"") != std::string::npos);
    CB_CHECK(once.find("StageContent") == std::string::npos);               // the old targets are dropped
    CB_CHECK(once.find("old1.cpp") == std::string::npos && once.find("old.h") == std::string::npos);
    CB_CHECK(once.find("0xFFFE0001") != std::string::npos && once.find("0x11111111") == std::string::npos);   // our own stamp is replaced
    // the same files again give the same project. (The first run leaves the blank lines of the groups it removed; the next one
    // strips them, as the original did, and from then on a run changes nothing.)
    vcxproj::run(dir, rules);
    const std::string settled = fs::read_text(dir / "Game.vcxproj");
    vcxproj::run(dir, rules);
    CB_CHECK_EQ(fs::read_text(dir / "Game.vcxproj"), settled);
    // a config edit is picked up by the next run
    fs::write_text(dir / "platform.config.json", "{\"title_id\": \"1234ABCD\"}");
    vcxproj::run(dir, rules);
    const std::string edited = fs::read_text(dir / "Game.vcxproj");
    CB_CHECK(edited.find("0x1234ABCD") != std::string::npos && edited.find("0xFFFE0001") == std::string::npos);
    // a project folder without a .vcxproj is an error; the name picks one of several
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)vcxproj::run(tmp.path() / "nowhere", rules));
    fs::write_text(tmp.path() / "two" / "B.vcxproj", "<Project>\n</Project>\n");
    fs::write_text(tmp.path() / "two" / "A.vcxproj", "<Project>\n</Project>\n");
    CB_CHECK_EQ(fs::to_text(vcxproj::run(tmp.path() / "two", rules).vcxproj.filename()), "A.vcxproj");   // the first by name
    vcxproj::Options named;
    named.name = "B";
    CB_CHECK_EQ(fs::to_text(vcxproj::run(tmp.path() / "two", rules, named).vcxproj.filename()), "B.vcxproj");
    named.name = "Missing";
    CB_CHECK_THROWS_AS(cb::IoError, (void)vcxproj::run(tmp.path() / "two", rules, named));
});

}  // namespace
