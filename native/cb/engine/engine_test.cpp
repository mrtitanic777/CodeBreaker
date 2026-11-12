#include "cb/engine/engine.hpp"

#include <atomic>
#include <chrono>
#include <cstdio>
#include <map>
#include <mutex>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::engine;

namespace {

// A world to ask the engine in: a binary, a folder of databases, the steps, and what the knowledge base says now.
struct World {
    fs::ScopedTempDir dir{"engine"};
    std::filesystem::path binary;
    facts::Databases databases;
    Steps steps;
    std::map<std::string, std::string> pieces;   // "platform:kind:key" -> its digest now (absent: not there)
    knowledge::ReadDigests digests;
    Engine engine;
    std::atomic<int> made_a{0}, made_b{0};

    World()
        : binary(dir.path() / "prog.bin"),
          databases(facts::Config{dir.path() / "db", 1 * facts::kGiB}),
          digests([this](const knowledge::Read& r) {
              const auto it = pieces.find(r.text());
              return it == pieces.end() ? std::string(knowledge::kAbsent) : it->second;
          }),
          engine(steps, databases, digests, 4) {
        fs::write_file(binary, Bytes{1, 2, 3, 4, 5});
    }

    // size: the binary's size; twice: twice that. Counting how often each is made.
    void declare_chain(const std::string& version = "v1") {
        Step a;
        a.kind = "t.size";
        a.reads = {"binary"};
        a.fixed_version = "a-" + version;
        a.fn = [this](Context& ctx) -> StepResult {
            ++made_a;
            const Value binary = ctx.need("binary");
            const auto* doc = std::get_if<knowledge::Value>(&binary);
            return Value(knowledge::Value(*doc->get("size")->integer()));
        };
        steps.declare(std::move(a));
        Step b;
        b.kind = "t.twice";
        b.reads = {"t.size"};
        b.fixed_version = "b-1";
        b.fn = [this](Context& ctx) -> StepResult {
            ++made_b;
            const Value size = ctx.need("t.size");
            return Value(knowledge::Value(*std::get<knowledge::Value>(size).integer() * 2));
        };
        steps.declare(std::move(b));
    }
};

std::int64_t number_of(const Value& v) { return *std::get<knowledge::Value>(v).integer(); }

std::size_t kept_of(const std::vector<Run::Count>& counts, const std::string& kind) {
    for (const auto& c : counts)
        if (c.kind == kind) return c.kept;
    return 0;
}
std::size_t made_of(const std::vector<Run::Count>& counts, const std::string& kind) {
    for (const auto& c : counts)
        if (c.kind == kind) return c.made;
    return 0;
}

}  // namespace

CB_TEST("engine.engine.the_binary_is_given_when_its_database_is_opened", [] {
    World w;
    const auto db = w.engine.open(w.binary);
    const Value v = w.engine.get(db, "binary");
    const auto& doc = std::get<knowledge::Value>(v);
    CB_CHECK_EQ(*doc.get("size")->integer(), 5);
    CB_CHECK(doc.get("path")->text()->ends_with("prog.bin"));
    const auto row = db->row("binary", "");
    CB_CHECK(row.has_value());
    CB_CHECK(row->given());
    CB_CHECK_EQ(row->hash, "sha256:" + db->sha);
    // opened again, the row is as it was (nothing rewritten: its time of making stays)
    const double made = row->made;
    const auto again = w.engine.open(w.binary);
    CB_CHECK_EQ(again->row("binary", "")->made, made);
});

CB_TEST("engine.engine.a_fact_is_made_once_and_kept", [] {
    World w;
    w.declare_chain();
    const auto db = w.engine.open(w.binary);
    CB_CHECK_EQ(number_of(w.engine.get(db, "t.twice")), 10);
    CB_CHECK_EQ(w.made_a.load(), 1);
    CB_CHECK_EQ(w.made_b.load(), 1);
    auto counts = w.engine.last_run(*db);
    CB_CHECK_EQ(made_of(counts, "t.twice"), 1u);
    CB_CHECK_EQ(made_of(counts, "t.size"), 1u);

    // asked again (a new run): found current, nothing made
    CB_CHECK_EQ(number_of(w.engine.get(db, "t.twice")), 10);
    CB_CHECK_EQ(w.made_a.load(), 1);
    CB_CHECK_EQ(w.made_b.load(), 1);
    counts = w.engine.last_run(*db);
    CB_CHECK_EQ(made_of(counts, "t.twice"), 0u);
    CB_CHECK_EQ(kept_of(counts, "t.twice"), 1u);
    // what the fact was made from is recorded: the fact it read, with the hash it had
    const auto why = w.engine.why(*db, "t.twice");
    CB_CHECK(why.has_value());
    CB_CHECK_EQ(why->inputs.size(), 1u);
    CB_CHECK_EQ(why->inputs[0].input.kind, "t.size");
    CB_CHECK(why->inputs[0].same);
    CB_CHECK_EQ(why->version, "b-1");
    CB_CHECK_EQ(why->version_now.value(), "b-1");
});

