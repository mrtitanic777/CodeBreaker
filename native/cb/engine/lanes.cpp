#include "cb/engine/lanes.hpp"

#include <algorithm>
#include <any>
#include <chrono>
#include <format>
#include <mutex>
#include <set>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/jobs/pool.hpp"

namespace cb::engine::lanes {

using knowledge::Value;
using verify::lanes::Built;
using verify::lanes::Prepared;

namespace {

constexpr std::string_view kLaneCode = "engine.lanes:1";   // the identity of the code these steps are (their versions go with it)

// ---- documents --------------------------------------------------------------------------------------------------------------

const Value& lane_doc(const facts::Value& fact, std::string_view what) {
    const auto* doc = std::get_if<Value>(&fact);
    if (doc == nullptr) throw FormatError("engine fact", 0, std::format("{} is not a document", what));
    return *doc;
}

const Value& lane_member(const Value& doc, std::string_view name) {
    const Value* v = doc.get(name);
    if (v == nullptr) throw FormatError("engine fact", 0, std::format("a document lacks '{}'", name));
    return *v;
}

std::string lane_text(const Value& v) {
    const std::string* t = v.text();
    if (t == nullptr) throw FormatError("engine fact", 0, "a document member is not text");
    return *t;
}

Value lane_texts(const std::vector<std::string>& items) {
    Value::List out;
    for (const std::string& s : items) out.emplace_back(s);
    return Value(std::move(out));
}

std::vector<std::string> lane_text_list(const Value& v) {
    const Value::List* l = v.list();
    if (l == nullptr) throw FormatError("engine fact", 0, "a document member is not a list");
    std::vector<std::string> out;
    for (const Value& x : *l) out.push_back(lane_text(x));
    return out;
}

Value lane_nullable(const std::optional<std::string>& s) { return s ? Value(*s) : Value(); }

std::string lane_hash(const facts::Value& v) { return facts::hash_of(facts::encode(v)); }

std::filesystem::path lane_binary(const facts::Value& binary) { return fs::from_text(lane_text(lane_member(lane_doc(binary, "the binary"), "path"))); }

// The base of a prepared module that a document describes (what the signatures are made of).
Prepared lane_prepared_of(const ModuleDoc& d) {
    Prepared p;
    p.header = d.header;
    p.order = d.order;
    p.key = d.key;
    p.all_keys = d.all_keys;
    p.texts = d.texts;
    p.sig = d.sig;
    return p;
}

ModuleDoc lane_doc_of(const Prepared& p) {
    ModuleDoc d;
    d.header = p.header;
    d.order = p.order;
    d.key = p.key;
    d.all_keys = p.all_keys;
    d.texts = p.texts;
    d.sig = p.sig;
    return d;
}

std::string lane_sha1(std::string_view text) {
    return crypto::to_hex(crypto::sha1(ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size())));
}

// The weak limits a good verdict gets when a function it calls was a stand-in: its own, and "stub-callee", sorted.
std::string lane_with_stub_callee(std::vector<std::string> limits) {
    limits.push_back(std::string(word(verify::WeakLimit::StubCallee)));
    std::sort(limits.begin(), limits.end());
    limits.erase(std::unique(limits.begin(), limits.end()), limits.end());
    return text::join(limits, ",");
}

// What a verdict step is handed besides its keys: the names of the functions, those that reach a stand-in, and the build.
struct LaneVerdictHint {
    std::map<std::string, std::string> names;                                    // key -> function name
    std::set<std::string> tainted;                                               // names whose build reaches a stand-in
    std::function<std::shared_ptr<const Built>()> ready;                         // the build the judge runs
};

}  // namespace

// ---- the document of a module ---------------------------------------------------------------------------------------------

Value to_value(const ModuleDoc& d) {
    Value::Map key, texts;
    for (const std::string& n : d.order) {   // (the order of the functions, then any others by name: a stable document)
        if (const auto k = d.key.find(n); k != d.key.end()) key.insert_or_assign(n, Value(k->second));
        if (const auto t = d.texts.find(n); t != d.texts.end()) texts.insert_or_assign(n, Value(t->second));
    }
    std::map<std::string, std::string> rest_keys(d.key.begin(), d.key.end()), rest_texts(d.texts.begin(), d.texts.end());
    for (const auto& [n, k] : rest_keys) key.try_emplace(n, Value(k));
    for (const auto& [n, t] : rest_texts) texts.try_emplace(n, Value(t));
    return Value(Value::Map{{"header", Value(d.header)},
                            {"order", lane_texts(d.order)},
                            {"key", Value(std::move(key))},
                            {"all_keys", lane_texts(d.all_keys)},
                            {"texts", Value(std::move(texts))},
                            {"sig", Value(d.sig)}});
}

