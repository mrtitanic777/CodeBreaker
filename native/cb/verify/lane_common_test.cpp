// Tests of what every lane's judge shares: the switches, the fingerprint store, the text of calls and arguments, the
// memory digest, a run's fingerprint, the argument patterns, the guided/explored limits and the verdict boundary.
#include "cb/verify/lane_common.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
using namespace cb::verify;
namespace lc = cb::verify::lane_common;

emulate::MemoryWrite write(std::uint64_t address, std::uint32_t size, std::uint64_t value, std::optional<std::uint64_t> old,
                           bool harness = false) {
    emulate::MemoryWrite w;
    w.address = address;
    w.size = size;
    w.value = value;
    w.old = old;
    w.harness = harness;
    return w;
}

compare::Fingerprint sample(std::int64_t ret) {
    compare::Fingerprint f;
    f.ret = ret;
    f.exit = "ret";
    return f;
}

}  // namespace

CB_TEST("verify.lane_common.switches_read_the_environment", [] {
    {
        fs::ScopedEnv a("CB_NO_FP_CACHE", std::nullopt), b("CB_NO_GUIDED", std::nullopt), c("CB_NO_CONCOLIC", std::nullopt),
            d("CB_PLAIN", std::nullopt);
        CB_CHECK(lc::fp_cache_on() && lc::guided_on() && lc::concolic_on() && !lc::plain());
    }
    {
        fs::ScopedEnv a("CB_NO_FP_CACHE", "1"), b("CB_NO_GUIDED", "1"), c("CB_NO_CONCOLIC", "1"), d("CB_PLAIN", "1");
        CB_CHECK(!lc::fp_cache_on() && !lc::guided_on() && !lc::concolic_on() && lc::plain());
    }
    {
        fs::ScopedEnv a("CB_NO_FP_CACHE", ""), d("CB_PLAIN", "");   // set but empty is off
        CB_CHECK(lc::fp_cache_on() && !lc::plain());
    }
});

CB_TEST("verify.lane_common.remembered_runs_once_per_key", [] {
    fs::ScopedEnv on("CB_NO_FP_CACHE", std::nullopt);
    std::map<std::string, compare::Fingerprint> kept;
    lc::FingerprintStore store{
        [&](std::string_view k) -> std::optional<compare::Fingerprint> {
            auto it = kept.find(std::string(k));
            return it == kept.end() ? std::nullopt : std::optional<compare::Fingerprint>(it->second);
        },
        [&](std::string_view k, const compare::Fingerprint& f) { kept[std::string(k)] = f; }};
    int runs = 0;
    auto run = [&] {
        ++runs;
        return sample(runs);
    };
    CB_CHECK(lc::remembered(store, "k", run).ret == 1);
    CB_CHECK(lc::remembered(store, "k", run).ret == 1);   // the first run's, from the store
    CB_CHECK_EQ(runs, 1);
    lc::remembered(store, std::nullopt, run);                                           // no key: never kept
    lc::remembered(store, std::nullopt, run);
    CB_CHECK_EQ(runs, 3);
    CB_CHECK_EQ(kept.size(), std::size_t{1});
});

CB_TEST("verify.lane_common.remembered_keeps_only_observations", [] {
    fs::ScopedEnv on("CB_NO_FP_CACHE", std::nullopt);
    std::map<std::string, compare::Fingerprint> kept;
    lc::FingerprintStore store{
        [&](std::string_view k) -> std::optional<compare::Fingerprint> {
            auto it = kept.find(std::string(k));
            return it == kept.end() ? std::nullopt : std::optional<compare::Fingerprint>(it->second);
        },
        [&](std::string_view k, const compare::Fingerprint& f) { kept[std::string(k)] = f; }};
    compare::Fingerprint bad;
    bad.err = "could not trace";
    lc::remembered(store, "bad", [&] { return bad; });
    CB_CHECK(kept.empty());                                                              // not an observation
    lc::remembered(store, "no", [&] { return sample(1); }, [](const compare::Fingerprint&) { return false; });
    CB_CHECK(kept.empty());                                                              // `keep` turned it down
    lc::remembered(store, "yes", [&] { return sample(1); }, [](const compare::Fingerprint&) { return true; });
    CB_CHECK_EQ(kept.size(), std::size_t{1});
});

CB_TEST("verify.lane_common.remembered_does_nothing_when_the_cache_is_off", [] {
    fs::ScopedEnv off("CB_NO_FP_CACHE", "1");
    int gets = 0;
    lc::FingerprintStore store{[&](std::string_view) -> std::optional<compare::Fingerprint> {
                                   ++gets;
                                   return std::nullopt;
                               },
                               [&](std::string_view, const compare::Fingerprint&) { ++gets; }};
    int runs = 0;
    lc::remembered(store, "k", [&] {
        ++runs;
        return sample(1);
    });
    CB_CHECK(runs == 1 && gets == 0);
});

