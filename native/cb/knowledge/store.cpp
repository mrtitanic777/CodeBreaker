// cb-unity: alone (helpers named for what they do here)
#include "cb/knowledge/store.hpp"
#include "cb/knowledge/package.hpp"

#include <algorithm>
#include <chrono>
#include <map>

#include "cb/base/sqlite.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"

namespace cb::knowledge {

namespace sql = cb::base::sqlite;

namespace {

// The tag of the `tag` column: the value is a `Value::encode()`.
constexpr const char* kTagValue = "K";
constexpr std::string_view kBodiesPlatform = "bodies";

// The knowledge base's tables. A whole kind's rows have a `seq` (the order they were made in); learned and given rows have
// none and come after.
constexpr const char* kSchema = R"(
CREATE TABLE IF NOT EXISTS knowledge(
    platform TEXT NOT NULL, kind TEXT NOT NULL, key TEXT NOT NULL, seq INTEGER,
    hash TEXT NOT NULL, tag TEXT NOT NULL, value BLOB, source TEXT, origin TEXT NOT NULL, made REAL,
    PRIMARY KEY (platform, kind, key));
CREATE TABLE IF NOT EXISTS loaded(
    platform TEXT NOT NULL, kind TEXT NOT NULL, stamp TEXT, source TEXT, made REAL,
    PRIMARY KEY (platform, kind));
CREATE TABLE IF NOT EXISTS module(sha TEXT PRIMARY KEY, name TEXT, platform TEXT);
CREATE TABLE IF NOT EXISTS body(
    module TEXT NOT NULL, addr TEXT NOT NULL, hash TEXT NOT NULL, name TEXT, source TEXT,
    PRIMARY KEY (module, addr));
CREATE INDEX IF NOT EXISTS body_hash ON body(hash);
CREATE TABLE IF NOT EXISTS body_name(hash TEXT PRIMARY KEY, name TEXT, source TEXT, ts REAL);
CREATE TABLE IF NOT EXISTS carried(what TEXT PRIMARY KEY, mark TEXT);
)";

double seconds_now() {
    return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
}

sql::Value text_param(std::string_view s) { return std::string(s); }

std::string cell_text(const sql::Value& v) {
    const std::string* s = std::get_if<std::string>(&v);
    return s != nullptr ? *s : std::string();
}

std::optional<std::string> cell_optional_text(const sql::Value& v) {
    const std::string* s = std::get_if<std::string>(&v);
    if (s == nullptr) return std::nullopt;
    return *s;
}

std::int64_t cell_int(const sql::Value& v) {
    const std::int64_t* i = std::get_if<std::int64_t>(&v);
    return i != nullptr ? *i : 0;
}

sql::Value optional_param(const std::optional<std::string>& s) {
    return s ? sql::Value(*s) : sql::Value(std::monostate{});
}

Value decode_cell(const sql::Value& blob) {
    const Bytes* b = std::get_if<Bytes>(&blob);
    if (b == nullptr) throw FormatError("knowledge store", 0, "a piece has no value");
    return Value::decode(*b);
}

std::string sha1_text(std::string_view text) {
    const Bytes bytes(text.begin(), text.end());
    return crypto::to_hex(crypto::sha1(bytes));
}

}  // namespace

std::string_view origin_name(Origin origin) {
    switch (origin) {
        case Origin::Authored: return "authored";
        case Origin::Derived: return "derived";
        case Origin::Learned: return "learned";
        case Origin::Given: return "given";
    }
    return "learned";
}

std::optional<Origin> parse_origin(std::string_view name) {
    for (const Origin o : {Origin::Authored, Origin::Derived, Origin::Learned, Origin::Given})
        if (origin_name(o) == name) return o;
    return std::nullopt;
}

struct Store::Db {
    sql::Database database;
    // {(platform, kind): {key: hash}}, read once and kept while nobody else writes the file (digest asks for the hash of a
    // piece for every read of every step).
    std::map<std::pair<std::string, std::string>, std::map<std::string, std::string>> hashes;
    std::int64_t data_version = -1;

    explicit Db(sql::Database d) : database(std::move(d)) {}

    // The hashes of a kind, read again when another connection wrote the file since.
    std::map<std::string, std::string>& hashes_of(std::string_view platform, std::string_view kind) {
        const auto version = database.query("PRAGMA data_version");
        const std::int64_t now = version.empty() ? 0 : cell_int(version[0][0]);
        if (now != data_version) {
            hashes.clear();
            data_version = now;
        }
        const auto id = std::make_pair(std::string(platform), std::string(kind));
        const auto found = hashes.find(id);
        if (found != hashes.end()) return found->second;
        std::map<std::string, std::string>& h = hashes[id];
        for (const sql::Row& row : database.query("SELECT key, hash FROM knowledge WHERE platform=? AND kind=?",
                                                  {text_param(platform), text_param(kind)}))
            h[cell_text(row[0])] = cell_text(row[1]);
        return h;
    }
};