ModuleDoc module_doc_of(const Value& v) {
    ModuleDoc d;
    d.header = lane_text(lane_member(v, "header"));
    d.order = lane_text_list(lane_member(v, "order"));
    d.all_keys = lane_text_list(lane_member(v, "all_keys"));
    d.sig = lane_text(lane_member(v, "sig"));
    const Value::Map* key = lane_member(v, "key").map();
    const Value::Map* texts = lane_member(v, "texts").map();
    if (key == nullptr || texts == nullptr) throw FormatError("engine fact", 0, "a module document's key or texts is not a map");
    for (const auto& [n, k] : *key) d.key.emplace(n, lane_text(k));
    for (const auto& [n, t] : *texts) d.texts.emplace(n, lane_text(t));
    return d;
}

ModuleDoc patched(const ModuleDoc& doc, const std::map<std::string, std::string>& given) {
    if (given.empty()) return doc;
    ModuleDoc out = doc;
    std::string joined = doc.sig + '\0';
    for (const auto& [name, text] : given) {   // (sorted by name: the same hand-made texts are the same sig)
        out.texts[name] = text;
        joined += name + '\0' + text + '\0';
    }
    out.sig = lane_sha1(joined);
    return out;
}

std::map<std::string, std::string> signatures_of(const Prepared& module, bool per_function) {
    if (per_function) return verify::reconstruct::signatures(module, module.sig);
    std::map<std::string, std::string> out;
    for (const std::string& n : module.order) out[n] = module.sig;
    return out;
}

// ---- the object ------------------------------------------------------------------------------------------------------------

namespace {

// The build as the judge needs it, kept by the work folder it was made in.
struct LaneLiveBuild {
    std::string hash;                           // of the build fact this was made for
    std::shared_ptr<const Built> built;
};

}  // namespace

namespace { class LaneSteps; }   // (below)

struct detail::LaneState {
    Steps& steps;
    Engine& engine;
    std::function<std::optional<std::string>(const std::filesystem::path&)> kind_of_file;

    std::mutex mutex;
    std::vector<std::string> kind_order;
    std::map<std::string, Setup> setups;                                         // by K
    std::map<std::string, std::weak_ptr<LaneSteps>> lane_steps;                  // by K: the steps keep themselves alive
    std::map<std::string, std::shared_ptr<const Prepared>> modules;              // live modules: lane|database|document hash[|given sig]
    std::map<std::string, LaneLiveBuild> builds;                                 // by work folder

    LaneState(Steps& s, Engine& e, std::function<std::optional<std::string>(const std::filesystem::path&)> k)
        : steps(s), engine(e), kind_of_file(std::move(k)) {}
};

namespace {

using detail::LaneState;

// The module as its fact now is, with each function given by hand in the place of its C: the document.
struct LaneNow {
    ModuleDoc doc;
    std::map<std::string, std::string> given;    // name -> C given by hand that differs from the module's
    std::string base_hash;                       // of lane.K.module's value
};

LaneNow lane_now(Context& ctx, const std::string& L) {
    LaneNow now;
    const facts::Value module = ctx.need(L + "module");
    now.base_hash = lane_hash(module);
    now.doc = module_doc_of(lane_doc(module, L + "module"));
    const facts::Value keys_fact = ctx.need(L + "keys");
    const std::vector<std::string> keys = lane_text_list(lane_member(lane_doc(keys_fact, L + "keys"), "keys"));
    for (const auto& [k, f] : ctx.need_many(L + "function", keys)) {
        const Value& doc = lane_doc(f, L + "function");
        const Value::List* pair = doc.list();
        if (pair == nullptr || pair->size() != 2 || (*pair)[1].text() == nullptr) continue;   // not a function, or one with no C given
        const std::string name = lane_text((*pair)[0]);
        const std::string text = lane_text((*pair)[1]);
        const auto have = now.doc.texts.find(name);
        if (have == now.doc.texts.end() || have->second != text) now.given[name] = text;
    }
    now.doc = patched(now.doc, now.given);
    return now;
}

}  // namespace

namespace {

// The lane's steps, over the object's state.
class LaneSteps : public std::enable_shared_from_this<LaneSteps> {
public:
    LaneSteps(std::shared_ptr<LaneState> state, Setup setup) : state_(std::move(state)), setup_(std::move(setup)),
                                                                   K_(setup_.kind->name()), L_("lane." + K_ + ".") {}

