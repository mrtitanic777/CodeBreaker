// codebreaker/base/cache.hpp -- COMPAT SHIM over cb/base/cache.hpp (docs/CPP_DESIGN.md 6.3). Deleted with its last py:: consumer.
//
// base/cache is refactored: the cache lives in native/cb/base/cache.hpp (a Cache object, a versioned C++ file format,
// typed codecs) and the SQLite wrapper in native/cb/base/sqlite.hpp. This header keeps the old names and signatures so
// the ports that have not been refactored yet (build/*, decompile/*, engine/*, exact/*, java/*, port/*, verify/*,
// analyze/facts ...) keep compiling: each function converts its py:: arguments, calls the new one, and converts back.
//
// What a caller sees that is different, on purpose (Logan, 2026-11-11: the Python's on-disk formats are dropped):
//   * the files are cb's own (state/cache/<kind>/<generation>.cbc); a cache the Python wrote is not read, it is rebuilt;
//   * entries carry an envelope, so get()/put() are raw bytes as before but only of what this side put;
//   * value<T>/keep<T> below still write the pickles the old callers' own `pickle::Codec` specialisations describe --
//     those four callers (verify/compare, verify/lanes/win16, verify/authored/match, port/verify) define codecs against
//     the pickle node tree, so the pickle reader and writer stay here, inline and unchanged, until they are refactored
//     onto cb::base::cache::Codec. Nothing else uses them.
//   * Db (a Python-sqlite3 look-alike used by analyze/facts) is a shim over cb::base::sqlite::Database.
//   * `_db()` is gone (nothing used it); sizes() reports the state folder only.
#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <set>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "cb/base/cache.hpp"
#include "cb/base/sqlite.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::base::cache {

namespace newc = ::cb::base::cache;

// --- constants ----------------------------------------------------------------------------------
// The version of the code that makes each kind: the module, and the name in it.
inline py::dict<py::str, std::tuple<py::str, py::str>> GENERATIONS = {
    {py::str("fingerprints"), {py::str("codebreaker.verify.compare"), py::str("REV")}},
    {py::str("masm"), {py::str("codebreaker.build.masm"), py::str("CACHE_VER")}},
    {py::str("decompiled"), {py::str("codebreaker.decompile.decompiler"), py::str("generation")}},
    {py::str("decompiled_fn"), {py::str("codebreaker.decompile.decompiler"), py::str("generation")}},
    {py::str("gate16_tables"), {py::str("codebreaker.decompile.decompiler"), py::str("generation")}},
    {py::str("compilable16"), {py::str("codebreaker.decompile.c_text"), py::str("generation")}}};
// ... and, in C++, what reading that value gives: repr(value) as Python writes it.
using GenerationRepr = std::function<std::optional<py::str>()>;
inline py::dict<py::str, GenerationRepr> GENERATION_REPRS;
inline const int64_t DEFAULT_MAX_GB = 2;
// The kinds every state folder and checkout shares (settings.build_cache).
inline const py::set<py::str> SHARED = {py::str("wasm16"), py::str("objects"),     py::str("never_set"),
                                        py::str("masm"),   py::str("assembled16"), py::str("omf_publics")};
inline const int64_t DEFAULT_SHARED_MAX_GB = 6;
inline int64_t CHECK_EVERY = 32 << 20;     // (kept for callers that read it; the new cache's constant is fixed)
inline int64_t TOUCH = 3600;
inline const py::str DROPPED = py::str("_dropped");

