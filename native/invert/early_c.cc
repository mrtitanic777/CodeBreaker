// early_c.cc -- invert/early_c.py's register value tracker (class R), recover and recover_cfg, in C++,
// decision for decision.  Register values are Python str objects (ecu::Str: None, the text, and the
// object's identity, which `s.ax is not old` reads); every regular expression is the Python's, run by
// native/l2's pyre (Python's re semantics); the one set whose iteration order the output depends on
// (_wregs' registers, in _kill_aliases) is iterated in CPython's order, from the caller's str hashes.
#include "early_c.h"

#include <algorithm>
#include <climits>
#include <set>
#include <unordered_map>

#include "early_c_util.h"
#include "flow16.h"

namespace cbinv {

using namespace ecu;
using std::string;

namespace {

typedef std::shared_ptr<std::vector<string>> List;
inline List newlist() { return std::make_shared<std::vector<string>>(); }
const int NI = INT_MIN;   // None, as an instruction index

enum { AX, BX, CX, DX, SI, DI };
const char *WRN[6] = {"ax", "bx", "cx", "dx", "si", "di"};
const int REGS_ORDER[6] = {AX, SI, DI, CX, BX, DX};   // R._REGS
int wr(const string &n) {
  for (int k = 0; k < 6; k++)
    if (n == WRN[k]) return k;
  return -1;
}
const char *BYTEREG[8] = {"al", "bl", "cl", "dl", "ah", "bh", "ch", "dh"};
bool isbyte(const string &n) {
  for (auto b : BYTEREG)
    if (n == b) return true;
  return false;
}
string parent(const string &b8) {   // BYTEPARENT[b8] (KeyError past the eight)
  if (!isbyte(b8)) raise(PY_KEY, "'" + b8 + "'");
  return string(1, b8[0]) + "x";
}
bool in(const string &x, std::initializer_list<const char *> v) {
  for (auto p : v)
    if (x == p) return true;
  return false;
}
const char *HALF(const string &r) {
  if (r == "ax") return "ax|al|ah";
  if (r == "bx") return "bx|bl|bh";
  if (r == "cx") return "cx|cl|ch";
  if (r == "dx") return "dx|dl|dh";
  if (r == "si") return "si";
  if (r == "di") return "di";
  if (r == "es") return "es";
  raise(PY_KEY, "'" + r + "'");
}
string ALU(const string &mn) {
  if (mn == "add") return "+";
  if (mn == "sub") return "-";
  if (mn == "and") return "&";
  if (mn == "or") return "|";
  if (mn == "xor") return "^";
  raise(PY_KEY, "'" + mn + "'");
}
bool isalu(const string &mn) { return in(mn, {"add", "sub", "and", "or", "xor"}); }
const char *JCCs[][2] = {{"jl", "<"}, {"jle", "<="}, {"jg", ">"}, {"jge", ">="}, {"je", "=="}, {"jne", "!="}, {"ja", ">"},
                         {"jae", ">="}, {"jb", "<"}, {"jbe", "<="}, {"jc", "<"}, {"jnc", ">="}};
const char *INVs[][2] = {{"jge", "<"}, {"jg", "<="}, {"jle", ">"}, {"jl", ">="}, {"jne", "=="}, {"je", "!="}, {"jz", "!="},
                         {"jnz", "=="}, {"ja", "<="}, {"jae", "<"}, {"jb", ">="}, {"jbe", ">"}, {"jc", ">="}, {"jnc", "<"}};
const char *jcc_get(const string &m) {
  for (auto &p : JCCs)
    if (m == p[0]) return p[1];
  return nullptr;
}
const char *inv_get(const string &m) {
  for (auto &p : INVs)
    if (m == p[0]) return p[1];
  return nullptr;
}
bool isjcc(const string &m) { return jcc_get(m) != nullptr; }
bool cjmp(const string &m) { return isjcc(m) || in(m, {"jz", "jnz", "js", "jns"}); }
bool fpop(const string &m) {
  return in(m, {"fld", "fild", "fldz", "fld1", "fst", "fstp", "fist", "fistp", "fchs", "fxch", "fadd", "faddp", "fsub",
                "fsubp", "fmul", "fmulp", "fdiv", "fdivp", "fsubr", "fsubrp", "fdivr", "fdivrp", "fiadd", "fisub",
                "fimul", "fidiv", "fisubr", "fidivr", "fwait", "fnop", "finit", "fninit", "fclex", "fnclex"});
}
bool tracked_byte(const string &m) {
  return in(m, {"mov", "add", "sub", "or", "and", "xor", "inc", "dec", "shl", "sal", "shr", "sar", "neg", "not"});
}
const char *NUMRX = "(0x[0-9a-f]+|\\d+)$";
const char *TOKRX = "(?<![\\w>.])(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh)(?![\\w(])";

// an insertion-ordered dict str -> Str (the byte registers)
struct OD {
  std::vector<std::pair<string, Str>> v;
  int find(const string &k) const {
    for (size_t j = 0; j < v.size(); j++)
      if (v[j].first == k) return (int)j;
    return -1;
  }
  bool has(const string &k) const { return find(k) >= 0; }
  Str get(const string &k) const {
    int j = find(k);
    return j < 0 ? Str() : v[j].second;
  }
  const Str &operator[](const string &k) const {
    int j = find(k);
    if (j < 0) raise(PY_KEY, "'" + k + "'");
    return v[j].second;
  }
  void set(const string &k, const Str &x) {
    int j = find(k);
    if (j < 0) v.push_back({k, x});
    else v[j].second = x;
  }
  Str pop(const string &k) {   // pop(k): KeyError when absent
    int j = find(k);
    if (j < 0) raise(PY_KEY, "'" + k + "'");
    Str x = v[j].second;
    v.erase(v.begin() + j);
    return x;
  }
  void pop0(const string &k) {   // pop(k, None)
    int j = find(k);
    if (j >= 0) v.erase(v.begin() + j);
  }
  bool empty() const { return v.empty(); }
};

bool hascall(const Str &v) {   // R._hascall (str(None) has no call)
  if (v.none()) return false;
  static const std::set<string> P = {"BYTE", "HIBYTE", "SX", "S8", "S16", "LONG", "LO", "HI", "LSHR", "LSAR",
                                     "FARM", "FAROFF", "U32", "S32", "USE"};
  for (auto &n : RX("([A-Za-z_]\\w*)\\(").findall(v.s()))
    if (!P.count(n)) return true;
  return false;
}
bool hascall(const string &v) { return hascall(fr(v)); }

string shr(const EcCfg &C, const string &mn, const string &x, const string &n, bool byte = false) {
  if (!C.deadregs) return x + " >> " + n;
  if (mn == "sar") return C.mark ? (byte ? "SAR8(" : "SAR(") + x + ", " + n + ")" : x + " >> " + n;
  return C.mark || byte ? x + " >> " + n : "(unsigned)" + x + " >> " + n;
}

struct Snap {
  Str r[6];
  bool has_div = false;
  Str div0, div1;
  bool retlive = false;
  int axsrc = NI, bxsrc = NI;
  std::vector<Str> args;
  OD breg;
  int i = 0;
};

struct LC {   // _lastcmp: ((a, b), w)
  Str a, b;
  string w;
};

struct Blk {   // flow16's Block, as the model reads it
  int lo, hi;
  long long addr;
  string term;
  bool has_cases;
  string selreg;
  int selbase;
};

struct R {
  const EcCfg &C;
  EcLast &LAST;
  const std::vector<CsIns> &ins;
  long long fs, base;
  const std::map<long long, string> &cxmap;
  bool msc3;
  Str rg[6];
  bool has_div = false;
  Str div0, div1;
  long long maxidx = -1, lmax = -1;
  std::vector<Str> args;
  bool longfn = false, floatfn = false;
  bool has_cmp = false;
  Str cmp0, cmp1;
  std::set<long long> locals;
  OD breg;
  std::set<int> hoisted;
  List out;
  int i = 0;
  bool retlive = false;
  int axsrc = NI, bxsrc = NI;
  std::vector<long long> ranks;
  std::vector<bool> regconst;
  bool callstmt = false;
  std::set<long long> gototargets;
  bool emitgoto = false;
  std::map<int, int> tgt;
  bool has_epi = false;
  long long epi = 0, pbase = 4, retn = 0;
  // the attributes the Python sets on the way (getattr(s, "_x", default))
  Str cbw;
  bool s8_on = false, sm_on = false, p8_on = false, pw_on = false;
  int s8_i = 0, sm_i = 0, p8_i = 0, pw_i = 0;
  Str s8_v, sm_v, p8_v, pw_v;
  long long tn = 0;
  bool has_fst = false;
  std::vector<string> fst;
  int cond_lo = 0;
  bool has_lastcmp = false;
  LC lastcmp;
  std::map<int, LC> cmpat;
  int blockcmp = NI;
  int rv = -1;
  int emit_start = 0;
  int range_hi = -1;
  std::set<long long> labels, labeled;
  std::map<long long, long long> gotoargs;
  std::vector<std::set<string>> loops;
  bool retlin = false;
  int firstlin = NI;
  const std::vector<Blk> *blocks = nullptr;

  R(const EcCfg &c, EcLast &last, const std::vector<CsIns> &in_, long long fs_, long long base_,
    const std::map<long long, string> &cx, bool m3)
      : C(c), LAST(last), ins(in_), fs(fs_), base(base_), cxmap(cx), msc3(m3) {
    std::unordered_map<long long, int> a2i;
    for (size_t k = 0; k < ins.size(); k++) a2i[ins[k].address] = (int)k;
    for (size_t k = 0; k < ins.size(); k++) {
      const CsIns &ii = ins[k];
      if (cjmp(ii.mnemonic) || ii.mnemonic == "jmp") {
        if (isnum(ii.op_str)) {
          auto it = a2i.find(int0(ii.op_str));
          tgt[(int)k] = it == a2i.end() ? NI : it->second;
        }
      }
    }
    if (!ins.empty() && in(ins.back().mnemonic, {"ret", "retf"})) {
      long long e = ins.back().address;
      long long j = (long long)ins.size() - 2;
      while (j >= 0 && (in(ins[j].mnemonic, {"leave", "pop"}) || (ins[j].mnemonic == "mov" && ins[j].op_str == "sp, bp"))) {
        e = ins[j].address;
        j--;
      }
      has_epi = true;
      epi = e;
    }
    pbase = 4;
    for (auto &x : ins)
      if (x.mnemonic == "retf") pbase = 6;
    retn = 0;
    for (auto &x : ins)
      if (in(x.mnemonic, {"ret", "retf"}) && isnum(x.op_str)) {
        retn = int0(x.op_str);
        break;
      }
    if (retn) maxidx = floordiv(retn, 2) - 1;
  }

