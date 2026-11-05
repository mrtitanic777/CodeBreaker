// testkit -- what tests/run_all.py asks of native code.
//
//   testkit plan   --root R --changed FILE --suites FILE [--deps FILE]
//       Which suites does a change reach?  FILE of --changed: lines "M path" / "D path".  --suites: lines
//       "name<TAB>repo-relative path of the suite file<TAB>flag"; flag "py" = reads every .py file (tidy).
//       --deps: lines "name<TAB>entry...", what the last good run of the suite loaded (paths or module names).
//       Prints "ALL <why>" (the map cannot place the change) or one "RUN name" per suite (none: nothing).
//   testkit lookup --cache D [--cwd C] -- argv...      the compile cache: "HIT" (outputs put in place, exit 0),
//       "MISS <key>" (exit 1: run it, and if it printed nothing and succeeded, `store` it) or "SKIP <why>" (exit 2).
//   testkit store  --cache D [--cwd C] --key K -- argv...
//   testkit prune  --cache D --mb N                    least recently used entries go first
//
// The dependency map is the import graph tools/check_imports.py walks (every import statement, anywhere in a
// file, relative ones too; the module names written as strings count as a wider reading used for a suite
// that has no recorded run), plus the data files a suite reads.  The cache keys a g++ -c twice: exactly, on
// the compiler's bytes, the flags and the output of `g++ -E -P` (so every header, system ones included, is in
// the key), and cheaply, on the compiler, flags and source bytes plus a manifest of the files the preprocessor
// read (checked by content for project files, size and time for system headers) that saves the preprocess on
// the next run; a link on the bytes of everything it links; javac on the JDK's bytes, the flags, every source,
// and every folder and jar on its class and source paths.  Only runs that succeed are stored; a command the key cannot cover
// (response file, -g, -M*, a default class path) is SKIPped, never cached.
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
#include <chrono>
static void lap(const char* what) {                      // TESTKIT_TIMING=1: seconds since the last lap, on stderr
  static auto last = std::chrono::steady_clock::now();
  if (!getenv("TESTKIT_TIMING")) return;
  auto now = std::chrono::steady_clock::now();
  fprintf(stderr, "testkit: %-18s %.3fs\n", what, std::chrono::duration<double>(now - last).count());
  last = now;
}
using std::string;
using std::vector;
typedef std::set<string> Set;

// ------------------------------------------------------------------------------------------------ utilities
static string slurp(const fs::path& p) {
  std::ifstream f(p, std::ios::binary);
  std::ostringstream o;
  o << f.rdbuf();
  return o.str();
}
static vector<string> lines(const string& text) {
  vector<string> out;
  std::istringstream in(text);
  string l;
  while (std::getline(in, l)) {
    if (!l.empty() && l.back() == '\r') l.pop_back();
    if (!l.empty()) out.push_back(l);
  }
  return out;
}
static vector<string> split(const string& s, char c) {
  vector<string> out;
  string cur;
  for (char ch : s) {
    if (ch == c) { out.push_back(cur); cur.clear(); } else cur += ch;
  }
  out.push_back(cur);
  return out;
}
static string slash(string s) {
  std::replace(s.begin(), s.end(), '\\', '/');
  return s;
}
static bool endsWith(const string& s, const string& e) { return s.size() >= e.size() && s.compare(s.size() - e.size(), e.size(), e) == 0; }
static bool startsWith(const string& s, const string& e) { return s.compare(0, e.size(), e) == 0; }
static bool isWordChar(char c) { return isalnum((unsigned char)c) || c == '_'; }
static bool hasWord(const string& s, const string& w) {
  for (size_t at = s.find(w); at != string::npos; at = s.find(w, at + 1)) {
    bool l = at == 0 || !isWordChar(s[at - 1]);
    bool r = at + w.size() >= s.size() || !isWordChar(s[at + w.size()]);
    if (l && r) return true;
  }
  return false;
}

// ------------------------------------------------------------------------------------------------ sha1
struct Sha1 {
  uint32_t h[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
  uint8_t buf[64];
  size_t n = 0;
  uint64_t total = 0;
  static uint32_t rol(uint32_t x, int k) { return (x << k) | (x >> (32 - k)); }
  void block(const uint8_t* p) {
    uint32_t w[80];
    for (int i = 0; i < 16; i++) w[i] = (uint32_t)p[4 * i] << 24 | (uint32_t)p[4 * i + 1] << 16 | (uint32_t)p[4 * i + 2] << 8 | p[4 * i + 3];
    for (int i = 16; i < 80; i++) w[i] = rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
    for (int i = 0; i < 80; i++) {
      uint32_t f, k;
      if (i < 20) { f = (b & c) | (~b & d); k = 0x5A827999; }
      else if (i < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }
      else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }
      else { f = b ^ c ^ d; k = 0xCA62C1D6; }
      uint32_t t = rol(a, 5) + f + e + k + w[i];
      e = d; d = c; c = rol(b, 30); b = a; a = t;
    }
    h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e;
  }
  void add(const void* data, size_t len) {
    const uint8_t* p = (const uint8_t*)data;
    total += len;
    while (len) {
      size_t take = std::min(len, 64 - n);
      memcpy(buf + n, p, take);
      n += take; p += take; len -= take;
      if (n == 64) { block(buf); n = 0; }
    }
  }
  string hex() {
    uint64_t bits = total * 8;
    uint8_t pad = 0x80;
    add(&pad, 1);
    uint8_t z = 0;
    while (n != 56) add(&z, 1);
    uint8_t len[8];
    for (int i = 0; i < 8; i++) len[i] = (uint8_t)(bits >> (56 - 8 * i));
    add(len, 8);
    char out[41];
    for (int i = 0; i < 5; i++) snprintf(out + 8 * i, 9, "%08x", h[i]);
    return out;
  }
};
// a key is a sequence of length-prefixed parts
struct Key {
  Sha1 s;
  void add(const string& part) {
    uint64_t n = part.size();
    s.add(&n, 8);
    s.add(part.data(), part.size());
  }
  string done() { return s.hex(); }
};
static string fileSha(const fs::path& p, const string& cache = "") {
  std::error_code ec;
  string memo;
  if (!cache.empty()) {
    auto size = fs::file_size(p, ec);
    if (!ec && size > (8u << 20)) {
      Key k;
      k.add(fs::absolute(p).string());
      k.add(std::to_string(size));
      k.add(std::to_string(fs::last_write_time(p, ec).time_since_epoch().count()));
      memo = cache + "/filehash/" + k.done();
      if (fs::exists(memo)) return slurp(memo);
    }
  }
  std::ifstream f(p, std::ios::binary);
  Sha1 s;
  char b[1 << 16];
  while (f) { f.read(b, sizeof b); s.add(b, (size_t)f.gcount()); }
  string h = s.hex();
  if (!memo.empty()) {
    fs::create_directories(fs::path(memo).parent_path(), ec);
    std::ofstream(memo, std::ios::binary) << h;
  }
  return h;
}

