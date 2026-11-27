#include "verify/reconstruct.hpp"

#include <algorithm>
#include <atomic>
#include <set>
#include <tuple>
#include <vector>

#include "core/fs.hpp"
#include "core/testing.hpp"

// Design notes:
//   * a file nothing reconstructs is a UsageError; `try_identity` is the form for a caller
//     that treats that as "no identity";
//   * the judge runs in this process (one at a time, or `judge_threads` threads) unless the caller gives `judge_batch`;
//   * the engine's way of making the same results (engine/lanes) is not part of this module;
//   * a run reports what it did in an `Outcome` (its `line()` is the one line printed).

using namespace cb;
using namespace cb::store;
using namespace cb::verify;
using namespace cb::verify::reconstruct;

namespace {

// A module of functions whose texts are kept in a table the test changes between runs.
struct Module {
    std::vector<std::string> order;
    std::map<std::string, std::string> texts;
};

struct Counters {
    std::atomic<int> built{0}, judged{0};
};

class FakeLane final : public lanes::Kind {
public:
    FakeLane(const Module& module, Counters& counters, std::set<std::string> not_built = {}, std::string digest = "d1")
        : module_(module), counters_(counters), not_built_(std::move(not_built)), digest_(std::move(digest)) {}

    const std::string& name() const override {
        static const std::string kName = "fake";
        return kName;
    }
    lanes::Profile profile(const std::filesystem::path&) const override { return {digest_, "fake settings " + digest_}; }
    std::unique_ptr<lanes::Prepared> prepare(const std::filesystem::path&, const std::filesystem::path&) const override {
        auto p = std::make_unique<lanes::Prepared>();
        p->header = "typedef int T;";
        p->order = module_.order;
        for (const std::string& n : module_.order) {
            p->key[n] = "key:" + n;
            p->all_keys.push_back("key:" + n);
            p->texts[n] = module_.texts.at(n);
        }
        p->all_keys.push_back("key:nothing");   // a function the decompiler made no text for
        p->sig = "whole";
        return p;
    }
    lanes::Built build(const lanes::Prepared& p, const std::filesystem::path&, const std::filesystem::path&) const override {
        ++counters_.built;
        lanes::Built b;
        for (const std::string& n : p.order) {
            if (not_built_.contains(n))
                b.missing[n] = "does not compile: " + n;
            else
                b.items.push_back(n);
        }
        b.judge = [this](const std::string& n) {
            ++counters_.judged;
            store::Judgement j;
            if (n == "bad") {
                j.verdict = store::Verdict::Divergent;
                j.reason = "ret-differs";
            } else if (n == "boom") {
                throw std::runtime_error("the emulator fell over");
            } else {
                j.verdict = store::Verdict::Equivalent;
                j.reason = "every compared result alike";
            }
            return j;
        };
        return b;
    }

private:
    const Module& module_;
    Counters& counters_;
    std::set<std::string> not_built_;
    std::string digest_;
};

struct Fixture {
    fs::ScopedTempDir tmp{"cb-reconstruct"};
    store::FunctionStore store{tmp.path() / "functions.sqlite", tmp.path() / "db"};
    Kinds kinds;
    Module module;
    Counters counters;
    std::vector<std::string> log;
    std::optional<Reconstructor> rec;

    explicit Fixture(Module m, std::set<std::string> not_built = {}) : module(std::move(m)) {
        kinds.add(std::make_shared<FakeLane>(module, counters, std::move(not_built)));
        Services s{.kinds = kinds, .store = store};
        s.kind_of_file = [](const std::filesystem::path& p) -> std::optional<std::string> {
            return p.extension() == ".fk" ? std::optional<std::string>("fake") : std::nullopt;
        };
        s.content_sha = [](const std::filesystem::path&) { return std::optional<std::string>("sha-of-the-module"); };
        s.scratch = tmp.path() / "work";
        s.identify = [](const std::filesystem::path&) {
            return std::map<std::string, std::pair<std::string, std::string>>{{"key:strlen", {"strlen", "CRT"}}};
        };
        s.decompiler_rev = "42";
        s.log = [this](std::string_view line) { log.emplace_back(line); };
        rec.emplace(std::move(s));
    }
    std::map<std::string, store::Best> best() { return store.best("sha-of-the-module", "d1"); }
};

}  // namespace

