// Tests of naming a program's functions and data after another version of it, on the real projects of the research (LIB 2.40 after 2.20, LINK 2.44
// after 2.40, C 3.00's P2 after P1) against what the live Python's exact/names.py answered (testdata/w5b3.golden.json): the names and how each was
// found, the data names, the pairs of places in the order they were made (the order the seed stage reads them in), the command's text with -v and
// without, the SYMBOLS --write makes (and keeps on a second run), the names a project has, the functions it is made of and the masked instructions of
// a function; and, on a project of our own, the "names" and "place_by" files in both of their formats.
//
// DIVERGES from the Python: a carver that fails on a project is the same as none (both fall back to the places the project names), but a failure is
// logged at the debug level; the Python swallowed every exception there.
#include "cb/exact/names.hpp"

#include "cb/exact/real_projects.hpp"

using namespace cb;
using namespace cb::exact;
using test::canon;
using test::check;
using test::golden;

namespace {

Json known_json(const names::Known& k) {
    auto sorted = [](const auto& map) {
        std::vector<std::pair<std::int64_t, std::string>> v;
        for (const auto& [a, n] : map) v.emplace_back(a, n);
        std::sort(v.begin(), v.end());
        Json j = Json::array();
        for (const auto& [a, n] : v) j.push_back(Json::array({a, n}));
        return j;
    };
    return Json::array({sorted(k.code), sorted(k.data)});
}

Json named_json(const std::map<std::int64_t, names::Named>& m) {
    Json j = Json::array();
    for (const auto& [a, n] : m) j.push_back(Json::array({a, n.name, n.how}));
    return j;
}

Json pairs_json(const OrderedMap<std::int64_t, std::int64_t>& m) {
    Json j = Json::array();
    for (const auto& [a, b] : m) j.push_back(Json::array({a, b}));
    return j;
}

}  // namespace

CB_TEST("exact.names.the_names_carried_between_real_versions_are_the_pythons", [] {
    test::Real real;
    if (test::skipped(real, "names")) return;
    for (auto it = golden().at("names").begin(); it != golden().at("names").end(); ++it) {
        const std::string key = it.key();
        const std::size_t arrow = key.find(" <- ");
        if (arrow == std::string::npos) continue;
        const std::string a = key.substr(0, arrow), b = key.substr(arrow + 4);
        const auto pa = real.open_named(a), pb = real.open_named(b);
        const names::Carried c = names::carry(*pa, *pb);
        const Json& want = it.value();
        check(canon(named_json(c.code)) == canon(want.at("names")), key + ": the function names");
        check(canon(named_json(c.data)) == canon(want.at("data")), key + ": the data names");
        check(canon(pairs_json(c.pairs)) == canon(want.at("pairs")), key + ": the pairs, in the order they were found");
        check(canon(pairs_json(c.data_pairs)) == canon(want.at("dpairs")), key + ": the data pairs");
        const std::string other = fs::to_text(test::research_root() / b);
        {
            testing::CapturedStd cap;
            check(names::run(*pa, {"--from", other, "-v"}) == want.at("main").at(1).get<int>(), key + ": exit code");
            check(cap.out() == want.at("main").at(0).get<std::string>(), key + ": the -v text");
        }
        testing::CapturedStd cap;
        names::run(*pa, {"--from", other});
        check(cap.out() == want.at("quiet").at(0).get<std::string>(), key + ": the quiet text");
    }
});

CB_TEST("exact.names.what_a_project_has_the_functions_it_is_made_of_and_their_masked_instructions", [] {
    test::Real real;
    if (test::skipped(real, "names known")) return;
    for (auto it = golden().at("names").begin(); it != golden().at("names").end(); ++it) {
        const std::string key = it.key();
        if (!key.starts_with("known ")) continue;
        const std::string name = key.substr(6);
        const auto p = real.open_named(name);
        const Json& want = it.value();
        check(canon(known_json(names::known(*p))) == canon(want.at(0)), key + ": known");
        check(canon(known_json(names::known(*p, std::set<std::int64_t>{}))) == canon(want.at(1)), key + ": known with no starts");
        const auto fs = names::functions(*p);
        Json head = Json::array();
        for (std::size_t i = 0; i < fs.size() && i < 40; ++i) head.push_back(Json::array({fs[i].first, fs[i].second}));
        check(canon(head) == canon(want.at(2)), key + ": functions");
        Json reads = Json::array();
        for (std::size_t i = 0; i < fs.size() && i < 2; ++i) {
            Json insns = Json::array();
            const auto code = names::read(*p, fs[i].first, fs[i].second);
            for (std::size_t k = 0; k < code.size() && k < 6; ++k)
                insns.push_back(Json::array({code[k].masked, code[k].call ? Json(*code[k].call) : Json(nullptr), code[k].data ? Json(*code[k].data) : Json(nullptr)}));
            reads.push_back(insns);
        }
        check(canon(reads) == canon(want.at(3)), key + ": the masked instructions");
    }
});

