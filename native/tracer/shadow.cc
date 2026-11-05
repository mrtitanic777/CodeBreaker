// shadow.cc -- the path explorer's symbolic shadow of an x86 run, kept up beside the emulator
// instruction by instruction (what emulate/shadow.py's Shadow16 / Shadow32 / ShadowLE did from
// the Python hooks, and emulate/symbolic.py's expression constructors).
//
// Before each instruction runs, its p-code (the decompiler's own SLEIGH specification, lifted
// by pcodecheck -- asked through Python's lifter once per distinct instruction for the life of
// the process) is evaluated twice over at once: concretely, from the registers and memory as
// they are, and symbolically for the values made from an input.  Only the symbolic half is kept:
// which register bytes and memory bytes hold an expression over the inputs.  A conditional jump
// whose condition is such an expression is a condition of the path; its direction is the one
// the emulator then takes (the next instruction says which).
//
// Every step is shadow.py's, in the same order, making the same expressions (symbolic.py's
// folds included), so the path, the trail, the inputs and what was lost are the same.  An
// expression is a node of this shadow's arena; Python reads them back as symbolic.py's tuples
// (codebreaker/emulate/native_shadow.py) only when the run is over.
//
// Values are at most 128 bits here (an XMM register; x87's 80): an instruction with a wider
// value (AVX) ends the shadow ("shadow: wide"), where Python's integers went on.
#include <algorithm>
#include <cstring>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#ifdef _MSC_VER
#include <intrin.h>
#endif

#include "cbtrace.h"
#include "shadow.h"

