// engine_test.cc -- cbengine.dll through its C interface alone (no Python): the fact store, a run
// with steps written here, the store, the knowledge base and the verdict vocabulary.
//
//   cbengine_test <a scratch folder>        (ctest gives it build/testdata; it is emptied first)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "cbengine.h"

namespace fs = std::filesystem;

static int g_failed = 0, g_checks = 0;
#define CHECK(c)                                                            \
  do {                                                                      \
    g_checks++;                                                             \
    if (!(c)) {                                                             \
      g_failed++;                                                           \
      fprintf(stderr, "FAILED %s:%d: %s (%s)\n", __FILE__, __LINE__, #c, cbe_error()); \
    }                                                                       \
  } while (0)

// ---- row buffers ----------------------------------------------------------------------------------
struct F { int t = 0; long long i = 0; double d = 0; std::string s; };

static std::vector<F> fields(const char* p, long long n) {
  std::vector<F> out;
  const char* e = p + n;
  while (p < e) {
    F f;
    f.t = (unsigned char)*p++;
    if (f.t == 1) { memcpy(&f.i, p, 8); p += 8; }
    else if (f.t == 2) { memcpy(&f.d, p, 8); p += 8; }
    else if (f.t == 3 || f.t == 4) { long long k; memcpy(&k, p, 8); p += 8; f.s.assign(p, (size_t)k); p += k; }
    out.push_back(f);
  }
  return out;
}

struct Buf {
  std::string b;
  Buf& text(const std::string& s) { long long n = (long long)s.size(); b.push_back(3); b.append((char*)&n, 8); b += s; return *this; }
  Buf& none() { b.push_back(0); return *this; }
  Buf& integer(long long v) { b.push_back(1); b.append((char*)&v, 8); return *this; }
};

static std::vector<F> take(int rc, char** out, long long* n) {      // (pointers: read after the call)
  std::vector<F> f;
  if (rc == 0) {
    f = fields(*out, *n);
    cbe_free(*out);
  }
  return f;
}

// ---- steps written here ----------------------------------------------------------------------------
// t.up <- t.src: the source upper-cased; t.len <- t.up: its length (so a change of case only
// stops at t.len); t.batch: one call for every key; t.fan <- t.src: made side by side
static std::mutex g_mu;
static std::map<std::string, int> g_made;
static void* g_db = nullptr;

static std::string value_of(const char* kind, const char* key) {
  char* out;
  long long n;
  auto f = take(cbe_db_row(g_db, kind, key, &out, &n), &out, &n);
  return f.size() >= 5 ? f[4].s : std::string();
}

static int64_t step_cb(const char*) { return 0; }
static int64_t version_cb(const char* kind, void* sink) {
  std::string v = std::string("v1:") + kind;
  cbe_sink_put(sink, v.data(), (int64_t)v.size());
  return 0;
}
static int64_t digest_cb(const char* read, void* sink) {
  std::string h = std::string("absent");
  (void)read;
  cbe_sink_put(sink, h.data(), (int64_t)h.size());
  return 0;
}
static void log_cb(void*, const char*) {}

static int64_t make_cb(void*, const char* kind, int32_t n, void** ctxs, int64_t, int64_t, int32_t) {
  std::string k = kind;
  for (int i = 0; i < n; i++) {
    char* out;
    long long len;
    cbe_ctx_key(ctxs[i], &out, &len);
    std::string key(out, (size_t)len);
    cbe_free(out);
    {
      std::lock_guard<std::mutex> g(g_mu);
      g_made[k + "@" + key]++;
    }
    std::string v;
    if (k == "t.up") {
      if (cbe_ctx_need(ctxs[i], "t.src", "")) return 2;
      v = value_of("t.src", "");
      for (auto& c : v) c = (char)toupper((unsigned char)c);
    } else if (k == "t.len") {
      if (cbe_ctx_need(ctxs[i], "t.up", "")) return 2;
      v = std::to_string(value_of("t.up", "").size());
    } else if (k == "t.fan") {
      if (cbe_ctx_need(ctxs[i], "t.src", "")) return 2;
      v = key + ":" + value_of("t.src", "");
    } else if (k == "t.batch") {
      if (key == "skip") { cbe_ctx_unkept(ctxs[i]); continue; }
      v = "b:" + key;
    } else if (k == "t.bad") {
      return cbe_ctx_need(ctxs[i], "t.len", "") ? 2 : 2;     // (t.len is not declared: refused)
    }
    cbe_ctx_result(ctxs[i], "S", v.data(), (int64_t)v.size());
  }
  return 0;
}

static void reg(const char* kind, std::vector<std::string> reads, bool batch = false) {
  Buf b;
  for (auto& r : reads) b.text(r);
  CHECK(cbe_step_register(kind, "", b.b.data(), (int64_t)b.b.size(), batch, 0) == 0);
}

static int made(const std::string& k) {
  std::lock_guard<std::mutex> g(g_mu);
  return g_made[k];
}

static std::string current(void* run, const char* kind, const char* key, int* rc = nullptr) {
  int kept = 0;
  char hash[128];
  int r = cbe_run_current(run, kind, key, &kept, hash);
  if (rc) *rc = r;
  return r == 0 ? std::string(hash) : std::string();
}

int main(int argc, char** argv) {
  fs::path dir = argc > 1 ? fs::path(argv[1]) : fs::current_path() / "testdata";
  std::error_code ec;
  fs::remove_all(dir, ec);
  fs::create_directories(dir);

  // ---- hashes and the fact store ----
  char h[64];
  CHECK(cbe_digest("a", "bc", 2, h) == 0 && std::string(h) == "a9993e364706816aba3e25717850c26c9cd0d89d");
  void* db;
  CHECK(cbe_db_open((dir / "a.sqlite").string().c_str(), &db) == 0);
  g_db = db;
  CHECK(cbe_db_write(db, "t.src", "", "S", "alpha", 5, "given", "", "", 0, 0.0, h) == 0);
  char d2[64];
  cbe_digest("S", "alpha", 5, d2);
  CHECK(std::string(h) == d2);
  std::string big(5000, 'x');
  CHECK(cbe_db_write(db, "t.big", "1", "B", big.data(), (int64_t)big.size(), "given", "", "", 0, 0.0, h) == 0);
  {
    char* out;
    long long n;
    auto f = take(cbe_db_row(db, "t.big", "1", &out, &n), &out, &n);
    CHECK(f.size() == 12 && f[4].t == 0 && f[5].s == h && f[6].i == 5000);
    auto b = take(cbe_db_blob(db, h, &out, &n), &out, &n);
    CHECK(cbe_db_blob(db, h, &out, &n) == 0 && n == 5000);
    cbe_free(out);
    int64_t gone;
    CHECK(cbe_db_forget(db, "t.big", nullptr, &gone) == 0 && gone == 1);
  }

  // ---- a run ----
  cbe_set_callbacks(step_cb, version_cb, make_cb, digest_cb, log_cb);
  reg("t.src", {});
  reg("t.up", {"t.src"});
  reg("t.len", {"t.up"});
  reg("t.fan", {"t.src"});
  reg("t.batch", {}, true);
  reg("t.bad", {"t.src"});
  void* run;
  CHECK(cbe_run_new(db, 0, &run) == 0);
  current(run, "t.len", "");
  CHECK(value_of("t.len", "") == "5" && value_of("t.up", "") == "ALPHA");
  CHECK(made("t.up@") == 1 && made("t.len@") == 1);
  cbe_run_free(run);
  cbe_run_new(db, 0, &run);                                    // warm: nothing made
  current(run, "t.len", "");
  CHECK(made("t.up@") == 1 && made("t.len@") == 1);
  {
    char* out;
    long long n;
    auto f = take(cbe_run_counts(run, &out, &n), &out, &n);      // t.len kept (its inputs checked first)
    bool kept = false;
    for (size_t i = 0; i + 2 < f.size(); i += 3)
      if (f[i].s == "t.len" && f[i + 1].i == 1 && f[i + 2].i == 0) kept = true;
    CHECK(kept);
  }
  cbe_run_free(run);
  cbe_db_write(db, "t.src", "", "S", "ALPHA", 5, "given", "", "", 0, 0.0, h);   // same upper case
  cbe_run_new(db, 0, &run);
  current(run, "t.len", "");
  CHECK(made("t.up@") == 2 && made("t.len@") == 1);            // the change stops where it stops mattering
  cbe_run_free(run);
  cbe_db_write(db, "t.src", "", "S", "alphabet", 8, "given", "", "", 0, 0.0, h);
  cbe_run_new(db, 0, &run);
  current(run, "t.len", "");
  CHECK(made("t.up@") == 3 && made("t.len@") == 2 && value_of("t.len", "") == "8");
  cbe_run_free(run);

  // side by side, two at a time; then a batch with an unkept value
  cbe_run_new(db, 0, &run);
  Buf keys;
  for (const char* k : {"0", "1", "2", "3", "4", "1"}) keys.text(k);
  CHECK(cbe_run_many(run, "t.fan", keys.b.data(), (int64_t)keys.b.size(), 0, 2, 0) == 0);
  CHECK(made("t.fan@0") == 1 && made("t.fan@4") == 1 && value_of("t.fan", "3") == "3:alphabet");
  Buf bk;
  for (const char* k : {"a", "skip", "c"}) bk.text(k);
  CHECK(cbe_run_many(run, "t.batch", bk.b.data(), (int64_t)bk.b.size(), 0, 2, 0) == 0);
  CHECK(value_of("t.batch", "a") == "b:a" && value_of("t.batch", "skip").empty());
  int rc;
  current(run, "t.bad", "", &rc);
  CHECK(rc == 3);                                               // (the callback's own failure, as Python's)
  current(run, "t.nothing", "", &rc);
  CHECK(rc == 1 && strstr(cbe_error(), "no step makes facts of the kind 't.nothing'"));
  cbe_run_free(run);
  cbe_run_new(db, 0, &run);
  CHECK(cbe_run_many(run, "t.batch", bk.b.data(), (int64_t)bk.b.size(), 0, 2, 0) == 0);
  CHECK(made("t.batch@a") == 1 && made("t.batch@skip") == 2);   // only the one not kept
  cbe_run_free(run);

  // a new version drops the old facts
  int64_t dropped;
  int unused;
  cbe_db_cleaned(db, 2, "", &unused);                           // (as in a new process: once per process)
  CHECK(cbe_db_drop_other_versions(db, "t.fan", "v2:t.fan", &dropped) == 0 && dropped == 5);

  // ---- the verdict vocabulary ----
  {
    char* out;
    long long n;
    Buf a;
    a.text("same").text("truncated,foo");
    auto f = take(cbe_verdicts_call("classify", a.b.data(), (int64_t)a.b.size(), &out, &n), &out, &n);
    CHECK(f.size() == 2 && f[0].s == "weak" && f[1].s == "truncated");
    Buf b;
    b.text("consistent");
    f = take(cbe_verdicts_call("classify", b.b.data(), (int64_t)b.b.size(), &out, &n), &out, &n);
    CHECK(f.size() == 2 && f[0].s == "weak" && f[1].s == "fault-only");
    Buf c;
    c.text("no-synth");
    f = take(cbe_verdicts_call("verdict", c.b.data(), (int64_t)c.b.size(), &out, &n), &out, &n);
    CHECK(f.size() == 1 && f[0].s == "no-source");
    Buf d;
    d.text(" Runtime Library ");
    f = take(cbe_verdicts_call("klass", d.b.data(), (int64_t)d.b.size(), &out, &n), &out, &n);
    CHECK(f.size() == 1 && f[0].s == "identified");
  }

  // ---- the function store ----
  {
    void* st;
    std::string sha(64, 'a');
    CHECK(cbe_store_open((dir / "functions.sqlite").string().c_str(), (dir / "db").string().c_str(), 0, &st) == 0);
    char* out;
    long long n;
    Buf m;
    m.text(sha).text("x.dll").text("C:/x.dll").text("pe");
    CHECK(take(cbe_store_call(st, "add_module", m.b.data(), (int64_t)m.b.size(), &out, &n), &out, &n).size() == 1);
    Buf r;
    r.text(sha).text("core").text("1").text("p1");
    r.text("1000").text("f").text("int f;").text("s1").text("same").none().text("truncated").none().none();
    r.text("2000").text("g").none().none().text("no-synth").none().none().none().text("no C");
    CHECK(cbe_store_call(st, "record_many", r.b.data(), (int64_t)r.b.size(), &out, &n) == 0);
    cbe_free(out);
    Buf t;
    t.text("p1").text(sha);
    auto f = take(cbe_store_call(st, "tally", t.b.data(), (int64_t)t.b.size(), &out, &n), &out, &n);
    std::map<std::string, long long> tally;
    for (size_t i = 1; i + 1 < f.size(); i += 2) tally[f[i].s] = f[i + 1].i;
    CHECK(tally["weak"] == 1 && tally["no-source"] == 1 && tally["verified"] == 0);
    Buf o;
    f = take(cbe_store_call(st, "overview", o.b.data(), 0, &out, &n), &out, &n);
    CHECK(f.size() == 8 && f[2].s == "x.dll" && f[5].i == 2);
    cbe_store_free(st);
  }

  // ---- the knowledge base ----
  {
    void* kb;
    std::string p = (dir / "db" / "knowledge.sqlite").string();
    CHECK(cbe_kb_open(p.c_str(), &kb) == 0);
    char* out;
    long long n;
    Buf put;
    put.text("win32").text("prototype").text("test").text("learned");
    put.text("GetDlgItem").text("S").text("HWND GetDlgItem(HWND,int)");
    CHECK(cbe_kb_call(kb, "put_many", put.b.data(), (int64_t)put.b.size(), &out, &n) == 0);
    cbe_free(out);
    Buf q;
    q.text("win32:prototype:GetDlgItem");
    auto f = take(cbe_kb_call(kb, "digest", q.b.data(), (int64_t)q.b.size(), &out, &n), &out, &n);
    char want[64];
    std::string v = "HWND GetDlgItem(HWND,int)";
    cbe_digest("S", v.data(), (int64_t)v.size(), want);
    CHECK(f.size() == 2 && f[1].s == want);
    Buf q2;
    q2.text("win32:prototype:Nope");
    f = take(cbe_kb_call(kb, "digest", q2.b.data(), (int64_t)q2.b.size(), &out, &n), &out, &n);
    CHECK(f.size() == 2 && f[1].s == "absent");
    cbe_kb_close(kb);
    cbe_kb_release(kb);
  }

  cbe_db_close(db);
  cbe_db_release(db);
  printf("%d checks, %d failed\n", g_checks, g_failed);
  return g_failed ? 1 : 0;
}
