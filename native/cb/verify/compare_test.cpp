// Tests of the comparer: the first difference between two fingerprints, its one-line detail, judging a body over
// several inputs (what makes a match weaker than it looks), and the fingerprint's cache entry.
#include "cb/verify/compare.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb::verify;
using compare::Difference;
using compare::Fingerprint;
using compare::Input;

// A run that returned `ret`, calling `deps`, leaving `writes`.
Fingerprint returned(std::int64_t ret, std::vector<std::string> deps = {}) {
    Fingerprint f;
    f.ret = ret;
    f.exit = "ret";
    f.deps = std::move(deps);
    f.writes = "none";
    f.pop = 8;
    f.saved = true;
    return f;
}

struct TestInput : Input {
    int id = 0;
};
TestInput input(int id, std::vector<std::int64_t> args = {}) {
    TestInput v;
    v.id = id;
    v.args = std::move(args);
    return v;
}

}  // namespace

CB_TEST("verify.compare.same_runs_agree", [] {
    CB_CHECK(compare::first_difference(returned(5), returned(5)) == Difference::None);
    CB_CHECK_EQ(compare::difference_detail(returned(5), returned(5)), std::string());
});

CB_TEST("verify.compare.each_difference_has_its_kind_and_reason", [] {
    Fingerprint a = returned(5, {"Foo(1,2)"});
    Fingerprint b = a;
    b.exit = "exception: unmapped";
    CB_CHECK(compare::first_difference(a, b) == Difference::Exit);
    CB_CHECK_EQ(std::string(compare::reason(Difference::Exit)), "exit-differs");

    b = a;
    b.deps = {"Foo(1,3)"};
    CB_CHECK(compare::first_difference(a, b) == Difference::Deps);

    b = a;
    b.ret = 6;
    CB_CHECK(compare::first_difference(a, b) == Difference::Ret);
    CB_CHECK_EQ(std::string(compare::reason(Difference::Ret)), "ret-differs");

    b = a;
    b.writes = "x";
    CB_CHECK(compare::first_difference(a, b) == Difference::Writes);

    b = a;
    b.pop = 12;
    CB_CHECK(compare::first_difference(a, b) == Difference::Stack);

    b = a;
    b.saved = false;
    CB_CHECK(compare::first_difference(a, b) == Difference::Registers);
    // ... but a rebuild that preserves what the original did not is no difference
    a.saved = false;
    b.saved = true;
    CB_CHECK(compare::first_difference(a, b) == Difference::None);
});

CB_TEST("verify.compare.the_return_value_counts_only_when_the_function_returned", [] {
    Fingerprint a = returned(0x12340001);
    Fingerprint b = returned(0x56780001);
    CB_CHECK(compare::first_difference(a, b) == Difference::Ret);
    CB_CHECK(compare::first_difference(a, b, 0xFFFF0000u) == Difference::None);   // leftover high half: not a result
    a.exit = b.exit = "exception: bad";
    CB_CHECK(compare::first_difference(a, b) == Difference::None);                // EAX at a fault says nothing
});

CB_TEST("verify.compare.a_run_cut_off_compares_only_what_it_did_before", [] {
    Fingerprint a;
    a.exit = "truncated";
    a.truncated = true;
    a.deps = {"A()", "B()"};
    a.steps = 1000;
    Fingerprint b = a;
    b.deps = {"A()", "B()", "C()"};                       // the rebuild went further: its calls start with the original's
    CB_CHECK(compare::first_difference(a, b) == Difference::None);
    b.deps = {"A()"};                                      // fewer calls: it did not make every call the original made
    CB_CHECK(compare::first_difference(a, b) == Difference::Deps);
    b.deps = {"A()", "B()"};
    b.exit = "ret";                                        // ended where the original was still going
    b.steps = 900;
    CB_CHECK(compare::first_difference(a, b) == Difference::Exit);
    b.steps = 5000;                                        // ... unless it had run more steps than the original was given
    CB_CHECK(compare::first_difference(a, b) == Difference::None);
    b.exit = "wild-call";                                  // a wild call is never excused
    b.steps = 5000;
    CB_CHECK(compare::first_difference(a, b) == Difference::Exit);
});

