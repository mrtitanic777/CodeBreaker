// cb-unity: alone
// The walker of the Microsoft C 2.03 / 3.00 model: instruction by instruction (recover) and block by block over the structured control-flow
// graph (recover_cfg), around the Tracker's register values.
#include <algorithm>
#include <climits>

#include "invert/early_c_tracker.hpp"
#include "invert/flow16.hpp"

namespace cb::invert::early_c_model {


std::string carry_kind(const std::vector<platforms::Insn> &ins, long long k) {
  if (k < 1 || k >= (long long)ins.size()) return "";
  const platforms::Insn &c = ins[k], &p = ins[k - 1];
  auto memparts = [](const string &op, string &w, string &b, long long &off) -> bool {
    auto m = mt("(word|byte) ptr (?:\\w\\w:)?\\[(.*)\\]$", strip(op));
    if (!m) return false;
    string inner = group_of(m, 2);
    auto m2 = mt("(.*?)(?: ([+-]) )?(0x[0-9a-f]+|\\d+)$", inner);
    w = group_of(m, 1);
    if (m2 && (group_of(m2, 1).empty() || m2.has(2))) {
      off = parse_number(group_of(m2, 3));
      b = group_of(m2, 1);
      if (m2.has(2) && group_of(m2, 2) == "-") off = -off;
      return true;
    }
    b = inner;
    off = 0;
    return true;
  };
  if (in(c.mnemonic, {"rcl", "rcr"})) {
    auto cp = partition(c.op_str, ", "), pp = partition(p.op_str, ", ");
    bool mem = false;
    if (wr(cp.first) < 0 || wr(pp.first) < 0) {
      string aw, ab, bw, bb;
      long long ao = 0, bo = 0;
      bool a_ = memparts(pp.first, aw, ab, ao), b_ = memparts(cp.first, bw, bb, bo);
      // lo_, hi_ = (a_, b_) if rcl else (b_, a_)
      long long lo_o = c.mnemonic == "rcl" ? ao : bo, hi_o = c.mnemonic == "rcl" ? bo : ao;
      mem = a_ && b_ && aw == bw && bw == "word" && ab == bb && hi_o - lo_o == 2;
    }
    if (cp.second != "1" || pp.second != "1" || (!mem && (wr(cp.first) < 0 || wr(pp.first) < 0)) || cp.first == pp.first)
      return "";
    if (c.mnemonic == "rcl" && in(p.mnemonic, {"shl", "sal"})) return "shl1";
    if (c.mnemonic == "rcr" && in(p.mnemonic, {"shr", "sar"})) return p.mnemonic == "shr" ? "shr1" : "sar1";
    return "";
  }
  if (!in(c.mnemonic, {"adc", "sbb"}) || !has(c.op_str, ", ") || !has(p.op_str, ", ")) {
    if (!(in(c.mnemonic, {"adc", "sbb"}) && p.mnemonic == "neg")) return "";
  }
  auto cp = partition(c.op_str, ", "), pp = partition(p.op_str, ", ");
  const string &cd = cp.first, &cs = cp.second, &pd = pp.first;
  if (c.mnemonic == "sbb" && cd == cs && wr(cd) >= 0 && p.mnemonic == "cmp") return "setcc";
  if (c.mnemonic == "adc" && cs == "0" && p.mnemonic == "neg" && wr(p.op_str) >= 0 && wr(cd) >= 0 && cd != p.op_str &&
      k + 1 < (long long)ins.size() && ins[k + 1].mnemonic == "neg" && ins[k + 1].op_str == cd)
    return "neg";
  if (!((p.mnemonic == "add" && c.mnemonic == "adc") || (p.mnemonic == "sub" && c.mnemonic == "sbb"))) return "";
  if (wr(pd) >= 0 && wr(cd) >= 0 && pd != cd) return "pair";
  string aw, ab, bw, bb;
  long long ao = 0, bo = 0;
  bool a = memparts(pd, aw, ab, ao), b = memparts(cd, bw, bb, bo);
  if (a && b && aw == bw && bw == "word" && ab == bb && bo - ao == 2) return "pair";
  return "";
}

int Tracker::struct_copy(int i0, const List &o) {
  auto mo = [&](long long k) { return std::make_pair(ins[k].mnemonic, ins[k].op_str); };
  typedef std::pair<string, string> P;
  std::vector<P> seq;
  for (long long k = i0; k < std::min<long long>(i0 + 4, N()); k++) seq.push_back(mo(k));
  auto head2 = [&](const std::vector<P> &v, P a, P b) { return v.size() >= 2 && v[0] == a && v[1] == b; };
  bool bare = head2(seq, {"push", "ds"}, {"pop", "es"}) && seq.size() >= 2 && saved_has("si") && saved_has("di");
  // (seq[:2] == [..]: a shorter seq is not equal)
  OD given;
  int ii = i0, iorig = i0;
  const std::vector<P> PLAIN = {{"push", "si"}, {"push", "di"}, {"push", "ds"}, {"pop", "es"}};
  if (head2(seq, {"push", "si"}, {"push", "di"}) && !bare) {
    long long k = ii + 2;
    if (k < N() && ins[k].mnemonic == "xchg" && in(ins[k].op_str, {"di, si", "si, di"})) {
      Value vs = or_else(rg[SI], "si"), vd = or_else(rg[DI], "di");
      given = OD();
      given.set("di", fresh(text_of(vs)));
      given.set("si", fresh(text_of(vd)));
      k++;
    }
    while (k < ii + 4 && k < N() && in(ins[k].mnemonic, {"mov", "lea"}) && mt("(di|si), ", ins[k].op_str)) {
      auto pr = split_pair(ins[k].op_str, ", ");
      const string &r_ = pr.first, &src = pr.second;
      Value v_;
      if (ins[k].mnemonic == "lea") {
        v_ = memname(src);
        v_ = truthy(v_) ? fresh("&" + v_.s()) : Value();
      } else if (src == "si" || src == "di") {
        v_ = given.has(src) ? given.get(src) : or_else(getr(src), src.c_str());
      } else if (in(src, {"ax", "bx", "cx", "dx"})) {
        v_ = val(src);
      } else if (fm("0x[0-9a-f]+|\\d+", src)) {
        long long nv = num(src);
        if (nv > 0) {
          string h = hex(nv);
          while (h.size() < 4) h = "0" + h;
          v_ = fresh("&g_" + h);
        } else {
          v_ = from_int(nv);
        }
      } else {
        v_ = memname(src);
      }
      if (v_.empty()) break;
      given.set(r_, fresh(text_of(v_)));
      k++;
      if (k < N() && ins[k].mnemonic == "xchg" && in(ins[k].op_str, {"di, si", "si, di"}) && given.v.size() == 2) {
        Value gs = given["si"], gd = given["di"];
        given = OD();
        given.set("di", gs);
        given.set("si", gd);
        k++;
      }
    }
    bool pp = k + 1 < N() && mo(k) == P("push", "ds") && mo(k + 1) == P("pop", "es");
    if (!given.empty() && pp) {
      seq = PLAIN;
      iorig = ii;
      ii = (int)k - 2;
    } else if (!given.empty() && k < N() &&
               (in(ins[k].mnemonic, {"movsw", "movsb"}) ||
                (ins[k].mnemonic == "mov" && starts(ins[k].op_str, "cx, ") && k + 1 < N() && starts(ins[k + 1].mnemonic, "rep"))) &&
               [&]() {
                 for (long long q = std::max(0, ii - 24); q < ii; q++)
                   if (mo(q) == P("pop", "es")) return true;
                 return false;
               }()) {
      seq = PLAIN;
      iorig = ii;
      ii = (int)k - 4;
    } else {
      given = OD();
    }
  }
  if (seq != PLAIN && !bare) return 0;
  long long j = ii + (bare ? 2 : 4), n = 0;
  if (j + 1 < N() && ins[j].mnemonic == "mov" && mt("cx, (0x[0-9a-f]+|\\d+)$", ins[j].op_str) &&
      starts(ins[j + 1].mnemonic, "rep") && has(ins[j + 1].mnemonic + ins[j + 1].op_str, "movs")) {
    string t = at(split(ins[j + 1].mnemonic + ins[j + 1].op_str, "movs"), 1);
    n = num(at(split(ins[j].op_str, ", "), 1)) * (has(slice(t, 0, 1), "w") ? 2 : 1);
    j += 2;
  } else {
    while (j < N() && in(ins[j].mnemonic, {"movsw", "movsb"})) {
      n += ins[j].mnemonic == "movsw" ? 2 : 1;
      j++;
    }
  }
  if (!n || (!bare && (j + 1 >= N() || mo(j) != P("pop", "di") || mo(j + 1) != P("pop", "si")))) return 0;
  Value d_ = !rg[DI].empty() ? rg[DI] : interned("di"), s_ = !rg[SI].empty() ? rg[SI] : interned("si");
  string ns = decimal(n);
  if (!given.empty()) {
    if (given.has("di")) d_ = given.get("di");
    if (given.has("si")) s_ = given.get("si");
    string ds = text_of(d_), ss = text_of(s_);
    if (!fm("[\\w\\->.&]+", ds)) ds = "(" + ds + ")";
    if (!fm("[\\w\\->.&]+", ss)) ss = "(" + ss + ")";
    emit(o, "*(struct _b" + ns + " *)" + ds + " = *(struct _b" + ns + " *)" + ss + ";");
    return (int)(j + 2 - iorig);
  }
  if (bare) {
    for (int rep = 0; rep < 2; rep++) {
      if (o->empty()) break;
      auto mk = fm("(si|di) = (-?\\d+|[\\w\\->.]+);", strip(o->back()));
      if (!mk || !in(group_of(mk, 1), {"si", "di"}) || (group_of(mk, 1) == "si" && text_of(s_) != "si") || (group_of(mk, 1) == "di" && text_of(d_) != "di"))
        break;
      string v_ = group_of(mk, 2);
      if (fm("-?\\d+", v_) && (parse_number(v_) & 0xffff) >= 0x100) v_ = "&g_" + hex(parse_number(v_) & 0xffff);
      if (group_of(mk, 1) == "si") s_ = fresh(v_);
      else d_ = fresh(v_);
      o->pop_back();
      if (ranks.size() > o->size()) ranks.pop_back(), regconst.pop_back();
    }
  }
  emit(o, "*(struct _b" + ns + " *)" + text_of(d_) + " = *(struct _b" + ns + " *)" + text_of(s_) + ";");
  if (bare) rg[SI] = rg[DI] = Value();
  return (int)(j + (bare ? 0 : 2) - ii);
}

int Tracker::arg_copy(int i0) {
  if (i0 + 5 >= N() || ins[i0].mnemonic != "sub" || !fm("sp, (0x[0-9a-f]+|\\d+)", ins[i0].op_str)) return 0;
  const platforms::Insn &m = ins[i0 + 1];
  if (!in(m.mnemonic, {"mov", "lea"}) || !starts(m.op_str, "si, ")) return 0;
  if (!(ins[i0 + 2].mnemonic == "mov" && ins[i0 + 2].op_str == "di, sp" && ins[i0 + 3].mnemonic == "push" &&
        ins[i0 + 3].op_str == "ss" && ins[i0 + 4].mnemonic == "pop" && ins[i0 + 4].op_str == "es"))
    return 0;
  long long n = num(at(split(ins[i0].op_str, ", "), 1));
  long long j = i0 + 5, k = 0;
  while (j < N() && in(ins[j].mnemonic, {"movsw", "movsb"})) {
    k += ins[j].mnemonic == "movsw" ? 2 : 1;
    j++;
  }
  if (k != n) return 0;
  string src = slice(m.op_str, 4);
  string v;
  if (m.mnemonic == "lea") {
    Value nm = memname(src);
    if (!(truthy(nm) && fm("\\w+", nm.s()))) return 0;
    v = "&" + nm.s();
  } else {
    Value x = wr(src) >= 0 ? val(src) : or_else(memname(src), src.c_str());
    v = fm("[\\w\\->.]+", text_of(x)) ? text_of(x) : "(" + text_of(x) + ")";
  }
  args.push_back(fresh(n == 8 ? "*(double *)" + v : "*(struct _b" + decimal(n) + " *)" + v));
  return (int)(j - i0);
}

bool Tracker::var_later(const string &d) {
  long long k = i + 1;
  while (k < N() && in(ins[k].mnemonic, {"shl", "sal", "add"}) && split(ins[k].op_str, ", ")[0] == d) k++;
  for (long long q = std::max(0LL, k); q < N(); q++) {
    const platforms::Insn &x = ins[q];
    auto o = split(x.op_str, ", ");
    if (in(x.mnemonic, {"mov", "lea", "lds", "les"}) && o[0] == d && !sr("\\b" + d + "\\b", split_at_most(x.op_str, ", ", 1).back()))
      return false;
    if (in(x.mnemonic, {"inc", "dec", "neg", "not"}) && x.op_str == d) return true;
    if (in(x.mnemonic, {"add", "sub", "shl", "shr", "sar", "and", "or", "xor", "adc", "sbb"}) && o[0] == d) return true;
    if (in(x.mnemonic, {"cmp", "test"}) && std::find(o.begin(), o.end(), d) != o.end()) return true;
    if (x.mnemonic == "mov" && o.size() == 2 && o[1] == d && o[0] != "bx") return true;
    if (x.mnemonic == "push" && x.op_str == d && q != N() - 1) return true;
  }
  return false;
}

int Tracker::fp_arg(int i0) {
  if (i0 + 2 >= N() || ins[i0].mnemonic != "sub" || fst.empty()) return 0;
  auto m = fm("sp, (ax|0x8|8|4)", ins[i0].op_str);
  if (!m || (group_of(m, 1) == "ax" && !in(text_of(rg[AX]), {"8", "4"}))) return 0;
  if (!(ins[i0 + 1].mnemonic == "mov" && ins[i0 + 1].op_str == "bx, sp")) return 0;
  long long j = i0 + 2;
  while (j < N() && in(ins[j].mnemonic, {"wait", "fwait", "nop"})) j++;
  Hit f;
  if (j < N()) f = fm("(qword|dword) ptr \\[bx\\]", ins[j].op_str);
  if (!f || ins[j].mnemonic != "fstp") return 0;
  string v = fst.back();
  fst.pop_back();
  args.push_back(fresh(group_of(f, 1) == "qword" ? "(double)" + v : "(float)" + v));
  if (group_of(m, 1) == "ax") rg[AX] = Value();
  return (int)(j + 1 - i0);
}

int Tracker::scratch_index(int i0) {
  if (i0 + 2 >= N() || ins[i0].mnemonic != "mov") return 0;
  auto m = fm("(si|di), (ax|bx|cx|dx|si|di)", ins[i0].op_str);
  if (!m || group_of(m, 1) == group_of(m, 2) || !(ins[i0 + 1].mnemonic == "shl" && ins[i0 + 1].op_str == group_of(m, 1) + ", 1")) return 0;
  string r = group_of(m, 1);
  long long k = i0 + 2;
  if (!has(ins[k].op_str, "[bx + " + r + "]")) return 0;
  for (long long q = k + 1; q < N(); q++) {
    const platforms::Insn &x = ins[q];
    if (cjmp(x.mnemonic) || in(x.mnemonic, {"jmp", "ret", "retf", "call"})) break;
    if (sr("\\b" + r + "\\b", x.op_str)) {
      if (x.mnemonic == "mov" && starts(x.op_str, (r + ", ").c_str()) && !sr("\\b" + r + "\\b", slice(x.op_str, 4))) break;
      return 0;
    }
  }
  string v = text_of(val(group_of(m, 2)));
  setr(r, fresh("(" + (fm("[\\w\\->.]+", v) ? v : "(" + v + ")") + " << 1)"));
  return 2;
}

int Tracker::step0(int i0, const List &o) {
  if (msc3) {
    int n = struct_copy(i0, o);
    if (n) return i0 + n;
    n = arg_copy(i0);
    if (n) return i0 + n;
    n = scratch_index(i0);
    if (n) return i0 + n;
    n = fp_arg(i0);
    if (n) return i0 + n;
  }
  i = i0;
  out = o;
  if (hoisted.count(i0)) return i0 + 1;
  if (C.mark) {
    int n = carry(i0, o);
    if (n) return i0 + n;
  }
  int n = C.dead_registers ? 0 : long_pat(i0);
  if (n) {
    retlive = true;
    return i0 + n;
  }
  n = long_stmt(i0, o);
  if (n) return i0 + n;
  n = carry(i0, o);
  if (n) return i0 + n;
  const platforms::Insn &ii = I(i0);
  const string mn = ii.mnemonic, ops = ii.op_str;
  string d0 = strip(split(ops, ",")[0]);
  if (in(mn, {"wait", "fwait"}) || (starts(mn, "f") && fpop(mn))) {
    int k = fp_step(i0, o);
    if (k) return k;
  }
  if (in(mn, {"cmp", "test"}) && count_of(ops, ", ") == 1) {
    auto ab = split(ops, ", ");
    try {
      Value x = opval(ab[0]);
      Value y = opval(ab[1]);
      string w = (has(ops, "byte ptr") || isbyte(ab[0])) ? "S8" : "S16";
      if (mn == "cmp") lastcmp = LC{x, y, w};
      else lastcmp = LC{ab[0] == ab[1] ? x : fresh("(" + text_of(x) + " & " + text_of(y) + ")"), interned("0"), w};
      has_lastcmp = true;
      blockcmp = i0;
    } catch (const FormatError &) {
    }
  }
  if (in(mn, {"or", "and", "test"}) && count_of(ops, ",") == 1 && d0 == strip(split(ops, ",")[1]) && !has(ops, "[")) return i0 + 1;
  kill_aliases(i0);
  if (C.mark && C.far_pointers) {
    if (mn == "les") {
      auto pr = partition(ops, ", ");
      Value seg = memname(plus2(pr.second));
      string sg = seg.empty() ? "es" : seg.s();
      if (has(pr.second, "es:")) {
        string t = newt();
        emit(o, t + " = " + sg + ";");
        expr_step(i0);
        if (wr(pr.first) >= 0 && !getr(pr.first).empty()) materialize(o, pr.first);
        emit(o, "es = " + t + ";");
        return i0 + 1;
      }
      emit(o, "es = " + sg + ";");
    } else if (mn == "mov" && starts(ops, "es, ")) {
      string r_ = strip(slice(ops, 4));
      Value v = wr(r_) >= 0 ? getr(r_) : Value();
      emit(o, "es = " + (!v.empty() ? v.s() : text_of(or_else(memname(r_), r_.c_str()))) + ";");
      return i0 + 1;
    } else if (mn == "pop" && ops == "es") {
      Value v;
      if (!args.empty()) v = args.back(), args.pop_back();
      else v = interned("es");
      if (text_of(v) != "es") emit(o, "es = " + text_of(v) + ";");
      return i0 + 1;
    }
  }
  if (byte_on_word(mn, ops)) return i0 + 1;
  for (auto &t_ : RX("\\b[abcd][lh]\\b").find_all(ops))
    if (!breg.has(t_) && hascall(or_else(getr(parent(t_)), ""))) materialize(o, parent(t_));
  if (isbyte(d0)) p8_on = true, p8_i = i0, p8_v = bval(d0);
  else p8_on = false;
  {
    auto o2 = split_at_most(ops, ", ", 1);
    if (mn == "mov" && isbyte(d0) && o2.size() == 2 && has(o2[1], "[")) sm_on = true, sm_i = i0, sm_v = memname(o2[1]);
    else sm_on = false;
  }
  if (isbyte(d0)) pw_on = true, pw_i = i0, pw_v = getr(parent(d0));
  else pw_on = false;
  {
    auto o_ = split(ops, ", ");
    if (mn == "mov" && o_.size() == 2 && isbyte(o_[0]) && isbyte(o_[1])) s8_on = true, s8_i = i0, s8_v = bval(o_[1]);
    else s8_on = false;
  }
  binval(mn, d0);
  if (mn == "mov" && has(d0, "[") && isbyte(rpartition(ops, ", ").second)) {
    string src = rpartition(ops, ", ").second;
    if (!breg.has(src) && hascall(or_else(getr(parent(src)), ""))) materialize(o, parent(src));
    Value dn = memname(has(d0, "byte ptr") ? d0 : "byte ptr " + d0);
    emit(o, text_of(dn) + " = " + text_of(bval(src)) + ";");
    return i0 + 1;
  }
  if (mn == "cmp" && starts(ops, "ax, ") && i0 + 2 < N() && isjcc(ins[i0 + 1].mnemonic) && ins[i0 + 2].mnemonic == "xor") {
    rg[AX] = fresh("(" + text_of(rg[AX]) + " " + jcc_get(ins[i0 + 1].mnemonic) + " " + text_of(memname(slice(ops, 4))) + ")");
    retlive = true;
    axsrc = NI;
    int q = i0;
    while (q < N() && I(q).op_str != "ax, bx") q++;
    return q + 1;
  }
  if (mn == "pop" && C.dead_registers && wr(ops) >= 0 && !args.empty()) {
    string v = text_of(args.back());
    args.pop_back();
    if (!getr(ops).empty() && getr(ops).s() != ops) setr(ops, Value());
    if (in(ops, {"ax", "bx", "cx", "dx"})) breg.pop0(string(1, ops[0]) + "l"), breg.pop0(string(1, ops[0]) + "h");
    if (v != ops) emit(o, ops + " = " + v + ";");
    setr(ops, Value());
    if (ops == "ax") axsrc = NI, retlive = true;
    return i0 + 1;
  }
  if (mn == "push") {
    if (ops == "cs" && i0 + 1 < N() && ins[i0 + 1].mnemonic == "call") return i0 + 1;
    if (ops == "ax" && rg[AX].empty() && msc3 && breg.has("al") && text_of(breg.get("ah")) == "0") args.push_back(fresh(text_of(breg["al"])));
    else if (ops == "ax") args.push_back(!rg[AX].empty() ? rg[AX] : interned("ax"));
    else if (in(ops, {"bx", "cx", "dx", "si", "di"})) {
      Value v = getr(ops);
      args.push_back(!v.empty() ? v : fresh(ops));
    } else args.push_back(or_else(memname(ops), ops.c_str()));
    return i0 + 1;
  }
  if (mn == "call") {
    auto m = mt(NUMRX, ops);
    string a = join(take_args(i0, "near", m ? decimal(num(group_of(m, 1))) : ""), ", ");
    if (a.empty() && !fst.empty()) a = fst.back(), fst.pop_back();
    if (a.empty() && msc3 && i0 >= 3) {
      std::vector<std::pair<string, string>> pre;
      for (int q = i0 - 3; q < i0; q++) pre.push_back({ins[q].mnemonic, ins[q].op_str});
      auto &last = pre.back();
      std::set<string> heads;
      for (int q = 0; q < 2; q++)
        if (pre[q].first == "mov") heads.insert(split(pre[q].second, ",")[0]);
      if (last.first == "mov" && (starts(last.second, "cl, ") || starts(last.second, "cx, ")) && heads == std::set<string>{"ax", "dx"}) {
        string lo_ = text_of(!rg[AX].empty() ? rg[AX] : interned("ax")), hi_ = text_of(!rg[DX].empty() ? rg[DX] : interned("dx"));
        string c1 = at(split(last.second, ", "), 1);
        a = "LONG(" + hi_ + ", " + lo_ + "), " + (is_number(c1) ? decimal(num(c1)) : "cl");
        rg[AX] = Value();
        rg[DX] = Value();
      } else if (last.first == "mov" && starts(last.second, "cl, ") && [&]() {
                   for (int q = 0; q < 2; q++)
                     if (in(pre[q].first, {"cdq", "cwd"}) ||
                         (pre[q].first == "mov" && (starts(pre[q].second, "ax, ") || starts(pre[q].second, "dx, "))))
                       return true;
                   return false;
                 }() && [&]() {
                   for (int q = 0; q < 2; q++)
                     if (!(in(pre[q].first, {"mov", "cdq", "cwd"}) && !starts(pre[q].second, "si,") && !starts(pre[q].second, "di,")))
                       return false;
                   return true;
                 }()) {
        string src_ = at(split_at_most(last.second, ", ", 1), 1);
        string cnt = is_number(src_) ? decimal(num(src_)) : (has(src_, "[") ? text_of(memname(src_)) : "cl");
        string lo_ = text_of(!rg[AX].empty() ? rg[AX] : interned("ax"));
        bool cd = false;
        for (int q = 0; q < 2; q++)
          if (in(pre[q].first, {"cdq", "cwd"})) cd = true;
        if (cd) a = "(long)" + lo_ + ", " + cnt;
        else a = "LONG(" + text_of(!rg[DX].empty() ? rg[DX] : interned("dx")) + ", " + lo_ + "), " + cnt;
        rg[AX] = Value();
        rg[DX] = Value();
      }
    }
    string expr;
    if (m) expr = "sub_" + hex(num(group_of(m, 1))) + "(" + a + ")";
    else {
      string fp = text_of(or_else(memname(ops), ops.c_str()));
      if (C.dead_registers && wr(ops) >= 0) {
        Value v_ = getr(ops);
        if (!v_.empty()) fp = v_.s();
      }
      expr = "(*" + fp + ")(" + a + ")";
    }
    call_result(i0, o, expr);
    return i0 + 1;
  }
  if (mn == "lcall") {
    auto mm = mt("(0x[0-9a-f]+|\\d+),\\s*(0x[0-9a-f]+|\\d+)$", ops);
    string a = join(take_args(i0, "far", mm ? decimal(num(group_of(mm, 1))) + "," + decimal(num(group_of(mm, 2))) : ""), ", ");
    string expr;
    if (mm) expr = "sub_" + hex(num(group_of(mm, 1)) * 16 + num(group_of(mm, 2))) + "(" + a + ")";
    else expr = "(*" + text_of(or_else(memname(ops), ops.c_str())) + ")(" + a + ")";
    call_result(i0, o, expr);
    return i0 + 1;
  }
  if (mn == "mov" && ops == "sp, bp") return i0 + 1;
  if (mn == "mov" && has(split(ops, ", ")[0], "[")) {
    auto pr = rpartition(ops, ", ");
    const string dest = pr.first, src = pr.second;
    if (src == "ax") {
      long long rank = (axsrc != NI && axsrc < i0) ? axsrc : i0;
      bool rc = axsrc != NI && (bool)mt("-?\\d+$|-?0x[0-9a-f]+$", text_of(rg[AX]));
      Value dn = memname(dest);
      emit(o, text_of(dn) + " = " + text_of(!rg[AX].empty() ? rg[AX] : interned("ax")) + ";", rank, rc);
      if (!rg[AX].empty() && rg[AX].s() != "ax") rg[AX] = memname(dest);
      axsrc = NI;
      retlive = false;
      return i0 + 1;
    }
    if (in(src, {"bx", "cx", "dx", "si", "di"})) {
      long long rank = (src == "bx" && bxsrc != NI && bxsrc < i0) ? bxsrc : i0;
      Value held = !getr(src).empty() ? getr(src) : fresh(src);
      Value dn = memname(dest);
      emit(o, text_of(dn) + " = " + text_of(held) + ";", rank);
      if (text_of(held) != src) setr(src, memname(dest));
      if (src == "bx") bxsrc = NI;
      return i0 + 1;
    }
    if (is_number(src)) {
      Value dn = memname(dest);
      emit(o, text_of(dn) + " = " + decimal(num(src)) + ";");
      return i0 + 1;
    }
    if (in(src, {"ds", "es", "ss", "cs"})) {
      Value dn = memname(dest);
      emit(o, text_of(dn) + " = " + src + ";");
      return i0 + 1;
    }
  }
  if (isalu(mn) && has(split(ops, ", ")[0], "[") && ends(ops, ", ax")) {
    Value d = memname(split(ops, ",")[0]);
    Value av = !rg[AX].empty() ? rg[AX] : interned("ax");
    emit(o, text_of(d) + " = " + text_of(d) + " " + ALU(mn) + " " + text_of(av) + ";");
    return i0 + 1;
  }
  if (isalu(mn) && has(split(ops, ", ")[0], "[")) {
    auto pr = rpartition(ops, ", ");
    const string dst = pr.first, src = pr.second;
    if (is_number(src) || in(src, {"bx", "cx", "dx", "si", "di"})) {
      Value d = memname(dst);
      string held = is_number(src) ? decimal(num(src)) : text_of(val(src));
      emit(o, text_of(d) + " = " + text_of(d) + " " + ALU(mn) + " " + held + ";");
      return i0 + 1;
    }
    if (isbyte(src)) {
      Value d = memname(has(dst, "byte ptr") ? dst : "byte ptr " + dst);
      emit(o, text_of(d) + " = " + text_of(d) + " " + ALU(mn) + " " + text_of(bval(src)) + ";");
      return i0 + 1;
    }
  }
  if (in(mn, {"inc", "dec"}) && ops == "ax") {
    string b = text_of(!rg[AX].empty() ? rg[AX] : interned("ax"));
    rg[AX] = fresh("(" + b + " " + (mn == "inc" ? "+" : "-") + " 1)");
    retlive = true;
    axsrc = NI;
    return i0 + 1;
  }
  if (in(mn, {"inc", "dec"}) && in(ops, {"bx", "cx", "dx", "si", "di"})) {
    Value cur = getr(ops);
    emit(o, ops + " = " + (cur.empty() ? ops : cur.s()) + " " + (mn == "inc" ? "+" : "-") + " 1;");
    setr(ops, Value());
    if (ops == "bx") bxsrc = NI;
    return i0 + 1;
  }
  if (msc3 && in(mn, {"inc", "dec"}) && starts(ops, "word ptr [") && !isbyte(ops)) {
    string nm = text_of(or_else(memname(ops), ops.c_str()));
    string op2 = mn == "inc" ? "++" : "--";
    const platforms::Insn *nx = i0 + 1 < N() ? &ins[i0 + 1] : nullptr;
    if (msc3 && i0 + 1 >= (range_hi >= 0 ? range_hi : N())) nx = nullptr;
    if (nx && nx->mnemonic == "mov" && ends(nx->op_str, ", " + ops) && in(split(nx->op_str, ",")[0], {"bx", "si", "di", "ax", "cx", "dx"})) {
      string r = split(nx->op_str, ",")[0];
      setr(r, fresh(op2 + nm));
      if (r == "bx") bxsrc = NI;
      if (r == "ax") axsrc = NI;
      return i0 + 2;
    }
    for (const char *r : {"bx", "si", "di", "ax", "cx", "dx"}) {
      Value v = getr(r);
      if (!v.empty() && v.s() == nm && read_before_write(r, i0 + 1)) {
        setr(r, fresh(nm + op2));
        if (string(r) == "bx") bxsrc = NI;
        if (string(r) == "ax") axsrc = NI;
        return i0 + 1;
      }
    }
  }
  if (in(mn, {"inc", "dec"}) && !isbyte(ops)) {
    string nm = text_of(or_else(memname(ops), ops.c_str()));
    string op1 = mn == "inc" ? "+" : "-";
    string pn = (mt("^[A-Za-z_]\\w*(->f\\d+)*$", nm) || starts(nm, "BYTE(")) ? nm : "(" + nm + ")";
    emit(o, fs > 0 ? nm + " = " + nm + " " + op1 + " 1;" : pn + (mn == "inc" ? "++" : "--") + ";");
    return i0 + 1;
  }
  if (mn == "int") {
    emit(o, "int_" + replace_all(replace_all(ops, "0x", ""), " ", "") + "();");
    return i0 + 1;
  }
  auto shcount = [&](const char *dflt) -> string {
    if (ends(ops, "cl")) return breg.has("cl") ? text_of(breg["cl"]) : text_of(!rg[CX].empty() ? rg[CX] : interned(dflt));
    return text_of(val(at(split(ops, ", "), 1)));
  };
  if (in(mn, {"shl", "sal", "shr", "sar"}) && has(split(ops, ",")[0], "[")) {
    Value d = memname(split(ops, ",")[0]);
    string cnt = shcount("cx");
    if (in(mn, {"shr", "sar"})) emit(o, text_of(d) + " = " + shr(C, mn, text_of(d), cnt, has(ops, "byte ptr")) + ";");
    else emit(o, text_of(d) + " = " + text_of(d) + " << " + cnt + ";");
    return i0 + 1;
  }
  if (in(mn, {"neg", "not"}) && has(ops, "[")) {
    Value d = memname(ops);
    emit(o, text_of(d) + " = " + (mn == "neg" ? "-" : "~") + text_of(d) + ";");
    return i0 + 1;
  }
  if (isbyte(d0) && in(mn, {"shl", "sal", "shr", "sar"})) {
    string cur = p8_on ? text_of(p8_v) : d0;
    string cnt = shcount("cl");
    if (in(mn, {"shr", "sar"})) breg.set(d0, fresh("((" + shr(C, mn, cur, cnt, true) + ") & 0xff)"));
    else breg.set(d0, fresh("((" + cur + " << " + cnt + ") & 0xff)"));
    return i0 + 1;
  }
  if (isbyte(d0) && in(mn, {"neg", "not"})) {
    string cur = p8_on ? text_of(p8_v) : d0;
    breg.set(d0, fresh("((" + string(mn == "neg" ? "-" : "~") + cur + ") & 0xff)"));
    return i0 + 1;
  }
  if (isalu(mn) && in(d0, {"bx", "cx", "dx", "si", "di"}) && split(ops, ", ").size() == 2) {
    string src = strip(split(ops, ", ")[1]);
    if (src == d0 && in(mn, {"sub", "xor"}) && d0 == "dx" && i0 + 1 < N() && in(ins[i0 + 1].mnemonic, {"div", "idiv"})) {
      rg[DX] = Value();
      return i0 + 1;
    }
    if (src == d0 && in(mn, {"sub", "xor"})) emit(o, d0 + " = 0;");
    else if (src == d0 && mn == "or") {
    } else {
      Value rh = wr(src) >= 0 ? val(src) : rhs(src);
      Value cur = getr(d0);
      emit(o, d0 + " = " + (cur.empty() ? d0 : cur.s()) + " " + ALU(mn) + " " + text_of(rh) + ";");
    }
    setr(d0, Value());
    return i0 + 1;
  }
  if (in(mn, {"rep movsb", "rep movsw", "repne movsb", "repne movsw"})) {
    string n_ = text_of(or_else(rg[CX], "cx"));
    if (ends(mn, "movsw")) n_ = is_digits(n_) ? decimal(parse_number(n_) * 2) : "(" + n_ + " * 2)";
    string dd = text_of(or_else(rg[DI], "di"));
    emit(o, string(C.mark ? (ends(mn, "w") ? "memcpy_w" : "memcpy_b") : "memcpy") + "(" +
                (C.mark && C.far_pointers ? "FAROFF(es, " + dd + ")" : dd) + ", " + text_of(or_else(rg[SI], "si")) + ", " + n_ + ");");
    rg[SI] = rg[DI] = Value();
    rg[CX] = interned("0");
    return i0 + 1;
  }
  if (in(mn, {"rep stosb", "rep stosw", "repne stosb", "repne stosw"})) {
    string n_ = text_of(or_else(rg[CX], "cx"));
    if (ends(mn, "stosw")) n_ = is_digits(n_) ? decimal(parse_number(n_) * 2) : "(" + n_ + " * 2)";
    string dd = text_of(or_else(rg[DI], "di"));
    emit(o, string(C.mark ? (ends(mn, "w") ? "memset_w" : "memset_b") : "memset") + "(" +
                (C.mark && C.far_pointers ? "FAROFF(es, " + dd + ")" : dd) + ", " + text_of(!rg[AX].empty() ? rg[AX] : interned("ax")) + ", " +
                n_ + ");");
    rg[DI] = Value();
    rg[CX] = interned("0");
    return i0 + 1;
  }
  if (mn == "xchg" && has(ops, "[")) {
    std::vector<string> o_;
    for (auto &x : split(ops, ", ")) o_.push_back(strip(x));
    string reg, mem;
    for (auto &x : o_)
      if (reg.empty() && wr(x) >= 0) reg = x;
    for (auto &x : o_)
      if (mem.empty() && has(x, "[")) mem = x;
    if (!reg.empty() && !mem.empty()) {
      Value nm = memname(mem);
      Value held = getr(reg);
      if (held.empty()) held = fresh(reg);
      string t = newt();
      emit(o, t + " = " + text_of(nm) + ";");
      emit(o, text_of(nm) + " = " + text_of(held) + ";");
      setr(reg, fresh(t));
      if (reg == "ax") axsrc = NI, retlive = true;
      if (reg == "bx") bxsrc = NI;
      return i0 + 1;
    }
  }
  if (in(mn, {"movsb", "movsw", "stosb", "stosw", "lodsb", "lodsw", "xlatb"})) {
    int w = ends(mn, "w") ? 2 : 1;
    auto pinreg = [&](const string &r) {
      Value v = getr(r);
      if (!v.empty() && v.s() != r) materialize(o, r);
      setr(r, Value());
    };
    auto settle_ax = [&]() {
      pinreg("ax");
      for (const char *b8 : {"al", "ah"})
        if (breg.has(b8)) {
          Value v = breg.pop(b8);
          if (text_of(v) != b8) emit(o, string(b8) + " = " + text_of(v) + ";");
        }
    };
    auto B = [&](const string &x) -> string {
      if (C.mark) return "BYTE(" + x + ")";
      return starts(x, "*") ? "*(char *)" + x.substr(1) : x;
    };
    string DI_ = (C.mark && C.far_pointers) ? "FARM(es, *di)" : "*di";
    string ws = decimal(w);
    string head = mn.substr(0, 4);
    if (head == "movs") {
      pinreg("si");
      pinreg("di");
      emit(o, w == 1 ? B(DI_) + " = " + B("*si") + ";" : DI_ + " = *si;");
      emit(o, "si = si + " + ws + ";");
      emit(o, "di = di + " + ws + ";");
    } else if (head == "stos") {
      pinreg("di");
      settle_ax();
      emit(o, w == 1 ? B(DI_) + " = al;" : DI_ + " = ax;");
      emit(o, "di = di + " + ws + ";");
    } else if (head == "lods") {
      pinreg("si");
      settle_ax();
      emit(o, w == 1 ? "al = " + B("*si") + ";" : string("ax = *si;"));
      emit(o, "si = si + " + ws + ";");
      axsrc = NI;
      retlive = true;
    } else {
      pinreg("bx");
      settle_ax();
      emit(o, "al = " + B("*(bx + al)") + ";");
      axsrc = NI;
      retlive = true;
    }
    return i0 + 1;
  }
  if (mn == "jmp") {
    auto m = mt(NUMRX, ops);
    if (m) {
      long long t = num(group_of(m, 1));
      if (t > static_cast<long long>(ins.back().address) || t < static_cast<long long>(ins.front().address)) {
        std::vector<string> a;
        for (auto it = args.rbegin(); it != args.rend(); ++it) a.push_back(text_of(*it));
        emit(o, "return sub_" + hex(t & 0xFFFF) + "(" + join(a, ", ") + ");");
        args.clear();
      } else if (has_epi && t >= epi && !rg[AX].empty() && retlive) {
        if (!rg[DX].empty() && rg[DX].s() != "dx") {
          emit(o, "dx = " + rg[DX].s() + ";");
          rg[DX] = interned("dx");
        }
        emit(o, "return " + text_of(rg[AX]) + ";");
      } else if (!(i0 + 1 < N() && t == static_cast<long long>(ins[i0 + 1].address))) {
        gototargets.insert(t);
        emit(o, "goto L_" + hex(t) + ";");
      }
    }
    return i0 + 1;
  }
  if (mn == "ret" && !rg[AX].empty() && retlive) {
    if (!rg[DX].empty() && rg[DX].s() != "dx") {
      emit(o, "dx = " + rg[DX].s() + ";");
      rg[DX] = interned("dx");
    }
    emit(o, "return " + text_of(rg[AX]) + ";");
    return i0 + 1;
  }
  expr_step(i0);
  return i0 + 1;
}

void Tracker::reset() {
  for (auto &x : rg) x = Value();
  has_div = false;
  div0 = div1 = Value();
  breg = OD();
  maxidx = -1;
  lmax = -1;
  args.clear();
  longfn = floatfn = false;
  hoisted.clear();
  out.reset();
  i = 0;
  retlive = false;
  axsrc = bxsrc = NI;
  ranks.clear();
  regconst.clear();
  locals.clear();
  callstmt = false;
}
Snap Tracker::snap() {
  Snap d;
  for (int k = 0; k < 6; k++) d.r[k] = rg[k];
  d.has_div = has_div, d.div0 = div0, d.div1 = div1;
  d.retlive = retlive, d.axsrc = axsrc, d.bxsrc = bxsrc, d.args = args, d.breg = breg, d.i = i;
  return d;
}
void Tracker::rest(const Snap &d) {
  for (int k = 0; k < 6; k++) rg[k] = d.r[k];
  has_div = d.has_div, div0 = d.div0, div1 = d.div1;
  retlive = d.retlive, axsrc = d.axsrc, bxsrc = d.bxsrc, args = d.args, breg = d.breg, i = d.i;
}

bool Tracker::terminal(const std::vector<string> &body) {
  std::vector<string> lines;
  for (auto &l : body)
    if (!strip(l).empty()) lines.push_back(l);
  if (lines.empty()) return false;
  string last = strip(lines.back());
  if (last != "}") return (bool)mt("(break|continue|return\\b|goto )", last);
  long long d = indent_of(lines.back());
  std::vector<std::vector<string>> arms;
  std::vector<string> cur;
  long long k = (long long)lines.size() - 2;
  bool has_else = false, broke = false;
  while (k >= 0) {
    const string &t = lines[k];
    if (indent_of(t) == d) {
      string st = strip(t);
      if (st == "} else {") {
        arms.push_back(std::vector<string>(cur.rbegin(), cur.rend()));
        cur.clear();
        has_else = true;
      } else if (mt("\\} else if \\(.*\\) \\{$", st)) {
        arms.push_back(std::vector<string>(cur.rbegin(), cur.rend()));
        cur.clear();
      } else if (mt("if \\(.*\\) \\{$", st)) {
        arms.push_back(std::vector<string>(cur.rbegin(), cur.rend()));
        broke = true;
        break;
      } else return false;
    } else if (indent_of(t) < d) return false;
    else cur.push_back(t);
    k--;
  }
  if (!broke) return false;
  if (!has_else) return false;
  for (auto &a : arms)
    if (!terminal(a)) return false;
  return true;
}

void Tracker::phi_args(const std::vector<Value> &a_args, const std::vector<Value> &b_args, std::vector<string> &a_body,
                 std::vector<string> &b_body, bool a_term, bool b_term) {
  const Rx &rr = RX(TOKRX);
  auto one = [&](const std::vector<Value> &as, std::vector<string> &body) {
    std::vector<Value> o;
    for (auto &x : as) {
      if (rr.search(text_of(x))) {
        string t = newt();
        body.push_back(t + " = " + text_of(x) + ";");
        o.push_back(fresh(t));
      } else o.push_back(x);
    }
    args = o;
  };
  if (a_term && !b_term) return one(b_args, b_body);
  if (b_term && !a_term) return one(a_args, a_body);
  if (a_args.size() != b_args.size()) return;
  bool same = true;
  for (size_t k = 0; k < a_args.size(); k++)
    if (!a_args[k].equals(b_args[k])) same = false;
  bool anyr = false;
  for (auto &x : a_args)
    if (rr.search(text_of(x))) anyr = true;
  if (same && !anyr) return;
  std::vector<Value> o;
  for (size_t k = 0; k < a_args.size(); k++) {
    const Value &x = a_args[k], &y = b_args[k];
    if (text_of(x) == text_of(y) && !rr.search(text_of(x))) {
      o.push_back(x);
      continue;
    }
    string t = newt();
    a_body.push_back(t + " = " + text_of(x) + ";");
    b_body.push_back(t + " = " + text_of(y) + ";");
    o.push_back(fresh(t));
  }
  args = o;
}

void Tracker::phi(const Snap &ts, const Snap &es, std::vector<string> &tb, std::vector<string> &eb) {
  bool tterm = terminal(tb), eterm = terminal(eb);
  phi_args(ts.args, es.args, tb, eb, tterm, eterm);
  for (int r : REGS_ORDER) {
    const Value &tv = ts.r[r], &ev = es.r[r];
    string rn = WRN[r];
    if (tv.equals(ev)) {
      rg[r] = tv;
      continue;
    }
    if (!C.dead_registers && !live(rn, i) && !hascall(tv) && !hascall(ev) &&
        !(msc3 && rn == "ax" && retval() && (tv.empty() || ev.empty()))) {
      rg[r] = Value();
      continue;
    }
    if (!tv.empty() && tv.s() != rn && !tterm) tb.push_back(rn + " = " + tv.s() + ";");
    if (!ev.empty() && ev.s() != rn && !eterm) eb.push_back(rn + " = " + ev.s() + ";");
    rg[r] = Value();
  }
  for (const char *r : BYTEREG) {
    Value tv = ts.breg.get(r), ev = es.breg.get(r);
    if (tv.equals(ev)) {
      if (!tv.empty()) breg.set(r, tv);
      continue;
    }
    if (!tv.empty() && tv.s() != r && !tterm) tb.push_back(string(r) + " = " + tv.s() + ";");
    if (!ev.empty() && ev.s() != r && !eterm) eb.push_back(string(r) + " = " + ev.s() + ";");
    breg.pop0(r);
  }
}

bool anytext(const std::vector<string> &b) {
  for (auto &x : b)
    if (!strip(x).empty()) return true;
  return false;
}

List Tracker::body(int lo, int hi) {
  List b = walk(lo, hi);
  if (anytext(*b) || callstmt) return b;
  Value s0[6];
  for (int k = 0; k < 6; k++) s0[k] = rg[k];
  auto a0 = args;
  bool hd = has_div;
  Value d0 = div0, d1 = div1;
  bool rl = retlive;
  int ax0 = axsrc, bx0 = bxsrc;
  List o0 = out;
  OD b0 = breg;
  callstmt = true;
  List b2 = walk(lo, hi);
  callstmt = false;
  for (int k = 0; k < 6; k++) rg[k] = s0[k];
  args = a0, has_div = hd, div0 = d0, div1 = d1, retlive = rl, axsrc = ax0, bxsrc = bx0, out = o0, breg = b0;
  return anytext(*b2) ? b2 : b;
}

List Tracker::walk(int lo, int hi) {
  List o = newlist();
  int i0 = lo;
  auto addb = [&](const List &dst, const List &src) {
    for (auto &b : *src) dst->push_back("    " + b);
  };
  while (i0 < hi) {
    out = o;
    if (emitgoto && gototargets.count(I(i0).address)) o->push_back("L_" + hex(I(i0).address) + ":");
    std::vector<int> backc;
    for (auto &kv : tgt)
      if (kv.second == i0 && cjmp(ins[kv.first].mnemonic) && i0 < kv.first && kv.first < hi) backc.push_back(kv.first);
    if (!backc.empty()) {
      int J = *std::min_element(backc.begin(), backc.end());
      int ci = J - 1;
      while (ci > i0 && !in(I(ci).mnemonic, {"cmp", "test"})) ci--;
      const platforms::Insn &cm = I(ci);
      string jcc = I(J).mnemonic;
      string cond;
      if (cm.mnemonic == "cmp") {
        auto p = split(cm.op_str, ", ");
        auto reg_text = [&](const string &t) -> string {
          if (t == "ax") return text_of(!rg[AX].empty() ? rg[AX] : interned("ax"));
          if (t == "bx") return text_of(!rg[BX].empty() ? rg[BX] : interned("bx"));
          Value m = memname(t);
          return truthy(m) ? m.s() : (is_number(t) ? decimal(num(t)) : t);
        };
        string l = reg_text(p[0]);
        string r = reg_text(at(p, 1));
        const char *jc = jcc_get(jcc);
        cond = r == "0" ? l : "(" + l + " " + (jc ? jc : (jcc == "jne" ? "!=" : "==")) + " " + r + ")";
      } else if (cm.mnemonic == "test" && has(cm.op_str, "[") && (jcc == "jne" || jcc == "je")) {
        auto p = split(cm.op_str, ", ");
        Value v = memname(p[0]);
        string k = at(p, 1);
        string vs = !v.empty() ? v.s() : p[0];
        if (k != p[0]) vs = "(" + vs + " & " + (is_number(k) ? decimal(num(k)) : text_of(or_else(memname(k), k.c_str()))) + ")";
        cond = jcc == "jne" ? vs : "(" + vs + " == 0)";
      } else {
        cond = text_of(!rg[AX].empty() ? rg[AX] : interned("ax"));
      }
      List b = body(i0, ci);
      o->push_back("do {");
      addb(o, b);
      o->push_back("} while (" + cond + ");");
      i0 = J + 1;
      continue;
    }
    std::vector<int> back;
    for (auto &kv : tgt)
      if (kv.second == i0 && ins[kv.first].mnemonic == "jmp" && i0 < kv.first && kv.first < hi) back.push_back(kv.first);
    if (!back.empty()) {
      int J = *std::min_element(back.begin(), back.end());
      auto cj = cond_at(i0);
      tat(cj.second);
      List b = body(cj.second + 1, J);
      o->push_back("while (" + cj.first + ") {");
      addb(o, b);
      o->push_back("}");
      i0 = J + 1;
      continue;
    }
    int tt = tget(i0 + 1);
    if (in(I(i0).mnemonic, {"cmp", "test"}) && i0 + 1 < hi && cjmp(I(i0 + 1).mnemonic) && tt != NI && tt > i0 + 1 &&
        !(i0 + 2 < N() && ins[i0 + 2].mnemonic == "xor")) {
      auto cj = cond_at(i0);
      string cond = cj.first;
      int jcc = cj.second;
      int tg = tget(jcc);
      if (tg != NI && !(jcc + 1 <= tg && tg <= hi)) {
        long long ta = (0 <= tg && tg < N()) ? ins[tg].address : (tg & 0xFFFF);
        gototargets.insert(ta);
        o->push_back("if (!(" + cond + ")) goto L_" + hex(ta) + ";");
        i0 = jcc + 1;
        continue;
      }
      if (tg != NI && jcc + 1 <= tg && tg <= hi) {
        int je = tg - 1;
        int end = (je >= jcc + 1 && I(je).mnemonic == "jmp") ? tget(je) : NI;
        bool end_ok = end != NI && tg < end && end <= hi && (!has_epi || static_cast<long long>(I(end).address) < epi);
        if (end_ok) {
          Snap pre = snap();
          List then_b = body(jcc + 1, je);
          Snap then_s = snap();
          rest(pre);
          List else_b = body(tg, end);
          Snap else_s = snap();
          phi(then_s, else_s, *then_b, *else_b);
          o->push_back("if (" + cond + ") {");
          addb(o, then_b);
          o->push_back("} else {");
          addb(o, else_b);
          o->push_back("}");
          i0 = end;
          continue;
        }
        Snap pre = snap();
        List b = body(jcc + 1, tg);
        Snap then_s = snap();
        for (int r : REGS_ORDER) {
          if (then_s.r[r].equals(pre.r[r])) {
            rg[r] = then_s.r[r];
            continue;
          }
          const Value &pv = pre.r[r], &tv = then_s.r[r];
          if (!pv.empty() && pv.s() != WRN[r]) o->push_back(string(WRN[r]) + " = " + pv.s() + ";");
          if (!tv.empty() && tv.s() != WRN[r]) b->push_back(string(WRN[r]) + " = " + tv.s() + ";");
          rg[r] = Value();
        }
        o->push_back("if (" + cond + ") {");
        addb(o, b);
        o->push_back("}");
        i0 = tg;
        continue;
      }
    }
    i0 = step(i0, o);
  }
  return o;
}

string Tracker::negate(const string &cond) {
  auto m = mt("^\\((.*) (==|!=|<=|>=|<|>) (.*)\\)$", cond);
  if (m) {
    string op = group_of(m, 2), n;
    if (op == "==") n = "!=";
    else if (op == "!=") n = "==";
    else if (op == "<") n = ">=";
    else if (op == ">=") n = "<";
    else if (op == ">") n = "<=";
    else n = ">";
    return "(" + group_of(m, 1) + " " + n + " " + group_of(m, 3) + ")";
  }
  return "!(" + cond + ")";
}

int Tracker::cmp_start(int jcc_idx, int lo) {
  int j = jcc_idx - 1;
  while (j >= lo) {
    const string &mn = I(j).mnemonic;
    string d0 = strip(split(I(j).op_str, ",")[0]);
    if (mn == "cmp" || mn == "test") return j;
    if (in(mn, {"cbw", "cwde", "nop"}) || (in(mn, {"mov", "lea", "les"}) && !has(d0, "[") && (wr(d0) >= 0 || isbyte(d0)))) {
      j--;
      continue;
    }
    break;
  }
  return jcc_idx;
}

void Tracker::emit_range(int lo, int hi, const List &o) {
  int i0 = std::max(lo, emit_start);
  range_hi = hi;
  while (i0 < hi) {
    int ni = step(i0, o);
    i0 = ni > i0 ? ni : i0 + 1;
  }
}

void Tracker::emit_block(const Blk &blk, const List &o) {
  using flow16::End;
  int end = (blk.term == End::Jump || blk.term == End::Branch || blk.term == End::Return || blk.term == End::TailCall) ? blk.hi - 1 : blk.hi;
  emit_range(blk.lo, end, o);
  if (blk.term == flow16::End::Return && retval() && !rg[DX].empty() && rg[DX].s() != "dx") {
    o->push_back("dx = " + rg[DX].s() + ";");
    rg[DX] = interned("dx");
  }
  if (blk.term == flow16::End::Return && !rg[AX].empty() && retlive) o->push_back("return " + rg[AX].s() + ";");
  else if (blk.term == flow16::End::Return && retval()) {
    Value v = !rg[AX].empty() ? rg[AX] : or_else(breg.get("al"), "ax");
    o->push_back("return " + text_of(v) + ";");
  } else if (blk.term == flow16::End::Return) o->push_back("return;");
  else if (blk.term == flow16::End::TailCall) {
    const platforms::Insn &last = I(blk.hi - 1);
    auto m = mt(NUMRX, last.op_str);
    if (m) {
      std::vector<string> a;
      for (auto it = args.rbegin(); it != args.rend(); ++it) a.push_back(text_of(*it));
      o->push_back("return sub_" + hex(parse_number(group_of(m, 1)) & 0xFFFF) + "(" + join(a, ", ") + ");");
      args.clear();
    }
  }
}

string Tracker::emit_cond(const Blk &blk, const List &o, const std::set<string> *flush) {
  int jcc = blk.hi - 1;
  int cs = cmp_start(jcc, blk.lo);
  emit_range(blk.lo, cs, o);
  out = o;
  cond_lo = blk.lo;
  if (flush) pin(o, *flush);
  has_cmp = false;
  return cond_at(cs).first;
}

void Tracker::flush_calls(Snap &sn, const Snap &pre, std::vector<string> &b) {
  if (terminal(b)) return;
  for (int r : REGS_ORDER) {
    const Value v = sn.r[r];
    if (!v.empty() && !v.equals(pre.r[r]) && hascall(v)) {
      b.push_back(string(WRN[r]) + " = " + v.s() + ";");
      sn.r[r] = r == AX ? interned("ax") : Value();
    }
  }
}

bool Tracker::selfref(const string &r, const string &v) {
  std::vector<string> names = {r};
  if (in(r, {"ax", "bx", "cx", "dx"})) names.push_back(string(1, r[0]) + "l"), names.push_back(string(1, r[0]) + "h");
  if (isbyte(r)) names.push_back(parent(r));
  for (auto &n : names)
    if (sr("(?<![\\w>.])" + n + "(?![\\w(])", v)) return true;
  return false;
}

void Tracker::plain_if(string cond, std::vector<string> &b, const Snap &pre, const Snap &then_s, const List &o) {
  bool bterm = terminal(b);
  phi_args(then_s.args, pre.args, b, *o, bterm, false);
  for (int r : REGS_ORDER) {
    string rn = WRN[r];
    if (then_s.r[r].equals(pre.r[r])) {
      rg[r] = then_s.r[r];
      continue;
    }
    if (!C.dead_registers && !live(rn, i) && !hascall(pre.r[r]) && !hascall(then_s.r[r])) {
      rg[r] = Value();
      continue;
    }
    if (!pre.r[r].empty() && pre.r[r].s() != rn) {
      o->push_back(rn + " = " + pre.r[r].s() + ";");
      if ((hascall(pre.r[r]) || selfref(rn, pre.r[r].s())) && has(cond, pre.r[r].s())) cond = replace_all(cond, pre.r[r].s(), rn);
    }
    if (!then_s.r[r].empty() && then_s.r[r].s() != rn && !bterm) b.push_back(rn + " = " + then_s.r[r].s() + ";");
    rg[r] = Value();
  }
  for (const char *r : BYTEREG) {
    Value pv = pre.breg.get(r), tv = then_s.breg.get(r);
    if (pv.equals(tv)) {
      if (!tv.empty()) breg.set(r, tv);
      continue;
    }
    if (!pv.empty() && pv.s() != r) {
      o->push_back(string(r) + " = " + pv.s() + ";");
      if (selfref(r, pv.s()) && has(cond, pv.s())) cond = replace_all(cond, pv.s(), r);
    }
    if (!tv.empty() && tv.s() != r && !bterm) b.push_back(string(r) + " = " + tv.s() + ";");
    breg.pop0(r);
  }
  if (anytext(b)) {
    o->push_back("if (" + cond + ") {");
    for (auto &x : b) o->push_back("    " + x);
    o->push_back("}");
  }
}

void Tracker::emit_seq(const Seq &nodes, const List &o) {
  for (auto &n : nodes) emit_node(n, o);
}

void Tracker::slot_args(const List &o) {
  long long n_ = (long long)args.size();
  std::vector<std::pair<long long, string>> mv;
  for (long long k = 0; k < n_; k++)
    if (text_of(args[k]) != "targ" + decimal(n_ - 1 - k)) mv.push_back({n_ - 1 - k, text_of(args[k])});
  std::vector<long long> done;
  for (auto &kv : mv) {
    string v = kv.second;
    std::vector<long long> hit;
    for (long long d : done)
      if (sr("\\btarg" + decimal(d) + "\\b", v)) hit.push_back(d);
    for (long long d : hit) {
      v = Rx("\\btarg" + decimal(d) + "\\b").replace_all(v, "tsave" + decimal(d));
      long long idx = -1;
      for (long long q = (long long)o->size() - 1; q >= 0; q--)
        if (starts((*o)[q], ("targ" + decimal(d) + " = ").c_str())) {
          idx = q;
          break;
        }
      if (idx < 0) unsupported( "StopIteration");
      string sv = "tsave" + decimal(d) + " = targ" + decimal(d) + ";";
      if (!(idx && (*o)[idx - 1] == sv)) o->insert(o->begin() + idx, sv);
    }
    o->push_back("targ" + decimal(kv.first) + " = " + v + ";");
    done.push_back(kv.first);
  }
  args.clear();
  for (long long k = 0; k < n_; k++) args.push_back(fresh("targ" + decimal(n_ - 1 - k)));
}

void Tracker::emit_node(const Node &node, const List &o) {
  const std::vector<Blk> &B = *blocks;
  auto k = node.kind;
  auto addv = [&](const std::vector<string> &v) {
    for (auto &x : v) o->push_back("    " + x);
  };
  if ((k == Node::Kind::Block || k == Node::Kind::If || k == Node::Kind::IfElse || k == Node::Kind::While || k == Node::Kind::Loop || k == Node::Kind::DoWhile ||
       k == Node::Kind::Switch) &&
      !labels.empty()) {
    long long a = at(B, node.block).addr;
    if (labels.count(a) && !labeled.count(a)) {
      long long na = gotoargs.count(a) ? gotoargs[a] : 0;
      if (terminal(*o)) {
        for (auto &x : rg) x = Value();
        breg = OD();
      } else {
        i = at(B, node.block).lo - 1;
        pin(o, {"ax", "bx", "cx", "dx", "si", "di"});
        if (na && C.dead_registers) slot_args(o);
        else if (na) {
          long long L = (long long)args.size(), from = std::max(0LL, L - na);
          for (long long k2 = from; k2 < L; k2++) o->push_back("targ" + decimal(k2 - from) + " = " + text_of(args[k2]) + ";");
        }
      }
      if (na && C.dead_registers) {
        args.clear();
        for (long long k2 = 0; k2 < na; k2++) args.push_back(fresh("targ" + decimal(na - 1 - k2)));
      } else if (na) {
        std::vector<Value> a2;
        if ((long long)args.size() >= na) a2.assign(args.begin(), args.end() - na);
        for (long long k2 = 0; k2 < na; k2++) a2.push_back(fresh("targ" + decimal(k2)));
        args = a2;
      }
      o->push_back("L_" + hex(a) + ":");
      labeled.insert(a);
    }
  }
  if (k == Node::Kind::Block) {
    emit_block(at(B, node.block), o);
  } else if (k == Node::Kind::If) {
    string cond = emit_cond(at(B, node.block), o);
    if (node.negated) cond = negate(cond);
    Snap pre = snap();
    List b = newlist();
    emit_seq(node.body, b);
    Snap then_s = snap();
    flush_calls(then_s, pre, *b);
    rest(pre);
    plain_if(cond, *b, pre, then_s, o);
  } else if (k == Node::Kind::IfElse) {
    string cond = emit_cond(at(B, node.block), o);
    Snap pre = snap();
    List tb = newlist();
    emit_seq(node.body, tb);
    Snap ts = snap();
    flush_calls(ts, pre, *tb);
    rest(pre);
    List eb = newlist();
    emit_seq(node.else_body, eb);
    Snap es = snap();
    flush_calls(es, pre, *eb);
    phi(ts, es, *tb, *eb);
    bool te = anytext(*tb), ee = anytext(*eb);
    if (te && ee) {
      o->push_back("if (" + cond + ") {");
      addv(*tb);
      o->push_back("} else {");
      addv(*eb);
      o->push_back("}");
    } else if (te) {
      o->push_back("if (" + cond + ") {");
      addv(*tb);
      o->push_back("}");
    } else if (ee) {
      o->push_back("if (" + negate(cond) + ") {");
      addv(*eb);
      o->push_back("}");
    }
  } else if (k == Node::Kind::While || k == Node::Kind::Loop) {
    std::set<int> bids = {node.block};
    tree_bids(node.body, bids);
    std::set<string> W = loop_written(bids);
    std::set<string> WM = W;
    for (auto &x : mem_lazy()) WM.insert(x);
    pin(o, WM);
    loops.push_back(W);
    if (k == Node::Kind::While) {
      List lead = newlist();
      string cond = emit_cond(at(B, node.block), lead);
      if (node.negated) cond = negate(cond);
      if (has_break(node.body)) pin(lead, W);
      Snap hs = snap();
      List b = newlist();
      emit_seq(node.body, b);
      if (!terminal(*b)) pin(b, W);
      while (!b->empty() && strip(b->back()) == "continue;") b->pop_back();
      loops.pop_back();
      rest(hs);
      if (anytext(*lead)) {
        o->push_back("for (;;) {");
        addv(*lead);
        o->push_back("    if (" + negate(cond) + ") break;");
        addv(*b);
        o->push_back("}");
      } else {
        o->push_back("while (" + cond + ") {");
        addv(*b);
        o->push_back("}");
      }
    } else {
      List b = newlist();
      emit_seq(node.body, b);
      if (!terminal(*b)) pin(b, W);
      while (!b->empty() && strip(b->back()) == "continue;") b->pop_back();
      loops.pop_back();
      for (auto &r : W) setr(r, Value());
      std::vector<string> drop;
      for (auto &kv : breg.v)
        if (W.count(parent(kv.first))) drop.push_back(kv.first);
      for (auto &x : drop) breg.pop(x);
      o->push_back("for (;;) {");
      addv(*b);
      o->push_back("}");
    }
  } else if (k == Node::Kind::DoWhile) {
    std::set<int> bids = {node.block, node.join};
    tree_bids(node.body, bids);
    std::set<string> W = loop_written(bids);
    std::set<string> WM = W;
    for (auto &x : mem_lazy()) WM.insert(x);
    pin(o, WM);
    loops.push_back(W);
    List b = newlist();
    Seq body_t(node.body.begin(), node.body.empty() ? node.body.end() : node.body.end() - 1);
    emit_seq(body_t, b);
    string cond = emit_cond(at(B, node.join), b, &W);
    if (node.negated) cond = negate(cond);
    loops.pop_back();
    for (auto &r : W) setr(r, Value());
    std::vector<string> drop;
    for (auto &kv : breg.v)
      if (W.count(parent(kv.first))) drop.push_back(kv.first);
    for (auto &x : drop) breg.pop(x);
    o->push_back("do {");
    addv(*b);
    o->push_back("} while (" + cond + ");");
  } else if (k == Node::Kind::Switch) {
    const Blk &blk = at(B, node.block);
    int hi = blk.hi;
    Value sel;
    std::set<string> allr = {"ax", "bx", "cx", "dx", "si", "di"};
    if (blk.has_cases) {
      emit_range(blk.lo, blk.selbase != flow16::kNone ? blk.lo + blk.selbase : hi - 2, o);
      if (blk.selreg.empty()) broken( "attribute name must be string, not 'NoneType'");
      const string &reg = blk.selreg;
      if (reg == "al") sel = breg.has("al") ? breg.get("al") : (!rg[AX].empty() ? rg[AX] : interned("al"));
      else if (reg == "ax") sel = !rg[AX].empty() ? rg[AX] : interned("ax");
      else {
        Value v_ = getr(reg);
        sel = !v_.empty() ? v_ : fresh(reg);
      }
      if (hascall(or_else(sel, ""))) {
        materialize(o, "ax");
        sel = (reg == "ax" && !rg[AX].empty()) ? rg[AX] : sel;
      }
    } else {
      std::vector<std::pair<string, string>> t3;
      for (int q = std::max(blk.lo, hi - 3); q < hi; q++) t3.push_back({I(q).mnemonic, replace_all(I(q).op_str, " ", "")});
      if (t3.size() != 3 || t3[1] != std::make_pair(string("xchg"), string("bx,ax")) ||
          !(t3[0] == std::make_pair(string("add"), string("ax,ax")) || t3[0] == std::make_pair(string("shl"), string("ax,1"))))
        fail( "unrecognized jump-table dispatch");
      emit_range(blk.lo, hi - 3, o);
      if (hascall(or_else(rg[AX], ""))) materialize(o, "ax");
      sel = !rg[AX].empty() ? rg[AX] : interned("ax");
    }
    pin(o, {"cx", "dx", "si", "di"});
    rg[AX] = Value();
    rg[BX] = Value();
    breg = OD();
    Snap pre = snap();
    std::vector<std::pair<std::vector<flow16::CaseValue>, std::vector<string>>> arms;
    for (auto &c : node.arms) {
      rest(pre);
      List b = newlist();
      emit_seq(c.second, b);
      if (!terminal(*b)) pin(b, allr);
      arms.push_back({c.first, *b});
    }
    for (auto &r : allr) setr(r, Value());
    breg = OD();
    auto isdef = [](const std::vector<flow16::CaseValue> &v) {
      for (auto &x : v)
        if (x.is_default) return true;
      return false;
    };
    bool brk = false;
    for (auto &c : node.arms)
      if (has_break(c.second)) brk = true;
    if (brk) {
      string t = newt();
      o->push_back(t + " = " + text_of(sel) + ";");
      std::vector<std::pair<std::vector<flow16::CaseValue>, std::vector<string>>> a2;
      for (auto &x : arms)
        if (!isdef(x.first)) a2.push_back(x);
      for (auto &x : arms)
        if (isdef(x.first)) a2.push_back(x);
      std::function<std::vector<string>(size_t, const string &)> chain = [&](size_t q, const string &ind) {
        std::vector<string> L;
        if (q == a2.size()) return L;
        auto &vals = a2[q].first;
        auto &b = a2[q].second;
        string c;
        if (isdef(vals)) c = "1";
        else {
          std::vector<string> cs;
          for (auto &v : vals) cs.push_back("(" + t + " == " + decimal(v.value) + ")");
          c = join(cs, " || ");
        }
        L.push_back(ind + "if (" + c + ") {");
        for (auto &x : b) L.push_back(ind + "    " + x);
        auto rest_ = chain(q + 1, ind + "    ");
        if (!rest_.empty()) {
          L.push_back(ind + "} else {");
          L.insert(L.end(), rest_.begin(), rest_.end());
          L.push_back(ind + "}");
        } else L.push_back(ind + "}");
        return L;
      };
      for (auto &x : chain(0, "")) o->push_back(x);
    } else {
      o->push_back("switch (" + text_of(sel) + ") {");
      for (auto &x : arms) {
        for (auto &v : x.first) o->push_back(v.is_default ? "default:" : "case " + decimal(v.value) + ":");
        addv(x.second);
        if (!terminal(x.second)) o->push_back("    break;");
      }
      o->push_back("}");
    }
  } else if (k == Node::Kind::Break) {
    if (!loops.empty()) pin(o, loops.back());
    o->push_back("break;");
  } else if (k == Node::Kind::Continue) {
    if (!loops.empty()) pin(o, loops.back());
    o->push_back("continue;");
  } else if (k == Node::Kind::Goto) {
    if (node.block != flow16::kNone) {
      long long ta = at(B, node.block).addr;
      i = at(B, node.block).lo - 1;
      pin(o, {"ax", "bx", "cx", "dx", "si", "di"});
      if (!args.empty()) {
        gotoargs[ta] = std::max(gotoargs.count(ta) ? gotoargs[ta] : 0LL, (long long)args.size());
        if (C.dead_registers) slot_args(o);
        else
          for (size_t k2 = 0; k2 < args.size(); k2++) o->push_back("targ" + decimal((long long)k2) + " = " + text_of(args[k2]) + ";");
      }
      gototargets.insert(ta);
      o->push_back("goto L_" + hex(ta) + ";");
    }
  }
}

// ------------------------------------------------------------------------------------ drivers
std::vector<platforms::Insn> decode(const std::vector<uint8_t> &code, const Settings &C) {
  auto lin = disasm16(code, 0);
  bool any = false;
  for (auto &x : lin)
    if (x.mnemonic == "jmp" && has(x.op_str, "[")) any = true;
  if (!C.hooks.switch_targets || !any) return lin;
  std::map<long long, platforms::Insn> seen;
  std::vector<long long> work = {0};
  long long n = (long long)code.size();
  while (!work.empty()) {
    long long a = work.back();
    work.pop_back();
    while (0 <= a && a < n && !seen.count(a)) {
      auto one = disasm16(code.data() + a, (size_t)std::min<long long>(16, n - a), a);
      if (one.empty()) break;
      platforms::Insn ii = one[0];
      seen[a] = ii;
      const string mn = ii.mnemonic;
      bool m = is_number(ii.op_str);
      if (mn == "ret" || mn == "retf") break;
      if (mn == "jmp") {
        if (m) work.push_back(parse_number(ii.op_str));
        else {
          std::vector<long long> t;
          if (C.hooks.switch_targets(a, t))
            for (auto x : t) work.push_back(x);
        }
        break;
      }
      if (cjmp(mn) && m) work.push_back(parse_number(ii.op_str));
      a += ii.size;
    }
  }
  std::vector<platforms::Insn> o;
  for (auto &kv : seen) o.push_back(kv.second);
  return o;
}

std::vector<platforms::Insn> unshare_jcc(const std::vector<platforms::Insn> &ins) {
  std::unordered_map<long long, int> a2i;
  for (size_t k = 0; k < ins.size(); k++) a2i[ins[k].address] = (int)k;
  std::vector<platforms::Insn> o;
  bool changed = false;
  auto fake = [](long long a, int size, const string &mn, const string &op) {
    platforms::Insn x;
    x.address = static_cast<std::uint64_t>(a), x.size = static_cast<std::uint8_t>(size), x.mnemonic = mn, x.op_str = op;
    return x;
  };
  for (size_t k = 0; k < ins.size(); k++) {
    const platforms::Insn &x = ins[k];
    if (x.mnemonic == "jmp" && k &&
        in(ins[k - 1].mnemonic, {"cmp", "test", "or", "and", "xor", "add", "sub", "inc", "dec", "neg"}) && x.size >= 2) {
      int t = NI;
      if (is_number(x.op_str)) {
        auto it = a2i.find(parse_number(x.op_str));
        if (it != a2i.end()) t = it->second;
      }
      if (t != NI && t + 1 < (int)ins.size() && cjmp(ins[t].mnemonic) && !in(ins[t].mnemonic, {"jcxz", "jecxz"}) &&
          is_number(ins[t].op_str)) {
        o.push_back(fake(x.address, 1, ins[t].mnemonic, ins[t].op_str));
        o.push_back(fake(x.address + 1, x.size - 1, "jmp", "0x" + hex(ins[t + 1].address)));
        changed = true;
        continue;
      }
      if (t != NI && t + 1 < (int)ins.size() && in(ins[t].mnemonic, {"sbb", "adc", "rcl", "rcr", "cmc", "lahf", "pushf"}) &&
          !in(ins[t + 1].mnemonic, {"sbb", "adc", "rcl", "rcr"}) && !cjmp(ins[t + 1].mnemonic)) {
        o.push_back(fake(x.address, 1, ins[t].mnemonic, ins[t].op_str));
        o.push_back(fake(x.address + 1, x.size - 1, "jmp", "0x" + hex(ins[t + 1].address)));
        changed = true;
        continue;
      }
    }
    o.push_back(x);
  }
  return changed ? o : ins;
}

// The operand of `sub sp, N` / `mov ax, N`: nothing when it is not a number (a symbolic operand).
std::optional<long long> frame_operand(const std::string &op_str) {
  const std::vector<string> parts = split(op_str, ",");
  if (parts.size() < 2) fail("a frame instruction without a second operand");
  return try_parse_hex(strip(parts[1]));
}

void prologue(const std::vector<platforms::Insn> &ins, const Settings &C, long long &fs, int &k, bool &msc3) {
  long long n = (long long)ins.size();
  auto popped = [&](int q) {
    for (long long z = q + 1; z < n; z++)
      if (ins[z].mnemonic == "pop" && ins[z].op_str == ins[q].op_str) return true;
    return false;
  };
  if (!(ins[0].mnemonic == "push" && ins[0].op_str == "bp")) {
    k = 0;
    while (k < n && ins[k].mnemonic == "push" && in(ins[k].op_str, {"si", "di"}) && popped(k)) k++;
    fs = 0, msc3 = false;
    return;
  }
  if (C.caller_cleans && n > 4 && ins[1].op_str == "bp, sp" && ins[2].mnemonic == "push" && ins[2].op_str == "di" &&
      ins[3].mnemonic == "push" && ins[3].op_str == "si") {
    fs = 4;
    k = 4;
    if (ins[k].mnemonic == "sub" && starts(ins[k].op_str, "sp,")) {
      if (const auto size = frame_operand(ins[k].op_str)) {
        fs += *size;
        k++;
      }
    }
    msc3 = true;
    return;
  }
  fs = 0;
  k = 1;
  if (ins[k].mnemonic == "sub" && starts(ins[k].op_str, "sp")) {
    fs = parse_hex(at(split(ins[k].op_str, ","), 1));
    k++;
    while (k < n && !has(ins[k].op_str, "bp, sp")) k++;
  }
  k++;
  msc3 = false;
  if (fs == 0 && k + 1 < n && in(ins[k + 1].mnemonic, {"call", "lcall"})) {
    if (ins[k].mnemonic == "mov" && starts(ins[k].op_str, "ax,")) {
      if (const auto size = frame_operand(ins[k].op_str)) {
        fs = *size;
        msc3 = true;
        k += 2;
      }
    } else if (ins[k].mnemonic == "xor" && replace_all(ins[k].op_str, " ", "") == "ax,ax") {
      fs = 0, msc3 = true, k += 2;
    }
  }
  if (fs == 0 && !msc3 && k < n && ins[1].op_str == "bp, sp" && ins[k].mnemonic == "sub" && starts(ins[k].op_str, "sp,")) {
    if (const auto size = frame_operand(ins[k].op_str)) {
      fs = *size;
      msc3 = true;
      k++;
    }
  }
  if (!msc3 && C.frame_after_bp && n > 1 && ins[1].op_str == "bp, sp") msc3 = true;
  if (ins[1].op_str == "bp, sp")
    while (k < n && ins[k].mnemonic == "push" && in(ins[k].op_str, {"si", "di"}) && popped(k)) k++;
}

std::vector<string> params_of(const Tracker &r) {
  std::vector<string> p;
  long long m = (r.floatfn || r.longfn) ? r.lmax : r.maxidx;
  for (long long j = 0; j <= m; j++) p.push_back(utf8_char('a' + j));
  return p;
}
Recovery finish(const Tracker &r, long long fs, std::vector<string> stmts) {
  Recovery res;
  res.frame_size = fs;
  res.statements = std::move(stmts);
  res.params = params_of(r);
  res.return_type = r.floatfn ? "float" : r.longfn ? "long" : "int";
  res.locals.assign(r.locals.begin(), r.locals.end());
  return res;
}



Recovery recover(const std::vector<uint8_t> &code, long long base, const std::map<long long, std::string> &cxmap,
                    const Settings &C, Findings &LAST) {
  auto ins = disasm16(code, 0);
  if (ins.size() < 2) return Recovery();
  long long n = (long long)ins.size();
  long long fs = 0;
  int k = 1;
  if (ins[k].mnemonic == "sub" && starts(ins[k].op_str, "sp")) {
    fs = parse_hex(at(split(ins[k].op_str, ","), 1));
    k++;
    while (k < n && !has(ins[k].op_str, "bp, sp")) k++;
  }
  k++;
  bool msc3 = false;
  if (fs == 0 && k + 1 < n && ins[k + 1].mnemonic == "call") {
    if (ins[k].mnemonic == "mov" && starts(ins[k].op_str, "ax,")) {
      if (const auto size = frame_operand(ins[k].op_str)) {
        fs = *size;
        msc3 = true;
        k += 2;
      }
    } else if (ins[k].mnemonic == "xor" && replace_all(ins[k].op_str, " ", "") == "ax,ax") {
      fs = 0, msc3 = true, k += 2;
    }
  }
  if (fs == 0 && !msc3 && C.frame_after_bp && k < n && ins[1].op_str == "bp, sp" && at(ins, k).mnemonic == "sub" &&
      starts(ins[k].op_str, "sp,")) {
    if (const auto size = frame_operand(ins[k].op_str)) {
      fs = *size;
      msc3 = true;
      k++;
    }
  }
  if (!msc3 && C.frame_after_bp && ins[1].op_str == "bp, sp") msc3 = true;
  LAST = Findings();
  LAST.has_saved = LAST.has_register_params = true;
  if (msc3) {
    while (k < n && ins[k].mnemonic == "push" && in(ins[k].op_str, {"si", "di"})) LAST.saved.push_back(ins[k].op_str), k++;
    while (k < n && ins[k].mnemonic == "mov") {
      auto m_ = mt("(si|di), word ptr \\[bp \\+ (0x[0-9a-f]+|\\d+)\\]$", ins[k].op_str);
      if (!m_) break;
      string r = group_of(m_, 1);
      bool saved = std::find(LAST.saved.begin(), LAST.saved.end(), r) != LAST.saved.end();
      bool inrp = false;
      for (auto &kv : LAST.register_params)
        if (kv.first == r) inrp = true;
      if (!saved || inrp) break;
      if (C.reread_register_params) {
        bool again = false;
        for (long long q = k + 1; q < n; q++)
          if (has(ins[q].op_str, "[bp + " + group_of(m_, 2) + "]")) again = true;
        if (again) break;
      }
      LAST.register_params.push_back({r, floor_quotient(parse_number(group_of(m_, 2)) - 4, 2)});
      k++;
    }
  }
  Tracker r(C, LAST, ins, fs, base, cxmap, msc3);
  for (auto &kv : LAST.register_params) r.maxidx = std::max(r.maxidx, kv.second);
  List st = r.walk(k, (int)n);
  std::vector<string> stmts = *st;
  if (!r.gototargets.empty()) {
    r.reset();
    r.emitgoto = true;
    stmts = *r.walk(k, (int)n);
    stmts = fold_shortcircuit(stmts);
  }
  bool plain = !stmts.empty() && r.ranks.size() == stmts.size();
  if (plain)
    for (auto &x : stmts)
      if (has(x, "{") || has(x, "}") || has(x, "goto") || starts(x, "L_")) plain = false;
  if (plain) {
    std::vector<size_t> perm(stmts.size());
    for (size_t j = 0; j < perm.size(); j++) perm[j] = j;
    std::stable_sort(perm.begin(), perm.end(), [&](size_t a, size_t b) { return r.ranks[a] < r.ranks[b]; });
    std::vector<string> s2;
    std::vector<bool> rc;
    for (auto j : perm) s2.push_back(stmts[j]), rc.push_back(r.regconst[j]);
    stmts = dead_store_recover(s2, rc);
  }
  return finish(r, fs, stmts);
}

Recovery recover_cfg(const std::vector<uint8_t> &code, long long base, const std::map<long long, std::string> &cxmap,
                        const Settings &C, Findings &LAST) {
  auto ins = decode(code, C);
  if (ins.size() < 2) return Recovery();
  if (C.dead_registers) ins = unshare_jcc(ins);
  long long fs;
  int k;
  bool msc3;
  prologue(ins, C, fs, k, msc3);
  flow16::Options flow;
  flow.compare_switches = C.compare_switches;
  flow.uncapped = C.uncapped_structures;
  if (C.hooks.switch_targets)
    flow.switch_targets = [&](long long address) -> std::optional<std::vector<long long>> {
      std::vector<long long> targets;
      if (C.hooks.switch_targets(address, targets)) return targets;
      return std::nullopt;
    };
  const flow16::Cfg cfg = flow16::build_cfg(ins, flow);
  std::vector<Blk> blocks;
  for (const flow16::Block &b : cfg.blocks)
    blocks.push_back(Blk{static_cast<int>(b.first), static_cast<int>(b.last), static_cast<long long>(at(ins, static_cast<long long>(b.first)).address),
                         b.end, b.cases.has_value(), b.selector, b.selector_end});
  Seq tree = flow16::structure(cfg, 0, C.uncapped_structures);
  Tracker r(C, LAST, ins, fs, base, cxmap, msc3);
  r.emit_start = k;
  r.blocks = &blocks;
  List top = newlist();
  r.out = top;
  r.emit_seq(tree, top);
  std::vector<string> stmts = *top;
  std::unique_ptr<Tracker> r2;
  if (!r.gototargets.empty()) {
    r2.reset(new Tracker(C, LAST, ins, fs, base, cxmap, msc3));
    r2->emit_start = k;
    r2->blocks = &blocks;
    r2->labels = r.gototargets;
    r2->gotoargs = r.gotoargs;
    List t2 = newlist();
    r2->out = t2;
    r2->emit_seq(tree, t2);
    stmts = *t2;
  }
  const Tracker &rf = r2 ? *r2 : r;
  stmts = strip_tail_returns(tidy(stmts));
  if (C.dead_registers) {
    stmts = dead_regs(stmts);
    if (C.mark || C.unsigned_compares) stmts = fold_ladders(stmts, C.mark);
    stmts = tidy(fold_goto_or(stmts));
  }
  if (!C.mark) {
    for (auto &x : stmts) x = RX("^(\\s*)USE\\((.*)\\);$").replace_all(x, "\\1/* the next call takes \\2 in registers */");
    stmts = fold_longs(stmts);
    std::vector<string> used;
    for (const char *x : {"ax", "bx", "cx", "dx", "si", "di"}) {
      const Rx &rx = RX("(?<![\\w>.])" + string(x) + "(?![\\w(])");
      for (auto &ln : stmts)
        if (rx.search(ln)) {
          used.push_back(x);
          break;
        }
    }
    if (!used.empty() && anytext(stmts)) stmts.insert(stmts.begin(), "/* register variables: " + join(used, ", ") + " */");
  }
  return finish(rf, fs, stmts);
}

}  // namespace cb::invert::early_c_model