CB_TEST("verify.lane_common.store_of_reaches_the_process_cache", [] {
    fs::ScopedEnv on("CB_NO_FP_CACHE", std::nullopt);
    fs::ScopedTempDir dir("cb-lane-common");
    base::cache::Config config;
    config.state_dir = dir.path() / "state";
    config.shared_dir = dir.path() / "shared";
    base::cache::Cache cache(config);
    compare::declare_cache(cache);
    const lc::FingerprintStore store = lc::store_of(cache);
    int runs = 0;
    const std::string key = compare::cache_key({"module", "main", 1});
    auto run = [&] {
        ++runs;
        compare::Fingerprint f = sample(7);
        f.deps = {"A(1)"};
        return f;
    };
    const compare::Fingerprint first = lc::remembered(store, key, run);
    const compare::Fingerprint again = lc::remembered(store, key, run);
    CB_CHECK_EQ(runs, 1);
    CB_CHECK(first == again);
});

CB_TEST("verify.lane_common.call_text", [] {
    CB_CHECK_EQ(lc::call("CreateFileA"), "CreateFileA");
    CB_CHECK_EQ(lc::call("CreateFileA", std::vector<std::string>{}), "CreateFileA()");
    CB_CHECK_EQ(lc::call("f", std::vector<std::string>{"a0", "S", "1f"}), "f(a0,S,1f)");
});

CB_TEST("verify.lane_common.calls_skip_cap_and_show", [] {
    const std::vector<lc::Called> seq = {{"A", std::vector<std::int64_t>{1, 2}},
                                         {"skipme", std::vector<std::int64_t>{}},
                                         {"B", std::nullopt},
                                         {std::nullopt, std::vector<std::int64_t>{3}},
                                         {"D", std::vector<std::int64_t>{}}};
    auto show = [](std::int64_t v) { return std::format("<{}>", v); };
    auto skip = [](const std::optional<std::string>& n) { return n == "skipme"; };
    const std::vector<std::string> all = lc::calls(seq, show, nullptr, skip);
    CB_CHECK(all == (std::vector<std::string>{"A(<1>,<2>)", "B", "?(<3>)", "D()"})); // B: arguments not known; a nameless call: "?"
    const std::vector<std::string> capped = lc::calls(seq, show, nullptr, skip, 2);
    CB_CHECK(capped == (std::vector<std::string>{"A(<1>,<2>)", "B", "..."}));
    const std::vector<std::string> renamed =
        lc::calls(seq, show, [](const std::optional<std::string>& n) { return n ? "x_" + *n : std::string("anon"); }, skip);
    CB_CHECK_EQ(renamed.front(), "x_A(<1>,<2>)");
    CB_CHECK_EQ(renamed[2], "anon(<3>)");
});

CB_TEST("verify.lane_common.called_reads_a_run", [] {
    emulate::Run run;
    run.calls.push_back(emulate::Call{"GetTickCount", std::vector<std::int64_t>{}, {}});
    run.calls.push_back(emulate::Call{std::nullopt, std::nullopt, {}});
    const std::vector<lc::Called> got = lc::called(run);
    CB_CHECK(got.size() == 2 && got[0].name == "GetTickCount" && got[0].args && got[0].args->empty());
    CB_CHECK(!got[1].name && !got[1].args);
});

CB_TEST("verify.lane_common.argument_forms_in_order", [] {
    lc::ArgumentContext c;
    c.inputs = {7, 0x12345, 0x12345};
    c.above = 0x100;
    c.small = true;
    c.frame = {std::nullopt, lc::AddressRange{0x7ff00000, 0x80000000}};
    c.image = lc::AddressRange{0x400000, 0x500000};
    CB_CHECK_EQ(lc::argument(0x12345, c), "a1");           // an input above `above`: the first one that matches
    CB_CHECK_EQ(lc::argument(7, c), "7");                  // below `above`: a constant that happens to equal an input
    CB_CHECK_EQ(lc::argument(0xFFFFFFFF, c), "-1");        // a small constant, signed
    CB_CHECK_EQ(lc::argument(0x7ff01000, c), "S");             // in the frame
    CB_CHECK_EQ(lc::argument(0x401000, c), "P");           // in the image
    CB_CHECK_EQ(lc::argument(0x1f3a0000, c), "1f3a0000");  // the value itself
    c.small = false;
    CB_CHECK_EQ(lc::argument(5, c), "5");
    c.image = [](std::int64_t v) { return v == 0x99; };
    CB_CHECK_EQ(lc::argument(0x99, c), "P");
    CB_CHECK_EQ(lc::argument(0x98, c), "98");
    CB_CHECK_EQ(lc::argument(0x7fff, lc::ArgumentContext{}), "7fff");   // no context: the value
    CB_CHECK_EQ(lc::argument(-5, lc::ArgumentContext{}), "-5");
});