    void declare() {
        profile();
        module();
        function();
        keys();
        texts();
        build();
        built();
        verdict();
        rows();
    }

private:
    Step base(std::string kind, std::vector<std::string> reads, StepFn fn, std::string doc) const {
        Step s;
        s.kind = std::move(kind);
        s.reads = std::move(reads);
        s.platform = setup_.platform;
        s.fn = std::move(fn);
        s.doc = std::move(doc);
        s.code = {std::string(kLaneCode)};
        return s;
    }

    // The module the lane prepared, live: the one this object holds under the document's hash, else made again by the lane.
    std::shared_ptr<const Prepared> live_module(Context& ctx, const LaneNow& now) const {
        const std::string key = std::format("{}|{}|{}|{}", K_, ctx.db().sha, now.base_hash, now.given.empty() ? "" : now.doc.sig);
        {
            const std::lock_guard<std::mutex> lock(state_->mutex);
            if (const auto it = state_->modules.find(key); it != state_->modules.end()) return it->second;
        }
        const std::filesystem::path binary = lane_binary(ctx.need("binary"));
        std::unique_ptr<Prepared> fresh = setup_.kind->prepare(binary, ctx.work("lane"));
        for (const auto& [name, text] : now.given) fresh->texts[name] = text;   // (a lane's Prepared cannot be copied: it is made again with them)
        fresh->sig = now.doc.sig;
        std::shared_ptr<const Prepared> made = std::move(fresh);
        const std::lock_guard<std::mutex> lock(state_->mutex);
        return state_->modules.try_emplace(key, std::move(made)).first->second;
    }

    // The lane's build of the module, and what the rows read of it.
    struct BuildMade {
        Value doc;
        std::shared_ptr<const Built> built;
    };
    BuildMade run_build(const Prepared& module, const std::filesystem::path& binary, const std::filesystem::path& work) const {
        std::error_code ec;
        std::filesystem::remove(work / "built.mark", ec);
        auto built = std::make_shared<const Built>(setup_.kind->build(module, binary, work));
        const verify::reconstruct::KindTraits& traits = setup_.traits;
        std::map<std::string, std::string> objects;
        if (traits.per_function && traits.object_digests) objects = traits.object_digests(work);
        const std::map<std::string, std::string> built_sigs = objects.empty() ? std::map<std::string, std::string>{}
                                                                              : verify::reconstruct::signatures(module, "", &objects);
        std::set<std::string> dropped;
        for (const auto& [n, why] : built->missing) dropped.insert(n);
        const std::set<std::string> tainted = verify::reconstruct::reaches(module.texts, built->items, dropped);
        {
            std::filesystem::create_directories(work);
            fs::write_text(work / "built.mark", std::format("{}\n", std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()));
        }
        Value::Map names, dropped_map, built_map;
        for (const auto& [name, key] : module.key) names.insert_or_assign(name, Value(key));
        for (const auto& [name, why] : built->missing) dropped_map.insert_or_assign(name, Value(why));
        for (const auto& [name, sig] : built_sigs) built_map.insert_or_assign(name, Value(sig));
        Value doc(Value::Map{{"items", lane_texts(built->items)},
                             {"dropped", Value(std::move(dropped_map))},
                             {"names", Value(std::move(names))},   // (name -> key; the rows turn it round)
                             {"built", Value(std::move(built_map))},
                             {"tainted", lane_texts(std::vector<std::string>(tainted.begin(), tainted.end()))}});
        return {std::move(doc), std::move(built)};
    }

    void profile() {
        Step s = base(L_ + "profile", {"binary"}, [setup = setup_](Context& ctx) -> StepResult {
            const verify::lanes::Profile p = setup.kind->profile(lane_binary(ctx.need("binary")));
            return facts::Value(Value(Value::List{Value(p.digest), Value(p.settings)}));
        }, "The lane's profile of the binary: [digest, settings] -- what its verdicts are filed under.");
        s.always = true;
        state_->steps.declare(std::move(s));
    }

    void module() {
        Step s = base(L_ + "module", {"binary"}, [self = this->shared(), L = L_](Context& ctx) -> StepResult {
            const std::filesystem::path binary = lane_binary(ctx.need("binary"));
            std::unique_ptr<Prepared> made = self->setup_.kind->prepare(binary, ctx.work("lane"));
            const Value doc = to_value(lane_doc_of(*made));
            std::shared_ptr<const Prepared> live = std::move(made);
            const std::lock_guard<std::mutex> lock(self->state_->mutex);
            self->state_->modules[std::format("{}|{}|{}|", self->K_, ctx.db().sha, lane_hash(facts::Value(doc)))] = std::move(live);
            return facts::Value(doc);
        }, "The module decompiled, as the lane prepares it (its prepare): its header, functions, keys and texts.");
        s.stamp = setup_.module_stamp;
        state_->steps.declare(std::move(s));
    }

