// msc6.cc -- invert/msc6.py in C++, decision for decision: the Microsoft C 6.0a (and Visual C++
// 1.52c frame) model that reads a 16-bit function back into the C it was compiled from -- register
// values, branches, do-while loops, short-circuit and materialised conditions, calls (far, near,
// cdecl, COM vtable), far pointers, longs, globals and stack locals -- declining at the first
// instruction it does not model.  The operand text is Capstone's (cs.h), matched as msc6.py's
// regular expressions match it (pyutil.h).
#include "msc6.h"

#include <algorithm>
#include <cstring>
#include <set>

#include "cs.h"
#include "pyutil.h"

namespace cbinv {

namespace {

typedef std::vector<uint8_t> Bytes;
typedef std::map<std::string, std::string> Reg;
typedef std::optional<std::vector<std::string>> Lines;

Bytes hexb(const char *h) {
  Bytes b;
  for (size_t i = 0; h[i] && h[i + 1]; i += 2) {
    auto d = [](char c) { return isd(c) ? c - '0' : c - 'a' + 10; };
    b.push_back((uint8_t)(d(h[i]) * 16 + d(h[i + 1])));
  }
  return b;
}

const Bytes PROLOGUE = hexb("1e589045558bec1e8ed8");
const Bytes EPI = hexb("4d4d8be51f5d4d");
const Bytes PROLOGUE_MOVDS = hexb("8cd89045558bec1e8ed8");
const Bytes EPI_LEA = hexb("8d66fe1f5d4d");
const Bytes PROLOGUE_NEAR = hexb("558bec1e8ed8");
const Bytes EPI_NEAR_LEA = hexb("8d66fe1f5d");
const Bytes EPI_NEAR_MOV = hexb("8be51f5d");
const Bytes EPI_SUBBP = hexb("83ed028be51f5d4d");

bool startswith(const Bytes &b, const Bytes &p) { return b.size() >= p.size() && std::equal(p.begin(), p.end(), b.begin()); }

long long find(const Bytes &b, const Bytes &p, long long from = 0, long long to = -1) {
  long long n = to < 0 ? (long long)b.size() : std::min<long long>(to, (long long)b.size());
  for (long long i = std::max<long long>(from, 0); i + (long long)p.size() <= n; i++)
    if (std::equal(p.begin(), p.end(), b.begin() + i)) return i;
  return -1;
}

long long rfind(const Bytes &b, const Bytes &p, long long from = 0, long long to = -1) {
  long long n = to < 0 ? (long long)b.size() : std::min<long long>(to, (long long)b.size());
  for (long long i = n - (long long)p.size(); i >= std::max<long long>(from, 0); i--)
    if (std::equal(p.begin(), p.end(), b.begin() + i)) return i;
  return -1;
}

int at(const Bytes &b, long long i) {   // b[i], IndexError past the end
  if (i < 0 || i >= (long long)b.size()) raise(PY_INDEX, "index out of range");
  return b[(size_t)i];
}

int byte_or(const Bytes &b, long long i) {   // b[i:i+1] as one byte, -1 when empty
  return (i >= 0 && i < (long long)b.size()) ? b[(size_t)i] : -1;
}

int jcc_inv(int op, std::string &out) {
  switch (op) {
    case 0x74: out = "!="; return 1;
    case 0x75: out = "=="; return 1;
    case 0x7c: out = ">="; return 1;
    case 0x7d: out = "<"; return 1;
    case 0x7e: out = ">"; return 1;
    case 0x7f: out = "<="; return 1;
  }
  return 0;
}
std::string JCC_INV(int op) {
  std::string s;
  if (!jcc_inv(op, s)) raise(PY_KEY, std::to_string(op));
  return s;
}
std::string JCC_DIRECT(int op) {
  switch (op) {
    case 0x75: return "!= 0";
    case 0x74: return "== 0";
    case 0x7c: return "<";
    case 0x7d: return ">=";
    case 0x7e: return "<=";
    case 0x7f: return ">";
  }
  raise(PY_KEY, std::to_string(op));
}
int JCC_OPP_get(int op) {
  switch (op) {
    case 0x74: return 0x75;
    case 0x75: return 0x74;
    case 0x7c: return 0x7d;
    case 0x7d: return 0x7c;
    case 0x7e: return 0x7f;
    case 0x7f: return 0x7e;
  }
  return op;
}
int JCC_OPP(int op) {
  int r = JCC_OPP_get(op);
  if (r == op) raise(PY_KEY, std::to_string(op));
  return r;
}
bool in_jcc_set(const std::string &m) {
  return m == "je" || m == "jne" || m == "jl" || m == "jge" || m == "jle" || m == "jg";
}
const char *alu2(const std::string &m) {
  if (m == "add") return "+";
  if (m == "sub") return "-";
  if (m == "and") return "&";
  if (m == "or") return "|";
  if (m == "xor") return "^";
  if (m == "shl") return "<<";
  if (m == "shr") return ">>";
  if (m == "sar") return ">>";
  if (m == "imul") return "*";
  return nullptr;
}
bool is_wreg(const std::string &r) {
  return r == "ax" || r == "bx" || r == "cx" || r == "dx" || r == "si" || r == "di";
}
bool is_rawreg(const std::string &r) {
  static const std::set<std::string> R = {"ax", "bx", "cx", "dx", "si", "di", "bp", "sp",
                                          "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh"};
  return R.count(r) != 0;
}

std::string param_name(long long off) { return chr('a' + floordiv(off - 6, 2)); }

int u16le(const Bytes &op, int i) { return at(op, i) | (at(op, i + 1) << 8); }

struct Layout {
  std::map<long long, std::pair<std::string, std::string>> lanes;
  std::map<long long, std::string> pnames, longs;
};

Layout layout_params(const std::optional<Params> &params) {
  Layout L;
  long long off = 6;
  if (!params) return L;
  for (auto it = params->rbegin(); it != params->rend(); ++it) {
    const std::string &nm = it->first, &ty = it->second;
    if (ty == "far") {
      L.lanes[off] = {nm, "lo"};
      L.lanes[off + 2] = {nm, "hi"};
      off += 4;
    } else if (ty == "long") {
      L.longs[off] = nm;
      off += 4;
    } else {
      L.pnames[off] = nm;
      off += 2;
    }
  }
  return L;
}

const std::string *get(const Reg &r, const std::string &k) {
  auto it = r.find(k);
  return it == r.end() ? nullptr : &it->second;
}
bool truthy(const Reg &r, const std::string &k) {
  auto p = get(r, k);
  return p && !p->empty();
}

struct FarMem {
  std::string expr;
  bool byte;
};

struct Dec {
  const std::vector<CsIns> &md;
  long long end;
  const std::map<long long, std::string> &imports;
  const std::map<long long, std::pair<std::string, std::string>> &syms;
  const std::map<long long, std::string> &nearmap;
  long long func_base;
  bool has_nearcall = false;
  ODict nears_used;
  std::set<std::string> cdecl_imports;
  Layout L;
  std::map<long long, long long> a2i;
  long long calls = 0;
  std::map<std::string, std::string> regvars;
  std::vector<std::string> decl_order;
  ODict globs;
  std::map<std::string, std::string> farty;
  ODict localdecl;
  std::map<std::string, std::string> farptr, vtbl;
  bool uses_vtbl = false;
  std::map<long long, std::pair<long long, int>> loops;
  std::map<long long, std::string> counter;
  std::vector<long long> counter_order;   // the counter dict's insertion order
  std::map<std::string, std::string> cname;
  std::set<long long> active;
  std::set<long long> memoff;
  bool is_void = false;
  std::string rtype;
  std::vector<std::string> args;
  std::string fail = "?";
  std::string esbx = "p";
  std::optional<std::string> pendfar, skip_push;

