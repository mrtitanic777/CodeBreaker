// cb-unity: alone
// emulate/cpu/pcode_neon_arm32.cpp -- see pcode_neon_arm32.hpp. Each instruction is the lane library's operation on the bytes of the registers it names (ARM ARM A8.8); what the
// library does not report (the saturation of a result, QC) is found here from the lanes; the floating point is the library's with the "standard FPSCR value" of Advanced SIMD:
// round to nearest, flush to zero, default NaN.
#include "emulate/cpu/pcode_neon_arm32.hpp"

#include <bit>
#include <cmath>
#include <cstring>

#include "emulate/cpu/pcode_vector.hpp"

namespace cb::emulate::pcode_neon_arm32 {

namespace vec = cb::emulate::vec;
namespace t2 = cb::decompile::arm::t2;
using Op = t2::NeonOp;
using Byte = std::uint8_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;

static_assert(std::endian::native == std::endian::little, "the registers are held as the host's integers");

namespace {

vec::FpEnv simdEnv() {
    vec::FpEnv env;
    env.round = vec::Round::NearestEven;
    env.nan = vec::NanRule::Arm;
    env.minmax = vec::MinMax::ArmPropagate;
    env.flushInputs = true;
    env.flushResults = true;
    env.defaultNan = true;
    return env;
}

// The library's exception bits as the status register's.
u32 fpscrFlags(unsigned f) {
    return (f & vec::kInvalid ? 1u : 0u) | (f & vec::kDivideByZero ? 2u : 0u) | (f & vec::kOverflow ? 4u : 0u) | (f & vec::kUnderflow ? 8u : 0u) | (f & vec::kInexact ? 16u : 0u) |
           (f & vec::kInputDenormal ? 128u : 0u);
}

std::uint32_t maskOf(unsigned first, unsigned count) {
    std::uint32_t m = 0;
    for (unsigned k = 0; k < count; ++k)
        if (first + k < 32) m |= 1u << (first + k);
    return m;
}

// ARM's estimate tables as the manual computes them (RecipEstimate and RecipSqrtEstimate of A2.7.3: a 9 bit fixed point number in, 256 to 511 out).
u32 recipEstimate9(u32 a) {
    a = a * 2 + 1;
    const u32 b = (1u << 19) / a;
    return (b + 1) / 2;
}
u32 rsqrtEstimate9(u32 a) {
    if (a < 256) a = a * 2 + 1;
    else a = (((a >> 1) << 1) + 1) * 2;
    u64 b = 512;
    while (static_cast<u64>(a) * (b + 1) * (b + 1) < (1ull << 28)) ++b;
    return static_cast<u32>((b + 1) / 2);
}

struct Machine {
    const t2::NeonDp& x;
    Regs& regs;
    unsigned eb, nb, bits;
    vec::Sign sign;
    Byte N[16] = {}, M[16] = {}, D[16] = {}, O[16] = {}, O2[16] = {};
    unsigned flags = 0;
    bool qc = false;
    vec::FpEnv env = simdEnv();

    Machine(const t2::NeonDp& op, Regs& r) : x(op), regs(r), eb(1u << op.size), nb(op.q ? 16 : 8), bits(8u << op.size), sign(op.u ? vec::Sign::Unsigned : vec::Sign::Signed) {}

    void load(Byte* dst, unsigned reg, unsigned count) const {
        for (unsigned k = 0; k < count; ++k) std::memcpy(dst + 8 * k, &regs.d[(reg + k) & 31], 8);
    }
    void put(unsigned reg, const Byte* src, unsigned bytes) {
        for (unsigned k = 0; k < bytes / 8; ++k) std::memcpy(&regs.d[(reg + k) & 31], src + 8 * k, 8);
    }
    u64 lane(const Byte* a, unsigned i, unsigned ebytes) const { return vec::getLane(a, ebytes, i); }
    i64 slane(const Byte* a, unsigned i, unsigned ebytes) const { return vec::signExtend(vec::getLane(a, ebytes, i), ebytes); }
    bool differ(const Byte* a, const Byte* b, unsigned bytes) const { return std::memcmp(a, b, bytes) != 0; }
    static bool isMin(u64 v, unsigned ebytes) { return v == (1ull << (8 * ebytes - 1)); }

    // A saturating add or subtract: the result is the library's, QC is whether a lane differs from the wrapping one (the exact result of one add is less than a lane's width
    // outside the range, so a clamped lane never equals the wrapped one).
    void addSub(bool sub, bool sat) {
        if (sat) {
            Byte wrapped[16];
            if (sub) {
                vec::subSat(O, N, M, nb, eb, sign);
                vec::sub(wrapped, N, M, nb, eb);
            } else {
                vec::addSat(O, N, M, nb, eb, sign);
                vec::add(wrapped, N, M, nb, eb);
            }
            qc = qc || differ(O, wrapped, nb);
        } else if (sub) {
            vec::sub(O, N, M, nb, eb);
        } else {
            vec::add(O, N, M, nb, eb);
        }
    }

