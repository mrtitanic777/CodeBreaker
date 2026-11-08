// codebreaker/engine/native_engine.cpp -- typed C++ port of codebreaker.engine.native_engine (see
// native_engine.hpp). native/engine/*.cc is linked in; the cbe_* functions of cbengine.h are called
// directly, as the Python calls them through ctypes.

#ifndef CBE_BUILD
#define CBE_BUILD
#endif
#include "codebreaker/engine/native_engine.hpp"

#include <atomic>
#include <cstdio>
#include <cstring>
#include <map>

#include "cbengine.h"                               // native/engine
#include "codebreaker/engine/engine.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/store.hpp"
#include "codebreaker/store/functions.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::engine::native_engine {

// ---- loading ----
py::str dll_path() { return py::os::path::join(py::settings::get_str(py::str("engine_bin")), py::str("cbengine.dll")); }
bool enabled() { return true; }
std::optional<py::str> why_not() { return std::nullopt; }

namespace {

// ---- helpers ----
std::string B(const py::str& s) { return s.utf8(); }                 // s.encode("utf-8", "surrogatepass")
const char* P(const std::string& s) { return s.c_str(); }

py::bytes to_bytes(const char* p, int64_t n) { return n > 0 ? py::bytes(p, static_cast<size_t>(n)) : py::bytes(); }

// ---- errors ----
// number -> an exception a callback raised, until it is raised again at the call that started the run
std::mutex g_errs_lock;
std::map<int64_t, std::exception_ptr> g_errs;
std::atomic<int64_t> g_next_err{1};

int64_t keep(std::exception_ptr e, bool lookup) {
    std::lock_guard<std::mutex> g(g_errs_lock);
    int64_t n = g_next_err++;
    g_errs[n] = e;
    while (g_errs.size() > 512) g_errs.erase(g_errs.begin());          // (ones the C++ caught and let go)
    return (n << 1) | (lookup ? 1 : 0);
}

[[noreturn]] void fail(int rc) {
    std::string msg = cbe_error() ? cbe_error() : "";
    if (rc == 3) {
        std::exception_ptr e;
        {
            std::lock_guard<std::mutex> g(g_errs_lock);
            auto it = g_errs.find(cbe_error_py());
            if (it != g_errs.end()) {
                e = it->second;
                g_errs.erase(it);
            }
        }
        if (e) std::rethrow_exception(e);
        throw py::RuntimeError("an exception in a Python step, lost: " + msg);
    }
    if (rc == 1) throw py::LookupError(msg);
    if (rc == 5) throw py::KeyError(msg);
    if (rc == 6) throw py::ValueError(msg);
    if (rc == 4) {
        if (cbe_error_code() == 19) throw sqlite3_IntegrityError(msg);
        throw sqlite3_OperationalError(msg);
    }
    if (rc == 7) {
        int code = cbe_error_code();
        if (code == 2 || code == 3) throw py::FileNotFoundError(msg);
        throw py::OSError(msg);
    }
    throw py::RuntimeError(msg);
}

void call(int rc) {
    if (rc) fail(rc);
}

// A call that hands back a buffer: -> its bytes.
template <class F>
py::bytes out(F&& f) {
    char* p = nullptr;
    int64_t n = 0;
    int rc = f(&p, &n);
    if (rc) fail(rc);
    py::bytes b = to_bytes(p, n);
    cbe_free(p);
    return b;
}

// ---- the callbacks ----
std::mutex g_lock;                                            // the tables below
std::map<void*, Run*> g_runs;                                 // the C++ run's address -> its Run here
struct StoreEntry {
    FunctionStore* store = nullptr;
    std::function<void(const py::str&)> log;
};
std::map<void*, StoreEntry> g_stores;                         // the C++ store's address -> (store, its carry's log)
std::map<int64_t, std::any> g_hints;                          // number -> a hint handed to a step (need_many's)
std::map<int64_t, std::shared_ptr<knowledge::reads::Reads>> g_contexts;   // number -> the asker's context
std::atomic<int64_t> g_ids{1};

void put_sink(void* sink, const py::str& text) {
    std::string b = B(text);
    cbe_sink_put(sink, b.data(), static_cast<int64_t>(b.size()));
}

template <class F>
int64_t guarded(F&& f) {
    try {
        f();
        return 0;
    } catch (const py::LookupError&) {
        return keep(std::current_exception(), true);
    } catch (...) {
        return keep(std::current_exception(), false);
    }
}

int64_t on_step(const char* kind) {
    return guarded([&] {
        py::str k(kind);
        auto found = STEPS().get(k);
        if (!found) _step_of(k);
        else declared(*found);
    });
}

int64_t on_version(const char* kind, void* sink) {
    return guarded([&] { put_sink(sink, _step_of(py::str(kind)).version()); });
}

int64_t on_digest(const char* read, void* sink) {
    return guarded([&] { put_sink(sink, knowledge::reads::digest(py::str(read))); });
}

void on_log(void* who, const char* text_in) {
    try {
        py::str text(text_in);
        Run* run = nullptr;
        StoreEntry entry;
        bool has_store = false;
        {
            std::lock_guard<std::mutex> g(g_lock);
            auto r = g_runs.find(who);
            if (r != g_runs.end()) run = r->second;
            auto s = g_stores.find(who);
            if (s != g_stores.end()) {
                entry = s->second;
                has_store = true;
            }
        }
        if (run) {
            if (run->log) run->log(text);
            return;
        }
        if (has_store && entry.log) {
            entry.log(text.slice_from(1));
        } else if (text.slice(0, 1) == "1") {
            std::string line = text.slice_from(1).utf8() + "\n";
            std::fwrite(line.data(), 1, line.size(), stderr);
        }
    } catch (...) {
    }
}

int64_t on_make(void* run_p, const char* kind_in, int32_t n, void** ctxs, int64_t hint, int64_t context,
                int32_t batch) {
    return guarded([&] {
        Run* run = nullptr;
        {
            std::lock_guard<std::mutex> g(g_lock);
            auto it = g_runs.find(run_p);
            if (it == g_runs.end()) throw py::KeyError("run");
            run = it->second;
        }
        py::str kind(kind_in);
        std::vector<void*> ptrs(ctxs, ctxs + n);
        std::any h;
        if (hint) {
            std::lock_guard<std::mutex> g(g_lock);
            auto it = g_hints.find(hint);
            if (it != g_hints.end()) h = it->second;
        }
        auto body = [&] {
            const Step& s = _step_of(kind);
            std::vector<std::unique_ptr<Context>> cs;
            for (void* p : ptrs) cs.push_back(std::make_unique<Context>(*run, s, p, h));
            if (batch) {
                py::dict<py::str, Context*> by_key;
                for (auto& c : cs) by_key.set(c->key, c.get());
                py::dict<py::str, StepResult> values;
                knowledge::reads::Reads* rr = nullptr;
                {
                    knowledge::reads::recording r;                 // (what the knowledge base was asked: inputs too)
                    values = s.batch_fn(by_key, h);
                    rr = nullptr;
                    py::list<py::str> keys = r.sorted_keys();
                    for (auto& c : cs) c->knowledge(keys);
                }
                (void)rr;
                for (auto& c : cs) {
                    auto v = values.get(c->key);
                    if (v) {
                        // c._give(values[c.key])
                        const StepResult& res = *v;
                        py::str k = c->key;
                        if (const Unkept* u = std::get_if<Unkept>(&res)) {
                            run->set_value(s.kind, k, u->value);
                            call(cbe_ctx_unkept(c->p));
                        } else {
                            const Value& val = std::get<Value>(res);
                            auto [tag, data] = facts::encode(val);
                            run->set_value(s.kind, k, val);
                            std::string t = B(tag);
                            call(cbe_ctx_result(c->p, t.c_str(), data.raw().data(), data.size()));
                        }
                    }
                }
            } else {
                Context& c = *cs[0];
                StepResult v;
                py::list<py::str> keys;
                {
                    knowledge::reads::recording r;
                    v = s.fn(c, c.key);
                    keys = r.sorted_keys();
                }
                c.knowledge(keys);
                if (const Unkept* u = std::get_if<Unkept>(&v)) {
                    run->set_value(s.kind, c.key, u->value);
                    call(cbe_ctx_unkept(c.p));
                } else {
                    const Value& val = std::get<Value>(v);
                    auto [tag, data] = facts::encode(val);
                    run->set_value(s.kind, c.key, val);
                    std::string t = B(tag);
                    call(cbe_ctx_result(c.p, t.c_str(), data.raw().data(), data.size()));
                }
            }
        };
        if (context) {
            std::shared_ptr<knowledge::reads::Reads> asker;
            {
                std::lock_guard<std::mutex> g(g_lock);
                asker = g_contexts.at(context);
            }
            knowledge::reads::Adopt adopt(asker);               // _CONTEXTS[context].copy().run(body)
            body();
        } else {
            body();
        }
    });
}

bool g_ready = false;
std::once_flag g_once;

// _load(): the callbacks, the step registry (steps declared before the engine was ready)
void ready() {
    std::call_once(g_once, [] {
        if (cbe_abi() != 1) throw py::OSError("cbengine.dll is another version: rebuild native/engine");
        cbe_set_callbacks(on_step, on_version, on_make, on_digest, on_log);
        g_ready = true;
        for (const auto& kv : STEPS().items()) declared(*std::get<1>(kv));
    });
}

// the DLL, or the reason it cannot be had, raised
void need() { ready(); }

py::bytes pack_strings(const py::list<py::str>& xs) {
    Cells c;
    for (const py::str& x : xs) c.emplace_back(x);
    return pack(c);
}

py::bytes pack_inputs(const py::list<facts::Input>& inputs) {
    Cells c;
    for (const auto& i : inputs) {
        c.emplace_back(std::get<0>(i));
        c.emplace_back(std::get<1>(i));
        c.emplace_back(std::get<2>(i));
    }
    return pack(c);
}

}  // namespace