  // -------------------------------------------------------------------------------- helpers
  const CsIns &I(long long k) const { return at(ins, k); }
  long long N() const { return (long long)ins.size(); }
  Str *regp(const string &n) {
    int k = wr(n);
    return k < 0 ? nullptr : &rg[k];
  }
  Str getr(const string &n) {   // getattr(s, n, None) for a register name
    Str *p = regp(n);
    return p ? *p : Str();
  }
  void setr(const string &n, const Str &v) {
    Str *p = regp(n);
    if (p) *p = v;
  }
  int tget(int j) const {   // s.tgt.get(j)
    auto it = tgt.find(j);
    return it == tgt.end() ? NI : it->second;
  }
  int tat(int j) const {   // s.tgt[j]
    auto it = tgt.find(j);
    if (it == tgt.end()) raise(PY_KEY, std::to_string(j));
    return it->second;
  }
  const CsIns &Iv(int k) const {   // s.ins[k] where k may be None
    if (k == NI) raise(PY_TYPE, "list indices must be integers or slices, not NoneType");
    return I(k);
  }
  string newt() {
    tn++;
    return "t" + std::to_string(tn);
  }
  void push_out(const List &o, const string &t, long long rank, bool rc = false) {
    if (!o) raise(PY_ATTR, "'NoneType' object has no attribute 'append'");
    o->push_back(t);
    ranks.push_back(rank);
    regconst.push_back(rc);
  }
  bool saved_has(const string &r) const {
    if (!LAST.has_saved) return false;
    for (auto &x : LAST.saved)
      if (x == r) return true;
    return false;
  }
  long long num(const string &x) const { return int0(x); }

  string call_op(int ci, const string &def) {
    const CsIns &c = I(ci);
    const auto &b = c.bytes;
    if (b.size() < 3 || b[0] != 0xe8) return def;
    long long rel = b[1] | (b[2] << 8);
    if (rel >= 0x8000) rel -= 0x10000;
    long long t = (base + c.address + 3 + rel) & 0xffff;
    auto it = cxmap.find(t);
    return it == cxmap.end() ? def : it->second;
  }
  string pname(long long off) {
    long long pb = pbase;
    long long idx = msc3 ? floordiv(off - pb, 2) : floordiv(off - fs - pb, 2);
    if (retn && 0 <= idx && idx < floordiv(retn, 2)) idx = floordiv(retn, 2) - 1 - idx;
    maxidx = std::max(maxidx, idx);
    return (0 <= idx && idx < 26) ? cbinv::chr('a' + idx) : "p" + std::to_string(idx);
  }
  string ptrbase(const string &reg) {
    Str v = getr(reg);
    if (v.none() || mt("-?\\d+$|-?0x[0-9a-f]+$", v.s())) return reg;
    const string &s = v.s();
    if (mt("^[A-Za-z_]\\w*(->f\\d+)*$", s) || (starts(s, "(") && ends(s, ")"))) return s;
    return "(" + s + ")";
  }
  static string plus2(const string &op) {
    auto m = sr("\\[(.*)\\]", op);
    if (!m) return op;
    string e = g(m, 1);
    auto m2 = mt("(.*) ([+-]) (0x[0-9a-f]+|\\d+)$", e);
    if (m2) {
      long long v = int0(g(m2, 3)) * (g(m2, 2) == "+" ? 1 : -1) + 2;
      e = v == 0 ? g(m2, 1) : g(m2, 1) + " " + (v > 0 ? "+" : "-") + " 0x" + X(std::llabs(v));
    } else if (mt("(0x[0-9a-f]+|\\d+)$", e)) {
      e = "0x" + X(int0(e) + 2);
    } else {
      e = e + " + 2";
    }
    return op.substr(0, m.start()) + "[" + e + "]" + op.substr(m.end());
  }

  // ------------------------------------------------------------------------------ the 8087
  int fp_step(int i0, const List &o) {
    const CsIns &ii = I(i0);
    const string &mn = ii.mnemonic, &ops = ii.op_str;
    has_fst = true;
    auto &st = fst;
    LAST.has_fptypes = true;
    if (in(mn, {"wait", "fwait", "fnop", "finit", "fninit", "fclex", "fnclex"})) return i0 + 1;
    auto mem = [&](const string &op, bool integer) -> Str {
      auto m = mt("(?:(\\w+):\\s*)?(byte|word|dword|qword|tbyte) ptr (.*)$", op);
      if (!m) return Str();
      string size = g(m, 2);
      Str nm = memname((size != "byte" ? "word ptr " : "byte ptr ") + g(m, 3));
      if (nm.none()) return Str();
      string ty;
      if (integer) ty = size == "word" ? "int" : (size == "dword" || size == "qword") ? "long" : "";
      else ty = size == "dword" ? "float" : (size == "qword" || size == "tbyte") ? "double" : "";
      if (!ty.empty() && C.fpcasts && sr("->|\\[|\\.", nm.s()) && ty != "int") return fr("*(" + ty + " *)&" + nm.s());
      if (!ty.empty()) {
        bool f = false;
        for (auto &kv : LAST.fptypes)
          if (kv.first == nm.s()) f = true;
        if (!f) LAST.fptypes.push_back({nm.s(), ty});
      }
      return nm;
    };
    auto sti = [&](const string &op) -> int {
      auto m = fm("st\\((\\d)\\)", strip(op));
      if (m) return (int)int0(g(m, 1));
      return strip(op) == "st" ? 0 : -1;
    };
    auto idx = [&](long long k) -> string & {   // st[k], negative from the end
      return at(st, k);
    };
    const char *OPS = nullptr;
    auto opsof = [](const string &b) -> const char * {
      if (in(b, {"fadd", "fiadd"})) return "+";
      if (in(b, {"fsub", "fisub", "fsubr", "fisubr"})) return "-";
      if (in(b, {"fmul", "fimul"})) return "*";
      if (in(b, {"fdiv", "fidiv", "fdivr", "fidivr"})) return "/";
      return nullptr;
    };
    if (in(mn, {"fld", "fild"})) {
      int k = sti(ops);
      if (k >= 0) {
        if (k >= (int)st.size()) return 0;
        string v = idx(-1 - k);
        st.push_back(v);
        return i0 + 1;
      }
      Str x = mem(ops, mn == "fild");
      if (x.none()) return 0;
      st.push_back(x.s());
      return i0 + 1;
    }
    if (in(mn, {"fldz", "fld1"})) {
      st.push_back(mn == "fldz" ? "0.0" : "1.0");
      return i0 + 1;
    }
    if (in(mn, {"fst", "fstp", "fist", "fistp"})) {
      if (st.empty()) return 0;
      int k = sti(ops);
      bool p = ends(mn, "p");
      if (k >= 0) {
        string v = st.back();
        if (k) idx(-1 - k) = v;
        if (p) st.pop_back();
        return i0 + 1;
      }
      int j = i0 + 1;
      while (j < N() && in(ins[j].mnemonic, {"wait", "fwait", "nop"})) j++;
      auto m = mt("qword ptr \\[(0x[0-9a-f]+|\\d+)\\]$", ops);
      if (m && j < N() && ins[j].mnemonic == "mov" && ins[j].op_str == "ax, " + g(m, 1)) {
        LAST.has_fpret = true;
        LAST.fpret = "double";
        emit(o, "return " + st.back() + ";");
        if (p) st.pop_back();
        return j + 1;
      }
      Str x = mem(ops, starts(mn, "fi"));
      if (x.none()) return 0;
      emit(o, x.s() + " = " + st.back() + ";");
      if (p) st.pop_back();
      return i0 + 1;
    }
    if (mn == "fchs") {
      if (st.empty()) return 0;
      st.back() = "(-" + st.back() + ")";
      return i0 + 1;
    }
    if (mn == "fxch") {
      int k = sti(ops);
      if (k <= 0) k = 1;
      if (k >= (int)st.size()) return 0;
      std::swap(st.back(), idx(-1 - k));
      return i0 + 1;
    }
    string b = (ends(mn, "p") && opsof(mn.substr(0, mn.size() - 1))) ? mn.substr(0, mn.size() - 1) : mn;
    OPS = opsof(b);
    if (OPS) {
      string op = OPS;
      bool rev = ends(b, "r");
      std::vector<string> parts;
      if (!ops.empty())
        for (auto &x : split(ops, ",")) parts.push_back(strip(x));
      if (parts.size() == 1 && sti(parts[0]) < 0) {
        if (st.empty()) return 0;
        Str x = mem(parts[0], starts(b, "fi"));
        if (x.none()) return 0;
        st.back() = rev ? "(" + x.s() + " " + op + " " + st.back() + ")" : "(" + st.back() + " " + op + " " + x.s() + ")";
        return i0 + 1;
      }
      if (parts.empty()) parts = {"st(1)", "st"};
      if (parts.size() == 1) parts = ends(mn, "p") ? std::vector<string>{parts[0], "st"} : std::vector<string>{"st", parts[0]};
      int d = sti(at(parts, 0)), src = sti(at(parts, 1));
      if (d < 0 || src < 0 || std::max(d, src) >= (int)st.size()) return 0;
      string a = idx(-1 - d), bb = idx(-1 - src);
      idx(-1 - d) = rev ? "(" + bb + " " + op + " " + a + ")" : "(" + a + " " + op + " " + bb + ")";
      if (ends(mn, "p")) st.pop_back();
      return i0 + 1;
    }
    return 0;
  }

  // ------------------------------------------------------------------------------ memory names
  Str memname(const string &op) {
    Str r = memname0(op);
    if (C.mark && C.farptr && !r.none() && sr("\\bes:", op)) r = fr("FARM(es, " + r.s() + ")");
    if (C.mark && !r.none() && has(op, "byte ptr") && has(op, "[")) return fr("BYTE(" + r.s() + ")");
    return r;
  }
  Str memname0(const string &op0) {
    string op = replace_all(replace_all(op0, "word ptr ", ""), "byte ptr ", "");
    op = RX("\\b(es|ds|ss|cs|fs|gs):").sub("", op);
    if (msc3) {
      auto m = sr("\\[bp - (0x[0-9a-f]+|\\d+)\\]", op);
      if (m) {
        long long o = num(g(m, 1));
        locals.insert(o);
        return fr("v" + D(o));
      }
      m = sr("\\[bp \\+ (0x[0-9a-f]+|\\d+)\\]", op);
      if (m) {
        long long o = num(g(m, 1));
        string nm = pname(o);
        return fr(((o - pbase) % 2) ? "HIBYTE(" + nm + ")" : nm);
      }
    } else {
      auto m = sr("\\[bp \\+ (0x[0-9a-f]+|\\d+)\\]", op);
      if (m || has(op, "[bp]")) {
        long long o = m ? num(g(m, 1)) : 0;
        if (o < fs) {
          locals.insert(o);
          return fr("v" + D(o));
        }
        string nm = pname(o);
        return fr(((o - fs - pbase) % 2) ? "HIBYTE(" + nm + ")" : nm);
      }
    }
    auto m = sr("\\[bp - (0x[0-9a-f]+|\\d+)\\]", op);
    if (m) {
      long long o = num(g(m, 1));
      locals.insert(o);
      return fr("v" + D(o));
    }
    for (const char *reg : {"si", "bx", "di"}) {
      Str cv = getr(reg);
      string R_ = reg;
      if (!cv.none() && mt("\\d+$", cv.s())) {
        auto m1 = sr("\\[" + R_ + "(?: ([+-]) (0x[0-9a-f]+|\\d+))?\\]", op);
        if (m1) {
          long long d = m1.has(2) ? num(g(m1, 2)) : 0;
          long long base_ = int0(cv.s());
          bool minus = m1.has(1) && g(m1, 1) == "-";
          return fr("g_" + X((base_ + (!minus ? d : -d)) & 0xffff));
        }
      }
      auto m2 = sr("\\[" + R_ + " \\+ (0x[0-9a-f]+|\\d+)\\]", op);
      if (m2 && msc3 && !cv.none() && !cbw.none() && cv.s() == cbw.s() && num(g(m2, 1)) >= 0x40)
        return fr("g_" + X(num(g(m2, 1))) + "[" + cv.s() + "]");
      if (m2) return fr(ptrbase(reg) + "->f" + D(num(g(m2, 1))));
      auto m3 = sr("\\[" + R_ + " - (0x[0-9a-f]+|\\d+)\\]", op);
      if (m3) return fr(ptrbase(reg) + "[-" + D(num(g(m3, 1))) + "]");
      if (has(op, "[" + R_ + "]")) return fr("*" + ptrbase(reg));
    }
    m = sr("\\[(bx|bp|si|di) \\+ (si|di) ([+-]) (0x[0-9a-f]+|\\d+)\\]", op);
    if (m) return fr(ptrbase(g(m, 1)) + "[" + ptrbase(g(m, 2)) + " " + g(m, 3) + " " + D(num(g(m, 4))) + "]");
    m = sr("\\[bx \\+ si\\]|\\[bx \\+ di\\]|\\[si \\+ di\\]", op);
    if (m) {
      string tail = at(split(op, " + "), 1);
      while (!tail.empty() && tail.back() == ']') tail.pop_back();
      return fr(ptrbase(sl(op, 1, 3)) + "[" + ptrbase(tail) + "]");
    }
    m = sr("\\[(0x[0-9a-f]+|\\d+)\\]", op);
    if (m) return fr("g_" + X(num(g(m, 1))));
    string t = strip(op);
    if (in(t, {"ax", "bx", "cx", "dx", "si", "di", "bp", "ds", "es", "ss", "cs"})) return fr(t);
    return Str();
  }
  Str rhs(const string &op) { return isnum(op) ? fromint(num(op)) : memname(op); }