    void function() {
        Step s = base(L_ + "function", {L_ + "module"}, [L = L_](Context& ctx) -> StepResult {
            const ModuleDoc d = module_doc_of(lane_doc(ctx.need(L + "module"), L + "module"));
            for (const std::string& name : d.order) {
                const auto k = d.key.find(name);
                if (k == d.key.end() || k->second != ctx.key()) continue;
                const auto t = d.texts.find(name);
                return facts::Value(Value(Value::List{Value(name), t == d.texts.end() ? Value() : Value(t->second)}));
            }
            return facts::Value(Value());
        }, "One function: [name, C] (or given by hand: Engine::put).");
        state_->steps.declare(std::move(s));
    }

    void keys() {
        Step s = base(L_ + "keys", {L_ + "module"}, [L = L_](Context& ctx) -> StepResult {
            const ModuleDoc d = module_doc_of(lane_doc(ctx.need(L + "module"), L + "module"));
            std::set<std::string> have;
            for (const std::string& n : d.order)
                if (const auto k = d.key.find(n); k != d.key.end()) have.insert(k->second);
            return facts::Value(Value(Value::Map{{"keys", lane_texts(std::vector<std::string>(have.begin(), have.end()))},
                                                 {"all_keys", lane_texts(d.all_keys)}}));
        }, "The keys of the functions that have C, and of every function of the module.");
        state_->steps.declare(std::move(s));
    }

    void texts() {
        Step s = base(L_ + "texts", {L_ + "module", L_ + "keys", L_ + "function"}, [this_ = this->shared(), L = L_](Context& ctx) -> StepResult {
            const LaneNow now = lane_now(ctx, L);
            const Prepared view = lane_prepared_of(now.doc);
            const std::map<std::string, std::string> sig = signatures_of(view, this_->setup_.traits.per_function);
            Value::List order;
            Value::Map functions;
            for (const std::string& n : now.doc.order) {
                const auto k = now.doc.key.find(n);
                if (k == now.doc.key.end()) continue;
                order.emplace_back(k->second);
                const auto t = now.doc.texts.find(n);
                const auto g = sig.find(n);
                functions.insert_or_assign(k->second, Value(Value::List{Value(n), Value(g == sig.end() ? std::string() : g->second),
                                                                        t == now.doc.texts.end() ? Value() : Value(t->second)}));
            }
            return facts::Value(Value(Value::Map{{"all_keys", lane_texts(now.doc.all_keys)},
                                                 {"header", Value(now.doc.header)},
                                                 {"order", Value(std::move(order))},
                                                 {"functions", Value(std::move(functions))}}));
        }, "{all_keys, header, order, functions: {key: [name, signature, C]}}: the module's C as it is now (a function given by hand in it), and each function's signature -- what its verdict is filed under.");
        state_->steps.declare(std::move(s));
    }

    void build() {
        Step s = base(L_ + "build", {"binary", L_ + "module", L_ + "keys", L_ + "function"}, [this_ = this->shared(), L = L_](Context& ctx) -> StepResult {
            const LaneNow now = lane_now(ctx, L);
            const std::filesystem::path binary = lane_binary(ctx.need("binary"));
            const std::filesystem::path work = ctx.work("lane");
            const std::shared_ptr<const Prepared> module = this_->live_module(ctx, now);
            BuildMade made = this_->run_build(*module, binary, work);
            const std::lock_guard<std::mutex> lock(this_->state_->mutex);
            this_->state_->builds[fs::to_text(work)] = {lane_hash(facts::Value(made.doc)), made.built};
            return facts::Value(std::move(made.doc));
        }, "The functions built for judging (the lane's build): {items, dropped, names, built, tainted}.");
        s.stamp = setup_.build_stamp;
        state_->steps.declare(std::move(s));
    }