CB_TEST("verify.compare.a_truncated_run_does_not_compare_memory", [] {
    Fingerprint a = returned(1);
    Fingerprint b = returned(1);
    a.truncated = true;
    b.writes = "different";
    CB_CHECK(compare::first_difference(a, b) == Difference::None);
});

CB_TEST("verify.compare.detail_says_what_differed", [] {
    Fingerprint a = returned(0x10);
    Fingerprint b = a;
    b.exit = "exception: bad";
    CB_CHECK_EQ(compare::difference_detail(a, b), std::string("exit ret vs exception: bad"));
    b = a;
    b.ret = 0x2a;
    CB_CHECK_EQ(compare::difference_detail(a, b), std::string("ret 10 vs 2a"));
    b = a;
    a.writes = "";                                         // (an empty text reads as "none", as before)
    b.writes = "esp+4:4:1";
    CB_CHECK_EQ(compare::difference_detail(a, b), std::string("writes none vs esp+4"));
    b = a;
    b.pop = 12;
    a.pop = 8;
    CB_CHECK_EQ(compare::difference_detail(a, b), std::string("removes 8 bytes on return vs 12"));
    b = a;
    b.saved = false;
    CB_CHECK_EQ(compare::difference_detail(a, b), std::string("saved registers preserved vs changed"));
});

CB_TEST("verify.compare.detail_names_the_first_call_that_differs", [] {
    Fingerprint a = returned(0, {"Open(1,2)", "Read(3)", "Close(3)"});
    Fingerprint b = a;
    b.deps = {"Open(1,2)", "Read(4)"};
    CB_CHECK_EQ(compare::difference_detail(a, b), std::string("call #1 same call, argument 0 differs: Read(3) vs Read(4)"));
    b.deps = {"Open(1,2)", "Read(3)", "Close(3)", "Extra(1)"};
    CB_CHECK_EQ(compare::difference_detail(a, b), std::string("call #3 rebuilt calls extra: (nothing) vs Extra(1)"));
    b.deps = {"Open(1,2)", "Read(3)"};
    CB_CHECK_EQ(compare::difference_detail(a, b), std::string("call #2 rebuilt stops calling: Close(3) vs (nothing)"));
    b.deps = {"Open(1,2)", "Write(3)", "Close(3)"};
    CB_CHECK_EQ(compare::difference_detail(a, b), std::string("call #1 different call: Read(3) vs Write(3)"));
    b.deps = {"Open(1,9)", "Read(3)", "Close(3)"};
    CB_CHECK_EQ(compare::difference_detail(a, b), std::string("call #0 same call, argument 1 differs: Open(1,2) vs Open(1,9)"));
    Fingerprint c = returned(0, {"Open(7,8,9)"});
    Fingerprint d = returned(0, {"Open(0,8,1)"});
    CB_CHECK_EQ(compare::difference_detail(c, d), std::string("call #0 same call, argument 0+2 differs: Open(7,8,9) vs Open(0,8,1)"));
});

CB_TEST("verify.compare.detail_names_the_input_and_is_cut_at_300_bytes", [] {
    Fingerprint a = returned(1), b = returned(2);
    Input v;
    v.args = {0x10, 255};
    CB_CHECK_EQ(compare::difference_detail(a, b, &v), std::string("ret 1 vs 2  [input 10,ff]"));
    v.stub_ret = 0x20;
    CB_CHECK_EQ(compare::difference_detail(a, b, &v), std::string("ret 1 vs 2  [input 10,ff ret=20]"));
    v.stub_ret = 0;                                        // zero means nothing set
    CB_CHECK_EQ(compare::difference_detail(a, b, &v), std::string("ret 1 vs 2  [input 10,ff]"));
    a.deps = {std::string(500, 'x') + "(1)"};
    b.deps = {};
    CB_CHECK_EQ(compare::difference_detail(a, b).size(), std::size_t{300});
});

namespace {

using Run = std::function<Fingerprint(const TestInput&)>;

compare::Outcome judge(const Run& original, const Run& rebuilt, const std::vector<TestInput>& vectors,
                       const std::vector<TestInput>& confirm = {}, std::int64_t insns = 0) {
    return compare::compare<TestInput>(original, rebuilt, vectors, confirm, {.insns = insns});
}

}  // namespace

