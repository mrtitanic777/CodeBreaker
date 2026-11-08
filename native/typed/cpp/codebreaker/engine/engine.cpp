// codebreaker/engine/engine.cpp -- typed C++ port of codebreaker.engine.engine (see engine.hpp).

#include "codebreaker/engine/engine.hpp"

#include <mutex>
#include <thread>

#include "codebreaker/base/parallel.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::engine {

namespace {

std::mutex& steps_lock() {
    static std::mutex m;
    return m;
}

// _RUNS: the last Run of each database path
std::mutex& runs_lock() {
    static std::mutex m;
    return m;
}
py::dict<py::str, std::shared_ptr<Run>>& runs() {
    static py::dict<py::str, std::shared_ptr<Run>> d;
    return d;
}

}  // namespace

// ---- Step ----
py::str Step::version() const {
    // A digest of the code that makes this step's facts: the step's own code, each module and function in
    // `uses`, and what stamp() gives, now.
    if (fixed_version) return *fixed_version;
    std::string acc;
    {
        std::lock_guard<std::mutex> g(mu_);
        if (!code_digest_) {
            std::string h = kind.utf8();
            for (const py::str& c : code) h += c.utf8();
            code_digest_ = py::str(h);
        }
        acc = code_digest_->utf8();
    }
    if (stamp) acc += py::repr(stamp()).utf8();
    py::str hex = py::hashlib::sha1(py::bytes(acc)).hexdigest();
    return hex.slice(0, 16);
}

py::dict<py::str, std::shared_ptr<Step>>& STEPS() {
    static py::dict<py::str, std::shared_ptr<Step>> d;
    return d;
}

std::function<void(const py::str&)>& platform_loader() {
    static std::function<void(const py::str&)> f;
    return f;
}
std::function<py::list<py::str>()>& platform_names() {
    static std::function<py::list<py::str>()> f;
    return f;
}

namespace {
py::list<py::str> step_modules() {
    // {platform -> the module whose steps it has} (imported when asked)
    if (platform_names()) return platform_names()();
    return py::list<py::str>();
}

std::shared_ptr<Step> declare(const std::shared_ptr<Step>& s) {
    {
        std::lock_guard<std::mutex> g(steps_lock());
        STEPS().set(s->kind, s);
    }
    native_engine::declared(*s);                        // (the C++ registry, once its engine is ready)
    return s;
}
}  // namespace

std::shared_ptr<Step> step(const py::str& kind, StepFn fn, const py::list<py::str>& reads, const py::str& platform,
                           const py::list<py::str>& uses, std::function<py::str()> stamp, bool batch, bool always,
                           const std::optional<py::str>& version, const py::list<py::str>& code, const py::str& doc) {
    auto s = std::make_shared<Step>();
    s->kind = kind;
    s->fn = std::move(fn);
    s->reads = reads;
    s->platform = platform;
    s->uses = uses;
    s->stamp = std::move(stamp);
    s->batch = batch;
    s->always = always;
    s->fixed_version = version;
    s->doc = doc;
    s->code = code;
    for (const py::str& u : uses) s->code.append(u);
    return declare(s);
}

std::shared_ptr<Step> batch_step(const py::str& kind, BatchFn fn, const py::list<py::str>& reads,
                                 const py::str& platform, const py::list<py::str>& uses,
                                 std::function<py::str()> stamp, bool always, const std::optional<py::str>& version,
                                 const py::list<py::str>& code, const py::str& doc) {
    auto s = std::make_shared<Step>();
    s->kind = kind;
    s->batch_fn = std::move(fn);
    s->reads = reads;
    s->platform = platform;
    s->uses = uses;
    s->stamp = std::move(stamp);
    s->batch = true;
    s->always = always;
    s->fixed_version = version;
    s->doc = doc;
    s->code = code;
    for (const py::str& u : uses) s->code.append(u);
    return declare(s);
}

void load(const py::str& platform) {
    if (platform_loader()) platform_loader()(platform);
}