// ---- row buffers ----
py::bytes pack(const Cells& values) {
    std::string o;
    for (const Cell& v : values) {
        if (std::holds_alternative<std::monostate>(v)) {
            o.push_back(0);
        } else if (const int64_t* i = std::get_if<int64_t>(&v)) {
            o.push_back(1);
            o.append(reinterpret_cast<const char*>(i), 8);
        } else if (const double* d = std::get_if<double>(&v)) {
            o.push_back(2);
            o.append(reinterpret_cast<const char*>(d), 8);
        } else if (const py::str* s = std::get_if<py::str>(&v)) {
            std::string b = s->utf8();
            int64_t n = static_cast<int64_t>(b.size());
            o.push_back(3);
            o.append(reinterpret_cast<const char*>(&n), 8);
            o += b;
        } else {
            const std::string& b = std::get<py::bytes>(v).raw();
            int64_t n = static_cast<int64_t>(b.size());
            o.push_back(4);
            o.append(reinterpret_cast<const char*>(&n), 8);
            o += b;
        }
    }
    return py::bytes(o);
}

Cells unpack(const py::bytes& data) {
    Cells out_;
    const std::string& d = data.raw();
    size_t i = 0, n = d.size();
    while (i < n) {
        int t = static_cast<unsigned char>(d[i]);
        i += 1;
        if (t == 0) {
            out_.emplace_back();
        } else if (t == 1) {
            int64_t v;
            std::memcpy(&v, d.data() + i, 8);
            out_.emplace_back(v);
            i += 8;
        } else if (t == 2) {
            double v;
            std::memcpy(&v, d.data() + i, 8);
            out_.emplace_back(v);
            i += 8;
        } else {
            int64_t k;
            std::memcpy(&k, d.data() + i, 8);
            i += 8;
            std::string b = d.substr(i, static_cast<size_t>(k));
            i += static_cast<size_t>(k);
            if (t == 3) out_.emplace_back(py::str(b));
            else out_.emplace_back(py::bytes(b));
        }
    }
    return out_;
}

std::vector<Cells> _rows(const py::bytes& data) {
    Cells f = unpack(data);
    std::vector<Cells> rows;
    if (f.empty()) return rows;
    int64_t n = cell_int(f[0]);
    if (!n) return rows;
    for (size_t i = 1; i < f.size(); i += static_cast<size_t>(n)) {
        rows.emplace_back(f.begin() + i, f.begin() + std::min(f.size(), i + static_cast<size_t>(n)));
    }
    return rows;
}

py::str cell_str(const Cell& c) {
    if (const py::str* s = std::get_if<py::str>(&c)) return *s;
    if (const py::bytes* b = std::get_if<py::bytes>(&c)) return b->decode("utf-8", "surrogatepass");
    if (const int64_t* i = std::get_if<int64_t>(&c)) return py::str(std::to_string(*i));
    if (const double* d = std::get_if<double>(&c)) return py::repr(*d);
    return py::str("");
}
std::optional<py::str> cell_opt_str(const Cell& c) {
    if (std::holds_alternative<std::monostate>(c)) return std::nullopt;
    return cell_str(c);
}
int64_t cell_int(const Cell& c) {
    if (const int64_t* i = std::get_if<int64_t>(&c)) return *i;
    if (const double* d = std::get_if<double>(&c)) return static_cast<int64_t>(*d);
    return 0;
}
double cell_double(const Cell& c) {
    if (const double* d = std::get_if<double>(&c)) return *d;
    if (const int64_t* i = std::get_if<int64_t>(&c)) return static_cast<double>(*i);
    return 0.0;
}
py::bytes cell_bytes(const Cell& c) {
    if (const py::bytes* b = std::get_if<py::bytes>(&c)) return *b;
    if (const py::str* s = std::get_if<py::str>(&c)) return s->encode("utf-8", "surrogatepass");
    return py::bytes();
}