CB_TEST("engine.engine.a_given_fact_changed_makes_stale_what_was_made_from_it", [] {
    World w;
    Step cfg_user;
    cfg_user.kind = "t.use";
    cfg_user.reads = {"t.cfg"};
    cfg_user.fixed_version = "1";
    cfg_user.fn = [&w](Context& ctx) -> StepResult {
        ++w.made_a;
        return Value("used:" + std::get<std::string>(ctx.need("t.cfg")));
    };
    w.steps.declare(std::move(cfg_user));
    Step cfg;
    cfg.kind = "t.cfg";
    cfg.fixed_version = "1";
    cfg.fn = [](Context&) -> StepResult { return Value(std::string("default")); };
    w.steps.declare(std::move(cfg));

    const auto db = w.engine.open(w.binary);
    w.engine.put(*db, "t.cfg", "", Value(std::string("edited")));
    CB_CHECK_EQ(std::get<std::string>(w.engine.get(db, "t.use")), "used:edited");
    CB_CHECK_EQ(w.made_a.load(), 1);
    // the same value given again: what was made from it stays
    w.engine.put(*db, "t.cfg", "", Value(std::string("edited")));
    CB_CHECK_EQ(std::get<std::string>(w.engine.get(db, "t.use")), "used:edited");
    CB_CHECK_EQ(w.made_a.load(), 1);
    // a different one: made again
    w.engine.put(*db, "t.cfg", "", Value(std::string("other")));
    CB_CHECK_EQ(std::get<std::string>(w.engine.get(db, "t.use")), "used:other");
    CB_CHECK_EQ(w.made_a.load(), 2);
    // forgotten: its step makes it again, and what was made from the edit is stale
    CB_CHECK_EQ(w.engine.forget(*db, "t.cfg", std::string_view("")), 1u);
    CB_CHECK_EQ(std::get<std::string>(w.engine.get(db, "t.use")), "used:default");
    CB_CHECK_EQ(w.made_a.load(), 3);
});

CB_TEST("engine.engine.a_new_step_version_remakes_but_an_equal_result_leaves_dependents_alone", [] {
    World w;
    w.declare_chain("v1");
    const auto db = w.engine.open(w.binary);
    (void)w.engine.get(db, "t.twice");
    CB_CHECK_EQ(w.made_a.load(), 1);
    CB_CHECK_EQ(w.made_b.load(), 1);

    w.declare_chain("v2");   // t.size's code changed; it still comes out 5
    CB_CHECK_EQ(number_of(w.engine.get(db, "t.twice")), 10);
    CB_CHECK_EQ(w.made_a.load(), 2);   // made again ...
    CB_CHECK_EQ(w.made_b.load(), 1);   // ... and nothing made from it is stale: the same hash
    CB_CHECK_EQ(w.engine.why(*db, "t.size")->version, "a-v2");
});

