// Tests of the path explorer, over small functions modelled in C++: each input's run is worked out by the test (what
// the emulator and its shadow do for the real lanes), and the questions go to the real Z3.
#include "cb/verify/concolic.hpp"

#include "cb/core/testing.hpp"

namespace {

using namespace cb::verify::concolic;
namespace sym = cb::emulate::symbolic;

struct In {
    std::uint32_t x = 0;
    std::uint32_t y = 0;
    bool seen = false;   // noted by a run, as the lanes' runs note things in their input
};

// A function of one 32-bit argument `x` (and `y`): a list of tests, each `x <op> constant` (or `y ...`); a run takes
// them in order and stops at the first one that is not taken (as a chain of `if ... return`).
struct Test {
    int site;
    sym::Op op;                 // Eq ULt ULe ...
    std::uint32_t constant;
    bool on_y = false;
    bool swapped = false;       // the constant on the left: `constant <op> input`
    Kind kind = Kind::Jump;
};

class Model {
public:
    explicit Model(std::vector<Test> tests, bool continue_after_not_taken = false)
        : tests_(std::move(tests)), continue_(continue_after_not_taken) {}

    bool holds(const Test& t, const In& v) const {
        const std::uint32_t in = t.on_y ? v.y : v.x;
        const std::uint32_t a = t.swapped ? t.constant : in, b = t.swapped ? in : t.constant;
        switch (t.op) {
            case sym::Op::Eq: return a == b;
            case sym::Op::ULt: return a < b;
            case sym::Op::ULe: return a <= b;
            case sym::Op::SLt: return static_cast<std::int32_t>(a) < static_cast<std::int32_t>(b);
            default: return false;
        }
    }

    std::optional<Observation> run(In& v) {
        v.seen = true;
        Observation o;
        for (const Test& t : tests_) {
            const sym::Expr* in = pool_.var(t.on_y ? "arg1" : "arg0", 32);
            const sym::Expr* k = pool_.constant(t.constant, 32);
            const sym::Expr* cond = pool_.compare(t.op, t.swapped ? k : in, t.swapped ? in : k);
            const bool taken = holds(t, v);
            o.path.push_back({t.site, taken, cond, t.kind});
            if (t.kind != Kind::Pin) o.trail.push_back({t.site, taken});
            if (!taken && !continue_) break;
        }
        o.vars["arg0"] = {InputVar::Place::Arg, 0, 32, static_cast<std::int64_t>(v.x)};
        o.vars["arg1"] = {InputVar::Place::Arg, 1, 32, static_cast<std::int64_t>(v.y)};
        ++runs_;
        return o;
    }

    static std::optional<In> derive(const In& v, const Values& values, const Vars&) {
        In w = v;
        w.seen = false;
        if (auto it = values.find("arg0"); it != values.end()) w.x = static_cast<std::uint32_t>(it->second);
        if (auto it = values.find("arg1"); it != values.end()) w.y = static_cast<std::uint32_t>(it->second);
        return w;
    }

    Exploration<In> explore(std::vector<In> seeds, Options options = {}, std::map<std::size_t, Trail> known = {}) {
        const std::function<std::optional<Observation>(In&)> run = [&](In& v) { return this->run(v); };
        const std::function<std::optional<In>(const In&, const Values&, const Vars&)> derive = &Model::derive;
        seeds_ = std::move(seeds);
        return cb::verify::concolic::explore<In>(run, seeds_, derive, options, known);
    }

    int runs() const { return runs_; }
    const std::vector<In>& seeds() const { return seeds_; }

private:
    std::vector<Test> tests_;
    bool continue_;
    sym::ExprPool pool_;
    std::vector<In> seeds_;
    int runs_ = 0;
};

}  // namespace

