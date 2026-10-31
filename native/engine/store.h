// store.h -- the function store (codebreaker/store/functions.py): every function of every module
// and what was made of it.  A module's tables (function, candidate, verdict, text, identified,
// built, variant) are in that binary's program database -- the same connection the engine keeps
// its facts on --, and the store's own file is their index (module, profile, summary,
// store_about).  The same schema as the Python store: a store written by either reads in the
// other, and a store from before the databases is carried over the same way, once.
#pragma once
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>

#include "db.h"
#include "facts.h"

namespace cbe {

class FunctionStore {
 public:
  // path: the index's file; folder: where the modules' databases are
  FunctionStore(const std::string& path, const std::string& folder, bool readonly);
  std::string path, folder;
  bool readonly;

  // one method of functions.FunctionStore by name; arguments and result as row buffers (the
  // result's first field is how many fields a row has)
  void call(const std::string& method, Reader& args, Writer& out);
  void carry(Writer& out);                 // -> (name, count) rows of what was carried this time
  void close();

 private:
  struct Module {
    std::shared_ptr<Conn> conn;            // the engine's connection to the database (or one of its own, read-only)
    std::shared_ptr<Database> db;          // (keeps the engine's open: shared)
    bool empty = false;                    // read-only, and there is no database
  };
  std::unique_ptr<Conn> index_;
  std::mutex mu_;
  std::map<std::string, std::shared_ptr<Module>> modules_;

  std::shared_ptr<Module> m(const std::string& module);
  std::vector<Row> mrows(const std::string& module, const std::string& sql, const std::vector<Field>& args);
  bool mone(const std::string& module, const std::string& sql, const std::vector<Field>& args, Row& out);
  Conn& writable(const std::string& module);
  void summarize(const std::string& module);
  static int64_t function_id(Conn& c, const std::string& module, const Field& addr, const Field& name,
                             const Field& size);

  // a candidates() row: addr, engine, rev, sig, name, source, problem, verdict, reason, limits, detail, judged_sig
  std::vector<Row> candidates(const std::string& module, const Field& profile, const Field& engine);
  std::vector<std::pair<std::string, Row>> best(const std::string& module, const Field& profile,
                                                const std::vector<std::string>& prefer);
  std::vector<Row> variants(const std::string& module, const Field& compiler);
  std::vector<std::pair<std::string, Row>> best_variant(const std::string& module, const Field& compiler);
  std::vector<std::string> addresses(const std::string& module);
  std::map<std::string, std::pair<Field, Field>> identified(const std::string& module);
  std::string about(const std::string& name, bool* has);
  void set_about(const std::string& name, const std::string& value);
  bool old_tables();
  std::vector<std::pair<std::string, int64_t>> carry_module(const std::string& sha, double since,
                                                            const std::set<std::string>& names);
};

std::string latest_sql();

}  // namespace cbe