namespace shim {

inline std::filesystem::path as_path(const py::str& s) { return ::cb::fs::from_text(s.utf8()); }

// settings.main_checkout(): the main checkout this one belongs to: itself, or -- for a worktree, whose .git is a
// file naming <main>/.git/worktrees/<name> -- the main one.
inline std::filesystem::path main_checkout() {
    const std::filesystem::path root = as_path(py::settings::checkout_root());
    const std::filesystem::path dot = root / ".git";
    std::error_code ec;
    if (std::filesystem::is_regular_file(dot, ec)) {
        const std::string text(::cb::text::trim(::cb::fs::read_text(dot)));
        if (text.starts_with("gitdir:")) {
            const std::filesystem::path where = std::filesystem::path(::cb::text::trim(std::string_view(text).substr(7))).lexically_normal();
            const std::filesystem::path up = where.parent_path();    // <main>/.git/worktrees
            if (up.filename() == "worktrees" && up.parent_path().filename() == ".git") return up.parent_path().parent_path();
        }
    }
    return root;
}

// float(settings.get(key) or default), the default when it is no number
inline double gb_setting(const char* key, int64_t dflt) {
    try {
        py::settings::Value v = py::settings::get(py::str(key));
        if (!py::settings::truthy_(v)) return static_cast<double>(dflt);
        switch (v.index()) {
            case 0: return py::float_from_str(std::get<py::str>(v));
            case 1: return static_cast<double>(std::get<int64_t>(v));
            case 2: return std::get<double>(v);
            default: return 1.0;
        }
    } catch (const py::TypeError&) {
        return static_cast<double>(dflt);
    } catch (const py::ValueError&) {
        return static_cast<double>(dflt);
    }
}

inline newc::Config make_config() {
    newc::Config c;
    c.state_dir = as_path(py::settings::get_str(py::str("state"))) / "cache";
    std::filesystem::path shared = as_path(py::settings::get_str(py::str("build_cache")));
    if (shared.empty()) shared = main_checkout() / "state" / "build_cache";
    c.shared_dir = shared;
    c.max_bytes = static_cast<std::uint64_t>(gb_setting("cache_max_gb", DEFAULT_MAX_GB) * static_cast<double>(newc::kGiB));
    c.shared_max_bytes = static_cast<std::uint64_t>(gb_setting("build_cache_max_gb", DEFAULT_SHARED_MAX_GB) * static_cast<double>(newc::kGiB));
    return c;
}

// The one cache this old API fronts, built from the settings on first use and again after _close() (so a harness that
// changes the state folder between runs, as the old one did, finds its new folder).
struct State {
    std::mutex mutex;
    std::unique_ptr<newc::Cache> cache;
    std::set<std::string> declared;
    std::map<std::string, std::tuple<int64_t, int64_t>> hits;   // kept across _close(), as _HITS was
};
inline State& state() {
    static State s;
    return s;
}

// settings -> repr of what a kind's generation is made of (GENERATION_REPRS), or "" when there is none
inline std::string generation_text(const py::str& kind) {
    if (!GENERATIONS.contains(kind)) return "";
    if (!GENERATION_REPRS.contains(kind)) return "";
    const std::optional<py::str> repr = GENERATION_REPRS.at(kind)();
    return repr ? repr->utf8() : std::string();
}

// The cache, with `kind` declared to it. Call with the state mutex held.
inline newc::Cache& cache_for(const py::str& kind) {
    State& st = state();
    if (!st.cache) {
        st.cache = std::make_unique<newc::Cache>(make_config());
        st.declared.clear();
    }
    const std::string name = kind.utf8();
    if (!st.declared.contains(name)) {
        newc::KindSpec spec;
        spec.scope = SHARED.contains(kind) ? newc::Scope::Shared : newc::Scope::State;
        spec.generation = generation_text(kind);
        st.cache->declare_kind(name, spec);
        st.declared.insert(name);
    }
    return *st.cache;
}

inline newc::Cache& any_cache() {
    State& st = state();
    if (!st.cache) {
        st.cache = std::make_unique<newc::Cache>(make_config());
        st.declared.clear();
    }
    return *st.cache;
}

inline void remember_counts() {
    State& st = state();
    if (!st.cache) return;
    for (const auto& [kind, h] : st.cache->counts()) {
        auto& t = st.hits[kind];
        std::get<0>(t) += static_cast<int64_t>(h.found);
        std::get<1>(t) += static_cast<int64_t>(h.missed);
    }
}

}  // namespace shim

// --- keys ---------------------------------------------------------------------------------------
// One part of a key: Python's key(*parts) takes str, bytes, any value (its str()), or a list/tuple
// (the str() of its items joined with \x1f).
using Part = std::variant<py::str, py::bytes, int64_t, double, bool, py::none_t, py::list<py::str>, py::list<int64_t>>;

inline py::str key(const std::vector<Part>& parts) {
    std::vector<newc::KeyPart> converted;
    for (const Part& p : parts) {
        std::visit(
            [&](const auto& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, py::str>) converted.emplace_back(v.utf8());
                else if constexpr (std::is_same_v<T, py::bytes>) converted.emplace_back(::cb::compat::from_py(v));
                else if constexpr (std::is_same_v<T, int64_t>) converted.emplace_back(v);
                else if constexpr (std::is_same_v<T, double>) converted.emplace_back(v);
                else if constexpr (std::is_same_v<T, bool>) converted.emplace_back(std::string_view(v ? "True" : "False"));
                else if constexpr (std::is_same_v<T, py::none_t>) converted.emplace_back(std::string_view("None"));
                else if constexpr (std::is_same_v<T, py::list<py::str>>) {
                    std::vector<std::string> items;
                    for (const py::str& s : v.vec()) items.push_back(s.utf8());
                    converted.emplace_back(items);
                } else {
                    std::vector<std::string> items;
                    for (int64_t n : v.vec()) items.push_back(std::to_string(n));
                    converted.emplace_back(items);
                }
            },
            p);
    }
    return ::cb::compat::to_py(newc::key(converted));
}

// What identifies a program: a newer build of it is a different tool.
inline py::str tool(const py::str& path) { return ::cb::compat::to_py(newc::tool_id(shim::as_path(path))); }