CB_TEST("verify.concolic.finds_each_path_of_a_chain_of_tests", [] {
    // if (x == 5) { if (x < 3) ... } -- a run stops at the first test not taken
    Model m({{1, sym::Op::Eq, 5}, {2, sym::Op::ULt, 3}});
    Exploration<In> got = m.explore({In{0}});
    CB_CHECK(got.complete);
    CB_CHECK_EQ(got.paths, 2);        // x != 5, and x == 5 (where 5 < 3 does not hold)
    CB_CHECK(!got.budget_spent);
    CB_CHECK_EQ(got.unknown, 0);
    CB_CHECK_EQ(got.missed, 0);
    CB_CHECK_EQ(got.unsat, 1);        // x == 5 and x < 3 together: a path that does not exist
    CB_CHECK_EQ(got.kept.size(), std::size_t{1});
    CB_CHECK_EQ(got.kept[0].x, 5u);
    CB_CHECK(got.edges.contains(Step{1, false}));
    CB_CHECK(got.edges.contains(Step{1, true}));
    CB_CHECK(got.edges.contains(Step{2, false}));
    CB_CHECK(!got.edges.contains(Step{2, true}));
});

CB_TEST("verify.concolic.three_ways_through_two_branches", [] {
    // the second test is reached whether or not the first is taken: x==5 && x<3 is the one that cannot be
    Model m({{1, sym::Op::Eq, 5}, {2, sym::Op::ULt, 3}}, true);
    Exploration<In> got = m.explore({In{0}});
    CB_CHECK(got.complete);
    CB_CHECK_EQ(got.paths, 3);
    CB_CHECK_EQ(got.unsat, 1);
    CB_CHECK_EQ(got.kept.size(), std::size_t{2});
    CB_CHECK_EQ(got.summary(), std::string("all 3 paths, 3 runs, 3 questions (1 impossible)"));
    // the inputs kept: x == 5, and the one that is neither 5 nor under 3 (at its bound: 3)
    std::set<std::uint32_t> xs;
    for (const In& v : got.kept) xs.insert(v.x);
    CB_CHECK(xs == (std::set<std::uint32_t>{3, 5}));
});

CB_TEST("verify.concolic.an_impossible_turn_is_a_path_that_does_not_exist", [] {
    // x < 3, then 10 < x: with the first taken the second cannot be turned
    Model m({{1, sym::Op::ULt, 3}, {2, sym::Op::ULt, 10, false, true}});
    Exploration<In> got = m.explore({In{0}});
    CB_CHECK(got.complete);
    CB_CHECK_EQ(got.unsat, 1);
    CB_CHECK_EQ(got.paths, 2);        // x < 3 (then the second test not taken), x >= 3
    CB_CHECK_EQ(got.kept.size(), std::size_t{1});
    CB_CHECK(got.kept[0].x >= 3);
});

CB_TEST("verify.concolic.the_input_that_turns_a_comparison_sits_at_its_bound", [] {
    Model m({{1, sym::Op::ULt, 100}});
    Exploration<In> got = m.explore({In{7}});
    CB_CHECK_EQ(got.kept.size(), std::size_t{1});
    CB_CHECK_EQ(got.kept[0].x, 100u);      // what was compared equal to what it was compared with
    // ... and for `<=`, the other side of the bound is one past it
    Model n({{1, sym::Op::ULe, 100}});
    Exploration<In> got2 = n.explore({In{7}});
    CB_CHECK_EQ(got2.kept.size(), std::size_t{1});
    CB_CHECK(got2.kept[0].x == 101u);      // (x == 100 does not turn x <= 100; one past does)
});

CB_TEST("verify.concolic.signed_comparisons_and_the_other_input", [] {
    Model m({{1, sym::Op::SLt, 0, /*on_y=*/true}});
    Exploration<In> got = m.explore({In{0, 5}});
    CB_CHECK_EQ(got.kept.size(), std::size_t{1});
    CB_CHECK_EQ(got.kept[0].y, 0xFFFFFFFFu);                      // -1: the bound's neighbour that turns y < 0
    CB_CHECK(m.holds({1, sym::Op::SLt, 0, true}, got.kept[0]));   // y is now negative
});

