// cb/recompile/switches.cpp -- see switches.hpp.

#include "cb/recompile/switches.hpp"

#include <algorithm>
#include <array>
#include <format>

#include "cb/core/arena.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"

namespace cb::recompile::switches {
namespace {

using Reg = std::uint32_t;

constexpr std::uint32_t kNop = 0x60000000;      // ori r0, r0, 0
constexpr std::uint32_t kBctr = 0x4E800420;     // bctr, no link
constexpr std::uint32_t kWindow = 40;           // instructions looked back for the guard and the register values

constexpr std::int32_t sext16(std::uint32_t v) { return static_cast<std::int32_t>(v & 0x8000 ? v | 0xFFFF0000u : v & 0xFFFF); }

// ---- decoding: only the instructions jump-table sequences use -----------------------------------------------------
enum class Kind { Nop, Addis, Addi, Ori, Rlwinm, Cmplwi, Cmpwi, Lwz, Bc, Bclr, B, Loadx, Add, Or, Mtctr, Other };

struct Insn {
    Kind kind = Kind::Other;
    std::uint32_t dst = 0;       // the register written (rD; rA for ori, rlwinm and or)
    std::uint32_t src = 0;       // the first source (rA; rS for ori, rlwinm, or and mtctr)
    std::uint32_t src2 = 0;      // the second source (rB)
    std::int32_t imm = 0;        // the immediate (sign-extended; ori's and cmplwi's are unsigned 16-bit)
    std::uint32_t crf = 0;       // cmplwi / cmpwi: the CR field
    std::uint32_t sh = 0, mb = 0, me = 0;   // rlwinm
    std::uint32_t size = 0;      // loadx: 1, 2 or 4 bytes
    std::uint32_t bo = 0, bi = 0, aa_lk = 0;   // bc (bclr has bo and bi)
};

Insn decode(std::uint32_t w) {
    const std::uint32_t op = w >> 26;
    const std::uint32_t rd = (w >> 21) & 31, ra = (w >> 16) & 31, rb = (w >> 11) & 31;
    const std::uint32_t imm = w & 0xFFFF;
    Insn i;
    if (w == kNop) {
        i.kind = Kind::Nop;
        return i;
    }
    switch (op) {
        case 15: i.kind = Kind::Addis; i.dst = rd; i.src = ra; i.imm = sext16(imm); return i;
        case 14: i.kind = Kind::Addi; i.dst = rd; i.src = ra; i.imm = sext16(imm); return i;
        case 24: i.kind = Kind::Ori; i.dst = ra; i.src = rd; i.imm = static_cast<std::int32_t>(imm); return i;
        case 21:
            i.kind = Kind::Rlwinm; i.dst = ra; i.src = rd; i.sh = rb; i.mb = (w >> 6) & 31; i.me = (w >> 1) & 31;
            return i;
        case 10:
            if (!((w >> 21) & 1)) {
                i.kind = Kind::Cmplwi; i.crf = rd >> 2; i.src = ra; i.imm = static_cast<std::int32_t>(imm);
                return i;
            }
            break;
        case 11:
            if (!((w >> 21) & 1)) {
                i.kind = Kind::Cmpwi; i.crf = rd >> 2; i.src = ra; i.imm = sext16(imm);
                return i;
            }
            break;
        case 32: i.kind = Kind::Lwz; i.dst = rd; i.src = ra; i.imm = sext16(imm); return i;
        case 16: i.kind = Kind::Bc; i.bo = rd; i.bi = ra; i.aa_lk = w & 3; return i;
        case 19:
            if (((w >> 1) & 0x3FF) == 16) {
                i.kind = Kind::Bclr; i.bo = rd; i.bi = ra;
                return i;
            }
            break;
        case 18: i.kind = Kind::B; return i;
        case 31: {
            const std::uint32_t xo = (w >> 1) & 0x3FF;
            if (xo == 23 || xo == 279 || xo == 87) {
                i.kind = Kind::Loadx; i.dst = rd; i.src = ra; i.src2 = rb; i.size = xo == 23 ? 4 : xo == 279 ? 2 : 1;
                return i;
            }
            if ((xo & 0x1FF) == 266) {
                i.kind = Kind::Add; i.dst = rd; i.src = ra; i.src2 = rb;
                return i;
            }
            if (xo == 444) {   // mr when rS == rB
                i.kind = Kind::Or; i.dst = ra; i.src = rd; i.src2 = rb;
                return i;
            }
            if (xo == 467 && (((w >> 16) & 31) | (((w >> 11) & 31) << 5)) == 9) {
                i.kind = Kind::Mtctr; i.src = rd;
                return i;
            }
            break;
        }
        default: break;
    }
    i.kind = Kind::Other;
    return i;
}

bool is_branch(Kind k) { return k == Kind::B || k == Kind::Bc || k == Kind::Bclr; }

// ---- which general registers an instruction writes ----------------------------------------------------------------
template <std::size_t N>
constexpr bool in(std::uint32_t v, const std::array<std::uint32_t, N>& set) {
    return std::find(set.begin(), set.end(), v) != set.end();
}
constexpr std::array<std::uint32_t, 13> kXStores{151, 183, 215, 247, 407, 439, 149, 181, 663, 695, 727, 759, 983};   // stwx stbx sthx stdx stf*x (+u)
constexpr std::array<std::uint32_t, 19> kXWritesRa{28, 60, 124, 284, 316, 412, 444, 476, 24, 536, 792, 824, 922, 954, 26, 27, 58, 539, 794};
constexpr std::array<std::uint32_t, 6> kXStoresUpdate{183, 247, 439, 181, 695, 759};
constexpr bool is_d_load(std::uint32_t op) { return op == 32 || op == 33 || op == 34 || op == 35 || (op >= 40 && op <= 43); }
constexpr bool is_d_store(std::uint32_t op) { return (op >= 36 && op <= 39) || op == 44 || op == 45 || (op >= 52 && op <= 55); }
constexpr bool is_fp_load(std::uint32_t op) { return op >= 48 && op <= 51; }
constexpr bool is_update_form(std::uint32_t op) { return (op & 1) != 0; }   // lwzu, stwu, lfsu ...: the odd opcode of each pair

// What an instruction writes: nothing known (it may clobber anything), one register, or a set of registers (a bit mask).
struct Writes {
    enum class Is { Unknown, Reg, Set } is = Is::Unknown;
    std::uint32_t reg = 0;
    std::uint32_t mask = 0;
    static Writes one(std::uint32_t r) { return {Is::Reg, r, 0}; }
    static Writes set(std::uint32_t m) { return {Is::Set, 0, m}; }
    bool unknown() const { return is == Is::Unknown; }
    bool is_reg(std::uint32_t r) const { return is == Is::Reg && reg == r; }
    bool in_set(std::uint32_t r) const { return is == Is::Set && ((mask >> r) & 1); }
    // may write r, or write something unknown
    bool clobbers(std::uint32_t r) const { return unknown() || is_reg(r) || in_set(r); }
};
constexpr std::uint32_t bit(std::uint32_t r) { return 1u << r; }

// The registers a raw word writes. Unknown for words this scan has no model of.
Writes gpr_writes(std::uint32_t w) {
    const std::uint32_t op = w >> 26;
    const std::uint32_t rd = (w >> 21) & 31, ra = (w >> 16) & 31;
    if (w == kNop) return Writes::set(0);
    if (op == 14 || op == 15 || op == 7 || op == 12 || op == 13 || op == 8) return Writes::set(bit(rd));   // addi addis mulli addic addic. subfic
    if (op == 24 || op == 25 || op == 26 || op == 27 || op == 28 || op == 29 || op == 20 || op == 21 || op == 23)
        return Writes::set(bit(ra));   // ori oris xori xoris andi. andis. rlwimi rlwinm rlwnm
    if (op == 10 || op == 11 || op == 16 || op == 18 || op == 19) return Writes::set(0);   // compares and branches (they write LR/CTR only)
    if (is_d_load(op)) return Writes::set(is_update_form(op) ? bit(rd) | bit(ra) : bit(rd));
    if (is_d_store(op) || is_fp_load(op)) return Writes::set(is_update_form(op) ? bit(ra) : 0);
    if (op == 58) return Writes::set((w & 3) == 1 ? bit(rd) | bit(ra) : bit(rd));   // ld, ldu, lwa
    if (op == 62) return Writes::set((w & 3) == 1 ? bit(ra) : 0);                    // std, stdu
    if (op == 59 || op == 63 || op == 4) {   // FPU and VMX: no general destination in the forms seen here
        const std::uint32_t xo = (w >> 1) & 0x3FF;
        return op == 63 && xo == 583 ? Writes() : Writes::set(0);   // mffs: kept unknown, as the scan was written
    }
    if (op == 31) {
        const std::uint32_t xo = (w >> 1) & 0x3FF;
        if (in(xo, kXStores)) return Writes::set(in(xo, kXStoresUpdate) ? bit(ra) : 0);
        if (xo == 0 || xo == 32) return Writes::set(0);   // cmpw, cmplw
        if (in(xo, kXWritesRa)) return Writes::set(bit(ra));
        if (xo == 467 || xo == 144) return Writes::set(0);   // mtspr, mtcrf
        return Writes::set(bit(rd));
    }
    return Writes();
}

// The register a decoded instruction writes, when the scan can evaluate it; else what gpr_writes says.
Writes writes(const Insn& insn, std::uint32_t w) {
    switch (insn.kind) {
        case Kind::Addis: case Kind::Addi: case Kind::Ori: case Kind::Rlwinm: case Kind::Loadx: case Kind::Add:
        case Kind::Or: case Kind::Lwz:
            return Writes::one(insn.dst);
        default: return gpr_writes(w);
    }
}

// ---- symbolic values ----------------------------------------------------------------------------------------------
// idx, a constant, a + b, a << n, or a load of `n` bytes at a. Nodes live in the arena of one analysis.
struct Expr {
    enum class Is { Idx, Const, Add, Shl, Load } is = Is::Idx;
    std::uint32_t n = 0;   // the constant, the shift count or the size of the load
    const Expr* a = nullptr;
    const Expr* b = nullptr;
};

// The code and the one analysis in progress: the arena its expressions are built in.
class Scan {
public:
    Scan(const CodeImage& image) : image_(image) {}

