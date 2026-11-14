// Tests of the 16-bit match driver (verify/authored/match16): how a module's functions are grouped into the original's source files
// (real 16-bit calls decoded by Capstone), how the compiler and switches are chosen segment by segment and function by function, how
// each function's way of being said is judged, what is remembered between runs, and what the report says. The toolchain is a fake:
// it "compiles" a function to the original's bytes under the settings and ways of saying a test names, and to the original with one
// byte changed under any other -- so what is tested is the selection (what the driver asks, what it keeps), not Microsoft C.
//
// Every expectation was worked out by hand (the sample, the ranking by score with ties in the order tried, the file's
// vote; a file's total goes to the first of the greatest, by the order tried).
//
// Design notes: the toolchain, the text passes and the run-time library are the caller's (inputs, not imports); the
// remembered choices are written with two-space indent; a function's compiled code is kept in the result
// (`Result::code`), not on the function `run` as attributes.
#include "cb/verify/authored/match16.hpp"

#include <set>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"

namespace {

namespace m16 = cb::verify::authored::match16;
using cb::verify::ByteMatch;
using m16::Function;
using m16::Setting;
using m16::Verdict;

cb::Bytes m16t_bytes(std::initializer_list<int> values) {
    cb::Bytes b;
    for (const int v : values) b.push_back(static_cast<std::uint8_t>(v));
    return b;
}

// A 16-bit function: `body` padded to `length` with NOPs, then ret.
Function m16t_fn(const std::string& name, std::uint32_t segment, std::uint32_t offset, cb::Bytes body = {}) {
    body.push_back(0xC3);   // ret
    return {name, segment, offset, {body, cb::Bytes(body.size(), 0)}};
}

// call near to `target` from a function at `at` (the displacement is relative to the instruction after the call)
cb::Bytes m16t_call(std::uint32_t at, std::uint32_t target) {
    const std::uint16_t rel = static_cast<std::uint16_t>(target - at - 3);
    return m16t_bytes({0xE8, rel & 0xFF, rel >> 8});
}

std::vector<std::string> m16t_flat(const std::vector<std::vector<std::string>>& files) {
    std::vector<std::string> out;
    for (const auto& f : files) {
        std::string joined;
        for (const std::string& n : f) joined += (joined.empty() ? "" : "+") + n;
        out.push_back(joined);
    }
    return out;
}

const cb::platforms::Decoder& m16t_decoder() {
    static const cb::platforms::Decoder d = cb::platforms::Decoder::open(cb::platforms::Arch::X86, 16, false);
    return d;
}

// The fake compiler. `identical[name]` lists the "setting|way" strings under which a function comes out the original's bytes.
class FakeChain : public m16::Toolchain {
public:
    std::map<std::string, std::set<std::string>> identical;
    std::map<std::string, m16::Masked> originals;
    std::vector<std::string> asked;   // "compiler switches way (n functions)" for each compile
    std::vector<std::string> compilers_flags = {"/Od", "/AM /Os"};

    std::vector<std::string> flag_sets(const std::string&) const override { return compilers_flags; }

    std::map<std::string, std::optional<m16::Masked>> compile(const m16::Compilation& request) override {
        asked.push_back(request.compiler + " " + request.switches + " " + request.header + " (" + std::to_string(request.sources.size()) + ")");
        std::map<std::string, std::optional<m16::Masked>> out;
        for (const auto& [fn, text] : request.sources) {
            if (text == "does not compile") {
                out[fn] = std::nullopt;
                continue;
            }
            m16::Masked built = originals.at(fn);
            const auto it = identical.find(fn);
            const bool same = it != identical.end() && it->second.contains(request.switches + "|" + text);
            if (!same) built.code[0] ^= 0xFF;   // one byte wrong
            out[fn] = built;
        }
        return out;
    }

    Verdict distance(const m16::Masked& built, const m16::Masked& original) const override {
        std::size_t differing = 0;
        for (std::size_t i = 0; i < original.code.size(); ++i) differing += built.code[i] != original.code[i] ? 1 : 0;
        return {differing == 0 ? ByteMatch::Identical : ByteMatch::SameSize, differing, original.code.size()};
    }
};

// Variants whose text for every function is `<name>-text` (so a test can say which way made it identical by that text) and whose
// header is `H-<variant>`.
std::vector<m16::Variant> m16t_variants(const std::vector<std::string>& stages, const std::vector<Function>& fns) {
    std::vector<m16::Variant> vs = m16::make_variants(stages);
    for (m16::Variant& v : vs) {
        v.header = "H-" + v.name;
        for (const Function& f : fns) v.texts[f.name] = v.name;
    }
    return vs;
}

}  // namespace