    void built() {
        Step s = base(L_ + "built", {L_ + "build", L_ + "texts"}, [L = L_](Context& ctx) -> StepResult {
            const Value b = lane_doc(ctx.need(L + "build"), L + "build");
            std::optional<std::string> found;
            for (const auto& [n, k] : *lane_member(b, "names").map())
                if (lane_text(k) == ctx.key()) found = n;
            bool in_items = false;
            if (found)
                for (const Value& x : *lane_member(b, "items").list())
                    if (lane_text(x) == *found) in_items = true;
            if (!found || !in_items) {
                std::string why;
                if (found)
                    if (const Value* w = lane_member(b, "dropped").get(*found)) why = lane_text(*w);
                return facts::Value(Value(Value::List{Value("dropped"), Value(why.empty() ? std::string("not in the build") : why)}));
            }
            const Value::Map& built = *lane_member(b, "built").map();
            if (!built.empty()) {
                const Value* sig = lane_member(b, "built").get(*found);
                return facts::Value(Value(Value::List{Value("built"), sig ? *sig : Value()}));
            }
            const Value texts = lane_doc(ctx.need(L + "texts"), L + "texts");
            const Value& fn = lane_member(lane_member(texts, "functions"), ctx.key());
            return facts::Value(Value(Value::List{Value("built"), (*fn.list())[1]}));
        }, "What the function's verdict is reached for: [\"built\", signature] or [\"dropped\", why].");
        state_->steps.declare(std::move(s));
    }

    void verdict() {
        Step s = base(L_ + "verdict", {L_ + "built", L_ + "profile"}, nullptr,
                      "The function judged beside the original (the lane's judge): [verdict, reason, limits, detail]. Asked for by lane.K.rows, which hands it the build.");
        s.batch_fn = [this_ = this->shared(), L = L_](std::span<Context* const> ctxs) -> std::map<std::string, StepResult> {
            if (ctxs.empty()) return {};
            const auto* hint = std::any_cast<LaneVerdictHint>(&ctxs.front()->hint());
            if (hint == nullptr) throw UsageError("a verdict is judged in a build of the module: ask for " + L + "rows");
            std::map<std::string, StepResult> out;
            std::vector<std::string> todo_names;        // the functions to judge
            std::map<std::string, std::string> key_of;  // name -> key
            for (Context* ctx : ctxs) {
                const Value b = lane_doc(ctx->need(L + "built", ctx->key()), L + "built");
                ctx->need(L + "profile");
                const Value::List& what = *b.list();
                if (lane_text(what.at(0)) == "dropped") {
                    out[ctx->key()] = facts::Value(Value(Value::List{Value(std::string(verify::word(verify::Verdict::NoCompile))), what.at(1), Value(""), Value("")}));
                } else {
                    const std::string& name = hint->names.at(ctx->key());
                    todo_names.push_back(name);
                    key_of[name] = ctx->key();
                }
            }
            if (!todo_names.empty()) {
                const std::shared_ptr<const Built> built = hint->ready();
                jobs::RunOptions<std::string, verify::Judgement> options;
                options.jobs = static_cast<int>(this_->state_->engine.jobs());
                options.crashed = [](const std::string&) { return verify::not_run(); };
                // One function's judge failing alone is that function's "not run"; the others go on.
                const auto judge_one = [&built](const std::string& n) {
                    try {
                        return built->judge(n);
                    } catch (const std::exception& e) {   // isolation boundary: one function of a batch
                        log::warn(std::format("lane judge: {} failed: {}", n, e.what()));
                        return verify::not_run();
                    }
                };
                const auto judged = jobs::run(todo_names, judge_one, [](const std::string& n) { return n; }, options);
                for (const std::string& n : todo_names) {
                    const verify::Judgement* j = judged.find(n);
                    const verify::Judgement got = j ? *j : verify::not_run();
                    const std::string limits = hint->tainted.contains(n) && verify::is_good(got.verdict) ? lane_with_stub_callee(got.limits) : got.limits_joined();
                    out[key_of.at(n)] = facts::Value(Value(Value::List{Value(std::string(verify::word(got.verdict))), Value(got.reason), Value(limits), Value(got.detail)}));
                }
            }
            return out;
        };
        state_->steps.declare(std::move(s));
    }

