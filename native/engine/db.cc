// db.cc -- SQLite's C API, as the Python modules used it through sqlite3.
#include "db.h"

#include <sqlite3.h>

namespace cbe {

Conn::Conn(const std::string& p, bool ro, int timeout_ms) : path(p), readonly(ro) {
  int flags = ro ? SQLITE_OPEN_READONLY : (SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
  flags |= SQLITE_OPEN_FULLMUTEX;
  int rc = sqlite3_open_v2(p.c_str(), &db_, flags, nullptr);
  if (rc != SQLITE_OK) {
    std::string m = db_ ? sqlite3_errmsg(db_) : "out of memory";
    if (db_) sqlite3_close_v2(db_);
    db_ = nullptr;
    Error e(ERR_SQLITE, m == "unable to open database file" ? m : m);
    e.code = rc;
    throw e;
  }
  sqlite3_busy_timeout(db_, timeout_ms);
  sqlite3_extended_result_codes(db_, 0);
}

Conn::~Conn() {
  for (auto& kv : cache_) sqlite3_finalize(kv.second);
  if (db_) sqlite3_close_v2(db_);
}

void Conn::release(sqlite3_stmt* st) {
  sqlite3_reset(st);
  sqlite3_clear_bindings(st);
}

void Conn::fail(int rc) {
  Error e(ERR_SQLITE, sqlite3_errmsg(db_));
  e.code = rc & 0xFF;
  throw e;
}

void Conn::exec(const std::string& sql) {
  std::lock_guard<std::recursive_mutex> g(mu);
  char* err = nullptr;
  int rc = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err);
  if (rc != SQLITE_OK) {
    std::string m = err ? err : sqlite3_errmsg(db_);
    sqlite3_free(err);
    Error e(ERR_SQLITE, m);
    e.code = rc & 0xFF;
    throw e;
  }
}

sqlite3_stmt* Conn::prepare(const std::string& sql, const std::vector<Field>& args) {
  sqlite3_stmt* st = nullptr;
  int rc;
  auto it = cache_.find(sql);
  if (it != cache_.end()) {
    st = it->second;
  } else {
    rc = sqlite3_prepare_v3(db_, sql.c_str(), (int)sql.size(), SQLITE_PREPARE_PERSISTENT, &st, nullptr);
    if (rc != SQLITE_OK) fail(rc);
    if (cache_.size() >= 256) {                         // (bounded: drop them all and start again)
      for (auto& kv : cache_) sqlite3_finalize(kv.second);
      cache_.clear();
    }
    cache_[sql] = st;
  }
  for (size_t i = 0; i < args.size(); i++) {
    const Field& f = args[i];
    int k = (int)i + 1;
    switch (f.type) {
      case 0: rc = sqlite3_bind_null(st, k); break;
      case 1: rc = sqlite3_bind_int64(st, k, f.i); break;
      case 2: rc = sqlite3_bind_double(st, k, f.d); break;
      case 3: rc = sqlite3_bind_text(st, k, f.s.data(), (int)f.s.size(), SQLITE_TRANSIENT); break;
      default: rc = sqlite3_bind_blob(st, k, f.s.data(), (int)f.s.size(), SQLITE_TRANSIENT); break;
    }
    if (rc != SQLITE_OK) {
      release(st);
      fail(rc);
    }
  }
  return st;
}

static Field column(sqlite3_stmt* st, int i) {
  switch (sqlite3_column_type(st, i)) {
    case SQLITE_INTEGER: return Field::integer(sqlite3_column_int64(st, i));
    case SQLITE_FLOAT: return Field::real(sqlite3_column_double(st, i));
    case SQLITE_TEXT: {
      const char* p = (const char*)sqlite3_column_text(st, i);
      return Field::text(std::string(p ? p : "", sqlite3_column_bytes(st, i)));
    }
    case SQLITE_BLOB: {
      const char* p = (const char*)sqlite3_column_blob(st, i);
      return Field::bytes(std::string(p ? p : "", sqlite3_column_bytes(st, i)));
    }
    default: return Field::none();
  }
}

std::vector<Row> Conn::rows(const std::string& sql, const std::vector<Field>& args) {
  std::lock_guard<std::recursive_mutex> g(mu);
  sqlite3_stmt* st = prepare(sql, args);
  std::vector<Row> out;
  int n = sqlite3_column_count(st);
  for (;;) {
    int rc = sqlite3_step(st);
    if (rc == SQLITE_DONE) break;
    if (rc != SQLITE_ROW) {
      release(st);
      fail(rc);
    }
    Row r;
    r.reserve(n);
    for (int i = 0; i < n; i++) r.push_back(column(st, i));
    out.push_back(std::move(r));
  }
  release(st);
  return out;
}

bool Conn::one(const std::string& sql, const std::vector<Field>& args, Row& out) {
  std::lock_guard<std::recursive_mutex> g(mu);
  sqlite3_stmt* st = prepare(sql, args);
  int rc = sqlite3_step(st);
  bool got = false;
  if (rc == SQLITE_ROW) {
    int n = sqlite3_column_count(st);
    out.clear();
    for (int i = 0; i < n; i++) out.push_back(column(st, i));
    got = true;
  } else if (rc != SQLITE_DONE) {
    release(st);
    fail(rc);
  }
  release(st);
  return got;
}

int64_t Conn::run(const std::string& sql, const std::vector<Field>& args) {
  std::lock_guard<std::recursive_mutex> g(mu);
  sqlite3_stmt* st = prepare(sql, args);
  int rc;
  while ((rc = sqlite3_step(st)) == SQLITE_ROW) {
  }
  if (rc != SQLITE_DONE) {
    release(st);
    fail(rc);
  }
  release(st);
  return sqlite3_changes64(db_);
}

int64_t Conn::last_rowid() { return sqlite3_last_insert_rowid(db_); }

bool Conn::in_transaction() { return sqlite3_get_autocommit(db_) == 0; }

std::vector<std::string> Conn::column_names(const std::string& table) {
  std::vector<std::string> out;
  for (auto& r : rows("PRAGMA table_info(" + table + ")")) out.push_back(r[1].s);
  return out;
}

Tx::Tx(Conn& c) : c_(c) {
  c_.mu.lock();
  if (!c_.in_transaction()) {
    try {
      c_.exec("BEGIN");
    } catch (...) {
      c_.mu.unlock();
      throw;
    }
    mine_ = true;
  }
}

Tx::~Tx() {
  if (mine_ && !done_) {
    try {
      c_.exec("ROLLBACK");
    } catch (...) {
    }
  }
  c_.mu.unlock();
}

void Tx::commit() {
  if (mine_ && !done_) c_.exec("COMMIT");
  done_ = true;
}

void write_rows(Writer& w, const std::vector<Row>& rows) {
  for (auto& r : rows)
    for (auto& f : r) w.field(f);
}

}  // namespace cbe
