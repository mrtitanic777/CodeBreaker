// The engine held to its recorded answers (testdata/engine_facts.golden.json): a small pipeline of steps, the same
// asks and changes in the same order, and after each the value, how many facts of each kind were found current and how many made, which were made, and every
// fact row as kept (kind, key, hash, tag, size, step, version, inputs, given). The golden is JSON, a bytes value as {"$b": latin-1 text}.
//
// The pipeline: n.words (the binary's words) -> n.word <i> -> n.upper <i> -> n.joined; n.len <i> (a number whose equal results cut off what reads them); n.batch
// (one call for all the stale keys, one value unkept); n.side (a file beside the binary); n.bad (reads what it did not declare).
//
// A refused read or a kind no step makes is an error of its own type (the message names the step or the kind; its wording is not compared); the order of the inputs of a fact that read many at once is the order of the keys.
#include <algorithm>
#include <mutex>
#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/engine/engine.hpp"

using namespace cb;
using namespace cb::engine;
using Jx = nlohmann::json;

namespace {

// What the golden's values look like: a document as a JSON value, text as a string, bytes as {"$b": ...}.
Jx eg_flat(const knowledge::Value& v) {
    switch (v.kind()) {
        case knowledge::Value::Kind::Null: return nullptr;
        case knowledge::Value::Kind::Bool: return *v.boolean();
        case knowledge::Value::Kind::Int: return *v.integer();
        case knowledge::Value::Kind::Double: return *v.number();
        case knowledge::Value::Kind::Text: return *v.text();
        case knowledge::Value::Kind::List: {
            Jx a = Jx::array();
            for (const knowledge::Value& x : *v.list()) a.push_back(eg_flat(x));
            return a;
        }
        case knowledge::Value::Kind::Map: {
            Jx o = Jx::object();
            for (const auto& [k, x] : *v.map()) o[k] = eg_flat(x);
            return o;
        }
    }
    return nullptr;
}

Jx eg_flat(const Value& v) {
    if (const auto* bytes = std::get_if<Bytes>(&v)) return Jx{{"$b", std::string(bytes->begin(), bytes->end())}};
    if (const auto* text = std::get_if<std::string>(&v)) return *text;
    return eg_flat(std::get<knowledge::Value>(v));
}

std::string eg_text_of(const Value& v) { return std::get<std::string>(v); }

struct EgWorld {
    fs::ScopedTempDir dir{"engine-golden"};
    std::filesystem::path binary = dir.path() / "thing.bin";
    facts::Databases databases{facts::Config{dir.path() / "db", 1 * facts::kGiB}};
    Steps steps;
    knowledge::ReadDigests digests{[](const knowledge::Read&) { return std::string(knowledge::kAbsent); }};
    Engine engine{steps, databases, digests, 4};
    std::mutex made_lock;
    std::vector<std::pair<std::string, std::string>> made;

    void made_now(const std::string& kind, const std::string& key) {
        std::lock_guard hold(made_lock);
        made.emplace_back(kind, key);
    }

    static std::string path_of(Context& ctx) { return *std::get<knowledge::Value>(ctx.need("binary")).get("path")->text(); }