    // Does a left shift of a lane by `count` overflow the lane (as a signed or an unsigned number)?
    bool leftOverflows(u64 raw, unsigned count, unsigned ebytes, bool is_signed) const {
        const unsigned w = 8 * ebytes;
        if (raw == 0) return false;
        if (count >= w) return true;
        const u64 mask = w == 64 ? ~0ull : (1ull << w) - 1;
        const u64 wrapped = (raw << count) & mask;
        if (is_signed) return (vec::signExtend(wrapped, ebytes) >> count) != vec::signExtend(raw, ebytes);
        return (wrapped >> count) != raw;
    }

    // Does the lane, shifted right by `count` (rounded when `round`), lie outside what `out_bytes` hold as a signed or an unsigned number?
    bool narrowOverflows(u64 raw, unsigned ebytes, unsigned count, bool round, bool src_signed, bool dst_signed, unsigned out_bytes) const {
        const unsigned ob = 8 * out_bytes;
        if (src_signed) {
            const i64 v = vec::signExtend(raw, ebytes);
            i64 y = v;
            if (count > 0) y = (v >> count) + (round ? ((v >> (count - 1)) & 1) : 0);
            if (dst_signed) return y < -(i64{1} << (ob - 1)) || y > (i64{1} << (ob - 1)) - 1;
            return y < 0 || static_cast<u64>(y) > ((1ull << ob) - 1);
        }
        u64 y = raw;
        if (count > 0) y = (raw >> count) + (round ? ((raw >> (count - 1)) & 1) : 0);
        return y > ((1ull << ob) - 1);
    }

    u32 fpRecipEstimate(u32 op) {
        if ((op & 0x7F800000u) == 0 && (op & 0x7FFFFFu)) {   // a denormal input is read as a zero
            flags |= vec::kInputDenormal;
            op &= 0x80000000u;
        }
        if ((op & 0x7FFFFFFFu) > 0x7F800000u) {
            if (!(op & 0x400000u)) flags |= vec::kInvalid;
            return 0x7FC00000u;
        }
        const u32 sgn = op & 0x80000000u, exp = op >> 23 & 0xFF, frac = op & 0x7FFFFFu;
        if (exp == 0xFF) return sgn;
        if (exp == 0) {
            flags |= vec::kDivideByZero;
            return sgn | 0x7F800000u;
        }
        if (exp >= 253) {
            flags |= vec::kUnderflow;
            return sgn;
        }
        const u32 est = recipEstimate9(0x100 | frac >> 15);
        return sgn | (253 - exp) << 23 | (est & 0xFF) << 15;
    }
    u32 fpRsqrtEstimate(u32 op) {
        if ((op & 0x7F800000u) == 0 && (op & 0x7FFFFFu)) {
            flags |= vec::kInputDenormal;
            op &= 0x80000000u;
        }
        if ((op & 0x7FFFFFFFu) > 0x7F800000u) {
            if (!(op & 0x400000u)) flags |= vec::kInvalid;
            return 0x7FC00000u;
        }
        const u32 sgn = op & 0x80000000u, exp = op >> 23 & 0xFF, frac = op & 0x7FFFFFu;
        if (exp == 0) {
            flags |= vec::kDivideByZero;
            return sgn | 0x7F800000u;
        }
        if (sgn) {
            flags |= vec::kInvalid;
            return 0x7FC00000u;
        }
        if (exp == 0xFF) return 0;
        const u32 scaled = (exp & 1) == 0 ? 0x100 | frac >> 15 : 0x80 | frac >> 16;
        const u32 est = rsqrtEstimate9(scaled);
        return ((380 - exp) / 2) << 23 | (est & 0xFF) << 15;
    }

