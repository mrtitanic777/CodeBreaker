// engine/engine.hpp -- the incremental engine: a fact is made once, and made again only when what it was made from
// changed.
//
//     Steps steps;                                                // the registry; "binary" and "file" are in it
//     steps.declare({.kind = "function.object", .fn = make_object, .reads = {"function.source", "ne.header"},
//                    .platform = "win16"});                         // make_object(ctx): ctx.need("function.source", ctx.key())
//     Engine engine(steps, databases, digests, /*jobs*/ 8);
//     auto db = engine.open("lz32.dll");                           // the binary's database; its bytes are the fact `binary`
//     Value image = engine.get(db, "ne.image");                    // the fact, current: kept, or made again
//     auto texts = engine.get_many(db, "function.c", keys);        // {key: value}, the stale ones made side by side
//     engine.put(*db, "function.c", "1:7cd4", text);               // a fact given by hand (an edit): kept until forgotten
//     engine.forget(*db, "function.c", "1:7cd4");                  // ... and made by its step again
//     engine.last_run(*db);                                        // [{kind, kept, made}] of the last get
//
// A step makes one kind of fact. It is declared with what it reads and makes. Its VERSION is a digest of its code (the
// texts in `code`: the step's own source identity and one per module or helper it uses) and of what `stamp()` gives (the
// tools it runs, the settings it reads). The facts it reads are recorded as it reads them (`ctx.need`), each with the
// hash it had: what a fact depends on is what its step actually asked for. `reads` is the promise (asking for a kind
// not in it is a UsageError).
//
// When a fact is asked for it is CURRENT if its step's version is the version of now and every fact it was made from
// is current and has the hash it had then; that is decided once per run, and recursively. A stale one is made again --
// and when it comes out the same as before (the same hash) nothing made from it is stale. `get_many` finds which of the
// facts asked for are stale, then makes those at once on threads. A step declared with a `batch_fn` is given all its stale
// keys in one call instead. Some facts are not made from facts: a step declared `always` is run once per run (a file's
// hash) and its value compared. A step may return Unkept{value}: the value is used this run and not kept.
//
// What a step read of the knowledge base is recorded as it runs (knowledge/reads.hpp) and each piece is an input of the
// fact like any other: ("kb", "platform:kind:key", the piece's hash now).
//
// Nothing here is global: the registry, the databases and the knowledge digests belong to the caller. An Engine and its
// Runs are thread-safe; a step's function runs on whichever thread the run chose.
#pragma once

#include <any>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include "core/error.hpp"
#include "engine/facts.hpp"
#include "knowledge/reads.hpp"

namespace cb::engine {

using Value = facts::Value;

// A step's value used this run and not kept.
struct Unkept {
    Value value;
};
using StepResult = std::variant<Value, Unkept>;

// No step makes facts of the kind asked for.
class UnknownStepError : public Error {
public:
    explicit UnknownStepError(std::string_view kind) : Error("no step makes facts of the kind '" + std::string(kind) + "'"), kind_(kind) {}
    const std::string& step_kind() const { return kind_; }
    CB_ERROR_BOILERPLATE(UnknownStepError)

private:
    std::string kind_;
};

class Context;
class Engine;
class Run;

// fn(ctx) -> value; the key being made is ctx.key().
using StepFn = std::function<StepResult(Context& ctx)>;
// A batch step is given a context for each stale key and answers {key: value}; a key it leaves out is an error.
using BatchFn = std::function<std::map<std::string, StepResult>(std::span<Context* const> contexts)>;

struct Step {
    std::string kind;
    StepFn fn;                          // set for a one-key step
    BatchFn batch_fn;                   // set for a batch step
    std::vector<std::string> reads;     // the kinds it may ask for ("binary" and "file" always may)
    std::string platform;               // "": every platform
    std::function<std::string()> stamp; // the tools it runs, the settings it reads: digested with the code
    bool always = false;                // run once per run (a file's hash), its value compared
    std::optional<std::string> fixed_version;   // fixes the version instead of the code's digest (tests)
    std::string doc;                    // what it makes, in a paragraph
    std::vector<std::string> code;      // the step's own code identity, then one per module or helper it uses