// ------------------------------------------------------------------------------------------------ python scan
struct Tok { int kind; string s; bool first; };           // kind: 0 name, 1 string, 2 op, 3 end of statement
struct Import { int level; string module; vector<string> names; };
struct Facts { vector<Import> imports; Set strings; bool computed = false; };

static vector<Tok> tokenize(const string& t) {
  vector<Tok> out;
  size_t i = 0, n = t.size();
  int depth = 0;
  bool first = true;
  auto push = [&](int kind, string s) { out.push_back({kind, std::move(s), first}); first = false; };
  while (i < n) {
    char c = t[i];
    if (c == '\n') {
      if (depth <= 0) { if (!out.empty() && out.back().kind != 3) { out.push_back({3, "", false}); } first = true; depth = 0; }
      i++;
    } else if (c == ' ' || c == '\t' || c == '\r' || c == '\f') i++;
    else if (c == '\\' && i + 1 < n && (t[i + 1] == '\n' || t[i + 1] == '\r')) { i += 2; if (i < n && t[i - 1] == '\r' && t[i] == '\n') i++; }
    else if (c == '#') { while (i < n && t[i] != '\n') i++; }
    else if (isalpha((unsigned char)c) || c == '_' || (unsigned char)c >= 0x80 || c == '"' || c == '\'') {
      size_t j = i;
      bool raw = false, str = c == '"' || c == '\'';
      if (!str) {
        while (j < n && (isalnum((unsigned char)t[j]) || t[j] == '_' || (unsigned char)t[j] >= 0x80)) j++;
        string w = t.substr(i, j - i);
        str = j < n && (t[j] == '"' || t[j] == '\'') && w.size() <= 3 && w.find_first_not_of("rRbBuUfF") == string::npos;
        if (!str) { push(0, w); i = j; continue; }
        raw = w.find_first_of("rR") != string::npos;
      }
      char q = t[j];
      bool triple = j + 2 < n && t[j + 1] == q && t[j + 2] == q;
      size_t s = j + (triple ? 3 : 1), e = s;
      for (; e < n; e++) {
        if (t[e] == '\\') { e++; continue; }
        if (triple ? (t[e] == q && e + 2 < n && t[e + 1] == q && t[e + 2] == q) : (t[e] == q || t[e] == '\n')) break;
      }
      (void)raw;
      push(1, t.substr(s, e > s ? e - s : 0));
      i = e + (triple ? 3 : 1);
    } else if (isdigit((unsigned char)c)) {
      size_t j = i;
      while (j < n && (isalnum((unsigned char)t[j]) || t[j] == '.' || t[j] == '_')) j++;
      push(2, "0");
      i = j;
    } else {
      if (c == '(' || c == '[' || c == '{') depth++;
      if (c == ')' || c == ']' || c == '}') depth--;
      push(2, string(1, c));
      i++;
    }
  }
  return out;
}

static Facts scan(const string& text) {
  Facts f;
  vector<Tok> t = tokenize(text);
  for (size_t i = 0; i < t.size(); i++) {
    const Tok& k = t[i];
    if (k.kind == 1) {
      bool doc = k.first && i + 1 < t.size() && t[i + 1].kind == 3;       // a lone string: a docstring
      if (!doc) f.strings.insert(k.s);
    } else if (k.kind == 0 && (k.s == "import_module" || k.s == "__import__") && i + 2 < t.size() && t[i + 1].s == "(" && t[i + 2].kind != 1) {
      f.computed = true;
    } else if (k.kind == 0 && (k.s == "import" || k.s == "from") && (k.first || (i > 0 && t[i - 1].kind == 2 && (t[i - 1].s == ":" || t[i - 1].s == ";")))) {
      size_t j = i + 1;
      auto dotted = [&]() {
        string d;
        while (j < t.size() && (t[j].kind == 0 || t[j].s == ".")) {
          if (t[j].kind == 0 && (t[j].s == "as" || t[j].s == "import")) break;
          d += t[j].s; j++;
        }
        return d;
      };
      if (k.s == "import") {
        for (;;) {
          string d = dotted();
          if (!d.empty()) f.imports.push_back({0, d, {}});
          if (j < t.size() && t[j].s == "as") j += 2;
          if (j < t.size() && t[j].s == ",") { j++; continue; }
          break;
        }
      } else {
        int level = 0;
        while (j < t.size() && t[j].s == ".") { level++; j++; }
        string m = dotted();
        if (j < t.size() && t[j].s == "import") {
          j++;
          Import im{level, m, {}};
          while (j < t.size() && t[j].kind != 3) {
            if (t[j].kind == 0 && t[j].s != "as") im.names.push_back(t[j].s);
            else if (t[j].s == "as") j++;
            j++;
          }
          f.imports.push_back(im);
        }
      }
    }
  }
  return f;
}

// ------------------------------------------------------------------------------------------------ the map
struct Repo {
  fs::path root;
  vector<string> files;                                     // sorted, repo-relative, '/'
  Set known;
  std::map<string, Facts> facts;
  std::map<string, Set> imports, reach;
  std::map<string, vector<string>> byBase;