// What identifies a program by its bytes (for a key the shared build cache files under).
inline py::str tool_bytes(const py::str& path) {
    std::lock_guard lock(shim::state().mutex);
    return ::cb::compat::to_py(shim::any_cache().tool_bytes_id(shim::as_path(path)));
}

// The same for folders of files a tool reads (a compiler's headers): every file's name below the folder and its bytes.
inline py::str files_bytes(const py::list<py::str>& folders) {
    std::vector<std::filesystem::path> paths;
    for (const py::str& f : folders.vec()) paths.push_back(shim::as_path(f));
    std::lock_guard lock(shim::state().mutex);
    return ::cb::compat::to_py(shim::any_cache().files_bytes_id(paths));
}

// The same for a folder of files a tool reads.
inline py::str folder(const py::str& path) { return ::cb::compat::to_py(newc::folder_id(shim::as_path(path))); }

// Is caching turned off for this run (CB_NO_CACHE)?
inline bool off() { return newc::Config::disabled_by_environment(); }

// state/cache, where every kind is kept -- or, for a SHARED kind, the shared build cache.
inline py::str root(const std::optional<py::str>& kind = std::nullopt) {
    const newc::Config c = shim::make_config();
    return ::cb::compat::to_py(::cb::fs::to_text(kind && SHARED.contains(*kind) ? c.shared_dir : c.state_dir));
}

// [(folder, cap in bytes)]: state's caches, then the shared build cache.
inline py::list<std::tuple<py::str, int64_t>> roots() {
    const newc::Config c = shim::make_config();
    py::list<std::tuple<py::str, int64_t>> out;
    out.append(std::make_tuple(::cb::compat::to_py(::cb::fs::to_text(c.state_dir)), static_cast<int64_t>(c.max_bytes)));
    out.append(std::make_tuple(::cb::compat::to_py(::cb::fs::to_text(c.shared_dir)), static_cast<int64_t>(c.shared_max_bytes)));
    return out;
}

// The cap on all of state's caches together: the setting cache_max_gb (default 2).
inline int64_t max_bytes() { return static_cast<int64_t>(shim::make_config().max_bytes); }

// --- sqlite -------------------------------------------------------------------------------------
// sqlite3.Error
struct SqliteError : py::Exception {
    SqliteError() : py::Exception() {}
    explicit SqliteError(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "sqlite3.Error"; }
};

// A value in a sqlite row or parameter: None, INTEGER, REAL, TEXT, BLOB (sqlite genuinely mixes these).
using SqlParam = std::variant<std::monostate, int64_t, double, py::str, py::bytes>;
using SqlRow = std::vector<SqlParam>;

// sqlite3.Connection (the part the module uses), over cb::base::sqlite::Database. Like Python's, a connection begins a
// transaction before INSERT/UPDATE/DELETE/REPLACE and commit() ends it.
class Db {
public:
    static std::shared_ptr<Db> connect(const py::str& path, double timeout = 5.0) {
        auto db = std::shared_ptr<Db>(new Db());
        try {
            db->db_.emplace(::cb::base::sqlite::Database::open(shim::as_path(path),
                                                               std::chrono::milliseconds(static_cast<long long>(timeout * 1000))));
        } catch (const ::cb::IoError& e) {
            throw SqliteError(e.what());
        }
        return db;
    }
    // run a statement to its end, rows discarded
    void execute(const std::string& sql, const std::vector<SqlParam>& params = {}) { query(sql, params); }
    // run a statement and return all its rows
    std::vector<SqlRow> query(const std::string& sql, const std::vector<SqlParam>& params = {}) {
        open_check();
        try {
            if (!in_transaction_ && is_dml(sql)) {
                db_->exec("BEGIN");
                in_transaction_ = true;
            }
            std::vector<::cb::base::sqlite::Value> bound;
            for (const SqlParam& p : params) bound.push_back(to_value(p));
            std::vector<SqlRow> out;
            for (const auto& row : db_->query(sql, bound)) {
                SqlRow r;
                for (const auto& v : row) r.push_back(from_value(v));
                out.push_back(std::move(r));
            }
            return out;
        } catch (const ::cb::IoError& e) {
            throw SqliteError(e.what());
        }
    }
    void executemany(const std::string& sql, const std::vector<std::vector<SqlParam>>& rows) {
        for (const auto& r : rows) query(sql, r);
    }
    void commit() {
        open_check();
        if (!in_transaction_) return;
        try {
            db_->exec("COMMIT");
        } catch (const ::cb::IoError& e) {
            throw SqliteError(e.what());
        }
        in_transaction_ = false;
    }
    void close() { db_.reset(); }
    // PRAGMA database_list's file of "main"
    py::str filename() {
        open_check();
        return ::cb::compat::to_py(::cb::fs::to_text(db_->path()));
    }

private:
    Db() = default;
    void open_check() const {
        if (!db_) throw SqliteError("Cannot operate on a closed database.");
    }
    static bool is_dml(const std::string& sql) {
        const std::string_view head = ::cb::text::ltrim(sql);
        for (std::string_view word : {"insert", "update", "delete", "replace"})
            if (head.size() >= word.size() && ::cb::text::iequals(head.substr(0, word.size()), word)) return true;
        return false;
    }
    static ::cb::base::sqlite::Value to_value(const SqlParam& p) {
        switch (p.index()) {
            case 0: return std::monostate{};
            case 1: return std::get<int64_t>(p);
            case 2: return std::get<double>(p);
            case 3: return std::get<py::str>(p).utf8();
            default: return ::cb::compat::from_py(std::get<py::bytes>(p));
        }
    }
    static SqlParam from_value(const ::cb::base::sqlite::Value& v) {
        switch (v.index()) {
            case 0: return std::monostate{};
            case 1: return std::get<int64_t>(v);
            case 2: return std::get<double>(v);
            case 3: return py::str(std::get<std::string>(v));
            default: return ::cb::compat::to_py(::cb::ByteView(std::get<::cb::Bytes>(v)));
        }
    }
    std::optional<::cb::base::sqlite::Database> db_;
    bool in_transaction_ = false;
};