CB_TEST("engine.engine.many_makes_the_stale_side_by_side_and_a_batch_step_in_one_call", [] {
    World w;
    std::atomic<int> calls{0};
    Step one;
    one.kind = "t.one";
    one.fixed_version = "1";
    one.fn = [&](Context& ctx) -> StepResult {
        ++calls;
        return Value("one-" + ctx.key());
    };
    w.steps.declare(std::move(one));
    std::atomic<int> batch_calls{0};
    std::atomic<std::size_t> batch_size{0};
    Step batch;
    batch.kind = "t.batch";
    batch.fixed_version = "1";
    batch.batch_fn = [&](std::span<Context* const> contexts) {
        ++batch_calls;
        batch_size = contexts.size();
        std::map<std::string, StepResult> out;
        for (Context* c : contexts) out[c->key()] = Value("batch-" + c->key() + "-" + std::any_cast<std::string>(c->hint()));
        return out;
    };
    w.steps.declare(std::move(batch));

    const auto db = w.engine.open(w.binary);
    const std::vector<std::string> keys = {"a", "b", "c", "a", "d"};   // a asked twice
    const auto got = w.engine.get_many(db, "t.one", keys);
    CB_CHECK_EQ(got.size(), 4u);
    CB_CHECK_EQ(std::get<std::string>(got.at("c")), "one-c");
    CB_CHECK_EQ(calls.load(), 4);

    // a second asking: only the new key is made
    const std::vector<std::string> more = {"a", "e"};
    (void)w.engine.get_many(db, "t.one", more);
    CB_CHECK_EQ(calls.load(), 5);

    // a batch step is given all its stale keys at once, and the asker's hint
    Run run(w.engine, db);
    const auto batched = run.many("t.batch", std::vector<std::string>{"x", "y", "z"}, std::string("hint"));
    CB_CHECK_EQ(batch_calls.load(), 1);
    CB_CHECK_EQ(batch_size.load(), 3u);
    CB_CHECK_EQ(std::get<std::string>(batched.at("y")), "batch-y-hint");
});

CB_TEST("engine.engine.a_batch_step_that_leaves_a_key_out_is_an_error", [] {
    World w;
    Step batch;
    batch.kind = "t.batch";
    batch.fixed_version = "1";
    batch.batch_fn = [](std::span<Context* const> contexts) {
        std::map<std::string, StepResult> out;
        out[contexts[0]->key()] = Value(std::string("only"));
        return out;
    };
    w.steps.declare(std::move(batch));
    const auto db = w.engine.open(w.binary);
    CB_CHECK_THROWS_AS(InternalError, (void)w.engine.get_many(db, "t.batch", std::vector<std::string>{"p", "q"}));
});

CB_TEST("engine.engine.an_unkept_value_is_used_this_run_and_made_again_the_next", [] {
    World w;
    std::atomic<int> calls{0};
    Step s;
    s.kind = "t.volatile";
    s.fixed_version = "1";
    s.fn = [&](Context&) -> StepResult {
        ++calls;
        return Unkept{Value(std::string("now"))};
    };
    w.steps.declare(std::move(s));
    const auto db = w.engine.open(w.binary);
    CB_CHECK_EQ(std::get<std::string>(w.engine.get(db, "t.volatile")), "now");
    CB_CHECK(!db->row("t.volatile", "").has_value());
    CB_CHECK_EQ(std::get<std::string>(w.engine.get(db, "t.volatile")), "now");
    CB_CHECK_EQ(calls.load(), 2);
    // in one run it is made once, however often it is asked
    Run run(w.engine, db);
    (void)run.value("t.volatile");
    (void)run.value("t.volatile");
    CB_CHECK_EQ(calls.load(), 3);
    CB_CHECK(!run.hash_of("t.volatile").has_value());
});

CB_TEST("engine.engine.a_step_may_only_read_what_it_declared", [] {
    World w;
    Step s;
    s.kind = "t.sneaky";
    s.reads = {"t.cfg"};
    s.fixed_version = "1";
    s.fn = [](Context& ctx) -> StepResult { return ctx.need("t.other"); };
    w.steps.declare(std::move(s));
    const auto db = w.engine.open(w.binary);
    CB_CHECK_THROWS_AS(UsageError, (void)w.engine.get(db, "t.sneaky"));
    CB_CHECK_THROWS_AS(UnknownStepError, (void)w.engine.get(db, "t.nobody_makes_this"));
});