  Dec(const std::vector<CsIns> &m, long long endaddr, const std::map<long long, std::string> &imp,
      const std::map<long long, std::pair<std::string, std::string>> &sy, const std::optional<Params> &params,
      const std::map<long long, std::string> &nm, long long fb)
      : md(m), end(endaddr), imports(imp), syms(sy), nearmap(nm), func_base(fb) {
    L = layout_params(params);
    for (size_t i = 0; i < md.size(); i++) a2i[md[i].address] = (long long)i;
    for (size_t i = 0; i < md.size(); i++) {
      const CsIns &ins = md[i];
      if (in_jcc_set(ins.mnemonic) && starts(ins.op_str, "0x")) {
        auto t = a2i.find(int16(ins.op_str));
        if (t != a2i.end() && t->second <= (long long)i) {
          loops[t->second] = {(long long)i, ins.bytes[0]};
          for (long long k = t->second; k < (long long)i; k++)
            if ((md[k].mnemonic == "dec" || md[k].mnemonic == "inc") && (md[k].op_str == "si" || md[k].op_str == "di")) {
              if (!counter.count(t->second)) counter_order.push_back(t->second);
              counter[t->second] = md[k].op_str;
            }
        }
      }
    }
    for (long long t : counter_order) {
      const std::string &reg = counter[t];
      if (!cname.count(reg)) cname[reg] = chr('i' + (long long)cname.size());
    }
  }

  long long N() const { return (long long)md.size(); }

  const CsIns &M(long long i) const {   // md[i], Python's negative indexing included
    long long j = i < 0 ? i + N() : i;
    if (j < 0 || j >= N()) raise(PY_INDEX, "list index out of range");
    return md[(size_t)j];
  }

  std::optional<long long> ti(long long addr) const {
    auto j = a2i.find(addr);
    if (j == a2i.end()) {
      if (addr >= end) return N();
      return std::nullopt;
    }
    return j->second;
  }

  long long resolve_jmp(long long addr) const {
    std::set<long long> seen;
    while (!seen.count(addr)) {
      seen.insert(addr);
      auto t = a2i.find(addr);
      if (t == a2i.end() || t->second >= N()) return addr;
      const CsIns &ins = md[(size_t)t->second];
      if (ins.mnemonic == "jmp" && starts(ins.op_str, "0x")) {
        addr = int16(ins.op_str);
        continue;
      }
      return addr;
    }
    return addr;
  }

  std::optional<std::string> cond_of(const CsIns &test, int jcc_op, Reg &reg) {
    std::string cc = JCC_INV(jcc_op);
    const std::string &m = test.mnemonic, &ops = test.op_str;
    if (m == "or" && has(ops, ",")) {
      auto rr = split2(ops, ", ");
      if (rr.first == rr.second) return val(rr.first, reg) + " " + cc + " 0";
      std::string v1 = get(reg, rr.first) ? *get(reg, rr.first) : "";
      std::string v2 = get(reg, rr.second) ? *get(reg, rr.second) : "";
      if (has(v1, "#") && has(v2, "#") && split(v1, "#")[0] == split(v2, "#")[0])
        return split(v1, "#")[0] + " " + cc + " 0";
    }
    if (m == "cmp") {
      auto lr = split2(ops, ", ");
      std::string a = val(lr.first, reg);
      return a + " " + cc + " " + val(lr.second, reg);
    }
    return std::nullopt;
  }

  struct SC {
    std::string cond;
    long long join, exit_i;
  };

  std::optional<SC> shortcircuit(long long i, long long hi, Reg &reg) {
    struct Pair {
      long long ci;
      int jop;
      long long t;
    };
    std::vector<Pair> pairs;
    long long idx = i;
    while (idx + 1 < hi && (M(idx).mnemonic == "cmp" || M(idx).mnemonic == "or" || M(idx).mnemonic == "test") &&
           in_jcc_set(M(idx + 1).mnemonic)) {
      auto tgt = ti(int16(M(idx + 1).op_str));
      if (!tgt || *tgt <= idx) break;
      pairs.push_back({idx, M(idx + 1).bytes[0], *tgt});
      idx += 2;
    }
    if (pairs.size() < 3) return std::nullopt;
    long long join = idx;
    std::set<long long> exits;
    for (auto &p : pairs)
      if (p.t > join) exits.insert(p.t);
    if (exits.size() != 1 || join >= hi) return std::nullopt;
    long long exit_i = *exits.begin();
    std::set<long long> startset;
    for (auto &p : pairs) startset.insert(p.ci);
    for (auto &p : pairs)
      if (p.t != join && p.t != exit_i && !startset.count(p.t)) return std::nullopt;
    std::vector<std::vector<std::string>> terms;
    std::vector<std::string> cur;
    for (auto &p : pairs) {
      if (p.t == join) {
        auto c = cond_of(M(p.ci), JCC_OPP_get(p.jop), reg);
        if (!c) return std::nullopt;
        cur.push_back(*c);
        terms.push_back(cur);
        cur.clear();
      } else {
        auto c = cond_of(M(p.ci), p.jop, reg);
        if (!c) return std::nullopt;
        cur.push_back(*c);
      }
    }
    if (!cur.empty()) terms.push_back(cur);
    if (terms.size() < 2) return std::nullopt;
    std::string cond;
    for (size_t k = 0; k < terms.size(); k++) {
      if (k) cond += " || ";
      cond += "(";
      for (size_t j = 0; j < terms[k].size(); j++) cond += (j ? " && " : "") + terms[k][j];
      cond += ")";
    }
    return SC{cond, join, exit_i};
  }

  struct BM {
    std::string R, expr;
    long long J;
  };

  std::optional<BM> bool_materialize(long long i, long long hi, Reg &reg) {
    std::vector<std::pair<long long, int>> pairs;
    long long idx = i;
    while (idx + 1 < hi && M(idx).mnemonic == "cmp" && ends(M(idx).op_str, ", 0") && M(idx + 1).mnemonic == "jne") {
      auto t = ti(int16(M(idx + 1).op_str));
      if (!t || *t <= idx) break;
      pairs.push_back({idx, M(idx + 1).bytes[0]});
      idx += 2;
    }
    if (pairs.empty()) return std::nullopt;
    auto T = ti(int16(M(pairs[0].first + 1).op_str));
    for (auto &p : pairs)
      if (ti(int16(M(p.first + 1).op_str)) != T) return std::nullopt;
    if (idx + 1 >= hi || M(idx).mnemonic != "sub") return std::nullopt;
    auto ab = partition(M(idx).op_str, ", ");
    if (ab.second.empty() || ab.first != ab.second || !is_wreg(ab.first)) return std::nullopt;
    std::string R = ab.first;
    if (M(idx + 1).mnemonic != "jmp") return std::nullopt;
    auto J = ti(int16(M(idx + 1).op_str));
    if (!J || !T || *T >= N()) return std::nullopt;
    if (M(*T).mnemonic != "mov" || M(*T).op_str != R + ", 1") return std::nullopt;
    if (*T + 1 != *J) return std::nullopt;
    std::vector<std::string> terms;
    for (auto &p : pairs) {
      auto c = cond_of(M(p.first), JCC_OPP(p.second), reg);
      if (!c) return std::nullopt;
      terms.push_back(*c);
    }
    std::string e = "(";
    for (size_t k = 0; k < terms.size(); k++) e += (k ? " || " : "") + terms[k];
    return BM{R, e + ")", *J};
  }

  std::string pname(long long off) const {
    auto p = L.pnames.find(off);
    if (p != L.pnames.end()) return p->second;
    auto l = L.lanes.find(off);
    if (l != L.lanes.end()) return l->second.first;
    return param_name(off);
  }

  std::string local(long long off, bool byte = false) {
    std::string nm = "loc_" + hexs(off);
    if (!localdecl.count(nm)) localdecl.set(nm, byte ? "char" : "int");
    return nm;
  }

