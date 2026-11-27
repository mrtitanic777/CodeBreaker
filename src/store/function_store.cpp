#include "store/function_store.hpp"

#include <algorithm>
#include <chrono>
#include <mutex>
#include <set>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "base/sqlite.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/text.hpp"

namespace cb::store {

namespace sql = cb::base::sqlite;
namespace stdfs = std::filesystem;

namespace {

constexpr std::chrono::milliseconds kBusy = std::chrono::seconds(60);

// The index: one row per module, per profile, and the summaries every write brings up to date.
const char* const kIndexSchema[] = {
    "CREATE TABLE IF NOT EXISTS module(sha TEXT PRIMARY KEY, name TEXT, path TEXT, kind TEXT, ts REAL)",
    "CREATE TABLE IF NOT EXISTS profile(digest TEXT PRIMARY KEY, settings TEXT, ts REAL)",
    "CREATE TABLE IF NOT EXISTS summary(module TEXT PRIMARY KEY, functions INTEGER, identified INTEGER, judged INTEGER, last REAL,"
    " last_profile TEXT, profiles TEXT, latest TEXT)",
    "CREATE TABLE IF NOT EXISTS store_about(name TEXT PRIMARY KEY, value TEXT)"};

// A module's tables, in its program database.
const char* const kModuleSchema[] = {
    "CREATE TABLE IF NOT EXISTS function(id INTEGER PRIMARY KEY, module TEXT NOT NULL, addr TEXT NOT NULL, name TEXT, size INTEGER,"
    " UNIQUE(module, addr))",
    "CREATE TABLE IF NOT EXISTS candidate(id INTEGER PRIMARY KEY, function INTEGER NOT NULL, engine TEXT NOT NULL, rev TEXT, sig TEXT,"
    " name TEXT, source TEXT, problem TEXT, ts REAL, UNIQUE(function, engine))",
    "CREATE TABLE IF NOT EXISTS verdict(candidate INTEGER NOT NULL, profile TEXT NOT NULL, sig TEXT, verdict TEXT, reason TEXT,"
    " limits TEXT, detail TEXT, ts REAL, carried INTEGER NOT NULL DEFAULT 0, PRIMARY KEY (candidate, profile))",
    "CREATE TABLE IF NOT EXISTS text(module TEXT NOT NULL, engine TEXT NOT NULL, kind TEXT NOT NULL, body TEXT, ts REAL,"
    " PRIMARY KEY (module, engine, kind))",
    "CREATE INDEX IF NOT EXISTS candidate_function ON candidate(function)",
    "CREATE TABLE IF NOT EXISTS identified(module TEXT NOT NULL, addr TEXT NOT NULL, name TEXT, library TEXT, ts REAL,"
    " PRIMARY KEY (module, addr))",
    "CREATE TABLE IF NOT EXISTS built(module TEXT NOT NULL, addr TEXT NOT NULL, profile TEXT NOT NULL, sig TEXT,"
    " PRIMARY KEY (module, addr, profile))",
    "CREATE TABLE IF NOT EXISTS variant(id INTEGER PRIMARY KEY, function INTEGER NOT NULL, compiler TEXT NOT NULL, source TEXT,"
    " parent INTEGER, knobs TEXT, distance INTEGER, total INTEGER, verdict TEXT, behaves TEXT, ts REAL,"
    " UNIQUE(function, compiler, knobs))",
    "CREATE INDEX IF NOT EXISTS variant_function ON variant(function)"};

// Each function's latest verdict under any profile: (addr, verdict, limits).
const char* const kLatestSql =
    "SELECT addr, verdict, limits FROM (SELECT f.addr AS addr, v.verdict AS verdict, v.limits AS limits,"
    " ROW_NUMBER() OVER (PARTITION BY f.id ORDER BY v.ts DESC) AS k"
    " FROM function f JOIN candidate c ON c.function = f.id JOIN verdict v ON v.candidate = c.id WHERE f.module = ?) WHERE k = 1";

// ---- values
sql::Value txt(std::string_view s) { return std::string(s); }
sql::Value text_or_null(const std::optional<std::string>& s) { return s ? sql::Value(*s) : sql::Value(); }
sql::Value text_or_null(const std::optional<std::string_view>& s) { return s ? sql::Value(std::string(*s)) : sql::Value(); }
sql::Value int_or_null(const std::optional<std::int64_t>& v) { return v ? sql::Value(*v) : sql::Value(); }
sql::Value real(double v) { return v; }

double now_seconds() { return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count(); }

// A column read as text: a number somebody stored in a text column reads as its digits.
std::optional<std::string> as_text(const sql::Value& v) {
    if (const auto* s = std::get_if<std::string>(&v)) return *s;
    if (const auto* i = std::get_if<std::int64_t>(&v)) return std::to_string(*i);
    if (const auto* d = std::get_if<double>(&v)) return text::format_double(*d);
    if (const auto* b = std::get_if<Bytes>(&v)) return std::string(b->begin(), b->end());
    return std::nullopt;
}
std::string as_string(const sql::Value& v) { return as_text(v).value_or(""); }
std::optional<std::int64_t> as_int(const sql::Value& v) {
    if (const auto* i = std::get_if<std::int64_t>(&v)) return *i;
    if (const auto* d = std::get_if<double>(&v)) return static_cast<std::int64_t>(*d);
    return std::nullopt;
}
std::int64_t as_count(const sql::Value& v) { return as_int(v).value_or(0); }
std::optional<double> as_real(const sql::Value& v) {
    if (const auto* d = std::get_if<double>(&v)) return *d;
    if (const auto* i = std::get_if<std::int64_t>(&v)) return static_cast<double>(*i);
    return std::nullopt;
}

// A judge's word as it is stored now; a word the vocabulary does not know is kept as it is.
std::optional<std::string> verdict_word(const std::optional<std::string>& stored) {
    if (!stored) return std::nullopt;
    if (const auto v = store::parse_verdict(*stored)) return std::string(store::word(*v));
    return stored;
}
std::optional<std::string> match_word(const std::optional<std::string>& stored) {
    if (!stored) return std::nullopt;
    if (const auto m = store::parse_byte_match(*stored)) return std::string(store::word(*m));
    return stored;
}

store::Classification classify_row(const std::optional<std::string>& verdict, std::string_view limits) {
    std::optional<store::Verdict> v;
    if (verdict) v = store::parse_verdict(*verdict);
    return store::classify(v, limits);
}

// The comparison "the sig it was judged at is the sig it has": two absent ones are the same.
bool same_sig(const std::optional<std::string>& a, const std::optional<std::string>& b) { return a == b; }

}  // namespace

store::Classification Candidate::classification() const { return classify_row(verdict, limits); }

// One module's open database, and the lock that keeps one statement run at a time through a write of several.
struct FunctionStoreModule {
    std::optional<sql::Database> db;   // empty: a read-only store with no such database
    std::mutex mu;
};

struct FunctionStore::Impl {
    stdfs::path index_path, folder;
    bool readonly = false;
    std::optional<sql::Database> index;   // empty: a read-only store with no index
    std::mutex index_mu;
    std::mutex modules_mu;
    std::unordered_map<std::string, std::shared_ptr<FunctionStoreModule>> modules;   // lookup only
};

namespace {

using Impl = FunctionStore::Impl;

[[noreturn]] void read_only_error(const stdfs::path& p) { throw IoError(p, 0, "attempt to write a readonly store"); }

// The module's database, opened (and its tables made) once.
std::shared_ptr<FunctionStoreModule> module_db(Impl& impl, std::string_view module) {
    std::lock_guard<std::mutex> lock(impl.modules_mu);
    const auto found = impl.modules.find(std::string(module));
    if (found != impl.modules.end()) return found->second;
    const stdfs::path file = impl.folder / (std::string(module) + ".sqlite");
    auto made = std::make_shared<FunctionStoreModule>();
    std::error_code ec;
    if (impl.readonly) {
        if (stdfs::is_regular_file(file, ec)) {
            made->db.emplace(sql::Database::open(file, kBusy));
            made->db->exec("PRAGMA query_only=ON");
        }
    } else {
        fs::make_dirs(impl.folder);
        made->db.emplace(sql::Database::open(file, kBusy));
        (void)made->db->query("PRAGMA journal_mode=WAL");
        made->db->exec("PRAGMA synchronous=NORMAL");
        sql::Database::Transaction tx(*made->db);
        for (const char* stmt : kModuleSchema) made->db->exec(stmt);
        bool carried = false;   // a verdict table from before `carried` is brought up to date
        for (const sql::Row& r : made->db->query("PRAGMA table_info(verdict)")) carried = carried || as_string(r[1]) == "carried";
        if (!carried) made->db->exec("ALTER TABLE verdict ADD COLUMN carried INTEGER NOT NULL DEFAULT 0");
        tx.commit();
    }
    impl.modules.emplace(std::string(module), made);
    return made;
}

std::vector<sql::Row> module_rows_of(Impl& impl, std::string_view module, std::string_view sql_text, const std::vector<sql::Value>& args) {
    const auto m = module_db(impl, module);
    std::lock_guard<std::mutex> lock(m->mu);
    if (!m->db) return {};
    return m->db->query(sql_text, args);
}

std::optional<sql::Row> module_one(Impl& impl, std::string_view module, std::string_view sql_text, const std::vector<sql::Value>& args) {
    std::vector<sql::Row> rows = module_rows_of(impl, module, sql_text, args);
    if (rows.empty()) return std::nullopt;
    return std::move(rows.front());
}

// The id of a function, made when the module has none at that address.
std::int64_t function_id(sql::Database& db, std::string_view module, std::string_view addr, const std::optional<std::string>& name) {
    const auto found = db.query("SELECT id FROM function WHERE module=? AND addr=?", {txt(module), txt(addr)});
    if (!found.empty()) return *as_int(found[0][0]);
    db.exec("INSERT INTO function(module, addr, name, size) VALUES(?,?,?,NULL)", {txt(module), txt(addr), text_or_null(name)});
    return *as_int(db.query("SELECT id FROM function WHERE module=? AND addr=?", {txt(module), txt(addr)})[0][0]);
}

// What a write needs: the module's database, for writing.
std::shared_ptr<FunctionStoreModule> writable(Impl& impl, std::string_view module) {
    if (impl.readonly) read_only_error(impl.index_path);
    return module_db(impl, module);
}

void write_index(Impl& impl, std::string_view statement, const std::vector<sql::Value>& args) {
    if (impl.readonly || !impl.index) read_only_error(impl.index_path);
    std::lock_guard<std::mutex> lock(impl.index_mu);
    sql::Database::Transaction tx(*impl.index);
    impl.index->exec(statement, args);
    tx.commit();
}

std::vector<sql::Row> index_rows(Impl& impl, std::string_view statement, const std::vector<sql::Value>& args = {}) {
    if (!impl.index) return {};
    std::lock_guard<std::mutex> lock(impl.index_mu);
    return impl.index->query(statement, args);
}

Json to_json(const ClassCount& counts) {
    Json j = Json::object();
    for (const auto& [cls, n] : counts) j[std::string(store::word(cls))] = n;
    return j;
}

ClassCount class_count_from(const std::string& json_text) {
    ClassCount out;
    Json j;
    if (json_text.empty() || !json::try_parse(json_text, j) || !j.is_object()) return out;
    for (const auto& [k, v] : j.items()) {
        if (const auto cls = store::parse_class(k); cls && v.is_number_integer()) out[*cls] = v.get<std::int64_t>();
    }
    return out;
}

// The module's summary in the index, from its database: the counts every write keeps current.
void summarize(Impl& impl, std::string_view module) {
    if (impl.readonly || !impl.index) return;
    Summary s;
    std::optional<std::string> last_profile;
    const std::string joined = "FROM verdict v JOIN candidate c ON c.id=v.candidate JOIN function f ON f.id=c.function WHERE f.module=?";
    const std::vector<sql::Value> args = {txt(module)};
    s.functions = as_count((*module_one(impl, module, "SELECT COUNT(*) FROM function WHERE module=?", args))[0]);
    s.identified = as_count((*module_one(impl, module, "SELECT COUNT(*) FROM identified WHERE module=?", args))[0]);
    const sql::Row counts = *module_one(impl, module, "SELECT COUNT(*), MAX(v.ts) " + joined, args);
    s.judged = as_count(counts[0]);
    s.last = as_real(counts[1]);
    if (const auto lp = module_one(impl, module, "SELECT v.profile " + joined + " ORDER BY v.ts DESC LIMIT 1", args)) last_profile = as_text((*lp)[0]);
    std::set<std::string> profiles;
    for (const sql::Row& r : module_rows_of(impl, module, "SELECT DISTINCT v.profile " + joined, args)) profiles.insert(as_string(r[0]));
    for (const sql::Row& r : module_rows_of(impl, module, kLatestSql, args)) {
        const std::optional<std::string> v = verdict_word(as_text(r[1]));
        s.latest[classify_row(v, as_string(r[2])).cls]++;
    }
    Json plist = Json::array();
    for (const std::string& p : profiles) plist.push_back(p);
    write_index(impl,
                "INSERT OR REPLACE INTO summary(module, functions, identified, judged, last, last_profile, profiles, latest)"
                " VALUES(?,?,?,?,?,?,?,?)",
                {txt(module), s.functions, s.identified, s.judged, s.last ? sql::Value(*s.last) : sql::Value(), text_or_null(last_profile),
                 plist.dump(), to_json(s.latest).dump()});
}

Candidate candidate_from(const sql::Row& r) {
    Candidate c;
    c.addr = as_string(r[0]);
    c.engine = as_string(r[1]);
    c.rev = as_text(r[2]);
    c.sig = as_text(r[3]);
    c.name = as_text(r[4]);
    c.source = as_text(r[5]);
    c.problem = as_text(r[6]);
    c.verdict = verdict_word(as_text(r[7]));
    c.reason = as_string(r[8]);
    c.limits = as_string(r[9]);
    c.detail = as_string(r[10]);
    c.judged_sig = as_text(r[11]);
    return c;
}

Variant variant_from(const sql::Row& r) {
    Variant v;
    v.addr = as_string(r[0]);
    v.compiler = as_string(r[1]);
    v.source = as_text(r[2]);
    v.parent = as_int(r[3]);
    v.knobs = as_string(r[4]);
    v.distance = as_int(r[5]);
    v.total = as_int(r[6]);
    v.match = match_word(as_text(r[7]));
    v.behaves = verdict_word(as_text(r[8]));
    return v;
}

}  // namespace

// ---- opening
FunctionStore::FunctionStore(stdfs::path index_file, stdfs::path folder, bool readonly) : impl_(std::make_unique<Impl>()) {
    impl_->index_path = std::move(index_file);
    impl_->folder = std::move(folder);
    impl_->readonly = readonly;
    std::error_code ec;
    if (readonly) {
        if (stdfs::is_regular_file(impl_->index_path, ec)) {
            impl_->index.emplace(sql::Database::open(impl_->index_path, kBusy));
            impl_->index->exec("PRAGMA query_only=ON");
        }
        return;
    }
    fs::make_dirs(impl_->index_path.parent_path());
    impl_->index.emplace(sql::Database::open(impl_->index_path, kBusy));
    (void)impl_->index->query("PRAGMA journal_mode=WAL");
    impl_->index->exec("PRAGMA synchronous=NORMAL");
    for (const char* stmt : kIndexSchema) impl_->index->exec(stmt);
}

FunctionStore::~FunctionStore() = default;

const stdfs::path& FunctionStore::path() const { return impl_->index_path; }
const stdfs::path& FunctionStore::folder() const { return impl_->folder; }
bool FunctionStore::readonly() const { return impl_->readonly; }

stdfs::path FunctionStore::database_path(std::string_view module) const { return impl_->folder / (std::string(module) + ".sqlite"); }

std::optional<stdfs::path> FunctionStore::module_database(std::string_view module) {
    const auto m = module_db(*impl_, module);
    if (!m->db) return std::nullopt;
    return database_path(module);
}

// ---- writing
void FunctionStore::add_module(std::string_view sha, std::string_view name, std::string_view path, std::string_view kind) {
    write_index(*impl_,
                "INSERT INTO module(sha, name, path, kind, ts) VALUES(?,?,?,?,?) ON CONFLICT(sha) DO UPDATE SET name=excluded.name,"
                " path=excluded.path, kind=excluded.kind",
                {txt(sha), txt(name), txt(path), txt(kind), real(now_seconds())});
    (void)module_db(*impl_, sha);   // its database, with the tables
}

void FunctionStore::add_profile(std::string_view digest, std::string_view settings) {
    write_index(*impl_, "INSERT OR IGNORE INTO profile(digest, settings, ts) VALUES(?,?,?)", {txt(digest), txt(settings), real(now_seconds())});
}

void FunctionStore::add_functions(std::string_view module, std::span<const std::string> addrs) {
    const auto m = writable(*impl_, module);
    {
        std::lock_guard<std::mutex> lock(m->mu);
        sql::Database::Transaction tx(*m->db);
        for (const std::string& addr : addrs) function_id(*m->db, module, addr, std::nullopt);
        tx.commit();
    }
    summarize(*impl_, module);
}

void FunctionStore::record_many(std::string_view module, std::string_view engine, std::optional<std::string_view> rev,
                                std::string_view profile, std::span<const Record> records) {
    const auto m = writable(*impl_, module);
    const double t = now_seconds();
    {
        std::lock_guard<std::mutex> lock(m->mu);
        sql::Database::Transaction tx(*m->db);
        for (const Record& r : records) {
            const std::int64_t f = function_id(*m->db, module, r.addr, r.name);
            const auto old = m->db->query("SELECT id, sig FROM candidate WHERE function=? AND engine=?", {f, txt(engine)});
            std::int64_t cid;
            if (!old.empty()) {
                cid = *as_int(old[0][0]);
                // a candidate whose sig changed is another candidate: its verdicts under other profiles no longer say anything
                if (!same_sig(as_text(old[0][1]), r.sig)) m->db->exec("DELETE FROM verdict WHERE candidate=? AND profile<>?", {cid, txt(profile)});
                m->db->exec("UPDATE candidate SET rev=?, sig=?, name=?, source=?, problem=?, ts=? WHERE id=?",
                            {text_or_null(rev), text_or_null(r.sig), text_or_null(r.name), text_or_null(r.source), text_or_null(r.problem), t, cid});
            } else {
                m->db->exec("INSERT INTO candidate(function, engine, rev, sig, name, source, problem, ts) VALUES(?,?,?,?,?,?,?,?)",
                            {f, txt(engine), text_or_null(rev), text_or_null(r.sig), text_or_null(r.name), text_or_null(r.source),
                             text_or_null(r.problem), t});
                cid = *as_int(m->db->query("SELECT id FROM candidate WHERE function=? AND engine=?", {f, txt(engine)})[0][0]);
            }
            m->db->exec("INSERT OR REPLACE INTO verdict(candidate, profile, sig, verdict, reason, limits, detail, ts, carried)"
                        " VALUES(?,?,?,?,?,?,?,?,0)",
                        {cid, txt(profile), text_or_null(r.sig), text_or_null(verdict_word(r.verdict)), txt(r.reason), txt(r.limits),
                         txt(r.detail), t});
        }
        tx.commit();
    }
    summarize(*impl_, module);
}

void FunctionStore::record(std::string_view module, std::string_view engine, std::optional<std::string_view> rev,
                           std::string_view profile, const Record& record) {
    record_many(module, engine, rev, profile, std::span<const Record>(&record, 1));
}

void FunctionStore::put_text(std::string_view module, std::string_view engine, std::string_view kind, std::string_view body) {
    const auto m = writable(*impl_, module);
    std::lock_guard<std::mutex> lock(m->mu);
    sql::Database::Transaction tx(*m->db);
    m->db->exec("INSERT OR REPLACE INTO text(module, engine, kind, body, ts) VALUES(?,?,?,?,?)",
                {txt(module), txt(engine), txt(kind), txt(body), real(now_seconds())});
    tx.commit();
}

void FunctionStore::set_built(std::string_view module, std::string_view profile, const std::map<std::string, std::string>& sigs) {
    const auto m = writable(*impl_, module);
    std::lock_guard<std::mutex> lock(m->mu);
    sql::Database::Transaction tx(*m->db);
    for (const auto& [addr, sig] : sigs) {
        m->db->exec("INSERT OR REPLACE INTO built(module, addr, profile, sig) VALUES(?,?,?,?)", {txt(module), txt(addr), txt(profile), txt(sig)});
    }
    tx.commit();
}

void FunctionStore::set_identified(std::string_view module, const std::map<std::string, std::pair<std::string, std::string>>& found) {
    const auto m = writable(*impl_, module);
    const double t = now_seconds();
    {
        std::lock_guard<std::mutex> lock(m->mu);
        sql::Database::Transaction tx(*m->db);
        m->db->exec("DELETE FROM identified WHERE module=?", {txt(module)});
        for (const auto& [addr, who] : found) {
            m->db->exec("INSERT INTO identified(module, addr, name, library, ts) VALUES(?,?,?,?,?)",
                        {txt(module), txt(addr), txt(who.first), txt(who.second), t});
        }
        tx.commit();
    }
    summarize(*impl_, module);
}

std::int64_t FunctionStore::record_variant(std::string_view module, std::string_view addr, std::string_view compiler, std::string_view source,
                                           std::optional<std::int64_t> distance, std::optional<std::int64_t> total, std::string_view match,
                                           std::string_view knobs, std::optional<std::int64_t> parent, std::string_view behaves) {
    const auto m = writable(*impl_, module);
    std::lock_guard<std::mutex> lock(m->mu);
    sql::Database::Transaction tx(*m->db);
    const std::int64_t f = function_id(*m->db, module, addr, std::nullopt);
    m->db->exec("INSERT INTO variant(function, compiler, source, parent, knobs, distance, total, verdict, behaves, ts) VALUES(?,?,?,?,?,?,?,?,?,?)"
                " ON CONFLICT(function, compiler, knobs) DO UPDATE SET source=excluded.source, parent=excluded.parent,"
                " distance=excluded.distance, total=excluded.total, verdict=excluded.verdict, behaves=excluded.behaves, ts=excluded.ts",
                {f, txt(compiler), txt(source), int_or_null(parent), txt(knobs), int_or_null(distance), int_or_null(total),
                 text_or_null(match_word(std::string(match))), text_or_null(verdict_word(std::string(behaves))), real(now_seconds())});
    const auto id = m->db->query("SELECT id FROM variant WHERE function=? AND compiler=? AND knobs=?", {f, txt(compiler), txt(knobs)});
    tx.commit();
    return *as_int(id[0][0]);
}

// ---- reading a module
std::vector<Candidate> FunctionStore::candidates(std::string_view module, std::string_view profile, std::optional<std::string_view> engine) {
    std::string q =
        "SELECT f.addr, c.engine, c.rev, c.sig, c.name, c.source, c.problem, v.verdict, v.reason, v.limits, v.detail, v.sig"
        " FROM function f JOIN candidate c ON c.function=f.id LEFT JOIN verdict v ON v.candidate=c.id AND v.profile=? WHERE f.module=?";
    std::vector<sql::Value> args = {txt(profile), txt(module)};
    if (engine && !engine->empty()) {
        q += " AND c.engine=?";
        args.push_back(txt(*engine));
    }
    std::vector<Candidate> out;
    for (const sql::Row& r : module_rows_of(*impl_, module, q, args)) out.push_back(candidate_from(r));
    return out;
}

std::map<std::string, Candidate> FunctionStore::judged(std::string_view module, std::string_view profile, std::string_view engine) {
    std::map<std::string, Candidate> out;
    for (Candidate& c : candidates(module, profile, engine)) {
        if (c.verdict && same_sig(c.judged_sig, c.sig)) out[c.addr] = std::move(c);
    }
    return out;
}

std::vector<Stored> FunctionStore::stored(std::string_view module, std::string_view addr) {
    std::vector<Stored> out;
    for (const sql::Row& c : module_rows_of(*impl_, module,
                                            "SELECT c.id, c.engine, c.rev, c.name, c.source, c.problem FROM candidate c"
                                            " JOIN function f ON f.id=c.function WHERE f.module=? AND f.addr=?",
                                            {txt(module), txt(addr)})) {
        Stored s;
        s.engine = as_string(c[1]);
        s.rev = as_text(c[2]);
        s.name = as_text(c[3]);
        s.source = as_string(c[4]);
        s.problem = as_string(c[5]);
        for (const sql::Row& v : module_rows_of(*impl_, module,
                                                "SELECT profile, verdict, reason, limits, carried FROM verdict WHERE candidate=? ORDER BY ts DESC",
                                                {c[0]})) {
            s.verdicts.push_back({as_string(v[0]), verdict_word(as_text(v[1])), as_string(v[2]), as_string(v[3]), as_count(v[4]) != 0});
        }
        out.push_back(std::move(s));
    }
    return out;
}

std::string FunctionStore::get_text(std::string_view module, std::string_view engine, std::string_view kind) {
    const auto r = module_one(*impl_, module, "SELECT body FROM text WHERE module=? AND engine=? AND kind=?", {txt(module), txt(engine), txt(kind)});
    return r ? as_string((*r)[0]) : std::string();
}

std::map<std::string, std::optional<std::string>> FunctionStore::built(std::string_view module, std::string_view profile) {
    std::map<std::string, std::optional<std::string>> out;
    for (const sql::Row& r : module_rows_of(*impl_, module, "SELECT addr, sig FROM built WHERE module=? AND profile=?", {txt(module), txt(profile)})) {
        out[as_string(r[0])] = as_text(r[1]);
    }
    return out;
}

std::map<std::string, Best> FunctionStore::best(std::string_view module, std::string_view profile, std::span<const std::string> prefer) {
    // the place of an engine in `prefer` (a name given twice has its last place); an engine not named comes after them all
    std::map<std::string, std::size_t, std::less<>> rank;
    for (std::size_t i = 0; i < prefer.size(); ++i) rank[prefer[i]] = i;
    struct Key {
        std::size_t cls, rank;
        bool operator<(const Key& o) const { return cls != o.cls ? cls < o.cls : rank < o.rank; }
    };
    std::map<std::string, std::pair<Key, Best>> chosen;
    for (Candidate& c : candidates(module, profile)) {
        if (!c.verdict) continue;   // not judged under this profile
        const store::Classification k = c.classification();
        Best b;
        b.cls = k.cls;
        b.weak = k.reasons;
        if (b.cls != store::Class::NoSource && (!c.source || c.source->empty())) b.cls = store::Class::NoSource;   // no source to speak of
        const auto order = std::find(store::kClassOrder.begin(), store::kClassOrder.end(), b.cls);
        const auto at = rank.find(c.engine);
        const Key key{static_cast<std::size_t>(order - store::kClassOrder.begin()), at == rank.end() ? rank.size() : at->second};
        const auto have = chosen.find(c.addr);
        if (have == chosen.end() || key < have->second.first) {
            b.candidate = std::move(c);
            const std::string addr = b.candidate.addr;
            chosen[addr] = {key, std::move(b)};
        }
    }
    std::map<std::string, Best> out;
    for (auto& [addr, kb] : chosen) out.emplace(addr, std::move(kb.second));
    return out;
}

std::map<std::string, store::Class> FunctionStore::latest(std::string_view module) {
    std::map<std::string, store::Class> out;
    for (const sql::Row& r : module_rows_of(*impl_, module, kLatestSql, {txt(module)})) {
        out[as_string(r[0])] = classify_row(verdict_word(as_text(r[1])), as_string(r[2])).cls;
    }
    return out;
}

std::vector<std::string> FunctionStore::addresses(std::string_view module) {
    std::vector<std::string> out;
    for (const sql::Row& r : module_rows_of(*impl_, module, "SELECT addr FROM function WHERE module=? ORDER BY addr", {txt(module)})) {
        out.push_back(as_string(r[0]));
    }
    return out;
}

std::map<std::string, std::pair<std::optional<std::string>, std::optional<std::string>>> FunctionStore::identified(std::string_view module) {
    std::map<std::string, std::pair<std::optional<std::string>, std::optional<std::string>>> out;
    for (const sql::Row& r : module_rows_of(*impl_, module, "SELECT addr, name, library FROM identified WHERE module=?", {txt(module)})) {
        out[as_string(r[0])] = {as_text(r[1]), as_text(r[2])};
    }
    return out;
}

ClassCount FunctionStore::tally(std::string_view profile, std::string_view module, std::span<const std::string> prefer) {
    ClassCount counts;
    for (const store::Class c : store::kClassOrder) counts[c] = 0;
    const std::map<std::string, Best> best_of = best(module, profile, prefer);
    for (const std::string& addr : addresses(module)) {
        const auto it = best_of.find(addr);
        counts[it != best_of.end() ? it->second.cls : store::Class::NoSource]++;
    }
    return counts;
}

ClassCount FunctionStore::scores(std::string_view profile, std::string_view module, std::span<const std::string> prefer) {
    ClassCount counts;
    for (const store::Class c : store::kClassScored) counts[c] = 0;
    const std::map<std::string, Best> best_of = best(module, profile, prefer);
    const auto known = identified(module);
    for (const std::string& addr : addresses(module)) {
        const auto it = best_of.find(addr);
        store::Class c = it != best_of.end() ? it->second.cls : store::Class::NoSource;
        // a function recognised as a runtime library's is counted on its own, unless it was also verified (the stronger statement)
        if (known.contains(addr) && c != store::Class::Verified && c != store::Class::Weak) c = store::Class::Identified;
        counts[c]++;
    }
    return counts;
}

std::vector<Variant> FunctionStore::variants(std::string_view module, std::optional<std::string_view> compiler) {
    std::string q =
        "SELECT f.addr, v.compiler, v.source, v.parent, v.knobs, v.distance, v.total, v.verdict, v.behaves"
        " FROM variant v JOIN function f ON f.id=v.function WHERE f.module=?";
    std::vector<sql::Value> args = {txt(module)};
    if (compiler && !compiler->empty()) {
        q += " AND v.compiler=?";
        args.push_back(txt(*compiler));
    }
    std::vector<Variant> out;
    for (const sql::Row& r : module_rows_of(*impl_, module, q, args)) out.push_back(variant_from(r));
    return out;
}

std::map<std::string, Variant> FunctionStore::best_variant(std::string_view module, std::string_view compiler) {
    constexpr std::int64_t kNoDistance = std::int64_t{1} << 30;
    std::map<std::string, std::pair<std::pair<int, std::int64_t>, Variant>> chosen;
    for (Variant& v : variants(module, compiler)) {
        const bool behaves = v.behaves && (*v.behaves == "equivalent" || *v.behaves == "consistent" || v.behaves->empty());
        const std::pair<int, std::int64_t> key{behaves ? 0 : 1, v.distance.value_or(kNoDistance)};
        const auto have = chosen.find(v.addr);
        if (have == chosen.end() || key < have->second.first) {
            const std::string addr = v.addr;
            chosen[addr] = {key, std::move(v)};
        }
    }
    std::map<std::string, Variant> out;
    for (auto& [addr, kv] : chosen) out.emplace(addr, std::move(kv.second));
    return out;
}

FunctionStore::MatchTally FunctionStore::match_tally(std::string_view module, std::string_view compiler) {
    MatchTally t;
    for (const auto& [addr, v] : best_variant(module, compiler)) {
        ++t.functions;
        if (v.match == std::string(store::word(store::ByteMatch::Identical))) ++t.identical;
        if (v.match == std::string(store::word(store::ByteMatch::SameSize))) ++t.same_size;
    }
    t.other = t.functions - t.identical - t.same_size;
    return t;
}

// ---- across modules
std::map<std::string, Summary> FunctionStore::summaries() {
    std::map<std::string, Summary> out;
    for (const sql::Row& r : index_rows(*impl_, "SELECT module, functions, identified, judged, last, last_profile, profiles, latest FROM summary")) {
        Summary s;
        s.functions = as_count(r[1]);
        s.identified = as_count(r[2]);
        s.judged = as_count(r[3]);
        s.last = as_real(r[4]);
        s.last_profile = as_text(r[5]);
        Json profiles;
        if (json::try_parse(as_string(r[6]), profiles) && profiles.is_array()) {
            for (const Json& p : profiles) {
                if (p.is_string()) s.profiles.push_back(p.get<std::string>());
            }
        }
        s.latest = class_count_from(as_string(r[7]));
        out.emplace(as_string(r[0]), std::move(s));
    }
    return out;
}

std::vector<ModuleRow> FunctionStore::modules(std::optional<std::string_view> profile) {
    std::set<std::string> judged_under;
    if (profile) {
        for (const auto& [sha, s] : summaries()) {
            if (std::find(s.profiles.begin(), s.profiles.end(), *profile) != s.profiles.end()) judged_under.insert(sha);
        }
    }
    std::vector<ModuleRow> out;
    for (const sql::Row& r : index_rows(*impl_, "SELECT sha, name FROM module ORDER BY name")) {
        ModuleRow m{as_string(r[0]), as_string(r[1]), "", ""};
        if (!profile || judged_under.contains(m.sha)) out.push_back(std::move(m));
    }
    return out;
}

std::vector<ModuleRow> FunctionStore::module_rows(std::optional<std::string_view> kind) {
    std::vector<ModuleRow> out;
    const auto rows = kind ? index_rows(*impl_, "SELECT sha, name, path, kind FROM module WHERE kind=? ORDER BY name", {txt(*kind)})
                           : index_rows(*impl_, "SELECT sha, name, path, kind FROM module ORDER BY name");
    for (const sql::Row& r : rows) out.push_back({as_string(r[0]), as_string(r[1]), as_string(r[2]), as_string(r[3])});
    return out;
}

std::optional<std::string> FunctionStore::module_path(std::string_view name) {
    const auto rows = index_rows(*impl_, "SELECT path FROM module WHERE name=? ORDER BY ts DESC LIMIT 1", {txt(name)});
    if (rows.empty()) return std::nullopt;
    return as_text(rows[0][0]);
}

std::optional<std::string> FunctionStore::latest_profile(std::string_view kind) {
    const auto rows = index_rows(*impl_,
                                 "SELECT s.last_profile FROM summary s JOIN module m ON m.sha=s.module WHERE m.kind=? AND s.last IS NOT NULL"
                                 " ORDER BY s.last DESC LIMIT 1",
                                 {txt(kind)});
    if (rows.empty()) return std::nullopt;
    return as_text(rows[0][0]);
}

std::vector<RecentModule> FunctionStore::recent(std::int64_t limit) {
    std::vector<RecentModule> out;
    for (const sql::Row& r : index_rows(*impl_,
                                        "SELECT m.name, m.kind, s.judged, s.last FROM summary s JOIN module m ON m.sha=s.module"
                                        " WHERE s.last IS NOT NULL ORDER BY s.last DESC LIMIT ?",
                                        {limit})) {
        out.push_back({as_string(r[0]), as_string(r[1]), as_count(r[2]), as_real(r[3]).value_or(0)});
    }
    return out;
}

std::int64_t FunctionStore::verdicts_since(double t) {
    std::int64_t n = 0;
    for (const sql::Row& r : index_rows(*impl_, "SELECT module FROM summary WHERE last > ?", {t})) {
        if (const auto c = module_one(*impl_, as_string(r[0]), "SELECT COUNT(*) FROM verdict WHERE ts > ?", {t})) n += as_count((*c)[0]);
    }
    return n;
}

std::vector<Overview> FunctionStore::overview() {
    const std::map<std::string, Summary> sums = summaries();
    std::vector<Overview> out;
    for (const ModuleRow& m : module_rows()) {
        Overview o;
        o.sha = m.sha;
        o.name = m.name;
        o.kind = m.kind;
        o.path = m.path;
        for (const store::Class c : store::kClassOrder) o.latest[c] = 0;
        if (const auto it = sums.find(m.sha); it != sums.end()) {
            o.functions = it->second.functions;
            o.identified = it->second.identified;
            for (const auto& [cls, n] : it->second.latest) o.latest[cls] = n;
        }
        std::int64_t sum = 0;
        for (const auto& [cls, n] : o.latest) sum += n;
        o.latest[store::Class::NoSource] += std::max<std::int64_t>(0, o.functions - sum);   // (functions with no verdict at all)
        out.push_back(std::move(o));
    }
    return out;
}

std::optional<std::string> FunctionStore::about(std::string_view name) {
    const auto rows = index_rows(*impl_, "SELECT value FROM store_about WHERE name=?", {txt(name)});
    if (rows.empty()) return std::nullopt;
    return as_text(rows[0][0]);
}

void FunctionStore::set_about(std::string_view name, std::string_view value) {
    write_index(*impl_, "INSERT OR REPLACE INTO store_about(name, value) VALUES(?,?)", {txt(name), txt(value)});
}

}  // namespace cb::store