namespace cbt {

// ---- 128-bit unsigned values ------------------------------------------------------------------
struct U128 {
    uint64_t lo = 0, hi = 0;
    U128() {}
    U128(uint64_t v) : lo(v), hi(0) {}
    U128(uint64_t l, uint64_t h) : lo(l), hi(h) {}
    bool zero() const { return !lo && !hi; }
    bool operator==(const U128 &o) const { return lo == o.lo && hi == o.hi; }
    bool operator!=(const U128 &o) const { return !(*this == o); }
    bool operator<(const U128 &o) const { return hi != o.hi ? hi < o.hi : lo < o.lo; }
    bool operator<=(const U128 &o) const { return !(o < *this); }
    U128 operator&(const U128 &o) const { return U128(lo & o.lo, hi & o.hi); }
    U128 operator|(const U128 &o) const { return U128(lo | o.lo, hi | o.hi); }
    U128 operator^(const U128 &o) const { return U128(lo ^ o.lo, hi ^ o.hi); }
    U128 operator~() const { return U128(~lo, ~hi); }
    U128 operator+(const U128 &o) const {
        U128 r(lo + o.lo, hi + o.hi);
        if (r.lo < lo)
            r.hi++;
        return r;
    }
    U128 operator-(const U128 &o) const {
        U128 r(lo - o.lo, hi - o.hi);
        if (lo < o.lo)
            r.hi--;
        return r;
    }
    U128 shl(uint64_t n) const {
        if (n >= 128)
            return U128();
        if (n == 0)
            return *this;
        if (n >= 64)
            return U128(0, lo << (n - 64));
        return U128(lo << n, (hi << n) | (lo >> (64 - n)));
    }
    U128 shr(uint64_t n) const {
        if (n >= 128)
            return U128();
        if (n == 0)
            return *this;
        if (n >= 64)
            return U128(hi >> (n - 64), 0);
        return U128((lo >> n) | (hi << (64 - n)), hi >> n);
    }
    bool bit(int n) const { return n < 64 ? (lo >> n) & 1 : n < 128 ? (hi >> (n - 64)) & 1 : false; }
    int bit_length() const {
        for (int i = 127; i >= 0; i--)
            if (bit(i))
                return i + 1;
        return 0;
    }
    int popcount() const {
        int c = 0;
        for (uint64_t v : {lo, hi})
            while (v) {
                v &= v - 1;
                c++;
            }
        return c;
    }
};

static U128 mul128(const U128 &a, const U128 &b) {
    uint64_t h;
#ifdef _MSC_VER
    uint64_t l = _umul128(a.lo, b.lo, &h);
#else
    unsigned __int128 p = (unsigned __int128)a.lo * b.lo;
    uint64_t l = uint64_t(p);
    h = uint64_t(p >> 64);
#endif
    h += a.lo * b.hi + a.hi * b.lo;
    return U128(l, h);
}

static void divmod128(const U128 &a, const U128 &b, U128 &q, U128 &r) {
    q = U128();
    r = U128();
    if (!a.hi && !b.hi) {
        q = U128(a.lo / b.lo);
        r = U128(a.lo % b.lo);
        return;
    }
    for (int i = a.bit_length() - 1; i >= 0; i--) {
        r = r.shl(1);
        if (a.bit(i))
            r.lo |= 1;
        if (b <= r) {
            r = r - b;
            q = q | U128(1).shl(uint64_t(i));
        }
    }
}

static U128 mask_bytes(int64_t size) {           // shadow._mask
    if (size <= 0)
        return U128();
    if (size >= 16)
        return U128(~0ull, ~0ull);
    return U128(1).shl(uint64_t(8 * size)) - U128(1);
}
static U128 mask_bits(int64_t bits) {
    if (bits <= 0)
        return U128();
    if (bits >= 128)
        return U128(~0ull, ~0ull);
    return U128(1).shl(uint64_t(bits)) - U128(1);
}
static bool negative(const U128 &v, int64_t size) { return size > 0 && v.bit(int(8 * size - 1)); }
static U128 neg(const U128 &v) { return U128() - v; }

// ---- expressions (symbolic.py) ----------------------------------------------------------------
enum Op : int32_t {
    E_VAR, E_CONST, E_ADD, E_SUB, E_AND, E_OR, E_XOR, E_MUL, E_UDIV, E_UREM, E_SDIV, E_SREM, E_NEG,
    E_NOT, E_SHL, E_LSHR, E_ASHR, E_ZEXT, E_SEXT, E_EXTRACT, E_CONCAT, E_ITE, E_POPCOUNT, E_BOOL,
    E_EQ, E_ULT, E_ULE, E_SLT, E_SLE, E_BNOT, E_BAND, E_BOR, E_BXOR, E_NZ, E_TRUE
};

struct Expr {
    int32_t op = 0;
    int64_t bits = 0;
    const Expr *k[3] = {nullptr, nullptr, nullptr};
    int64_t lo = 0;                  // extract: from bit
    U128 val;                        // const
    int32_t name = -1;               // var: string index
    mutable int64_t index = -1;      // (export)
};

struct PyErr {
    const char *type;
};

using Entry = std::pair<const Expr *, int>;     // byte k of an expression

struct Varnode {
    char space = 0;                  // 'r' 'u' 'c' 'm', or 0 for none, '@' for a name
    U128 off;
    int64_t size = 0;
    std::string name;
};

enum OpCode : int32_t {
    P_OTHER, P_COPY, P_LOAD, P_STORE, P_CALLOTHER, P_CBRANCH, P_BRANCH, P_BRANCHIND, P_CALLIND,
    P_RETURN, P_CALL,
    P_INT_ADD, P_INT_SUB, P_INT_AND, P_INT_OR, P_INT_XOR, P_INT_MULT, P_INT_DIV, P_INT_REM,
    P_INT_SDIV, P_INT_SREM, P_INT_EQUAL, P_INT_NOTEQUAL, P_INT_LESS, P_INT_LESSEQUAL, P_INT_SLESS,
    P_INT_SLESSEQUAL, P_INT_ZEXT, P_INT_SEXT, P_INT_CARRY, P_INT_SCARRY, P_INT_SBORROW, P_INT_2COMP,
    P_INT_NEGATE, P_INT_LEFT, P_INT_RIGHT, P_INT_SRIGHT, P_BOOL_NEGATE, P_BOOL_AND, P_BOOL_OR,
    P_BOOL_XOR, P_PIECE, P_SUBPIECE, P_POPCOUNT, P_LZCOUNT
};

static const char *const OPNAMES[] = {
    "", "COPY", "LOAD", "STORE", "CALLOTHER", "CBRANCH", "BRANCH", "BRANCHIND", "CALLIND", "RETURN",
    "CALL", "INT_ADD", "INT_SUB", "INT_AND", "INT_OR", "INT_XOR", "INT_MULT", "INT_DIV", "INT_REM",
    "INT_SDIV", "INT_SREM", "INT_EQUAL", "INT_NOTEQUAL", "INT_LESS", "INT_LESSEQUAL", "INT_SLESS",
    "INT_SLESSEQUAL", "INT_ZEXT", "INT_SEXT", "INT_CARRY", "INT_SCARRY", "INT_SBORROW", "INT_2COMP",
    "INT_NEGATE", "INT_LEFT", "INT_RIGHT", "INT_SRIGHT", "BOOL_NEGATE", "BOOL_AND", "BOOL_OR",
    "BOOL_XOR", "PIECE", "SUBPIECE", "POPCOUNT", "LZCOUNT"};

struct POp {
    int32_t code = P_OTHER;
    std::string opc;
    Varnode out;
    std::vector<Varnode> in;
};

struct Lifted {
    int64_t length = 0;
    std::vector<POp> ops;
    std::vector<uint64_t> reads, writes;    // register bytes
    bool full = false, wide = false;
};

// The lifted instructions, for the life of the process (pcode.Lifter's memo): mode, bytes, offset.
static std::mutex lift_lock;
static std::unordered_map<std::string, std::unique_ptr<Lifted>> lifted_memo;
static std::unordered_set<std::string> undecoded;

static int32_t opcode_of(const std::string &s) {
    static std::unordered_map<std::string, int32_t> table;
    static std::once_flag once;
    std::call_once(once, [] {
        for (int32_t i = 1; i < int32_t(sizeof OPNAMES / sizeof OPNAMES[0]); i++)
            table[OPNAMES[i]] = i;
    });
    auto it = table.find(s);
    return it == table.end() ? P_OTHER : it->second;
}

static U128 parse_hex(const std::string &t) {
    U128 v;
    for (char ch : t) {
        int d = ch >= '0' && ch <= '9' ? ch - '0' : ch >= 'a' && ch <= 'f' ? ch - 'a' + 10
              : ch >= 'A' && ch <= 'F' ? ch - 'A' + 10 : -1;
        if (d < 0)
            throw PyErr{"ValueError"};
        v = v.shl(4) | U128(uint64_t(d));
    }
    return v;
}

static Varnode parse_varnode(const std::string &text, bool window) {   // pcode._varnode
    Varnode v;
    if (text == "-")
        return v;
    if (!text.empty() && text[0] == '@') {
        v.space = '@';
        v.name = text.substr(1);
        return v;
    }
    size_t a = text.find(':'), b = a == std::string::npos ? a : text.find(':', a + 1);
    if (a == std::string::npos || b == std::string::npos || text.find(':', b + 1) != std::string::npos)
        throw PyErr{"ValueError"};
    std::string space = text.substr(0, a);
    v.space = space.size() == 1 ? space[0] : '?';
    v.off = parse_hex(text.substr(a + 1, b - a - 1));
    if (v.space == 'm' && window)
        v.off = v.off & U128(0xFFFF);
    v.size = std::stoll(text.substr(b + 1));
    return v;
}

static std::vector<std::string> split(const std::string &s, char c) {
    std::vector<std::string> out;
    size_t at = 0;
    for (;;) {
        size_t n = s.find(c, at);
        out.push_back(s.substr(at, n == std::string::npos ? std::string::npos : n - at));
        if (n == std::string::npos)
            break;
        at = n + 1;
    }
    return out;
}

static Lifted *parse_line(const std::string &line, bool window) {   // pcode.Lifter.lift
    auto out = std::make_unique<Lifted>();
    size_t sp = line.find(' ');
    std::string length = line.substr(0, sp), body = sp == std::string::npos ? "" : line.substr(sp + 1);
    out->length = std::stoll(length);
    std::set<uint64_t> reads, writes;
    if (!body.empty()) {
        for (const std::string &text : split(body, ';')) {
            std::vector<std::string> parts = split(text, ' ');
            if (parts.size() < 2)
                throw PyErr{"IndexError"};
            POp op;
            op.opc = parts[0];
            op.code = opcode_of(parts[0]);
            op.out = parse_varnode(parts[1], window);
            for (size_t i = 2; i < parts.size(); i++)
                op.in.push_back(parse_varnode(parts[i], window));
            if (op.code == P_LOAD || op.code == P_STORE || op.code == P_CALLOTHER || op.code == P_CBRANCH ||
                op.code == P_BRANCHIND || op.code == P_CALLIND || op.code == P_RETURN)
                out->full = true;
            for (const Varnode &v : op.in) {
                if (v.space == 'r')
                    for (int64_t b = 0; b < v.size; b++)
                        reads.insert(v.off.lo + uint64_t(b));
                else if (v.space == 'm')
                    out->full = true;
                if (v.space != '@' && v.space && v.size > 16)
                    out->wide = true;
            }
            if (op.out.space) {
                if (op.out.space == 'r')
                    for (int64_t b = 0; b < op.out.size; b++)
                        writes.insert(op.out.off.lo + uint64_t(b));
                else if (op.out.space == 'm')
                    out->full = true;
                if (op.out.size > 16)
                    out->wide = true;
            }
            out->ops.push_back(std::move(op));
        }
    }
    out->reads.assign(reads.begin(), reads.end());
    out->writes.assign(writes.begin(), writes.end());
    return out.release();
}

// ---- the shadow -------------------------------------------------------------------------------
struct VarRec {
    int32_t name;
    int32_t kind;          // 0 arg, 1 reg, 2 mem, 3 ret, 4 carry
    int32_t where_tag;     // 0 None, 1 int, 2 str
    uint64_t where_int;
    int32_t where_str;
    int64_t bits;
    uint64_t value;
};

struct PathEntry {
    uint64_t site;
    bool taken;
    const Expr *cond;
    int32_t kind;          // 0 jump, 1 inner, 2 pin
};

struct Place {
    int32_t uc_id;         // or -1
    int32_t flag;          // EFLAGS bit, or -1
    int32_t reg;           // index into the register table
    int32_t shift;
};

struct Shadow {
    int32_t kind = 32;                       // 16, 32, 33 (a driver's: ShadowLE)
    std::string mode;
    bool window = false;
    uint64_t mask = 0xFFFFFFFF;              // view.mask
    uint64_t gdt_addr = 0;
    bool real_mode = false;
    int32_t eflags_id = 0;
    cbt_sh_lift lift = nullptr;
    std::vector<std::pair<uint64_t, uint64_t>> inputs;
    std::vector<std::string> reg_names;
    std::vector<int64_t> reg_off, reg_size;
    std::vector<int32_t> reg_uc, reg_flag;
    std::unordered_map<std::string, int32_t> named;
    std::unordered_map<uint64_t, Place> places;
    std::vector<std::string> clobbered;
    struct Res { std::string reg; int k, j; };
    std::vector<Res> result;
    int64_t ret_bits = 32;

