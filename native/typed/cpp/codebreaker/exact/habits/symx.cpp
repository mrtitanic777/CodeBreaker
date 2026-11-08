// codebreaker/exact/habits/symx.cpp -- C++ from codebreaker.exact.habits.symx.

#include "codebreaker/exact/habits/symx.hpp"

#include <vector>

namespace codebreaker::exact::habits::symx {

const py::list<py::str> R16{py::str("ax"), py::str("cx"), py::str("dx"), py::str("bx"), py::str("si"), py::str("di")};
const py::dict<py::str, py::str> LO{{py::str("al"), py::str("ax")}, {py::str("cl"), py::str("cx")},
                                    {py::str("dl"), py::str("dx")}, {py::str("bl"), py::str("bx")}};
const py::dict<py::str, py::str> HI{{py::str("ah"), py::str("ax")}, {py::str("ch"), py::str("cx")},
                                    {py::str("dh"), py::str("dx")}, {py::str("bh"), py::str("bx")}};
const py::dict<py::str, py::str> COMM{{py::str("add"), py::str("+")}, {py::str("or"), py::str("|")},
                                      {py::str("and"), py::str("&")}, {py::str("xor"), py::str("^")}};
const py::dict<py::str, int64_t> UNIT{{py::str("+"), 0}, {py::str("*"), 1}, {py::str("|"), 0}, {py::str("&"), 0xFFFF}, {py::str("^"), 0}};

namespace {

// How a kind of Expr lays out its items after the tag: i = the int n, s = the str name, e = the next
// value in args, L = all of args (a list of terms), o = the next value in args or None if there is none.
const char* shape_of(const py::str& tag) {
    if (tag == "c") return "i";
    if (tag == "r") return "s";
    if (tag == "neg" || tag == "lo" || tag == "zx" || tag == "high" || tag == "res") return "e";
    if (tag == "m") return "esi";
    if (tag == "lohi" || tag == "cmp") return "ee";
    if (tag == "ord") return "io";
    if (tag == "shr") return "ei";
    if (tag == "div" || tag == "mod") return "eee";
    if (tag == "f") return "";
    return "L";   // + * | & ^ (and nary of any other tag, which the python builds the same way)
}

Expr node(const char* tag, std::initializer_list<Expr> args) {
    Expr e;
    e.tag = py::str(tag);
    e.args = py::list<Expr>(args);
    return e;
}

Expr F() {
    Expr e;
    e.tag = py::str("f");
    return e;
}

Expr reg_(const char* name) {
    Expr e;
    e.tag = py::str("r");
    e.name = py::str(name);
    return e;
}

// ("ord", opcode, flags): flags None is no args
Expr ord_node(int64_t opcode, const std::optional<Expr>& flags) {
    Expr e;
    e.tag = py::str("ord");
    e.n = opcode;
    if (flags) e.args.append(*flags);
    return e;
}

Expr shr_node(const Expr& x, int64_t n) {
    Expr e = node("shr", {x});
    e.n = n;
    return e;
}

Expr mem_node(const Expr& a, const py::str& size, int64_t k) {
    Expr e = node("m", {a});
    e.name = size;
    e.n = k;
    return e;
}

Expr res_node(const Expr& v) { return node("res", {v}); }

// int(text, 0) for the digits the regexes let through: 0x hex or decimal, a minus sign; Python
// refuses a decimal with a leading zero and a digit after it (ValueError).
int64_t int0(const py::str& text) {
    std::string t = text.utf8();
    size_t i = 0;
    bool negative = false;
    if (i < t.size() && (t[i] == '-' || t[i] == '+')) negative = t[i++] == '-';
    int base = 10;
    if (i + 1 < t.size() && t[i] == '0' && (t[i + 1] == 'x' || t[i + 1] == 'X')) {
        base = 16;
        i += 2;
    } else if (i + 1 < t.size() && t[i] == '0') {
        for (size_t j = i; j < t.size(); j++)
            if (t[j] != '0') throw py::ValueError("invalid literal for int() with base 0: " + py::repr(text).utf8());
    }
    if (i >= t.size()) throw py::ValueError("invalid literal for int() with base 0: " + py::repr(text).utf8());
    uint64_t v = 0;
    for (; i < t.size(); i++) {
        char c = t[i];
        int d = (c >= '0' && c <= '9') ? c - '0' : (c >= 'a' && c <= 'f') ? c - 'a' + 10 : (c >= 'A' && c <= 'F') ? c - 'A' + 10 : 99;
        if (d >= base) throw py::ValueError("invalid literal for int() with base 0: " + py::repr(text).utf8());
        if (v > (static_cast<uint64_t>(INT64_MAX) - d) / static_cast<uint64_t>(base)) throw py::OverflowError("int too large for int64");   // (Python's would go on)
        v = v * static_cast<uint64_t>(base) + static_cast<uint64_t>(d);
    }
    return negative ? -static_cast<int64_t>(v) : static_cast<int64_t>(v);
}

py::str join(const std::vector<py::str>& parts, const char* sep) {
    py::str out;
    for (size_t i = 0; i < parts.size(); i++) {
        if (i) out = out + py::str(sep);
        out = out + parts[i];
    }
    return out;
}

}  // namespace

bool operator==(const Expr& a, const Expr& b) { return a.tag == b.tag && a.n == b.n && a.name == b.name && a.args == b.args; }

Expr C(int64_t n) {
    Expr e;
    e.tag = py::str("c");
    e.n = n & 0xFFFF;
    return e;
}

bool isc(const Expr& x) { return x.tag == "c" || x.tag == "f"; }

int64_t fold(const py::str& tag, int64_t a, int64_t b) {
    if (tag == "+") return a + b;
    if (tag == "*") return a * b;
    if (tag == "|") return a | b;
    if (tag == "&") return a & b;
    return a ^ b;
}

Expr nary(const py::str& tag, const py::list<Expr>& xs) {
    py::list<Expr> out;
    std::optional<Expr> k;
    for (const Expr& x : xs) {
        py::list<Expr> parts = (x.tag == tag) ? x.args : py::list<Expr>{x};
        for (const Expr& y : parts) {
            if (isc(y)) {
                if (!k) {
                    k = y;
                } else if (k->tag == "f" || y.tag == "f") {
                    k = F();
                } else {
                    k = C(fold(tag, k->n, y.n));
                }
            } else {
                out.append(y);
            }
        }
    }
    if (k && *k != C(UNIT.at(tag))) out.append(*k);
    if (out.empty()) return C(UNIT.at(tag));
    if (out.size() == 1) return out[0];
    out.sort([](const Expr& e) { return canon(e); });
    Expr e;
    e.tag = tag;
    e.args = out;
    return e;
}

Expr neg(const Expr& x) {
    if (x.tag == "c") return C(-x.n);
    return node("neg", {x});
}

py::str canon(const py::list<Expr>& x) {
    std::vector<py::str> parts;
    for (const Expr& y : x) parts.push_back(canon(y));
    return py::str("[") + join(parts, " ") + py::str("]");
}

py::str canon(const Expr& x) {
    if (isc(x)) return py::str("K");
    std::vector<py::str> parts{x.tag};
    int64_t next = 0;   // the next of args
    for (const char* s = shape_of(x.tag); *s; s++) {
        switch (*s) {
            case 'i': parts.push_back(py::to_str(x.n)); break;
            case 's': parts.push_back(x.name); break;
            case 'e': parts.push_back(canon(x.args[next++])); break;
            case 'L': parts.push_back(canon(x.args)); break;
            case 'o': parts.push_back(next < x.args.size() ? canon(x.args[next++]) : py::str("None")); break;
        }
    }
    return py::str("(") + join(parts, " ") + py::str(")");
}

py::str canon(const Slot& x) { return py::str("(") + x.size + py::str(" ") + canon(x.value) + py::str(")"); }

py::str show(const py::list<Expr>& x) {
    std::vector<py::str> parts;
    for (const Expr& y : x) parts.push_back(show(y));
    return py::str("[") + join(parts, ", ") + py::str("]");
}

py::str show(const Expr& x) {
    std::vector<py::str> parts{py::repr(x.tag)};
    int64_t next = 0;
    for (const char* s = shape_of(x.tag); *s; s++) {
        switch (*s) {
            case 'i': parts.push_back(py::to_str(x.n)); break;
            case 's': parts.push_back(py::repr(x.name)); break;
            case 'e': parts.push_back(show(x.args[next++])); break;
            case 'L': parts.push_back(show(x.args)); break;
            case 'o': parts.push_back(next < x.args.size() ? show(x.args[next++]) : py::str("None")); break;
        }
    }
    py::str body = join(parts, ", ");
    if (parts.size() == 1) body = body + py::str(",");   // a tuple of one
    return py::str("(") + body + py::str(")");
}

bool unify(const Expr& a, const Expr& b) {
    if (a == b) return true;
    if (isc(a) && isc(b)) return a.tag == "f" || b.tag == "f";
    if (a.tag != b.tag) return false;
    int64_t next = 0;
    for (const char* s = shape_of(a.tag); *s; s++) {
        switch (*s) {
            case 'i':
                if (a.n != b.n) return false;
                break;
            case 's':
                if (a.name != b.name) return false;
                break;
            case 'e':
                if (!unify(a.args[next], b.args[next])) return false;
                next++;
                break;
            case 'L':
                if (!unify(a.args, b.args)) return false;
                break;
            case 'o': {
                bool ha = next < a.args.size(), hb = next < b.args.size();
                if (ha != hb) return false;
                if (ha && !unify(a.args[next], b.args[next])) return false;
                next++;
                break;
            }
        }
    }
    return true;
}

bool unify(const py::list<Expr>& a, const py::list<Expr>& b) {
    if (a == b) return true;
    if (a.size() != b.size()) return false;
    for (int64_t i = 0; i < a.size(); i++)
        if (!unify(a[i], b[i])) return false;
    return true;
}

bool unify(const Write& a, const Write& b) { return unify(a.addr, b.addr) && a.size == b.size && unify(a.value, b.value); }

bool unify(const py::list<Write>& a, const py::list<Write>& b) {
    if (a.size() != b.size()) return false;
    for (int64_t i = 0; i < a.size(); i++)
        if (!unify(a[i], b[i])) return false;
    return true;
}

bool unify(const Slot& a, const Slot& b) { return a.size == b.size && unify(a.value, b.value); }

bool unify(const py::list<Slot>& a, const py::list<Slot>& b) {
    if (a.size() != b.size()) return false;
    for (int64_t i = 0; i < a.size(); i++)
        if (!unify(a[i], b[i])) return false;
    return true;
}

bool unify(const std::optional<Expr>& a, const std::optional<Expr>& b) {
    if (!a || !b) return !a && !b;   // None is only the same as None
    return unify(*a, *b);
}

namespace {

// The symbolic machine run() steps: registers, pushes, stores, frame temporaries, the flags.
class Machine {
  public:
    using Fixed = std::optional<py::set<int64_t>>;
    struct Operand {
        char kind;   // 'M' memory with a size, 'A' an address in brackets, 'R' a register, 'I' an immediate
        py::str size;
        py::str text;
    };