    void rows() {
        Step s = base(L_ + "rows",
                      {"binary", L_ + "module", L_ + "keys", L_ + "texts", L_ + "function", L_ + "build", L_ + "built", L_ + "verdict", L_ + "profile"},
                      [this_ = this->shared(), L = L_](Context& ctx) -> StepResult {
                          const std::filesystem::path binary = lane_binary(ctx.need("binary"));
                          const Value t = lane_doc(ctx.need(L + "texts"), L + "texts");
                          const Value::Map& fns = *lane_member(t, "functions").map();
                          const Value profile = lane_doc(ctx.need(L + "profile"), L + "profile");
                          const std::vector<std::string> all_keys = lane_text_list(lane_member(t, "all_keys"));
                          Value::List rows;
                          for (const std::string& k : all_keys) {
                              if (fns.find(k) != nullptr) continue;
                              rows.push_back(Value(Value::Map{{"addr", Value(k)}, {"sig", Value("-")}, {"name", Value("")}, {"source", Value()},
                                                              {"verdict", Value(std::string(verify::word(verify::Verdict::NoSource)))},
                                                              {"reason", Value("the decompiler produced nothing for this function")}, {"problem", Value("none")}}));
                          }
                          Value::Map out_built;
                          auto result = [&](Value rows_now, Value built_now) {
                              return facts::Value(Value(Value::Map{{"profile", (*profile.list())[0]}, {"settings", (*profile.list())[1]}, {"header", lane_member(t, "header")},
                                                                   {"all_keys", lane_member(t, "all_keys")}, {"rows", std::move(rows_now)}, {"built", std::move(built_now)}}));
                          };
                          if (fns.empty()) return result(Value(std::move(rows)), Value(Value::Map{}));
                          const facts::Value build_fact = ctx.need(L + "build");
                          const Value b = lane_doc(build_fact, L + "build");
                          std::vector<std::string> mine;   // judged in the module's order
                          for (const Value& k : *lane_member(t, "order").list()) {
                              const std::string key = lane_text(k);
                              if (std::find(mine.begin(), mine.end(), key) == mine.end()) mine.push_back(key);
                          }
                          ctx.need_many(L + "built", mine);
                          std::map<std::string, std::string> names;
                          for (const std::string& k : mine) names[k] = lane_text((*fns.find(k)->list())[0]);
                          LaneVerdictHint hint;
                          hint.names = names;
                          for (const Value& n : *lane_member(b, "tainted").list()) hint.tainted.insert(lane_text(n));
                          const std::filesystem::path work = ctx.work("lane");
                          const std::string build_hash = lane_hash(build_fact);
                          hint.ready = [this_, &ctx, L, binary, work, build_hash]() -> std::shared_ptr<const Built> {
                              {   // the kept build: its files are there and this object still holds its judge
                                  const std::lock_guard<std::mutex> lock(this_->state_->mutex);
                                  const auto it = this_->state_->builds.find(fs::to_text(work));
                                  if (it != this_->state_->builds.end() && it->second.hash == build_hash && std::filesystem::is_regular_file(work / "built.mark")) return it->second.built;
                              }
                              // its files gone, or this is a new process: made again there
                              const LaneNow now = lane_now(ctx, L);
                              const std::shared_ptr<const Prepared> module = this_->live_module(ctx, now);
                              BuildMade made = this_->run_build(*module, binary, work);
                              const std::lock_guard<std::mutex> lock(this_->state_->mutex);
                              this_->state_->builds[fs::to_text(work)] = {lane_hash(facts::Value(made.doc)), made.built};
                              return made.built;
                          };
                          const auto verdicts = ctx.need_many(L + "verdict", mine, std::any(hint));
                          for (const std::string& k : mine) {
                              const Value::List& fn = *fns.find(k)->list();
                              const Value::List& v = *lane_doc(verdicts.at(k), L + "verdict").list();
                              Value::Map row{{"addr", Value(k)}, {"sig", fn[1]}, {"name", fn[0]}, {"source", fn[2]}, {"verdict", v[0]},
                                             {"reason", v[1]}, {"limits", v[2]}, {"detail", v[3]}};
                              if (lane_text(v[0]) == verify::word(verify::Verdict::NoCompile)) row.insert_or_assign("problem", Value("compile"));
                              rows.push_back(Value(std::move(row)));
                          }
                          Value::Map built_now;
                          const Value::Map& sigs = *lane_member(b, "built").map();
                          for (const auto& [k, n] : names)
                              if (const Value* sig = sigs.find(n)) built_now.insert_or_assign(k, *sig);
                          return result(Value(std::move(rows)), Value(std::move(built_now)));
                      },
                      "The rows reconstruct records: {profile, settings, header, all_keys, rows, built}.");
        state_->steps.declare(std::move(s));
    }