// ---- the program database ----
bool Cleaned::contains(const py::str& step) const {
    int got = 0;
    call(cbe_db_cleaned(db_.handle(), 0, P(B(step)), &got));
    return got != 0;
}
void Cleaned::add(const py::str& step) {
    int got = 0;
    call(cbe_db_cleaned(db_.handle(), 1, P(B(step)), &got));
}
void Cleaned::clear() {
    int got = 0;
    call(cbe_db_cleaned(db_.handle(), 2, nullptr, &got));
}

Database::Database(const py::str& path_) : path(path_), cleaned(*this) {
    need();
    void* h = nullptr;
    call(cbe_db_open(P(B(path_)), &h));
    h_ = h;
}

Database::~Database() {
    if (h_) cbe_db_release(h_);
}

namespace {
// facts.Row from the twelve fields a row buffer holds: kind, key, hash, tag, value, blob, size, step, version,
// inputs, made, took
std::unique_ptr<facts::Row> make_row(Database* db, const Cells& f, size_t at) {
    std::optional<py::bytes> value;
    if (!std::holds_alternative<std::monostate>(f[at + 4])) value = cell_bytes(f[at + 4]);
    return std::make_unique<facts::Row>(db, cell_str(f[at]), cell_str(f[at + 1]), cell_str(f[at + 2]),
                                        cell_str(f[at + 3]), value, cell_opt_str(f[at + 5]), cell_int(f[at + 6]),
                                        cell_str(f[at + 7]), cell_str(f[at + 8]), cell_opt_str(f[at + 9]),
                                        cell_double(f[at + 10]), cell_double(f[at + 11]));
}
}  // namespace

std::unique_ptr<facts::Row> Database::row(const py::str& kind, const py::str& key) const {
    py::bytes data = out([&](char** p, int64_t* n) { return cbe_db_row(h_, P(B(kind)), P(B(key)), p, n); });
    if (data.empty()) return nullptr;
    Cells f = unpack(data);
    return make_row(const_cast<Database*>(this), f, 0);
}

py::list<facts::Row> Database::rows(const std::optional<py::str>& kind) const {
    std::string k = kind ? B(*kind) : std::string();
    py::bytes data = out([&](char** p, int64_t* n) { return cbe_db_rows(h_, kind && !kind->empty() ? k.c_str() : nullptr, p, n); });
    Cells f = unpack(data);
    py::list<facts::Row> rs;
    for (size_t i = 0; i + 12 <= f.size(); i += 12) rs.append(*make_row(const_cast<Database*>(this), f, i));
    return rs;
}

std::vector<Cells> Database::kinds() const {
    py::bytes data = out([&](char** p, int64_t* n) { return cbe_db_kinds(h_, p, n); });
    Cells f = unpack(data);
    std::vector<Cells> rs;
    for (size_t i = 0; i + 4 <= f.size(); i += 4) rs.emplace_back(f.begin() + i, f.begin() + i + 4);
    return rs;
}

py::bytes Database::blob(const py::str& hash) const {
    return out([&](char** p, int64_t* n) { return cbe_db_blob(h_, P(B(hash)), p, n); });
}

std::optional<py::str> Database::about(const py::str& name) const {
    char* p = nullptr;
    int64_t n = 0;
    int found = 0;
    int rc = cbe_db_about(h_, P(B(name)), &p, &n, &found);
    if (rc) fail(rc);
    std::optional<py::str> r;
    if (found) r = py::str(std::string(p, static_cast<size_t>(n)));
    cbe_free(p);
    return r;
}

py::str Database::about(const py::str& name, const py::str& value) const {
    call(cbe_db_set_about(h_, P(B(name)), P(B(value))));
    return value;
}

py::str Database::write(const py::str& kind, const py::str& key, const facts::Value& value, const py::str& step,
                        const py::str& version, const py::list<facts::Input>& inputs, double took,
                        const std::optional<std::tuple<py::str, py::bytes>>& encoded) const {
    auto [tag, data] = encoded ? *encoded : facts::encode(value);
    py::bytes ins = pack_inputs(inputs);
    char hash[64 + 1] = {0};
    call(cbe_db_write(h_, P(B(kind)), P(B(key)), P(B(tag)), data.raw().data(), data.size(), P(B(step)),
                      P(B(version)), ins.raw().data(), ins.size(), took, hash));
    return py::str(std::string(hash));
}

void Database::write_raw(const py::str& kind, const py::str& key, const py::str& hash, const py::str& tag,
                         const py::bytes& data, const py::str& step, const py::str& version,
                         const py::str& inputs_json, double took) const {
    call(cbe_db_write_raw(h_, P(B(kind)), P(B(key)), P(B(hash)), P(B(tag)), data.raw().data(), data.size(),
                          P(B(step)), P(B(version)), P(B(inputs_json)), took));
}

void Database::remake(const py::str& kind, const py::str& key, const py::str& version,
                      const py::list<facts::Input>& inputs) const {
    py::bytes ins = pack_inputs(inputs);
    call(cbe_db_remake(h_, P(B(kind)), P(B(key)), P(B(version)), ins.raw().data(), ins.size()));
}

int64_t Database::forget(const py::str& kind, const std::optional<py::str>& key) const {
    int64_t n = 0;
    std::string k = key ? B(*key) : std::string();
    call(cbe_db_forget(h_, P(B(kind)), key ? k.c_str() : nullptr, &n));
    return n;
}

int64_t Database::drop_other_versions(const py::str& step, const py::str& version) const {
    int64_t n = 0;
    call(cbe_db_drop_other_versions(h_, P(B(step)), P(B(version)), &n));
    return n;
}

void Database::collect() const { call(cbe_db_collect(h_)); }

void Database::close() {
    facts::forget_open(path);
    if (h_) {
        cbe_db_close(h_);
        cbe_db_release(h_);
        h_ = nullptr;
    }
}

py::str sha(const py::str& path) {
    need();
    char o[65] = {0};
    call(cbe_sha(P(B(path)), o));
    return py::str(std::string(o));
}

py::list<std::tuple<py::str, int64_t, double>> databases(const py::str& folder) {
    need();
    py::bytes data = out([&](char** p, int64_t* n) { return cbe_databases(P(B(folder)), p, n); });
    Cells f = unpack(data);
    py::list<std::tuple<py::str, int64_t, double>> rs;
    for (size_t i = 0; i + 3 <= f.size(); i += 3) rs.append({cell_str(f[i]), cell_int(f[i + 1]), cell_double(f[i + 2])});
    return rs;
}

py::list<py::str> hold(int64_t cap, const py::str& folder) {
    need();
    py::bytes data = out([&](char** p, int64_t* n) { return cbe_hold(cap, P(B(folder)), p, n); });
    py::list<py::str> gone;
    for (const Cell& c : unpack(data)) gone.append(cell_str(c));
    return gone;
}