CB_TEST("verify.reconstruct.signatures_follow_what_a_function_reaches", [] {
    lanes::Prepared p;
    p.header = "typedef int T;";
    p.order = {"a", "b", "c", "d"};
    p.texts = {{"a", "int a(void) { return b(); }"}, {"b", "int b(void) { return 1; }"}, {"c", "int c(void) { return 2; }"},
               {"d", "int d(void) { return 3 + 0x1f; }"}};
    const auto before = signatures(p, "whole");
    CB_CHECK_EQ(before.size(), 4u);
    CB_CHECK_EQ(before.at("a").size(), 40u);

    // a change to c touches only c
    p.texts["c"] = "int c(void) { return 22; }";
    const auto after_c = signatures(p, "whole");
    CB_CHECK_NE(after_c.at("c"), before.at("c"));
    CB_CHECK_EQ(after_c.at("a"), before.at("a"));
    CB_CHECK_EQ(after_c.at("b"), before.at("b"));
    CB_CHECK_EQ(after_c.at("d"), before.at("d"));

    // a change to b touches b and its caller a
    p.texts["b"] = "int b(void) { return 11; }";
    const auto after_b = signatures(p, "whole");
    CB_CHECK_NE(after_b.at("b"), after_c.at("b"));
    CB_CHECK_NE(after_b.at("a"), after_c.at("a"));
    CB_CHECK_EQ(after_b.at("d"), after_c.at("d"));

    // a change to the header touches everything
    p.header = "typedef long T;";
    const auto after_h = signatures(p, "whole");
    for (const std::string n : {"a", "b", "c", "d"})
        CB_CHECK_NE(after_h.at(n), after_b.at(n));
});

CB_TEST("verify.reconstruct.signatures_of_a_ring_are_one", [] {
    lanes::Prepared p;
    p.header = "h";
    p.order = {"ping", "pong", "lone"};
    p.texts = {{"ping", "int ping(void) { return pong(); }"}, {"pong", "int pong(void) { return ping(); }"}, {"lone", "int lone(void) { return 0; }"}};
    const auto s = signatures(p, "whole");
    CB_CHECK_EQ(s.at("ping"), s.at("pong"));
    CB_CHECK_NE(s.at("ping"), s.at("lone"));
});

CB_TEST("verify.reconstruct.signatures_of_what_was_built_ignore_the_source", [] {
    lanes::Prepared p;
    p.header = "h1";
    p.order = {"a", "b"};
    p.texts = {{"a", "int a(void) { return b(); }"}, {"b", "int b(void) { return 1; }"}};
    const std::map<std::string, std::string> objects = {{"a", "code-a"}, {"b", "code-b"}};
    const auto first = signatures(p, {}, &objects);
    p.header = "h2";
    p.texts["b"] = "int b(void) { /* note */ return 1; }";
    CB_CHECK(signatures(p, {}, &objects) == first);
    const std::map<std::string, std::string> changed = {{"a", "code-a"}, {"b", "code-b2"}};
    const auto second = signatures(p, {}, &changed);
    CB_CHECK_NE(second.at("b"), first.at("b"));
    CB_CHECK_NE(second.at("a"), first.at("a"));   // a reaches b
});

CB_TEST("verify.reconstruct.signatures_of_a_module_without_texts_are_the_whole", [] {
    lanes::Prepared p;
    p.order = {"x", "y"};
    const auto s = signatures(p, "the-whole");
    CB_CHECK_EQ(s.at("x"), "the-whole");
    CB_CHECK_EQ(s.at("y"), "the-whole");
});

CB_TEST("verify.reconstruct.reaches", [] {
    const std::unordered_map<std::string, std::string> texts = {
        {"a", "int a(void) { return b(1); }"}, {"b", "int b(int x) { return stub (x); }"}, {"c", "int c(void) { return 0; }"},
        {"d", "int d(void) { return a(); }"}, {"e", "int e(void) { return xstub(1); }"}};
    const std::vector<std::string> built = {"a", "b", "c", "d", "e"};
    const auto tainted = reaches(texts, built, {"stub"});
    CB_CHECK(tainted == (std::set<std::string>{"a", "b", "d"}));   // b calls stub, a calls b, d calls a; xstub is another name
    CB_CHECK(reaches(texts, built, {}).empty());
    // the name before the first "{" is the function's own, not a call
    const std::unordered_map<std::string, std::string> own = {{"stub", "int stub(void) { return 0; }"}};
    const std::vector<std::string> only = {"stub"};
    CB_CHECK(reaches(own, only, {"stub"}).empty());
});