CB_TEST("verify.concolic.the_budget_limits_runs_and_says_the_paths_are_a_sample", [] {
    Model m({{1, sym::Op::Eq, 1}, {2, sym::Op::Eq, 2}, {3, sym::Op::Eq, 3}, {4, sym::Op::Eq, 4}});
    Options o;
    o.budget = 2;
    Exploration<In> got = m.explore({In{0}}, o);
    CB_CHECK_EQ(got.runs, 2);
    CB_CHECK(got.budget_spent);
    CB_CHECK(!got.complete);
    CB_CHECK(got.summary().find("(sampled)") != std::string::npos);
    CB_CHECK_EQ(m.runs(), 2);
});

CB_TEST("verify.concolic.questions_are_limited_too", [] {
    Model m({{1, sym::Op::Eq, 1}, {2, sym::Op::Eq, 2}});
    Options o;
    o.questions = 1;
    Exploration<In> got = m.explore({In{0}}, o);
    CB_CHECK_EQ(got.queries, 1);
    CB_CHECK(got.budget_spent);
    CB_CHECK(!got.complete);
});

CB_TEST("verify.concolic.keeps_at_most_what_it_is_asked_to", [] {
    Model m({{1, sym::Op::Eq, 1}, {2, sym::Op::Eq, 2}, {3, sym::Op::Eq, 3}});
    Options o;
    o.keep = 1;
    Exploration<In> got = m.explore({In{0}}, o);
    CB_CHECK_EQ(got.kept.size(), std::size_t{1});
    CB_CHECK(!got.complete);
});

CB_TEST("verify.concolic.seeds_that_took_a_known_path_are_not_run_again", [] {
    Model m({{1, sym::Op::ULt, 100}});
    // two seeds the judge already ran, both taking the jump: only the first is run (with its shadow)
    std::map<std::size_t, Trail> known = {{0, {{1, true}}}, {1, {{1, true}}}, {2, {{1, false}}}};
    Exploration<In> got = m.explore({In{1}, In{2}, In{500}}, {}, known);
    CB_CHECK(m.seeds()[0].seen);
    CB_CHECK(!m.seeds()[1].seen);
    CB_CHECK(m.seeds()[2].seen);
    CB_CHECK_EQ(got.paths, 2);
    CB_CHECK_EQ(got.kept.size(), std::size_t{0});   // both paths were already found by the seeds
    CB_CHECK(got.complete);
});

CB_TEST("verify.concolic.only_the_functions_own_jumps_count_as_paths", [] {
    // site 9 belongs to a function it calls
    Model m({{9, sym::Op::ULt, 50}, {1, sym::Op::ULt, 100}}, true);
    Options o;
    o.own = [](std::int64_t site) { return site != 9; };
    Exploration<In> got = m.explore({In{0}}, o);
    CB_CHECK(got.complete);
    CB_CHECK_EQ(got.paths, 2);                       // by site 1 alone
    CB_CHECK(!got.edges.contains(Step{9, true}));    // (the callee's jumps are not the function's edges)
    CB_CHECK_EQ(got.unsat, 1);                       // x < 50 and x >= 100 together
    // the callee's other side is looked for too, and leads on to the function's own second path
    CB_CHECK_EQ(got.kept.size(), std::size_t{1});
    CB_CHECK_EQ(got.kept[0].x, 100u);
});

CB_TEST("verify.concolic.a_pinned_condition_is_held_but_not_asked", [] {
    // y is held where the inputs put it (a pointer argument): a condition that is a pin
    Model m({{5, sym::Op::Eq, 7, /*on_y=*/true, false, Kind::Pin}, {1, sym::Op::ULt, 10}}, true);
    Exploration<In> got = m.explore({In{0, 7}});
    CB_CHECK_EQ(got.pins, 2);                        // one in each run
    CB_CHECK_EQ(got.kept.size(), std::size_t{1});
    CB_CHECK_EQ(got.kept[0].y, 7u);                  // still where it was pinned
    CB_CHECK(got.kept[0].x >= 10);
});