  vector<string> under(const string& d) const {
    vector<string> out;
    auto it = std::lower_bound(files.begin(), files.end(), d + "/");
    for (; it != files.end() && startsWith(*it, d + "/"); ++it) out.push_back(*it);
    return out;
  }
  Set moduleFiles(const string& dotted) const {          // what importing it runs: it and each parent's __init__
    Set out;
    vector<string> parts = split(dotted, '.');
    string base;
    for (size_t i = 0; i < parts.size(); i++) {
      base += (i ? "/" : "") + parts[i];
      if (known.count(base + ".py")) out.insert(base + ".py");
      else if (known.count(base + "/__init__.py")) out.insert(base + "/__init__.py");
      else break;
    }
    return out;
  }
  Set resolveEntry(const string& e) const {               // a recorded entry: a repo path or a module name
    if (endsWith(e, ".py")) return known.count(e) ? Set{e} : Set{};
    Set out = moduleFiles(e);
    string top = "tests/" + split(e, '.')[0] + ".py";
    if (known.count(top)) out.insert(top);
    return out;
  }
  void load(const fs::path& r) {
    root = r;
    for (const char* d : {"codebreaker", "tests", "tools"}) {
      std::error_code ec;
      for (fs::recursive_directory_iterator it(root / d, ec), end; it != end; it.increment(ec)) {
        if (ec) break;
        if (it->is_directory() && it->path().filename() == "__pycache__") { it.disable_recursion_pending(); continue; }
        if (it->is_regular_file() && it->path().extension() == ".py")
          files.push_back(slash(fs::relative(it->path(), root).generic_string()));
      }
    }
    std::sort(files.begin(), files.end());
    known.insert(files.begin(), files.end());
    for (auto& f : files) { facts[f] = scan(slurp(root / f)); byBase[fs::path(f).filename().string()].push_back(f); }
    for (auto& f : files) {
      const Facts& fa = facts[f];
      string here = fs::path(f).parent_path().generic_string();
      Set hard;
      auto add = [&](const string& name) {
        for (auto& x : moduleFiles(name)) hard.insert(x);
        string top = split(name, '.')[0];
        for (string b : {here, string("tests"), string("tools")}) if (known.count(b + "/" + top + ".py")) hard.insert(b + "/" + top + ".py");
      };
      for (auto& im : fa.imports) {
        string m = im.module;
        if (im.level) {
          vector<string> pkg = split(here, '/');
          pkg.resize(pkg.size() >= (size_t)im.level - 1 ? pkg.size() - (im.level - 1) : 0);
          string base;
          for (auto& p : pkg) base += (base.empty() ? "" : ".") + p;
          m = base + (m.empty() ? "" : (base.empty() ? "" : ".") + m);
        }
        if (m.empty()) continue;
        add(m);
        for (auto& nm : im.names) add(m + "." + nm);
      }
      hard.erase(f);
      imports[f] = hard;
      Set wide = hard;
      for (auto& s : fa.strings) {
        bool mod = startsWith(s, "codebreaker");
        for (size_t i = 0; mod && i < s.size(); i++) if (!(isWordChar(s[i]) || s[i] == '.')) mod = false;
        if (mod) {
          string name = endsWith(s, ".") ? s.substr(0, s.size() - 1) : s;
          if (endsWith(s, ".")) { string d = name; std::replace(d.begin(), d.end(), '.', '/'); for (auto& x : under(d)) wide.insert(x); }
          for (auto& x : moduleFiles(name)) wide.insert(x);
        } else if (endsWith(s, ".py") && s.size() < 200) {
          auto it = byBase.find(fs::path(slash(s)).filename().string());
          if (it != byBase.end()) wide.insert(it->second.begin(), it->second.end());
        }
      }
      if (fa.computed) for (auto& x : under(here)) wide.insert(x);
      wide.erase(f);
      reach[f] = wide;
    }
  }
  static Set closure(Set roots, const std::map<string, Set>& edges) {
    vector<string> todo(roots.begin(), roots.end());
    while (!todo.empty()) {
      string x = todo.back(); todo.pop_back();
      auto it = edges.find(x);
      if (it == edges.end()) continue;
      for (auto& y : it->second) if (roots.insert(y).second) todo.push_back(y);
    }
    return roots;
  }
};

static Set cmakeTargets(const fs::path& dir) {
  Set out;
  string t = slurp(dir / "CMakeLists.txt");
  for (const char* key : {"add_library(", "add_executable("}) {
    for (size_t at = t.find(key); at != string::npos; at = t.find(key, at + 1)) {
      size_t s = at + strlen(key);
      while (s < t.size() && isspace((unsigned char)t[s])) s++;
      size_t e = s;
      while (e < t.size() && (isalnum((unsigned char)t[e]) || t[e] == '_')) e++;
      string name = t.substr(s, e - s);
      if (name.size() > 2 && name != "sqlite3") out.insert(name);
    }
  }
  return out;
}

