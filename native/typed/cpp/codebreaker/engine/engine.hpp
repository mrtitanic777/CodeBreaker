// codebreaker/engine/engine.hpp -- typed C++ port of codebreaker.engine.engine.
//
// engine.py -- the incremental engine: a fact is made once, and made again only when what it was made from changed.
//
//     auto db = engine::open(path);                         // the binary's database; its bytes are the fact `binary`
//     auto image = engine::get(db, "ne.image");             // the fact, current: kept, or made again
//     auto texts = engine::get_many(db, "function.c", keys);// {key: value}, the stale ones made side by side
//     engine::put(db, "function.c", "1:7cd4", text);        // a fact given by hand (an edit): kept until forgotten
//     engine::forget(db, "function.c", "1:7cd4");           // ... and made by its step again
//     engine::last_run(db);                                 // {kind: [kept, made]} of the last get
//
// A step makes one kind of fact. It is a function, declared with what it reads and makes:
//
//     engine::step("function.object", fn, {"function.source", "ne.header"}, "win16");
//     // fn(ctx, key):  auto src = ctx.need("function.source", key);  ...  return value;
//
// Its VERSION is a digest of its code and of what `stamp()` gives. The facts it reads are recorded as it
// reads them (ctx.need), each with the hash it had: what a fact depends on is what its step actually asked
// for. `reads` is the promise (asking for a kind not in it is an error).
//
// When a fact is asked for it is CURRENT if its step's version is the version of now and every fact it was
// made from is current and has the hash it had then; that is decided once per run, and recursively. A stale
// one is made again -- and when it comes out the same as before (the same hash) nothing made from it is
// stale. get_many() finds which of the facts asked for are stale, then makes those at once on threads. A
// step declared `batch` is given all its stale keys in one call instead. Some facts are not made from facts:
// a step declared `always` is run once per run (a file's hash) and its value compared. A step may return
// Unkept(value): the value is used this run and not kept.
//
// What a step reads of the knowledge base is recorded as it runs (knowledge/reads.hpp) and each piece is an
// input of the fact like any other: ("kb", "platform:kind:key", the piece's hash).
//
// The core of this -- the registry, which facts are current, the inputs recorded, what is written, kept or
// remade, the order and the worker threads -- is C++ (native/engine, linked in): native_engine::Run is its.
//
// Notes on the port:
//   * A step's code is a std::function, not a Python function: its VERSION cannot be a digest of the
//     function's source text (inspect), so a Step carries `code`, the list of texts (the step's own source
//     identity, and one per module / helper named in `uses`) its version digests. The stamp is the
//     std::function<py::str()> whose result (the Python's repr(stamp())) is digested too; the digest of its
//     source text is not part of the version.
//   * platforms.step_modules() / platforms.named(p).steps (modules the Python imports to declare their
//     steps) have no typed port: platform_loader() is the hook that declares a platform's steps, and
//     platform_names() lists the platforms (unset: none).
//   * inspect.getdoc(fn): `doc` is given at declaration.
#pragma once

#include <any>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <variant>

#include "codebreaker/engine/facts.hpp"
#include "codebreaker/engine/native_engine.hpp"
#include "py.hpp"

namespace codebreaker::engine {

using Value = facts::Value;
using Context = native_engine::Context;
using Run = native_engine::Run;

// A step's value used this run and not kept.
struct Unkept {
    Value value;
};
using StepResult = std::variant<Value, Unkept>;

// fn(ctx, key) -> value
using StepFn = std::function<StepResult(Context&, const py::str&)>;
// A batch step: fn({key: ctx}, hint) -> {key: value}
using BatchFn = std::function<py::dict<py::str, StepResult>(const py::dict<py::str, Context*>&, const std::any&)>;

struct Step {
    py::str kind;
    StepFn fn;
    BatchFn batch_fn;                       // set for batch steps
    py::list<py::str> reads;
    py::str platform;
    py::list<py::str> uses;                 // modules / helpers named, as their code texts
    std::function<py::str()> stamp;         // the tools it runs, the settings it reads: repr'd and digested
    bool batch = false, always = false;
    std::optional<py::str> fixed_version;
    py::str doc;                            // the first paragraph of the step's docstring
    py::list<py::str> code;                 // the step's own code identity, then `uses`' (what version() digests)