    py::dict<py::str, Expr> reg;
    py::list<Expr> pushes;
    py::list<Write> writes;
    py::dict<int64_t, Slot> slots;
    std::optional<Expr> flags;   // the python's flags[0]

    Machine() {
        for (const py::str& r : R16) reg.set(r, [&] {
            Expr e;
            e.tag = py::str("r");
            e.name = r;
            return e;
        }());
    }

    Expr num(const py::str& t, const Fixed& fx) const {
        int64_t v = int0(t);
        if (fx && !fx->empty() && fx->contains(v)) return F();
        return C(v);
    }

    Expr addr(const py::str& t, const Fixed& fx) const {
        py::list<Expr> terms;
        int64_t sign = 1;
        for (const py::str& tok : py::split(t)) {
            if (tok == "+") {
                sign = 1;
            } else if (tok == "-") {
                sign = -1;
            } else if (R16.contains(tok)) {
                terms.append(reg.at(tok));
            } else if (tok == "bp") {
                terms.append(reg_("bp"));
            } else if (py::re::fullmatch(py::str("0x[0-9a-f]+|\\d+"), tok)) {
                Expr v = num(tok, fx);
                terms.append(sign > 0 ? v : neg(v));
            } else {
                throw No(t);
            }
        }
        return nary(py::str("+"), terms);
    }

