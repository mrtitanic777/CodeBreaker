// early_c_text.cc -- invert/early_c.py's text passes over the statements, decision for decision:
// _tidy, _strip_tail_returns, _dead_regs, _fold_ladders (_neg_cond), _fold_goto_or, _fold_longs,
// _fold_shortcircuit, _dead_store_recover, detect_copies and to_c (with early_c_frame's
// emit_local_decls).  The regular expressions are the Python's, run by pyre.
#include <algorithm>
#include <functional>
#include <map>
#include <set>

#include "early_c.h"
#include "early_c_util.h"

namespace cbinv {

using namespace ecu;
using std::string;

namespace {
const char *TOK = "(?<![\\w>.])(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh)(?![\\w(])";
const std::set<string> &PURE() {
  static const std::set<string> P = {"BYTE", "HIBYTE", "SX", "S8", "S16", "LONG", "LO", "HI", "LSHR", "LSAR",
                                     "U32", "S32", "FARM", "FAROFF", "unsigned", "int", "char", "long"};
  return P;
}
std::vector<string> atoms(const string &r) {
  if (r == "ax") return {"al", "ah"};
  if (r == "bx") return {"bl", "bh"};
  if (r == "cx") return {"cl", "ch"};
  if (r == "dx") return {"dl", "dh"};
  return {r};
}
bool is_atom_name(const string &h) {
  static const std::set<string> A = {"ax", "bx", "cx", "dx", "si", "di", "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh"};
  return A.count(h) > 0;
}
std::vector<string> slice(const Lines &v, long long a, long long b) {
  long long n = (long long)v.size();
  if (a < 0) a = std::max(0LL, a + n);
  if (b < 0) b = std::max(0LL, b + n);
  if (b > n) b = n;
  if (a >= b) return {};
  return Lines(v.begin() + a, v.begin() + b);
}
void splice(Lines &v, long long a, long long b, const Lines &with) {   // v[a:b] = with
  long long n = (long long)v.size();
  if (a > n) a = n;
  if (b > n) b = n;
  if (b < a) b = a;
  v.erase(v.begin() + a, v.begin() + b);
  v.insert(v.begin() + a, with.begin(), with.end());
}
}  // namespace

Lines ec_tidy(Lines lines) {
  bool changed = true;
  while (changed) {
    changed = false;
    for (long long i = 0; i < (long long)lines.size() - 2; i++) {
      const string a = lines[i];
      long long n = indent(a);
      string pad = spaces(n), inner = pad + "    ";
      const string b = lines[i + 1];
      if (!(starts(b, (inner + "if (").c_str()) && ends(b, ") {") && indent(b) == n + 4)) continue;
      bool is_else = a == pad + "} else {";
      if (!(is_else || (starts(a, (pad + "if (").c_str()) && ends(a, ") {")))) continue;
      long long j = i + 2;
      bool has_else = false, nonej = false;
      while (j < (long long)lines.size()) {
        if (lines[j] == inner + "}") break;
        if (indent(lines[j]) == n + 4 && starts(lines[j], (inner + "} else").c_str())) has_else = true;
        if (indent(lines[j]) <= n && !strip(lines[j]).empty()) {
          nonej = true;
          break;
        }
        j++;
      }
      if (nonej || j >= (long long)lines.size() - 1 || lines[j + 1] != pad + "}") continue;
      Lines mid;
      for (long long q = i + 2; q < j; q++) mid.push_back(sl(lines[q], 4));
      if (is_else) {
        Lines w = {pad + "} else " + strip(b)};
        w.insert(w.end(), mid.begin(), mid.end());
        w.push_back(pad + "}");
        splice(lines, i, j + 2, w);
        changed = true;
        break;
      }
      if (!has_else) {
        string ca = sl(a, n + 3, -2), cb = sl(b, n + 7, -2);
        Lines w = {pad + "if (" + ca + " && " + cb + ") {"};
        w.insert(w.end(), mid.begin(), mid.end());
        w.push_back(pad + "}");
        splice(lines, i, j + 2, w);
        changed = true;
        break;
      }
    }
  }
  return lines;
}

Lines ec_strip_tail_returns(Lines lines) {
  bool changed = true;
  while (changed) {
    changed = false;
    for (long long i = 0; i < (long long)lines.size(); i++) {
      if (strip(lines[i]) != "return;") continue;
      long long j = i + 1;
      bool tail = false;
      long long L = (long long)lines.size();
      while (true) {
        if (j >= L) {
          tail = true;
          break;
        }
        string t = strip(lines[j]);
        long long d = indent(lines[j]);
        if (t.empty()) {
          j++;
          continue;
        }
        if (!starts(t, "}")) break;
        long long k = j - 1;
        while (k >= 0 && !(indent(lines[k]) == d && ends(rstrip(lines[k]), "{"))) k--;
        if (k < 0 || !mt("(\\} else )?if \\(.*\\) \\{$|\\} else \\{$", strip(lines[k]))) break;
        if (t == "}") {
          j++;
          continue;
        }
        long long e = j;
        while (true) {
          e++;
          while (e < L && !(indent(lines[e]) == d && starts(strip(lines[e]), "}"))) e++;
          if (e >= L || strip(lines[e]) == "}") break;
        }
        if (e >= L) break;
        j = e + 1;
      }
      if (!tail) continue;
      string prev = i ? strip(lines[i - 1]) : "";
      string nxt = i + 1 < L ? strip(lines[i + 1]) : "";
      if (prev == "} else {" && nxt == "}") {
        lines.erase(lines.begin() + i - 1, lines.begin() + i + 1);
        changed = true;
        break;
      }
      if (ends(prev, "{") && prev != "{") continue;
      lines.erase(lines.begin() + i);
      changed = true;
      break;
    }
  }
  return lines;
}

// ------------------------------------------------------------------------------------ _dead_regs
namespace {
struct Succ {
  enum K { NEXT, TGT, LABEL, INT, NONE } k = NONE;
  std::shared_ptr<long long> t;   // TGT: ["tgt", x] (LLONG_MIN = None)
  string label;
  long long v = 0;
};
const long long NN = LLONG_MIN;
struct DrOp {
  std::set<string> use, dfn, out;
  long long line = NN;
  bool has_succ = false;
  std::vector<Succ> succ;
  bool rem = false;
  long long src = 0;
};
Succ S_next() {
  Succ s;
  s.k = Succ::NEXT;
  return s;
}
Succ S_int(long long v) {
  Succ s;
  s.k = Succ::INT;
  s.v = v;
  return s;
}
Succ S_none() { return Succ(); }
Succ S_tgt(const std::shared_ptr<long long> &t) {
  Succ s;
  s.k = Succ::TGT;
  s.t = t;
  return s;
}
}  // namespace

Lines ec_dead_regs(Lines lines) {
  const re::Regex &DRT = RX(TOK);
  auto uses = [&](const string &t) {
    std::set<string> o;
    for (auto &m : DRT.finditer(t))
      for (auto &a : atoms(m.group(1))) o.insert(a);
    return o;
  };
  auto pure = [&](const string &e) {
    if (has(e, "++") || has(e, "--")) return false;
    for (auto &m : RX("([A-Za-z_]\\w*)\\s*\\(").finditer(e))
      if (!PURE().count(m.group(1))) return false;
    return true;
  };

  for (int round = 0; round < 16; round++) {
    std::vector<DrOp> ops;
    std::map<string, long long> labels;
    long long pos = 0;
    bool bad = false;
    std::vector<size_t> stack_sw;
    auto op = [&](std::set<string> use = {}, std::set<string> dfn = {}, long long line = NN, bool rem = false) -> size_t {
      DrOp o;
      o.use = use, o.dfn = dfn, o.line = line, o.rem = rem, o.src = pos - 1;
      ops.push_back(o);
      return ops.size() - 1;
    };
    typedef std::shared_ptr<long long> T;
    std::function<std::pair<string, string>(T, T)> block;
    std::function<void(const string &, T, T)> chain = [&](const string &c, T brk, T cont) {
      size_t jz = op(uses(c));
      ops[jz].has_succ = true;
      ops[jz].succ = {S_next(), S_none()};
      auto ea = block(brk, cont);
      if (ea.first == "else" || ea.first == "elif") {
        size_t jm = op();
        ops[jm].has_succ = true;
        ops[jm].succ = {S_none()};
        ops[jz].succ[1] = S_int((long long)ops.size());
        if (ea.first == "else") block(brk, cont);
        else chain(ea.second, brk, cont);
        ops[jm].succ[0] = S_int((long long)ops.size());
      } else {
        ops[jz].succ[1] = S_int((long long)ops.size());
      }
    };
    block = [&](T brk, T cont) -> std::pair<string, string> {
      while (pos < (long long)lines.size()) {
        long long i = pos;
        string ln = strip(lines[i]);
        pos++;
        if (ln.empty() || starts(ln, "/*")) continue;
        if (ln == "}") return {"end", ""};
        if (ln == "} else {") return {"else", ""};
        auto m = mt("\\} else if \\((.*)\\) \\{$", ln);
        if (m) return {"elif", g(m, 1)};
        m = mt("\\} while \\((.*)\\);$", ln);
        if (m) return {"dowhile", g(m, 1)};
        m = mt("if \\((.*)\\) (break|continue);$", ln);
        if (m) {
          size_t o = op(uses(g(m, 1)));
          T tg = g(m, 2) == "break" ? brk : cont;
          if (!tg) {
            bad = true;
            return {"end", ""};
          }
          ops[o].has_succ = true;
          ops[o].succ = {S_next(), S_tgt(tg)};
          continue;
        }
        m = mt("if \\((.*)\\) \\{$", ln);
        if (m) {
          chain(g(m, 1), brk, cont);
          continue;
        }
        m = mt("while \\((.*)\\) \\{$", ln);
        if (m || ln == "for (;;) {") {
          long long top = (long long)ops.size();
          T b = std::make_shared<long long>(NN);
          long long jz = -1;
          if (m) jz = (long long)op(uses(g(m, 1)));
          block(b, std::make_shared<long long>(top));
          size_t back = op();
          ops[back].has_succ = true;
          ops[back].succ = {S_int(top)};
          *b = (long long)ops.size();
          if (jz >= 0) ops[jz].has_succ = true, ops[jz].succ = {S_next(), S_int((long long)ops.size())};
          continue;
        }
        if (ln == "do {") {
          long long top = (long long)ops.size();
          T b = std::make_shared<long long>(NN), c = std::make_shared<long long>(NN);
          auto ea = block(b, c);
          if (ea.first != "dowhile") {
            bad = true;
            return {"end", ""};
          }
          *c = (long long)ops.size();
          size_t t = op(uses(ea.second));
          ops[t].has_succ = true;
          ops[t].succ = {S_next(), S_int(top)};
          *b = (long long)ops.size();
          continue;
        }
        m = mt("switch \\((.*)\\) \\{$", ln);
        if (m) {
          size_t sw = op(uses(g(m, 1)));
          ops[sw].has_succ = true;
          ops[sw].succ.clear();
          T b = std::make_shared<long long>(NN);
          stack_sw.push_back(sw);
          block(b, cont);
          stack_sw.pop_back();
          *b = (long long)ops.size();
          ops[sw].succ.push_back(S_int((long long)ops.size()));
          continue;
        }
        if (mt("(case -?\\w+|default):$", ln)) {
          if (stack_sw.empty()) {
            bad = true;
            return {"end", ""};
          }
          ops[stack_sw.back()].succ.push_back(S_int((long long)ops.size()));
          continue;
        }
        if (ln == "break;" || ln == "continue;") {
          T tg = ln == "break;" ? brk : cont;
          if (!tg) {
            bad = true;
            return {"end", ""};
          }
          size_t o = op();
          ops[o].has_succ = true;
          ops[o].succ = {S_tgt(tg)};
          continue;
        }
        m = mt("return\\b(.*);$", ln);
        if (m) {
          auto u = uses(g(m, 1));
          if (!strip(g(m, 1)).empty()) u.insert("dl"), u.insert("dh");
          size_t o = op(u);
          ops[o].has_succ = true;
          ops[o].succ.clear();
          continue;
        }
        m = mt("goto (\\w+);$", ln);
        if (m) {
          size_t o = op();
          Succ s;
          s.k = Succ::LABEL;
          s.label = g(m, 1);
          ops[o].has_succ = true;
          ops[o].succ = {s};
          continue;
        }
        m = mt("(\\w+):$", ln);
        if (m) {
          labels[g(m, 1)] = (long long)ops.size();
          continue;
        }
        if (!ends(ln, ";")) {
          bad = true;
          return {"end", ""};
        }
        m = mt("(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh) = (.*);$", ln);
        if (m && !has(g(m, 2), " = ")) {
          auto a = atoms(g(m, 1));
          op(uses(g(m, 2)), std::set<string>(a.begin(), a.end()), i, pure(g(m, 2)));
        } else {
          op(uses(ln));
        }
      }
      return {"eof", ""};
    };
    auto ea = block(nullptr, nullptr);
    if (bad || ea.first != "eof") return lines;
    long long n = (long long)ops.size();
    std::vector<std::vector<long long>> succ;
    for (long long k = 0; k < n; k++) {
      std::vector<long long> s_;
      if (!ops[k].has_succ) s_ = {k + 1};
      else
        for (auto &x : ops[k].succ) {
          long long v;
          switch (x.k) {
            case Succ::NEXT: v = k + 1; break;
            case Succ::TGT: v = *x.t; break;
            case Succ::LABEL: v = labels.count(x.label) ? labels[x.label] : -1; break;
            case Succ::INT: v = x.v; break;
            default: v = NN;
          }
          s_.push_back(v);
        }
      for (auto x : s_)
        if (x == NN || x == -1) return lines;
      succ.push_back(s_);
    }
    std::vector<std::set<string>> livein(n + 1);
    bool changed = true;
    while (changed) {
      changed = false;
      for (long long k = n - 1; k >= 0; k--) {
        DrOp &o = ops[k];
        std::set<string> out;
        for (auto x : succ[k]) {
          auto &li = livein[std::min(x, n)];
          out.insert(li.begin(), li.end());
        }
        o.out = out;
        std::set<string> nw;
        for (auto &x : out)
          if (!o.dfn.count(x)) nw.insert(x);
        nw.insert(o.use.begin(), o.use.end());
        if (nw != livein[k]) livein[k] = nw, changed = true;
      }
    }
    std::set<long long> kill;
    for (auto &o : ops) {
      if (!o.rem || o.line == NN) continue;
      bool inter = false;
      for (auto &x : o.dfn)
        if (o.out.count(x)) inter = true;
      if (!inter) kill.insert(o.line);
    }
    auto callsf = [&](const string &t) {
      long long c = 0;
      for (auto &m : RX("([A-Za-z_]\\w*)\\s*\\(").finditer(t)) {
        string g1 = m.group(1);
        if (!PURE().count(g1) && g1 != "if" && g1 != "switch" && g1 != "return" && g1 != "while") c++;
      }
      return c;
    };
    std::map<long long, string> sub;
    std::set<long long> busy = kill;
    for (long long k = 0; k < n - 1; k++) {
      DrOp &o = ops[k], &q = ops[k + 1];
      long long i = o.src;
      if (succ[k] != std::vector<long long>{k + 1} || q.src != i + 1 || busy.count(i) || busy.count(i + 1)) continue;
      auto m = mt("(\\s*)(ax|bx|cx|dx|si|di|t\\d+) = (.*);$", at(lines, i));
      if (!m || has(g(m, 3), " = ")) continue;
      string reg = g(m, 2), e = g(m, 3);
      string nxt = at(lines, i + 1);
      string t = strip(nxt);
      if (!(ends(t, ";") || mt("(if|switch) \\(.*\\) \\{$", t)) || mt("if \\(.*\\) (break|continue);$", t)) continue;
      if (starts(t, "USE(")) continue;
      if (starts(t, "goto ") || starts(t, "break") || starts(t, "continue")) continue;
      re::Regex pat("(?<![\\w>.&])" + reg + "(?![\\w(])");
      if (pat.findall(nxt).size() != 1) continue;
      if (srd("(BYTE|HIBYTE|LONG|LO|HI)\\(" + reg + "\\)", nxt) || re::Regex("\\s*" + reg + "\\b").match(nxt)) continue;
      if (reg[0] == 't') {
        if (has(e, "LONG(") || has(e, "(long")) continue;
        re::Regex wb("\\b" + reg + "\\b");
        long long tot = 0;
        for (auto &x : lines) tot += (long long)wb.findall(x).size();
        if (tot != 2) continue;
      } else {
        auto av = atoms(reg);
        std::set<string> at_(av.begin(), av.end());
        bool still = false;
        for (auto &x : at_)
          if (q.out.count(x) && !q.dfn.count(x)) still = true;
        if (still) continue;
        bool half = false;
        for (auto &h : DRT.findall(nxt)) {
          if (!is_atom_name(h) || h == reg) continue;
          for (auto &x : atoms(h))
            if (at_.count(x)) half = true;
        }
        if (half) continue;
      }
      long long ce = callsf(e), cn = callsf(nxt);
      if ((ce && cn) || cn > 1 || has(e, "++") || has(e, "--")) continue;
      auto mo = pat.search(nxt);
      if (ce) {
        string rest = nxt.substr(0, mo.start()) + nxt.substr(mo.end());
        auto cut = sr("(?<![=!<>]) = ", rest);
        string rhs = cut ? rest.substr(cut.end()) : rest;
        bool bad2 = false;
        for (auto &w : RX("[A-Za-z_]\\w*").findall(rhs))
          if (!PURE().count(w) && w != "if" && w != "switch" && w != "return") bad2 = true;
        if (bad2) continue;
      } else if (cn) {
        std::vector<re::Match> mc;
        for (auto &mm : RX("([A-Za-z_]\\w*)\\s*\\(").finditer(nxt)) {
          string g1 = mm.group(1);
          if (!PURE().count(g1) && g1 != "if" && g1 != "switch" && g1 != "return" && g1 != "while") mc.push_back(mm);
        }
        long long d = 0, end = NN;
        for (long long qq = mc[0].end(); qq < (long long)nxt.size(); qq++) {
          if (nxt[qq] == '(') d++;
          else if (nxt[qq] == ')') {
            if (d == 0) {
              end = qq;
              break;
            }
            d--;
          }
        }
        if (end == NN || !(mc[0].end() <= mo.start() && mo.start() < end)) continue;
      }
      if ((has(nxt, "&&") || has(nxt, "||")) && (ce || cn)) continue;
      bool atom = (bool)mt("(\\w+|\\d+|[A-Za-z_]\\w*\\([^()]*\\))$", e) || (starts(e, "(") && ends(e, ")"));
      string rep = atom ? e : "(" + e + ")";
      sub[i] = pat.sub_fn([&](const re::Match &) { return rep; }, nxt);
      busy.insert(i), busy.insert(i + 1);
    }
    if (kill.empty() && sub.empty()) break;
    for (auto &kv : sub) lines[kv.first + 1] = kv.second;
    for (auto &kv : sub) kill.insert(kv.first);
    Lines nl;
    for (long long i = 0; i < (long long)lines.size(); i++)
      if (!kill.count(i)) nl.push_back(lines[i]);
    lines = nl;
    bool ch = true;
    while (ch) {
      ch = false;
      for (long long i = 0; i < (long long)lines.size() - 1; i++) {
        string a = strip(lines[i]), b = strip(lines[i + 1]);
        if (a == "} else {" && b == "}") {
          lines.erase(lines.begin() + i);
          ch = true;
          break;
        }
        auto m = mt("if \\((.*)\\) \\{$", a);
        if (m && b == "}" && pure(g(m, 1))) {
          lines.erase(lines.begin() + i, lines.begin() + i + 2);
          ch = true;
          break;
        }
        m = mt("(\\s*)if \\(\\((.*) [!=]= 0\\)\\) \\{$", lines[i]);
        if (m && b == "}") {
          splice(lines, i, i + 2, {g(m, 1) + g(m, 2) + ";"});
          ch = true;
          break;
        }
        m = mt("(\\s*)((?:\\} else )?if) \\((.*)\\) \\{$", lines[i]);
        auto m2 = mt("\\} else if \\((.*)\\) \\{$", b);
        auto negp = [&](const string &c) { return starts(c, "(") ? c : "(" + c + ")"; };
        if (m && m2) {
          long long d = 0, j = i + 2;
          while (j < (long long)lines.size()) {
            string t = strip(lines[j]);
            if (starts(t, "}") && d == 0 && t == "}") break;
            if (starts(t, "}")) d--;
            if (ends(t, "{")) d++;
            j++;
          }
          if (j < (long long)lines.size()) {
            string ind = g(m, 1);
            for (long long q = i + 2; q <= j; q++) lines[q] = "    " + lines[q];
            lines.insert(lines.begin() + j + 1, ind + "}");
            splice(lines, i, i + 2, {ind + g(m, 2) + " (!" + negp(g(m, 3)) + ") {", ind + "    if (" + g(m2, 1) + ") {"});
            ch = true;
            break;
          }
        }
        if (m && b == "} else {") {
          splice(lines, i, i + 2, {g(m, 1) + g(m, 2) + " (!" + negp(g(m, 3)) + ") {"});
          ch = true;
          break;
        }
      }
    }
  }
  return lines;
}

// ------------------------------------------------------------------------------------ longs
namespace {
bool adjacent(const string &lo, const string &hi) {
  auto m = fm("g_([0-9a-f]+)", lo), n = fm("g_([0-9a-f]+)", hi);
  if (m && n) return int16(g(n, 1)) == int16(g(m, 1)) + 2;
  m = fm("v(\\d+)", lo), n = fm("v(\\d+)", hi);
  if (m && n) return int0(g(n, 1)) == int0(g(m, 1)) - 2;
  m = fm("(.+)->f(\\d+)", lo), n = fm("(.+)->f(\\d+)", hi);
  if (m && n) return g(m, 1) == g(n, 1) && int0(g(n, 2)) == int0(g(m, 2)) + 2;
  return false;
}
}  // namespace

std::string ec_fold_long_expr(const std::string &e) {
  string out;
  size_t i = 0;
  while (i < e.size()) {
    if (e.compare(i, 5, "LONG(") == 0 && (i == 0 || !(isalnum((unsigned char)e[i - 1]) || e[i - 1] == '_'))) {
      long long d = 0;
      size_t j = i + 5, cut = string::npos;
      while (j < e.size()) {
        if (e[j] == '(') d++;
        else if (e[j] == ')') {
          if (d == 0) break;
          d--;
        } else if (e[j] == ',' && d == 0 && cut == string::npos) cut = j;
        j++;
      }
      if (cut != string::npos && j < e.size()) {
        string hi = strip(e.substr(i + 5, cut - (i + 5))), lo = strip(e.substr(cut + 1, j - (cut + 1)));
        if (adjacent(lo, hi)) {
          out += "*(long *)&" + lo;
          i = j + 1;
          continue;
        }
        if (hi == "0") {
          out += "(long)" + (fm("[\\w>.\\-]+", lo) ? lo : "(" + lo + ")");
          i = j + 1;
          continue;
        }
      }
    }
    out += e[i];
    i++;
  }
  return out;
}

Lines ec_fold_longs(const Lines &lines) {
  Lines out;
  size_t i = 0;
  while (i < lines.size()) {
    string a = lines[i], b = i + 1 < lines.size() ? lines[i + 1] : "", c = i + 2 < lines.size() ? lines[i + 2] : "";
    auto m1 = fm("(\\s*)(.+?) = LO\\((.*)\\);", a), m2 = fm("(\\s*)(.+?) = HI\\((.*)\\);", b);
    bool hasE = false;
    string E, ind, lo;
    size_t n = 0;
    if (m1 && m2 && g(m1, 1) == g(m2, 1) && g(m1, 3) == g(m2, 3) && adjacent(g(m1, 2), g(m2, 2))) {
      E = g(m1, 3), n = 2, ind = g(m1, 1), lo = g(m1, 2), hasE = true;
    } else {
      auto m0 = fm("(\\s*)(t\\d+) = (.*LONG\\(.*);", a);
      m1 = fm("(\\s*)(.+?) = LO\\((t\\d+)\\);", b);
      m2 = fm("(\\s*)(.+?) = HI\\((t\\d+)\\);", c);
      if (m0 && m1 && m2 && g(m1, 3) == g(m2, 3) && g(m2, 3) == g(m0, 2) && adjacent(g(m1, 2), g(m2, 2))) {
        re::Regex wb("\\b" + g(m0, 2) + "\\b");
        bool later = false;
        for (size_t q = i + 3; q < lines.size(); q++)
          if (wb.search(lines[q])) later = true;
        if (!later) E = g(m0, 3), n = 3, ind = g(m0, 1), lo = g(m1, 2), hasE = true;
      }
    }
    if (!hasE) {
      out.push_back(a);
      i++;
      continue;
    }
    string e2 = ec_fold_long_expr(E), obj = "*(long *)&" + lo;
    if (starts(e2, (obj + " + ").c_str())) out.push_back(ind + obj + " += " + e2.substr(obj.size() + 3) + ";");
    else if (starts(e2, (obj + " - ").c_str())) out.push_back(ind + obj + " -= " + e2.substr(obj.size() + 3) + ";");
    else out.push_back(ind + obj + " = " + e2 + ";");
    i += n;
  }
  return out;
}

// ------------------------------------------------------------------------------------ ladders, ||
Lines ec_fold_ladders(Lines lines, bool mark) {
  struct PI {
    string cond;
    Lines then_;
    bool has_else = false;
    Lines else_;
    long long next = 0;
  };
  auto parse_if = [&](long long i, PI &r) -> bool {
    auto m = mt("( *)if \\((.*)\\) \\{$", at(lines, i));
    if (!m) return false;
    long long d = (long long)g(m, 1).size(), j = i + 1, L = (long long)lines.size();
    while (j < L && !(indent(lines[j]) == d && starts(strip(lines[j]), "}"))) j++;
    if (j >= L) return false;
    string t = strip(lines[j]);
    if (t == "}") {
      r = PI{g(m, 2), slice(lines, i + 1, j), false, {}, j + 1};
      return true;
    }
    if (t != "} else {") return false;
    long long k = j + 1;
    while (k < L && !(indent(lines[k]) == d && starts(strip(lines[k]), "}"))) k++;
    if (k >= L || strip(lines[k]) != "}") return false;
    r = PI{g(m, 2), slice(lines, i + 1, j), true, slice(lines, j + 1, k), k + 1};
    return true;
  };
  struct Rel {
    string a, op, b, sg;
  };
  auto rel = [&](const string &c0, Rel &r) -> bool {
    if (!(starts(c0, "(") && ends(c0, ")"))) return false;
    string c = c0.substr(1, c0.size() - 2);
    long long d = 0;
    for (size_t i = 0; i < c.size(); i++) {
      char ch = c[i];
      if (ch == '(') d++;
      else if (ch == ')') d--;
      else if (d == 0 && ch == ' ') {
        for (const char *op : {"<=", ">=", "<", ">"}) {
          string o = op;
          if (c.compare(i + 1, o.size() + 1, o + " ") == 0) {
            string a = c.substr(0, i), b = sl(c, (long long)(i + o.size() + 2));
            if (has(a, "&&") || has(a, "||") || has(b, "&&") || has(b, "||") || has(a + b, "S8(")) return false;
            auto ms = fm("S16\\((.*)\\)", a), mb = fm("S16\\((.*)\\)", b);
            if (ms && mb) {
              r = Rel{g(ms, 1), o, g(mb, 1), "s"};
              return true;
            }
            if (starts(a, "(unsigned)")) {
              r = Rel{a.substr(10), o, b, "u"};
              return true;
            }
            r = Rel{a, o, b, mark ? "u" : "s"};
            return true;
          }
        }
      }
    }
    return false;
  };
  auto same = [](const Lines &x, const Lines &y) {
    if (x.size() != y.size()) return false;
    for (size_t k = 0; k < x.size(); k++)
      if (strip(x[k]) != strip(y[k])) return false;
    return true;
  };
  long long i = 0;
  while (i < (long long)lines.size()) {
    PI O, In, Lw;
    Rel r1, r2, r3;
    bool okO = parse_if(i, O);
    if (!okO || !rel(O.cond, r1) || (r1.op != "<=" && r1.op != ">=") || O.then_.empty()) {
      i++;
      continue;
    }
    if (!parse_if(i + 1, In) || In.next != i + 1 + (long long)O.then_.size()) {
      i++;
      continue;
    }
    if (!rel(In.cond, r2) || r2.a != r1.a || r2.b != r1.b || r2.sg != r1.sg || std::set<string>{r1.op, r2.op} != std::set<string>{"<=", ">="}) {
      i++;
      continue;
    }
    if (!parse_if(i + 2, Lw) || Lw.next != i + 2 + (long long)In.then_.size()) {
      i++;
      continue;
    }
    if (!rel(Lw.cond, r3) || r3.sg != "u" || Lw.then_.empty()) {
      i++;
      continue;
    }
    const Lines &X_ = Lw.then_, &Y1 = Lw.else_;
    const Lines &lt_body = r1.op == "<=" ? In.else_ : O.else_;
    const Lines &gt_body = r1.op == "<=" ? O.else_ : In.else_;
    bool c_lt = r3.op == "<" || r3.op == "<=", c_gt = r3.op == ">" || r3.op == ">=";
    if (!same(lt_body, c_lt ? X_ : Y1) || !same(gt_body, c_gt ? X_ : Y1)) {
      i++;
      continue;
    }
    string d = spaces(indent(lines[i]));
    string w = r1.sg == "s" ? "S32" : "U32";
    string c;
    if (mark) c = "(" + w + "(LONG(" + r1.a + ", " + r3.a + ")) " + r3.op + " " + w + "(LONG(" + r1.b + ", " + r3.b + ")))";
    else
      c = ec_fold_long_expr("(" + string(r1.sg == "u" ? "(unsigned long)" : "") + "LONG(" + r1.a + ", " + r3.a + ") " + r3.op +
                            " LONG(" + r1.b + ", " + r3.b + "))");
    auto cut8 = [](const string &l) { return starts(l, "        ") ? l.substr(8) : l; };
    Lines nw = {d + "if (" + c + ") {"};
    for (auto &l : X_) nw.push_back(cut8(l));
    if (!Y1.empty()) {
      nw.push_back(d + "} else {");
      for (auto &l : Y1) nw.push_back(cut8(l));
    }
    nw.push_back(d + "}");
    splice(lines, i, O.next, nw);
  }
  return lines;
}

namespace {
string neg_cond(const string &c) {
  if (starts(c, "(") && ends(c, ")")) {
    string x = c.substr(1, c.size() - 2);
    long long d = 0;
    bool ok = true, has_at = false;
    size_t at_i = 0;
    string at_op;
    for (size_t i = 0; i < x.size(); i++) {
      char ch = x[i];
      if (ch == '(') d++;
      else if (ch == ')') {
        d--;
        if (d < 0) {
          ok = false;
          break;
        }
      } else if (d == 0 && ch == ' ') {
        for (const char *op : {"==", "!=", "<=", ">=", "<", ">", "&&", "||"}) {
          string o = op;
          if (x.compare(i + 1, o.size() + 1, o + " ") == 0) {
            if (o == "&&" || o == "||" || has_at) ok = false;
            else has_at = true, at_i = i, at_op = o;
          }
        }
      }
    }
    if (ok && d == 0 && has_at) {
      string flip = at_op == "==" ? "!=" : at_op == "!=" ? "==" : at_op == "<" ? ">=" : at_op == ">=" ? "<" : at_op == ">" ? "<=" : ">";
      return "(" + x.substr(0, at_i) + " " + flip + " " + sl(x, (long long)(at_i + at_op.size() + 2)) + ")";
    }
    if (ok && d == 0 && starts(x, "!") && starts(x.substr(1), "(") && ends(x, ")")) {
      long long dd = 0;
      size_t j = 0;
      string t = x.substr(1);
      for (j = 0; j < t.size(); j++) {
        dd += t[j] == '(';
        dd -= t[j] == ')';
        if (dd == 0) break;
      }
      if (j == t.size()) j = t.size() - 1;
      if ((long long)j == (long long)x.size() - 2) return t;
    }
  }
  return "(!(" + c + "))";
}
}  // namespace

Lines ec_fold_goto_or(Lines lines) {
  auto close = [&](long long i, long long d) -> long long {
    long long j = i + 1;
    while (j < (long long)lines.size() && !(indent(lines[j]) == d && starts(strip(lines[j]), "}"))) j++;
    return j < (long long)lines.size() ? j : -1;
  };
  auto top_or = [](const string &c) {
    long long d = 0;
    for (size_t i = 0; i < c.size(); i++) {
      d += c[i] == '(';
      d -= c[i] == ')';
      if (d == 0 && c.compare(i, 4, " || ") == 0) return true;
    }
    return false;
  };
  auto par = [&](const string &c) { return top_or(c) ? "(" + c + ")" : c; };
  auto others = [&](const string &L, long long skip) {
    long long n = 0;
    re::Regex rx("\\bgoto " + L + ";");
    for (long long q = 0; q < (long long)lines.size(); q++)
      if (q != skip && rx.search(lines[q])) n++;
    return n;
  };
  auto dedent = [](const Lines &xs) {
    Lines o;
    for (auto &x : xs) o.push_back(starts(x, "    ") ? x.substr(4) : x);
    return o;
  };
  bool changed = true;
  while (changed) {
    changed = false;
    for (long long i = 0; i < (long long)lines.size() - 3; i++) {
      auto m = mt("( *)((?:\\} else )?if) \\((.*)\\) \\{$", lines[i]);
      if (!m) continue;
      long long d = (long long)g(m, 1).size();
      string pad = g(m, 1);
      long long j = close(i, d);
      if (j < 0) continue;
      auto mq = mt("\\} else if \\((.*)\\) \\{$", strip(lines[j]));
      auto lab = mt("(\\w+):$", strip(lines[i + 1]));
      if (mq && lab && j + 2 < (long long)lines.size() && strip(lines[j + 1]) == "goto " + g(lab, 1) + ";" &&
          indent(lines[j + 2]) == d && starts(strip(lines[j + 2]), "}")) {
        string L = g(lab, 1);
        long long keep = others(L, j + 1);
        lines[i] = pad + g(m, 2) + " (" + g(m, 3) + " || " + g(mq, 1) + ") {";
        lines.erase(lines.begin() + j, lines.begin() + j + 2);
        if (!keep) lines.erase(lines.begin() + i + 1);
        changed = true;
        break;
      }
      if (strip(lines[j]) != "} else {" || j + 2 >= (long long)lines.size()) continue;
      auto gg = mt("goto (\\w+);$", strip(lines[j + 1]));
      if (!gg || strip(lines[j + 2]) != "}" || indent(lines[j + 2]) != d) continue;
      auto mi = mt("( *)if \\((.*)\\) \\{$", lines[i + 1]);
      if (!mi || (long long)g(mi, 1).size() != d + 4) continue;
      long long k = close(i + 1, d + 4);
      if (k < 0) continue;
      string t = strip(lines[k]);
      long long e_end;
      bool hasy = false;
      Lines ybody;
      if (t == "}") e_end = k;
      else if (t == "} else {") {
        e_end = close(k, d + 4);
        if (e_end < 0 || strip(lines[e_end]) != "}") continue;
        ybody = slice(lines, k + 1, e_end);
        hasy = true;
      } else continue;
      if (e_end + 1 != j) continue;
      string L = g(gg, 1);
      long long keep = others(L, j + 1);
      Lines xbody = slice(lines, i + 2, k);
      string cond;
      if (!xbody.empty() && strip(xbody[0]) == L + ":") {
        if (!keep) xbody.erase(xbody.begin());
        cond = neg_cond(g(m, 3)) + " || " + g(mi, 2);
      } else if (hasy && !ybody.empty() && strip(ybody[0]) == L + ":") {
        if (!keep) ybody.erase(ybody.begin());
        cond = par(g(m, 3)) + " && " + par(g(mi, 2));
      } else continue;
      Lines nw = {pad + g(m, 2) + " (" + cond + ") {"};
      for (auto &x : dedent(xbody)) nw.push_back(x);
      if (hasy) {
        nw.push_back(pad + "} else {");
        for (auto &x : dedent(ybody)) nw.push_back(x);
      }
      nw.push_back(pad + "}");
      splice(lines, i, j + 3, nw);
      changed = true;
      break;
    }
  }
  return lines;
}

Lines ec_fold_shortcircuit(Lines lines) {
  const re::Regex &guard = RX("^(\\s*)if \\(!\\((.*)\\)\\) goto (L_[0-9a-f]+);$");
  bool changed = true;
  while (changed) {
    changed = false;
    for (size_t i = 0; i < lines.size(); i++) {
      auto m = mt("^(\\s*)if \\((.*)\\) \\{$", lines[i]);
      if (!m) continue;
      string ind = g(m, 1), A = g(m, 2);
      re::Match gm;
      if (i + 1 < lines.size()) gm = guard.match(lines[i + 1]);
      if (!gm || g(gm, 1) != ind + "    ") continue;
      string B = g(gm, 2), L = g(gm, 3);
      long long je = -1;
      for (size_t j = i + 2; j < lines.size(); j++) {
        string s = rstrip(lines[j]);
        if (s == ind + "} else {") {
          je = (long long)j;
          break;
        }
        if (s == ind + "}") break;
      }
      if (je < 0 || je + 1 >= (long long)lines.size()) continue;
      if (strip(lines[je + 1]) != L + ":") continue;
      long long nl = 0, ng = 0;
      re::Regex gr("goto " + L + ";");
      for (auto &x : lines) {
        if (strip(x) == L + ":") nl++;
        if (gr.search(x)) ng++;
      }
      if (nl != 1 || ng != 1) continue;
      Lines nw(lines.begin(), lines.begin() + i);
      nw.push_back(ind + "if ((" + A + ") && (" + B + ")) {");
      nw.insert(nw.end(), lines.begin() + i + 2, lines.begin() + je + 1);
      nw.insert(nw.end(), lines.begin() + je + 2, lines.end());
      lines = nw;
      changed = true;
      break;
    }
  }
  return lines;
}

Lines ec_dead_store_recover(const Lines &stmts, const std::vector<bool> &rc) {
  auto is_used = [&](const string &V) {
    re::Regex rx("\\b" + re::escape(V) + "\\b");
    for (auto &t : stmts)
      if (has(t, " = ") && rx.search(splitn(t, " = ", 1)[1])) return true;
    return false;
  };
  Lines res;
  for (size_t idx = 0; idx < stmts.size(); idx++) {
    const string &s = stmts[idx];
    auto m = mt("(v\\d+) = (-?\\d+);$", s);
    if (idx < rc.size() && rc[idx] && m && !is_used(g(m, 1))) {
      string V = g(m, 1), Cn = g(m, 2);
      Lines hosts;
      for (auto &t : stmts) {
        auto mt_ = mt("(v\\d+) = ", t);
        if (mt_ && g(mt_, 1) != V && std::find(hosts.begin(), hosts.end(), g(mt_, 1)) == hosts.end()) hosts.push_back(g(mt_, 1));
      }
      if (hosts.size() == 1) {
        res.push_back(hosts[0] + " = " + Cn + ";");
        res.push_back(V + " = " + hosts[0] + ";");
        continue;
      }
    }
    res.push_back(s);
  }
  return res;
}

std::vector<std::pair<long long, long long>> ec_detect_copies(const Lines &stmts) {
  for (auto &s : stmts)
    if (has(s, "{") || has(s, "}")) return {};
  std::vector<std::pair<long long, long long>> parsed;   // (lval | NN, src | NN)
  for (auto &s : stmts) {
    auto m = mt("v(\\d+) = v(\\d+);$", strip(s));
    if (m) {
      parsed.push_back({int0(g(m, 1)), int0(g(m, 2))});
      continue;
    }
    auto m2 = mt("v(\\d+) =", strip(s));
    parsed.push_back({m2 ? int0(g(m2, 1)) : NN, NN});
  }
  std::vector<std::pair<long long, long long>> copies;
  for (size_t i = 0; i < parsed.size(); i++) {
    long long dest = parsed[i].first, src = parsed[i].second;
    if (src == NN) continue;
    bool init_before = false, over_after = false;
    for (size_t j = 0; j < i; j++)
      if (parsed[j].first == src) init_before = true;
    for (size_t j = i + 1; j < parsed.size(); j++)
      if (parsed[j].first == src) over_after = true;
    if (init_before && !over_after) copies.push_back({dest, src});
  }
  return copies;
}

namespace {
// early_c_frame: _chain_swaps, infer_decl_order, emit_local_decls, fmt_decl
std::vector<std::pair<long long, long long>> chain_swaps(const std::vector<std::pair<long long, long long>> &copies) {
  std::map<long long, long long> outg;
  std::set<long long> incoming;
  for (auto &c : copies) outg[c.second] = c.first, incoming.insert(c.first);
  std::vector<std::pair<long long, long long>> swaps;
  std::set<long long> seen;
  for (auto &c : copies) {
    long long src = c.second;
    if (incoming.count(src) || seen.count(src)) continue;
    long long cur = src;
    seen.insert(src);
    size_t guard = 0;
    while (outg.count(cur) && guard++ <= outg.size()) cur = outg[cur], seen.insert(cur);
    if (src != cur) swaps.push_back({src, cur});
  }
  return swaps;
}
}  // namespace

std::string ec_to_c(const std::string &name, long long fs, const std::vector<std::string> &params, const Lines &stmts,
                    const std::string &ptype, const std::vector<long long> &locals, const std::vector<long long> &longset) {
  string sig = params.empty() ? name + "()" : name + "(" + join(params, ", ") + ")";
  string decl = params.empty() ? "" : ptype + " " + join(params, ", ") + ";\n";
  std::set<long long> ls(longset.begin(), longset.end());
  std::set<long long> os(locals.begin(), locals.end());
  std::vector<long long> offs(os.begin(), os.end());
  auto copies = ec_detect_copies(stmts);
  string ldecl;
  auto fmt = [](const string &t, const string &n, long long c) {
    return c > 1 ? t + " " + n + "[" + D(c) + "];" : t + " " + n + ";";
  };
  if (!offs.empty()) {
    bool contiguous = true;
    for (size_t k = 0; k + 1 < offs.size(); k++)
      if (offs[k + 1] - offs[k] != (ls.count(offs[k]) ? 4 : 2)) contiguous = false;
    if (!copies.empty() && contiguous) {
      std::map<long long, long long> pos;
      for (auto o : offs) pos[o] = o;
      for (auto &s : chain_swaps(copies))
        if (pos.count(s.first) && pos.count(s.second)) std::swap(pos[s.first], pos[s.second]);
      std::vector<long long> order = offs;
      std::stable_sort(order.begin(), order.end(), [&](long long a, long long b) { return pos[a] < pos[b]; });
      for (auto o : order) ldecl += "    " + fmt(ls.count(o) ? "long" : "int", "v" + D(o), 1) + "\n";
    } else {
      for (size_t k = 0; k < offs.size(); k++) {
        long long o = offs[k];
        string t = ls.count(o) ? "long" : "int";
        long long sz = ls.count(o) ? 4 : 2;
        ldecl += "    " + fmt(t, "v" + D(o), 1) + "\n";
        long long nxt = k + 1 < offs.size() ? offs[k + 1] : fs;
        long long gap = nxt - o - sz;
        if (gap > 0) ldecl += "    " + fmt("char", "_p" + D(o), gap) + "\n";
      }
    }
  }
  Lines body;
  for (auto &s : stmts) body.push_back("    " + s);
  return sig + "\n" + decl + "{\n" + ldecl + join(body, "\n") + "\n}\n";
}

}  // namespace cbinv