  void movptr(const Str &d, const string &src) {
    const string &dn = d.s();
    if (in(src, {"ax", "bx", "cx", "dx", "si", "di", "bp", "sp"})) {
      Str val = getr(src);
      if (val.none()) val = fr(src);
      if (in(val.s(), {"ax", "bx", "cx", "dx", "si", "di", "bp", "sp"}) || has(val.s(), "(")) {
        emit(out, dn + " = " + val.s() + ";");
        setr(dn, d);
        if (has(val.s(), "(") && wr(src) >= 0) {
          setr(src, d);
          if (src == "ax") axsrc = NI;
        }
      } else {
        setr(dn, val);
      }
    } else if (isnum(src)) {
      setr(dn, fromint(num(src)));
    } else if (msc3 &&
               ((sr("\\[(?:si|di)(?: [+-] (?:0x[0-9a-f]+|\\d+))?\\]", src) && sl(at(split(src, "["), 1), 0, 2) != dn) ||
                (fm("word ptr \\[(?:bp [+-] (?:0x[0-9a-f]+|\\d+)|bx(?: \\+ (?:0x[0-9a-f]+|\\d+))?|0x[0-9a-f]+|\\d+)\\]", src) &&
                 var_later(dn))) &&
               !memname(src).none() && out) {
      emit(out, dn + " = " + S(memname(src)) + ";");
      setr(dn, d);
    } else {
      setr(dn, memname(src));
    }
  }

  static bool writes(const string &mn, const string &dst, const string &reg) {
    if (dst == reg) return true;
    if ((reg == "ax" || reg == "dx") && in(mn, {"imul", "idiv", "mul", "div", "cwd"})) return true;
    if (reg == "ax" && in(mn, {"call", "lcall"})) return true;
    return false;
  }
  int future_store(const string &reg, int i0) {
    for (long long j = i0 + 1; j < N(); j++) {
      const string &mn = ins[j].mnemonic, &ops = ins[j].op_str;
      string dst = split(ops, ", ")[0];
      if (mn == "mov" && starts(replace_all(replace_all(dst, "word ptr ", ""), "byte ptr ", ""), "[") && ends(ops, ", " + reg))
        return (int)j;
      if (writes(mn, dst, reg)) return NI;
    }
    return NI;
  }
  Str rsrc(const string &reg) {
    Str cur = getr(reg);
    if (!cur.none() && mt("-?\\d+$|-?0x[0-9a-f]+$", cur.s())) {
      int j = future_store(reg, i);
      if (j != NI) {
        Str var = memname(split(ins[j].op_str, ", ")[0]);
        if (!hoisted.count(j) && out) {
          emit(out, S(var) + " = " + cur.s() + ";");
          hoisted.insert(j);
        }
        setr(reg, var);
        return var;
      }
    }
    return !cur.none() ? cur : fr(reg);
  }
  Str val(const string &tok) {
    if (tok == "ax") return !rg[AX].none() ? rg[AX] : lit("ax");
    if (tok == "si" || tok == "di") {
      Str v = getr(tok);
      return !v.none() ? v : fr(tok);
    }
    return in(tok, {"bx", "cx", "dx"}) ? rsrc(tok) : rhs(tok);
  }

  // ------------------------------------------------------------------------------ liveness
  bool live(const string &reg, int i0) {
    re::Regex rx("\\b(" + string(HALF(reg)) + ")\\b");
    std::set<int> seen;
    std::vector<int> work = {i0 + 1};
    while (!work.empty()) {
      int j = work.back();
      work.pop_back();
      while (j != NI && 0 <= j && j < N() && !seen.count(j)) {
        seen.insert(j);
        const CsIns &ii = ins[j];
        const string &mn = ii.mnemonic, &ops = ii.op_str;
        auto pr = partition(ops, ", ");
        const string &d0 = pr.first, &src = pr.second;
        if (in(mn, {"ret", "retf"})) {
          if (reg == "ax" && retval()) return true;
          break;
        }
        if (in(mn, {"mov", "lea", "les", "lds", "pop"}) && d0 == reg) {
          if (rx.search(src)) return true;
          break;
        }
        if (in(mn, {"xor", "sub"}) && d0 == reg && src == reg) break;
        if (rx.search(ops)) return true;
        if (in(mn, {"call", "lcall"}) && in(reg, {"ax", "bx", "cx", "dx"})) break;
        if (mn == "jmp") {
          j = tget(j);
          continue;
        }
        if (cjmp(mn)) work.push_back(tget(j));
        j++;
      }
    }
    return false;
  }

  // _wregs: the registers in CPython's set order (names: ax bx cx dx si di es = 0..6)
  std::vector<string> wregs(const string &mn, const string &ops) {
    static const char *NM[7] = {"ax", "bx", "cx", "dx", "si", "di", "es"};
    int slot[8];
    for (int &x : slot) x = -1;
    int used = 0;
    auto add = [&](const string &n) {
      int k = -1;
      for (int q = 0; q < 7; q++)
        if (n == NM[q]) k = q;
      if (k < 0) return;
      unsigned long long h = (unsigned long long)C.hash[k], perturb = h;
      size_t i0 = (size_t)(h & 7);
      for (;;) {
        if (slot[i0] < 0) {
          slot[i0] = k;
          used++;
          return;
        }
        if (slot[i0] == k) return;
        perturb >>= 5;
        i0 = (size_t)((i0 * 5 + 1 + perturb) & 7);
      }
    };
    auto result = [&]() {
      std::vector<string> o;
      for (int q = 0; q < 8; q++)
        if (slot[q] >= 0) o.push_back(NM[slot[q]]);
      return o;
    };
    string base_ = at(wsplit(mn), -1);
    static const std::map<string, std::vector<string>> IMPLW = {
        {"mul", {"ax", "dx"}}, {"imul", {"ax", "dx"}}, {"div", {"ax", "dx"}}, {"idiv", {"ax", "dx"}},
        {"cwd", {"dx"}}, {"cdq", {"dx"}}, {"cbw", {"ax"}}, {"cwde", {"ax"}}, {"xlatb", {"ax"}},
        {"lodsb", {"ax", "si"}}, {"lodsw", {"ax", "si"}}, {"stosb", {"di"}}, {"stosw", {"di"}},
        {"movsb", {"si", "di"}}, {"movsw", {"si", "di"}}, {"scasb", {"di"}}, {"scasw", {"di"}},
        {"cmpsb", {"si", "di"}}, {"cmpsw", {"si", "di"}}, {"loop", {"cx"}},
        {"call", {"ax", "bx", "cx", "dx"}}, {"lcall", {"ax", "bx", "cx", "dx"}}};
    auto it = IMPLW.find(base_);
    if (it != IMPLW.end())
      for (auto &x : it->second) add(x);
    if (starts(mn, "rep")) add("cx");
    if (in(base_, {"cmp", "test", "push", "jmp", "ret", "retf", "nop", "mul", "imul", "div", "idiv"}) || cjmp(base_) ||
        it != IMPLW.end())
      return result();
    std::vector<string> p;
    if (!ops.empty())
      for (auto &x : split(ops, ",")) p.push_back(strip(x));
    if (C.mark && C.farptr && (base_ == "les" || (!p.empty() && p[0] == "es" && in(base_, {"mov", "pop"})))) add("es");
    if (!p.empty()) {
      string d = isbyte(p[0]) ? parent(p[0]) : p[0];
      if (wr(d) >= 0) add(d);
      if (base_ == "xchg" && p.size() > 1) {
        d = isbyte(p[1]) ? parent(p[1]) : p[1];
        if (wr(d) >= 0) add(d);
      }
    }
    (void)used;
    return result();
  }

  void kill_aliases(int i0) {
    const CsIns &ii = I(i0);
    auto w = wregs(ii.mnemonic, ii.op_str);
    if (w.empty() || !out) return;
    for (auto &W : w) {
      re::Regex rx("(?<![\\w>.])(" + string(HALF(W)) + ")(?![\\w(])");
      for (int O = 0; O < 6; O++) {
        if (W == WRN[O]) continue;
        Str v = rg[O];
        if (v.none() || v.s() == WRN[O]) continue;
        if (rx.search(v.s())) {
          if (live(WRN[O], i0 - 1) || hascall(v)) materialize(out, WRN[O]);
          else rg[O] = Str();
        }
      }
      auto items = breg.v;
      for (auto &kv : items) {
        if (parent(kv.first) != W && rx.search(S(kv.second))) {
          if (live(parent(kv.first), i0 - 1)) push_out(out, kv.first + " = " + S(kv.second) + ";", i0);
          breg.pop0(kv.first);
        }
      }
      for (size_t k = 0; k < args.size(); k++) {
        if (in(ii.mnemonic, {"call", "lcall"})) break;
        if (rx.search(S(args[k]))) {
          string t = newt();
          push_out(out, t + " = " + S(args[k]) + ";", i0);
          args[k] = fr(t);
        }
      }
    }
  }