    // pe.image: the module linked (the lane "pe"'s own step).
public:
    void declare_pe_image(PeLink link, std::string platform, std::function<std::string()> stamp) {
        Step s = base("pe.image", {"binary", L_ + "module", L_ + "keys", L_ + "function"},
                      [this_ = this->shared(), link = std::move(link), L = L_](Context& ctx) -> StepResult {
                          const LaneNow now = lane_now(ctx, L);
                          const std::filesystem::path binary = lane_binary(ctx.need("binary"));
                          const std::shared_ptr<const Prepared> module = this_->live_module(ctx, now);
                          const std::filesystem::path folder = ctx.work("build");
                          Linked linked = link(*module, binary, folder);
                          Value::Map rejected;
                          for (const auto& [name, why] : linked.rejected) rejected.insert_or_assign(name, Value(why));
                          Value file, sha;
                          if (linked.image) {
                              const std::string name = text::lower(fs::to_text(binary.filename()));
                              fs::write_file(folder / name, *linked.image);
                              file = Value(name);
                              sha = Value(crypto::to_hex(crypto::sha256(*linked.image)));
                          }
                          return facts::Value(Value(Value::Map{{"name", Value(linked.name)}, {"image", file}, {"image_sha256", sha},
                                                               {"rejected", Value(std::move(rejected))}, {"report", Value(linked.report)}}));
                      },
                      "The module linked (the packager's link, then the check of the image against the original): {name, report, rejected, image: its file in the build folder, image_sha256}.");
        s.platform = std::move(platform);
        s.stamp = std::move(stamp);
        state_->steps.declare(std::move(s));
    }

private:
    // (each step keeps this object alive: it captures what it needs by it)
    std::shared_ptr<LaneSteps> shared() { return shared_from_this(); }

    std::shared_ptr<LaneState> state_;
    Setup setup_;
    std::string K_, L_;
};

}  // namespace

}  // namespace cb::engine::lanes

// ---- the object's members ---------------------------------------------------------------------------------------------------