// --- generations --------------------------------------------------------------------------------
// The name of the generation of `kind` the code of now makes ("g" and a short digest), or None when what names it
// could not be read (then nothing of the kind is dropped).
inline std::optional<py::str> generation(const py::str& kind) {
    if (!GENERATIONS.contains(kind)) return py::str("g0");
    if (!GENERATION_REPRS.contains(kind)) return std::nullopt;
    const std::optional<py::str> repr = GENERATION_REPRS.at(kind)();
    if (!repr) return std::nullopt;
    return py::str("g" + ::cb::crypto::to_hex(::cb::crypto::sha1(::cb::as_bytes(repr->utf8()))).substr(0, 12));
}

// --- results ------------------------------------------------------------------------------------
inline std::optional<py::bytes> get(const py::str& kind, const py::str& k) {
    if (off()) return std::nullopt;
    std::lock_guard lock(shim::state().mutex);
    std::optional<::cb::Bytes> got = shim::cache_for(kind).get(kind.utf8(), k.utf8());
    if (!got) return std::nullopt;
    return ::cb::compat::to_py(::cb::ByteView(*got));
}

inline void put(const py::str& kind, const py::str& k, const py::bytes& data) {
    if (off()) return;
    ::cb::Bytes bytes = ::cb::compat::from_py(data);
    std::lock_guard lock(shim::state().mutex);
    shim::cache_for(kind).put(kind.utf8(), k.utf8(), bytes);
}

// Where the result filed under `k` is, for a tool that writes its own file. Call hold() after writing.
inline py::str path(const py::str& kind, const py::str& k) {
    std::lock_guard lock(shim::state().mutex);
    return ::cb::compat::to_py(::cb::fs::to_text(shim::cache_for(kind).path(kind.utf8(), k.utf8())));
}

// {kind: (found, not found)} in this process, for a run's summary.
inline py::dict<py::str, std::tuple<int64_t, int64_t>> counts() {
    std::lock_guard lock(shim::state().mutex);
    shim::remember_counts();
    py::dict<py::str, std::tuple<int64_t, int64_t>> out;
    for (const auto& [kind, t] : shim::state().hits) out.set(py::str(kind), t);
    return out;
}

// --- the cap ------------------------------------------------------------------------------------
// {kind: bytes on disk}, every generation of it, and what is waiting to be deleted (the state folder).
inline py::dict<py::str, int64_t> sizes(const std::optional<py::str>& = std::nullopt) {
    std::lock_guard lock(shim::state().mutex);
    py::dict<py::str, int64_t> out;
    for (const auto& s : shim::any_cache().stats())
        if (s.scope == newc::Scope::State) out.set(py::str(s.kind), static_cast<int64_t>(s.bytes));
    return out;
}

// Bring all of state's caches back under `cap` bytes (default the setting cache_max_gb), and the shared build cache
// under its own: past it, the results used longest ago go first, from any kind. -> bytes freed
inline int64_t hold(const std::optional<int64_t>& cap = std::nullopt) {
    std::lock_guard lock(shim::state().mutex);
    return static_cast<int64_t>(shim::any_cache().hold(cap ? std::optional<std::uint64_t>(static_cast<std::uint64_t>(*cap)) : std::nullopt));
}

// --- seeing and emptying it ---------------------------------------------------------------------
// Every kind back to its generation of now and the cap held. -> [(kind, what was dropped)]
inline py::list<std::tuple<py::str, py::list<py::str>>> prune() {
    std::lock_guard lock(shim::state().mutex);
    py::list<std::tuple<py::str, py::list<py::str>>> out;
    for (const newc::Dropped& d : shim::any_cache().prune()) {
        py::list<py::str> names;
        for (const std::string& g : d.generations) names.append(py::str(g));
        out.append(std::make_tuple(py::str(d.kind), names));
    }
    return out;
}