    std::deque<Expr> arena;
    std::vector<std::string> strings;
    std::unordered_map<std::string, int32_t> string_ids;
    std::unordered_map<uint64_t, uint64_t> bases;
    std::unordered_map<uint64_t, Entry> reg, mem;
    std::unordered_set<uint64_t> touched;
    std::vector<VarRec> vars;
    std::unordered_map<std::string, const Expr *> sym;
    std::vector<PathEntry> path;
    std::vector<std::pair<uint64_t, bool>> trail;
    std::set<std::string> lost;
    int64_t pins = 0, steps = 0;
    bool off = false;
    struct Pending { uint64_t site, target, fall; const Expr *entry; bool guess; };
    bool has_pending = false;
    Pending pending{};

    // the export
    std::vector<uint64_t> x_nodes, x_path, x_trail, x_vars, x_lost;
    std::string x_strings;
    cbt_sh_result x_result;

    int32_t intern(const std::string &s) {
        auto it = string_ids.find(s);
        if (it != string_ids.end())
            return it->second;
        int32_t id = int32_t(strings.size());
        strings.push_back(s);
        string_ids.emplace(s, id);
        return id;
    }
    bool is_input(uint64_t b) const {
        for (auto &r : inputs)
            if (r.first <= b && b < r.second)
                return true;
        return false;
    }

    // -- symbolic.py
    const Expr *node(int32_t op, int64_t bits, const Expr *a = nullptr, const Expr *b = nullptr,
                     const Expr *c = nullptr) {
        arena.emplace_back();
        Expr &e = arena.back();
        e.op = op;
        e.bits = bits;
        e.k[0] = a;
        e.k[1] = b;
        e.k[2] = c;
        return &e;
    }
    const Expr *S_const(const U128 &v, int64_t bits) {
        const Expr *e = node(E_CONST, bits);
        const_cast<Expr *>(e)->val = v & mask_bits(bits);
        return e;
    }
    const Expr *S_binary(int32_t op, const Expr *a, const Expr *b) { return node(op, a->bits, a, b); }
    const Expr *S_unary(int32_t op, const Expr *a) { return node(op, a->bits, a); }
    const Expr *S_zext(const Expr *a, int64_t to) { return a->bits == to ? a : node(E_ZEXT, to, a); }
    const Expr *S_sext(const Expr *a, int64_t to) { return a->bits == to ? a : node(E_SEXT, to, a); }
    const Expr *S_extract(const Expr *a, int64_t lo, int64_t width) {
        if (lo == 0 && width == a->bits)
            return a;
        if (a->op == E_CONCAT) {
            const Expr *hi_part = a->k[0], *lo_part = a->k[1];
            if (lo >= lo_part->bits)
                return S_extract(hi_part, lo - lo_part->bits, width);
            if (lo + width <= lo_part->bits)
                return S_extract(lo_part, lo, width);
        }
        if (a->op == E_ZEXT && lo + width <= a->k[0]->bits)
            return S_extract(a->k[0], lo, width);
        if (a->op == E_EXTRACT)
            return S_extract(a->k[0], a->lo + lo, width);
        const Expr *e = node(E_EXTRACT, width, a);
        const_cast<Expr *>(e)->lo = lo;
        return e;
    }
    const Expr *S_concat(const Expr *hi, const Expr *lo) {
        if (hi->op == E_EXTRACT && lo->op == E_EXTRACT && hi->k[0] == lo->k[0] && hi->lo == lo->lo + lo->bits)
            return S_extract(hi->k[0], lo->lo, hi->bits + lo->bits);
        if (hi->op == E_CONST && lo->op == E_CONST)
            return S_const(hi->val.shl(uint64_t(lo->bits)) | lo->val, hi->bits + lo->bits);
        return node(E_CONCAT, hi->bits + lo->bits, hi, lo);
    }
    const Expr *S_to_bool(const Expr *c) { return node(E_BOOL, 8, c); }
    const Expr *S_nonzero(const Expr *e) {
        if (e->op == E_BOOL)
            return e->k[0];
        if (e->op == E_ZEXT && e->k[0]->op == E_BOOL)
            return e->k[0]->k[0];
        return node(E_NZ, 0, e);
    }
    const Expr *S_negate(const Expr *c) { return c->op == E_BNOT ? c->k[0] : node(E_BNOT, 0, c); }
    const Expr *S_compare(int32_t op, const Expr *a, const Expr *b) {
        if (op == E_EQ) {
            const Expr *xs[2][2] = {{a, b}, {b, a}};
            for (auto &p : xs) {
                const Expr *x = p[0], *y = p[1];
                if (x->op == E_BOOL && y->op == E_CONST)
                    return y->val == U128(1) ? x->k[0] : y->val.zero() ? S_negate(x->k[0]) : node(E_EQ, 0, x, y);
            }
        }
        return node(op, 0, a, b);
    }
    const Expr *S_logic(int32_t op, const Expr *x, const Expr *y) { return node(op, 0, x, y); }

    // -- inputs
    const Expr *var(const std::string &name, int64_t bits, int32_t kind, int32_t where_tag, uint64_t where_int,
                    const std::string &where_str, uint64_t value) {
        auto it = sym.find(name);
        if (it != sym.end())
            return it->second;
        VarRec v{intern(name), kind, where_tag, where_int, where_tag == 2 ? intern(where_str) : -1, bits, value};
        vars.push_back(v);
        const Expr *e = node(E_VAR, bits);
        const_cast<Expr *>(e)->name = v.name;
        sym.emplace(name, e);
        return e;
    }

    void forget_regs(const std::string &n) {
        auto it = named.find(n);
        if (it == named.end())
            return;
        int64_t o = reg_off[size_t(it->second)], size = reg_size[size_t(it->second)];
        for (int64_t b = o; b < o + size; b++)
            reg.erase(uint64_t(b));
    }
    int32_t reg_index(const std::string &n) const {
        auto it = named.find(n);
        if (it == named.end())
            throw PyErr{"KeyError"};
        return it->second;
    }

    uint64_t segbase(uc_engine *uc, uint64_t sel) {
        auto it = bases.find(sel);
        if (it != bases.end())
            return it->second;
        uint64_t base = 0;
        if (kind == 16 && real_mode) {
            base = sel * 16;
        } else if (kind == 16) {             // windows16.selector_base: the GDT as it is now
            uint8_t d[8];
            if (UC.mem_read(uc, gdt_addr + (sel >> 3) * 8, d, 8) == UC_ERR_OK)
                base = uint64_t(d[2]) | uint64_t(d[3]) << 8 | uint64_t(d[4]) << 16 | uint64_t(d[7]) << 24;
        }
        bases.emplace(sel, base);
        return base;
    }