  // re.match(r"(word|byte) ptr \w+:\[(\w+)(?:\s*\+\s*(\w+))?\]$", tok)
  std::optional<FarMem> farmem(const std::string &tok) {
    Cur c(tok);
    std::string wty, seg, base, disp;
    if (c.lit("word ptr ")) {
      wty = "word";
    } else if (c.lit("byte ptr ")) {
      wty = "byte";
    } else {
      return std::nullopt;
    }
    if (!c.word(seg) || !c.lit(":[") || !c.word(base)) return std::nullopt;
    size_t save = c.i;
    bool got = false;
    {
      c.ws();
      if (c.lit("+")) {
        c.ws();
        std::string w;
        if (c.word(w) && c.lit("]") && c.end()) {
          disp = w;
          got = true;
        }
      }
    }
    if (!got) {
      c.i = save;
      if (!(c.lit("]") && c.end())) return std::nullopt;
    }
    auto p = farptr.find(base);
    if (p == farptr.end()) return std::nullopt;
    std::string cty = wty == "byte" ? "char" : "WORD";
    long long d = disp.empty() ? 0 : int0(disp);
    std::string expr = d ? "*(" + cty + " far *)((char far *)" + p->second + " + " + std::to_string(d) + ")"
                         : "*(" + cty + " far *)" + p->second;
    return FarMem{expr, wty == "byte"};
  }

  std::string val(std::string tok, Reg &reg) {
    if (has(tok, ":[")) {
      auto ff = farmem(has(tok, "ptr") ? tok : "word ptr " + tok);
      if (ff) return ff->expr;
    }
    bool byte = has(tok, "byte ptr");
    tok = replace_all(replace_all(tok, "word ptr ", ""), "byte ptr ", "");
    if (auto r = get(reg, tok)) return *r;
    std::string w;
    if (bp_plus_at(tok, 0, w)) return pname(int0(w));
    if (bp_minus_full(tok, 0, w)) return local(int0(w), byte);
    std::string h;
    if (hexmem_full(tok, 0, h)) {
      long long off = int16(h);
      auto nm = syms.find(off), lo = syms.find(off - 2);
      if (nm != syms.end() && nm->second.second == "long") {
        globs.set(nm->second.first, "long");
        return nm->second.first;
      }
      if (lo != syms.end() && lo->second.second == "long") {
        globs.set(lo->second.first, "long");
        return "(WORD)(" + lo->second.first + " >> 16)";
      }
      std::string name = nm != syms.end() ? nm->second.first : "gW_" + hexs(off);
      globs.set(name, "word");
      return name;
    }
    {   // \[(bx|si|di)(?:\s*([+-])\s*(0x[0-9a-f]+|\d+))?\]$
      Cur c(tok);
      std::string base;
      if (c.lit("[") && (c.lit("bx") || c.lit("si") || c.lit("di"))) {
        base = tok.substr(1, 2);
        size_t save = c.i;
        std::string sign, num;
        bool ok = false;
        c.ws();
        if (c.i < tok.size() && (tok[c.i] == '+' || tok[c.i] == '-')) {
          sign = tok.substr(c.i, 1);
          c.i++;
          c.ws();
          if (hex_or_dec_then(c, num, "]", true)) ok = true;
        }
        if (!ok) {
          c.i = save;
          sign.clear();
          num.clear();
          ok = c.lit("]") && c.end();
        }
        if (ok) {
          auto bv = get(reg, base);
          if (bv && !is_rawreg(*bv)) {
            long long d = num.empty() ? 0 : int0(num);
            if (sign == "-") d = -d;
            std::string cty = byte ? "char" : "WORD";
            if (d) return "*(" + cty + " *)((char *)" + *bv + " + " + std::to_string(d) + ")";
            return "*(" + cty + " *)" + *bv;
          }
        }
      }
    }
    if (num_tok(tok)) return std::to_string(int0(tok));
    return tok;
  }

  long long longload_has(long long off, std::string &nm) const {
    auto p = L.longs.find(off);
    if (p == L.longs.end()) return 0;
    nm = p->second;
    return 1;
  }

  long long long_fold(long long i, long long hi, Reg &reg) {
    const std::string &m = M(i).mnemonic, &ops = M(i).op_str;
    std::string w, w2, nm;
    // ax, word ptr \[bp \+ (\w+)\]$
    bool g = starts(ops, "ax, word ptr ") && bp_plus_full(ops, 13, w);
    if (m == "mov" && g && i + 1 < hi) {
      long long off = int0(w);
      bool has_nm = longload_has(off, nm) && !nm.empty();
      bool n2 = starts(M(i + 1).op_str, "dx, word ptr ") && bp_plus_full(M(i + 1).op_str, 13, w2);
      if (has_nm && M(i + 1).mnemonic == "mov" && n2 && int0(w2) == off + 2) {
        reg["dxax"] = nm;
        return i + 2;
      }
    }
    if (m == "shl" && ops == "ax, 1" && reg.count("dxax")) {
      long long k = 0, j = i;
      while (j + 1 < hi && M(j).mnemonic == "shl" && M(j).op_str == "ax, 1" && M(j + 1).mnemonic == "rcl" &&
             M(j + 1).op_str == "dx, 1") {
        k++;
        j += 2;
      }
      if (k) {
        reg["dxax"] = "(" + reg["dxax"] + " << " + std::to_string(k) + ")";
        return j;
      }
    }
    if (m == "add" && g && reg.count("dxax") && i + 1 < hi) {
      long long off = int0(w);
      bool has_nm = longload_has(off, nm) && !nm.empty();
      bool n2 = starts(M(i + 1).op_str, "dx, word ptr ") && bp_plus_full(M(i + 1).op_str, 13, w2);
      if (has_nm && M(i + 1).mnemonic == "adc" && n2 && int0(w2) == off + 2) {
        reg["dxax"] = reg["dxax"] + " + " + nm;
        return i + 2;
      }
    }
    if (m == "sub" && ops == "dx, dx" && truthy(reg, "ax")) {
      reg["dxax"] = "(long)" + reg["ax"];
      return i + 1;
    }
    bool gd = starts(ops, "dx, word ptr ") && bp_plus_full(ops, 13, w);
    if (m == "mov" && gd && i + 1 < hi && M(i + 1).mnemonic == "sub" && M(i + 1).op_str == "ax, ax") {
      long long off = int0(w);
      auto p = L.pnames.find(off);
      std::string n = (p != L.pnames.end() && !p->second.empty()) ? p->second : param_name(off);
      reg["dxax"] = "(long)" + n + " << 16";
      return i + 2;
    }
    return i;
  }

  // is the instruction after a call its result's consumer?
  bool consumer(const CsIns *nx) const {
    if (!nx) return false;
    const std::string &m = nx->mnemonic, &o = nx->op_str;
    if (m == "mov" && ends(o, ", ax")) return true;
    if (m == "push" && o == "ax") return true;
    if (m == "cmp" || m == "test") {
      for (auto &p : split(o, ", "))
        if (p == "ax") return true;
    }
    if (m == "or" && o == "ax, ax") return true;
    if (m == "jmp" && starts(o, "0x") && int16(o) >= end) return true;
    return false;
  }

  void after_call(std::vector<std::string> &out, Reg &reg, const CsIns *nx, bool cons, const std::string &call) {
    if (!cons && !(nx == nullptr && !is_void)) {
      out.push_back(call + ";");
      reg.erase("_lastcall");
    } else if (cons && nx && (nx->mnemonic == "cmp" || nx->mnemonic == "test" || nx->mnemonic == "or")) {
      reg.erase("_lastcall");
    }
  }

  static void indent(std::vector<std::string> &out, const std::vector<std::string> &body) {
    for (auto &t : body) out.push_back("    " + t);
  }

  std::string near_name(long long tgt) {
    auto p = nearmap.find(tgt);
    return p != nearmap.end() ? p->second : "sub_" + hexs(tgt);
  }

