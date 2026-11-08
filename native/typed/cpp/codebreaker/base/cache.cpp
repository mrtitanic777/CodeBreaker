// codebreaker/base/cache.cpp -- C++ from codebreaker.base.cache.
//
// See cache.hpp for how the Python's pickled values, its generation lookup (an import) and its
// sqlite3 module map to C++.  The sqlite3 amalgamation is native/engine/third_party/sqlite/sqlite3.c.

#include "codebreaker/base/cache.hpp"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <mutex>
#include <random>
#include <sstream>

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

#include "py_os.hpp"
#include "py_settings.hpp"
#include "sqlite3.h"

namespace codebreaker::base::cache {

namespace fs = std::filesystem;

// The docstring of cache.py, whose second paragraph is the usage `main` prints.
static const char* const DOC_TEXT = R"cachedoc(cache.py -- never do the same work twice: results kept by what they were made from.

    from codebreaker.base import cache
    k = cache.key(cache.tool(compiler), flags, header, text)     # everything the result depends on
    data = cache.get("objects", k)                               # bytes, or None
    cache.put("objects", k, data)
    answer = cache.value("checks", k)                            # any Python value, or None
    cache.keep("checks", k, answer)
    p = cache.path("port-objects", k)                            # a file of its own, for a tool to write
    cache.hold()                                                 # back under the size cap (after such files)