    // The word at an address; zero (as padding reads) when the address is not in the image.
    std::uint32_t word(std::int64_t addr) const {
        const std::int64_t off = addr - image_.base;
        if (off < 0 || off + 4 > static_cast<std::int64_t>(image_.data.size())) return 0;
        const std::uint8_t* p = &image_.data[static_cast<std::size_t>(off)];
        return (std::uint32_t{p[0]} << 24) | (std::uint32_t{p[1]} << 16) | (std::uint32_t{p[2]} << 8) | p[3];
    }
    Insn insn(std::int64_t addr) const { return decode(word(addr)); }

    // `size` bytes of table data; the address outside the image is a value the scan could not follow.
    Result<std::uint32_t> load(std::uint32_t size, std::uint64_t addr) const {
        const std::int64_t off = static_cast<std::int64_t>(addr) - image_.base;
        if (off < 0 || static_cast<std::uint64_t>(off) + size > image_.data.size()) {
            return fail(std::format("load of {} bytes at {:08X} is outside the image", size, addr));
        }
        std::uint32_t v = 0;
        for (std::uint32_t k = 0; k < size; ++k) v = (v << 8) | image_.data[static_cast<std::size_t>(off) + k];
        return v;
    }

    // The scan could not follow a jump. It is the routine outcome for code that is not a switch, so it travels as a
    // Result, not an exception; the error names the bctr being analysed.
    Failure fail(const std::string& why) const { return UnsupportedError("switches", current_bctr_, why); }