static int plan(std::map<string, string>& a) {
  Repo repo;
  repo.load(a["--root"]);
  struct Suite { string name, rel, flag; };
  vector<Suite> suites;
  for (auto& l : lines(slurp(a["--suites"]))) { auto p = split(l, '\t'); suites.push_back({p[0], p[1], p.size() > 2 ? p[2] : ""}); }
  std::map<string, vector<string>> traced;
  if (!a["--deps"].empty() && fs::exists(a["--deps"]))
    for (auto& l : lines(slurp(a["--deps"]))) { auto p = split(l, '\t'); traced[p[0]] = vector<string>(p.begin() + 1, p.end()); }
  vector<string> changed;
  for (auto& l : lines(slurp(a["--changed"]))) {
    if (l[0] == 'D') { std::cout << "ALL " << l.substr(2) << " was deleted\n"; return 0; }
    changed.push_back(slash(l.substr(2)));
  }
  std::map<string, Set> closures;
  for (auto& s : suites) {
    Set roots;
    if (repo.known.count(s.rel)) roots.insert(s.rel);
    string stem = fs::path(s.rel).stem().string();
    if (startsWith(stem, "test_")) for (auto& x : repo.under("tests/" + stem.substr(5))) roots.insert(x);
    auto t = traced.find(s.name);
    if (t != traced.end() && t->second.size() > 0) {
      Set c = Repo::closure(roots, repo.imports);
      for (auto& e : t->second) for (auto& x : repo.resolveEntry(e)) c.insert(x);
      closures[s.name] = c;
    } else closures[s.name] = Repo::closure(roots, repo.reach);
  }
  Set chosen;
  auto reachedBy = [&](const Set& seeds) {
    for (auto& s : suites) for (auto& x : seeds) if (closures[s.name].count(x)) { chosen.insert(s.name); break; }
  };
  auto naming = [&](std::function<bool(const string&)> pred) {
    Set out;
    for (auto& f : repo.files) for (auto& c : repo.facts[f].strings) if (pred(c)) { out.insert(f); break; }
    return out;
  };
  bool anyPy = false;
  for (auto& path : changed) {
    vector<string> parts = split(path, '/');
    const string& top = parts[0];
    if (endsWith(path, ".py")) anyPy = true;
    if (endsWith(path, ".py") && repo.known.count(path)) reachedBy({path});
    else if (top == "codebreaker" || top == "tools") {                   // data beside the code that reads it
      string d = fs::path(path).parent_path().generic_string();
      auto direct = [&](const string& dir) { for (auto& g : repo.under(dir)) if (g.find('/', dir.size() + 1) == string::npos) return true; return false; };
      while (!d.empty() && d != "." && !direct(d)) d = fs::path(d).parent_path().generic_string();
      if (d.empty() || d == ".") { std::cout << "ALL " << path << " is data no .py file sits near\n"; return 0; }
      Set seeds;
      for (auto& g : repo.under(d)) seeds.insert(g);
      reachedBy(seeds);
    } else if (top == "tests" && parts.size() > 2 && std::any_of(suites.begin(), suites.end(), [&](const Suite& s) { return s.name == "test_" + parts[1] + ".py"; })) {
      chosen.insert("test_" + parts[1] + ".py");
    } else if (top == "tests" && parts.size() > 2) {                    // fixtures/, data/: whoever names the folder
      string dir = parts[1];
      reachedBy(naming([&](const string& c) { for (auto& seg : split(slash(c), '/')) if (seg == dir) return true; return false; }));
    } else if (top == "docs" || endsWith(path, ".md") || path == "NOTICE" || path == "LICENSE") {
      string base = fs::path(path).filename().string();
      reachedBy(naming([&](const string& c) { return c.find(base) != string::npos; }));
    } else if (top == "native" && parts.size() > 2) {
      string lib = parts[1];
      Set targets = cmakeTargets(repo.root / "native" / lib);
      Set seeds = naming([&](const string& c) {
        string s = slash(c);
        if (hasWord(s, lib + "_bin")) return true;
        size_t at = s.find("native/" + lib);
        if (at != string::npos && !isWordChar(at + 7 + lib.size() < s.size() ? s[at + 7 + lib.size()] : ' ')) return true;
        for (auto& t : targets) if (hasWord(s, t)) return true;
        return false;
      });
      for (auto& f : repo.files) if (repo.facts[f].strings.count("native") && repo.facts[f].strings.count(lib)) seeds.insert(f);
      if (seeds.empty()) { std::cout << "ALL no .py file names native/" << lib << "\n"; return 0; }
      reachedBy(seeds);
    } else {
      std::cout << "ALL " << path << " is not on the dependency map\n";
      return 0;
    }
  }
  if (anyPy) for (auto& s : suites) if (s.flag == "py") chosen.insert(s.name);
  for (auto& s : suites) if (chosen.count(s.name)) std::cout << "RUN " << s.name << "\n";
  return 0;
}

// ------------------------------------------------------------------------------------------------ compile cache
static string quote(const string& s) { return "\"" + s + "\""; }
// runs a command, -> its standard output ("" and rc != 0 when it failed)
static string capture(const vector<string>& argv, int* rc, bool raw = false) {
  string cmd;
  for (auto& x : argv) cmd += (cmd.empty() ? "" : " ") + (raw && (x == "<" || x == "nul" || x == "2>&1" || x == "-") ? x : quote(x));
  cmd = "\"" + cmd + (raw ? "" : " 2>nul") + "\"";
  FILE* p = _popen(cmd.c_str(), "rb");
  string out;
  if (!p) { *rc = -1; return out; }
  char b[1 << 16];
  size_t n;
  while ((n = fread(b, 1, sizeof b, p)) > 0) out.append(b, n);
  *rc = _pclose(p);
  return out;
}
struct Skip { string why; };
struct Plan {
  string kind, key, dkey;                 // key: what is stored under; dkey: the cheap key tried first (g++ -c)
  std::function<string()> pre;            // g++ -c: the key from the preprocessed text (costs a preprocess)
  string exe, src;                        // g++ -c: for the manifest of the files the unit read
  vector<string> preArgs, folders;
  size_t listed = 0;                      // the first `listed` folders are the -I/-iquote ones: their file names are checked
  vector<string> outputs;                 // g++: -o and the map; javac: the -d folder
};
static string trim(string s) { while (!s.empty() && isspace((unsigned char)s.back())) s.pop_back(); return s; }
static string absPath(const string& p) { return fs::absolute(p).string(); }