  void pin(const List &o, const std::set<string> &regs) {
    std::vector<int> batch;
    for (int r = 0; r < 6; r++)
      if (regs.count(WRN[r]) && !rg[r].none() && rg[r].s() != WRN[r] && (live(WRN[r], i) || hascall(rg[r])))
        batch.push_back(r);
    if (batch.size() > 1) {
      bool any = false;
      for (int a : batch)
        for (int b2 : batch)
          if (a != b2 && !any && srd("(?<![\\w>.])(" + string(HALF(WRN[a])) + ")(?![\\w(])", S(rg[b2]))) any = true;
      if (any)
        for (int r : batch) {
          string t = newt();
          push_out(o, t + " = " + S(rg[r]) + ";", i);
          rg[r] = fr(t);
        }
    }
    for (int r = 0; r < 6; r++) {
      if (!regs.count(WRN[r])) continue;
      std::vector<string> bs;
      for (auto &kv : breg.v)
        if (parent(kv.first) == WRN[r]) bs.push_back(kv.first);
      for (auto &b8 : bs) {
        Str v = breg.pop(b8);
        if (S(v) != b8 && live(WRN[r], i)) push_out(o, b8 + " = " + S(v) + ";", i);
      }
      Str v = rg[r];
      if (v.none() || v.s() == WRN[r]) continue;
      if (live(WRN[r], i) || hascall(v)) materialize(o, WRN[r]);
      else rg[r] = Str();
    }
  }

  std::set<string> mem_lazy() {
    std::set<string> o;
    for (int r = 0; r < 6; r++) {
      Str v = rg[r];
      if (!v.none() && v.s() != WRN[r] && !mt("-?\\d+$", v.s())) o.insert(WRN[r]);
    }
    for (auto &kv : breg.v)
      if (!mt("-?\\d+$", S(kv.second))) o.insert(parent(kv.first));
    return o;
  }

  void tree_bids(const Seq &nodes, std::set<int> &acc) {
    for (auto &n : nodes) {
      switch (n.kind) {
        case Node::BLOCK: acc.insert(n.a); break;
        case Node::IF: acc.insert(n.a); tree_bids(n.s1, acc); break;
        case Node::IFELSE: acc.insert(n.a); tree_bids(n.s1, acc); tree_bids(n.s2, acc); break;
        case Node::WHILE:
        case Node::LOOP: acc.insert(n.a); tree_bids(n.s1, acc); break;
        case Node::SWITCH:
          acc.insert(n.a);
          for (auto &c : n.cases) tree_bids(c.second, acc);
          break;
        case Node::DOWHILE: acc.insert(n.a); acc.insert(n.join); tree_bids(n.s1, acc); break;
        default: break;
      }
    }
  }
  bool has_break(const Seq &nodes) {
    for (auto &n : nodes) {
      if (n.kind == Node::BREAK) return true;
      if (n.kind == Node::IF && has_break(n.s1)) return true;
      if (n.kind == Node::IFELSE && (has_break(n.s1) || has_break(n.s2))) return true;
      if (n.kind == Node::SWITCH)
        for (auto &c : n.cases)
          if (has_break(c.second)) return true;
    }
    return false;
  }
  std::set<string> loop_written(const std::set<int> &bids) {
    std::set<string> w;
    for (int b : bids) {
      const Blk &bl = at(*blocks, b);
      for (int k = bl.lo; k < bl.hi; k++)
        for (auto &x : wregs(ins[k].mnemonic, ins[k].op_str)) w.insert(x);
    }
    return w;
  }

  bool wlive(const string &reg, int i0) {
    re::Regex rx("\\b" + reg + "\\b");
    std::set<int> seen;
    std::vector<int> work = {i0 + 1};
    while (!work.empty()) {
      int j = work.back();
      work.pop_back();
      while (j != NI && 0 <= j && j < N() && !seen.count(j)) {
        seen.insert(j);
        const CsIns &ii = ins[j];
        const string &mn = ii.mnemonic, &ops = ii.op_str;
        auto pr = partition(ops, ", ");
        if (in(mn, {"ret", "retf"})) {
          if (reg == "ax" && retval()) return true;
          break;
        }
        if (in(mn, {"mov", "lea", "les", "lds", "pop"}) && pr.first == reg) {
          if (rx.search(pr.second)) return true;
          break;
        }
        if (in(mn, {"xor", "sub"}) && pr.first == reg && pr.second == reg) break;
        if (in(mn, {"cbw", "cwde"}) && reg == "ax") break;
        if (in(mn, {"call", "lcall"})) break;
        if (rx.search(ops)) return true;
        if (mn == "jmp") {
          j = tget(j);
          continue;
        }
        if (cjmp(mn)) work.push_back(tget(j));
        j++;
      }
    }
    return false;
  }

  void byte_settle(const string &d8, int i0) {
    string par = parent(d8);
    if (!getr(par).none() || !out || !breg.has(d8)) return;
    if (wlive(par, i0)) {
      Str v = breg.pop(d8);
      if (S(v) != d8) push_out(out, d8 + " = " + S(v) + ";", i0);
    }
  }

  void materialize(const List &o, const string &reg, long long rank = LLONG_MIN) {
    Str v = getr(reg);
    if (v.none() || v.s() == reg) return;
    re::Regex rx("(?<![\\w>.])(" + string(HALF(reg)) + ")(?![\\w(])");
    string sv = v.s();
    for (size_t k2 = 0; k2 < args.size(); k2++) {
      string a2 = S(args[k2]);
      if (has(a2, sv) && sv.size() > reg.size()) args[k2] = fr(replace_all(a2, sv, reg));
      else if (rx.search(a2)) {
        string t = newt();
        push_out(o, t + " = " + a2 + ";", i);
        args[k2] = fr(t);
      }
    }
    std::vector<std::pair<string, string>> batch;
    for (int q = 0; q < 6; q++) {
      string on = WRN[q];
      if (on == reg) continue;
      Str ov = rg[q];
      if (ov.none() || ov.s() == on) continue;
      if (has(ov.s(), sv) && sv.size() > reg.size()) rg[q] = fr(replace_all(ov.s(), sv, reg));
      else if (rx.search(ov.s())) batch.push_back({on, ov.s()});
    }
    if (!batch.empty()) {
      batch.push_back({reg, sv});
      std::vector<std::pair<string, string>> tmp;
      for (auto &b : batch) {
        string t = newt();
        tmp.push_back({b.first, t});
        push_out(o, t + " = " + b.second + ";", i);
      }
      for (auto &b : tmp) {
        push_out(o, b.first + " = " + b.second + ";", i);
        if (b.first != reg) setr(b.first, b.first == "ax" ? lit("ax") : Str());
      }
    } else {
      push_out(o, reg + " = " + sv + ";", rank == LLONG_MIN ? i : rank);
    }
    setr(reg, reg == "ax" ? lit("ax") : Str());
  }

  void emit(const List &o, string text, long long rank = LLONG_MIN, bool rc = false) {
    for (int r = 0; r < 6; r++) {
      Str v = rg[r];
      if (!v.none() && v.s() != WRN[r] && hascall(v) && !has(text, v.s())) materialize(o, WRN[r], rank);
    }
    for (size_t k = 0; k < args.size(); k++)
      if (hascall(args[k]) && !has(text, S(args[k]))) {
        string t = newt();
        push_out(o, t + " = " + S(args[k]) + ";", i);
        args[k] = fr(t);
      }
    if (!args.empty() && !mt("t\\d+ = ", text) && (mt("[^=(]*[^=!<>]=[^=]", text) || sr("\\+\\+|--", text)) &&
        !mt("(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh) = ", text)) {
      for (size_t k = 0; k < args.size(); k++)
        if (sr("g_|\\*|->|\\[", S(args[k]))) {
          string t = newt();
          push_out(o, t + " = " + S(args[k]) + ";", i);
          args[k] = fr(t);
        }
    }
    for (int r = 0; r < 6; r++) {
      Str v = rg[r];
      if (v.none() || v.s() == WRN[r] || !hascall(v) || !has(text, v.s())) continue;
      if (ends(text, " = " + v.s() + ";")) continue;
      if (live(WRN[r], i)) {
        materialize(o, WRN[r], rank);
        text = replace_all(text, v.s(), WRN[r]);
      } else {
        rg[r] = Str();
      }
    }
    auto m = mt("(.+?)(?: = (.*);|\\+\\+;|--;)$", text);
    if (m) {
      string g1 = g(m, 1);
      string g2 = m.has(2) ? g(m, 2) : "";
      string core = RX("^BYTE\\((.*)\\)$").sub("\\1", g1);
      re::Regex pat8("(?<![\\w>.])" + re::escape(core) + "(?![\\w(])");
      auto items = breg.v;
      for (auto &kv : items) {
        string sv = S(kv.second);
        if (sv != kv.first && pat8.search(sv) && sv == g2) {
          breg.set(kv.first, fr(g1));
          continue;
        }
        if (sv != kv.first && pat8.search(sv) && sv != g2) {
          breg.pop(kv.first);
          if (live(parent(kv.first), i)) push_out(o, kv.first + " = " + sv + ";", i);
        }
      }
    }
    if (C.mark && m && sr("->|\\*|\\[|g_", g(m, 1))) {
      for (int r = 0; r < 6; r++) {
        Str v = rg[r];
        if (v.none() || v.s() == WRN[r] || (m.has(2) && v.s() == g(m, 2))) continue;
        if (sr("->|\\*|\\[|g_", v.s()) && live(WRN[r], i)) materialize(o, WRN[r], rank);
      }
    }
    if (m && wr(g(m, 1)) < 0 && !isbyte(g(m, 1))) {
      string lhs = RX("^BYTE\\((.*)\\)$").sub("\\1", g(m, 1));
      re::Regex pat("(?<![\\w>.])" + re::escape(lhs) + "(?![\\w(])");
      for (int r = 0; r < 6; r++) {
        Str v = rg[r];
        if (v.none() || v.s() == WRN[r] || (m.has(2) && v.s() == g(m, 2))) continue;
        if (pat.search(v.s()) && live(WRN[r], i)) materialize(o, WRN[r], rank);
      }
    }
    push_out(o, text, rank == LLONG_MIN ? i : rank, rc);
  }

  void binval(const string &mn, const string &d0) {
    if (in(mn, {"call", "lcall"})) {
      breg.pop0("al");
      breg.pop0("ah");
    } else if (in(d0, {"ax", "bx", "cx", "dx"}) && !in(mn, {"cmp", "test", "push"})) {
      breg.pop0(string(1, d0[0]) + "l");
      breg.pop0(string(1, d0[0]) + "h");
    } else if (isbyte(d0) && !in(mn, {"cmp", "test", "push"})) {
      setr(parent(d0), Str());
      if (!tracked_byte(mn)) breg.pop0(d0);
    }
  }

  bool byte_on_word(const string &mn, const string &ops) {
    if (!in(mn, {"and", "or", "xor"}) || !has(ops, ", ")) return false;
    auto p = splitn(ops, ", ", 1);
    const string &d8 = p[0], &src = p[1];
    if (!isbyte(d8) || breg.has(d8) || !isnum(src)) return false;
    string par = parent(d8);
    Str cur = getr(par);
    if (!in(par, {"ax", "bx", "cx", "dx"}) || cur.none()) return false;
    long long k = num(src);
    bool hi = ends(d8, "h");
    if (mn == "and") k = hi ? ((k << 8) | 0xFF) : (0xFF00 | k);
    else if (hi) k <<= 8;
    setr(par, fr("(" + cur.s() + " " + ALU(mn) + " 0x" + X(k) + ")"));
    if (par == "ax") axsrc = NI, retlive = true;
    if (par == "bx") bxsrc = NI;
    return true;
  }

