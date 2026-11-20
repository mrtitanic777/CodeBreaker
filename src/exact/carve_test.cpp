// Tests of where a program's own functions begin and end, on the real projects of the research (C 3.00's P0 and P1, LIB 3.00, LINK 2.44,
// the C 1.0x compiler, MASM 5.10) against the recorded answers (testdata/w5b3.golden.json):
// the functions with why each begins, the region, the command's text, the hex patterns of the profiles run over a program's code, and starts the
// project adds and drops.
//
// Behaviour pinned here:
//   * A start the project adds past where the program's own code ends is a FormatError.
//   * The prologue patterns are run by cb::Regex over the bytes (`\xHH` for a byte); the same places
//     are found (the golden's matches of every pattern over P0's first 6000 bytes).
#include "exact/carve.hpp"

#include "core/regex.hpp"
#include "exact/real_projects.hpp"

using namespace cb;
using namespace cb::exact;
using test::canon;
using test::check;
using test::golden;

namespace {

Json functions_json(const std::vector<carve::Function>& fs) {
    Json out = Json::array();
    for (const carve::Function& f : fs) out.push_back(Json::array({f.address, f.length, f.why}));
    return out;
}

}  // namespace

CB_TEST("exact.carve.the_functions_of_a_real_program_match_the_recorded_output", [] {
    test::Real real;
    if (test::skipped(real, "carve")) return;
    for (auto it = golden().at("carve").begin(); it != golden().at("carve").end(); ++it) {
        const std::string name = it.key();
        const auto p = real.open_named(name);
        const Json got = functions_json(carve::functions(*p));
        check(canon(got) == canon(it.value().at("functions")), name + ": the functions\n  got  " + canon(got).substr(0, 300) + "\n  want " + canon(it.value().at("functions")).substr(0, 300));
        const carve::Region r = carve::region(*p);
        Json skip = Json::array();
        for (const auto& [a, b] : r.skip) skip.push_back(Json::array({a, b}));
        check(canon(Json::array({r.lo, r.hi, skip})) == canon(it.value().at("region")), name + ": the region");
        check(!got.empty(), name + ": it has functions");
    }
});

CB_TEST("exact.carve.the_command_prints_the_recorded_output_and_writes_src_funcs", [] {
    test::Real real;
    if (test::skipped(real, "carve command")) return;
    for (const char* name : {"msc-3.0/recon/p0", "lib-3.00", "masm-5.10"}) {
        const Json& want = golden().at("carve").at(name);
        const auto p = real.open_named(name);
        {
            testing::CapturedStd cap;
            check(carve::run(*p, {"-v"}) == want.at("main").at(1).get<int>(), std::string(name) + ": exit code");
            check(cap.out() == want.at("main").at(0).get<std::string>(), std::string(name) + ": -v text");
        }
        testing::CapturedStd cap;
        carve::run(*p, {});
        check(cap.out() == want.at("quiet").at(0).get<std::string>(), std::string(name) + ": quiet text");
    }
    // --write: src/FUNCS, one start a line with its length and why
    fs::ScopedTempDir dir("w5b3-carve");
    const auto folder = test::copy_project("msc-3.0/recon/p0", dir.path() / "p0");
    const auto p = real.open(folder);
    testing::CapturedStd cap;
    carve::run(*p, {"--write"});
    const std::string funcs = fs::read_text(p->path("FUNCS"));
    CB_CHECK(funcs.starts_with("# where the program's own functions begin, and their lengths (codebreaker exact carve)\n"));
    const std::vector<carve::Function> fs = carve::functions(*p);
    CB_CHECK(funcs.find(std::format("{:05x} {}  {}\n", fs.front().address, fs.front().length, fs.front().why)) != std::string::npos);
    CB_CHECK_EQ(text::lines(funcs).size(), fs.size() + 1);
});

CB_TEST("exact.carve.the_profiles_patterns_find_the_same_places_in_a_programs_code", [] {
    test::Real real;
    if (test::skipped(real, "carve patterns")) return;
    const auto p = real.open_named("msc-3.0/recon/p0");
    const ByteView img = p->image();
    const std::string_view subject(reinterpret_cast<const char*>(img.data()), 6000);
    for (const Json& row : golden().at("hexrx")) {
        const std::string pattern = row.at(0).get<std::string>();
        const Json& want = row.at(1);
        if (want.is_string()) {   // "error": the pattern is not a regular expression
            CB_CHECK_THROWS_AS(ParseError, (void)Regex(carve::hex_pattern(pattern), RegexFlags::DotAll));
            continue;
        }
        const Regex re(carve::hex_pattern(pattern), RegexFlags::DotAll);
        Json every = Json::array();
        re.for_each_match(subject, [&](const RegexMatch& m) {
            every.push_back(Json::array({m.start(), m.end(), re.group_count() > 0 && m.has(1) ? Json(m.start(1)) : Json(nullptr)}));
            return true;
        });
        check(every.size() == want.at("n").get<std::size_t>(), "pattern '" + pattern + "': the number of matches");
        Json first = Json::array(), last = Json::array();
        for (std::size_t i = 0; i < every.size() && i < 40; ++i) first.push_back(every[i]);
        for (std::size_t i = every.size() > 5 ? every.size() - 5 : 0; i < every.size(); ++i) last.push_back(every[i]);
        check(canon(first) == canon(want.at("first")) && canon(last) == canon(want.at("last")), "pattern '" + pattern + "': the matches");
    }
    // the translation itself: a byte is \xHH, '..' any byte, the regex's own characters stay
    CB_CHECK_EQ(carve::hex_pattern("55 8b ec"), R"(\x55\x8b\xec)");
    CB_CHECK_EQ(carve::hex_pattern("55 (?:33c0|b8....) e8 (....)"), R"(\x55(?:\x33\xc0|\xb8..)\xe8(..))");
    CB_CHECK_EQ(carve::hex_pattern("2e 2a"), R"(\x2e\x2a)");   // a byte that is a metacharacter is only itself
    CB_CHECK_EQ(carve::hex_pattern(""), "");
    CB_CHECK_EQ(carve::hex_pattern("5 5"), R"(\x55)");   // (the blanks are for the eye: 5 5 is the byte 55)
});

CB_TEST("exact.carve.starts_the_project_adds_and_drops_and_where_the_code_begins", [] {
    test::Real real;
    if (test::skipped(real, "carve crafted")) return;
    for (auto it = golden().at("carve_crafted").begin(); it != golden().at("carve_crafted").end(); ++it) {
        fs::ScopedTempDir dir("w5b3-carve");
        const auto folder = test::copy_project("msc-3.0/recon/p0", dir.path() / "p");
        Json cfg = json::load(folder / "project.json");
        if (it.key() == "add_drop") {
            cfg["starts_add"] = Json::array({16, "0x20"});   // (a number and a text)
            cfg["starts_drop"] = Json::array({"0x0", 4});
        } else if (it.key() == "code_start") {
            cfg["code_start"] = "0x100";
        } else {
            cfg["starts_add"] = Json::array({60000});
        }
        json::save(folder / "project.json", cfg, json::Style::PythonCompatiblePretty);
        const auto p = real.open(folder);
        if (it.value().is_string()) {   // a start past the end of the program's own code
            CB_CHECK_THROWS_AS(FormatError, (void)carve::functions(*p));
            continue;
        }
        check(canon(functions_json(carve::functions(*p))) == canon(it.value()), "carve " + it.key());
    }
});