// The digests are the ones the fingerprints already in the cache were made with (computed independently from the
// layout `[(place, held), ...]`, sha1, first 16 hex digits): changing them would orphan the cache.
CB_TEST("verify.lane_common.writes_digest_is_stable", [] {
    CB_CHECK_EQ(lc::writes({}), "");
    lc::State s;
    s[lc::Place(std::uint64_t{4096})] = lc::Held(std::uint8_t{5});
    s[lc::Place(std::uint64_t{4097})] = lc::Held(std::string("S"));
    CB_CHECK_EQ(lc::writes(s), "2:a337cdbf5e9315c6");
    lc::State t;
    t[lc::Place(lc::TaggedPlace{"stack", 1})] = lc::Held(std::uint8_t{0xff});
    t[lc::Place(lc::TaggedPlace{"stack", 0})] = lc::Held(std::uint8_t{1});
    t[lc::Place(lc::TaggedPlace{"heap", 2})] = lc::Held(std::uint8_t{7});
    CB_CHECK_EQ(lc::writes(t), "3:98b0fc8b1ea80ae8");                 // the order the places were added in does not matter
});

CB_TEST("verify.lane_common.changed_keeps_what_differs", [] {
    emulate::Run run;
    run.writes = {write(0x1000, 4, 0x11223344, 0x11223300),   // only the low byte changes
                  write(0x2000, 2, 0xAABB, 0xAABB),            // written with what it held
                  write(0x3000, 4, 0x00402000, std::nullopt)}; // nothing known before: every byte counts
    const lc::State got = lc::changed(run);
    lc::State want;
    want[lc::Place(std::uint64_t{0x1000})] = lc::Held(std::uint8_t{0x44});
    for (std::uint64_t i = 0; i < 4; ++i) want[lc::Place(0x3000 + i)] = lc::Held(std::uint8_t{static_cast<std::uint8_t>((0x00402000 >> (8 * i)) & 0xFF)});
    CB_CHECK(got == want);
    // a word's token stands for each of its bytes
    lc::ChangedOptions o;
    o.word = [](std::uint64_t v) -> std::optional<std::string> { return v == 0x402000 ? std::optional<std::string>("S") : std::nullopt; };
    const lc::State tokens = lc::changed(run, o);
    CB_CHECK(tokens.at(lc::Place(std::uint64_t{0x3000})) == lc::Held(std::string("S")));
    CB_CHECK(tokens.at(lc::Place(std::uint64_t{0x3003})) == lc::Held(std::string("S")));
    CB_CHECK(tokens.at(lc::Place(std::uint64_t{0x1000})) == lc::Held(std::uint8_t{0x44}));
    CB_CHECK(lc::left(run) == std::optional<std::string>("5:13752ca0fc17f7d6"));   // the digest of the first state above
});

CB_TEST("verify.lane_common.changed_names_places_and_skips", [] {
    emulate::Run run;
    run.writes = {write(0x1000, 2, 0x0102, 0), write(0x9000, 1, 9, 0), write(0x5000, 2, 0x0A0B, 0, true)};
    lc::ChangedOptions o;
    o.place = [](std::uint64_t a) -> lc::Where {
        if (a == 0x1000) return lc::Place(lc::TaggedPlace{"stack", 4});
        return lc::Skip{};                                   // 0x9000 is not a place the lane names
    };
    const lc::State got = lc::changed(run, o);
    CB_CHECK_EQ(got.size(), std::size_t{2});                  // (the harness's write is never reported)
    CB_CHECK(got.at(lc::Place(lc::TaggedPlace{"stack", 4})) == lc::Held(std::uint8_t{0x02}));
    CB_CHECK(got.at(lc::Place(lc::TaggedPlace{"stack", 5})) == lc::Held(std::uint8_t{0x01}));
    // a function names each byte, and may leave some out
    o.place = [](std::uint64_t a) -> lc::Where {
        if (a != 0x1000) return lc::Skip{};
        return lc::ByteNamer([](std::size_t i) -> std::optional<lc::Place> {
            if (i == 0) return std::nullopt;
            return lc::Place(lc::TaggedPlace{"reg", static_cast<std::int64_t>(i)});
        });
    };
    const lc::State named = lc::changed(run, o);
    CB_CHECK_EQ(named.size(), std::size_t{1});
    CB_CHECK(named.at(lc::Place(lc::TaggedPlace{"reg", 1})) == lc::Held(std::uint8_t{0x01}));
});