    // The VFP forms (fused multiply-add, the conversions with fraction bits, half precision, two core registers and two singles) with the status register's control bits clear:
    // no flush, the NaN of an operand is the result, round to nearest.
    u32 sget(unsigned k) const { return static_cast<u32>(regs.d[(k >> 1) & 31] >> (32 * (k & 1))); }
    void sset(unsigned k, u32 v) {
        u64& d = regs.d[(k >> 1) & 31];
        d = (d & ~(0xFFFFFFFFull << (32 * (k & 1)))) | static_cast<u64>(v) << (32 * (k & 1));
    }
    void vfp() {
        vec::FpEnv e;
        e.nan = vec::NanRule::Arm;
        const bool dbl = x.size == 3;
        const unsigned ew = dbl ? 8 : 4;
        const auto getb = [&](Byte* out, unsigned r) {   // the register's bytes (a single or a double)
            if (dbl) std::memcpy(out, &regs.d[r & 31], 8);
            else vec::store(out, 4, sget(r));
        };
        const auto putb = [&](unsigned r, const Byte* in) {
            if (dbl) std::memcpy(&regs.d[r & 31], in, 8);
            else sset(r, static_cast<u32>(vec::load(in, 4)));
        };
        switch (x.op) {
            case Op::VFma: {
                Byte a[8], b[8], c[8], r[8];
                getb(a, x.n);
                getb(b, x.m);
                getb(c, x.d);
                const bool neg_product = x.imm == 1 || x.imm == 2, neg_addend = x.imm == 2 || x.imm == 3;   // (negated first, as the manual does: a NaN operand's sign changes with it)
                if (neg_product) a[ew - 1] ^= 0x80;
                if (neg_addend) c[ew - 1] ^= 0x80;
                vec::fp3(r, a, b, c, ew, ew, vec::Fp3::MulAdd, e, flags);
                putb(x.d, r);
                break;
            }
            case Op::VCvtFx: {
                const unsigned width_bytes = x.idx / 8;
                const vec::Sign sg = x.u ? vec::Sign::Unsigned : vec::Sign::Signed;
                Byte a[8], r[8] = {};
                if (x.acc) {   // to fixed point, toward zero
                    getb(a, x.d);
                    e.round = vec::Round::TowardZero;
                    vec::floatToInt(r, a, ew, ew, width_bytes, sg, x.imm, vec::IntOverflow::Saturate, e, flags);
                    u64 v = vec::load(r, width_bytes);
                    if (!x.u) v = static_cast<u64>(vec::signExtend(v, width_bytes));
                    if (!dbl) v &= 0xFFFFFFFFull;
                    Byte out[8];
                    vec::store(out, 8, v);
                    putb(x.d, out);
                } else {
                    Byte raw[8];
                    getb(raw, x.d);
                    vec::intToFloat(r, raw, width_bytes, width_bytes, ew, sg, x.imm, e, flags);
                    putb(x.d, r);
                }
                break;
            }
            case Op::VCvtHalf: {
                Byte a[4], r[4] = {};
                vec::store(a, 4, sget(x.m));
                if (x.acc) {   // single to half
                    vec::floatConvert(r, a, 4, 4, 2, e, flags);
                    const u32 h = static_cast<u32>(vec::load(r, 2)), old = sget(x.d);
                    sset(x.d, x.imm ? (old & 0xFFFFu) | h << 16 : (old & 0xFFFF0000u) | h);
                } else {
                    vec::store(a, 2, x.imm ? sget(x.m) >> 16 : sget(x.m) & 0xFFFFu);
                    unsigned seen = 0;
                    vec::floatConvert(r, a, 2, 2, 4, e, seen);
                    flags |= seen & ~vec::kInputDenormal;
                    sset(x.d, static_cast<u32>(vec::load(r, 4)));
                }
                break;
            }
            default:   // VMovPair
                if (x.u) {
                    regs.r[x.rt] = sget(x.m);
                    regs.r[x.idx] = sget(x.m + 1);
                } else {
                    sset(x.m, regs.r[x.rt]);
                    sset(x.m + 1, regs.r[x.idx]);
                }
                break;
        }
    }