Store::Store(const std::filesystem::path& file, DataFolders folders)
    : file_(file), sources_(std::move(folders)), db_(std::make_unique<Db>(sql::Database::open(file, std::chrono::seconds(60)))) {
    db_->database.exec("PRAGMA journal_mode=WAL");
    db_->database.exec("PRAGMA synchronous=NORMAL");
    // one statement per exec: SQLite's prepare runs the first only
    const std::string_view schema = kSchema;
    for (std::size_t from = 0; from < schema.size();) {
        const std::size_t end = schema.find(';', from);
        if (end == std::string_view::npos) break;
        db_->database.exec(schema.substr(from, end + 1 - from));
        from = end + 1;
    }
}

Store::~Store() = default;

std::filesystem::path Store::default_path(const std::filesystem::path& state_dir) {
    if (const std::optional<std::string> env = fs::get_env("CB_KNOWLEDGE_DB"); env && !env->empty()) return fs::from_text(*env);
    return state_dir / "db" / "knowledge.v2.sqlite";
}

std::filesystem::path Store::beside(const std::filesystem::path& function_store_file) {
    return std::filesystem::absolute(function_store_file).parent_path() / "db" / "knowledge.v2.sqlite";
}

// ---- pieces ---------------------------------------------------------------------------------------------------------

std::optional<Value> Store::get(std::string_view platform, std::string_view kind, std::string_view key) {
    std::lock_guard lock(mutex_);
    const auto rows = db_->database.query("SELECT value FROM knowledge WHERE platform=? AND kind=? AND key=?",
                                          {text_param(platform), text_param(kind), text_param(key)});
    if (rows.empty()) return std::nullopt;
    return decode_cell(rows[0][0]);
}

bool Store::has(std::string_view platform, std::string_view kind, std::string_view key) {
    std::lock_guard lock(mutex_);
    return !db_->database
                .query("SELECT 1 FROM knowledge WHERE platform=? AND kind=? AND key=?",
                       {text_param(platform), text_param(kind), text_param(key)})
                .empty();
}

void Store::put(std::string_view platform, std::string_view kind, std::string_view key, const Value& value,
                std::string_view source, Origin origin) {
    Rows one;
    one.insert_or_assign(std::string(key), value);
    put_many(platform, kind, one, source, origin);
}

void Store::put_many(std::string_view platform, std::string_view kind, const Rows& values, std::string_view source,
                     Origin origin) {
    std::lock_guard lock(mutex_);
    const double made = seconds_now();
    {
        sql::Database::Transaction tx(db_->database);
        for (const auto& [key, value] : values) {
            db_->database.exec(
                "INSERT OR REPLACE INTO knowledge(platform, kind, key, seq, hash, tag, value, source, origin, made) "
                "VALUES(?,?,?,NULL,?,?,?,?,?,?)",
                {text_param(platform), text_param(kind), key, value.digest(), std::string(kTagValue), value.encode(),
                 text_param(source), std::string(origin_name(origin)), made});
        }
        tx.commit();
    }
    // the hashes this process holds stay right without reading the kind again
    const auto held = db_->hashes.find({std::string(platform), std::string(kind)});
    if (held != db_->hashes.end()) {
        for (const auto& [key, value] : values) held->second[key] = value.digest();
    }
}

void Store::forget(std::string_view platform, std::string_view kind, std::string_view key) {
    std::lock_guard lock(mutex_);
    {
        sql::Database::Transaction tx(db_->database);
        db_->database.exec("DELETE FROM knowledge WHERE platform=? AND kind=? AND key=?",
                           {text_param(platform), text_param(kind), text_param(key)});
        tx.commit();
    }
    const auto held = db_->hashes.find({std::string(platform), std::string(kind)});
    if (held != db_->hashes.end()) held->second.erase(std::string(key));
}

std::optional<show::Provenance> Store::provenance(std::string_view platform, std::string_view kind, std::string_view key) {
    std::lock_guard lock(mutex_);
    const auto rows = db_->database.query("SELECT source, origin, made, hash FROM knowledge WHERE platform=? AND kind=? AND key=?",
                                          {text_param(platform), text_param(kind), text_param(key)});
    if (rows.empty()) return std::nullopt;
    show::Provenance p;
    p.source = cell_text(rows[0][0]);
    p.origin = cell_text(rows[0][1]);
    if (const double* made = std::get_if<double>(&rows[0][2])) p.made_at = *made;
    p.hash = cell_text(rows[0][3]);
    return p;
}

