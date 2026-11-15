#include "cb/engine/engine.hpp"

#include <algorithm>
#include <chrono>
#include <format>
#include <set>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/text.hpp"

namespace cb::engine {

namespace {

double clock_now() {
    using namespace std::chrono;
    return duration_cast<duration<double>>(system_clock::now().time_since_epoch()).count();
}

// The path as the machine spells it: on Windows with backslashes (the binary fact has always been recorded that way), elsewhere
// as it is (a backslash there is a letter of a name, not a separator, and the lanes read this text back as a path).
std::string native_text(const std::filesystem::path& p) {
    std::error_code ec;
    const std::filesystem::path abs = std::filesystem::absolute(p, ec);
    std::string s = fs::to_text((ec ? p : abs).lexically_normal());
    if (std::filesystem::path::preferred_separator == '\\') std::replace(s.begin(), s.end(), '/', '\\');
    return s;
}

// The facts of kind "file": a file a step reads that is not a fact (a list kept in the source tree): its SHA-1.
StepResult file_fact(Context& ctx) {
    try {
        return Value(crypto::to_hex(crypto::sha1(fs::read_file(fs::from_text(ctx.key())))));
    } catch (const IoError&) {
        return Value(std::string("missing"));
    }
}

// Runs the locks of a set of facts for a scope.
class LockAll {
public:
    explicit LockAll(std::vector<std::recursive_mutex*> locks) : locks_(std::move(locks)) {
        for (std::recursive_mutex* m : locks_) m->lock();
    }
    ~LockAll() {
        for (std::recursive_mutex* m : locks_) m->unlock();
    }
    LockAll(const LockAll&) = delete;
    LockAll& operator=(const LockAll&) = delete;

private:
    std::vector<std::recursive_mutex*> locks_;
};

std::string piece_digest(knowledge::ReadDigests& digests, const std::string& text) {
    const auto read = knowledge::Read::parse(text);
    return read ? digests.digest(*read) : std::string(knowledge::kAbsent);
}

}  // namespace

// ---- Step ---------------------------------------------------------------------------------------------------------

bool Step::declares(std::string_view read_kind) const {
    return std::find(reads.begin(), reads.end(), read_kind) != reads.end();
}

std::string Step::version() const {
    if (fixed_version) return *fixed_version;
    std::string acc = kind;
    for (const std::string& c : code) acc += c;
    if (stamp) acc += stamp();
    return crypto::to_hex(crypto::sha1(to_bytes(acc))).substr(0, 16);
}

// ---- Steps --------------------------------------------------------------------------------------------------------

Steps::Steps() {
    Step binary;
    binary.kind = "binary";
    binary.fn = [](Context&) -> StepResult { throw UnknownStepError("binary (it is given when its database is opened)"); };
    binary.code = {"binary"};
    binary.doc = "The binary itself: its path (the hash is of its bytes). Given when the database is opened.";
    declare(std::move(binary));

    Step file;
    file.kind = "file";
    file.fn = file_fact;
    file.always = true;
    file.code = {"file"};
    file.doc = "A file a step reads that is not a fact (a list kept in the source tree): its SHA-1.";
    declare(std::move(file));
}

std::shared_ptr<const Step> Steps::declare(Step step) {
    auto shared = std::make_shared<const Step>(std::move(step));
    const std::lock_guard<std::mutex> g(mutex_);
    steps_[shared->kind] = shared;
    return shared;
}

void Steps::set_loader(Loader loader, std::vector<std::string> platforms) {
    const std::lock_guard<std::mutex> g(mutex_);
    loader_ = std::move(loader);
    platforms_ = std::move(platforms);
    loaded_.clear();
}

std::shared_ptr<const Step> Steps::find(std::string_view kind) const {
    const std::lock_guard<std::mutex> g(mutex_);
    const auto it = steps_.find(kind);
    return it == steps_.end() ? nullptr : it->second;
}

void Steps::load(std::string_view platform) {
    Loader loader;
    {
        const std::lock_guard<std::mutex> g(mutex_);
        if (!loader_ || std::find(loaded_.begin(), loaded_.end(), platform) != loaded_.end()) return;
        loaded_.emplace_back(platform);
        loader = loader_;
    }
    loader(*this, platform);   // (it declares steps: the lock is not held)
}

void Steps::load_all() {
    std::vector<std::string> platforms;
    {
        const std::lock_guard<std::mutex> g(mutex_);
        platforms = platforms_;
    }
    for (const std::string& p : platforms) load(p);
}

std::shared_ptr<const Step> Steps::of(std::string_view kind) {
    if (auto s = find(kind)) return s;
    load_all();   // (its platform's steps are not declared yet)
    if (auto s = find(kind)) return s;
    throw UnknownStepError(kind);
}

std::vector<std::shared_ptr<const Step>> Steps::list(std::optional<std::string_view> platform) {
    if (platform && !platform->empty()) load(*platform);
    else load_all();
    std::vector<std::shared_ptr<const Step>> out;
    const std::lock_guard<std::mutex> g(mutex_);
    for (const auto& [kind, s] : steps_)
        if (!platform || platform->empty() || s->platform == *platform || s->platform.empty()) out.push_back(s);
    return out;
}

// ---- Context ------------------------------------------------------------------------------------------------------

Context::Context(Run& run, std::shared_ptr<const Step> step, std::string key, std::any hint)
    : run_(run), step_(std::move(step)), key_(std::move(key)), hint_(std::move(hint)) {}

facts::Database& Context::db() const { return run_.db(); }

void Context::may(std::string_view kind) const {
    if (!step_->declares(kind) && kind != "binary" && kind != "file")
        throw UsageError(std::format("step {} reads {}, which it does not declare", step_->kind, kind));
}

void Context::record(std::string_view kind, std::string_view key) {
    Run::Done d;
    run_.done_of({std::string(kind), std::string(key)}, d);
    inputs_.push_back({std::string(kind), std::string(key), d.kept ? d.hash : std::string("unkept")});
}

Value Context::need(std::string_view kind, std::string_view key) {
    may(kind);
    run_.current(std::string(kind), std::string(key));
    record(kind, key);
    return run_.value(kind, key);
}

std::map<std::string, Value> Context::need_many(std::string_view kind, std::span<const std::string> keys_in, const std::any& hint) {
    may(kind);
    std::vector<std::string> keys;   // each once, in the order asked
    std::set<std::string> seen;
    for (const std::string& k : keys_in)
        if (seen.insert(k).second) keys.push_back(k);
    std::map<std::string, Value> out = run_.many(kind, keys, hint);
    for (const std::string& k : keys) record(kind, k);
    return out;
}

std::optional<Value> Context::peek(std::string_view kind, std::string_view key) {
    may(kind);
    const std::string keys[] = {std::string(key)};
    if (!run_.stale(kind, keys).empty()) return std::nullopt;
    Run::Done d;
    if (!run_.done_of({std::string(kind), std::string(key)}, d) || !d.kept) return std::nullopt;
    record(kind, key);
    return run_.value(kind, key);
}

Value Context::file(const std::filesystem::path& path) { return need("file", native_text(path)); }

void Context::knowledge(std::span<const std::string> reads) {
    std::set<std::string> have;
    for (const facts::Input& in : inputs_)
        if (in.kind == knowledge::kReadInputKind) have.insert(in.key);
    for (const std::string& read : reads) {
        if (!have.insert(read).second) continue;
        // (read now, not as the run first saw it: a piece the step itself learned is recorded as it now is)
        const std::string h = piece_digest(run_.engine().digests(), read);
        run_.set_done({std::string(knowledge::kReadInputKind), read}, {true, h});
        inputs_.push_back({std::string(knowledge::kReadInputKind), read, h});
    }
}

std::optional<Value> Context::previous() const {
    const auto row = run_.db().row(step_->kind, key_);
    if (!row) return std::nullopt;
    try {
        return row->value();
    } catch (const Error&) {   // a fact kept in a form no longer readable is no previous
        return std::nullopt;
    }
}

std::filesystem::path Context::work(const std::filesystem::path& sub) const { return facts::Databases::work_folder(run_.db(), sub); }

// ---- Run ----------------------------------------------------------------------------------------------------------

Run::Run(Engine& engine, std::shared_ptr<facts::Database> db, Log log)
    : engine_(engine), db_(std::move(db)), log_(std::move(log)), log_on_(static_cast<bool>(log_)) {}

std::recursive_mutex& Run::lock_for(const Key& key) {
    const std::lock_guard<std::recursive_mutex> g(mutex_);
    auto& slot = locks_[key];
    if (!slot) slot = std::make_unique<std::recursive_mutex>();
    return *slot;
}

void Run::count(const std::string& kind, bool made) {
    const std::lock_guard<std::recursive_mutex> g(mutex_);
    auto it = counts_.find(kind);
    if (it == counts_.end()) {
        count_order_.push_back(kind);
        it = counts_.emplace(kind, std::pair<std::size_t, std::size_t>{0, 0}).first;
    }
    (made ? it->second.second : it->second.first)++;
}

std::vector<Run::Count> Run::counts() const {
    auto& self = const_cast<Run&>(*this);   // (the lock is not part of the answer)
    const std::lock_guard<std::recursive_mutex> g(self.mutex_);
    std::vector<Count> out;
    for (const std::string& kind : count_order_) out.push_back({kind, counts_.at(kind).first, counts_.at(kind).second});
    return out;
}

bool Run::done_of(const Key& key, Done& out) {
    const std::lock_guard<std::recursive_mutex> g(mutex_);
    const auto it = done_.find(key);
    if (it == done_.end()) return false;
    out = it->second;
    return true;
}

void Run::set_done(const Key& key, const Done& done) {
    const std::lock_guard<std::recursive_mutex> g(mutex_);
    done_[key] = done;
}

void Run::set_value(const Key& key, Value value) {
    const std::lock_guard<std::recursive_mutex> g(mutex_);
    values_[key] = std::move(value);
}

std::optional<Value> Run::cached_value(const Key& key) {
    const std::lock_guard<std::recursive_mutex> g(mutex_);
    const auto it = values_.find(key);
    if (it == values_.end()) return std::nullopt;
    return it->second;
}

void Run::say(const std::string& text) {
    if (log_on_ && log_) log_(text);
}

std::string Run::version_of(const std::string& kind) {
    const std::lock_guard<std::recursive_mutex> g(mutex_);
    const auto it = versions_.find(kind);
    if (it != versions_.end()) return it->second;
    const std::string version = engine_.steps().of(kind)->version();
    versions_[kind] = version;
    db_->drop_other_versions(kind, version);
    return version;
}

Run::Done Run::current(const std::string& kind, const std::string& key) {
    const Key id{kind, key};
    Done d;
    if (done_of(id, d)) return d;
    if (kind == knowledge::kReadInputKind) {   // a piece of the knowledge base: its hash now, once a run
        const std::string h = piece_digest(engine_.digests(), key);
        const std::lock_guard<std::recursive_mutex> g(mutex_);
        const auto it = done_.find(id);
        if (it != done_.end()) return it->second;
        d = {true, h};
        done_[id] = d;
        return d;
    }
    const std::lock_guard<std::recursive_mutex> lock(lock_for(id));
    if (done_of(id, d)) return d;
    std::optional<facts::Row> row = db_->row(kind, key);
    if (row && still(*row)) {
        d = {true, row->hash};
        set_done(id, d);
        count(kind, false);
        return d;
    }
    std::map<std::string, std::optional<facts::Row>> rows;
    rows[key] = std::move(row);
    make(kind, {key}, &rows, {});
    done_of(id, d);
    return d;
}

bool Run::still(const facts::Row& row) {
    if (row.given()) return true;
    const auto step = engine_.steps().of(row.kind);
    if (step->always || row.version.empty() || row.version != version_of(row.kind)) return false;
    for (const facts::Input& in : row.inputs) {
        try {
            const Done d = current(in.kind, in.key);
            if (!d.kept || d.hash != in.hash) return false;
        } catch (const UnknownStepError&) {   // what it was made from is no longer made by anything
            return false;
        }
    }
    return true;
}

std::vector<std::string> Run::stale(std::string_view kind_in, std::span<const std::string> keys) {
    const std::string kind(kind_in);
    std::vector<std::string> out;
    Done d;
    for (const std::string& key : keys) {
        const Key id{kind, key};
        if (done_of(id, d)) continue;
        const std::lock_guard<std::recursive_mutex> lock(lock_for(id));
        if (done_of(id, d)) continue;
        const std::optional<facts::Row> row = db_->row(kind, key);
        if (row && still(*row)) {
            set_done(id, {true, row->hash});
            count(kind, false);
        } else {
            out.push_back(key);
        }
    }
    return out;
}

void Run::make(const std::string& kind, const std::vector<std::string>& keys,
               const std::map<std::string, std::optional<facts::Row>>* rows, const std::any& hint) {
    const std::shared_ptr<const Step> step = engine_.steps().of(kind);
    const std::string version = version_of(kind);
    std::vector<std::unique_ptr<Context>> contexts;
    for (const std::string& k : keys) contexts.push_back(std::make_unique<Context>(*this, step, k, hint));

    const double started = clock_now();
    // What the step asked of the knowledge base is inputs too (recorded on its thread, whichever it is).
    const auto recorded_reads = [](const knowledge::Recording& rec) {
        std::vector<std::string> out;
        for (const knowledge::Read& r : rec.reads()->sorted()) out.push_back(r.text());
        return out;
    };
    if (step->batch()) {
        std::vector<Context*> ptrs;
        for (auto& c : contexts) ptrs.push_back(c.get());
        std::map<std::string, StepResult> values;
        std::vector<std::string> reads;
        {
            const knowledge::Recording rec;
            values = step->batch_fn(ptrs);
            reads = recorded_reads(rec);
        }
        for (auto& c : contexts) c->knowledge(reads);
        std::vector<std::string> missing;
        for (auto& c : contexts) {
            const auto it = values.find(c->key());
            if (it == values.end()) missing.push_back(c->key());
            else c->result_ = std::move(it->second);
        }
        if (!missing.empty()) {
            std::string list;
            for (std::size_t i = 0; i < missing.size() && i < 4; ++i) list += (i ? ", " : "") + missing[i];
            throw InternalError(std::format("step {} made no {} for {}", kind, kind, list));
        }
    } else {
        for (auto& c : contexts) {
            std::vector<std::string> reads;
            {
                const knowledge::Recording rec;
                c->result_ = step->fn(*c);
                reads = recorded_reads(rec);
            }
            c->knowledge(reads);
        }
    }

    const double took = (clock_now() - started) / static_cast<double>(std::max<std::size_t>(1, keys.size()));
    for (auto& c : contexts) {
        const std::string& k = c->key();
        const Key id{kind, k};
        StepResult& result = *c->result_;
        if (auto* unkept = std::get_if<Unkept>(&result)) {
            db_->forget(kind, k);   // (what was kept before is no longer it)
            set_done(id, Done{});
            set_value(id, std::move(unkept->value));
            count(kind, true);
            continue;
        }
        Value& value = std::get<Value>(result);
        const facts::Encoded encoded = facts::encode(value);
        const std::string h = facts::hash_of(encoded);
        std::optional<facts::Row> row;
        if (rows && rows->contains(k)) row = rows->at(k);
        else row = db_->row(kind, k);
        if (row && row->hash == h && !row->given()) {
            db_->remake(kind, k, version, c->inputs());   // (the same as before: what was made from it holds)
        } else {
            db_->write(kind, k, encoded, kind, version, c->inputs(), took);
        }
        set_done(id, {true, h});
        set_value(id, std::move(value));
        count(kind, true);
    }
    const double elapsed = clock_now() - started;
    if (log_on_ && !keys.empty() && (keys.size() > 1 || elapsed > 1.0))
        say(std::format("  made {} {} ({:.1f}s)", keys.size(), kind, elapsed));
}

void Run::make_all(const std::string& kind, const std::vector<std::string>& todo, const std::any& hint) {
    const std::shared_ptr<const Step> step = engine_.steps().of(kind);
    std::vector<std::recursive_mutex*> locks;
    for (const std::string& k : todo) locks.push_back(&lock_for({kind, k}));
    if (step->batch()) {
        const LockAll held(locks);
        std::vector<std::string> left;
        Done d;
        for (const std::string& k : todo)
            if (!done_of({kind, k}, d)) left.push_back(k);
        if (!left.empty()) make(kind, left, nullptr, hint);
        return;
    }
    const auto one = [&](const std::string& k) {
        const std::lock_guard<std::recursive_mutex> lock(lock_for({kind, k}));
        Done d;
        if (!done_of({kind, k}, d)) make(kind, {k}, nullptr, hint);
    };
    const std::size_t n = std::min<std::size_t>(std::max<std::size_t>(engine_.jobs(), 1), todo.size());
    if (n <= 1) {
        for (const std::string& k : todo) one(k);
        return;
    }
    const double started = clock_now();
    const bool had = log_on_.exchange(false);   // (one line for the lot, not one each)
    try {
        parallel::ThreadPool pool(n);
        parallel::parallel_for(pool, todo.size(), [&](std::size_t i) { one(todo[i]); });
    } catch (...) {
        log_on_ = had;
        throw;   // (the first item's, once every item has ended)
    }
    log_on_ = had;
    if (log_on_) say(std::format("  made {} {}, {} at once ({:.1f}s)", todo.size(), kind, n, clock_now() - started));
}

std::map<std::string, Value> Run::many(std::string_view kind_in, std::span<const std::string> keys_in, const std::any& hint) {
    const std::string kind(kind_in);
    std::vector<std::string> keys;   // each once
    {
        std::set<std::string> seen;
        for (const std::string& k : keys_in)
            if (seen.insert(k).second) keys.push_back(k);
    }
    const std::vector<std::string> todo = stale(kind, keys);
    if (!todo.empty()) make_all(kind, todo, hint);
    std::map<std::string, Value> out;
    for (const std::string& k : keys) {
        current(kind, k);
        out.emplace(k, value(kind, k));
    }
    return out;
}

Value Run::value(std::string_view kind_in, std::string_view key_in) {
    const std::string kind(kind_in), key(key_in);
    current(kind, key);
    if (auto v = cached_value({kind, key})) return std::move(*v);
    const std::optional<facts::Row> row = db_->row(kind, key);
    if (!row) throw InternalError(std::format("the fact {}:{} is current but not kept", kind, key));
    Value v = row->value();
    set_value({kind, key}, v);
    return v;
}

std::optional<std::string> Run::hash_of(std::string_view kind, std::string_view key) {
    const Done d = current(std::string(kind), std::string(key));
    if (!d.kept) return std::nullopt;
    return d.hash;
}

// ---- Engine -------------------------------------------------------------------------------------------------------

Engine::Engine(Steps& steps, facts::Databases& databases, knowledge::ReadDigests& digests, std::size_t jobs)
    : steps_(steps), databases_(databases), digests_(digests), jobs_(std::max<std::size_t>(1, jobs)) {}

std::shared_ptr<facts::Database> Engine::open(const std::filesystem::path& path, const std::optional<std::filesystem::path>& folder) {
    auto db = databases_.open_for(path, folder);
    std::error_code ec;
    const auto size = static_cast<std::int64_t>(std::filesystem::file_size(path, ec));
    const std::string where = native_text(path);
    const std::string hash = "sha256:" + db->sha;

    bool current = false;
    if (const auto row = db->row("binary", ""); row && row->hash == hash) {
        try {
            const Value kept = row->value();   // {"path": ..., "size": ...}
            if (const auto* doc = std::get_if<knowledge::Value>(&kept)) {
                const auto* map = doc->map();
                const auto* p = doc->get("path");
                const auto* s = doc->get("size");
                current = map && map->size() == 2 && p && s && p->text() && *p->text() == where && s->integer() && *s->integer() == size;
            }
        } catch (const Error&) {   // a binary row in a form no longer readable is written again
        }
    }
    if (!current) {
        knowledge::Value::Map doc;
        doc.insert_or_assign("path", knowledge::Value(where));
        doc.insert_or_assign("size", knowledge::Value(size));
        const facts::Encoded e = facts::encode(Value(knowledge::Value(std::move(doc))));
        db->write_raw("binary", "", hash, e.tag, e.data, facts::kGiven, "", "[]", 0);
    }
    return db;
}

std::shared_ptr<Run> Engine::remember(const std::shared_ptr<facts::Database>& db, Run::Log log) {
    auto run = std::make_shared<Run>(*this, db, std::move(log));
    const std::lock_guard<std::mutex> g(runs_mutex_);
    runs_[fs::to_text(db->path())] = run;
    return run;
}

Value Engine::get(const std::shared_ptr<facts::Database>& db, std::string_view kind, std::string_view key, Run::Log log) {
    return remember(db, std::move(log))->value(kind, key);
}

std::map<std::string, Value> Engine::get_many(const std::shared_ptr<facts::Database>& db, std::string_view kind,
                                              std::span<const std::string> keys, Run::Log log) {
    return remember(db, std::move(log))->many(kind, keys);
}

std::vector<Run::Count> Engine::last_run(const facts::Database& db) const {
    std::shared_ptr<Run> run;
    {
        const std::lock_guard<std::mutex> g(runs_mutex_);
        const auto it = runs_.find(fs::to_text(db.path()));
        if (it != runs_.end()) run = it->second;
    }
    return run ? run->counts() : std::vector<Run::Count>{};
}

std::string Engine::put(facts::Database& db, std::string_view kind, std::string_view key, const Value& value) {
    return db.write(kind, key, value, facts::kGiven, "", {}, 0);
}

std::uint64_t Engine::forget(facts::Database& db, std::string_view kind, std::optional<std::string_view> key) {
    return db.forget(kind, key);
}

std::optional<Why> Engine::why(facts::Database& db, std::string_view kind, std::string_view key) {
    const auto row = db.row(kind, key);
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
    if (row->given()) {
        out.version_now = row->version;
    } else if (const auto step = steps_.find(kind)) {
        out.version_now = step->version();
    } else {
        try {
            out.version_now = steps_.of(kind)->version();
        } catch (const UnknownStepError&) {
            out.version_now = std::nullopt;
        }
    }
    for (const facts::Input& in : row->inputs) {
        if (in.kind == knowledge::kReadInputKind) {
            out.inputs.push_back({in, piece_digest(digests_, in.key) == in.hash});
            continue;
        }
        const auto now = db.row(in.kind, in.key);
        out.inputs.push_back({in, now && now->hash == in.hash});
    }
    return out;
}

}  // namespace cb::engine