// ---- a run and the context a step is given ----
namespace {
// A hint and the asker's context, numbered for the C++ run while a call is in it.
class Handed {
  public:
    explicit Handed(const std::any& hint) {
        h = hint.has_value() ? g_ids++ : 0;
        c = g_ids++;
        std::lock_guard<std::mutex> g(g_lock);
        if (h) g_hints[h] = hint;
        g_contexts[c] = knowledge::reads::capture();
    }
    ~Handed() {
        std::lock_guard<std::mutex> g(g_lock);
        g_hints.erase(h);
        g_contexts.erase(c);
    }
    int64_t h = 0, c = 0;
};
}  // namespace

Run::Run(std::shared_ptr<Database> db_, bool log_on) : db(std::move(db_)) {
    need();
    void* p = nullptr;
    call(cbe_run_new(db->handle(), log_on ? 1 : 0, &p));
    p_ = p;
    std::lock_guard<std::mutex> g(g_lock);
    g_runs[p_] = this;
}

Run::Run(std::shared_ptr<Database> db_, std::function<void(const py::str&)> log_) : db(std::move(db_)), log(log_) {
    need();
    void* p = nullptr;
    call(cbe_run_new(db->handle(), log ? 1 : 0, &p));
    p_ = p;
    std::lock_guard<std::mutex> g(g_lock);
    g_runs[p_] = this;
}

Run::~Run() {
    if (p_) {
        {
            std::lock_guard<std::mutex> g(g_lock);
            g_runs.erase(p_);
        }
        cbe_run_free(p_);
    }
}

py::dict<py::str, py::list<int64_t>> Run::counts() const {
    py::bytes data = out([&](char** p, int64_t* n) { return cbe_run_counts(p_, p, n); });
    Cells f = unpack(data);
    py::dict<py::str, py::list<int64_t>> d;
    for (size_t i = 0; i + 3 <= f.size(); i += 3) d.set(cell_str(f[i]), py::list<int64_t>{cell_int(f[i + 1]), cell_int(f[i + 2])});
    return d;
}

std::optional<py::str> Run::hash_of(const py::str& kind, const py::str& key) {
    int kept = 0;
    char h[128 + 1] = {0};
    call(cbe_run_current(p_, P(B(kind)), P(B(key)), &kept, h));
    if (!kept) return std::nullopt;
    return py::str(std::string(h));
}

facts::Value Run::value(const py::str& kind, const py::str& key) {
    hash_of(kind, key);
    return _value(kind, key);
}

facts::Value Run::_value(const py::str& kind, const py::str& key) {
    {
        std::lock_guard<std::recursive_mutex> g(mu_);
        auto got = values_.get(std::make_tuple(kind, key));
        if (got) return *got;
    }
    auto row = db->row(kind, key);
    if (!row) throw py::AttributeError("'NoneType' object has no attribute 'value'");
    facts::Value v = row->value();
    std::lock_guard<std::recursive_mutex> g(mu_);
    values_.set(std::make_tuple(kind, key), v);
    return v;
}

void Run::set_value(const py::str& kind, const py::str& key, const facts::Value& v) {
    std::lock_guard<std::recursive_mutex> g(mu_);
    values_.set(std::make_tuple(kind, key), v);
}

py::list<py::str> Run::stale(const py::str& kind, const py::list<py::str>& keys) {
    py::bytes b = pack_strings(keys);
    py::bytes data = out([&](char** p, int64_t* n) { return cbe_run_stale(p_, P(B(kind)), b.raw().data(), b.size(), p, n); });
    py::list<py::str> r;
    for (const Cell& c : unpack(data)) r.append(cell_str(c));
    return r;
}

py::dict<py::str, facts::Value> Run::many(const py::str& kind, const py::list<py::str>& keys_in, const std::any& hint) {
    py::list<py::str> keys;                                      // list(dict.fromkeys(keys))
    py::set<py::str> seen;
    for (const py::str& k : keys_in) {
        if (!seen.contains(k)) {
            seen.add(k);
            keys.append(k);
        }
    }
    py::bytes b = pack_strings(keys);
    {
        Handed handed(hint);
        call(cbe_run_many(p_, P(B(kind)), b.raw().data(), b.size(), handed.h, jobs(), handed.c));
    }
    py::dict<py::str, facts::Value> d;
    for (const py::str& k : keys) d.set(k, _value(kind, k));
    return d;
}

Context::Context(Run& run_, const Step& step_, void* p_, const std::any& hint_)
    : run(run_), step(step_), p(p_), hint(hint_), db(run_.db) {
    py::bytes k = out([&](char** q, int64_t* n) { return cbe_ctx_key(p_, q, n); });
    key = k.decode("utf-8", "surrogatepass");
}

py::list<facts::Input> Context::inputs() const {
    py::bytes data = out([&](char** q, int64_t* n) { return cbe_ctx_inputs(p, q, n); });
    Cells f = unpack(data);
    py::list<facts::Input> r;
    for (size_t i = 0; i + 3 <= f.size(); i += 3) r.append(facts::Input{cell_str(f[i]), cell_str(f[i + 1]), cell_str(f[i + 2])});
    return r;
}

facts::Value Context::need(const py::str& kind, const py::str& key_) {
    call(cbe_ctx_need(p, P(B(kind)), P(B(key_))));
    return run._value(kind, key_);
}

py::dict<py::str, facts::Value> Context::need_many(const py::str& kind, const py::list<py::str>& keys_in,
                                                    const std::any& hint_) {
    py::list<py::str> keys;
    py::set<py::str> seen;
    for (const py::str& k : keys_in) {
        if (!seen.contains(k)) {
            seen.add(k);
            keys.append(k);
        }
    }
    py::bytes b = pack_strings(keys);
    {
        Handed handed(hint_);
        call(cbe_ctx_need_many(p, P(B(kind)), b.raw().data(), b.size(), handed.h, jobs(), handed.c));
    }
    py::dict<py::str, facts::Value> d;
    for (const py::str& k : keys) d.set(k, run._value(kind, k));
    return d;
}

std::optional<facts::Value> Context::peek(const py::str& kind, const py::str& key_) {
    int found = 0;
    call(cbe_ctx_peek(p, P(B(kind)), P(B(key_)), &found));
    if (!found) return std::nullopt;
    return run._value(kind, key_);
}

facts::Value Context::file(const py::str& path) { return need(py::str("file"), py::os::path::abspath(path)); }

void Context::knowledge(const py::list<py::str>& keys) {
    py::bytes b = pack_strings(keys);
    call(cbe_ctx_knowledge(p, b.raw().data(), b.size()));
}

std::optional<facts::Value> Context::previous() const {
    auto row = db->row(step.kind, key);
    try {
        if (row) return row->value();
        return std::nullopt;
    } catch (const py::Exception&) {
        return std::nullopt;
    }
}

py::str Context::work(const py::list<py::str>& parts) const { return facts::work(*db, parts); }

void declared(const Step& s) {
    if (!g_ready) return;                       // (the registry, once the engine is ready)
    py::bytes reads = pack_strings(s.reads);
    call(cbe_step_register(P(B(s.kind)), P(B(s.platform)), reads.raw().data(), reads.size(), s.batch ? 1 : 0,
                           s.always ? 1 : 0));
}