static bool oddText(const string& t) {          // text whose compile result carries the time it was compiled
  for (const char* w : {"__DATE__", "__TIME__", "__TIMESTAMP__"})
    if (t.find(w) != string::npos) return true;
  return false;
}
// every file under a folder: (a sha of the relative names and bytes, whether one of its sources is oddText).
// Kept on disk by the folder's listing (names, sizes, times), so an unchanged folder costs a stat walk.
static std::pair<string, bool> treeInfo(const fs::path& dir, const string& cache, size_t limit = 20000) {
  vector<std::pair<fs::path, std::pair<uint64_t, int64_t>>> files;
  std::error_code ec;
  for (fs::recursive_directory_iterator it(dir, ec), end; it != end; it.increment(ec)) {
    if (ec) break;
    if (it->is_regular_file()) {
      auto t = it->last_write_time(ec).time_since_epoch().count();
      files.push_back({it->path(), {it->file_size(ec), (int64_t)t}});
    }
    if (files.size() > limit) throw Skip{"folder too big to key on"};
  }
  std::sort(files.begin(), files.end());
  Key list;
  list.add(fs::absolute(dir).string());
  for (auto& f : files) { list.add(slash(fs::relative(f.first, dir).generic_string())); list.add(std::to_string(f.second.first)); list.add(std::to_string(f.second.second)); }
  fs::path memo = fs::path(cache) / "tree" / list.done();
  if (fs::exists(memo)) { string m = slurp(memo); return {m.substr(0, 40), m.size() > 40 && m[40] == '1'}; }
  Key k;
  bool odd = false;
  for (auto& f : files) {
    k.add(slash(fs::relative(f.first, dir).generic_string()));
    k.add(fileSha(f.first, cache));
    string ext = f.first.extension().string();
    if (!odd && (ext == ".h" || ext == ".hpp" || ext == ".c" || ext == ".cpp" || ext == ".cc" || ext == ".inc" || ext == ".hh")) odd = oddText(slurp(f.first));
  }
  string h = k.done();
  fs::create_directories(memo.parent_path(), ec);
  std::ofstream(memo, std::ios::binary) << h << (odd ? "1" : "0");
  return {h, odd};
}
static string treeSha(const fs::path& dir, const string& cache) { return treeInfo(dir, cache).first; }

// The compiler's identity: its bytes and its helpers' (cc1plus, as, ld, ...), and for a link the libraries it
// would pick. Asking the compiler for them costs a dozen process starts, so the answer is kept on disk,
// keyed by the compiler's path, size and time.
static string toolchain(const string& exe, const string& cache, bool libs) {
  std::error_code ec;
  Key id;
  id.add(fs::absolute(exe).string());
  id.add(std::to_string(fs::file_size(exe, ec)));
  id.add(std::to_string(fs::last_write_time(exe, ec).time_since_epoch().count()));
  id.add(libs ? "libs" : "tools");
  fs::path memo = fs::path(cache) / "toolchain" / id.done();
  if (fs::exists(memo)) return slurp(memo);
  Key k;
  k.add(fileSha(exe, cache));
  int rc;
  if (!libs) {
    for (const char* prog : {"cc1plus", "cc1", "as", "ld", "collect2"}) {
      string p = trim(capture({exe, string("-print-prog-name=") + prog}, &rc));
      if (rc == 0 && fs::path(p).is_absolute() && fs::is_regular_file(p)) k.add(string(prog) + fileSha(p, cache));
    }
  } else {
    for (const char* lib : {"libstdc++.a", "libgcc.a", "libmingw32.a", "libmsvcrt.a", "libkernel32.a"}) {
      string p = trim(capture({exe, string("-print-file-name=") + lib}, &rc));
      if (rc == 0 && fs::path(p).is_absolute() && fs::is_regular_file(p)) k.add(fileSha(p, cache));
    }
  }
  string h = k.done();
  fs::create_directories(memo.parent_path(), ec);
  std::ofstream(memo, std::ios::binary) << h;
  return h;
}

// The manifest of a compiled unit (ccache's "direct mode"): the key is the compiler, flags and the source's
// bytes; beside it is the list of every file the preprocessor read -- project files (under the -I/-iquote
// folders or the source's folder) by content, system headers by size and time -- and the names in those
// folders. A hit needs the key and every one of them to match; the exact key (the preprocessed text) is the
// fallback when one does not. A unit that reads __FILE__, assert and the like is never given a manifest.
static string folderNames(const string& folder) {
  vector<string> names;
  std::error_code ec;
  for (fs::recursive_directory_iterator it(folder, ec), end; it != end; it.increment(ec)) {
    if (ec) break;
    if (it->is_regular_file()) names.push_back(slash(fs::relative(it->path(), folder).generic_string()));
    if (names.size() > 5000) throw Skip{"folder too big to key on"};
  }
  std::sort(names.begin(), names.end());
  Key k;
  for (auto& n : names) k.add(n);
  return k.done();
}
static fs::path manifestPath(const string& cache, const string& dkey) { return fs::path(cache) / "manifest" / dkey; }
static string why(const char* what, const string& detail = "") {
  if (getenv("TESTKIT_TIMING")) fprintf(stderr, "testkit: manifest %s %s\n", what, detail.c_str());
  return "";
}
static string manifestVerify(const string& cache, const Plan& pl) {
  fs::path mp = manifestPath(cache, pl.dkey);
  if (!fs::exists(mp)) return why("no manifest");
  string blob;
  std::error_code ec;
  for (auto& l : lines(slurp(mp))) {
    if (l[0] == 'B') blob = l.substr(2);
    else if (l[0] == 'F') {                                           // F <folder> <hash of its file names>
      size_t sp = l.find(' ', 2);
      size_t idx = (size_t)atoi(l.c_str() + 2);
      if (idx >= pl.folders.size() || folderNames(pl.folders[idx]) != l.substr(sp + 1)) return why("folder names", l);
    } else if (l[0] == 'P') {                                         // P <folder> <sha> <relative path>
      size_t s1 = l.find(' ', 2), s2 = l.find(' ', s1 + 1);
      size_t idx = (size_t)atoi(l.c_str() + 2);
      fs::path f = idx < pl.folders.size() ? fs::path(pl.folders[idx]) / l.substr(s2 + 1) : fs::path();
      if (f.empty() || !fs::is_regular_file(f, ec) || fileSha(f) != l.substr(s1 + 1, s2 - s1 - 1)) return why("project file", l);
    } else if (l[0] == 'S') {                                         // S <size> <time> <path>
      size_t s1 = l.find(' ', 2), s2 = l.find(' ', s1 + 1);
      fs::path f = l.substr(s2 + 1);
      if (!fs::is_regular_file(f, ec) || std::to_string(fs::file_size(f, ec)) != l.substr(2, s1 - 2) ||
          std::to_string(fs::last_write_time(f, ec).time_since_epoch().count()) != l.substr(s1 + 1, s2 - s1 - 1)) return why("system file", l);
    }
  }
  return blob;
}
static void manifestWrite(const string& cache, const Plan& pl, const string& blob) {
  if (pl.dkey.empty()) return;
  int rc;
  vector<string> cmd{pl.exe};
  cmd.insert(cmd.end(), pl.preArgs.begin(), pl.preArgs.end());
  cmd.insert(cmd.end(), {"-M", pl.src});
  string rule = capture(cmd, &rc);
  if (rc != 0) return;
  {                                                                   // a path in the preprocessed text (__FILE__, assert): the
    vector<string> e{pl.exe};                                         // object has it too, so the unit is keyed on that text alone
    e.insert(e.end(), pl.preArgs.begin(), pl.preArgs.end());
    e.insert(e.end(), {"-E", "-P", pl.src});
    string text = capture(e, &rc);
    if (rc != 0) return;
    string flat = text;
    std::replace(flat.begin(), flat.end(), '\\', '/');
    for (auto& f : pl.folders) if (flat.find(slash(absPath(f))) != string::npos) return;
  }
  vector<string> deps;                                                // the rule's prerequisites ("\ " in a name, "\\\n" between lines)
  string cur;
  for (size_t i = 0; i < rule.size(); i++) {
    char c = rule[i];
    if (c == '\\' && i + 1 < rule.size() && rule[i + 1] == ' ') { cur += ' '; i++; }
    else if (c == '\\' && i + 1 < rule.size() && (rule[i + 1] == '\n' || rule[i + 1] == '\r')) { i += rule[i + 1] == '\r' ? 2 : 1; if (!cur.empty()) deps.push_back(cur); cur.clear(); }
    else if (isspace((unsigned char)c)) { if (!cur.empty()) deps.push_back(cur); cur.clear(); }
    else cur += c;
  }
  if (!cur.empty()) deps.push_back(cur);
  string out = "B " + blob + "\n";
  for (size_t i = 0; i < pl.listed; i++) out += "F " + std::to_string(i) + " " + folderNames(pl.folders[i]) + "\n";
  string srcAbs = slash(absPath(pl.src));
  for (auto& d : deps) {
    if (endsWith(d, ":") || d == "-") continue;
    std::error_code ec;
    fs::path f = fs::absolute(d, ec);
    string fs_ = slash(f.string());
    if (fs_ == srcAbs || !fs::is_regular_file(f, ec)) continue;
    int where = -1;
    for (size_t i = 0; i < pl.folders.size() && where < 0; i++) {
      string root = slash(absPath(pl.folders[i])) + "/";
      if (startsWith(fs_, root)) { where = (int)i; out += "P " + std::to_string(i) + " " + fileSha(f) + " " + fs_.substr(root.size()) + "\n"; }
    }
    if (where >= 0) { string ext = f.extension().string(); if ((ext == ".h" || ext == ".hpp" || ext == ".inc" || ext == ".hh" || ext == ".cpp" || ext == ".c") && oddText(slurp(f))) return; }
    else out += "S " + std::to_string(fs::file_size(f, ec)) + " " + std::to_string(fs::last_write_time(f, ec).time_since_epoch().count()) + " " + f.string() + "\n";
  }
  fs::path mp = manifestPath(cache, pl.dkey);
  std::error_code ec;
  fs::create_directories(mp.parent_path(), ec);
  fs::path tmp = fs::path(mp.string() + "." + std::to_string(rand()) + ".tmp");
  std::ofstream(tmp, std::ios::binary) << out;
  fs::rename(tmp, mp, ec);
  if (ec) fs::remove(tmp, ec);
}