    // A digest of the code that makes this step's facts, and what stamp() gives, now.
    py::str version() const;

  private:
    mutable std::optional<py::str> code_digest_;     // worked out once per process
    mutable std::mutex mu_;
};

// kind -> Step
py::dict<py::str, std::shared_ptr<Step>>& STEPS();

// How a platform's steps are declared (platforms.named(platform).steps imported), and which platforms there
// are (platforms.step_modules()). Unset: no platform has steps beyond the built-in two.
std::function<void(const py::str&)>& platform_loader();
std::function<py::list<py::str>()>& platform_names();

// Declare the function as the step that makes facts of `kind` (see the file's text). `version` fixes the
// version instead of the code's digest (tests).
std::shared_ptr<Step> step(const py::str& kind, StepFn fn, const py::list<py::str>& reads = {},
                           const py::str& platform = py::str(""), const py::list<py::str>& uses = {},
                           std::function<py::str()> stamp = nullptr, bool batch = false, bool always = false,
                           const std::optional<py::str>& version = std::nullopt,
                           const py::list<py::str>& code = {}, const py::str& doc = py::str(""));
std::shared_ptr<Step> batch_step(const py::str& kind, BatchFn fn, const py::list<py::str>& reads = {},
                                 const py::str& platform = py::str(""), const py::list<py::str>& uses = {},
                                 std::function<py::str()> stamp = nullptr, bool always = false,
                                 const std::optional<py::str>& version = std::nullopt,
                                 const py::list<py::str>& code = {}, const py::str& doc = py::str(""));

// {kind: Step} of a platform's steps (its module imported), or of every step known.
py::dict<py::str, std::shared_ptr<Step>> steps(const std::optional<py::str>& platform = std::nullopt);
void load(const py::str& platform);

// How many facts are made at once: the machine's processors, or fewer by the setting low_load.
int64_t jobs();

// One asking of the engine: each fact decided current or made again at most once in it -- the C++ core's.
std::shared_ptr<Run> make_run(const std::shared_ptr<native_engine::Database>& db,
                              std::function<void(const py::str&)> log = nullptr);

const Step& _step_of(const py::str& kind);

// The database of the binary at `path`, with the binary given as the fact `binary`.
std::shared_ptr<native_engine::Database> open(const py::str& path, const std::optional<py::str>& folder = std::nullopt);

// The fact, current (see the file's text).
Value get(const std::shared_ptr<native_engine::Database>& db, const py::str& kind,
          const py::str& key = py::str(""), std::function<void(const py::str&)> log = nullptr);
py::dict<py::str, Value> get_many(const std::shared_ptr<native_engine::Database>& db, const py::str& kind,
                                  const py::list<py::str>& keys, std::function<void(const py::str&)> log = nullptr);

// {kind: [kept, made]} of the last get on this database in this process.
py::dict<py::str, py::list<int64_t>> last_run(const native_engine::Database& db);

// A fact given by hand: what is made from it is stale when it differs from what was there.
py::str put(native_engine::Database& db, const py::str& kind, const py::str& key, const Value& value);

// Facts gone (one, or a kind): the next get makes them again.
int64_t forget(native_engine::Database& db, const py::str& kind, const std::optional<py::str>& key = std::nullopt);

// How a fact was made: {step, version, current version, made, took, inputs: [(kind, key, hash, same now?)]}
// -- read only; nothing is made.
struct Why {
    py::str kind, key, hash;
    int64_t size = 0;
    py::str step, version;
    double made = 0, took = 0;
    std::optional<py::str> version_now;
    py::list<std::tuple<py::str, py::str, py::str, bool>> inputs;
};
std::optional<Why> why(native_engine::Database& db, const py::str& kind, const py::str& key = py::str(""));

}  // namespace codebreaker::engine