// Close the connection of `kind` (every kind when none is named), and forget the folders of path().
inline void _close(const std::optional<py::str>& = std::nullopt) {
    std::lock_guard lock(shim::state().mutex);
    shim::remember_counts();
    shim::state().cache.reset();      // (and the settings are read again on next use)
    shim::state().declared.clear();
}

// Everything kept, gone. -> bytes freed
inline int64_t clear() {
    std::lock_guard lock(shim::state().mutex);
    return static_cast<int64_t>(shim::any_cache().clear());
}

// [(kind, bytes, results, generations on disk, the generation of now)]
inline py::list<std::tuple<py::str, int64_t, int64_t, py::list<py::str>, std::optional<py::str>>> stats() {
    std::lock_guard lock(shim::state().mutex);
    py::list<std::tuple<py::str, int64_t, int64_t, py::list<py::str>, std::optional<py::str>>> out;
    for (const auto& s : shim::any_cache().stats()) {
        if (s.scope != newc::Scope::State) continue;
        py::list<py::str> gens;
        for (const std::string& g : s.generations) gens.append(py::str(g));
        out.append(std::make_tuple(py::str(s.kind), static_cast<int64_t>(s.bytes), static_cast<int64_t>(s.results), gens,
                                   s.current ? std::optional<py::str>(py::str(*s.current)) : std::nullopt));
    }
    return out;
}

// python -m codebreaker cache [--stats | --prune | --clear]
inline int64_t main(const py::list<py::str>& argv) {
    auto has = [&](const char* flag) {
        for (const py::str& a : argv.vec())
            if (a == py::str(flag)) return true;
        return false;
    };
    if (has("--clear")) {
        ::cb::out("cache cleared: {} bytes freed\n", clear());
        return 0;
    }
    if (has("--prune")) {
        for (const auto& [kind, names] : prune()) {
            std::string joined;
            for (const py::str& n : names.vec()) joined += (joined.empty() ? "" : ", ") + n.utf8();
            ::cb::out("  {:<16} dropped old generation(s) {}\n", kind.utf8(), joined);
        }
    } else if (!argv.empty() && !(argv[0] == py::str("--stats"))) {
        ::cb::out("python -m codebreaker cache [--stats | --prune | --clear]\n");
        return 2;
    }
    std::lock_guard lock(shim::state().mutex);
    ::cb::out("{}", newc::describe(shim::any_cache().stats(), shim::any_cache().config().max_bytes));
    return 0;
}

// --- pickle: what the old callers' codecs are written against (inline, unchanged from the port) -------------
// text.encode("utf-8", "replace"): a lone surrogate becomes "?"
inline std::string encode_replace_(const py::str& s) {
    std::string out;
    for (char32_t c : s.u32()) {
        if (c >= 0xD800 && c < 0xE000) out += '?';
        else out += py::str(c).utf8();
    }
    return out;
}