CB_TEST("verify.match16.score", [] {
    CB_CHECK_EQ(m16::score(Verdict{ByteMatch::Identical, 0, 10}), 2.0);
    CB_CHECK_EQ(m16::score(Verdict{ByteMatch::SameSize, 2, 10}), 0.8);
    CB_CHECK_EQ(m16::score(Verdict{ByteMatch::NoCompile, 0, 0}), 0.0);
    CB_CHECK_EQ(m16::score(Verdict{ByteMatch::Different, 5, 0}), -4.0);   // nothing to compare: the total counts as one
    const Verdict many[] = {{ByteMatch::Identical, 0, 4}, {ByteMatch::SameSize, 1, 4}, {ByteMatch::NoCompile, 0, 0}};
    CB_CHECK_EQ(m16::score(many), 2.75);
});

CB_TEST("verify.match16.variants", [] {
    const std::vector<std::string> stages = {"fold", "far", "frame"};
    std::vector<std::string> names;
    for (const m16::Variant& v : m16::make_variants(stages)) names.push_back(v.name);
    const std::vector<std::string> expected = {"plain", "fold", "far", "fold+far", "frame", "fold+frame", "far+frame", "fold+far+frame"};
    CB_CHECK(names == expected);
    CB_CHECK_EQ(m16::make_variants({}).size(), std::size_t{1});
    CB_CHECK_EQ(m16::make_variants({}).front().name, "plain");
    CB_CHECK(m16::make_variants(stages)[3].stages == (std::set<std::string>{"fold", "far"}));
});

CB_TEST("verify.match16.source_files", [] {
    // f0 calls f1 (so they are one file); f2 calls f0 (so f2 joins them: a file is contiguous); g is alone in another segment;
    // h0 and h1 call nothing in their segment, and h1 calls something that is not a function of the module
    const std::vector<Function> fns = {m16t_fn("g", 2, 0x00, m16t_call(0x00, 0x30)),   // a call to something not in the module
                                       m16t_fn("f2", 1, 0x20, m16t_call(0x20, 0x00)), m16t_fn("f1", 1, 0x10),
                                       m16t_fn("f0", 1, 0x00, m16t_call(0x00, 0x10)), m16t_fn("h0", 3, 0x00), m16t_fn("h1", 3, 0x08)};
    // segments in the order first met (2, 1, 3), each by offset
    CB_CHECK(m16t_flat(m16::source_files(fns, m16t_decoder())) == (std::vector<std::string>{"g", "f0+f1+f2", "h0", "h1"}));
    // two files in one segment: a call joins the functions between caller and callee only
    const std::vector<Function> two = {m16t_fn("a", 1, 0x00, m16t_call(0x00, 0x10)), m16t_fn("b", 1, 0x10), m16t_fn("c", 1, 0x20), m16t_fn("d", 1, 0x30, m16t_call(0x30, 0x20))};
    CB_CHECK(m16t_flat(m16::source_files(two, m16t_decoder())) == (std::vector<std::string>{"a+b", "c+d"}));
    // a call to itself joins nothing; a far call (with a segment) is not a near one
    const std::vector<Function> itself = {m16t_fn("a", 1, 0x00, m16t_call(0x00, 0x00)), m16t_fn("b", 1, 0x10, m16t_bytes({0x9A, 0x00, 0x00, 0x01, 0x00}))};
    CB_CHECK(m16t_flat(m16::source_files(itself, m16t_decoder())) == (std::vector<std::string>{"a", "b"}));
    CB_CHECK(m16::source_files({}, m16t_decoder()).empty());
});