CB_TEST("verify.lane_common.changed_respects_endianness_most_and_everything", [] {
    emulate::Run big;
    big.endian = emulate::Endian::Big;
    big.writes = {write(0x100, 4, 0x01020304, 0x01020300)};
    const lc::State b = lc::changed(big);
    CB_CHECK_EQ(b.size(), std::size_t{1});                    // big-endian: the last byte is the low one
    CB_CHECK(b.at(lc::Place(std::uint64_t{0x103})) == lc::Held(std::uint8_t{0x04}));
    emulate::Run little;
    little.writes = {write(0x100, 8, 0x0807060504030201ull, 0)};
    lc::ChangedOptions o;
    o.most = 2;
    CB_CHECK_EQ(lc::changed(little, o).size(), std::size_t{2});
    // `everything`: the byte's earlier value is the first write's, even one the lane left out (the harness's)
    emulate::Run seeded;
    seeded.writes = {write(0x200, 1, 5, 0, true), write(0x200, 1, 0, 5)};
    CB_CHECK_EQ(lc::changed(seeded).size(), std::size_t{0});           // it ends as the harness's first write found it: 0
    o = {};
    o.everything = false;
    CB_CHECK_EQ(lc::changed(seeded, o).size(), std::size_t{1});        // before = what the first kept write found: 5
});

CB_TEST("verify.lane_common.left_is_none_when_writes_were_capped", [] {
    emulate::Run run;
    run.capped = true;
    run.writes = {write(0x1000, 1, 1, 0)};
    CB_CHECK(!lc::left(run).has_value());
    run.capped = false;
    CB_CHECK(lc::left(run).has_value());
    run.writes.clear();
    CB_CHECK(lc::left(run) == std::optional<std::string>(""));
});

CB_TEST("verify.lane_common.fingerprint_of_a_returned_run", [] {
    emulate::Run run;
    run.exit = "ret";
    run.stack = 8;
    lc::FingerprintParts parts;
    parts.ret = 42;
    parts.saved = true;
    parts.cov = {1, 2};
    parts.reach = {1, 2, 3};
    parts.steps = 17;
    const compare::Fingerprint fp = lc::fingerprint(run, {"A(1)"}, parts);
    CB_CHECK(fp.ret == 42 && fp.exit == "ret" && !fp.truncated && fp.deps == std::vector<std::string>{"A(1)"});
    CB_CHECK(fp.writes == std::optional<std::string>("") && fp.pop == 8 && fp.saved == true && fp.steps == 17);
    CB_CHECK(fp.cov.size() == 2 && fp.reach.size() == 3 && !fp.err);
});

CB_TEST("verify.lane_common.fingerprint_of_a_run_that_did_not_return", [] {
    emulate::Run run;
    run.exit = "truncated";
    run.truncated = true;
    run.stack = 8;
    lc::FingerprintParts parts;
    parts.saved = true;
    parts.writes = std::nullopt;
    const compare::Fingerprint fp = lc::fingerprint(run, {}, parts);
    CB_CHECK(fp.exit == "truncated" && fp.truncated);
    CB_CHECK(!fp.pop && !fp.saved && !fp.writes);              // the stack and the saved registers are for a return only
});

CB_TEST("verify.lane_common.argument_patterns", [] {
    CB_CHECK(lc::counting(3) == (std::vector<std::uint64_t>{1, 2, 3}));
    CB_CHECK(lc::counting(3, 10, 5) == (std::vector<std::uint64_t>{10, 15, 20}));
    CB_CHECK(lc::counting(0) == (std::vector<std::uint64_t>{}));
    CB_CHECK(lc::descending(3, 0xFFFFFFF0) == (std::vector<std::uint64_t>{0xFFFFFFF0, 0xFFFFFFEF, 0xFFFFFFEE}));
    const auto mixed = lc::alternating(5, std::uint64_t{0x1000}, [](std::size_t i) { return std::uint64_t{i * 3}; });
    CB_CHECK(mixed == (std::vector<std::uint64_t>{0x1000, 3, 0x1000, 9, 0x1000}));
    const auto both = lc::alternating(4, [](std::size_t i) { return std::uint64_t{i}; }, std::uint64_t{7});
    CB_CHECK(both == (std::vector<std::uint64_t>{0, 7, 2, 7}));
});