// ---- the function store ----
namespace {
const char* const kRow[] = {"addr", "engine", "rev", "sig", "name", "source", "problem", "verdict", "reason",
                            "limits", "detail", "judged_sig"};
Cell at(const Cells& r, size_t i) { return i < r.size() ? r[i] : Cell(); }

Candidate candidate_of(const Cells& r) {
    Candidate c;
    c.addr = at(r, 0); c.engine = at(r, 1); c.rev = at(r, 2); c.sig = at(r, 3); c.name = at(r, 4);
    c.source = at(r, 5); c.problem = at(r, 6); c.verdict = at(r, 7); c.reason = at(r, 8); c.limits = at(r, 9);
    c.detail = at(r, 10); c.judged_sig = at(r, 11);
    return c;
}
Variant variant_of(const Cells& r) {
    Variant v;
    v.addr = at(r, 0); v.compiler = at(r, 1); v.source = at(r, 2); v.parent = at(r, 3); v.knobs = at(r, 4);
    v.distance = at(r, 5); v.total = at(r, 6); v.verdict = at(r, 7); v.behaves = at(r, 8);
    return v;
}
}  // namespace

FunctionStore::FunctionStore(const std::optional<py::str>& path_, bool readonly_) : readonly(readonly_) {
    need();
    auto env = py::os::environ_::get(py::str("CB_FUNCTIONS_DB"));
    own_path = (path_ && !path_->empty()) || (env && !env->empty());
    path = (path_ && !path_->empty()) ? *path_ : store::functions::default_path();
    folder = store::functions::databases_folder(path, own_path);
    void* p = nullptr;
    call(cbe_store_open(P(B(path)), P(B(py::os::path::abspath(folder))), readonly ? 1 : 0, &p));
    p_ = p;
    {
        std::lock_guard<std::mutex> g(g_lock);
        g_stores[p_] = StoreEntry{this, nullptr};
    }
    if (!readonly) carry();
}

FunctionStore::~FunctionStore() { close(); }

std::vector<Cells> FunctionStore::_c(const py::str& method, const Cells& args) {
    py::bytes b = pack(args);
    py::bytes data = out([&](char** q, int64_t* n) {
        return cbe_store_call(p_, P(B(method)), b.raw().data(), b.size(), q, n);
    });
    return _rows(data);
}

void FunctionStore::close() {
    if (p_) {
        {
            std::lock_guard<std::mutex> g(g_lock);
            g_stores.erase(p_);
        }
        cbe_store_free(p_);
        p_ = nullptr;
    }
}

py::str FunctionStore::database_path(const py::str& module) const {
    return py::os::path::join(folder, module + ".sqlite");
}

std::optional<py::str> FunctionStore::module_db(const py::str& module) {
    py::str p = database_path(module);
    if (readonly) return py::os::path::isfile(p) ? std::optional<py::str>(p) : std::nullopt;
    _c(py::str("open_module"), {module});                       // (its tables made, as _m does)
    return p;
}

py::dict<py::str, Summary> FunctionStore::summaries() {
    py::dict<py::str, Summary> out_;
    for (const Cells& r : _c(py::str("summaries"), {})) {
        Summary s;
        s.functions = cell_int(r[1]);
        s.identified = cell_int(r[2]);
        s.judged = cell_int(r[3]);
        s.last = r[4];
        s.last_profile = r[5];
        py::str profiles = std::holds_alternative<std::monostate>(r[6]) || cell_str(r[6]).empty() ? py::str("[]") : cell_str(r[6]);
        for (const auto& x : facts::json_loads(profiles).list) s.profiles.append(x.s);
        py::str latest = std::holds_alternative<std::monostate>(r[7]) || cell_str(r[7]).empty() ? py::str("{}") : cell_str(r[7]);
        for (const auto& kv : facts::json_loads(latest).dict.items()) s.latest.set(std::get<0>(kv), std::get<1>(kv).i);
        out_.set(cell_str(r[0]), s);
    }
    return out_;
}

// ---- writing
void FunctionStore::add_module(const py::str& sha_, const py::str& name, const py::str& path_, const py::str& kind) {
    _c(py::str("add_module"), {sha_, name.lower(), path_, kind});
}
void FunctionStore::add_profile(const py::str& digest, const py::str& settings) {
    _c(py::str("add_profile"), {digest, settings});
}
void FunctionStore::add_functions(const py::str& module, const py::list<py::str>& addrs) {
    Cells a{module};
    for (const py::str& x : addrs) a.emplace_back(x);
    _c(py::str("add_functions"), a);
}
void FunctionStore::record_many(const py::str& module, const py::str& engine_, const Cell& rev,
                                const py::str& profile, const py::list<RecordRow>& rows) {
    Cells a{module, engine_, rev, profile};
    for (const RecordRow& r : rows) {
        a.emplace_back(r.addr);
        a.push_back(r.name);
        a.push_back(r.source);
        a.push_back(r.sig);
        a.emplace_back(r.verdict);
        a.push_back(r.reason);
        a.push_back(r.limits);
        a.push_back(r.detail);
        a.push_back(r.problem);
    }
    _c(py::str("record_many"), a);
}
void FunctionStore::put_text(const py::str& module, const py::str& engine_, const py::str& kind, const py::str& body) {
    _c(py::str("put_text"), {module, engine_, kind, body});
}
Cell FunctionStore::get_text(const py::str& module, const py::str& engine_, const py::str& kind) {
    return _c(py::str("get_text"), {module, engine_, kind}).at(0).at(0);
}
void FunctionStore::record(const py::str& module, const py::str& addr, const py::str& engine_, const Cell& rev,
                           const Cell& sig, const Cell& name, const Cell& source, const py::str& profile,
                           const py::str& verdict, const Cell& reason, const Cell& limits, const Cell& detail,
                           const Cell& problem) {
    py::list<RecordRow> rows;
    rows.append(RecordRow{addr, name, source, sig, verdict, reason, limits, detail, problem});
    record_many(module, engine_, rev, profile, rows);
}

// ---- reading
py::list<Candidate> FunctionStore::candidates(const py::str& module, const py::str& profile,
                                              const std::optional<py::str>& engine_) {
    Cell e;
    if (engine_) e = *engine_;
    py::list<Candidate> r;
    for (const Cells& x : _c(py::str("candidates"), {module, profile, e})) r.append(candidate_of(x));
    return r;
}

py::list<Stored> FunctionStore::stored(const py::str& module, const py::str& addr) {
    py::list<Stored> out_;
    std::map<int64_t, size_t> where;                 // idx -> its place in out_
    for (const Cells& r : _c(py::str("stored"), {module, addr})) {
        int64_t idx = cell_int(r[0]);
        if (!where.count(idx)) {
            Stored s;
            s.engine = r[1]; s.rev = r[2]; s.name = r[3]; s.source = r[4]; s.problem = r[5];
            where[idx] = static_cast<size_t>(out_.size());
            out_.append(s);
        }
        bool has = cell_int(r[6]) != 0;
        if (has) {
            Verdict v;
            v.profile = r[7]; v.verdict = r[8]; v.reason = r[9]; v.limits = r[10];
            v.carried = cell_int(r[11]) != 0;
            out_.vec()[where[idx]].verdicts.append(v);
        }
    }
    return out_;
}