    bool batch() const { return static_cast<bool>(batch_fn); }
    bool declares(std::string_view read_kind) const;
    // A digest of the code that makes this step's facts, and what stamp() gives, now (16 hex digits).
    std::string version() const;
};

// The registry of steps by kind. A platform's steps may be declared lazily: `set_loader` says which platforms there
// are and how to declare one's steps, and a kind nobody has declared yet is looked for by loading them all.
class Steps {
public:
    using Loader = std::function<void(Steps& steps, std::string_view platform)>;

    Steps();   // declares the two built-in kinds: "binary" and "file"

    // Declare a step (replacing one of the same kind); -> the registered step.
    std::shared_ptr<const Step> declare(Step step);
    // Declare how the platforms' steps are loaded (once per platform, when asked for) and which platforms there are.
    void set_loader(Loader loader, std::vector<std::string> platforms);

    // The step of `kind`; UnknownStepError when none makes it (after loading every platform's steps).
    std::shared_ptr<const Step> of(std::string_view kind);
    std::shared_ptr<const Step> find(std::string_view kind) const;
    // {kind: step} of a platform's steps (and the platform-free ones), or of every step known (every platform loaded).
    std::vector<std::shared_ptr<const Step>> list(std::optional<std::string_view> platform = std::nullopt);

private:
    void load(std::string_view platform);
    void load_all();

    mutable std::mutex mutex_;
    std::map<std::string, std::shared_ptr<const Step>, std::less<>> steps_;   // by kind, listed in kind order
    Loader loader_;
    std::vector<std::string> platforms_;
    std::vector<std::string> loaded_;
};

// What a step is given: the key it makes, and the facts it reads, recorded as it asks.
class Context {
public:
    Context(Run& run, std::shared_ptr<const Step> step, std::string key, std::any hint);

    const std::string& key() const { return key_; }
    const Step& step() const { return *step_; }
    // What the asker passed to many / need_many for the steps made on its behalf.
    const std::any& hint() const { return hint_; }
    facts::Database& db() const;
    // The facts read so far, as they will be recorded.
    const std::vector<facts::Input>& inputs() const { return inputs_; }

    // A fact this one is made from (current, made first if need be).
    Value need(std::string_view kind, std::string_view key = "");
    // Many facts of one kind, the stale ones made side by side.
    std::map<std::string, Value> need_many(std::string_view kind, std::span<const std::string> keys, const std::any& hint = {});
    // The fact if it is current without making anything, else nullopt (and nothing is recorded).
    std::optional<Value> peek(std::string_view kind, std::string_view key = "");
    // A file that is not a fact: its SHA-1 is the input.
    Value file(const std::filesystem::path& path);
    // Pieces of the knowledge base read ("platform:kind:key"): inputs, with their hashes now.
    void knowledge(std::span<const std::string> reads);
    // The value this fact had before this run, if any.
    std::optional<Value> previous() const;
    // A folder for the files the step writes as it works, beside the database (made when asked for).
    std::filesystem::path work(const std::filesystem::path& sub = {}) const;

private:
    friend class Run;
    void may(std::string_view kind) const;
    void record(std::string_view kind, std::string_view key);

    Run& run_;
    std::shared_ptr<const Step> step_;
    std::string key_;
    std::any hint_;
    std::vector<facts::Input> inputs_;
    std::optional<StepResult> result_;
};

// One asking of the engine: each fact decided current or made again at most once in it.
class Run {
public:
    using Log = std::function<void(std::string_view)>;

    Run(Engine& engine, std::shared_ptr<facts::Database> db, Log log = nullptr);
    Run(const Run&) = delete;
    Run& operator=(const Run&) = delete;

    struct Count {
        std::string kind;
        std::size_t kept = 0, made = 0;
    };
    // How many facts of each kind were found current and how many made, in the order the kinds were first met.
    std::vector<Count> counts() const;

    facts::Database& db() const { return *db_; }
    Engine& engine() const { return engine_; }

    // The fact's value, current (made first if need be).
    Value value(std::string_view kind, std::string_view key = "");
    // The hash of the fact as kept; nullopt for a value that was not kept, or a piece of the knowledge base's absent.
    std::optional<std::string> hash_of(std::string_view kind, std::string_view key = "");
    // The keys among `keys` that are stale (the rest are decided current, and counted).
    std::vector<std::string> stale(std::string_view kind, std::span<const std::string> keys);
    // Make `keys` current, the stale ones side by side; -> {key: value}.
    std::map<std::string, Value> many(std::string_view kind, std::span<const std::string> keys, const std::any& hint = {});

private:
    friend class Context;
    friend class Engine;