static Plan planCc(const vector<string>& av, const string& cache) {
  const char* never[] = {"-M", "-save-temps", "-E", "-S", "-g", "-v", "-###", "--version", "-print", "-dump", "-pg", "-fprofile", "-coverage", "--coverage", "-flto", "-fdump", "-Wa,", "-time"};
  string exe = av[0];
  if (!fs::exists(exe)) {
    int rc;
    string where = trim(capture({"where", exe}, &rc));
    if (rc != 0 || where.empty()) throw Skip{"compiler not found"};
    exe = lines(where)[0];
  }
  string out, mapfile;
  vector<string> inputs, flags, pre, folders;
  bool compile = false;
  for (size_t i = 1; i < av.size(); i++) {
    const string& t = av[i];
    for (const char* n : never) if (startsWith(t, n)) throw Skip{"option " + t};
    if (t[0] == '@' || (startsWith(t, "-o") && t != "-o")) throw Skip{"option " + t};
    if (t == "-o") out = av.at(++i);
    else if (startsWith(t, "-Wl,-Map,")) { mapfile = t.substr(9); flags.push_back("-Wl,-Map,<map>"); }
    else if (startsWith(t, "-Wl,") && t.find_first_of("/\\") != string::npos) throw Skip{"linker option with a path"};
    else if (t == "-iquote" || t == "-isystem" || t == "-idirafter" || t == "-I" || t == "-L" || startsWith(t, "-I") || startsWith(t, "-L")) {
      string opt = t.substr(0, t == "-iquote" ? 7 : t == "-isystem" ? 8 : t == "-idirafter" ? 10 : 2);
      string d = t.size() > opt.size() ? t.substr(opt.size()) : av.at(++i);
      if (!fs::is_directory(d)) throw Skip{"folder missing"};
      if (opt != "-L") { pre.push_back(opt); pre.push_back(absPath(d)); folders.push_back(absPath(d)); }
      flags.push_back(opt + "<dir>");
    } else if (t == "-include" || t == "-imacros") {
      string p = av.at(++i);
      if (!fs::is_regular_file(p)) throw Skip{"include file missing"};
      flags.push_back(t + "<" + fileSha(p) + ">"); pre.push_back(t); pre.push_back(absPath(p));
    } else if (t == "-D" || t == "-U" || t == "-x" || t == "-l" || t == "-Xlinker" || t == "-Xpreprocessor" || t == "-T" || t == "-u" || t == "-z") {
      flags.push_back(t); flags.push_back(av.at(++i)); pre.push_back(t); pre.push_back(av[i]);
    } else if (t[0] == '-') {
      if (t == "-c") compile = true;
      flags.push_back(t);
      if (!(startsWith(t, "-O") || startsWith(t, "-Wl,") || startsWith(t, "-l") || t == "-c")) pre.push_back(t);
    } else {
      if (!fs::is_regular_file(t)) throw Skip{"input missing: " + t};
      inputs.push_back(t);
    }
  }
  if (out.empty() || inputs.empty()) throw Skip{"no -o or no input"};
  Key k;
  int rc;
  lap("start");
  k.add(toolchain(exe, cache, false));
  lap("toolchain");
  for (auto& f : flags) k.add(f);
  k.add(fs::path(out).extension().string());
  Plan pl;
  pl.outputs.push_back(absPath(out));
  if (!mapfile.empty()) pl.outputs.push_back(absPath(mapfile));
  string ext = inputs[0].size() > 4 ? inputs[0].substr(inputs[0].rfind('.')) : "";
  if (compile && inputs.size() == 1 && (ext == ".cpp" || ext == ".cc" || ext == ".c" || ext == ".cxx")) {
    string src = absPath(inputs[0]);
    Key base = k;
    pl.kind = "cc";
    string text = slurp(src);
    if (!oddText(text)) {                                   // the cheap key: the source's bytes (a manifest says the rest)
      Key d = base;
      d.add("direct2");
      d.add(text);
      pl.dkey = d.done();
    }
    pl.exe = exe; pl.src = src; pl.preArgs = pre;
    for (auto& f : folders) if (std::find(pl.folders.begin(), pl.folders.end(), f) == pl.folders.end()) pl.folders.push_back(f);
    pl.listed = pl.folders.size();                         // (the source's own folder is read by content only: its
    string home = fs::path(src).parent_path().string();    //  listing changes as a test writes its outputs there)
    if (std::find(pl.folders.begin(), pl.folders.end(), home) == pl.folders.end()) pl.folders.push_back(home);
    pl.pre = [=]() mutable {                                // the exact key: the preprocessed text
      vector<string> cmd{exe};
      cmd.insert(cmd.end(), pre.begin(), pre.end());
      cmd.insert(cmd.end(), {"-E", "-P", src});
      int rc2;
      string text = capture(cmd, &rc2);
      if (rc2 != 0) throw Skip{"does not preprocess"};
      base.add("pre");
      base.add(text);
      return base.done();
    };
    return pl;
  } else {
    k.add("link");
    k.add(toolchain(exe, cache, true));
    for (auto& in : inputs) { k.add(fs::path(in).extension().string()); k.add(fileSha(in, cache)); }
    pl.kind = "link";
  }
  pl.key = k.done();
  return pl;
}