py::dict<py::str, std::shared_ptr<Step>> steps(const std::optional<py::str>& platform) {
    if (platform && !platform->empty()) {
        load(*platform);
        py::dict<py::str, std::shared_ptr<Step>> d;
        for (const auto& kv : STEPS().items()) {
            const auto& s = std::get<1>(kv);
            if (s->platform == *platform || s->platform.empty()) d.set(std::get<0>(kv), s);
        }
        return d;
    }
    for (const py::str& p : step_modules()) load(p);
    return STEPS().copy();
}

int64_t jobs() {
    // How many facts are made at once: the machine's processors, or fewer by the setting low_load (a number;
    // or true: the machine's budget, base/parallel.workers()).
    int64_t cpus = static_cast<int64_t>(std::thread::hardware_concurrency());
    if (cpus <= 0) cpus = 4;
    py::settings::Value n = py::settings::get(py::str("low_load"));
    if (std::holds_alternative<std::monostate>(n)) return std::max<int64_t>(1, cpus);
    if (const bool* b = std::get_if<bool>(&n)) {
        if (!*b) return std::max<int64_t>(1, cpus);
        return base::parallel::workers();
    }
    if (const int64_t* i = std::get_if<int64_t>(&n)) {
        if (*i == 0) return std::max<int64_t>(1, cpus);
        return std::max<int64_t>(1, *i);
    }
    if (const double* d = std::get_if<double>(&n)) {
        if (*d == 0) return std::max<int64_t>(1, cpus);
        return std::max<int64_t>(1, static_cast<int64_t>(*d));
    }
    const py::str& s = std::get<py::str>(n);
    if (s == "" || s == "0" || s == "false" || s == "False") return std::max<int64_t>(1, cpus);
    py::str low = s.lower();
    if (low == "true" || low == "yes" || low == "on") return base::parallel::workers();
    try {
        return std::max<int64_t>(1, py::int_parse(s));
    } catch (const py::ValueError&) {
        return base::parallel::workers();
    }
}

// --- the facts every platform has ---
namespace {
// "binary": the binary itself: its path (the hash is of its bytes). Given when the database is opened.
// "file": a file a step reads that is not a fact (a list kept in the source tree): its SHA-1.
const bool builtin_steps_ = [] {
    step(
        py::str("binary"),
        [](Context&, const py::str&) -> StepResult {
            throw py::LookupError("the binary is given when its database is opened (engine.open)");
        },
        {}, py::str(""), {}, nullptr, false, false, std::nullopt, py::list<py::str>{py::str("binary")},
        py::str("The binary itself: its path (the hash is of its bytes). Given when the database is opened."));
    step(
        py::str("file"),
        [](Context&, const py::str& key) -> StepResult {
            try {
                py::bytes data = py::open_rb(key).read();
                return Value(py::str(py::hashlib::sha1(data).hexdigest()));
            } catch (const py::OSError&) {
                return Value(py::str("missing"));
            }
        },
        {}, py::str(""), {}, nullptr, false, true, std::nullopt, py::list<py::str>{py::str("file")},
        py::str("A file a step reads that is not a fact (a list kept in the source tree): its SHA-1."));
    return true;
}();
}  // namespace

// --- a run ---
std::shared_ptr<Run> make_run(const std::shared_ptr<native_engine::Database>& db, std::function<void(const py::str&)> log) {
    return std::make_shared<Run>(db, std::move(log));
}

const Step& _step_of(const py::str& kind) {
    std::shared_ptr<Step> found;
    {
        std::lock_guard<std::mutex> g(steps_lock());
        auto f = STEPS().get(kind);
        if (f) found = f;
    }
    if (!found) {
        for (const py::str& p : step_modules()) load(p);
        std::lock_guard<std::mutex> g(steps_lock());
        auto f = STEPS().get(kind);
        if (f) found = f;
    }
    if (!found) throw py::LookupError("no step makes facts of the kind " + py::repr(kind).utf8());
    return *found;
}

// --- asking ---
namespace {
py::str binary_hash(const native_engine::Database& db) { return py::str("sha256:") + *db.sha; }

// The binary's fact: its value where it is, its hash its bytes' (moving it changes nothing).
void give_binary(native_engine::Database& db, const facts::Json& value) {
    auto [tag, data] = facts::encode(Value(value));
    db.write_raw(py::str("binary"), py::str(""), binary_hash(db), tag, data, facts::GIVEN, py::str(""), py::str("[]"), 0);
}
}  // namespace