CB_TEST("engine.engine.a_file_that_is_not_a_fact_is_an_input_by_its_hash", [] {
    World w;
    const auto list = w.dir.path() / "list.txt";
    fs::write_text(list, "alpha\n");
    Step s;
    s.kind = "t.listed";
    s.fixed_version = "1";
    s.fn = [&](Context& ctx) -> StepResult {
        ++w.made_a;
        return Value("saw:" + std::get<std::string>(ctx.file(list)).substr(0, 4));
    };
    s.reads = {};
    w.steps.declare(std::move(s));
    const auto db = w.engine.open(w.binary);
    (void)w.engine.get(db, "t.listed");
    (void)w.engine.get(db, "t.listed");
    CB_CHECK_EQ(w.made_a.load(), 1);
    fs::write_text(list, "beta\n");
    (void)w.engine.get(db, "t.listed");
    CB_CHECK_EQ(w.made_a.load(), 2);   // the file changed: its hash is not the one recorded
    // a file that is not there has a hash too: "missing"
    CB_CHECK_EQ(std::get<std::string>(w.engine.get(db, "file", fs::to_text(w.dir.path() / "nope.txt"))), "missing");
});

CB_TEST("engine.engine.what_a_step_read_of_the_knowledge_base_is_an_input", [] {
    World w;
    w.pieces["win32:proto:Foo"] = "h1";
    Step s;
    s.kind = "t.proto";
    s.fixed_version = "1";
    s.fn = [&](Context&) -> StepResult {
        ++w.made_a;
        knowledge::record("win32", "proto", "Foo");   // (the accessors of the base do this)
        return Value(std::string("uses Foo"));
    };
    w.steps.declare(std::move(s));
    const auto db = w.engine.open(w.binary);
    (void)w.engine.get(db, "t.proto");
    const auto why = w.engine.why(*db, "t.proto");
    CB_CHECK_EQ(why->inputs.size(), 1u);
    CB_CHECK_EQ(why->inputs[0].input.kind, "kb");
    CB_CHECK_EQ(why->inputs[0].input.key, "win32:proto:Foo");
    CB_CHECK_EQ(why->inputs[0].input.hash, "h1");

    (void)w.engine.get(db, "t.proto");
    CB_CHECK_EQ(w.made_a.load(), 1);                // the piece is as it was
    w.pieces["win32:proto:Foo"] = "h2";
    // (a new Engine's run reads the digest again: the digests object is the one memo; a fresh one asks the base)
    knowledge::ReadDigests fresh([&](const knowledge::Read& r) { return w.pieces.at(r.text()); });
    Engine again(w.steps, w.databases, fresh, 2);
    (void)again.get(db, "t.proto");
    CB_CHECK_EQ(w.made_a.load(), 2);                // the piece changed: made again, and now recorded as h2
    CB_CHECK_EQ(again.why(*db, "t.proto")->inputs[0].input.hash, "h2");
});

CB_TEST("engine.engine.peek_answers_only_from_what_is_current", [] {
    World w;
    w.declare_chain();
    Step peeker;
    peeker.kind = "t.peek";
    peeker.reads = {"t.size"};
    peeker.fixed_version = "1";
    peeker.fn = [](Context& ctx) -> StepResult {
        const auto got = ctx.peek("t.size");
        return Value(std::string(got ? "had" : "had not"));
    };
    w.steps.declare(std::move(peeker));
    const auto db = w.engine.open(w.binary);
    CB_CHECK_EQ(std::get<std::string>(w.engine.get(db, "t.peek")), "had not");
    CB_CHECK_EQ(w.made_a.load(), 0);   // peeking made nothing
    (void)w.engine.get(db, "t.size");
    w.engine.forget(*db, "t.peek");
    CB_CHECK_EQ(std::get<std::string>(w.engine.get(db, "t.peek")), "had");
});

CB_TEST("engine.engine.the_previous_value_and_the_work_folder", [] {
    World w;
    std::optional<Value> seen;
    std::filesystem::path folder;
    Step s;
    s.kind = "t.prev";
    s.fixed_version = "1";
    s.fn = [&](Context& ctx) -> StepResult {
        seen = ctx.previous();
        folder = ctx.work("objs");
        return Value(std::string("v"));
    };
    w.steps.declare(std::move(s));
    const auto db = w.engine.open(w.binary);
    (void)w.engine.get(db, "t.prev");
    CB_CHECK(!seen.has_value());
    CB_CHECK_EQ(folder.filename().string(), "objs");
    w.steps.declare([&] {
        Step again;
        again.kind = "t.prev";
        again.fixed_version = "2";
        again.fn = [&](Context& ctx) -> StepResult {
            seen = ctx.previous();
            return Value(std::string("v2"));
        };
        return again;
    }());
    (void)w.engine.get(db, "t.prev");
    CB_CHECK(seen.has_value());
    CB_CHECK_EQ(std::get<std::string>(*seen), "v");
});