    EgWorld() {
        fs::write_text(binary, "alpha beta skip gamma");
        fs::write_text(dir.path() / "side.txt", "side");
        const auto declare = [&](const char* kind, std::vector<std::string> reads, StepFn fn) {
            Step s;
            s.kind = kind;
            s.reads = std::move(reads);
            s.fixed_version = "1";
            s.fn = std::move(fn);
            steps.declare(std::move(s));
        };
        declare("n.words", {"binary"}, [this](Context& ctx) -> StepResult {
            made_now("n.words", ctx.key());
            knowledge::Value::List words;
            const std::string content = fs::read_text(path_of(ctx));
            for (const std::string_view w : text::split_ws(content)) words.emplace_back(std::string(w));
            return Value(knowledge::Value(std::move(words)));
        });
        declare("n.word", {"n.words"}, [this](Context& ctx) -> StepResult {
            made_now("n.word", ctx.key());
            const knowledge::Value words = std::get<knowledge::Value>(ctx.need("n.words"));
            const std::size_t i = static_cast<std::size_t>(std::stoi(ctx.key()));
            if (i >= words.list()->size()) return Value(knowledge::Value());
            return Value(*(*words.list())[i].text());
        });
        declare("n.upper", {"n.word"}, [this](Context& ctx) -> StepResult {
            made_now("n.upper", ctx.key());
            const Value w = ctx.need("n.word", ctx.key());
            const std::string* text = std::holds_alternative<std::string>(w) ? &std::get<std::string>(w) : nullptr;
            return Value(text ? text::upper(*text) : std::string());
        });
        declare("n.len", {"n.word"}, [this](Context& ctx) -> StepResult {
            made_now("n.len", ctx.key());
            const Value w = ctx.need("n.word", ctx.key());
            knowledge::Value::Map m;
            m.insert_or_assign("n", knowledge::Value(static_cast<std::int64_t>(std::holds_alternative<std::string>(w) ? std::get<std::string>(w).size() : 0)));
            return Value(knowledge::Value(std::move(m)));
        });
        declare("n.joined", {"n.upper", "n.words", "n.len"}, [this](Context& ctx) -> StepResult {
            made_now("n.joined", ctx.key());
            const std::size_t n = std::get<knowledge::Value>(ctx.need("n.words")).list()->size();
            std::vector<std::string> keys;
            for (std::size_t i = 0; i < n; ++i) keys.push_back(std::to_string(i));
            const auto upper = ctx.need_many("n.upper", keys);
            const auto lens = ctx.need_many("n.len", keys);
            std::vector<std::string> up;
            for (const std::string& k : keys) up.push_back(eg_text_of(upper.at(k)));
            std::int64_t total = 0;
            for (const std::string& k : keys) total += *std::get<knowledge::Value>(lens.at(k)).get("n")->integer();
            const auto peeked = ctx.peek("n.upper", "0");
            knowledge::Value::List out;
            out.emplace_back(text::join(up, " "));
            out.emplace_back(total);
            out.emplace_back(peeked ? knowledge::Value(eg_text_of(*peeked)) : knowledge::Value());
            return Value(knowledge::Value(std::move(out)));
        });
        {
            Step s;
            s.kind = "n.batch";
            s.reads = {"n.word"};
            s.fixed_version = "1";
            s.batch_fn = [this](std::span<Context* const> contexts) {
                std::vector<std::string> keys;
                for (Context* c : contexts) keys.push_back(c->key());
                std::sort(keys.begin(), keys.end());
                std::string joined;
                for (const std::string& k : keys) joined += (joined.empty() ? "" : ",") + k;
                made_now("n.batch", joined);
                std::map<std::string, StepResult> out;
                for (Context* c : contexts) {
                    // (the word is read twice: once to compare, once to keep)
                    if (eg_text_of(c->need("n.word", c->key())) == "skip") out.emplace(c->key(), Unkept{Value(std::string("?"))});
                    else {
                        const std::string word = eg_text_of(c->need("n.word", c->key()));
                        out.emplace(c->key(), Value(Bytes(word.begin(), word.end())));
                    }
                }
                return out;
            };
            steps.declare(std::move(s));
        }
        declare("n.side", {"n.words"}, [this](Context& ctx) -> StepResult {
            made_now("n.side", ctx.key());
            const std::filesystem::path side = std::filesystem::path(path_of(ctx)).parent_path() / "side.txt";
            std::string out = fs::read_text(side);
            out += std::to_string(std::get<knowledge::Value>(ctx.need("n.words")).list()->size());
            out += eg_text_of(ctx.file(std::filesystem::path(path_of(ctx)).parent_path() / "side.txt"));   // (the binary is read again)
            return Value(out);
        });
        declare("n.bad", {"n.word"}, [](Context& ctx) -> StepResult { return ctx.need("n.words"); });   // (not declared)
    }