    struct Done {
        bool kept = false;     // has a hash (else an unkept value)
        std::string hash;
    };
    using Key = std::pair<std::string, std::string>;

    Done current(const std::string& kind, const std::string& key);
    bool done_of(const Key& key, Done& out);
    void set_done(const Key& key, const Done& done);
    std::recursive_mutex& lock_for(const Key& key);
    void count(const std::string& kind, bool made);
    std::string version_of(const std::string& kind);
    bool still(const facts::Row& row);
    void make(const std::string& kind, const std::vector<std::string>& keys,
              const std::map<std::string, std::optional<facts::Row>>* rows, const std::any& hint);
    void make_all(const std::string& kind, const std::vector<std::string>& todo, const std::any& hint);
    void say(const std::string& text);
    void set_value(const Key& key, Value value);
    std::optional<Value> cached_value(const Key& key);

    Engine& engine_;
    std::shared_ptr<facts::Database> db_;
    Log log_;
    std::atomic<bool> log_on_;

    std::recursive_mutex mutex_;
    std::map<Key, Done> done_;
    std::map<Key, std::unique_ptr<std::recursive_mutex>> locks_;
    std::map<std::string, std::string> versions_;
    std::vector<std::string> count_order_;
    std::map<std::string, std::pair<std::size_t, std::size_t>> counts_;
    std::map<Key, Value> values_;   // values made or read in this run (the steps write it from their threads)
};

// How a fact was made: its step, the version then and now, and each input with whether it still holds.
struct Why {
    struct InputNow {
        facts::Input input;
        bool same = false;   // the input's hash is what it was
    };
    std::string kind, key, hash;
    std::uint64_t size = 0;
    std::string step, version;
    double made = 0, took = 0;
    std::optional<std::string> version_now;   // nullopt when no step makes the kind any more
    std::vector<InputNow> inputs;
};

class Engine {
public:
    // `jobs` facts are made at once at most (cb::parallel::workers() is what callers usually pass).
    Engine(Steps& steps, facts::Databases& databases, knowledge::ReadDigests& digests, std::size_t jobs);

    Steps& steps() const { return steps_; }
    facts::Databases& databases() const { return databases_; }
    knowledge::ReadDigests& digests() const { return digests_; }
    std::size_t jobs() const { return jobs_; }

    // The database of the binary at `path`, with the binary given as the fact `binary` ({path, size}; its hash is the
    // binary's SHA-256, so moving the file changes nothing made from it).
    std::shared_ptr<facts::Database> open(const std::filesystem::path& path, const std::optional<std::filesystem::path>& folder = std::nullopt);

    // The fact, current (see the file's text).
    Value get(const std::shared_ptr<facts::Database>& db, std::string_view kind, std::string_view key = "", Run::Log log = nullptr);
    std::map<std::string, Value> get_many(const std::shared_ptr<facts::Database>& db, std::string_view kind,
                                          std::span<const std::string> keys, Run::Log log = nullptr);

    // Per kind, [kept, made] of the last get on this database in this engine.
    std::vector<Run::Count> last_run(const facts::Database& db) const;

    // A fact given by hand: what is made from it is stale when it differs from what was there. -> its hash.
    std::string put(facts::Database& db, std::string_view kind, std::string_view key, const Value& value);
    // Facts gone (one, or a kind): the next get makes them again. -> how many.
    std::uint64_t forget(facts::Database& db, std::string_view kind, std::optional<std::string_view> key = std::nullopt);
    // How a fact was made -- read only; nothing is made. nullopt when there is no such fact.
    std::optional<Why> why(facts::Database& db, std::string_view kind, std::string_view key = "");

private:
    std::shared_ptr<Run> remember(const std::shared_ptr<facts::Database>& db, Run::Log log);

    Steps& steps_;
    facts::Databases& databases_;
    knowledge::ReadDigests& digests_;
    std::size_t jobs_;
    mutable std::mutex runs_mutex_;
    std::map<std::string, std::shared_ptr<Run>> runs_;   // the last Run of each database (by path): looked up
};

}  // namespace cb::engine