std::shared_ptr<native_engine::Database> open(const py::str& path, const std::optional<py::str>& folder) {
    auto db = facts::open_for(path, folder);
    auto row = db->row(py::str("binary"), py::str(""));
    facts::Json value;
    value.kind = facts::Json::Dict;
    facts::Json p;
    p.kind = facts::Json::Str;
    p.s = py::os::path::abspath(path);
    facts::Json sz;
    sz.kind = facts::Json::Int;
    sz.i = py::os::path::getsize(path);
    value.dict.set(py::str("path"), p);
    value.dict.set(py::str("size"), sz);
    bool current = false;
    if (row && row->hash == binary_hash(*db)) {
        Value v = row->value();
        const facts::Json* j = std::get_if<facts::Json>(&v);
        // row.value() == {"path": ..., "size": ...}
        if (j && j->kind == facts::Json::Dict && j->dict.size() == 2 && j->dict.contains(py::str("path")) &&
            j->dict.contains(py::str("size"))) {
            const facts::Json& jp = j->dict.at(py::str("path"));
            const facts::Json& js = j->dict.at(py::str("size"));
            current = jp.kind == facts::Json::Str && jp.s == p.s && js.kind == facts::Json::Int && js.i == sz.i;
        }
    }
    if (!current) give_binary(*db, value);
    return db;
}

Value get(const std::shared_ptr<native_engine::Database>& db, const py::str& kind, const py::str& key,
          std::function<void(const py::str&)> log) {
    auto r = make_run(db, std::move(log));
    {
        std::lock_guard<std::mutex> g(runs_lock());
        runs().set(db->path, r);
    }
    return r->value(kind, key);
}

py::dict<py::str, Value> get_many(const std::shared_ptr<native_engine::Database>& db, const py::str& kind,
                                  const py::list<py::str>& keys, std::function<void(const py::str&)> log) {
    auto r = make_run(db, std::move(log));
    {
        std::lock_guard<std::mutex> g(runs_lock());
        runs().set(db->path, r);
    }
    return r->many(kind, keys);
}

py::dict<py::str, py::list<int64_t>> last_run(const native_engine::Database& db) {
    std::shared_ptr<Run> r;
    {
        std::lock_guard<std::mutex> g(runs_lock());
        auto f = runs().get(db.path);
        if (f) r = f;
    }
    py::dict<py::str, py::list<int64_t>> out;
    if (!r) return out;
    for (const auto& kv : r->counts().items()) {
        py::list<int64_t> copy;
        for (int64_t x : std::get<1>(kv)) copy.append(x);
        out.set(std::get<0>(kv), copy);
    }
    return out;
}

py::str put(native_engine::Database& db, const py::str& kind, const py::str& key, const Value& value) {
    return db.write(kind, key, value, facts::GIVEN);
}

int64_t forget(native_engine::Database& db, const py::str& kind, const std::optional<py::str>& key) {
    return db.forget(kind, key);
}

std::optional<Why> why(native_engine::Database& db, const py::str& kind, const py::str& key) {
    auto row = db.row(kind, key);
    if (!row) return std::nullopt;
    Why out;
    out.kind = kind;
    out.key = key;
    out.hash = row->hash;
    out.size = row->size;
    out.step = row->step;
    out.version = row->version;
    out.made = row->made;
    out.took = row->took;
    try {
        out.version_now = row->given() ? row->version : _step_of(kind).version();
    } catch (const py::LookupError&) {
        out.version_now = std::nullopt;
    }
    for (const auto& in : row->inputs) {
        const py::str& k = std::get<0>(in);
        const py::str& key2 = std::get<1>(in);
        const py::str& h = std::get<2>(in);
        if (k == knowledge::reads::KIND) {
            out.inputs.append({k, key2, h, knowledge::reads::digest(key2) == h});
            continue;
        }
        auto now = db.row(k, key2);
        out.inputs.append({k, key2, h, now && now->hash == h});
    }
    return out;
}

}  // namespace codebreaker::engine