CB_TEST("verify.reconstruct.a_run_judges_and_records", [] {
    Fixture f({{"ok", "bad", "gone"}, {{"ok", "int ok(void) { return 1; }"}, {"bad", "int bad(void) { return 2; }"}, {"gone", "int gone(void) { return 3; }"}}},
              {"gone"});
    const Outcome o = f.rec->run("m.fk");
    CB_CHECK_EQ(o.sha, "sha-of-the-module");
    CB_CHECK_EQ(o.profile, "d1");
    CB_CHECK_EQ(o.name, "m.fk");
    CB_CHECK_EQ(o.functions, 4u);   // the three with texts and the one the decompiler made nothing of
    CB_CHECK_EQ(o.judged, 3u);
    CB_CHECK_EQ(f.counters.judged.load(), 2);   // "gone" was not built
    CB_CHECK_EQ(f.log.size(), 1u);
    CB_CHECK(f.log[0].starts_with("m.fk: 4 functions, 3 judged now: "));

    const auto best = f.best();
    CB_CHECK_EQ(best.at("key:ok").cls, Class::Verified);
    CB_CHECK_EQ(best.at("key:bad").cls, Class::Unverified);
    CB_CHECK_EQ(best.at("key:gone").cls, Class::NoCompile);
    CB_CHECK_EQ(best.at("key:gone").candidate.reason, "does not compile: gone");
    CB_CHECK_EQ(best.at("key:nothing").cls, Class::NoSource);
    CB_CHECK_EQ(best.at("key:ok").candidate.rev.value_or(""), "42");
    CB_CHECK_EQ(best.at("key:ok").candidate.source.value_or(""), "int ok(void) { return 1; }");
    CB_CHECK_EQ(f.store.get_text("sha-of-the-module", "decompiler", "header"), "typedef int T;");
    CB_CHECK_EQ(o.scores.at(Class::Verified), 1);
    CB_CHECK_EQ(o.scores.at(Class::Unverified), 1);
    CB_CHECK_EQ(f.store.identified("sha-of-the-module").size(), 1u);
});

CB_TEST("verify.reconstruct.a_function_judged_for_its_text_is_not_built_again", [] {
    Fixture f({{"a", "b"}, {{"a", "int a(void) { return 1; }"}, {"b", "int b(void) { return 2; }"}}});
    (void)f.rec->run("m.fk");
    CB_CHECK_EQ(f.counters.built.load(), 1);
    CB_CHECK_EQ(f.counters.judged.load(), 2);

    const Outcome again = f.rec->run("m.fk");   // nothing changed
    CB_CHECK_EQ(again.judged, 0u);
    CB_CHECK_EQ(f.counters.built.load(), 1);
    CB_CHECK_EQ(f.counters.judged.load(), 2);
});

CB_TEST("verify.reconstruct.the_judge_failing_alone_is_not_run", [] {
    Fixture f({{"ok", "boom"}, {{"ok", "int ok(void) { return 1; }"}, {"boom", "int boom(void) { return 2; }"}}});
    (void)f.rec->run("m.fk");
    const auto best = f.best();
    CB_CHECK_EQ(best.at("key:ok").cls, Class::Verified);
    CB_CHECK_EQ(best.at("key:boom").cls, Class::NotJudged);
});

CB_TEST("verify.reconstruct.a_stand_in_callee_weakens_a_match", [] {
    Fixture f({{"caller", "gone"}, {{"caller", "int caller(void) { return gone(); }"}, {"gone", "int gone(void) { return 3; }"}}}, {"gone"});
    (void)f.rec->run("m.fk");
    const auto best = f.best();
    CB_CHECK_EQ(best.at("key:caller").cls, Class::Weak);
    CB_CHECK(best.at("key:caller").weak.contains(WeakLimit::StubCallee));
});