CB_TEST("verify.lane_common.guided_is_off_with_its_switch", [] {
    guided::Input seed;
    seed.args = {1, 2};
    const std::vector<guided::Input> seeds = {seed};
    int traced = 0;
    const guided::Run run = [&](const guided::Input&) -> std::optional<guided::Observation> {
        ++traced;
        return std::nullopt;
    };
    fs::ScopedEnv off("CB_NO_GUIDED", "1");
    CB_CHECK(lc::guided(run, seeds, 1).empty());
    CB_CHECK_EQ(traced, 0);
});

CB_TEST("verify.lane_common.guided_finds_inputs_for_branches_no_seed_took", [] {
    fs::ScopedEnv on("CB_NO_GUIDED", std::nullopt);
    // a function that branches on `arg0 == 0x1234`: the seed takes the other way; the compare is what guided reads
    guided::Input seed;
    seed.args = {1};
    const std::vector<guided::Input> seeds = {seed};
    const guided::Run run = [](const guided::Input& in) -> std::optional<guided::Observation> {
        guided::Observation o;
        guided::Branch b;
        b.address = 0x401010;
        b.taken = in.args[0] == 0x1234;
        guided::Compare c;
        c.op = guided::CompareOp::Other;
        c.size = 4;
        c.a = in.args[0];
        c.b = 0x1234;
        b.compare = c;
        o.branches.push_back(b);
        o.reach = {0x401000, 0x401010};
        o.args_at = 0x8000;
        o.stack_low = 0x7000;
        o.stack_high = 0x9000;
        return o;
    };
    const std::vector<guided::Input> found = lc::guided(run, seeds, 7);
    bool hit = false;
    for (const guided::Input& in : found) hit = hit || in.args[0] == 0x1234;
    CB_CHECK(hit);
    CB_CHECK(static_cast<int>(found.size()) <= lc::kMaxGuided);
});

CB_TEST("verify.lane_common.explored_limits_and_path_limits", [] {
    // an exploration of a function with no jumps: one path, and nothing more to find
    struct Seed {
        int id = 0;
    };
    std::vector<Seed> seeds = {{1}};
    const std::function<std::optional<concolic::Observation>(Seed&)> run = [](Seed&) -> std::optional<concolic::Observation> {
        return concolic::Observation{};
    };
    const std::function<std::optional<Seed>(const Seed&, const concolic::Values&, const concolic::Vars&)> derive =
        [](const Seed&, const concolic::Values&, const concolic::Vars&) -> std::optional<Seed> { return std::nullopt; };
    const concolic::Exploration<Seed> got = lc::explored<Seed>(run, std::span<Seed>(seeds), derive, [](std::int64_t) { return true; });
    CB_CHECK(got.runs >= 1 && got.paths == 1);
    CB_CHECK(got.kept.empty());                            // (the seeds are not among the inputs it makes)
    CB_CHECK(lc::path_limits(nullptr).empty());
    concolic::Stats s;
    s.paths = 3;
    CB_CHECK(lc::path_limits(&s) == (std::vector<std::string>{"paths=3"}));
    s.complete = true;
    CB_CHECK(lc::path_limits(&s) == (std::vector<std::string>{"paths=3", "paths-all"}));
});

CB_TEST("verify.lane_common.judged_passes_a_verdict_through", [] {
    const lc::Judgement j = lc::judged([] {
        return lc::Judgement{Verdict::Divergent, "return value", {"argc-unknown"}, "ret 1 != 2"};
    });
    CB_CHECK(j.verdict == Verdict::Divergent && j.reason == "return value" && j.detail == "ret 1 != 2");
    CB_CHECK(j.limits == (std::vector<std::string>{"argc-unknown"}));
});

CB_TEST("verify.lane_common.judged_turns_a_failure_into_inconclusive", [] {
    const lc::Judgement format = lc::judged([]() -> lc::Judgement { throw FormatError("reader", 4, "truncated header"); });
    CB_CHECK(format.verdict == Verdict::Inconclusive);
    CB_CHECK_EQ(format.reason, "judge: FormatError");
    CB_CHECK(format.limits.empty() && format.detail.empty());
    const lc::Judgement other = lc::judged([]() -> lc::Judgement { throw std::runtime_error("boom"); });
    CB_CHECK(other.verdict == Verdict::Inconclusive);
    CB_CHECK_EQ(other.reason, "judge: Exception");
});

CB_TEST("verify.lane_common.judged_lets_a_missing_tool_through", [] {
    CB_CHECK_THROWS_AS(NotFoundError, lc::judged([]() -> lc::Judgement { throw NotFoundError("unicorn.dll", 2, "no such library"); }));
});