static Plan planJavac(const vector<string>& av, const string& cache, bool after) {
  string exe = av[0];
  if (!fs::exists(exe)) {
    int rc;
    string where = trim(capture({"where", exe}, &rc));
    if (rc != 0 || where.empty()) throw Skip{"javac not found"};
    exe = lines(where)[0];
  }
  string out;
  vector<string> flags, sources, cp, sp;
  auto paths = [](const string& s) { vector<string> r; for (auto& x : split(s, ';')) if (!x.empty()) r.push_back(x); return r; };
  for (size_t i = 1; i < av.size(); i++) {
    const string& t = av[i];
    if (t == "-d") out = av.at(++i);
    else if (t == "-cp" || t == "-classpath" || t == "--class-path") { for (auto& x : paths(av.at(++i))) cp.push_back(x); }
    else if (t == "-sourcepath" || t == "--source-path") { for (auto& x : paths(av.at(++i))) sp.push_back(x); }
    else if (t == "-encoding" || t == "--release" || t == "-source" || t == "-target") { flags.push_back(t); flags.push_back(av.at(++i)); }
    else if (t[0] == '@') {
      for (auto& l : lines(slurp(t.substr(1)))) {
        string s = l;
        while (!s.empty() && (s.front() == '"' || isspace((unsigned char)s.front()))) s.erase(0, 1);
        while (!s.empty() && (s.back() == '"' || isspace((unsigned char)s.back()))) s.pop_back();
        if (!s.empty()) { if (!fs::is_regular_file(s)) throw Skip{"source missing"}; sources.push_back(s); }
      }
    } else if (t == "-nowarn" || t == "-parameters" || t == "-deprecation" || startsWith(t, "-Xlint") || startsWith(t, "-g:") || t == "-proc:none" || t == "-implicit:none") flags.push_back(t);
    else if (t[0] == '-') throw Skip{"option " + t};
    else { if (!fs::is_regular_file(t)) throw Skip{"source missing"}; sources.push_back(t); }
  }
  if (out.empty() || sources.empty()) throw Skip{"no -d or no sources"};
  if (!after && fs::exists(out) && !fs::is_empty(out)) throw Skip{"the -d folder is not empty"};
  if (cp.empty() && (getenv("CLASSPATH") || sp.empty())) throw Skip{"the default class path is not a thing to key on"};
  if (cp.empty()) for (auto& e : fs::directory_iterator(".")) if (e.path().extension() == ".class" || e.path().extension() == ".jar") throw Skip{"class files in the working folder"};
  Key k;
  k.add("javac");
  k.add(fileSha(exe, cache));
  fs::path home = fs::weakly_canonical(exe).parent_path().parent_path();
  if (fs::exists(home / "lib" / "modules")) k.add(fileSha(home / "lib" / "modules", cache));
  if (fs::exists(home / "release")) k.add(slurp(home / "release"));
  for (auto& f : flags) k.add(f);
  for (auto* list : {&cp, &sp}) {
    k.add(list == &cp ? "cp" : "sp");
    for (auto& e : *list) k.add(fs::is_directory(e) ? treeSha(e, cache) : fs::is_regular_file(e) ? fileSha(e, cache) : "-");
  }
  for (auto& s : sources) k.add(fs::path(s).filename().string() + ":" + fileSha(s, cache));
  Plan pl;
  pl.kind = "javac";
  pl.outputs.push_back(absPath(out));
  pl.key = k.done();
  return pl;
}

static Plan makePlan(const vector<string>& av, const string& cache, bool after = false) {
  string name = fs::path(av[0]).stem().string();
  std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return (char)tolower(c); });
  if (name == "javac") return planJavac(av, cache, after);
  if (name == "g++" || name == "gcc" || name == "c++" || name == "cc" || name == "clang" || name == "clang++") return planCc(av, cache);
  throw Skip{"not a compiler"};
}
static fs::path entry(const string& cache, const string& key) { return fs::path(cache) / "obj" / key.substr(0, 2) / key; }