CB_TEST("verify.reconstruct.judging_on_threads_and_through_a_batch", [] {
    Module m{{"a", "b", "c"}, {{"a", "int a(void) { return 1; }"}, {"b", "int b(void) { return 2; }"}, {"c", "int c(void) { return 3; }"}}};
    {
        Fixture f(m);
        Services s = f.rec->services();
        s.judge_threads = 3;
        Reconstructor threaded(std::move(s));
        (void)threaded.run("m.fk");
        CB_CHECK_EQ(f.counters.judged.load(), 3);
        CB_CHECK_EQ(f.best().at("key:c").cls, Class::Verified);
    }
    {
        Fixture f(m);
        Services s = f.rec->services();
        std::vector<std::string> asked;
        s.judge_batch = [&](const Services::JudgeBatch& batch) {
            asked = batch.names;
            std::optional<std::map<std::string, Judgement>> out = std::map<std::string, Judgement>{};
            Judgement j;
            j.verdict = Verdict::Divergent;
            j.reason = "from the batch";
            (*out)["a"] = j;   // "b" and "c" are given no answer
            return out;
        };
        Reconstructor batched(std::move(s));
        (void)batched.run("m.fk");
        CB_CHECK_EQ(asked.size(), 3u);
        CB_CHECK_EQ(f.counters.judged.load(), 0);
        const auto best = f.best();
        CB_CHECK_EQ(best.at("key:a").cls, Class::Unverified);
        CB_CHECK_EQ(best.at("key:b").cls, Class::NotJudged);
    }
    {
        Fixture f(m);   // a batch that declines: the threads judge
        Services s = f.rec->services();
        s.judge_batch = [](const Services::JudgeBatch&) { return std::nullopt; };
        Reconstructor declined(std::move(s));
        (void)declined.run("m.fk");
        CB_CHECK_EQ(f.counters.judged.load(), 3);
    }
});

CB_TEST("verify.reconstruct.a_run_tells_its_steps_and_counts_each_function_judged", [] {
    Module m{{"a", "b", "c", "gone"},
             {{"a", "int a(void) { return 1; }"}, {"b", "int b(void) { return 2; }"}, {"c", "int c(void) { return 3; }"}, {"gone", "int gone(void) { return 4; }"}}};
    for (const int threads : {1, 3}) {
        Fixture f(m, {"gone"});
        Services s = f.rec->services();
        s.judge_threads = threads;
        std::vector<std::tuple<Step, std::size_t, std::size_t>> told;
        s.progress = [&](Step step, std::size_t done, std::size_t total) { told.emplace_back(step, done, total); };
        (void)Reconstructor(std::move(s)).run("m.fk");
        CB_CHECK(!told.empty() && told.front() == std::make_tuple(Step::Preparing, std::size_t{0}, std::size_t{0}));
        CB_CHECK(std::find(told.begin(), told.end(), std::make_tuple(Step::Building, std::size_t{0}, std::size_t{4})) != told.end());   // the four with no verdict
        std::set<std::size_t> counted;
        for (const auto& [step, done, total] : told)
            if (step == Step::Judging && done > 0) {
                CB_CHECK_EQ(total, 3u);   // ("gone" did not build: it is not judged)
                counted.insert(done);
            }
        CB_CHECK(counted == (std::set<std::size_t>{1, 2, 3}));
        CB_CHECK(told.back() == std::make_tuple(Step::Judging, std::size_t{3}, std::size_t{3}));
    }
});

CB_TEST("verify.reconstruct.a_run_told_to_stop_leaves_by_stopped_and_records_nothing", [] {
    Module m{{"a", "b", "c"}, {{"a", "int a(void) { return 1; }"}, {"b", "int b(void) { return 2; }"}, {"c", "int c(void) { return 3; }"}}};
    Fixture f(m);
    Services s = f.rec->services();
    int asked = 0;
    s.stop = [&] { return ++asked > 2; };   // (asked before the build, before judging, then before each function)
    bool stopped = false;
    try {
        (void)Reconstructor(std::move(s)).run("m.fk");
    } catch (const Stopped&) {
        stopped = true;
    }
    CB_CHECK(stopped);
    CB_CHECK_EQ(f.counters.judged.load(), 0);
    const auto best = f.best();
    CB_CHECK(!best.contains("key:a") || best.at("key:a").cls == Class::NoSource);   // no verdict was recorded
    const Outcome again = f.rec->run("m.fk");   // a run not stopped judges them all
    CB_CHECK_EQ(again.judged, 3u);
    CB_CHECK_EQ(f.counters.judged.load(), 3);
});