CB_TEST("engine.engine.platform_steps_are_loaded_when_a_kind_is_not_known", [] {
    World w;
    int loads = 0;
    w.steps.set_loader(
        [&](Steps& steps, std::string_view platform) {
            ++loads;
            Step s;
            s.kind = std::string(platform) + ".thing";
            s.platform = std::string(platform);
            s.fixed_version = "1";
            s.fn = [](Context&) -> StepResult { return Value(std::string("made")); };
            steps.declare(std::move(s));
        },
        {"win16", "pe"});
    const auto db = w.engine.open(w.binary);
    CB_CHECK_EQ(std::get<std::string>(w.engine.get(db, "pe.thing")), "made");
    CB_CHECK_EQ(loads, 2);   // every platform's steps are looked through once
    CB_CHECK(w.steps.find("win16.thing") != nullptr);
    // a platform's list holds its steps and the platform-free ones
    const auto listed = w.steps.list("pe");
    bool binary = false, thing = false, other = false;
    for (const auto& s : listed) {
        binary |= s->kind == "binary";
        thing |= s->kind == "pe.thing";
        other |= s->kind == "win16.thing";
    }
    CB_CHECK(binary && thing && !other);
    CB_CHECK_THROWS_AS(UnknownStepError, (void)w.steps.of("never.declared"));
});

CB_TEST("engine.engine.an_exception_in_a_step_reaches_the_asker", [] {
    World w;
    Step s;
    s.kind = "t.fail";
    s.fixed_version = "1";
    s.fn = [](Context& ctx) -> StepResult {
        if (ctx.key() == "bad") throw FormatError("t", 0, "damaged");
        return Value(std::string("fine"));
    };
    w.steps.declare(std::move(s));
    const auto db = w.engine.open(w.binary);
    CB_CHECK_THROWS_AS(FormatError, (void)w.engine.get_many(db, "t.fail", std::vector<std::string>{"ok1", "bad", "ok2"}));
    // the facts that could be made were
    CB_CHECK(db->row("t.fail", "ok1").has_value());
});

CB_TEST("engine.engine.the_version_is_the_code_and_the_stamp", [] {
    Step a;
    a.kind = "k";
    a.code = {"step k", "helper 1"};
    const std::string v1 = a.version();
    CB_CHECK_EQ(v1.size(), 16u);
    Step b = a;
    CB_CHECK_EQ(b.version(), v1);
    b.code = {"step k", "helper 2"};
    CB_CHECK(b.version() != v1);
    Step c = a;
    c.stamp = [] { return std::string("tool 2.0"); };
    CB_CHECK(c.version() != v1);
    c.fixed_version = "fixed";
    CB_CHECK_EQ(c.version(), "fixed");
});

// Speed: two thousand facts of one kind made side by side, then found current by the next run.
CB_TEST("engine.engine.two_thousand_facts_made_and_found_current", [] {
    World w;
    Step s;
    s.kind = "t.square";
    s.fixed_version = "1";
    s.fn = [](Context& ctx) -> StepResult { return Value(knowledge::Value(static_cast<std::int64_t>(ctx.key().size() * 7))); };
    w.steps.declare(std::move(s));
    std::vector<std::string> keys;
    for (int i = 0; i < 2000; ++i) keys.push_back("key-" + std::to_string(i));
    const auto db = w.engine.open(w.binary);
    const auto t0 = std::chrono::steady_clock::now();
    const auto made = w.engine.get_many(db, "t.square", keys);
    const auto t1 = std::chrono::steady_clock::now();
    const auto again = w.engine.get_many(db, "t.square", keys);
    const auto t2 = std::chrono::steady_clock::now();
    CB_CHECK_EQ(made.size(), 2000u);
    CB_CHECK_EQ(again.size(), 2000u);
    CB_CHECK_EQ(made_of(w.engine.last_run(*db), "t.square"), 0u);
    const auto ms = [](auto a, auto b) { return std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count(); };
    std::printf("engine: 2000 facts made %lld ms, found current %lld ms\n", static_cast<long long>(ms(t0, t1)), static_cast<long long>(ms(t1, t2)));
    CB_CHECK(ms(t0, t2) < 60000);
});
