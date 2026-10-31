// facts.cc -- the program database (facts.py), on SQLite's C API.
#include "facts.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sys/utime.h>

#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

namespace cbe {

const char* const GIVEN = "given";

static const char* SCHEMA =
    "CREATE TABLE IF NOT EXISTS fact(\n"
    "    kind TEXT NOT NULL, key TEXT NOT NULL, hash TEXT NOT NULL, tag TEXT NOT NULL,\n"
    "    value BLOB, blob TEXT, size INTEGER, step TEXT NOT NULL, version TEXT, inputs TEXT,\n"
    "    made REAL, took REAL,\n"
    "    PRIMARY KEY (kind, key));\n"
    "CREATE INDEX IF NOT EXISTS fact_step ON fact(step, version);\n"
    "CREATE TABLE IF NOT EXISTS blob(hash TEXT PRIMARY KEY, data BLOB);\n"
    "CREATE TABLE IF NOT EXISTS about(name TEXT PRIMARY KEY, value TEXT);\n";

static const char* COLS = "SELECT kind, key, hash, tag, value, blob, size, step, version, inputs, made, took FROM fact";

std::vector<Input> FactRow::parsed_inputs() const {
  std::vector<Input> out;
  if (inputs.null() || inputs.s.empty()) return out;
  Json j = json_parse(inputs.s);
  for (auto& e : j.a) {
    if (e.t != Json::ARR || e.a.size() != 3) raise(ERR_VALUE, "an input is not (kind, key, hash)");
    Input in;
    for (int k = 0; k < 3; k++) in[k] = e.a[k].t == Json::STR ? e.a[k].s : e.a[k].raw;
    out.push_back(in);
  }
  return out;
}

void FactRow::write(Writer& w) const {
  w.text(kind);
  w.text(key);
  w.text(hash);
  w.text(tag);
  if (has_value) w.bytes(value); else w.none();
  if (has_blob) w.text(blob); else w.none();
  w.field(size);
  w.text(step);
  w.field(version);
  w.field(inputs);
  w.field(made);
  w.field(took);
}

static FactRow to_fact(Row& r) {
  FactRow f;
  f.kind = r[0].s;
  f.key = r[1].s;
  f.hash = r[2].s;
  f.tag = r[3].s;
  f.has_value = !r[4].null();
  f.value = std::move(r[4].s);
  f.has_blob = !r[5].null();
  f.blob = r[5].s;
  f.size = r[6];
  f.step = r[7].s;
  f.version = r[8];
  f.inputs = r[9];
  f.made = r[10];
  f.took = r[11];
  return f;
}

Database::Database(const std::string& p) : path(p) {
  std::error_code ec;
  fs::create_directories(fs::path(widen(p)).parent_path(), ec);
  conn = std::make_shared<Conn>(p, false, 30000);
  conn->exec("PRAGMA auto_vacuum=INCREMENTAL");
  conn->exec("PRAGMA journal_mode=WAL");
  conn->exec("PRAGMA synchronous=NORMAL");
  conn->exec(SCHEMA);
  _wutime(widen(p).c_str(), nullptr);        // when it was last opened: the cap goes by it
}

bool Database::row(const std::string& kind, const std::string& key, FactRow& out) {
  Row r;
  if (!conn->one(std::string(COLS) + " WHERE kind=? AND key=?", {Field::text(kind), Field::text(key)}, r))
    return false;
  out = to_fact(r);
  return true;
}

std::vector<FactRow> Database::rows(const std::string* kind) {
  std::vector<Row> got = kind ? conn->rows(std::string(COLS) + " WHERE kind=? ORDER BY key", {Field::text(*kind)})
                              : conn->rows(std::string(COLS) + " ORDER BY kind, key");
  std::vector<FactRow> out;
  out.reserve(got.size());
  for (auto& r : got) out.push_back(to_fact(r));
  return out;
}

std::vector<Row> Database::kinds() {
  return conn->rows("SELECT kind, COUNT(*), SUM(size), GROUP_CONCAT(DISTINCT step) FROM fact GROUP BY kind ORDER BY kind");
}

std::string Database::blob(const std::string& h) {
  Row r;
  if (!conn->one("SELECT data FROM blob WHERE hash=?", {Field::text(h)}, r))
    raise(ERR_KEY, "blob " + h + " is missing from " + path);
  return r[0].s;
}

bool Database::about(const std::string& name, std::string& out) {
  Row r;
  if (!conn->one("SELECT value FROM about WHERE name=?", {Field::text(name)}, r) || r[0].null()) return false;
  out = r[0].s;
  return true;
}

void Database::set_about(const std::string& name, const std::string& value) {
  Tx tx(*conn);
  conn->run("INSERT OR REPLACE INTO about(name, value) VALUES(?,?)", {Field::text(name), Field::text(value)});
  tx.commit();
}

std::string Database::write(const std::string& kind, const std::string& key, const std::string& tag,
                            const std::string& data, const std::string& step, const std::string& version,
                            const std::string& inputs_json, double took) {
  std::string h = sha1_hex2(tag, data.data(), data.size());
  bool inline_ = data.size() <= BLOB_AT;
  Tx tx(*conn);
  if (!inline_) conn->run("INSERT OR IGNORE INTO blob(hash, data) VALUES(?,?)", {Field::text(h), Field::bytes(data)});
  conn->run("INSERT OR REPLACE INTO fact(kind, key, hash, tag, value, blob, size, step, version, "
            "inputs, made, took) VALUES(?,?,?,?,?,?,?,?,?,?,?,?)",
            {Field::text(kind), Field::text(key), Field::text(h), Field::text(tag),
             inline_ ? Field::bytes(data) : Field::none(), inline_ ? Field::none() : Field::text(h),
             Field::integer((int64_t)data.size()), Field::text(step), Field::text(version), Field::text(inputs_json),
             Field::real(now()), Field::real(took)});
  tx.commit();
  return h;
}

void Database::write_raw(const std::string& kind, const std::string& key, const std::string& hash,
                         const std::string& tag, const std::string& data, const std::string& step,
                         const std::string& version, const std::string& inputs_json, double took) {
  Tx tx(*conn);
  conn->run("INSERT OR REPLACE INTO fact(kind, key, hash, tag, value, blob, size, step, version, inputs, "
            "made, took) VALUES(?,?,?,?,?,NULL,?,?,?,?,?,?)",
            {Field::text(kind), Field::text(key), Field::text(hash), Field::text(tag), Field::bytes(data),
             Field::integer((int64_t)data.size()), Field::text(step), Field::text(version), Field::text(inputs_json),
             Field::real(now()), took == 0 ? Field::integer(0) : Field::real(took)});
  tx.commit();
}

void Database::remake(const std::string& kind, const std::string& key, const std::string& version,
                      const std::string& inputs_json) {
  Tx tx(*conn);
  conn->run("UPDATE fact SET version=?, inputs=?, made=? WHERE kind=? AND key=?",
            {Field::text(version), Field::text(inputs_json), Field::real(now()), Field::text(kind), Field::text(key)});
  tx.commit();
}

int64_t Database::forget(const std::string& kind, const std::string* key) {
  Tx tx(*conn);
  int64_t n = key ? conn->run("DELETE FROM fact WHERE kind=? AND key=?", {Field::text(kind), Field::text(*key)})
                  : conn->run("DELETE FROM fact WHERE kind=?", {Field::text(kind)});
  tx.commit();
  return n;
}

bool Database::cleaned(const std::string& step) {
  std::lock_guard<std::mutex> g(cleaned_mu_);
  return cleaned_.count(step) > 0;
}

void Database::mark_cleaned(const std::string& step) {
  std::lock_guard<std::mutex> g(cleaned_mu_);
  cleaned_.insert(step);
}

void Database::clear_cleaned() {
  std::lock_guard<std::mutex> g(cleaned_mu_);
  cleaned_.clear();
}

int64_t Database::drop_other_versions(const std::string& step, const std::string& version) {
  {
    std::lock_guard<std::mutex> g(cleaned_mu_);
    if (cleaned_.count(step) || step == GIVEN) return 0;
  }
  int64_t n;
  {
    Tx tx(*conn);
    n = conn->run("DELETE FROM fact WHERE step=? AND version<>?", {Field::text(step), Field::text(version)});
    tx.commit();
    std::lock_guard<std::mutex> g(cleaned_mu_);
    cleaned_.insert(step);
  }
  if (n) collect();
  return n;
}

void Database::collect() {
  std::lock_guard<std::recursive_mutex> g(conn->mu);
  {
    Tx tx(*conn);
    conn->run("DELETE FROM blob WHERE hash NOT IN (SELECT blob FROM fact WHERE blob IS NOT NULL)");
    tx.commit();
  }
  try {
    conn->rows("PRAGMA incremental_vacuum");
    conn->rows("PRAGMA wal_checkpoint(TRUNCATE)");
  } catch (const Error&) {
  }
}

// ---- the open databases -----------------------------------------------------------------------
static std::mutex g_open_mu;
static std::map<std::string, std::shared_ptr<Database>> g_open;

static std::string abspath(const std::string& p) {
  std::wstring w = widen(p);
  wchar_t buf[32768];
  DWORD n = GetFullPathNameW(w.c_str(), 32768, buf, nullptr);
  return (n && n < 32768) ? narrow(std::wstring(buf, n)) : p;
}

std::shared_ptr<Database> open_at(const std::string& given) {
  std::string path = abspath(given);                  // (os.path.abspath: the shim's key and the store's agree)
  std::lock_guard<std::mutex> g(g_open_mu);
  auto it = g_open.find(path);
  if (it != g_open.end()) return it->second;
  auto db = std::make_shared<Database>(path);
  g_open[path] = db;
  return db;
}

void close_db(const std::string& path) {
  std::lock_guard<std::mutex> g(g_open_mu);
  g_open.erase(abspath(path));
}

std::vector<std::string> open_paths() {
  std::lock_guard<std::mutex> g(g_open_mu);
  std::vector<std::string> out;
  for (auto& kv : g_open) out.push_back(kv.first);
  return out;
}

// ---- the binary's SHA-256 -----------------------------------------------------------------------
std::string normcase(const std::string& p) {
  std::wstring w = widen(p);
  wchar_t buf[32768];
  DWORD n = GetFullPathNameW(w.c_str(), 32768, buf, nullptr);
  std::wstring full = (n && n < 32768) ? std::wstring(buf, n) : w;
  for (auto& c : full) {
    if (c == L'/') c = L'\\';
  }
  CharLowerBuffW(&full[0], (DWORD)full.size());
  return narrow(full);
}

static std::mutex g_sha_mu;
static std::map<std::tuple<std::string, int64_t, int64_t>, std::string> g_sha;

std::string sha_file(const std::string& path) {
  WIN32_FILE_ATTRIBUTE_DATA a;
  if (!GetFileAttributesExW(widen(path).c_str(), GetFileExInfoStandard, &a)) {
    Error e(ERR_OS, "[WinError " + std::to_string(GetLastError()) + "] cannot stat: '" + path + "'");
    e.code = (int)GetLastError();
    throw e;
  }
  int64_t size = ((int64_t)a.nFileSizeHigh << 32) | a.nFileSizeLow;
  int64_t ft = ((int64_t)a.ftLastWriteTime.dwHighDateTime << 32) | a.ftLastWriteTime.dwLowDateTime;
  int64_t mtime_ns = (ft - 116444736000000000LL) * 100;
  auto seen = std::make_tuple(normcase(path), size, mtime_ns);
  {
    std::lock_guard<std::mutex> g(g_sha_mu);
    auto it = g_sha.find(seen);
    if (it != g_sha.end()) return it->second;
  }
  std::string got = sha256_file(widen(path));
  if (now() - (double)mtime_ns / 1e9 > 2.0) {      // (a file written just now may be written again within the clock's tick)
    std::lock_guard<std::mutex> g(g_sha_mu);
    g_sha[seen] = got;
  }
  return got;
}

// ---- the cap on all databases -------------------------------------------------------------------
static int64_t size_of(const std::wstring& p) {
  int64_t total = 0;
  for (const wchar_t* suffix : {L"", L"-wal", L"-shm"}) {
    std::error_code ec;
    auto n = fs::file_size(p + suffix, ec);
    if (!ec) total += (int64_t)n;
  }
  return total;
}

std::vector<DbFile> databases(const std::string& folder) {
  std::vector<DbFile> out;
  std::error_code ec;
  fs::path f(widen(folder));
  if (!fs::is_directory(f, ec)) return out;
  for (auto& e : fs::directory_iterator(f, ec)) {
    std::wstring n = e.path().filename().wstring();
    if (n.size() < 7 || n.compare(n.size() - 7, 7, L".sqlite") != 0 || n == L"knowledge.sqlite") continue;
    std::wstring p = (f / n).wstring();
    WIN32_FILE_ATTRIBUTE_DATA a;
    if (!GetFileAttributesExW(p.c_str(), GetFileExInfoStandard, &a)) continue;
    int64_t ft = ((int64_t)a.ftLastWriteTime.dwHighDateTime << 32) | a.ftLastWriteTime.dwLowDateTime;
    out.push_back({narrow(p), size_of(p), (double)(ft - 116444736000000000LL) / 1e7});
  }
  std::stable_sort(out.begin(), out.end(), [](const DbFile& a, const DbFile& b) { return a.mtime < b.mtime; });
  return out;
}

bool holds_verdicts(const std::string& p) {
  try {
    Conn c(p, true, 5000);
    Row r;
    return c.one("SELECT 1 FROM sqlite_master WHERE type='table' AND name='verdict'", {}, r);
  } catch (const Error&) {
    return false;
  }
}

static int64_t facts_dropped(const std::string& p) {
  try {
    Conn c(p, false, 30000);
    {
      Tx tx(c);
      c.run("DELETE FROM fact");
      c.run("DELETE FROM blob");
      tx.commit();
    }
    c.exec("VACUUM");
    c.rows("PRAGMA wal_checkpoint(TRUNCATE)");
  } catch (const Error&) {
  }
  return size_of(widen(p));
}

std::vector<std::string> hold(int64_t cap, const std::string& folder) {
  std::vector<DbFile> dbs = databases(folder);
  int64_t total = 0;
  for (auto& d : dbs) total += d.size;
  std::vector<std::string> gone;
  if (total <= cap) return gone;
  std::set<std::string> mine;
  for (auto& p : open_paths()) mine.insert(normcase(p));
  for (auto& d : dbs) {
    if ((double)total <= (double)cap * 0.8) break;
    if (mine.count(normcase(d.path))) continue;
    std::wstring w = widen(d.path);
    std::error_code ec;
    fs::remove_all(w.substr(0, w.size() - 7) + L".work", ec);
    if (holds_verdicts(d.path)) {                       // (the function store's tables stay: only the facts go)
      total -= d.size - facts_dropped(d.path);
      gone.push_back(d.path);
      continue;
    }
    for (const wchar_t* suffix : {L"", L"-wal", L"-shm"}) fs::remove(w + suffix, ec);
    total -= d.size;
    gone.push_back(d.path);
  }
  return gone;
}

}  // namespace cbe
