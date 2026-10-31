// api.cc -- the C interface (cbengine.h): each call caught, its error kept for the shim to raise.
#include "cbengine.h"

#include <cstdlib>
#include <cstring>

#include "engine.h"
#include "facts.h"
#include "knowledge.h"
#include "store.h"
#include "verdicts.h"

using namespace cbe;

namespace {
thread_local std::string t_error;
thread_local int64_t t_py = 0;
thread_local int t_code = 0;

template <class F>
int guard(F&& f) {
  try {
    f();
    return 0;
  } catch (const Error& e) {
    t_error = e.what();
    t_py = e.py;
    t_code = e.code;
    if (e.kind == ERR_PYTHON) return ERR_PYTHON;
    return e.kind;
  } catch (const std::exception& e) {
    t_error = e.what();
    t_py = 0;
    t_code = 0;
    return ERR_OTHER;
  } catch (...) {
    t_error = "an unknown C++ exception";
    t_py = 0;
    t_code = 0;
    return ERR_OTHER;
  }
}

void give(const std::string& s, char** out, int64_t* n) {
  *n = (int64_t)s.size();
  *out = (char*)malloc(s.size() ? s.size() : 1);
  if (!s.empty()) memcpy(*out, s.data(), s.size());
}

std::shared_ptr<Database>& D(void* h) { return *static_cast<std::shared_ptr<Database>*>(h); }
Run* R(void* h) { return static_cast<Run*>(h); }
Context* C(void* h) { return static_cast<Context*>(h); }

std::vector<std::string> strings(const char* p, int64_t n) {
  Reader r(p, (size_t)n);
  return r.strings();
}

std::vector<std::vector<std::string>> triples(const char* p, int64_t n) {
  auto s = strings(p, n);
  std::vector<std::vector<std::string>> out;
  for (size_t i = 0; i + 2 < s.size(); i += 3) out.push_back({s[i], s[i + 1], s[i + 2]});
  return out;
}

struct StoreH { std::unique_ptr<FunctionStore> s; };
}  // namespace

