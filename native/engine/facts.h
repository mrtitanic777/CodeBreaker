// facts.h -- the program database: one SQLite file per binary, every fact about it with where it
// came from (codebreaker/engine/facts.py, the same schema, so databases written by either read
// unchanged in the other).
//
// A fact is (kind, key, value); with it the step that made it, that step's version and the
// (kind, key, hash) of every fact it was made from.  A value is kept as a tag and bytes (B bytes,
// S text, J JSON, P a pickle -- the Python shim encodes and decodes them: what a value IS is
// Python's), and its hash is the SHA-1 of the tag and the bytes.  Past BLOB_AT bytes a value goes
// once into the blob table by its hash.
#pragma once
#include <array>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include "db.h"
#include "util.h"

namespace cbe {

constexpr size_t BLOB_AT = 4096;
extern const char* const GIVEN;        // "given": the step of a fact set by hand

using Input = std::array<std::string, 3>;          // (kind, key, hash)

struct FactRow {
  std::string kind, key, hash, tag;
  bool has_value = false;
  std::string value;                   // inline bytes
  bool has_blob = false;
  std::string blob;                    // the blob's hash
  Field size, version, inputs, made, took;   // as kept (None where NULL)
  std::string step;
  bool given() const { return step == GIVEN; }
  std::vector<Input> parsed_inputs() const;
  void write(Writer& w) const;         // the 12 columns, in facts.py's order
};

class Database {
 public:
  explicit Database(const std::string& path);
  std::string path;
  std::shared_ptr<Conn> conn;
  std::string sha;                     // the binary's SHA-256 (set by open_for)

  bool row(const std::string& kind, const std::string& key, FactRow& out);
  std::vector<FactRow> rows(const std::string* kind);
  std::vector<Row> kinds();
  std::string blob(const std::string& h);
  bool about(const std::string& name, std::string& out);
  void set_about(const std::string& name, const std::string& value);

  // -> the hash
  std::string write(const std::string& kind, const std::string& key, const std::string& tag,
                    const std::string& data, const std::string& step, const std::string& version,
                    const std::string& inputs_json, double took);
  // a row as given (the binary's fact: its hash is its bytes', not its value's)
  void write_raw(const std::string& kind, const std::string& key, const std::string& hash, const std::string& tag,
                 const std::string& data, const std::string& step, const std::string& version,
                 const std::string& inputs_json, double took);
  void remake(const std::string& kind, const std::string& key, const std::string& version,
              const std::string& inputs_json);
  int64_t forget(const std::string& kind, const std::string* key);
  int64_t drop_other_versions(const std::string& step, const std::string& version);
  void collect();
  bool cleaned(const std::string& step);
  void mark_cleaned(const std::string& step);
  void clear_cleaned();                // (a new process, as far as versions go: the tests)

 private:
  std::mutex cleaned_mu_;
  std::set<std::string> cleaned_;
};

// one Database per path, per process (facts._OPEN)
std::shared_ptr<Database> open_at(const std::string& path);
void close_db(const std::string& path);
std::vector<std::string> open_paths();

std::string sha_file(const std::string& path);                 // facts.sha, kept by (path, size, time)
std::string normcase(const std::string& path);                 // os.path.normcase(os.path.abspath(p))

struct DbFile { std::string path; int64_t size; double mtime; };
std::vector<DbFile> databases(const std::string& folder);
std::vector<std::string> hold(int64_t cap, const std::string& folder);
bool holds_verdicts(const std::string& path);

}  // namespace cbe