    const Expr* make(Expr::Is is, std::uint32_t n, const Expr* a = nullptr, const Expr* b = nullptr) {
        return arena_.make<Expr>(Expr{is, n, a, b});
    }
    const Expr* constant(std::uint32_t v) { return make(Expr::Is::Const, v); }
    const Expr* idx() { return make(Expr::Is::Idx, 0); }

    const Expr* simplify(const Expr* e);

    std::uint32_t text_start = 0;
    std::uint32_t text_end = 0;

    // ---- the recognisers ----
    struct Guard {
        std::uint32_t addr = 0;     // the cmplwi
        Reg reg = 0;                // the register it compares
        std::uint32_t count = 0;    // entries: the bound + 1
        std::uint32_t default_target = 0;
    };

    std::optional<std::pair<std::uint32_t, std::uint32_t>> branch_over(std::uint32_t a) const;
    std::optional<Guard> find_guard(std::uint32_t bctr) const;
    Result<const Expr*> eval_reg(Reg reg, std::uint32_t addr, const Guard& guard);
    Result<const Expr*> eval_insn(const Insn& d, std::uint32_t a, const Guard& guard);
    std::optional<std::pair<std::uint32_t, std::int32_t>> guard_slot(const Guard& guard) const;
    bool slot_unchanged(std::int32_t offset, std::uint32_t start, std::uint32_t end) const;
    bool unchanged(Reg reg, std::uint32_t start, std::uint32_t end) const;
    std::optional<SwitchTable> analyze_unguarded(std::uint32_t bctr, Extent extent);
    Result<std::optional<SwitchTable>> analyze(std::uint32_t bctr);
    bool indexed_load_before(std::uint32_t bctr, int window = 12) const;