CB_TEST("verify.compare.equal_runs_with_a_clean_return_are_equivalent", [] {
    Run same = [](const TestInput& v) { return returned(v.id); };
    const compare::Outcome o = judge(same, same, {input(1), input(2)});
    CB_CHECK(o.verdict == Verdict::Equivalent);
    CB_CHECK(o.limits.empty());
    CB_CHECK(o.limit_codes().empty());
});

CB_TEST("verify.compare.the_first_difference_settles_it_and_later_inputs_are_not_run", [] {
    int rebuilt_runs = 0;
    Run original = [](const TestInput& v) { return returned(v.id); };
    Run rebuilt = [&](const TestInput& v) {
        ++rebuilt_runs;
        return returned(v.id == 2 ? 99 : v.id);
    };
    const compare::Outcome o = judge(original, rebuilt, {input(1), input(2, {7}), input(3)});
    CB_CHECK(o.verdict == Verdict::Divergent);
    CB_CHECK_EQ(o.reason, std::string("ret-differs"));
    CB_CHECK_EQ(o.detail, std::string("ret 2 vs 63  [input 7]"));
    CB_CHECK(o.limits.empty());
    CB_CHECK_EQ(rebuilt_runs, 2);
});

CB_TEST("verify.compare.confirm_inputs_run_only_after_a_clean_return", [] {
    int runs = 0;
    Run original = [&](const TestInput& v) {
        ++runs;
        return returned(v.id);
    };
    Run rebuilt = [](const TestInput& v) { return returned(v.id == 50 ? 0 : v.id); };   // differs on a confirm input only
    compare::Outcome o = judge(original, rebuilt, {input(1)}, {input(50)});
    CB_CHECK(o.verdict == Verdict::Divergent);
    CB_CHECK_EQ(runs, 2);

    // no clean return among the basic inputs: the confirm inputs are not run
    runs = 0;
    Run faults = [&](const TestInput&) {
        ++runs;
        Fingerprint f;
        f.exit = "exception: unmapped";
        f.writes = "none";
        return f;
    };
    o = judge(faults, faults, {input(1)}, {input(50)});
    CB_CHECK(o.verdict == Verdict::Consistent);
    CB_CHECK_EQ(runs, 2);   // (one input, both sides: the confirm input never ran)
});

CB_TEST("verify.compare.failing_alike_is_consistent_and_says_so", [] {
    Run faults = [](const TestInput&) {
        Fingerprint f;
        f.exit = "exception: unmapped";
        f.writes = "none";
        return f;
    };
    const compare::Outcome o = judge(faults, faults, {input(1)});
    CB_CHECK(o.verdict == Verdict::Consistent);
    CB_CHECK(o.limits.contains(WeakLimit::FaultOnly));
    CB_CHECK(classify(o.verdict, o.limits).cls == Class::Weak);
});

CB_TEST("verify.compare.what_makes_a_match_weaker", [] {
    Run original = [](const TestInput& v) {
        Fingerprint f = returned(v.id, {"Api", "...", "Other(1)"});   // (an import of unknown arity; a list cut at its cap)
        f.writes.reset();                                              // (more writes than are recorded)
        return f;
    };
    const compare::Outcome o = judge(original, original, {input(1)});
    CB_CHECK(o.verdict == Verdict::Equivalent);
    CB_CHECK(o.limits.contains(WeakLimit::ArgcUnknown));
    CB_CHECK(o.limits.contains(WeakLimit::CallsCapped));
    CB_CHECK(o.limits.contains(WeakLimit::WritesCapped));
    CB_CHECK(classify(o.verdict, o.limits).cls == Class::Weak);
    const std::vector<std::string> expected = {"argc-unknown", "calls-capped", "writes-capped"};
    CB_CHECK(o.limit_codes() == expected);
});

CB_TEST("verify.compare.a_truncated_run_is_not_a_clean_return", [] {
    Run cut = [](const TestInput&) {
        Fingerprint f;
        f.exit = "truncated";
        f.truncated = true;
        f.writes = "none";
        return f;
    };
    const compare::Outcome o = judge(cut, cut, {input(1)});
    CB_CHECK(o.verdict == Verdict::Consistent);
    CB_CHECK(o.limits.contains(WeakLimit::Truncated));
    CB_CHECK(o.limits.contains(WeakLimit::FaultOnly));
});

