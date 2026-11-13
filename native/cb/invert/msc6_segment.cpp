// cb-unity: alone
#include "cb/invert/msc6_segment.hpp"

#include <algorithm>
#include <cstring>
#include <map>
#include <set>

#include "cb/core/capstone_api.hpp"
#include "cb/invert/model_text.hpp"
#include "cb/invert/operand_match.hpp"
#include "cb/platforms/decoder.hpp"

namespace cb::invert::msc6_segment {

using namespace cb::invert::model;

namespace {

// The 16-bit instructions of `code` loaded at `address`, up to the first byte that does not decode.
std::vector<platforms::Insn> disasm16(const Bytes &code, long long address) {
  thread_local platforms::DecoderSet decoders;
  return decoders.get(platforms::Arch::X86, 16).disasm(code, static_cast<std::uint64_t>(address));
}

// An instruction with the operand detail whole-function matching reads (Capstone's: a register, an immediate or a memory operand with its base and
// displacement).
struct Operand {
  int type = 0;   // 1 register, 2 immediate, 3 memory
  long long imm = 0;
  int mem_base = 0;
  long long mem_disp = 0;
};
struct Detailed {
  std::string mnemonic, op_str;
  std::vector<Operand> operands;
};

std::vector<Detailed> detailed(const Bytes &code) {
  thread_local platforms::DecoderSet decoders;
  std::vector<Detailed> out;
  decoders.get(platforms::Arch::X86, 16, true).each(code, 0, 0, [&](const cs_insn &c) {
    Detailed d;
    d.mnemonic = c.mnemonic;
    d.op_str = c.op_str;
    if (c.detail) {
      const cs_x86 &x = c.detail->x86;
      for (int j = 0; j < x.op_count; j++) {
        Operand op;
        op.type = static_cast<int>(x.operands[j].type);
        if (x.operands[j].type == X86_OP_IMM) op.imm = x.operands[j].imm;
        if (x.operands[j].type == X86_OP_MEM) {
          op.mem_base = static_cast<int>(x.operands[j].mem.base);
          op.mem_disp = x.operands[j].mem.disp;
        }
        d.operands.push_back(op);
      }
    }
    out.push_back(std::move(d));
    return true;
  });
  return out;
}

Bytes bytes_of_hex(const char *h) {
  Bytes b;
  for (size_t i = 0; h[i] && h[i + 1]; i += 2) {
    auto d = [](char c) { return is_digit_char(c) ? c - '0' : c - 'a' + 10; };
    b.push_back((uint8_t)(d(h[i]) * 16 + d(h[i + 1])));
  }
  return b;
}

std::string hex_of(const Bytes &b, size_t a = 0, size_t e = std::string::npos) {
  static const char *D = "0123456789abcdef";
  std::string s;
  e = std::min(e, b.size());
  for (size_t i = a; i < e; i++) {
    s += D[b[i] >> 4];
    s += D[b[i] & 15];
  }
  return s;
}

const Bytes PROLOGUE = bytes_of_hex("1e589045558bec1e8ed8");
const Bytes EPI = bytes_of_hex("4d4d8be51f5d4d");

long long bfind(const Bytes &b, const Bytes &p, long long from, long long to = -1) {
  long long n = to < 0 ? (long long)b.size() : std::min<long long>(to, (long long)b.size());
  for (long long i = std::max<long long>(from, 0); i + (long long)p.size() <= n; i++)
    if (std::equal(p.begin(), p.end(), b.begin() + i)) return i;
  return -1;
}
long long brfind(const Bytes &b, const Bytes &p, long long from, long long to) {
  long long n = std::min<long long>(to, (long long)b.size());
  for (long long i = n - (long long)p.size(); i >= std::max<long long>(from, 0); i--)
    if (std::equal(p.begin(), p.end(), b.begin() + i)) return i;
  return -1;
}
int at(const Bytes &b, long long i) {
  if (i < 0 || i >= (long long)b.size()) fail( "index out of range");
  return b[(size_t)i];
}
int byte_or(const Bytes &b, long long i) { return (i >= 0 && i < (long long)b.size()) ? b[(size_t)i] : -1; }
Bytes slice(const Bytes &b, long long a, long long e) {
  a = std::max<long long>(0, std::min<long long>(a, (long long)b.size()));
  e = std::max<long long>(a, std::min<long long>(e, (long long)b.size()));
  return Bytes(b.begin() + a, b.begin() + e);
}

// ---- msc6_calls ----

std::vector<std::pair<long long, long long>> func_bounds(const Bytes &segb) {
  std::vector<std::pair<long long, long long>> outs;
  long long p = bfind(segb, PROLOGUE, 0), consumed = 0;
  while (p >= 0) {
    if (p >= consumed) {
      long long e = bfind(segb, EPI, p);
      if (e >= 0) {
        long long q = e + (long long)EPI.size();
        int c = byte_or(segb, q);
        long long end = c == 0xca ? q + 3 : (c == 0xcb || c == 0xc3) ? q + 1 : q;
        outs.push_back({p, end});
        consumed = end;
      }
    }
    p = bfind(segb, PROLOGUE, p + 1);
  }
  return outs;
}

std::optional<long long> retf_total(const Bytes &segb, long long start, long long end) {
  long long e = brfind(segb, EPI, start, end);
  if (e < 0) return std::nullopt;
  long long q = e + (long long)EPI.size();
  // a `ca` with fewer than two bytes after it is padding, not `retf N`: no total (it raised IndexError and lost the segment)
  if (byte_or(segb, q) == 0xca && q + 2 < (long long)segb.size()) return at(segb, q + 1) | (at(segb, q + 2) << 8);
  return std::nullopt;
}

std::set<long long> far_body_offsets(const Bytes &fb) {
  std::set<long long> offs;
  for (auto &ins : disasm16(fb, 0)) {
    std::string w;
    if ((ins.mnemonic == "les" || ins.mnemonic == "lds") && search_bp_plus(ins.op_str, w)) offs.insert(parse_number(w));
    bool mm = (starts(ins.op_str, "es, word ptr ") || starts(ins.op_str, "ds, word ptr ")) && bp_plus_at(ins.op_str, 13, w);
    if (ins.mnemonic == "mov" && mm) offs.insert(parse_number(w) - 2);
  }
  return offs;
}

std::set<long long> long_body_offsets(const Bytes &fb) {
  std::set<long long> offs;
  auto insns = disasm16(fb, 0);
  for (size_t i = 0; i + 1 < insns.size(); i++) {
    std::string w1, w2;
    bool m1 = starts(insns[i].op_str, "ax, word ptr ") && bp_plus_full(insns[i].op_str, 13, w1);
    bool m2 = starts(insns[i + 1].op_str, "dx, word ptr ") && bp_plus_full(insns[i + 1].op_str, 13, w2);
    if (insns[i].mnemonic == "mov" && insns[i + 1].mnemonic == "mov" && m1 && m2 && parse_number(w2) == parse_number(w1) + 2)
      offs.insert(parse_number(w1));
  }
  return offs;
}

bool segpush(int b) { return b == 0x06 || b == 0x0e || b == 0x16 || b == 0x1e; }

// [(words, is_far)] in push order
std::vector<std::pair<int, bool>> parse_arg_run(const std::vector<platforms::Insn> &insns, long long ci) {
  std::vector<const platforms::Insn *> run;
  for (long long j = ci - 1; j >= 0 && insns[(size_t)j].mnemonic == "push"; j--) run.push_back(&insns[(size_t)j]);
  std::reverse(run.begin(), run.end());
  std::vector<std::pair<int, bool>> args;
  size_t k = 0;
  while (k < run.size()) {
    const platforms::Insn *ins = run[k];
    const platforms::Insn *nxt = k + 1 < run.size() ? run[k + 1] : nullptr;
    bool far = false;
    if (segpush(ins->bytes[0]) && nxt) {
      far = true;
    } else if (nxt) {
      std::string w1, w2;
      bool m1 = search_bp_plus(ins->op_str, w1), m2 = search_bp_plus(nxt->op_str, w2);
      if (m1 && m2) {
        long long a = parse_number(w1);
        if (a == parse_number(w2) + 2) far = true;
      }
    }
    if (far) {
      args.push_back({2, true});
      k += 2;
    } else {
      args.push_back({1, false});
      k += 1;
    }
  }
  return args;
}

long long maxbp(const Bytes &fb) {
  long long m = 4;
  for (auto &ins : disasm16(fb, 0))
    for (auto &w : finditer_bp_plus(ins.op_str)) {
      long long o = parse_number(w);
      if (o >= 6) m = std::max(m, o);
    }
  return m;
}

// ---- segment_thunk ----

const Bytes PRO_8CD8 = bytes_of_hex("8cd89045558bec1e8ed8");
const Bytes PRO_1E58 = bytes_of_hex("1e589045558bec1e8ed8");
const Bytes T_EPI_LEA = bytes_of_hex("8d66fe1f5d4d");
const Bytes T_EPI_STD = bytes_of_hex("4d4d8be51f5d4d");

bool at_is(const Bytes &b, size_t p, const Bytes &x) {
  return p + x.size() <= b.size() && std::equal(x.begin(), x.end(), b.begin() + p);
}

const char *body_op(const Bytes &fb, size_t p, int &sz) {
  size_t n = fb.size();
  int c = fb[p];
  auto eq2 = [&](int a, int b2) { return p + 2 <= n && fb[p] == a && fb[p + 1] == b2; };
  if (p + 3 <= n && fb[p] == 0x66 && fb[p + 1] == 0xff && fb[p + 2] == 0x76 && p + 4 <= n) return sz = 4, "pushd";
  if (c == 0xff && p + 3 <= n && fb[p + 1] == 0x76) return sz = 3, "pushw";
  if (c == 0xff && p + 4 <= n && fb[p + 1] == 0x36) return sz = 4, "pushm";
  if (0x50 <= c && c <= 0x57) return sz = 1, "pushr";
  if (c == 0x1e || c == 0x06 || c == 0x0e || c == 0x16) return sz = 1, "pushseg";
  if (c == 0x1f || c == 0x07) return sz = 1, "popseg";
  if (c == 0x6a && p + 2 <= n) return sz = 2, "pushi8";
  if (c == 0x68 && p + 3 <= n) return sz = 3, "pushi16";
  if (c == 0x9a && p + 5 <= n) return sz = 5, "lcall";
  if (eq2(0x2b, 0xc0) || eq2(0x33, 0xc0)) return sz = 2, "zero";
  if (c == 0x99) return sz = 1, "cdq";
  if (c == 0x98) return sz = 1, "cwd";
  if (eq2(0x83, 0xc4) && p + 3 <= n) return sz = 3, "addsp";
  if (c == 0xb8 && p + 3 <= n) return sz = 3, "movax";
  sz = 0;
  return nullptr;
}


std::optional<Thunk> thunk_parse(const Bytes &fb) {
  Thunk t;
  if (at_is(fb, 0, PRO_8CD8)) {
    t.prologue = PRO_8CD8;
  } else if (at_is(fb, 0, PRO_1E58)) {
    t.prologue = PRO_1E58;
  } else {
    return std::nullopt;
  }
  size_t p = t.prologue.size();
  if (!(p + 5 <= fb.size() && fb[p] == 0xb8 && fb[p + 3] == 0x8e && fb[p + 4] == 0xd8)) return std::nullopt;
  t.segment = fb[p + 1] | (fb[p + 2] << 8);
  p += 5;
  while (p < fb.size()) {
    int sz;
    const char *nm = body_op(fb, p, sz);
    if (!nm) break;
    t.body.push_back({nm, Bytes(fb.begin() + p, fb.begin() + std::min(fb.size(), p + sz))});
    p += sz;
  }
  if (at_is(fb, p, T_EPI_LEA)) {
    t.epilogue = T_EPI_LEA;
  } else if (at_is(fb, p, T_EPI_STD)) {
    t.epilogue = T_EPI_STD;
  } else {
    return std::nullopt;
  }
  p += t.epilogue.size();
  if (p < fb.size() && fb[p] == 0xca && p + 3 <= fb.size()) {
    t.retf = fb[p + 1] | (fb[p + 2] << 8);
    p += 3;
  } else if (p < fb.size() && fb[p] == 0xcb) {
    p += 1;
  } else {
    return std::nullopt;
  }
  t.padding.assign(fb.begin() + p, fb.end());
  for (auto x : t.padding)
    if (x != 0x90 && x != 0x00) return std::nullopt;
  return t;
}

std::string fmt(const char *f, long long v) {
  char b[64];
  snprintf(b, sizeof b, f, v);
  return b;
}

std::string thunk_asm(const Thunk &t, const std::string &name) {
  static const char *GP[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
  std::vector<std::string> L = {name + " PROC FAR", "    inc  bp", "    push bp", "    mov  bp, sp",
                                "    push ds", "    mov  ds, ax", fmt("    mov  ax, %04llXh", t.segment), "    mov  ds, ax"};
  for (auto &nb : t.body) {
    const std::string &nm = nb.op;
    const Bytes &raw = nb.bytes;
    auto R = [&](size_t i) { return (long long)at(raw, (long long)i); };
    if (nm == "pushd") L.push_back(fmt("    push dword ptr [bp+%02llXh]", R(3)));
    else if (nm == "pushw") L.push_back(fmt("    push word ptr [bp+%02llXh]", R(2)));
    else if (nm == "pushm") L.push_back(fmt("    push word ptr [%04llXh]", R(2) | (R(3) << 8)));
    else if (nm == "pushr") L.push_back(std::string("    push ") + GP[raw[0] - 0x50]);
    else if (nm == "pushseg") L.push_back(std::string("    push ") + (raw[0] == 0x1e ? "ds" : raw[0] == 0x06 ? "es" : raw[0] == 0x0e ? "cs" : "ss"));
    else if (nm == "popseg") L.push_back(std::string("    pop  ") + (raw[0] == 0x1f ? "ds" : "es"));
    else if (nm == "pushi8") L.push_back(fmt("    push %02llXh", R(1)));
    else if (nm == "pushi16") L.push_back(fmt("    push %04llXh", R(1) | (R(2) << 8)));
    else if (nm == "lcall") L.push_back("    call FAR PTR target");
    else if (nm == "zero") L.push_back("    xor  ax, ax");
    else if (nm == "cdq") L.push_back("    cdq");
    else if (nm == "cwd") L.push_back("    cwde");
    else if (nm == "addsp") L.push_back(fmt("    add  sp, %02llXh", R(2)));
    else if (nm == "movax") L.push_back(fmt("    mov  ax, %04llXh", R(1) | (R(2) << 8)));
  }
  L.push_back(t.epilogue == T_EPI_LEA ? "    lea  sp, [bp-2]" : "    dec  bp");
  L.push_back("    pop  ds");
  L.push_back("    pop  bp");
  L.push_back("    dec  bp");
  L.push_back("    retf " + std::to_string(t.retf ? *t.retf : 0));
  L.push_back(name + " ENDP");
  std::string s;
  for (size_t k = 0; k < L.size(); k++) s += (k ? "\n" : "") + L[k];
  return s;
}

std::vector<std::pair<std::string, std::string>> whole_function_candidates(const Bytes &fb) {
  const auto ins = detailed(fb);
  if (ins.size() < 6) return {};
  static const char *PRO[] = {"push ds", "pop ax", "nop", "inc bp", "push bp", "mov bp, sp", "push ds", "mov ds, ax"};
  size_t i = 0;
  for (auto p : PRO) {
    if (i >= ins.size()) return {};
    std::string t = ins[i].mnemonic + " " + ins[i].op_str;
    size_t a = t.find_first_not_of(" \t\n\r\f\v"), b = t.find_last_not_of(" \t\n\r\f\v");
    t = a == std::string::npos ? "" : t.substr(a, b - a + 1);
    if (t != p) return {};
    i++;
  }
  if (i < ins.size() && ins[i].mnemonic == "sub") i++;
  std::vector<std::pair<bool, long long>> pushes;   // (is_bp, value)
  while (i < ins.size() && ins[i].mnemonic == "push") {
    if (ins[i].operands.empty()) fail( "list index out of range");
    const Operand &op = ins[i].operands[0];
    if (op.type == 1) break;
    if (op.type == 3 && op.mem_base != 0) {
      pushes.push_back({true, op.mem_disp});
    } else if (op.type == 2) {
      pushes.push_back({false, op.imm});
    } else {
      return {};
    }
    i++;
  }
  if (!(i + 1 < ins.size() && !ins[i].operands.empty() && ins[i].operands[0].type == 1 && ins[i + 1].mnemonic == "call")) return {};
  i += 2;
  if (i < ins.size() && ins[i].mnemonic == "add") i++;
  for (size_t j = i; j < ins.size(); j++) {
    const std::string &m = ins[j].mnemonic;
    if (m == "call" || m == "les" || m == "lea" || (m == "mov" && !has(ins[j].op_str, "sp"))) return {};
  }
  bool anybp = false;
  for (auto &p : pushes)
    if (p.first) anybp = true;
  if (!anybp) return {};
  const Detailed &ret = ins.back();
  bool pascal = ret.mnemonic == "retf" && !ret.op_str.empty();
  std::vector<long long> offs;
  long long nz = 0;
  for (auto &p : pushes) {
    if (p.first) offs.push_back(p.second);
    else if (p.second == 0) nz++;
  }
  std::set<long long> uq(offs.begin(), offs.end());
  std::string params;
  for (auto o : uq) params += (params.empty() ? "int p" : ", int p") + hex(o);
  if (params.empty()) params = "void";
  std::string conv = pascal ? "pascal " : "";
  std::vector<std::pair<std::string, std::string>> out;
  for (int rev = 0; rev < 2; rev++) {
    std::vector<std::string> fwd(nz, "0");
    if (rev) {
      for (auto it = offs.rbegin(); it != offs.rend(); ++it) fwd.push_back("p" + hex(*it));
    } else {
      for (auto o : offs) fwd.push_back("p" + hex(o));
    }
    out.emplace_back("thunk", "extern int far SIB();\nint far " + conv + "F(" + params + "){ return SIB(" + join(fwd, ", ") + "); }");
  }
  return out;
}

}  // namespace

std::vector<std::pair<std::uint32_t, std::uint32_t>> framed_functions(ByteView segment) {
  const Bytes segb(segment.begin(), segment.end());
  std::vector<std::pair<std::uint32_t, std::uint32_t>> out;
  for (const auto &[start, end] : func_bounds(segb)) out.emplace_back(static_cast<std::uint32_t>(start), static_cast<std::uint32_t>(end));
  return out;
}

std::vector<Signature> infer_signatures(ByteView segment) {
  const Bytes segb(segment.begin(), segment.end());
  const auto bounds = func_bounds(segb);
  std::set<long long> startset;
  for (auto &b : bounds) startset.insert(b.first);
  std::map<long long, std::optional<long long>> retf;
  std::map<long long, std::set<long long>> far_body, long_body, far_call;
  for (auto &b : bounds) retf[b.first] = retf_total(segb, b.first, b.second);
  for (auto &b : bounds) far_body[b.first] = far_body_offsets(slice(segb, b.first, b.second));
  for (auto &b : bounds) long_body[b.first] = long_body_offsets(slice(segb, b.first, b.second));
  for (auto &b : bounds) far_call[b.first];
  const auto insns = disasm16(segb, 0);
  for (size_t i = 0; i < insns.size(); i++) {
    const platforms::Insn &ins = insns[i];
    if (!(ins.raw()[0] == 0xe8 && ins.mnemonic == "call")) continue;
    const long long v = ins.raw()[1] | (ins.raw()[2] << 8);
    const long long tgt = static_cast<long long>(ins.address) + ins.size + (v >= 0x8000 ? v - 0x10000 : v);
    if (!startset.count(tgt)) continue;
    const auto args = parse_arg_run(insns, static_cast<long long>(i));
    long long off = 6;
    for (auto it = args.rbegin(); it != args.rend(); ++it) {
      if (it->second) far_call[tgt].insert(off);
      off += it->first * 2;
    }
  }
  std::vector<Signature> out;
  for (auto &b : bounds) {
    const long long s = b.first, e = b.second;
    const auto total = retf[s];
    std::set<long long> far = far_body[s];
    far.insert(far_call[s].begin(), far_call[s].end());
    std::set<long long> lng;
    for (auto o : long_body[s])
      if (!far.count(o)) lng.insert(o);
    const long long limit = (total && *total) ? 6 + *total : maxbp(slice(segb, s, e)) + 2;
    Signature sig;
    sig.offset = static_cast<std::uint32_t>(s);
    long long off = 6;
    while (off < limit) {
      std::string name = "p" + std::to_string(sig.params.size());
      if (far.count(off) && off + 4 <= limit) {
        sig.params.push_back({name, msc6::Kind::Far});
        off += 4;
      } else if (lng.count(off) && off + 4 <= limit) {
        sig.params.push_back({name, msc6::Kind::Long});
        off += 4;
      } else {
        sig.params.push_back({name, msc6::Kind::Int});
        off += 2;
      }
    }
    if (!sig.params.empty()) out.push_back(std::move(sig));
  }
  return out;
}

ThunkKind Thunk::kind() const {
  for (auto &b : body)
    if (b.op == "lcall") return ThunkKind::Forwarder;
  return ThunkKind::ReturnStub;
}

Bytes Thunk::bytes() const {
  Bytes o = prologue;
  o.push_back(0xb8);
  o.push_back(static_cast<uint8_t>(segment & 0xff));
  o.push_back(static_cast<uint8_t>(segment >> 8));
  o.push_back(0x8e);
  o.push_back(0xd8);
  for (auto &b : body) o.insert(o.end(), b.bytes.begin(), b.bytes.end());
  o.insert(o.end(), epilogue.begin(), epilogue.end());
  if (!retf) {
    o.push_back(0xcb);
  } else {
    o.push_back(0xca);
    o.push_back(static_cast<uint8_t>(*retf & 0xff));
    o.push_back(static_cast<uint8_t>(*retf >> 8));
  }
  o.insert(o.end(), padding.begin(), padding.end());
  return o;
}

std::optional<Thunk> thunk(ByteView function, bool exact) {
  const Bytes fb(function.begin(), function.end());
  auto t = thunk_parse(fb);
  if (!t || (exact && t->bytes() != fb)) return std::nullopt;
  return t;
}

std::string thunk_assembly(const Thunk &t, std::string_view name) {
  return thunk_asm(t, std::string(name));
}

std::vector<Shape> shapes(ByteView function) {
  const Bytes fb(function.begin(), function.end());
  std::vector<Shape> out;
  for (auto &c : whole_function_candidates(fb)) out.push_back(Shape{c.first, c.second});
  return out;
}

}  // namespace cb::invert::msc6_segment
