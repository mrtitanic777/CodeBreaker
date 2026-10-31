// store.cc -- the function store (store/functions.py), on SQLite's C API.
#include "store.h"

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <set>

#include "engine.h"
#include "verdicts.h"

namespace fs = std::filesystem;

namespace cbe {

static const char* INDEX =
    "CREATE TABLE IF NOT EXISTS module(\n"
    "    sha TEXT PRIMARY KEY, name TEXT, path TEXT, kind TEXT, ts REAL);\n"
    "CREATE TABLE IF NOT EXISTS profile(digest TEXT PRIMARY KEY, settings TEXT, ts REAL);\n"
    "CREATE TABLE IF NOT EXISTS summary(\n"
    "    module TEXT PRIMARY KEY, functions INTEGER, identified INTEGER, judged INTEGER, last REAL,\n"
    "    last_profile TEXT, profiles TEXT, latest TEXT);\n"
    "CREATE TABLE IF NOT EXISTS store_about(name TEXT PRIMARY KEY, value TEXT);\n";

static const char* MODULE =
    "CREATE TABLE IF NOT EXISTS function(\n"
    "    id INTEGER PRIMARY KEY, module TEXT NOT NULL, addr TEXT NOT NULL, name TEXT, size INTEGER,\n"
    "    UNIQUE(module, addr));\n"
    "CREATE TABLE IF NOT EXISTS candidate(\n"
    "    id INTEGER PRIMARY KEY, function INTEGER NOT NULL, engine TEXT NOT NULL, rev TEXT, sig TEXT,\n"
    "    name TEXT, source TEXT, problem TEXT, ts REAL,\n"
    "    UNIQUE(function, engine));\n"
    "CREATE TABLE IF NOT EXISTS verdict(\n"
    "    candidate INTEGER NOT NULL, profile TEXT NOT NULL, sig TEXT, verdict TEXT, reason TEXT,\n"
    "    limits TEXT, detail TEXT, ts REAL, carried INTEGER NOT NULL DEFAULT 0,\n"
    "    PRIMARY KEY (candidate, profile));\n"
    "CREATE TABLE IF NOT EXISTS text(\n"
    "    module TEXT NOT NULL, engine TEXT NOT NULL, kind TEXT NOT NULL, body TEXT, ts REAL,\n"
    "    PRIMARY KEY (module, engine, kind));\n"
    "CREATE INDEX IF NOT EXISTS candidate_function ON candidate(function);\n"
    "CREATE TABLE IF NOT EXISTS identified(\n"
    "    module TEXT NOT NULL, addr TEXT NOT NULL, name TEXT, library TEXT, ts REAL,\n"
    "    PRIMARY KEY (module, addr));\n"
    "CREATE TABLE IF NOT EXISTS built(\n"
    "    module TEXT NOT NULL, addr TEXT NOT NULL, profile TEXT NOT NULL, sig TEXT,\n"
    "    PRIMARY KEY (module, addr, profile));\n"
    "CREATE TABLE IF NOT EXISTS variant(\n"
    "    id INTEGER PRIMARY KEY, function INTEGER NOT NULL, compiler TEXT NOT NULL, source TEXT,\n"
    "    parent INTEGER, knobs TEXT, distance INTEGER, total INTEGER, verdict TEXT, behaves TEXT, ts REAL,\n"
    "    UNIQUE(function, compiler, knobs));\n"
    "CREATE INDEX IF NOT EXISTS variant_function ON variant(function);\n";

std::string latest_sql() {
  return "\n    SELECT addr, verdict, limits FROM (\n"
         "      SELECT f.addr AS addr, v.verdict AS verdict, v.limits AS limits,\n"
         "             ROW_NUMBER() OVER (PARTITION BY f.id ORDER BY v.ts DESC) AS k\n"
         "      FROM function f JOIN candidate c ON c.function = f.id JOIN verdict v ON v.candidate = c.id\n"
         "      WHERE f.module = ?) WHERE k = 1";
}

static Field T(const std::string& s) { return Field::text(s); }

// `x or ""`
static Field or_empty(const Field& f) {
  if (f.null() || ((f.type == 3 || f.type == 4) && f.s.empty()) || (f.type == 1 && f.i == 0) ||
      (f.type == 2 && f.d == 0.0))
    return Field::text("");
  return f;
}

// V.verdict on a Field (None stays None)
static Field fverdict(const Field& f) { return f.type == 3 ? Field::text(V::verdict(f.s)) : f; }
static Field fmatch(const Field& f) { return f.type == 3 ? Field::text(V::match(f.s)) : f; }

static std::pair<std::string, std::vector<std::string>> fclassify(const Field& verdict, const Field& limits) {
  std::string v = verdict.s;
  std::string lim = limits.type == 3 ? limits.s : std::string();
  return V::classify(verdict.null() ? nullptr : &v, lim);
}

static bool truthy(const Field& f) {
  return !(f.null() || ((f.type == 3 || f.type == 4) && f.s.empty()) || (f.type == 1 && f.i == 0) ||
           (f.type == 2 && f.d == 0.0));
}

static double num(const Field& f) { return f.type == 1 ? (double)f.i : f.type == 2 ? f.d : 0.0; }

static void say(void* who, const std::string& text) {
  if (g_cb.log) g_cb.log(who, text.c_str());
}

static std::string json_dict_sorted(const std::map<std::string, int64_t>& m) {
  std::string o = "{";
  bool first = true;
  for (auto& kv : m) {
    if (!first) o += ", ";
    first = false;
    o += json_str(kv.first) + ": " + std::to_string(kv.second);
  }
  return o + "}";
}

static std::string json_list(const std::vector<std::string>& v) {
  std::string o = "[";
  for (size_t i = 0; i < v.size(); i++) o += (i ? ", " : "") + json_str(v[i]);
  return o + "]";
}

// ---- opening ---------------------------------------------------------------------------------
FunctionStore::FunctionStore(const std::string& p, const std::string& f, bool ro) : path(p), folder(f), readonly(ro) {
  if (ro) {
    index_.reset(new Conn(p, true, 60000));
    return;
  }
  std::error_code ec;
  fs::create_directories(fs::path(widen(p)).parent_path(), ec);
  index_.reset(new Conn(p, false, 60000));
  index_->exec("PRAGMA journal_mode=WAL");
  index_->exec("PRAGMA synchronous=NORMAL");
  index_->exec(INDEX);
}

void FunctionStore::close() {
  std::lock_guard<std::mutex> g(mu_);
  modules_.clear();
  index_.reset();
}

std::shared_ptr<FunctionStore::Module> FunctionStore::m(const std::string& module) {
  std::lock_guard<std::mutex> g(mu_);
  auto it = modules_.find(module);
  if (it != modules_.end()) return it->second;
  std::string p = narrow(fs::path(widen(folder)).wstring() + L"\\" + widen(module + ".sqlite"));
  auto got = std::make_shared<Module>();
  if (readonly) {
    std::error_code ec;
    if (fs::is_regular_file(widen(p), ec)) got->conn = std::make_shared<Conn>(p, true, 60000);
    else got->empty = true;
  } else {
    got->db = open_at(p);                       // (the engine's connection to it, shared)
    got->conn = got->db->conn;
    Tx tx(*got->conn);
    got->conn->exec(MODULE);
    auto cols = got->conn->column_names("verdict");
    if (std::find(cols.begin(), cols.end(), "carried") == cols.end())
      got->conn->exec("ALTER TABLE verdict ADD COLUMN carried INTEGER NOT NULL DEFAULT 0");
    tx.commit();
  }
  modules_[module] = got;
  return got;
}

std::vector<Row> FunctionStore::mrows(const std::string& module, const std::string& sql, const std::vector<Field>& args) {
  auto mm = m(module);
  if (mm->empty) return {};
  return mm->conn->rows(sql, args);
}

bool FunctionStore::mone(const std::string& module, const std::string& sql, const std::vector<Field>& args, Row& out) {
  auto mm = m(module);
  if (mm->empty) return false;
  return mm->conn->one(sql, args, out);
}

Conn& FunctionStore::writable(const std::string& module) {
  if (readonly) {
    Error e(ERR_SQLITE, "attempt to write a readonly database");
    e.code = 8;
    throw e;
  }
  return *m(module)->conn;
}

// ---- the summary of a module, in the index ----------------------------------------------------
void FunctionStore::summarize(const std::string& module) {
  Row r;
  mone(module, "SELECT COUNT(*) FROM function WHERE module=?", {T(module)}, r);
  Field functions = r[0];
  mone(module, "SELECT COUNT(*) FROM identified WHERE module=?", {T(module)}, r);
  Field identified = r[0];
  std::string q = "FROM verdict v JOIN candidate c ON c.id=v.candidate JOIN function f ON f.id=c.function WHERE f.module=?";
  mone(module, "SELECT COUNT(*), MAX(v.ts) " + q, {T(module)}, r);
  Field judged = r[0], last = r[1];
  Row lp;
  bool has_lp = mone(module, "SELECT v.profile " + q + " ORDER BY v.ts DESC LIMIT 1", {T(module)}, lp);
  std::vector<std::string> profiles;
  for (auto& row : mrows(module, "SELECT DISTINCT v.profile " + q, {T(module)})) profiles.push_back(row[0].s);
  std::sort(profiles.begin(), profiles.end());
  std::map<std::string, int64_t> latest;
  for (auto& row : mrows(module, latest_sql(), {T(module)})) latest[fclassify(row[1], row[2]).first]++;
  Tx tx(*index_);
  index_->run("INSERT OR REPLACE INTO summary(module, functions, identified, judged, last, last_profile, "
              "profiles, latest) VALUES(?,?,?,?,?,?,?,?)",
              {T(module), functions, identified, judged, last, has_lp ? lp[0] : Field::none(), T(json_list(profiles)),
               T(json_dict_sorted(latest))});
  tx.commit();
}

int64_t FunctionStore::function_id(Conn& c, const std::string& module, const Field& addr, const Field& name,
                                   const Field& size) {
  Row r;
  if (c.one("SELECT id FROM function WHERE module=? AND addr=?", {T(module), addr}, r)) return r[0].i;
  c.run("INSERT INTO function(module, addr, name, size) VALUES(?,?,?,?)", {T(module), addr, name, size});
  return c.last_rowid();
}

// ---- reading ---------------------------------------------------------------------------------
std::vector<Row> FunctionStore::candidates(const std::string& module, const Field& profile, const Field& engine) {
  std::string q = "SELECT f.addr, c.engine, c.rev, c.sig, c.name, c.source, c.problem, v.verdict, v.reason, "
                  "v.limits, v.detail, v.sig FROM function f JOIN candidate c ON c.function=f.id "
                  "LEFT JOIN verdict v ON v.candidate=c.id AND v.profile=? WHERE f.module=?";
  std::vector<Field> args = {profile, T(module)};
  if (truthy(engine)) {
    q += " AND c.engine=?";
    args.push_back(engine);
  }
  auto rows = mrows(module, q, args);
  for (auto& r : rows) r[7] = fverdict(r[7]);
  return rows;
}

std::vector<std::pair<std::string, Row>> FunctionStore::best(const std::string& module, const Field& profile,
                                                             const std::vector<std::string>& prefer) {
  std::map<std::string, size_t> rank;              // {e: i for i, e in enumerate(prefer)}: a repeated name's last place
  for (size_t i = 0; i < prefer.size(); i++) rank[prefer[i]] = i;
  std::vector<std::string> order;
  std::map<std::string, std::pair<std::pair<int, size_t>, Row>> out;
  for (auto& r : candidates(module, profile, Field::none())) {
    if (r[7].null()) continue;
    auto c = fclassify(r[7], r[9]);
    std::string cls = c.first;
    std::string weak;
    for (size_t i = 0; i < c.second.size(); i++) weak += (i ? "," : "") + c.second[i];
    if (cls != V::NO_SOURCE && !truthy(r[5])) cls = V::NO_SOURCE;
    auto it = rank.find(r[1].s);
    std::pair<int, size_t> key(V::order_index(cls), it == rank.end() ? rank.size() : it->second);
    r.push_back(T(cls));
    r.push_back(T(weak));
    std::string addr = r[0].s;
    auto have = out.find(addr);
    if (have == out.end()) {
      order.push_back(addr);
      out[addr] = {key, r};
    } else if (key < have->second.first) {
      have->second = {key, r};
    }
  }
  std::vector<std::pair<std::string, Row>> res;
  for (auto& a : order) res.emplace_back(a, out[a].second);
  return res;
}

std::vector<std::string> FunctionStore::addresses(const std::string& module) {
  std::vector<std::string> out;
  for (auto& r : mrows(module, "SELECT addr FROM function WHERE module=? ORDER BY addr", {T(module)}))
    out.push_back(r[0].s);
  return out;
}

std::map<std::string, std::pair<Field, Field>> FunctionStore::identified(const std::string& module) {
  std::map<std::string, std::pair<Field, Field>> out;
  for (auto& r : mrows(module, "SELECT addr, name, library FROM identified WHERE module=?", {T(module)}))
    out[r[0].s] = {r[1], r[2]};
  return out;
}

std::vector<Row> FunctionStore::variants(const std::string& module, const Field& compiler) {
  std::string q = "SELECT f.addr, v.compiler, v.source, v.parent, v.knobs, v.distance, v.total, v.verdict, "
                  "v.behaves FROM variant v JOIN function f ON f.id=v.function WHERE f.module=?";
  std::vector<Field> args = {T(module)};
  if (truthy(compiler)) {
    q += " AND v.compiler=?";
    args.push_back(compiler);
  }
  auto rows = mrows(module, q, args);
  for (auto& r : rows) {                             // (older spellings read as the words now)
    r[7] = fmatch(r[7]);
    r[8] = fverdict(r[8]);
  }
  return rows;
}

std::vector<std::pair<std::string, Row>> FunctionStore::best_variant(const std::string& module, const Field& compiler) {
  std::vector<std::string> order;
  std::map<std::string, std::pair<std::pair<int, int64_t>, Row>> out;
  for (auto& r : variants(module, compiler)) {
    bool ok = r[8].type == 3 && (r[8].s == V::EQUIVALENT || r[8].s == V::CONSISTENT || r[8].s.empty());
    int64_t dist = r[5].null() ? ((int64_t)1 << 30) : r[5].i;
    std::pair<int, int64_t> key(ok ? 0 : 1, dist);
    std::string addr = r[0].s;
    auto have = out.find(addr);
    if (have == out.end()) {
      order.push_back(addr);
      out[addr] = {key, r};
    } else if (key < have->second.first) {
      have->second = {key, r};
    }
  }
  std::vector<std::pair<std::string, Row>> res;
  for (auto& a : order) res.emplace_back(a, out[a].second);
  return res;
}

// ---- the store from before the databases, carried over -----------------------------------------
std::string FunctionStore::about(const std::string& name, bool* has) {
  Row r;
  *has = index_->one("SELECT value FROM store_about WHERE name=?", {T(name)}, r) && !r[0].null();
  return *has ? r[0].s : std::string();
}

void FunctionStore::set_about(const std::string& name, const std::string& value) {
  Tx tx(*index_);
  index_->run("INSERT OR REPLACE INTO store_about(name, value) VALUES(?,?)", {T(name), T(value)});
  tx.commit();
}

static std::set<std::string> table_names(Conn& c) {
  std::set<std::string> out;
  for (auto& r : c.rows("SELECT name FROM sqlite_master WHERE type='table'")) out.insert(r[0].s);
  return out;
}

bool FunctionStore::old_tables() {
  auto names = table_names(*index_);
  return names.count("function") && names.count("candidate") && names.count("verdict");
}

void FunctionStore::carry(Writer& out) {
  std::vector<std::pair<std::string, int64_t>> result;
  auto finish = [&]() {
    out.integer(2);
    for (auto& kv : result) {
      out.text(kv.first);
      out.integer(kv.second);
    }
  };
  if (readonly || !old_tables()) return finish();
  auto names = table_names(*index_);
  bool has_upto;
  std::string upto = about("carried_upto", &has_upto);
  double since = has_upto ? strtod(upto.c_str(), nullptr) : 0.0;
  double newest = since;
  for (const char* t : {"candidate", "verdict", "text", "identified", "variant"}) {
    if (!names.count(t)) continue;
    Row r;
    index_->one(std::string("SELECT MAX(ts) FROM ") + t, {}, r);
    newest = std::max(newest, truthy(r[0]) ? num(r[0]) : 0.0);
  }
  bool carried_before;
  about("carried", &carried_before);
  if (carried_before && newest <= since) return finish();
  std::vector<std::pair<std::string, std::string>> q = {
      {"function", "SELECT DISTINCT module FROM function WHERE id IN (SELECT function FROM candidate WHERE ts > ?)"},
      {"verdict", "SELECT DISTINCT f.module FROM verdict v JOIN candidate c ON c.id=v.candidate "
                  "JOIN function f ON f.id=c.function WHERE v.ts > ?"},
      {"text", "SELECT DISTINCT module FROM text WHERE ts > ?"},
      {"identified", "SELECT DISTINCT module FROM identified WHERE ts > ?"},
      {"variant", "SELECT DISTINCT f.module FROM variant v JOIN function f ON f.id=v.function WHERE v.ts > ?"}};
  std::set<std::string> modules;
  if (!carried_before) {
    for (auto& r : index_->rows("SELECT DISTINCT module FROM function")) modules.insert(r[0].s);
    for (auto& r : index_->rows("SELECT sha FROM module")) modules.insert(r[0].s);
  } else {
    for (auto& t : q)
      if (names.count(t.first))
        for (auto& r : index_->rows(t.second, {Field::real(since)})) modules.insert(r[0].s);
  }
  bool loud = !carried_before && !modules.empty();
  auto tell = [&](const std::string& s) {
    if (g_cb.log) say(this, s);
    else if (loud) fprintf(stderr, "%s\n", s.c_str());
  };
  if (loud)
    tell(fmt("the function store: carrying %d modules' rows into their databases (%s), once", (int)modules.size(),
             folder.c_str()));
  // total: in the order its names were first added (a Python dict's), as carried_module gives them
  std::vector<std::pair<std::string, int64_t>>& total = result;
  auto add = [&](const std::string& k, int64_t n, bool replace) {
    for (auto& kv : total)
      if (kv.first == k) { kv.second = replace ? n : kv.second + n; return; }
    total.emplace_back(k, n);
  };
  double t0 = now();
  int i = 0;
  for (auto& sha : modules) {
    for (auto& kv : carry_module(sha, since, names)) add(kv.first, kv.second, false);
    if (i && i % 200 == 0) tell(fmt("  %d of %d modules (%.0fs)", i, (int)modules.size(), now() - t0));
    i++;
  }
  add("modules", (int64_t)modules.size(), true);
  set_about("carried_upto", py_float(newest));
  if (!carried_before) {
    std::string o = "{";                               // json.dumps(dict(total, at=time.time()))
    for (auto& kv : total) o += json_str(kv.first) + ": " + std::to_string(kv.second) + ", ";
    o += "\"at\": " + py_float(now()) + "}";
    set_about("carried", o);
    std::map<std::string, int64_t> sorted(total.begin(), total.end());
    std::string parts;
    for (auto& kv : sorted) parts += (parts.empty() ? "" : ", ") + std::to_string(kv.second) + " " + kv.first;
    tell(fmt("the function store: carried %s (%.0fs)", parts.c_str(), now() - t0));
  }
  finish();
}

std::vector<std::pair<std::string, int64_t>> FunctionStore::carry_module(const std::string& sha, double since,
                                                                         const std::set<std::string>& names) {
  Conn& src = *index_;
  std::map<std::string, int64_t> out;
  Conn& db = writable(sha);
  {
    Tx tx(db);
    for (auto& r : src.rows("SELECT id, addr, name, size FROM function WHERE module=?", {T(sha)}))
      db.run("INSERT OR IGNORE INTO function(module, addr, name, size) VALUES(?,?,?,?)", {T(sha), r[1], r[2], r[3]});
    std::map<std::string, int64_t> new_id;
    for (auto& r : db.rows("SELECT addr, id FROM function WHERE module=?", {T(sha)})) new_id[r[0].s] = r[1].i;
    std::map<int64_t, std::string> addr_of;
    for (auto& r : src.rows("SELECT id, addr FROM function WHERE module=?", {T(sha)})) addr_of[r[0].i] = r[1].s;
    out["functions"] = (int64_t)addr_of.size();
    std::map<int64_t, int64_t> cid;
    int64_t n = 0;
    for (auto& r : src.rows("SELECT c.id, c.function, c.engine, c.rev, c.sig, c.name, c.source, c.problem, c.ts FROM candidate c "
                            "JOIN function f ON f.id=c.function WHERE f.module=?", {T(sha)})) {
      int64_t nf = new_id[addr_of[r[1].i]];
      Row have;
      if (!db.one("SELECT id, ts FROM candidate WHERE function=? AND engine=?", {Field::integer(nf), r[2]}, have)) {
        db.run("INSERT INTO candidate(function, engine, rev, sig, name, source, problem, ts) VALUES(?,?,?,?,?,?,?,?)",
               {Field::integer(nf), r[2], r[3], r[4], r[5], r[6], r[7], r[8]});
        cid[r[0].i] = db.last_rowid();
        n++;
      } else {
        cid[r[0].i] = have[0].i;
        double ts = truthy(r[8]) ? num(r[8]) : 0.0, hts = truthy(have[1]) ? num(have[1]) : 0.0;
        if (ts > hts && ts > since) {
          db.run("UPDATE candidate SET rev=?, sig=?, name=?, source=?, problem=?, ts=? WHERE id=?",
                 {r[3], r[4], r[5], r[6], r[7], r[8], have[0]});
          n++;
        }
      }
    }
    out["candidates"] = n;
    n = 0;
    for (auto& r : src.rows("SELECT v.candidate, v.profile, v.sig, v.verdict, v.reason, v.limits, v.detail, v.ts FROM verdict v "
                            "JOIN candidate c ON c.id=v.candidate JOIN function f ON f.id=c.function WHERE f.module=?",
                            {T(sha)})) {
      Row have;
      bool got = db.one("SELECT ts FROM verdict WHERE candidate=? AND profile=?", {Field::integer(cid[r[0].i]), r[1]}, have);
      double ts = truthy(r[7]) ? num(r[7]) : 0.0;
      if (!got || (ts > (truthy(have[0]) ? num(have[0]) : 0.0) && ts > since)) {
        db.run("INSERT OR REPLACE INTO verdict(candidate, profile, sig, verdict, reason, limits, detail, ts, "
               "carried) VALUES(?,?,?,?,?,?,?,?,1)",
               {Field::integer(cid[r[0].i]), r[1], r[2], r[3], r[4], r[5], r[6], r[7]});
        n++;
      }
    }
    out["verdicts"] = n;
    if (names.count("text")) {
      auto rows = src.rows("SELECT engine, kind, body, ts FROM text WHERE module=?", {T(sha)});
      for (auto& r : rows) {
        Row have;
        bool got = db.one("SELECT ts FROM text WHERE module=? AND engine=? AND kind=?", {T(sha), r[0], r[1]}, have);
        if (!got || (truthy(r[3]) ? num(r[3]) : 0.0) > (truthy(have[0]) ? num(have[0]) : 0.0))
          db.run("INSERT OR REPLACE INTO text(module, engine, kind, body, ts) VALUES(?,?,?,?,?)", {T(sha), r[0], r[1], r[2], r[3]});
      }
      out["texts"] = (int64_t)rows.size();
    }
    if (names.count("identified")) {
      auto rows = src.rows("SELECT addr, name, library, ts FROM identified WHERE module=?", {T(sha)});
      Row nw;
      db.one("SELECT MAX(ts) FROM identified WHERE module=?", {T(sha)}, nw);
      double mx = 0;
      for (auto& r : rows) mx = std::max(mx, truthy(r[3]) ? num(r[3]) : 0.0);
      if (!rows.empty() && (nw[0].null() || mx > num(nw[0]))) {
        db.run("DELETE FROM identified WHERE module=?", {T(sha)});
        for (auto& r : rows)
          db.run("INSERT INTO identified(module, addr, name, library, ts) VALUES(?,?,?,?,?)", {T(sha), r[0], r[1], r[2], r[3]});
      }
      out["identified"] = (int64_t)rows.size();
    }
    if (names.count("built")) {
      auto rows = src.rows("SELECT addr, profile, sig FROM built WHERE module=?", {T(sha)});
      for (auto& r : rows)
        db.run("INSERT OR IGNORE INTO built(module, addr, profile, sig) VALUES(?,?,?,?)", {T(sha), r[0], r[1], r[2]});
      out["built"] = (int64_t)rows.size();
    }
    if (names.count("variant")) {
      auto rows = src.rows("SELECT v.id, v.function, v.compiler, v.source, v.parent, v.knobs, v.distance, v.total, "
                           "v.verdict, v.behaves, v.ts FROM variant v JOIN function f ON f.id=v.function "
                           "WHERE f.module=? ORDER BY v.id", {T(sha)});
      std::vector<int64_t> order;
      std::map<int64_t, std::pair<int64_t, Field>> vid;
      for (auto& r : rows) {
        int64_t nf = new_id[addr_of[r[1].i]];
        db.run("INSERT INTO variant(function, compiler, source, parent, knobs, distance, total, verdict, "
               "behaves, ts) VALUES(?,?,?,?,?,?,?,?,?,?) ON CONFLICT(function, compiler, knobs) DO UPDATE SET "
               "source=excluded.source, distance=excluded.distance, total=excluded.total, "
               "verdict=excluded.verdict, behaves=excluded.behaves, ts=excluded.ts WHERE excluded.ts > variant.ts",
               {Field::integer(nf), r[2], r[3], Field::none(), r[5], r[6], r[7], r[8], r[9], r[10]});
        Row id;
        db.one("SELECT id FROM variant WHERE function=? AND compiler=? AND knobs=?", {Field::integer(nf), r[2], r[5]}, id);
        if (!vid.count(r[0].i)) order.push_back(r[0].i);
        vid[r[0].i] = {id[0].i, r[4]};
      }
      for (auto ov : order) {                         // (a variant's parent, by its new number)
        auto& e = vid[ov];
        if (e.second.type == 1 && vid.count(e.second.i))
          db.run("UPDATE variant SET parent=? WHERE id=?", {Field::integer(vid[e.second.i].first), Field::integer(e.first)});
      }
      out["variants"] = (int64_t)rows.size();
    }
    tx.commit();
  }
  summarize(sha);
  std::vector<std::pair<std::string, int64_t>> ordered;            // (the order Python's dict was filled in)
  for (const char* k : {"functions", "candidates", "verdicts", "texts", "identified", "built", "variants"})
    if (out.count(k)) ordered.emplace_back(k, out[k]);
  return ordered;
}

// ---- one method, by name ------------------------------------------------------------------------
static void rows_out(Writer& out, int ncols, const std::vector<Row>& rows) {
  out.integer(ncols);
  write_rows(out, rows);
}

static Field opt(Reader& a) { return a.done() ? Field::none() : a.next(); }

void FunctionStore::call(const std::string& method, Reader& a, Writer& out) {
  const std::string& M = method;
  if (M == "add_module") {
    std::string sha = a.str();
    Field name = a.next(), p = a.next(), kind = a.next();
    Tx tx(*index_);
    index_->run("INSERT INTO module(sha, name, path, kind, ts) VALUES(?,?,?,?,?) "
                "ON CONFLICT(sha) DO UPDATE SET name=excluded.name, path=excluded.path, "
                "kind=excluded.kind", {T(sha), name, p, kind, Field::real(now())});
    tx.commit();
    m(sha);
    return rows_out(out, 0, {});
  }
  if (M == "add_profile") {
    Field d = a.next(), s = a.next();
    Tx tx(*index_);
    index_->run("INSERT OR IGNORE INTO profile(digest, settings, ts) VALUES(?,?,?)", {d, s, Field::real(now())});
    tx.commit();
    return rows_out(out, 0, {});
  }
  if (M == "add_functions") {
    std::string module = a.str();
    Conn& db = writable(module);
    {
      Tx tx(db);
      while (!a.done()) function_id(db, module, a.next(), Field::none(), Field::none());
      tx.commit();
    }
    summarize(module);
    return rows_out(out, 0, {});
  }
  if (M == "record_many") {
    std::string module = a.str();
    Field engine = a.next(), rev = a.next(), profile = a.next();
    double t = now();
    Conn& db = writable(module);
    {
      Tx tx(db);
      while (!a.done()) {
        Field addr = a.next(), name = a.next(), source = a.next(), sig = a.next(), verdict = a.next(),
              reason = a.next(), limits = a.next(), detail = a.next(), problem = a.next();
        int64_t f = function_id(db, module, addr, name, Field::none());
        Row old;
        int64_t cid;
        if (db.one("SELECT id, sig FROM candidate WHERE function=? AND engine=?", {Field::integer(f), engine}, old)) {
          cid = old[0].i;
          bool same = (old[1].null() && sig.null()) ||
                      (old[1].type == sig.type && old[1].s == sig.s && old[1].i == sig.i && old[1].d == sig.d);
          if (!same) db.run("DELETE FROM verdict WHERE candidate=? AND profile<>?", {Field::integer(cid), profile});
          db.run("UPDATE candidate SET rev=?, sig=?, name=?, source=?, problem=?, ts=? WHERE id=?",
                 {rev, sig, name, source, problem, Field::real(t), Field::integer(cid)});
        } else {
          db.run("INSERT INTO candidate(function, engine, rev, sig, name, source, problem, ts) VALUES(?,?,?,?,?,?,?,?)",
                 {Field::integer(f), engine, rev, sig, name, source, problem, Field::real(t)});
          cid = db.last_rowid();
        }
        db.run("INSERT OR REPLACE INTO verdict(candidate, profile, sig, verdict, reason, limits, detail, ts, carried) "
               "VALUES(?,?,?,?,?,?,?,?,0)",
               {Field::integer(cid), profile, sig, fverdict(verdict), or_empty(reason), or_empty(limits),
                or_empty(detail), Field::real(t)});
      }
      tx.commit();
    }
    summarize(module);
    return rows_out(out, 0, {});
  }
  if (M == "put_text") {
    std::string module = a.str();
    Field engine = a.next(), kind = a.next(), body = a.next();
    Conn& db = writable(module);
    Tx tx(db);
    db.run("INSERT OR REPLACE INTO text(module, engine, kind, body, ts) VALUES(?,?,?,?,?)",
           {T(module), engine, kind, body, Field::real(now())});
    tx.commit();
    return rows_out(out, 0, {});
  }
  if (M == "get_text") {
    std::string module = a.str();
    Field engine = a.next(), kind = a.next();
    Row r;
    bool got = mone(module, "SELECT body FROM text WHERE module=? AND engine=? AND kind=?", {T(module), engine, kind}, r);
    return rows_out(out, 1, {{got ? r[0] : T("")}});
  }
  if (M == "candidates" || M == "judged") {
    std::string module = a.str();
    Field profile = a.next(), engine = opt(a);
    auto rows = candidates(module, profile, engine);
    if (M == "judged") {
      std::vector<Row> keep;
      for (auto& r : rows) {
        bool same = (r[11].null() && r[3].null()) || (r[11].type == r[3].type && r[11].s == r[3].s && r[11].i == r[3].i);
        if (!r[7].null() && same) keep.push_back(r);
      }
      rows = keep;
    }
    return rows_out(out, 12, rows);
  }
  if (M == "stored") {
    std::string module = a.str();
    Field addr = a.next();
    std::vector<Row> res;
    int64_t idx = 0;
    for (auto& c : mrows(module, "SELECT c.id, c.engine, c.rev, c.name, c.source, c.problem FROM candidate c "
                                 "JOIN function f ON f.id=c.function WHERE f.module=? AND f.addr=?", {T(module), addr})) {
      Row head = {Field::integer(idx), c[1], c[2], c[3], or_empty(c[4]), or_empty(c[5])};
      auto vs = mrows(module, "SELECT profile, verdict, reason, limits, carried FROM verdict "
                              "WHERE candidate=? ORDER BY ts DESC", {c[0]});
      if (vs.empty()) {
        Row r = head;
        for (int k = 0; k < 6; k++) r.push_back(Field::none());
        r[6] = Field::integer(0);
        res.push_back(r);
      }
      for (auto& v : vs) {
        Row r = head;
        r.push_back(Field::integer(1));
        r.push_back(v[0]);
        r.push_back(fverdict(v[1]));
        r.push_back(or_empty(v[2]));
        r.push_back(or_empty(v[3]));
        r.push_back(Field::integer(truthy(v[4]) ? 1 : 0));
        res.push_back(r);
      }
      idx++;
    }
    return rows_out(out, 12, res);
  }
  if (M == "built") {
    std::string module = a.str();
    Field profile = a.next();
    return rows_out(out, 2, mrows(module, "SELECT addr, sig FROM built WHERE module=? AND profile=?", {T(module), profile}));
  }
  if (M == "set_built") {
    std::string module = a.str();
    Field profile = a.next();
    Conn& db = writable(module);
    Tx tx(db);
    while (!a.done()) {
      Field addr = a.next(), sig = a.next();
      db.run("INSERT OR REPLACE INTO built(module, addr, profile, sig) VALUES(?,?,?,?)", {T(module), addr, profile, sig});
    }
    tx.commit();
    return rows_out(out, 0, {});
  }
  if (M == "best") {
    std::string module = a.str();
    Field profile = a.next();
    auto prefer = a.strings();
    std::vector<Row> rows;
    for (auto& kv : best(module, profile, prefer)) rows.push_back(kv.second);
    return rows_out(out, 14, rows);
  }
  if (M == "latest") {
    std::string module = a.str();
    std::vector<Row> rows;
    for (auto& r : mrows(module, latest_sql(), {T(module)})) rows.push_back({r[0], T(fclassify(r[1], r[2]).first)});
    return rows_out(out, 2, rows);
  }
  if (M == "addresses") {
    std::string module = a.str();
    std::vector<Row> rows;
    for (auto& s : addresses(module)) rows.push_back({T(s)});
    return rows_out(out, 1, rows);
  }
  if (M == "tally" || M == "scores") {
    Field profile = a.next();
    std::string module = a.str();
    auto prefer = a.strings();
    std::map<std::string, std::string> cls;
    for (auto& kv : best(module, profile, prefer)) cls[kv.first] = kv.second[12].s;
    const std::vector<std::string>& keys = M == "tally" ? V::ORDER : V::SCORED;
    std::map<std::string, int64_t> n;
    std::map<std::string, std::pair<Field, Field>> known;
    if (M == "scores") known = identified(module);
    for (auto& addr : addresses(module)) {
      auto it = cls.find(addr);
      std::string c = it != cls.end() ? it->second : std::string(V::NO_SOURCE);
      if (M == "scores" && known.count(addr) && c != V::VERIFIED && c != V::WEAK) c = V::IDENTIFIED;
      n[c]++;
    }
    std::vector<Row> rows;
    for (auto& k : keys) rows.push_back({T(k), Field::integer(n[k])});
    for (auto& kv : n)                                  // (a class outside the keys: Python's dict gains it)
      if (std::find(keys.begin(), keys.end(), kv.first) == keys.end()) rows.push_back({T(kv.first), Field::integer(kv.second)});
    return rows_out(out, 2, rows);
  }
  if (M == "set_identified") {
    std::string module = a.str();
    double t = now();
    Conn& db = writable(module);
    {
      Tx tx(db);
      db.run("DELETE FROM identified WHERE module=?", {T(module)});
      while (!a.done()) {
        Field addr = a.next(), name = a.next(), lib = a.next();
        db.run("INSERT INTO identified(module, addr, name, library, ts) VALUES(?,?,?,?,?)", {T(module), addr, name, lib, Field::real(t)});
      }
      tx.commit();
    }
    summarize(module);
    return rows_out(out, 0, {});
  }
  if (M == "identified") {
    std::string module = a.str();
    return rows_out(out, 3, mrows(module, "SELECT addr, name, library FROM identified WHERE module=?", {T(module)}));
  }
  if (M == "record_variant") {
    std::string module = a.str();
    Field addr = a.next(), compiler = a.next(), source = a.next(), distance = a.next(), total = a.next(),
          verdict = a.next(), knobs = a.next(), parent = a.next(), behaves = a.next();
    Conn& db = writable(module);
    Tx tx(db);
    int64_t f = function_id(db, module, addr, Field::none(), Field::none());
    db.run("INSERT INTO variant(function, compiler, source, parent, knobs, distance, total, verdict, "
           "behaves, ts) VALUES(?,?,?,?,?,?,?,?,?,?) ON CONFLICT(function, compiler, knobs) DO UPDATE SET "
           "source=excluded.source, parent=excluded.parent, distance=excluded.distance, "
           "total=excluded.total, verdict=excluded.verdict, behaves=excluded.behaves, ts=excluded.ts",
           {Field::integer(f), compiler, source, parent, knobs, distance, total, fmatch(verdict), fverdict(behaves),
            Field::real(now())});
    Row r;
    db.one("SELECT id FROM variant WHERE function=? AND compiler=? AND knobs=?", {Field::integer(f), compiler, knobs}, r);
    tx.commit();
    return rows_out(out, 1, {{r[0]}});
  }
  if (M == "variants") {
    std::string module = a.str();
    return rows_out(out, 9, variants(module, opt(a)));
  }
  if (M == "best_variant") {
    std::string module = a.str();
    Field compiler = a.next();
    std::vector<Row> rows;
    for (auto& kv : best_variant(module, compiler)) rows.push_back(kv.second);
    return rows_out(out, 9, rows);
  }
  if (M == "match_tally") {
    std::string module = a.str();
    Field compiler = a.next();
    auto best = best_variant(module, compiler);
    int64_t ident = 0, same = 0;
    for (auto& kv : best) {
      if (kv.second[7].type == 3 && kv.second[7].s == V::IDENTICAL) ident++;
      if (kv.second[7].type == 3 && kv.second[7].s == V::SAME_SIZE) same++;
    }
    int64_t n = (int64_t)best.size();
    return rows_out(out, 4, {{Field::integer(n), Field::integer(ident), Field::integer(same), Field::integer(n - ident - same)}});
  }
  if (M == "summaries") {
    return rows_out(out, 8, index_->rows("SELECT module, functions, identified, judged, last, last_profile, profiles, latest FROM summary"));
  }
  if (M == "modules") {
    Field profile = opt(a);
    auto rows = index_->rows("SELECT sha, name FROM module ORDER BY name");
    if (profile.null()) return rows_out(out, 2, rows);
    std::set<std::string> judged;
    for (auto& r : index_->rows("SELECT module, profiles FROM summary")) {
      Json j = json_parse(r[1].null() || r[1].s.empty() ? "[]" : r[1].s);
      for (auto& p : j.a)
        if (p.t == Json::STR && p.s == profile.s) judged.insert(r[0].s);
    }
    std::vector<Row> keep;
    for (auto& r : rows)
      if (judged.count(r[0].s)) keep.push_back(r);
    return rows_out(out, 2, keep);
  }
  if (M == "module_rows") {
    Field kind = opt(a);
    if (kind.null()) return rows_out(out, 4, index_->rows("SELECT sha, name, path, kind FROM module ORDER BY name"));
    return rows_out(out, 4, index_->rows("SELECT sha, name, path, kind FROM module WHERE kind=? ORDER BY name", {kind}));
  }
  if (M == "module_path") {
    Field name = a.next();
    Row r;
    if (index_->one("SELECT path FROM module WHERE name=? ORDER BY ts DESC LIMIT 1", {name}, r)) return rows_out(out, 1, {r});
    return rows_out(out, 1, {});
  }
  if (M == "latest_profile") {
    Field kind = a.next();
    Row r;
    if (index_->one("SELECT s.last_profile FROM summary s JOIN module m ON m.sha=s.module "
                    "WHERE m.kind=? AND s.last IS NOT NULL ORDER BY s.last DESC LIMIT 1", {kind}, r))
      return rows_out(out, 1, {r});
    return rows_out(out, 1, {});
  }
  if (M == "recent") {
    Field limit = a.next();
    return rows_out(out, 4, index_->rows("SELECT m.name, m.kind, s.judged, s.last FROM summary s JOIN module m ON m.sha=s.module "
                                         "WHERE s.last IS NOT NULL ORDER BY s.last DESC LIMIT ?", {limit}));
  }
  if (M == "verdicts_since") {
    Field t = a.next();
    int64_t n = 0;
    for (auto& r : index_->rows("SELECT module FROM summary WHERE last > ?", {t})) {
      Row c;
      if (mone(r[0].s, "SELECT COUNT(*) FROM verdict WHERE ts > ?", {t}, c)) n += c[0].i;
    }
    return rows_out(out, 1, {{Field::integer(n)}});
  }
  if (M == "overview") {
    std::map<std::string, Row> sums;
    for (auto& r : index_->rows("SELECT module, functions, identified, latest FROM summary")) sums[r[0].s] = r;
    std::vector<Row> rows;
    for (auto& mr : index_->rows("SELECT sha, name, path, kind FROM module ORDER BY name")) {
      auto it = sums.find(mr[0].s);
      std::vector<std::pair<std::string, int64_t>> latest;
      for (auto& k : V::ORDER) latest.emplace_back(k, 0);
      int64_t n = 0, ident = 0;
      if (it != sums.end()) {
        n = truthy(it->second[1]) ? it->second[1].i : 0;
        ident = truthy(it->second[2]) ? it->second[2].i : 0;
        Json j = json_parse(it->second[3].null() || it->second[3].s.empty() ? "{}" : it->second[3].s);
        for (auto& kv : j.o) {
          int64_t v = kv.second.t == Json::NUM ? strtoll(kv.second.raw.c_str(), nullptr, 10) : 0;
          bool found = false;
          for (auto& l : latest)
            if (l.first == kv.first) { l.second = v; found = true; }
          if (!found) latest.emplace_back(kv.first, v);
        }
      }
      int64_t sum = 0;
      for (auto& l : latest) sum += l.second;
      for (auto& l : latest)
        if (l.first == V::NO_SOURCE) l.second += std::max<int64_t>(0, n - sum);   // (functions with no verdict at all)
      std::string lj = "{";
      for (size_t i = 0; i < latest.size(); i++)
        lj += (i ? ", " : "") + json_str(latest[i].first) + ": " + std::to_string(latest[i].second);
      lj += "}";
      rows.push_back({mr[0], mr[1], mr[3], mr[2], Field::integer(n), Field::integer(ident), T(lj)});
    }
    std::stable_sort(rows.begin(), rows.end(), [](const Row& x, const Row& y) {
      return (x[1].type == 3 ? x[1].s : std::string()) < (y[1].type == 3 ? y[1].s : std::string());
    });
    return rows_out(out, 7, rows);
  }
  if (M == "carry") return carry(out);
  if (M == "open_module") {                            // (its database opened and its tables made: _m)
    m(a.str());
    return rows_out(out, 0, {});
  }
  if (M == "about") {                                  // _about(name)
    bool has;
    std::string v = about(a.str(), &has);
    return rows_out(out, 1, has ? std::vector<Row>{{T(v)}} : std::vector<Row>{});
  }
  if (M == "set_about") {
    std::string name = a.str(), value = a.str();
    set_about(name, value);
    return rows_out(out, 0, {});
  }
  raise(ERR_LOOKUP, "the function store has no method " + M);
}

}  // namespace cbe
