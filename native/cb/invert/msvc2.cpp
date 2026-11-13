// cb-unity: alone
// The Microsoft C/C++ 2.0 (32-bit) code generator: forward (construct -> bytes), inverse (bytes -> construct) and the register-dataflow walk.
#include "cb/invert/msvc2.hpp"

#include <algorithm>
#include <map>
#include <optional>
#include <set>
#include <exception>
#include <sstream>

#include "cb/invert/model_text.hpp"
#include "cb/invert/operand_match.hpp"
#include "cb/platforms/decoder.hpp"

namespace cb::invert::msvc2 {

namespace detail {

using namespace cb::invert::model;

// A node part as text: a string as it is, an integer in decimal.
std::string show(const Term &v) {
  if (v.kind() == Term::Kind::Str) return v.text();
  if (v.kind() == Term::Kind::Bool) return v.number() ? "True" : "False";
  return std::to_string(v.number());
}


// The 32-bit instructions of `code` at address 0, up to the first byte that does not decode.
std::vector<platforms::Insn> disasm32(const std::uint8_t *code, std::size_t n, long long address) {
  thread_local platforms::DecoderSet decoders;
  return decoders.get(platforms::Arch::X86, 32).disasm(ByteView(code, n), static_cast<std::uint64_t>(address));
}

typedef std::string ByteString;
typedef std::optional<ByteString> OB;
typedef std::optional<long long> OI;

// ---- bytes, as Python's bytes([...]) and struct.pack build them ------------
ByteString bb(std::initializer_list<long long> xs) {
  ByteString o;
  for (long long x : xs) {
    if (x < 0 || x > 255) fail( "bytes must be in range(0, 256)");
    o += (char)(unsigned char)x;
  }
  return o;
}
ByteString pack_i(long long v) {   // struct.pack("<i", v)
  if (v < -2147483648LL || v > 2147483647LL) unsupported( "argument out of range");
  unsigned u = (unsigned)(int)v;
  return bb({u & 0xff, (u >> 8) & 0xff, (u >> 16) & 0xff, (u >> 24) & 0xff});
}
ByteString pack_H(long long v) {   // struct.pack("<H", v)
  if (v < 0 || v > 0xffff) unsupported( "ushort format requires 0 <= number <= 65535");
  return bb({v & 0xff, (v >> 8) & 0xff});
}

Term A(long long k) { return Term::tuple({Term::string("arg"), Term::integer(k)}); }
Term C(long long v) { return Term::tuple({Term::string("const"), Term::integer(v)}); }

bool in_ops(const Term &op) {   // op in OPS
  static const char *O[] = {"+", "-", "&", "|", "^", "*"};
  if (op.kind() != Term::Kind::Str) return false;
  for (auto o : O)
    if (op.text() == o) return true;
  return false;
}

// =========================================================================
// msvc2_forward: construct -> bytes
// =========================================================================
enum { EAX = 0, ECX = 1, EDX = 2 };
const ByteString PRO = "\x55\x8b\xec";   // push ebp; mov ebp,esp

long long argoff(long long k) { return 4 + 4 * k; }
ByteString ebp_rm(int reg, long long off) { return bb({0x40 | (reg << 3) | 0x05, off & 0xFF}); }
ByteString esp_rm(int reg, long long off) { return bb({0x40 | (reg << 3) | 0x04, 0x24, off & 0xFF}); }
ByteString ld(int reg, long long k) { return bb({0x8B}) + ebp_rm(reg, argoff(k)); }
ByteString push_arg(long long k) { return bb({0xFF, 0x75, argoff(k) & 0xFF}); }

int memop_code(const Term &op) {   // _MEMOP[op]
  static const std::pair<const char *, int> M[] = {{"+", 0x03}, {"-", 0x2B}, {"&", 0x23}, {"|", 0x0B}, {"^", 0x33}};
  if (op.kind() == Term::Kind::Str)
    for (auto &m : M)
      if (op.text() == m.first) return m.second;
  fail( "'" + op.text() + "'");
}
bool in_memop(const Term &op) {
  return op.kind() == Term::Kind::Str && (op.text() == "+" || op.text() == "-" || op.text() == "&" || op.text() == "|" || op.text() == "^");
}
ByteString memop(const Term &op, int reg, long long k) {
  if (op.is("*")) return bb({0x0F, 0xAF}) + ebp_rm(reg, argoff(k));
  return bb({memop_code(op)}) + ebp_rm(reg, argoff(k));
}
ByteString epi(long long nargs) { return bb({0x5D, 0xC2}) + pack_H(4 * nargs); }
ByteString ret_n(long long nargs) { return nargs == 0 ? bb({0xC3}) : bb({0xC2}) + pack_H(4 * nargs); }

OB op_const_bytes(const Term &op, const Term &cv) {
  long long c = cv.number();
  if (!(-128 <= c && c <= 127)) return std::nullopt;
  if (op.is("+")) return c == 1 ? bb({0x40}) : c == -1 ? bb({0x48}) : bb({0x83, 0xC0, c & 0xFF});
  if (op.is("-")) return c == 1 ? bb({0x48}) : c == -1 ? bb({0x40}) : bb({0x83, 0xC0, (-c) & 0xFF});
  if (op.is("&")) return bb({0x83, 0xE0, c & 0xFF});
  if (op.is("|")) return bb({0x0C, c & 0xFF});
  if (op.is("^")) return bb({0x83, 0xF0, c & 0xFF});
  if (op.is("*")) return c == 5 ? bb({0x8D, 0x04, 0x80}) : bb({0x6B, 0xC0, c & 0xFF});
  return std::nullopt;
}

// _flatten_chain: (op, [arg leaves left to right]) of a one-operator left chain
bool flatten_chain(const Term &E, Term &op, std::vector<long long> &leaves) {
  if (!E[0].is("binop")) return false;
  op = E[1];
  leaves.clear();
  const Term *node = &E;
  while ((*node)[0].is("binop") && (*node)[1] == op) {
    if (!(*node)[3][0].is("arg")) return false;
    leaves.push_back((*node)[3][1].number());
    node = &(*node)[2];
  }
  if (!(*node)[0].is("arg")) return false;
  leaves.push_back((*node)[1].number());
  std::reverse(leaves.begin(), leaves.end());
  return true;
}

ByteString const_body(long long v) {
  if (v == 0) return bb({0x33, 0xC0});
  if (-128 <= v && v <= 127) return bb({0x6A, v & 0xFF, 0x58});
  return bb({0xB8}) + pack_i(v);
}

OB ret_body(const Term &E) {
  const Term &k = E[0];
  if (k.is("const")) return const_body(E[1].number());
  if (k.is("arg")) return ld(EAX, E[1].number());
  if (k.is("binop")) {
    const Term &op = E[1], &Lh = E[2], &R = E[3];
    if (Lh[0].is("arg") && R[0].is("const")) {
      OB ob = op_const_bytes(op, R[1]);
      if (!ob) return std::nullopt;
      return ld(EAX, Lh[1].number()) + *ob;
    }
    if (Lh[0].is("arg") && R[0].is("arg")) {
      long long li = Lh[1].number(), ri = R[1].number();
      if (op.is("*") && li == ri) return ld(EAX, li) + bb({0x0F, 0xAF, 0xC0});
      if (op.is("+")) return ld(EAX, ri) + ld(ECX, li) + bb({0x03, 0xC1});
      if (op.is("-")) return ld(EAX, li) + memop(op, EAX, ri);
      return ld(EAX, ri) + memop(op, EAX, li);
    }
  }
  return std::nullopt;
}

OB emit_call_seq(const Term &cargs, bool indirect);

OB materialize(const Term &Vv) {
  const Term &k = Vv[0];
  if (k.is("arg")) return ld(EAX, Vv[1].number());
  if (k.is("const")) return const_body(Vv[1].number());
  if (k.is("global")) return bb({0xA1, 0, 0, 0, 0});
  if (k.is("call")) return emit_call_seq(Vv[2], false);
  if (k.is("icall") || k.is("icall_ret")) return emit_call_seq(Vv[1], true);
  if (k.is("load") || k.is("deref") || k.is("index")) {
    const Term *base;
    OI d;
    if (k.is("deref")) {
      base = &Vv[1];
      d = 0;
    } else if (k.is("index")) {
      base = &Vv[1];
      if (Vv[2][0].is("const")) d = Vv[2][1].number() * 4;
    } else {
      base = &Vv[1];
      d = Vv[2].number();
    }
    if (!d) return std::nullopt;
    OB b = materialize(*base);
    if (!b) return std::nullopt;
    ByteString mem;
    if (*d == 0) mem = bb({0x8B, 0x00});
    else if (-0x80 <= *d && *d < 0x80) mem = bb({0x8B, 0x40, *d & 0xFF});
    else mem = bb({0x8B, 0x80}) + pack_i(*d);
    return *b + mem;
  }
  if (k.is("callbinret")) {
    if (Vv.size() != 4) fail( "unpack");
    const Term &op = Vv[1], &call = Vv[2], &other = Vv[3];
    OB b = materialize(call);
    if (!b) return std::nullopt;
    if (other[0].is("arg")) return *b + memop(op, EAX, other[1].number());
    OB ob = op_const_bytes(op, other[1]);
    if (!ob) return std::nullopt;
    return *b + *ob;
  }
  if (k.is("binop")) {
    const Term &op = Vv[1], &Lh = Vv[2], &R = Vv[3];
    if (!in_ops(op)) return std::nullopt;
    if (R[0].is("const")) {
      OB b = materialize(Lh);
      OB ob = op_const_bytes(op, R[1]);
      if (b && ob) return *b + *ob;
      return std::nullopt;
    }
    return std::nullopt;
  }
  return std::nullopt;
}

OB emit_call_seq(const Term &cargs, bool indirect) {
  ByteString seq;
  if (!cargs.is_sequence()) broken( "argument to reversed() must be a sequence");
  for (size_t j = cargs.items().size(); j-- > 0;) {
    const Term &a = cargs.items()[j];
    if (a[0].is("arg")) {
      seq += push_arg(a[1].number());
    } else if (a[0].is("const")) {
      long long v = a[1].number();
      seq += (-128 <= v && v <= 127) ? bb({0x6A, v & 0xFF}) : bb({0x68}) + pack_i(v);
    } else {
      return std::nullopt;
    }
  }
  return seq + (indirect ? bb({0xFF, 0x15, 0, 0, 0, 0}) : bb({0xE8, 0, 0, 0, 0}));
}

OB emit_stmt(const Term &st) {
  if (st[0].is("voidcall") && st[1].is("__stdcall")) return emit_call_seq(st[2], false);
  if (st[0].is("ivoidcall")) return emit_call_seq(st[1], true);
  if (st[0].is("store")) {
    if (st.size() != 4) fail( "unpack");
    const Term &base = st[1], &dispv = st[2], &value = st[3];
    if (value == C(0)) {
      OB b = materialize(base);
      if (!b) return std::nullopt;
      long long disp = dispv.number();
      if (disp == 0) return *b + bb({0x83, 0x20, 0x00});
      if (-0x80 <= disp && disp < 0x80) return *b + bb({0x83, 0x60, disp & 0xFF, 0x00});
      return *b + bb({0x83, 0xA0}) + pack_i(disp) + bb({0x00});
    }
    if (!base[0].is("arg")) return std::nullopt;
    ByteString bbase = bb({0x8B, 0x4D, argoff(base[1].number()) & 0xFF});
    OB vb = materialize(value);
    if (!vb) return std::nullopt;
    ByteString seq = (value[0].is("call") || value[0].is("icall")) ? *vb + bbase : bbase + *vb;
    long long disp = dispv.number();
    if (disp == 0) return seq + bb({0x89, 0x01});
    if (-0x80 <= disp && disp < 0x80) return seq + bb({0x89, 0x41, disp & 0xFF});
    return seq + bb({0x89, 0x81}) + pack_i(disp);
  }
  if (st[0].is("if")) {
    const Term &cond = st[1], &then = st[2];
    ByteString body;
    if (!then.is_sequence()) broken( "not iterable");
    for (auto &s : then.items()) {
      OB bs = emit_stmt(s);
      if (!bs) return std::nullopt;
      body += *bs;
    }
    if (body.empty() || body.size() > 127) return std::nullopt;
    ByteString condb;
    bool je;
    if (cond[0].is("nz") || cond[0].is("z")) {
      condb = bb({0x83, 0x7D, argoff(cond[1].number()) & 0xFF, 0x00});
      je = cond[0].is("nz");
    } else if (cond[0].is("callnz") || cond[0].is("callz")) {
      OB cs = emit_call_seq(cond[2], false);
      if (!cs) return std::nullopt;
      condb = *cs + bb({0x85, 0xC0});
      je = cond[0].is("callnz");
    } else if (cond[0].is("icallnz") || cond[0].is("icallz")) {
      OB cs = emit_call_seq(cond[1], true);
      if (!cs) return std::nullopt;
      condb = *cs + bb({0x85, 0xC0});
      je = cond[0].is("icallnz");
    } else {
      return std::nullopt;
    }
    return condb + bb({je ? 0x74 : 0x75, (long long)body.size()}) + body;
  }
  return std::nullopt;
}

OB emit_seq(const Term &node) {
  long long nargs = node.at("nargs").number();
  ByteString seq = PRO;
  const Term &stmts = node.at("stmts");
  if (!stmts.is_sequence()) broken( "not iterable");
  for (auto &st : stmts.items()) {
    OB cs = emit_stmt(st);
    if (!cs) return std::nullopt;
    seq += *cs;
  }
  const Term &ret = node.get("ret");
  if (!ret.none()) {
    OB cs = materialize(ret);
    if (!cs) cs = ret_body(ret);
    if (!cs) return std::nullopt;
    seq += *cs;
  }
  ByteString e;
  if (node.get("leave").truthy()) e = bb({0xC9}) + (nargs == 0 ? bb({0xC3}) : bb({0xC2}) + pack_H(4 * nargs));
  else e = epi(nargs);
  return seq + e;
}

OB emit_fastcall(const Term &node) {
  long long nargs = node.at("nargs").number();
  const Term &E = node.at("expr");
  if (nargs > 2) return std::nullopt;
  const Term &kind = E[0];
  ByteString ret = bb({0xC3});
  if (kind.is("arg")) {
    if (E[1].is_number(1)) return bb({0x8B, 0xC1}) + ret;
    if (E[1].is_number(2)) return bb({0x8B, 0xC2}) + ret;
    return std::nullopt;
  }
  if (kind.is("deref")) {
    const Term &a = E[1];
    if (!a[0].is("arg") || !(a[1].is_number(1) || a[1].is_number(2))) return std::nullopt;
    return bb({0x8B, a[1].is_number(1) ? 0x01 : 0x02}) + ret;
  }
  if (kind.is("index")) {
    const Term &p = E[1], &idx = E[2];
    if (!p[0].is("arg") || !(p[1].is_number(1) || p[1].is_number(2)) || !idx[0].is("const")) return std::nullopt;
    long long d = idx[1].number() * 4;
    ByteString mm;
    if (d == 0) mm = bb({p[1].is_number(1) ? 0x01 : 0x02});
    else if (0 < d && d < 0x80) mm = bb({p[1].is_number(1) ? 0x41 : 0x42, d});
    else return std::nullopt;
    return bb({0x8B}) + mm + ret;
  }
  if (kind.is("binop")) {
    const Term &op = E[1], &Lh = E[2], &R = E[3];
    if (op.is("+") && Lh == A(1) && R == A(2)) return bb({0x8D, 0x04, 0x0A}) + ret;
    if (in_memop(op) && Lh[0].is("deref") && Lh[1] == A(1) && R == A(2))
      return bb({0x8B, 0x01, memop_code(op), 0xC2}) + ret;
    return std::nullopt;
  }
  return std::nullopt;
}

OB emit_fpo(const Term &node) {
  long long nargs = node.at("nargs").number();
  const Term &E = node.at("expr");
  const Term &kind = E[0];
  ByteString ret = ret_n(nargs);
  auto fld = [](int reg, long long k) { return bb({0x8B}) + esp_rm(reg, 4 * k); };
  auto fmem = [](const Term &op, int reg, long long k) {
    if (op.is("*")) return bb({0x0F, 0xAF}) + esp_rm(reg, 4 * k);
    return bb({memop_code(op)}) + esp_rm(reg, 4 * k);
  };
  if (kind.is("arg")) return fld(EAX, E[1].number()) + ret;
  if (kind.is("unary")) {
    const Term &uop = E[1], &a = E[2];
    if (!a[0].is("arg")) return std::nullopt;
    if (uop.is("neg")) return fld(EAX, a[1].number()) + bb({0xF7, 0xD8}) + ret;
    if (uop.is("not")) return fld(EAX, a[1].number()) + bb({0xF7, 0xD0}) + ret;
    if (uop.is("lognot")) return bb({0x33, 0xC0}) + bb({0x39}) + esp_rm(EAX, 4 * a[1].number()) + bb({0x0F, 0x94, 0xC0}) + ret;
    return std::nullopt;
  }
  if (kind.is("deref")) {
    if (!E[1][0].is("arg")) return std::nullopt;
    return fld(EAX, E[1][1].number()) + bb({0x8B, 0x00}) + ret;
  }
  if (kind.is("index")) {
    const Term &p = E[1], &idx = E[2];
    if (!p[0].is("arg")) return std::nullopt;
    if (idx[0].is("arg")) return fld(EAX, idx[1].number()) + fld(ECX, p[1].number()) + bb({0x8B, 0x04, 0x81}) + ret;
    if (idx[0].is("const")) {
      long long d = idx[1].number() * 4;
      ByteString mem;
      if (d == 0) mem = bb({0x8B, 0x00});
      else if (0 < d && d < 0x80) mem = bb({0x8B, 0x40, d});
      else return std::nullopt;
      return fld(EAX, p[1].number()) + mem + ret;
    }
    return std::nullopt;
  }
  if (kind.is("binop")) {
    const Term &op = E[1], &Lh = E[2], &R = E[3];
    if (!in_ops(op)) return std::nullopt;
    Term cop;
    std::vector<long long> lv;
    if (flatten_chain(E, cop, lv) && lv.size() >= 3) {
      if (cop.is("*") || cop.is("&") || cop.is("|") || cop.is("^")) {
        ByteString seq = fld(EAX, lv.back());
        for (size_t j = lv.size() - 1; j-- > 0;) seq += fmem(cop, EAX, lv[j]);
        return seq + ret;
      }
      if (cop.is("-")) {
        ByteString seq = fld(EAX, lv[0]);
        std::vector<long long> sub(lv.begin() + 1, lv.end()), order(sub.begin() + 1, sub.end());
        order.push_back(sub[0]);
        for (long long k : order) seq += fmem(Term::string("-"), EAX, k);
        return seq + ret;
      }
      return std::nullopt;
    }
    if (Lh[0].is("arg") && R[0].is("const")) {
      OB ob = op_const_bytes(op, R[1]);
      if (!ob) return std::nullopt;
      return fld(EAX, Lh[1].number()) + *ob + ret;
    }
    if (Lh[0].is("arg") && R[0].is("arg")) {
      long long li = Lh[1].number(), ri = R[1].number();
      if (op.is("*") && li == ri) return fld(EAX, li) + bb({0x0F, 0xAF, 0xC0}) + ret;
      if (op.is("+")) return fld(EAX, ri) + fld(ECX, li) + bb({0x03, 0xC1}) + ret;
      if (op.is("-")) return fld(EAX, li) + fmem(op, EAX, ri) + ret;
      return fld(EAX, ri) + fmem(op, EAX, li) + ret;
    }
    return std::nullopt;
  }
  return std::nullopt;
}

bool all_args(const Term &cargs) {
  if (!cargs.is_sequence()) broken( "not iterable");
  for (auto &a : cargs.items())
    if (!a[0].is("arg")) return false;
  return true;
}

// argprod: (a, b) of an arg*arg product
bool argprod(const Term &X, long long &a, long long &b) {
  if (X[0].is("binop") && X[1].is("*") && X[2][0].is("arg") && X[3][0].is("arg")) {
    a = X[2][1].number();
    b = X[3][1].number();
    return true;
  }
  return false;
}

OB emit(const Term &node) {
  if (node.kind() == Term::Kind::Dict && !node.get("stmts").none()) return emit_seq(node);
  Term nargsv, E;
  try {
    nargsv = node.at("nargs");
    E = node.at("expr");
  } catch (const FormatError &) {
    return std::nullopt;
  }
  if (E.none()) return std::nullopt;
  if (node.get("cc").is("fastcall")) return emit_fastcall(node);
  if (node.get("fpo").truthy()) return emit_fpo(node);
  long long nargs = nargsv.number();
  if (E[0].is("binop") && in_ops(E[1]) && E[2][0].is("call") && E[3][0].is("call") && E[2][1].is("__stdcall") &&
      E[3][1].is("__stdcall") && all_args(E[2][2]) && all_args(E[3][2])) {
    const Term &op = E[1], &cargsL = E[2][2], &cargsR = E[3][2];
    ByteString seq = PRO + bb({0x56});
    OB cr = emit_call_seq(cargsR, false);
    if (!cr) return std::nullopt;
    seq += *cr;
    for (size_t j = cargsL.items().size(); j-- > 0;) seq += push_arg(cargsL.items()[j][1].number());
    seq += bb({0x8B, 0xF0, 0xE8, 0, 0, 0, 0});
    seq += op.is("*") ? bb({0x0F, 0xAF, 0xC6}) : bb({memop_code(op), 0xC6});
    return seq + bb({0x5E}) + epi(nargs);
  }
  const Term &kind = E[0];
  ByteString rn = ret_n(nargs);
  if (kind.is("const")) return const_body(E[1].number()) + rn;
  if (kind.is("empty")) return nargs == 0 ? bb({0xC3}) : PRO + epi(nargs);
  if (kind.is("unary")) {
    const Term &uop = E[1], &a = E[2];
    if (!a[0].is("arg")) return std::nullopt;
    if (uop.is("neg")) return PRO + ld(EAX, a[1].number()) + bb({0xF7, 0xD8}) + epi(nargs);
    if (uop.is("not")) return PRO + ld(EAX, a[1].number()) + bb({0xF7, 0xD0}) + epi(nargs);
    if (uop.is("lognot"))
      return PRO + bb({0x33, 0xC0}) + bb({0x39}) + ebp_rm(EAX, argoff(a[1].number())) + bb({0x0F, 0x94, 0xC0}) + epi(nargs);
    return std::nullopt;
  }
  if (kind.is("gread")) {
    if (nargs != 0) return std::nullopt;
    return bb({0xA1, 0, 0, 0, 0}) + rn;
  }
  if (kind.is("gwrite")) {
    const Term &val = E[2];
    if (val[0].is("const")) return bb({0xC7, 0x05, 0, 0, 0, 0}) + pack_i(val[1].number()) + rn;
    if (val[0].is("arg")) return PRO + ld(EAX, val[1].number()) + bb({0xA3, 0, 0, 0, 0}) + epi(nargs);
    return std::nullopt;
  }
  if (kind.is("gincdec")) return bb({0xFF, E[2].number() > 0 ? 0x05 : 0x0D, 0, 0, 0, 0}) + rn;
  if (kind.is("call") || kind.is("icall")) {
    const Term &conv = E[1], &cargs = E[2];
    if (!all_args(cargs)) return std::nullopt;
    if (kind.is("call") && cargs.items().empty()) return bb({0xE9, 0, 0, 0, 0});
    ByteString seq = PRO;
    for (size_t j = cargs.items().size(); j-- > 0;) seq += push_arg(cargs.items()[j][1].number());
    seq += kind.is("call") ? bb({0xE8, 0, 0, 0, 0}) : bb({0xFF, 0x15, 0, 0, 0, 0});
    if (conv.is("__cdecl")) seq += bb({0x59});
    return seq + epi(nargs);
  }
  if (kind.is("callbin")) {
    const Term &op = E[1], &conv = E[2], &cargs = E[3], &other = E[4];
    if (!conv.is("__stdcall") || !in_ops(op) || !all_args(cargs)) return std::nullopt;
    ByteString seq = PRO;
    for (size_t j = cargs.items().size(); j-- > 0;) seq += push_arg(cargs.items()[j][1].number());
    seq += bb({0xE8, 0, 0, 0, 0});
    if (other[0].is("arg")) {
      seq += memop(op, EAX, other[1].number());
    } else if (other[0].is("const")) {
      OB ob = op_const_bytes(op, other[1]);
      if (!ob) return std::nullopt;
      seq += *ob;
    } else {
      return std::nullopt;
    }
    return seq + epi(nargs);
  }
  if (kind.is("arg")) return PRO + ld(EAX, E[1].number()) + epi(nargs);
  if (kind.is("deref")) {
    if (!E[1][0].is("arg")) return std::nullopt;
    return PRO + ld(EAX, E[1][1].number()) + bb({0x8B, 0x00}) + epi(nargs);
  }
  if (kind.is("index")) {
    const Term &p = E[1], &idx = E[2];
    if (!p[0].is("arg")) return std::nullopt;
    if (idx[0].is("arg")) return PRO + ld(EAX, idx[1].number()) + ld(ECX, p[1].number()) + bb({0x8B, 0x04, 0x81}) + epi(nargs);
    if (idx[0].is("const")) {
      long long d = idx[1].number() * 4;
      ByteString mem;
      if (d == 0) mem = bb({0x8B, 0x00});
      else if (0 < d && d < 0x80) mem = bb({0x8B, 0x40, d});
      else return std::nullopt;
      return PRO + ld(EAX, p[1].number()) + mem + epi(nargs);
    }
    return std::nullopt;
  }
  if (kind.is("store")) {
    const Term &p = E[1], &val = E[2];
    if (!p[0].is("arg")) return std::nullopt;
    if (val[0].is("arg")) return PRO + ld(ECX, p[1].number()) + ld(EAX, val[1].number()) + bb({0x89, 0x01}) + epi(nargs);
    if (val[0].is("const") && val[1].is_number(0)) return PRO + ld(EAX, p[1].number()) + bb({0x83, 0x20, 0x00}) + epi(nargs);
    if (val[0].is("const")) return PRO + ld(EAX, p[1].number()) + bb({0xC7, 0x00}) + pack_i(val[1].number()) + epi(nargs);
    return std::nullopt;
  }
  if (kind.is("cmp")) {
    const Term &op = E[1], &Lh = E[2], &R = E[3];
    if (!Lh[0].is("arg") || !R[0].is("arg")) return std::nullopt;
    static const std::pair<const char *, int> SET[] = {{"<", 0x9C}, {"<=", 0x9E}, {">", 0x9F}, {">=", 0x9D}, {"==", 0x94}, {"!=", 0x95}};
    int sc = -1;
    if (op.kind() == Term::Kind::Str)
      for (auto &x : SET)
        if (op.text() == x.first) sc = x.second;
    if (sc < 0) return std::nullopt;
    return PRO + ld(ECX, Lh[1].number()) + bb({0x33, 0xC0}) + bb({0x3B}) + ebp_rm(ECX, argoff(R[1].number())) + bb({0x0F, sc, 0xC0}) +
           epi(nargs);
  }
  if (kind.is("binop")) {
    const Term &op = E[1], &Lh = E[2], &R = E[3];
    if (!in_ops(op)) return std::nullopt;
    Term cop;
    std::vector<long long> lv;
    if (flatten_chain(E, cop, lv) && lv.size() >= 3) {
      if (cop.is("*") || cop.is("&") || cop.is("|") || cop.is("^")) {
        ByteString seq = ld(EAX, lv.back());
        for (size_t j = lv.size() - 1; j-- > 0;) seq += memop(cop, EAX, lv[j]);
        return PRO + seq + epi(nargs);
      }
      if (cop.is("-")) {
        ByteString seq = ld(EAX, lv[0]);
        std::vector<long long> sub(lv.begin() + 1, lv.end()), order(sub.begin() + 1, sub.end());
        order.push_back(sub[0]);
        for (long long k : order) seq += memop(Term::string("-"), EAX, k);
        return PRO + seq + epi(nargs);
      }
      if (cop.is("+")) {
        ByteString seq = ld(EAX, lv.back()) + ld(ECX, lv[lv.size() - 2]) + bb({0x03, 0xC1});
        for (size_t j = lv.size() - 2; j-- > 0;) seq += memop(Term::string("+"), EAX, lv[j]);
        return PRO + seq + epi(nargs);
      }
      return std::nullopt;
    }
    long long a, b, c, d;
    if ((op.is("+") || op.is("-")) && argprod(Lh, a, b) && argprod(R, c, d)) {
      ByteString seq = ld(EAX, b) + ld(ECX, d) + memop(Term::string("*"), EAX, a) + memop(Term::string("*"), ECX, c) +
                  (op.is("+") ? bb({0x03, 0xC1}) : bb({0x2B, 0xC1}));
      return PRO + seq + epi(nargs);
    }
    if (op.is("+")) {
      const Term *XY[2][2] = {{&Lh, &R}, {&R, &Lh}};
      for (auto &xy : XY) {
        long long p0, p1;
        if (argprod(*xy[0], p0, p1) && (*xy[1])[0].is("arg"))
          return PRO + ld(EAX, p1) + memop(Term::string("*"), EAX, p0) + memop(Term::string("+"), EAX, (*xy[1])[1].number()) + epi(nargs);
      }
    }
    if (op.is("*") && Lh[0].is("binop") && Lh[1].is("+") && Lh[2][0].is("arg") && Lh[3][0].is("arg") && R[0].is("arg"))
      return PRO + ld(EAX, Lh[3][1].number()) + ld(ECX, Lh[2][1].number()) + bb({0x03, 0xC1}) + memop(Term::string("*"), EAX, R[1].number()) + epi(nargs);
    if ((op.is("+") || op.is("-")) && Lh[0].is("binop") && (Lh[1].is("+") || Lh[1].is("-")) && Lh[2][0].is("arg") &&
        Lh[3][0].is("arg") && R[0].is("arg")) {
      std::vector<std::pair<Term, long long>> tail = {{Lh[1], Lh[3][1].number()}, {op, R[1].number()}};
      std::vector<long long> plus, minus;
      for (auto &t : tail) {
        if (t.first.is("+")) plus.push_back(t.second);
        if (t.first.is("-")) minus.push_back(t.second);
      }
      if (plus.size() == 1 && minus.size() == 1) {
        ByteString seq = ld(EAX, plus[0]) + memop(Term::string("-"), EAX, minus[0]) + memop(Term::string("+"), EAX, Lh[2][1].number());
        return PRO + seq + epi(nargs);
      }
    }
    if (Lh[0].is("arg") && R[0].is("const")) {
      OB ob = op_const_bytes(op, R[1]);
      if (!ob) return std::nullopt;
      return PRO + ld(EAX, Lh[1].number()) + *ob + epi(nargs);
    }
    if (Lh[0].is("arg") && R[0].is("arg")) {
      long long li = Lh[1].number(), ri = R[1].number();
      if (op.is("*") && li == ri) return PRO + ld(EAX, li) + bb({0x0F, 0xAF, 0xC0}) + epi(nargs);
      if (op.is("+")) return PRO + ld(EAX, ri) + ld(ECX, li) + bb({0x03, 0xC1}) + epi(nargs);
      if (op.is("-")) return PRO + ld(EAX, li) + memop(op, EAX, ri) + epi(nargs);
      return PRO + ld(EAX, ri) + memop(op, EAX, li) + epi(nargs);
    }
    return std::nullopt;
  }
  return std::nullopt;
}

// ---- to_c -------------------------------------------------------------------
std::string c_expr(const Term &E) {
  if (E[0].is("const")) return show(E[1]);
  if (E[0].is("arg")) return "arg" + std::to_string(E[1].number());
  if (E[0].is("deref")) return "(*" + c_expr(E[1]) + ")";
  if (E[0].is("index")) return c_expr(E[1]) + "[" + c_expr(E[2]) + "]";
  std::string l = c_expr(E[2]);
  return "(" + l + " " + show(E[1]) + " " + c_expr(E[3]) + ")";
}

void ptr_args(const Term &E, std::set<long long> &s) {
  if (E[0].is("deref")) {
    if (E[1][0].is("arg")) s.insert(E[1][1].number());
    ptr_args(E[1], s);
  } else if (E[0].is("index") || E[0].is("store")) {
    if (E[1][0].is("arg")) s.insert(E[1][1].number());
  } else if (E[0].is("binop") || E[0].is("cmp")) {
    ptr_args(E[2], s);
    ptr_args(E[3], s);
  }
}

std::string ints_sig(const Term &cargs) {   // ", ".join("int" for _ in cargs) or "void"
  std::string o;
  for (size_t j = 0; j < cargs.size(); j++) o += j ? ", int" : "int";
  return o.empty() ? "void" : o;
}

std::string argnames(const Term &cargs) {   // ", ".join("arg%d" % a[1] for a in cargs)
  std::string o;
  for (size_t j = 0; j < cargs.size(); j++) o += (j ? ", arg" : "arg") + std::to_string(cargs[(long long)j][1].number());
  return o;
}

struct SeqC {
  std::vector<std::string> externs;
  int ctr = 0;
  std::string callC(const Term &cargs, bool indirect) {
    std::string gsig = ints_sig(cargs), nm, c = std::to_string(ctr);
    if (indirect) {
      externs.push_back("extern int (__stdcall *p" + c + ")(" + gsig + ");");
      nm = "(*p" + c + ")";
    } else {
      externs.push_back("extern int __stdcall g" + c + "(" + gsig + ");");
      nm = "g" + c;
    }
    std::string args;
    for (size_t j = 0; j < cargs.items().size(); j++) {
      const Term &a = cargs.items()[j];
      if (j) args += ", ";
      args += a[0].is("arg") ? "arg" + std::to_string(a[1].number()) : show(a[1]);
    }
    ctr++;
    return nm + "(" + args + ")";
  }
  std::string lvalue(const Term &base, long long disp) {
    std::string b = valC(base);
    return disp == 0 ? "(*" + b + ")" : "(*(int*)((char*)" + b + " + " + std::to_string(disp) + "))";
  }
  std::string stmtC(const Term &st) {
    if (st[0].is("ivoidcall")) return callC(st[1], true) + ";";
    if (st[0].is("voidcall")) return callC(st[2], false) + ";";
    if (st[0].is("if")) {
      const Term &cond = st[1], &then = st[2];
      std::string c;
      if (cond[0].is("nz")) c = "arg" + std::to_string(cond[1].number());
      else if (cond[0].is("z")) c = "!arg" + std::to_string(cond[1].number());
      else if (cond[0].is("callnz")) c = callC(cond[2], false);
      else if (cond[0].is("callz")) c = "!" + callC(cond[2], false);
      else if (cond[0].is("icallnz")) c = callC(cond[1], true);
      else if (cond[0].is("icallz")) c = "!" + callC(cond[1], true);
      else c = "0";
      std::string body;
      for (size_t j = 0; j < then.size(); j++) body += (j ? " " : "") + stmtC(then[(long long)j]);
      return "if (" + c + ") { " + body + " }";
    }
    if (st[0].is("store")) {
      if (st.size() != 4) fail( "unpack");
      std::string lv = lvalue(st[1], st[2].number());
      return lv + " = " + valC(st[3]) + ";";
    }
    return "";
  }
  std::string valC(const Term &Vv) {
    const Term &k = Vv[0];
    if (k.is("arg")) return "arg" + std::to_string(Vv[1].number());
    if (k.is("const")) return show(Vv[1]);
    if (k.is("global")) {
      if (std::find(externs.begin(), externs.end(), "extern int G;") == externs.end()) externs.insert(externs.begin(), "extern int G;");
      return "G";
    }
    if (k.is("call")) return callC(Vv[2], false);
    if (k.is("icall") || k.is("icall_ret")) return callC(Vv[1], true);
    if (k.is("deref")) return "(*" + valC(Vv[1]) + ")";
    if (k.is("index")) {
      std::string b = valC(Vv[1]);
      return b + "[" + std::to_string(Vv[2][1].number()) + "]";
    }
    if (k.is("load")) return lvalue(Vv[1], Vv[2].number());
    if (k.is("callbinret")) {
      const Term &op = Vv[1], &call = Vv[2], &other = Vv[3];
      std::string oc = other[0].is("arg") ? "arg" + std::to_string(other[1].number()) : show(other[1]);
      std::string cc = valC(call);
      return "(" + cc + " " + op.text() + " " + oc + ")";
    }
    if (k.is("binop")) {
      std::string l = valC(Vv[2]);
      return "(" + l + " " + show(Vv[1]) + " " + valC(Vv[3]) + ")";
    }
    return "0";
  }
};

std::string params_of(long long nargs, const std::set<long long> *ptr) {
  if (nargs == 0) return "void";
  std::string o;
  for (long long i = 0; i < nargs; i++) {
    if (i) o += ", ";
    o += std::string("int") + (ptr && ptr->count(i + 1) ? "*" : "") + " arg" + std::to_string(i + 1);
  }
  return o;
}

std::string to_c_seq(const Term &node) {
  long long nargs = node.at("nargs").number();
  std::string params = params_of(nargs, nullptr);
  SeqC s;
  std::vector<std::string> body;
  const Term &stmts = node.at("stmts");
  for (size_t j = 0; j < stmts.size(); j++) body.push_back(s.stmtC(stmts[(long long)j]));
  const Term &ret = node.get("ret");
  std::string rtype = "void";
  if (!ret.none()) {
    rtype = "int";
    body.push_back("return " + s.valC(ret) + ";");
  }
  std::string ex, bo;
  for (size_t j = 0; j < s.externs.size(); j++) ex += (j ? " " : "") + s.externs[j];
  for (size_t j = 0; j < body.size(); j++) bo += (j ? " " : "") + body[j];
  return ex + " " + rtype + " __stdcall f(" + params + ") { " + bo + " }";
}

std::string to_c(const Term &node) {
  if (node.kind() == Term::Kind::Dict && !node.get("stmts").none()) return to_c_seq(node);
  long long nargs = node.at("nargs").number();
  const Term &E = node.at("expr");
  std::set<long long> ptr;
  ptr_args(E, ptr);
  std::string params = params_of(nargs, &ptr);
  if (node.get("cc").is("fastcall")) return "int __fastcall f(" + params + ") { return " + c_expr(E) + "; }";
  if (E[0].is("binop") && E[2][0].is("call") && E[3][0].is("call")) {
    const Term &Lh = E[2], &R = E[3];
    return "extern int __stdcall g0(" + ints_sig(Lh[2]) + "); extern int __stdcall g1(" + ints_sig(R[2]) +
           "); int __stdcall f(" + params + ") { return g0(" + argnames(Lh[2]) + ") " + show(E[1]) + " g1(" +
           argnames(R[2]) + "); }";
  }
  if (E[0].is("store")) return "void __stdcall f(" + params + ") { *arg" + std::to_string(E[1][1].number()) + " = " + c_expr(E[2]) + "; }";
  if (E[0].is("call"))
    return "extern int " + show(E[1]) + " g(" + ints_sig(E[2]) + "); int __stdcall f(" + params + ") { return g(" +
           argnames(E[2]) + "); }";
  if (E[0].is("gread")) return "extern int G; int __stdcall f(void) { return G; }";
  if (E[0].is("gwrite")) return "extern int G; void __stdcall f(" + params + ") { G = " + c_expr(E[2]) + "; }";
  if (E[0].is("gincdec")) return std::string("extern int G; void __stdcall f(void) { G ") + (E[2].number() > 0 ? "++" : "--") + "; }";
  if (E[0].is("icall"))
    return "extern int (" + show(E[1]) + " *P)(" + ints_sig(E[2]) + "); int __stdcall f(" + params + ") { return P(" +
           argnames(E[2]) + "); }";
  if (E[0].is("callbin")) {
    const Term &other = E[4];
    std::string oc = other[0].is("arg") ? "arg" + std::to_string(other[1].number()) : show(other[1]);
    return "extern int " + show(E[2]) + " g(" + ints_sig(E[3]) + "); int __stdcall f(" + params + ") { return g(" +
           argnames(E[3]) + ") " + show(E[1]) + " " + oc + "; }";
  }
  if (E[0].is("empty")) return "void __stdcall f(" + params + ") {}";
  if (E[0].is("unary")) {
    std::string u;
    if (E[1].is("neg")) u = "-";
    else if (E[1].is("not")) u = "~";
    else if (E[1].is("lognot")) u = "!";
    else fail( "'" + show(E[1]) + "'");
    return "int __stdcall f(" + params + ") { return " + u + "arg" + std::to_string(E[2][1].number()) + "; }";
  }
  return "int __stdcall f(" + params + ") { return " + c_expr(E) + "; }";
}

// =========================================================================
// msvc2 / msvc2_registers: bytes -> candidates
// =========================================================================
typedef std::pair<std::string, std::string> MO;   // (mnemonic, op_str)

std::vector<MO> dis(const ByteString &code) {
  std::vector<MO> out;
  for (auto &i : disasm32((const uint8_t *)code.data(), code.size(), 0)) out.push_back({i.mnemonic, i.op_str});
  return out;
}

long long pint0(const std::string &s) {   // int(s, 0): Python strips whitespace first
  size_t a = s.find_first_not_of(" \t\n\r\f\v"), b = s.find_last_not_of(" \t\n\r\f\v");
  if (a == std::string::npos) return parse_number(s);
  return parse_number(s.substr(a, b - a + 1));
}
long long sconst(const std::string &s) {   // _sconst (ValueError)
  long long v = pint0(s);
  return v > 0x7fffffff ? v - 0x100000000LL : v;
}
OI sconst_or_none(const std::string &s) {
  try {
    return sconst(s);
  } catch (const FormatError &) {
    return std::nullopt;
  }
}
std::optional<Term> cst(const std::string &s) {   // _cst
  OI v = sconst_or_none(s);
  if (!v) return std::nullopt;
  return C(*v);
}

// `\[(e[sb]p) \+ (0x[0-9a-f]+|\d+)\]` searched; bases: which registers the class allows
bool search_stack(const std::string &o, bool esp_too, std::string &base, long long &off) {
  for (size_t p = o.find('['); p != std::string::npos; p = o.find('[', p + 1)) {
    Cursor c(o, p + 1);
    if (c.lit("ebp + ")) base = "ebp";
    else if (esp_too && c.lit("esp + ")) base = "esp";
    else continue;
    std::string g;
    if (!hex_or_dec_then(c, g, "]", false)) continue;
    off = parse_number(g);
    return true;
  }
  return false;
}
OI argk(const std::string &o) {
  std::string base;
  long long off;
  if (!search_stack(o, true, base, off)) return std::nullopt;
  if (base == "esp") return (off >= 4 && off % 4 == 0) ? OI(off / 4) : std::nullopt;
  return (off >= 8 && (off - 4) % 4 == 0) ? OI((off - 4) / 4) : std::nullopt;
}
OI argk_ebp(const std::string &o) {
  std::string base;
  long long off;
  if (!search_stack(o, false, base, off)) return std::nullopt;
  return (off >= 8 && (off - 4) % 4 == 0) ? OI((off - 4) / 4) : std::nullopt;
}

// re.search(r"\[(0x[0-9a-f]+|\d+)\]", o)
bool search_abs(const std::string &o) {
  for (size_t p = o.find('['); p != std::string::npos; p = o.find('[', p + 1)) {
    Cursor c(o, p + 1);
    std::string g;
    if (hex_or_dec_then(c, g, "]", false)) return true;
  }
  return false;
}

// `(?:dword ptr )?\[(0x[0-9a-f]+|\d+)\]` at `at`; the position after it, or npos
size_t abs_at(const std::string &s, size_t at) {
  Cursor c(s, at);
  c.lit("dword ptr ");
  std::string g;
  if (c.lit("[") && hex_or_dec_then(c, g, "]", false)) return c.i;
  return std::string::npos;
}

const char *R6[] = {"eax", "ecx", "edx", "esi", "edi", "ebx"};
bool is_reg6(const std::string &s) {
  for (auto r : R6)
    if (s == r) return true;
  return false;
}

// `(?:dword ptr )?\[(<regs>)(?: \+ (0x[0-9a-f]+|\d+))?\]<tail>$` at `at` (disp only when allowed)
bool regmem(const std::string &s, size_t at, std::initializer_list<const char *> regs, bool with_disp, const char *tail,
            std::string &reg, long long &disp) {
  Cursor c(s, at);
  c.lit("dword ptr ");
  if (!c.lit("[")) return false;
  reg.clear();
  for (auto r : regs)
    if (s.compare(c.i, 3, r) == 0) reg = r;
  if (reg.empty()) return false;
  c.i += 3;
  size_t mid = c.i;
  std::string g;
  std::string close = std::string("]") + tail;
  if (with_disp && c.lit(" + ") && hex_or_dec_then(c, g, close.c_str(), true)) {
    disp = parse_number(g);
    return true;
  }
  c.i = mid;
  if (c.lit(close.c_str()) && c.end()) {
    disp = 0;
    return true;
  }
  return false;
}
bool regmem6(const std::string &s, std::string &reg, long long &disp) {
  return regmem(s, 0, {"eax", "ecx", "edx", "esi", "edi", "ebx"}, true, "", reg, disp);
}

// re.search(r"\b(ecx|edx)\b", s)
bool word_ecx_edx(const std::string &s) {
  for (size_t p = 0; p + 3 <= s.size(); p++) {
    if (s.compare(p, 3, "ecx") != 0 && s.compare(p, 3, "edx") != 0) continue;
    if ((p == 0 || !is_word_char(s[p - 1])) && (p + 3 == s.size() || !is_word_char(s[p + 3]))) return true;
  }
  return false;
}

// re.match(r"0x([0-9a-f]+)$", s)
bool hex_target(const std::string &s, long long &v) {
  Cursor c(s);
  std::string h;
  if (c.lit("0x") && c.hexrun(h) && c.end()) {
    v = parse_hex(h);
    return true;
  }
  return false;
}

std::string at1(const std::string &s, const std::string &sep, long long j) {   // s.split(sep)[j]
  auto v = split(s, sep);
  long long m = (long long)v.size();
  if (j < 0) j += m;
  if (j < 0 || j >= m) fail( "list index out of range");
  return v[(size_t)j];
}

const std::pair<const char *, const char *> ALU_MN[] = {{"add", "+"}, {"sub", "-"}, {"and", "&"}, {"or", "|"}, {"xor", "^"}};
const char *alu(const std::string &m) {
  for (auto &x : ALU_MN)
    if (m == x.first) return x.second;
  return nullptr;
}

Term node(long long nargs, const Term &expr) {
  Term d = Term::dict();
  d.set("nargs", Term::integer(nargs));
  d.set("expr", expr);
  return d;
}

long long maxarg(const Term &e) {
  if (e[0].is("arg")) return e[1].number();
  if (e[0].is("deref") || e[0].is("index")) return maxarg(e[1]);
  if (e[0].is("binop")) return std::max(maxarg(e[2]), maxarg(e[3]));
  return 0;
}

void fastcall_candidates(const std::vector<MO> &ops, std::vector<Term> &out) {
  if (ops.empty()) return;
  auto fc = [](const Term &expr) {
    Term d = Term::dict();
    d.set("nargs", Term::integer(maxarg(expr)));
    d.set("cc", Term::string("fastcall"));
    d.set("expr", expr);
    return d;
  };
  const std::string &m0 = ops[0].first, &o0 = ops[0].second;
  if (ops.size() == 1 && m0 == "mov" && (o0 == "eax, ecx" || o0 == "eax, edx")) {
    out.push_back(fc(A(at1(o0, ", ", 1) == "ecx" ? 1 : 2)));
    return;
  }
  if (ops.size() == 1 && m0 == "lea" && o0 == "eax, [edx + ecx]") {
    out.push_back(fc(Term::tuple({Term::string("binop"), Term::string("+"), A(1), A(2)})));
    return;
  }
  std::string reg;
  long long d = 0;
  if (m0 == "mov" && starts(o0, "eax, ") && regmem(o0, 5, {"ecx", "edx"}, true, "", reg, d)) {
    long long k = reg == "ecx" ? 1 : 2;
    if (ops.size() == 1) {
      if (d == 0) out.push_back(fc(Term::tuple({Term::string("deref"), A(k)})));
      if (d % 4 == 0) out.push_back(fc(Term::tuple({Term::string("index"), A(k), C(floor_quotient(d, 4))})));
      return;
    }
    if (ops.size() == 2 && d == 0 && k == 1) {
      const char *op = alu(ops[1].first);
      if (op && ops[1].second == "eax, edx") out.push_back(fc(Term::tuple({Term::string("binop"), Term::string(op), Term::tuple({Term::string("deref"), A(1)}), A(2)})));
      return;
    }
  }
}

std::optional<Term> parse_ret_body(const std::vector<MO> &ops) {
  if (ops.empty()) return std::nullopt;
  const std::string &m0 = ops[0].first, &o0 = ops[0].second;
  if (ops.size() == 1 && m0 == "xor" && o0 == "eax, eax") return C(0);
  if (ops.size() == 2 && m0 == "push" && ops[1] == MO("pop", "eax")) return cst(o0);
  if (ops.size() == 1 && m0 == "mov" && starts(o0, "eax, ") && !has(o0, "[")) return cst(at1(o0, ", ", 1));
  if (m0 != "mov" || !starts(o0, "eax,")) return std::nullopt;
  OI k0 = argk(o0);
  if (!k0) return std::nullopt;
  if (ops.size() == 1) return A(*k0);
  if (ops.size() == 3 && ops[1].first == "mov" && starts(ops[1].second, "ecx,") && ops[2] == MO("add", "eax, ecx")) {
    OI li = argk(ops[1].second);
    if (li) return Term::tuple({Term::string("binop"), Term::string("+"), A(*li), A(*k0)});
  }
  if (ops.size() == 2) {
    const std::string &m1 = ops[1].first, &o1 = ops[1].second;
    if (m1 == "inc") return Term::tuple({Term::string("binop"), Term::string("+"), A(*k0), C(1)});
    if (m1 == "dec") return Term::tuple({Term::string("binop"), Term::string("-"), A(*k0), C(1)});
    if (m1 == "imul") {
      OI kk = argk(o1);
      if (kk) return Term::tuple({Term::string("binop"), Term::string("*"), A(*k0), A(*kk)});
    }
    if (const char *op = alu(m1)) {
      OI kk = argk(o1);
      if (kk) return Term::tuple({Term::string("binop"), Term::string(op), A(*k0), A(*kk)});
      std::optional<Term> c;
      if (has(o1, ", ")) c = cst(at1(o1, ", ", 1));
      if (c) return Term::tuple({Term::string("binop"), Term::string(op), A(*k0), *c});
    }
  }
  return std::nullopt;
}

// `(?:dword ptr )?\[ebp \+ (?:0x[0-9a-f]+|\d+)\], 0$`
bool cmp_arg_zero(const std::string &o) {
  Cursor c(o);
  c.lit("dword ptr ");
  std::string g;
  return c.lit("[ebp + ") && hex_or_dec_then(c, g, "], 0", true);
}

struct ParsedStmts {
  std::vector<Term> stmts;
  size_t next;
};

std::optional<ParsedStmts> parse_stmts(const std::vector<platforms::Insn> &ins, size_t i, size_t end, const std::map<long long, size_t> &addr2idx) {
  std::vector<Term> stmts, pending;
  while (i < end) {
    const platforms::Insn &x = ins[i];
    const std::string &m = x.mnemonic, &o = x.op_str;
    if (m == "push") {
      if (has(o, "[")) {
        OI k = argk(o);
        if (!k) return std::nullopt;
        pending.push_back(A(*k));
      } else {
        auto c = cst(o);
        if (!c) return std::nullopt;
        pending.push_back(*c);
      }
      i++;
    } else if (m == "call") {
      bool ind = has(o, "[");
      if (ind && !search_abs(o)) return std::nullopt;
      std::vector<Term> cargs(pending.rbegin(), pending.rend());
      stmts.push_back(ind ? Term::tuple({Term::string("ivoidcall"), Term::list(cargs)}) : Term::tuple({Term::string("voidcall"), Term::string("__stdcall"), Term::list(cargs)}));
      pending.clear();
      i++;
    } else if (m == "cmp" && cmp_arg_zero(o) && i + 1 < end && (ins[i + 1].mnemonic == "je" || ins[i + 1].mnemonic == "jne")) {
      if (!pending.empty()) return std::nullopt;
      OI ak = argk(o);
      const platforms::Insn &jcc = ins[i + 1];
      long long tgt;
      if (!ak || !hex_target(jcc.op_str, tgt)) return std::nullopt;
      auto it = addr2idx.find(tgt);
      if (it == addr2idx.end() || it->second <= i + 1) return std::nullopt;
      size_t body_end = it->second;
      auto sub = parse_stmts(ins, i + 2, body_end, addr2idx);
      if (!sub || sub->next != body_end) return std::nullopt;
      Term cond = Term::tuple({Term::string(jcc.mnemonic == "je" ? "nz" : "z"), Term::integer(*ak)});
      stmts.push_back(Term::tuple({Term::string("if"), cond, Term::list(sub->stmts)}));
      i = body_end;
    } else if (m == "test" && o == "eax, eax" && i + 1 < end && (ins[i + 1].mnemonic == "je" || ins[i + 1].mnemonic == "jne") &&
               pending.empty() && !stmts.empty() && (stmts.back()[0].is("voidcall") || stmts.back()[0].is("ivoidcall"))) {
      const platforms::Insn &jcc = ins[i + 1];
      long long tgt;
      if (!hex_target(jcc.op_str, tgt)) return std::nullopt;
      auto it = addr2idx.find(tgt);
      if (it == addr2idx.end() || it->second <= i + 1) return std::nullopt;
      size_t body_end = it->second;
      auto sub = parse_stmts(ins, i + 2, body_end, addr2idx);
      if (!sub || sub->next != body_end) return std::nullopt;
      Term call_stmt = stmts.back();
      stmts.pop_back();
      bool je = jcc.mnemonic == "je";
      Term cond = call_stmt[0].is("voidcall") ? Term::tuple({Term::string(je ? "callnz" : "callz"), call_stmt[1], call_stmt[2]})
                                           : Term::tuple({Term::string(je ? "icallnz" : "icallz"), call_stmt[1]});
      stmts.push_back(Term::tuple({Term::string("if"), cond, Term::list(sub->stmts)}));
      i = body_end;
    } else {
      break;
    }
  }
  if (!pending.empty()) return std::nullopt;
  return ParsedStmts{stmts, i};
}

Term seqnode(long long nargs, const std::vector<Term> &stmts, const Term &ret, bool leave) {
  Term d = Term::dict();
  d.set("nargs", Term::integer(nargs));
  d.set("stmts", Term::list(stmts));
  d.set("ret", ret);
  d.set("leave", Term::boolean(leave));
  return d;
}

void seq_candidates(const ByteString &code, long long nargs, bool leave, std::vector<Term> &out) {
  auto md = disasm32((const uint8_t *)code.data(), code.size(), 0);
  size_t lo = 0, hi = md.size();
  if (md.size() >= 2 && md[0].mnemonic == "push" && md[0].op_str == "ebp" && md[1].mnemonic == "mov" && md[1].op_str == "ebp, esp") lo = 2;
  while (hi > lo && (md[hi - 1].mnemonic == "ret" || md[hi - 1].mnemonic == "retn")) hi--;
  if (hi > lo && (md[hi - 1].mnemonic == "leave" || (md[hi - 1].mnemonic == "pop" && md[hi - 1].op_str == "ebp"))) hi--;
  std::vector<platforms::Insn> body(md.begin() + lo, md.begin() + hi);
  std::map<long long, size_t> addr2idx;
  for (size_t j = 0; j < body.size(); j++) addr2idx[body[j].address] = j;   // a later duplicate address wins, as in the dict
  if (hi < md.size()) addr2idx[md[hi].address] = body.size();
  auto res = parse_stmts(body, 0, body.size(), addr2idx);
  if (!res) return;
  const std::vector<Term> &stmts = res->stmts;
  if (stmts.empty()) return;
  std::vector<MO> trailing;
  for (size_t j = res->next; j < body.size(); j++) trailing.push_back({body[j].mnemonic, body[j].op_str});
  if (!trailing.empty()) {
    auto ret = parse_ret_body(trailing);
    if (ret) out.push_back(seqnode(nargs, stmts, *ret, leave));
  } else {
    out.push_back(seqnode(nargs, stmts, Term(), leave));
    const Term &last = stmts.back();
    std::vector<Term> head(stmts.begin(), stmts.end() - 1);
    if (last[0].is("voidcall")) out.push_back(seqnode(nargs, head, Term::tuple({Term::string("call"), Term::string("__stdcall"), last[2]}), leave));
    else if (last[0].is("ivoidcall")) out.push_back(seqnode(nargs, head, Term::tuple({Term::string("icall_ret"), last[1]}), leave));
  }
}

std::optional<std::vector<Term>> pushargs(const std::vector<MO> &pushes) {
  std::vector<Term> out;
  for (auto &p : pushes) {
    if (p.first != "push") return std::nullopt;
    if (has(p.second, "[")) {
      OI k = argk(p.second);
      if (!k) return std::nullopt;
      out.push_back(A(*k));
    } else {
      auto c = cst(p.second);
      if (!c) return std::nullopt;
      out.push_back(*c);
    }
  }
  std::reverse(out.begin(), out.end());
  return out;
}

void call2_candidates(const std::vector<MO> &ops, long long nargs, std::vector<Term> &out) {
  if (ops.size() < 6 || ops[0] != MO("push", "esi") || ops.back() != MO("pop", "esi")) return;
  std::vector<MO> inner(ops.begin() + 1, ops.end() - 1);
  auto mit = std::find(inner.begin(), inner.end(), MO("mov", "esi, eax"));
  if (mit == inner.end()) return;
  size_t mi = mit - inner.begin();
  if (mi + 3 != inner.size() || inner[mi + 1].first != "call") return;
  std::string key = inner[mi + 2].first + " " + inner[mi + 2].second;
  static const std::pair<const char *, const char *> R_OP[] = {{"add eax, esi", "+"}, {"sub eax, esi", "-"}, {"and eax, esi", "&"},
                                                               {"or eax, esi", "|"},  {"xor eax, esi", "^"}, {"imul eax, esi", "*"}};
  const char *opk = nullptr;
  for (auto &x : R_OP)
    if (key == x.first) opk = x.second;
  if (!opk) return;
  size_t rc = 0;
  while (rc < inner.size() && inner[rc].first != "call") rc++;
  if (rc == inner.size()) return;
  if (has(inner[rc].second, "[") || has(inner[mi + 1].second, "[")) return;
  auto Rargs = pushargs(std::vector<MO>(inner.begin(), inner.begin() + rc));
  std::vector<MO> lseg;
  for (size_t j = rc + 1; j < mi; j++) lseg.push_back(inner[j]);
  auto Largs = pushargs(lseg);
  if (!Rargs || !Largs) return;
  out.push_back(node(nargs, Term::tuple({Term::string("binop"), Term::string(opk), Term::tuple({Term::string("call"), Term::string("__stdcall"), Term::list(*Largs)}), Term::tuple({Term::string("call"), Term::string("__stdcall"), Term::list(*Rargs)})})));
}

// ---- msvc2_registers.interp -------------------------------------------------
std::optional<Term> interp(const ByteString &code, long long nargs) {
  auto ins = disasm32((const uint8_t *)code.data(), code.size(), 0);
  if (ins.empty()) return std::nullopt;
  if (!(ins.size() >= 2 && ins[0].mnemonic == "push" && ins[0].op_str == "ebp" && ins[1].mnemonic == "mov" && ins[1].op_str == "ebp, esp"))
    return std::nullopt;
  size_t i = 2;
  std::vector<std::string> saved;
  auto callee_saved = [](const std::string &r) { return r == "esi" || r == "edi" || r == "ebx"; };
  while (i < ins.size() && ins[i].mnemonic == "push" && callee_saved(ins[i].op_str)) saved.push_back(ins[i++].op_str);
  size_t hi = ins.size();
  if (hi > i && (ins[hi - 1].mnemonic == "ret" || ins[hi - 1].mnemonic == "retn")) hi--;
  else return std::nullopt;
  bool leave = false;
  if (hi > i && ins[hi - 1].mnemonic == "leave") {
    leave = true;
    hi--;
  } else if (hi > i && ins[hi - 1].mnemonic == "pop" && ins[hi - 1].op_str == "ebp") {
    hi--;
  } else {
    return std::nullopt;
  }
  for (size_t j = saved.size(); j-- > 0;) {
    if (hi > i && ins[hi - 1].mnemonic == "pop" && ins[hi - 1].op_str == saved[j]) hi--;
    else return std::nullopt;
  }
  std::map<std::string, Term> env;
  std::vector<Term> stmts, pending;
  auto envget = [&](const std::string &r) -> Term {
    auto it = env.find(r);
    return it == env.end() ? Term() : it->second;
  };
  auto flush_dead_eax = [&]() {
    Term v = envget("eax");
    if (!v.none() && (v[0].is("call") || v[0].is("icall")))
      stmts.push_back(v[0].is("call") ? Term::tuple({Term::string("voidcall"), Term::string("__stdcall"), v[2]}) : Term::tuple({Term::string("ivoidcall"), v[1]}));
  };
  for (size_t j = i; j < hi; j++) {
    const std::string &m = ins[j].mnemonic, &o = ins[j].op_str;
    if (m == "push") {
      if (has(o, "[")) {
        OI k = argk_ebp(o);
        if (!k) return std::nullopt;
        pending.push_back(A(*k));
      } else if (env.count(o)) {
        pending.push_back(env[o]);
      } else {
        OI c = sconst_or_none(o);
        if (!c) return std::nullopt;
        pending.push_back(C(*c));
      }
    } else if (m == "call") {
      if (has(o, "[") && !search_abs(o)) return std::nullopt;
      flush_dead_eax();
      std::vector<Term> cargs(pending.rbegin(), pending.rend());
      pending.clear();
      Term val = has(o, "[") ? Term::tuple({Term::string("icall"), Term::list(cargs)}) : Term::tuple({Term::string("call"), Term::string("__stdcall"), Term::list(cargs)});
      std::map<std::string, Term> kept;
      for (auto &kv : env)
        if (callee_saved(kv.first)) kept.insert(kv);
      env = kept;
      env["eax"] = val;
    } else if (m == "mov") {
      auto pr = partition(o, ", ");
      const std::string &dst = pr.first, &src = pr.second;
      std::string mreg;
      long long disp;
      if (regmem6(dst, mreg, disp)) {
        Term base = envget(mreg);
        if (base.none()) return std::nullopt;
        Term val;
        if (is_reg6(src)) {
          val = envget(src);
          if (val.none()) return std::nullopt;
        } else {
          OI c = sconst_or_none(src);
          if (!c) return std::nullopt;
          val = C(*c);
        }
        stmts.push_back(Term::tuple({Term::string("store"), base, Term::integer(disp), val}));
        continue;
      }
      if (!is_reg6(dst)) return std::nullopt;
      OI k = argk_ebp(o);
      bool consumes_eax = false;
      Term nv;
      if (k) {
        nv = A(*k);
      } else if (is_reg6(src)) {
        nv = envget(src);
        consumes_eax = src == "eax";
      } else {
        std::string lreg;
        long long ldisp;
        bool ml = regmem6(src, lreg, ldisp);
        size_t e = abs_at(src, 0);
        bool mg = e != std::string::npos && e == src.size();
        if (ml && !envget(lreg).none()) {
          nv = Term::tuple({Term::string("load"), env[lreg], Term::integer(ldisp)});
          consumes_eax = lreg == "eax";
        } else if (mg && dst == "eax") {
          nv = Term::tuple({Term::string("global")});
        } else {
          OI c = sconst_or_none(src);
          if (c) nv = C(*c);
        }
      }
      if (dst == "eax" && !consumes_eax) flush_dead_eax();
      env[dst] = nv;
    } else if (m == "xor" && (o == "eax, eax" || o == "ecx, ecx" || o == "edx, edx")) {
      if (o == "eax, eax") flush_dead_eax();
      env[at1(o, ",", 0)] = C(0);
    } else if (m == "and" && [&] {
                 std::string r;
                 long long d;
                 return regmem(o, 0, {"eax", "ecx", "edx", "esi", "edi", "ebx"}, false, ", 0", r, d);
               }()) {
      std::string r;
      long long d;
      regmem(o, 0, {"eax", "ecx", "edx", "esi", "edi", "ebx"}, false, ", 0", r, d);
      Term base = envget(r);
      if (base.none()) return std::nullopt;
      stmts.push_back(Term::tuple({Term::string("store"), base, Term::integer(0), C(0)}));
    } else if ((m == "inc" || m == "dec") && env.count(o)) {
      env[o] = Term::tuple({Term::string("binop"), Term::string(m == "inc" ? "+" : "-"), env[o], C(1)});
    } else if (alu(m) && starts(o, "eax")) {
      std::string rhs = partition(o, ", ").second;
      if (env.count(rhs)) {
        if (!env.count("eax")) fail( "'eax'");
        if (env["eax"].none()) return std::nullopt;
        env["eax"] = Term::tuple({Term::string("binop"), Term::string(alu(m)), env["eax"], env[rhs]});
      } else {
        OI k = argk_ebp(o);
        std::optional<Term> rv;
        if (k) rv = A(*k);
        else if (OI c = sconst_or_none(rhs)) rv = C(*c);
        if (!rv || envget("eax").none()) return std::nullopt;
        env["eax"] = Term::tuple({Term::string("binop"), Term::string(alu(m)), env["eax"], *rv});
      }
    } else {
      return std::nullopt;
    }
  }
  if (!pending.empty() || !saved.empty()) return std::nullopt;
  Term ret = envget("eax");
  Term base = Term::dict();
  base.set("nargs", Term::integer(nargs));
  base.set("stmts", Term::list(stmts));
  base.set("leave", Term::boolean(leave));
  Term c1 = base;
  c1.set("ret", ret);
  std::vector<Term> cands = {c1};
  if (!ret.none()) {
    Term c2 = base;
    c2.set("ret", Term());
    cands.push_back(c2);
  }
  return Term::list(cands);
}

// ---- msvc2._candidates ------------------------------------------------------
void candidates(const ByteString &code, std::vector<Term> &out) {
  auto ins = dis(code);
  if (ins.empty()) return;
  long long nargs = 0;
  if (ins.back().first == "ret" && !ins.back().second.empty()) nargs = floor_quotient(pint0(ins.back().second), 4);
  std::vector<MO> ops;
  for (auto &x : ins) {
    if (x == MO("push", "ebp") || x == MO("mov", "ebp, esp") || x == MO("pop", "ebp") || x == MO("leave", "")) continue;
    if (x.first == "ret") continue;
    ops.push_back(x);
  }
  if (auto rn = interp(code, nargs))
    for (auto &n : rn->items()) out.push_back(n);

  auto loadk = [&](const char *reg) -> OI {
    std::string p = std::string(reg) + ",";
    for (auto &x : ops)
      if (x.first == "mov" && starts(x.second, p.c_str()))
        if (OI k = argk(x.second)) return k;
    return std::nullopt;
  };

  if (!ops.empty() && (ops[0].first == "mov" || ops[0].first == "lea") && starts(ops[0].second, "eax,") && word_ecx_edx(ops[0].second) &&
      !has(ops[0].second, "esp") && !has(ops[0].second, "ebp")) {
    fastcall_candidates(ops, out);
    return;
  }
  if (ops.size() == 1 && ops[0] == MO("xor", "eax, eax")) {
    out.push_back(node(nargs, C(0)));
    return;
  }
  if (ops.size() == 2 && ops[0].first == "push" && ops[1] == MO("pop", "eax")) {
    if (OI c = sconst_or_none(ops[0].second)) out.push_back(node(nargs, C(*c)));
    return;
  }
  if (ops.size() == 1 && ops[0].first == "mov" && starts(ops[0].second, "eax, ") && !has(ops[0].second, "[") && !has(ops[0].second, "ptr")) {
    if (OI c = sconst_or_none(at1(ops[0].second, ", ", 1))) out.push_back(node(nargs, C(*c)));
    return;
  }
  if (ins.size() == 1 && ins[0].first == "jmp") {
    out.push_back(node(nargs, Term::tuple({Term::string("call"), Term::string("__stdcall"), Term::list()})));
    return;
  }
  size_t ci = 0;
  while (ci < ops.size() && ops[ci].first != "call") ci++;
  if (ci < ops.size()) {
    const std::string &callop = ops[ci].second;
    std::vector<OI> pushes;
    for (auto &x : ops)
      if (x.first == "push" && starts(x.second, "dword")) pushes.push_back(argk(x.second));
    bool allk = !pushes.empty();
    for (auto &p : pushes) allk = allk && p.has_value();
    if (allk) {
      bool is_cdecl = std::find(ops.begin(), ops.end(), MO("pop", "ecx")) != ops.end();
      Term conv = Term::string(is_cdecl ? "__cdecl" : "__stdcall");
      std::vector<Term> args;
      for (size_t j = pushes.size(); j-- > 0;) args.push_back(A(*pushes[j]));
      bool ind = has(callop, "[");
      out.push_back(node(nargs, Term::tuple({Term::string(ind ? "icall" : "call"), conv, Term::list(args)})));
      if (!ind && !is_cdecl && ops.size() - ci - 1 == 1) {
        const std::string &tm = ops[ci + 1].first, &to = ops[ci + 1].second;
        std::optional<Term> op, other;
        if (tm == "inc") {
          op = Term::string("+");
          other = C(1);
        } else if (tm == "dec") {
          op = Term::string("-");
          other = C(1);
        } else if (tm == "lea" && to == "eax, [eax + eax*4]") {
          op = Term::string("*");
          other = C(5);
        } else if (tm == "imul") {
          OI k = argk(to);
          op = Term::string("*");
          other = k ? std::optional<Term>(A(*k)) : cst(at1(to, ", ", -1));
        } else if (const char *a = alu(tm)) {
          OI k = argk(to);
          op = Term::string(a);
          other = k ? std::optional<Term>(A(*k)) : cst(at1(to, ", ", 1));
        }
        if (op && other) out.push_back(node(nargs, Term::tuple({Term::string("callbin"), *op, conv, Term::list(args), *other})));
      }
    }
    bool leave = false;
    for (auto &x : ins) leave = leave || x.first == "leave";
    seq_candidates(code, nargs, leave, out);
    call2_candidates(ops, nargs, out);
    return;
  }

  // globals
  if (ops.size() == 1 && ops[0].first == "mov" && starts(ops[0].second, "eax, ") && abs_at(ops[0].second, 5) == ops[0].second.size()) {
    out.push_back(node(nargs, Term::tuple({Term::string("gread"), Term::string("G")})));
    return;
  }
  const std::string *gw = nullptr;
  size_t gw_end = 0;
  for (auto &x : ops)
    if (x.first == "mov") {
      size_t e = abs_at(x.second, 0);
      if (e != std::string::npos && x.second.compare(e, 2, ", ") == 0) {
        gw = &x.second;
        gw_end = e + 2;
        break;
      }
    }
  if (gw) {
    std::string rhs = gw->substr(gw_end);
    if (rhs.empty()) unsupported( "'NoneType' object has no attribute 'group'");
    if (rhs == "eax") {
      if (OI ak = loadk("eax")) out.push_back(node(nargs, Term::tuple({Term::string("gwrite"), Term::string("G"), A(*ak)})));
    } else if (OI c = sconst_or_none(rhs)) {
      out.push_back(node(nargs, Term::tuple({Term::string("gwrite"), Term::string("G"), C(*c)})));
    }
    return;
  }
  for (auto &x : ops)
    if ((x.first == "inc" || x.first == "dec") && abs_at(x.second, 0) == x.second.size()) {
      out.push_back(node(nargs, Term::tuple({Term::string("gincdec"), Term::string("G"), Term::integer(x.first == "inc" ? 1 : -1)})));
      return;
    }

  // stores
  std::string r;
  long long dd;
  for (auto &x : ops)
    if (x.first == "mov" && regmem(x.second, 0, {"ecx"}, false, ", eax", r, dd)) {
      OI pk = loadk("ecx"), vk = loadk("eax");
      if (pk && vk) out.push_back(node(nargs, Term::tuple({Term::string("store"), A(*pk), A(*vk)})));
      return;
    }
  for (auto &x : ops)
    if (x.first == "and" && regmem(x.second, 0, {"eax"}, false, ", 0", r, dd)) {
      if (OI pk = loadk("eax")) out.push_back(node(nargs, Term::tuple({Term::string("store"), A(*pk), C(0)})));
      return;
    }
  for (auto &x : ops) {
    if (x.first != "mov") continue;
    Cursor c(x.second);
    c.lit("dword ptr ");
    if (!c.lit("[eax], ")) continue;
    OI pk = loadk("eax");
    std::string rhs = x.second.substr(c.i);
    if (rhs.empty()) unsupported( "'NoneType' object has no attribute 'group'");
    if (pk)
      if (OI v = sconst_or_none(rhs)) out.push_back(node(nargs, Term::tuple({Term::string("store"), A(*pk), C(*v)})));
    return;
  }

  // p[i]
  for (auto &x : ops)
    if (x.first == "mov" && starts(x.second, "eax,") && has(x.second, "eax*4]")) {
      OI ik = loadk("eax"), pk = loadk("ecx");
      if (ik && pk) out.push_back(node(nargs, Term::tuple({Term::string("index"), A(*pk), A(*ik)})));
      return;
    }

  // comparisons
  const std::string *setcc = nullptr;
  for (auto &x : ops)
    if (starts(x.first, "set")) {
      setcc = &x.first;
      break;
    }
  if (setcc) {
    OI Lk = loadk("ecx"), Rk;
    for (auto &x : ops)
      if (x.first == "cmp")
        if (OI k = argk(x.second)) {
          Rk = k;
          break;
        }
    static const std::pair<const char *, const char *> SET[] = {{"setl", "<"},  {"setle", "<="}, {"setg", ">"},
                                                                {"setge", ">="}, {"sete", "=="},  {"setne", "!="}};
    const char *rel = nullptr;
    for (auto &x : SET)
      if (*setcc == x.first) rel = x.second;
    if (Lk && Rk && rel) out.push_back(node(nargs, Term::tuple({Term::string("cmp"), Term::string(rel), A(*Lk), A(*Rk)})));
    else if (*setcc == "sete" && Rk) out.push_back(node(nargs, Term::tuple({Term::string("unary"), Term::string("lognot"), A(*Rk)})));
    return;
  }

  if (ops.empty()) {
    out.push_back(node(nargs, Term::tuple({Term::string("empty")})));
    return;
  }

  std::string first_reg;
  long long k0 = 0;
  if (ops[0].first == "mov") {
    std::string rr = strip(at1(ops[0].second, ",", 0));
    if (rr == "eax" || rr == "ecx")
      if (OI kk = argk(ops[0].second)) {
        first_reg = rr;
        k0 = *kk;
      }
  }
  int n_imul = 0;
  for (auto &x : ops) n_imul += x.first == "imul";
  bool has_addec = std::find(ops.begin(), ops.end(), MO("add", "eax, ecx")) != ops.end();
  bool has_subec = std::find(ops.begin(), ops.end(), MO("sub", "eax, ecx")) != ops.end();
  auto first_argk = [&](const char *mn, const char *prefix) -> OI {
    for (auto &x : ops)
      if (x.first == mn && starts(x.second, prefix))
        if (OI k = argk(x.second)) return k;
    return std::nullopt;
  };
  auto bin = [](const char *op, const Term &l, const Term &r) { return Term::tuple({Term::string("binop"), Term::string(op), l, r}); };

  if (first_reg == "eax" && (has_addec || has_subec)) {
    if (n_imul == 2) {
      OI b = k0, d = loadk("ecx"), a = first_argk("imul", "eax,"), c = first_argk("imul", "ecx,");
      if (a && b && c && d)
        out.push_back(node(nargs, bin(has_addec ? "+" : "-", bin("*", A(*a), A(*b)), bin("*", A(*c), A(*d)))));
      return;
    }
    if (has_addec && n_imul == 1) {
      OI b = k0, a = loadk("ecx"), c = first_argk("imul", "eax,");
      if (a && b && c) out.push_back(node(nargs, bin("*", bin("+", A(*a), A(*b)), A(*c))));
      return;
    }
    if (has_addec) {
      long long R = k0;
      OI Lk = loadk("ecx");
      std::vector<long long> extra;
      for (auto &x : ops)
        if (x.first == "add" && starts(x.second, "eax,") && x.second != "eax, ecx")
          if (OI k = argk(x.second)) extra.push_back(*k);
      if (Lk) {
        std::vector<long long> leaves(extra.rbegin(), extra.rend());
        leaves.push_back(*Lk);
        leaves.push_back(R);
        Term e = A(leaves[0]);
        for (size_t j = 1; j < leaves.size(); j++) e = bin("+", e, A(leaves[j]));
        out.push_back(node(nargs, e));
      }
      return;
    }
    return;
  }

  bool imul_eax = false, add_eax_arg = false;
  for (auto &x : ops) {
    if (x.first == "imul" && starts(x.second, "eax,")) imul_eax = true;
    if (x.first == "add" && starts(x.second, "eax,") && argk(x.second)) add_eax_arg = true;
  }
  if (first_reg == "eax" && imul_eax && add_eax_arg) {
    long long pR = k0;
    OI pL = first_argk("imul", "eax,"), leaf = first_argk("add", "eax,");
    if (pL && leaf) {
      Term P = bin("*", A(*pL), A(pR));
      out.push_back(node(nargs, bin("+", P, A(*leaf))));
      out.push_back(node(nargs, bin("+", A(*leaf), P)));
    }
    return;
  }

  if (first_reg == "eax" && ops.size() == 3 && ops[1].first == "sub" && argk(ops[1].second) && ops[2].first == "add" && argk(ops[2].second)) {
    long long P = k0, Mk = *argk(ops[1].second), H = *argk(ops[2].second);
    out.push_back(node(nargs, bin("-", bin("+", A(H), A(P)), A(Mk))));
    return;
  }

  if (first_reg != "eax") return;

  if (ops.size() == 2 && ops[1].first == "mov") {
    std::string rg;
    long long d;
    if (starts(ops[1].second, "eax, ") && regmem(ops[1].second, 5, {"eax"}, true, "", rg, d)) {
      if (d == 0) out.push_back(node(nargs, Term::tuple({Term::string("deref"), A(k0)})));
      if (d % 4 == 0) out.push_back(node(nargs, Term::tuple({Term::string("index"), A(k0), C(floor_quotient(d, 4))})));
      return;
    }
  }
  if (ops.size() == 2 && (ops[1].first == "neg" || ops[1].first == "not") && ops[1].second == "eax") {
    out.push_back(node(nargs, Term::tuple({Term::string("unary"), Term::string(ops[1].first), A(k0)})));
    return;
  }
  if (ops.size() == 2 && ops[1] == MO("imul", "eax, eax")) {
    out.push_back(node(nargs, bin("*", A(k0), A(k0))));
    return;
  }

  struct Step {
    std::string op;
    bool arg;
    long long v;
  };
  std::vector<Step> steps;
  for (size_t j = 1; j < ops.size(); j++) {
    const std::string &mn = ops[j].first, &op = ops[j].second;
    if (mn == "inc") {
      steps.push_back({"+", false, 1});
    } else if (mn == "dec") {
      steps.push_back({"-", false, 1});
    } else if (mn == "lea" && op == "eax, [eax + eax*4]") {
      steps.push_back({"*", false, 5});
    } else if (mn == "imul") {
      if (OI k = argk(op)) steps.push_back({"*", true, *k});
      else if (OI c = sconst_or_none(at1(op, ", ", -1))) steps.push_back({"*", false, *c});
      else return;
    } else if (const char *a = alu(mn)) {
      if (OI k = argk(op)) steps.push_back({a, true, *k});
      else if (OI c = sconst_or_none(at1(op, ", ", 1))) steps.push_back({a, false, *c});
      else return;
    } else {
      return;
    }
  }
  auto leafv = [](const Step &s) { return s.arg ? A(s.v) : C(s.v); };
  bool all_arg = true, same_op = true, all_sub_arg = true;
  for (auto &s : steps) {
    all_arg = all_arg && s.arg;
    same_op = same_op && s.op == steps[0].op;
    all_sub_arg = all_sub_arg && s.op == "-" && s.arg;
  }
  if (steps.empty()) {
    out.push_back(node(nargs, A(k0)));
  } else if (steps.size() == 1) {
    Term R = leafv(steps[0]);
    out.push_back(node(nargs, bin(steps[0].op.c_str(), A(k0), R)));
    if (steps[0].arg) out.push_back(node(nargs, bin(steps[0].op.c_str(), R, A(k0))));
  } else if (all_arg && same_op && (steps[0].op == "*" || steps[0].op == "&" || steps[0].op == "|" || steps[0].op == "^")) {
    std::vector<long long> idxs = {k0};
    for (auto &s : steps) idxs.push_back(s.v);
    std::reverse(idxs.begin(), idxs.end());
    Term e = A(idxs[0]);
    for (size_t j = 1; j < idxs.size(); j++) e = bin(steps[0].op.c_str(), e, A(idxs[j]));
    out.push_back(node(nargs, e));
  } else if (all_sub_arg) {
    std::vector<long long> src = {steps.back().v};
    for (size_t j = 0; j + 1 < steps.size(); j++) src.push_back(steps[j].v);
    Term e = A(k0);
    for (long long x : src) e = bin("-", e, A(x));
    out.push_back(node(nargs, e));
  } else {
    Term e = A(k0);
    for (auto &s : steps) e = bin(s.op.c_str(), e, leafv(s));
    out.push_back(node(nargs, e));
  }
}

std::set<long long> reloc_mask(const ByteString &code) {
  std::set<long long> m;
  for (auto &ins : disasm32((const uint8_t *)code.data(), code.size(), 0)) {
    const auto &b = ins.bytes;
    long long a = ins.address, n = (long long)b.size();
    auto add = [&](long long lo, long long hi) {
      for (long long x = lo; x < hi; x++) m.insert(x);
    };
    if (ins.mnemonic == "call" || ins.mnemonic == "jmp") {
      if (n >= 1 && (b[0] == 0xe8 || b[0] == 0xe9) && n == 5) add(a + 1, a + 5);
      else if (has(ins.op_str, "]") && n >= 6) add(a + n - 4, a + n);
    } else if (n >= 1 && (b[0] == 0xa1 || b[0] == 0xa3)) {
      add(a + 1, a + 5);
    } else if (n >= 2 && b[0] == 0xc7 && b[1] == 0x05) {
      add(a + 2, a + 6);
    } else if (n >= 2 && b[0] == 0xff && (b[1] == 0x05 || b[1] == 0x0d)) {
      add(a + 2, a + 6);
    }
  }
  return m;
}

bool match(const ByteString &pred, const ByteString &code, const std::set<long long> &mask) {
  if (pred.size() != code.size()) return false;
  for (size_t j = 0; j < code.size(); j++)
    if (!mask.count((long long)j) && pred[j] != code[j]) return false;
  return true;
}

Term invert(const ByteString &code, const std::set<long long> *mask) {
  auto ins = dis(code);
  bool fpo = !(ins.size() >= 2 && ins[0] == MO("push", "ebp") && ins[1] == MO("mov", "ebp, esp"));
  std::vector<Term> cands;
  std::exception_ptr late;   // what the candidate search raised after yielding these
  try {
    candidates(code, cands);
  } catch (const Error &) {
    late = std::current_exception();
  }
  for (auto &nd : cands) {
    try {
      Term n = nd;
      const Term &ex = nd.get("expr");
      if (fpo && ex.truthy()) {
        const Term &k = ex[0];
        if (k.is("arg") || k.is("binop") || k.is("unary") || k.is("deref") || k.is("index")) n.set("fpo", Term::boolean(true));
      }
      OB pred = emit(n);
      if (!pred) continue;
      std::set<long long> m = mask ? *mask : reloc_mask(*pred);
      if (match(*pred, code, m)) {
        Term r = Term::dict();
        r.set("nargs", n.at("nargs"));
        r.set("expr", n.get("expr"));
        r.set("c", Term::string(to_c(n)));
        r.set("node", n);
        return r;
      }
    } catch (const std::exception &) {
      continue;   // a candidate the forward model cannot lay out is not an answer
    }
  }
  if (late) std::rethrow_exception(late);
  return Term();
}

}  // namespace detail

std::optional<Bytes> emit(const Term &node) {
  const auto out = detail::emit(node);
  if (!out) return std::nullopt;
  return Bytes(out->begin(), out->end());
}

std::string to_c(const Term &node) { return detail::to_c(node); }

std::optional<Reading> invert(ByteView code, const std::set<long long> *mask) {
  const detail::ByteString raw(code.begin(), code.end());
  const Term found = detail::invert(raw, mask);
  if (found.none()) return std::nullopt;
  Reading r;
  r.nargs = found.at("nargs").number();
  r.expr = found.get("expr");
  r.c = found.at("c").text();
  r.node = found.at("node");
  return r;
}

std::optional<Term> interp(ByteView code, long long nargs) {
  const detail::ByteString raw(code.begin(), code.end());
  return detail::interp(raw, nargs);
}

FrameLayout frame_layout(std::span<const Local> locals) {
  FrameLayout out;
  std::int64_t cumulative = 0;
  for (int pass = 0; pass < 2; ++pass)
    for (std::size_t j = 0; j < locals.size(); ++j) {
      if (locals[j].is_array != (pass == 1)) continue;
      cumulative += (static_cast<std::int64_t>(locals[j].size) + 3) & ~std::int64_t{3};
      out.offsets.emplace_back(j, -cumulative);
    }
  out.frame_size = static_cast<std::uint32_t>(cumulative);
  return out;
}

}  // namespace cb::invert::msvc2