    void after_call(uc_engine *, bool symbolic, uint64_t value, bool has_args_at, uint64_t args_at, int64_t nargs,
                    const char *why, int word, uint64_t carry) {
        has_pending = false;
        bases.clear();
        if (off)
            return;
        if (why && *why && has_args_at) {
            for (uint64_t b = args_at; b < args_at + uint64_t(word * nargs); b++)
                if (mem.count(b)) {
                    lost.insert(why);
                    break;
                }
        }
        for (auto &n : clobbered)
            forget_regs(n);
        if (symbolic) {
            const Expr *r = var("ret", ret_bits, 3, 0, 0, "", value & mask_bits(ret_bits).lo);
            for (auto &res : result) {
                int32_t i = reg_index(res.reg);
                reg[uint64_t(reg_off[size_t(i)] + res.k)] = Entry(r, res.j);
            }
        }
        if (kind == 33 && symbolic && !off) {    // ShadowLE: the carry flag too
            int32_t i = reg_index("CF");
            reg[uint64_t(reg_off[size_t(i)])] = Entry(var("carry", 8, 4, 0, 0, "", carry & 1), 0);
        }
    }

    void step(uc_engine *uc, uint64_t address, uint64_t cs_base);
    void unknown(const char *why) {
        lost.insert(why);
        reg.clear();
    }
    void run(uc_engine *uc, uint64_t address, uint64_t cs_base, const Lifted &ins);
    const Lifted *lifted(uc_engine *uc, uint64_t address, uint64_t cs_base, bool &none);
};

// -- shadow._join
struct Val {
    U128 c;
    const Expr *s = nullptr;
};

void Shadow::step(uc_engine *uc, uint64_t address, uint64_t cs_base) {
    if (has_pending) {
        has_pending = false;
        Pending p = pending;
        if (address == p.target || address == p.fall) {
            bool went = address == p.target;
            trail.emplace_back(p.site, went);
            if (p.entry) {
                if (went != p.guess)
                    lost.insert("disagree");
                path.push_back(PathEntry{p.site, went, p.entry, 0});
            }
        }
    }
    steps++;
    bool none = false;
    const Lifted *ins = lifted(uc, address, cs_base, none);
    if (none && !ins)
        return;                              // (the code could not be read)
    if (!ins) {
        unknown("undecoded");
        return;
    }
    if (!ins->full) {
        bool reads = false;
        if (!reg.empty())
            for (uint64_t b : ins->reads)
                if (reg.count(b)) {
                    reads = true;
                    break;
                }
        if (!reads) {
            if (!reg.empty())
                for (uint64_t b : ins->writes)
                    reg.erase(b);
            return;
        }
    }
    if (ins->wide)
        throw PyErr{"wide"};
    run(uc, address, cs_base, *ins);
    if (path.size() >= 400) {                // _CAP_PATH
        lost.insert("path-capped");
        off = true;
    }
}

// The lifted instruction at `address` (none=true and nullptr: its code could not be read).
const Lifted *Shadow::lifted(uc_engine *uc, uint64_t address, uint64_t cs_base, bool &none) {
    uint8_t raw[16];
    size_t n = 16;
    if (UC.mem_read(uc, address, raw, 16) != UC_ERR_OK) {      // UnicornX86.code
        n = size_t((address | 0xFFF) + 1 - address);
        if (UC.mem_read(uc, address, raw, n) != UC_ERR_OK) {
            none = true;
            return nullptr;
        }
    }
    uint64_t offv = address - cs_base;
    std::string key = mode;
    key.push_back('\0');
    key.append(reinterpret_cast<const char *>(raw), n);
    key.append(reinterpret_cast<const char *>(&offv), 8);
    {
        std::lock_guard<std::mutex> g(lift_lock);
        auto it = lifted_memo.find(key);
        if (it != lifted_memo.end())
            return it->second.get();
        if (undecoded.count(key))
            return nullptr;
    }
    std::vector<char> buf(4096);
    int got = lift(raw, int32_t(n), offv, buf.data(), int32_t(buf.size()));
    if (got > int32_t(buf.size()) - 1) {
        buf.resize(size_t(got) + 1);
        got = lift(raw, int32_t(n), offv, buf.data(), int32_t(buf.size()));
    }
    if (got == -2) {                          // Python raised in the lifter: its type's name
        static thread_local std::string name;
        name.assign(buf.data());
        throw PyErr{name.c_str()};
    }
    std::lock_guard<std::mutex> g(lift_lock);
    if (got < 0 || got > int32_t(buf.size()) - 1) {
        undecoded.insert(key);
        return nullptr;
    }
    std::string line(buf.data(), size_t(got));
    if (line.empty() || line.compare(0, 3, "err") == 0) {
        undecoded.insert(key);
        return nullptr;
    }
    Lifted *l = parse_line(line, window);
    lifted_memo[key].reset(l);
    return l;
}

static int64_t sx_small(const U128 &v, int64_t size) {     // shadow._sx of a p-code offset
    int64_t bits = 8 * size;
    uint64_t x = v.lo;
    if (bits >= 64)
        return int64_t(x);
    if ((x >> (bits - 1)) & 1)
        return int64_t(x) - (int64_t(1) << bits);
    return int64_t(x);
}

void Shadow::run(uc_engine *uc, uint64_t address, uint64_t cs_base, const Lifted &ins) {
    struct Byte { uint8_t c; bool has; Entry e; };
    std::map<uint64_t, Byte> local_reg, local_mem;
    std::vector<uint64_t> local_reg_order, local_mem_order;
    struct Uniq { int64_t size; U128 c; const Expr *s; };
    std::unordered_map<uint64_t, Uniq> uniq;
    std::unordered_map<int32_t, uint64_t> conc;
    const uint64_t amask = 0xFFFFFFFF;       // view.addresses

    auto concrete_reg = [&](uint64_t b) -> uint8_t {
        auto it = places.find(b);
        if (it == places.end())
            return 0;
        const Place &p = it->second;
        auto ci = conc.find(p.reg);
        uint64_t v;
        if (ci == conc.end()) {
            if (p.flag >= 0)
                v = (cbt::reg(uc, eflags_id) >> p.flag) & 1;
            else
                v = cbt::reg(uc, p.uc_id);
            conc.emplace(p.reg, v);
        } else {
            v = ci->second;
        }
        return uint8_t((p.shift < 64 ? v >> p.shift : 0) & 0xFF);
    };
    auto join = [&](const std::vector<const Entry *> &ents, const U128 &c, int64_t size) -> const Expr * {
        const Entry *first = ents[0];
        if (first) {
            bool all = true;
            for (int64_t k = 0; k < size && all; k++) {
                const Entry *e = ents[size_t(k)];
                all = e && e->first == first->first && e->second == first->second + k;
            }
            if (all)
                return S_extract(first->first, 8 * first->second, 8 * size);
        }
        const Expr *out = nullptr;
        for (int64_t k = 0; k < size; k++) {
            const Entry *e = ents[size_t(k)];
            const Expr *byte = e ? S_extract(e->first, 8 * e->second, 8)
                                 : S_const(c.shr(uint64_t(8 * k)) & U128(0xFF), 8);
            out = out ? S_concat(byte, out) : byte;
        }
        return out;
    };
    auto load = [&](uint64_t at, int64_t size) -> Val {
        std::vector<uint8_t> data(size_t(size), 0);
        if (size > 0 && UC.mem_read(uc, at, data.data(), size_t(size)) != UC_ERR_OK)
            std::fill(data.begin(), data.end(), 0);
        U128 c;
        for (int64_t k = size - 1; k >= 0; k--)
            c = c.shl(8) | U128(data[size_t(k)]);
        std::vector<Entry> held(static_cast<size_t>(size));
        std::vector<const Entry *> ents(size_t(size), nullptr);
        bool any_s = false;
        for (int64_t k = 0; k < size; k++) {
            uint64_t b = at + uint64_t(k);
            auto li = local_mem.find(b);
            if (li != local_mem.end()) {
                c = (c & ~U128(0xFF).shl(uint64_t(8 * k))) | U128(li->second.c).shl(uint64_t(8 * k));
                if (li->second.has) {
                    held[size_t(k)] = li->second.e;
                    ents[size_t(k)] = &held[size_t(k)];
                }
            } else {
                auto mi = mem.find(b);
                if (mi != mem.end()) {
                    held[size_t(k)] = mi->second;
                    ents[size_t(k)] = &held[size_t(k)];
                } else if (!touched.count(b) && is_input(b)) {
                    char nm[32];
                    snprintf(nm, sizeof nm, "m%llx", (unsigned long long)b);
                    Entry e(var(nm, 8, 2, 1, b, "", data[size_t(k)]), 0);
                    mem[b] = e;
                    held[size_t(k)] = e;
                    ents[size_t(k)] = &held[size_t(k)];
                }
            }
            any_s = any_s || ents[size_t(k)];
        }
        Val v;
        v.c = c;
        v.s = any_s ? join(ents, c, size) : nullptr;
        return v;
    };
    auto store = [&](uint64_t at, int64_t size, const U128 &c, const Expr *s) {
        for (int64_t k = 0; k < size; k++) {
            uint64_t b = at + uint64_t(k);
            Byte by{uint8_t(c.shr(uint64_t(8 * k)).lo & 0xFF), s != nullptr, Entry(s, int(k))};
            if (!local_mem.count(b))
                local_mem_order.push_back(b);
            local_mem[b] = by;
        }
    };
    auto read = [&](const Varnode &vn) -> Val {
        Val v;
        if (vn.space == 'c') {
            v.c = vn.off & mask_bytes(vn.size);
            return v;
        }
        if (vn.space == 'u') {
            auto it = uniq.find(vn.off.lo);
            if (it != uniq.end() && it->second.size == vn.size) {
                v.c = it->second.c;
                v.s = it->second.s;
                return v;
            }
            if (it != uniq.end() && it->second.size > vn.size) {
                v.c = it->second.c & mask_bytes(vn.size);
                v.s = it->second.s ? S_extract(it->second.s, 0, 8 * vn.size) : nullptr;
                return v;
            }
            return v;
        }
        if (vn.space == 'r') {
            std::vector<Entry> store_(static_cast<size_t>(vn.size));
            std::vector<const Entry *> ents(size_t(vn.size), nullptr);
            bool any_s = false;
            for (int64_t k = 0; k < vn.size; k++) {
                uint64_t b = vn.off.lo + uint64_t(k);
                uint8_t cb;
                auto li = local_reg.find(b);
                if (li != local_reg.end()) {
                    cb = li->second.c;
                    if (li->second.has) {
                        store_[size_t(k)] = li->second.e;
                        ents[size_t(k)] = &store_[size_t(k)];
                    }
                } else {
                    cb = concrete_reg(b);
                    auto ri = reg.find(b);
                    if (ri != reg.end()) {
                        store_[size_t(k)] = ri->second;
                        ents[size_t(k)] = &store_[size_t(k)];
                    }
                }
                v.c = v.c | U128(cb).shl(uint64_t(8 * k));
                any_s = any_s || ents[size_t(k)];
            }
            v.s = any_s ? join(ents, v.c, vn.size) : nullptr;
            return v;
        }
        return load(vn.off.lo, vn.size);       // (ram named directly)
    };
    auto write = [&](const Varnode &vn, U128 c, const Expr *s) {
        c = c & mask_bytes(vn.size);
        if (vn.space == 'u') {
            uniq[vn.off.lo] = Uniq{vn.size, c, s};
        } else if (vn.space == 'r') {
            for (int64_t k = 0; k < vn.size; k++) {
                uint64_t b = vn.off.lo + uint64_t(k);
                Byte by{uint8_t(c.shr(uint64_t(8 * k)).lo & 0xFF), s != nullptr, Entry(s, int(k))};
                if (!local_reg.count(b))
                    local_reg_order.push_back(b);
                local_reg[b] = by;
            }
        } else if (vn.space == 'm') {
            store(vn.off.lo, vn.size, c, s);
        }
    };
    auto pin = [&](const U128 &c, const Expr *s) {
        if (s) {
            path.push_back(PathEntry{address, true, S_compare(E_EQ, s, S_const(c, s->bits)), 2});
            pins++;
        }
    };

    const std::vector<POp> &ops = ins.ops;
    int64_t i = 0, n = int64_t(ops.size());
    bool done = false;
    while (i < n && !done) {
        int64_t at = i;
        if (at < 0) {
            at += n;
            if (at < 0)
                throw PyErr{"IndexError"};
        }
        const POp &op = ops[size_t(at)];
        i++;
        const std::vector<Varnode> &args = op.in;
        auto arg = [&](size_t k) -> const Varnode & {
            if (k >= args.size())
                throw PyErr{"IndexError"};
            return args[k];
        };
        switch (op.code) {
        case P_COPY: {
            Val v = read(arg(0));
            write(op.out, v.c, v.s);
            break;
        }
        case P_LOAD: {
            Val a = read(arg(1));
            pin(a.c, a.s);
            Val v = load(a.c.lo & amask, op.out.size);
            write(op.out, v.c, v.s);
            break;
        }
        case P_STORE: {
            Val a = read(arg(1));
            pin(a.c, a.s);
            Val v = read(arg(2));
            store(a.c.lo & amask, arg(2).size, v.c & mask_bytes(arg(2).size), v.s);
            break;
        }
        case P_CALLOTHER: {
            const std::string &name = arg(0).name;
            if (name == "segment") {
                Val sv = read(arg(1));
                Val ov = read(arg(2));
                pin(sv.c, sv.s);
                uint64_t base = segbase(uc, sv.c.lo);
                const Expr *s = nullptr;
                U128 c;
                if (mask == 0xFFFF) {            // (real or protected 16-bit: base + 16-bit offset)
                    c = U128(base) + (ov.c & U128(0xFFFF));
                    if (ov.s) {
                        const Expr *off16 = S_extract(ov.s, 0, 16);
                        int64_t bits = 8 * op.out.size;
                        s = bits > 16 ? S_binary(E_ADD, S_zext(off16, bits), S_const(U128(base), bits)) : off16;
                    }
                } else {                         // (flat: the offset is the address)
                    c = U128(base) + ov.c;
                    if (ov.s) {
                        int64_t bits = 8 * op.out.size;
                        s = ov.s->bits == bits ? ov.s : ov.s->bits < bits ? S_zext(ov.s, bits) : S_extract(ov.s, 0, bits);
                        if (base)
                            s = S_binary(E_ADD, s, S_const(U128(base), bits));
                    }
                }
                write(op.out, c, s);
            } else {
                for (size_t k = 1; k < args.size(); k++) {
                    if (args[k].space == '@' || !args[k].space)
                        continue;
                    if (read(args[k]).s) {
                        lost.insert("userop:" + name);
                        break;
                    }
                }
                if (op.out.space)
                    write(op.out, U128(), nullptr);
            }
            break;
        }
        case P_CBRANCH: {
            Val cv = read(arg(1));
            bool taken = !cv.c.zero();
            const Varnode &tgt = arg(0);
            if (tgt.space == 'c') {              // (a branch inside the instruction's p-code)
                if (cv.s)
                    path.push_back(PathEntry{address, taken, S_nonzero(cv.s), 1});
                if (taken)
                    i = i - 1 + sx_small(tgt.off, tgt.size);
            } else {
                uint64_t target = cs_base + (tgt.off.lo & mask), fall = address + uint64_t(ins.length);
                if (target == fall || target == address) {
                    if (cv.s)
                        path.push_back(PathEntry{address, taken, S_nonzero(cv.s), 1});
                } else {
                    has_pending = true;
                    pending = Pending{address, target, fall, cv.s ? S_nonzero(cv.s) : nullptr, taken};
                }
                if (taken)
                    done = true;
            }
            break;
        }
        case P_BRANCH: {
            const Varnode &tgt = arg(0);
            if (tgt.space == 'c')
                i = i - 1 + sx_small(tgt.off, tgt.size);
            else
                done = true;
            break;
        }
        case P_BRANCHIND: case P_CALLIND: case P_RETURN: {
            Val v = read(arg(0));
            pin(v.c, v.s);
            done = true;
            break;
        }
        case P_CALL:
            done = true;
            break;
        default: {
            std::vector<Val> vals;
            for (const Varnode &a : args)
                vals.push_back(a.space == '@' ? Val() : read(a));
            if (vals.empty())
                throw PyErr{"IndexError"};
            // _arith
            int64_t osize = op.out.space ? op.out.size : 0;
            U128 m = mask_bytes(osize);
            U128 ac = vals[0].c, bc;
            const Expr *a_s = vals[0].s, *b_s = nullptr;
            int64_t asize = args[0].size, bsize = 0;
            bool sym = false;
            for (auto &v : vals)
                sym = sym || v.s;
            if (vals.size() > 1) {
                bc = vals[1].c;
                b_s = vals[1].s;
                bsize = args[1].size;
            }
            int32_t code = op.code;
            U128 c;
            auto sgnless = [&](const U128 &x, int64_t xs, const U128 &y, int64_t ys, bool eq) {
                bool nx = negative(x, xs), ny = negative(y, ys);
                if (nx != ny)
                    return nx;
                // same sign: compare the sign-extended values, i.e. their magnitudes' order
                U128 xe = nx ? (x | ~mask_bytes(xs)) : x, ye = ny ? (y | ~mask_bytes(ys)) : y;
                return eq ? xe <= ye : xe < ye;
            };
            auto absval = [&](const U128 &x, int64_t xs) { return negative(x, xs) ? neg(x | ~mask_bytes(xs)) : x; };
            bool known = true;
            switch (code) {
            case P_INT_ADD: c = (ac + bc) & m; break;
            case P_INT_SUB: c = (ac - bc) & m; break;
            case P_INT_AND: c = ac & bc; break;
            case P_INT_OR: c = ac | bc; break;
            case P_INT_XOR: c = ac ^ bc; break;
            case P_INT_MULT: c = mul128(ac, bc) & m; break;
            case P_INT_DIV: case P_INT_REM:
                if (bc.zero()) {
                    c = U128();
                } else {
                    U128 q, r;
                    divmod128(ac, bc, q, r);
                    c = (code == P_INT_DIV ? q : r) & m;
                }
                break;
            case P_INT_SDIV: case P_INT_SREM: {
                if (bc.zero()) {                 // (Python: `if y`)
                    c = U128();
                    break;
                }
                bool nx = negative(ac, asize), ny = negative(bc, bsize);
                U128 q, r;
                divmod128(absval(ac, asize), absval(bc, bsize), q, r);
                if (code == P_INT_SDIV)
                    c = (nx == ny ? q : neg(q)) & m;
                else
                    c = (nx ? neg(r) : r) & m;
                break;
            }
            case P_INT_EQUAL: c = U128(ac == bc); break;
            case P_INT_NOTEQUAL: c = U128(ac != bc); break;
            case P_INT_LESS: c = U128(ac < bc); break;
            case P_INT_LESSEQUAL: c = U128(ac <= bc); break;
            case P_INT_SLESS: c = U128(sgnless(ac, asize, bc, bsize, false)); break;
            case P_INT_SLESSEQUAL: c = U128(sgnless(ac, asize, bc, bsize, true)); break;
            case P_INT_ZEXT: c = ac; break;
            case P_INT_SEXT: c = (negative(ac, asize) ? (ac | ~mask_bytes(asize)) : ac) & m; break;
            case P_INT_CARRY: {
                U128 s = ac + bc;
                c = U128(s < ac || mask_bytes(asize) < s);
                break;
            }
            case P_INT_SCARRY: case P_INT_SBORROW: {
                // (whether the sum / difference of the two as signed asize-byte values leaves the range)
                U128 am = mask_bytes(asize);
                U128 x = ac & am, y = bc & am;
                U128 r = (code == P_INT_SCARRY ? x + y : x - y) & am;
                U128 t = code == P_INT_SCARRY ? ((x ^ r) & (y ^ r)) : ((x ^ y) & (x ^ r));
                bool over = negative(t, asize);
                // Python takes _sx of the full value: a value wider than asize changes the sum
                if ((ac & ~am).zero() && (bc & ~am).zero())
                    c = U128(over);
                else
                    throw PyErr{"wide"};
                break;
            }
            case P_INT_2COMP: c = neg(ac) & m; break;
            case P_INT_NEGATE: c = (~ac) & m; break;
            case P_INT_LEFT: c = bc < U128(uint64_t(8 * osize)) ? ac.shl(bc.lo) & m : U128(); break;
            case P_INT_RIGHT: c = bc < U128(uint64_t(8 * asize)) ? ac.shr(bc.lo) & m : U128(); break;
            case P_INT_SRIGHT: {
                uint64_t sh = U128(uint64_t(8 * asize)) < bc ? uint64_t(8 * asize) : bc.lo;
                bool ng = negative(ac, asize);
                U128 v = ng ? (ac | ~mask_bytes(asize)) : ac;
                U128 r = v.shr(sh);
                if (ng)
                    r = r | ~mask_bits(128 - int64_t(sh));
                c = r & m;
                break;
            }
            case P_BOOL_NEGATE: c = U128(ac.zero()); break;
            case P_BOOL_AND: c = U128(!ac.zero() && !bc.zero()); break;
            case P_BOOL_OR: c = U128(!ac.zero() || !bc.zero()); break;
            case P_BOOL_XOR: c = U128(!ac.zero() != !bc.zero()); break;
            case P_PIECE: c = (ac.shl(uint64_t(8 * bsize)) | bc) & m; break;
            case P_SUBPIECE: c = (bc.hi || bc.lo >= 16 ? U128() : ac.shr(8 * bc.lo)) & m; break;
            case P_POPCOUNT: c = U128(uint64_t(ac.popcount())) & m; break;
            case P_LZCOUNT: c = (U128(uint64_t(8 * asize)) - U128(uint64_t(ac.bit_length()))) & m; break;
            default: known = false;
            }
            if (!known) {                       // (floating point and the rest: not followed)
                if (sym)
                    lost.insert("op:" + op.opc);
                if (op.out.space)
                    write(op.out, U128(), nullptr);
                break;
            }
            const Expr *s = nullptr;
            if (sym) {
                const Expr *A = a_s ? a_s : S_const(ac, 8 * asize);
                const Expr *B = vals.size() > 1 ? (b_s ? b_s : S_const(bc, 8 * bsize)) : nullptr;
                switch (code) {
                case P_INT_ADD: s = S_binary(E_ADD, A, B); break;
                case P_INT_SUB: s = S_binary(E_SUB, A, B); break;
                case P_INT_AND: s = S_binary(E_AND, A, B); break;
                case P_INT_OR: s = S_binary(E_OR, A, B); break;
                case P_INT_XOR: s = S_binary(E_XOR, A, B); break;
                case P_INT_MULT: s = S_binary(E_MUL, A, B); break;
                case P_INT_DIV: s = S_binary(E_UDIV, A, B); break;
                case P_INT_REM: s = S_binary(E_UREM, A, B); break;
                case P_INT_SDIV: s = S_binary(E_SDIV, A, B); break;
                case P_INT_SREM: s = S_binary(E_SREM, A, B); break;
                case P_INT_EQUAL: s = S_to_bool(S_compare(E_EQ, A, B)); break;
                case P_INT_NOTEQUAL: s = S_to_bool(S_negate(S_compare(E_EQ, A, B))); break;
                case P_INT_LESS: s = S_to_bool(S_compare(E_ULT, A, B)); break;
                case P_INT_LESSEQUAL: s = S_to_bool(S_compare(E_ULE, A, B)); break;
                case P_INT_SLESS: s = S_to_bool(S_compare(E_SLT, A, B)); break;
                case P_INT_SLESSEQUAL: s = S_to_bool(S_compare(E_SLE, A, B)); break;
                case P_INT_ZEXT: s = S_zext(A, 8 * osize); break;
                case P_INT_SEXT: s = S_sext(A, 8 * osize); break;
                case P_INT_CARRY: s = S_to_bool(S_compare(E_ULT, S_binary(E_ADD, A, B), A)); break;
                case P_INT_SCARRY: case P_INT_SBORROW: {
                    const Expr *r = S_binary(code == P_INT_SCARRY ? E_ADD : E_SUB, A, B);
                    const Expr *x = code == P_INT_SCARRY
                        ? S_binary(E_AND, S_binary(E_XOR, A, r), S_binary(E_XOR, B, r))
                        : S_binary(E_AND, S_binary(E_XOR, A, B), S_binary(E_XOR, A, r));
                    s = S_to_bool(S_nonzero(S_extract(x, 8 * asize - 1, 1)));
                    break;
                }
                case P_INT_2COMP: s = S_unary(E_NEG, A); break;
                case P_INT_NEGATE: s = S_unary(E_NOT, A); break;
                case P_INT_LEFT: case P_INT_RIGHT: case P_INT_SRIGHT: {
                    int64_t width = 8 * asize;
                    int32_t kind_ = code == P_INT_LEFT ? E_SHL : code == P_INT_RIGHT ? E_LSHR : E_ASHR;
                    const Expr *amount, *big = nullptr;
                    if (bsize > asize) {
                        amount = S_extract(B, 0, width);
                        big = S_compare(E_ULE, S_const(U128(uint64_t(width)), 8 * bsize), B);
                    } else {
                        amount = S_zext(B, width);
                    }
                    s = S_binary(kind_, A, amount);
                    if (big) {
                        const Expr *over = kind_ == E_ASHR ? S_binary(E_ASHR, A, S_const(U128(uint64_t(width - 1)), width))
                                                           : S_const(U128(), width);
                        s = node(E_ITE, width, big, over, s);
                    }
                    break;
                }
                case P_BOOL_NEGATE: s = S_to_bool(S_negate(S_nonzero(A))); break;
                case P_BOOL_AND: s = S_to_bool(S_logic(E_BAND, S_nonzero(A), S_nonzero(B))); break;
                case P_BOOL_OR: s = S_to_bool(S_logic(E_BOR, S_nonzero(A), S_nonzero(B))); break;
                case P_BOOL_XOR: s = S_to_bool(S_logic(E_BXOR, S_nonzero(A), S_nonzero(B))); break;
                case P_PIECE: s = S_concat(A, B); break;
                case P_SUBPIECE: {
                    int64_t lo = 8 * int64_t(bc.lo);
                    int64_t width = std::min(8 * asize - lo, 8 * osize);
                    s = S_zext(S_extract(A, lo, width), 8 * osize);
                    break;
                }
                case P_POPCOUNT: s = node(E_POPCOUNT, 8 * osize, A); break;
                default:                         // (LZCOUNT)
                    lost.insert("op:" + op.opc);
                    s = nullptr;
                }
                if (s && s->bits != 8 * osize)
                    s = s->bits > 8 * osize ? S_extract(s, 0, 8 * osize) : S_zext(s, 8 * osize);
            }
            if (op.out.space)
                write(op.out, c, s);
        }
        }
    }
    // what the instruction left: kept where it is made from an input, forgotten elsewhere
    for (uint64_t b : local_reg_order) {
        const Byte &by = local_reg[b];
        if (!by.has)
            reg.erase(b);
        else
            reg[b] = by.e;
    }
    for (uint64_t b : local_mem_order) {
        const Byte &by = local_mem[b];
        touched.insert(b);
        if (!by.has)
            mem.erase(b);
        else
            mem[b] = by.e;
    }
}

// whatever goes wrong in the shadow ends the shadow, never the run (shadow._safely)
template <class F> static void safely(Shadow *s, F f) {
    try {
        f();
    } catch (const PyErr &e) {
        s->lost.insert(std::string("shadow: ") + e.type);
        s->off = true;
    } catch (const std::exception &) {
        s->lost.insert("shadow: ValueError");
        s->off = true;
    }
}

void sh_step(Shadow *s, uc_engine *uc, uint64_t address, uint64_t cs_base) {
    if (!s->off)
        safely(s, [&] { s->step(uc, address, cs_base); });
}
void sh_after_call(Shadow *s, uc_engine *uc, bool symbolic, uint64_t value, bool has_args_at, uint64_t args_at,
                   int64_t nargs, const char *why, int word, uint64_t carry) {
    safely(s, [&] { s->after_call(uc, symbolic, value, has_args_at, args_at, nargs, why, word, carry); });
}
void sh_written(Shadow *s, uint64_t at, uint64_t size) {
    for (uint64_t b = at; b < at + size; b++) {
        s->mem.erase(b);
        s->touched.insert(b);
    }
}
void sh_forget(Shadow *s, const char *name) {
    if (!s->off)
        safely(s, [&] { s->forget_regs(name); });
}
uint64_t sh_segbase(Shadow *s, uc_engine *uc, uint64_t sel) { return s->segbase(uc, sel); }

}  // namespace cbt