CB_TEST("verify.concolic.a_run_that_is_not_an_observation_is_a_missed_question", [] {
    Model m({{1, sym::Op::ULt, 100}});
    int runs = 0;
    const std::function<std::optional<Observation>(In&)> run = [&](In& v) -> std::optional<Observation> {
        return ++runs == 1 ? m.run(v) : std::nullopt;   // the derived input cannot be traced
    };
    const std::function<std::optional<In>(const In&, const Values&, const Vars&)> derive = &Model::derive;
    std::vector<In> seeds = {In{1}};
    Exploration<In> got = explore<In>(run, seeds, derive);
    CB_CHECK_EQ(got.missed, 1);
    CB_CHECK(!got.complete);
    CB_CHECK(got.kept.empty());
});

CB_TEST("verify.concolic.an_input_that_cannot_be_made_is_a_missed_question", [] {
    Model m({{1, sym::Op::ULt, 100}});
    const std::function<std::optional<Observation>(In&)> run = [&](In& v) { return m.run(v); };
    const std::function<std::optional<In>(const In&, const Values&, const Vars&)> derive = [](const In&, const Values&, const Vars&) { return std::nullopt; };
    std::vector<In> seeds = {In{1}};
    Exploration<In> got = explore<In>(run, seeds, derive);
    CB_CHECK_EQ(got.missed, 1);
    CB_CHECK(!got.complete);
});

CB_TEST("verify.concolic.a_run_that_was_cut_off_is_counted_but_not_kept", [] {
    Model m({{1, sym::Op::ULt, 100}});
    const std::function<std::optional<Observation>(In&)> run = [&](In& v) {
        std::optional<Observation> o = m.run(v);
        if (o && v.x >= 100) o->cut = true;      // the other way never ends
        return o;
    };
    const std::function<std::optional<In>(const In&, const Values&, const Vars&)> derive = &Model::derive;
    std::vector<In> seeds = {In{1}};
    Exploration<In> got = explore<In>(run, seeds, derive);
    CB_CHECK_EQ(got.paths, 2);
    CB_CHECK(got.kept.empty());
});

CB_TEST("verify.concolic.what_the_shadow_lost_means_the_paths_are_not_all_of_them", [] {
    Model m({{1, sym::Op::ULt, 100}});
    const std::function<std::optional<Observation>(In&)> run = [&](In& v) {
        std::optional<Observation> o = m.run(v);
        if (o) o->lost.insert("a value went into a table");
        return o;
    };
    const std::function<std::optional<In>(const In&, const Values&, const Vars&)> derive = &Model::derive;
    std::vector<In> seeds = {In{1}};
    Exploration<In> got = explore<In>(run, seeds, derive);
    CB_CHECK(!got.complete);
    CB_CHECK(got.lost.contains("a value went into a table"));
});

CB_TEST("verify.concolic.exploring_nothing_is_not_complete", [] {
    const std::function<std::optional<Observation>(In&)> run = [](In&) { return std::nullopt; };
    const std::function<std::optional<In>(const In&, const Values&, const Vars&)> derive = &Model::derive;
    std::vector<In> seeds = {In{1}};
    Exploration<In> got = explore<In>(run, seeds, derive);
    CB_CHECK(!got.complete);
    CB_CHECK_EQ(got.runs, 1);
    CB_CHECK_EQ(got.paths, 0);
});

CB_TEST("verify.concolic.two_explorations_do_not_share_a_solver_context", [] {
    // the same questions give the same answers however many explorations came before
    auto kept_of_one = [] {
        Model m({{1, sym::Op::ULt, 1000}, {2, sym::Op::Eq, 77}}, true);
        Exploration<In> got = m.explore({In{0}});
        std::vector<std::uint32_t> xs;
        for (const In& v : got.kept) xs.push_back(v.x);
        return xs;
    };
    const std::vector<std::uint32_t> first = kept_of_one();
    for (int i = 0; i < 3; ++i) CB_CHECK(kept_of_one() == first);
    CB_CHECK(!first.empty());
});