// --- pickle: the typed values -------------------------------------------------------------------
namespace pickle {

// A decoded pickle: the object, as a tree.  (A pickle holds objects of any kind, so this is the
// one place the kinds are mixed; value<T> turns the tree into T or refuses.)
struct Node {
    enum Kind { None, Bool, Int, BigInt, Float, Str, Bytes, List, Tuple, Dict } kind = None;
    bool b = false;
    int64_t i = 0;
    double d = 0.0;
    py::str s;
    py::bytes by;
    std::vector<Node> items;    // list / tuple items; dict: key, value, key, value ...
};

// pickle.loads for the kinds above; nullopt for a damaged pickle or one of any other kind
inline std::optional<Node> loads(const py::bytes& data);

// --- writing ---
inline void put_int(std::string& out, int64_t n);
inline void put_float(std::string& out, double d);
inline void put_str(std::string& out, const py::str& s);
inline void put_bytes(std::string& out, const py::bytes& b);
inline void put_mark(std::string& out);
inline void put_op(std::string& out, char op);

template <class T> struct Codec;

template <> struct Codec<bool> {
    static void dump(std::string& out, bool v) { out += v ? '\x88' : '\x89'; }
    static bool load(const Node& n, bool& v) { if (n.kind != Node::Bool) return false; v = n.b; return true; }
};
template <> struct Codec<int64_t> {
    static void dump(std::string& out, int64_t v) { put_int(out, v); }
    static bool load(const Node& n, int64_t& v) { if (n.kind != Node::Int) return false; v = n.i; return true; }
};
template <> struct Codec<double> {
    static void dump(std::string& out, double v) { put_float(out, v); }
    static bool load(const Node& n, double& v) { if (n.kind != Node::Float) return false; v = n.d; return true; }
};
template <> struct Codec<py::str> {
    static void dump(std::string& out, const py::str& v) { put_str(out, v); }
    static bool load(const Node& n, py::str& v) { if (n.kind != Node::Str) return false; v = n.s; return true; }
};
template <> struct Codec<py::bytes> {
    static void dump(std::string& out, const py::bytes& v) { put_bytes(out, v); }
    static bool load(const Node& n, py::bytes& v) { if (n.kind != Node::Bytes) return false; v = n.by; return true; }
};
template <class U> struct Codec<std::optional<U>> {
    static void dump(std::string& out, const std::optional<U>& v) { if (v) Codec<U>::dump(out, *v); else out += 'N'; }
    static bool load(const Node& n, std::optional<U>& v) {
        if (n.kind == Node::None) { v.reset(); return true; }
        U u{};
        if (!Codec<U>::load(n, u)) return false;
        v = std::move(u);
        return true;
    }
};
template <class U> struct Codec<py::list<U>> {
    static void dump(std::string& out, const py::list<U>& v) {
        put_op(out, ']');
        if (v.empty()) return;
        put_mark(out);
        for (const auto& x : v.vec()) Codec<U>::dump(out, x);
        put_op(out, 'e');
    }
    static bool load(const Node& n, py::list<U>& v) {
        if (n.kind != Node::List) return false;
        py::list<U> out;
        for (const Node& c : n.items) {
            U u{};
            if (!Codec<U>::load(c, u)) return false;
            out.append(u);
        }
        v = out;
        return true;
    }
};
template <class K, class V> struct Codec<py::dict<K, V>> {
    static void dump(std::string& out, const py::dict<K, V>& v) {
        put_op(out, '}');
        if (v.empty()) return;
        put_mark(out);
        for (const auto& kv : v.items()) {
            Codec<K>::dump(out, std::get<0>(kv));
            Codec<V>::dump(out, std::get<1>(kv));
        }
        put_op(out, 'u');
    }
    static bool load(const Node& n, py::dict<K, V>& v) {
        if (n.kind != Node::Dict) return false;
        py::dict<K, V> out;
        for (size_t i = 0; i + 1 < n.items.size(); i += 2) {
            K k{};
            V x{};
            if (!Codec<K>::load(n.items[i], k) || !Codec<V>::load(n.items[i + 1], x)) return false;
            out.set(k, x);
        }
        v = out;
        return true;
    }
};
template <class... U> struct Codec<std::tuple<U...>> {
    static void dump(std::string& out, const std::tuple<U...>& v) {
        constexpr size_t N = sizeof...(U);
        if constexpr (N == 0) {
            put_op(out, ')');
        } else {
            if constexpr (N > 3) put_mark(out);
            std::apply([&](const auto&... x) { (Codec<std::decay_t<decltype(x)>>::dump(out, x), ...); }, v);
            if constexpr (N == 1) put_op(out, '\x85');
            else if constexpr (N == 2) put_op(out, '\x86');
            else if constexpr (N == 3) put_op(out, '\x87');
            else put_op(out, 't');
        }
    }
    template <size_t... I> static bool load_(const Node& n, std::tuple<U...>& v, std::index_sequence<I...>) {
        return (Codec<std::tuple_element_t<I, std::tuple<U...>>>::load(n.items[I], std::get<I>(v)) && ...);
    }
    static bool load(const Node& n, std::tuple<U...>& v) {
        if (n.kind != Node::Tuple || n.items.size() != sizeof...(U)) return false;
        std::tuple<U...> out;
        if (!load_(n, out, std::index_sequence_for<U...>{})) return false;
        v = std::move(out);
        return true;
    }
};

}  // namespace pickle

// A value kept by keep<T>(), or nullopt (none, one of another type, or one another process is still writing).
template <class T> std::optional<T> value(const py::str& kind, const py::str& k) {
    std::optional<py::bytes> data = get(kind, k);
    if (!data) return std::nullopt;
    std::optional<pickle::Node> node = pickle::loads(*data);
    if (!node) return std::nullopt;
    T out{};
    if (!pickle::Codec<T>::load(*node, out)) return std::nullopt;
    return out;
}

// Keep a value under `k` (pickled as the Python object T stands for).
template <class T> void keep(const py::str& kind, const py::str& k, const T& v) {
    std::string out("\x80\x03", 2);
    pickle::Codec<T>::dump(out, v);
    out += '.';
    put(kind, k, py::bytes(out));
}