CB_TEST("verify.reconstruct.a_per_function_kind_keeps_the_verdict_when_the_code_is_the_same", [] {
    fs::ScopedTempDir tmp("cb-reconstruct");
    store::FunctionStore store(tmp.path() / "functions.sqlite", tmp.path() / "db");
    Module module{{"a", "b"}, {{"a", "int a(void) { return 1; }"}, {"b", "int b(void) { return 2; }"}}};
    Counters counters;
    Kinds kinds;
    std::map<std::string, std::string> objects = {{"a", "code-a"}, {"b", "code-b"}};
    kinds.add(std::make_shared<FakeLane>(module, counters),
              KindTraits{.per_function = true, .object_digests = [&](const std::filesystem::path&) { return objects; }});
    Services s{.kinds = kinds, .store = store};
    s.kind_of_file = [](const std::filesystem::path&) { return std::optional<std::string>("fake"); };
    s.content_sha = [](const std::filesystem::path&) { return std::optional<std::string>("sha-of-the-module"); };
    s.scratch = tmp.path() / "work";
    Reconstructor rec(std::move(s));

    CB_CHECK_EQ(rec.run("m.fk").judged, 2u);
    CB_CHECK_EQ(counters.judged.load(), 2);

    // b's text changes (a comment) and its code does not: a is untouched, b is built but not judged again
    module.texts["b"] = "int b(void) { /* note */ return 2; }";
    const Outcome second = rec.run("m.fk");
    CB_CHECK_EQ(second.kept, 1u);
    CB_CHECK_EQ(second.judged, 0u);
    CB_CHECK_EQ(counters.judged.load(), 2);
    CB_CHECK_EQ(store.best("sha-of-the-module", "d1").at("key:b").candidate.source.value_or(""), module.texts["b"]);

    // now b's code changes: it is judged again
    module.texts["b"] = "int b(void) { return 3; }";
    objects["b"] = "code-b2";
    CB_CHECK_EQ(rec.run("m.fk").judged, 1u);
    CB_CHECK_EQ(counters.judged.load(), 3);
});

CB_TEST("verify.reconstruct.identity_and_digests", [] {
    Fixture f({{"a"}, {{"a", "int a(void) { return 1; }"}}});
    CB_CHECK_EQ(f.rec->kind_name("x.fk").value_or(""), "fake");
    CB_CHECK(!f.rec->kind_name("x.txt"));
    const Identity id = f.rec->identity("x.fk");
    CB_CHECK_EQ(id.sha, "sha-of-the-module");
    CB_CHECK_EQ(id.profile, "d1");
    CB_CHECK(f.rec->digests() == (std::map<std::string, std::optional<std::string>>{{"fake", "d1"}}));
    CB_CHECK_THROWS_AS(UsageError, (void)f.rec->identity("x.txt"));
    CB_CHECK_THROWS_AS(UsageError, (void)f.rec->run("x.txt"));
    CB_CHECK(!f.rec->try_identity("x.txt"));

    // the hook the version ledger takes
    struct LedgerIdentity {
        std::string sha, profile;
    };
    const auto hook = identity_hook<LedgerIdentity>(*f.rec);
    const auto some = hook("x.fk");
    CB_CHECK(some.has_value());
    CB_CHECK_EQ(some->profile, "d1");
    CB_CHECK(!hook("x.txt"));
});

CB_TEST("verify.reconstruct.a_lane_whose_tools_are_missing_has_no_digest", [] {
    struct NoTools final : lanes::Kind {
        const std::string& name() const override {
            static const std::string kName = "notools";
            return kName;
        }
        lanes::Profile profile(const std::filesystem::path&) const override { throw ToolError("wasm", 1, "OpenWatcom is not set up"); }
        std::unique_ptr<lanes::Prepared> prepare(const std::filesystem::path&, const std::filesystem::path&) const override { return nullptr; }
        lanes::Built build(const lanes::Prepared&, const std::filesystem::path&, const std::filesystem::path&) const override { return {}; }
    };
    fs::ScopedTempDir tmp("cb-reconstruct");
    store::FunctionStore store(tmp.path() / "functions.sqlite", tmp.path() / "db");
    Kinds kinds;
    kinds.add(std::make_shared<NoTools>());
    Services s{.kinds = kinds, .store = store};
    s.kind_of_file = [](const std::filesystem::path&) { return std::optional<std::string>("notools"); };
    Reconstructor rec(std::move(s));
    CB_CHECK(!rec.digests().at("notools").has_value());
    CB_CHECK(!rec.try_identity("x").has_value());
});