Rows Store::table_locked(std::string_view platform, std::string_view kind) {
    Rows out;
    for (const sql::Row& row : db_->database.query(
             "SELECT key, value FROM knowledge WHERE platform=? AND kind=? ORDER BY seq IS NULL, seq, rowid",
             {text_param(platform), text_param(kind)}))
        out.insert_or_assign(cell_text(row[0]), decode_cell(row[1]));
    return out;
}

Rows Store::table(std::string_view platform, std::string_view kind) {
    std::lock_guard lock(mutex_);
    return table_locked(platform, kind);
}

Rows Store::authored_table(const KindId& kind) {
    if (AuthoredSources::find(kind) != nullptr) return sources_.authored(kind, *this);
    return table(kind.platform, kind.kind);
}

std::optional<std::string> Store::stamp_of(std::string_view platform, std::string_view kind) {
    std::lock_guard lock(mutex_);
    const auto rows = db_->database.query("SELECT stamp FROM loaded WHERE platform=? AND kind=?",
                                          {text_param(platform), text_param(kind)});
    if (rows.empty()) return std::nullopt;
    return cell_optional_text(rows[0][0]);
}

Rows Store::ensure(const KindId& kind, std::string_view stamp, const std::function<Rows()>& make, std::string_view source,
                   Origin origin) {
    std::lock_guard lock(mutex_);
    if (stamp_of(kind.platform, kind.kind) == std::optional<std::string>(std::string(stamp))) {
        return table_locked(kind.platform, kind.kind);
    }
    const Rows made = make();
    const double now = seconds_now();
    {
        sql::Database::Transaction tx(db_->database);
        db_->database.exec("DELETE FROM knowledge WHERE platform=? AND kind=? AND origin IN (?, ?)",
                           {text_param(kind.platform), text_param(kind.kind), std::string(origin_name(Origin::Authored)),
                            std::string(origin_name(Origin::Derived))});
        std::int64_t seq = 0;
        for (const auto& [key, value] : made) {
            db_->database.exec(
                "INSERT OR REPLACE INTO knowledge(platform, kind, key, seq, hash, tag, value, source, origin, made) "
                "VALUES(?,?,?,?,?,?,?,?,?,?)",
                {text_param(kind.platform), text_param(kind.kind), key, seq++, value.digest(), std::string(kTagValue), value.encode(),
                 text_param(source), std::string(origin_name(origin)), now});
        }
        db_->database.exec("INSERT OR REPLACE INTO loaded(platform, kind, stamp, source, made) VALUES(?,?,?,?,?)",
                           {text_param(kind.platform), text_param(kind.kind), text_param(stamp), text_param(source), now});
        tx.commit();
    }
    db_->hashes.erase({kind.platform, kind.kind});
    return table_locked(kind.platform, kind.kind);
}

// A kind is compared by its stamp at every ensure(), so there is nothing remembered that could go stale.
void Store::forget_ensured(const KindId&) {}

std::optional<std::string> Store::carried(std::string_view what) {
    std::lock_guard lock(mutex_);
    const auto rows = db_->database.query("SELECT mark FROM carried WHERE what=?", {text_param(what)});
    if (rows.empty()) return std::nullopt;
    return cell_optional_text(rows[0][0]);
}

void Store::set_carried(std::string_view what, std::string_view mark) {
    std::lock_guard lock(mutex_);
    db_->database.exec("INSERT OR REPLACE INTO carried(what, mark) VALUES(?,?)", {text_param(what), text_param(mark)});
}

// ---- digests ----------------------------------------------------------------------------------------------------------

std::string Store::whole_kind_digest_locked(std::string_view platform, std::string_view kind) {
    const std::map<std::string, std::string>& h = db_->hashes_of(platform, kind);
    if (h.empty()) return std::string(kAbsent);
    // keys in byte order (a std::map is): one SHA-1 over every "key NUL hash NL"
    std::string text;
    for (const auto& [key, hash] : h) {
        text += key;
        text += '\0';
        text += hash;
        text += '\n';
    }
    return sha1_text(text);
}

