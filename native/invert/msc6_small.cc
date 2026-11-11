// msc6_small.cc -- the three byte-level Microsoft C 6.0a readers, each as its Python was:
// msc6_function.invert_cleanup (the long-global-free idiom, LZEXPAND's Cleanup), msc6_walk.invert
// (the symbol-map-driven statement walker) and msc6_templates.invert (the leaf templates T0-T6).
// They read bytes, not instructions; Python's slicing, its IndexError and its asserts are kept.
#include <algorithm>
#include <set>

#include "msc6.h"
#include "pyutil.h"

namespace cbinv {

namespace {

typedef std::vector<uint8_t> Bytes;

const uint8_t PRO[] = {0x1e, 0x58, 0x90, 0x45, 0x55, 0x8b, 0xec, 0x1e, 0x8e, 0xd8};
const uint8_t EPI[] = {0x4d, 0x4d, 0x8b, 0xe5, 0x1f, 0x5d, 0x4d};

int B(const Bytes &b, long long i) {   // b[i] (Python indexing, negative included)
  long long j = i < 0 ? i + (long long)b.size() : i;
  if (j < 0 || j >= (long long)b.size()) raise(PY_INDEX, "index out of range");
  return b[(size_t)j];
}
int U16(const Bytes &b, long long i) { return B(b, i) | (B(b, i + 1) << 8); }

Bytes S(const Bytes &b, long long a, std::optional<long long> e = std::nullopt) {   // b[a:e]
  long long n = (long long)b.size();
  auto fix = [&](long long x) { return x < 0 ? std::max<long long>(0, x + n) : std::min(x, n); };
  long long lo = fix(a), hi = e ? fix(*e) : n;
  if (hi < lo) hi = lo;
  return Bytes(b.begin() + lo, b.begin() + hi);
}
bool eq(const Bytes &b, long long a, std::initializer_list<int> x) {   // b[a:a+len(x)] == x
  Bytes s = S(b, a, a + (long long)x.size());
  if (s.size() != x.size()) return false;
  size_t k = 0;
  for (int v : x)
    if (s[k++] != v) return false;
  return true;
}
bool eq_epi(const Bytes &b, long long a) {
  Bytes s = S(b, a, a + 7);
  return s.size() == 7 && std::equal(s.begin(), s.end(), EPI);
}
bool starts_pro(const Bytes &b) { return b.size() >= 10 && std::equal(PRO, PRO + 10, b.begin()); }
long long rfind_epi(const Bytes &b) {
  for (long long i = (long long)b.size() - 7; i >= 0; i--)
    if (std::equal(EPI, EPI + 7, b.begin() + i)) return i;
  return -1;
}
long long find_epi(const Bytes &b, long long from) {
  for (long long i = from; i + 7 <= (long long)b.size(); i++)
    if (std::equal(EPI, EPI + 7, b.begin() + i)) return i;
  return -1;
}
std::string hexs_b(const Bytes &b) {
  static const char *D = "0123456789abcdef";
  std::string s;
  for (auto x : b) s += D[x >> 4], s += D[x & 15];
  return s;
}
std::string sgn16(int v) { return std::to_string(v >= 0x8000 ? v - 0x10000 : v); }
std::string arg_a(int byte) { return "a" + std::to_string(floordiv(byte - 6, 2)); }
std::string jlist(const std::vector<std::string> &v) {
  std::string s = "[";
  for (size_t k = 0; k < v.size(); k++) s += (k ? ", " : "") + jstr(v[k]);
  return s + "]";
}

// ---- msc6_walk ----

typedef std::map<long long, std::pair<std::string, std::string>> Syms;

struct Decl {
  ODict globs;
  bool imports = false;
};

struct Pushes {
  std::vector<std::string> args;
  long long p;
};

Pushes pushes(const Bytes &b, long long p, long long end, const Syms &syms, Decl &d) {
  std::vector<std::string> args;
  while (p < end) {
    int c = B(b, p);
    if (c == 0x6a) {
      int v = B(b, p + 1);
      args.push_back(std::to_string(v >= 0x80 ? v - 0x100 : v));
      p += 2;
    } else if (c == 0x68) {
      args.push_back(std::to_string(U16(b, p + 1)));
      p += 3;
    } else if (eq(b, p, {0xff, 0x76})) {
      args.push_back(arg_a(B(b, p + 2)));
      p += 3;
    } else if (eq(b, p, {0xff, 0x36})) {
      long long off = U16(b, p + 2);
      auto nm = syms.find(off), lo = syms.find(off - 2);
      if (nm != syms.end() && nm->second.second == "long") {
        d.globs.set(nm->second.first, "long");
        args.push_back("(WORD)" + nm->second.first);
      } else if (lo != syms.end() && lo->second.second == "long") {
        d.globs.set(lo->second.first, "long");
        args.push_back("(WORD)(" + lo->second.first + " >> 16)");
      } else if (nm != syms.end()) {
        d.globs.set(nm->second.first, "word");
        args.push_back(nm->second.first);
      } else {
        std::string g = "gW" + hexs(off);
        d.globs.set(g, "word");
        args.push_back(g);
      }
      p += 4;
    } else {
      break;
    }
  }
  return {args, p};
}

struct WalkR {
  std::optional<std::vector<std::string>> lines;
  long long p = 0;
  std::string why;
};

WalkR walk(const Bytes &b, long long p, long long end, const Syms &syms, Decl &d);

WalkR stmt(const Bytes &b, long long p, long long end, const Syms &syms, Decl &d) {
  long long q0 = p;
  Pushes ps = pushes(b, p, end, syms, d);
  long long q = ps.p;
  if (q < end && B(b, q) == 0x9a) {
    d.imports = true;
    std::string a;
    for (size_t k = 0; k < ps.args.size(); k++) a += (k ? ", " : "") + ps.args[k];
    return {std::vector<std::string>{"CALLEE(" + a + ");"}, q + 5, ""};
  }
  if (eq(b, p, {0x2b, 0xc0}) && B(b, p + 2) == 0xa3 && B(b, p + 5) == 0xa3) {
    long long lo = U16(b, p + 6);
    auto nm = syms.find(lo);
    if (nm != syms.end() && nm->second.second == "long") {
      d.globs.set(nm->second.first, "long");
      return {std::vector<std::string>{nm->second.first + " = 0L;"}, p + 8, ""};
    }
  }
  if (eq(b, p, {0xc7, 0x06})) {
    long long off = U16(b, p + 2);
    int imm = U16(b, p + 4);
    auto nm = syms.find(off);
    std::string name = nm != syms.end() ? nm->second.first : "gW" + hexs(off);
    d.globs.set(name, "word");
    return {std::vector<std::string>{name + " = " + std::to_string(imm) + ";"}, p + 6, ""};
  }
  if (eq(b, p, {0xb8}) && eq_epi(b, p + 3)) return {std::vector<std::string>{"return " + sgn16(U16(b, p + 1)) + ";"}, p + 3, ""};
  if (eq(b, p, {0x8b, 0x46}) && eq_epi(b, p + 3)) return {std::vector<std::string>{"return " + arg_a(B(b, p + 2)) + ";"}, p + 3, ""};
  if (eq(b, p, {0xa1}) && eq(b, p + 3, {0x0b, 0x06}) && B(b, p + 7) == 0x74) {
    long long lo = U16(b, p + 5);
    auto nm = syms.find(lo);
    if (nm != syms.end() && nm->second.second == "long") {
      d.globs.set(nm->second.first, "long");
      long long skip = p + 9 + B(b, p + 8);
      WalkR t = walk(b, p + 9, skip, syms, d);
      if (!t.lines) return {std::nullopt, p, "then-block unmodeled"};
      std::vector<std::string> out = {"if (" + nm->second.first + " != 0L) {"};
      for (auto &x : *t.lines) out.push_back("    " + x);
      out.push_back("}");
      return {out, skip, ""};
    }
  }
  return {std::nullopt, p, "unmodeled @+" + std::to_string(p - q0) + ": " + hexs_b(S(b, p, p + 6))};
}

WalkR walk(const Bytes &b, long long p, long long end, const Syms &syms, Decl &d) {
  std::vector<std::string> out;
  while (p < end) {
    WalkR r = stmt(b, p, end, syms, d);
    if (!r.lines) return {std::nullopt, 0, r.why};
    out.insert(out.end(), r.lines->begin(), r.lines->end());
    p = r.p;
  }
  return {out, p, ""};
}

// ---- msc6_templates ----

bool jcc_inv_t(int op, std::string &cc) {
  static const std::map<int, const char *> J = {{0x74, "!="}, {0x75, "=="}, {0x7c, ">="}, {0x7d, "<"}, {0x7e, ">"},
                                                {0x7f, "<="}, {0x72, ">="}, {0x73, "<"}, {0x76, ">"}, {0x77, "<="}};
  auto it = J.find(op);
  if (it == J.end()) return false;
  cc = it->second;
  return true;
}

std::optional<std::string> leaf_stmt(const Bytes &bb) {
  if (eq(bb, 0, {0xb8}) && bb.size() == 3) return "return " + sgn16(bb[1] | (bb[2] << 8)) + ";";
  if (eq(bb, 0, {0x8b, 0x46}) && bb.size() == 3) return "return " + arg_a(bb[2]) + ";";
  if (eq(bb, 0, {0xa1}) && bb.size() == 3) return std::string("return gG;");
  return std::nullopt;
}

std::optional<std::vector<std::string>> recover(const Bytes &b) {
  std::string cc;
  if (eq(b, 0, {0x83, 0x7e}) && b.size() >= 5 && jcc_inv_t(b[4], cc)) {
    std::string lval = arg_a(b[2]);
    int val = b[3] < 0x80 ? b[3] : b[3] - 0x100;
    int rel = B(b, 5);
    long long skip_off = 6 + (rel >= 0x80 ? rel - 0x100 : rel);
    Bytes then = S(b, 6, skip_off), then_body, rest;
    if (eq(S(then, -2, -1), 0, {0xeb}) && S(then, -2, -1).size() == 1) {
      then_body = S(then, 0, -2);
    } else {
      then_body = then;
    }
    rest = S(b, skip_off);
    auto ts = leaf_stmt(then_body);
    if (!ts) return std::nullopt;
    std::vector<std::string> out = {"if (" + lval + " " + cc + " " + std::to_string(val) + ")", "    " + *ts};
    if (!rest.empty()) {
      auto rs = recover(rest);
      if (!rs) return std::nullopt;
      out.insert(out.end(), rs->begin(), rs->end());
    }
    return out;
  }
  auto ls = leaf_stmt(b);
  if (ls && !ls->empty()) return std::vector<std::string>{*ls};
  return std::nullopt;
}

std::string params_a(long long n) {
  std::string s;
  for (long long i = 0; i < n; i++) s += (i ? ", int a" : "int a") + std::to_string(i);
  return s.empty() ? "void" : s;
}

std::string declined(const std::string &why) { return "{\"t\": null, \"w\": " + jstr(why) + "}"; }

}  // namespace

std::string msc6_cleanup(const Bytes &b, const std::map<long long, std::string> &syms) {
  if (!starts_pro(b)) raise(PY_ASSERT, "");
  long long p = 10;
  if (eq(b, p, {0x83, 0xec})) p += 3;
  std::vector<std::string> lines, used;
  for (;;) {
    if (eq(b, p, {0xa1}) && eq(b, p + 3, {0x0b, 0x06}) && B(b, p + 7) == 0x74) {
      long long lo = U16(b, p + 5);
      (void)U16(b, p + 1);
      auto it = syms.find(lo);
      std::string name = it != syms.end() ? it->second : "g_" + hexs(lo);
      used.push_back(name);
      long long q = p + 9;
      if (!eq(b, q, {0xff, 0x36})) raise(PY_ASSERT, "");
      q += 4;
      if (!eq(b, q, {0x9a})) raise(PY_ASSERT, "");
      q += 5;
      if (!eq(b, q, {0x2b, 0xc0})) raise(PY_ASSERT, "");
      q += 2;
      if (!eq(b, q, {0xa3})) raise(PY_ASSERT, "");
      q += 3;
      if (!eq(b, q, {0xa3})) raise(PY_ASSERT, "");
      q += 3;
      lines.push_back("if (" + name + " != 0L) {");
      lines.push_back("    GlobalFree((WORD)(" + name + " >> 16));");
      lines.push_back("    " + name + " = 0L;");
      lines.push_back("}");
      p = q;
      continue;
    }
    break;
  }
  std::string decls = "typedef unsigned int WORD;\nextern WORD far pascal GlobalFree(WORD);\n";
  for (auto &n : used) decls += "extern long " + n + ";\n";
  std::string body;
  for (auto &l : lines) body += "    " + l + "\n";
  return jstr(decls + "\nvoid far Cleanup(void)\n{\n" + body + "}");
}

std::string msc6_walk(const Bytes &b, const Syms &syms, const std::string &callee) {
  if (!starts_pro(b)) return declined("prologue miss");
  long long p = 10;
  if (eq(b, p, {0x83, 0xec})) p += 3;
  long long ei = rfind_epi(b);
  if (ei < 0) return declined("epilogue miss");
  Decl d;
  WalkR r = walk(b, p, ei, syms, d);
  if (!r.lines) return declined(r.why);
  std::string decls = "typedef unsigned int WORD;\n";
  if (d.imports) decls += "extern WORD far pascal " + callee + "(WORD);\n";
  for (auto &g : d.globs.keys) decls += "extern " + std::string(d.globs.v[g] == "long" ? "long" : "int") + " " + g + ";\n";
  std::string body;
  for (auto &l : *r.lines) body += "    " + replace_all(l, "CALLEE", callee) + "\n";
  std::string m = "{\"stmts\": " + std::to_string(r.lines->size()) + ", \"globals\": {";
  for (size_t k = 0; k < d.globs.keys.size(); k++) m += (k ? ", " : "") + jstr(d.globs.keys[k]) + ": " + jstr(d.globs.v[d.globs.keys[k]]);
  return "{\"t\": " + jstr(decls + "\nvoid far F(void)\n{\n" + body + "}") + ", \"m\": " + m + "}}}";
}

std::string msc6_templates(const Bytes &b) {
  if (!starts_pro(b)) return declined("prologue not the /Gsw loadds template (T0 miss)");
  long long p = 10;
  int frame = 0;
  if (eq(b, p, {0x83, 0xec})) {
    frame = B(b, p + 2);
    p += 3;
  }
  long long ei = find_epi(b, p);
  if (ei < 0) return declined("epilogue (T0 tail) not found");
  Bytes body = S(b, p, ei);
  long long q = ei + 7, argbytes;
  if (eq(b, q, {0xca})) {
    argbytes = B(b, q + 1) | (B(b, q + 2) << 8);
    q += 3;
  } else if (eq(b, q, {0xcb})) {
    argbytes = 0;
    q += 1;
  } else {
    return declined("no far return (ca/cb) after epilogue");
  }
  bool trailing_nop = q < (long long)b.size() && b[(size_t)q] == 0x90;
  long long nargs = floordiv(argbytes, 2);
  ODict globals_used;
  std::optional<std::string> st;
  std::string rtype = "int";
  bool unused_local = false;
  std::string cc;
  if (body.empty()) {
    if (frame && trailing_nop) unused_local = true;
  } else if (eq(body, 0, {0xb8}) && body.size() == 3) {
    st = "return " + sgn16(body[1] | (body[2] << 8)) + ";";
  } else if (eq(body, 0, {0x8b, 0x46}) && body.size() == 3) {
    st = "return " + arg_a(body[2]) + ";";
  } else if (eq(body, 0, {0xa1}) && body.size() == 3) {
    st = "return gG;";
    globals_used.set("gG", "int");
  } else if (eq(body, 0, {0xc7, 0x06}) && body.size() == 6) {
    st = "gG = " + std::to_string(body[4] | (body[5] << 8)) + ";";
    globals_used.set("gG", "int");
    rtype = "void";
  } else if (std::find(body.begin(), body.end(), 0x9a) != body.end()) {
    long long callidx = std::find(body.begin(), body.end(), 0x9a) - body.begin();
    if ((long long)body.size() - callidx != 5) return declined("T5: code after the CALLF not modeled (" + hexs_b(body) + ")");
    std::vector<std::string> ps;
    long long i = 0;
    bool ok = true;
    while (i < callidx) {
      if (body[(size_t)i] == 0x6a) {
        int v = B(body, i + 1);
        ps.push_back(std::to_string(v >= 0x80 ? v - 0x100 : v));
        i += 2;
      } else if (body[(size_t)i] == 0x68) {
        ps.push_back(std::to_string(B(body, i + 1) | (B(body, i + 2) << 8)));
        i += 3;
      } else if (eq(body, i, {0xff, 0x76})) {
        ps.push_back(arg_a(B(body, i + 2)));
        i += 3;
      } else {
        ok = false;
        break;
      }
    }
    if (!ok) return declined("T5: arg-push form not modeled (" + hexs_b(S(body, 0, callidx)) + ")");
    std::string ints;
    for (size_t k = 0; k < ps.size(); k++) ints += k ? ", int" : "int";
    std::string a;
    for (size_t k = 0; k < ps.size(); k++) a += (k ? ", " : "") + ps[k];
    std::string src = "extern void far pascal G(" + (ints.empty() ? std::string("void") : ints) + ");\nvoid far pascal F(" +
                      params_a(nargs) + ")\n{\n    G(" + a + ");\n}";
    return "{\"t\": " + jstr(src) + ", \"m\": {\"nargs\": " + std::to_string(nargs) + ", \"call_args\": " + jlist(ps) +
           ", \"template\": \"T5 far-call\"}}";
  } else if (eq(body, 0, {0x83, 0x7e}) && body.size() >= 5 && jcc_inv_t(body[4], cc)) {
    auto lines = recover(body);
    if (!lines) return declined("T6: branch structure not fully modeled (" + hexs_b(body) + ")");
    std::string inner;
    for (auto &l : *lines) inner += "    " + l + "\n";
    return "{\"t\": " + jstr("int far pascal F(" + params_a(nargs) + ")\n{\n" + inner + "}") + ", \"m\": {\"nargs\": " +
           std::to_string(nargs) + ", \"template\": \"T6 if-branch\", \"lines\": " + jlist(*lines) + "}}";
  } else {
    return declined("body not a modeled leaf template (" + std::to_string(body.size()) + " bytes: " + hexs_b(body) + ")");
  }
  std::string decls;
  for (auto &g : globals_used.keys) decls += globals_used.v[g] + " " + g + ";\n";
  std::string inner = unused_local ? "    int i;\n" : (st && !st->empty() ? "    " + *st + "\n" : "");
  std::string src = decls + rtype + " far pascal F(" + params_a(nargs) + ")\n{\n" + inner + "}";
  std::string tmpl = st && !st->empty() ? *st : (unused_local ? "int i" : "empty");
  return "{\"t\": " + jstr(src) + ", \"m\": {\"nargs\": " + std::to_string(nargs) + ", \"frame\": " + std::to_string(frame) +
         ", \"trailing_nop\": " + (trailing_nop ? "true" : "false") + ", \"rtype\": " + jstr(rtype) +
         ", \"globals\": " + jlist(globals_used.keys) + ", \"template\": " + jstr(tmpl) + "}}";
}

}  // namespace cbinv