  static bool wordish(const string &v) { return (bool)fm("[\\w\\->.]+", v); }

  void expr_step(int i0) {
    i = i0;
    Str old = rg[AX];
    const CsIns &ii = I(i0);
    const string &mn = ii.mnemonic, &ops = ii.op_str;
    if (in(mn, {"or", "and", "test"}) && count(ops, ",") == 1 && !has(ops, "[") &&
        strip(split(ops, ",")[0]) == strip(split(ops, ",")[1]))
      return;
    kill_aliases(i0);
    if (byte_on_word(mn, ops)) return;
    string d_ = strip(split(ops, ",")[0]);
    Str sv_;
    bool has_sv = false;
    if (mn == "mov" && count(ops, ", ") == 1 && isbyte(split(ops, ", ")[0]) && isbyte(split(ops, ", ")[1])) {
      sv_ = (s8_on && s8_i == i0) ? s8_v : bval(split(ops, ", ")[1]);
      has_sv = true;
    }
    if (!(sm_on && sm_i == i0)) {
      auto o2 = splitn(ops, ", ", 1);
      if (mn == "mov" && isbyte(d_) && o2.size() == 2 && has(o2[1], "[")) {
        sm_on = true;
        sm_i = i0;
        sm_v = memname(o2[1]);
      } else {
        sm_on = false;
      }
    }
    Str pre8;
    if (p8_on && p8_i == i0) pre8 = p8_v;
    else if (isbyte(d_)) pre8 = bval(d_);
    binval(mn, d_);
    if (mn == "mov" && isbyte(split(ops, ", ")[0])) {
      string d8 = split(ops, ", ")[0];
      string src = at(splitn(ops, ", ", 1), 1);
      if (isnum(src)) breg.set(d8, fromint(num(src)));
      else if (isbyte(src)) breg.set(d8, has_sv && !sv_.none() ? sv_ : (breg.has(src) ? breg.get(src) : fr(src)));
      else {
        Str v = (sm_on && sm_i == i0 && truthy(sm_v)) ? sm_v : memname(src);
        breg.set(d8, truthy(v) ? v : fr(d8));
      }
      string par = parent(d8);
      if (wr(par) >= 0) {
        Str pv = getr(par);
        if (!pv.none() && pv.s() != par && out && wlive(par, i0)) {
          Str keep = breg.pop(d8);
          materialize(out, par);
          breg.set(d8, keep);
        }
        setr(par, Str());
      }
      byte_settle(d8, i0);
      return;
    }
    if (isalu(mn) && isbyte(split(ops, ", ")[0])) {
      string d8 = split(ops, ", ")[0];
      string src = at(splitn(ops, ", ", 1), 1);
      if ((mn == "sub" || mn == "xor") && src == d8) {
        breg.set(d8, lit("0"));
        string lo = d8 == "ah" ? "al" : d8 == "bh" ? "bl" : d8 == "ch" ? "cl" : d8 == "dh" ? "dl" : "";
        if (!lo.empty() && breg.has(lo)) {
          setr(parent(d8), fr(b8(breg[lo])));
          if (d8 == "ah") axsrc = NI, retlive = true;
        } else {
          Str w_;
          bool hw = false;
          if (pw_on && pw_i == i0) w_ = pw_v, hw = true;
          if (!lo.empty() && hw && !w_.none() && w_.s() != parent(d8)) {
            setr(parent(d8), fr("(" + w_.s() + " & 0xff)"));
            breg.pop0(d8);
            if (d8 == "ah") axsrc = NI, retlive = true;
          } else {
            byte_settle(d8, i0);
          }
        }
        return;
      }
      if (msc3 && !C.mark && in(d8, {"ah", "bh", "ch", "dh"}) && in(mn, {"or", "xor", "and"}) && isnum(src) && !breg.has(d8)) {
        string w_ = parent(d8);
        long long k_ = num(src);
        Str cur_ = getr(w_);
        string cs_ = !cur_.none() ? cur_.s() : w_;
        long long kw = mn != "and" ? (k_ << 8) : ((k_ << 8) | 0xff);
        setr(w_, fr("(" + cs_ + " " + (mn == "or" ? "|" : mn == "xor" ? "^" : "&") + " 0x" + X(kw) + ")"));
        if (w_ == "ax") axsrc = NI, retlive = true;
        return;
      }
      Str cur = breg.has(d8) ? breg.get(d8) : (!pre8.none() ? pre8 : fr(d8));
      string opc = ALU(mn);
      string sv;
      if (isnum(src)) sv = D(num(src));
      else if (isbyte(src)) sv = C.deadregs ? S(bval(src)) : S(breg.has(src) ? breg.get(src) : fr(src));
      else sv = S(orS(memname(src), src.c_str()));
      breg.set(d8, fr("(" + S(cur) + " " + opc + " " + sv + ")"));
      byte_settle(d8, i0);
      return;
    }
    if (in(mn, {"inc", "dec"}) && isbyte(ops)) {
      Str c = breg.has(ops) ? breg.get(ops) : (!pre8.none() ? pre8 : fr(ops));
      breg.set(ops, fr("(" + S(c) + " " + (mn == "inc" ? "+" : "-") + " 1)"));
      return;
    }
    if (in(mn, {"cbw", "cwde"}) && breg.has("al")) {
      rg[AX] = C.mark ? fr("SX(" + S(breg["al"]) + ")") : breg["al"];
      axsrc = NI;
      cbw = fr(S(rg[AX]));
      if (msc3 && !C.mark && i0 > 0 && I(i0 - 1).mnemonic == "mov") {
        auto mb = mt("al, byte ptr (\\[bp [+-] (?:0x[0-9a-f]+|\\d+)\\])$", I(i0 - 1).op_str);
        if (mb) {
          bool any = false;
          for (auto &x : ins)
            if (has(x.op_str, "word ptr " + g(mb, 1))) {
              any = true;
              break;
            }
          if (any) {
            string v_ = S(rg[AX]);
            rg[AX] = fr("(char)" + (fm("[\\w\\->.]+", v_) ? v_ : "(" + v_ + ")"));
          }
        }
      }
      if (C.deadregs) breg.pop0("al"), breg.pop0("ah"), retlive = true;
      if (msc3) retlive = true;
      return;
    }
    if (in(mn, {"cbw", "cwde"}) && C.mark) {
      rg[AX] = fr("SX(" + S(orS(rg[AX], "ax")) + ")");
      if (rg[AX].none()) {}
      axsrc = NI;
      retlive = true;
      return;
    }
    if (in(mn, {"cbw", "cwde"}) && msc3 && !rg[AX].none() && !in(rg[AX].s(), {"ax", "al"})) {
      string v_ = rg[AX].s();
      rg[AX] = fr("(char)" + (fm("[\\w\\->.]+", v_) ? v_ : "(" + v_ + ")"));
      axsrc = NI;
      retlive = true;
      return;
    }
    if (in(mn, {"cbw", "cwde"}) && msc3) {
      rg[AX] = lit("al");
      axsrc = NI;
      retlive = true;
      return;
    }
    if (mn == "mov" && starts(ops, "ax, ")) {
      string src = sl(ops, 4);
      if (src == "dx" && has_div) rg[AX] = fr("(" + S(div0) + " % " + S(div1) + ")"), axsrc = NI;
      else if (in(src, {"bx", "cx", "dx"})) rg[AX] = rsrc(src), axsrc = NI;
      else if (src == "si" || src == "di") {
        Str v = getr(src);
        rg[AX] = !v.none() ? v : fr(src);
        axsrc = NI;
      } else if (isnum(src)) rg[AX] = rhs(src), axsrc = i0;
      else rg[AX] = rhs(src), axsrc = NI;
    } else if (mn == "mov" && (starts(ops, "si, ") || starts(ops, "di, "))) {
      Str d = fr(sl(ops, 0, 2));
      string src = sl(ops, 4);
      if (msc3 && isnum(src) && saved_has(d.s()) && out) {
        emit(out, d.s() + " = " + D(num(src)) + ";");
        setr(d.s(), Str());
      } else {
        movptr(d, src);
      }
    } else if (in(mn, {"les", "lds"})) {
      auto pr = partition(ops, ", ");
      if (wr(pr.first) >= 0) setr(pr.first, memname(pr.second));
    } else if (mn == "lea") {
      auto pr = partition(ops, ", ");
      Str nm = memname(pr.second);
      if (wr(pr.first) >= 0 && truthy(nm)) setr(pr.first, fr("&" + nm.s()));
    } else if (mn == "xchg") {
      auto o = split(ops, ", ");
      if (o.size() == 2 && wr(o[0]) >= 0 && wr(o[1]) >= 0) {
        Str va = getr(o[0]), vb = getr(o[1]);
        setr(o[0], !vb.none() ? vb : fr(o[1]));
        setr(o[1], !va.none() ? va : fr(o[0]));
      }
    } else if (mn == "mov" && starts(ops, "bx, ")) {
      string src = sl(ops, 4);
      if (in(src, {"ax", "cx", "dx", "si", "di", "bp"})) movptr(lit("bx"), src), bxsrc = NI;
      else rg[BX] = rhs(src), bxsrc = isnum(src) ? i0 : NI;
    } else if (mn == "mov" && starts(ops, "cx, ")) {
      string src = sl(ops, 4);
      if (in(src, {"ax", "bx", "dx", "si", "di", "bp"})) movptr(lit("cx"), src);
      else rg[CX] = rhs(src);
    } else if (mn == "mov" && starts(ops, "dx, ")) {
      string src = sl(ops, 4);
      if (in(src, {"ax", "bx", "cx", "si", "di", "bp"})) movptr(lit("dx"), src);
      else rg[DX] = rhs(src);
    } else if (in(mn, {"xor", "sub"}) && ops == "ax, ax") {
      rg[AX] = lit("0"), axsrc = NI;
    } else if (in(mn, {"xor", "sub"}) && ops == "bx, bx") {
      rg[BX] = lit("0"), bxsrc = NI;
    } else if (in(mn, {"cwd", "cdq"})) {
      string v_ = !rg[AX].none() ? rg[AX].s() : "ax";
      if (mt("\\d+$", v_)) rg[DX] = (int0(v_) & 0x8000) ? lit("65535") : lit("0");
      else rg[DX] = fr("(0 - (" + v_ + " >> 15))");
      if (msc3 && !C.mark && mt("\\d+$", v_) && i0 + 2 < N() && ins[i0 + 1].mnemonic == "push" &&
          ins[i0 + 1].op_str == "dx" && ins[i0 + 2].mnemonic == "push" && ins[i0 + 2].op_str == "ax")
        rg[DX] = fr("(0 - (" + v_ + " >> 15))");
    } else if (in(mn, {"or", "and"}) && ops == "ax, ax") {
    } else {
      string axv = S(!rg[AX].none() ? rg[AX] : lit("ax"));
      string cxv = S(!rg[CX].none() ? rg[CX] : lit("cx"));
      string d0 = strip(split(ops, ",")[0]);
      auto rv = [&](const string &r) { Str v = getr(r); return !v.none() ? v.s() : r; };
      auto shcnt = [&]() -> string {
        if (ends(ops, "cl")) return breg.has("cl") ? S(breg["cl"]) : cxv;
        return S(val(at(split(ops, ", "), 1)));
      };
      if (isalu(mn) && starts(ops, "ax, ")) rg[AX] = fr("(" + axv + " " + ALU(mn) + " " + S(val(sl(ops, 4))) + ")");
      else if (in(mn, {"imul", "mul"})) {
        if (mn == "mul" && msc3 && !C.mark && !starts(axv, "(unsigned)"))
          axv = "(unsigned)" + (wordish(axv) ? axv : "(" + axv + ")");
        rg[AX] = fr("(" + axv + " * " + S(val(ops)) + ")");
      } else if (in(mn, {"idiv", "div"})) {
        Str rn = val(ops);
        if (mn == "div" && msc3 && !C.mark && !starts(axv, "(unsigned)"))
          axv = "(unsigned)" + (wordish(axv) ? axv : "(" + axv + ")");
        has_div = true;
        div0 = fr(axv);
        div1 = rn;
        rg[AX] = fr("(" + axv + " / " + S(rn) + ")");
        rg[DX] = fr("(" + axv + " % " + S(rn) + ")");
      } else if (in(mn, {"shl", "sal"}) && wr(d0) >= 0) {
        string a = rv(d0);
        string c = shcnt();
        setr(d0, fr("(" + a + " << " + c + ")"));
      }
      else if (in(mn, {"shr", "sar"}) && wr(d0) >= 0) {
        string a = rv(d0);
        string c = shcnt();
        setr(d0, fr("(" + shr(C, mn, a, c) + ")"));
      }
      else if (mn == "neg" && wr(d0) >= 0) setr(d0, fr("(-" + rv(d0) + ")"));
      else if (mn == "not" && wr(d0) >= 0) setr(d0, fr("(~" + rv(d0) + ")"));
    }
    if (rg[AX].p != old.p) retlive = true;
  }

