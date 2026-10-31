// db.h -- one SQLite connection: statements bound from Fields, rows read back as Fields (the
// types Python's sqlite3 would give: int, float, text, bytes, None), transactions.
#pragma once
#include <mutex>
#include <unordered_map>
#include <string>
#include <vector>

#include "util.h"

struct sqlite3;
struct sqlite3_stmt;

namespace cbe {

class Conn {
 public:
  // flags: READWRITE|CREATE, or READONLY (a store opened to read: "file:...?mode=ro")
  Conn(const std::string& path, bool readonly, int timeout_ms);
  ~Conn();
  Conn(const Conn&) = delete;
  Conn& operator=(const Conn&) = delete;

  void exec(const std::string& sql);                                      // one or more statements
  std::vector<Row> rows(const std::string& sql, const std::vector<Field>& args = {});
  bool one(const std::string& sql, const std::vector<Field>& args, Row& out);   // the first row, if any
  int64_t run(const std::string& sql, const std::vector<Field>& args = {});     // -> rows changed
  int64_t last_rowid();
  bool in_transaction();
  std::vector<std::string> column_names(const std::string& table);        // PRAGMA table_info

  std::recursive_mutex mu;           // what a write of several statements goes under
  std::string path;
  bool readonly;

 private:
  sqlite3* db_ = nullptr;
  std::unordered_map<std::string, sqlite3_stmt*> cache_;   // prepared statements, kept (as Python's sqlite3 keeps 128)
  sqlite3_stmt* prepare(const std::string& sql, const std::vector<Field>& args);
  void release(sqlite3_stmt* st);
  [[noreturn]] void fail(int rc);
};

// BEGIN ... COMMIT, or ROLLBACK when left by an exception; inside an open transaction it adds
// nothing (Python's `with conn:` inside another).
class Tx {
 public:
  explicit Tx(Conn& c);
  ~Tx();
  void commit();

 private:
  Conn& c_;
  bool mine_ = false, done_ = false;
};

void write_rows(Writer& w, const std::vector<Row>& rows);

}  // namespace cbe
