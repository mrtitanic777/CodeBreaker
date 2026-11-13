// cb/invert/early_c_tracker.hpp -- the register-value tracker of the Microsoft C 2.03 / 3.00 model: what each register holds, as the C expression
// that computes it, while the walker steps through a function's instructions (cb/invert/early_c_walk.cpp) and the compiler's idioms (calls,
// 8087 sequences, longs, byte registers, structure copies, carries) are read off them (early_c_tracker.cpp).
//
// Internal to the early_c model: callers use cb/invert/early_c.hpp.
#pragma once

#include <climits>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "cb/invert/early_c_model.hpp"
#include "cb/invert/flow16.hpp"
#include "cb/invert/model_text.hpp"
#include "cb/platforms/decoder.hpp"

namespace cb::invert::early_c_model {

using namespace cb::invert::model;
using std::string;
using flow16::Node;
using flow16::Seq;

// The 16-bit instructions of `code` loaded at `address`, up to the first byte that does not decode.
inline std::vector<platforms::Insn> disasm16(const uint8_t *code, size_t n, long long address) {
  thread_local platforms::DecoderSet decoders;
  return decoders.get(platforms::Arch::X86, 16).disasm(ByteView(code, n), static_cast<std::uint64_t>(address));
}
inline std::vector<platforms::Insn> disasm16(const std::vector<uint8_t> &code, long long address) {
  return disasm16(code.data(), code.size(), address);
}

inline Value interned(const string &s) { return Value::intern(s); }
inline Value fresh(const string &s) { return Value::make(s); }
inline Value from_int(long long v) { return Value::from_int(v); }
inline string group_of(const Hit &m, int k) { return m.group(k); }
inline Value value_of(const Hit &m, int k) { return m.value(k); }
inline bool truthy(const Value &v) { return v.nonblank(); }


typedef std::shared_ptr<std::vector<string>> List;
inline List newlist() { return std::make_shared<std::vector<string>>(); }
const int NI = INT_MIN;   // None, as an instruction index

enum { AX, BX, CX, DX, SI, DI };
inline constexpr const char *WRN[6] = {"ax", "bx", "cx", "dx", "si", "di"};
const int REGS_ORDER[6] = {AX, SI, DI, CX, BX, DX};   // R._REGS
inline int wr(const string &n) {
  for (int k = 0; k < 6; k++)
    if (n == WRN[k]) return k;
  return -1;
}
inline constexpr const char *BYTEREG[8] = {"al", "bl", "cl", "dl", "ah", "bh", "ch", "dh"};
inline bool isbyte(const string &n) {
  for (auto b : BYTEREG)
    if (n == b) return true;
  return false;
}
inline string parent(const string &b8) {   // BYTEPARENT[b8] (KeyError past the eight)
  if (!isbyte(b8)) fail( "'" + b8 + "'");
  return string(1, b8[0]) + "x";
}
inline bool in(const string &x, std::initializer_list<const char *> v) {
  for (auto p : v)
    if (x == p) return true;
  return false;
}
inline const char *HALF(const string &r) {
  if (r == "ax") return "ax|al|ah";
  if (r == "bx") return "bx|bl|bh";
  if (r == "cx") return "cx|cl|ch";
  if (r == "dx") return "dx|dl|dh";
  if (r == "si") return "si";
  if (r == "di") return "di";
  if (r == "es") return "es";
  fail( "'" + r + "'");
}
inline string ALU(const string &mn) {
  if (mn == "add") return "+";
  if (mn == "sub") return "-";
  if (mn == "and") return "&";
  if (mn == "or") return "|";
  if (mn == "xor") return "^";
  fail( "'" + mn + "'");
}
inline bool isalu(const string &mn) { return in(mn, {"add", "sub", "and", "or", "xor"}); }
inline constexpr const char *JCCs[][2] = {{"jl", "<"}, {"jle", "<="}, {"jg", ">"}, {"jge", ">="}, {"je", "=="}, {"jne", "!="}, {"ja", ">"},
                         {"jae", ">="}, {"jb", "<"}, {"jbe", "<="}, {"jc", "<"}, {"jnc", ">="}};
inline constexpr const char *INVs[][2] = {{"jge", "<"}, {"jg", "<="}, {"jle", ">"}, {"jl", ">="}, {"jne", "=="}, {"je", "!="}, {"jz", "!="},
                         {"jnz", "=="}, {"ja", "<="}, {"jae", "<"}, {"jb", ">="}, {"jbe", ">"}, {"jc", ">="}, {"jnc", "<"}};
inline const char *jcc_get(const string &m) {
  for (auto &p : JCCs)
    if (m == p[0]) return p[1];
  return nullptr;
}
inline const char *inv_get(const string &m) {
  for (auto &p : INVs)
    if (m == p[0]) return p[1];
  return nullptr;
}
inline bool isjcc(const string &m) { return jcc_get(m) != nullptr; }
inline bool cjmp(const string &m) { return isjcc(m) || in(m, {"jz", "jnz", "js", "jns"}); }
inline bool fpop(const string &m) {
  return in(m, {"fld", "fild", "fldz", "fld1", "fst", "fstp", "fist", "fistp", "fchs", "fxch", "fadd", "faddp", "fsub",
                "fsubp", "fmul", "fmulp", "fdiv", "fdivp", "fsubr", "fsubrp", "fdivr", "fdivrp", "fiadd", "fisub",
                "fimul", "fidiv", "fisubr", "fidivr", "fwait", "fnop", "finit", "fninit", "fclex", "fnclex"});
}
inline bool tracked_byte(const string &m) {
  return in(m, {"mov", "add", "sub", "or", "and", "xor", "inc", "dec", "shl", "sal", "shr", "sar", "neg", "not"});
}
inline constexpr const char *NUMRX = "(0x[0-9a-f]+|\\d+)$";
inline constexpr const char *TOKRX = "(?<![\\w>.])(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh)(?![\\w(])";

// an insertion-ordered dict str -> Value (the byte registers)
struct OD {
  std::vector<std::pair<string, Value>> v;
  int find(const string &k) const {
    for (size_t j = 0; j < v.size(); j++)
      if (v[j].first == k) return (int)j;
    return -1;
  }
  bool has(const string &k) const { return find(k) >= 0; }
  Value get(const string &k) const {
    int j = find(k);
    return j < 0 ? Value() : v[j].second;
  }
  const Value &operator[](const string &k) const {
    int j = find(k);
    if (j < 0) fail( "'" + k + "'");
    return v[j].second;
  }
  void set(const string &k, const Value &x) {
    int j = find(k);
    if (j < 0) v.push_back({k, x});
    else v[j].second = x;
  }
  Value pop(const string &k) {   // pop(k): KeyError when absent
    int j = find(k);
    if (j < 0) fail( "'" + k + "'");
    Value x = v[j].second;
    v.erase(v.begin() + j);
    return x;
  }
  void pop0(const string &k) {   // pop(k, None)
    int j = find(k);
    if (j >= 0) v.erase(v.begin() + j);
  }
  bool empty() const { return v.empty(); }
};

inline bool hascall(const Value &v) {   // R._hascall (str(None) has no call)
  if (v.empty()) return false;
  static const std::set<string> P = {"BYTE", "HIBYTE", "SX", "S8", "S16", "LONG", "LO", "HI", "LSHR", "LSAR",
                                     "FARM", "FAROFF", "U32", "S32", "USE"};
  for (auto &n : RX("([A-Za-z_]\\w*)\\(").find_all(v.s()))
    if (!P.count(n)) return true;
  return false;
}
inline bool hascall(const string &v) { return hascall(fresh(v)); }

inline string shr(const Settings &C, const string &mn, const string &x, const string &n, bool byte = false) {
  if (!C.dead_registers) return x + " >> " + n;
  if (mn == "sar") return C.mark ? (byte ? "SAR8(" : "SAR(") + x + ", " + n + ")" : x + " >> " + n;
  return C.mark || byte ? x + " >> " + n : "(unsigned)" + x + " >> " + n;
}

struct Snap {
  Value r[6];
  bool has_div = false;
  Value div0, div1;
  bool retlive = false;
  int axsrc = NI, bxsrc = NI;
  std::vector<Value> args;
  OD breg;
  int i = 0;
};

struct LC {   // _lastcmp: ((a, b), w)
  Value a, b;
  string w;
};

struct Blk {   // flow16's Block, as the model reads it
  int lo, hi;
  long long addr;
  flow16::End term;
  bool has_cases;
  string selreg;
  int selbase;
};

class Tracker {
public:
  const Settings &C;
  Findings &LAST;
  const std::vector<platforms::Insn> &ins;
  long long fs, base;
  const std::map<long long, string> &cxmap;
  bool msc3;
  Value rg[6];
  bool has_div = false;
  Value div0, div1;
  long long maxidx = -1, lmax = -1;
  std::vector<Value> args;
  bool longfn = false, floatfn = false;
  bool has_cmp = false;
  Value cmp0, cmp1;
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
  Value cbw;
  bool s8_on = false, sm_on = false, p8_on = false, pw_on = false;
  int s8_i = 0, sm_i = 0, p8_i = 0, pw_i = 0;
  Value s8_v, sm_v, p8_v, pw_v;
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