CB_TEST("exact.names.write_puts_the_names_into_src_symbols_and_keeps_those_it_has", [] {
    test::Real real;
    if (test::skipped(real, "names write")) return;
    fs::ScopedTempDir dir("w5b3-names");
    const auto folder = test::copy_project("lib-2.40", dir.path() / "d");
    const auto p = real.open(folder);
    const Json& want = golden().at("names_write");
    const bool had = std::filesystem::exists(p->path("SYMBOLS"));
    check(had == !want.at("before").is_null(), "SYMBOLS before");
    std::string text;
    {
        testing::CapturedStd cap;
        names::run(*p, {"--from", fs::to_text(test::research_root() / "lib-2.20"), "--write"});
        text = cap.out();
    }
    check(fs::read_text(p->path("SYMBOLS")) == want.at("after").get<std::string>(), "the SYMBOLS written");
    // (the message names the file, as the platform writes a path: its first line is compared, and that it says where the file is)
    const std::string want_text = want.at("main").at(0).get<std::string>();
    const std::string first_line = want_text.substr(0, want_text.find("\nwritten to") + 1);
    check(text.starts_with(first_line), "the command's text");
    check(text.ends_with("SYMBOLS\n") && text.find("written to ") != std::string::npos, "the command says where it wrote");
    // a second run keeps what is there
    const std::string once = fs::read_text(p->path("SYMBOLS"));
    testing::CapturedStd cap;
    names::run(*p, {"--from", fs::to_text(test::research_root() / "lib-2.20"), "--write"});
    CB_CHECK_EQ(fs::read_text(p->path("SYMBOLS")), once);
});

CB_TEST("exact.names.the_arguments_and_the_names_files", [] {
    test::Real real;
    if (test::skipped(real, "names arguments")) return;
    const auto p = real.open_named("lib-2.40");
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(names::run(*p, {}), 1);   // no --from
        CB_CHECK_EQ(cap.out(), "names: --from OTHER (a project of another version of the program, with names)\n");
    }
    CB_CHECK_THROWS_AS(NotFoundError, (void)names::run(*p, {"--from", "no/such/project"}));
    // "names" and "place_by" files, in both formats: NAME fXXXXX | g_XXXX, and PXXXXX | GXXXX NAME; a line with an address on each side or none is nothing
    fs::ScopedTempDir dir("w5b3-names");
    const auto folder = test::copy_project("lib-2.40", dir.path() / "d");
    fs::write_text(folder / "src" / "NAMES", "# names\n_main f00010\nP00020 other\nG0030 datum\ng_0040 anything\nf00050 f00060\nx y\nonlyone\nPABCDE big\n_two P00070 # note\n");
    fs::write_text(folder / "src" / "PLACES", "placed f00080\nfn P00090\n");
    Json cfg = json::load(folder / "project.json");
    cfg["names"] = "src/NAMES";
    cfg["place_by"] = "PLACES";
    json::save(folder / "project.json", cfg, json::Style::PythonCompatiblePretty);
    const names::Known k = names::known(*real.open(folder), std::set<std::int64_t>{});
    CB_CHECK_EQ(*k.code.find(0x10), "_main");
    CB_CHECK_EQ(*k.code.find(0x20), "other");
    CB_CHECK_EQ(*k.data.find(0x30), "datum");
    CB_CHECK_EQ(*k.data.find(0x40), "anything");
    CB_CHECK(k.code.find(0x50) == nullptr);   // f00050 f00060: both are places: no name
    CB_CHECK_EQ(*k.code.find(0x70), "_two");
    CB_CHECK_EQ(*k.code.find(0x80), "placed");
    CB_CHECK_EQ(*k.code.find(0x90), "fn");
    CB_CHECK(k.code.find(0xABCDE) != nullptr && *k.code.find(0xABCDE) == "big");
    // a "names" file that is not there is none
    cfg["names"] = "src/NOTHERE";
    json::save(folder / "project.json", cfg, json::Style::PythonCompatiblePretty);
    const names::Known none = names::known(*real.open(folder), std::set<std::int64_t>{});
    CB_CHECK(none.code.find(0x10) == nullptr || *none.code.find(0x10) != "_main");
});