extern "C" {

int cbe_abi(void) { return 1; }
const char* cbe_error(void) { return t_error.c_str(); }
int64_t cbe_error_py(void) { return t_py; }
int cbe_error_code(void) { return t_code; }
void cbe_free(void* p) { free(p); }
void cbe_sink_put(void* sink, const char* p, int64_t n) { static_cast<Sink*>(sink)->s.assign(p, (size_t)n); }

void cbe_set_callbacks(cbe_step_cb step, cbe_version_cb version, cbe_make_cb make, cbe_digest_cb digest,
                       cbe_log_cb log) {
  g_cb.step = step;
  g_cb.version = version;
  g_cb.make = make;
  g_cb.digest = digest;
  g_cb.log = log;
}

int cbe_step_register(const char* kind, const char* platform, const char* reads, int64_t nreads, int batch, int always) {
  return guard([&] {
    StepInfo s;
    s.kind = kind;
    s.platform = platform ? platform : "";
    s.reads = strings(reads, nreads);
    s.batch = batch != 0;
    s.always = always != 0;
    register_step(s);
  });
}

int cbe_step_unregister(const char* kind) { return guard([&] { unregister_step(kind); }); }

// ---- the program database -----------------------------------------------------------------------
int cbe_db_open(const char* path, void** out) {
  return guard([&] { *out = new std::shared_ptr<Database>(open_at(path)); });
}

void cbe_db_release(void* db) { delete static_cast<std::shared_ptr<Database>*>(db); }

int cbe_db_close(void* db) {
  return guard([&] { close_db(D(db)->path); });
}

int cbe_db_path(void* db, char** out, int64_t* n) { return guard([&] { give(D(db)->path, out, n); }); }

int cbe_db_set_sha(void* db, const char* sha) { return guard([&] { D(db)->sha = sha; }); }

int cbe_db_row(void* db, const char* kind, const char* key, char** out, int64_t* n) {
  return guard([&] {
    FactRow r;
    Writer w;
    if (D(db)->row(kind, key, r)) r.write(w);
    give(w.out, out, n);
  });
}

int cbe_db_rows(void* db, const char* kind, char** out, int64_t* n) {
  return guard([&] {
    std::string k = kind ? kind : "";
    Writer w;
    for (auto& r : D(db)->rows(kind ? &k : nullptr)) r.write(w);
    give(w.out, out, n);
  });
}

int cbe_db_kinds(void* db, char** out, int64_t* n) {
  return guard([&] {
    Writer w;
    write_rows(w, D(db)->kinds());
    give(w.out, out, n);
  });
}

int cbe_db_blob(void* db, const char* hash, char** out, int64_t* n) {
  return guard([&] { give(D(db)->blob(hash), out, n); });
}

int cbe_db_about(void* db, const char* name, char** out, int64_t* n, int* found) {
  return guard([&] {
    std::string v;
    *found = D(db)->about(name, v) ? 1 : 0;
    give(v, out, n);
  });
}

int cbe_db_set_about(void* db, const char* name, const char* value) {
  return guard([&] { D(db)->set_about(name, value); });
}

int cbe_db_write(void* db, const char* kind, const char* key, const char* tag, const char* data, int64_t ndata,
                 const char* step, const char* version, const char* inputs, int64_t ninputs, double took, char* hash) {
  return guard([&] {
    std::string h = D(db)->write(kind, key, tag, std::string(data, (size_t)ndata), step, version,
                                 json_inputs(triples(inputs, ninputs)), took);
    strncpy(hash, h.c_str(), 63);
    hash[63] = 0;
  });
}

int cbe_db_write_raw(void* db, const char* kind, const char* key, const char* hash, const char* tag, const char* data,
                     int64_t ndata, const char* step, const char* version, const char* inputs_json, double took) {
  return guard([&] {
    D(db)->write_raw(kind, key, hash, tag, std::string(data, (size_t)ndata), step, version, inputs_json, took);
  });
}

int cbe_db_remake(void* db, const char* kind, const char* key, const char* version, const char* inputs, int64_t ninputs) {
  return guard([&] { D(db)->remake(kind, key, version, json_inputs(triples(inputs, ninputs))); });
}

int cbe_db_forget(void* db, const char* kind, const char* key, int64_t* n) {
  return guard([&] {
    std::string k = key ? key : "";
    *n = D(db)->forget(kind, key ? &k : nullptr);
  });
}

int cbe_db_drop_other_versions(void* db, const char* step, const char* version, int64_t* n) {
  return guard([&] { *n = D(db)->drop_other_versions(step, version); });
}

int cbe_db_collect(void* db) { return guard([&] { D(db)->collect(); }); }

int cbe_db_cleaned(void* db, int op, const char* step, int* out) {
  return guard([&] {
    *out = 0;
    if (op == 0) *out = D(db)->cleaned(step) ? 1 : 0;
    else if (op == 1) D(db)->drop_other_versions(step, std::string("\x01never a version"));   // (unused)
    else D(db)->clear_cleaned();
  });
}

int cbe_digest(const char* tag, const char* data, int64_t n, char* out) {
  return guard([&] {
    std::string h = sha1_hex2(tag, data, (size_t)n);
    memcpy(out, h.c_str(), 41);
  });
}

int cbe_sha(const char* path, char* out) {
  return guard([&] {
    std::string h = sha_file(path);
    memcpy(out, h.c_str(), 65);
  });
}

int cbe_databases(const char* folder, char** out, int64_t* n) {
  return guard([&] {
    Writer w;
    for (auto& d : databases(folder)) {
      w.text(d.path);
      w.integer(d.size);
      w.real(d.mtime);
    }
    give(w.out, out, n);
  });
}

int cbe_hold(int64_t cap, const char* folder, char** out, int64_t* n) {
  return guard([&] {
    Writer w;
    for (auto& p : hold(cap, folder)) w.text(p);
    give(w.out, out, n);
  });
}

// ---- runs ----------------------------------------------------------------------------------------
int cbe_run_new(void* db, int log, void** out) {
  return guard([&] { *out = new Run(D(db), log != 0); });
}

void cbe_run_free(void* run) { delete R(run); }

int cbe_run_set_log(void* run, int log) {
  return guard([&] { R(run)->log = log != 0; });
}

int cbe_run_current(void* run, const char* kind, const char* key, int* kept, char* hash) {
  return guard([&] {
    Done d = R(run)->current(kind, key);
    *kept = d.kept ? 1 : 0;
    strncpy(hash, d.hash.c_str(), 127);
    hash[127] = 0;
  });
}

int cbe_run_many(void* run, const char* kind, const char* keys, int64_t nkeys, int64_t hint, int jobs, int64_t context) {
  return guard([&] { R(run)->many(kind, strings(keys, nkeys), hint, jobs, context); });
}

int cbe_run_stale(void* run, const char* kind, const char* keys, int64_t nkeys, char** out, int64_t* n) {
  return guard([&] {
    Writer w;
    for (auto& k : R(run)->stale(kind, strings(keys, nkeys))) w.text(k);
    give(w.out, out, n);
  });
}

int cbe_run_counts(void* run, char** out, int64_t* n) {
  return guard([&] {
    Writer w;
    for (auto& kv : R(run)->counts()) {
      w.text(kv.first);
      w.integer(kv.second[0]);
      w.integer(kv.second[1]);
    }
    give(w.out, out, n);
  });
}

int cbe_ctx_need(void* ctx, const char* kind, const char* key) { return guard([&] { C(ctx)->need(kind, key); }); }

int cbe_ctx_need_many(void* ctx, const char* kind, const char* keys, int64_t nkeys, int64_t hint, int jobs,
                      int64_t context) {
  return guard([&] { C(ctx)->need_many(kind, strings(keys, nkeys), hint, jobs, context); });
}

int cbe_ctx_peek(void* ctx, const char* kind, const char* key, int* found) {
  return guard([&] { *found = C(ctx)->peek(kind, key) ? 1 : 0; });
}

int cbe_ctx_knowledge(void* ctx, const char* reads, int64_t nreads) {
  return guard([&] { C(ctx)->knowledge(strings(reads, nreads)); });
}

int cbe_ctx_result(void* ctx, const char* tag, const char* data, int64_t n) {
  return guard([&] {
    Context* c = C(ctx);
    c->tag = tag;
    c->data.assign(data, (size_t)n);
    c->unkept = false;
    c->has_result = true;
  });
}

int cbe_ctx_unkept(void* ctx) {
  return guard([&] {
    C(ctx)->unkept = true;
    C(ctx)->has_result = true;
  });
}

int cbe_ctx_inputs(void* ctx, char** out, int64_t* n) {
  return guard([&] {
    Writer w;
    for (auto& in : C(ctx)->inputs) {
      w.text(in[0]);
      w.text(in[1]);
      w.text(in[2]);
    }
    give(w.out, out, n);
  });
}

int cbe_ctx_key(void* ctx, char** out, int64_t* n) { return guard([&] { give(C(ctx)->key, out, n); }); }

// ---- the function store and the knowledge base ------------------------------------------------------
int cbe_store_open(const char* path, const char* folder, int readonly, void** out) {
  return guard([&] {
    auto h = new StoreH();
    try {
      h->s.reset(new FunctionStore(path, folder, readonly != 0));
    } catch (...) {
      delete h;
      throw;
    }
    *out = h;
  });
}

int cbe_store_call(void* store, const char* method, const char* args, int64_t nargs, char** out, int64_t* n) {
  return guard([&] {
    Reader r(args, (size_t)nargs);
    Writer w;
    static_cast<StoreH*>(store)->s->call(method, r, w);
    give(w.out, out, n);
  });
}

void cbe_store_free(void* store) {
  auto h = static_cast<StoreH*>(store);
  try {
    if (h->s) h->s->close();
  } catch (...) {
  }
  delete h;
}

int cbe_kb_open(const char* path, void** out) {
  // a connection of its own each (the shim keeps one per path, as knowledge/store.py's open() does)
  return guard([&] { *out = new std::shared_ptr<KnowledgeBase>(std::make_shared<KnowledgeBase>(path)); });
}

int cbe_kb_call(void* kb, const char* method, const char* args, int64_t nargs, char** out, int64_t* n) {
  return guard([&] {
    Reader r(args, (size_t)nargs);
    Writer w;
    (*static_cast<std::shared_ptr<KnowledgeBase>*>(kb))->call(method, r, w);
    give(w.out, out, n);
  });
}

void cbe_kb_release(void* kb) { delete static_cast<std::shared_ptr<KnowledgeBase>*>(kb); }

int cbe_kb_close(void* kb) { return guard([&] { (*static_cast<std::shared_ptr<KnowledgeBase>*>(kb))->close(); }); }

// ---- the verdict vocabulary --------------------------------------------------------------------------
int cbe_verdicts_call(const char* method, const char* args, int64_t nargs, char** out, int64_t* n) {
  return guard([&] {
    std::string M = method;
    Reader a(args, (size_t)nargs);
    Writer w;
    auto word = [&](std::string (*f)(const std::string&)) {
      Field x = a.next();
      if (x.null()) w.none();
      else w.text(f(x.s));
    };
    if (M == "verdict") word(V::verdict);
    else if (M == "klass") word(V::klass);
    else if (M == "match") word(V::match);
    else if (M == "classify") {
      Field v = a.next();
      Field lim = a.done() ? Field::none() : a.next();
      auto c = V::classify(v.null() ? nullptr : &v.s, lim.type == 3 ? lim.s : std::string());
      w.text(c.first);
      for (auto& s : c.second) w.text(s);
    } else if (M == "table") {                       // a table by name, for the shim's check against verdicts.py
      std::string t = a.str();
      const std::vector<std::string>* list = t == "VERDICTS" ? &V::VERDICTS : t == "GOOD" ? &V::GOOD
                                           : t == "ORDER" ? &V::ORDER : t == "SCORED" ? &V::SCORED
                                           : t == "BUILT" ? &V::BUILT : t == "MATCHES" ? &V::MATCHES : nullptr;
      const std::vector<std::pair<std::string, std::string>>* map =
          t == "OLD" ? &V::OLD : t == "CLASS_OLD" ? &V::CLASS_OLD : t == "MATCH_OLD" ? &V::MATCH_OLD
          : t == "CLASS" ? &V::CLASS : t == "TITLE" ? &V::TITLE : t == "LABEL" ? &V::LABEL
          : t == "SAID" ? &V::SAID : t == "MATCH_LABEL" ? &V::MATCH_LABEL : t == "WEAK_LIMITS" ? &V::WEAK_LIMITS
          : nullptr;
      if (list) for (auto& s : *list) w.text(s);
      else if (map) for (auto& kv : *map) { w.text(kv.first); w.text(kv.second); }
      else raise(ERR_LOOKUP, "no verdict table " + t);
    } else {
      raise(ERR_LOOKUP, "the verdict vocabulary has no call " + M);
    }
    give(w.out, out, n);
  });
}

}  // extern "C"
