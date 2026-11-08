// codebreaker/exact/habits/boolx.cpp -- C++ port of codebreaker.exact.habits.boolx.

#include "codebreaker/exact/habits/boolx.hpp"

#include <algorithm>

namespace codebreaker::exact::habits::boolx {

namespace {

const py::re::Pattern NUM(py::str("0x[0-9a-f]+|\\d+"), 0);   // (the constant operands: re.fullmatch(r"0x[0-9a-f]+|\d+", x))
const py::re::Pattern WORDS(py::str("\\b(si|di)\\b"), 0);
const py::re::Pattern INSN(py::str("(\\w+)\\s*(.*)"), 0);

// int(text, 0): a literal with its own base prefix; a leading zero is not allowed before more digits.
int64_t int0(const py::str& s) {
    const std::string t = s.strip().utf8();
    size_t i = (!t.empty() && (t[0] == '+' || t[0] == '-')) ? 1 : 0;
    if (t.size() > i + 1 && t[i] == '0' && ((t[i + 1] >= '0' && t[i + 1] <= '9') || t[i + 1] == '_')) {
        for (size_t j = i; j < t.size(); ++j) {
            if (t[j] != '0' && t[j] != '_') throw py::ValueError("invalid literal for int() with base 0: " + py::repr(s).utf8());
        }
    }
    return py::int_from_str(s, 0);
}

py::str vstr(const Key& k, int64_t v) {
    if (k.kind == "cmp") return py::str(v == LT ? "lt" : v == EQ ? "eq" : "gt");
    return py::to_str(v);
}

Val integer(int64_t i) {
    Val v;
    v.kind = Val::Int;
    v.i = i;
    return v;
}

Val reg(const py::str& r) {
    Val v;
    v.kind = Val::Reg;
    v.s = r;
    return v;
}

Val mem(const py::str& key) {
    Val v;
    v.kind = Val::Mem;
    v.s = key;
    return v;
}

Val bits(int64_t b) {
    Val v;
    v.kind = Val::Bits;
    v.i = b;
    return v;
}

// the flags: none yet, ("cmp", outcome) or ("log", zf, cf)
struct Flags {
    enum Kind { None, Cmp, Log } kind = None;
    int64_t outcome = 0;
    std::optional<bool> zf, cf;
};

Flags cmp_flags(int64_t outcome) {
    Flags f;
    f.kind = Flags::Cmp;
    f.outcome = outcome;
    return f;
}

Flags log_flags(std::optional<bool> zf, std::optional<bool> cf) {
    Flags f;
    f.kind = Flags::Log;
    f.zf = zf;
    f.cf = cf;
    return f;
}

bool truthy(const std::optional<bool>& b) { return b.has_value() && *b; }

const std::vector<int64_t> BIT_DOM = {0, 1};
const std::vector<int64_t> CMP_DOM = {LT, EQ, GT};

}  // namespace

py::str Key::repr() const {
    if (kind == "bool") return py::str("('bool', ") + py::repr(a) + py::str(")");
    if (kind == "bits") return py::str("('bits', ") + py::repr(a) + py::str(", ") + py::to_str(n) + py::str(")");
    return py::str("('cmp', ") + py::repr(a) + py::str(", ") + py::repr(b) + py::str(")");
}

py::str Key::tail_repr() const {
    if (kind == "bool") return py::str("(") + py::repr(a) + py::str(",)");
    if (kind == "bits") return py::str("(") + py::repr(a) + py::str(", ") + py::to_str(n) + py::str(")");
    return py::str("(") + py::repr(a) + py::str(", ") + py::repr(b) + py::str(")");
}

bool operator<(const Key& x, const Key& y) {
    return std::tie(x.kind, x.a, x.b, x.n) < std::tie(y.kind, y.a, y.b, y.n);
}

py::str Val::repr() const {
    switch (kind) {
        case Int: return py::to_str(i);
        case Mem: return py::str("('mem', ") + py::repr(s) + py::str(")");
        case Reg: return py::str("('reg', ") + py::repr(s) + py::str(")");
        default: return py::str("('bits', ") + py::to_str(i) + py::str(")");
    }
}

const py::dict<py::str, py::str> R8 = {
    {py::str("al"), py::str("ax")}, {py::str("cl"), py::str("cx")}, {py::str("dl"), py::str("dx")}, {py::str("bl"), py::str("bx")}};
const py::list<py::str> R16 = {py::str("ax"), py::str("cx"), py::str("dx"), py::str("bx")};
const py::list<py::str> JCC = {py::str("je"), py::str("jne"), py::str("jb"), py::str("jae"), py::str("ja"), py::str("jbe")};
const py::re::Pattern MEM(py::str("(?:byte ptr |word ptr )?\\[([^\\]]+)\\]"), 0);

Result run(const py::list<Instruction>& ins, int64_t lo, int64_t hi, const Assignment& assign,
           const std::optional<py::dict<py::str, py::str>>& init) {
    std::map<int64_t, int64_t> at;
    for (int64_t k = 0; k < py::len(ins); ++k) at[std::get<0>(ins[k])] = k;
    py::dict<py::str, Val> regs;
    py::dict<py::str, Val> temps;
    py::dict<py::str, py::str> base;
    if (init && !init->empty()) {
        base = init->copy();
    } else {
        base.set(py::str("si"), py::str("si0"));
        base.set(py::str("di"), py::str("di0"));
    }
    Flags flags;
    int64_t k = 0;
    int64_t steps = 0;

    auto atom = [&](const Key& key, const std::vector<int64_t>& dom) -> int64_t {
        auto it = assign.find(key);
        if (it == assign.end()) throw Need(key, dom);
        return it->second;
    };
    auto memkey = [&](const py::str& inner) -> py::str {
        return WORDS.sub_fn([&](const py::re::Match& m) { return py::str("<") + base.at(m.group(1)) + py::str(">"); }, inner);
    };
    // an operand as a value: an int, or ('mem', key)
    auto val = [&](const py::str& op0) -> Val {
        py::str op = op0.strip();
        if (R16.contains(op)) return regs.get(op, reg(op));
        if (R8.contains(op)) return regs.get(R8.at(op), reg(R8.at(op)));
        if (NUM.fullmatch(op)) return integer(int0(op));
        auto m = MEM.fullmatch(op);
        if (m) {
            py::str key = memkey(m->group(1));
            if (temps.contains(key)) return temps.at(key);
            return mem(key);
        }
        throw No(py::str("operand ") + op);
    };
    auto boolean = [&](const Val& v) -> int64_t {
        if (v.kind == Val::Int) return v.i & 1;
        if (v.kind == Val::Mem) {
            Key key;
            key.kind = py::str("bool");
            key.a = v.s;
            return atom(key, BIT_DOM);
        }
        throw No(py::str("not a Boolean: ") + v.repr());
    };

    while (k < py::len(ins)) {
        steps += 1;
        if (steps > 400) throw No(py::str("loop"));
        const Instruction& in = ins[k];
        const py::str t = std::get<1>(in);
        auto m0 = INSN.match(t);
        if (!m0) throw py::AttributeError("'NoneType' object has no attribute 'group'");
        const py::str op = m0->group(1);
        const py::str rest = m0->group(2);
        py::list<py::str> a;
        if (!rest.empty()) {
            for (const py::str& x : py::split(rest, py::str(","))) a.append(x.strip());
        }
        if (JCC.contains(op) || op == "jmp") {
            int64_t tgt = int0(a[0]);
            bool take;
            if (op == "jmp") {
                take = true;
            } else {
                if (flags.kind == Flags::None) throw No(py::str("jump without flags"));
                if (flags.kind == Flags::Cmp) {
                    int64_t s = flags.outcome;
                    take = op == "je" ? s == EQ : op == "jne" ? s != EQ : op == "jb" ? s == LT
                         : op == "jae" ? s != LT : op == "ja" ? s == GT : s != GT;
                } else {
                    if (op == "je" || op == "jne") {
                        take = op == "je" ? truthy(flags.zf) : !truthy(flags.zf);
                    } else if (op == "jb" || op == "jae") {
                        if (!flags.cf.has_value()) throw No(py::str("carry not known"));
                        take = op == "jb" ? *flags.cf : !*flags.cf;
                    } else {
                        throw No(op + py::str(" after a logical operation"));
                    }
                }
            }
            if (take) {
                if (tgt == hi) return Result{false, 0};
                if (lo <= tgt && tgt < hi) {
                    auto it = at.find(tgt);
                    if (it == at.end()) throw No(py::str("jump into an instruction"));
                    k = it->second;
                    continue;
                }
                return Result{true, tgt};
            }
            k += 1;
            continue;
        }
        if (op == "mov") {
            if (py::len(a) != 2) {
                throw py::ValueError(py::len(a) < 2 ? "not enough values to unpack (expected 2, got " + std::to_string(py::len(a)) + ")"
                                                    : "too many values to unpack (expected 2)");
            }
            const py::str d = a[0];
            const py::str s = a[1];
            if (d == "si" || d == "di") {
                auto ms = MEM.fullmatch(s);
                base.set(d, ms ? memkey(ms->group(1)) : s);
                k += 1;
                continue;
            }
            if (R16.contains(d) || R8.contains(d)) {
                regs.set(R8.get(d, d), val(s));
                k += 1;
                continue;
            }
            auto md = MEM.fullmatch(d);
            if (md && md->group(1).startswith("bp - ") && (R16.contains(s) || R8.contains(s))) {
                temps.set(memkey(md->group(1)), val(s));
                k += 1;
                continue;
            }
            throw No(py::str("store ") + t);
        }
        if (op == "inc" && R16.contains(a[0])) {
            auto v = regs.get(a[0]);
            if (!v || v->kind != Val::Int) throw No(py::str("inc of an unknown"));
            regs.set(a[0], integer(v->i + 1));
            k += 1;
            continue;
        }
        if (op == "xor" && (a[0] == "ah" || a[0] == "ch" || a[0] == "dh" || a[0] == "bh") && a[1] == a[0]) {
            k += 1;
            continue;
        }
        if (op == "xor" && R8.contains(a[0]) && a[1] == "1") {
            py::str r = R8.at(a[0]);
            int64_t nv = 1 - boolean(regs.get(r, reg(r)));
            regs.set(r, integer(nv));
            flags = log_flags(nv == 0, false);
            k += 1;
            continue;
        }
        if ((op == "and" || op == "or") && R8.contains(a[0])) {
            py::str r = R8.at(a[0]);
            int64_t x = boolean(regs.get(r, reg(r)));
            int64_t y = boolean(val(a[1]));
            int64_t nv = op == "and" ? (x & y) : (x | y);
            regs.set(r, integer(nv));
            flags = log_flags(nv == 0, false);
            k += 1;
            continue;
        }
        if (op == "and" && R16.contains(a[0]) && NUM.fullmatch(a[1])) {
            auto v = regs.get(a[0]);
            if (v && v->kind == Val::Mem) {
                Key key;
                key.kind = py::str("bits");
                key.a = v->s;
                key.n = int0(a[1]);
                int64_t b = atom(key, BIT_DOM);
                regs.set(a[0], bits(b));
                flags = log_flags(b == 0, false);
                k += 1;
                continue;
            }
            throw No(py::str("and of ") + (v ? v->repr() : py::str("None")));
        }
        if (op == "cmp") {
            Val x = val(a[0]);
            Val y = val(a[1]);
            if (x.kind == Val::Int && y.kind == Val::Int) {
                flags = cmp_flags(x.i < y.i ? LT : x.i == y.i ? EQ : GT);
            } else {
                Key key;
                key.kind = py::str("cmp");
                key.a = x.repr();
                key.b = y.repr();
                flags = cmp_flags(atom(key, CMP_DOM));
            }
            k += 1;
            continue;
        }
        if (op == "test" && a[1] == "1" && MEM.fullmatch(a[0])) {
            flags = log_flags(boolean(val(a[0])) == 0, false);
            k += 1;
            continue;
        }
        if (op == "shr" && R16.contains(a[0]) && a[1] == "1") {
            auto v = regs.get(a[0]);
            if (!v) throw No(py::str("shr of an unknown"));
            int64_t b = boolean(*v);
            flags = log_flags(std::nullopt, b == 1);
            regs.set(a[0], integer(0));
            k += 1;
            continue;
        }
        throw No(py::str("instruction ") + t);
    }
    return Result{false, 0};
}

Outcomes outcomes(const py::list<Instruction>& ins, int64_t lo, int64_t hi,
                  const std::optional<py::dict<py::str, py::str>>& init) {
    Outcomes out;
    std::vector<Assignment> todo(1);
    while (!todo.empty()) {
        Assignment asg = todo.back();
        todo.pop_back();
        try {
            out[asg] = run(ins, lo, hi, asg, init);
        } catch (const Need& n) {
            for (int64_t v : n.dom) {
                Assignment d = asg;
                d[n.key] = v;
                todo.push_back(d);
            }
        }
        if (static_cast<int64_t>(out.size() + todo.size()) > 300000) throw No(py::str("too many cases"));
    }
    return out;
}

SameResult same(const py::list<Instruction>& iw, int64_t lw, int64_t hw,
                const py::list<Instruction>& ig, int64_t lg, int64_t hg,
                const std::optional<py::dict<py::str, py::str>>& initw,
                const std::optional<py::dict<py::str, py::str>>& initg) {
    auto no = [](const py::str& why) { SameResult r; r.same = false; r.why = why; return r; };
    Outcomes A, B;
    try {
        A = outcomes(iw, lw, hw, initw);
        B = outcomes(ig, lg, hg, initg);
    } catch (const No& e) {
        return no(py::str(e.what()));
    }
    std::map<Key, int64_t> doms;   // key -> the number of values it takes
    for (const Outcomes* side : {&A, &B}) {
        for (const auto& kv : *side) {
            for (const auto& item : kv.first) doms.emplace(item.first, item.first.kind == "cmp" ? 3 : 2);
        }
    }
    std::vector<Key> ks;
    for (const auto& kv : doms) ks.push_back(kv.first);
    std::stable_sort(ks.begin(), ks.end(), [](const Key& x, const Key& y) { return x.repr() < y.repr(); });
    std::vector<std::tuple<int64_t, int64_t>> elses;
    int64_t n = 1;
    for (const Key& k : ks) {
        n = std::min<int64_t>(n * doms.at(k), INT64_MAX / 4);   // (only compared with the limit below)
    }
    bool ends = false, outs = false;
    for (const auto& kv : A) (kv.second.out ? outs : ends) = true;
    if (ks.empty() || (ends + outs) < 2) return no(py::str("not a condition: it always goes the same way"));
    if (n > 300000) return no(py::str("too many cases"));
    try {
        std::vector<int64_t> idx(ks.size(), 0);
        while (true) {
            Assignment asg;
            for (size_t i = 0; i < ks.size(); ++i) asg[ks[i]] = idx[i];
            Result ra = run(iw, lw, hw, asg, initw);
            Result rb = run(ig, lg, hg, asg, initg);
            if (ra.out != rb.out) {
                py::list<py::str> parts;
                for (const Key& k : ks) parts.append(k.tail_repr() + py::str("=") + vstr(k, asg.at(k)));
                return no(py::str("with ") + py::join(py::str(", "), parts) + py::str(" the original ") +
                          py::str(ra.out ? "out" : "end") + py::str(", the compiled ") + py::str(rb.out ? "out" : "end"));
            }
            if (ra.out) {
                auto e = std::make_tuple(ra.target, rb.target);
                if (std::find(elses.begin(), elses.end(), e) == elses.end()) elses.push_back(e);
            }
            // the next combination (the last unknown varies fastest, as itertools.product does)
            bool carried = true;
            for (size_t p = ks.size(); p > 0 && carried; --p) {
                if (++idx[p - 1] < doms.at(ks[p - 1])) carried = false;
                else idx[p - 1] = 0;
            }
            if (carried) break;
        }
    } catch (const No&) {
        return no(py::str("a case one of them does not have"));
    } catch (const Need&) {
        return no(py::str("a case one of them does not have"));
    }
    std::vector<int64_t> as, bs;
    for (const auto& e : elses) {
        if (std::find(as.begin(), as.end(), std::get<0>(e)) == as.end()) as.push_back(std::get<0>(e));
        if (std::find(bs.begin(), bs.end(), std::get<1>(e)) == bs.end()) bs.push_back(std::get<1>(e));
    }
    if (as.size() > 1 || bs.size() > 1) return no(py::str("more than one way out"));
    SameResult r;
    r.same = true;
    if (!elses.empty()) r.elses = elses.front();
    return r;
}

}  // namespace codebreaker::exact::habits::boolx
