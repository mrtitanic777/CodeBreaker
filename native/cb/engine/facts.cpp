#include "cb/engine/facts.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>

#include <nlohmann/json.hpp>

#include "cb/base/sqlite.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/text.hpp"

namespace cb::engine::facts {

namespace stdfs = std::filesystem;
namespace sql = base::sqlite;

namespace {

constexpr const char* kSchema =
    "CREATE TABLE IF NOT EXISTS fact(\n"
    "    kind TEXT NOT NULL, key TEXT NOT NULL, hash TEXT NOT NULL, tag TEXT NOT NULL,\n"
    "    value BLOB, blob TEXT, size INTEGER, step TEXT NOT NULL, version TEXT, inputs TEXT,\n"
    "    made REAL, took REAL,\n"
    "    PRIMARY KEY (kind, key));\n"
    "CREATE INDEX IF NOT EXISTS fact_step ON fact(step, version);\n"
    "CREATE TABLE IF NOT EXISTS blob(hash TEXT PRIMARY KEY, data BLOB);\n"
    "CREATE TABLE IF NOT EXISTS about(name TEXT PRIMARY KEY, value TEXT);\n";

constexpr const char* kColumns = "SELECT kind, key, hash, tag, value, blob, size, step, version, inputs, made, took FROM fact";

double now_seconds() {
    return std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
}

double to_seconds(stdfs::file_time_type t) {
    return std::chrono::duration<double>(std::chrono::clock_cast<std::chrono::system_clock>(t).time_since_epoch()).count();
}

// ---- a column of a row -------------------------------------------------------------------------------------------

std::string text_of(const sql::Value& v) {
    if (const auto* s = std::get_if<std::string>(&v)) return *s;
    return {};
}
std::uint64_t count_of(const sql::Value& v) {
    if (const auto* i = std::get_if<std::int64_t>(&v)) return *i < 0 ? 0 : static_cast<std::uint64_t>(*i);
    return 0;
}
double real_of(const sql::Value& v) {
    if (const auto* d = std::get_if<double>(&v)) return *d;
    if (const auto* i = std::get_if<std::int64_t>(&v)) return static_cast<double>(*i);
    return 0;
}

// ---- values -------------------------------------------------------------------------------------------------------

nlohmann::json json_of(const knowledge::Value& v) {   // (nlohmann::json keeps object keys sorted: the form hashed)
    using K = knowledge::Value::Kind;
    switch (v.kind()) {
        case K::Null: return nullptr;
        case K::Bool: return *v.boolean();
        case K::Int: return *v.integer();
        case K::Double:
            if (!std::isfinite(*v.number())) throw UnsupportedError("facts", 0, "a number JSON cannot say (NaN or infinity)");
            return *v.number();
        case K::Text: return *v.text();
        case K::List: {
            nlohmann::json a = nlohmann::json::array();
            for (const auto& x : *v.list()) a.push_back(json_of(x));
            return a;
        }
        case K::Map: {
            nlohmann::json o = nlohmann::json::object();
            for (const auto& [k, x] : *v.map()) o[k] = json_of(x);
            return o;
        }
    }
    return nullptr;
}

std::string hash_of(char tag, ByteView data) {
    Bytes all;
    all.reserve(data.size() + 1);
    all.push_back(static_cast<std::uint8_t>(tag));
    append(all, data);
    return crypto::to_hex(crypto::sha1(all));
}

std::string inputs_text(const std::vector<Input>& inputs) {
    // the layout the Python wrote: [["kind", "key", "hash"], ...]
    Json a = Json::array();
    for (const Input& i : inputs) a.push_back(Json::array({i.kind, i.key, i.hash}));
    return json::dump(a, json::Style::PythonCompatible);
}

std::string norm(const stdfs::path& p) {
    std::error_code ec;
    stdfs::path a = stdfs::absolute(p, ec);
    return text::lower(fs::to_text(ec ? p : a));
}

}  // namespace

Encoded encode(const Value& value) {
    if (const Bytes* b = std::get_if<Bytes>(&value)) return {'B', *b};
    if (const std::string* s = std::get_if<std::string>(&value)) return {'S', to_bytes(*s)};
    const knowledge::Value& doc = std::get<knowledge::Value>(value);
    if (const std::string* t = doc.text()) return {'S', to_bytes(*t)};   // (a text is kept as text)
    return {'J', to_bytes(json_of(doc).dump(-1, ' ', true))};
}

Value decode(char tag, ByteView data) {
    switch (tag) {
        case 'B': return Bytes(data.begin(), data.end());
        case 'S': return std::string(as_text(data));
        case 'J': return knowledge::Value::from_json_text(as_text(data), "fact");
        case 'P': throw UnsupportedError("facts", 0, "a fact kept as a Python pickle is made again, not read");
        default: break;
    }
    throw UnsupportedError("facts", 0, std::string("an unknown tag ") + tag);
}

// ---- Database -----------------------------------------------------------------------------------------------------

struct Database::Impl {
    explicit Impl(const stdfs::path& file) : conn(sql::Database::open(file, std::chrono::seconds(30))) {}
    mutable std::mutex mutex;
    mutable sql::Database conn;
};

Database::Database(stdfs::path path) : path_(std::move(path)), impl_(std::make_unique<Impl>(path_)) {
    impl_->conn.exec("PRAGMA auto_vacuum=INCREMENTAL");
    (void)impl_->conn.query("PRAGMA journal_mode=WAL");
    impl_->conn.exec("PRAGMA synchronous=NORMAL");
    for (const auto statement : text::split(kSchema, ';'))
        if (!text::trim(statement).empty()) impl_->conn.exec(statement);
    std::error_code ec;
    stdfs::last_write_time(path_, stdfs::file_time_type::clock::now(), ec);   // when it was last opened: the cap goes by it
}

Database::~Database() = default;

std::shared_ptr<Database> Database::open(const stdfs::path& file) { return std::shared_ptr<Database>(new Database(file)); }

Bytes Row::data() const {
    if (inline_) return *inline_;
    return db_->blob(*blob_);
}

Value Row::value() const { return decode(tag, data()); }

std::optional<Row> Database::row(std::string_view kind, std::string_view key) const {
    const std::lock_guard<std::mutex> g(impl_->mutex);
    const auto rows = impl_->conn.query(std::string(kColumns) + " WHERE kind=? AND key=?", {std::string(kind), std::string(key)});
    if (rows.empty()) return std::nullopt;
    Row out;
    out.db_ = this;
    const sql::Row& r = rows.front();
    out.kind = text_of(r[0]); out.key = text_of(r[1]); out.hash = text_of(r[2]);
    const std::string tag = text_of(r[3]);
    out.tag = tag.empty() ? 'B' : tag[0];
    if (const auto* b = std::get_if<Bytes>(&r[4])) out.inline_ = *b;
    if (const auto* s = std::get_if<std::string>(&r[5])) out.blob_ = *s;
    out.size = count_of(r[6]); out.step = text_of(r[7]); out.version = text_of(r[8]);
    if (const auto* s = std::get_if<std::string>(&r[9]); s && !s->empty())
        for (const auto& item : json::parse(*s)) out.inputs.push_back({item.at(0).get<std::string>(), item.at(1).get<std::string>(), item.at(2).get<std::string>()});
    out.made = real_of(r[10]); out.took = real_of(r[11]);
    return out;
}

std::vector<Row> Database::rows(std::optional<std::string_view> kind) const {
    std::vector<std::string> keys;
    std::vector<std::string> kinds;
    {
        const std::lock_guard<std::mutex> g(impl_->mutex);
        const auto found = kind ? impl_->conn.query("SELECT kind, key FROM fact WHERE kind=? ORDER BY key", {std::string(*kind)})
                                : impl_->conn.query("SELECT kind, key FROM fact ORDER BY kind, key");
        for (const auto& r : found) { kinds.push_back(text_of(r[0])); keys.push_back(text_of(r[1])); }
    }
    std::vector<Row> out;
    for (std::size_t i = 0; i < keys.size(); ++i)
        if (auto r = row(kinds[i], keys[i])) out.push_back(std::move(*r));
    return out;
}

std::vector<KindSummary> Database::kinds() const {
    const std::lock_guard<std::mutex> g(impl_->mutex);
    std::vector<KindSummary> out;
    for (const auto& r : impl_->conn.query("SELECT kind, COUNT(*), SUM(size), GROUP_CONCAT(DISTINCT step) FROM fact GROUP BY kind ORDER BY kind"))
        out.push_back({text_of(r[0]), count_of(r[1]), count_of(r[2]), text_of(r[3])});
    return out;
}

Bytes Database::blob(std::string_view hash) const {
    const std::lock_guard<std::mutex> g(impl_->mutex);
    const auto rows = impl_->conn.query("SELECT data FROM blob WHERE hash=?", {std::string(hash)});
    if (rows.empty()) throw FormatError("facts", 0, "blob " + std::string(hash) + " is missing from " + fs::to_text(path_));
    if (const auto* b = std::get_if<Bytes>(&rows.front()[0])) return *b;
    return {};
}

std::optional<std::string> Database::about(std::string_view name) const {
    const std::lock_guard<std::mutex> g(impl_->mutex);
    const auto rows = impl_->conn.query("SELECT value FROM about WHERE name=?", {std::string(name)});
    if (rows.empty()) return std::nullopt;
    if (const auto* s = std::get_if<std::string>(&rows.front()[0])) return *s;
    return std::nullopt;
}

void Database::set_about(std::string_view name, std::string_view value) {
    const std::lock_guard<std::mutex> g(impl_->mutex);
    impl_->conn.exec("INSERT OR REPLACE INTO about(name, value) VALUES(?,?)", {std::string(name), std::string(value)});
}

std::string Database::write(std::string_view kind, std::string_view key, const Value& value, std::string_view step, std::string_view version,
                            const std::vector<Input>& inputs, double took) {
    const Encoded e = encode(value);
    const std::string h = hash_of(e.tag, e.data);
    const bool small = e.data.size() <= kBlobAt;
    const std::lock_guard<std::mutex> g(impl_->mutex);
    sql::Database::Transaction tx(impl_->conn);
    if (!small) impl_->conn.exec("INSERT OR IGNORE INTO blob(hash, data) VALUES(?,?)", {h, e.data});
    impl_->conn.exec("INSERT OR REPLACE INTO fact(kind, key, hash, tag, value, blob, size, step, version, inputs, made, took) VALUES(?,?,?,?,?,?,?,?,?,?,?,?)",
                     {std::string(kind), std::string(key), h, std::string(1, e.tag), small ? sql::Value(e.data) : sql::Value(std::monostate{}),
                      small ? sql::Value(std::monostate{}) : sql::Value(h), static_cast<std::int64_t>(e.data.size()), std::string(step),
                      std::string(version), inputs_text(inputs), now_seconds(), took});
    tx.commit();
    return h;
}

void Database::write_raw(std::string_view kind, std::string_view key, std::string_view hash, char tag, ByteView data, std::string_view step,
                         std::string_view version, std::string_view inputs_json, double took) {
    const std::lock_guard<std::mutex> g(impl_->mutex);
    sql::Database::Transaction tx(impl_->conn);
    impl_->conn.exec("INSERT OR REPLACE INTO fact(kind, key, hash, tag, value, blob, size, step, version, inputs, made, took) VALUES(?,?,?,?,?,NULL,?,?,?,?,?,?)",
                     {std::string(kind), std::string(key), std::string(hash), std::string(1, tag), Bytes(data.begin(), data.end()),
                      static_cast<std::int64_t>(data.size()), std::string(step), std::string(version), std::string(inputs_json), now_seconds(), took});
    tx.commit();
}

void Database::remake(std::string_view kind, std::string_view key, std::string_view version, const std::vector<Input>& inputs) {
    const std::lock_guard<std::mutex> g(impl_->mutex);
    impl_->conn.exec("UPDATE fact SET version=?, inputs=?, made=? WHERE kind=? AND key=?",
                     {std::string(version), inputs_text(inputs), now_seconds(), std::string(kind), std::string(key)});
}

std::uint64_t Database::forget(std::string_view kind, std::optional<std::string_view> key) {
    const std::lock_guard<std::mutex> g(impl_->mutex);
    if (key) impl_->conn.exec("DELETE FROM fact WHERE kind=? AND key=?", {std::string(kind), std::string(*key)});
    else impl_->conn.exec("DELETE FROM fact WHERE kind=?", {std::string(kind)});
    return static_cast<std::uint64_t>(impl_->conn.changes());
}

bool Database::cleaned(std::string_view step) const {
    const std::lock_guard<std::mutex> g(cleaned_mutex_);
    return cleaned_.contains(step);
}

void Database::mark_cleaned(std::string_view step) {
    const std::lock_guard<std::mutex> g(cleaned_mutex_);
    cleaned_.insert(std::string(step));
}

std::uint64_t Database::drop_other_versions(std::string_view step, std::string_view version) {
    if (step == kGiven || cleaned(step)) return 0;
    std::uint64_t n;
    {
        const std::lock_guard<std::mutex> g(impl_->mutex);
        impl_->conn.exec("DELETE FROM fact WHERE step=? AND version<>?", {std::string(step), std::string(version)});
        n = static_cast<std::uint64_t>(impl_->conn.changes());
    }
    mark_cleaned(step);
    if (n) collect();
    return n;
}

void Database::collect() {
    const std::lock_guard<std::mutex> g(impl_->mutex);
    impl_->conn.exec("DELETE FROM blob WHERE hash NOT IN (SELECT blob FROM fact WHERE blob IS NOT NULL)");
    try {   // (the room is a courtesy: a busy file keeps it for the next time)
        (void)impl_->conn.query("PRAGMA incremental_vacuum");
        (void)impl_->conn.query("PRAGMA wal_checkpoint(TRUNCATE)");
    } catch (const IoError&) {
    }
}

// ---- Databases ----------------------------------------------------------------------------------------------------

std::shared_ptr<Database> Databases::open_at(const stdfs::path& file) {
    const std::string key = norm(file);
    const std::lock_guard<std::mutex> g(mutex_);
    if (const auto it = open_.find(key); it != open_.end()) return it->second;
    auto db = Database::open(file);
    open_[key] = db;
    return db;
}

void Databases::close(const stdfs::path& file) {
    const std::lock_guard<std::mutex> g(mutex_);
    open_.erase(norm(file));
}

std::string Databases::sha_of(const stdfs::path& binary) {
    std::error_code ec;
    const std::uint64_t size = stdfs::file_size(binary, ec);
    if (ec) throw NotFoundError(binary, ec.value(), "cannot stat");
    const stdfs::file_time_type mtime = stdfs::last_write_time(binary, ec);
    const auto seen = std::make_tuple(norm(binary), size, static_cast<std::int64_t>(mtime.time_since_epoch().count()));
    {
        const std::lock_guard<std::mutex> g(mutex_);
        if (const auto it = shas_.find(seen); it != shas_.end()) return it->second;
    }
    const fs::MappedFile mapped(binary);
    const std::string got = crypto::to_hex(crypto::sha256(mapped.view()));
    if (now_seconds() - to_seconds(mtime) > 2.0) {   // (a file written just now may be written again within the clock's tick)
        const std::lock_guard<std::mutex> g(mutex_);
        shas_[seen] = got;
    }
    return got;
}

std::shared_ptr<Database> Databases::open_for(const stdfs::path& binary, const std::optional<stdfs::path>& folder) {
    const std::string s = sha_of(binary);
    auto db = open_at((folder ? *folder : config_.root) / (s + ".sqlite"));
    db->sha = s;
    if (!db->about("name")) db->set_about("name", binary.filename().string());
    std::error_code ec;
    db->set_about("path", fs::to_text(stdfs::absolute(binary, ec)));
    return db;
}

stdfs::path Databases::work_folder(const Database& db, const stdfs::path& sub) {
    stdfs::path p = db.path();
    p.replace_extension(".work");
    if (!sub.empty()) p /= sub;
    fs::make_dirs(p);
    return p;
}

namespace {

std::uint64_t size_with_sidecars(const stdfs::path& p) {
    std::uint64_t total = 0;
    for (const char* suffix : {"", "-wal", "-shm"}) {
        std::error_code ec;
        const auto n = stdfs::file_size(stdfs::path(p.string() + suffix), ec);
        if (!ec) total += n;
    }
    return total;
}

bool holds_verdicts(const stdfs::path& p) {
    try {
        sql::Database db = sql::Database::open(p, std::chrono::seconds(5));
        return !db.query("SELECT 1 FROM sqlite_master WHERE type='table' AND name='verdict'").empty();
    } catch (const IoError&) {
        return false;
    }
}

// The function store's tables stay: only the facts go. -> the size afterwards.
std::uint64_t drop_facts(const stdfs::path& p) {
    try {
        sql::Database db = sql::Database::open(p, std::chrono::seconds(30));
        db.exec("DELETE FROM fact");
        db.exec("DELETE FROM blob");
        db.exec("VACUUM");
        (void)db.query("PRAGMA wal_checkpoint(TRUNCATE)");
    } catch (const IoError&) {
    }
    return size_with_sidecars(p);
}

}  // namespace

std::vector<DatabaseFile> Databases::databases(const std::optional<stdfs::path>& folder) const {
    std::vector<DatabaseFile> out;
    const stdfs::path dir = folder ? *folder : config_.root;
    std::error_code ec;
    if (!stdfs::is_directory(dir, ec)) return out;
    for (const auto& e : stdfs::directory_iterator(dir, ec)) {
        const std::string name = e.path().filename().string();
        if (!name.ends_with(".sqlite") || name == "knowledge.sqlite") continue;
        std::error_code tec;
        const auto mtime = stdfs::last_write_time(e.path(), tec);
        if (tec) continue;
        out.push_back({e.path(), size_with_sidecars(e.path()), to_seconds(mtime)});
    }
    std::stable_sort(out.begin(), out.end(), [](const DatabaseFile& a, const DatabaseFile& b) { return a.opened < b.opened; });
    return out;
}

std::vector<stdfs::path> Databases::hold(std::optional<std::uint64_t> cap_in, const std::optional<stdfs::path>& folder) {
    const std::uint64_t cap = cap_in.value_or(config_.max_bytes);
    const std::vector<DatabaseFile> dbs = databases(folder);
    std::uint64_t total = 0;
    for (const auto& d : dbs) total += d.bytes;
    std::vector<stdfs::path> gone;
    if (total <= cap) return gone;
    std::set<std::string> mine;
    {
        const std::lock_guard<std::mutex> g(mutex_);
        for (const auto& kv : open_) mine.insert(kv.first);
    }
    for (const DatabaseFile& d : dbs) {
        if (static_cast<double>(total) <= static_cast<double>(cap) * 0.8) break;
        if (mine.contains(norm(d.path))) continue;
        std::error_code ec;
        stdfs::path work = d.path;
        work.replace_extension(".work");
        stdfs::remove_all(work, ec);
        if (holds_verdicts(d.path)) {
            total -= d.bytes - std::min(d.bytes, drop_facts(d.path));
            gone.push_back(d.path);
            continue;
        }
        for (const char* suffix : {"", "-wal", "-shm"}) stdfs::remove(stdfs::path(d.path.string() + suffix), ec);
        total -= std::min(total, d.bytes);
        gone.push_back(d.path);
    }
    return gone;
}

}  // namespace cb::engine::facts
