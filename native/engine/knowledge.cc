// knowledge.cc -- the knowledge base's database (knowledge/store.py) and reads.py's digests.
#include "knowledge.h"

#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

namespace cbe {

static const char* SCHEMA =
    "CREATE TABLE IF NOT EXISTS knowledge(\n"
    "    platform TEXT NOT NULL, kind TEXT NOT NULL, key TEXT NOT NULL, seq INTEGER,\n"
    "    hash TEXT NOT NULL, tag TEXT NOT NULL, value BLOB, source TEXT, origin TEXT NOT NULL, made REAL,\n"
    "    PRIMARY KEY (platform, kind, key));\n"
    "CREATE TABLE IF NOT EXISTS loaded(\n"
    "    platform TEXT NOT NULL, kind TEXT NOT NULL, stamp TEXT, source TEXT, made REAL,\n"
    "    PRIMARY KEY (platform, kind));\n"
    "CREATE TABLE IF NOT EXISTS module(sha TEXT PRIMARY KEY, name TEXT, platform TEXT);\n"
    "CREATE TABLE IF NOT EXISTS body(\n"
    "    module TEXT NOT NULL, addr TEXT NOT NULL, hash TEXT NOT NULL, name TEXT, source TEXT,\n"
    "    PRIMARY KEY (module, addr));\n"
    "CREATE INDEX IF NOT EXISTS body_hash ON body(hash);\n"
    "CREATE TABLE IF NOT EXISTS body_name(hash TEXT PRIMARY KEY, name TEXT, source TEXT, ts REAL);\n"
    "CREATE TABLE IF NOT EXISTS carried(what TEXT PRIMARY KEY, mark TEXT);\n";

static const char* ABSENT = "absent";

static bool truthy_platform(const Field& f) { return !f.null() && !(f.type == 3 && f.s.empty()); }
static Field T(const std::string& s) { return Field::text(s); }

KnowledgeBase::KnowledgeBase(const std::string& p) : path(p) {
  std::error_code ec;
  fs::create_directories(fs::path(widen(p)).parent_path(), ec);
  conn_.reset(new Conn(p, false, 60000));
  conn_->exec("PRAGMA journal_mode=WAL");
  conn_->exec("PRAGMA synchronous=NORMAL");
  conn_->exec(SCHEMA);
}

void KnowledgeBase::close() {
  std::lock_guard<std::recursive_mutex> g(mu_);
  conn_.reset();
}

std::map<std::string, std::string>& KnowledgeBase::hashes(const std::string& platform, const std::string& kind) {
  std::lock_guard<std::recursive_mutex> g(mu_);
  Row r;
  conn_->one("PRAGMA data_version", {}, r);
  if (r[0].i != version_) {                            // (another process wrote the file since)
    hashes_.clear();
    version_ = r[0].i;
  }
  auto k = std::make_pair(platform, kind);
  auto it = hashes_.find(k);
  if (it != hashes_.end()) return it->second;
  auto& h = hashes_[k];
  for (auto& row : conn_->rows("SELECT key, hash FROM knowledge WHERE platform=? AND kind=?", {T(platform), T(kind)}))
    h[row[0].s] = row[1].s;
  return h;
}

bool KnowledgeBase::kind_digest(const std::string& platform, const std::string& kind, std::string& out) {
  std::lock_guard<std::recursive_mutex> g(mu_);
  auto& h = hashes(platform, kind);
  if (h.empty()) return false;
  std::string text;
  std::vector<std::string> keys;
  for (auto& kv : h) keys.push_back(kv.first);
  std::sort(keys.begin(), keys.end());                  // (code point order: sorted() of str)
  // d.update(repr((k, h[k])) ...) for each, in order: one SHA-1 over the concatenation
  for (auto& k : keys) text += "(" + py_repr(k) + ", " + py_repr(h[k]) + ")";
  out = sha1_hex(text);
  return true;
}

static std::string repr_field(const Field& f) {
  if (f.null()) return "None";
  if (f.type == 1) return std::to_string(f.i);
  return py_repr(f.s);
}

bool KnowledgeBase::name(const std::string& body, Row& out) {
  std::lock_guard<std::recursive_mutex> g(mu_);
  Row r;
  if (conn_->one("SELECT name, source FROM body_name WHERE hash=?", {T(body)}, r)) {
    out = r;
    return true;
  }
  std::vector<std::pair<Field, std::string>> names;
  for (auto& row : conn_->rows("SELECT DISTINCT b.name, m.name FROM body b LEFT JOIN module m ON m.sha=b.module "
                               "WHERE b.hash=? AND b.name IS NOT NULL", {T(body)})) {
    bool have = false;
    for (auto& n : names)
      if (n.first.type == row[0].type && n.first.s == row[0].s) have = true;
    if (!have) names.emplace_back(row[0], (row[1].null() || row[1].s.empty()) ? "another module" : row[1].s);
  }
  if (names.size() == 1) {
    out = {names[0].first, T("the same body in " + names[0].second)};
    return true;
  }
  return false;
}

std::vector<std::pair<std::string, std::pair<Field, Field>>> KnowledgeBase::names_from_bodies(const std::string& module) {
  std::lock_guard<std::recursive_mutex> g(mu_);
  auto given = conn_->rows("SELECT b.addr, b.hash, n.name, n.source FROM body b JOIN body_name n ON n.hash=b.hash "
                           "WHERE b.module=?", {T(module)});
  auto others = conn_->rows("SELECT b.addr, o.name, m.name FROM body b JOIN body o ON o.hash=b.hash AND o.module<>b.module "
                            "AND o.name IS NOT NULL LEFT JOIN module m ON m.sha=o.module WHERE b.module=? AND b.name IS NULL",
                            {T(module)});
  std::vector<std::pair<std::string, std::pair<Field, Field>>> out;
  std::map<std::string, size_t> at;
  auto put = [&](const std::string& addr, const Field& n, const Field& s) {
    auto it = at.find(addr);
    if (it != at.end()) out[it->second].second = {n, s};
    else {
      at[addr] = out.size();
      out.push_back({addr, {n, s}});
    }
  };
  for (auto& r : given) put(r[0].s, r[2], r[3]);
  std::vector<std::string> order;
  std::map<std::string, std::vector<std::pair<std::string, std::string>>> seen;
  for (auto& r : others) {
    auto& v = seen[r[0].s];
    if (v.empty() && std::find(order.begin(), order.end(), r[0].s) == order.end()) order.push_back(r[0].s);
    bool have = false;
    for (auto& e : v)
      if (e.first == r[1].s) have = true;
    if (!have) v.emplace_back(r[1].s, (r[2].null() || r[2].s.empty()) ? "another module" : r[2].s);
  }
  for (auto& addr : order) {
    auto& v = seen[addr];
    if (!at.count(addr) && v.size() == 1) put(addr, T(v[0].first), T("the same body in " + v[0].second));
  }
  return out;
}

std::string KnowledgeBase::digest(const std::string& read) {
  std::lock_guard<std::recursive_mutex> lock(mu_);
  size_t a = read.find(':');
  size_t b = a == std::string::npos ? a : read.find(':', a + 1);
  if (b == std::string::npos) raise(ERR_VALUE, "not enough values to unpack (expected 3, got " +
                                                   std::to_string(a == std::string::npos ? 1 : 2) + ")");
  std::string platform = read.substr(0, a), kind = read.substr(a + 1, b - a - 1), key = read.substr(b + 1);
  if (platform == "bodies") {
    std::string text;
    if (kind == "name") {
      Row r;
      if (!name(key, r)) return ABSENT;
      text = "(" + repr_field(r[0]) + ", " + repr_field(r[1]) + ")";
    } else if (kind == "names") {
      auto got = names_from_bodies(key);
      if (got.empty()) return ABSENT;
      // sorted(dict.items()): by address (each address once)
      std::sort(got.begin(), got.end(), [](const auto& x, const auto& y) { return x.first < y.first; });
      text = "[";
      for (size_t i = 0; i < got.size(); i++) {
        if (i) text += ", ";
        text += "(" + py_repr(got[i].first) + ", (" + repr_field(got[i].second.first) + ", " +
                repr_field(got[i].second.second) + "))";
      }
      text += "]";
    } else {
      return ABSENT;
    }
    return sha1_hex(text);
  }
  if (key == "*") {
    std::string d;
    return kind_digest(platform, kind, d) ? d : std::string(ABSENT);
  }
  auto& h = hashes(platform, kind);
  auto it = h.find(key);
  return it == h.end() ? std::string(ABSENT) : it->second;
}

// ---- one method, by name ------------------------------------------------------------------------
static void rows_out(Writer& out, int ncols, const std::vector<Row>& rows) {
  out.integer(ncols);
  write_rows(out, rows);
}

void KnowledgeBase::call(const std::string& M, Reader& a, Writer& out) {
  std::lock_guard<std::recursive_mutex> g(mu_);
  if (!conn_) raise(ERR_SQLITE, "Cannot operate on a closed database.");
  Conn& c = *conn_;
  if (M == "get") {
    Field p = a.next(), k = a.next(), key = a.next();
    Row r;
    if (c.one("SELECT tag, value FROM knowledge WHERE platform=? AND kind=? AND key=?", {p, k, key}, r))
      return rows_out(out, 2, {r});
    return rows_out(out, 2, {});
  }
  if (M == "has") {
    Field p = a.next(), k = a.next(), key = a.next();
    Row r;
    bool got = c.one("SELECT 1 FROM knowledge WHERE platform=? AND kind=? AND key=?", {p, k, key}, r);
    return rows_out(out, 1, {{Field::integer(got ? 1 : 0)}});
  }
  if (M == "put_many" || M == "replace_kind") {
    std::string platform = a.str(), kind = a.str();
    Field stamp = M == "replace_kind" ? a.next() : Field::none();
    Field source = a.next(), origin = a.next();
    double t = now();
    std::vector<std::pair<std::string, std::string>> put;
    {
      Tx tx(c);
      if (M == "replace_kind")
        c.run("DELETE FROM knowledge WHERE platform=? AND kind=? AND origin IN (?, ?)",
              {T(platform), T(kind), T("authored"), T("derived")});
      int64_t seq = 0;
      while (!a.done()) {
        Field key = a.next(), tag = a.next(), data = a.next();
        std::string h = sha1_hex2(tag.s, data.s.data(), data.s.size());
        c.run("INSERT OR REPLACE INTO knowledge(platform, kind, key, seq, hash, tag, value, source, origin, made) "
              "VALUES(?,?,?,?,?,?,?,?,?,?)",
              {T(platform), T(kind), key, M == "replace_kind" ? Field::integer(seq) : Field::none(), T(h), tag,
               Field::bytes(data.s), source, origin, Field::real(t)});
        put.emplace_back(key.s, h);
        seq++;
      }
      if (M == "replace_kind")
        c.run("INSERT OR REPLACE INTO loaded(platform, kind, stamp, source, made) VALUES(?,?,?,?,?)",
              {T(platform), T(kind), stamp, source, Field::real(t)});
      tx.commit();
    }
    auto k = std::make_pair(platform, kind);
    if (M == "replace_kind") hashes_.erase(k);
    else {
      auto it = hashes_.find(k);
      if (it != hashes_.end())
        for (auto& kv : put) it->second[kv.first] = kv.second;
    }
    return rows_out(out, 0, {});
  }
  if (M == "forget") {
    std::string platform = a.str(), kind = a.str(), key = a.str();
    Tx tx(c);
    c.run("DELETE FROM knowledge WHERE platform=? AND kind=? AND key=?", {T(platform), T(kind), T(key)});
    tx.commit();
    auto it = hashes_.find(std::make_pair(platform, kind));
    if (it != hashes_.end()) it->second.erase(key);
    return rows_out(out, 0, {});
  }
  if (M == "about") {
    Field p = a.next(), k = a.next(), key = a.next();
    Row r;
    if (c.one("SELECT source, origin, made, hash FROM knowledge WHERE platform=? AND kind=? AND key=?", {p, k, key}, r))
      return rows_out(out, 4, {r});
    return rows_out(out, 4, {});
  }
  if (M == "table") {
    Field p = a.next(), k = a.next();
    return rows_out(out, 3, c.rows("SELECT key, tag, value FROM knowledge WHERE platform=? AND kind=? "
                                   "ORDER BY seq IS NULL, seq, rowid", {p, k}));
  }
  if (M == "hashes") {
    std::string platform = a.str(), kind = a.str();
    std::vector<Row> rows;
    for (auto& kv : hashes(platform, kind)) rows.push_back({T(kv.first), T(kv.second)});
    return rows_out(out, 2, rows);
  }
  if (M == "hash_of") {
    std::string platform = a.str(), kind = a.str(), key = a.str();
    auto& h = hashes(platform, kind);
    auto it = h.find(key);
    if (it == h.end()) return rows_out(out, 1, {});
    return rows_out(out, 1, {{T(it->second)}});
  }
  if (M == "kind_digest") {
    std::string platform = a.str(), kind = a.str(), d;
    if (!kind_digest(platform, kind, d)) return rows_out(out, 1, {});
    return rows_out(out, 1, {{T(d)}});
  }
  if (M == "digest") return rows_out(out, 1, {{T(digest(a.str()))}});
  if (M == "stamp") {
    Field p = a.next(), k = a.next();
    Row r;
    if (c.one("SELECT stamp FROM loaded WHERE platform=? AND kind=?", {p, k}, r)) return rows_out(out, 1, {r});
    return rows_out(out, 1, {});
  }
  if (M == "kinds") {
    Field platform = a.done() ? Field::none() : a.next();
    bool by = truthy_platform(platform);
    auto rows = c.rows(std::string("SELECT platform, kind, origin, COUNT(*), SUM(LENGTH(value)) FROM knowledge ") +
                           (by ? "WHERE platform=? " : "") + "GROUP BY platform, kind, origin ORDER BY platform, kind",
                       by ? std::vector<Field>{platform} : std::vector<Field>{});
    std::map<std::pair<std::string, std::string>, Field> loaded;
    for (auto& r : c.rows("SELECT platform, kind, source FROM loaded")) loaded[{r[0].s, r[1].s}] = r[2];
    struct E { Field p, k; int64_t n = 0, size = 0; std::vector<std::pair<std::string, int64_t>> origins; Field src; };
    std::vector<E> es;
    std::map<std::pair<std::string, std::string>, size_t> at;
    for (auto& r : rows) {
      auto key = std::make_pair(r[0].s, r[1].s);
      auto it = at.find(key);
      if (it == at.end()) {
        E e;
        e.p = r[0];
        e.k = r[1];
        auto l = loaded.find(key);
        e.src = l == loaded.end() ? T("") : l->second;
        at[key] = es.size();
        es.push_back(e);
        it = at.find(key);
      }
      E& e = es[it->second];
      e.n += r[3].i;
      e.size += r[4].null() ? 0 : r[4].i;
      e.origins.emplace_back(r[2].s, r[3].i);
    }
    std::vector<Row> res;
    for (auto& e : es) {
      std::string oj = "{";
      for (size_t i = 0; i < e.origins.size(); i++)
        oj += (i ? ", " : "") + json_str(e.origins[i].first) + ": " + std::to_string(e.origins[i].second);
      oj += "}";
      res.push_back({e.p, e.k, Field::integer(e.n), Field::integer(e.size), T(oj), e.src});
    }
    return rows_out(out, 6, res);
  }
  if (M == "carried") {
    std::string what = a.str();
    if (!a.done()) {
      Field mark = a.next();
      Tx tx(c);
      c.run("INSERT OR REPLACE INTO carried(what, mark) VALUES(?,?)", {T(what), mark});
      tx.commit();
      return rows_out(out, 1, {{mark}});
    }
    Row r;
    if (c.one("SELECT mark FROM carried WHERE what=?", {T(what)}, r)) return rows_out(out, 1, {r});
    return rows_out(out, 1, {});
  }
  if (M == "add_module") {
    Field sha = a.next(), name = a.next(), platform = a.next();
    Tx tx(c);
    c.run("INSERT OR REPLACE INTO module(sha, name, platform) VALUES(?,?,?)", {sha, name, platform});
    tx.commit();
    return rows_out(out, 0, {});
  }
  if (M == "set_bodies") {
    std::string module = a.str();
    Tx tx(c);
    c.run("DELETE FROM body WHERE module=?", {T(module)});
    while (!a.done()) {
      Field addr = a.next(), h = a.next(), n = a.next(), s = a.next();
      c.run("INSERT INTO body(module, addr, hash, name, source) VALUES(?,?,?,?,?)", {T(module), addr, h, n, s});
    }
    tx.commit();
    return rows_out(out, 0, {});
  }
  if (M == "bodies") {
    return rows_out(out, 4, c.rows("SELECT addr, hash, name, source FROM body WHERE module=?", {a.next()}));
  }
  if (M == "name_body") {
    Field h = a.next(), n = a.next(), s = a.next();
    Tx tx(c);
    c.run("INSERT OR REPLACE INTO body_name(hash, name, source, ts) VALUES(?,?,?,?)", {h, n, s, Field::real(now())});
    tx.commit();
    return rows_out(out, 0, {});
  }
  if (M == "name") {
    Row r;
    if (name(a.str(), r)) return rows_out(out, 2, {r});
    return rows_out(out, 2, {});
  }
  if (M == "names_from_bodies") {
    std::vector<Row> rows;
    for (auto& kv : names_from_bodies(a.str())) rows.push_back({T(kv.first), kv.second.first, kv.second.second});
    return rows_out(out, 3, rows);
  }
  if (M == "shared_bodies") {
    Row x, y;
    c.one("SELECT COUNT(*), COUNT(DISTINCT hash) FROM body", {}, x);
    c.one("SELECT COUNT(*), COALESCE(SUM(n), 0) FROM (SELECT COUNT(*) n FROM body GROUP BY hash "
          "HAVING COUNT(DISTINCT module) > 1)", {}, y);
    return rows_out(out, 4, {{x[0], x[1], y[0], y[1]}});
  }
  if (M == "modules_with_body") {
    Row r;
    c.one("SELECT COUNT(DISTINCT module) FROM body WHERE hash=?", {a.next()}, r);
    return rows_out(out, 1, {r});
  }
  if (M == "carry_bodies") {
    // bodies and body names an older function store holds, copied in once (and again when names
    // were added there since): the store's index file, opened to read
    std::string store = a.str(), label = a.str();
    std::vector<Row> names, rows, modules;
    Row cnt, nb;
    try {
      Conn s(store, true, 60000);
      s.one("SELECT COUNT(*), COALESCE(MAX(ts), 0) FROM body_name", {}, cnt);
      s.one("SELECT COUNT(*) FROM body", {}, nb);
      std::string mark = std::to_string(cnt[0].i) + ":" +
                         (cnt[1].type == 2 ? py_float(cnt[1].d) : std::to_string(cnt[1].i)) + ":" + std::to_string(nb[0].i);
      Row have;
      if (c.one("SELECT mark FROM carried WHERE what=?", {T(label)}, have) && have[0].s == mark) return rows_out(out, 0, {});
      names = s.rows("SELECT hash, name, source, ts FROM body_name");
      rows = s.rows("SELECT module, addr, hash, name, source FROM body");
      modules = s.rows("SELECT sha, name, kind FROM module WHERE sha IN (SELECT DISTINCT module FROM body)");
      Tx tx(c);
      for (auto& r : names) c.run("INSERT OR IGNORE INTO body_name(hash, name, source, ts) VALUES(?,?,?,?)", r);
      for (auto& r : rows) c.run("INSERT OR IGNORE INTO body(module, addr, hash, name, source) VALUES(?,?,?,?,?)", r);
      for (auto& r : modules) c.run("INSERT OR IGNORE INTO module(sha, name, platform) VALUES(?,?,?)", r);
      c.run("INSERT OR REPLACE INTO carried(what, mark) VALUES(?,?)", {T(label), T(mark)});
      tx.commit();
    } catch (const Error& e) {
      if (e.kind != ERR_SQLITE) throw;               // (no such tables there: nothing to carry)
    }
    return rows_out(out, 0, {});
  }
  raise(ERR_LOOKUP, "the knowledge base has no method " + M);
}

static std::mutex g_kb_mu;
static std::map<std::string, std::shared_ptr<KnowledgeBase>> g_kb;

std::shared_ptr<KnowledgeBase> open_kb(const std::string& path) {
  std::lock_guard<std::mutex> g(g_kb_mu);
  auto it = g_kb.find(path);
  if (it != g_kb.end()) return it->second;
  auto kb = std::make_shared<KnowledgeBase>(path);
  g_kb[path] = kb;
  return kb;
}

void close_kb(const std::string& path) {
  std::shared_ptr<KnowledgeBase> kb;
  {
    std::lock_guard<std::mutex> g(g_kb_mu);
    auto it = g_kb.find(path);
    if (it == g_kb.end()) return;
    kb = it->second;
    g_kb.erase(it);
  }
  kb->close();
}

}  // namespace cbe