  std::string join_args(const std::vector<std::string> &a) const {
    std::string s;
    for (size_t k = 0; k < a.size(); k++) s += (k ? ", " : "") + a[k];
    return s;
  }

  Lines walk(long long blo, long long hi, Reg reg) {
    std::vector<std::string> out;
    long long i = blo, guard = 0;
    while (i < hi) {
      guard++;
      if (guard > 8 * N() + 64) {
        fail = "iteration backstop (control-flow not modeled)";
        return std::nullopt;
      }
      const CsIns &ins = M(i);
      const Bytes &op = ins.bytes;
      const std::string mn = ins.mnemonic, ops = ins.op_str;
      fail = mn + " " + ops;
      long long j = long_fold(i, hi, reg);
      if (j != i) {
        i = j;
        continue;
      }
      // do-while loop head
      if (loops.count(i) && !active.count(i)) {
        active.insert(i);
        long long jcc_i = loops[i].first;
        int jop = loops[i].second;
        std::string ctr, cn;
        bool has_ctr = counter.count(i) != 0, has_cn = false;
        if (has_ctr) {
          ctr = counter[i];
          if (cname.count(ctr)) {
            cn = cname[ctr];
            has_cn = true;
          }
        }
        if (has_cn && !cn.empty()) reg[ctr] = cn;
        const CsIns &flag = M(jcc_i - 1);
        long long body_end;
        std::string cond;
        if (flag.mnemonic == "cmp") {
          body_end = jcc_i - 1;
          auto lr = split2(flag.op_str, ", ");
          std::string a = val(lr.first, reg);
          std::string d = JCC_DIRECT(jop);
          cond = a + " " + d + " " + val(lr.second, reg);
        } else {
          body_end = jcc_i;
          cond = (has_cn && !cn.empty()) ? cn + " " + JCC_DIRECT(jop) : JCC_DIRECT(jop);
        }
        Lines body = walk(i, body_end, reg);
        active.erase(i);
        if (!body) return std::nullopt;
        out.push_back("do {");
        indent(out, *body);
        out.push_back("} while (" + cond + ");");
        if (has_cn && !cn.empty()) reg[ctr] = cn;
        i = jcc_i + 1;
        continue;
      }
      if ((mn == "dec" || mn == "inc") && cname.count(ops)) {
        out.push_back(cname[ops] + (mn == "dec" ? "--" : "++") + ";");
        i++;
        continue;
      }
      if (mn == "dec" || mn == "inc") {
        std::string h;
        size_t a0 = starts(ops, "word ptr ") ? 9 : 0;
        bool dm = hexmem_full(ops, a0, h) || (a0 && hexmem_full(ops, 0, h));
        if (dm) {
          long long off = int16(h);
          auto nm = syms.find(off);
          std::string name = nm != syms.end() ? nm->second.first : "gW_" + hexs(off);
          globs.set(name, nm != syms.end() ? nm->second.second : "word");
          out.push_back(name + (mn == "dec" ? "--" : "++") + ";");
          i++;
          continue;
        }
      }
      // long-global test
      if (op[0] == 0xa1 && i + 2 < hi && M(i + 1).bytes.size() >= 2 && M(i + 1).bytes[0] == 0x0b &&
          M(i + 1).bytes[1] == 0x06 && (M(i + 2).mnemonic == "je" || M(i + 2).mnemonic == "jne")) {
        long long lo = u16le(M(i + 1).bytes, 2);
        auto nm = syms.find(lo);
        if (nm != syms.end() && nm->second.second == "long") {
          globs.set(nm->second.first, "long");
          const CsIns &jcc = M(i + 2);
          std::string cond = nm->second.first + " " + JCC_INV(jcc.bytes[0]) + " 0L";
          auto skip_i = ti(int16(jcc.op_str));
          if (!skip_i) return std::nullopt;
          Lines then = walk(i + 3, *skip_i, reg);
          if (!then) return std::nullopt;
          out.push_back("if (" + cond + ") {");
          indent(out, *then);
          out.push_back("}");
          i = *skip_i;
          continue;
        }
      }
      // long-global store zero
      if (op.size() >= 2 && op[0] == 0x2b && op[1] == 0xc0 && i + 2 < hi && M(i + 1).bytes[0] == 0xa3 &&
          M(i + 2).bytes[0] == 0xa3) {
        long long lo = u16le(M(i + 2).bytes, 1);
        auto nm = syms.find(lo);
        if (nm != syms.end() && nm->second.second == "long") {
          globs.set(nm->second.first, "long");
          out.push_back(nm->second.first + " = 0L;");
          reg["ax"] = "0";
          i += 3;
          continue;
        }
      }
      if ((mn == "cmp" || mn == "or" || mn == "test") && i + 1 < hi && in_jcc_set(M(i + 1).mnemonic)) {
        auto sc = shortcircuit(i, hi, reg);
        if (sc) {
          Lines body = walk(sc->join, sc->exit_i, reg);
          if (!body) return std::nullopt;
          out.push_back("if (" + sc->cond + ") {");
          indent(out, *body);
          out.push_back("}");
          i = sc->exit_i;
          continue;
        }
      }
      if (mn == "cmp" && i + 1 < hi && M(i + 1).mnemonic == "jne") {
        auto bm = bool_materialize(i, hi, reg);
        if (bm) {
          reg[bm->R] = bm->expr;
          i = bm->J;
          continue;
        }
      }
      if (i + 1 < hi && in_jcc_set(M(i + 1).mnemonic) && (mn == "or" || mn == "cmp")) {
        const CsIns &jcc = M(i + 1);
        int jop = jcc.bytes[0];
        auto skip_o = ti(int16(jcc.op_str));
        if (!skip_o || *skip_o <= i) {
          fail = "backward branch (goto): " + ops;
          return std::nullopt;
        }
        long long skip_i = *skip_o;
        bool has_els = false;
        long long els0 = 0, els1 = 0, cont = skip_i, then_end = skip_i, jx = skip_i - 1;
        if (jx > i + 1 && M(jx).mnemonic == "jmp" && starts(M(jx).op_str, "0x")) {
          auto l2 = ti(int16(M(jx).op_str));
          if (l2 && skip_i < *l2 && *l2 < N()) {
            then_end = jx;
            has_els = true;
            els0 = skip_i;
            els1 = *l2;
            cont = *l2;
          }
        }
        Lines then = walk(i + 2, then_end, reg);
        if (!then) return std::nullopt;
        if (has_els && then->empty()) {
          auto cond = cond_of(ins, JCC_OPP_get(jop), reg);
          Lines body = walk(els0, els1, reg);
          if (!cond || !body) return std::nullopt;
          out.push_back("if (" + *cond + ") {");
          indent(out, *body);
          out.push_back("}");
          i = cont;
          continue;
        }
        auto cond = cond_of(ins, jop, reg);
        if (!cond) return std::nullopt;
        out.push_back("if (" + *cond + ") {");
        indent(out, *then);
        if (has_els) {
          Lines eb = walk(els0, els1, reg);
          if (!eb) return std::nullopt;
          out.push_back("} else {");
          indent(out, *eb);
        }
        out.push_back("}");
        i = cont;
        continue;
      }
      if (mn == "jmp") {
        long long tgt = resolve_jmp(int16(ops));
        if (tgt >= end && reg.count("ax")) out.push_back("return " + reg["ax"] + ";");
        i++;
        continue;
      }
      // long-constant argument
      if (mn == "mov" && i + 3 < hi) {
        bool gl = starts(ops, "ax, ") && num_tok(ops, 4);
        bool gh = starts(M(i + 1).op_str, "dx, ") && num_tok(M(i + 1).op_str, 4);
        if (gl && gh && M(i + 1).mnemonic == "mov" && M(i + 2).mnemonic == "push" && M(i + 2).op_str == "dx" &&
            M(i + 3).mnemonic == "push" && M(i + 3).op_str == "ax") {
          long long lo = int0(ops.substr(4)) & 0xffff, hiw = int0(M(i + 1).op_str.substr(4)) & 0xffff;
          args.push_back(std::to_string((hiw << 16) | lo) + "L");
          reg["ax"] = std::to_string(lo);
          reg["dx"] = std::to_string(hiw);
          i += 4;
          continue;
        }
      }
      if (mn == "push" && ops == "dx" && i + 1 < hi && truthy(reg, "dxax") && M(i + 1).mnemonic == "push" &&
          M(i + 1).op_str == "ax") {
        args.push_back(reg["dxax"]);
        i += 2;
        continue;
      }
      // intra-segment far call: push cs ; call near
      if (mn == "push" && ops == "cs" && i + 1 < hi && M(i + 1).mnemonic == "call") {
        long long tgt = func_base + int16(M(i + 1).op_str);
        std::string nm = near_name(tgt);
        has_nearcall = true;
        nears_used.set(nm, "1");
        reg["ax"] = nm + "(" + join_args(args) + ")";
        args.clear();
        reg["_lastcall"] = reg["ax"];
        const CsIns *nx = i + 2 < hi ? &M(i + 2) : nullptr;
        if (nx && nx->mnemonic == "add" && starts(nx->op_str, "sp,")) nx = i + 3 < hi ? &M(i + 3) : nullptr;
        bool cons = consumer(nx);
        after_call(out, reg, nx, cons, reg["ax"]);
        i += 2;
        continue;
      }
      if (mn == "call" && op[0] == 0xe8) {
        long long tgt = func_base + int16(ops);
        std::string nm = near_name(tgt);
        has_nearcall = true;
        nears_used.set(nm, "1");
        reg["ax"] = nm + "(" + join_args(args) + ")";
        args.clear();
        reg["_lastcall"] = reg["ax"];
        const CsIns *nx = i + 1 < hi ? &M(i + 1) : nullptr;
        if (nx && nx->mnemonic == "add" && starts(nx->op_str, "sp,")) nx = i + 2 < hi ? &M(i + 2) : nullptr;
        bool cons = consumer(nx);
        after_call(out, reg, nx, cons, reg["ax"]);
        i++;
        continue;
      }
      if (op[0] == 0x9a) {   // CALLF
        auto ip = imports.find(calls);
        std::string nm = ip != imports.end() ? ip->second : "G" + std::to_string(calls);
        calls++;
        const CsIns *nxt = i + 1 < hi ? &M(i + 1) : nullptr;
        bool is_cdecl = nxt && nxt->mnemonic == "add" && starts(nxt->op_str, "sp,");
        std::vector<std::string> argv;
        if (is_cdecl) {
          cdecl_imports.insert(nm);
          argv.assign(args.rbegin(), args.rend());
          nxt = i + 2 < hi ? &M(i + 2) : nullptr;
        } else {
          argv = args;
        }
        reg["ax"] = nm + "(" + join_args(argv) + ")";
        args.clear();
        reg["_lastcall"] = reg["ax"];
        bool consumed = consumer(nxt);
        after_call(out, reg, nxt, consumed, reg["ax"]);
      } else if (mn == "lcall" && starts(ops, "es:[")) {
        // es:\[(\w+)(?:\s*\+\s*(0x[0-9a-f]+|\d+))?\]
        Cur c(ops, 4);
        std::string base, disp;
        bool bm = c.word(base);
        if (bm) {
          size_t save = c.i;
          bool got = false;
          c.ws();
          if (c.lit("+")) {
            c.ws();
            if (hex_or_dec_then(c, disp, "]", false)) got = true;
          }
          if (!got) {
            c.i = save;
            disp.clear();
            bm = c.lit("]");
          }
        }
        if (!(bm && vtbl.count(base))) {
          fail = "lcall " + ops;
          return std::nullopt;
        }
        std::string obj = vtbl[base];
        uses_vtbl = true;
        long long idx = floordiv(disp.empty() ? 0 : int0(disp), 4);
        std::string call = "(*(PROC far * far *)" + obj + ")[" + std::to_string(idx) + "](" + join_args(args) + ")";
        args.clear();
        reg["ax"] = call;
        reg["_lastcall"] = call;
        const CsIns *nxt = i + 1 < hi ? &M(i + 1) : nullptr;
        bool consumed = consumer(nxt);
        after_call(out, reg, nxt, consumed, call);
      } else if (mn == "les" || mn == "lds") {
        auto pr = partition(ops, ", ");
        const std::string dst = pr.first, memt = pr.second;
        std::string w, h;
        bool m = search_bp_plus(memt, w);
        auto hm = finditer_hexmem(memt);
        bool mg = !hm.empty();
        std::optional<FarMem> ff;
        if (has(memt, ":")) {
          auto sp = memt.find("ptr");
          std::string rest = sp == std::string::npos ? memt : memt.substr(sp + 3);
          size_t a = rest.find_first_not_of(" \t\n\r\f\v"), b = rest.find_last_not_of(" \t\n\r\f\v");
          rest = a == std::string::npos ? "" : rest.substr(a, b - a + 1);
          ff = farmem("word ptr " + rest);
        }
        if (m) {
          long long o = int0(w);
          auto lane = L.lanes.find(o);
          std::string p = lane != L.lanes.end() ? lane->second.first : pname(o);
          farptr[dst] = p;
          esbx = p;
        } else if (ff) {
          farptr[dst] = ff->expr;
          esbx = ff->expr;
          // re.search(r":\[(\w+)\]", memt)
          size_t k = 0;
          std::string bw;
          bool found = false;
          while ((k = memt.find(":[", k)) != std::string::npos) {
            Cur c(memt, k + 2);
            if (c.word(bw) && c.lit("]")) {
              found = true;
              break;
            }
            k++;
          }
          if (found && farptr.count(bw) && !farptr[bw].empty()) vtbl[dst] = farptr[bw];
        } else if (mg) {
          long long off = int16(hm[0]);
          auto nm = syms.find(off);
          std::string p = nm != syms.end() ? nm->second.first : "gW_" + hexs(off);
          globs.set(p, "word");
          farptr[dst] = p;
          esbx = p;
        } else {
          return std::nullopt;
        }
      } else if (mn == "push" && (ops == "ss" || ops == "ds" || ops == "es")) {
        const CsIns *nx = i + 1 < hi ? &M(i + 1) : nullptr;
        if (nx && nx->mnemonic == "push") {
          std::string ov = replace_all(nx->op_str, "word ptr ", "");
          std::string dflt = val(ov, reg);
          std::string off = get(reg, ov) ? *get(reg, ov) : dflt;
          if (!is_rawreg(off)) {
            args.push_back("(char far *)" + off);
            i += 2;
            continue;
          }
        }
        fail = "push " + ops;
        return std::nullopt;
      } else if (mn == "push" && has(ops, ":[")) {
        auto ff = farmem(ops);
        if (!ff) {
          fail = "push " + ops;
          return std::nullopt;
        }
        args.push_back(ff->expr);
      } else if (mn == "push" && starts(ops, "dword ptr ")) {
        std::string loc = ops.substr(10), w, h;
        bool gp = bp_plus_full(loc, 0, w);
        bool gl = !gp && bp_minus_full(loc, 0, w);
        bool gg = !gp && !gl && hexmem_full(loc, 0, h);
        if (gp) {
          long long off = int0(w);
          std::string nm;
          bool ok = longload_has(off, nm) != 0;
          if (!ok) {
            auto ln = L.lanes.find(off);
            if (ln != L.lanes.end() && ln->second.second == "lo") {
              nm = ln->second.first;
              ok = true;
            }
          }
          if (!ok) {
            fail = "push " + ops;
            return std::nullopt;
          }
          args.push_back(nm);
        } else if (gl) {
          std::string nm = "loc_" + hexs(int0(w));
          localdecl.set(nm, "long");
          args.push_back(nm);
        } else if (gg) {
          long long off = int16(h);
          auto sy = syms.find(off);
          std::string nm = (sy != syms.end() && sy->second.second == "long") ? sy->second.first : "gL_" + hexs(off);
          globs.set(nm, "long");
          args.push_back(nm);
        } else {
          fail = "push " + ops;
          return std::nullopt;
        }
      } else if (mn == "push") {
        std::string r = replace_all(ops, "word ptr ", ""), h, w;
        bool mm = hexmem_full(r, 0, h);
        if (r == "es:[bx]") {
          farty[esbx] = "int";
          args.push_back("*" + esbx);
        } else if (get(reg, r) && has(*get(reg, r), "#")) {
          auto nl = split2(*get(reg, r), "#");
          if (nl.second == "hi") {
            pendfar = nl.first;
          } else if (pendfar && *pendfar == nl.first) {
            args.push_back(nl.first);
            pendfar.reset();
          } else {
            args.push_back(nl.first);
          }
        } else if (get(reg, r)) {
          if (skip_push && *skip_push == r) {
            skip_push.reset();
          } else if (reg[r] == "0" && i + 1 < hi && M(i + 1).mnemonic == "push" && M(i + 1).op_str == r) {
            args.push_back("0L");
            skip_push = r;
          } else {
            args.push_back(reg[r]);
          }
        } else if (bp_plus_at(r, 0, w)) {
          long long o = int0(w);
          auto lane = L.lanes.find(o);
          if (lane != L.lanes.end() && lane->second.second == "hi") {
            pendfar = lane->second.first;
          } else if (lane != L.lanes.end() && lane->second.second == "lo" && pendfar && *pendfar == lane->second.first) {
            args.push_back(lane->second.first);
            pendfar.reset();
          } else {
            args.push_back(pname(o));
          }
        } else if (bp_minus_full(r, 0, w)) {
          args.push_back(local(int0(w)));
        } else if (num_tok(r)) {
          long long v = int0(r);
          if (op[0] == 0x68 && memoff.count(v)) {
            auto nm = syms.find(v);
            std::string name = nm != syms.end() ? nm->second.first : "gW_" + hexs(v);
            globs.set(name, nm != syms.end() ? nm->second.second : "word");
            args.push_back("&" + name);
          } else {
            args.push_back(std::to_string(v));
          }
        } else if (mm) {
          long long off = int16(h);
          auto nm = syms.find(off), lo = syms.find(off - 2);
          if (nm != syms.end() && nm->second.second == "long") {
            globs.set(nm->second.first, "long");
            args.push_back("(WORD)" + nm->second.first);
          } else if (lo != syms.end() && lo->second.second == "long") {
            globs.set(lo->second.first, "long");
            args.push_back("(WORD)(" + lo->second.first + " >> 16)");
          } else if (nm != syms.end()) {
            globs.set(nm->second.first, "word");
            args.push_back(nm->second.first);
          } else {
            std::string g = "gW_" + hexs(off);
            globs.set(g, "word");
            args.push_back(g);
          }
        } else {
          return std::nullopt;
        }
      } else if (mn == "mov") {
        auto pr = partition(ops, ", ");
        const std::string dst = pr.first, src = pr.second;
        if (farptr.count(src)) {
          farptr[dst] = farptr[src];
        } else if (farptr.count(dst) && !farptr.count(src)) {
          farptr.erase(dst);
        }
        if (has(src, ":[")) {
          auto ff = farmem(src);
          if (!ff) {
            fail = "mov " + ops;
            return std::nullopt;
          }
          reg[dst] = ff->expr;
          i++;
          continue;
        }
        if (has(dst, ":[")) {
          auto ff = farmem(dst);
          if (!ff) {
            fail = "mov " + ops;
            return std::nullopt;
          }
          std::string dflt = val(src, reg);
          std::string v = get(reg, src) ? *get(reg, src) : dflt;
          if (is_rawreg(v)) {
            fail = "store raw " + v;
            return std::nullopt;
          }
          out.push_back(ff->expr + " = " + v + ";");
          i++;
          continue;
        }
        std::string v, w, h;
        if (get(reg, src)) {
          v = *get(reg, src);
        } else if (starts(src, "word ptr ") && bp_plus_at(src, 9, w)) {
          long long o = int0(w);
          if (L.lanes.count(o)) {
            v = L.lanes[o].first + "#" + L.lanes[o].second;
          } else if (L.pnames.count(o)) {
            v = L.pnames[o];
          } else {
            v = pname(o);
          }
        } else if (starts(src, "byte ptr ") && bp_plus_at(src, 9, w)) {
          v = pname(int0(w));
        } else if (num_tok(src)) {
          v = std::to_string(int0(src));
        } else if ((starts(src, "word ptr ") || starts(src, "byte ptr ")) &&
                   (hexmem_full(src, 9, h) || [&] {   // \[(bp - \w+)\]$
                     Cur c(src, 9);
                     std::string ww;
                     return c.lit("[bp - ") && c.word(ww) && c.lit("]") && c.end();
                   }())) {
          v = val(src, reg);
        } else {
          return std::nullopt;
        }
        if (has(v, "#")) {
          std::string base = split(v, "#")[0];
          if (dst == "es" || dst == "ds") {
            for (auto &kv : reg)
              if (kv.second == base + "#lo") farptr[kv.first] = base;
          } else if (ends(v, "#lo") && ((get(reg, "es") && *get(reg, "es") == base + "#hi") ||
                                        (get(reg, "ds") && *get(reg, "ds") == base + "#hi"))) {
            farptr[dst] = base;
          }
        }
        if (src == "ax" && i + 1 < hi && M(i + 1).mnemonic == "mov") {
          const std::string &nxo = M(i + 1).op_str;
          size_t a0 = starts(dst, "word ptr ") ? 9 : 0;
          std::string lw, gh;
          bool lb = bp_minus_full(dst, a0, lw) || (a0 && bp_minus_full(dst, 0, lw));
          bool lg = !lb && (hexmem_full(dst, a0, gh) || (a0 && hexmem_full(dst, 0, gh)));
          size_t b0 = starts(nxo, "word ptr ") ? 9 : 0;
          if (lb) {
            long long n = int0(lw);
            std::string hw;
            bool hh = bp_minus_full(nxo, b0, hw, ", dx") || (b0 && bp_minus_full(nxo, 0, hw, ", dx"));
            if (hh && int0(hw) == n - 2) {
              std::string nm = local(n, false);
              localdecl.set(nm, "long");
              out.push_back(nm + " = " + v + ";");
              i += 2;
              continue;
            }
          } else if (lg) {
            long long g = int16(gh);
            std::string hw;
            bool hh = hexmem_full(nxo, b0, hw, ", dx") || (b0 && hexmem_full(nxo, 0, hw, ", dx"));
            if (hh && int16(hw) == g + 2) {
              auto sym = syms.find(g);
              std::string name = sym != syms.end() ? sym->second.first : "gL_" + hexs(g);
              globs.set(name, "long");
              out.push_back(name + " = " + v + ";");
              i += 2;
              continue;
            }
          }
        }
        {
          std::string wty, lw;
          size_t a0 = 0;
          if (starts(dst, "word ptr ")) {
            wty = "word";
            a0 = 9;
          } else if (starts(dst, "byte ptr ")) {
            wty = "byte";
            a0 = 9;
          }
          bool ml = bp_minus_full(dst, a0, lw);
          if (!ml && a0) {
            ml = bp_minus_full(dst, 0, lw);
            if (ml) wty.clear();
          }
          if (ml) {
            std::string nm = local(int0(lw), wty == "byte");
            out.push_back(nm + " = " + v + ";");
            i++;
            continue;
          }
          std::string gh;
          bool mg = hexmem_full(dst, a0, gh);
          if (!mg && a0) {
            mg = hexmem_full(dst, 0, gh);
            if (mg) wty.clear();
          }
          if (mg) {
            std::string wdt = wty.empty() ? "word" : wty;
            long long off = int16(gh);
            auto nm = syms.find(off), lolane = syms.find(off - 2);
            if ((nm != syms.end() && nm->second.second == "long") || (lolane != syms.end() && lolane->second.second == "long"))
              return std::nullopt;
            std::string name;
            if (wdt == "byte") {
              name = "gB_" + hexs(off);
              globs.set(name, "byte");
            } else {
              name = nm != syms.end() ? nm->second.first : "gW_" + hexs(off);
              globs.set(name, "word");
            }
            out.push_back(name + " = " + v + ";");
            i++;
            continue;
          }
        }
        if (cname.count(dst)) {
          std::string cn = cname[dst];
          if (std::find(decl_order.begin(), decl_order.end(), cn) == decl_order.end()) decl_order.push_back(cn);
          out.push_back(cn + " = " + v + ";");
          reg[dst] = cn;
        } else if (get(reg, src) && (dst == "si" || dst == "di") && !regvars.count(dst) && get(reg, "_lastcall") &&
                   v == *get(reg, "_lastcall")) {
          std::string vn = "v" + std::to_string(regvars.size());
          regvars[dst] = vn;
          decl_order.push_back(vn);
          out.push_back(vn + " = " + v + ";");
          for (auto &a : args)
            if (a == v) a = vn;
          reg[dst] = vn;
          reg.erase("_lastcall");
        } else {
          reg[dst] = v;
        }
      } else if ((mn == "add" || mn == "sub") && starts(ops, "sp,")) {
      } else if (mn == "cwd" || mn == "cdq") {
        reg["dxax"] = "(long)" + (get(reg, "ax") ? *get(reg, "ax") : std::string("0"));
      } else if (mn == "nop") {
      } else if (mn == "sub" && has(ops, ",") && [&] {
                   auto p = split(ops, ", ");
                   if (p.size() < 2) raise(PY_INDEX, "list index out of range");
                   return p[0] == p[1];
                 }()) {
        std::string dst = split(ops, ", ")[0];
        if (cname.count(dst)) {
          std::string cn = cname[dst];
          if (std::find(decl_order.begin(), decl_order.end(), cn) == decl_order.end()) decl_order.push_back(cn);
          out.push_back(cn + " = 0;");
          reg[dst] = cn;
        } else {
          reg[dst] = "0";
        }
      } else if (alu2(mn) && has(ops, ",")) {
        auto pr = partition(ops, ", ");
        if (!is_wreg(pr.first)) return std::nullopt;
        std::string a = val(pr.first, reg);
        std::string b = val(pr.second, reg);
        reg[pr.first] = "(" + a + " " + alu2(mn) + " " + b + ")";
      } else if ((mn == "neg" || mn == "not") && is_wreg(ops)) {
        reg[ops] = std::string("(") + (mn == "neg" ? "-" : "~") + val(ops, reg) + ")";
      } else if (mn == "lea") {
        auto pr = partition(ops, ", ");
        std::string w, h;
        if (bp_minus_full(pr.second, 0, w)) {
          reg[pr.first] = "&" + local(int0(w));
        } else if (hexmem_full(pr.second, 0, h)) {
          long long off = int16(h);
          auto nm = syms.find(off);
          std::string name = nm != syms.end() ? nm->second.first : "gW_" + hexs(off);
          globs.set(name, nm != syms.end() ? nm->second.second : "word");
          reg[pr.first] = "&" + name;
        } else {
          return std::nullopt;
        }
      } else {
        fail = mn + " " + ops;
        return std::nullopt;
      }
      i++;
    }
    bool final = hi >= N();
    if (truthy(reg, "_lastcall")) {
      std::string joined;
      for (auto &t : out) joined += t;
      const std::string lc = reg["_lastcall"];
      bool same = final && get(reg, "ax") && *get(reg, "ax") == lc;
      if (!has(joined, lc) && !same) {
        out.push_back(lc + ";");
        reg.erase("_lastcall");
      }
    }
    if (final && blo == 0 && !is_void) {
      if (rtype == "long" && truthy(reg, "dxax")) {
        out.push_back("return " + reg["dxax"] + ";");
      } else if (truthy(reg, "ax")) {
        out.push_back("return " + reg["ax"] + ";");
      }
    }
    return out;
  }
};

}  // namespace

std::string jstr(const std::string &s) {
  std::string o = "\"";
  for (unsigned char c : s) {
    if (c == '"') {
      o += "\\\"";
    } else if (c == '\\') {
      o += "\\\\";
    } else if (c < 0x20) {
      char b[8];
      snprintf(b, sizeof b, "\\u%04x", c);
      o += b;
    } else {
      o += (char)c;
    }
  }
  return o + "\"";
}

std::string msc6_invert(const Msc6In &in) {
  const Bytes &b = in.code;
  long long p;
  if (startswith(b, PROLOGUE)) {
    p = (long long)PROLOGUE.size();
  } else if (startswith(b, PROLOGUE_MOVDS)) {
    p = (long long)PROLOGUE_MOVDS.size();
  } else if (startswith(b, PROLOGUE_NEAR)) {
    p = (long long)PROLOGUE_NEAR.size();
  } else {
    return "{\"t\": null, \"w\": \"prologue miss\"}";
  }
  if (byte_or(b, p) == 0x83 && byte_or(b, p + 1) == 0xec) {
    p += 3;
  } else if (byte_or(b, p) == 0x81 && byte_or(b, p + 1) == 0xec) {
    p += 4;
  }
  while (byte_or(b, p) == 0x57 || byte_or(b, p) == 0x56) p++;
  long long ei = rfind(b, EPI), epilen = (long long)EPI.size();
  if (ei < 0) ei = rfind(b, EPI_LEA), epilen = (long long)EPI_LEA.size();
  if (ei < 0) ei = rfind(b, EPI_SUBBP), epilen = (long long)EPI_SUBBP.size();
  if (ei < 0) ei = rfind(b, EPI_NEAR_LEA), epilen = (long long)EPI_NEAR_LEA.size();
  if (ei < 0) ei = rfind(b, EPI_NEAR_MOV), epilen = (long long)EPI_NEAR_MOV.size();
  if (ei < 0) return "{\"t\": null, \"w\": \"no epilogue\"}";
  long long be = ei;
  while (be > p && (b[(size_t)be - 1] == 0x5e || b[(size_t)be - 1] == 0x5f)) be--;
  std::vector<CsIns> md;
  if (be > p) md = disasm16(b.data() + p, (size_t)(be - p), p);
  std::set<long long> memoff;
  for (auto &ins : md)
    for (auto &h : finditer_hexmem(ins.op_str)) memoff.insert(int16(h));
  std::optional<Params> params = in.params;
  {
    std::set<long long> far_offs, long_offs, accessed;
    long long maxo = 4;
    std::map<std::string, long long> seg_from;
    for (size_t j = 0; j < md.size(); j++) {
      const CsIns &ins = md[j];
      std::string w, w2;
      if (ins.mnemonic == "les" || ins.mnemonic == "lds")
        if (search_bp_plus(ins.op_str, w)) far_offs.insert(int0(w));
      bool mm = (starts(ins.op_str, "es, word ptr ") || starts(ins.op_str, "ds, word ptr ")) && bp_plus_at(ins.op_str, 13, w);
      if (ins.mnemonic == "mov" && mm) far_offs.insert(int0(w) - 2);
      {   // (\w+), word ptr \[bp \+ (\w+)\]$
        Cur c(ins.op_str);
        std::string r;
        bool gm = c.word(r) && c.lit(", word ptr ") && bp_plus_full(ins.op_str, c.i, w);
        if (ins.mnemonic == "mov" && gm) seg_from[r] = int0(w);
      }
      {   // (es|ds), (ax|bx|cx|dx|si|di)$
        const std::string &o = ins.op_str;
        if (ins.mnemonic == "mov" && o.size() == 6 && (starts(o, "es, ") || starts(o, "ds, ")) && is_wreg(o.substr(4)) &&
            seg_from.count(o.substr(4)))
          far_offs.insert(seg_from[o.substr(4)] - 2);
      }
      if (ins.mnemonic == "push" && starts(ins.op_str, "dword ptr ") && bp_plus_full(ins.op_str, 10, w)) {
        long long o = int0(w);
        long_offs.insert(o);
        maxo = std::max(maxo, o + 2);
      }
      if (ins.mnemonic == "mov" && starts(ins.op_str, "ax, word ptr ") && bp_plus_full(ins.op_str, 13, w) && j + 1 < md.size()) {
        long long o = int0(w);
        if (md[j + 1].mnemonic == "mov" && starts(md[j + 1].op_str, "dx, word ptr ") && bp_plus_full(md[j + 1].op_str, 13, w2) &&
            int0(w2) == o + 2) {
          long_offs.insert(o);
          maxo = std::max(maxo, o + 2);
        }
      }
      for (auto &x : finditer_bp_plus(ins.op_str)) {
        long long oo = int0(x);
        if (oo >= 6) {
          accessed.insert(oo);
          maxo = std::max(maxo, oo);
        }
      }
    }
    if (!far_offs.empty() || !long_offs.empty()) {
      long long q0 = ei + epilen;
      std::optional<long long> retf_total;
      if (byte_or(b, q0) == 0xca) retf_total = at(b, q0 + 1) | (at(b, q0 + 2) << 8);
      long long limit = (retf_total && *retf_total) ? 6 + *retf_total : maxo + 2;
      std::vector<std::string> inferred;
      long long oo = 6;
      while (oo < limit) {
        if (far_offs.count(oo) && oo + 4 <= limit) {
          inferred.push_back("far");
          oo += 4;
        } else if (long_offs.count(oo) && oo + 4 <= limit) {
          inferred.push_back("long");
          oo += 4;
        } else if (accessed.count(oo) || (retf_total && *retf_total && oo < limit)) {
          inferred.push_back("int");
          oo += 2;
        } else {
          oo += 2;
        }
      }
      Params built;
      for (size_t k = 0; k < inferred.size(); k++) built.push_back({"p" + std::to_string(k), inferred[inferred.size() - 1 - k]});
      if (!params) {
        params = built;
      } else {
        Layout pl = layout_params(params);
        bool all_int = true;
        for (auto &pp : *params)
          if (pp.second != "int") all_int = false;
        bool miss = false;
        for (auto o : far_offs)
          if (!pl.lanes.count(o)) miss = true;
        for (auto o : long_offs)
          if (!pl.longs.count(o)) miss = true;
        if (all_int || miss) params = built;
      }
    }
  }
  if (!params) {
    long long q0 = ei + epilen;
    if (byte_or(b, q0) == 0xca) {
      long long cnt = (at(b, q0 + 1) | (at(b, q0 + 2) << 8)) / 2;
      if (0 < cnt && cnt <= 32) {
        Params pp;
        for (long long k = 0; k < cnt; k++) pp.push_back({"p" + std::to_string(k), "int"});
        params = pp;
      }
    }
  }
  Dec d(md, be, in.imports, in.syms, params, in.nearmap, in.func_base);
  d.memoff = memoff;
  d.is_void = in.rtype == "void";
  d.rtype = in.rtype;
  Lines lines = d.walk(0, (long long)md.size(), Reg());
  if (!lines) return "{\"t\": null, \"w\": " + jstr("unmodeled: " + d.fail) + "}";
  long long q = ei + epilen;
  bool pascal = true;
  long long conv = 0;
  if (byte_or(b, q) == 0xca) {
    conv = (at(b, q + 1) | (at(b, q + 2) << 8)) / 2;
  } else if (byte_or(b, q) == 0xcb || byte_or(b, q) == 0xc3) {
    pascal = false;
  }
  long long ninfer = 0;
  for (auto &ins : md)
    for (auto &x : finditer_bp_plus(ins.op_str)) {
      long long o = int0(x);
      if (o >= 6) ninfer = std::max(ninfer, floordiv(o - 6, 2) + 1);
    }
  std::string kw = pascal ? "pascal " : "";
  std::string ptext;
  if (params && !params->empty()) {
    std::vector<std::string> sig;
    for (auto &pp : *params) {
      if (pp.second == "far") {
        auto f = d.farty.find(pp.first);
        sig.push_back((f != d.farty.end() ? f->second : std::string("char")) + " far *" + pp.first);
      } else if (pp.second == "long") {
        sig.push_back("long " + pp.first);
      } else {
        sig.push_back("int " + pp.first);
      }
    }
    for (size_t k = 0; k < sig.size(); k++) ptext += (k ? ", " : "") + sig[k];
    if (ptext.empty()) ptext = "void";
  } else {
    long long nparam = pascal ? conv : ninfer;
    if (pascal && ninfer > nparam) nparam = ninfer;
    for (long long k = 0; k < nparam; k++) ptext += (k ? ", int " : "int ") + chr('a' + k);
    if (ptext.empty()) ptext = "void";
  }
  std::set<std::string> seen;
  std::string decls = "typedef unsigned int WORD;\n";
  if (d.uses_vtbl) decls += "typedef WORD (far pascal *PROC)();\n";
  for (long long k = 0; k < d.calls; k++) {
    auto ip = in.imports.find(k);
    std::string nm = ip != in.imports.end() ? ip->second : "G" + std::to_string(k);
    if (!seen.count(nm)) {
      std::string pk = d.cdecl_imports.count(nm) ? "" : "pascal ";
      decls += "extern WORD far " + pk + nm + "();\n";
      seen.insert(nm);
    }
  }
  for (auto &g : d.globs.keys) {
    const std::string &t = d.globs.v[g];
    decls += "extern " + std::string(t == "long" ? "long" : t == "byte" ? "char" : "int") + " " + g + ";\n";
  }
  for (auto &snm : d.nears_used.keys) decls += "extern int far " + snm + "();\n";
  std::string body;
  for (auto &v : d.decl_order) body += "    register int " + v + ";\n";
  std::vector<std::pair<long long, std::string>> locs;
  for (auto &nm : d.localdecl.keys) locs.push_back({int16(split(nm, "_")[1]), nm});
  std::stable_sort(locs.begin(), locs.end(), [](auto &a, auto &c) { return a.first < c.first; });
  for (auto &l : locs) body += "    " + d.localdecl.v[l.second] + " " + l.second + ";\n";
  for (auto &ln : *lines) body += "    " + ln + "\n";
  std::string text = decls + "\n" + in.rtype + " far " + kw + "F(" + ptext + ")\n{\n" + body + "}";
  std::string m = "{\"calls\": " + std::to_string(d.calls) + ", \"regvars\": [";
  for (size_t k = 0; k < d.decl_order.size(); k++) m += (k ? ", " : "") + jstr(d.decl_order[k]);
  m += "], \"globals\": {";
  for (size_t k = 0; k < d.globs.keys.size(); k++)
    m += (k ? ", " : "") + jstr(d.globs.keys[k]) + ": " + jstr(d.globs.v[d.globs.keys[k]]);
  m += "}, \"cdecl\": " + std::string(pascal ? "false" : "true") + ", \"has_nearcall\": " + (d.has_nearcall ? "true" : "false") +
       ", \"nears\": [";
  for (size_t k = 0; k < d.nears_used.keys.size(); k++) m += (k ? ", " : "") + jstr(d.nears_used.keys[k]);
  m += "]}";
  return "{\"t\": " + jstr(text) + ", \"m\": " + m + "}";
}

}  // namespace cbinv