using namespace cbt;

extern "C" {

CBT_API void *cbt_sh_new(const cbt_sh_config *c) {
    Shadow *s = new Shadow();
    s->kind = c->kind;
    s->mode = c->mode ? c->mode : "";
    s->window = c->kind == 16;
    s->mask = c->kind == 16 ? 0xFFFF : 0xFFFFFFFF;
    s->gdt_addr = c->gdt_addr;
    s->real_mode = c->real_mode != 0;
    s->eflags_id = c->eflags_id;
    s->lift = c->lift;
    for (int64_t i = 0; i < c->ninputs; i++)
        s->inputs.emplace_back(c->inputs[2 * i], c->inputs[2 * i + 1]);
    for (int32_t i = 0; i < c->nregs; i++) {
        s->reg_names.push_back(c->reg_names[i]);
        s->reg_off.push_back(c->reg_off[i]);
        s->reg_size.push_back(c->reg_size[i]);
        s->reg_uc.push_back(c->reg_uc[i]);
        s->reg_flag.push_back(c->reg_flag[i]);
        s->named[c->reg_names[i]] = i;
        if (c->reg_uc[i] < 0 && c->reg_flag[i] < 0)
            continue;                         // (not known to the view)
        for (int64_t k = 0; k < c->reg_size[i]; k++)
            s->places[uint64_t(c->reg_off[i] + k)] = Place{c->reg_uc[i], c->reg_flag[i], i, int32_t(8 * k)};
    }
    if (c->kind == 16) {
        s->result = {{"EAX", 0, 0}, {"EAX", 1, 1}, {"EAX", 2, 2}, {"EAX", 3, 3}, {"EDX", 0, 2}, {"EDX", 1, 3}};
        s->clobbered = {"EAX", "EDX", "ESP", "EIP", "CS"};
    } else {
        s->result = {{"EAX", 0, 0}, {"EAX", 1, 1}, {"EAX", 2, 2}, {"EAX", 3, 3}};
        s->clobbered = {"EAX", "ESP", "EIP"};
    }
    return s;
}

CBT_API void cbt_sh_free(void *h) { delete static_cast<Shadow *>(h); }

CBT_API int cbt_sh_enter(void *h, const char *name, int32_t vkind, int64_t where_int, const char *where_str,
                         uint64_t value, int32_t bits, int32_t at_reg, uint64_t at, int64_t at_size) {
    Shadow *s = static_cast<Shadow *>(h);
    const Expr *v = s->var(name, bits, vkind, vkind == 0 ? 1 : 2, uint64_t(where_int), where_str ? where_str : "", value);
    if (at_reg) {
        for (int64_t j = 0; j < at_size; j++)
            if (j < bits / 8)
                s->reg[at + uint64_t(j)] = Entry(v, int(j));
    } else {
        for (int64_t j = 0; j < bits / 8; j++)
            s->mem[at + uint64_t(j)] = Entry(v, int(j));
    }
    return 0;
}

CBT_API void cbt_sh_step(void *h, void *uch, uint64_t address, uint64_t cs_base) {
    sh_step(static_cast<Shadow *>(h), static_cast<uc_engine *>(uch), address, cs_base);
}

CBT_API void cbt_sh_after_call(void *h, void *uch, int32_t symbolic, uint64_t value, int32_t has_args_at,
                               uint64_t args_at, int64_t nargs, const char *why, int32_t word, uint64_t carry) {
    sh_after_call(static_cast<Shadow *>(h), static_cast<uc_engine *>(uch), symbolic != 0, value, has_args_at != 0,
                  args_at, nargs, why, word, carry);
}

CBT_API void cbt_sh_written(void *h, uint64_t at, uint64_t size) { sh_written(static_cast<Shadow *>(h), at, size); }
CBT_API void cbt_sh_forget(void *h, const char *name) { sh_forget(static_cast<Shadow *>(h), name); }
CBT_API uint64_t cbt_sh_segbase(void *h, void *uch, uint64_t sel) {
    return sh_segbase(static_cast<Shadow *>(h), static_cast<uc_engine *>(uch), sel);
}

CBT_API const cbt_sh_result *cbt_sh_export(void *h) {
    Shadow *s = static_cast<Shadow *>(h);
    s->x_nodes.clear();
    s->x_path.clear();
    s->x_trail.clear();
    s->x_vars.clear();
    s->x_lost.clear();
    for (auto &e : s->arena)
        e.index = -1;
    // the nodes the path reaches, each after what it is made of
    std::vector<std::pair<const Expr *, int>> stack;
    auto emit = [&](const Expr *root) {
        if (root->index >= 0)
            return;
        stack.emplace_back(root, 0);
        while (!stack.empty()) {
            auto &top = stack.back();
            const Expr *e = top.first;
            if (top.second < 3) {
                const Expr *kid = e->k[top.second++];
                if (kid && kid->index < 0)
                    stack.emplace_back(kid, 0);
                continue;
            }
            stack.pop_back();
            if (e->index >= 0)
                continue;
            e->index = int64_t(s->x_nodes.size() / CBT_SH_NODE_WORDS);
            uint64_t row[CBT_SH_NODE_WORDS] = {
                uint64_t(e->op), uint64_t(e->bits),
                e->k[0] ? uint64_t(e->k[0]->index) : ~0ull, e->k[1] ? uint64_t(e->k[1]->index) : ~0ull,
                e->k[2] ? uint64_t(e->k[2]->index) : ~0ull, uint64_t(e->lo), e->op == E_VAR ? uint64_t(e->name) : e->val.lo,
                e->val.hi};
            s->x_nodes.insert(s->x_nodes.end(), row, row + CBT_SH_NODE_WORDS);
        }
    };
    for (auto &p : s->path) {
        emit(p.cond);
        uint64_t row[4] = {p.site, p.taken ? 1u : 0u, uint64_t(p.cond->index), uint64_t(p.kind)};
        s->x_path.insert(s->x_path.end(), row, row + 4);
    }
    for (auto &t : s->trail) {
        s->x_trail.push_back(t.first);
        s->x_trail.push_back(t.second ? 1 : 0);
    }
    for (auto &v : s->vars) {
        uint64_t row[7] = {uint64_t(v.name), uint64_t(v.kind), uint64_t(v.where_tag),
                           v.where_tag == 2 ? uint64_t(v.where_str) : v.where_int, uint64_t(v.bits), v.value, 0};
        s->x_vars.insert(s->x_vars.end(), row, row + CBT_SH_VAR_WORDS);
    }
    for (auto &l : s->lost)
        s->x_lost.push_back(uint64_t(s->intern(l)));
    s->x_strings.clear();
    for (auto &str : s->strings) {
        s->x_strings += str;
        s->x_strings.push_back('\0');
    }
    cbt_sh_result &r = s->x_result;
    r.nodes = s->x_nodes.data();
    r.nnodes = int64_t(s->x_nodes.size() / CBT_SH_NODE_WORDS);
    r.path = s->x_path.data();
    r.npath = int64_t(s->path.size());
    r.trail = s->x_trail.data();
    r.ntrail = int64_t(s->trail.size());
    r.vars = s->x_vars.data();
    r.nvars = int64_t(s->vars.size());
    r.lost = s->x_lost.data();
    r.nlost = int64_t(s->x_lost.size());
    r.strings = s->x_strings.data();
    r.strings_len = int64_t(s->x_strings.size());
    r.nstrings = int64_t(s->strings.size());
    r.pins = s->pins;
    r.steps = s->steps;
    r.off = s->off ? 1 : 0;
    return &r;
}

}  // extern "C"