CB_TEST("verify.compare.coverage_is_a_share_of_the_originals_instructions", [] {
    Run original = [](const TestInput& v) {
        Fingerprint f = returned(v.id);
        f.cov = {1, 2, 3};
        f.reach = {1, 2, 3, 4};
        return f;
    };
    compare::Outcome o = judge(original, original, {input(1)}, {}, 10);   // 3 of 10: under half
    CB_CHECK(o.verdict == Verdict::Equivalent);
    CB_CHECK(o.coverage == 30);
    CB_CHECK(o.limits.contains(WeakLimit::LowCoverage));
    const std::vector<std::string> expected = {"coverage=30", "low-coverage"};
    CB_CHECK(o.limit_codes() == expected);
    o = judge(original, original, {input(1)}, {}, 4);                     // 3 of 4: enough
    CB_CHECK(o.coverage == 75);
    CB_CHECK(!o.limits.contains(WeakLimit::LowCoverage));
    o = judge(original, original, {input(1)}, {}, 2);                     // (never more than 100)
    CB_CHECK(o.coverage == 100);
});

CB_TEST("verify.compare.inputs_that_could_not_be_traced_are_not_a_judgement", [] {
    Run broken = [](const TestInput&) {
        Fingerprint f;
        f.err = "no entry";
        return f;
    };
    Run fine = [](const TestInput& v) { return returned(v.id); };
    compare::Outcome o = judge(broken, fine, {input(1), input(2)});
    CB_CHECK(o.verdict == Verdict::Inconclusive);
    CB_CHECK_EQ(o.reason, std::string("retail: no entry"));
    CB_CHECK(o.limits.contains(WeakLimit::Unobserved));
    // some inputs traced, some not: it is judged on the ones that were, and says so
    Run half = [](const TestInput& v) {
        if (v.id == 1) {
            Fingerprint f;
            f.err = "x";
            return f;
        }
        return returned(v.id);
    };
    o = judge(half, half, {input(1), input(2)});
    CB_CHECK(o.verdict == Verdict::Equivalent);
    CB_CHECK(o.limits.contains(WeakLimit::Unobserved));
    // the rebuilt side failing
    Run rebuilt_broken = [](const TestInput& v) {
        Fingerprint f;
        if (v.id == 1) f.err = "build";
        else f = returned(v.id);
        return f;
    };
    o = judge(fine, rebuilt_broken, {input(1), input(2)});
    CB_CHECK(o.verdict == Verdict::Equivalent);
    CB_CHECK(o.limits.contains(WeakLimit::Unobserved));
});

CB_TEST("verify.compare.leftover_bits_are_asked_only_for_a_return_that_differs", [] {
    Run original = [](const TestInput& v) { return returned(0x11110000 | v.id); };
    Run rebuilt = [](const TestInput& v) { return returned(0x22220000 | v.id); };
    int asked = 0;
    auto leftover = [&](const std::function<Fingerprint(const TestInput&)>&, const TestInput&, const Fingerprint&) -> std::uint32_t {
        ++asked;
        return 0xFFFF0000u;
    };
    const std::vector<TestInput> vs = {input(1), input(2)};
    const compare::Outcome o = compare::compare<TestInput>(original, rebuilt, vs, {}, {}, nullptr, leftover);
    CB_CHECK(o.verdict == Verdict::Equivalent);   // only the low half is a result, and it agrees
    CB_CHECK(o.ret_bits == 16);
    CB_CHECK_EQ(asked, 2);
    // a leftover that does not cover the difference
    auto no_help = [&](const std::function<Fingerprint(const TestInput&)>&, const TestInput&, const Fingerprint&) -> std::uint32_t { return 0xFFFFFF00u; };
    Run rebuilt_low = [](const TestInput& v) { return returned(0x11110000 | (v.id + 1)); };
    const compare::Outcome p = compare::compare<TestInput>(original, rebuilt_low, vs, {}, {}, nullptr, no_help);
    CB_CHECK(p.verdict == Verdict::Divergent);
    CB_CHECK(p.ret_bits == 8);
});