py::dict<py::str, Cell> FunctionStore::built(const py::str& module, const py::str& profile) {
    py::dict<py::str, Cell> d;
    for (const Cells& r : _c(py::str("built"), {module, profile})) d.set(cell_str(r[0]), r[1]);
    return d;
}

void FunctionStore::set_built(const py::str& module, const py::str& profile, const py::dict<py::str, py::str>& sigs) {
    Cells a{module, profile};
    for (const auto& kv : sigs.items()) {
        a.emplace_back(std::get<0>(kv));
        a.emplace_back(std::get<1>(kv));
    }
    _c(py::str("set_built"), a);
}

py::dict<py::str, Candidate> FunctionStore::judged(const py::str& module, const py::str& profile, const py::str& engine_) {
    py::dict<py::str, Candidate> d;
    for (const Cells& x : _c(py::str("judged"), {module, profile, engine_})) {
        Candidate c = candidate_of(x);
        d.set(cell_str(c.addr), c);
    }
    return d;
}

py::dict<py::str, Candidate> FunctionStore::best(const py::str& module, const py::str& profile,
                                                 const py::list<py::str>& prefer) {
    Cells a{module, profile};
    for (const py::str& x : prefer) a.emplace_back(x);
    py::dict<py::str, Candidate> d;
    for (const Cells& x : _c(py::str("best"), a)) {
        Candidate c = candidate_of(x);
        c.klass = x.at(12);
        py::str weak = cell_str(x.at(13));
        if (!weak.empty()) c.weak = py::split(weak, py::str(","));
        d.set(cell_str(c.addr), c);
    }
    return d;
}

py::dict<py::str, Cell> FunctionStore::latest(const py::str& module) {
    py::dict<py::str, Cell> d;
    for (const Cells& r : _c(py::str("latest"), {module})) d.set(cell_str(r[0]), r[1]);
    return d;
}

py::list<py::str> FunctionStore::addresses(const py::str& module) {
    py::list<py::str> r;
    for (const Cells& x : _c(py::str("addresses"), {module})) r.append(cell_str(x[0]));
    return r;
}

py::dict<py::str, Cell> FunctionStore::tally(const py::str& profile, const py::str& module,
                                             const py::list<py::str>& prefer) {
    Cells a{profile, module};
    for (const py::str& x : prefer) a.emplace_back(x);
    py::dict<py::str, Cell> d;
    for (const Cells& r : _c(py::str("tally"), a)) d.set(cell_str(r[0]), r[1]);
    return d;
}

void FunctionStore::set_identified(const py::str& module, const py::dict<py::str, std::tuple<py::str, py::str>>& found) {
    Cells a{module};
    for (const auto& kv : found.items()) {
        a.emplace_back(std::get<0>(kv));
        a.emplace_back(std::get<0>(std::get<1>(kv)));
        a.emplace_back(std::get<1>(std::get<1>(kv)));
    }
    _c(py::str("set_identified"), a);
}

py::dict<py::str, std::tuple<Cell, Cell>> FunctionStore::identified(const py::str& module) {
    py::dict<py::str, std::tuple<Cell, Cell>> d;
    for (const Cells& r : _c(py::str("identified"), {module})) d.set(cell_str(r[0]), std::make_tuple(r[1], r[2]));
    return d;
}

py::dict<py::str, Cell> FunctionStore::scores(const py::str& profile, const py::str& module, const py::list<py::str>& prefer) {
    Cells a{profile, module};
    for (const py::str& x : prefer) a.emplace_back(x);
    py::dict<py::str, Cell> d;
    for (const Cells& r : _c(py::str("scores"), a)) d.set(cell_str(r[0]), r[1]);
    return d;
}

// ---- bodies and their names: the knowledge base's
std::shared_ptr<KnowledgeBase> FunctionStore::knowledge() {
    if (!knowledge_) {
        auto kb = knowledge::store::open(own_path ? std::optional<py::str>(knowledge::store::beside(path)) : std::nullopt);
        kb->carry_bodies(path, py::str("function store ") + py::os::path::normcase(py::os::path::abspath(path)));
        knowledge_ = kb;
    }
    return knowledge_;
}

void FunctionStore::set_bodies(const py::str& module, const py::dict<py::str, std::tuple<py::str, Cell, Cell>>& rows) {
    for (const Cells& r : module_rows()) {
        if (cell_str(r[0]) == module) {
            knowledge()->add_module(module, cell_str(r[1]), cell_str(r[3]));
            break;
        }
    }
    knowledge()->set_bodies(module, rows);
}
py::dict<py::str, std::tuple<py::str, Cell, Cell>> FunctionStore::bodies(const py::str& module) { return knowledge()->bodies(module); }
void FunctionStore::name_body(const py::str& digest, const py::str& name, const py::str& source) {
    knowledge()->name_body(digest, name, source);
}
py::dict<py::str, std::tuple<Cell, Cell>> FunctionStore::names_from_bodies(const py::str& module) {
    return knowledge()->names_from_bodies(module);
}
Cells FunctionStore::shared_bodies() { return knowledge()->shared_bodies(); }
Cell FunctionStore::modules_with_body(const py::str& digest) { return knowledge()->modules_with_body(digest); }

// ---- byte-match variants
Cell FunctionStore::record_variant(const py::str& module, const py::str& addr, const py::str& compiler,
                                   const py::str& source, const Cell& distance, const Cell& total,
                                   const py::str& verdict, const py::str& knobs, const Cell& parent,
                                   const py::str& behaves) {
    return _c(py::str("record_variant"), {module, addr, compiler, source, distance, total, verdict, knobs, parent, behaves})
        .at(0).at(0);
}
py::list<Variant> FunctionStore::variants(const py::str& module, const std::optional<py::str>& compiler) {
    Cell c;
    if (compiler) c = *compiler;
    py::list<Variant> r;
    for (const Cells& x : _c(py::str("variants"), {module, c})) r.append(variant_of(x));
    return r;
}
py::dict<py::str, Variant> FunctionStore::best_variant(const py::str& module, const py::str& compiler) {
    py::dict<py::str, Variant> d;
    for (const Cells& x : _c(py::str("best_variant"), {module, compiler})) d.set(cell_str(x[0]), variant_of(x));
    return d;
}
Cells FunctionStore::match_tally(const py::str& module, const py::str& compiler) {
    return _c(py::str("match_tally"), {module, compiler}).at(0);
}