namespace cb::engine::lanes {

namespace {

// How a kind's rows come back out of the fact.
Judged lane_judged_of(const Value& doc) {
    Judged out;
    out.profile = lane_text(lane_member(doc, "profile"));
    out.settings = lane_text(lane_member(doc, "settings"));
    out.header = lane_text(lane_member(doc, "header"));
    out.all_keys = lane_text_list(lane_member(doc, "all_keys"));
    for (const Value& r : *lane_member(doc, "rows").list()) {
        JudgedRow row;
        row.addr = lane_text(lane_member(r, "addr"));
        row.sig = lane_text(lane_member(r, "sig"));
        row.name = lane_text(lane_member(r, "name"));
        if (const std::string* t = lane_member(r, "source").text()) row.source = *t;
        const std::optional<verify::Verdict> verdict = verify::parse_verdict(lane_text(lane_member(r, "verdict")));
        if (!verdict) throw FormatError("engine fact", 0, "a row has a verdict word that is none: " + lane_text(lane_member(r, "verdict")));
        row.verdict = *verdict;
        row.reason = lane_text(lane_member(r, "reason"));
        if (const Value* v = r.get("limits")) row.limits = lane_text(*v);
        if (const Value* v = r.get("detail")) row.detail = lane_text(*v);
        if (const Value* v = r.get("problem")) row.problem = lane_text(*v);
        out.rows.push_back(std::move(row));
    }
    for (const auto& [k, sig] : *lane_member(doc, "built").map()) out.built[k] = lane_text(sig);
    return out;
}

}  // namespace

Lanes::Lanes(Steps& steps, Engine& engine, std::function<std::optional<std::string>(const std::filesystem::path&)> kind_of_file)
    : state_(std::make_shared<detail::LaneState>(steps, engine, std::move(kind_of_file))) {}

Lanes::~Lanes() = default;

std::string Lanes::declare(Setup setup) {
    if (!setup.kind) throw UsageError("a lane's steps need its Kind");
    const std::string K = setup.kind->name();
    {
        const std::lock_guard<std::mutex> lock(state_->mutex);
        if (std::find(state_->kind_order.begin(), state_->kind_order.end(), K) == state_->kind_order.end()) state_->kind_order.push_back(K);
        state_->setups[K] = setup;
    }
    const auto steps = std::make_shared<LaneSteps>(state_, std::move(setup));
    steps->declare();
    const std::lock_guard<std::mutex> lock(state_->mutex);
    state_->lane_steps[K] = steps;
    return K;
}

void Lanes::declare_pe_image(PeLink link, std::string platform, std::function<std::string()> stamp) {
    std::shared_ptr<LaneSteps> pe;
    {
        const std::lock_guard<std::mutex> lock(state_->mutex);
        if (const auto it = state_->lane_steps.find("pe"); it != state_->lane_steps.end()) pe = it->second.lock();
    }
    if (!pe) throw UsageError("pe.image is made from the lane \"pe\": declare that lane first");
    pe->declare_pe_image(std::move(link), std::move(platform), std::move(stamp));
}

std::vector<std::string> Lanes::kinds() const {
    const std::lock_guard<std::mutex> lock(state_->mutex);
    return state_->kind_order;
}

bool Lanes::on(const std::filesystem::path& module) const {
    const std::optional<std::string> k = state_->kind_of_file(module);
    if (!k) return false;
    const std::lock_guard<std::mutex> lock(state_->mutex);
    return state_->setups.contains(*k);
}

std::shared_ptr<facts::Database> Lanes::open_module(const std::filesystem::path& module) {
    Engine& engine = state_->engine;
    if (std::filesystem::is_regular_file(module)) return engine.open(module);
    const std::optional<std::string> k = state_->kind_of_file(module);
    std::optional<std::string> sha;
    if (k) {
        const std::lock_guard<std::mutex> lock(state_->mutex);
        if (const auto it = state_->setups.find(*k); it != state_->setups.end() && it->second.traits.sha) sha = it->second.traits.sha(module);
    }
    if (!sha) throw UsageError(fs::to_text(module) + ": nothing says what its results are filed under");
    auto db = engine.databases().open_at(engine.databases().config().root / (*sha + ".sqlite"));
    db->sha = *sha;
    const std::filesystem::path absolute = std::filesystem::absolute(module);
    if (!db->about("name")) db->set_about("name", fs::to_text(module.has_filename() ? module.filename() : module.parent_path().filename()));
    db->set_about("path", fs::to_text(absolute));
    const Value doc(Value::Map{{"path", Value(fs::to_text(absolute))}, {"size", Value(std::int64_t{0})}});
    const std::string hash = "sha256:" + *sha;
    bool current = false;
    if (const auto row = db->row("binary", ""); row && row->hash == hash) {
        try {
            const facts::Value kept = row->value();
            current = std::holds_alternative<Value>(kept) && std::get<Value>(kept) == doc;
        } catch (const Error&) {   // a binary row in a form no longer readable is written again
        }
    }
    if (!current) {
        const facts::Encoded e = facts::encode(facts::Value(doc));
        db->write_raw("binary", "", hash, e.tag, e.data, facts::kGiven, "", "[]", 0);
    }
    return db;
}

Judged Lanes::judged(const std::filesystem::path& module, const Run::Log& log) {
    const std::optional<std::string> k = state_->kind_of_file(module);
    if (!k || !on(module)) throw UsageError(fs::to_text(module) + ": no declared lane takes it");
    const auto db = open_module(module);
    const facts::Value rows = state_->engine.get(db, "lane." + *k + ".rows", "", log);
    Judged out = lane_judged_of(lane_doc(rows, "the rows"));
    out.made = state_->engine.last_run(*db);
    return out;
}

PeBuild Lanes::build_pe(const std::filesystem::path& module, const std::optional<std::filesystem::path>& outdir, const Run::Log& log) {
    const std::optional<std::string> k = state_->kind_of_file(module);
    if (!k || *k != "pe") throw UsageError(fs::to_text(module) + ": a module of another lane is not linked whole here");
    const std::string name = text::lower(fs::to_text(module.filename()));
    const auto started = std::chrono::steady_clock::now();
    const auto db = state_->engine.open(module);
    if (log) log(std::format("{}: on the engine ({})", name, fs::to_text(db->path())));
    facts::Value got = state_->engine.get(db, "pe.image", "", log);
    const std::filesystem::path folder = facts::Databases::work_folder(*db, "build");
    if (const Value* file = lane_doc(got, "pe.image").get("image"); file && file->text() && !std::filesystem::is_regular_file(folder / *file->text())) {
        // the build folder was cleared since the fact was made: made again
        state_->engine.forget(*db, "pe.image");
        got = state_->engine.get(db, "pe.image", "", log);
    }
    const std::vector<Run::Count> made = state_->engine.last_run(*db);
    if (log) {
        std::string counts;
        std::vector<Run::Count> sorted = made;
        std::sort(sorted.begin(), sorted.end(), [](const Run::Count& a, const Run::Count& b) { return a.kind < b.kind; });
        for (const Run::Count& c : sorted) counts += std::format("{}{} {}/{}", counts.empty() ? "" : ", ", c.kind, c.kept, c.made);
        log(std::format("{}: {:.1f}s; facts kept/made: {}", name, std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count(), counts));
    }
    const Value& doc = lane_doc(got, "pe.image");
    PeBuild out;
    out.name = lane_text(lane_member(doc, "name"));
    out.report = lane_text(lane_member(doc, "report"));
    for (const auto& [n, why] : *lane_member(doc, "rejected").map()) out.rejected[n] = lane_text(why);
    if (const std::string* file = lane_member(doc, "image").text()) {
        out.image = folder / *file;
        if (outdir) {
            std::filesystem::create_directories(*outdir);
            std::filesystem::copy_file(folder / *file, *outdir / *file, std::filesystem::copy_options::overwrite_existing);
            out.image = *outdir / *file;
        }
    }
    return out;
}

}  // namespace cb::engine::lanes
