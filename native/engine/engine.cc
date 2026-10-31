// engine.cc -- the incremental engine's core (engine.py's Run and Context), with its worker pool.
#include "engine.h"

#include <algorithm>
#include <chrono>
#include <exception>
#include <set>
#include <thread>

namespace cbe {

Callbacks g_cb;

void check_py(int64_t status) {
  if (!status) return;
  Error e(ERR_PYTHON, "an exception in Python");
  e.py = status >> 1;
  e.py_lookup = (status & 1) != 0;
  throw e;
}

static double clock_now() {
  using namespace std::chrono;
  return duration_cast<duration<double>>(system_clock::now().time_since_epoch()).count();
}

// ---- the step registry -----------------------------------------------------------------------
static std::mutex g_steps_mu;
static std::map<std::string, std::shared_ptr<const StepInfo>> g_steps;

bool StepInfo::declares(const std::string& k) const {
  return std::find(reads.begin(), reads.end(), k) != reads.end();
}

void register_step(const StepInfo& s) {
  std::lock_guard<std::mutex> g(g_steps_mu);
  g_steps[s.kind] = std::make_shared<const StepInfo>(s);
}

void unregister_step(const std::string& kind) {
  std::lock_guard<std::mutex> g(g_steps_mu);
  g_steps.erase(kind);
}

static std::shared_ptr<const StepInfo> find_step(const std::string& kind) {
  std::lock_guard<std::mutex> g(g_steps_mu);
  auto it = g_steps.find(kind);
  return it == g_steps.end() ? nullptr : it->second;
}

std::shared_ptr<const StepInfo> step_of(const std::string& kind) {
  auto s = find_step(kind);
  if (!s && g_cb.step) {                        // (its module not imported yet: the platforms' step modules)
    check_py(g_cb.step(kind.c_str()));
    s = find_step(kind);
  }
  if (!s) raise(ERR_LOOKUP, "no step makes facts of the kind " + py_repr(kind));
  return s;
}

static std::string digest_of(const std::string& read) {
  Sink sink;
  check_py(g_cb.digest(read.c_str(), &sink));
  return sink.s;
}

// ---- a run -----------------------------------------------------------------------------------
Run::Run(std::shared_ptr<Database> d, bool lg) : db(std::move(d)), log(lg) {}

std::recursive_mutex& Run::lock_for(const std::string& kind, const std::string& key) {
  std::lock_guard<std::recursive_mutex> g(mu_);
  auto& p = locks_[Key(kind, key)];
  if (!p) p.reset(new std::recursive_mutex());
  return *p;
}

void Run::count(const std::string& kind, bool made) {
  std::lock_guard<std::recursive_mutex> g(mu_);
  auto it = counts_.find(kind);
  if (it == counts_.end()) {
    count_order_.push_back(kind);
    it = counts_.emplace(kind, std::array<int64_t, 2>{0, 0}).first;
  }
  it->second[made ? 1 : 0]++;
}

std::vector<std::pair<std::string, std::array<int64_t, 2>>> Run::counts() {
  std::lock_guard<std::recursive_mutex> g(mu_);
  std::vector<std::pair<std::string, std::array<int64_t, 2>>> out;
  for (auto& k : count_order_) out.emplace_back(k, counts_[k]);
  return out;
}

bool Run::done_of(const std::string& kind, const std::string& key, Done& out) {
  std::lock_guard<std::recursive_mutex> g(mu_);
  auto it = done_.find(Key(kind, key));
  if (it == done_.end()) return false;
  out = it->second;
  return true;
}

void Run::set_done(const std::string& kind, const std::string& key, const Done& d) {
  std::lock_guard<std::recursive_mutex> g(mu_);
  done_[Key(kind, key)] = d;
}

void Run::say(const std::string& text) {
  if (log && g_cb.log) g_cb.log(this, text.c_str());
}

std::string Run::version(const std::string& kind) {
  std::lock_guard<std::recursive_mutex> g(mu_);
  auto it = versions_.find(kind);
  if (it != versions_.end()) return it->second;
  step_of(kind);
  Sink sink;
  check_py(g_cb.version(kind.c_str(), &sink));
  versions_[kind] = sink.s;
  db->drop_other_versions(kind, sink.s);
  return sink.s;
}

Done Run::current(const std::string& kind, const std::string& key) {
  Done d;
  if (done_of(kind, key, d)) return d;
  if (kind == KB) {                                   // a piece of the knowledge base: its hash now, once a run
    std::string h = digest_of(key);
    std::lock_guard<std::recursive_mutex> g(mu_);
    auto it = done_.find(Key(kind, key));
    if (it != done_.end()) return it->second;
    d.kept = true;
    d.hash = h;
    done_[Key(kind, key)] = d;
    return d;
  }
  std::lock_guard<std::recursive_mutex> lk(lock_for(kind, key));
  if (done_of(kind, key, d)) return d;
  auto row = std::make_unique<FactRow>();
  bool have = db->row(kind, key, *row);
  if (have && still(*row)) {
    d.kept = true;
    d.hash = row->hash;
    set_done(kind, key, d);
    count(kind, false);
    return d;
  }
  std::map<std::string, std::unique_ptr<FactRow>> rows;
  rows[key] = have ? std::move(row) : nullptr;
  make(kind, {key}, &rows, 0, 0);
  done_of(kind, key, d);
  return d;
}

bool Run::still(const FactRow& row) {
  if (row.given()) return true;
  auto s = step_of(row.kind);
  if (s->always || row.version.null() || row.version.s != version(row.kind)) return false;
  for (auto& in : row.parsed_inputs()) {
    try {
      Done d = current(in[0], in[1]);
      if (!d.kept || d.hash != in[2]) return false;
    } catch (const Error& e) {
      if (!e.lookup()) throw;
      return false;
    }
  }
  return true;
}

std::vector<std::string> Run::stale(const std::string& kind, const std::vector<std::string>& keys) {
  std::vector<std::string> out;
  Done d;
  for (auto& key : keys) {
    if (done_of(kind, key, d)) continue;
    std::lock_guard<std::recursive_mutex> lk(lock_for(kind, key));
    if (done_of(kind, key, d)) continue;
    FactRow row;
    if (db->row(kind, key, row) && still(row)) {
      d.kept = true;
      d.hash = row.hash;
      set_done(kind, key, d);
      count(kind, false);
    } else {
      out.push_back(key);
    }
  }
  return out;
}

void Run::make(const std::string& kind, const std::vector<std::string>& keys,
               const std::map<std::string, std::unique_ptr<FactRow>>* rows, int64_t hint, int64_t context) {
  auto s = step_of(kind);
  std::string ver = version(kind);
  std::vector<std::unique_ptr<Context>> ctxs;
  for (auto& k : keys) {
    auto c = std::make_unique<Context>();
    c->run = this;
    c->step = s;
    c->key = k;
    c->hint = hint;
    ctxs.push_back(std::move(c));
  }
  double t = clock_now();
  if (s->batch) {
    std::vector<void*> ptrs;
    for (auto& c : ctxs) ptrs.push_back(c.get());
    check_py(g_cb.make(this, kind.c_str(), (int32_t)ptrs.size(), ptrs.data(), hint, context, 1));
    std::vector<std::string> missing;
    for (auto& c : ctxs) {
      if (!c->has_result) missing.push_back(c->key);
    }
    if (!missing.empty()) {
      std::string list;
      for (size_t i = 0; i < missing.size() && i < 4; i++) list += (i ? ", " : "") + missing[i];
      raise(ERR_LOOKUP, "step " + kind + " made no " + kind + " for " + list);
    }
  } else {
    for (auto& c : ctxs) {
      void* p = c.get();
      check_py(g_cb.make(this, kind.c_str(), 1, &p, hint, context, 0));
      if (!c->has_result) raise(ERR_OTHER, "step " + kind + " gave no value for " + c->key);
    }
  }
  double took = (clock_now() - t) / (double)std::max<size_t>(1, keys.size());
  for (auto& c : ctxs) {
    const std::string& k = c->key;
    if (c->unkept) {
      db->forget(kind, &k);                            // (what was kept before is no longer it)
      set_done(kind, k, Done());
      count(kind, true);
      continue;
    }
    std::string h = sha1_hex2(c->tag, c->data.data(), c->data.size());
    std::vector<std::vector<std::string>> ins;
    for (auto& in : c->inputs) ins.push_back({in[0], in[1], in[2]});
    std::string ins_json = json_inputs(ins);
    const FactRow* row = nullptr;
    FactRow fresh;
    if (rows && rows->count(k)) {
      row = rows->at(k).get();
    } else if (db->row(kind, k, fresh)) {
      row = &fresh;
    }
    if (row && row->hash == h && !row->given()) {
      db->remake(kind, k, ver, ins_json);              // (the same as before: what was made from it holds)
    } else {
      db->write(kind, k, c->tag, c->data, kind, ver, ins_json, took);
    }
    Done d;
    d.kept = true;
    d.hash = h;
    set_done(kind, k, d);
    count(kind, true);
  }
  double el = clock_now() - t;
  if (log && !keys.empty() && (keys.size() > 1 || el > 1.0))
    say(fmt("  made %d %s (%.1fs)", (int)keys.size(), kind.c_str(), el));
}

void Run::many(const std::string& kind, const std::vector<std::string>& keys_in, int64_t hint, int jobs,
               int64_t context) {
  std::vector<std::string> keys;
  {
    std::set<std::string> seen;
    for (auto& k : keys_in)
      if (seen.insert(k).second) keys.push_back(k);
  }
  std::vector<std::string> todo = stale(kind, keys);
  if (todo.empty()) return;
  auto s = step_of(kind);
  std::vector<std::recursive_mutex*> locks;
  for (auto& k : todo) locks.push_back(&lock_for(kind, k));
  Done d;
  if (s->batch) {
    for (auto* m : locks) m->lock();
    try {
      std::vector<std::string> left;
      for (auto& k : todo)
        if (!done_of(kind, k, d)) left.push_back(k);
      if (!left.empty()) make(kind, left, nullptr, hint, context);
    } catch (...) {
      for (auto* m : locks) m->unlock();
      throw;
    }
    for (auto* m : locks) m->unlock();
    return;
  }
  auto one = [&](const std::string& k, int64_t ctx) {
    std::lock_guard<std::recursive_mutex> lk(lock_for(kind, k));
    Done dd;
    if (!done_of(kind, k, dd)) make(kind, {k}, nullptr, hint, ctx);
  };
  int n = std::min<int>(std::max(jobs, 1), (int)todo.size());
  if (n <= 1) {
    for (auto& k : todo) one(k, 0);
    return;
  }
  double t = clock_now();
  bool had = log.exchange(false);                      // (one line for the lot, not one each)
  std::vector<std::exception_ptr> errs(todo.size());
  std::atomic<size_t> next(0);
  std::vector<std::thread> pool;
  for (int w = 0; w < n; w++) {
    pool.emplace_back([&]() {
      for (;;) {
        size_t i = next++;
        if (i >= todo.size()) return;
        try {
          one(todo[i], context);
        } catch (...) {
          errs[i] = std::current_exception();
        }
      }
    });
  }
  for (auto& th : pool) th.join();
  log = had;
  for (auto& e : errs)
    if (e) std::rethrow_exception(e);                  // (the first item's, once every item has ended)
  if (log) say(fmt("  made %d %s, %d at once (%.1fs)", (int)todo.size(), kind.c_str(), n, clock_now() - t));
}

// ---- a context -------------------------------------------------------------------------------
void Context::may(const std::string& kind) const {
  if (!step->declares(kind) && kind != "binary" && kind != "file")
    raise(ERR_LOOKUP, "step " + step->kind + " reads " + kind + ", which it does not declare (reads=" +
                          py_repr_tuple(step->reads) + ")");
}

void Context::record(const std::string& kind, const std::string& key) {
  Done d;
  run->done_of(kind, key, d);
  inputs.push_back({kind, key, d.kept ? d.hash : std::string("unkept")});
}

void Context::need(const std::string& kind, const std::string& key) {
  may(kind);
  run->current(kind, key);
  record(kind, key);
}

void Context::need_many(const std::string& kind, const std::vector<std::string>& keys, int64_t h, int jobs,
                        int64_t context) {
  may(kind);
  run->many(kind, keys, h, jobs, context);
  std::set<std::string> seen;
  for (auto& k : keys) {
    if (!seen.insert(k).second) continue;
    run->current(kind, k);                             // (as many()'s answer reads each: current already)
    record(kind, k);
  }
}

bool Context::peek(const std::string& kind, const std::string& key) {
  may(kind);
  if (!run->stale(kind, {key}).empty()) return false;
  Done d;
  if (!run->done_of(kind, key, d) || !d.kept) return false;
  record(kind, key);
  return true;
}

void Context::knowledge(const std::vector<std::string>& reads) {
  std::set<std::string> have;
  for (auto& in : inputs)
    if (in[0] == KB) have.insert(in[1]);
  for (auto& read : reads) {
    if (have.count(read)) continue;
    have.insert(read);
    // (read now, not as the run first saw it: a piece the step itself learned is recorded as it now is)
    std::string h = digest_of(read);
    Done d;
    d.kept = true;
    d.hash = h;
    run->set_done(KB, read, d);
    inputs.push_back({KB, read, h});
  }
}

}  // namespace cbe