// ---- across modules: the index
std::vector<Cells> FunctionStore::modules(const std::optional<py::str>& profile) {
    Cell c;
    if (profile) c = *profile;
    return _c(py::str("modules"), {c});
}
std::vector<Cells> FunctionStore::module_rows(const std::optional<py::str>& kind) {
    Cell c;
    if (kind) c = *kind;
    return _c(py::str("module_rows"), {c});
}
std::optional<py::str> FunctionStore::module_path(const py::str& name) {
    auto got = _c(py::str("module_path"), {name});
    if (got.empty()) return std::nullopt;
    return cell_opt_str(got[0][0]);
}
py::dict<py::str, std::tuple<int64_t, int64_t, Cell>> FunctionStore::stamps() {
    py::dict<py::str, std::tuple<int64_t, int64_t, Cell>> d;
    for (const auto& kv : summaries().items()) {
        const Summary& s = std::get<1>(kv);
        if (s.functions) d.set(std::get<0>(kv), std::make_tuple(s.functions, s.judged, s.last));
    }
    return d;
}
std::optional<py::str> FunctionStore::latest_profile(const py::str& kind) {
    auto got = _c(py::str("latest_profile"), {kind});
    if (got.empty()) return std::nullopt;
    return cell_opt_str(got[0][0]);
}
std::vector<Cells> FunctionStore::recent(int64_t limit) { return _c(py::str("recent"), {limit}); }
Cell FunctionStore::verdicts_since(double t) { return _c(py::str("verdicts_since"), {t}).at(0).at(0); }
py::list<Overview> FunctionStore::overview() {
    py::list<Overview> r;
    for (const Cells& x : _c(py::str("overview"), {})) {
        Overview o;
        o.sha = cell_str(x[0]); o.name = cell_str(x[1]); o.kind = cell_str(x[2]); o.path = cell_str(x[3]);
        o.functions = cell_int(x[4]);
        o.identified = cell_int(x[5]);
        for (const auto& kv : facts::json_loads(cell_str(x[6])).dict.items()) o.latest.set(std::get<0>(kv), std::get<1>(kv).i);
        r.append(o);
    }
    return r;
}

// ---- the store from before the databases
std::optional<py::str> FunctionStore::_about(const py::str& name) {
    auto got = _c(py::str("about"), {name});
    if (got.empty()) return std::nullopt;
    return cell_opt_str(got[0][0]);
}
void FunctionStore::_about(const py::str& name, const py::str& value) { _c(py::str("set_about"), {name, value}); }

py::dict<py::str, Cell> FunctionStore::carry(const std::function<void(const py::str&)>& log) {
    py::dict<py::str, Cell> d;
    if (readonly) return d;
    {
        std::lock_guard<std::mutex> g(g_lock);
        g_stores[p_] = StoreEntry{this, log};
    }
    struct Reset {
        FunctionStore* self;
        ~Reset() {
            std::lock_guard<std::mutex> g(g_lock);
            g_stores[self->p_] = StoreEntry{self, nullptr};
        }
    } reset{this};
    for (const Cells& r : _c(py::str("carry"), {})) d.set(cell_str(r[0]), r[1]);
    return d;
}

// ---- the knowledge base ----
KnowledgeBase::KnowledgeBase(const py::str& path_) : path(path_) {
    need();
    void* p = nullptr;
    call(cbe_kb_open(P(B(path_)), &p));
    p_ = p;
}

KnowledgeBase::~KnowledgeBase() {
    if (p_) cbe_kb_release(p_);
}

std::vector<Cells> KnowledgeBase::_c(const py::str& method, const Cells& args) {
    py::bytes b = pack(args);
    py::bytes data = out([&](char** q, int64_t* n) {
        return cbe_kb_call(p_, P(B(method)), b.raw().data(), b.size(), q, n);
    });
    return _rows(data);
}

void KnowledgeBase::close() {
    if (p_) {
        call(cbe_kb_close(p_));
        cbe_kb_release(p_);
        p_ = nullptr;
    }
}

std::optional<facts::Value> KnowledgeBase::get(const py::str& platform, const py::str& kind, const py::str& key) {
    {
        std::lock_guard<std::recursive_mutex> g(mu_);
        auto t = tables_.get(std::make_tuple(platform, kind));
        if (t) {
            auto v = t->get(key);
            return v;
        }
    }
    auto got = _c(py::str("get"), {platform, kind, key});
    if (got.empty()) return std::nullopt;
    return facts::decode(cell_str(got[0][0]), cell_bytes(got[0][1]));
}

bool KnowledgeBase::has(const py::str& platform, const py::str& kind, const py::str& key) {
    {
        std::lock_guard<std::recursive_mutex> g(mu_);
        auto t = tables_.get(std::make_tuple(platform, kind));
        if (t) return t->contains(key);
    }
    return cell_int(_c(py::str("has"), {platform, kind, key}).at(0).at(0)) != 0;
}

void KnowledgeBase::put(const py::str& platform, const py::str& kind, const py::str& key, const facts::Value& value,
                        const py::str& source, const py::str& origin) {
    py::dict<py::str, facts::Value> one;
    one.set(key, value);
    put_many(platform, kind, one, source, origin);
}

void KnowledgeBase::put_many(const py::str& platform, const py::str& kind, const py::dict<py::str, facts::Value>& values,
                             const py::str& source, const py::str& origin) {
    Cells flat{platform, kind, source, origin};
    for (const auto& kv : values.items()) {
        auto [tag, data] = facts::encode(std::get<1>(kv));
        flat.emplace_back(std::get<0>(kv));
        flat.emplace_back(tag);
        flat.emplace_back(data);
    }
    _c(py::str("put_many"), flat);
    std::lock_guard<std::recursive_mutex> g(mu_);
    auto t = tables_.get(std::make_tuple(platform, kind));
    if (t) t->update(values);
}

void KnowledgeBase::forget(const py::str& platform, const py::str& kind, const py::str& key) {
    _c(py::str("forget"), {platform, kind, key});
    std::lock_guard<std::recursive_mutex> g(mu_);
    auto t = tables_.get(std::make_tuple(platform, kind));
    if (t) t->pop(key, facts::Value());
}

std::optional<Cells> KnowledgeBase::about(const py::str& platform, const py::str& kind, const py::str& key) {
    auto got = _c(py::str("about"), {platform, kind, key});
    if (got.empty()) return std::nullopt;
    return got[0];
}

py::dict<py::str, facts::Value> KnowledgeBase::table(const py::str& platform, const py::str& kind) {
    {
        std::lock_guard<std::recursive_mutex> g(mu_);
        auto t = tables_.get(std::make_tuple(platform, kind));
        if (t) return *t;
    }
    py::dict<py::str, facts::Value> t;
    for (const Cells& r : _c(py::str("table"), {platform, kind})) t.set(cell_str(r[0]), facts::decode(cell_str(r[1]), cell_bytes(r[2])));
    std::lock_guard<std::recursive_mutex> g(mu_);
    tables_.set(std::make_tuple(platform, kind), t);
    return t;
}

