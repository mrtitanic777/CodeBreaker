// codebreaker/base/cache.hpp -- C++ from codebreaker.base.cache.
//
// cache.py -- never do the same work twice: results kept by what they were made from. (The module's
// whole docstring is kept in cache.cpp as DOC; `main` prints its usage block.)
//
//     namespace cache = codebreaker::base::cache;
//     py::str k = cache::key({cache::Part(cache::tool(compiler)), cache::Part(flags), ...});
//     std::optional<py::bytes> data = cache::get(py::str("objects"), k);
//     cache::put(py::str("objects"), k, data);
//     std::optional<T> answer = cache::value<T>(py::str("checks"), k);    // T: see "Values" below
//     cache::keep(py::str("checks"), k, answer);
//     py::str p = cache::path(py::str("port-objects"), k);
//     cache::hold();
//
// On-disk format (identical to the Python module's, so either side reads what the other wrote):
// state/cache/<kind>/<generation>.sqlite, table entry(k TEXT PRIMARY KEY, v BLOB, used INTEGER),
// WAL mode, incremental auto-vacuum; a kind of files has a folder <generation>/ instead.
//
// Values.  Python's value()/keep() store any picklable value.  Here the value API is typed: T is
// one of bool, int64_t, double, py::str, py::bytes, std::optional<U>, py::list<U>, py::dict<K, V>,
// std::tuple<...> of those.  keep<T> writes a pickle (protocol 3) of the Python object that type
// stands for (bool -> bool, int64_t -> int, double -> float, str -> str, bytes -> bytes,
// optional -> the value or None, list -> list, dict -> dict, tuple -> tuple); value<T> reads any
// protocol-0-to-5 pickle of such an object (Python's HIGHEST_PROTOCOL ones included) and returns
// nullopt when the row is missing, holds a pickle of something else (another type, a class
// instance, an int past int64, a set ...), or is damaged -- where Python's value() returns None
// for a missing or damaged row and the object for the rest.  A value Python's keep() wrote is
// therefore read back here when it is made of those types, and a value kept here is read back by
// Python's value() as the object named above.  The raw bytes API (get / put) is the same as Python's.
//
// Generations.  The Python module imports the module that owns a kind's version
// (GENERATIONS[kind] = (module, name)) and takes repr() of the value, or of the result of calling
// it.  Here a kind named in GENERATIONS asks GENERATION_REPRS[kind] -- a function returning that
// repr text (std::nullopt, or no function registered: "what names it could not be read", so
// generation() is None and nothing of the kind is dropped).  A kind not in GENERATIONS has
// one generation, "g0", as in Python.
//
// Settings come from py::settings (state, build_cache, cache_max_gb, build_cache_max_gb);
// settings.state()/build_cache()/main_checkout() are small local helpers here (that module has no
// port yet).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "py.hpp"