  Tracker(const Settings &c, Findings &last, const std::vector<platforms::Insn> &in_, long long fs_, long long base_,
    const std::map<long long, string> &cx, bool m3)
      : C(c), LAST(last), ins(in_), fs(fs_), base(base_), cxmap(cx), msc3(m3) {
    std::unordered_map<long long, int> a2i;
    for (size_t k = 0; k < ins.size(); k++) a2i[ins[k].address] = (int)k;
    for (size_t k = 0; k < ins.size(); k++) {
      const platforms::Insn &ii = ins[k];
      if (cjmp(ii.mnemonic) || ii.mnemonic == "jmp") {
        if (is_number(ii.op_str)) {
          auto it = a2i.find(parse_number(ii.op_str));
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
      if (in(x.mnemonic, {"ret", "retf"}) && is_number(x.op_str)) {
        retn = parse_number(x.op_str);
        break;
      }
    if (retn) maxidx = floor_quotient(retn, 2) - 1;
  }

  // -------------------------------------------------------------------------------- helpers
  const platforms::Insn &I(long long k) const { return at(ins, k); }
  long long N() const { return (long long)ins.size(); }
  Value *regp(const string &n) {
    int k = wr(n);
    return k < 0 ? nullptr : &rg[k];
  }
  Value getr(const string &n) {   // getattr(s, n, None) for a register name
    Value *p = regp(n);
    return p ? *p : Value();
  }
  void setr(const string &n, const Value &v) {
    Value *p = regp(n);
    if (p) *p = v;
  }
  int tget(int j) const {   // s.tgt.get(j)
    auto it = tgt.find(j);
    return it == tgt.end() ? NI : it->second;
  }
  int tat(int j) const {   // s.tgt[j]
    auto it = tgt.find(j);
    if (it == tgt.end()) fail( std::to_string(j));
    return it->second;
  }
  const platforms::Insn &Iv(int k) const {   // s.ins[k] where k may be None
    if (k == NI) broken( "list indices must be integers or slices, not NoneType");
    return I(k);
  }
  string newt() {
    tn++;
    return "t" + std::to_string(tn);
  }
  void push_out(const List &o, const string &t, long long rank, bool rc = false) {
    if (!o) broken( "'NoneType' object has no attribute 'append'");
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
  long long num(const string &x) const { return parse_number(x); }

  string call_op(int ci, const string &def) {
    const platforms::Insn &c = I(ci);
    const ByteView b = c.raw();
    if (b.size() < 3 || b[0] != 0xe8) return def;
    long long rel = b[1] | (b[2] << 8);
    if (rel >= 0x8000) rel -= 0x10000;
    long long t = (base + c.address + 3 + rel) & 0xffff;
    auto it = cxmap.find(t);
    return it == cxmap.end() ? def : it->second;
  }
  string pname(long long off) {
    long long pb = pbase;
    long long idx = msc3 ? floor_quotient(off - pb, 2) : floor_quotient(off - fs - pb, 2);
    if (retn && 0 <= idx && idx < floor_quotient(retn, 2)) idx = floor_quotient(retn, 2) - 1 - idx;
    maxidx = std::max(maxidx, idx);
    return (0 <= idx && idx < 26) ? utf8_char('a' + idx) : "p" + std::to_string(idx);
  }
  string ptrbase(const string &reg) {
    Value v = getr(reg);
    if (v.empty() || mt("-?\\d+$|-?0x[0-9a-f]+$", v.s())) return reg;
    const string &s = v.s();
    if (mt("^[A-Za-z_]\\w*(->f\\d+)*$", s) || (starts(s, "(") && ends(s, ")"))) return s;
    return "(" + s + ")";
  }
  static string plus2(const string &op) {
    auto m = sr("\\[(.*)\\]", op);
    if (!m) return op;
    string e = group_of(m, 1);
    auto m2 = mt("(.*) ([+-]) (0x[0-9a-f]+|\\d+)$", e);
    if (m2) {
      long long v = parse_number(group_of(m2, 3)) * (group_of(m2, 2) == "+" ? 1 : -1) + 2;
      e = v == 0 ? group_of(m2, 1) : group_of(m2, 1) + " " + (v > 0 ? "+" : "-") + " 0x" + hex(std::llabs(v));
    } else if (mt("(0x[0-9a-f]+|\\d+)$", e)) {
      e = "0x" + hex(parse_number(e) + 2);
    } else {
      e = e + " + 2";
    }
    return op.substr(0, m.start()) + "[" + e + "]" + op.substr(m.end());
  }

  // ------------------------------------------------------------------------------ the 8087
  int fp_step(int i0, const List &o) {
    const platforms::Insn &ii = I(i0);
    const string &mn = ii.mnemonic, &ops = ii.op_str;
    has_fst = true;
    auto &st = fst;
    LAST.has_fp_types = true;
    if (in(mn, {"wait", "fwait", "fnop", "finit", "fninit", "fclex", "fnclex"})) return i0 + 1;
    auto mem = [&](const string &op, bool integer) -> Value {
      auto m = mt("(?:(\\w+):\\s*)?(byte|word|dword|qword|tbyte) ptr (.*)$", op);
      if (!m) return Value();
      string size = group_of(m, 2);
      Value nm = memname((size != "byte" ? "word ptr " : "byte ptr ") + group_of(m, 3));
      if (nm.empty()) return Value();
      string ty;
      if (integer) ty = size == "word" ? "int" : (size == "dword" || size == "qword") ? "long" : "";
      else ty = size == "dword" ? "float" : (size == "qword" || size == "tbyte") ? "double" : "";
      if (!ty.empty() && C.fp_casts && sr("->|\\[|\\.", nm.s()) && ty != "int") return fresh("*(" + ty + " *)&" + nm.s());
      if (!ty.empty()) {
        bool f = false;
        for (auto &kv : LAST.fp_types)
          if (kv.first == nm.s()) f = true;
        if (!f) LAST.fp_types.push_back({nm.s(), ty});
      }
      return nm;
    };
    auto sti = [&](const string &op) -> int {
      auto m = fm("st\\((\\d)\\)", strip(op));
      if (m) return (int)parse_number(group_of(m, 1));
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
      Value x = mem(ops, mn == "fild");
      if (x.empty()) return 0;
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
      if (m && j < N() && ins[j].mnemonic == "mov" && ins[j].op_str == "ax, " + group_of(m, 1)) {
        LAST.has_fp_return = true;
        LAST.fp_return = "double";
        emit(o, "return " + st.back() + ";");
        if (p) st.pop_back();
        return j + 1;
      }
      Value x = mem(ops, starts(mn, "fi"));
      if (x.empty()) return 0;
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
        Value x = mem(parts[0], starts(b, "fi"));
        if (x.empty()) return 0;
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
  Value memname(const string &op) {
    Value r = memname0(op);
    if (C.mark && C.far_pointers && !r.empty() && sr("\\bes:", op)) r = fresh("FARM(es, " + r.s() + ")");
    if (C.mark && !r.empty() && has(op, "byte ptr") && has(op, "[")) return fresh("BYTE(" + r.s() + ")");
    return r;
  }
  Value memname0(const string &op0) {
    string op = replace_all(replace_all(op0, "word ptr ", ""), "byte ptr ", "");
    op = RX("\\b(es|ds|ss|cs|fs|gs):").replace_all(op, "");
    if (msc3) {
      auto m = sr("\\[bp - (0x[0-9a-f]+|\\d+)\\]", op);
      if (m) {
        long long o = num(group_of(m, 1));
        locals.insert(o);
        return fresh("v" + decimal(o));
      }
      m = sr("\\[bp \\+ (0x[0-9a-f]+|\\d+)\\]", op);
      if (m) {
        long long o = num(group_of(m, 1));
        string nm = pname(o);
        return fresh(((o - pbase) % 2) ? "HIBYTE(" + nm + ")" : nm);
      }
    } else {
      auto m = sr("\\[bp \\+ (0x[0-9a-f]+|\\d+)\\]", op);
      if (m || has(op, "[bp]")) {
        long long o = m ? num(group_of(m, 1)) : 0;
        if (o < fs) {
          locals.insert(o);
          return fresh("v" + decimal(o));
        }
        string nm = pname(o);
        return fresh(((o - fs - pbase) % 2) ? "HIBYTE(" + nm + ")" : nm);
      }
    }
    auto m = sr("\\[bp - (0x[0-9a-f]+|\\d+)\\]", op);
    if (m) {
      long long o = num(group_of(m, 1));
      locals.insert(o);
      return fresh("v" + decimal(o));
    }
    for (const char *reg : {"si", "bx", "di"}) {
      Value cv = getr(reg);
      string R_ = reg;
      if (!cv.empty() && mt("\\d+$", cv.s())) {
        auto m1 = sr("\\[" + R_ + "(?: ([+-]) (0x[0-9a-f]+|\\d+))?\\]", op);
        if (m1) {
          long long d = m1.has(2) ? num(group_of(m1, 2)) : 0;
          long long base_ = parse_number(cv.s());
          bool minus = m1.has(1) && group_of(m1, 1) == "-";
          return fresh("g_" + hex((base_ + (!minus ? d : -d)) & 0xffff));
        }
      }
      auto m2 = sr("\\[" + R_ + " \\+ (0x[0-9a-f]+|\\d+)\\]", op);
      if (m2 && msc3 && !cv.empty() && !cbw.empty() && cv.s() == cbw.s() && num(group_of(m2, 1)) >= 0x40)
        return fresh("g_" + hex(num(group_of(m2, 1))) + "[" + cv.s() + "]");
      if (m2) return fresh(ptrbase(reg) + "->f" + decimal(num(group_of(m2, 1))));
      auto m3 = sr("\\[" + R_ + " - (0x[0-9a-f]+|\\d+)\\]", op);
      if (m3) return fresh(ptrbase(reg) + "[-" + decimal(num(group_of(m3, 1))) + "]");
      if (has(op, "[" + R_ + "]")) return fresh("*" + ptrbase(reg));
    }
    m = sr("\\[(bx|bp|si|di) \\+ (si|di) ([+-]) (0x[0-9a-f]+|\\d+)\\]", op);
    if (m) return fresh(ptrbase(group_of(m, 1)) + "[" + ptrbase(group_of(m, 2)) + " " + group_of(m, 3) + " " + decimal(num(group_of(m, 4))) + "]");
    m = sr("\\[bx \\+ si\\]|\\[bx \\+ di\\]|\\[si \\+ di\\]", op);
    if (m) {
      string tail = at(split(op, " + "), 1);
      while (!tail.empty() && tail.back() == ']') tail.pop_back();
      return fresh(ptrbase(slice(op, 1, 3)) + "[" + ptrbase(tail) + "]");
    }
    m = sr("\\[(0x[0-9a-f]+|\\d+)\\]", op);
    if (m) return fresh("g_" + hex(num(group_of(m, 1))));
    string t = strip(op);
    if (in(t, {"ax", "bx", "cx", "dx", "si", "di", "bp", "ds", "es", "ss", "cs"})) return fresh(t);
    return Value();
  }
  Value rhs(const string &op) { return is_number(op) ? from_int(num(op)) : memname(op); }

  void movptr(const Value &d, const string &src) {
    const string &dn = d.s();
    if (in(src, {"ax", "bx", "cx", "dx", "si", "di", "bp", "sp"})) {
      Value val = getr(src);
      if (val.empty()) val = fresh(src);
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
    } else if (is_number(src)) {
      setr(dn, from_int(num(src)));
    } else if (msc3 &&
               ((sr("\\[(?:si|di)(?: [+-] (?:0x[0-9a-f]+|\\d+))?\\]", src) && slice(at(split(src, "["), 1), 0, 2) != dn) ||
                (fm("word ptr \\[(?:bp [+-] (?:0x[0-9a-f]+|\\d+)|bx(?: \\+ (?:0x[0-9a-f]+|\\d+))?|0x[0-9a-f]+|\\d+)\\]", src) &&
                 var_later(dn))) &&
               !memname(src).empty() && out) {
      emit(out, dn + " = " + text_of(memname(src)) + ";");
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
  Value rsrc(const string &reg) {
    Value cur = getr(reg);
    if (!cur.empty() && mt("-?\\d+$|-?0x[0-9a-f]+$", cur.s())) {
      int j = future_store(reg, i);
      if (j != NI) {
        Value var = memname(split(ins[j].op_str, ", ")[0]);
        if (!hoisted.count(j) && out) {
          emit(out, text_of(var) + " = " + cur.s() + ";");
          hoisted.insert(j);
        }
        setr(reg, var);
        return var;
      }
    }
    return !cur.empty() ? cur : fresh(reg);
  }
  Value val(const string &tok) {
    if (tok == "ax") return !rg[AX].empty() ? rg[AX] : interned("ax");
    if (tok == "si" || tok == "di") {
      Value v = getr(tok);
      return !v.empty() ? v : fresh(tok);
    }
    return in(tok, {"bx", "cx", "dx"}) ? rsrc(tok) : rhs(tok);
  }

  // ------------------------------------------------------------------------------ liveness
  bool live(const string &reg, int i0) {
    const Rx &rx = RX("\\b(" + string(HALF(reg)) + ")\\b");
    std::set<int> seen;
    std::vector<int> work = {i0 + 1};
    while (!work.empty()) {
      int j = work.back();
      work.pop_back();
      while (j != NI && 0 <= j && j < N() && !seen.count(j)) {
        seen.insert(j);
        const platforms::Insn &ii = ins[j];
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

  // The registers an instruction writes (the 16-bit ones the model tracks, es when far pointers are modelled), in the model's fixed order.
  std::vector<string> wregs(const string &mn, const string &ops) {
    static const char *const kNames[7] = {"ax", "bx", "cx", "dx", "si", "di", "es"};
    unsigned written = 0;
    const auto add = [&](const string &n) {
      for (int q = 0; q < 7; q++)
        if (n == kNames[q]) written |= 1u << q;
    };
    const auto result = [&]() {
      std::vector<string> o;
      for (int q = 0; q < 7; q++)
        if (written & (1u << q)) o.push_back(kNames[q]);
      return o;
    };
    const string base_ = at(split_words(mn), -1);
    static const std::map<string, std::vector<string>> kImplicit = {
        {"mul", {"ax", "dx"}}, {"imul", {"ax", "dx"}}, {"div", {"ax", "dx"}}, {"idiv", {"ax", "dx"}},
        {"cwd", {"dx"}}, {"cdq", {"dx"}}, {"cbw", {"ax"}}, {"cwde", {"ax"}}, {"xlatb", {"ax"}},
        {"lodsb", {"ax", "si"}}, {"lodsw", {"ax", "si"}}, {"stosb", {"di"}}, {"stosw", {"di"}},
        {"movsb", {"si", "di"}}, {"movsw", {"si", "di"}}, {"scasb", {"di"}}, {"scasw", {"di"}},
        {"cmpsb", {"si", "di"}}, {"cmpsw", {"si", "di"}}, {"loop", {"cx"}},
        {"call", {"ax", "bx", "cx", "dx"}}, {"lcall", {"ax", "bx", "cx", "dx"}}};
    const auto it = kImplicit.find(base_);
    if (it != kImplicit.end())
      for (auto &x : it->second) add(x);
    if (starts(mn, "rep")) add("cx");
    if (in(base_, {"cmp", "test", "push", "jmp", "ret", "retf", "nop", "mul", "imul", "div", "idiv"}) || cjmp(base_) ||
        it != kImplicit.end())
      return result();
    std::vector<string> p;
    if (!ops.empty())
      for (auto &x : split(ops, ",")) p.push_back(strip(x));
    if (C.mark && C.far_pointers && (base_ == "les" || (!p.empty() && p[0] == "es" && in(base_, {"mov", "pop"})))) add("es");
    if (!p.empty()) {
      string d = isbyte(p[0]) ? parent(p[0]) : p[0];
      if (wr(d) >= 0) add(d);
      if (base_ == "xchg" && p.size() > 1) {
        d = isbyte(p[1]) ? parent(p[1]) : p[1];
        if (wr(d) >= 0) add(d);
      }
    }
    return result();
  }

  void kill_aliases(int i0) {
    const platforms::Insn &ii = I(i0);
    auto w = wregs(ii.mnemonic, ii.op_str);
    if (w.empty() || !out) return;
    for (auto &W : w) {
      const Rx &rx = RX("(?<![\\w>.])(" + string(HALF(W)) + ")(?![\\w(])");
      for (int O = 0; O < 6; O++) {
        if (W == WRN[O]) continue;
        Value v = rg[O];
        if (v.empty() || v.s() == WRN[O]) continue;
        if (rx.search(v.s())) {
          if (live(WRN[O], i0 - 1) || hascall(v)) materialize(out, WRN[O]);
          else rg[O] = Value();
        }
      }
      auto items = breg.v;
      for (auto &kv : items) {
        if (parent(kv.first) != W && rx.search(text_of(kv.second))) {
          if (live(parent(kv.first), i0 - 1)) push_out(out, kv.first + " = " + text_of(kv.second) + ";", i0);
          breg.pop0(kv.first);
        }
      }
      for (size_t k = 0; k < args.size(); k++) {
        if (in(ii.mnemonic, {"call", "lcall"})) break;
        if (rx.search(text_of(args[k]))) {
          string t = newt();
          push_out(out, t + " = " + text_of(args[k]) + ";", i0);
          args[k] = fresh(t);
        }
      }
    }
  }

  void pin(const List &o, const std::set<string> &regs) {
    std::vector<int> batch;
    for (int r = 0; r < 6; r++)
      if (regs.count(WRN[r]) && !rg[r].empty() && rg[r].s() != WRN[r] && (live(WRN[r], i) || hascall(rg[r])))
        batch.push_back(r);
    if (batch.size() > 1) {
      bool any = false;
      for (int a : batch)
        for (int b2 : batch)
          if (a != b2 && !any && sr("(?<![\\w>.])(" + string(HALF(WRN[a])) + ")(?![\\w(])", text_of(rg[b2]))) any = true;
      if (any)
        for (int r : batch) {
          string t = newt();
          push_out(o, t + " = " + text_of(rg[r]) + ";", i);
          rg[r] = fresh(t);
        }
    }
    for (int r = 0; r < 6; r++) {
      if (!regs.count(WRN[r])) continue;
      std::vector<string> bs;
      for (auto &kv : breg.v)
        if (parent(kv.first) == WRN[r]) bs.push_back(kv.first);
      for (auto &b8 : bs) {
        Value v = breg.pop(b8);
        if (text_of(v) != b8 && live(WRN[r], i)) push_out(o, b8 + " = " + text_of(v) + ";", i);
      }
      Value v = rg[r];
      if (v.empty() || v.s() == WRN[r]) continue;
      if (live(WRN[r], i) || hascall(v)) materialize(o, WRN[r]);
      else rg[r] = Value();
    }
  }

  std::set<string> mem_lazy() {
    std::set<string> o;
    for (int r = 0; r < 6; r++) {
      Value v = rg[r];
      if (!v.empty() && v.s() != WRN[r] && !mt("-?\\d+$", v.s())) o.insert(WRN[r]);
    }
    for (auto &kv : breg.v)
      if (!mt("-?\\d+$", text_of(kv.second))) o.insert(parent(kv.first));
    return o;
  }

  void tree_bids(const Seq &nodes, std::set<int> &acc) {
    for (auto &n : nodes) {
      switch (n.kind) {
        case Node::Kind::Block: acc.insert(n.block); break;
        case Node::Kind::If: acc.insert(n.block); tree_bids(n.body, acc); break;
        case Node::Kind::IfElse: acc.insert(n.block); tree_bids(n.body, acc); tree_bids(n.else_body, acc); break;
        case Node::Kind::While:
        case Node::Kind::Loop: acc.insert(n.block); tree_bids(n.body, acc); break;
        case Node::Kind::Switch:
          acc.insert(n.block);
          for (auto &c : n.arms) tree_bids(c.second, acc);
          break;
        case Node::Kind::DoWhile: acc.insert(n.block); acc.insert(n.join); tree_bids(n.body, acc); break;
        default: break;
      }
    }
  }
  bool has_break(const Seq &nodes) {
    for (auto &n : nodes) {
      if (n.kind == Node::Kind::Break) return true;
      if (n.kind == Node::Kind::If && has_break(n.body)) return true;
      if (n.kind == Node::Kind::IfElse && (has_break(n.body) || has_break(n.else_body))) return true;
      if (n.kind == Node::Kind::Switch)
        for (auto &c : n.arms)
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
    const Rx &rx = RX("\\b" + reg + "\\b");
    std::set<int> seen;
    std::vector<int> work = {i0 + 1};
    while (!work.empty()) {
      int j = work.back();
      work.pop_back();
      while (j != NI && 0 <= j && j < N() && !seen.count(j)) {
        seen.insert(j);
        const platforms::Insn &ii = ins[j];
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
    if (!getr(par).empty() || !out || !breg.has(d8)) return;
    if (wlive(par, i0)) {
      Value v = breg.pop(d8);
      if (text_of(v) != d8) push_out(out, d8 + " = " + text_of(v) + ";", i0);
    }
  }

  void materialize(const List &o, const string &reg, long long rank = LLONG_MIN) {
    Value v = getr(reg);
    if (v.empty() || v.s() == reg) return;
    const Rx &rx = RX("(?<![\\w>.])(" + string(HALF(reg)) + ")(?![\\w(])");
    string sv = v.s();
    for (size_t k2 = 0; k2 < args.size(); k2++) {
      string a2 = text_of(args[k2]);
      if (has(a2, sv) && sv.size() > reg.size()) args[k2] = fresh(replace_all(a2, sv, reg));
      else if (rx.search(a2)) {
        string t = newt();
        push_out(o, t + " = " + a2 + ";", i);
        args[k2] = fresh(t);
      }
    }
    std::vector<std::pair<string, string>> batch;
    for (int q = 0; q < 6; q++) {
      string on = WRN[q];
      if (on == reg) continue;
      Value ov = rg[q];
      if (ov.empty() || ov.s() == on) continue;
      if (has(ov.s(), sv) && sv.size() > reg.size()) rg[q] = fresh(replace_all(ov.s(), sv, reg));
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
        if (b.first != reg) setr(b.first, b.first == "ax" ? interned("ax") : Value());
      }
    } else {
      push_out(o, reg + " = " + sv + ";", rank == LLONG_MIN ? i : rank);
    }
    setr(reg, reg == "ax" ? interned("ax") : Value());
  }

  void emit(const List &o, string text, long long rank = LLONG_MIN, bool rc = false) {
    for (int r = 0; r < 6; r++) {
      Value v = rg[r];
      if (!v.empty() && v.s() != WRN[r] && hascall(v) && !has(text, v.s())) materialize(o, WRN[r], rank);
    }
    for (size_t k = 0; k < args.size(); k++)
      if (hascall(args[k]) && !has(text, text_of(args[k]))) {
        string t = newt();
        push_out(o, t + " = " + text_of(args[k]) + ";", i);
        args[k] = fresh(t);
      }
    if (!args.empty() && !mt("t\\d+ = ", text) && (mt("[^=(]*[^=!<>]=[^=]", text) || sr("\\+\\+|--", text)) &&
        !mt("(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh) = ", text)) {
      for (size_t k = 0; k < args.size(); k++)
        if (sr("g_|\\*|->|\\[", text_of(args[k]))) {
          string t = newt();
          push_out(o, t + " = " + text_of(args[k]) + ";", i);
          args[k] = fresh(t);
        }
    }
    for (int r = 0; r < 6; r++) {
      Value v = rg[r];
      if (v.empty() || v.s() == WRN[r] || !hascall(v) || !has(text, v.s())) continue;
      if (ends(text, " = " + v.s() + ";")) continue;
      if (live(WRN[r], i)) {
        materialize(o, WRN[r], rank);
        text = replace_all(text, v.s(), WRN[r]);
      } else {
        rg[r] = Value();
      }
    }
    auto m = mt("(.+?)(?: = (.*);|\\+\\+;|--;)$", text);
    if (m) {
      string g1 = group_of(m, 1);
      string g2 = m.has(2) ? group_of(m, 2) : "";
      string core = RX("^BYTE\\((.*)\\)$").replace_all(g1, "\\1");
      Rx pat8("(?<![\\w>.])" + escape(core) + "(?![\\w(])");
      auto items = breg.v;
      for (auto &kv : items) {
        string sv = text_of(kv.second);
        if (sv != kv.first && pat8.search(sv) && sv == g2) {
          breg.set(kv.first, fresh(g1));
          continue;
        }
        if (sv != kv.first && pat8.search(sv) && sv != g2) {
          breg.pop(kv.first);
          if (live(parent(kv.first), i)) push_out(o, kv.first + " = " + sv + ";", i);
        }
      }
    }
    if (C.mark && m && sr("->|\\*|\\[|g_", group_of(m, 1))) {
      for (int r = 0; r < 6; r++) {
        Value v = rg[r];
        if (v.empty() || v.s() == WRN[r] || (m.has(2) && v.s() == group_of(m, 2))) continue;
        if (sr("->|\\*|\\[|g_", v.s()) && live(WRN[r], i)) materialize(o, WRN[r], rank);
      }
    }
    if (m && wr(group_of(m, 1)) < 0 && !isbyte(group_of(m, 1))) {
      string lhs = RX("^BYTE\\((.*)\\)$").replace_all(group_of(m, 1), "\\1");
      Rx pat("(?<![\\w>.])" + escape(lhs) + "(?![\\w(])");
      for (int r = 0; r < 6; r++) {
        Value v = rg[r];
        if (v.empty() || v.s() == WRN[r] || (m.has(2) && v.s() == group_of(m, 2))) continue;
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
      setr(parent(d0), Value());
      if (!tracked_byte(mn)) breg.pop0(d0);
    }
  }

  bool byte_on_word(const string &mn, const string &ops) {
    if (!in(mn, {"and", "or", "xor"}) || !has(ops, ", ")) return false;
    auto p = split_at_most(ops, ", ", 1);
    const string &d8 = p[0], &src = p[1];
    if (!isbyte(d8) || breg.has(d8) || !is_number(src)) return false;
    string par = parent(d8);
    Value cur = getr(par);
    if (!in(par, {"ax", "bx", "cx", "dx"}) || cur.empty()) return false;
    long long k = num(src);
    bool hi = ends(d8, "h");
    if (mn == "and") k = hi ? ((k << 8) | 0xFF) : (0xFF00 | k);
    else if (hi) k <<= 8;
    setr(par, fresh("(" + cur.s() + " " + ALU(mn) + " 0x" + hex(k) + ")"));
    if (par == "ax") axsrc = NI, retlive = true;
    if (par == "bx") bxsrc = NI;
    return true;
  }

  static bool wordish(const string &v) { return (bool)fm("[\\w\\->.]+", v); }

  void expr_step(int i0) {
    i = i0;
    Value old = rg[AX];
    const platforms::Insn &ii = I(i0);
    const string &mn = ii.mnemonic, &ops = ii.op_str;
    if (in(mn, {"or", "and", "test"}) && count_of(ops, ",") == 1 && !has(ops, "[") &&
        strip(split(ops, ",")[0]) == strip(split(ops, ",")[1]))
      return;
    kill_aliases(i0);
    if (byte_on_word(mn, ops)) return;
    string d_ = strip(split(ops, ",")[0]);
    Value sv_;
    bool has_sv = false;
    if (mn == "mov" && count_of(ops, ", ") == 1 && isbyte(split(ops, ", ")[0]) && isbyte(split(ops, ", ")[1])) {
      sv_ = (s8_on && s8_i == i0) ? s8_v : bval(split(ops, ", ")[1]);
      has_sv = true;
    }
    if (!(sm_on && sm_i == i0)) {
      auto o2 = split_at_most(ops, ", ", 1);
      if (mn == "mov" && isbyte(d_) && o2.size() == 2 && has(o2[1], "[")) {
        sm_on = true;
        sm_i = i0;
        sm_v = memname(o2[1]);
      } else {
        sm_on = false;
      }
    }
    Value pre8;
    if (p8_on && p8_i == i0) pre8 = p8_v;
    else if (isbyte(d_)) pre8 = bval(d_);
    binval(mn, d_);
    if (mn == "mov" && isbyte(split(ops, ", ")[0])) {
      string d8 = split(ops, ", ")[0];
      string src = at(split_at_most(ops, ", ", 1), 1);
      if (is_number(src)) breg.set(d8, from_int(num(src)));
      else if (isbyte(src)) breg.set(d8, has_sv && !sv_.empty() ? sv_ : (breg.has(src) ? breg.get(src) : fresh(src)));
      else {
        Value v = (sm_on && sm_i == i0 && truthy(sm_v)) ? sm_v : memname(src);
        breg.set(d8, truthy(v) ? v : fresh(d8));
      }
      string par = parent(d8);
      if (wr(par) >= 0) {
        Value pv = getr(par);
        if (!pv.empty() && pv.s() != par && out && wlive(par, i0)) {
          Value keep = breg.pop(d8);
          materialize(out, par);
          breg.set(d8, keep);
        }
        setr(par, Value());
      }
      byte_settle(d8, i0);
      return;
    }
    if (isalu(mn) && isbyte(split(ops, ", ")[0])) {
      string d8 = split(ops, ", ")[0];
      string src = at(split_at_most(ops, ", ", 1), 1);
      if ((mn == "sub" || mn == "xor") && src == d8) {
        breg.set(d8, interned("0"));
        string lo = d8 == "ah" ? "al" : d8 == "bh" ? "bl" : d8 == "ch" ? "cl" : d8 == "dh" ? "dl" : "";
        if (!lo.empty() && breg.has(lo)) {
          setr(parent(d8), fresh(b8(breg[lo])));
          if (d8 == "ah") axsrc = NI, retlive = true;
        } else {
          Value w_;
          bool hw = false;
          if (pw_on && pw_i == i0) w_ = pw_v, hw = true;
          if (!lo.empty() && hw && !w_.empty() && w_.s() != parent(d8)) {
            setr(parent(d8), fresh("(" + w_.s() + " & 0xff)"));
            breg.pop0(d8);
            if (d8 == "ah") axsrc = NI, retlive = true;
          } else {
            byte_settle(d8, i0);
          }
        }
        return;
      }
      if (msc3 && !C.mark && in(d8, {"ah", "bh", "ch", "dh"}) && in(mn, {"or", "xor", "and"}) && is_number(src) && !breg.has(d8)) {
        string w_ = parent(d8);
        long long k_ = num(src);
        Value cur_ = getr(w_);
        string cs_ = !cur_.empty() ? cur_.s() : w_;
        long long kw = mn != "and" ? (k_ << 8) : ((k_ << 8) | 0xff);
        setr(w_, fresh("(" + cs_ + " " + (mn == "or" ? "|" : mn == "xor" ? "^" : "&") + " 0x" + hex(kw) + ")"));
        if (w_ == "ax") axsrc = NI, retlive = true;
        return;
      }
      Value cur = breg.has(d8) ? breg.get(d8) : (!pre8.empty() ? pre8 : fresh(d8));
      string opc = ALU(mn);
      string sv;
      if (is_number(src)) sv = decimal(num(src));
      else if (isbyte(src)) sv = C.dead_registers ? text_of(bval(src)) : text_of(breg.has(src) ? breg.get(src) : fresh(src));
      else sv = text_of(or_else(memname(src), src.c_str()));
      breg.set(d8, fresh("(" + text_of(cur) + " " + opc + " " + sv + ")"));
      byte_settle(d8, i0);
      return;
    }
    if (in(mn, {"inc", "dec"}) && isbyte(ops)) {
      Value c = breg.has(ops) ? breg.get(ops) : (!pre8.empty() ? pre8 : fresh(ops));
      breg.set(ops, fresh("(" + text_of(c) + " " + (mn == "inc" ? "+" : "-") + " 1)"));
      return;
    }
    if (in(mn, {"cbw", "cwde"}) && breg.has("al")) {
      rg[AX] = C.mark ? fresh("SX(" + text_of(breg["al"]) + ")") : breg["al"];
      axsrc = NI;
      cbw = fresh(text_of(rg[AX]));
      if (msc3 && !C.mark && i0 > 0 && I(i0 - 1).mnemonic == "mov") {
        auto mb = mt("al, byte ptr (\\[bp [+-] (?:0x[0-9a-f]+|\\d+)\\])$", I(i0 - 1).op_str);
        if (mb) {
          bool any = false;
          for (auto &x : ins)
            if (has(x.op_str, "word ptr " + group_of(mb, 1))) {
              any = true;
              break;
            }
          if (any) {
            string v_ = text_of(rg[AX]);
            rg[AX] = fresh("(char)" + (fm("[\\w\\->.]+", v_) ? v_ : "(" + v_ + ")"));
          }
        }
      }
      if (C.dead_registers) breg.pop0("al"), breg.pop0("ah"), retlive = true;
      if (msc3) retlive = true;
      return;
    }
    if (in(mn, {"cbw", "cwde"}) && C.mark) {
      rg[AX] = fresh("SX(" + text_of(or_else(rg[AX], "ax")) + ")");
      if (rg[AX].empty()) {}
      axsrc = NI;
      retlive = true;
      return;
    }
    if (in(mn, {"cbw", "cwde"}) && msc3 && !rg[AX].empty() && !in(rg[AX].s(), {"ax", "al"})) {
      string v_ = rg[AX].s();
      rg[AX] = fresh("(char)" + (fm("[\\w\\->.]+", v_) ? v_ : "(" + v_ + ")"));
      axsrc = NI;
      retlive = true;
      return;
    }
    if (in(mn, {"cbw", "cwde"}) && msc3) {
      rg[AX] = interned("al");
      axsrc = NI;
      retlive = true;
      return;
    }
    if (mn == "mov" && starts(ops, "ax, ")) {
      string src = slice(ops, 4);
      if (src == "dx" && has_div) rg[AX] = fresh("(" + text_of(div0) + " % " + text_of(div1) + ")"), axsrc = NI;
      else if (in(src, {"bx", "cx", "dx"})) rg[AX] = rsrc(src), axsrc = NI;
      else if (src == "si" || src == "di") {
        Value v = getr(src);
        rg[AX] = !v.empty() ? v : fresh(src);
        axsrc = NI;
      } else if (is_number(src)) rg[AX] = rhs(src), axsrc = i0;
      else rg[AX] = rhs(src), axsrc = NI;
    } else if (mn == "mov" && (starts(ops, "si, ") || starts(ops, "di, "))) {
      Value d = fresh(slice(ops, 0, 2));
      string src = slice(ops, 4);
      if (msc3 && is_number(src) && saved_has(d.s()) && out) {
        emit(out, d.s() + " = " + decimal(num(src)) + ";");
        setr(d.s(), Value());
      } else {
        movptr(d, src);
      }
    } else if (in(mn, {"les", "lds"})) {
      auto pr = partition(ops, ", ");
      if (wr(pr.first) >= 0) setr(pr.first, memname(pr.second));
    } else if (mn == "lea") {
      auto pr = partition(ops, ", ");
      Value nm = memname(pr.second);
      if (wr(pr.first) >= 0 && truthy(nm)) setr(pr.first, fresh("&" + nm.s()));
    } else if (mn == "xchg") {
      auto o = split(ops, ", ");
      if (o.size() == 2 && wr(o[0]) >= 0 && wr(o[1]) >= 0) {
        Value va = getr(o[0]), vb = getr(o[1]);
        setr(o[0], !vb.empty() ? vb : fresh(o[1]));
        setr(o[1], !va.empty() ? va : fresh(o[0]));
      }
    } else if (mn == "mov" && starts(ops, "bx, ")) {
      string src = slice(ops, 4);
      if (in(src, {"ax", "cx", "dx", "si", "di", "bp"})) movptr(interned("bx"), src), bxsrc = NI;
      else rg[BX] = rhs(src), bxsrc = is_number(src) ? i0 : NI;
    } else if (mn == "mov" && starts(ops, "cx, ")) {
      string src = slice(ops, 4);
      if (in(src, {"ax", "bx", "dx", "si", "di", "bp"})) movptr(interned("cx"), src);
      else rg[CX] = rhs(src);
    } else if (mn == "mov" && starts(ops, "dx, ")) {
      string src = slice(ops, 4);
      if (in(src, {"ax", "bx", "cx", "si", "di", "bp"})) movptr(interned("dx"), src);
      else rg[DX] = rhs(src);
    } else if (in(mn, {"xor", "sub"}) && ops == "ax, ax") {
      rg[AX] = interned("0"), axsrc = NI;
    } else if (in(mn, {"xor", "sub"}) && ops == "bx, bx") {
      rg[BX] = interned("0"), bxsrc = NI;
    } else if (in(mn, {"cwd", "cdq"})) {
      string v_ = !rg[AX].empty() ? rg[AX].s() : "ax";
      if (mt("\\d+$", v_)) rg[DX] = (parse_number(v_) & 0x8000) ? interned("65535") : interned("0");
      else rg[DX] = fresh("(0 - (" + v_ + " >> 15))");
      if (msc3 && !C.mark && mt("\\d+$", v_) && i0 + 2 < N() && ins[i0 + 1].mnemonic == "push" &&
          ins[i0 + 1].op_str == "dx" && ins[i0 + 2].mnemonic == "push" && ins[i0 + 2].op_str == "ax")
        rg[DX] = fresh("(0 - (" + v_ + " >> 15))");
    } else if (in(mn, {"or", "and"}) && ops == "ax, ax") {
    } else {
      string axv = text_of(!rg[AX].empty() ? rg[AX] : interned("ax"));
      string cxv = text_of(!rg[CX].empty() ? rg[CX] : interned("cx"));
      string d0 = strip(split(ops, ",")[0]);
      auto reg_text = [&](const string &r) { Value v = getr(r); return !v.empty() ? v.s() : r; };
      auto shcnt = [&]() -> string {
        if (ends(ops, "cl")) return breg.has("cl") ? text_of(breg["cl"]) : cxv;
        return text_of(val(at(split(ops, ", "), 1)));
      };
      if (isalu(mn) && starts(ops, "ax, ")) rg[AX] = fresh("(" + axv + " " + ALU(mn) + " " + text_of(val(slice(ops, 4))) + ")");
      else if (in(mn, {"imul", "mul"})) {
        if (mn == "mul" && msc3 && !C.mark && !starts(axv, "(unsigned)"))
          axv = "(unsigned)" + (wordish(axv) ? axv : "(" + axv + ")");
        rg[AX] = fresh("(" + axv + " * " + text_of(val(ops)) + ")");
      } else if (in(mn, {"idiv", "div"})) {
        Value rn = val(ops);
        if (mn == "div" && msc3 && !C.mark && !starts(axv, "(unsigned)"))
          axv = "(unsigned)" + (wordish(axv) ? axv : "(" + axv + ")");
        has_div = true;
        div0 = fresh(axv);
        div1 = rn;
        rg[AX] = fresh("(" + axv + " / " + text_of(rn) + ")");
        rg[DX] = fresh("(" + axv + " % " + text_of(rn) + ")");
      } else if (in(mn, {"shl", "sal"}) && wr(d0) >= 0) {
        string a = reg_text(d0);
        string c = shcnt();
        setr(d0, fresh("(" + a + " << " + c + ")"));
      }
      else if (in(mn, {"shr", "sar"}) && wr(d0) >= 0) {
        string a = reg_text(d0);
        string c = shcnt();
        setr(d0, fresh("(" + shr(C, mn, a, c) + ")"));
      }
      else if (mn == "neg" && wr(d0) >= 0) setr(d0, fresh("(-" + reg_text(d0) + ")"));
      else if (mn == "not" && wr(d0) >= 0) setr(d0, fresh("(~" + reg_text(d0) + ")"));
    }
    if (!rg[AX].same_as(old)) retlive = true;
  }

  // ------------------------------------------------------------------------------ conditions
  std::pair<string, int> cond_at(int i0) {
    int j = i0;
    auto resolve = [&](const string &t) -> Value {
      if (t == "ax") return !rg[AX].empty() ? rg[AX] : interned("ax");
      if (t == "bx") return !rg[BX].empty() ? rg[BX] : interned("bx");
      if (in(t, {"cx", "dx", "si", "di"})) {
        Value v = getr(t);
        return !v.empty() ? v : fresh(t);
      }
      if (isbyte(t)) return bval(t, true);
      Value v = memname(t);
      return !v.empty() ? v : (is_number(t) ? from_int(num(t)) : fresh(t));
    };
    while (j < N() && !cjmp(ins[j].mnemonic)) {
      const string &mn = ins[j].mnemonic, &ops = ins[j].op_str;
      if (mn == "cmp") {
        auto p = split(ops, ", ");
        Value a = resolve(p[0]), b = resolve(at(p, 1));
        has_cmp = true, cmp0 = a, cmp1 = b;
        auto bs = fm("byte ptr (\\[bp [+-] (?:0x[0-9a-f]+|\\d+)\\])", p[0]);
        if (msc3 && !C.mark && bs) {
          bool any = false;
          for (auto &x : ins)
            if (has(x.op_str, "word ptr " + group_of(bs, 1))) {
              any = true;
              break;
            }
          if (any && fm("\\w+", text_of(cmp0))) {
            bool k_ok = (bool)fm("0x[0-9a-f]+|\\d+", p[1]);
            long long k_ = k_ok ? num(p[1]) : 0;
            Value nb = (k_ok && k_ >= 0x80) ? fresh("(char)0x" + hex(k_)) : cmp1;
            cmp0 = fresh("(char)" + text_of(cmp0));
            cmp1 = nb;
          }
        }
      } else if (mn == "test") {
        auto p = split(ops, ", ");
        Value a0 = resolve(p[0]);
        has_cmp = true;
        cmp0 = (p.size() > 1 && p[0] == p[1]) ? a0 : fresh("(" + text_of(a0) + " & " + text_of(resolve(at(p, 1))) + ")");
        cmp1 = interned("0");
      } else {
        expr_step(j);
      }
      j++;
    }
    if (j >= N()) return {text_of(!rg[AX].empty() ? rg[AX] : interned("0")), j};
    string jcc = ins[j].mnemonic;
    auto width = [&]() -> string {
      int k = j - 1;
      while (k >= 0 && in(ins[k].mnemonic, {"mov", "lea", "push", "pop", "les", "cbw", "cwde"})) k--;
      string o = k >= 0 ? ins[k].op_str : "";
      return (has(o, "byte ptr") || isbyte(strip(split(o, ",")[0]))) ? "S8" : "S16";
    };
    if (jcc == "js" || jcc == "jns") {
      string w = width();
      string v = has_cmp ? "(" + text_of(cmp0) + " - " + text_of(cmp1) + ")" : text_of(flagval(j));
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
          Value a = resolve(p[0]), b = resolve(at(p, 1));
          has_cmp = true, cmp0 = a, cmp1 = b;
          break;
        }
        if (mk == "test") {
          auto p = split(ins[k].op_str, ", ");
          Value a0 = resolve(p[0]);
          has_cmp = true;
          cmp0 = p[0] == at(p, 1) ? a0 : fresh("(" + text_of(a0) + " & " + text_of(resolve(p[1])) + ")");
          cmp1 = interned("0");
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
            string key = text_of(it->second.a) + "\x01" + text_of(it->second.b) + "\x01" + it->second.w;
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
      string a_ = text_of(cmp0), b_ = text_of(cmp1);
      string w;
      if (sg.empty() && C.mark) w = width();
      else w = sg;
      has_lastcmp = true;
      lastcmp = LC{cmp0, cmp1, w.empty() ? "S16" : w};
      cmpat[j] = lastcmp;
      if (!sg.empty()) a_ = sg + "(" + a_ + ")", b_ = sg + "(" + b_ + ")";
      else if (C.unsigned_compares && !C.mark && in(jcc, {"jb", "jae", "ja", "jbe", "jnb", "jnae", "jna", "jnbe", "jc", "jnc"})) {
        if (mt("-\\d+$", b_)) b_ = "0x" + hex(parse_number(b_) & 0xFFFF);
        a_ = "(unsigned)" + a_;
      }
      const char *iv = inv_get(jcc);
      cond = "(" + a_ + " " + (iv ? iv : "?") + " " + b_ + ")";
    } else {
      string v = text_of(flagval(j));
      const char *op = inv_get(jcc);
      if (!sg.empty()) v = sg + "(" + v + ")";
      cond = op ? "(" + v + " " + op + " 0)" : v;
    }
    for (int r = 0; r < 6; r++) {
      Value v = rg[r];
      if (!v.empty() && hascall(v) && has(cond, v.s())) rg[r] = Value();
    }
    return {cond, j};
  }
  static bool flagset(const string &m) {
    return in(m, {"add", "sub", "and", "or", "xor", "inc", "dec", "neg", "shl", "sal", "shr", "sar", "adc", "sbb"});
  }
  Value flagval(int j) {
    int k = j - 1;
    while (k >= 0) {
      const string &mn = ins[k].mnemonic;
      if (in(mn, {"call", "lcall", "jmp", "ret", "retf"}) || cjmp(mn)) break;
      if (flagset(mn)) {
        string d0 = strip(split(ins[k].op_str, ",")[0]);
        if (d0 == "ax") break;
        if (in(d0, {"bx", "cx", "dx", "si", "di"})) {
          Value v = getr(d0);
          return !v.empty() ? v : fresh(d0);
        }
        if (isbyte(d0)) return bval(d0, true);
        if (has(d0, "[")) return or_else(memname(d0), "ax");
        break;
      }
      k--;
    }
    return !rg[AX].empty() ? rg[AX] : interned("ax");
  }
  static string b8(const Value &v0) {
    string v = text_of(v0);
    if (mt("(BYTE\\([^()]*(\\([^()]*\\))*[^()]*\\)|\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5]|[a-d][lh])$", v) || ends(v, "& 0xff)") ||
        !sr("[-+*<>]", v))
      return v;
    return "(" + v + " & 0xff)";
  }
  Value bval(const string &r8, bool trunc = false) {
    if (breg.has(r8)) return trunc ? fresh(b8(breg[r8])) : breg[r8];
    Value pv = getr(parent(r8));
    if (pv.empty()) return fresh(r8);
    if (!ends(r8, "l")) return fresh("((" + pv.s() + " >> 8) & 0xff)");
    if (trunc && !mt("(BYTE\\(.*\\)|\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$", pv.s())) return fresh("(" + pv.s() + " & 0xff)");
    return pv;
  }

  // ------------------------------------------------------------------------------ longs
  string lparam(const string &op) {
    auto m = sr("\\[bp \\+ (0x[0-9a-f]+|\\d+)\\]", op);
    if (!m) broken("a match that is not there");
    long long off = num(group_of(m, 1));
    long long idx = floor_quotient(off - fs - 4, 4);
    lmax = std::max(lmax, idx);
    return utf8_char('a' + idx);
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
      rg[AX] = fresh("(" + a + " " + o + " " + b + ")");
      return 8;
    }
    if (i0 + 3 < N() && mnm(i0) == "mov" && starts(op(i0), "ax, word ptr [bp") && in(mnm(i0 + 1), {"add", "sub"}) &&
        starts(op(i0 + 1), "ax, word ptr [bp") && mnm(i0 + 2) == "mov" && starts(op(i0 + 2), "bx, word ptr [bp") &&
        in(mnm(i0 + 3), {"adc", "sbb"})) {
      longfn = true;
      string a = lparam(op(i0)), b = lparam(op(i0 + 1));
      rg[AX] = fresh("(" + a + " " + (mnm(i0 + 1) == "add" ? "+" : "-") + " " + b + ")");
      return 4;
    }
    if (i0 + 5 < N() && starts(op(i0), "ax, word ptr [bp") && starts(op(i0 + 1), "bx, word ptr [bp") &&
        starts(op(i0 + 3), "dx, word ptr [bp") && mnm(i0 + 4) == "call" && op(i0 + 5) == "ax, bx") {
      longfn = true;
      string a = lparam(op(i0 + 1)), b = lparam(op(i0 + 3));
      string o = call_op(i0 + 4, "*");
      rg[AX] = fresh("(" + a + " " + o + " " + b + ")");
      return 6;
    }
    if (i0 + 3 < N() && mnm(i0) == "mov" && mt("ax, word ptr \\[bp - ", op(i0)) && mnm(i0 + 1) == "mov" &&
        mt("dx, word ptr \\[bp - ", op(i0 + 1)) && in(mnm(i0 + 2), {"add", "sub"}) && mt("ax, (0x[0-9a-f]+|\\d+)$", op(i0 + 2)) &&
        in(mnm(i0 + 3), {"adc", "sbb"}) && op(i0 + 3) == "dx, 0" && ((mnm(i0 + 2) == "add") == (mnm(i0 + 3) == "adc"))) {
      longfn = true;
      Value lo = memname(at(split_at_most(op(i0), ", ", 1), 1));
      string o = mnm(i0 + 2) == "add" ? "+" : "-";
      rg[AX] = fresh("(" + text_of(lo) + " " + o + " " + decimal(num(at(split(op(i0 + 2), ", "), 1))) + ")");
      return 4;
    }
    return 0;
  }
  int long_stmt(int i0, const List &o) {
    if (i0 + 1 >= N()) return 0;
    const platforms::Insn &a = ins[i0], &b = ins[i0 + 1];
    if (in(a.mnemonic, {"add", "sub"}) && in(b.mnemonic, {"adc", "sbb"}) && ((a.mnemonic == "add") == (b.mnemonic == "adc"))) {
      auto ma = mt("word ptr \\[0x([0-9a-f]+)\\], (0x[0-9a-f]+|\\d+)$", a.op_str);
      auto mb = mt("word ptr \\[0x([0-9a-f]+)\\], 0$", b.op_str);
      if (ma && mb) {
        long long lo = parse_hex(group_of(ma, 1)), hi = parse_hex(group_of(mb, 1));
        if (hi == lo + 2) {
          emit(o, "*(long *)&g_" + hex(lo) + " " + (a.mnemonic == "add" ? "+" : "-") + "= " + decimal(num(group_of(ma, 2))) + ";");
          return 2;
        }
      }
    }
    return 0;
  }
  Value opval(const string &tok0) {
    string tok = strip(tok0);
    if (is_number(tok)) return from_int(num(tok));
    if (tok == "ax") return !rg[AX].empty() ? rg[AX] : interned("ax");
    if (wr(tok) >= 0) {
      Value v = getr(tok);
      return !v.empty() ? v : fresh(tok);
    }
    if (isbyte(tok)) return bval(tok, true);
    return or_else(memname(tok), tok.c_str());
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
  void phi_args(const std::vector<Value> &a_args, const std::vector<Value> &b_args, std::vector<string> &a_body,
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
}  // namespace cb::invert::early_c_model