py::dict<py::str, Cell> KnowledgeBase::hashes(const py::str& platform, const py::str& kind) {
    py::dict<py::str, Cell> d;
    for (const Cells& r : _c(py::str("hashes"), {platform, kind})) d.set(cell_str(r[0]), r[1]);
    return d;
}
std::optional<py::str> KnowledgeBase::hash_of(const py::str& platform, const py::str& kind, const py::str& key) {
    auto got = _c(py::str("hash_of"), {platform, kind, key});
    if (got.empty()) return std::nullopt;
    return cell_opt_str(got[0][0]);
}
std::optional<py::str> KnowledgeBase::kind_digest(const py::str& platform, const py::str& kind) {
    auto got = _c(py::str("kind_digest"), {platform, kind});
    if (got.empty()) return std::nullopt;
    return cell_opt_str(got[0][0]);
}
py::str KnowledgeBase::digest(const py::str& read) { return cell_str(_c(py::str("digest"), {read}).at(0).at(0)); }
Cell KnowledgeBase::stamp(const py::str& platform, const py::str& kind) {
    auto got = _c(py::str("stamp"), {platform, kind});
    if (got.empty()) return Cell();
    return got[0][0];
}

py::dict<py::str, facts::Value> KnowledgeBase::ensure_values(
    const py::str& platform, const py::str& kind, const py::str& stamp_,
    const std::function<py::dict<py::str, facts::Value>()>& make, const py::str& source, const py::str& origin) {
    {
        std::lock_guard<std::recursive_mutex> g(mu_);
        auto e = ensured_.get(std::make_tuple(platform, kind));
        if (e && *e == stamp_) return table(platform, kind);
    }
    Cell have = stamp(platform, kind);
    bool same = std::holds_alternative<py::str>(have) && std::get<py::str>(have) == stamp_;
    if (!same) {
        py::dict<py::str, facts::Value> values = make();
        Cells flat{platform, kind, stamp_, source, origin};
        for (const auto& kv : values.items()) {
            auto [tag, data] = facts::encode(std::get<1>(kv));
            flat.emplace_back(std::get<0>(kv));
            flat.emplace_back(tag);
            flat.emplace_back(data);
        }
        _c(py::str("replace_kind"), flat);
        std::lock_guard<std::recursive_mutex> g(mu_);
        tables_.pop(std::make_tuple(platform, kind), py::dict<py::str, facts::Value>());
    }
    {
        std::lock_guard<std::recursive_mutex> g(mu_);
        ensured_.set(std::make_tuple(platform, kind), stamp_);
    }
    return table(platform, kind);
}

knowledge::sources::Rows KnowledgeBase::ensure(const py::str& platform, const py::str& kind, const py::str& stamp_,
                                               const std::function<knowledge::sources::Rows()>& make,
                                               const py::str& source) {
    auto made = [&]() {
        py::dict<py::str, facts::Value> v;
        for (const auto& kv : make().items()) v.set(std::get<0>(kv), facts::Value(std::get<1>(kv)));
        return v;
    };
    knowledge::sources::Rows rows;
    for (const auto& kv : ensure_values(platform, kind, stamp_, made, source).items()) {
        const facts::Value& v = std::get<1>(kv);
        knowledge::sources::Json j;
        if (const auto* js = std::get_if<knowledge::sources::Json>(&v)) {
            j = *js;
        } else if (const py::str* s = std::get_if<py::str>(&v)) {
            j.kind = knowledge::sources::Json::Str;
            j.s = *s;
        }
        rows.set(std::get<0>(kv), j);
    }
    return rows;
}

void KnowledgeBase::forget_ensured(const py::str& platform, const py::str& kind) {
    std::lock_guard<std::recursive_mutex> g(mu_);
    ensured_.pop(std::make_tuple(platform, kind), py::str(""));
}

std::vector<Cells> KnowledgeBase::kinds(const std::optional<py::str>& platform) {
    Cell c;
    if (platform) c = *platform;
    return _c(py::str("kinds"), {c});
}

Cell KnowledgeBase::carried(const py::str& what, const std::optional<py::str>& mark) {
    auto got = mark ? _c(py::str("carried"), {what, *mark}) : _c(py::str("carried"), {what});
    if (got.empty()) return Cell();
    return got[0][0];
}

void KnowledgeBase::_read(const py::str& kind, const py::str& key) {
    if (knowledge::reads::active() &&
        py::os::path::normcase(path) == py::os::path::normcase(py::os::path::abspath(knowledge::store::default_path()))) {
        knowledge::reads::record(py::str("bodies"), kind, key);
    }
}

void KnowledgeBase::add_module(const py::str& sha_, const py::str& name, const py::str& platform) {
    _c(py::str("add_module"), {sha_, name, platform});
}

void KnowledgeBase::set_bodies(const py::str& module, const py::dict<py::str, std::tuple<py::str, Cell, Cell>>& rows) {
    Cells flat{module};
    for (const auto& kv : rows.items()) {
        const auto& t = std::get<1>(kv);
        flat.emplace_back(std::get<0>(kv));
        flat.emplace_back(std::get<0>(t));
        flat.push_back(std::get<1>(t));
        flat.push_back(std::get<2>(t));
    }
    _c(py::str("set_bodies"), flat);
}

py::dict<py::str, std::tuple<py::str, Cell, Cell>> KnowledgeBase::bodies(const py::str& module) {
    py::dict<py::str, std::tuple<py::str, Cell, Cell>> d;
    for (const Cells& r : _c(py::str("bodies"), {module})) d.set(cell_str(r[0]), std::make_tuple(cell_str(r[1]), r[2], r[3]));
    return d;
}

void KnowledgeBase::name_body(const py::str& body_id, const py::str& name_, const py::str& source) {
    _c(py::str("name_body"), {body_id, name_, source});
}

std::optional<Cells> KnowledgeBase::name(const py::str& body_id, bool record) {
    if (record) _read(py::str("name"), body_id);
    auto got = _c(py::str("name"), {body_id});
    if (got.empty()) return std::nullopt;
    return got[0];
}

py::dict<py::str, std::tuple<Cell, Cell>> KnowledgeBase::names_from_bodies(const py::str& module, bool record) {
    if (record) _read(py::str("names"), module);
    py::dict<py::str, std::tuple<Cell, Cell>> d;
    for (const Cells& r : _c(py::str("names_from_bodies"), {module})) d.set(cell_str(r[0]), std::make_tuple(r[1], r[2]));
    return d;
}

Cells KnowledgeBase::shared_bodies() { return _c(py::str("shared_bodies"), {}).at(0); }
Cell KnowledgeBase::modules_with_body(const py::str& digest_) { return _c(py::str("modules_with_body"), {digest_}).at(0).at(0); }

void KnowledgeBase::carry_bodies(const py::str& store_db, const py::str& label) {
    _c(py::str("carry_bodies"), {store_db, label});
}

// ---- the verdict vocabulary ----
Cells verdicts_call(const py::str& method, const Cells& args) {
    need();
    py::bytes b = pack(args);
    return unpack(out([&](char** q, int64_t* n) {
        return cbe_verdicts_call(P(B(method)), b.raw().data(), b.size(), q, n);
    }));
}

}  // namespace codebreaker::engine::native_engine