    struct Shape {
        TableKind kind = TableKind::Absolute;
        std::uint32_t size = 0, table = 0, base = 0, shift = 0;
    };
    Result<Shape> table_shape(const Expr* ctr) const;
    Result<std::uint32_t> table_entry(const Shape& s, std::uint64_t i) const;

private:
    const CodeImage& image_;
    Arena arena_;
    std::uint32_t current_bctr_ = 0;
};

const Expr* Scan::simplify(const Expr* e) {
    if (e->is == Expr::Is::Add) {
        const Expr *a = e->a, *b = e->b;
        if (a->is == Expr::Is::Const && b->is == Expr::Is::Const) return constant(a->n + b->n);
        if (a->is == Expr::Is::Add && a->b->is == Expr::Is::Const && b->is == Expr::Is::Const)
            return simplify(make(Expr::Is::Add, 0, a->a, constant(a->b->n + b->n)));
        if (b->is == Expr::Is::Const && b->n == 0) return a;
        if (a->is == Expr::Is::Const && a->n == 0) return b;
        if (a->is != Expr::Is::Const && b->is == Expr::Is::Const) return make(Expr::Is::Add, 0, b, a);   // constant first
    }
    if (e->is == Expr::Is::Shl && e->n == 0) return e->a;
    return e;
}

// `ble crN, a+4` at a-4 skipping `b default` at a: (crN, default), else nullopt.
std::optional<std::pair<std::uint32_t, std::uint32_t>> Scan::branch_over(std::uint32_t a) const {
    const std::uint32_t w = word(a);
    if ((w >> 26) != 18 || (w & 3)) return std::nullopt;   // b, relative, no link
    const std::uint32_t prev = word(static_cast<std::int64_t>(a) - 4);
    const Insn d = decode(prev);
    if (d.kind != Kind::Bc || d.bo != 4 || (d.bi & 3) != 1 || (prev & 3)) return std::nullopt;   // ble crN (relative, no link)
    if (static_cast<std::uint32_t>(a - 4 + sext16(prev & 0xFFFC)) != a + 4) return std::nullopt;
    std::int64_t off = w & 0x03FFFFFC;
    if (off & 0x02000000) off -= 0x04000000;
    return std::pair{d.bi >> 2, static_cast<std::uint32_t>(a + off)};
}

// Locate `cmplwi crN, rI, max` guarded by `bgt/ble crN`. Two guard shapes occur: `bgt crN, default` falling through
// to the table code, and `ble crN, L; b default; L:` (the conditional branch skips an unconditional one).
std::optional<Scan::Guard> Scan::find_guard(std::uint32_t bctr) const {
    std::optional<std::uint32_t> cr;
    std::uint32_t default_target = 0;
    std::uint32_t i = 0;
    while (i < kWindow - 1) {
        ++i;
        const std::int64_t a = static_cast<std::int64_t>(bctr) - 4 * static_cast<std::int64_t>(i);
        if (a < text_start) return std::nullopt;
        const std::uint32_t at = static_cast<std::uint32_t>(a);
        const Insn d = insn(a);
        if (d.kind == Kind::B) {
            // otherwise the guard and the bctr must be straight-line code
            const auto over = cr ? std::nullopt : branch_over(at);
            if (!over) return std::nullopt;
            cr = over->first;
            default_target = over->second;
            ++i;   // skip the ble
            continue;
        }
        if (!cr && (d.kind == Kind::Bc || d.kind == Kind::Bclr)) {
            if ((d.bi & 3) == 1 && (d.bo == 12 || d.bo == 4)) {   // the gt bit, branch-if-true (bgt) / branch-if-false (ble)
                cr = d.bi >> 2;
                if (d.kind == Kind::Bc && d.bo == 12) {
                    const std::uint32_t w = word(a);
                    default_target = static_cast<std::uint32_t>(sext16(w & 0xFFFC));
                    if (!(w & 2)) default_target += at;   // relative
                }
                continue;
            }
            return std::nullopt;
        }
        if (cr && d.kind == Kind::Cmplwi && d.crf == *cr) {
            return Guard{at, d.src, static_cast<std::uint32_t>(d.imm) + 1, default_target};
        }
    }
    return std::nullopt;
}

// The symbolic value of `reg` just before the instruction at `addr`.
Result<const Expr*> Scan::eval_reg(Reg reg, std::uint32_t addr, const Guard& guard) {
    std::int64_t a = static_cast<std::int64_t>(addr) - 4;
    while (a >= static_cast<std::int64_t>(guard.addr) - 4 * static_cast<std::int64_t>(kWindow)) {
        if (a == guard.addr && reg == guard.reg) return idx();
        const std::uint32_t w = word(a);
        const Insn d = decode(w);
        const Writes wr = writes(d, w);
        if (wr.unknown()) {
            if (a > guard.addr) return fail(std::format("unknown insn {:#010x} at {:#x}", w, a));
        } else if (wr.is == Writes::Is::Reg) {
            if (wr.reg == reg) return eval_insn(d, static_cast<std::uint32_t>(a), guard);
        } else if (wr.in_set(reg)) {
            return fail(std::format("r{} defined by unsupported insn {:#010x} at {:#x}", reg, w, a));
        }
        a -= 4;
    }
    return fail(std::format("r{} not resolved", reg));
}

Result<const Expr*> Scan::eval_insn(const Insn& d, std::uint32_t a, const Guard& guard) {
    switch (d.kind) {
        case Kind::Addis: case Kind::Addi: {
            const Expr* base = nullptr;
            if (d.src == 0) {
                base = constant(0);
            } else {
                CB_TRY_ASSIGN(const Expr* b, eval_reg(d.src, a, guard));
                base = b;
            }
            const std::uint32_t add = d.kind == Kind::Addis ? static_cast<std::uint32_t>(d.imm) << 16 : static_cast<std::uint32_t>(d.imm);
            return simplify(make(Expr::Is::Add, 0, base, constant(add)));
        }
        case Kind::Ori: {
            CB_TRY_ASSIGN(const Expr* v, eval_reg(d.src, a, guard));
            if (v->is == Expr::Is::Const) return constant(v->n | static_cast<std::uint32_t>(d.imm));
            if (d.imm == 0) return v;
            return fail("ori on non-const");
        }
        case Kind::Rlwinm: {
            if (d.mb == 0 && d.me == 31 - d.sh) {
                CB_TRY_ASSIGN(const Expr* v, eval_reg(d.src, a, guard));
                return simplify(make(Expr::Is::Shl, d.sh, v));
            }
            return fail(std::format("rlwinm {},{},{}", d.sh, d.mb, d.me));
        }
        case Kind::Loadx: {
            const Expr* base = nullptr;
            if (d.src == 0) {
                base = constant(0);
            } else {
                CB_TRY_ASSIGN(const Expr* b, eval_reg(d.src, a, guard));
                base = b;
            }
            CB_TRY_ASSIGN(const Expr* offset, eval_reg(d.src2, a, guard));
            return make(Expr::Is::Load, d.size, simplify(make(Expr::Is::Add, 0, base, offset)));
        }
        case Kind::Add: {
            CB_TRY_ASSIGN(const Expr* x, eval_reg(d.src, a, guard));
            CB_TRY_ASSIGN(const Expr* y, eval_reg(d.src2, a, guard));
            return simplify(make(Expr::Is::Add, 0, x, y));
        }
        case Kind::Or:
            if (d.src == d.src2) {
                // `mr rX, rI` before the guard: rX is a copy of the index if rI is unchanged up to the cmplwi.
                if (d.src == guard.reg && a < guard.addr && unchanged(guard.reg, a + 4, guard.addr)) return idx();
                return eval_reg(d.src, a, guard);
            }
            break;
        case Kind::Lwz:
            if (d.src == 1) {
                // Unoptimised (debug) code keeps the switch value in a stack slot and loads it again after the guard:
                // `lwz rG, off(r1); cmplwi rG, max; bgt default; lwz rI, off(r1)`. rI is the index if the guard's
                // register came from the same slot and nothing stored to it (or moved r1) in between.
                const auto slot = guard_slot(guard);
                if (slot && slot->second == d.imm && slot_unchanged(d.imm, slot->first + 4, a)) return idx();
            }
            break;
        default: break;
    }
    return fail(std::format("cannot evaluate instruction at {:#x}", a));
}

// (address, offset) of the `lwz rG, offset(r1)` that loads the guard's register just before the cmplwi.
std::optional<std::pair<std::uint32_t, std::int32_t>> Scan::guard_slot(const Guard& guard) const {
    for (std::uint32_t i = 1; i < 9; ++i) {
        const std::uint32_t a = guard.addr - 4 * i;
        const std::uint32_t w = word(a);
        const Insn d = decode(w);
        if (d.kind == Kind::Lwz && d.dst == guard.reg) {
            if (d.src == 1) return std::pair{a, d.imm};
            return std::nullopt;
        }
        if (writes(d, w).clobbers(guard.reg)) return std::nullopt;
    }
    return std::nullopt;
}

// No store to offset(r1) and no write to r1 in [start, end).
bool Scan::slot_unchanged(std::int32_t offset, std::uint32_t start, std::uint32_t end) const {
    for (std::uint32_t a = start; a < end; a += 4) {
        const std::uint32_t w = word(a);
        const std::uint32_t op = w >> 26;
        if (is_d_store(op) && ((w >> 16) & 31) == 1 && sext16(w & 0xFFFF) == offset) return false;
        if (writes(decode(w), w).clobbers(1)) return false;
    }
    return true;
}

// Nothing in [start, end) writes `reg` (or may).
bool Scan::unchanged(Reg reg, std::uint32_t start, std::uint32_t end) const {
    for (std::uint32_t a = start; a < end; a += 4) {
        const std::uint32_t w = word(a);
        const Insn d = decode(w);
        // `rotlwi rX, rX, 0` (clrldi of the upper word): the 32-bit index is the same
        if (d.kind == Kind::Rlwinm && d.dst == reg && d.src == reg && d.sh == 0 && d.mb == 0 && d.me == 31) continue;
        if (writes(d, w).clobbers(reg)) return false;
    }
    return true;
}

Result<Scan::Shape> Scan::table_shape(const Expr* ctr) const {
    // const + rest, else (0, e)
    auto split_const = [](const Expr* e) -> std::pair<std::uint32_t, const Expr*> {
        if (e->is == Expr::Is::Add && e->a->is == Expr::Is::Const) return {e->a->n, e->b};
        return {0, e};
    };
    auto [base, rest] = split_const(ctr);
    std::uint32_t shift = 0;
    if (rest->is == Expr::Is::Shl && rest->a->is == Expr::Is::Load) {
        shift = rest->n;
        rest = rest->a;
    }
    if (rest->is != Expr::Is::Load) return fail("ctr is not a table load");
    const std::uint32_t size = rest->n;
    auto [table, index] = split_const(rest->a);
    // the index must be the guard's register, scaled by the entry size
    std::optional<std::uint32_t> scale;
    if (index->is == Expr::Is::Idx) scale = 1;
    else if (index->is == Expr::Is::Shl && index->a->is == Expr::Is::Idx) scale = 1u << index->n;
    if (scale != size) return fail(std::format("table index scale mismatch for entries of {} bytes", size));
    if (size == 4 && (base || shift)) return fail("absolute table with offset");
    return Shape{size == 1 ? TableKind::Byte : size == 2 ? TableKind::Short : TableKind::Absolute, size, table, base, shift};
}

Result<std::uint32_t> Scan::table_entry(const Shape& s, std::uint64_t i) const {
    CB_TRY_ASSIGN(const std::uint32_t v, load(s.size, static_cast<std::uint64_t>(s.table) + s.size * i));
    return s.base + (v << s.shift);
}

// A table with no bounds check before it (a `switch` whose default the compiler knows is unreachable, e.g. a leaf
// function switching on an argument, or `default: __assume(0)`).
//
// The index is the register the table load scales (or uses directly for byte tables), taken where it is used; it
// must still hold that value at the bctr. Without a bound, the table runs while its entries are word-aligned
// addresses inside the function `extent`: an entry equal to its end is MSVC's slot for the unreachable default. An
// inline table (data right after the bctr) ends exactly where the first case's code begins. Extra trailing entries
// only add case labels that are never taken; a missing one would fall back to an indirect jump and be reported.
std::optional<SwitchTable> Scan::analyze_unguarded(std::uint32_t bctr, Extent extent) {
    current_bctr_ = bctr;
    std::uint32_t a = bctr - 4;
    std::optional<std::uint32_t> load_at, mt;
    for (int k = 0; k < 12; ++k) {
        const Insn d = insn(a);
        if (is_branch(d.kind)) break;
        if (d.kind == Kind::Mtctr && !mt) mt = a;
        if (d.kind == Kind::Loadx && mt) {
            load_at = a;
            break;
        }
        a -= 4;
    }
    if (!load_at) return std::nullopt;
    const Insn load_insn = insn(*load_at);
    for (const Reg cand : {load_insn.src2, load_insn.src}) {
        std::uint32_t use = *load_at;
        Reg reg = cand;
        // The scaling `slwi rX, rI, n` (if any) is where the index is used.
        for (std::int64_t u = static_cast<std::int64_t>(*load_at) - 4; u > static_cast<std::int64_t>(*load_at) - 4 * 8; u -= 4) {
            const std::uint32_t w = word(u);
            const Insn d = decode(w);
            if (is_branch(d.kind)) break;
            if (writes(d, w).clobbers(cand)) {
                if (d.kind == Kind::Rlwinm && d.mb == 0 && d.me == 31 - d.sh) {
                    use = static_cast<std::uint32_t>(u);
                    reg = d.src;
                }
                break;
            }
        }
        const Guard guard{use - 4, reg, 0, 0};
        if (!unchanged(reg, use, bctr)) continue;
        const auto ctr = eval_reg(insn(*mt).src, *mt, guard);
        if (!ctr.ok()) continue;
        const auto shape_result = table_shape(ctr.value());
        if (!shape_result.ok()) continue;
        const Shape shape = shape_result.value();

        const bool inline_table = bctr < shape.table && shape.table < bctr + 16;
        std::vector<std::uint32_t> labels;
        for (std::uint64_t i = 0; i < 4096; ++i) {
            // an inline table ends where the first case after it begins (a case may jump back before it: Up's
            // 0x8258D3C4 table sends most entries to 0x8258D370)
            if (inline_table) {
                std::uint32_t first_case = 0xFFFFFFFF;
                for (const std::uint32_t x : labels) {
                    if (x > shape.table) first_case = std::min(first_case, x);
                }
                if (static_cast<std::uint64_t>(shape.table) + shape.size * i >= first_case) break;
            }
            const auto entry = table_entry(shape, i);
            if (!entry.ok()) break;   // the table runs off the image
            const std::uint32_t v = entry.value();
            if ((v & 3) || v < extent.begin || v > extent.end) break;
            labels.push_back(v);
        }
        if (labels.size() < 2) return std::nullopt;
        SwitchTable t;
        t.bctr = bctr;
        t.index_reg = reg;
        t.kind = shape.kind;
        t.labels = std::move(labels);
        t.table = shape.table;
        t.inline_table = inline_table;
        return t;
    }
    return std::nullopt;
}

Result<std::optional<SwitchTable>> Scan::analyze(std::uint32_t bctr) {
    current_bctr_ = bctr;
    const auto guard = find_guard(bctr);
    if (!guard) return std::optional<SwitchTable>();
    // mtctr may not be immediately before bctr in every sequence; search a little
    std::uint32_t mt_addr = bctr - 4;
    while (insn(mt_addr).kind != Kind::Mtctr) {
        mt_addr -= 4;
        if (mt_addr <= guard->addr) return fail("no mtctr");
    }
    CB_TRY_ASSIGN(const Expr* ctr, eval_reg(insn(mt_addr).src, mt_addr, *guard));
    if (!unchanged(guard->reg, guard->addr + 4, bctr)) return fail("index register clobbered before bctr");
    CB_TRY_ASSIGN(const Shape shape, table_shape(ctr));
    SwitchTable t;
    t.bctr = bctr;
    t.index_reg = guard->reg;
    t.kind = shape.kind;
    for (std::uint32_t i = 0; i < guard->count; ++i) {
        CB_TRY_ASSIGN(const std::uint32_t v, table_entry(shape, i));
        t.labels.push_back(v);
    }
    t.default_target = guard->default_target;
    t.guard = guard->addr;
    return std::optional<SwitchTable>(std::move(t));
}

// An indexed load (lwzx/lhzx/lbzx) precedes the bctr in straight-line code.
bool Scan::indexed_load_before(std::uint32_t bctr, int window) const {
    for (int i = 1; i < window; ++i) {
        const std::int64_t a = static_cast<std::int64_t>(bctr) - 4 * i;
        if (a < text_start) break;
        const Insn d = insn(a);
        if (is_branch(d.kind)) break;
        if (d.kind == Kind::Loadx) return true;
    }
    return false;
}

// The functions are sorted by begin: the one that contains addr, if any.
std::optional<Extent> containing(std::span<const Extent> funcs, std::uint32_t addr) {
    auto it = std::upper_bound(funcs.begin(), funcs.end(), addr, [](std::uint32_t a, const Extent& f) { return a < f.begin; });
    if (it == funcs.begin()) return std::nullopt;
    --it;
    if (it->begin <= addr && addr < it->end) return *it;
    return std::nullopt;
}

// The bounds of the .pdata function containing addr, else the gap between the .pdata functions around it.
Extent extent_around(std::span<const Extent> funcs, std::uint32_t addr, std::uint32_t text_start, std::uint32_t text_end) {
    if (const auto fn = containing(funcs, addr)) return *fn;
    const auto it = std::upper_bound(funcs.begin(), funcs.end(), addr, [](std::uint32_t a, const Extent& f) { return a < f.begin; });
    const std::uint32_t lo = it != funcs.begin() ? (it - 1)->end : text_start;
    const std::uint32_t hi = it != funcs.end() ? it->begin : text_end;
    return {lo, hi};
}

}  // namespace

std::string_view kind_name(TableKind kind) {
    switch (kind) {
        case TableKind::Absolute: return "absolute";
        case TableKind::Byte: return "byte";
        case TableKind::Short: return "short";
        case TableKind::XenonAnalyse: return "XenonAnalyse";
    }
    return "absolute";
}

Found find(const CodeImage& image) {
    Scan scan(image);
    scan.text_start = image.code_begin;
    scan.text_end = image.code_end;
    Found out;
    for (std::uint64_t at = image.code_begin; at < image.code_end; at += 4) {
        const std::uint32_t a = static_cast<std::uint32_t>(at);
        if (scan.word(a) != kBctr) continue;
        auto analysed = scan.analyze(a);
        if (!analysed.ok()) {
            out.report.unresolved.push_back({a, analysed.error().what()});
            continue;
        }
        std::optional<SwitchTable> t = std::move(analysed).value();
        if (!t) {
            t = scan.analyze_unguarded(a, extent_around(image.functions, a, image.code_begin, image.code_end));
            if (!t) {
                if (scan.indexed_load_before(a)) out.report.pointer_calls.push_back(a);
                continue;
            }
        }
        const std::optional<Extent> fn = containing(image.functions, a);
        // fn.end: the unreachable-default slot
        if (fn && std::any_of(t->labels.begin(), t->labels.end(), [&](std::uint32_t l) { return l < fn->begin || l > fn->end; })) {
            out.report.outside.push_back({a, *fn});
        }
        t->function = fn;
        ++out.report.kinds[t->kind];
        out.tables.push_back(std::move(*t));
    }
    out.report.tables = out.tables.size();
    return out;
}

std::string format_file(std::span<const SwitchTable> tables) {
    std::string text = "# Switch tables, found by CodeBreaker (codebreaker/recompile/switches.py)\n\n";
    for (const SwitchTable& t : tables) {
        if (!t.guard) {
            const std::string where = t.inline_table ? "inline after the bctr" : std::format("at 0x{:X}", t.table);
            text += std::format("# {} table {}, no bounds check (length from its entries)\n", kind_name(t.kind), where);
        } else {
            text += std::format("# {} table, guard at 0x{:X}\n", kind_name(t.kind), *t.guard);
        }
        text += std::format("[[switch]]\nbase = 0x{:X}\nr = {}\ndefault = 0x{:X}\nlabels = [\n", t.bctr, t.index_reg, t.default_target);
        for (const std::uint32_t l : t.labels) text += std::format("    0x{:X},\n", l);
        text += "]\n\n";
    }
    return text;
}

void write(const std::filesystem::path& path, std::span<const SwitchTable> tables) {
    fs::write_text(path, format_file(tables));
}

}  // namespace cb::recompile::switches