std::string Store::digest(const Read& read) {
    std::lock_guard lock(mutex_);
    if (read.platform == kBodiesPlatform) {
        if (read.kind == "name") {
            const std::optional<BodyName> n = name(read.key);
            if (!n) return std::string(kAbsent);
            return Value(Value::List{Value(n->name), Value(n->source)}).digest();
        }
        if (read.kind == "names") {
            const auto found = names_from_bodies(read.key);
            if (found.empty()) return std::string(kAbsent);
            Value::List all;
            for (const auto& [addr, n] : found) all.push_back(Value(Value::List{Value(addr), Value(n.name), Value(n.source)}));
            return Value(std::move(all)).digest();
        }
        return std::string(kAbsent);
    }
    if (read.whole_kind()) return whole_kind_digest_locked(read.platform, read.kind);
    const std::map<std::string, std::string>& h = db_->hashes_of(read.platform, read.kind);
    const auto it = h.find(read.key);
    return it == h.end() ? std::string(kAbsent) : it->second;
}

// ---- bodies ---------------------------------------------------------------------------------------------------------

void Store::add_module(std::string_view sha, std::string_view name, std::string_view platform) {
    std::lock_guard lock(mutex_);
    db_->database.exec("INSERT OR REPLACE INTO module(sha, name, platform) VALUES(?,?,?)",
                       {text_param(sha), text_param(name), text_param(platform)});
}

void Store::set_bodies(std::string_view module, std::span<const BodyRow> bodies) {
    std::lock_guard lock(mutex_);
    sql::Database::Transaction tx(db_->database);
    db_->database.exec("DELETE FROM body WHERE module=?", {text_param(module)});
    for (const BodyRow& b : bodies) {
        db_->database.exec("INSERT INTO body(module, addr, hash, name, source) VALUES(?,?,?,?,?)",
                           {text_param(module), b.addr, b.digest, optional_param(b.name), optional_param(b.source)});
    }
    tx.commit();
}

std::vector<BodyRow> Store::bodies(std::string_view module) {
    std::lock_guard lock(mutex_);
    std::vector<BodyRow> out;
    for (const sql::Row& row : db_->database.query("SELECT addr, hash, name, source FROM body WHERE module=?", {text_param(module)}))
        out.push_back({cell_text(row[0]), cell_text(row[1]), cell_optional_text(row[2]), cell_optional_text(row[3])});
    return out;
}

void Store::name_body(std::string_view digest_text, std::string_view name_text, std::string_view source) {
    std::lock_guard lock(mutex_);
    db_->database.exec("INSERT OR REPLACE INTO body_name(hash, name, source, ts) VALUES(?,?,?,?)",
                       {text_param(digest_text), text_param(name_text), text_param(source), seconds_now()});
}

std::optional<BodyName> Store::name(std::string_view digest_text) {
    std::lock_guard lock(mutex_);
    const auto given = db_->database.query("SELECT name, source FROM body_name WHERE hash=?", {text_param(digest_text)});
    if (!given.empty()) return BodyName{cell_text(given[0][0]), cell_text(given[0][1])};
    // no name was given: the one every module that names this body agrees on
    std::vector<std::pair<std::string, std::string>> names;   // (name, the first module that gave it)
    for (const sql::Row& row : db_->database.query(
             "SELECT DISTINCT b.name, m.name FROM body b LEFT JOIN module m ON m.sha=b.module WHERE b.hash=? AND b.name IS NOT NULL",
             {text_param(digest_text)})) {
        const std::string n = cell_text(row[0]);
        if (std::any_of(names.begin(), names.end(), [&](const auto& e) { return e.first == n; })) continue;
        const std::string module = cell_text(row[1]);
        names.emplace_back(n, module.empty() ? "another module" : module);
    }
    if (names.size() != 1) return std::nullopt;
    return BodyName{names[0].first, "the same body in " + names[0].second};
}

std::vector<std::pair<std::string, BodyName>> Store::names_from_bodies(std::string_view module) {
    std::lock_guard lock(mutex_);
    std::map<std::string, BodyName> out;   // by address: the result is iterated into a digest, so its order is the address's
    for (const sql::Row& row : db_->database.query(
             "SELECT b.addr, n.name, n.source FROM body b JOIN body_name n ON n.hash=b.hash WHERE b.module=?", {text_param(module)}))
        out[cell_text(row[0])] = BodyName{cell_text(row[1]), cell_text(row[2])};
    // a body this module has not named: the name the other modules give it, when they agree
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> others;
    for (const sql::Row& row : db_->database.query(
             "SELECT b.addr, o.name, m.name FROM body b JOIN body o ON o.hash=b.hash AND o.module<>b.module AND o.name IS NOT NULL "
             "LEFT JOIN module m ON m.sha=o.module WHERE b.module=? AND b.name IS NULL",
             {text_param(module)})) {
        auto& v = others[cell_text(row[0])];
        const std::string n = cell_text(row[1]);
        if (std::any_of(v.begin(), v.end(), [&](const auto& e) { return e.first == n; })) continue;
        const std::string from = cell_text(row[2]);
        v.emplace_back(n, from.empty() ? "another module" : from);
    }
    for (const auto& [addr, v] : others) {
        if (out.count(addr) == 0 && v.size() == 1) out[addr] = BodyName{v[0].first, "the same body in " + v[0].second};
    }
    return {out.begin(), out.end()};
}