CB_TEST("verify.match16.choosing_settings_by_segment_and_by_file", [] {
    // segment 1 has file A (f0 calls f1) and file B (f2); segment 2 is g. /AM /Os (S2) makes A identical, /Od (S1) makes B and g identical.
    std::vector<Function> fns = {m16t_fn("f0", 1, 0x00, m16t_call(0x00, 0x10)), m16t_fn("f1", 1, 0x10), m16t_fn("f2", 1, 0x20), m16t_fn("g", 2, 0x00)};
    FakeChain chain;
    for (const Function& f : fns) chain.originals[f.name] = f.original;
    chain.identical = {{"f0", {"/AM /Os|plain"}}, {"f1", {"/AM /Os|plain"}}, {"f2", {"/Od|plain"}}, {"g", {"/Od|plain"}}};
    std::vector<std::string> log;
    m16::Inputs in;
    in.functions = fns;
    in.variants = m16t_variants({}, fns);
    in.compilers = {"cc"};
    in.toolchain = &chain;
    in.log = [&](std::string_view l) { log.emplace_back(l); };
    const m16::Result r = m16::run(in);
    // the segments' own choices: segment 1 is two functions identical under S2 and one under S1: S2 scores more (4 against 2.75: a one-byte function that is wrong scores nothing)
    CB_CHECK(r.chosen.at(1) == (Setting{"cc", "/AM /Os"}));
    CB_CHECK(r.chosen.at(2) == (Setting{"cc", "/Od"}));
    // ... and file B (f2), which comes nearer under S1, is judged under it: every function ends identical
    CB_CHECK(r.setting.at("f2") == (Setting{"cc", "/Od"}));
    CB_CHECK(r.setting.at("f0") == (Setting{"cc", "/AM /Os"}));
    CB_CHECK(r.setting.at("f1") == (Setting{"cc", "/AM /Os"}));
    for (const char* fn : {"f0", "f1", "f2", "g"}) CB_CHECK(r.verdicts.at(fn).match == ByteMatch::Identical);
    CB_CHECK_EQ(r.verdicts.size(), std::size_t{4});
    CB_CHECK_EQ(r.settings.size(), std::size_t{2});
    CB_CHECK(m16t_flat(r.files) == (std::vector<std::string>{"f0+f1", "f2", "g"}));
    // what was said while choosing
    CB_CHECK(!log.empty());
    CB_CHECK_EQ(log[0], "  segment 1   cc       /AM /Os          (4.0 of a sample of 3)");
    CB_CHECK_EQ(log[1], "  segment 2   cc       /Od              (2.0 of a sample of 1)");
    CB_CHECK_EQ(log[2], "  1 functions nearer under another of their segment's switches than its first");
    // the winner of each is the one way of saying there is, and the text is its text
    CB_CHECK_EQ(r.winner.at("f0"), "plain");
    CB_CHECK_EQ(r.texts.at("f0"), "plain");
    CB_CHECK(r.code.at("f0").has_value());
});

CB_TEST("verify.match16.ways_of_saying_a_function", [] {
    // f0 is identical only said with its temporaries folded and its far parameters joined; f1 only plain; f2 never compiles
    std::vector<Function> fns = {m16t_fn("f0", 1, 0x00), m16t_fn("f1", 1, 0x10), m16t_fn("f2", 1, 0x20)};
    FakeChain chain;
    chain.compilers_flags = {"/AM"};
    for (const Function& f : fns) chain.originals[f.name] = f.original;
    chain.identical = {{"f0", {"/AM|fold+far"}}, {"f1", {"/AM|plain"}}};
    m16::Inputs in;
    in.functions = fns;
    in.variants = m16t_variants({"fold", "far"}, fns);
    for (m16::Variant& v : in.variants) v.texts["f2"] = "does not compile";
    in.compilers = {"cc"};
    in.toolchain = &chain;
    const m16::Result r = m16::run(in);
    CB_CHECK_EQ(r.winner.at("f0"), "fold+far");
    CB_CHECK(r.verdicts.at("f0").match == ByteMatch::Identical);
    CB_CHECK_EQ(r.winner.at("f1"), "plain");
    CB_CHECK(r.verdicts.at("f2").match == ByteMatch::NoCompile);
    CB_CHECK(!r.code.at("f2").has_value());
    // the final judging asks once for each way of saying, with that way's own header
    std::size_t with_header = 0;
    for (const std::string& a : chain.asked) with_header += a.find("H-fold+far") != std::string::npos ? 1 : 0;
    CB_CHECK(with_header >= 1);
    // the report: counts by verdict, which way won the identical ones, and the nearest misses
    const std::vector<std::string> lines = m16::report_lines("setup.dll", r);
    CB_CHECK_EQ(lines[0], "setup.dll: 3 functions compiled with Microsoft C (cc /AM): identical 2, does not compile 1");
    CB_CHECK_EQ(lines[1], "  identical by way of saying: fold+far 1, plain 1");
    CB_CHECK_EQ(lines.size(), std::size_t{2});   // no near misses to list
});