// --- pickle ---------------------------------------------------------------------------------------
namespace pickle {

inline void put_op(std::string& out, char op) { out += op; }
inline void put_mark(std::string& out) { out += '('; }

inline void le_(std::string& out, uint64_t v, int n) {
    for (int i = 0; i < n; i++) out += static_cast<char>((v >> (8 * i)) & 0xFF);
}

inline void put_int(std::string& out, int64_t n) {
    if (n >= 0 && n <= 0xFF) {
        out += 'K';
        out += static_cast<char>(n);
    } else if (n >= 0 && n <= 0xFFFF) {
        out += 'M';
        le_(out, static_cast<uint64_t>(n), 2);
    } else if (n >= -0x80000000LL && n <= 0x7FFFFFFFLL) {
        out += 'J';
        le_(out, static_cast<uint64_t>(n), 4);
    } else {                                   // LONG1: the fewest bytes of two's complement
        std::string b;
        int64_t v = n;
        do {
            b += static_cast<char>(v & 0xFF);
            v >>= 8;
        } while (!((v == 0 && !(b.back() & 0x80)) || (v == -1 && (b.back() & 0x80))));
        out += '\x8a';
        out += static_cast<char>(b.size());
        out += b;
    }
}

inline void put_float(std::string& out, double d) {
    uint64_t bits;
    std::memcpy(&bits, &d, 8);
    out += 'G';
    for (int i = 7; i >= 0; i--) out += static_cast<char>((bits >> (8 * i)) & 0xFF);
}

inline void put_str(std::string& out, const py::str& s) {
    std::string u = encode_replace_(s);
    out += 'X';
    le_(out, u.size(), 4);
    out += u;
}

inline void put_bytes(std::string& out, const py::bytes& b) {
    const std::string& r = b.raw();
    if (r.size() < 256) {
        out += 'C';
        out += static_cast<char>(r.size());
    } else {
        out += 'B';
        le_(out, r.size(), 4);
    }
    out += r;
}

namespace {

struct Bad {};      // a pickle that cannot be read as the kinds above

// the object being built; a container's items are shared (the pickle's memo refers to objects)
struct P {
    Node::Kind kind = Node::None;
    bool b = false;
    int64_t i = 0;
    double d = 0.0;
    py::str s;
    py::bytes by;
    std::vector<std::shared_ptr<P>> items;
};
using PP = std::shared_ptr<P>;

Node to_node_(const P& p, int depth) {
    if (depth > 200) throw Bad();
    Node n;
    n.kind = p.kind;
    n.b = p.b;
    n.i = p.i;
    n.d = p.d;
    n.s = p.s;
    n.by = p.by;
    for (const PP& c : p.items) n.items.push_back(to_node_(*c, depth + 1));
    return n;
}

}  // namespace

// pickle.loads for the kinds above; nullopt for a damaged pickle or one of any other kind
inline std::optional<Node> loads(const py::bytes& data) {
    const std::string& s = data.raw();
    size_t pos = 0;
    std::vector<PP> stack;
    std::vector<size_t> marks;
    std::map<uint64_t, PP> memo;
    uint64_t next_memo = 0;
    auto need = [&](size_t n) {
        if (pos + n > s.size()) throw Bad();
    };
    auto rd = [&](int n) {
        need(static_cast<size_t>(n));
        uint64_t v = 0;
        for (int k = 0; k < n; k++) v |= static_cast<uint64_t>(static_cast<unsigned char>(s[pos + k])) << (8 * k);
        pos += static_cast<size_t>(n);
        return v;
    };
    auto top = [&]() -> PP& {
        if (stack.empty()) throw Bad();
        return stack.back();
    };
    auto make = [&](Node::Kind k) {
        PP p = std::make_shared<P>();
        p->kind = k;
        return p;
    };
    auto mark = [&]() {
        if (marks.empty()) throw Bad();
        size_t m = marks.back();
        marks.pop_back();
        if (m > stack.size()) throw Bad();
        return m;
    };
    auto from_mark = [&](Node::Kind k) {      // list / tuple / dict of what is above the mark
        size_t m = mark();
        PP p = make(k);
        p->items.assign(stack.begin() + static_cast<std::ptrdiff_t>(m), stack.end());
        stack.resize(m);
        stack.push_back(p);
    };
    auto int_node = [&](int64_t v) {
        PP p = make(Node::Int);
        p->i = v;
        stack.push_back(p);
    };
    auto long_node = [&](size_t n) {
        need(n);
        PP p = make(Node::Int);
        if (n > 8) {
            p->kind = Node::BigInt;
        } else if (n > 0) {
            uint64_t v = 0;
            for (size_t k = 0; k < n; k++) v |= static_cast<uint64_t>(static_cast<unsigned char>(s[pos + k])) << (8 * k);
            if (n < 8 && (static_cast<unsigned char>(s[pos + n - 1]) & 0x80)) v |= ~uint64_t(0) << (8 * n);
            p->i = static_cast<int64_t>(v);
        }
        pos += n;
        stack.push_back(p);
    };
    auto str_node = [&](size_t n) {
        need(n);
        PP p = make(Node::Str);
        p->s = py::str(s.substr(pos, n));
        pos += n;
        stack.push_back(p);
    };
    auto bytes_node = [&](size_t n) {
        need(n);
        PP p = make(Node::Bytes);
        p->by = py::bytes(s.substr(pos, n));
        pos += n;
        stack.push_back(p);
    };
    auto tuple_n = [&](size_t n) {
        if (stack.size() < n) throw Bad();
        PP p = make(Node::Tuple);
        p->items.assign(stack.end() - static_cast<std::ptrdiff_t>(n), stack.end());
        stack.resize(stack.size() - n);
        stack.push_back(p);
    };
    try {
        for (;;) {
            need(1);
            unsigned char op = static_cast<unsigned char>(s[pos++]);
            switch (op) {
                case 0x80: rd(1); break;                       // PROTO
                case 0x95: rd(8); break;                       // FRAME
                case 0x94: memo[next_memo++] = top(); break;   // MEMOIZE
                case 'q': { uint64_t i = rd(1); memo[i] = top(); break; }   // BINPUT
                case 'r': { uint64_t i = rd(4); memo[i] = top(); break; }   // LONG_BINPUT
                case 'h': { auto it = memo.find(rd(1)); if (it == memo.end()) throw Bad(); stack.push_back(it->second); break; }   // BINGET
                case 'j': { auto it = memo.find(rd(4)); if (it == memo.end()) throw Bad(); stack.push_back(it->second); break; }   // LONG_BINGET
                case 'K': int_node(static_cast<int64_t>(rd(1))); break;
                case 'M': int_node(static_cast<int64_t>(rd(2))); break;
                case 'J': int_node(static_cast<int32_t>(rd(4))); break;
                case 0x8a: long_node(static_cast<size_t>(rd(1))); break;     // LONG1
                case 0x8b: long_node(static_cast<size_t>(rd(4))); break;     // LONG4
                case 'G': {                                                   // BINFLOAT (big-endian)
                    need(8);
                    uint64_t bits = 0;
                    for (int k = 0; k < 8; k++) bits = (bits << 8) | static_cast<unsigned char>(s[pos + k]);
                    pos += 8;
                    PP p = make(Node::Float);
                    std::memcpy(&p->d, &bits, 8);
                    stack.push_back(p);
                    break;
                }
                case 0x8c: str_node(static_cast<size_t>(rd(1))); break;      // SHORT_BINUNICODE
                case 'X': str_node(static_cast<size_t>(rd(4))); break;       // BINUNICODE
                case 0x8d: str_node(static_cast<size_t>(rd(8))); break;      // BINUNICODE8
                case 'C': bytes_node(static_cast<size_t>(rd(1))); break;     // SHORT_BINBYTES
                case 'B': bytes_node(static_cast<size_t>(rd(4))); break;     // BINBYTES
                case 0x8e: bytes_node(static_cast<size_t>(rd(8))); break;    // BINBYTES8
                case 'N': stack.push_back(make(Node::None)); break;
                case 0x88: { PP p = make(Node::Bool); p->b = true; stack.push_back(p); break; }
                case 0x89: stack.push_back(make(Node::Bool)); break;
                case ']': stack.push_back(make(Node::List)); break;
                case ')': stack.push_back(make(Node::Tuple)); break;
                case '}': stack.push_back(make(Node::Dict)); break;
                case '(': marks.push_back(stack.size()); break;
                case 'l': from_mark(Node::List); break;
                case 't': from_mark(Node::Tuple); break;
                case 'd': from_mark(Node::Dict); break;
                case 0x85: tuple_n(1); break;
                case 0x86: tuple_n(2); break;
                case 0x87: tuple_n(3); break;
                case 'a': {                                                   // APPEND
                    if (stack.size() < 2) throw Bad();
                    PP item = stack.back();
                    stack.pop_back();
                    if (stack.back()->kind != Node::List) throw Bad();
                    stack.back()->items.push_back(item);
                    break;
                }
                case 'e': {                                                   // APPENDS
                    size_t m = mark();
                    if (m == 0 || stack[m - 1]->kind != Node::List) throw Bad();
                    PP list = stack[m - 1];
                    list->items.insert(list->items.end(), stack.begin() + static_cast<std::ptrdiff_t>(m), stack.end());
                    stack.resize(m);
                    break;
                }
                case 's': {                                                   // SETITEM
                    if (stack.size() < 3) throw Bad();
                    PP v = stack.back();
                    stack.pop_back();
                    PP k = stack.back();
                    stack.pop_back();
                    if (stack.back()->kind != Node::Dict) throw Bad();
                    stack.back()->items.push_back(k);
                    stack.back()->items.push_back(v);
                    break;
                }
                case 'u': {                                                   // SETITEMS
                    size_t m = mark();
                    if (m == 0 || stack[m - 1]->kind != Node::Dict || ((stack.size() - m) & 1)) throw Bad();
                    PP d = stack[m - 1];
                    d->items.insert(d->items.end(), stack.begin() + static_cast<std::ptrdiff_t>(m), stack.end());
                    stack.resize(m);
                    break;
                }
                case '.': {                                                   // STOP
                    if (stack.size() != 1) throw Bad();
                    return to_node_(*stack[0], 0);
                }
                default: throw Bad();                                         // anything else: not a kind this reads
            }
        }
    } catch (const Bad&) {
        return std::nullopt;
    }
}

}  // namespace pickle



}  // namespace codebreaker::base::cache
