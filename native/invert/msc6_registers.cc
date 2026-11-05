// msc6_registers.cc -- invert/msc6_registers.py, decision for decision: the register-value tracker
// for MSC 6.0a's register variables (invert_reg: SI/DI locals and the `push ax; mov si,ax` value
// number) and the counted for-loop with a register counter (invert_loop).  Answers are JSON:
// {"t": text, "m": meta} or {"t": null, "w": why}.
#include <map>
#include <set>
#include <string>
#include <vector>

#include "cs.h"
#include "msc6.h"
#include "pyutil.h"

namespace cbinv {

namespace {

const std::string PROLOGUE = "\x1e\x58\x90\x45\x55\x8b\xec\x1e\x8e\xd8";
const std::string EPI = "\x4d\x4d\x8b\xe5\x1f\x5d\x4d";

typedef std::map<long long, std::string> Imports;

std::string param(long long off) { return chr('a' + floordiv(off - 6, 2)); }   // [bp+6]=a, +8=b, ...

std::string declined(const std::string &why) { return "{\"t\": null, \"w\": " + jstr(why) + "}"; }

std::string jlist(const std::vector<std::string> &v) {
  std::string o = "[";
  for (size_t j = 0; j < v.size(); j++) o += (j ? ", " : "") + jstr(v[j]);
  return o + "]";
}

std::string join(const std::vector<std::string> &v, const char *sep) {
  std::string o;
  for (size_t j = 0; j < v.size(); j++) o += (j ? sep : "") + v[j];
  return o;
}

std::string import_name(const Imports &im, long long k) {
  auto it = im.find(k);
  return it != im.end() ? it->second : "G" + std::to_string(k);
}

// re.match(r"word ptr \[bp \+ (\w+)\]", r) and then re.search(r"\+ (\w+)", r).group(1)
bool bp_param(const std::string &r, std::string &w) {
  std::string first;
  if (!starts(r, "word ptr ") || !bp_plus_at(r, 9, first)) return false;
  size_t p = 0;
  while ((p = r.find("+ ", p)) != std::string::npos) {
    Cur c(r, p + 2);
    if (c.word(w)) return true;
    p++;
  }
  return false;
}

// re.match(r"\d|0x", r)
bool num_start(const std::string &r) { return (!r.empty() && isd(r[0])) || starts(r, "0x"); }

unsigned char at(const std::string &b, size_t i) {   // b[i] (IndexError)
  if (i >= b.size()) raise(PY_INDEX, "index out of range");
  return (unsigned char)b[i];
}

}  // namespace

std::string msc6_invert_reg(const std::string &b, const Imports &imports) {
  if (b.compare(0, PROLOGUE.size(), PROLOGUE) != 0) return declined("prologue miss");
  size_t p = PROLOGUE.size();
  if (b.size() >= p + 2 && b.compare(p, 2, "\x83\xec") == 0) p += 3;
  std::vector<std::string> saved;
  while (p < b.size() && (b[p] == 0x57 || b[p] == 0x56)) saved.push_back(b[p++] == 0x57 ? "di" : "si");
  size_t ei = b.rfind(EPI);
  if (ei == std::string::npos) return declined("epilogue miss");
  size_t be = ei;
  while (be > p && (b[be - 1] == 0x5e || b[be - 1] == 0x5f)) be--;
  std::string body = be > p ? b.substr(p, be - p) : "";

  std::map<std::string, std::string> reg;   // reg -> C expression ("_lastcall" too)
  std::vector<std::string> args, stmts;
  std::vector<std::pair<std::string, std::string>> regvars;   // reg -> local name, in order
  auto regvar_of = [&](const std::string &r) -> bool {
    for (auto &x : regvars)
      if (x.first == r) return true;
    return false;
  };
  long long callno = 0;
  for (auto &ins : disasm16((const uint8_t *)body.data(), body.size(), 0)) {
    const std::string &mn = ins.mnemonic, &ops = ins.op_str;
    std::string w;
    if (ins.bytes[0] == 0x9a) {
      std::string expr = import_name(imports, callno++) + "(" + join(args, ", ") + ")";
      args.clear();
      reg["ax"] = expr;
      reg["_lastcall"] = expr;
    } else if (mn == "push") {
      if (reg.count(ops)) args.push_back(reg[ops]);
      else if (bp_param(ops, w)) args.push_back(param(int0(w)));
      else if (num_start(ops)) args.push_back(std::to_string(int0(ops)));
      else return declined("push form unmodeled: " + ops);
    } else if (mn == "mov") {
      auto pr = partition(ops, ", ");
      const std::string &dst = pr.first, &srcx = pr.second;
      if (dst == "si" || dst == "di" || dst == "ax" || dst == "cx" || dst == "dx" || dst == "bx") {
        if (reg.count(srcx)) {
          std::string val = reg[srcx];
          auto lc = reg.find("_lastcall");
          if ((dst == "si" || dst == "di") && !regvar_of(dst) && lc != reg.end() && val == lc->second) {
            std::string vn = chr('b' + (long long)regvars.size());
            regvars.push_back({dst, vn});
            stmts.push_back(vn + " = " + val + ";");
            for (auto &a : args)
              if (a == val) a = vn;
            reg[dst] = vn;
          } else {
            reg[dst] = val;
          }
        } else if (bp_param(srcx, w)) {
          reg[dst] = param(int0(w));
        } else if (num_start(srcx)) {
          reg[dst] = std::to_string(int0(srcx));
        } else {
          return declined("mov src unmodeled: " + srcx);
        }
      } else {
        return declined("mov dst unmodeled: " + dst);
      }
    } else if (mn == "sub" && ops == "ax, ax") {
      reg["ax"] = "0";
    } else {
      return declined("insn unmodeled: " + mn + " " + ops);
    }
  }

  // _finish
  size_t q = ei + EPI.size();
  long long conv_args = 0;
  if (q < b.size() && (unsigned char)b[q] == 0xca) conv_args = floordiv(at(b, q + 1) | (at(b, q + 2) << 8), 2);
  std::vector<std::string> ps;
  for (long long i = 0; i < conv_args; i++) ps.push_back("int " + chr('a' + i));
  std::string params = ps.empty() ? "void" : join(ps, ", ");
  std::string decls = "typedef unsigned int WORD;\n";
  std::set<std::string> seen;
  for (long long ci = 0; ci < callno; ci++) {
    std::string nm = import_name(imports, ci);
    if (seen.insert(nm).second) decls += "extern int far pascal " + nm + "();\n";
  }
  std::vector<std::string> body_lines = stmts;
  auto lc = reg.find("_lastcall");
  if (lc != reg.end() && !lc->second.empty() && join(stmts, "").find(lc->second) == std::string::npos) body_lines.push_back(lc->second + ";");
  auto ax = reg.find("ax");
  if (ax != reg.end() && !ax->second.empty()) body_lines.push_back("return " + ax->second + ";");
  std::string inner;
  std::vector<std::string> names;
  for (auto &x : regvars) {
    inner += "    register int " + x.second + ";\n";
    names.push_back(x.second);
  }
  for (auto &ln : body_lines) inner += "    " + ln + "\n";
  std::string src = decls + "\nint far pascal F(" + params + ")\n{\n" + inner + "}";
  return "{\"t\": " + jstr(src) + ", \"m\": {\"saved\": " + jlist(saved) + ", \"regvars\": " + jlist(names) +
         ", \"stmts\": " + jlist(body_lines) + "}}";
}

std::string msc6_invert_loop(const std::string &b, const Imports &imports) {
  if (b.compare(0, PROLOGUE.size(), PROLOGUE) != 0) return declined("prologue miss");
  size_t p = PROLOGUE.size();
  if (b.size() >= p + 2 && b.compare(p, 2, "\x83\xec") == 0) p += 3;
  while (p < b.size() && (b[p] == 0x57 || b[p] == 0x56)) p++;
  if (!(p + 2 <= b.size() && b.compare(p, 2, "\x8b\x7e") == 0)) return declined("no di=param");
  std::string a = param(at(b, p + 2));
  p += 3;
  if (!(p + 2 <= b.size() && b.compare(p, 2, "\x2b\xf6") == 0)) return declined("no si=0 (loop counter init)");
  p += 2;
  if (!(p + 2 <= b.size() && b.compare(p, 2, "\x0b\xff") == 0) || at(b, p + 2) != 0x7e) return declined("no loop guard (or di,di; jle)");
  p += 4;
  size_t lstart = p;
  size_t j = p <= b.size() ? b.find("\x3b\xf7", p) : std::string::npos;
  if (j == std::string::npos || at(b, j + 2) != 0x7c) return declined("no back-edge (cmp si,di; jl)");
  size_t inc = j - 1;
  if (at(b, inc) != 0x46) return declined("no counter increment (inc si)");
  std::string bodyb = inc > lstart ? b.substr(lstart, inc - lstart) : "";
  std::vector<std::string> args, calls;
  for (auto &ins : disasm16((const uint8_t *)bodyb.data(), bodyb.size(), 0)) {
    const std::string &mn = ins.mnemonic, &ops = ins.op_str;
    if (mn == "push" && ops == "si") {
      args.push_back("i");
    } else if (mn == "push" && num_start(ops)) {
      args.push_back(std::to_string(int0(ops)));
    } else if (ins.bytes[0] == 0x9a) {
      calls.push_back(import_name(imports, (long long)calls.size()) + "(" + join(args, ", ") + ")");
      args.clear();
    } else {
      return declined("loop body insn unmodeled: " + mn + " " + ops);
    }
  }
  std::string decls = "typedef unsigned int WORD;\n";
  for (size_t k = 0; k < calls.size(); k++) decls += "extern int far pascal " + import_name(imports, (long long)k) + "();\n";
  std::string body_c;
  if (calls.size() != 1)
    for (auto &c : calls) body_c += "    " + c + ";\n";
  else
    body_c = "        " + calls[0] + ";";
  std::string forline = "    for (i = 0; i < " + a + "; i++)\n" + body_c + "\n";
  std::string src = decls + "\nint far pascal F(int " + a + ")\n{\n    register int i;\n" + forline + "    return i;\n}";
  return "{\"t\": " + jstr(src) + ", \"m\": {\"counter\": \"i\", \"limit\": " + jstr(a) + ", \"body_calls\": " + jlist(calls) + "}}";
}

}  // namespace cbinv
