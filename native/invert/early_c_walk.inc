// early_c_walk.inc -- the rest of class R (statements, the linear walker, the tree emitter) and the
// drivers recover / recover_cfg; included by early_c.cc (one translation unit, its unnamed namespace).
namespace cbinv {
namespace {

std::string carry_kind(const std::vector<CsIns> &ins, long long k) {
  if (k < 1 || k >= (long long)ins.size()) return "";
  const CsIns &c = ins[k], &p = ins[k - 1];
  auto memparts = [](const string &op, string &w, string &b, long long &off) -> bool {
    auto m = mt("(word|byte) ptr (?:\\w\\w:)?\\[(.*)\\]$", strip(op));
    if (!m) return false;
    string inner = g(m, 2);
    auto m2 = mt("(.*?)(?: ([+-]) )?(0x[0-9a-f]+|\\d+)$", inner);
    w = g(m, 1);
    if (m2 && (g(m2, 1).empty() || m2.has(2))) {
      off = int0(g(m2, 3));
      b = g(m2, 1);
      if (m2.has(2) && g(m2, 2) == "-") off = -off;
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

int R::struct_copy(int i0, const List &o) {
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
      Str vs = orS(rg[SI], "si"), vd = orS(rg[DI], "di");
      given = OD();
      given.set("di", fr(S(vs)));
      given.set("si", fr(S(vd)));
      k++;
    }
    while (k < ii + 4 && k < N() && in(ins[k].mnemonic, {"mov", "lea"}) && mt("(di|si), ", ins[k].op_str)) {
      auto pr = split2(ins[k].op_str, ", ");
      const string &r_ = pr.first, &src = pr.second;
      Str v_;
      if (ins[k].mnemonic == "lea") {
        v_ = memname(src);
        v_ = truthy(v_) ? fr("&" + v_.s()) : Str();
      } else if (src == "si" || src == "di") {
        v_ = given.has(src) ? given.get(src) : orS(getr(src), src.c_str());
      } else if (in(src, {"ax", "bx", "cx", "dx"})) {
        v_ = val(src);
      } else if (fm("0x[0-9a-f]+|\\d+", src)) {
        long long nv = num(src);
        if (nv > 0) {
          string h = X(nv);
          while (h.size() < 4) h = "0" + h;
          v_ = fr("&g_" + h);
        } else {
          v_ = fromint(nv);
        }
      } else {
        v_ = memname(src);
      }
      if (v_.none()) break;
      given.set(r_, fr(S(v_)));
      k++;
      if (k < N() && ins[k].mnemonic == "xchg" && in(ins[k].op_str, {"di, si", "si, di"}) && given.v.size() == 2) {
        Str gs = given["si"], gd = given["di"];
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
    n = num(at(split(ins[j].op_str, ", "), 1)) * (has(sl(t, 0, 1), "w") ? 2 : 1);
    j += 2;
  } else {
    while (j < N() && in(ins[j].mnemonic, {"movsw", "movsb"})) {
      n += ins[j].mnemonic == "movsw" ? 2 : 1;
      j++;
    }
  }
  if (!n || (!bare && (j + 1 >= N() || mo(j) != P("pop", "di") || mo(j + 1) != P("pop", "si")))) return 0;
  Str d_ = !rg[DI].none() ? rg[DI] : lit("di"), s_ = !rg[SI].none() ? rg[SI] : lit("si");
  string ns = D(n);
  if (!given.empty()) {
    if (given.has("di")) d_ = given.get("di");
    if (given.has("si")) s_ = given.get("si");
    string ds = S(d_), ss = S(s_);
    if (!fm("[\\w\\->.&]+", ds)) ds = "(" + ds + ")";
    if (!fm("[\\w\\->.&]+", ss)) ss = "(" + ss + ")";
    emit(o, "*(struct _b" + ns + " *)" + ds + " = *(struct _b" + ns + " *)" + ss + ";");
    return (int)(j + 2 - iorig);
  }
  if (bare) {
    for (int rep = 0; rep < 2; rep++) {
      if (o->empty()) break;
      auto mk = fm("(si|di) = (-?\\d+|[\\w\\->.]+);", strip(o->back()));
      if (!mk || !in(g(mk, 1), {"si", "di"}) || (g(mk, 1) == "si" && S(s_) != "si") || (g(mk, 1) == "di" && S(d_) != "di"))
        break;
      string v_ = g(mk, 2);
      if (fm("-?\\d+", v_) && (int0(v_) & 0xffff) >= 0x100) v_ = "&g_" + X(int0(v_) & 0xffff);
      if (g(mk, 1) == "si") s_ = fr(v_);
      else d_ = fr(v_);
      o->pop_back();
      if (ranks.size() > o->size()) ranks.pop_back(), regconst.pop_back();
    }
  }
  emit(o, "*(struct _b" + ns + " *)" + S(d_) + " = *(struct _b" + ns + " *)" + S(s_) + ";");
  if (bare) rg[SI] = rg[DI] = Str();
  return (int)(j + (bare ? 0 : 2) - ii);
}

int R::arg_copy(int i0) {
  if (i0 + 5 >= N() || ins[i0].mnemonic != "sub" || !fm("sp, (0x[0-9a-f]+|\\d+)", ins[i0].op_str)) return 0;
  const CsIns &m = ins[i0 + 1];
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
  string src = sl(m.op_str, 4);
  string v;
  if (m.mnemonic == "lea") {
    Str nm = memname(src);
    if (!(truthy(nm) && fm("\\w+", nm.s()))) return 0;
    v = "&" + nm.s();
  } else {
    Str x = wr(src) >= 0 ? val(src) : orS(memname(src), src.c_str());
    v = fm("[\\w\\->.]+", S(x)) ? S(x) : "(" + S(x) + ")";
  }
  args.push_back(fr(n == 8 ? "*(double *)" + v : "*(struct _b" + D(n) + " *)" + v));
  return (int)(j - i0);
}

bool R::var_later(const string &d) {
  long long k = i + 1;
  while (k < N() && in(ins[k].mnemonic, {"shl", "sal", "add"}) && split(ins[k].op_str, ", ")[0] == d) k++;
  for (long long q = std::max(0LL, k); q < N(); q++) {
    const CsIns &x = ins[q];
    auto o = split(x.op_str, ", ");
    if (in(x.mnemonic, {"mov", "lea", "lds", "les"}) && o[0] == d && !srd("\\b" + d + "\\b", splitn(x.op_str, ", ", 1).back()))
      return false;
    if (in(x.mnemonic, {"inc", "dec", "neg", "not"}) && x.op_str == d) return true;
    if (in(x.mnemonic, {"add", "sub", "shl", "shr", "sar", "and", "or", "xor", "adc", "sbb"}) && o[0] == d) return true;
    if (in(x.mnemonic, {"cmp", "test"}) && std::find(o.begin(), o.end(), d) != o.end()) return true;
    if (x.mnemonic == "mov" && o.size() == 2 && o[1] == d && o[0] != "bx") return true;
    if (x.mnemonic == "push" && x.op_str == d && q != N() - 1) return true;
  }
  return false;
}

int R::fp_arg(int i0) {
  if (i0 + 2 >= N() || ins[i0].mnemonic != "sub" || fst.empty()) return 0;
  auto m = fm("sp, (ax|0x8|8|4)", ins[i0].op_str);
  if (!m || (g(m, 1) == "ax" && !in(S(rg[AX]), {"8", "4"}))) return 0;
  if (!(ins[i0 + 1].mnemonic == "mov" && ins[i0 + 1].op_str == "bx, sp")) return 0;
  long long j = i0 + 2;
  while (j < N() && in(ins[j].mnemonic, {"wait", "fwait", "nop"})) j++;
  re::Match f;
  if (j < N()) f = fm("(qword|dword) ptr \\[bx\\]", ins[j].op_str);
  if (!f || ins[j].mnemonic != "fstp") return 0;
  string v = fst.back();
  fst.pop_back();
  args.push_back(fr(g(f, 1) == "qword" ? "(double)" + v : "(float)" + v));
  if (g(m, 1) == "ax") rg[AX] = Str();
  return (int)(j + 1 - i0);
}

int R::scratch_index(int i0) {
  if (i0 + 2 >= N() || ins[i0].mnemonic != "mov") return 0;
  auto m = fm("(si|di), (ax|bx|cx|dx|si|di)", ins[i0].op_str);
  if (!m || g(m, 1) == g(m, 2) || !(ins[i0 + 1].mnemonic == "shl" && ins[i0 + 1].op_str == g(m, 1) + ", 1")) return 0;
  string r = g(m, 1);
  long long k = i0 + 2;
  if (!has(ins[k].op_str, "[bx + " + r + "]")) return 0;
  for (long long q = k + 1; q < N(); q++) {
    const CsIns &x = ins[q];
    if (cjmp(x.mnemonic) || in(x.mnemonic, {"jmp", "ret", "retf", "call"})) break;
    if (srd("\\b" + r + "\\b", x.op_str)) {
      if (x.mnemonic == "mov" && starts(x.op_str, (r + ", ").c_str()) && !srd("\\b" + r + "\\b", sl(x.op_str, 4))) break;
      return 0;
    }
  }
  string v = S(val(g(m, 2)));
  setr(r, fr("(" + (fm("[\\w\\->.]+", v) ? v : "(" + v + ")") + " << 1)"));
  return 2;
}

int R::step0(int i0, const List &o) {
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
  int n = C.deadregs ? 0 : long_pat(i0);
  if (n) {
    retlive = true;
    return i0 + n;
  }
  n = long_stmt(i0, o);
  if (n) return i0 + n;
  n = carry(i0, o);
  if (n) return i0 + n;
  const CsIns &ii = I(i0);
  const string mn = ii.mnemonic, ops = ii.op_str;
  string d0 = strip(split(ops, ",")[0]);
  if (in(mn, {"wait", "fwait"}) || (starts(mn, "f") && fpop(mn))) {
    int k = fp_step(i0, o);
    if (k) return k;
  }
  if (in(mn, {"cmp", "test"}) && count(ops, ", ") == 1) {
    auto ab = split(ops, ", ");
    try {
      Str x = opval(ab[0]);
      Str y = opval(ab[1]);
      string w = (has(ops, "byte ptr") || isbyte(ab[0])) ? "S8" : "S16";
      if (mn == "cmp") lastcmp = LC{x, y, w};
      else lastcmp = LC{ab[0] == ab[1] ? x : fr("(" + S(x) + " & " + S(y) + ")"), lit("0"), w};
      has_lastcmp = true;
      blockcmp = i0;
    } catch (const PyError &) {
    }
  }
  if (in(mn, {"or", "and", "test"}) && count(ops, ",") == 1 && d0 == strip(split(ops, ",")[1]) && !has(ops, "[")) return i0 + 1;
  kill_aliases(i0);
  if (C.mark && C.farptr) {
    if (mn == "les") {
      auto pr = partition(ops, ", ");
      Str seg = memname(plus2(pr.second));
      string sg = seg.none() ? "es" : seg.s();
      if (has(pr.second, "es:")) {
        string t = newt();
        emit(o, t + " = " + sg + ";");
        expr_step(i0);
        if (wr(pr.first) >= 0 && !getr(pr.first).none()) materialize(o, pr.first);
        emit(o, "es = " + t + ";");
        return i0 + 1;
      }
      emit(o, "es = " + sg + ";");
    } else if (mn == "mov" && starts(ops, "es, ")) {
      string r_ = strip(sl(ops, 4));
      Str v = wr(r_) >= 0 ? getr(r_) : Str();
      emit(o, "es = " + (!v.none() ? v.s() : S(orS(memname(r_), r_.c_str()))) + ";");
      return i0 + 1;
    } else if (mn == "pop" && ops == "es") {
      Str v;
      if (!args.empty()) v = args.back(), args.pop_back();
      else v = lit("es");
      if (S(v) != "es") emit(o, "es = " + S(v) + ";");
      return i0 + 1;
    }
  }
  if (byte_on_word(mn, ops)) return i0 + 1;
  for (auto &t_ : RX("\\b[abcd][lh]\\b").findall(ops))
    if (!breg.has(t_) && hascall(orS(getr(parent(t_)), ""))) materialize(o, parent(t_));
  if (isbyte(d0)) p8_on = true, p8_i = i0, p8_v = bval(d0);
  else p8_on = false;
  {
    auto o2 = splitn(ops, ", ", 1);
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
    if (!breg.has(src) && hascall(orS(getr(parent(src)), ""))) materialize(o, parent(src));
    Str dn = memname(has(d0, "byte ptr") ? d0 : "byte ptr " + d0);
    emit(o, S(dn) + " = " + S(bval(src)) + ";");
    return i0 + 1;
  }
  if (mn == "cmp" && starts(ops, "ax, ") && i0 + 2 < N() && isjcc(ins[i0 + 1].mnemonic) && ins[i0 + 2].mnemonic == "xor") {
    rg[AX] = fr("(" + S(rg[AX]) + " " + jcc_get(ins[i0 + 1].mnemonic) + " " + S(memname(sl(ops, 4))) + ")");
    retlive = true;
    axsrc = NI;
    int q = i0;
    while (q < N() && I(q).op_str != "ax, bx") q++;
    return q + 1;
  }
  if (mn == "pop" && C.deadregs && wr(ops) >= 0 && !args.empty()) {
    string v = S(args.back());
    args.pop_back();
    if (!getr(ops).none() && getr(ops).s() != ops) setr(ops, Str());
    if (in(ops, {"ax", "bx", "cx", "dx"})) breg.pop0(string(1, ops[0]) + "l"), breg.pop0(string(1, ops[0]) + "h");
    if (v != ops) emit(o, ops + " = " + v + ";");
    setr(ops, Str());
    if (ops == "ax") axsrc = NI, retlive = true;
    return i0 + 1;
  }
  if (mn == "push") {
    if (ops == "cs" && i0 + 1 < N() && ins[i0 + 1].mnemonic == "call") return i0 + 1;
    if (ops == "ax" && rg[AX].none() && msc3 && breg.has("al") && S(breg.get("ah")) == "0") args.push_back(fr(S(breg["al"])));
    else if (ops == "ax") args.push_back(!rg[AX].none() ? rg[AX] : lit("ax"));
    else if (in(ops, {"bx", "cx", "dx", "si", "di"})) {
      Str v = getr(ops);
      args.push_back(!v.none() ? v : fr(ops));
    } else args.push_back(orS(memname(ops), ops.c_str()));
    return i0 + 1;
  }
  if (mn == "call") {
    auto m = mt(NUMRX, ops);
    string a = join(take_args(i0, "near", m ? D(num(g(m, 1))) : ""), ", ");
    if (a.empty() && !fst.empty()) a = fst.back(), fst.pop_back();
    if (a.empty() && msc3 && i0 >= 3) {
      std::vector<std::pair<string, string>> pre;
      for (int q = i0 - 3; q < i0; q++) pre.push_back({ins[q].mnemonic, ins[q].op_str});
      auto &last = pre.back();
      std::set<string> heads;
      for (int q = 0; q < 2; q++)
        if (pre[q].first == "mov") heads.insert(split(pre[q].second, ",")[0]);
      if (last.first == "mov" && (starts(last.second, "cl, ") || starts(last.second, "cx, ")) && heads == std::set<string>{"ax", "dx"}) {
        string lo_ = S(!rg[AX].none() ? rg[AX] : lit("ax")), hi_ = S(!rg[DX].none() ? rg[DX] : lit("dx"));
        string c1 = at(split(last.second, ", "), 1);
        a = "LONG(" + hi_ + ", " + lo_ + "), " + (isnum(c1) ? D(num(c1)) : "cl");
        rg[AX] = Str();
        rg[DX] = Str();
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
        string src_ = at(splitn(last.second, ", ", 1), 1);
        string cnt = isnum(src_) ? D(num(src_)) : (has(src_, "[") ? S(memname(src_)) : "cl");
        string lo_ = S(!rg[AX].none() ? rg[AX] : lit("ax"));
        bool cd = false;
        for (int q = 0; q < 2; q++)
          if (in(pre[q].first, {"cdq", "cwd"})) cd = true;
        if (cd) a = "(long)" + lo_ + ", " + cnt;
        else a = "LONG(" + S(!rg[DX].none() ? rg[DX] : lit("dx")) + ", " + lo_ + "), " + cnt;
        rg[AX] = Str();
        rg[DX] = Str();
      }
    }
    string expr;
    if (m) expr = "sub_" + X(num(g(m, 1))) + "(" + a + ")";
    else {
      string fp = S(orS(memname(ops), ops.c_str()));
      if (C.deadregs && wr(ops) >= 0) {
        Str v_ = getr(ops);
        if (!v_.none()) fp = v_.s();
      }
      expr = "(*" + fp + ")(" + a + ")";
    }
    call_result(i0, o, expr);
    return i0 + 1;
  }
  if (mn == "lcall") {
    auto mm = mt("(0x[0-9a-f]+|\\d+),\\s*(0x[0-9a-f]+|\\d+)$", ops);
    string a = join(take_args(i0, "far", mm ? D(num(g(mm, 1))) + "," + D(num(g(mm, 2))) : ""), ", ");
    string expr;
    if (mm) expr = "sub_" + X(num(g(mm, 1)) * 16 + num(g(mm, 2))) + "(" + a + ")";
    else expr = "(*" + S(orS(memname(ops), ops.c_str())) + ")(" + a + ")";
    call_result(i0, o, expr);
    return i0 + 1;
  }
  if (mn == "mov" && ops == "sp, bp") return i0 + 1;
  if (mn == "mov" && has(split(ops, ", ")[0], "[")) {
    auto pr = rpartition(ops, ", ");
    const string dest = pr.first, src = pr.second;
    if (src == "ax") {
      long long rank = (axsrc != NI && axsrc < i0) ? axsrc : i0;
      bool rc = axsrc != NI && (bool)mt("-?\\d+$|-?0x[0-9a-f]+$", S(rg[AX]));
      Str dn = memname(dest);
      emit(o, S(dn) + " = " + S(!rg[AX].none() ? rg[AX] : lit("ax")) + ";", rank, rc);
      if (!rg[AX].none() && rg[AX].s() != "ax") rg[AX] = memname(dest);
      axsrc = NI;
      retlive = false;
      return i0 + 1;
    }
    if (in(src, {"bx", "cx", "dx", "si", "di"})) {
      long long rank = (src == "bx" && bxsrc != NI && bxsrc < i0) ? bxsrc : i0;
      Str rv = !getr(src).none() ? getr(src) : fr(src);
      Str dn = memname(dest);
      emit(o, S(dn) + " = " + S(rv) + ";", rank);
      if (S(rv) != src) setr(src, memname(dest));
      if (src == "bx") bxsrc = NI;
      return i0 + 1;
    }
    if (isnum(src)) {
      Str dn = memname(dest);
      emit(o, S(dn) + " = " + D(num(src)) + ";");
      return i0 + 1;
    }
    if (in(src, {"ds", "es", "ss", "cs"})) {
      Str dn = memname(dest);
      emit(o, S(dn) + " = " + src + ";");
      return i0 + 1;
    }
  }
  if (isalu(mn) && has(split(ops, ", ")[0], "[") && ends(ops, ", ax")) {
    Str d = memname(split(ops, ",")[0]);
    Str av = !rg[AX].none() ? rg[AX] : lit("ax");
    emit(o, S(d) + " = " + S(d) + " " + ALU(mn) + " " + S(av) + ";");
    return i0 + 1;
  }
  if (isalu(mn) && has(split(ops, ", ")[0], "[")) {
    auto pr = rpartition(ops, ", ");
    const string dst = pr.first, src = pr.second;
    if (isnum(src) || in(src, {"bx", "cx", "dx", "si", "di"})) {
      Str d = memname(dst);
      string rv = isnum(src) ? D(num(src)) : S(val(src));
      emit(o, S(d) + " = " + S(d) + " " + ALU(mn) + " " + rv + ";");
      return i0 + 1;
    }
    if (isbyte(src)) {
      Str d = memname(has(dst, "byte ptr") ? dst : "byte ptr " + dst);
      emit(o, S(d) + " = " + S(d) + " " + ALU(mn) + " " + S(bval(src)) + ";");
      return i0 + 1;
    }
  }
  if (in(mn, {"inc", "dec"}) && ops == "ax") {
    string b = S(!rg[AX].none() ? rg[AX] : lit("ax"));
    rg[AX] = fr("(" + b + " " + (mn == "inc" ? "+" : "-") + " 1)");
    retlive = true;
    axsrc = NI;
    return i0 + 1;
  }
  if (in(mn, {"inc", "dec"}) && in(ops, {"bx", "cx", "dx", "si", "di"})) {
    Str cur = getr(ops);
    emit(o, ops + " = " + (cur.none() ? ops : cur.s()) + " " + (mn == "inc" ? "+" : "-") + " 1;");
    setr(ops, Str());
    if (ops == "bx") bxsrc = NI;
    return i0 + 1;
  }
  if (msc3 && in(mn, {"inc", "dec"}) && starts(ops, "word ptr [") && !isbyte(ops)) {
    string nm = S(orS(memname(ops), ops.c_str()));
    string op2 = mn == "inc" ? "++" : "--";
    const CsIns *nx = i0 + 1 < N() ? &ins[i0 + 1] : nullptr;
    if (msc3 && i0 + 1 >= (range_hi >= 0 ? range_hi : N())) nx = nullptr;
    if (nx && nx->mnemonic == "mov" && ends(nx->op_str, ", " + ops) && in(split(nx->op_str, ",")[0], {"bx", "si", "di", "ax", "cx", "dx"})) {
      string r = split(nx->op_str, ",")[0];
      setr(r, fr(op2 + nm));
      if (r == "bx") bxsrc = NI;
      if (r == "ax") axsrc = NI;
      return i0 + 2;
    }
    for (const char *r : {"bx", "si", "di", "ax", "cx", "dx"}) {
      Str v = getr(r);
      if (!v.none() && v.s() == nm && read_before_write(r, i0 + 1)) {
        setr(r, fr(nm + op2));
        if (string(r) == "bx") bxsrc = NI;
        if (string(r) == "ax") axsrc = NI;
        return i0 + 1;
      }
    }
  }
  if (in(mn, {"inc", "dec"}) && !isbyte(ops)) {
    string nm = S(orS(memname(ops), ops.c_str()));
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
    if (ends(ops, "cl")) return breg.has("cl") ? S(breg["cl"]) : S(!rg[CX].none() ? rg[CX] : lit(dflt));
    return S(val(at(split(ops, ", "), 1)));
  };
  if (in(mn, {"shl", "sal", "shr", "sar"}) && has(split(ops, ",")[0], "[")) {
    Str d = memname(split(ops, ",")[0]);
    string cnt = shcount("cx");
    if (in(mn, {"shr", "sar"})) emit(o, S(d) + " = " + shr(C, mn, S(d), cnt, has(ops, "byte ptr")) + ";");
    else emit(o, S(d) + " = " + S(d) + " << " + cnt + ";");
    return i0 + 1;
  }
  if (in(mn, {"neg", "not"}) && has(ops, "[")) {
    Str d = memname(ops);
    emit(o, S(d) + " = " + (mn == "neg" ? "-" : "~") + S(d) + ";");
    return i0 + 1;
  }
  if (isbyte(d0) && in(mn, {"shl", "sal", "shr", "sar"})) {
    string cur = p8_on ? S(p8_v) : d0;
    string cnt = shcount("cl");
    if (in(mn, {"shr", "sar"})) breg.set(d0, fr("((" + shr(C, mn, cur, cnt, true) + ") & 0xff)"));
    else breg.set(d0, fr("((" + cur + " << " + cnt + ") & 0xff)"));
    return i0 + 1;
  }
  if (isbyte(d0) && in(mn, {"neg", "not"})) {
    string cur = p8_on ? S(p8_v) : d0;
    breg.set(d0, fr("((" + string(mn == "neg" ? "-" : "~") + cur + ") & 0xff)"));
    return i0 + 1;
  }
  if (isalu(mn) && in(d0, {"bx", "cx", "dx", "si", "di"}) && split(ops, ", ").size() == 2) {
    string src = strip(split(ops, ", ")[1]);
    if (src == d0 && in(mn, {"sub", "xor"}) && d0 == "dx" && i0 + 1 < N() && in(ins[i0 + 1].mnemonic, {"div", "idiv"})) {
      rg[DX] = Str();
      return i0 + 1;
    }
    if (src == d0 && in(mn, {"sub", "xor"})) emit(o, d0 + " = 0;");
    else if (src == d0 && mn == "or") {
    } else {
      Str rh = wr(src) >= 0 ? val(src) : rhs(src);
      Str cur = getr(d0);
      emit(o, d0 + " = " + (cur.none() ? d0 : cur.s()) + " " + ALU(mn) + " " + S(rh) + ";");
    }
    setr(d0, Str());
    return i0 + 1;
  }
  if (in(mn, {"rep movsb", "rep movsw", "repne movsb", "repne movsw"})) {
    string n_ = S(orS(rg[CX], "cx"));
    if (ends(mn, "movsw")) n_ = isdigits(n_) ? D(int0(n_) * 2) : "(" + n_ + " * 2)";
    string dd = S(orS(rg[DI], "di"));
    emit(o, string(C.mark ? (ends(mn, "w") ? "memcpy_w" : "memcpy_b") : "memcpy") + "(" +
                (C.mark && C.farptr ? "FAROFF(es, " + dd + ")" : dd) + ", " + S(orS(rg[SI], "si")) + ", " + n_ + ");");
    rg[SI] = rg[DI] = Str();
    rg[CX] = lit("0");
    return i0 + 1;
  }
  if (in(mn, {"rep stosb", "rep stosw", "repne stosb", "repne stosw"})) {
    string n_ = S(orS(rg[CX], "cx"));
    if (ends(mn, "stosw")) n_ = isdigits(n_) ? D(int0(n_) * 2) : "(" + n_ + " * 2)";
    string dd = S(orS(rg[DI], "di"));
    emit(o, string(C.mark ? (ends(mn, "w") ? "memset_w" : "memset_b") : "memset") + "(" +
                (C.mark && C.farptr ? "FAROFF(es, " + dd + ")" : dd) + ", " + S(!rg[AX].none() ? rg[AX] : lit("ax")) + ", " +
                n_ + ");");
    rg[DI] = Str();
    rg[CX] = lit("0");
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
      Str nm = memname(mem);
      Str rv = getr(reg);
      if (rv.none()) rv = fr(reg);
      string t = newt();
      emit(o, t + " = " + S(nm) + ";");
      emit(o, S(nm) + " = " + S(rv) + ";");
      setr(reg, fr(t));
      if (reg == "ax") axsrc = NI, retlive = true;
      if (reg == "bx") bxsrc = NI;
      return i0 + 1;
    }
  }
  if (in(mn, {"movsb", "movsw", "stosb", "stosw", "lodsb", "lodsw", "xlatb"})) {
    int w = ends(mn, "w") ? 2 : 1;
    auto pinreg = [&](const string &r) {
      Str v = getr(r);
      if (!v.none() && v.s() != r) materialize(o, r);
      setr(r, Str());
    };
    auto settle_ax = [&]() {
      pinreg("ax");
      for (const char *b8 : {"al", "ah"})
        if (breg.has(b8)) {
          Str v = breg.pop(b8);
          if (S(v) != b8) emit(o, string(b8) + " = " + S(v) + ";");
        }
    };
    auto B = [&](const string &x) -> string {
      if (C.mark) return "BYTE(" + x + ")";
      return starts(x, "*") ? "*(char *)" + x.substr(1) : x;
    };
    string DI_ = (C.mark && C.farptr) ? "FARM(es, *di)" : "*di";
    string ws = D(w);
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
      long long t = num(g(m, 1));
      if (t > ins.back().address || t < ins.front().address) {
        std::vector<string> a;
        for (auto it = args.rbegin(); it != args.rend(); ++it) a.push_back(S(*it));
        emit(o, "return sub_" + X(t & 0xFFFF) + "(" + join(a, ", ") + ");");
        args.clear();
      } else if (has_epi && t >= epi && !rg[AX].none() && retlive) {
        if (!rg[DX].none() && rg[DX].s() != "dx") {
          emit(o, "dx = " + rg[DX].s() + ";");
          rg[DX] = lit("dx");
        }
        emit(o, "return " + S(rg[AX]) + ";");
      } else if (!(i0 + 1 < N() && t == ins[i0 + 1].address)) {
        gototargets.insert(t);
        emit(o, "goto L_" + X(t) + ";");
      }
    }
    return i0 + 1;
  }
  if (mn == "ret" && !rg[AX].none() && retlive) {
    if (!rg[DX].none() && rg[DX].s() != "dx") {
      emit(o, "dx = " + rg[DX].s() + ";");
      rg[DX] = lit("dx");
    }
    emit(o, "return " + S(rg[AX]) + ";");
    return i0 + 1;
  }
  expr_step(i0);
  return i0 + 1;
}

void R::reset() {
  for (auto &x : rg) x = Str();
  has_div = false;
  div0 = div1 = Str();
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
Snap R::snap() {
  Snap d;
  for (int k = 0; k < 6; k++) d.r[k] = rg[k];
  d.has_div = has_div, d.div0 = div0, d.div1 = div1;
  d.retlive = retlive, d.axsrc = axsrc, d.bxsrc = bxsrc, d.args = args, d.breg = breg, d.i = i;
  return d;
}
void R::rest(const Snap &d) {
  for (int k = 0; k < 6; k++) rg[k] = d.r[k];
  has_div = d.has_div, div0 = d.div0, div1 = d.div1;
  retlive = d.retlive, axsrc = d.axsrc, bxsrc = d.bxsrc, args = d.args, breg = d.breg, i = d.i;
}

bool R::terminal(const std::vector<string> &body) {
  std::vector<string> lines;
  for (auto &l : body)
    if (!strip(l).empty()) lines.push_back(l);
  if (lines.empty()) return false;
  string last = strip(lines.back());
  if (last != "}") return (bool)mt("(break|continue|return\\b|goto )", last);
  long long d = indent(lines.back());
  std::vector<std::vector<string>> arms;
  std::vector<string> cur;
  long long k = (long long)lines.size() - 2;
  bool has_else = false, broke = false;
  while (k >= 0) {
    const string &t = lines[k];
    if (indent(t) == d) {
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
    } else if (indent(t) < d) return false;
    else cur.push_back(t);
    k--;
  }
  if (!broke) return false;
  if (!has_else) return false;
  for (auto &a : arms)
    if (!terminal(a)) return false;
  return true;
}

void R::phi_args(const std::vector<Str> &a_args, const std::vector<Str> &b_args, std::vector<string> &a_body,
                 std::vector<string> &b_body, bool a_term, bool b_term) {
  const re::Regex &rr = RX(TOKRX);
  auto one = [&](const std::vector<Str> &as, std::vector<string> &body) {
    std::vector<Str> o;
    for (auto &x : as) {
      if (rr.search(S(x))) {
        string t = newt();
        body.push_back(t + " = " + S(x) + ";");
        o.push_back(fr(t));
      } else o.push_back(x);
    }
    args = o;
  };
  if (a_term && !b_term) return one(b_args, b_body);
  if (b_term && !a_term) return one(a_args, a_body);
  if (a_args.size() != b_args.size()) return;
  bool same = true;
  for (size_t k = 0; k < a_args.size(); k++)
    if (!eq(a_args[k], b_args[k])) same = false;
  bool anyr = false;
  for (auto &x : a_args)
    if (rr.search(S(x))) anyr = true;
  if (same && !anyr) return;
  std::vector<Str> o;
  for (size_t k = 0; k < a_args.size(); k++) {
    const Str &x = a_args[k], &y = b_args[k];
    if (S(x) == S(y) && !rr.search(S(x))) {
      o.push_back(x);
      continue;
    }
    string t = newt();
    a_body.push_back(t + " = " + S(x) + ";");
    b_body.push_back(t + " = " + S(y) + ";");
    o.push_back(fr(t));
  }
  args = o;
}

void R::phi(const Snap &ts, const Snap &es, std::vector<string> &tb, std::vector<string> &eb) {
  bool tterm = terminal(tb), eterm = terminal(eb);
  phi_args(ts.args, es.args, tb, eb, tterm, eterm);
  for (int r : REGS_ORDER) {
    const Str &tv = ts.r[r], &ev = es.r[r];
    string rn = WRN[r];
    if (eq(tv, ev)) {
      rg[r] = tv;
      continue;
    }
    if (!C.deadregs && !live(rn, i) && !hascall(tv) && !hascall(ev) &&
        !(msc3 && rn == "ax" && retval() && (tv.none() || ev.none()))) {
      rg[r] = Str();
      continue;
    }
    if (!tv.none() && tv.s() != rn && !tterm) tb.push_back(rn + " = " + tv.s() + ";");
    if (!ev.none() && ev.s() != rn && !eterm) eb.push_back(rn + " = " + ev.s() + ";");
    rg[r] = Str();
  }
  for (const char *r : BYTEREG) {
    Str tv = ts.breg.get(r), ev = es.breg.get(r);
    if (eq(tv, ev)) {
      if (!tv.none()) breg.set(r, tv);
      continue;
    }
    if (!tv.none() && tv.s() != r && !tterm) tb.push_back(string(r) + " = " + tv.s() + ";");
    if (!ev.none() && ev.s() != r && !eterm) eb.push_back(string(r) + " = " + ev.s() + ";");
    breg.pop0(r);
  }
}

bool anytext(const std::vector<string> &b) {
  for (auto &x : b)
    if (!strip(x).empty()) return true;
  return false;
}

List R::body(int lo, int hi) {
  List b = walk(lo, hi);
  if (anytext(*b) || callstmt) return b;
  Str s0[6];
  for (int k = 0; k < 6; k++) s0[k] = rg[k];
  auto a0 = args;
  bool hd = has_div;
  Str d0 = div0, d1 = div1;
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

List R::walk(int lo, int hi) {
  List o = newlist();
  int i0 = lo;
  auto addb = [&](const List &dst, const List &src) {
    for (auto &b : *src) dst->push_back("    " + b);
  };
  while (i0 < hi) {
    out = o;
    if (emitgoto && gototargets.count(I(i0).address)) o->push_back("L_" + X(I(i0).address) + ":");
    std::vector<int> backc;
    for (auto &kv : tgt)
      if (kv.second == i0 && cjmp(ins[kv.first].mnemonic) && i0 < kv.first && kv.first < hi) backc.push_back(kv.first);
    if (!backc.empty()) {
      int J = *std::min_element(backc.begin(), backc.end());
      int ci = J - 1;
      while (ci > i0 && !in(I(ci).mnemonic, {"cmp", "test"})) ci--;
      const CsIns &cm = I(ci);
      string jcc = I(J).mnemonic;
      string cond;
      if (cm.mnemonic == "cmp") {
        auto p = split(cm.op_str, ", ");
        auto rv = [&](const string &t) -> string {
          if (t == "ax") return S(!rg[AX].none() ? rg[AX] : lit("ax"));
          if (t == "bx") return S(!rg[BX].none() ? rg[BX] : lit("bx"));
          Str m = memname(t);
          return truthy(m) ? m.s() : (isnum(t) ? D(num(t)) : t);
        };
        string l = rv(p[0]);
        string r = rv(at(p, 1));
        const char *jc = jcc_get(jcc);
        cond = r == "0" ? l : "(" + l + " " + (jc ? jc : (jcc == "jne" ? "!=" : "==")) + " " + r + ")";
      } else if (cm.mnemonic == "test" && has(cm.op_str, "[") && (jcc == "jne" || jcc == "je")) {
        auto p = split(cm.op_str, ", ");
        Str v = memname(p[0]);
        string k = at(p, 1);
        string vs = !v.none() ? v.s() : p[0];
        if (k != p[0]) vs = "(" + vs + " & " + (isnum(k) ? D(num(k)) : S(orS(memname(k), k.c_str()))) + ")";
        cond = jcc == "jne" ? vs : "(" + vs + " == 0)";
      } else {
        cond = S(!rg[AX].none() ? rg[AX] : lit("ax"));
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
        o->push_back("if (!(" + cond + ")) goto L_" + X(ta) + ";");
        i0 = jcc + 1;
        continue;
      }
      if (tg != NI && jcc + 1 <= tg && tg <= hi) {
        int je = tg - 1;
        int end = (je >= jcc + 1 && I(je).mnemonic == "jmp") ? tget(je) : NI;
        bool end_ok = end != NI && tg < end && end <= hi && (!has_epi || I(end).address < epi);
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
          if (eq(then_s.r[r], pre.r[r])) {
            rg[r] = then_s.r[r];
            continue;
          }
          const Str &pv = pre.r[r], &tv = then_s.r[r];
          if (!pv.none() && pv.s() != WRN[r]) o->push_back(string(WRN[r]) + " = " + pv.s() + ";");
          if (!tv.none() && tv.s() != WRN[r]) b->push_back(string(WRN[r]) + " = " + tv.s() + ";");
          rg[r] = Str();
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

string R::negate(const string &cond) {
  auto m = mt("^\\((.*) (==|!=|<=|>=|<|>) (.*)\\)$", cond);
  if (m) {
    string op = g(m, 2), n;
    if (op == "==") n = "!=";
    else if (op == "!=") n = "==";
    else if (op == "<") n = ">=";
    else if (op == ">=") n = "<";
    else if (op == ">") n = "<=";
    else n = ">";
    return "(" + g(m, 1) + " " + n + " " + g(m, 3) + ")";
  }
  return "!(" + cond + ")";
}

int R::cmp_start(int jcc_idx, int lo) {
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

void R::emit_range(int lo, int hi, const List &o) {
  int i0 = std::max(lo, emit_start);
  range_hi = hi;
  while (i0 < hi) {
    int ni = step(i0, o);
    i0 = ni > i0 ? ni : i0 + 1;
  }
}

void R::emit_block(const Blk &blk, const List &o) {
  int end = in(blk.term, {"jmp", "cond", "ret", "tailcall"}) ? blk.hi - 1 : blk.hi;
  emit_range(blk.lo, end, o);
  if (blk.term == "ret" && retval() && !rg[DX].none() && rg[DX].s() != "dx") {
    o->push_back("dx = " + rg[DX].s() + ";");
    rg[DX] = lit("dx");
  }
  if (blk.term == "ret" && !rg[AX].none() && retlive) o->push_back("return " + rg[AX].s() + ";");
  else if (blk.term == "ret" && retval()) {
    Str v = !rg[AX].none() ? rg[AX] : orS(breg.get("al"), "ax");
    o->push_back("return " + S(v) + ";");
  } else if (blk.term == "ret") o->push_back("return;");
  else if (blk.term == "tailcall") {
    const CsIns &last = I(blk.hi - 1);
    auto m = mt(NUMRX, last.op_str);
    if (m) {
      std::vector<string> a;
      for (auto it = args.rbegin(); it != args.rend(); ++it) a.push_back(S(*it));
      o->push_back("return sub_" + X(int0(g(m, 1)) & 0xFFFF) + "(" + join(a, ", ") + ");");
      args.clear();
    }
  }
}

string R::emit_cond(const Blk &blk, const List &o, const std::set<string> *flush) {
  int jcc = blk.hi - 1;
  int cs = cmp_start(jcc, blk.lo);
  emit_range(blk.lo, cs, o);
  out = o;
  cond_lo = blk.lo;
  if (flush) pin(o, *flush);
  has_cmp = false;
  return cond_at(cs).first;
}

void R::flush_calls(Snap &sn, const Snap &pre, std::vector<string> &b) {
  if (terminal(b)) return;
  for (int r : REGS_ORDER) {
    const Str v = sn.r[r];
    if (!v.none() && !eq(v, pre.r[r]) && hascall(v)) {
      b.push_back(string(WRN[r]) + " = " + v.s() + ";");
      sn.r[r] = r == AX ? lit("ax") : Str();
    }
  }
}

bool R::selfref(const string &r, const string &v) {
  std::vector<string> names = {r};
  if (in(r, {"ax", "bx", "cx", "dx"})) names.push_back(string(1, r[0]) + "l"), names.push_back(string(1, r[0]) + "h");
  if (isbyte(r)) names.push_back(parent(r));
  for (auto &n : names)
    if (srd("(?<![\\w>.])" + n + "(?![\\w(])", v)) return true;
  return false;
}

void R::plain_if(string cond, std::vector<string> &b, const Snap &pre, const Snap &then_s, const List &o) {
  bool bterm = terminal(b);
  phi_args(then_s.args, pre.args, b, *o, bterm, false);
  for (int r : REGS_ORDER) {
    string rn = WRN[r];
    if (eq(then_s.r[r], pre.r[r])) {
      rg[r] = then_s.r[r];
      continue;
    }
    if (!C.deadregs && !live(rn, i) && !hascall(pre.r[r]) && !hascall(then_s.r[r])) {
      rg[r] = Str();
      continue;
    }
    if (!pre.r[r].none() && pre.r[r].s() != rn) {
      o->push_back(rn + " = " + pre.r[r].s() + ";");
      if ((hascall(pre.r[r]) || selfref(rn, pre.r[r].s())) && has(cond, pre.r[r].s())) cond = replace_all(cond, pre.r[r].s(), rn);
    }
    if (!then_s.r[r].none() && then_s.r[r].s() != rn && !bterm) b.push_back(rn + " = " + then_s.r[r].s() + ";");
    rg[r] = Str();
  }
  for (const char *r : BYTEREG) {
    Str pv = pre.breg.get(r), tv = then_s.breg.get(r);
    if (eq(pv, tv)) {
      if (!tv.none()) breg.set(r, tv);
      continue;
    }
    if (!pv.none() && pv.s() != r) {
      o->push_back(string(r) + " = " + pv.s() + ";");
      if (selfref(r, pv.s()) && has(cond, pv.s())) cond = replace_all(cond, pv.s(), r);
    }
    if (!tv.none() && tv.s() != r && !bterm) b.push_back(string(r) + " = " + tv.s() + ";");
    breg.pop0(r);
  }
  if (anytext(b)) {
    o->push_back("if (" + cond + ") {");
    for (auto &x : b) o->push_back("    " + x);
    o->push_back("}");
  }
}

void R::emit_seq(const Seq &nodes, const List &o) {
  for (auto &n : nodes) emit_node(n, o);
}

void R::slot_args(const List &o) {
  long long n_ = (long long)args.size();
  std::vector<std::pair<long long, string>> mv;
  for (long long k = 0; k < n_; k++)
    if (S(args[k]) != "targ" + D(n_ - 1 - k)) mv.push_back({n_ - 1 - k, S(args[k])});
  std::vector<long long> done;
  for (auto &kv : mv) {
    string v = kv.second;
    std::vector<long long> hit;
    for (long long d : done)
      if (srd("\\btarg" + D(d) + "\\b", v)) hit.push_back(d);
    for (long long d : hit) {
      v = re::Regex("\\btarg" + D(d) + "\\b").sub("tsave" + D(d), v);
      long long idx = -1;
      for (long long q = (long long)o->size() - 1; q >= 0; q--)
        if (starts((*o)[q], ("targ" + D(d) + " = ").c_str())) {
          idx = q;
          break;
        }
      if (idx < 0) raise(PY_OTHER, "StopIteration");
      string sv = "tsave" + D(d) + " = targ" + D(d) + ";";
      if (!(idx && (*o)[idx - 1] == sv)) o->insert(o->begin() + idx, sv);
    }
    o->push_back("targ" + D(kv.first) + " = " + v + ";");
    done.push_back(kv.first);
  }
  args.clear();
  for (long long k = 0; k < n_; k++) args.push_back(fr("targ" + D(n_ - 1 - k)));
}

void R::emit_node(const Node &node, const List &o) {
  const std::vector<Blk> &B = *blocks;
  auto k = node.kind;
  auto addv = [&](const std::vector<string> &v) {
    for (auto &x : v) o->push_back("    " + x);
  };
  if ((k == Node::BLOCK || k == Node::IF || k == Node::IFELSE || k == Node::WHILE || k == Node::LOOP || k == Node::DOWHILE ||
       k == Node::SWITCH) &&
      !labels.empty()) {
    long long a = at(B, node.a).addr;
    if (labels.count(a) && !labeled.count(a)) {
      long long na = gotoargs.count(a) ? gotoargs[a] : 0;
      if (terminal(*o)) {
        for (auto &x : rg) x = Str();
        breg = OD();
      } else {
        i = at(B, node.a).lo - 1;
        pin(o, {"ax", "bx", "cx", "dx", "si", "di"});
        if (na && C.deadregs) slot_args(o);
        else if (na) {
          long long L = (long long)args.size(), from = std::max(0LL, L - na);
          for (long long k2 = from; k2 < L; k2++) o->push_back("targ" + D(k2 - from) + " = " + S(args[k2]) + ";");
        }
      }
      if (na && C.deadregs) {
        args.clear();
        for (long long k2 = 0; k2 < na; k2++) args.push_back(fr("targ" + D(na - 1 - k2)));
      } else if (na) {
        std::vector<Str> a2;
        if ((long long)args.size() >= na) a2.assign(args.begin(), args.end() - na);
        for (long long k2 = 0; k2 < na; k2++) a2.push_back(fr("targ" + D(k2)));
        args = a2;
      }
      o->push_back("L_" + X(a) + ":");
      labeled.insert(a);
    }
  }
  if (k == Node::BLOCK) {
    emit_block(at(B, node.a), o);
  } else if (k == Node::IF) {
    string cond = emit_cond(at(B, node.a), o);
    if (node.flag) cond = negate(cond);
    Snap pre = snap();
    List b = newlist();
    emit_seq(node.s1, b);
    Snap then_s = snap();
    flush_calls(then_s, pre, *b);
    rest(pre);
    plain_if(cond, *b, pre, then_s, o);
  } else if (k == Node::IFELSE) {
    string cond = emit_cond(at(B, node.a), o);
    Snap pre = snap();
    List tb = newlist();
    emit_seq(node.s1, tb);
    Snap ts = snap();
    flush_calls(ts, pre, *tb);
    rest(pre);
    List eb = newlist();
    emit_seq(node.s2, eb);
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
  } else if (k == Node::WHILE || k == Node::LOOP) {
    std::set<int> bids = {node.a};
    tree_bids(node.s1, bids);
    std::set<string> W = loop_written(bids);
    std::set<string> WM = W;
    for (auto &x : mem_lazy()) WM.insert(x);
    pin(o, WM);
    loops.push_back(W);
    if (k == Node::WHILE) {
      List lead = newlist();
      string cond = emit_cond(at(B, node.a), lead);
      if (node.flag) cond = negate(cond);
      if (has_break(node.s1)) pin(lead, W);
      Snap hs = snap();
      List b = newlist();
      emit_seq(node.s1, b);
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
      emit_seq(node.s1, b);
      if (!terminal(*b)) pin(b, W);
      while (!b->empty() && strip(b->back()) == "continue;") b->pop_back();
      loops.pop_back();
      for (auto &r : W) setr(r, Str());
      std::vector<string> drop;
      for (auto &kv : breg.v)
        if (W.count(parent(kv.first))) drop.push_back(kv.first);
      for (auto &x : drop) breg.pop(x);
      o->push_back("for (;;) {");
      addv(*b);
      o->push_back("}");
    }
  } else if (k == Node::DOWHILE) {
    std::set<int> bids = {node.a, node.join};
    tree_bids(node.s1, bids);
    std::set<string> W = loop_written(bids);
    std::set<string> WM = W;
    for (auto &x : mem_lazy()) WM.insert(x);
    pin(o, WM);
    loops.push_back(W);
    List b = newlist();
    Seq body_t(node.s1.begin(), node.s1.empty() ? node.s1.end() : node.s1.end() - 1);
    emit_seq(body_t, b);
    string cond = emit_cond(at(B, node.join), b, &W);
    if (node.flag) cond = negate(cond);
    loops.pop_back();
    for (auto &r : W) setr(r, Str());
    std::vector<string> drop;
    for (auto &kv : breg.v)
      if (W.count(parent(kv.first))) drop.push_back(kv.first);
    for (auto &x : drop) breg.pop(x);
    o->push_back("do {");
    addv(*b);
    o->push_back("} while (" + cond + ");");
  } else if (k == Node::SWITCH) {
    const Blk &blk = at(B, node.a);
    int hi = blk.hi;
    Str sel;
    std::set<string> allr = {"ax", "bx", "cx", "dx", "si", "di"};
    if (blk.has_cases) {
      emit_range(blk.lo, blk.selbase != NONE ? blk.lo + blk.selbase : hi - 2, o);
      if (blk.selreg.empty()) raise(PY_TYPE, "attribute name must be string, not 'NoneType'");
      const string &reg = blk.selreg;
      if (reg == "al") sel = breg.has("al") ? breg.get("al") : (!rg[AX].none() ? rg[AX] : lit("al"));
      else if (reg == "ax") sel = !rg[AX].none() ? rg[AX] : lit("ax");
      else {
        Str v_ = getr(reg);
        sel = !v_.none() ? v_ : fr(reg);
      }
      if (hascall(orS(sel, ""))) {
        materialize(o, "ax");
        sel = (reg == "ax" && !rg[AX].none()) ? rg[AX] : sel;
      }
    } else {
      std::vector<std::pair<string, string>> t3;
      for (int q = std::max(blk.lo, hi - 3); q < hi; q++) t3.push_back({I(q).mnemonic, replace_all(I(q).op_str, " ", "")});
      if (t3.size() != 3 || t3[1] != std::make_pair(string("xchg"), string("bx,ax")) ||
          !(t3[0] == std::make_pair(string("add"), string("ax,ax")) || t3[0] == std::make_pair(string("shl"), string("ax,1"))))
        raise(PY_VALUE, "unrecognized jump-table dispatch");
      emit_range(blk.lo, hi - 3, o);
      if (hascall(orS(rg[AX], ""))) materialize(o, "ax");
      sel = !rg[AX].none() ? rg[AX] : lit("ax");
    }
    pin(o, {"cx", "dx", "si", "di"});
    rg[AX] = Str();
    rg[BX] = Str();
    breg = OD();
    Snap pre = snap();
    std::vector<std::pair<std::vector<CaseVal>, std::vector<string>>> arms;
    for (auto &c : node.cases) {
      rest(pre);
      List b = newlist();
      emit_seq(c.second, b);
      if (!terminal(*b)) pin(b, allr);
      arms.push_back({c.first, *b});
    }
    for (auto &r : allr) setr(r, Str());
    breg = OD();
    auto isdef = [](const std::vector<CaseVal> &v) {
      for (auto &x : v)
        if (x.is_default) return true;
      return false;
    };
    bool brk = false;
    for (auto &c : node.cases)
      if (has_break(c.second)) brk = true;
    if (brk) {
      string t = newt();
      o->push_back(t + " = " + S(sel) + ";");
      std::vector<std::pair<std::vector<CaseVal>, std::vector<string>>> a2;
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
          for (auto &v : vals) cs.push_back("(" + t + " == " + D(v.v) + ")");
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
      o->push_back("switch (" + S(sel) + ") {");
      for (auto &x : arms) {
        for (auto &v : x.first) o->push_back(v.is_default ? "default:" : "case " + D(v.v) + ":");
        addv(x.second);
        if (!terminal(x.second)) o->push_back("    break;");
      }
      o->push_back("}");
    }
  } else if (k == Node::BREAK) {
    if (!loops.empty()) pin(o, loops.back());
    o->push_back("break;");
  } else if (k == Node::CONTINUE) {
    if (!loops.empty()) pin(o, loops.back());
    o->push_back("continue;");
  } else if (k == Node::GOTO) {
    if (node.a != NONE) {
      long long ta = at(B, node.a).addr;
      i = at(B, node.a).lo - 1;
      pin(o, {"ax", "bx", "cx", "dx", "si", "di"});
      if (!args.empty()) {
        gotoargs[ta] = std::max(gotoargs.count(ta) ? gotoargs[ta] : 0LL, (long long)args.size());
        if (C.deadregs) slot_args(o);
        else
          for (size_t k2 = 0; k2 < args.size(); k2++) o->push_back("targ" + D((long long)k2) + " = " + S(args[k2]) + ";");
      }
      gototargets.insert(ta);
      o->push_back("goto L_" + X(ta) + ";");
    }
  }
}

// ------------------------------------------------------------------------------------ drivers
std::vector<CsIns> decode(const std::vector<uint8_t> &code, const EcCfg &C) {
  auto lin = disasm16(code, 0);
  bool any = false;
  for (auto &x : lin)
    if (x.mnemonic == "jmp" && has(x.op_str, "[")) any = true;
  if (!C.hooks.sw || !any) return lin;
  std::map<long long, CsIns> seen;
  std::vector<long long> work = {0};
  long long n = (long long)code.size();
  while (!work.empty()) {
    long long a = work.back();
    work.pop_back();
    while (0 <= a && a < n && !seen.count(a)) {
      auto one = disasm16(code.data() + a, (size_t)std::min<long long>(16, n - a), a);
      if (one.empty()) break;
      CsIns ii = one[0];
      seen[a] = ii;
      const string mn = ii.mnemonic;
      bool m = isnum(ii.op_str);
      if (mn == "ret" || mn == "retf") break;
      if (mn == "jmp") {
        if (m) work.push_back(int0(ii.op_str));
        else {
          std::vector<long long> t;
          if (C.hooks.sw(a, t) == 1)
            for (auto x : t) work.push_back(x);
        }
        break;
      }
      if (cjmp(mn) && m) work.push_back(int0(ii.op_str));
      a += ii.size;
    }
  }
  std::vector<CsIns> o;
  for (auto &kv : seen) o.push_back(kv.second);
  return o;
}

std::vector<CsIns> unshare_jcc(const std::vector<CsIns> &ins) {
  std::unordered_map<long long, int> a2i;
  for (size_t k = 0; k < ins.size(); k++) a2i[ins[k].address] = (int)k;
  std::vector<CsIns> o;
  bool changed = false;
  auto fake = [](long long a, int size, const string &mn, const string &op) {
    CsIns x;
    x.address = a, x.size = size, x.mnemonic = mn, x.op_str = op;
    return x;
  };
  for (size_t k = 0; k < ins.size(); k++) {
    const CsIns &x = ins[k];
    if (x.mnemonic == "jmp" && k &&
        in(ins[k - 1].mnemonic, {"cmp", "test", "or", "and", "xor", "add", "sub", "inc", "dec", "neg"}) && x.size >= 2) {
      int t = NI;
      if (isnum(x.op_str)) {
        auto it = a2i.find(int0(x.op_str));
        if (it != a2i.end()) t = it->second;
      }
      if (t != NI && t + 1 < (int)ins.size() && cjmp(ins[t].mnemonic) && !in(ins[t].mnemonic, {"jcxz", "jecxz"}) &&
          isnum(ins[t].op_str)) {
        o.push_back(fake(x.address, 1, ins[t].mnemonic, ins[t].op_str));
        o.push_back(fake(x.address + 1, x.size - 1, "jmp", "0x" + X(ins[t + 1].address)));
        changed = true;
        continue;
      }
      if (t != NI && t + 1 < (int)ins.size() && in(ins[t].mnemonic, {"sbb", "adc", "rcl", "rcr", "cmc", "lahf", "pushf"}) &&
          !in(ins[t + 1].mnemonic, {"sbb", "adc", "rcl", "rcr"}) && !cjmp(ins[t + 1].mnemonic)) {
        o.push_back(fake(x.address, 1, ins[t].mnemonic, ins[t].op_str));
        o.push_back(fake(x.address + 1, x.size - 1, "jmp", "0x" + X(ins[t + 1].address)));
        changed = true;
        continue;
      }
    }
    o.push_back(x);
  }
  return changed ? o : ins;
}

void prologue(const std::vector<CsIns> &ins, const EcCfg &C, long long &fs, int &k, bool &msc3) {
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
  if (C.masm_cret && n > 4 && ins[1].op_str == "bp, sp" && ins[2].mnemonic == "push" && ins[2].op_str == "di" &&
      ins[3].mnemonic == "push" && ins[3].op_str == "si") {
    fs = 4;
    k = 4;
    if (ins[k].mnemonic == "sub" && starts(ins[k].op_str, "sp,")) {
      try {
        fs += int16(strip(at(split(ins[k].op_str, ","), 1)));
        k++;
      } catch (const PyError &e) {
        if (e.kind != PY_VALUE) throw;
      }
    }
    msc3 = true;
    return;
  }
  fs = 0;
  k = 1;
  if (ins[k].mnemonic == "sub" && starts(ins[k].op_str, "sp")) {
    fs = int16(at(split(ins[k].op_str, ","), 1));
    k++;
    while (k < n && !has(ins[k].op_str, "bp, sp")) k++;
  }
  k++;
  msc3 = false;
  if (fs == 0 && k + 1 < n && in(ins[k + 1].mnemonic, {"call", "lcall"})) {
    if (ins[k].mnemonic == "mov" && starts(ins[k].op_str, "ax,")) {
      try {
        fs = int16(strip(at(split(ins[k].op_str, ","), 1)));
        msc3 = true;
        k += 2;
      } catch (const PyError &e) {
        if (e.kind != PY_VALUE) throw;
      }
    } else if (ins[k].mnemonic == "xor" && replace_all(ins[k].op_str, " ", "") == "ax,ax") {
      fs = 0, msc3 = true, k += 2;
    }
  }
  if (fs == 0 && !msc3 && k < n && ins[1].op_str == "bp, sp" && ins[k].mnemonic == "sub" && starts(ins[k].op_str, "sp,")) {
    try {
      fs = int16(strip(at(split(ins[k].op_str, ","), 1)));
      msc3 = true;
      k++;
    } catch (const PyError &e) {
      if (e.kind != PY_VALUE) throw;
    }
  }
  if (!msc3 && C.frame_after_bp && n > 1 && ins[1].op_str == "bp, sp") msc3 = true;
  if (ins[1].op_str == "bp, sp")
    while (k < n && ins[k].mnemonic == "push" && in(ins[k].op_str, {"si", "di"}) && popped(k)) k++;
}

std::vector<string> params_of(const R &r) {
  std::vector<string> p;
  long long m = (r.floatfn || r.longfn) ? r.lmax : r.maxidx;
  for (long long j = 0; j <= m; j++) p.push_back(cbinv::chr('a' + j));
  return p;
}
EcResult finish(const R &r, long long fs, std::vector<string> stmts) {
  EcResult res;
  res.fs = fs;
  res.stmts = std::move(stmts);
  res.params = params_of(r);
  res.pt = r.floatfn ? "float" : r.longfn ? "long" : "int";
  res.loc.assign(r.locals.begin(), r.locals.end());
  return res;
}

}  // namespace

EcResult ec_recover(const std::vector<uint8_t> &code, long long base, const std::map<long long, std::string> &cxmap,
                    const EcCfg &C, EcLast &LAST) {
  auto ins = disasm16(code, 0);
  if (ins.size() < 2) return EcResult();
  long long n = (long long)ins.size();
  long long fs = 0;
  int k = 1;
  if (ins[k].mnemonic == "sub" && starts(ins[k].op_str, "sp")) {
    fs = int16(at(split(ins[k].op_str, ","), 1));
    k++;
    while (k < n && !has(ins[k].op_str, "bp, sp")) k++;
  }
  k++;
  bool msc3 = false;
  if (fs == 0 && k + 1 < n && ins[k + 1].mnemonic == "call") {
    if (ins[k].mnemonic == "mov" && starts(ins[k].op_str, "ax,")) {
      try {
        fs = int16(strip(at(split(ins[k].op_str, ","), 1)));
        msc3 = true;
        k += 2;
      } catch (const PyError &e) {
        if (e.kind != PY_VALUE) throw;
      }
    } else if (ins[k].mnemonic == "xor" && replace_all(ins[k].op_str, " ", "") == "ax,ax") {
      fs = 0, msc3 = true, k += 2;
    }
  }
  if (fs == 0 && !msc3 && C.frame_after_bp && k < n && ins[1].op_str == "bp, sp" && at(ins, k).mnemonic == "sub" &&
      starts(ins[k].op_str, "sp,")) {
    try {
      fs = int16(strip(at(split(ins[k].op_str, ","), 1)));
      msc3 = true;
      k++;
    } catch (const PyError &e) {
      if (e.kind != PY_VALUE) throw;
    }
  }
  if (!msc3 && C.frame_after_bp && ins[1].op_str == "bp, sp") msc3 = true;
  LAST = EcLast();
  LAST.has_saved = LAST.has_regparam = true;
  if (msc3) {
    while (k < n && ins[k].mnemonic == "push" && in(ins[k].op_str, {"si", "di"})) LAST.saved.push_back(ins[k].op_str), k++;
    while (k < n && ins[k].mnemonic == "mov") {
      auto m_ = mt("(si|di), word ptr \\[bp \\+ (0x[0-9a-f]+|\\d+)\\]$", ins[k].op_str);
      if (!m_) break;
      string r = g(m_, 1);
      bool saved = std::find(LAST.saved.begin(), LAST.saved.end(), r) != LAST.saved.end();
      bool inrp = false;
      for (auto &kv : LAST.regparam)
        if (kv.first == r) inrp = true;
      if (!saved || inrp) break;
      if (C.regparam_reread) {
        bool again = false;
        for (long long q = k + 1; q < n; q++)
          if (has(ins[q].op_str, "[bp + " + g(m_, 2) + "]")) again = true;
        if (again) break;
      }
      LAST.regparam.push_back({r, floordiv(int0(g(m_, 2)) - 4, 2)});
      k++;
    }
  }
  R r(C, LAST, ins, fs, base, cxmap, msc3);
  for (auto &kv : LAST.regparam) r.maxidx = std::max(r.maxidx, kv.second);
  List st = r.walk(k, (int)n);
  std::vector<string> stmts = *st;
  if (!r.gototargets.empty()) {
    r.reset();
    r.emitgoto = true;
    stmts = *r.walk(k, (int)n);
    stmts = ec_fold_shortcircuit(stmts);
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
    stmts = ec_dead_store_recover(s2, rc);
  }
  return finish(r, fs, stmts);
}

EcResult ec_recover_cfg(const std::vector<uint8_t> &code, long long base, const std::map<long long, std::string> &cxmap,
                        const EcCfg &C, EcLast &LAST) {
  auto ins = decode(code, C);
  if (ins.size() < 2) return EcResult();
  if (C.deadregs) ins = unshare_jcc(ins);
  long long fs;
  int k;
  bool msc3;
  prologue(ins, C, fs, k, msc3);
  std::vector<Ins> fi;
  std::map<long long, std::vector<long long>> sw;
  for (auto &x : ins) {
    fi.push_back({x.address, x.mnemonic, x.op_str});
    if (C.hooks.sw && x.mnemonic == "jmp" && has(x.op_str, "[")) {
      std::vector<long long> t;
      if (C.hooks.sw(x.address, t) == 1) sw[x.address] = t;
    }
  }
  std::vector<Block> fb = build_cfg(fi, sw, C.cmpswitch);
  std::vector<Blk> blocks;
  for (auto &b : fb) blocks.push_back(Blk{b.lo, b.hi, at(ins, b.lo).address, b.term, b.has_cases, b.selreg, b.selbase});
  Seq tree = structure(fb, 0, C.nocap);
  R r(C, LAST, ins, fs, base, cxmap, msc3);
  r.emit_start = k;
  r.blocks = &blocks;
  List top = newlist();
  r.out = top;
  r.emit_seq(tree, top);
  std::vector<string> stmts = *top;
  std::unique_ptr<R> r2;
  if (!r.gototargets.empty()) {
    r2.reset(new R(C, LAST, ins, fs, base, cxmap, msc3));
    r2->emit_start = k;
    r2->blocks = &blocks;
    r2->labels = r.gototargets;
    r2->gotoargs = r.gotoargs;
    List t2 = newlist();
    r2->out = t2;
    r2->emit_seq(tree, t2);
    stmts = *t2;
  }
  const R &rf = r2 ? *r2 : r;
  stmts = ec_strip_tail_returns(ec_tidy(stmts));
  if (C.deadregs) {
    stmts = ec_dead_regs(stmts);
    if (C.mark || C.unsigned_cast) stmts = ec_fold_ladders(stmts, C.mark);
    stmts = ec_tidy(ec_fold_goto_or(stmts));
  }
  if (!C.mark) {
    for (auto &x : stmts) x = RX("^(\\s*)USE\\((.*)\\);$").sub("\\1/* the next call takes \\2 in registers */", x);
    stmts = ec_fold_longs(stmts);
    std::vector<string> used;
    for (const char *x : {"ax", "bx", "cx", "dx", "si", "di"}) {
      const re::Regex &rx = RX("(?<![\\w>.])" + string(x) + "(?![\\w(])");
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

}  // namespace cbinv