std::size_t Store::modules_with_body(std::string_view digest_text) {
    std::lock_guard lock(mutex_);
    const auto rows = db_->database.query("SELECT COUNT(DISTINCT module) FROM body WHERE hash=?", {text_param(digest_text)});
    return rows.empty() ? 0 : static_cast<std::size_t>(cell_int(rows[0][0]));
}

// ---- show::Base --------------------------------------------------------------------------------------------------------

void Store::load_authored() {
    for (const AuthoredKind& k : AuthoredSources::catalogue()) (void)sources_.authored(k.id, *this);
}

std::vector<show::KindRow> Store::kinds(std::optional<std::string_view> platform_name) {
    std::lock_guard lock(mutex_);
    std::string sql_text = "SELECT platform, kind, origin, COUNT(*), SUM(LENGTH(value)) FROM knowledge ";
    std::vector<sql::Value> params;
    if (platform_name && !platform_name->empty()) {
        sql_text += "WHERE platform=? ";
        params.push_back(text_param(*platform_name));
    }
    sql_text += "GROUP BY platform, kind, origin ORDER BY platform, kind";
    std::map<std::pair<std::string, std::string>, std::string> loaded_from;
    for (const sql::Row& row : db_->database.query("SELECT platform, kind, source FROM loaded"))
        loaded_from[{cell_text(row[0]), cell_text(row[1])}] = cell_text(row[2]);
    std::vector<show::KindRow> out;
    for (const sql::Row& row : db_->database.query(sql_text, params)) {
        const std::string platform_text = cell_text(row[0]);
        const std::string kind_text = cell_text(row[1]);
        if (out.empty() || out.back().platform != platform_text || out.back().kind != kind_text) {
            show::KindRow k;
            k.platform = platform_text;
            k.kind = kind_text;
            const auto from = loaded_from.find({platform_text, kind_text});
            if (from != loaded_from.end()) k.source = from->second;
            out.push_back(std::move(k));
        }
        show::KindRow& k = out.back();
        k.keys += cell_int(row[3]);
        k.bytes += cell_int(row[4]);
        k.origins[cell_text(row[2])] += cell_int(row[3]);
    }
    return out;
}

show::SharedBodies Store::shared_bodies() {
    std::lock_guard lock(mutex_);
    show::SharedBodies out;
    const auto all = db_->database.query("SELECT COUNT(*), COUNT(DISTINCT hash) FROM body");
    if (!all.empty()) {
        out.recorded = cell_int(all[0][0]);
        out.distinct = cell_int(all[0][1]);
    }
    const auto several = db_->database.query(
        "SELECT COUNT(*) FROM (SELECT hash FROM body GROUP BY hash HAVING COUNT(DISTINCT module) > 1)");
    if (!several.empty()) out.in_several_modules = cell_int(several[0][0]);
    return out;
}

std::vector<std::string> Store::first_keys(std::string_view platform_name, std::string_view kind, std::size_t limit,
                                           std::size_t& total) {
    const Rows rows = authored_table({std::string(platform_name), std::string(kind)});
    total = rows.size();
    std::vector<std::string> keys;
    for (const auto& [key, value] : rows) {
        if (keys.size() >= limit) break;
        keys.push_back(key);
    }
    return keys;
}

std::optional<show::Stored> Store::value(std::string_view platform_name, std::string_view kind, std::string_view key) {
    const Rows rows = authored_table({std::string(platform_name), std::string(kind)});
    const Value* found = rows.find(std::string(key));
    if (found == nullptr) return std::nullopt;
    show::Stored out;
    out.kind = show::Stored::Kind::Structured;
    out.structured = *found;
    return out;
}

std::optional<show::Provenance> Store::about(std::string_view platform_name, std::string_view kind, std::string_view key) {
    return provenance(platform_name, kind, key);
}

}  // namespace cb::knowledge

namespace cb::knowledge {

std::string Store::platform(std::string_view name) const { return knowledge::platform(name); }

}  // namespace cb::knowledge