CB_TEST("verify.match16.settings_given_remembered_and_chosen_again", [] {
    cb::fs::ScopedTempDir dir("cbmatch16");
    std::vector<Function> fns = {m16t_fn("f0", 1, 0x00), m16t_fn("g", 2, 0x00)};
    FakeChain chain;
    for (const Function& f : fns) chain.originals[f.name] = f.original;
    chain.identical = {{"f0", {"/AM /Os|plain"}}, {"g", {"/Od|plain"}}};
    m16::Inputs in;
    in.functions = fns;
    in.variants = m16t_variants({}, fns);
    in.compilers = {"cc"};
    in.toolchain = &chain;
    in.choices_file = dir.path() / "choices.json";
    // switches given: every segment is built with them by the first compiler, nothing is looked for
    {
        m16::Inputs given = in;
        given.switches = "/AM /Os";
        given.choices_file.clear();
        const m16::Result r = m16::run(given);
        CB_CHECK(r.chosen.at(1) == (Setting{"cc", "/AM /Os"}));
        CB_CHECK(r.chosen.at(2) == (Setting{"cc", "/AM /Os"}));
        CB_CHECK(r.verdicts.at("g").match == ByteMatch::SameSize);   // g is not identical under them
        CB_CHECK(r.verdicts.at("f0").match == ByteMatch::Identical);
    }
    // a first run chooses and remembers
    const m16::Result first = m16::run(in);
    CB_CHECK(first.verdicts.at("g").match == ByteMatch::Identical);
    const cb::Json saved = cb::json::load(dir.path() / "choices.json");
    CB_CHECK_EQ(saved.at("segments").at("1").at(0).get<std::string>(), "cc");
    CB_CHECK_EQ(saved.at("segments").at("1").at(1).get<std::string>(), "/AM /Os");
    CB_CHECK_EQ(saved.at("segments").at("2").at(1).get<std::string>(), "/Od");
    CB_CHECK(saved.at("functions").empty());
    // the next run finds them again: far fewer compilations (the final judging only), and it says so
    const std::size_t before = chain.asked.size();
    std::vector<std::string> log;
    in.log = [&](std::string_view l) { log.emplace_back(l); };
    const m16::Result second = m16::run(in);
    CB_CHECK(chain.asked.size() - before <= 2);
    CB_CHECK(!log.empty() && log[0].find("as found before") != std::string::npos);
    CB_CHECK(second.verdicts.at("g").match == ByteMatch::Identical);
    // choosing again, or naming a compiler, does not read them back
    in.choose = true;
    const std::size_t again = chain.asked.size();
    (void)m16::run(in);
    CB_CHECK(chain.asked.size() - again > 2);
    // an earlier layout (the segments alone, no `segments` key) is read as the segments
    cb::fs::write_text(dir.path() / "old.json", R"({"1": ["cc", "/AM /Os"], "2": ["cc", "/Od"]})");
    in.choose = false;
    in.choices_file = dir.path() / "old.json";
    log.clear();
    (void)m16::run(in);
    CB_CHECK(!log.empty() && log[0].find("as found before") != std::string::npos);
});

CB_TEST("verify.match16.library_functions_are_identified_not_judged", [] {
    std::vector<Function> fns = {m16t_fn("f0", 1, 0x00), m16t_fn("_aFlmul", 1, 0x10)};
    FakeChain chain;
    chain.compilers_flags = {"/AM"};
    for (const Function& f : fns) chain.originals[f.name] = f.original;
    chain.identical = {{"f0", {"/AM|plain"}}};   // the library function would not come out identical
    m16::Inputs in;
    in.functions = fns;
    in.variants = m16t_variants({}, fns);
    in.compilers = {"cc"};
    in.toolchain = &chain;
    in.identify = [](std::span<const Function>) { return std::map<std::string, std::string>{{"_aFlmul", "_aFlmul"}}; };
    const m16::Result r = m16::run(in);
    CB_CHECK(r.verdicts.at("_aFlmul").match == ByteMatch::Identified);
    CB_CHECK_EQ(r.verdicts.at("_aFlmul").total, std::size_t{1});
    CB_CHECK(r.verdicts.at("f0").match == ByteMatch::Identical);
    CB_CHECK_EQ(r.library.size(), std::size_t{1});
    const std::vector<std::string> lines = m16::report_lines("a.dll", r);
    CB_CHECK_EQ(lines[0], "a.dll: 2 functions compiled with Microsoft C (cc /AM): identical 1, runtime library 1");
});

CB_TEST("verify.match16.near_misses_are_listed_nearest_first", [] {
    std::vector<Function> fns = {m16t_fn("a", 1, 0x00, m16t_bytes({1, 2, 3, 4, 5, 6, 7})), m16t_fn("b", 1, 0x10, m16t_bytes({1, 2}))};
    FakeChain chain;
    chain.compilers_flags = {"/AM"};
    for (const Function& f : fns) chain.originals[f.name] = f.original;
    m16::Inputs in;
    in.functions = fns;
    in.variants = m16t_variants({}, fns);
    in.compilers = {"cc"};
    in.toolchain = &chain;
    const m16::Result r = m16::run(in);
    const std::vector<std::string> lines = m16::report_lines("a.dll", r);
    CB_CHECK_EQ(lines[0], "a.dll: 2 functions compiled with Microsoft C (cc /AM): same size 2");
    // a: 1 of 8 bytes differ (0.125); b: 1 of 3 (0.33): a is nearer
    CB_CHECK_EQ(lines[1], "  a                            same size  1 of 8 bytes differ");
    CB_CHECK_EQ(lines[2], "  b                            same size  1 of 3 bytes differ");
});

CB_TEST("verify.match16.misuse", [] {
    m16::Inputs in;
    CB_CHECK_THROWS_AS(cb::UsageError, (void)m16::run(in));   // no toolchain
    FakeChain chain;
    in.toolchain = &chain;
    CB_CHECK_THROWS_AS(cb::UsageError, (void)m16::run(in));   // no compiler
});