    // Every row as kept: [kind, key, hash, tag, size, step, version, inputs, given]; the binary's size is where it was run, so it is left out.
    Jx record(facts::Database& db) {
        Jx rows = Jx::array();
        for (const facts::Row& r : db.rows()) {
            Jx inputs = Jx::array();
            for (const facts::Input& i : r.inputs) inputs.push_back(Jx::array({i.kind, i.key, i.hash}));
            rows.push_back(Jx::array({r.kind, r.key, r.hash, std::string(1, r.tag), r.kind == "binary" ? Jx(nullptr) : Jx(r.size), r.step, r.version, inputs, r.given()}));
        }
        return rows;
    }

    Jx counts(const std::vector<Run::Count>& c) {
        Jx o = Jx::object();
        for (const Run::Count& k : c) o[k.kind] = Jx::array({k.kept, k.made});
        return o;
    }
};

}  // namespace

CB_TEST("engine.engine_golden.the_same_facts_and_runs_as_recorded", [] {
    EgWorld w;
    const Jx golden = cb::json::load(testing::data_path("engine/testdata/engine_facts.golden.json"));
    const auto db = w.engine.open(w.binary, w.dir.path() / "db");
    std::size_t at = 0;

    // One step of the scenario: the value it gave, what was found and made, which were made, and every row kept after it.
    const auto step = [&](const char* what, const std::function<Jx()>& act) {
        const Jx& want = golden.at(at++);
        CB_CHECK_EQ(want[0].get<std::string>(), std::string(what));
        w.made.clear();
        const Jx value = act();
        CB_CHECK(value == want[1]);
        CB_CHECK(w.counts(w.engine.last_run(*db)) == want[2]);
        std::vector<std::pair<std::string, std::string>> made = w.made, want_made;
        for (const Jx& m : want[3]) {
            // (a batch's keys are a tuple here, the keys of one call joined by commas)
            if (m[1].is_array()) {
                std::string joined;
                for (const Jx& k : m[1]) joined += (joined.empty() ? "" : ",") + k.get<std::string>();
                want_made.emplace_back(m[0].get<std::string>(), joined);
            } else {
                want_made.emplace_back(m[0].get<std::string>(), m[1].get<std::string>());
            }
        }
        std::sort(made.begin(), made.end());
        std::sort(want_made.begin(), want_made.end());
        CB_CHECK(made == want_made);
        if (want.size() == 5) {
            // What depends on where it ran, or on the engine, is left out on both sides: the binary's size (it holds the binary's path), the place a file
            // fact's key names (the golden has "<d>"), and the version of the engine's own binary and file steps (a digest of the code).
            Jx expected = want[4], got = w.record(*db);
            const std::string folder = std::filesystem::path(w.dir.path()).make_preferred().string();
            for (Jx& row : expected) {
                if (row[0] == "binary") row[4] = nullptr;
                if (row[0] == "file") row[1] = folder + row[1].get<std::string>().substr(3);
                if (row[0] == "binary" || row[0] == "file") row[6] = "";
                for (Jx& input : row[7])
                    if (input[0] == "file") input[1] = folder + input[1].get<std::string>().substr(3);
            }
            for (Jx& row : got)
                if (row[0] == "binary" || row[0] == "file") row[6] = "";
            if (got == expected) {
                CB_CHECK(true);
            } else {
                std::string where = std::string(what) + ": rows " + std::to_string(got.size()) + " against " + std::to_string(want[4].size());
                for (std::size_t i = 0; i < std::min(got.size(), expected.size()); ++i)
                    if (got[i] != expected[i]) {
                        where += "\n  got  " + got[i].dump().substr(0, 600) + "\n  want " + expected[i].dump().substr(0, 600);
                        break;
                    }
                testing::count_check();
                testing::record_failure(__FILE__, __LINE__, "the rows kept differ: " + where);
            }
        }
    };
    const auto many_of = [&](const std::map<std::string, Value>& m) {
        Jx o = Jx::object();
        for (const auto& [k, v] : m) o[k] = eg_flat(v);
        return o;
    };

    step("cold", [&] { return eg_flat(w.engine.get(db, "n.joined")); });
    step("warm", [&] { return eg_flat(w.engine.get(db, "n.joined")); });
    step("many", [&] { return many_of(w.engine.get_many(db, "n.upper", std::vector<std::string>{"0", "1", "3", "1"})); });
    step("batch", [&] {
        Run run(w.engine, db);
        return many_of(run.many("n.batch", std::vector<std::string>{"0", "1", "2"}));
    });
    step("batch again", [&] {
        Run run(w.engine, db);
        return many_of(run.many("n.batch", std::vector<std::string>{"0", "1", "2"}));
    });
    step("side", [&] { return eg_flat(w.engine.get(db, "n.side")); });
    fs::write_text(w.dir.path() / "side.txt", "SIDE");
    step("side changed", [&] { return eg_flat(w.engine.get(db, "n.side")); });
    step("given", [&] { return Jx(w.engine.put(*db, "n.word", "1", Value(std::string("BETA")))); });
    step("given used", [&] { return eg_flat(w.engine.get(db, "n.joined")); });
    step("given other", [&] { return Jx(w.engine.put(*db, "n.word", "1", Value(std::string("delta")))); });
    step("given other used", [&] { return eg_flat(w.engine.get(db, "n.joined")); });
    step("forgot", [&] { return Jx(w.engine.forget(*db, "n.word", "1")); });
    step("forgot used", [&] { return eg_flat(w.engine.get(db, "n.joined")); });

    // a step that reads what it did not declare is refused, naming itself and the kind; a kind no step makes is an error naming the kind
    CB_CHECK_EQ(golden.at(at++)[0].get<std::string>(), "refused");
    try {
        (void)w.engine.get(db, "n.bad", "0");
        CB_CHECK(false);
    } catch (const Error& e) {
        const std::string message = e.what();
        CB_CHECK(message.find("n.bad") != std::string::npos && message.find("n.words") != std::string::npos);
    }
    CB_CHECK_EQ(golden.at(at++)[0].get<std::string>(), "no step");
    CB_CHECK_THROWS_AS(UnknownStepError, (void)w.engine.get(db, "n.nothing"));

    // how the joined fact was made: its inputs, each still what it was
    const Jx& why_want = golden.at(at++);
    CB_CHECK_EQ(why_want[0].get<std::string>(), "why");
    const auto why = w.engine.why(*db, "n.joined");
    CB_CHECK(why.has_value());
    if (why) {
        Jx got = Jx::array();
        for (const Why::InputNow& i : why->inputs) got.push_back(Jx::array({i.input.kind, i.input.key, i.input.hash, i.same}));
        CB_CHECK(got == why_want[1]);
    }

    // what each kind holds
    const Jx& kinds_want = golden.at(at++);
    CB_CHECK_EQ(kinds_want[0].get<std::string>(), "kinds");
    Jx kinds = Jx::array();
    for (const facts::KindSummary& k : db->kinds()) kinds.push_back(Jx::array({k.kind, k.count, k.kind == "binary" ? Jx(nullptr) : Jx(k.bytes)}));
    Jx kinds_golden = Jx::array();
    for (const Jx& k : kinds_want[1]) kinds_golden.push_back(Jx::array({k[0], k[1], k[0] == "binary" ? Jx(nullptr) : k[2]}));
    CB_CHECK(kinds == kinds_golden);

    // what the database says of itself
    const Jx& about_want = golden.at(at++);
    CB_CHECK_EQ(about_want[0].get<std::string>(), "about");
    CB_CHECK_EQ(db->about("name").value_or("?"), about_want[1].get<std::string>());
    CB_CHECK(!db->about("nothing").has_value() && about_want[2].is_null());
    CB_CHECK_EQ(at, golden.size());
});