// an entry, or the one an alias entry points at; "" when there is none
static fs::path found(const string& cache, const string& key) {
  if (key.empty()) return "";
  fs::path e = entry(cache, key);
  if (fs::exists(e / "alias")) e = entry(cache, trim(slurp(e / "alias")));
  return fs::exists(e / "ok") ? e : fs::path();
}
static void alias(const string& cache, const string& key, const string& target) {
  if (key.empty() || key == target) return;
  fs::path e = entry(cache, key);
  std::error_code ec;
  fs::create_directories(e, ec);
  std::ofstream(e / "alias.tmp", std::ios::binary) << target;
  fs::rename(e / "alias.tmp", e / "alias", ec);
}

static int lookup(std::map<string, string>& a, const vector<string>& av) {
  string cache = a["--cache"];
  if (!a["--cwd"].empty()) fs::current_path(a["--cwd"]);
  try {
    Plan pl = makePlan(av, cache);
    fs::path e;                                            // the cheap key and its manifest first, then the exact key
    string storeKey = pl.key;
    if (!pl.dkey.empty()) e = found(cache, manifestVerify(cache, pl));
    if (e.empty() && pl.pre) {
      storeKey = pl.pre();
      e = found(cache, storeKey);
      if (!e.empty()) manifestWrite(cache, pl, storeKey);
    } else if (e.empty()) e = found(cache, pl.key);
    if (e.empty()) { std::cout << "MISS " << storeKey << " " << pl.dkey << "\n"; return 1; }
    std::error_code ec;
    if (pl.kind == "javac") {
      fs::create_directories(pl.outputs[0], ec);
      fs::copy(e / "files", pl.outputs[0], fs::copy_options::recursive | fs::copy_options::overwrite_existing, ec);
    } else {
      for (size_t i = 0; i < pl.outputs.size(); i++) {
        fs::create_directories(fs::path(pl.outputs[i]).parent_path(), ec);
        fs::copy_file(e / ("out" + std::to_string(i)), pl.outputs[i], fs::copy_options::overwrite_existing, ec);
      }
    }
    if (ec) { std::cout << "MISS " << storeKey << " " << pl.dkey << "\n"; return 1; }
    fs::last_write_time(e / "ok", fs::file_time_type::clock::now(), ec);      // used just now: pruned last
    std::cout << "HIT\n";
    return 0;
  } catch (Skip& s) {
    std::cout << "SKIP " << s.why << "\n";
    return 2;
  } catch (std::exception& x) {
    std::cout << "SKIP " << x.what() << "\n";
    return 2;
  }
}

static int store(std::map<string, string>& a, const vector<string>& av) {
  string cache = a["--cache"];
  if (!a["--cwd"].empty()) fs::current_path(a["--cwd"]);
  try {
    Plan pl = makePlan(av, cache, true);
    string key = a["--key"];
    if (key.empty() || (!pl.pre && pl.key != key)) return 2;       // (the inputs changed under the run: store nothing)
    fs::path e = entry(cache, key);
    fs::path tmp = fs::path(e.string() + "." + std::to_string(rand()) + ".tmp");
    std::error_code ec;
    fs::create_directories(tmp, ec);
    if (pl.kind == "javac") fs::copy(pl.outputs[0], tmp / "files", fs::copy_options::recursive, ec);
    else for (size_t i = 0; i < pl.outputs.size(); i++) {
      if (!fs::is_regular_file(pl.outputs[i])) { fs::remove_all(tmp, ec); return 2; }
      fs::copy_file(pl.outputs[i], tmp / ("out" + std::to_string(i)), ec);
    }
    std::ofstream(tmp / "ok") << "ok";
    fs::rename(tmp, e, ec);
    if (ec) fs::remove_all(tmp, ec);                     // (another run stored it first)
    pl.dkey = a["--dkey"];
    manifestWrite(cache, pl, key);
    return 0;
  } catch (...) {
    return 2;
  }
}

static int prune(std::map<string, string>& a) {
  uint64_t limit = (uint64_t)atoll(a["--mb"].c_str()) << 20, total = 0;
  struct E { fs::file_time_type age; fs::path dir; uint64_t size; };
  vector<E> all;
  std::error_code ec;
  fs::path root = fs::path(a["--cache"]) / "obj";
  if (!fs::exists(root)) return 0;
  for (auto& d1 : fs::directory_iterator(root, ec)) for (auto& d2 : fs::directory_iterator(d1.path(), ec)) {
    uint64_t size = 0;
    for (auto& f : fs::recursive_directory_iterator(d2.path(), ec)) if (f.is_regular_file()) size += f.file_size();
    fs::file_time_type age = fs::exists(d2.path() / "ok") ? fs::last_write_time(d2.path() / "ok", ec) : fs::file_time_type::min();
    all.push_back({age, d2.path(), size});
    total += size;
  }
  std::sort(all.begin(), all.end(), [](const E& x, const E& y) { return x.age < y.age; });
  for (auto& e : all) { if (total <= limit) break; fs::remove_all(e.dir, ec); total -= e.size; }
  return 0;
}

int main(int argc, char** argv) {
  if (argc < 2) { fprintf(stderr, "usage: testkit plan|lookup|store|prune ...\n"); return 64; }
  string cmd = argv[1];
  std::map<string, string> a;
  vector<string> rest;
  for (int i = 2; i < argc; i++) {
    string s = argv[i];
    if (s == "--") { for (int j = i + 1; j < argc; j++) rest.push_back(argv[j]); break; }
    if (startsWith(s, "--") && i + 1 < argc) a[s] = argv[++i];
  }
  try {
    if (cmd == "plan") return plan(a);
    if (cmd == "lookup" && !rest.empty()) return lookup(a, rest);
    if (cmd == "store" && !rest.empty()) return store(a, rest);
    if (cmd == "prune") return prune(a);
  } catch (std::exception& e) {
    fprintf(stderr, "testkit: %s\n", e.what());
    return 70;
  }
  fprintf(stderr, "usage: testkit plan|lookup|store|prune ...\n");
  return 64;
}
