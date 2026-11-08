// codebreaker/recompile/switches.cpp -- typed C++ of codebreaker.recompile.switches (see switches.hpp).

#include "codebreaker/recompile/switches.hpp"

#include <algorithm>

#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/formats/xex.hpp"
#include "py_os.hpp"

namespace codebreaker::recompile::switches {

namespace {

constexpr int64_t MASK32 = 0xFFFFFFFF;

py::str hex_(int64_t v, const char* spec) { return py::format(v, spec); }

// the Python's struct.unpack_from(">I", data, off): a negative offset counts from the end of the data
int64_t be_(const py::bytes& data, int64_t size, int64_t off) {
    int64_t n = data.size();
    if (off < 0) {
        if (off + n < 0) throw py::StructError("offset out of range");
        off += n;
    }
    if (n - off < size) throw py::StructError("unpack_from requires a larger buffer");
    int64_t v = 0;
    for (int64_t i = 0; i < size; ++i) v = (v << 8) | data.byte_(off + i);
    return v;
}

bool is_branch_(Kind k) { return k == Kind::B || k == Kind::Bc || k == Kind::Bclr; }

py::set<int64_t> set_of_(std::initializer_list<int64_t> xs) {
    py::set<int64_t> s;
    for (int64_t x : xs) s.add(x);
    return s;
}

ExprP mk_(Expr::Is is, int64_t n = 0, ExprP a = nullptr, ExprP b = nullptr) {
    auto e = std::make_shared<Expr>();
    e->is = is;
    e->n = n;
    e->a = std::move(a);
    e->b = std::move(b);
    return e;
}
ExprP const_(int64_t n) { return mk_(Expr::Is::Const, n); }
ExprP idx_() { return mk_(Expr::Is::Idx); }
ExprP add_(ExprP a, ExprP b) { return mk_(Expr::Is::Add, 0, std::move(a), std::move(b)); }
ExprP shl_(ExprP a, int64_t n) { return mk_(Expr::Is::Shl, n, std::move(a)); }
ExprP load_(int64_t size, ExprP a) { return mk_(Expr::Is::Load, size, std::move(a)); }

// `return Python's (const, rest) of split_const`
std::tuple<int64_t, ExprP> split_const(const ExprP& e) {
    if (e->is == Expr::Is::Add && e->a->is == Expr::Is::Const) return {e->a->n, e->b};
    return {0, e};
}

// index_scale: the number the index is multiplied by, or None (-1: a scale is never negative)
int64_t index_scale(const ExprP& e) {
    if (e->is == Expr::Is::Idx) return 1;
    if (e->is == Expr::Is::Shl && e->a->is == Expr::Is::Idx) return int64_t(1) << e->n;
    return -1;
}

}  // namespace

// ---- Image -------------------------------------------------------------------------------------

Image::Image(const py::bytes& d, int64_t b) : data(d), base(b) {}

int64_t Image::u32(int64_t addr) const { return be_(data, 4, addr - base); }
int64_t Image::u16(int64_t addr) const { return be_(data, 2, addr - base); }
int64_t Image::u8(int64_t addr) const { return data[addr - base]; }

int64_t Image::load(int64_t size, int64_t addr) const {
    if (!(0 <= addr - base && addr - base <= data.size() - size)) {
        throw Unresolved(py::str("load of ") + py::to_str(size) + " bytes at " + hex_(addr, "08X") + " is outside the image");
    }
    if (size == 1) return u8(addr);
    if (size == 2) return u16(addr);
    if (size == 4) return u32(addr);
    throw py::KeyError(py::to_str(size));
}

int64_t sext16(int64_t v) { return (v & 0x8000) ? v - 0x10000 : v; }

// ---- decode ------------------------------------------------------------------------------------

py::str kind_name(Kind k) {
    switch (k) {
        case Kind::Nop: return "nop";
        case Kind::Addis: return "addis";
        case Kind::Addi: return "addi";
        case Kind::Ori: return "ori";
        case Kind::Rlwinm: return "rlwinm";
        case Kind::Cmplwi: return "cmplwi";
        case Kind::Cmpwi: return "cmpwi";
        case Kind::Lwz: return "lwz";
        case Kind::Bc: return "bc";
        case Kind::Bclr: return "bclr";
        case Kind::B: return "b";
        case Kind::Loadx: return "loadx";
        case Kind::Add: return "add";
        case Kind::Or: return "or";
        case Kind::Mtctr: return "mtctr";
        case Kind::Other: return "other";
    }
    return "";
}

static int64_t field_count_(Kind k) {
    switch (k) {
        case Kind::Nop: case Kind::B: return 0;
        case Kind::Mtctr: case Kind::Other: return 1;
        case Kind::Bclr: return 2;
        case Kind::Loadx: return 4;
        case Kind::Rlwinm: return 5;
        default: return 3;
    }
}

py::str repr(const Insn& d) {
    py::str out = py::str("(") + py::repr(kind_name(d.kind));
    int64_t n = field_count_(d.kind);
    for (int64_t i = 0; i < n; ++i) out = out + ", " + py::to_str(d.f[static_cast<size_t>(i)]);
    return out + (n == 0 ? ",)" : ")");
}

Insn decode(int64_t w) {
    int64_t op = w >> 26;
    int64_t rd = (w >> 21) & 31, ra = (w >> 16) & 31, rb = (w >> 11) & 31;
    int64_t imm = w & 0xFFFF;
    Insn r;
    if (w == 0x60000000) {
        r.kind = Kind::Nop;
        return r;
    }
    if (op == 15) {
        r.kind = Kind::Addis;
        r.f = {rd, ra, sext16(imm)};
        return r;
    }
    if (op == 14) {
        r.kind = Kind::Addi;
        r.f = {rd, ra, sext16(imm)};
        return r;
    }
    if (op == 24) {
        r.kind = Kind::Ori;
        r.f = {ra, rd, imm};  // dest is rA
        return r;
    }
    if (op == 21) {
        int64_t sh = rb, mb = (w >> 6) & 31, me = (w >> 1) & 31;
        r.kind = Kind::Rlwinm;
        r.f = {ra, rd, sh, mb, me};
        return r;
    }
    if (op == 10 && !((w >> 21) & 1)) {
        r.kind = Kind::Cmplwi;
        r.f = {rd >> 2, ra, imm};
        return r;
    }
    if (op == 11 && !((w >> 21) & 1)) {
        r.kind = Kind::Cmpwi;
        r.f = {rd >> 2, ra, sext16(imm)};
        return r;
    }
    if (op == 32) {
        r.kind = Kind::Lwz;
        r.f = {rd, ra, sext16(imm)};
        return r;
    }
    if (op == 16) {
        r.kind = Kind::Bc;
        r.f = {rd, ra, w & 3};  // BO, BI, AA|LK
        return r;
    }
    if (op == 19 && ((w >> 1) & 0x3FF) == 16) {
        r.kind = Kind::Bclr;
        r.f = {rd, ra};
        return r;
    }
    if (op == 18) {
        r.kind = Kind::B;
        return r;
    }
    if (op == 31) {
        int64_t xo = (w >> 1) & 0x3FF;
        if (xo == 23) {
            r.kind = Kind::Loadx;
            r.f = {rd, ra, rb, 4};
            return r;
        }
        if (xo == 279) {
            r.kind = Kind::Loadx;
            r.f = {rd, ra, rb, 2};
            return r;
        }
        if (xo == 87) {
            r.kind = Kind::Loadx;
            r.f = {rd, ra, rb, 1};
            return r;
        }
        if ((xo & 0x1FF) == 266) {
            r.kind = Kind::Add;
            r.f = {rd, ra, rb};
            return r;
        }
        if (xo == 444) {
            r.kind = Kind::Or;
            r.f = {ra, rd, rb};  // mr when rS == rB
            return r;
        }
        if (xo == 467 && (((w >> 16) & 31) | (((w >> 11) & 31) << 5)) == 9) {
            r.kind = Kind::Mtctr;
            r.f = {rd};
            return r;
        }
    }
    r.kind = Kind::Other;
    r.f = {w};
    return r;
}

const py::set<int64_t> X_STORES = set_of_({151, 183, 215, 247, 407, 439, 149, 181, 663, 695, 727, 759, 983});
const py::set<int64_t> X_WRITES_RA =
    set_of_({28, 60, 124, 284, 316, 412, 444, 476, 24, 536, 792, 824, 922, 954, 26, 27, 58, 539, 794});
const py::dict<int64_t, bool> D_LOADS = [] {
    py::dict<int64_t, bool> d;
    for (auto [k, v] : std::initializer_list<std::pair<int64_t, bool>>{{32, false}, {33, true}, {34, false}, {35, true},
                                                                         {40, false}, {41, true}, {42, false}, {43, true}})
        d.set(k, v);
    return d;
}();
const py::dict<int64_t, bool> D_STORES = [] {
    py::dict<int64_t, bool> d;
    for (auto [k, v] : std::initializer_list<std::pair<int64_t, bool>>{{36, false}, {37, true}, {38, false}, {39, true},
                                                                         {44, false}, {45, true}, {52, false}, {53, true},
                                                                         {54, false}, {55, true}})
        d.set(k, v);
    return d;
}();
const py::dict<int64_t, bool> FP_LOADS = [] {
    py::dict<int64_t, bool> d;
    for (auto [k, v] : std::initializer_list<std::pair<int64_t, bool>>{{48, false}, {49, true}, {50, false}, {51, true}})
        d.set(k, v);
    return d;
}();

std::optional<py::set<int64_t>> gpr_writes(int64_t w) {
    int64_t op = w >> 26;
    int64_t rd = (w >> 21) & 31, ra = (w >> 16) & 31;
    if (w == 0x60000000) return py::set<int64_t>();
    if (op == 14 || op == 15 || op == 7 || op == 12 || op == 13 || op == 8) {  // addi addis mulli addic addic. subfic
        return set_of_({rd});
    }
    if (op == 24 || op == 25 || op == 26 || op == 27 || op == 28 || op == 29 || op == 20 || op == 21 ||
        op == 23) {  // ori oris xori xoris andi. andis. rlwimi rlwinm rlwnm
        return set_of_({ra});
    }
    if (op == 10 || op == 11 || op == 16 || op == 18 || op == 19) {  // cmpli cmpi, branches (write LR/CTR only)
        return py::set<int64_t>();
    }
    if (D_LOADS.contains(op)) {
        return D_LOADS.at(op) ? set_of_({rd, ra}) : set_of_({rd});
    }
    if (D_STORES.contains(op) || FP_LOADS.contains(op)) {
        bool update = D_STORES.contains(op) ? D_STORES.at(op) : false;
        if (!update && FP_LOADS.contains(op)) update = FP_LOADS.at(op);
        return update ? set_of_({ra}) : py::set<int64_t>();
    }
    if (op == 58) {  // ld/ldu/lwa
        return (w & 3) == 1 ? set_of_({rd, ra}) : set_of_({rd});
    }
    if (op == 62) {  // std/stdu
        return (w & 3) == 1 ? set_of_({ra}) : py::set<int64_t>();
    }
    if (op == 59 || op == 63 || op == 4) {  // FPU / VMX (no GPR destinations in the forms seen here)
        int64_t xo = (w >> 1) & 0x3FF;
        if (op == 63 && xo == 583) return std::nullopt;  // mffs writes FPR; fine
        return py::set<int64_t>();
    }
    if (op == 31) {
        int64_t xo = (w >> 1) & 0x3FF;
        if (X_STORES.contains(xo)) {
            bool update = xo == 183 || xo == 247 || xo == 439 || xo == 181 || xo == 695 || xo == 759;
            return update ? set_of_({ra}) : py::set<int64_t>();
        }
        if (xo == 0 || xo == 32) return py::set<int64_t>();  // cmpw cmplw
        if (X_WRITES_RA.contains(xo)) return set_of_({ra});
        if (xo == 467 || xo == 144) return py::set<int64_t>();  // mtspr mtcrf
        return set_of_({rd});
    }
    return std::nullopt;
}

Writes writes(const Insn& insn, int64_t w) {
    Writes out;
    Kind k = insn.kind;
    if (k == Kind::Addis || k == Kind::Addi || k == Kind::Ori || k == Kind::Rlwinm || k == Kind::Loadx || k == Kind::Add ||
        k == Kind::Or || k == Kind::Lwz) {
        out.is = Writes::Is::Reg;
        out.reg = insn.at(1);
        return out;
    }
    std::optional<py::set<int64_t>> regs = gpr_writes(w);
    if (!regs) {
        out.is = Writes::Is::Unknown;
        return out;
    }
    out.is = Writes::Is::Set;
    out.regs = *regs;
    return out;
}

// ---- expressions -------------------------------------------------------------------------------

py::str repr(const Expr& e) {
    switch (e.is) {
        case Expr::Is::Idx: return "('idx',)";
        case Expr::Is::Const: return py::str("('const', ") + py::to_str(e.n) + ")";
        case Expr::Is::Add: return py::str("('add', ") + repr(*e.a) + ", " + repr(*e.b) + ")";
        case Expr::Is::Shl: return py::str("('shl', ") + repr(*e.a) + ", " + py::to_str(e.n) + ")";
        case Expr::Is::Load: return py::str("('load', ") + py::to_str(e.n) + ", " + repr(*e.a) + ")";
    }
    return "";
}

ExprP simplify(const ExprP& e) {
    if (e->is == Expr::Is::Add) {
        const ExprP& a = e->a;
        const ExprP& b = e->b;
        if (a->is == Expr::Is::Const && b->is == Expr::Is::Const) return const_((a->n + b->n) & MASK32);
        if (a->is == Expr::Is::Add && a->b->is == Expr::Is::Const && b->is == Expr::Is::Const) {
            return simplify(add_(a->a, const_((a->b->n + b->n) & MASK32)));
        }
        if (b->is == Expr::Is::Const && b->n == 0) return a;
        if (a->is == Expr::Is::Const && a->n == 0) return b;
        if (a->is != Expr::Is::Const && b->is == Expr::Is::Const) return add_(b, a);  // canonical: const first
    }
    if (e->is == Expr::Is::Shl && e->n == 0) return e->a;
    return e;
}

Shape table_shape(const ExprP& ctr) {
    auto [base, rest] = split_const(ctr);
    int64_t shift = 0;
    if (rest->is == Expr::Is::Shl && rest->a->is == Expr::Is::Load) {
        shift = rest->n;
        rest = rest->a;
    }
    if (rest->is != Expr::Is::Load) throw Unresolved(py::str("ctr shape ") + repr(*ctr));
    int64_t size = rest->n;
    ExprP addr = rest->a;
    auto [table, idx] = split_const(addr);
    if (index_scale(idx) != size) {
        throw Unresolved(py::str("table index scale mismatch ") + repr(*addr) + " size " + py::to_str(size));
    }
    if (size == 4 && (base || shift)) throw Unresolved("absolute table with offset");
    Shape s;
    if (size == 1) s.kind = "byte";
    else if (size == 2) s.kind = "short";
    else s.kind = "absolute";
    s.size = size;
    s.table = table;
    s.base = base;
    s.shift = shift;
    return s;
}

int64_t table_entry(const Image& img, const Shape& shape, int64_t i) {
    return (shape.base + (img.load(shape.size, shape.table + shape.size * i) << shape.shift)) & MASK32;
}

std::tuple<py::str, py::list<int64_t>> classify(const Image& img, const ExprP& ctr, int64_t count) {
    Shape shape = table_shape(ctr);
    py::list<int64_t> labels;
    for (int64_t i = 0; i < count; ++i) labels.append(table_entry(img, shape, i));
    return {shape.kind, labels};
}

// ---- Scanner -----------------------------------------------------------------------------------

Scanner::Scanner(const Image& image, int64_t text_start, int64_t text_end)
    : img_(image), text_start_(text_start), text_end_(text_end) {}

Insn Scanner::insn(int64_t addr) const { return decode(img_.u32(addr)); }

std::optional<std::tuple<int64_t, int64_t>> Scanner::branch_over(int64_t a) const {
    int64_t w = img_.u32(a);
    if ((w >> 26) != 18 || (w & 3)) return std::nullopt;  // b, relative, no link
    int64_t prev = img_.u32(a - 4);
    Insn d = decode(prev);
    if (d.kind != Kind::Bc || d.at(1) != 4 || (d.at(2) & 3) != 1 || (prev & 3)) return std::nullopt;  // ble crN (relative, no link)
    if (((a - 4 + sext16(prev & 0xFFFC)) & MASK32) != a + 4) return std::nullopt;
    int64_t off = w & 0x03FFFFFC;
    if (off & 0x02000000) off -= 0x04000000;
    return std::tuple<int64_t, int64_t>{d.at(2) >> 2, (a + off) & MASK32};
}

std::optional<std::tuple<int64_t, int64_t, int64_t, int64_t>> Scanner::find_guard(int64_t bctr) const {
    std::optional<int64_t> cr;
    int64_t default_ = 0;
    int64_t i = 0;
    while (i < WINDOW - 1) {
        i += 1;
        int64_t a = bctr - 4 * i;
        if (a < text_start_) return std::nullopt;
        Insn d = insn(a);
        if (d.kind == Kind::B) {
            std::optional<std::tuple<int64_t, int64_t>> over;
            if (!cr) over = branch_over(a);
            if (!over) return std::nullopt;  // otherwise the guard and the bctr must be straight-line code
            cr = std::get<0>(*over);
            default_ = std::get<1>(*over);
            i += 1;  // skip the ble
            continue;
        }
        if (!cr && (d.kind == Kind::Bc || d.kind == Kind::Bclr)) {
            int64_t bo = d.at(1), bi = d.at(2);
            if ((bi & 3) == 1 && (bo == 12 || bo == 4)) {  // gt bit, branch-if-true (bgt) / branch-if-false (ble)
                cr = bi >> 2;
                if (d.kind == Kind::Bc && bo == 12) {
                    int64_t w = img_.u32(a);
                    default_ = sext16(w & 0xFFFC) & MASK32;
                    if (!(w & 2)) default_ = (a + default_) & MASK32;  // relative
                }
                continue;
            }
            return std::nullopt;
        }
        if (cr && d.kind == Kind::Cmplwi && d.at(1) == *cr) return std::tuple<int64_t, int64_t, int64_t, int64_t>{a, d.at(2), d.at(3) + 1, default_};
    }
    return std::nullopt;
}

ExprP Scanner::eval_reg(int64_t reg, int64_t addr, const std::tuple<int64_t, int64_t>& guard) const {
    int64_t g_addr = std::get<0>(guard), g_reg = std::get<1>(guard);
    int64_t a = addr - 4;
    while (a >= g_addr - 4 * WINDOW) {
        if (a == g_addr && reg == g_reg) return idx_();
        int64_t word = img_.u32(a);
        Insn d = decode(word);
        Writes w = writes(d, word);
        if (w.unknown()) {
            if (a > g_addr) {
                throw Unresolved(py::str("unknown insn ") + hex_(word, "#010x") + " at " + hex_(a, "#x"));
            }
        } else if (w.is == Writes::Is::Reg) {
            if (w.reg == reg) return eval_insn(d, a, guard);
        } else if (w.regs.contains(reg)) {
            throw Unresolved(py::str("r") + py::to_str(reg) + " defined by unsupported insn " + hex_(word, "#010x") + " at " +
                             hex_(a, "#x"));
        }
        a -= 4;
    }
    throw Unresolved(py::str("r") + py::to_str(reg) + " not resolved");
}

ExprP Scanner::eval_insn(const Insn& d, int64_t a, const std::tuple<int64_t, int64_t>& guard) const {
    Kind kind = d.kind;
    if (kind == Kind::Addis) {
        ExprP base = d.at(2) == 0 ? const_(0) : eval_reg(d.at(2), a, guard);
        return simplify(add_(base, const_((d.at(3) * 65536) & MASK32)));
    }
    if (kind == Kind::Addi) {
        ExprP base = d.at(2) == 0 ? const_(0) : eval_reg(d.at(2), a, guard);
        return simplify(add_(base, const_(d.at(3) & MASK32)));
    }
    if (kind == Kind::Ori) {
        ExprP v = eval_reg(d.at(2), a, guard);
        if (v->is == Expr::Is::Const) return const_(v->n | d.at(3));
        if (d.at(3) == 0) return v;
        throw Unresolved("ori on non-const");
    }
    if (kind == Kind::Rlwinm) {
        int64_t rs = d.at(2), sh = d.at(3), mb = d.at(4), me = d.at(5);
        if (mb == 0 && me == 31 - sh) return simplify(shl_(eval_reg(rs, a, guard), sh));
        throw Unresolved(py::str("rlwinm ") + py::to_str(sh) + "," + py::to_str(mb) + "," + py::to_str(me));
    }
    if (kind == Kind::Loadx) {
        int64_t ra = d.at(2), rb = d.at(3), size = d.at(4);
        ExprP base = ra == 0 ? const_(0) : eval_reg(ra, a, guard);
        return load_(size, simplify(add_(base, eval_reg(rb, a, guard))));
    }
    if (kind == Kind::Add) {
        ExprP l = eval_reg(d.at(2), a, guard);
        ExprP r = eval_reg(d.at(3), a, guard);
        return simplify(add_(l, r));
    }
    if (kind == Kind::Or && d.at(2) == d.at(3)) {
        // `mr rX, rI` before the guard: rX is a copy of the index if rI is unchanged up to the cmplwi.
        if (d.at(2) == std::get<1>(guard) && a < std::get<0>(guard) && unchanged(std::get<1>(guard), a + 4, std::get<0>(guard))) {
            return idx_();
        }
        return eval_reg(d.at(2), a, guard);
    }
    if (kind == Kind::Lwz && d.at(2) == 1) {
        // Unoptimised (debug) code keeps the switch value in a stack slot and loads it again after the guard:
        // `lwz rG, off(r1); cmplwi rG, max; bgt default; lwz rI, off(r1)`. rI is the index if the guard's
        // register came from the same slot and nothing stored to it (or moved r1) in between.
        std::optional<std::tuple<int64_t, int64_t>> slot = guard_slot(guard);
        if (slot && std::get<1>(*slot) == d.at(3) && slot_unchanged(d.at(3), std::get<0>(*slot) + 4, a)) return idx_();
    }
    throw Unresolved(py::str("cannot evaluate ") + repr(d));
}

std::optional<std::tuple<int64_t, int64_t>> Scanner::guard_slot(const std::tuple<int64_t, int64_t>& guard) const {
    int64_t g_addr = std::get<0>(guard), g_reg = std::get<1>(guard);
    for (int64_t i = 1; i < 9; ++i) {
        int64_t a = g_addr - 4 * i;
        int64_t word = img_.u32(a);
        Insn d = decode(word);
        if (d.kind == Kind::Lwz && d.at(1) == g_reg) {
            if (d.at(2) == 1) return std::tuple<int64_t, int64_t>{a, d.at(3)};
            return std::nullopt;
        }
        Writes w = writes(d, word);
        if (w.unknown() || w.is_reg(g_reg) || w.in_set(g_reg)) return std::nullopt;
    }
    return std::nullopt;
}

bool Scanner::slot_unchanged(int64_t offset, int64_t start, int64_t end) const {
    for (int64_t a = start; a < end; a += 4) {
        int64_t word = img_.u32(a);
        int64_t op = word >> 26;
        if (D_STORES.contains(op) && ((word >> 16) & 31) == 1 && sext16(word & 0xFFFF) == offset) return false;
        Writes w = writes(decode(word), word);
        if (w.unknown() || w.is_reg(1) || w.in_set(1)) return false;
    }
    return true;
}

bool Scanner::unchanged(int64_t reg, int64_t start, int64_t end) const {
    for (int64_t a = start; a < end; a += 4) {
        int64_t word = img_.u32(a);
        Insn d = decode(word);
        if (d.kind == Kind::Rlwinm && d.at(1) == reg && d.at(2) == reg && d.at(3) == 0 && d.at(4) == 0 && d.at(5) == 31) {
            continue;  // `rotlwi rX, rX, 0` (clrldi of the upper word): the 32-bit index is the same
        }
        Writes w = writes(d, word);
        if (w.unknown() || w.is_reg(reg) || w.in_set(reg)) return false;
    }
    return true;
}

bool Scanner::index_reg_live(const std::tuple<int64_t, int64_t, int64_t, int64_t>& guard, int64_t bctr) const {
    return unchanged(std::get<1>(guard), std::get<0>(guard) + 4, bctr);
}

std::optional<Table> Scanner::analyze_unguarded(int64_t bctr, const std::tuple<int64_t, int64_t>& extent) const {
    int64_t a = bctr - 4;
    std::optional<int64_t> load, mt;
    for (int64_t n = 0; n < 12; ++n) {
        Insn d = insn(a);
        if (is_branch_(d.kind)) break;
        if (d.kind == Kind::Mtctr && !mt) mt = a;
        if (d.kind == Kind::Loadx && mt) {
            load = a;
            break;
        }
        a -= 4;
    }
    if (!load) return std::nullopt;
    Insn li = insn(*load);
    int64_t ra = li.at(2), rb = li.at(3);
    for (int64_t cand : {rb, ra}) {
        int64_t use = *load, reg = cand;
        // The scaling `slwi rX, rI, n` (if any) is where the index is used.
        int64_t u = *load - 4;
        while (u > *load - 4 * 8) {
            int64_t word = img_.u32(u);
            Insn d = decode(word);
            if (is_branch_(d.kind)) break;
            Writes w = writes(d, word);
            if (w.is_reg(cand) || w.in_set(cand) || w.unknown()) {
                if (d.kind == Kind::Rlwinm && d.at(4) == 0 && d.at(5) == 31 - d.at(3)) {
                    use = u;
                    reg = d.at(2);
                }
                break;
            }
            u -= 4;
        }
        std::tuple<int64_t, int64_t> guard{use - 4, reg};
        if (!unchanged(reg, use, bctr)) continue;
        Shape shape;
        try {
            shape = table_shape(eval_reg(insn(*mt).at(1), *mt, guard));
        } catch (const Unresolved&) {
            continue;
        }
        int64_t lo = std::get<0>(extent), hi = std::get<1>(extent);
        bool inline_ = bctr < shape.table && shape.table < bctr + 16;
        py::list<int64_t> labels;
        for (int64_t i = 0; i < 4096; ++i) {
            // an inline table ends where the first case after it begins (a case may jump back before it:
            // Up's 0x8258D3C4 table sends most entries to 0x8258D370)
            if (inline_) {
                int64_t first = 0xFFFFFFFF;
                for (int64_t x : labels) {
                    if (x > shape.table && x < first) first = x;
                }
                if (shape.table + shape.size * i >= first) break;
            }
            int64_t v = table_entry(img_, shape, i);
            if ((v & 3) || !(lo <= v && v <= hi)) break;
            labels.append(v);
        }
        if (labels.size() < 2) return std::nullopt;
        Table t;
        t.bctr = bctr;
        t.r = reg;
        t.kind = shape.kind;
        t.labels = labels;
        t.default_ = 0;
        t.guard = std::nullopt;
        t.table = shape.table;
        t.inline_ = inline_;
        return t;
    }
    return std::nullopt;
}

std::optional<Table> Scanner::analyze(int64_t bctr) const {
    auto guard = find_guard(bctr);
    if (!guard) return std::nullopt;
    (void)insn(bctr - 4);
    // mtctr may not be immediately before bctr in every sequence; search a little.
    int64_t mt_addr = bctr - 4;
    while (insn(mt_addr).kind != Kind::Mtctr) {
        mt_addr -= 4;
        if (mt_addr <= std::get<0>(*guard)) throw Unresolved("no mtctr");
    }
    ExprP ctr = eval_reg(insn(mt_addr).at(1), mt_addr, std::tuple<int64_t, int64_t>{std::get<0>(*guard), std::get<1>(*guard)});
    if (!index_reg_live(*guard, bctr)) throw Unresolved("index register clobbered before bctr");
    int64_t count = std::get<2>(*guard);
    auto [kind, labels] = classify(img_, ctr, count);
    Table t;
    t.bctr = bctr;
    t.r = std::get<1>(*guard);
    t.kind = kind;
    t.labels = labels;
    t.default_ = std::get<3>(*guard);
    t.guard = std::get<0>(*guard);
    return t;
}

// ---- the image ---------------------------------------------------------------------------------

bool indexed_load_before(const Image& img, int64_t bctr, int64_t text_start, int64_t window) {
    for (int64_t i = 1; i < window; ++i) {
        int64_t a = bctr - 4 * i;
        if (a < text_start) break;
        Insn d = decode(img.u32(a));
        if (is_branch_(d.kind)) break;
        if (d.kind == Kind::Loadx) return true;
    }
    return false;
}

std::tuple<int64_t, int64_t> extent(const py::list<std::tuple<int64_t, int64_t>>& funcs, int64_t addr, int64_t text_start,
                                    int64_t text_end) {
    auto fn = containing(funcs, addr);
    if (fn) return *fn;
    int64_t i = py::bisect_right(funcs, std::tuple<int64_t, int64_t>{addr, 0xFFFFFFFF});
    int64_t lo = i ? std::get<1>(funcs[i - 1]) : text_start;
    int64_t hi = i < funcs.size() ? std::get<0>(funcs[i]) : text_end;
    return {lo, hi};
}

std::optional<std::tuple<int64_t, int64_t>> containing(const py::list<std::tuple<int64_t, int64_t>>& funcs, int64_t addr) {
    int64_t i = py::bisect_right(funcs, std::tuple<int64_t, int64_t>{addr, 0xFFFFFFFF}) - 1;
    if (i >= 0 && std::get<0>(funcs[i]) <= addr && addr < std::get<1>(funcs[i])) return funcs[i];
    return std::nullopt;
}

std::tuple<py::list<Table>, Report> find(const py::str& path) {
    namespace pi = codebreaker::formats::powerpc_image;
    py::ref<pi::Image> image =
        pi::read(codebreaker::formats::xex::is_xex(path) ? codebreaker::formats::xex::basefile(path) : path);
    int64_t lo = std::get<0>(image->code), hi = std::get<1>(image->code);
    py::bytes data = image->bytes_at(image->base, hi - image->base);
    Image img(data, image->base);
    py::list<std::tuple<int64_t, int64_t>> funcs;
    {
        std::vector<std::tuple<int64_t, int64_t>> v;
        for (auto [b, e] : image->extents.items()) v.push_back({b, b + std::get<0>(e)});
        std::sort(v.begin(), v.end());
        funcs = py::list<std::tuple<int64_t, int64_t>>(v);
    }
    Scanner sc(img, lo, hi);
    py::list<Table> found;
    py::list<std::tuple<int64_t, py::str>> failed;
    py::list<std::tuple<int64_t, std::tuple<int64_t, int64_t>>> outside;
    py::list<int64_t> unguarded;
    for (int64_t a = lo; a < hi; a += 4) {
        if (img.u32(a) != 0x4E800420) continue;
        std::optional<Table> t;
        try {
            t = sc.analyze(a);
        } catch (const Unresolved& e) {
            failed.append({a, py::str(std::string(e.what()))});
            continue;
        }
        if (!t) {
            t = sc.analyze_unguarded(a, extent(funcs, a, lo, hi));
            if (!t) {
                if (indexed_load_before(img, a, lo)) unguarded.append(a);
                continue;
            }
        }
        auto fn = containing(funcs, a);
        if (fn) {
            bool bad = false;
            for (int64_t l : t->labels) {
                if (!(std::get<0>(*fn) <= l && l <= std::get<1>(*fn))) bad = true;  // fn[1]: unreachable-default slot
            }
            if (bad) outside.append({a, *fn});
        }
        t->func = fn;
        found.append(*t);
    }
    py::dict<py::str, int64_t> kinds;
    for (const Table& t : found) kinds.set(t.kind, kinds.get(t.kind, 0) + 1);
    Report report;
    report.tables = found.size();
    report.kinds = kinds;
    report.unresolved = failed;
    report.outside = outside;
    report.pointer_calls = unguarded;
    return {found, report};
}

void write(const py::str& path, const py::list<Table>& found) {
    py::list<py::str> lines;
    lines.append("# Switch tables, found by CodeBreaker (codebreaker/recompile/switches.py)");
    lines.append("");
    for (const Table& t : found) {
        if (!t.guard) {
            py::str where = t.inline_.value() ? py::str("inline after the bctr") : py::str("at 0x") + hex_(t.table.value(), "X");
            lines.append(py::str("# ") + t.kind + " table " + where + ", no bounds check (length from its entries)");
        } else {
            lines.append(py::str("# ") + t.kind + " table, guard at 0x" + hex_(*t.guard, "X"));
        }
        lines.append("[[switch]]");
        lines.append(py::str("base = 0x") + hex_(t.bctr, "X"));
        lines.append(py::str("r = ") + py::to_str(t.r));
        lines.append(py::str("default = 0x") + hex_(t.default_, "X"));
        lines.append("labels = [");
        for (int64_t l : t.labels) lines.append(py::str("    0x") + hex_(l, "X") + ",");
        lines.append("]");
        lines.append("");
    }
    py::file f = py::open(path, py::str("w"), std::nullopt, std::nullopt, py::str("\n"));
    f.write(py::join(py::str("\n"), lines) + "\n");
    f.close();
}

}  // namespace codebreaker::recompile::switches