    // the offset from bp of a frame temporary an address names, or None
    static std::optional<int64_t> slot(const Expr& a) {
        Expr bp = reg_("bp");
        if (a == bp) return 0;
        if (a.tag == "+" && a.args.size() == 2 && a.args.contains(bp)) {
            py::list<Expr> others;
            for (const Expr& x : a.args)
                if (x != bp) others.append(x);
            Expr k = others[0];
            if (k.tag == "c") return k.n;
        }
        return std::nullopt;
    }

    static Operand parse(const py::str& o) {
        if (auto m = py::re::fullmatch(py::str("(word|byte) ptr \\[(.+)\\]"), o)) return {'M', m->group(1), m->group(2)};
        if (auto m = py::re::fullmatch(py::str("\\[(.+)\\]"), o)) return {'A', py::str(), m->group(1)};
        if (R16.contains(o) || LO.contains(o) || HI.contains(o)) return {'R', py::str(), o};
        if (py::re::fullmatch(py::str("-?(0x[0-9a-f]+|\\d+)"), o)) return {'I', py::str(), o};
        throw No(o);
    }

    Expr get(const py::str& o, const Fixed& fx) {
        Operand p = parse(o);
        if (p.kind == 'I') return num(p.text, fx);
        if (p.kind == 'A') return addr(p.text, fx);
        if (p.kind == 'R') {
            if (R16.contains(p.text)) return reg.at(p.text);
            if (LO.contains(p.text)) return node("lo", {reg.at(LO.at(p.text))});
            throw No(o);
        }
        Expr a = addr(p.text, fx);
        std::optional<int64_t> s = slot(a);
        if (s) {
            if (slots.contains(*s)) {
                if (slots.at(*s).size != p.size) throw No(py::str("a temporary read in another size"));
                return slots.at(*s).value;
            }
            return mem_node(a, p.size, 0);
        }
        return mem_node(a, p.size, writes.size());
    }