  // ------------------------------------------------------------------------------ conditions
  std::pair<string, int> cond_at(int i0) {
    int j = i0;
    auto resolve = [&](const string &t) -> Str {
      if (t == "ax") return !rg[AX].none() ? rg[AX] : lit("ax");
      if (t == "bx") return !rg[BX].none() ? rg[BX] : lit("bx");
      if (in(t, {"cx", "dx", "si", "di"})) {
        Str v = getr(t);
        return !v.none() ? v : fr(t);
      }
      if (isbyte(t)) return bval(t, true);
      Str v = memname(t);
      return !v.none() ? v : (isnum(t) ? fromint(num(t)) : fr(t));
    };
    while (j < N() && !cjmp(ins[j].mnemonic)) {
      const string &mn = ins[j].mnemonic, &ops = ins[j].op_str;
      if (mn == "cmp") {
        auto p = split(ops, ", ");
        Str a = resolve(p[0]), b = resolve(at(p, 1));
        has_cmp = true, cmp0 = a, cmp1 = b;
        auto bs = fm("byte ptr (\\[bp [+-] (?:0x[0-9a-f]+|\\d+)\\])", p[0]);
        if (msc3 && !C.mark && bs) {
          bool any = false;
          for (auto &x : ins)
            if (has(x.op_str, "word ptr " + g(bs, 1))) {
              any = true;
              break;
            }
          if (any && fm("\\w+", S(cmp0))) {
            bool k_ok = (bool)fm("0x[0-9a-f]+|\\d+", p[1]);
            long long k_ = k_ok ? num(p[1]) : 0;
            Str nb = (k_ok && k_ >= 0x80) ? fr("(char)0x" + X(k_)) : cmp1;
            cmp0 = fr("(char)" + S(cmp0));
            cmp1 = nb;
          }
        }
      } else if (mn == "test") {
        auto p = split(ops, ", ");
        Str a0 = resolve(p[0]);
        has_cmp = true;
        cmp0 = (p.size() > 1 && p[0] == p[1]) ? a0 : fr("(" + S(a0) + " & " + S(resolve(at(p, 1))) + ")");
        cmp1 = lit("0");
      } else {
        expr_step(j);
      }
      j++;
    }
    if (j >= N()) return {S(!rg[AX].none() ? rg[AX] : lit("0")), j};
    string jcc = ins[j].mnemonic;
    auto width = [&]() -> string {
      int k = j - 1;
      while (k >= 0 && in(ins[k].mnemonic, {"mov", "lea", "push", "pop", "les", "cbw", "cwde"})) k--;
      string o = k >= 0 ? ins[k].op_str : "";
      return (has(o, "byte ptr") || isbyte(strip(split(o, ",")[0]))) ? "S8" : "S16";
    };
    if (jcc == "js" || jcc == "jns") {
      string w = width();
      string v = has_cmp ? "(" + S(cmp0) + " - " + S(cmp1) + ")" : S(flagval(j));
      if (C.mark) v = w + "(" + v + ")";
      return {"(" + v + " " + (jcc == "js" ? ">=" : "<") + " 0)", j};
    }
    string sg;
    if (C.mark && in(jcc, {"jl", "jle", "jg", "jge"})) sg = width();
    if (!has_cmp) {
      int k = j - 1, lo = cond_lo;
      while (k >= lo && k >= 0) {
        const string &mk = ins[k].mnemonic;
        if (mk == "cmp") {
          auto p = split(ins[k].op_str, ", ");
          Str a = resolve(p[0]), b = resolve(at(p, 1));
          has_cmp = true, cmp0 = a, cmp1 = b;
          break;
        }
        if (mk == "test") {
          auto p = split(ins[k].op_str, ", ");
          Str a0 = resolve(p[0]);
          has_cmp = true;
          cmp0 = p[0] == at(p, 1) ? a0 : fr("(" + S(a0) + " & " + S(resolve(p[1])) + ")");
          cmp1 = lit("0");
          break;
        }
        if (flagset(mk) || in(mk, {"call", "lcall", "jmp", "ret", "retf"}) || cjmp(mk)) break;
        k--;
      }
    }
    if (!has_cmp && has_lastcmp) {
      int k = j - 1, lo = cond_lo;
      bool own = false;
      while (k >= lo) {
        if (!in(I(k).mnemonic, {"mov", "lea", "les", "lds", "push", "pop", "nop", "xchg", "cbw", "cwde", "cwd", "cdq"})) {
          own = true;
          break;
        }
        k--;
      }
      if (!own) {
        LC src = lastcmp;
        std::vector<int> preds;
        for (auto &kv : tgt)
          if (kv.second == lo && cjmp(ins[kv.first].mnemonic)) preds.push_back(kv.first);
        if (lo > 0 && cjmp(I(lo - 1).mnemonic)) preds.push_back(lo - 1);
        std::map<string, LC> seen;
        std::vector<string> order;
        for (int q : preds) {
          auto it = cmpat.find(q);
          if (it != cmpat.end()) {
            string key = S(it->second.a) + "\x01" + S(it->second.b) + "\x01" + it->second.w;
            if (!seen.count(key)) order.push_back(key);
            seen[key] = it->second;
          }
        }
        if (seen.size() == 1) src = seen[order[0]];
        int bc = blockcmp;
        if (bc != NI && seen.empty() && (bc == lo - 1 || (tget(bc + 1) == lo && I(bc + 1).mnemonic == "jmp"))) src = lastcmp;
        has_cmp = true, cmp0 = src.a, cmp1 = src.b;
        if (C.mark && in(jcc, {"jl", "jle", "jg", "jge"})) sg = src.w;
      }
    }
    string cond;
    if (has_cmp) {
      string a_ = S(cmp0), b_ = S(cmp1);
      string w;
      if (sg.empty() && C.mark) w = width();
      else w = sg;
      has_lastcmp = true;
      lastcmp = LC{cmp0, cmp1, w.empty() ? "S16" : w};
      cmpat[j] = lastcmp;
      if (!sg.empty()) a_ = sg + "(" + a_ + ")", b_ = sg + "(" + b_ + ")";
      else if (C.unsigned_cast && !C.mark && in(jcc, {"jb", "jae", "ja", "jbe", "jnb", "jnae", "jna", "jnbe", "jc", "jnc"})) {
        if (mt("-\\d+$", b_)) b_ = "0x" + X(int0(b_) & 0xFFFF);
        a_ = "(unsigned)" + a_;
      }
      const char *iv = inv_get(jcc);
      cond = "(" + a_ + " " + (iv ? iv : "?") + " " + b_ + ")";
    } else {
      string v = S(flagval(j));
      const char *op = inv_get(jcc);
      if (!sg.empty()) v = sg + "(" + v + ")";
      cond = op ? "(" + v + " " + op + " 0)" : v;
    }
    for (int r = 0; r < 6; r++) {
      Str v = rg[r];
      if (!v.none() && hascall(v) && has(cond, v.s())) rg[r] = Str();
    }
    return {cond, j};
  }
  static bool flagset(const string &m) {
    return in(m, {"add", "sub", "and", "or", "xor", "inc", "dec", "neg", "shl", "sal", "shr", "sar", "adc", "sbb"});
  }
  Str flagval(int j) {
    int k = j - 1;
    while (k >= 0) {
      const string &mn = ins[k].mnemonic;
      if (in(mn, {"call", "lcall", "jmp", "ret", "retf"}) || cjmp(mn)) break;
      if (flagset(mn)) {
        string d0 = strip(split(ins[k].op_str, ",")[0]);
        if (d0 == "ax") break;
        if (in(d0, {"bx", "cx", "dx", "si", "di"})) {
          Str v = getr(d0);
          return !v.none() ? v : fr(d0);
        }
        if (isbyte(d0)) return bval(d0, true);
        if (has(d0, "[")) return orS(memname(d0), "ax");
        break;
      }
      k--;
    }
    return !rg[AX].none() ? rg[AX] : lit("ax");
  }
  static string b8(const Str &v0) {
    string v = S(v0);
    if (mt("(BYTE\\([^()]*(\\([^()]*\\))*[^()]*\\)|\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5]|[a-d][lh])$", v) || ends(v, "& 0xff)") ||
        !sr("[-+*<>]", v))
      return v;
    return "(" + v + " & 0xff)";
  }
  Str bval(const string &r8, bool trunc = false) {
    if (breg.has(r8)) return trunc ? fr(b8(breg[r8])) : breg[r8];
    Str pv = getr(parent(r8));
    if (pv.none()) return fr(r8);
    if (!ends(r8, "l")) return fr("((" + pv.s() + " >> 8) & 0xff)");
    if (trunc && !mt("(BYTE\\(.*\\)|\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$", pv.s())) return fr("(" + pv.s() + " & 0xff)");
    return pv;
  }