CB_TEST("verify.compare.directed_inputs_aim_at_what_no_input_reached", [] {
    // The basic input reaches instruction 1 only; the directed one reaches 2 and shows the rebuild differs there.
    Run original = [](const TestInput& v) {
        Fingerprint f = returned(v.id);
        f.cov = v.id == 1 ? std::vector<std::int64_t>{1} : std::vector<std::int64_t>{1, 2};
        return f;
    };
    Run rebuilt = [](const TestInput& v) { return returned(v.id == 2 ? 0 : v.id); };
    std::set<std::int64_t> seen_covered;
    auto directed = [&](const std::set<std::int64_t>& covered, const std::set<std::int64_t>&) {
        seen_covered = covered;
        return std::vector<TestInput>{input(2)};
    };
    const std::vector<TestInput> vs = {input(1)};
    const compare::Outcome o = compare::compare<TestInput>(original, rebuilt, vs, {}, {}, directed);
    CB_CHECK(o.verdict == Verdict::Divergent);
    CB_CHECK(seen_covered == std::set<std::int64_t>{1});
});

CB_TEST("verify.compare.a_body_that_only_returns_on_a_directed_input_is_then_confirmed", [] {
    int confirm_runs = 0;
    Run original = [&](const TestInput& v) {
        if (v.id == 100) ++confirm_runs;
        if (v.id < 10) {                      // the basic inputs fault
            Fingerprint f;
            f.exit = "exception: x";
            f.writes = "none";
            return f;
        }
        return returned(v.id);
    };
    auto directed = [](const std::set<std::int64_t>&, const std::set<std::int64_t>&) { return std::vector<TestInput>{input(10)}; };
    const std::vector<TestInput> vs = {input(1)}, confirm = {input(100)};
    const compare::Outcome o = compare::compare<TestInput>(original, original, vs, confirm, {}, directed);
    CB_CHECK(o.verdict == Verdict::Equivalent);
    CB_CHECK_EQ(confirm_runs, 2);   // (one run of the confirm input on each side)
});

CB_TEST("verify.compare.fingerprints_round_trip_through_the_cache", [] {
    cb::fs::ScopedTempDir dir("cb-compare");
    cb::base::cache::Config config;
    config.state_dir = dir.path() / "state";
    config.shared_dir = dir.path() / "shared";
    cb::base::cache::Cache cache(config);
    compare::declare_cache(cache);
    const std::string key = compare::cache_key({"module-hash", "0x401000", 7});
    CB_CHECK(!compare::cache_get(cache, key).has_value());
    Fingerprint f = returned(0x1234, {"A(1)", "B()"});
    f.cov = {0x401000, 0x401004};
    f.reach = {0x401000, 0x401004, 0x401008};
    f.steps = 42;
    f.err = std::nullopt;
    compare::cache_put(cache, key, f);
    const auto back = compare::cache_get(cache, key);
    CB_CHECK(back.has_value());
    CB_CHECK(back && *back == f);
    // a fingerprint with every optional empty
    Fingerprint bare;
    bare.exit = "truncated";
    bare.truncated = true;
    compare::cache_put(cache, "other", bare);
    CB_CHECK(compare::cache_get(cache, "other") == std::optional<Fingerprint>(bare));
});

CB_TEST("verify.compare.the_stored_form_decodes_or_is_a_miss", [] {
    Fingerprint f = returned(9, {"X(1)"});
    f.err = "oops";
    const cb::Bytes stored = compare::encode(f);
    CB_CHECK(compare::decode(stored) == std::optional<Fingerprint>(f));
    for (std::size_t cut : {std::size_t{0}, std::size_t{3}, stored.size() - 1}) {
        CB_CHECK(!compare::decode(cb::ByteView(stored.data(), cut)).has_value());
    }
});

CB_TEST("verify.compare.keys_name_the_revision_and_every_ingredient", [] {
    const std::string a = compare::cache_key({"m", "f", 1});
    CB_CHECK(a.starts_with("r25:"));
    CB_CHECK_EQ(a.size(), std::size_t{4 + 40});
    CB_CHECK_EQ(a, compare::cache_key({"m", "f", 1}));
    CB_CHECK_NE(a, compare::cache_key({"m", "f", 2}));
    CB_CHECK_NE(compare::cache_key({"ab", "c"}), compare::cache_key({"a", "bc"}));   // (parts are delimited)
    CB_CHECK_NE(compare::cache_key_text("x"), compare::cache_key_text("y"));
});
