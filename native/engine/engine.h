// engine.h -- the incremental engine's core (codebreaker/engine/engine.py): the step registry, a
// run (each fact decided current or made again at most once in it), the contexts a step is
// given, and the worker pool that makes stale facts side by side.
//
// A step's code is still Python where it is written in Python: the run calls it back (make), and
// asks Python for the two things only Python knows -- a step's version (a digest of its source,
// inspect) and a knowledge-base piece's hash now (reads.digest: its kind brought up to date first
// by Python's table builders).  Everything else -- which facts are current, the inputs recorded,
// what is written, kept or remade, the order and the threads -- is here.
#pragma once
#include <atomic>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "facts.h"

namespace cbe {

// The calls into the Python that remains.  Each returns 0, or a Python exception's number in the
// shim's table shifted left once, its low bit set when it is a LookupError.
struct Callbacks {
  int64_t (*step)(const char* kind) = nullptr;                       // register the step of `kind` (load its module)
  int64_t (*version)(const char* kind, void* sink) = nullptr;        // the step's version now
  int64_t (*make)(void* run, const char* kind, int32_t n, void** ctxs, int64_t hint, int64_t context,
                  int32_t batch) = nullptr;                          // run the step for these contexts
  int64_t (*digest)(const char* read, void* sink) = nullptr;         // reads.digest(read)
  void (*log)(void* run, const char* text) = nullptr;
};
extern Callbacks g_cb;

struct Sink { std::string s; };
void check_py(int64_t status);                    // a callback's status -> throws Error(ERR_PYTHON)

struct StepInfo {
  std::string kind, platform;
  std::vector<std::string> reads;
  bool batch = false, always = false;
  bool declares(const std::string& kind) const;
};
void register_step(const StepInfo& s);
void unregister_step(const std::string& kind);
std::shared_ptr<const StepInfo> step_of(const std::string& kind);    // LookupError when no step makes it

constexpr const char* KB = "kb";                  // reads.KIND: a piece of the knowledge base

struct Done {
  bool kept = false;                              // has a hash (else an unkept value: hash None)
  std::string hash;
};

class Run;

struct Context {
  Run* run;
  std::shared_ptr<const StepInfo> step;
  std::string key;
  int64_t hint;
  std::vector<Input> inputs;
  bool has_result = false, unkept = false;
  std::string tag, data;

  void may(const std::string& kind) const;
  void need(const std::string& kind, const std::string& key);
  void need_many(const std::string& kind, const std::vector<std::string>& keys, int64_t hint, int jobs,
                 int64_t context);
  bool peek(const std::string& kind, const std::string& key);
  void knowledge(const std::vector<std::string>& reads);
  void record(const std::string& kind, const std::string& key);
};

class Run {
 public:
  Run(std::shared_ptr<Database> db, bool log);
  std::shared_ptr<Database> db;
  std::atomic<bool> log;

  Done current(const std::string& kind, const std::string& key);
  std::string version(const std::string& kind);
  std::vector<std::string> stale(const std::string& kind, const std::vector<std::string>& keys);
  void many(const std::string& kind, const std::vector<std::string>& keys, int64_t hint, int jobs, int64_t context);
  bool done_of(const std::string& kind, const std::string& key, Done& out);
  void set_done(const std::string& kind, const std::string& key, const Done& d);
  std::vector<std::pair<std::string, std::array<int64_t, 2>>> counts();

 private:
  using Key = std::pair<std::string, std::string>;
  std::recursive_mutex mu_;
  std::map<Key, Done> done_;
  std::map<Key, std::unique_ptr<std::recursive_mutex>> locks_;
  std::map<std::string, std::string> versions_;
  std::vector<std::string> count_order_;
  std::map<std::string, std::array<int64_t, 2>> counts_;

  std::recursive_mutex& lock_for(const std::string& kind, const std::string& key);
  void count(const std::string& kind, bool made);
  bool still(const FactRow& row);
  // rows: what is known of each key's kept row (absent from the map: read it)
  void make(const std::string& kind, const std::vector<std::string>& keys,
            const std::map<std::string, std::unique_ptr<FactRow>>* rows, int64_t hint, int64_t context);
  void say(const std::string& text);
};

}  // namespace cbe