    // Returns the bytes of the destination that are written and sets writes_m when Vm is written too.
    unsigned run(bool& writes_m);
};

unsigned Machine::run(bool& writes_m) {
    if (x.op >= Op::VFma) {
        vfp();
        flags &= ~vec::kInputDenormal;   // (a denormal input is reported only when the flush to zero is on)
        return 0;
    }
    const unsigned q = x.q ? 2 : 1;
    unsigned out_bytes = nb;
    load(N, x.n, 2);
    load(D, x.d, 2);
    if (x.scalar) {
        Byte s[8];
        load(s, x.m, 1);
        for (unsigned i = 0; i < nb / eb; ++i) vec::setLane(M, eb, i, vec::getLane(s, eb, x.idx));
    } else {
        load(M, x.m, 2);
    }
    switch (x.op) {
        case Op::Hadd: vec::halvingAdd(O, N, M, nb, eb, sign, false); break;
        case Op::Rhadd: vec::halvingAdd(O, N, M, nb, eb, sign, true); break;
        case Op::Hsub: vec::halvingSub(O, N, M, nb, eb, sign); break;
        case Op::Qadd: addSub(false, true); break;
        case Op::Qsub: addSub(true, true); break;
        case Op::Add: addSub(false, false); break;
        case Op::Sub: addSub(true, false); break;
        case Op::Cgt: vec::compare(O, N, M, nb, eb, vec::Cmp::Gt, sign); break;
        case Op::Cge: vec::compare(O, N, M, nb, eb, vec::Cmp::Ge, sign); break;
        case Op::Ceq: vec::compare(O, N, M, nb, eb, vec::Cmp::Eq, sign); break;
        case Op::Tst: {
            Byte t[16], zero[16] = {};
            vec::bitAnd(t, N, M, nb);
            vec::compare(O, t, zero, nb, eb, vec::Cmp::Ne, vec::Sign::Unsigned);
            break;
        }
        case Op::ShiftReg:   // the value is Vm, the counts are Vn
            vec::shiftNeon(O, M, N, nb, eb, sign, x.round, x.sat);
            if (x.sat)
                for (unsigned i = 0; i < nb / eb; ++i) {
                    const int count = static_cast<std::int8_t>(lane(N, i, eb) & 0xFF);
                    if (count > 0 && leftOverflows(lane(M, i, eb), static_cast<unsigned>(count), eb, !x.u)) qc = true;
                }
            break;
        case Op::Max: vec::max(O, N, M, nb, eb, sign); break;
        case Op::Min: vec::min(O, N, M, nb, eb, sign); break;
        case Op::Abd: vec::absDiff(O, N, M, nb, eb, sign); break;
        case Op::Aba: {
            Byte t[16];
            vec::absDiff(t, N, M, nb, eb, sign);
            vec::add(O, D, t, nb, eb);
            break;
        }
        case Op::Mla: case Op::Mls: {
            Byte t[16];
            vec::mul(t, N, M, nb, eb);
            if (x.op == Op::Mla) vec::add(O, D, t, nb, eb);
            else vec::sub(O, D, t, nb, eb);
            break;
        }
        case Op::Mul: vec::mul(O, N, M, nb, eb); break;
        case Op::MulP: vec::polyMul(O, N, M, nb, 1); break;
        case Op::Pmax: vec::pairwise(O, N, M, 8, eb, x.u ? vec::Pair::MaxUnsigned : vec::Pair::MaxSigned); break;
        case Op::Pmin: vec::pairwise(O, N, M, 8, eb, x.u ? vec::Pair::MinUnsigned : vec::Pair::MinSigned); break;
        case Op::Padd: vec::pairwise(O, N, M, 8, eb, vec::Pair::Add); break;
        case Op::Qdmulh:
            vec::mulDoublingHigh(O, N, M, nb, eb, x.round);
            for (unsigned i = 0; i < nb / eb; ++i)
                if (isMin(lane(N, i, eb), eb) && isMin(lane(M, i, eb), eb)) qc = true;
            break;
        case Op::And: vec::bitAnd(O, N, M, nb); break;
        case Op::Bic: vec::bitAndNot(O, N, M, nb); break;
        case Op::Orr: vec::bitOr(O, N, M, nb); break;
        case Op::Orn: vec::bitOrNot(O, N, M, nb); break;
        case Op::Eor: vec::bitXor(O, N, M, nb); break;
        case Op::Bsl: vec::bitSelect(O, D, N, M, nb); break;
        case Op::Bit: vec::bitSelect(O, M, N, D, nb); break;
        case Op::Bif: vec::bitSelect(O, M, D, N, nb); break;
        // ---- floating point, three registers (single precision)
        case Op::FAdd: vec::fp2(O, N, M, nb, 4, vec::Fp2::Add, env, flags); break;
        case Op::FSub: vec::fp2(O, N, M, nb, 4, vec::Fp2::Sub, env, flags); break;
        case Op::FMul: vec::fp2(O, N, M, nb, 4, vec::Fp2::Mul, env, flags); break;
        case Op::FAbd: vec::fp2(O, N, M, nb, 4, vec::Fp2::AbsDiff, env, flags); break;
        case Op::FMla: case Op::FMls: {
            Byte t[16];
            vec::fp2(t, N, M, nb, 4, vec::Fp2::Mul, env, flags);
            vec::fp2(O, D, t, nb, 4, x.op == Op::FMla ? vec::Fp2::Add : vec::Fp2::Sub, env, flags);
            break;
        }
        case Op::Fma: vec::fp3(O, N, M, D, nb, 4, vec::Fp3::MulAdd, env, flags); break;
        case Op::Fms: vec::fp3(O, N, M, D, nb, 4, vec::Fp3::NegMulAdd, env, flags); break;
        case Op::FCeq: vec::fpCompare(O, N, M, nb, 4, vec::FpCmp::Eq, false, env, flags); break;
        case Op::FCge: vec::fpCompare(O, N, M, nb, 4, vec::FpCmp::Ge, true, env, flags); break;
        case Op::FCgt: vec::fpCompare(O, N, M, nb, 4, vec::FpCmp::Gt, true, env, flags); break;
        case Op::FAcge: case Op::FAcgt: {
            Byte a[16], b[16];
            vec::fp1(a, N, nb, 4, vec::Fp1::Abs, env, flags);
            vec::fp1(b, M, nb, 4, vec::Fp1::Abs, env, flags);
            vec::fpCompare(O, a, b, nb, 4, x.op == Op::FAcge ? vec::FpCmp::Ge : vec::FpCmp::Gt, true, env, flags);
            break;
        }
        case Op::FMax: vec::fp2(O, N, M, nb, 4, vec::Fp2::Max, env, flags); break;
        case Op::FMin: vec::fp2(O, N, M, nb, 4, vec::Fp2::Min, env, flags); break;
        case Op::FPadd: case Op::FPmax: case Op::FPmin: {   // the pairs n0 n1 and m0 m1: the operation on [n0, m0] and [n1, m1]
            Byte a[8], b[8];
            std::memcpy(a, N, 4);
            std::memcpy(a + 4, M, 4);
            std::memcpy(b, N + 4, 4);
            std::memcpy(b + 4, M + 4, 4);
            vec::fp2(O, a, b, 8, 4, x.op == Op::FPadd ? vec::Fp2::Add : x.op == Op::FPmax ? vec::Fp2::Max : vec::Fp2::Min, env, flags);
            break;
        }
        case Op::Recps: case Op::Rsqrts: {   // 2 - n m and (3 - n m) / 2, the multiply rounded first; an infinity times a zero gives the constant
            const bool rsqrt = x.op == Op::Rsqrts;
            for (unsigned i = 0; i < nb / 4; ++i) {
                const u32 a = static_cast<u32>(lane(N, i, 4)), b = static_cast<u32>(lane(M, i, 4));
                const bool az = (a & 0x7F800000) == 0, bz = (b & 0x7F800000) == 0;   // (zero, or a denormal that is read as zero)
                const bool ai = (a & 0x7FFFFFFF) == 0x7F800000, bi = (b & 0x7FFFFFFF) == 0x7F800000;
                const bool nan = (a & 0x7FFFFFFF) > 0x7F800000 || (b & 0x7FFFFFFF) > 0x7F800000;
                if (!nan && ((ai && bz) || (az && bi))) {
                    if (az && (a & 0x7FFFFF)) flags |= vec::kInputDenormal;
                    if (bz && (b & 0x7FFFFF)) flags |= vec::kInputDenormal;
                    vec::setLane(O, 4, i, rsqrt ? 0x3FC00000u : 0x40000000u);
                    continue;
                }
                Byte pa[4], pb[4], pm[4], k[4], r[4];
                vec::store(pa, 4, a);
                vec::store(pb, 4, b);
                vec::fp2(pm, pa, pb, 4, 4, vec::Fp2::Mul, env, flags);
                vec::store(k, 4, rsqrt ? 0x40400000u : 0x40000000u);
                vec::fp2(r, k, pm, 4, 4, vec::Fp2::Sub, env, flags);
                if (rsqrt) {
                    Byte half[4];
                    vec::store(half, 4, 0x3F000000u);
                    vec::fp2(r, r, half, 4, 4, vec::Fp2::Mul, env, flags);
                }
                vec::setLane(O, 4, i, vec::load(r, 4));
            }
            break;
        }
        // ---- different lengths: D sources, Q destination
        case Op::Addl: case Op::Subl: case Op::Addw: case Op::Subw: {
            Byte a[16], b[16];
            if (x.op == Op::Addl || x.op == Op::Subl) vec::widen(a, N, 8, eb, sign);
            else std::memcpy(a, N, 16);
            vec::widen(b, M, 8, eb, sign);
            if (x.op == Op::Addl || x.op == Op::Addw) vec::add(O, a, b, 16, 2 * eb);
            else vec::sub(O, a, b, 16, 2 * eb);
            out_bytes = 16;
            break;
        }
        case Op::Abal: case Op::Abdl: {
            Byte t[8], w[16];
            vec::absDiff(t, N, M, 8, eb, sign);
            vec::widen(w, t, 8, eb, vec::Sign::Unsigned);
            if (x.op == Op::Abal) vec::add(O, D, w, 16, 2 * eb);
            else std::memcpy(O, w, 16);
            out_bytes = 16;
            break;
        }
        case Op::Mlal: case Op::Mlsl: case Op::Mull: {
            Byte p[16];
            vec::mulWiden(p, N, M, 8, eb, sign, sign);
            if (x.op == Op::Mlal) vec::add(O, D, p, 16, 2 * eb);
            else if (x.op == Op::Mlsl) vec::sub(O, D, p, 16, 2 * eb);
            else std::memcpy(O, p, 16);
            out_bytes = 16;
            break;
        }
        case Op::MullP: vec::polyMulWiden(O, N, M, 8, 1); out_bytes = 16; break;
        case Op::Qdmull: case Op::Qdmlal: case Op::Qdmlsl: {
            Byte p[16], dbl[16];
            vec::mulWiden(p, N, M, 8, eb, vec::Sign::Signed, vec::Sign::Signed);
            vec::add(dbl, p, p, 16, 2 * eb);
            for (unsigned i = 0; i < 8 / eb; ++i)   // (the one product that doubles past the range: the minimum times itself)
                if (isMin(lane(N, i, eb), eb) && isMin(lane(M, i, eb), eb)) {
                    qc = true;
                    vec::setLane(dbl, 2 * eb, i, (1ull << (16 * eb - 1)) - 1);
                }
            if (x.op == Op::Qdmull) {
                std::memcpy(O, dbl, 16);
            } else {
                Byte wrapped[16];
                if (x.op == Op::Qdmlal) {
                    vec::addSat(O, D, dbl, 16, 2 * eb, vec::Sign::Signed);
                    vec::add(wrapped, D, dbl, 16, 2 * eb);
                } else {
                    vec::subSat(O, D, dbl, 16, 2 * eb, vec::Sign::Signed);
                    vec::sub(wrapped, D, dbl, 16, 2 * eb);
                }
                qc = qc || differ(O, wrapped, 16);
            }
            out_bytes = 16;
            break;
        }
        case Op::AddHn: case Op::SubHn: {   // Q sources, D destination: the high half of the sum or difference, rounded when asked
            Byte a[16], b[16], s[16];
            load(a, x.n, 2);
            load(b, x.m, 2);
            if (x.op == Op::AddHn) vec::add(s, a, b, 16, 2 * eb);
            else vec::sub(s, a, b, 16, 2 * eb);
            vec::narrowShiftRight(O, s, 16, 2 * eb, 8 * eb, x.round, vec::Narrow::Truncate);
            out_bytes = 8;
            break;
        }
        // ---- shifts by an immediate
        case Op::Shr:
            vec::shiftRightImm(O, M, nb, eb, sign, x.imm, x.round);
            if (x.acc) vec::add(O, D, O, nb, eb);
            break;
        case Op::Sri: case Op::Sli: vec::shiftInsert(O, D, M, nb, eb, x.op == Op::Sli, x.imm); break;
        case Op::ShlI: vec::shiftImm(O, M, nb, eb, vec::Shift::Left, x.imm, vec::Count::Saturate); break;
        case Op::Qshlu:
            vec::shiftLeftSatImm(O, M, nb, eb, vec::Sign::Signed, x.imm, true);
            for (unsigned i = 0; i < nb / eb; ++i) {
                const u64 raw = lane(M, i, eb);
                if (raw != 0 && (slane(M, i, eb) < 0 || leftOverflows(raw, x.imm, eb, false))) qc = true;
            }
            break;
        case Op::QshlI:
            vec::shiftLeftSatImm(O, M, nb, eb, sign, x.imm, false);
            for (unsigned i = 0; i < nb / eb; ++i)
                if (leftOverflows(lane(M, i, eb), x.imm, eb, !x.u)) qc = true;
            break;
        case Op::ShrN: case Op::Qshrn: case Op::Qshrun: {   // Q source, D destination; the destination's element is size
            const unsigned se = 2 * eb;
            const vec::Narrow mode = x.op == Op::ShrN ? vec::Narrow::Truncate : x.op == Op::Qshrun ? vec::Narrow::SignedToUnsigned : x.u ? vec::Narrow::UnsignedToUnsigned : vec::Narrow::SignedToSigned;
            vec::narrowShiftRight(O, M, 16, se, x.imm, x.round, mode);
            if (x.op != Op::ShrN)
                for (unsigned i = 0; i < 8 / eb; ++i)
                    if (narrowOverflows(lane(M, i, se), se, x.imm, x.round, x.op == Op::Qshrun || !x.u, x.op == Op::Qshrn ? !x.u : false, eb)) qc = true;
            out_bytes = 8;
            break;
        }
        case Op::Shll: {
            Byte w[16];
            vec::widen(w, M, 8, eb, sign);
            vec::shiftImm(O, w, 16, 2 * eb, vec::Shift::Left, x.imm, vec::Count::Saturate);
            out_bytes = 16;
            break;
        }
        case Op::CvtFixed: {
            vec::FpEnv e = env;
            if (x.acc) {
                e.round = vec::Round::TowardZero;
                vec::floatToInt(O, M, nb, 4, 4, sign, x.imm, vec::IntOverflow::Saturate, e, flags);
            } else {
                vec::intToFloat(O, M, nb, 4, 4, sign, x.imm, e, flags);
            }
            break;
        }
        // ---- two registers, miscellaneous
        case Op::Rev: vec::reverseInGroups(O, M, nb, eb, x.imm); break;
        case Op::Paddl: vec::addLongPairwise(O, nullptr, M, nb, eb, sign); break;
        case Op::Padal: vec::addLongPairwise(O, D, M, nb, eb, sign); break;
        case Op::Cls: vec::bitOp(O, M, nb, eb, vec::BitOp::CountLeadingSigns); break;
        case Op::Clz: vec::bitOp(O, M, nb, eb, vec::BitOp::CountLeadingZeros); break;
        case Op::Cnt: vec::bitOp(O, M, nb, 1, vec::BitOp::Popcount); break;
        case Op::Mvn: vec::bitNot(O, M, nb); break;
        case Op::Qabs: case Op::Qneg:
            if (x.op == Op::Qabs) vec::abs(O, M, nb, eb, true);
            else vec::neg(O, M, nb, eb, true);
            for (unsigned i = 0; i < nb / eb; ++i)
                if (isMin(lane(M, i, eb), eb)) qc = true;
            break;
        case Op::CmpZero: {
            Byte zero[16] = {};
            if (x.f) {
                const vec::FpCmp cmp = x.imm == 0 ? vec::FpCmp::Gt : x.imm == 1 ? vec::FpCmp::Ge : x.imm == 2 ? vec::FpCmp::Eq : x.imm == 3 ? vec::FpCmp::Le : vec::FpCmp::Lt;
                vec::fpCompare(O, M, zero, nb, 4, cmp, x.imm != 2, env, flags);
            } else {
                const vec::Cmp cmp = x.imm == 0 ? vec::Cmp::Gt : x.imm == 1 ? vec::Cmp::Ge : x.imm == 2 ? vec::Cmp::Eq : x.imm == 3 ? vec::Cmp::Le : vec::Cmp::Lt;
                vec::compare(O, M, zero, nb, eb, cmp, vec::Sign::Signed);
            }
            break;
        }
        case Op::Abs: case Op::Neg:
            if (x.f) {   // (a bit operation: no flush, no NaN processing)
                Byte mask[16];
                vec::splat(mask, nb, 4, x.op == Op::Abs ? 0x7FFFFFFFu : 0x80000000u);
                if (x.op == Op::Abs) vec::bitAnd(O, M, mask, nb);
                else vec::bitXor(O, M, mask, nb);
            } else if (x.op == Op::Abs) {
                vec::abs(O, M, nb, eb, false);
            } else {
                vec::neg(O, M, nb, eb, false);
            }
            break;
        case Op::Swp: std::memcpy(O, M, nb); std::memcpy(O2, D, nb); writes_m = true; break;
        case Op::Trn: vec::transpose(O, D, M, nb, eb, false); vec::transpose(O2, D, M, nb, eb, true); writes_m = true; break;
        case Op::Uzp: vec::unzip(O, D, M, nb, eb, false); vec::unzip(O2, D, M, nb, eb, true); writes_m = true; break;
        case Op::Zip: vec::zip(O, D, M, nb, eb, false); vec::zip(O2, D, M, nb, eb, true); writes_m = true; break;
        case Op::Movn: vec::narrow(O, M, 16, 2 * eb, vec::Narrow::Truncate); out_bytes = 8; break;
        case Op::Qmovn: case Op::Qmovun: {
            const bool unsigned_result = x.op == Op::Qmovun;
            vec::narrow(O, M, 16, 2 * eb, unsigned_result ? vec::Narrow::SignedToUnsigned : x.u ? vec::Narrow::UnsignedToUnsigned : vec::Narrow::SignedToSigned);
            for (unsigned i = 0; i < 8 / eb; ++i)
                if (narrowOverflows(lane(M, i, 2 * eb), 2 * eb, 0, false, unsigned_result || !x.u, unsigned_result ? false : !x.u, eb)) qc = true;
            out_bytes = 8;
            break;
        }
        case Op::Cvt16: {
            vec::FpEnv e = env;
            e.flushResults = false;
            if (x.acc) {   // half to single: D source, Q destination
                e.flushInputs = false;
                unsigned seen = 0;
                vec::floatConvert(O, M, 8, 2, 4, e, seen);
                flags |= seen & ~vec::kInputDenormal;   // (a denormal half is no input denormal)
                out_bytes = 16;
            } else {
                vec::floatConvert(O, M, 16, 4, 2, e, flags);
                out_bytes = 8;
            }
            break;
        }
        case Op::Recpe: case Op::Rsqrte:
            for (unsigned i = 0; i < nb / 4; ++i) {
                const u32 a = static_cast<u32>(lane(M, i, 4));
                u32 r;
                if (x.f) r = x.op == Op::Recpe ? fpRecipEstimate(a) : fpRsqrtEstimate(a);
                else if (x.op == Op::Recpe) r = (a & 0x80000000u) == 0 ? 0xFFFFFFFFu : recipEstimate9(a >> 23) << 23;
                else r = (a >> 30) == 0 ? 0xFFFFFFFFu : rsqrtEstimate9((a & 0x80000000u) ? a >> 23 : (a >> 23) & 0xFF) << 23;
                vec::setLane(O, 4, i, r);
            }
            break;
        case Op::CvtIntFloat: {
            vec::FpEnv e = env;
            if (x.imm >= 2) {
                e.round = vec::Round::TowardZero;
                vec::floatToInt(O, M, nb, 4, 4, x.imm == 2 ? vec::Sign::Signed : vec::Sign::Unsigned, 0, vec::IntOverflow::Saturate, e, flags);
            } else {
                vec::intToFloat(O, M, nb, 4, 4, x.imm == 0 ? vec::Sign::Signed : vec::Sign::Unsigned, 0, e, flags);
            }
            break;
        }
        case Op::Ext: vec::extract(O, N, M, nb, x.imm); break;
        case Op::Tbl: {
            Byte table[32] = {};
            load(table, x.n, x.imm + 1);
            std::memcpy(O, D, 8);
            vec::Lookup mode;
            mode.indexMask = 0xFF;
            mode.keepOutOfRange = x.tbx;
            vec::tableLookup(O, table, 8 * (x.imm + 1), M, 8, mode);
            out_bytes = 8;
            break;
        }
        case Op::DupScalar: vec::splat(O, nb, eb, lane(M, x.idx, eb)); break;
        case Op::MovToScalar:
            std::memcpy(O, D, 8);
            vec::setLane(O, eb, x.idx, regs.r[x.rt]);
            out_bytes = 8;
            break;
        case Op::MovFromScalar: {
            const u64 raw = lane(M, x.idx, eb);
            regs.r[x.rt] = eb == 4 || x.u ? static_cast<u32>(raw) : static_cast<u32>(vec::signExtend(raw, eb));
            out_bytes = 0;
            break;
        }
        case Op::DupCore: vec::splat(O, nb, eb, regs.r[x.rt] & vec::laneMask(eb)); break;
    }
    (void)q;
    return out_bytes;
}

}  // namespace

std::uint32_t reads(const t2::NeonDp& op) {
    if (op.op >= Op::VFma) {
        const bool dbl = op.size == 3;
        if (op.op == Op::VMovPair) return maskOf(op.m >> 1, 1) | maskOf((op.m + 1) >> 1, 1);
        if (op.op == Op::VCvtHalf) return maskOf(op.d >> 1, 1) | maskOf(op.m >> 1, 1);
        if (dbl) return maskOf(op.n, 1) | maskOf(op.m, 1) | maskOf(op.d, 1);
        return maskOf(op.n >> 1, 1) | maskOf(op.m >> 1, 1) | maskOf(op.d >> 1, 1);
    }
    if (op.op == Op::Tbl) return maskOf(op.n, op.imm + 1) | maskOf(op.m, 1) | maskOf(op.d, 1);
    if (op.op == Op::MovToScalar) return maskOf(op.d, 1);
    if (op.op == Op::MovFromScalar) return maskOf(op.m, 1);
    if (op.op == Op::DupCore) return maskOf(op.d, 2);
    return maskOf(op.n, 2) | maskOf(op.m, 2) | maskOf(op.d, 2);
}

std::uint32_t run(const t2::NeonDp& op, Regs& regs) {
    Machine m(op, regs);
    bool writes_m = false;
    const unsigned bytes = m.run(writes_m);
    if (bytes != 0) m.put(op.d, m.O, bytes);
    if (writes_m) m.put(op.m, m.O2, m.nb);
    return (m.qc ? 1u << 27 : 0u) | fpscrFlags(m.flags);
}

}  // namespace cb::emulate::pcode_neon_arm32