    void put(const py::str& o, const Expr& v, const Fixed& fx) {
        Operand p = parse(o);
        if (p.kind == 'R') {
            if (R16.contains(p.text)) {
                reg.set(p.text, v);
            } else if (LO.contains(p.text)) {
                const py::str& full = LO.at(p.text);
                Expr e = node("lohi", {v, reg.at(full)});
                reg.set(full, e);
            } else {
                throw No(o);
            }
            return;
        }
        if (p.kind != 'M') throw No(o);
        Expr a = addr(p.text, fx);
        std::optional<int64_t> s = slot(a);
        if (s) slots.set(*s, Slot{p.size, v});
        else writes.append(Write{a, p.size, v});
    }
};

}  // namespace

RunResult run(const py::list<Ins>& ins, const py::dict<int64_t, py::set<int64_t>>& fixed) {
    Machine m;
    int64_t k = 0;
    while (k < ins.size()) {
        const py::str& t = std::get<0>(ins[k]);
        std::optional<py::set<int64_t>> fx = fixed.get(k);
        auto mt = py::re::fullmatch(py::str("(\\w+)(?: (.+))?"), t);
        if (!mt) throw No(t);
        py::str op = mt->group(1);
        py::list<py::str> a;
        if (auto g2 = mt->group_opt(2)) {
            if (!g2->empty())
                for (const py::str& x : py::split(*g2, py::str(", "))) a.append(x.strip());
        }
        // the compiler's way to a Boolean's value: mov R,0 / jcc over / inc R
        if (op == "mov" && a.size() == 2 && a[1] == "0" && R16.contains(a[0]) && k + 2 < ins.size()) {
            const py::bytes& jb = std::get<1>(ins[k + 1]);
            int64_t opcode = jb[0];
            if (0x70 <= opcode && opcode <= 0x7F && jb[1] == 1 && std::get<0>(ins[k + 2]) == py::str("inc ") + a[0]) {
                m.reg.set(a[0], ord_node(opcode, m.flags));
                k += 3;
                continue;
            }
        }
        if (op == "mov") {
            py::str dst = a[0];
            Expr v = m.get(a[1], fx);
            m.put(dst, v, fx);
        } else if (op == "lea") {
            py::str dst = a[0];
            Expr v = m.get(a[1], fx);
            m.put(dst, v, fx);
        } else if (op == "xchg") {
            Expr x = m.get(a[0], fx);
            Expr y = m.get(a[1], fx);
            m.put(a[0], y, fx);
            m.put(a[1], x, fx);
        } else if (op == "push") {
            m.pushes.append(m.get(a[0], fx));
        } else if ((op == "xor" || op == "sub") && a[0] == a[1] && R16.contains(a[0])) {
            m.reg.set(a[0], C(0));
            m.flags = res_node(C(0));
        } else if (op == "xor" && a[0] == a[1] && HI.contains(a[0])) {
            const py::str& r = HI.at(a[0]);
            Expr v = m.reg.at(r);
            if (v.tag == "lohi") m.reg.set(r, node("zx", {v.args[0]}));
            else if (v.tag == "zx") m.reg.set(r, v);
            else m.reg.set(r, node("zx", {node("lo", {v})}));
        } else if (op == "and" && a[1] == "0xff" && R16.contains(a[0])) {
            Expr v = m.reg.at(a[0]);
            m.reg.set(a[0], v.tag == "zx" ? v : node("zx", {node("lo", {v})}));
            m.flags = res_node(m.reg.at(a[0]));
        } else if ((op == "or" || op == "and") && a[0] == a[1]) {
            m.flags = res_node(m.get(a[0], fx));
        } else if (COMM.contains(op)) {
            Expr x = m.get(a[0], fx);
            Expr y = m.get(a[1], fx);
            Expr v = nary(COMM.at(op), py::list<Expr>{x, y});
            m.put(a[0], v, fx);
            m.flags = res_node(v);
        } else if (op == "sub") {
            Expr x = m.get(a[0], fx);
            Expr y = neg(m.get(a[1], fx));
            Expr v = nary(py::str("+"), py::list<Expr>{x, y});
            m.put(a[0], v, fx);
            m.flags = res_node(v);
        } else if (op == "cmp") {
            Expr x = m.get(a[0], fx);
            Expr y = m.get(a[1], fx);
            m.flags = node("cmp", {x, y});
        } else if (op == "inc" || op == "dec") {
            Expr x = m.get(a[0], fx);
            Expr v = nary(py::str("+"), py::list<Expr>{x, C(op == "inc" ? 1 : -1)});
            m.put(a[0], v, fx);
            m.flags = res_node(v);
        } else if ((op == "shl" || op == "shr") && (a[1] == "1" || a[1] == "cl")) {
            int64_t n = 1;
            if (a[1] == "cl") {
                const Expr& cx = m.reg.at(py::str("cx"));
                if (cx.tag != "c") throw No(py::str("a shift by an unknown count"));
                n = cx.n;
            }
            Expr x = m.get(a[0], fx);
            Expr v;
            if (op == "shl") {
                v = nary(py::str("*"), py::list<Expr>{x, C(n >= 16 ? 0 : (int64_t(1) << n))});   // (C keeps 16 bits of 1 << n)
            } else if (x.tag == "shr") {
                v = shr_node(x.args[0], x.n + n);
            } else {
                v = shr_node(x, n);
            }
            m.put(a[0], v, fx);
            m.flags = res_node(v);
        } else if (op == "mul") {
            Expr ax = m.reg.at(py::str("ax"));
            Expr x = m.get(a[0], fx);
            Expr v = nary(py::str("*"), py::list<Expr>{ax, x});
            m.reg.set(py::str("ax"), v);
            m.reg.set(py::str("dx"), node("high", {v}));
            m.flags = res_node(v);
        } else if (op == "div") {
            Expr dx = m.reg.at(py::str("dx"));
            Expr ax = m.reg.at(py::str("ax"));
            Expr d = m.get(a[0], fx);
            m.reg.set(py::str("ax"), node("div", {dx, ax, d}));
            m.reg.set(py::str("dx"), node("mod", {dx, ax, d}));
            m.flags = std::nullopt;
        } else {
            throw No(t);
        }
        k++;
    }
    return RunResult{m.reg, m.pushes, m.writes, m.slots, m.flags};
}

std::tuple<bool, py::str> same(const py::list<Ins>& w, const py::list<Ins>& g, const py::dict<int64_t, py::set<int64_t>>& fixed,
                               bool regs_too, bool flags_too) {
    RunResult rw, rg;
    try {
        rw = run(w, py::dict<int64_t, py::set<int64_t>>());
        rg = run(g, fixed);
    } catch (const No& e) {
        return {false, py::str("not known: ") + py::str(std::string(e.what()))};
    }
    if (!unify(rw.pushes, rg.pushes)) return {false, py::str("other values pushed: ") + show(rw.pushes) + py::str(" / ") + show(rg.pushes)};
    if (!unify(rw.writes, rg.writes)) return {false, py::str("other values stored")};
    py::list<Slot> a = py::list<Slot>::from(rw.slots.values());
    py::list<Slot> b = py::list<Slot>::from(rg.slots.values());
    a.sort([](const Slot& s) { return canon(s); });
    b.sort([](const Slot& s) { return canon(s); });
    if (!unify(a, b)) return {false, py::str("other values left in the frame")};
    if (regs_too) {
        for (const py::str& r : R16)
            if (!unify(rw.reg.at(r), rg.reg.at(r))) return {false, py::str("other values left in the registers")};
    }
    if (flags_too && !unify(rw.flags, rg.flags)) return {false, py::str("another condition")};
    return {true, py::str("")};
}

}  // namespace codebreaker::exact::habits::symx