    python -m codebreaker cache [--stats]     what is kept, kind by kind, and the cap
    python -m codebreaker cache --prune       old generations dropped, the cap held
    python -m codebreaker cache --clear       everything kept, gone

A compiler given the same source and flags gives the same object file; the decompiler given the
same bytes gives the same C. Both take most of the time of every run, and most of what a run is
given is what the run before was given. So the result is filed under a hash of everything it
depends on -- the tool itself (its name, size and date), its options, its input -- and looked up
before the tool is started. Nothing is ever stale: a different input is a different name.

But a name no one asks for again is never asked for again, and kept for ever it is only weight
(the judge's fingerprints once filled 10 GB, mostly under revisions of the comparer long gone). So
the cache looks after its own size, by two rules:

  GENERATIONS  every kind of result is made by some version of the code, and the version is in
               its keys already (the comparer's REV, the decompiler's build and languages). The
               kind says which, once, in GENERATIONS below; its results are kept apart by it, and
               when the kind is first used in a process the results of every other generation
               go: the code of now can never ask for them. Going is one file deleted, not a
               look through millions of names.

  THE CAP      all of state's caches together are held under the setting `cache_max_gb`
               (default 2). As results are written, past the cap the ones used longest ago go
               first, from any kind, until four fifths of it is used.

SHARED       the kinds in SHARED below -- what an assembler or a compiler made of its inputs --
               are kept apart from any one state folder, in the build cache (the setting
               build_cache, default the main checkout's state/build_cache), so a fresh state
               or another worktree reuses them. Their keys name every input by its bytes
               (tool_bytes, files_bytes), never by a path or a date; their generations are
               never dropped (another checkout may run other code), only the cap
               (build_cache_max_gb, default 6) holds them.

Each kind is one sqlite file, state/cache/<kind>/<generation>.sqlite, its results rows in it (many
small results are one file, not millions); a kind whose results are files a tool writes itself
(path()) has a folder, state/cache/<kind>/<generation>/, instead. Deleting state/cache costs time
and nothing else. CB_NO_CACHE=1 turns every lookup into a miss and keeps nothing.
)cachedoc";

// The version of the code that makes each kind: the module, and the name in it (a value, or a
// function giving one), of what in the kind's keys changes with the code. A kind not named here
// has one generation; its results go only by the cap.
py::dict<py::str, std::tuple<py::str, py::str>> GENERATIONS = {
    {py::str("fingerprints"), {py::str("codebreaker.verify.compare"), py::str("REV")}},
    {py::str("masm"), {py::str("codebreaker.build.masm"), py::str("CACHE_VER")}},
    {py::str("decompiled"), {py::str("codebreaker.decompile.decompiler"), py::str("generation")}},
    {py::str("decompiled_fn"), {py::str("codebreaker.decompile.decompiler"), py::str("generation")}},
    {py::str("gate16_tables"), {py::str("codebreaker.decompile.decompiler"), py::str("generation")}},
    {py::str("compilable16"), {py::str("codebreaker.decompile.c_text"), py::str("generation")}},
};
py::dict<py::str, GenerationRepr> GENERATION_REPRS;
const int64_t DEFAULT_MAX_GB = 2;
// The kinds every state folder and checkout shares (settings.build_cache): results that are a pure
// function of the inputs their keys name, all of them by content.
const py::set<py::str> SHARED = {py::str("wasm16"), py::str("objects"), py::str("never_set"),
                                 py::str("masm"), py::str("assembled16"), py::str("omf_publics")};
const int64_t DEFAULT_SHARED_MAX_GB = 6;
int64_t CHECK_EVERY = 32 << 20;      // bytes a process writes between looks at the cap
int64_t TOUCH = 3600;                // a result's time of use is renewed at most this often (seconds)
const py::str DROPPED = py::str("_dropped");   // where what is going waits for its deletion, out of the way

namespace {

struct OpenEntry {
    int64_t pid = 0;
    std::shared_ptr<Db> db;          // null: the kind cannot be used
    std::optional<py::str> gen;
};

py::dict<py::str, std::tuple<int64_t, int64_t>> _HITS;
py::dict<py::str, OpenEntry> _OPEN;      // kind -> (process id, connection or none, generation)
py::dict<py::str, py::str> _FOLDERS;     // kind -> its generation folder, for path() kinds
std::recursive_mutex& LOCK() {
    static std::recursive_mutex m;
    return m;
}
int64_t _WRITTEN_BYTES = 0;              // bytes written since the cap was looked at
bool _WRITTEN_LOOKED = false;            // looked at yet?
std::map<std::tuple<std::string, int64_t, int64_t>, py::str> _BYTES;   // (path, size, mtime) -> digest of the file's bytes

int64_t getpid_() {
#ifdef _WIN32
    return static_cast<int64_t>(_getpid());
#else
    return static_cast<int64_t>(getpid());
#endif
}

// int(time.time())
int64_t now_() {
    return static_cast<int64_t>(py::time::time());
}

// int(x) of a float
int64_t int_of_(double x) {
    if (x != x) throw py::ValueError("cannot convert float NaN to integer");
    if (x > 9.2e18 || x < -9.2e18) throw py::OverflowError("cannot convert float infinity to integer");
    return static_cast<int64_t>(x);
}

// text.encode("utf-8", "replace"): a lone surrogate becomes "?"
std::string encode_replace_(const py::str& s) {
    std::string out;
    for (char32_t c : s.u32()) {
        if (c >= 0xD800 && c < 0xE000) out += '?';
        else out += py::str(c).utf8();
    }
    return out;
}

// os.stat(path): size, mtime in whole seconds, mtime in nanoseconds; false when it cannot be read
struct Stat {
    int64_t size = 0;
    int64_t mtime = 0;
    int64_t mtime_ns = 0;
};
bool stat_(const py::str& path, Stat& st) {
    if (path.u32().find(U'\0') != std::u32string::npos) return false;
    std::error_code ec;
    fs::path p = fs::u8path(path.utf8());
    bool dir = fs::is_directory(p, ec);
    if (ec) return false;
    int64_t size = 0;
    if (!dir) {
        size = static_cast<int64_t>(fs::file_size(p, ec));
        if (ec) return false;
    }
    int64_t ticks = static_cast<int64_t>(fs::last_write_time(p, ec).time_since_epoch().count());   // 100 ns since 1601
    if (ec) return false;
    st.size = size;
    st.mtime = ticks / 10000000 - 11644473600LL;
    st.mtime_ns = (ticks - 116444736000000000LL) * 100;
    return true;
}

std::string uuid_hex_() {
    static std::mt19937_64 gen{std::random_device{}()};
    static const char* digits = "0123456789abcdef";
    std::string out;
    for (int i = 0; i < 32; i++) out += digits[gen() & 15];
    return out;
}

// settings.state("cache"): a path inside the state folder, which is created if it is not there.
py::str state_cache_() {
    py::str path = py::os::path::join(py::settings::get_str(py::str("state")), py::str("cache"));
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

// settings.main_checkout(): the main checkout this one belongs to: itself, or -- for a worktree,
// whose .git is a file naming <main>/.git/worktrees/<name> -- the main one.
py::str main_checkout_() {
    py::str rootdir = py::settings::checkout_root();
    py::str dot = py::os::path::join(rootdir, py::str(".git"));
    if (py::os::path::isfile(dot)) {
        std::string line;
        {
            std::ifstream f(fs::u8path(dot.utf8()), std::ios::binary);
            std::stringstream ss;
            ss << f.rdbuf();
            line = ss.str();
        }
        py::str text = py::str(line).strip();
        if (text.startswith(py::str("gitdir:"))) {
            py::str where = py::os::path::normpath(text.slice(7, text.size()).strip());
            py::str up = py::os::path::dirname(where);           // <main>/.git/worktrees
            if (py::os::path::basename(up) == py::str("worktrees") &&
                py::os::path::basename(py::os::path::dirname(up)) == py::str(".git"))
                return py::os::path::dirname(py::os::path::dirname(up));
        }
    }
    return rootdir;
}

// settings.build_cache(): the shared build cache's folder (the setting build_cache), created on demand.
py::str build_cache_() {
    py::str path = py::settings::get_str(py::str("build_cache"));
    if (path.empty()) path = py::os::path::join(main_checkout_(), py::str("state"), py::str("build_cache"));
    py::os::makedirs(path, 0777, true);
    return path;
}

// float(settings.get(key) or default), the default when it is no number
double gb_setting_(const char* key, int64_t dflt) {
    try {
        py::settings::Value v = py::settings::get(py::str(key));
        if (!py::settings::truthy_(v)) return static_cast<double>(dflt);
        switch (v.index()) {
            case 0: return py::float_from_str(std::get<py::str>(v));
            case 1: return static_cast<double>(std::get<int64_t>(v));
            case 2: return std::get<double>(v);
            default: return 1.0;      // True
        }
    } catch (const py::TypeError&) {
        return static_cast<double>(dflt);
    } catch (const py::ValueError&) {
        return static_cast<double>(dflt);
    }
}

// name.split(".")[0]
py::str first_part_(const py::str& n) {
    int64_t i = n.find(py::str("."));
    return i < 0 ? n : n.slice(0, i);
}

// the text of the bytes `p`: Python's  b"%d:" % len(p)  then p
void feed_(const py::hashlib::Hash& h, const std::string& p) {
    h.update(py::bytes(std::to_string(p.size()) + ":"));
    h.update(py::bytes(p));
}

py::str gb_(int64_t n) {
    return n >= (int64_t(1) << 30) ? py::percent(py::str("%.2f GB"), static_cast<double>(n) / static_cast<double>(int64_t(1) << 30))
                                   : py::percent(py::str("%.1f MB"), static_cast<double>(n) / static_cast<double>(int64_t(1) << 20));
}

}  // namespace

// --- sqlite ---------------------------------------------------------------------------------------
struct Db::Impl {
    sqlite3* db = nullptr;
};

namespace {

[[noreturn]] void sql_fail_(sqlite3* db, const char* what) {
    std::string m = db ? sqlite3_errmsg(db) : what;
    throw SqliteError(m);
}

struct Stmt {
    sqlite3* db;
    sqlite3_stmt* st = nullptr;
    Stmt(sqlite3* d, const std::string& sql) : db(d) {
        if (sqlite3_prepare_v2(db, sql.c_str(), static_cast<int>(sql.size()), &st, nullptr) != SQLITE_OK) sql_fail_(db, "prepare");
    }
    ~Stmt() { sqlite3_finalize(st); }
    Stmt(const Stmt&) = delete;
    Stmt& operator=(const Stmt&) = delete;
};

bool is_dml_(const std::string& sql) {
    size_t i = 0;
    while (i < sql.size() && std::isspace(static_cast<unsigned char>(sql[i]))) i++;
    std::string w;
    while (i < sql.size() && std::isalpha(static_cast<unsigned char>(sql[i]))) w += static_cast<char>(std::toupper(static_cast<unsigned char>(sql[i++])));
    return w == "INSERT" || w == "UPDATE" || w == "DELETE" || w == "REPLACE";
}

}  // namespace

Db::Db(std::unique_ptr<Impl> p) : p_(std::move(p)) {}
Db::~Db() { close(); }

std::shared_ptr<Db> Db::connect(const py::str& path, double timeout) {
    std::unique_ptr<Impl> impl(new Impl());
    int rc = sqlite3_open_v2(path.utf8().c_str(), &impl->db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        std::string m = impl->db ? sqlite3_errmsg(impl->db) : "unable to open database file";
        sqlite3_close(impl->db);
        throw SqliteError(m);
    }
    sqlite3_busy_timeout(impl->db, static_cast<int>(timeout * 1000));
    return std::shared_ptr<Db>(new Db(std::move(impl)));
}

void Db::close() {
    if (p_ && p_->db) {
        sqlite3_close_v2(p_->db);
        p_->db = nullptr;
    }
}

std::vector<SqlRow> Db::query(const std::string& sql, const std::vector<SqlParam>& params) {
    if (!p_ || !p_->db) throw SqliteError("Cannot operate on a closed database.");
    sqlite3* db = p_->db;
    if (is_dml_(sql) && sqlite3_get_autocommit(db)) {      // (Python's implicit BEGIN before a change)
        Stmt b(db, "BEGIN");
        if (sqlite3_step(b.st) != SQLITE_DONE) sql_fail_(db, "begin");
    }
    Stmt s(db, sql);
    for (size_t i = 0; i < params.size(); i++) {
        int idx = static_cast<int>(i) + 1;
        int rc = SQLITE_OK;
        const SqlParam& p = params[i];
        switch (p.index()) {
            case 0: rc = sqlite3_bind_null(s.st, idx); break;
            case 1: rc = sqlite3_bind_int64(s.st, idx, std::get<int64_t>(p)); break;
            case 2: rc = sqlite3_bind_double(s.st, idx, std::get<double>(p)); break;
            case 3: {
                std::string t = std::get<py::str>(p).utf8();
                rc = sqlite3_bind_text(s.st, idx, t.data(), static_cast<int>(t.size()), SQLITE_TRANSIENT);
                break;
            }
            default: {
                const std::string& b = std::get<py::bytes>(p).raw();
                rc = sqlite3_bind_blob64(s.st, idx, b.data(), b.size(), SQLITE_TRANSIENT);
            }
        }
        if (rc != SQLITE_OK) sql_fail_(db, "bind");
    }
    std::vector<SqlRow> rows;
    for (;;) {
        int rc = sqlite3_step(s.st);
        if (rc == SQLITE_DONE) break;
        if (rc != SQLITE_ROW) sql_fail_(db, "step");
        SqlRow row;
        int n = sqlite3_column_count(s.st);
        for (int c = 0; c < n; c++) {
            switch (sqlite3_column_type(s.st, c)) {
                case SQLITE_INTEGER: row.emplace_back(static_cast<int64_t>(sqlite3_column_int64(s.st, c))); break;
                case SQLITE_FLOAT: row.emplace_back(sqlite3_column_double(s.st, c)); break;
                case SQLITE_TEXT:
                    row.emplace_back(py::str(std::string(reinterpret_cast<const char*>(sqlite3_column_text(s.st, c)),
                                                         static_cast<size_t>(sqlite3_column_bytes(s.st, c)))));
                    break;
                case SQLITE_BLOB: {
                    const void* b = sqlite3_column_blob(s.st, c);
                    size_t len = static_cast<size_t>(sqlite3_column_bytes(s.st, c));
                    row.emplace_back(py::bytes(len ? std::string(static_cast<const char*>(b), len) : std::string()));
                    break;
                }
                default: row.emplace_back(std::monostate{});
            }
        }
        rows.push_back(std::move(row));
    }
    return rows;
}

void Db::execute(const std::string& sql, const std::vector<SqlParam>& params) {
    query(sql, params);
}

void Db::executemany(const std::string& sql, const std::vector<std::vector<SqlParam>>& rows) {
    for (const auto& r : rows) query(sql, r);
}

void Db::commit() {
    if (!p_ || !p_->db) throw SqliteError("Cannot operate on a closed database.");
    if (!sqlite3_get_autocommit(p_->db)) {
        Stmt c(p_->db, "COMMIT");
        if (sqlite3_step(c.st) != SQLITE_DONE) sql_fail_(p_->db, "commit");
    }
}

py::str Db::filename() {
    if (!p_ || !p_->db) throw SqliteError("Cannot operate on a closed database.");
    const char* f = sqlite3_db_filename(p_->db, "main");
    return py::str(std::string(f ? f : ""));
}

// --- keys -----------------------------------------------------------------------------------------
namespace {

std::string part_bytes_(const Part& p) {
    switch (p.index()) {
        case 0: return encode_replace_(std::get<py::str>(p));
        case 1: return std::get<py::bytes>(p).raw();
        case 2: return encode_replace_(py::to_str(std::get<int64_t>(p)));
        case 3: return encode_replace_(py::to_str(std::get<double>(p)));
        case 4: return std::get<bool>(p) ? "True" : "False";
        case 5: return "None";
        case 6: {
            std::string out;
            bool first = true;
            for (const auto& x : std::get<py::list<py::str>>(p).vec()) {
                if (!first) out += '\x1f';
                first = false;
                out += encode_replace_(x);
            }
            return out;
        }
        default: {
            std::string out;
            bool first = true;
            for (int64_t x : std::get<py::list<int64_t>>(p).vec()) {
                if (!first) out += '\x1f';
                first = false;
                out += std::to_string(x);
            }
            return out;
        }
    }
}

}  // namespace

py::str key(const std::vector<Part>& parts) {
    py::hashlib::Hash h = py::hashlib::sha1();
    for (const Part& p : parts) feed_(h, part_bytes_(p));
    return h.hexdigest();
}

// What identifies a program: a newer build of it is a different tool.
py::str tool(const py::str& path) {
    Stat st;
    if (stat_(path, st))
        return py::percent(py::str("%s:%d:%d"), py::os::path::basename(path).lower(), st.size, st.mtime);
    return py::percent(py::str("%s:missing"), py::os::path::basename(path));
}

// What identifies a program by its bytes (for a key the shared build cache files under: a
// copy of the same tool elsewhere, or touched, is the same tool; a rebuilt one is not).
py::str tool_bytes(const py::str& path) {
    Stat st;
    if (!stat_(path, st)) return py::percent(py::str("%s:missing"), py::os::path::basename(path));
    auto k = std::make_tuple(py::os::path::normcase(py::os::path::abspath(path)).utf8(), st.size, st.mtime_ns);
    auto it = _BYTES.find(k);
    if (it == _BYTES.end()) {
        py::hashlib::Hash h = py::hashlib::sha1();
        std::ifstream f(fs::u8path(path.utf8()), std::ios::binary);
        if (!f) throw py::OSError("[Errno 13] Permission denied: " + py::repr(path).utf8());
        std::string block(1 << 20, '\0');
        while (f.read(&block[0], static_cast<std::streamsize>(block.size())) || f.gcount() > 0)
            h.update(py::bytes(block.substr(0, static_cast<size_t>(f.gcount()))));
        py::str got = py::percent(py::str("%s:%s"), py::os::path::basename(path).lower(), h.hexdigest());
        it = _BYTES.emplace(k, got).first;
    }
    return it->second;
}

// The same for folders of files a tool reads (a compiler's headers): every file's name below
// the folder and its bytes.
py::str files_bytes(const py::list<py::str>& folders) {
    py::list<py::str> parts;
    for (const py::str& top : folders) {
        for (const auto& entry : py::os::walk(top)) {
            const py::str& where = std::get<0>(entry);
            const py::list<py::str>& dirs = std::get<1>(entry);
            const py::list<py::str>& files = std::get<2>(entry);
            dirs.sort();
            for (const py::str& f : py::sorted(files)) {
                py::str p = py::os::path::join(where, f);
                parts.append(py::os::path::relpath(p, top).replace(py::str("\\"), py::str("/")).lower() + py::str("=") + tool_bytes(p));
            }
        }
    }
    return key({Part(parts)});
}

// The same for a folder of files a tool reads (a compiler's headers, a language's rules).
py::str folder(const py::str& path) {
    py::list<py::str> parts;
    for (const auto& entry : py::os::walk(path)) {
        const py::str& rootdir = std::get<0>(entry);
        for (const py::str& f : py::sorted(std::get<2>(entry))) {
            Stat st;
            if (stat_(py::os::path::join(rootdir, f), st))
                parts.append(py::percent(py::str("%s:%d:%d"), f, st.size, st.mtime));
        }
    }
    return key({Part(parts)});
}

// Is caching turned off for this run (CB_NO_CACHE)?
bool off() {
    std::optional<py::str> v = py::os::environ_::get(py::str("CB_NO_CACHE"));
    return v && !v->empty();
}

// state/cache, where every kind is kept -- or, for a SHARED kind, the shared build cache.
py::str root(const std::optional<py::str>& kind) {
    if (kind && SHARED.contains(*kind)) return build_cache_();
    return state_cache_();
}

// [(folder, cap in bytes)]: state's caches, then the shared build cache.
py::list<std::tuple<py::str, int64_t>> roots() {
    double gb = gb_setting_("build_cache_max_gb", DEFAULT_SHARED_MAX_GB);
    py::list<std::tuple<py::str, int64_t>> out;
    py::str state = state_cache_();
    int64_t mine = max_bytes();
    out.append(std::make_tuple(state, mine));
    py::str shared = build_cache_();
    out.append(std::make_tuple(shared, int_of_(gb * static_cast<double>(int64_t(1) << 30))));
    return out;
}

// The cap on all of state's caches together: the setting cache_max_gb (default 2).
int64_t max_bytes() {
    double gb = gb_setting_("cache_max_gb", DEFAULT_MAX_GB);
    return int_of_(gb * static_cast<double>(int64_t(1) << 30));
}

// --- generations ------------------------------------------------------------------------------
// The name of the generation of `kind` the code of now makes ("g" and a short digest), or
// None when what names it could not be read (then nothing of the kind is dropped).
std::optional<py::str> generation(const py::str& kind) {
    if (!GENERATIONS.contains(kind)) return py::str("g0");
    try {
        if (!GENERATION_REPRS.contains(kind)) return std::nullopt;     // (the module could not be imported)
        std::optional<py::str> r = GENERATION_REPRS.at(kind)();
        if (!r) return std::nullopt;
        return py::str("g") + key({Part(*r)}).slice(0, 12);
    } catch (const py::Exception&) {
        return std::nullopt;
    }
}

namespace {

void _remove(const py::str& path) {
    if (py::os::path::isdir(path)) {
        py::shutil::rmtree(path, true);
    } else {
        try {
            py::os::remove(path);
        } catch (const py::OSError&) {
        }
    }
}

// Every generation of a kind but `gen` out of its folder: moved aside at once (another
// process opening the kind finds them gone), then deleted.
void _drop_others(const py::str& path, const py::str& gen) {
    py::list<py::str> names;
    try {
        names = py::os::listdir(path);
    } catch (const py::OSError&) {
        return;
    }
    for (const py::str& n : names) {
        if (first_part_(n) == gen) continue;
        py::str aside = py::os::path::join(root(), DROPPED, py::str(uuid_hex_()));
        try {
            py::os::makedirs(py::os::path::dirname(aside), 0777, true);
            py::os::replace(py::os::path::join(path, n), aside);
        } catch (const py::OSError&) {
            continue;                             // (in use by a process of other code: next time)
        }
        _remove(aside);
    }
}

std::shared_ptr<Db> _connect(const py::str& path) {
    std::shared_ptr<Db> db = Db::connect(path, 10.0);
    db->execute("PRAGMA auto_vacuum=INCREMENTAL");         // (a new file only: rows deleted give the space back)
    db->execute("PRAGMA journal_mode=WAL");                // (workers read while one writes)
    db->execute("PRAGMA synchronous=NORMAL");
    db->execute("CREATE TABLE IF NOT EXISTS entry(k TEXT PRIMARY KEY, v BLOB, used INTEGER)");
    db->execute("CREATE INDEX IF NOT EXISTS entry_used ON entry(used)");
    db->commit();
    return db;
}

void _count(const py::str& kind, bool found) {
    std::tuple<int64_t, int64_t> h = _HITS.get(kind, std::make_tuple(int64_t(0), int64_t(0)));
    _HITS.set(kind, found ? std::make_tuple(std::get<0>(h) + 1, std::get<1>(h)) : std::make_tuple(std::get<0>(h), std::get<1>(h) + 1));
}

}  // namespace

// The connection to `kind`'s file of this generation, opened once per process (older
// generations dropped as it is), or null when it cannot be used.
std::shared_ptr<Db> _db(const py::str& kind) {
    std::lock_guard<std::recursive_mutex> guard(LOCK());
    if (_OPEN.contains(kind)) {
        const OpenEntry& had = _OPEN.at(kind);
        if (had.pid == getpid_()) return had.db;
    }
    std::shared_ptr<Db> db;
    std::optional<py::str> gen = generation(kind);
    try {
        py::str path = py::os::path::join(root(kind), kind);
        if (gen && !SHARED.contains(kind)) _drop_others(path, *gen);     // (a shared kind: another checkout's generation stays)
        py::os::makedirs(path, 0777, true);
        db = _connect(py::os::path::join(path, (gen ? *gen : py::str("g0")) + py::str(".sqlite")));
    } catch (const py::OSError&) {
        db = nullptr;
    } catch (const SqliteError&) {
        db = nullptr;
    }
    OpenEntry e;
    e.pid = getpid_();
    e.db = db;
    e.gen = gen;
    _OPEN.set(kind, e);
    return db;
}

// --- results --------------------------------------------------------------------------------------
std::optional<py::bytes> get(const py::str& kind, const py::str& k) {
    if (off()) return std::nullopt;
    std::shared_ptr<Db> db = _db(kind);
    std::optional<SqlRow> row;
    if (db) {
        std::lock_guard<std::recursive_mutex> guard(LOCK());
        try {
            std::vector<SqlRow> rows = db->query("SELECT v, used FROM entry WHERE k=?", {SqlParam(k)});
            if (!rows.empty()) row = rows[0];
            int64_t now = now_();
            if (row && std::get<int64_t>((*row)[1]) < now - TOUCH) {   // (used again: the last to go under the cap)
                db->execute("UPDATE entry SET used=? WHERE k=?", {SqlParam(now), SqlParam(k)});
                db->commit();
            }
        } catch (const SqliteError&) {
        }
    }
    _count(kind, row.has_value());
    if (!row) return std::nullopt;
    if (!std::holds_alternative<py::bytes>((*row)[0]))
        throw py::TypeError("cannot convert the stored value to bytes");
    return std::get<py::bytes>((*row)[0]);
}

void put(const py::str& kind, const py::str& k, const py::bytes& data) {
    if (off()) return;
    std::shared_ptr<Db> db = _db(kind);
    if (!db) return;
    {
        std::lock_guard<std::recursive_mutex> guard(LOCK());
        try {
            db->execute("INSERT OR REPLACE INTO entry(k, v, used) VALUES(?,?,?)",
                        {SqlParam(k), SqlParam(data), SqlParam(now_())});
            db->commit();                          // (one that loses the lock is made again next time)
        } catch (const SqliteError&) {
            return;
        }
        _WRITTEN_BYTES += data.size() + k.size();
        if (_WRITTEN_BYTES < CHECK_EVERY && _WRITTEN_LOOKED) return;
        _WRITTEN_BYTES = 0;
        _WRITTEN_LOOKED = true;
    }
    hold();
}

// Where the result filed under `k` is, for a tool that writes its own file (its folder is
// made when it is written). Such files count toward the cap too; call hold() after writing.
py::str path(const py::str& kind, const py::str& k) {
    std::lock_guard<std::recursive_mutex> guard(LOCK());
    py::str gen;
    if (_FOLDERS.contains(kind)) {
        gen = _FOLDERS.at(kind);
    } else {
        std::optional<py::str> g = generation(kind);
        if (g && !SHARED.contains(kind)) _drop_others(py::os::path::join(root(kind), kind), *g);
        gen = g ? *g : py::str("g0");
        _FOLDERS.set(kind, gen);
    }
    return py::os::path::join(root(kind), kind, gen, k.slice(0, 2), k);
}

// {kind: (found, not found)} in this process, for a run's summary.
py::dict<py::str, std::tuple<int64_t, int64_t>> counts() {
    return _HITS.copy();
}

// --- the cap --------------------------------------------------------------------------------------
namespace {

using Store = std::tuple<py::str, bool>;     // (path, is a database)

// {kind: [(path, is a database)]} of everything kept on disk now, of any generation.
py::dict<py::str, py::list<Store>> _stores(const std::optional<py::str>& base_ = std::nullopt) {
    py::dict<py::str, py::list<Store>> out;
    py::str base = (base_ && !base_->empty()) ? *base_ : root();
    if (!py::os::path::isdir(base)) return out;
    for (const py::str& kind : py::sorted(py::os::listdir(base))) {
        if (kind == DROPPED || !py::os::path::isdir(py::os::path::join(base, kind))) continue;
        py::list<Store> where;
        for (const py::str& n : py::sorted(py::os::listdir(py::os::path::join(base, kind)))) {
            if (n.endswith(py::str("-wal")) || n.endswith(py::str("-shm")) || n.endswith(py::str("-journal"))) continue;
            where.append(Store(py::os::path::join(base, kind, n), n.endswith(py::str(".sqlite"))));
        }
        out.set(kind, where);
    }
    return out;
}

int64_t _size(const py::str& path) {
    if (py::os::path::isdir(path)) {
        int64_t total = 0;
        for (const auto& entry : py::os::walk(path)) {
            for (const py::str& n : std::get<2>(entry)) {
                try {
                    total += py::os::path::getsize(py::os::path::join(std::get<0>(entry), n));
                } catch (const py::OSError&) {
                }
            }
        }
        return total;
    }
    int64_t total = 0;
    for (const char* suffix : {"", "-wal", "-shm"}) {
        try {
            total += py::os::path::getsize(path + py::str(suffix));
        } catch (const py::OSError&) {
        }
    }
    return total;
}

int64_t sum_values_(const py::dict<py::str, int64_t>& d) {
    int64_t t = 0;
    for (int64_t v : d.values()) t += v;
    return t;
}

}  // namespace

// {kind: bytes on disk}, every generation of it, and what is waiting to be deleted.
py::dict<py::str, int64_t> sizes(const std::optional<py::str>& base_) {
    py::str base = (base_ && !base_->empty()) ? *base_ : root();
    py::dict<py::str, int64_t> out;
    for (const auto& kv : _stores(base).items()) {
        int64_t t = 0;
        for (const Store& s : std::get<1>(kv).vec()) t += _size(std::get<0>(s));
        out.set(std::get<0>(kv), t);
    }
    if (py::os::path::isdir(py::os::path::join(base, DROPPED)))
        out.set(DROPPED, _size(py::os::path::join(base, DROPPED)));
    return out;
}

namespace {

// A connection to the file `p`: this process's own when it has one open, else a new one.
std::pair<std::shared_ptr<Db>, bool> _connection(const py::str& p) {
    for (const OpenEntry& e : _OPEN.values()) {
        if (e.pid == getpid_() && e.db &&
            py::os::path::normcase(e.db->filename()) == py::os::path::normcase(py::os::path::abspath(p)))
            return {e.db, false};
    }
    return {Db::connect(p, 10.0), true};
}

// Give the space of deleted rows back to the disk: the free pages cut off the file, the
// write-ahead log folded in and emptied.
void _shrink(const py::str& p) {
    try {
        auto [db, mine] = _connection(p);
        db->query("PRAGMA incremental_vacuum");      // (a page a step: every step)
        db->query("PRAGMA wal_checkpoint(TRUNCATE)");
        if (mine) db->close();
    } catch (const SqliteError&) {
    }
}

// One result, as _oldest lists it: when it was used, its size, and where it is -- a row of a
// database (db, rowid) or a file.
struct Old {
    int64_t used = 0;
    int64_t size = 0;
    std::optional<py::str> db;
    int64_t rowid = 0;
    py::str file;
};

// the results used longest ago
std::vector<Old> _oldest(const py::dict<py::str, py::list<Store>>& stores, int64_t n = 4000) {
    std::vector<Old> olds;
    for (const auto& kv : stores.items()) {
        for (const Store& s : std::get<1>(kv).vec()) {
            const py::str& p = std::get<0>(s);
            if (std::get<1>(s)) {
                try {
                    auto [db, mine] = _connection(p);
                    for (const SqlRow& r : db->query("SELECT rowid, used, length(k) + length(v) FROM entry ORDER BY used LIMIT ?",
                                                     {SqlParam(n)})) {
                        Old o;
                        o.used = std::get<int64_t>(r[1]);
                        o.size = std::get<int64_t>(r[2]);
                        o.db = p;
                        o.rowid = std::get<int64_t>(r[0]);
                        olds.push_back(o);
                    }
                    if (mine) db->close();
                } catch (const SqliteError&) {
                }
            } else if (py::os::path::isdir(p)) {
                for (const auto& entry : py::os::walk(p)) {
                    for (const py::str& name : std::get<2>(entry)) {
                        py::str f = py::os::path::join(std::get<0>(entry), name);
                        Stat st;
                        if (stat_(f, st)) {
                            Old o;
                            o.used = st.mtime;
                            o.size = st.size;
                            o.file = f;
                            olds.push_back(o);
                        }
                    }
                }
            }
        }
    }
    std::stable_sort(olds.begin(), olds.end(), [](const Old& a, const Old& b) { return a.used < b.used; });
    return olds;
}

// What goes from one database (or, with no database, which files).
struct Gone {
    std::optional<py::str> db;
    std::vector<int64_t> rowids;
    std::vector<py::str> files;
};

int64_t _hold(const py::str& base, int64_t cap) {
    int64_t freed = 0;
    bool shrunk = false;
    std::lock_guard<std::recursive_mutex> guard(LOCK());
    for (int round = 0; round < 16; round++) {
        int64_t total;
        try {
            total = sum_values_(sizes(base));
        } catch (const py::OSError&) {
            return freed;
        }
        if (total <= cap) return freed;
        py::dict<py::str, py::list<Store>> stores = _stores(base);
        if (!shrunk) {                       // (the logs folded in first: they may be most of it)
            for (const auto& kv : stores.items())
                for (const Store& s : std::get<1>(kv).vec())
                    if (std::get<1>(s)) _shrink(std::get<0>(s));
            shrunk = true;
            continue;
        }
        int64_t need = total - int_of_(static_cast<double>(cap) * 0.8), got = 0;
        std::vector<Gone> gone;
        for (const Old& o : _oldest(stores)) {
            if (got >= need) break;
            Gone* g = nullptr;
            for (Gone& x : gone)
                if (x.db.has_value() == o.db.has_value() && (!o.db || *x.db == *o.db)) {
                    g = &x;
                    break;
                }
            if (!g) {
                gone.emplace_back();
                g = &gone.back();
                g->db = o.db;
            }
            if (o.db) g->rowids.push_back(o.rowid);
            else g->files.push_back(o.file);
            got += o.size;
        }
        if (gone.empty()) return freed;
        for (const Gone& g : gone) {
            if (!g.db) {
                for (const py::str& f : g.files) _remove(f);
                continue;
            }
            try {
                auto [db, mine] = _connection(*g.db);
                std::vector<std::vector<SqlParam>> rows;
                for (int64_t r : g.rowids) rows.push_back({SqlParam(r)});
                db->executemany("DELETE FROM entry WHERE rowid=?", rows);
                db->commit();
                if (mine) db->close();
            } catch (const SqliteError&) {
                continue;
            }
            _shrink(*g.db);
        }
        freed += got;
    }
    return freed;
}

}  // namespace

// Bring all of state's caches back under `cap` bytes (default the setting cache_max_gb), and
// the shared build cache under its own (build_cache_max_gb): past it, the results used longest
// ago go first, from any kind, until four fifths of the cap is used. -> bytes freed (as the
// results counted them)
int64_t hold(const std::optional<int64_t>& cap) {
    int64_t freed = 0;
    int64_t n = 0;
    const py::list<std::tuple<py::str, int64_t>> all = roots();
    for (const auto& r : all.vec()) {
        freed += _hold(std::get<0>(r), (!cap || n) ? std::get<1>(r) : *cap);
        n++;
    }
    return freed;
}

// --- seeing and emptying it -----------------------------------------------------------------------
// Every kind back to its generation of now (the code is imported to say which) and the cap
// held. -> [(kind, what was dropped)]
py::list<std::tuple<py::str, py::list<py::str>>> prune() {
    py::list<std::tuple<py::str, py::list<py::str>>> dropped;
    for (const auto& kv : _stores().items()) {
        const py::str& kind = std::get<0>(kv);
        std::optional<py::str> gen = generation(kind);
        if (!gen) continue;
        py::list<py::str> old;
        for (const Store& s : std::get<1>(kv).vec())
            if (first_part_(py::os::path::basename(std::get<0>(s))) != *gen) old.append(py::os::path::basename(std::get<0>(s)));
        if (!old.empty()) {
            _close(kind);
            _drop_others(py::os::path::join(root(), kind), *gen);
            dropped.append(std::make_tuple(kind, old));
        }
    }
    _remove(py::os::path::join(root(), DROPPED));
    hold();
    return dropped;
}

void _close(const std::optional<py::str>& kind) {
    std::lock_guard<std::recursive_mutex> guard(LOCK());
    std::vector<py::str> kinds = kind ? std::vector<py::str>{*kind} : _OPEN.keys();
    for (const py::str& k : kinds) {
        if (!_OPEN.contains(k)) continue;
        OpenEntry had = _OPEN.pop(k);
        if (had.db) {
            try {
                had.db->close();
            } catch (const SqliteError&) {
            }
        }
    }
    if (!kind) _FOLDERS.clear();
    else _FOLDERS.pop(*kind, py::str(""));
}

// Everything kept, gone. -> bytes freed
int64_t clear() {
    _close();
    int64_t total = sum_values_(sizes());
    for (const py::str& kind : py::os::listdir(root())) _remove(py::os::path::join(root(), kind));
    return total;
}

// [(kind, bytes, results, generations on disk, the generation of now)]
py::list<std::tuple<py::str, int64_t, int64_t, py::list<py::str>, std::optional<py::str>>> stats() {
    py::list<std::tuple<py::str, int64_t, int64_t, py::list<py::str>, std::optional<py::str>>> out;
    for (const auto& kv : _stores().items()) {
        const py::str& kind = std::get<0>(kv);
        int64_t rows = 0;
        for (const Store& s : std::get<1>(kv).vec()) {
            const py::str& p = std::get<0>(s);
            if (std::get<1>(s)) {
                try {
                    auto [db, mine] = _connection(p);
                    rows += std::get<int64_t>(db->query("SELECT count(*) FROM entry")[0][0]);
                    if (mine) db->close();
                } catch (const SqliteError&) {
                }
            } else {
                for (const auto& entry : py::os::walk(p)) rows += std::get<2>(entry).size();
            }
        }
        int64_t size = 0;
        py::list<py::str> gens;
        for (const Store& s : std::get<1>(kv).vec()) {
            size += _size(std::get<0>(s));
            gens.append(first_part_(py::os::path::basename(std::get<0>(s))));
        }
        out.append(std::make_tuple(kind, size, rows, gens, generation(kind)));
    }
    return out;
}

// python -m codebreaker cache [--stats | --prune | --clear]
int64_t main(const py::list<py::str>& argv) {
    bool has_clear = false, has_prune = false;
    for (const py::str& a : argv) {
        if (a == py::str("--clear")) has_clear = true;
        if (a == py::str("--prune")) has_prune = true;
    }
    if (has_clear) {
        py::print(py::percent(py::str("cache cleared: %s freed"), gb_(clear())));
        return 0;
    }
    if (has_prune) {
        const py::list<std::tuple<py::str, py::list<py::str>>> dropped = prune();
        for (const auto& d : dropped.vec()) {
            py::list<py::str> names = std::get<1>(d);
            py::print(py::percent(py::str("  %-16s dropped old generation(s) %s"), std::get<0>(d), py::join(py::str(", "), names)));
        }
    } else if (!argv.empty() && !(argv[0] == py::str("--stats"))) {
        std::string doc(DOC_TEXT);       // __doc__.split("\n\n")[1]
        size_t a = doc.find("\n\n");
        size_t b = doc.find("\n\n", a + 2);
        py::print(py::str(doc.substr(a + 2, b - a - 2)));
        return 2;
    }
    int64_t total = 0;
    py::print(py::percent(py::str("%-16s %10s %10s  %s"), py::str("kind"), py::str("size"), py::str("results"),
                          py::str("generations (* = of now)")));
    const auto shown_stats = stats();
    for (const auto& s : shown_stats.vec()) {
        const py::str& kind = std::get<0>(s);
        int64_t size = std::get<1>(s), rows = std::get<2>(s);
        const py::list<py::str>& gens = std::get<3>(s);
        const std::optional<py::str>& now = std::get<4>(s);
        total += size;
        py::list<py::str> shown;
        for (const py::str& g : gens) shown.append(g + ((now && g == *now) ? py::str("*") : py::str("")));
        py::print(py::percent(py::str("%-16s %10s %10d  %s"), kind, gb_(size), rows, py::join(py::str(" "), shown)));
    }
    py::print(py::percent(py::str("%-16s %10s   (cap %s: setting cache_max_gb)"), py::str("all"), gb_(total), gb_(max_bytes())));
    return 0;
}

// --- pickle ---------------------------------------------------------------------------------------
namespace pickle {

void put_op(std::string& out, char op) { out += op; }
void put_mark(std::string& out) { out += '('; }

static void le_(std::string& out, uint64_t v, int n) {
    for (int i = 0; i < n; i++) out += static_cast<char>((v >> (8 * i)) & 0xFF);
}

void put_int(std::string& out, int64_t n) {
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

void put_float(std::string& out, double d) {
    uint64_t bits;
    std::memcpy(&bits, &d, 8);
    out += 'G';
    for (int i = 7; i >= 0; i--) out += static_cast<char>((bits >> (8 * i)) & 0xFF);
}

void put_str(std::string& out, const py::str& s) {
    std::string u = encode_replace_(s);
    out += 'X';
    le_(out, u.size(), 4);
    out += u;
}

void put_bytes(std::string& out, const py::bytes& b) {
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
std::optional<Node> loads(const py::bytes& data) {
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