namespace codebreaker::base::cache {

// --- constants ----------------------------------------------------------------------------------
// The version of the code that makes each kind: the module, and the name in it.
extern py::dict<py::str, std::tuple<py::str, py::str>> GENERATIONS;
// ... and, in C++, what reading that value gives: repr(value) as Python writes it.
using GenerationRepr = std::function<std::optional<py::str>()>;
extern py::dict<py::str, GenerationRepr> GENERATION_REPRS;
extern const int64_t DEFAULT_MAX_GB;
// The kinds every state folder and checkout shares (settings.build_cache).
extern const py::set<py::str> SHARED;
extern const int64_t DEFAULT_SHARED_MAX_GB;
extern int64_t CHECK_EVERY;     // bytes a process writes between looks at the cap
extern int64_t TOUCH;           // a result's time of use is renewed at most this often (seconds)
extern const py::str DROPPED;   // where what is going waits for its deletion, out of the way

// --- keys ---------------------------------------------------------------------------------------
// One part of a key: Python's key(*parts) takes str, bytes, any value (its str()), or a list/tuple
// (the str() of its items joined with \x1f).
using Part = std::variant<py::str, py::bytes, int64_t, double, bool, py::none_t, py::list<py::str>, py::list<int64_t>>;

py::str key(const std::vector<Part>& parts);

// What identifies a program: a newer build of it is a different tool.
py::str tool(const py::str& path);

// What identifies a program by its bytes (for a key the shared build cache files under: a
// copy of the same tool elsewhere, or touched, is the same tool; a rebuilt one is not).
py::str tool_bytes(const py::str& path);

// The same for folders of files a tool reads (a compiler's headers): every file's name below
// the folder and its bytes.
py::str files_bytes(const py::list<py::str>& folders);

// The same for a folder of files a tool reads (a compiler's headers, a language's rules).
py::str folder(const py::str& path);

// Is caching turned off for this run (CB_NO_CACHE)?
bool off();

// state/cache, where every kind is kept -- or, for a SHARED kind, the shared build cache.
py::str root(const std::optional<py::str>& kind = std::nullopt);

// [(folder, cap in bytes)]: state's caches, then the shared build cache.
py::list<std::tuple<py::str, int64_t>> roots();

// The cap on all of state's caches together: the setting cache_max_gb (default 2).
int64_t max_bytes();

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

// sqlite3.Connection (the part the module uses), over the sqlite3 amalgamation.  Like Python's, a
// connection begins a transaction before INSERT/UPDATE/DELETE/REPLACE and commit() ends it.
class Db {
public:
    static std::shared_ptr<Db> connect(const py::str& path, double timeout = 5.0);
    ~Db();
    Db(const Db&) = delete;
    Db& operator=(const Db&) = delete;
    // run a statement to its end, rows discarded
    void execute(const std::string& sql, const std::vector<SqlParam>& params = {});
    // run a statement and return all its rows
    std::vector<SqlRow> query(const std::string& sql, const std::vector<SqlParam>& params = {});
    void executemany(const std::string& sql, const std::vector<std::vector<SqlParam>>& rows);
    void commit();
    void close();
    // PRAGMA database_list's file of "main"
    py::str filename();
private:
    struct Impl;
    explicit Db(std::unique_ptr<Impl> p);
    std::unique_ptr<Impl> p_;
};

// --- generations --------------------------------------------------------------------------------
// The name of the generation of `kind` the code of now makes ("g" and a short digest), or
// None when what names it could not be read (then nothing of the kind is dropped).
std::optional<py::str> generation(const py::str& kind);

// The connection to `kind`'s file of this generation, opened once per process (older
// generations dropped as it is), or null when it cannot be used.
std::shared_ptr<Db> _db(const py::str& kind);

// --- results ------------------------------------------------------------------------------------
std::optional<py::bytes> get(const py::str& kind, const py::str& k);
void put(const py::str& kind, const py::str& k, const py::bytes& data);

// Where the result filed under `k` is, for a tool that writes its own file (its folder is
// made when it is written). Such files count toward the cap too; call hold() after writing.
py::str path(const py::str& kind, const py::str& k);

// {kind: (found, not found)} in this process, for a run's summary.
py::dict<py::str, std::tuple<int64_t, int64_t>> counts();

// --- the cap ------------------------------------------------------------------------------------
// {kind: bytes on disk}, every generation of it, and what is waiting to be deleted.
py::dict<py::str, int64_t> sizes(const std::optional<py::str>& base = std::nullopt);

// Bring all of state's caches back under `cap` bytes (default the setting cache_max_gb), and
// the shared build cache under its own (build_cache_max_gb): past it, the results used longest
// ago go first, from any kind, until four fifths of the cap is used. -> bytes freed (as the
// results counted them)
int64_t hold(const std::optional<int64_t>& cap = std::nullopt);

// --- seeing and emptying it ---------------------------------------------------------------------
// Every kind back to its generation of now (the code is imported to say which) and the cap
// held. -> [(kind, what was dropped)]
py::list<std::tuple<py::str, py::list<py::str>>> prune();

// Close the connection of `kind` (every kind when none is named), and forget the folders of path().
void _close(const std::optional<py::str>& kind = std::nullopt);

// Everything kept, gone. -> bytes freed
int64_t clear();

// [(kind, bytes, results, generations on disk, the generation of now)]
py::list<std::tuple<py::str, int64_t, int64_t, py::list<py::str>, std::optional<py::str>>> stats();

// python -m codebreaker cache [--stats | --prune | --clear]
int64_t main(const py::list<py::str>& argv);

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
std::optional<Node> loads(const py::bytes& data);

// --- writing ---
void put_int(std::string& out, int64_t n);
void put_float(std::string& out, double d);
void put_str(std::string& out, const py::str& s);
void put_bytes(std::string& out, const py::bytes& b);
void put_mark(std::string& out);
void put_op(std::string& out, char op);

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

}  // namespace codebreaker::base::cache