  // ------------------------------------------------------------------------------ longs
  string lparam(const string &op) {
    auto m = sr("\\[bp \\+ (0x[0-9a-f]+|\\d+)\\]", op);
    if (!m) bool_attr();
    long long off = num(g(m, 1));
    long long idx = floordiv(off - fs - 4, 4);
    lmax = std::max(lmax, idx);
    return cbinv::chr('a' + idx);
  }
  int long_pat(int i0) {
    auto op = [&](int k) -> const string & { return ins[k].op_str; };
    auto mnm = [&](int k) -> const string & { return ins[k].mnemonic; };
    if (i0 + 7 < N() && starts(op(i0), "ax, word ptr [bp") && starts(op(i0 + 1), "bx, word ptr [bp") &&
        starts(op(i0 + 3), "dx, word ptr [bp") && mnm(i0 + 4) == "lea" && mnm(i0 + 5) == "push" && mnm(i0 + 6) == "call" &&
        mnm(i0 + 7) == "call") {
      floatfn = true;
      string a = lparam(op(i0 + 3)), b = lparam(op(i0 + 1));
      string o = call_op(i0 + 6, "+");
      rg[AX] = fr("(" + a + " " + o + " " + b + ")");
      return 8;
    }
    if (i0 + 3 < N() && mnm(i0) == "mov" && starts(op(i0), "ax, word ptr [bp") && in(mnm(i0 + 1), {"add", "sub"}) &&
        starts(op(i0 + 1), "ax, word ptr [bp") && mnm(i0 + 2) == "mov" && starts(op(i0 + 2), "bx, word ptr [bp") &&
        in(mnm(i0 + 3), {"adc", "sbb"})) {
      longfn = true;
      string a = lparam(op(i0)), b = lparam(op(i0 + 1));
      rg[AX] = fr("(" + a + " " + (mnm(i0 + 1) == "add" ? "+" : "-") + " " + b + ")");
      return 4;
    }
    if (i0 + 5 < N() && starts(op(i0), "ax, word ptr [bp") && starts(op(i0 + 1), "bx, word ptr [bp") &&
        starts(op(i0 + 3), "dx, word ptr [bp") && mnm(i0 + 4) == "call" && op(i0 + 5) == "ax, bx") {
      longfn = true;
      string a = lparam(op(i0 + 1)), b = lparam(op(i0 + 3));
      string o = call_op(i0 + 4, "*");
      rg[AX] = fr("(" + a + " " + o + " " + b + ")");
      return 6;
    }
    if (i0 + 3 < N() && mnm(i0) == "mov" && mt("ax, word ptr \\[bp - ", op(i0)) && mnm(i0 + 1) == "mov" &&
        mt("dx, word ptr \\[bp - ", op(i0 + 1)) && in(mnm(i0 + 2), {"add", "sub"}) && mt("ax, (0x[0-9a-f]+|\\d+)$", op(i0 + 2)) &&
        in(mnm(i0 + 3), {"adc", "sbb"}) && op(i0 + 3) == "dx, 0" && ((mnm(i0 + 2) == "add") == (mnm(i0 + 3) == "adc"))) {
      longfn = true;
      Str lo = memname(at(splitn(op(i0), ", ", 1), 1));
      string o = mnm(i0 + 2) == "add" ? "+" : "-";
      rg[AX] = fr("(" + S(lo) + " " + o + " " + D(num(at(split(op(i0 + 2), ", "), 1))) + ")");
      return 4;
    }
    return 0;
  }
  int long_stmt(int i0, const List &o) {
    if (i0 + 1 >= N()) return 0;
    const CsIns &a = ins[i0], &b = ins[i0 + 1];
    if (in(a.mnemonic, {"add", "sub"}) && in(b.mnemonic, {"adc", "sbb"}) && ((a.mnemonic == "add") == (b.mnemonic == "adc"))) {
      auto ma = mt("word ptr \\[0x([0-9a-f]+)\\], (0x[0-9a-f]+|\\d+)$", a.op_str);
      auto mb = mt("word ptr \\[0x([0-9a-f]+)\\], 0$", b.op_str);
      if (ma && mb) {
        long long lo = int16(g(ma, 1)), hi = int16(g(mb, 1));
        if (hi == lo + 2) {
          emit(o, "*(long *)&g_" + X(lo) + " " + (a.mnemonic == "add" ? "+" : "-") + "= " + D(num(g(ma, 2))) + ";");
          return 2;
        }
      }
    }
    return 0;
  }
  Str opval(const string &tok0) {
    string tok = strip(tok0);
    if (isnum(tok)) return fromint(num(tok));
    if (tok == "ax") return !rg[AX].none() ? rg[AX] : lit("ax");
    if (wr(tok) >= 0) {
      Str v = getr(tok);
      return !v.none() ? v : fr(tok);
    }
    if (isbyte(tok)) return bval(tok, true);
    return orS(memname(tok), tok.c_str());
  }
  int carry(int i0, const List &o);
  std::vector<string> take_args(int i0, const string &kind, const string &target);
  string axrw(const string &mn, const string &ops);
  bool retval();
  void ax_fate(int i0, std::vector<int> &uses, bool &ret, bool &sf);
  void call_result(int i0, const List &o, const string &expr);
  bool read_before_write(const string &reg, int i0);
  int step(int i0, const List &o);
  int struct_copy(int i0, const List &o);
  int arg_copy(int i0);
  bool var_later(const string &d);
  int fp_arg(int i0);
  int scratch_index(int i0);
  int step0(int i0, const List &o);
  void reset();
  Snap snap();
  void rest(const Snap &d);
  static bool terminal(const std::vector<string> &body);
  void phi_args(const std::vector<Str> &a_args, const std::vector<Str> &b_args, std::vector<string> &a_body,
                std::vector<string> &b_body, bool a_term, bool b_term);
  void phi(const Snap &ts, const Snap &es, std::vector<string> &tb, std::vector<string> &eb);
  List body(int lo, int hi);
  List walk(int lo, int hi);
  static string negate(const string &cond);
  int cmp_start(int jcc_idx, int lo);
  void emit_range(int lo, int hi, const List &o);
  void emit_block(const Blk &blk, const List &o);
  string emit_cond(const Blk &blk, const List &o, const std::set<string> *flush = nullptr);
  void flush_calls(Snap &sn, const Snap &pre, std::vector<string> &body);
  static bool selfref(const string &r, const string &v);
  void plain_if(string cond, std::vector<string> &body, const Snap &pre, const Snap &then_s, const List &o);
  void emit_seq(const Seq &nodes, const List &o);
  void slot_args(const List &o);
  void emit_node(const Node &node, const List &o);
};

// ------------------------------------------------------------------------------------ carries
std::string carry_kind(const std::vector<CsIns> &ins, long long k);

int R::carry(int i0, const List &o) {
  if (I(i0).mnemonic == "sbb" && carry_kind(ins, i0) == "setcc") {
    kill_aliases(i0);
    auto pr = partition(I(i0 - 1).op_str, ", ");
    string r = strip(split(I(i0).op_str, ",")[0]);
    binval("mov", r);
    string oa = S(opval(pr.first));
    string ob = S(opval(pr.second));
    setr(r, fr("(0 - (" + oa + " < " + ob + "))"));
    if (r == "ax") axsrc = NI, retlive = true;
    if (r == "bx") bxsrc = NI;
    return 1;
  }
  string kind = i0 + 1 < N() ? carry_kind(ins, i0 + 1) : "";
  if (in(kind, {"shl1", "shr1", "sar1"})) {
    for (int r = 0; r < 6; r++)
      if (hascall(orS(rg[r], ""))) materialize(o, WRN[r]);
    string a = strip(split(ins[i0].op_str, ",")[0]), b = strip(split(ins[i0 + 1].op_str, ",")[0]);
    string lo = kind == "shl1" ? a : b, hi = kind == "shl1" ? b : a;
    if (msc3 && has(lo, "[") && has(hi, "[")) {
      Str ml = memname(lo);
      if (!ml.none()) {
        emit(o, "*(long *)&" + ml.s() + " " + (kind == "shl1" ? "<<" : ">>") + "= 1;");
        if (kind == "shr1") {
          LAST.has_ul = true;
          if (std::find(LAST.unsigned_longs.begin(), LAST.unsigned_longs.end(), ml.s()) == LAST.unsigned_longs.end())
            LAST.unsigned_longs.push_back(ml.s());
        }
        return 2;
      }
    }
    kill_aliases(i0);
    kill_aliases(i0 + 1);
    string vh = S(opval(hi));
    string vl = S(opval(lo));
    string v = "LONG(" + vh + ", " + vl + ")";
    string E = kind == "shl1" ? v + " + " + v : string(kind == "shr1" ? "LSHR" : "LSAR") + "(" + v + ")";
    binval("mov", lo);
    binval("mov", hi);
    setr(lo, fr("LO(" + E + ")"));
    setr(hi, fr("HI(" + E + ")"));
    if (lo == "ax" || hi == "ax") axsrc = NI, retlive = true;
    if (lo == "bx" || hi == "bx") bxsrc = NI;
    return 2;
  }
  if (kind != "pair" && kind != "neg") return 0;
  for (int r = 0; r < 6; r++)
    if (hascall(orS(rg[r], ""))) materialize(o, WRN[r]);
  const CsIns &p = ins[i0], &c = ins[i0 + 1];
  string E, lo, hi;
  int n;
  if (kind == "neg") {
    lo = strip(p.op_str);
    hi = strip(split(c.op_str, ",")[0]);
    kill_aliases(i0);
    kill_aliases(i0 + 1);
    string vh = S(opval(hi));
    string vl = S(opval(lo));
    E = "0 - LONG(" + vh + ", " + vl + ")";
    n = 3;
  } else {
    auto pl = partition(p.op_str, ", "), ph = partition(c.op_str, ", ");
    lo = pl.first, hi = ph.first;
    string x = pl.second, y = ph.second;
    string op = p.mnemonic == "add" ? "+" : "-";
    if (wr(lo) >= 0) kill_aliases(i0), kill_aliases(i0 + 1);
    string Xv = S(opval(x)), Yv = S(opval(y));
    if (wr(lo) < 0) {
      Str ml = memname(lo), mh = memname(hi);
      string t = newt();
      emit(o, t + " = LONG(" + S(mh) + ", " + S(ml) + ") " + op + " LONG(" + Yv + ", " + Xv + ");");
      emit(o, S(ml) + " = LO(" + t + ");");
      emit(o, S(mh) + " = HI(" + t + ");");
      return 2;
    }
    string lo0 = S(opval(lo)), hi0 = S(opval(hi));
    E = "LONG(" + hi0 + ", " + lo0 + ") " + op + " LONG(" + Yv + ", " + Xv + ")";
    n = 2;
  }
  binval("mov", lo);
  binval("mov", hi);
  setr(lo, fr("LO(" + E + ")"));
  setr(hi, fr("HI(" + E + ")"));
  if (lo == "ax" || hi == "ax") axsrc = NI, retlive = true;
  if (lo == "bx" || hi == "bx") bxsrc = NI;
  return n;
}

std::vector<string> R::take_args(int i0, const string &kind, const string &target) {
  long long n = LLONG_MIN;
  bool pascal = false;
  if (C.hooks.callregs && out) {
    std::vector<string> rr;
    int st = C.hooks.callregs(ins[i0].address, kind, target, rr);
    if (st == 1 && !rr.empty()) {
      for (auto &r : rr) {
        Str v = getr(r);
        if (!v.none() && v.s() != r) materialize(out, r);
        if (in(r, {"ax", "bx", "cx", "dx"}))
          for (string b8 : {string(1, r[0]) + "l", string(1, r[0]) + "h"})
            if (breg.has(b8)) {
              Str bv = breg.pop(b8);
              if (S(bv) != b8) push_out(out, b8 + " = " + S(bv) + ";", i0);
            }
        setr(r, Str());
      }
      push_out(out, "USE(" + join(rr, ", ") + ");", i0);
    }
  }
  if (C.hooks.callinfo) {
    long long rn = 0;
    int st = C.hooks.callinfo(ins[i0].address, kind, target, rn);
    if (st == 1 && rn) pascal = true, n = floordiv(rn, 2);
  }
  if (n == LLONG_MIN && i0 + 1 < N()) {
    const CsIns &nx = ins[i0 + 1];
    if (nx.mnemonic == "add" && mt("sp, (0x[0-9a-f]+|\\d+)$", nx.op_str)) n = floordiv(num(sl(nx.op_str, 4)), 2);
    else if (C.deadregs && nx.mnemonic == "jmp" && tget(i0 + 1) != NI) {
      const CsIns &tx = I(tat(i0 + 1));
      if (tx.mnemonic == "add" && mt("sp, (0x[0-9a-f]+|\\d+)$", tx.op_str)) n = floordiv(num(sl(tx.op_str, 4)), 2);
    }
  }
  const CsIns *nx_ = i0 + 1 < N() ? &ins[i0 + 1] : nullptr;
  if (nx_ && nx_->mnemonic == "jmp" && tget(i0 + 1) != NI) nx_ = &I(tat(i0 + 1));
  if (n == LLONG_MIN && !pascal && msc3 && nx_ && nx_->mnemonic == "push" && in(nx_->op_str, {"ax", "dx"})) n = 0;
  if (n == LLONG_MIN && !pascal && C.masm_cret) n = 0;
  std::vector<Str> take;
  if (n == LLONG_MIN || n > (long long)args.size()) {
    take = args;
    args.clear();
  } else {
    // Python: s.args[len - n:] (n may be negative only through a negative clean-up: slices clamp)
    long long L = (long long)args.size(), cut = L - n;
    if (cut < 0) cut = 0;
    if (cut > L) cut = L;
    take.assign(args.begin() + cut, args.end());
    args.erase(args.begin() + cut, args.end());
  }
  std::vector<string> o;
  if (pascal)
    for (auto &a : take) o.push_back(S(a));
  else
    for (auto it = take.rbegin(); it != take.rend(); ++it) o.push_back(S(*it));
  return o;
}

string R::axrw(const string &mn, const string &ops) {
  static const re::Regex AXR("\\b(ax|al|ah)\\b");
  if (in(mn, {"call", "lcall", "lodsb", "lodsw"})) return "def";
  if (in(mn, {"ret", "retf"})) return "ret";
  std::vector<string> p;
  if (!ops.empty())
    for (auto &x : split(ops, ",")) p.push_back(strip(x));
  string d0 = p.empty() ? "" : p[0], src = p.size() > 1 ? p[1] : "";
  if (in(mn, {"mov", "lea", "les", "lds", "pop"})) {
    if (d0 == "ax" || d0 == "al") return AXR.search(src) ? "use" : "def";
    if (d0 == "ah") return "xform";
    return AXR.search(ops) ? "use" : "";
  }
  string nos = replace_all(ops, " ", "");
  if (in(mn, {"xor", "sub"}) && nos == "ax,ax") return "def";
  if (in(mn, {"or", "and", "test"}) && (nos == "ax,ax" || nos == "al,al")) return "use";
  if (in(mn, {"cbw", "cwde", "cwd", "cdq"})) return "xform";
  if (in(mn, {"mul", "imul", "div", "idiv"})) return "xform";
  if (in(mn, {"cmp", "test", "push", "stosb", "stosw", "scasb", "scasw", "out", "xchg"}))
    return (AXR.search(ops) || in(mn, {"stosb", "stosw", "scasb", "scasw"})) ? "use" : "";
  if (in(d0, {"ax", "al", "ah"})) return "xform";
  return AXR.search(ops) ? "use" : "";
}

bool R::retval() {
  if (rv < 0 && C.retval >= 0) rv = C.retval;
  if (rv < 0) {
    std::set<std::pair<int, bool>> seen;
    std::vector<std::pair<int, bool>> work = {{emit_start, false}};
    bool r_ = true;
    while (!work.empty() && r_) {
      auto jd = work.back();
      work.pop_back();
      int j = jd.first;
      bool d = jd.second;
      while (j != NI && 0 <= j && j < N() && !seen.count({j, d})) {
        seen.insert({j, d});
        const string &mn = ins[j].mnemonic;
        if (in(mn, {"ret", "retf"})) {
          if (!d) r_ = false;
          break;
        }
        if (axrw(mn, ins[j].op_str) == "def") d = true;
        if (mn == "jmp") {
          j = tget(j);
          continue;
        }
        if (cjmp(mn)) work.push_back({tget(j), d});
        j++;
      }
    }
    if (!r_ && msc3) {
      seen.clear();
      work = {{emit_start, false}};
      while (!work.empty() && !r_) {
        auto jd = work.back();
        work.pop_back();
        int j = jd.first;
        bool d = jd.second;
        while (j != NI && 0 <= j && j < N() && !seen.count({j, d})) {
          seen.insert({j, d});
          const string &mn = ins[j].mnemonic;
          if (in(mn, {"ret", "retf"})) {
            if (d) r_ = true;
            break;
          }
          if (in(mn, {"call", "lcall"})) d = false;
          else if (axrw(mn, ins[j].op_str) == "def") d = true;
          if (mn == "jmp") {
            j = tget(j);
            continue;
          }
          if (cjmp(mn)) work.push_back({tget(j), d});
          j++;
        }
      }
    }
    rv = r_ ? 1 : 0;
  }
  return rv == 1;
}

void R::ax_fate(int i0, std::vector<int> &uses, bool &ret, bool &sf) {
  std::set<int> seen;
  std::vector<int> work = {i0 + 1};
  uses.clear();
  ret = false;
  int first = NI;
  bool linear = true;
  retlin = false;
  while (!work.empty()) {
    int j = work.back();
    work.pop_back();
    while (j != NI && 0 <= j && j < N() && !seen.count(j)) {
      seen.insert(j);
      const string &mn = ins[j].mnemonic;
      string k = axrw(mn, ins[j].op_str);
      if (k == "def") break;
      if (k == "ret") {
        if (retval()) {
          ret = true;
          if (linear) retlin = true;
        }
        break;
      }
      if (k == "use") {
        uses.push_back(j);
        if (first == NI && linear) first = j;
      }
      if (mn == "jmp") {
        j = tget(j);
        linear = false;
        continue;
      }
      if (cjmp(mn)) {
        work.push_back(tget(j));
        linear = false;
      }
      j++;
    }
  }
  sf = false;
  firstlin = first;
  if (first != NI) {
    const CsIns &f = ins[first];
    string d = split(f.op_str, ", ")[0];
    sf = f.mnemonic == "mov" && has(d, "[") && ends(f.op_str, ", ax");
  }
}

void R::call_result(int i0, const List &o, const string &expr) {
  for (size_t k = 0; k < args.size(); k++)
    if (sr("\\b([abcd][xlh])\\b", S(args[k]))) {
      string t = newt();
      emit(o, t + " = " + S(args[k]) + ";");
      args[k] = fr(t);
    }
  for (int r : {SI, DI}) {
    Str v = rg[r];
    if (!v.none() && v.s() != WRN[r] && !mt("^&|^-?\\d+$", v.s()) && live(WRN[r], i0)) materialize(o, WRN[r]);
  }
  for (int r = 0; r < 6; r++) {
    Str v = rg[r];
    if (!v.none() && v.s() != WRN[r] && hascall(v) && !has(expr, v.s())) materialize(o, WRN[r]);
  }
  rg[BX] = rg[CX] = rg[DX] = Str();
  bxsrc = NI;
  for (const char *b : {"bl", "bh", "cl", "ch", "dl", "dh", "al", "ah"}) breg.pop0(b);
  std::vector<int> uses;
  bool ret, sf;
  ax_fate(i0, uses, ret, sf);
  auto pinax = [&]() {
    emit(o, "ax = " + expr + ";");
    rg[AX] = lit("ax");
    retlive = true;
  };
  if (callstmt || (uses.empty() && !ret)) {
    emit(o, expr + ";");
    rg[AX] = Str();
    retlive = false;
  } else if (live("dx", i0)) pinax();
  else if (!uses.empty() && ret && C.masm_cret) pinax();
  else if (!uses.empty() && firstlin == NI) pinax();
  else if (uses.empty() && !retlin) pinax();
  else if (!sf && uses.size() == 1 && in(ins[uses[0]].mnemonic, {"cmp", "test", "or", "and"}) && [&]() {
             std::vector<int> u2;
             bool r2, s2;
             ax_fate(uses[0], u2, r2, s2);
             return !u2.empty() || r2;
           }())
    pinax();
  else if (sf || uses.size() <= 1) {
    rg[AX] = fr(expr);
    retlive = true;
  } else pinax();
  axsrc = NI;
}

bool R::read_before_write(const string &reg, int i0) {
  std::vector<string> names = {reg};
  if (in(reg, {"ax", "bx", "cx", "dx"})) names.push_back(string(1, reg[0]) + "l"), names.push_back(string(1, reg[0]) + "h");
  auto rd = [&](const string &n, const string &t) { return (bool)srd("\\b" + n + "\\b", t); };
  for (long long q = i0; q < std::min<long long>(i0 + 12, N()); q++) {
    if (q < 0) continue;
    const string &mn = ins[q].mnemonic, &ops = ins[q].op_str;
    if (starts(mn, "j") || in(mn, {"ret", "retf", "call"})) {
      if (mn != "call") return false;
      for (auto &n : names)
        if (rd(n, ops)) return true;
      return false;
    }
    std::vector<string> parts;
    for (auto &p : split(ops, ",")) parts.push_back(strip(p));
    std::vector<string> src(parts.size() > 1 ? parts.begin() + 1 : parts.end(), parts.end());
    string dst = parts.empty() ? "" : parts[0];
    for (auto &n : names)
      for (auto &p : src)
        if (rd(n, p)) return true;
    if (has(dst, "["))
      for (auto &n : names)
        if (rd(n, dst)) return true;
    bool dn = std::find(names.begin(), names.end(), dst) != names.end();
    if (in(mn, {"mov", "lea", "pop", "xchg"}) && dn) return false;
    if (!in(mn, {"mov", "lea", "pop"}) && dn) return true;
  }
  return false;
}

int R::step(int i0, const List &o) {
  int n = step0(i0, o);
  if (C.regvars_eager && msc3 && LAST.has_saved) {
    for (auto &r : std::vector<string>(LAST.saved)) {
      Str v = getr(r);
      std::vector<string> own = {r};
      if (LAST.has_regparam)
        for (auto &kv : LAST.regparam)
          if (kv.first == r) {
            own.push_back(pname(4 + 2 * kv.second));
            break;
          }
      if (!v.none() && v.s() != r && !mt("-?\\d+$|&|[A-Za-z_]\\w*$", v.s()) && !hascall(v)) {
        bool any = false;
        for (auto &ow : own)
          if (srd("\\b" + ow + "\\b", v.s())) any = true;
        if (any) materialize(o, r);
      }
    }
  }
  return n;
}

}  // namespace
}  // namespace cbinv

#include "early_c_walk.inc"
