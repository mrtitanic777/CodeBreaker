// cb-unity: alone
// The Tracker's larger readers (early_c_tracker.hpp): carries, call arguments, what happens to ax after an instruction, whether a function
// returns a value, a call's result.
#include "invert/early_c_tracker.hpp"

#include <algorithm>
#include <climits>

namespace cb::invert::early_c_model {

// ------------------------------------------------------------------------------------ carries
std::string carry_kind(const std::vector<platforms::Insn> &ins, long long k);

int Tracker::carry(int i0, const List &o) {
  if (I(i0).mnemonic == "sbb" && carry_kind(ins, i0) == "setcc") {
    kill_aliases(i0);
    auto pr = partition(I(i0 - 1).op_str, ", ");
    string r = strip(split(I(i0).op_str, ",")[0]);
    binval("mov", r);
    string oa = text_of(opval(pr.first));
    string ob = text_of(opval(pr.second));
    setr(r, fresh("(0 - (" + oa + " < " + ob + "))"));
    if (r == "ax") axsrc = NI, retlive = true;
    if (r == "bx") bxsrc = NI;
    return 1;
  }
  string kind = i0 + 1 < N() ? carry_kind(ins, i0 + 1) : "";
  if (in(kind, {"shl1", "shr1", "sar1"})) {
    for (int r = 0; r < 6; r++)
      if (hascall(or_else(rg[r], ""))) materialize(o, WRN[r]);
    string a = strip(split(ins[i0].op_str, ",")[0]), b = strip(split(ins[i0 + 1].op_str, ",")[0]);
    string lo = kind == "shl1" ? a : b, hi = kind == "shl1" ? b : a;
    if (msc3 && has(lo, "[") && has(hi, "[")) {
      Value ml = memname(lo);
      if (!ml.empty()) {
        emit(o, "*(long *)&" + ml.s() + " " + (kind == "shl1" ? "<<" : ">>") + "= 1;");
        if (kind == "shr1") {
          LAST.has_unsigned_longs = true;
          if (std::find(LAST.unsigned_longs.begin(), LAST.unsigned_longs.end(), ml.s()) == LAST.unsigned_longs.end())
            LAST.unsigned_longs.push_back(ml.s());
        }
        return 2;
      }
    }
    kill_aliases(i0);
    kill_aliases(i0 + 1);
    string vh = text_of(opval(hi));
    string vl = text_of(opval(lo));
    string v = "LONG(" + vh + ", " + vl + ")";
    string E = kind == "shl1" ? v + " + " + v : string(kind == "shr1" ? "LSHR" : "LSAR") + "(" + v + ")";
    binval("mov", lo);
    binval("mov", hi);
    setr(lo, fresh("LO(" + E + ")"));
    setr(hi, fresh("HI(" + E + ")"));
    if (lo == "ax" || hi == "ax") axsrc = NI, retlive = true;
    if (lo == "bx" || hi == "bx") bxsrc = NI;
    return 2;
  }
  if (kind != "pair" && kind != "neg") return 0;
  for (int r = 0; r < 6; r++)
    if (hascall(or_else(rg[r], ""))) materialize(o, WRN[r]);
  const platforms::Insn &p = ins[i0], &c = ins[i0 + 1];
  string E, lo, hi;
  int n;
  if (kind == "neg") {
    lo = strip(p.op_str);
    hi = strip(split(c.op_str, ",")[0]);
    kill_aliases(i0);
    kill_aliases(i0 + 1);
    string vh = text_of(opval(hi));
    string vl = text_of(opval(lo));
    E = "0 - LONG(" + vh + ", " + vl + ")";
    n = 3;
  } else {
    auto pl = partition(p.op_str, ", "), ph = partition(c.op_str, ", ");
    lo = pl.first, hi = ph.first;
    string x = pl.second, y = ph.second;
    string op = p.mnemonic == "add" ? "+" : "-";
    if (wr(lo) >= 0) kill_aliases(i0), kill_aliases(i0 + 1);
    string Xv = text_of(opval(x)), Yv = text_of(opval(y));
    if (wr(lo) < 0) {
      Value ml = memname(lo), mh = memname(hi);
      string t = newt();
      emit(o, t + " = LONG(" + text_of(mh) + ", " + text_of(ml) + ") " + op + " LONG(" + Yv + ", " + Xv + ");");
      emit(o, text_of(ml) + " = LO(" + t + ");");
      emit(o, text_of(mh) + " = HI(" + t + ");");
      return 2;
    }
    string lo0 = text_of(opval(lo)), hi0 = text_of(opval(hi));
    E = "LONG(" + hi0 + ", " + lo0 + ") " + op + " LONG(" + Yv + ", " + Xv + ")";
    n = 2;
  }
  binval("mov", lo);
  binval("mov", hi);
  setr(lo, fresh("LO(" + E + ")"));
  setr(hi, fresh("HI(" + E + ")"));
  if (lo == "ax" || hi == "ax") axsrc = NI, retlive = true;
  if (lo == "bx" || hi == "bx") bxsrc = NI;
  return n;
}

std::vector<string> Tracker::take_args(int i0, const string &kind, const string &target) {
  long long n = LLONG_MIN;
  bool pascal = false;
  if (C.hooks.callee_registers && out) {
    std::vector<string> rr;
    const bool st = C.hooks.callee_registers(ins[i0].address, kind, target, rr);
    if (st && !rr.empty()) {
      for (auto &r : rr) {
        Value v = getr(r);
        if (!v.empty() && v.s() != r) materialize(out, r);
        if (in(r, {"ax", "bx", "cx", "dx"}))
          for (string b8 : {string(1, r[0]) + "l", string(1, r[0]) + "h"})
            if (breg.has(b8)) {
              Value bv = breg.pop(b8);
              if (text_of(bv) != b8) push_out(out, b8 + " = " + text_of(bv) + ";", i0);
            }
        setr(r, Value());
      }
      push_out(out, "USE(" + join(rr, ", ") + ");", i0);
    }
  }
  if (C.hooks.callee_pop_bytes) {
    long long rn = 0;
    const bool st = C.hooks.callee_pop_bytes(ins[i0].address, kind, target, rn);
    if (st && rn) pascal = true, n = floor_quotient(rn, 2);
  }
  if (n == LLONG_MIN && i0 + 1 < N()) {
    const platforms::Insn &nx = ins[i0 + 1];
    if (nx.mnemonic == "add" && mt("sp, (0x[0-9a-f]+|\\d+)$", nx.op_str)) n = floor_quotient(num(slice(nx.op_str, 4)), 2);
    else if (C.dead_registers && nx.mnemonic == "jmp" && tget(i0 + 1) != NI) {
      const platforms::Insn &tx = I(tat(i0 + 1));
      if (tx.mnemonic == "add" && mt("sp, (0x[0-9a-f]+|\\d+)$", tx.op_str)) n = floor_quotient(num(slice(tx.op_str, 4)), 2);
    }
  }
  const platforms::Insn *nx_ = i0 + 1 < N() ? &ins[i0 + 1] : nullptr;
  if (nx_ && nx_->mnemonic == "jmp" && tget(i0 + 1) != NI) nx_ = &I(tat(i0 + 1));
  if (n == LLONG_MIN && !pascal && msc3 && nx_ && nx_->mnemonic == "push" && in(nx_->op_str, {"ax", "dx"})) n = 0;
  if (n == LLONG_MIN && !pascal && C.caller_cleans) n = 0;
  std::vector<Value> take;
  if (n == LLONG_MIN || n > (long long)args.size()) {
    take = args;
    args.clear();
  } else {
    // the last n arguments (n may be negative only through a negative clean-up: the slice clamps)
    long long L = (long long)args.size(), cut = L - n;
    if (cut < 0) cut = 0;
    if (cut > L) cut = L;
    take.assign(args.begin() + cut, args.end());
    args.erase(args.begin() + cut, args.end());
  }
  std::vector<string> o;
  if (pascal)
    for (auto &a : take) o.push_back(text_of(a));
  else
    for (auto it = take.rbegin(); it != take.rend(); ++it) o.push_back(text_of(*it));
  return o;
}

string Tracker::axrw(const string &mn, const string &ops) {
  static const Rx AXR("\\b(ax|al|ah)\\b");
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

bool Tracker::retval() {
  if (rv < 0 && C.returns_value >= 0) rv = C.returns_value;
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

void Tracker::ax_fate(int i0, std::vector<int> &uses, bool &ret, bool &sf) {
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
    const platforms::Insn &f = ins[first];
    string d = split(f.op_str, ", ")[0];
    sf = f.mnemonic == "mov" && has(d, "[") && ends(f.op_str, ", ax");
  }
}

void Tracker::call_result(int i0, const List &o, const string &expr) {
  for (size_t k = 0; k < args.size(); k++)
    if (sr("\\b([abcd][xlh])\\b", text_of(args[k]))) {
      string t = newt();
      emit(o, t + " = " + text_of(args[k]) + ";");
      args[k] = fresh(t);
    }
  for (int r : {SI, DI}) {
    Value v = rg[r];
    if (!v.empty() && v.s() != WRN[r] && !mt("^&|^-?\\d+$", v.s()) && live(WRN[r], i0)) materialize(o, WRN[r]);
  }
  for (int r = 0; r < 6; r++) {
    Value v = rg[r];
    if (!v.empty() && v.s() != WRN[r] && hascall(v) && !has(expr, v.s())) materialize(o, WRN[r]);
  }
  rg[BX] = rg[CX] = rg[DX] = Value();
  bxsrc = NI;
  for (const char *b : {"bl", "bh", "cl", "ch", "dl", "dh", "al", "ah"}) breg.pop0(b);
  std::vector<int> uses;
  bool ret, sf;
  ax_fate(i0, uses, ret, sf);
  auto pinax = [&]() {
    emit(o, "ax = " + expr + ";");
    rg[AX] = interned("ax");
    retlive = true;
  };
  if (callstmt || (uses.empty() && !ret)) {
    emit(o, expr + ";");
    rg[AX] = Value();
    retlive = false;
  } else if (live("dx", i0)) pinax();
  else if (!uses.empty() && ret && C.caller_cleans) pinax();
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
    rg[AX] = fresh(expr);
    retlive = true;
  } else pinax();
  axsrc = NI;
}

bool Tracker::read_before_write(const string &reg, int i0) {
  std::vector<string> names = {reg};
  if (in(reg, {"ax", "bx", "cx", "dx"})) names.push_back(string(1, reg[0]) + "l"), names.push_back(string(1, reg[0]) + "h");
  auto rd = [&](const string &n, const string &t) { return (bool)sr("\\b" + n + "\\b", t); };
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

int Tracker::step(int i0, const List &o) {
  int n = step0(i0, o);
  if (C.eager_register_variables && msc3 && LAST.has_saved) {
    for (auto &r : std::vector<string>(LAST.saved)) {
      Value v = getr(r);
      std::vector<string> own = {r};
      if (LAST.has_register_params)
        for (auto &kv : LAST.register_params)
          if (kv.first == r) {
            own.push_back(pname(4 + 2 * kv.second));
            break;
          }
      if (!v.empty() && v.s() != r && !mt("-?\\d+$|&|[A-Za-z_]\\w*$", v.s()) && !hascall(v)) {
        bool any = false;
        for (auto &ow : own)
          if (sr("\\b" + ow + "\\b", v.s())) any = true;
        if (any) materialize(o, r);
      }
    }
  }
  return n;
}

}  // namespace cb::invert::early_c_model
