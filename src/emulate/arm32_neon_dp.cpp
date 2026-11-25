// cb-unity: alone
// emulate/arm32_neon_dp.cpp -- the Advanced SIMD data-processing instructions and the transfers between the core registers and the scalars (decompile/arm32_neon_dp.hpp), run lane
// by lane as the ARM ARM's pseudocode says (A8.8 each instruction; A2.7 the floating point of Advanced SIMD: every operation sees the "standard FPSCR value" -- round to nearest,
// flush to zero, default NaN -- and sets the cumulative saturation flag QC and no other). The arithmetic is exact: a 128 bit integer type for the integer lanes, and for the
// float ones the host's double (which makes the rounding of a single exact for + - * / ) and fmaf. This is the own core's code; the one emulator's is engine/pcode_neon_arm32.cpp.
#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <cstring>

#include "decompile/arm32_neon_dp.hpp"
#include "decompile/arm_ext.hpp"
#include "emulate/arm.hpp"

namespace cb::emulate::arm {

namespace da = decompile::arm;
namespace t2 = da::t2;
using Op = t2::NeonOp;

namespace {

using u64 = std::uint64_t;
using i64 = std::int64_t;
using u32 = std::uint32_t;
using u8 = std::uint8_t;

// ---- exact integers: two's complement, 128 bits
struct W {
    i64 hi;
    u64 lo;
};
W ws(i64 v) { return {v < 0 ? -1 : 0, static_cast<u64>(v)}; }
W wu(u64 v) { return {0, v}; }
W operator+(W a, W b) {
    const u64 lo = a.lo + b.lo;
    return {static_cast<i64>(static_cast<u64>(a.hi) + static_cast<u64>(b.hi) + (lo < a.lo ? 1u : 0u)), lo};
}
W operator-(W a) {
    const u64 lo = ~a.lo + 1;
    return {static_cast<i64>(~static_cast<u64>(a.hi) + (lo == 0 ? 1u : 0u)), lo};
}
W operator-(W a, W b) { return a + (-b); }
bool operator<(W a, W b) { return a.hi != b.hi ? a.hi < b.hi : a.lo < b.lo; }
bool operator>(W a, W b) { return b < a; }
bool operator<=(W a, W b) { return !(b < a); }
bool operator>=(W a, W b) { return !(a < b); }
bool operator==(W a, W b) { return a.hi == b.hi && a.lo == b.lo; }
W shl(W a, unsigned n) {
    if (n == 0) return a;
    if (n >= 128) return {0, 0};
    if (n >= 64) return {static_cast<i64>(a.lo << (n - 64)), 0};
    return {static_cast<i64>((static_cast<u64>(a.hi) << n) | (a.lo >> (64 - n))), a.lo << n};
}
W sar(W a, unsigned n) {
    if (n == 0) return a;
    if (n >= 128) return a.hi < 0 ? W{-1, ~0ull} : W{0, 0};
    if (n >= 64) return {a.hi < 0 ? -1 : 0, static_cast<u64>(a.hi >> (n - 64))};
    return {a.hi >> n, (a.lo >> n) | (static_cast<u64>(a.hi) << (64 - n))};
}
W mulU(u64 a, u64 b) {
    const u64 a0 = a & 0xFFFFFFFFu, a1 = a >> 32, b0 = b & 0xFFFFFFFFu, b1 = b >> 32;
    const u64 p00 = a0 * b0, p01 = a0 * b1, p10 = a1 * b0, p11 = a1 * b1;
    const u64 mid = (p00 >> 32) + (p01 & 0xFFFFFFFFu) + (p10 & 0xFFFFFFFFu);
    return {static_cast<i64>(p11 + (p01 >> 32) + (p10 >> 32) + (mid >> 32)), (p00 & 0xFFFFFFFFu) | (mid << 32)};
}
W mulS(i64 a, i64 b) {
    const u64 ma = a < 0 ? 0 - static_cast<u64>(a) : static_cast<u64>(a), mb = b < 0 ? 0 - static_cast<u64>(b) : static_cast<u64>(b);
    const W p = mulU(ma, mb);
    return (a < 0) != (b < 0) ? -p : p;
}
W mulW(W a, W b) {   // the product of two numbers that each fit in 64 bits signed
    return mulS(static_cast<i64>(a.lo), static_cast<i64>(b.lo));
}
W lowBound(unsigned bits, bool sg) { return sg ? -shl(ws(1), bits - 1) : ws(0); }
W highBound(unsigned bits, bool sg) { return sg ? shl(ws(1), bits - 1) - ws(1) : shl(ws(1), bits) - ws(1); }
// The value saturated to `bits` bits; `sat` says it was out of range.
W clampTo(W v, unsigned bits, bool sg, bool& sat) {
    const W lo = lowBound(bits, sg), hi = highBound(bits, sg);
    if (v < lo) { sat = true; return lo; }
    if (v > hi) { sat = true; return hi; }
    return v;
}
u64 lowBits(W v, unsigned bits) { return bits >= 64 ? v.lo : v.lo & ((1ull << bits) - 1); }
W readLane(u64 raw, unsigned ebytes, bool uns) {
    const unsigned bits = ebytes * 8;
    if (uns || bits == 64) return wu(raw);   // (a 64 bit lane read as signed is read below)
    return ws(static_cast<i64>(raw << (64 - bits)) >> (64 - bits));
}
W readLaneS(u64 raw, unsigned ebytes, bool uns) {
    if (!uns && ebytes == 8) return ws(static_cast<i64>(raw));
    return readLane(raw, ebytes, uns);
}

struct V {
    u8 b[16] = {};
};
u64 lane(const V& v, unsigned eb, unsigned i) {
    u64 r = 0;
    std::memcpy(&r, v.b + i * eb, eb);
    return r;
}
void setLane(V& v, unsigned eb, unsigned i, u64 x) { std::memcpy(v.b + i * eb, &x, eb); }

// ---- the floating point of Advanced SIMD (the standard FPSCR value): round to nearest, flush to zero, default NaN; the cumulative exception flags are raised as the manual's
// FPProcessException does. A single + - * of floats is exact in a double, so the double's result rounded to a float is the single's (a sum may be inexact in the double: the
// error is found with TwoSum), and a fused multiply-add is checked the same way and falls back to fmaf.
constexpr u32 kDefaultNan = 0x7FC00000u;
constexpr u32 kIOC = 1, kDZC = 2, kOFC = 4, kUFC = 8, kIXC = 16, kIDC = 128;
struct Fx {
    u32 f = 0;
};
bool isNan(u32 b) { return (b & 0x7FFFFFFFu) > 0x7F800000u; }
bool isSnan(u32 b) { return isNan(b) && !(b & 0x00400000u); }
bool isInf(u32 b) { return (b & 0x7FFFFFFFu) == 0x7F800000u; }
bool isZero(u32 b) { return (b & 0x7FFFFFFFu) == 0; }
float asF(u32 b) { return std::bit_cast<float>(b); }
u32 asB(float f) { return std::bit_cast<u32>(f); }
double dbl(u32 b) { return static_cast<double>(asF(b)); }
u32 flushIn(u32 b, Fx& fx) {   // a denormal input is read as a zero of its sign, and raises the input denormal flag
    if ((b & 0x7F800000u) == 0 && (b & 0x007FFFFFu)) {
        fx.f |= kIDC;
        return b & 0x80000000u;
    }
    return b;
}
// Any NaN among the operands: the signaling ones raise invalid operation; the result is then the default NaN.
bool anyNan(Fx& fx, std::initializer_list<u32> ops) {
    bool nan = false, signaling = false;
    for (const u32 o : ops) {
        nan = nan || isNan(o);
        signaling = signaling || isSnan(o);
    }
    if (signaling) fx.f |= kIOC;
    return nan;
}
const double kMinNormal = std::ldexp(1.0, -126);
// The result `e` (a double: exact, or the once rounded sum) as a single; `inexact`: the double was already rounded.
u32 finish(double e, Fx& fx, bool inexact, double err = 0) {
    if (std::isnan(e)) {
        fx.f |= kIOC;
        return kDefaultNan;
    }
    if (e != 0 && (std::fabs(e) < kMinNormal || (std::fabs(e) == kMinNormal && err != 0 && (err < 0) != (e < 0)))) {   // (tiny before rounding: the exact result is just below the smallest normal)
        fx.f |= kUFC;
        return std::signbit(e) ? 0x80000000u : 0u;
    }
    const float r = static_cast<float>(e);
    if (std::isinf(r) && !std::isinf(e)) fx.f |= kOFC | kIXC;
    else if (static_cast<double>(r) != e || inexact) fx.f |= kIXC;
    return asB(r);
}
// A result already correctly rounded to a float `r` of the exact value near `approx`, known to be inexact.
u32 finishFused(float r, double approx, Fx& fx, double err = 0) {
    if (std::fabs(approx) < kMinNormal || (std::fabs(approx) == kMinNormal && (err < 0) != (approx < 0))) {
        fx.f |= kUFC;
        return std::signbit(approx) ? 0x80000000u : 0u;
    }
    fx.f |= std::isinf(r) ? (kOFC | kIXC) : kIXC;
    return asB(r);
}
double twoSumError(double a, double b, double s) {
    if (std::isinf(s) || std::isnan(s)) return 0;
    const double bb = s - a;
    return (a - (s - bb)) + (b - bb);
}
u32 fpAddSub(u32 a, u32 b, bool sub, Fx& fx) {
    a = flushIn(a, fx);
    b = flushIn(b, fx);
    if (anyNan(fx, {a, b})) return kDefaultNan;
    const double x = dbl(a), y = sub ? -dbl(b) : dbl(b), s = x + y;
    const double err = twoSumError(x, y, s);
    return finish(s, fx, err != 0, err);
}
u32 fpMul(u32 a, u32 b, Fx& fx) {
    a = flushIn(a, fx);
    b = flushIn(b, fx);
    if (anyNan(fx, {a, b})) return kDefaultNan;
    return finish(dbl(a) * dbl(b), fx, false);
}
u32 fpFma(u32 a, u32 b, u32 c, Fx& fx) {   // a * b + c with one rounding
    a = flushIn(a, fx);
    b = flushIn(b, fx);
    c = flushIn(c, fx);
    const bool invalid_product = (isInf(a) && isZero(b)) || (isZero(a) && isInf(b));
    if (anyNan(fx, {a, b, c})) {
        if (isNan(c) && invalid_product) fx.f |= kIOC;
        return kDefaultNan;
    }
    if (invalid_product) {
        fx.f |= kIOC;
        return kDefaultNan;
    }
    const double p = dbl(a) * dbl(b), z = dbl(c), s = p + z;
    if (std::isnan(s)) {
        fx.f |= kIOC;
        return kDefaultNan;
    }
    const double t = twoSumError(p, z, s);
    if (t == 0) return finish(s, fx, false);
    return finishFused(std::fma(asF(a), asF(b), asF(c)), s, fx, t);
}
u32 fpMax(u32 a, u32 b, bool is_max, Fx& fx) {
    a = flushIn(a, fx);
    b = flushIn(b, fx);
    if (anyNan(fx, {a, b})) return kDefaultNan;
    if (asF(a) == asF(b)) {   // (of two zeros: the positive for a maximum, the negative for a minimum)
        if (is_max) return (a & 0x80000000u) ? b : a;
        return (a & 0x80000000u) ? a : b;
    }
    return (asF(a) > asF(b)) == is_max ? a : b;
}
u32 fpCompare(u32 a, u32 b, int kind, bool absolute, Fx& fx) {   // kind: 0 equal, 1 greater or equal, 2 greater
    a = flushIn(a, fx);
    b = flushIn(b, fx);
    if (isNan(a) || isNan(b)) {
        if (kind != 0 || isSnan(a) || isSnan(b)) fx.f |= kIOC;
        return 0;
    }
    double p = dbl(a), q = dbl(b);
    if (absolute) {
        p = std::fabs(p);
        q = std::fabs(q);
    }
    return (kind == 0 ? p == q : kind == 1 ? p >= q : p > q) ? 0xFFFFFFFFu : 0;
}
// FRECPS and FRSQRTS: 2 - a b and (3 - a b) / 2 with one rounding; an infinity times a zero gives 2.0 and 1.5.
u32 fpStep(u32 a, u32 b, bool rsqrt, Fx& fx) {
    a = flushIn(a, fx);
    b = flushIn(b, fx);
    if (anyNan(fx, {a, b})) return kDefaultNan;
    if ((isInf(a) && isZero(b)) || (isZero(a) && isInf(b))) return rsqrt ? 0x3FC00000u : 0x40000000u;
    if (isInf(a) || isInf(b)) return ((a ^ b) >> 31) ? 0x7F800000u : 0xFF800000u;
    // (the multiply is rounded, then the subtract: ARMv7's VRECPS and VRSQRTS are not fused, as A64's are)
    const u32 sum = fpAddSub(rsqrt ? 0x40400000u : 0x40000000u, fpMul(a, b, fx), true, fx);
    return rsqrt ? finish(dbl(sum) * 0.5, fx, false) : sum;
}
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
u32 fpRecipEstimate(u32 op, Fx& fx) {
    op = flushIn(op, fx);
    if (isNan(op)) {
        if (isSnan(op)) fx.f |= kIOC;
        return kDefaultNan;
    }
    const u32 sign = op & 0x80000000u, exp = op >> 23 & 0xFF, frac = op & 0x7FFFFFu;
    if (exp == 0xFF) return sign;
    if (exp == 0) {
        fx.f |= kDZC;
        return sign | 0x7F800000u;
    }
    if (exp >= 253) {   // too large to have a normal reciprocal: flushed
        fx.f |= kUFC;
        return sign;
    }
    const u32 est = recipEstimate9(0x100 | frac >> 15);
    return sign | (253 - exp) << 23 | (est & 0xFF) << 15;
}
u32 fpRsqrtEstimate(u32 op, Fx& fx) {
    op = flushIn(op, fx);
    if (isNan(op)) {
        if (isSnan(op)) fx.f |= kIOC;
        return kDefaultNan;
    }
    const u32 sign = op & 0x80000000u, exp = op >> 23 & 0xFF, frac = op & 0x7FFFFFu;
    if (exp == 0) {
        fx.f |= kDZC;
        return sign | 0x7F800000u;
    }
    if (sign) {
        fx.f |= kIOC;
        return kDefaultNan;
    }
    if (exp == 0xFF) return 0;
    const u32 scaled = (exp & 1) == 0 ? 0x100 | frac >> 15 : 0x80 | frac >> 16;
    const u32 est = rsqrtEstimate9(scaled);
    return ((380 - exp) / 2) << 23 | (est & 0xFF) << 15;
}
u32 halfToSingle(u32 h, Fx& fx) {
    const u32 sign = (h >> 15 & 1) << 31, exp = h >> 10 & 31, frac = h & 0x3FF;
    if (exp == 31) {
        if (frac == 0) return sign | 0x7F800000u;
        if (!(frac & 0x200)) fx.f |= kIOC;
        return kDefaultNan;
    }
    if (exp == 0) {
        if (frac == 0) return sign;
        return sign | asB(static_cast<float>(std::ldexp(static_cast<double>(frac), -24)));
    }
    return sign | (exp - 15 + 127) << 23 | frac << 13;
}
u32 singleToHalf(u32 b, Fx& fx) {
    b = flushIn(b, fx);
    if (isNan(b)) {
        if (isSnan(b)) fx.f |= kIOC;
        return 0x7E00;
    }
    const u32 sign = (b >> 31) << 15;
    const double v = std::fabs(dbl(b));
    if (std::isinf(v)) return sign | 0x7C00;
    if (v >= 65520.0) {
        fx.f |= kOFC | kIXC;
        return sign | 0x7C00;
    }
    if (v < std::ldexp(1.0, -14)) {   // a denormal half (or zero)
        const double q = std::ldexp(v, 24), n = std::nearbyint(q);
        if (n != q) fx.f |= kUFC | kIXC;
        return sign | static_cast<u32>(n);
    }
    int e = 0;
    const double m = std::frexp(v, &e);   // v = m * 2^e, m in [0.5, 1)
    const double scaled = (m * 2 - 1) * 1024, rounded = std::nearbyint(scaled);
    if (rounded != scaled) fx.f |= kIXC;
    u32 frac = static_cast<u32>(rounded);
    int ex = e - 1 + 15;
    if (frac == 1024) {
        frac = 0;
        ++ex;
    }
    return sign | static_cast<u32>(ex) << 10 | frac;
}
// A float as a 32 bit integer, toward zero, saturating (the standard FPSCR value).
u32 floatToInt(u32 b, bool is_signed, unsigned fbits, Fx& fx) {
    b = flushIn(b, fx);
    if (isNan(b)) {
        fx.f |= kIOC;
        return 0;
    }
    const double v = std::ldexp(dbl(b), static_cast<int>(fbits)), t = std::trunc(v);
    if (is_signed) {
        if (t > 2147483647.0) {
            fx.f |= kIOC;
            return 0x7FFFFFFFu;
        }
        if (t < -2147483648.0) {
            fx.f |= kIOC;
            return 0x80000000u;
        }
        if (t != v) fx.f |= kIXC;
        return static_cast<u32>(static_cast<std::int32_t>(t));
    }
    if (t > 4294967295.0) {
        fx.f |= kIOC;
        return 0xFFFFFFFFu;
    }
    if (t < 0) {
        fx.f |= kIOC;
        return 0;
    }
    if (t != v) fx.f |= kIXC;
    return static_cast<u32>(t);
}
u32 intToFloat(u32 x, bool is_signed, unsigned fbits, Fx& fx) {
    const double v = is_signed ? static_cast<double>(static_cast<std::int32_t>(x)) : static_cast<double>(x);
    return finish(std::ldexp(v, -static_cast<int>(fbits)), fx, false);
}

// ---- the VFP's fused multiply-add, with the status register's default control bits (no flush, the NaN of the operand): a NaN operand is the result (a signaling one first,
// the operands in the order of the product's two and the addend), quieted; an invalid operation gives the default NaN.
template <typename B, typename F>
B vfpFma(B a, B b, B c) {
    constexpr bool single = sizeof(B) == 4;
    const B sign_bit = static_cast<B>(1) << (8 * sizeof(B) - 1);
    const B quiet = static_cast<B>(1) << (single ? 22 : 51);
    const B exp_bits = static_cast<B>(single ? 8 : 11);
    const B exp_mask = static_cast<B>(((static_cast<B>(1) << exp_bits) - 1) << (single ? 23 : 52));
    const B default_nan = exp_mask | quiet;
    const auto nan = [&](B v) { return (v & ~sign_bit) > exp_mask; };
    const auto inf = [&](B v) { return (v & ~sign_bit) == exp_mask; };
    const auto zero = [&](B v) { return (v & ~sign_bit) == 0; };
    const B ops[3] = {c, a, b};   // (Unicorn takes the addend first)
    for (const B v : ops)
        if (nan(v) && !(v & quiet)) return v | quiet;
    const bool invalid_product = (inf(a) && zero(b)) || (zero(a) && inf(b));
    for (unsigned i = 0; i < 3; ++i)
        if (nan(ops[i])) return invalid_product && i == 0 ? default_nan : ops[i];
    const F r = std::fma(std::bit_cast<F>(a), std::bit_cast<F>(b), std::bit_cast<F>(c));
    return std::isnan(r) ? default_nan : std::bit_cast<B>(r);
}
u32 halfFromSingleExact(u32 b) {   // round to nearest, denormals kept, a NaN's payload truncated and quieted
    const u32 sign = (b >> 31) << 15, exp = b >> 23 & 0xFF, frac = b & 0x7FFFFFu;
    if (exp == 0xFF) return frac ? sign | 0x7E00 | frac >> 13 : sign | 0x7C00;
    const double v = std::fabs(static_cast<double>(asF(b)));
    if (v >= 65520.0) return sign | 0x7C00;
    if (v < std::ldexp(1.0, -14)) return sign | static_cast<u32>(std::nearbyint(std::ldexp(v, 24)));
    int e = 0;
    const double m = std::frexp(v, &e);
    u32 f = static_cast<u32>(std::nearbyint((m * 2 - 1) * 1024));
    int ex = e - 1 + 15;
    if (f == 1024) {
        f = 0;
        ++ex;
    }
    return sign | static_cast<u32>(ex) << 10 | f;
}
u32 singleFromHalfExact(u32 h) {
    const u32 sign = (h >> 15 & 1) << 31, exp = h >> 10 & 31, frac = h & 0x3FF;
    if (exp == 31) return frac ? sign | 0x7FC00000u | frac << 13 : sign | 0x7F800000u;
    if (exp == 0) return frac == 0 ? sign : sign | asB(static_cast<float>(std::ldexp(static_cast<double>(frac), -24)));
    return sign | (exp - 15 + 127) << 23 | frac << 13;
}

}  // namespace

std::optional<Fault> Cpu::execute_neon_dp(const t2::NeonDp& x, std::uint32_t at) {
    if (x.op == Op::VFma || x.op == Op::VCvtFx || x.op == Op::VCvtHalf || x.op == Op::VMovPair) {   // the VFP's own
        const bool dbl = x.size == 3;
        const auto get32 = [&](unsigned k) { return vfp_[k & 63]; };
        const auto get64 = [&](unsigned d) { return static_cast<u64>(vfp_[2 * (d & 31)]) | static_cast<u64>(vfp_[2 * (d & 31) + 1]) << 32; };
        const auto put64 = [&](unsigned d, u64 v) {
            vfp_[2 * (d & 31)] = static_cast<u32>(v);
            vfp_[2 * (d & 31) + 1] = static_cast<u32>(v >> 32);
        };
        switch (x.op) {
            case Op::VFma: {
                const bool neg_product = x.imm == 1 || x.imm == 2, neg_addend = x.imm == 2 || x.imm == 3;
                if (dbl) {
                    const u64 sign = 1ull << 63;
                    put64(x.d, vfpFma<u64, double>(get64(x.n) ^ (neg_product ? sign : 0), get64(x.m), get64(x.d) ^ (neg_addend ? sign : 0)));
                } else {
                    vfp_[x.d & 63] = vfpFma<u32, float>(get32(x.n) ^ (neg_product ? 0x80000000u : 0), get32(x.m), get32(x.d) ^ (neg_addend ? 0x80000000u : 0));
                }
                return std::nullopt;
            }
            case Op::VCvtFx: {
                const unsigned width = x.idx;
                if (x.acc) {   // to fixed point
                    const double v = dbl ? std::bit_cast<double>(get64(x.d)) : static_cast<double>(asF(get32(x.d)));
                    u64 result = 0;
                    if (!std::isnan(v)) {
                        const double t = std::trunc(std::ldexp(v, static_cast<int>(x.imm)));
                        const double lo = x.u ? 0.0 : -std::ldexp(1.0, static_cast<int>(width) - 1), hi = x.u ? std::ldexp(1.0, static_cast<int>(width)) - 1 : std::ldexp(1.0, static_cast<int>(width) - 1) - 1;
                        const double c = t < lo ? lo : t > hi ? hi : t;
                        result = x.u ? static_cast<u64>(c) : static_cast<u64>(static_cast<i64>(c));
                    }
                    result &= width == 32 ? 0xFFFFFFFFull : 0xFFFFull;
                    if (!x.u && width == 16) result = static_cast<u64>(static_cast<i64>(static_cast<std::int16_t>(result)));   // (a signed result is sign extended)
                    if (!x.u && width == 32) result = static_cast<u64>(static_cast<i64>(static_cast<std::int32_t>(result)));
                    if (dbl) put64(x.d, result);
                    else vfp_[x.d & 63] = static_cast<u32>(result);
                } else {
                    const u32 raw = dbl ? static_cast<u32>(get64(x.d)) : get32(x.d);
                    const double v = x.u ? static_cast<double>(width == 32 ? raw : raw & 0xFFFFu)
                                         : static_cast<double>(width == 32 ? static_cast<std::int32_t>(raw) : static_cast<std::int16_t>(raw));
                    const double scaled = std::ldexp(v, -static_cast<int>(x.imm));
                    if (dbl) put64(x.d, std::bit_cast<u64>(scaled));
                    else vfp_[x.d & 63] = asB(static_cast<float>(scaled));
                }
                return std::nullopt;
            }
            case Op::VCvtHalf: {
                if (x.acc) {   // single to half
                    const u32 h = halfFromSingleExact(get32(x.m));
                    vfp_[x.d & 63] = x.imm ? (vfp_[x.d & 63] & 0xFFFFu) | h << 16 : (vfp_[x.d & 63] & 0xFFFF0000u) | h;
                } else {
                    vfp_[x.d & 63] = singleFromHalfExact(x.imm ? get32(x.m) >> 16 : get32(x.m) & 0xFFFFu);
                }
                return std::nullopt;
            }
            default:   // VMovPair
                if (x.u) {
                    r[x.rt] = get32(x.m);
                    r[x.idx] = get32(x.m + 1);
                } else {
                    vfp_[x.m & 63] = r[x.rt];
                    vfp_[(x.m + 1) & 63] = r[x.idx];
                }
                return std::nullopt;
        }
    }
    const unsigned eb = 1u << x.size, nb = x.q ? 16 : 8;
    const bool u = x.u;
    const auto rd = [&](unsigned d, unsigned bytes) {
        V v;
        for (unsigned k = 0; k < bytes / 4; ++k) {
            const u32 w = vfp_[(2 * d + k) & 63];
            std::memcpy(v.b + 4 * k, &w, 4);
        }
        return v;
    };
    const auto wr = [&](unsigned d, const V& v, unsigned bytes) {
        for (unsigned k = 0; k < bytes / 4; ++k) {
            u32 w;
            std::memcpy(&w, v.b + 4 * k, 4);
            vfp_[(2 * d + k) & 63] = w;
        }
    };
    bool qc = false;
    const auto lanesOf = [&](unsigned bytes, unsigned ebytes) { return bytes / ebytes; };
    // the second operand, the scalar splatted when the instruction has one
    const auto second = [&](unsigned bytes) {
        if (!x.scalar) return rd(x.m, bytes);
        const V s = rd(x.m, 8);
        V v;
        for (unsigned i = 0; i < bytes / eb; ++i) setLane(v, eb, i, lane(s, eb, x.idx));
        return v;
    };
    const unsigned bits = eb * 8;
    // each lane of a (and of b) as numbers, the result lane stored by `store`
    const auto each2 = [&](unsigned bytes, auto f) {   // f(W a, W b, W d) -> W
        const V a = rd(x.n, bytes), b = second(bytes), d = rd(x.d, bytes);
        V out;
        for (unsigned i = 0; i < bytes / eb; ++i)
            setLane(out, eb, i, lowBits(f(readLaneS(lane(a, eb, i), eb, u), readLaneS(lane(b, eb, i), eb, u), readLaneS(lane(d, eb, i), eb, u)), bits));
        wr(x.d, out, bytes);
    };
    const auto each1 = [&](unsigned bytes, auto f) {   // f(W m, W d) -> W
        const V m = rd(x.m, bytes), d = rd(x.d, bytes);
        V out;
        for (unsigned i = 0; i < bytes / eb; ++i) setLane(out, eb, i, lowBits(f(readLaneS(lane(m, eb, i), eb, u), readLaneS(lane(d, eb, i), eb, u)), bits));
        wr(x.d, out, bytes);
    };
    const auto ones = [&](bool c) { return c ? ws(-1) : ws(0); };
    const auto sat = [&](W v, unsigned nbits, bool sg) { return clampTo(v, nbits, sg, qc); };
    const auto finishQc = [&]() {
        if (qc) fpscr_ |= 1u << 27;
        return std::nullopt;
    };
    const auto exactRight = [&](W v, unsigned n, bool round) {   // the value shifted right by n with the rounding of the manual
        if (n == 0) return v;
        n = std::min(n, bits + 2);
        return sar(round ? v + shl(ws(1), n - 1) : v, n);
    };
    // floating point lane by lane; the flags the lanes raise are ORed into the status register at the end
    Fx fx;
    const auto fl = [&](auto f) {   // f(u32 a, u32 b, u32 d) -> u32
        const V a = rd(x.n, nb), b = second(nb), d = rd(x.d, nb);
        V out;
        for (unsigned i = 0; i < nb / 4; ++i) setLane(out, 4, i, f(static_cast<u32>(lane(a, 4, i)), static_cast<u32>(lane(b, 4, i)), static_cast<u32>(lane(d, 4, i))));
        wr(x.d, out, nb);
        fpscr_ |= fx.f;
    };
    const auto fl1 = [&](auto f) {
        const V m = rd(x.m, nb);
        V out;
        for (unsigned i = 0; i < nb / 4; ++i) setLane(out, 4, i, f(static_cast<u32>(lane(m, 4, i))));
        wr(x.d, out, nb);
        fpscr_ |= fx.f;
    };
    const auto pairwise = [&](auto f) {   // D registers only: the pairs of n, then the pairs of m
        const V a = rd(x.n, 8), b = rd(x.m, 8);
        V out;
        const unsigned n = 8 / eb;
        for (unsigned i = 0; i < n / 2; ++i) {
            setLane(out, eb, i, lowBits(f(readLaneS(lane(a, eb, 2 * i), eb, u), readLaneS(lane(a, eb, 2 * i + 1), eb, u)), bits));
            setLane(out, eb, n / 2 + i, lowBits(f(readLaneS(lane(b, eb, 2 * i), eb, u), readLaneS(lane(b, eb, 2 * i + 1), eb, u)), bits));
        }
        wr(x.d, out, 8);
    };
    const auto pairwiseF = [&](auto f) {
        const V a = rd(x.n, 8), b = rd(x.m, 8);
        V out;
        setLane(out, 4, 0, f(static_cast<u32>(lane(a, 4, 0)), static_cast<u32>(lane(a, 4, 1))));
        setLane(out, 4, 1, f(static_cast<u32>(lane(b, 4, 0)), static_cast<u32>(lane(b, 4, 1))));
        wr(x.d, out, 8);
        fpscr_ |= fx.f;
    };

    switch (x.op) {
        case Op::Hadd: each2(nb, [&](W a, W b, W) { return sar(a + b, 1); }); return std::nullopt;
        case Op::Rhadd: each2(nb, [&](W a, W b, W) { return sar(a + b + ws(1), 1); }); return std::nullopt;
        case Op::Hsub: each2(nb, [&](W a, W b, W) { return sar(a - b, 1); }); return std::nullopt;
        case Op::Qadd: each2(nb, [&](W a, W b, W) { return sat(a + b, bits, !u); }); return finishQc();
        case Op::Qsub: each2(nb, [&](W a, W b, W) { return sat(a - b, bits, !u); }); return finishQc();
        case Op::Cgt: each2(nb, [&](W a, W b, W) { return ones(a > b); }); return std::nullopt;
        case Op::Cge: each2(nb, [&](W a, W b, W) { return ones(a >= b); }); return std::nullopt;
        case Op::Tst: each2(nb, [&](W a, W b, W) { return ones((lowBits(a, bits) & lowBits(b, bits)) != 0); }); return std::nullopt;
        case Op::Ceq: each2(nb, [&](W a, W b, W) { return ones(a == b); }); return std::nullopt;
        case Op::Max: each2(nb, [&](W a, W b, W) { return a > b ? a : b; }); return std::nullopt;
        case Op::Min: each2(nb, [&](W a, W b, W) { return a < b ? a : b; }); return std::nullopt;
        case Op::Abd: each2(nb, [&](W a, W b, W) { return a > b ? a - b : b - a; }); return std::nullopt;
        case Op::Aba: each2(nb, [&](W a, W b, W d) { return d + (a > b ? a - b : b - a); }); return std::nullopt;
        case Op::Add: each2(nb, [&](W a, W b, W) { return a + b; }); return std::nullopt;
        case Op::Sub: each2(nb, [&](W a, W b, W) { return a - b; }); return std::nullopt;
        case Op::Mla: each2(nb, [&](W a, W b, W d) { return d + mulU(a.lo, b.lo); }); return std::nullopt;
        case Op::Mls: each2(nb, [&](W a, W b, W d) { return d - mulU(a.lo, b.lo); }); return std::nullopt;
        case Op::Mul: each2(nb, [&](W a, W b, W) { return mulU(a.lo, b.lo); }); return std::nullopt;
        case Op::MulP:
            each2(nb, [&](W a, W b, W) {
                u64 prod = 0;
                for (unsigned k = 0; k < 8; ++k)
                    if ((b.lo & 0xFF) >> k & 1) prod ^= (a.lo & 0xFF) << k;
                return wu(prod);
            });
            return std::nullopt;
        case Op::Pmax: pairwise([&](W a, W b) { return a > b ? a : b; }); return std::nullopt;
        case Op::Pmin: pairwise([&](W a, W b) { return a < b ? a : b; }); return std::nullopt;
        case Op::Padd: pairwise([&](W a, W b) { return a + b; }); return std::nullopt;
        case Op::Qdmulh:
            each2(nb, [&](W a, W b, W) {
                const W p = shl(mulS(static_cast<i64>(a.lo), static_cast<i64>(b.lo)), 1) + (x.round ? shl(ws(1), bits - 1) : ws(0));
                return sat(sar(p, bits), bits, true);
            });
            return finishQc();
        case Op::ShiftReg:
            each2(nb, [&](W b, W a, W) {   // (the value is Vm, the counts are Vn)
                const int count = static_cast<std::int8_t>(b.lo & 0xFF);
                if (count >= 0) {
                    const unsigned c = static_cast<unsigned>(count);
                    if (!x.sat) return shl(a, std::min(c, 127u));
                    if (a == ws(0)) return a;
                    if (c >= bits) {   // any other value is out of range
                        qc = true;
                        return a < ws(0) ? lowBound(bits, !u) : highBound(bits, !u);
                    }
                    return sat(shl(a, c), bits, !u);
                }
                const unsigned c = static_cast<unsigned>(-count);
                return exactRight(a, c, x.round);
            });
            return finishQc();
        case Op::And: case Op::Bic: case Op::Orr: case Op::Orn: case Op::Eor: case Op::Bsl: case Op::Bit: case Op::Bif: {
            const V a = rd(x.n, nb), b = rd(x.m, nb), d = rd(x.d, nb);
            V out;
            for (unsigned k = 0; k < nb; ++k) {
                const u8 p = a.b[k], q = b.b[k], dv = d.b[k];
                switch (x.op) {
                    case Op::And: out.b[k] = p & q; break;
                    case Op::Bic: out.b[k] = p & ~q; break;
                    case Op::Orr: out.b[k] = p | q; break;
                    case Op::Orn: out.b[k] = p | static_cast<u8>(~q); break;
                    case Op::Eor: out.b[k] = p ^ q; break;
                    case Op::Bsl: out.b[k] = (dv & p) | (static_cast<u8>(~dv) & q); break;
                    case Op::Bit: out.b[k] = (dv & static_cast<u8>(~q)) | (p & q); break;
                    default: out.b[k] = (dv & q) | (p & static_cast<u8>(~q)); break;
                }
            }
            wr(x.d, out, nb);
            return std::nullopt;
        }
        // ---- floating point, three registers
        case Op::FAdd: fl([&](u32 a, u32 b, u32) { return fpAddSub(a, b, false, fx); }); return std::nullopt;
        case Op::FSub: fl([&](u32 a, u32 b, u32) { return fpAddSub(a, b, true, fx); }); return std::nullopt;
        case Op::FMul: fl([&](u32 a, u32 b, u32) { return fpMul(a, b, fx); }); return std::nullopt;
        case Op::FAbd: fl([&](u32 a, u32 b, u32) { return fpAddSub(a, b, true, fx) & 0x7FFFFFFFu; }); return std::nullopt;
        case Op::FMla: fl([&](u32 a, u32 b, u32 d) { return fpAddSub(d, fpMul(a, b, fx), false, fx); }); return std::nullopt;
        case Op::FMls: fl([&](u32 a, u32 b, u32 d) { return fpAddSub(d, fpMul(a, b, fx), true, fx); }); return std::nullopt;
        case Op::Fma: fl([&](u32 a, u32 b, u32 d) { return fpFma(a, b, d, fx); }); return std::nullopt;
        case Op::Fms: fl([&](u32 a, u32 b, u32 d) { return fpFma(a ^ 0x80000000u, b, d, fx); }); return std::nullopt;
        case Op::FCeq: fl([&](u32 a, u32 b, u32) { return fpCompare(a, b, 0, false, fx); }); return std::nullopt;
        case Op::FCge: fl([&](u32 a, u32 b, u32) { return fpCompare(a, b, 1, false, fx); }); return std::nullopt;
        case Op::FCgt: fl([&](u32 a, u32 b, u32) { return fpCompare(a, b, 2, false, fx); }); return std::nullopt;
        case Op::FAcge: fl([&](u32 a, u32 b, u32) { return fpCompare(a, b, 1, true, fx); }); return std::nullopt;
        case Op::FAcgt: fl([&](u32 a, u32 b, u32) { return fpCompare(a, b, 2, true, fx); }); return std::nullopt;
        case Op::FMax: fl([&](u32 a, u32 b, u32) { return fpMax(a, b, true, fx); }); return std::nullopt;
        case Op::FMin: fl([&](u32 a, u32 b, u32) { return fpMax(a, b, false, fx); }); return std::nullopt;
        case Op::FPadd: pairwiseF([&](u32 a, u32 b) { return fpAddSub(a, b, false, fx); }); return std::nullopt;
        case Op::FPmax: pairwiseF([&](u32 a, u32 b) { return fpMax(a, b, true, fx); }); return std::nullopt;
        case Op::FPmin: pairwiseF([&](u32 a, u32 b) { return fpMax(a, b, false, fx); }); return std::nullopt;
        case Op::Recps: fl([&](u32 a, u32 b, u32) { return fpStep(a, b, false, fx); }); return std::nullopt;
        case Op::Rsqrts: fl([&](u32 a, u32 b, u32) { return fpStep(a, b, true, fx); }); return std::nullopt;
        default: break;
    }

    // ---- long and wide forms (D sources, Q destination); size is the source's element
    const auto longs = [&](auto f, bool wide_n = false, bool uses_d = false) {   // f(W a, W b, W d) -> W, d the destination's lane
        const unsigned le = eb * 2, lanes = 8 / eb;
        const V a = rd(x.n, wide_n ? 16 : 8), b = second(8), d = rd(x.d, 16);
        V out;
        for (unsigned i = 0; i < lanes; ++i) {
            const W av = wide_n ? readLaneS(lane(a, le, i), le, u) : readLaneS(lane(a, eb, i), eb, u);
            const W dv = uses_d ? readLaneS(lane(d, le, i), le, u) : ws(0);
            setLane(out, le, i, lowBits(f(av, readLaneS(lane(b, eb, i), eb, u), dv), le * 8));
        }
        wr(x.d, out, 16);
    };
    const unsigned lbits = bits * 2;
    switch (x.op) {
        case Op::Addl: longs([&](W a, W b, W) { return a + b; }); return std::nullopt;
        case Op::Subl: longs([&](W a, W b, W) { return a - b; }); return std::nullopt;
        case Op::Addw: longs([&](W a, W b, W) { return a + b; }, true); return std::nullopt;
        case Op::Subw: longs([&](W a, W b, W) { return a - b; }, true); return std::nullopt;
        case Op::Abal: longs([&](W a, W b, W d) { return d + (a > b ? a - b : b - a); }, false, true); return std::nullopt;
        case Op::Abdl: longs([&](W a, W b, W) { return a > b ? a - b : b - a; }); return std::nullopt;
        case Op::Mlal: longs([&](W a, W b, W d) { return d + mulW(a, b); }, false, true); return std::nullopt;
        case Op::Mlsl: longs([&](W a, W b, W d) { return d - mulW(a, b); }, false, true); return std::nullopt;
        case Op::Mull: longs([&](W a, W b, W) { return mulW(a, b); }); return std::nullopt;
        case Op::MullP:
            longs([&](W a, W b, W) {
                u64 prod = 0;
                for (unsigned k = 0; k < 8; ++k)
                    if ((b.lo & 0xFF) >> k & 1) prod ^= (a.lo & 0xFF) << k;
                return wu(prod);
            });
            return std::nullopt;
        case Op::Qdmull: longs([&](W a, W b, W) { return sat(shl(mulW(a, b), 1), lbits, true); }); return finishQc();
        case Op::Qdmlal: longs([&](W a, W b, W d) { return sat(d + sat(shl(mulW(a, b), 1), lbits, true), lbits, true); }, false, true); return finishQc();
        case Op::Qdmlsl: longs([&](W a, W b, W d) { return sat(d - sat(shl(mulW(a, b), 1), lbits, true), lbits, true); }, false, true); return finishQc();
        case Op::AddHn: case Op::SubHn: {   // Q sources, D destination: the high half of the sum, rounded when asked
            const unsigned se = eb * 2;
            const V a = rd(x.n, 16), b = rd(x.m, 16);
            V out;
            for (unsigned i = 0; i < 8 / eb; ++i) {
                const W av = wu(lane(a, se, i)), bv = wu(lane(b, se, i));
                W sum = x.op == Op::AddHn ? av + bv : av - bv;
                if (x.round) sum = sum + shl(ws(1), bits - 1);
                setLane(out, eb, i, lowBits(sar(sum, bits), bits));
            }
            wr(x.d, out, 8);
            return std::nullopt;
        }
        // ---- shifts by an immediate
        case Op::Shr: {
            const V m = rd(x.m, nb), d = rd(x.d, nb);
            V out;
            for (unsigned i = 0; i < nb / eb; ++i) {
                W v = exactRight(readLaneS(lane(m, eb, i), eb, u), x.imm, x.round);
                if (x.acc) v = v + wu(lane(d, eb, i));
                setLane(out, eb, i, lowBits(v, bits));
            }
            wr(x.d, out, nb);
            return std::nullopt;
        }
        case Op::Sri: case Op::Sli: {
            const V m = rd(x.m, nb), d = rd(x.d, nb);
            V out;
            const u64 all = bits == 64 ? ~0ull : (1ull << bits) - 1;
            for (unsigned i = 0; i < nb / eb; ++i) {
                const u64 src = lane(m, eb, i), dst = lane(d, eb, i);
                u64 mask, shifted;
                if (x.op == Op::Sri) { mask = x.imm >= bits ? 0 : all >> x.imm; shifted = x.imm >= bits ? 0 : src >> x.imm; }
                else { mask = (x.imm >= bits ? 0 : all << x.imm) & all; shifted = x.imm >= bits ? 0 : src << x.imm; }
                setLane(out, eb, i, (dst & ~mask) | (shifted & mask));
            }
            wr(x.d, out, nb);
            return std::nullopt;
        }
        case Op::ShlI: each1(nb, [&](W m, W) { return shl(m, x.imm); }); return std::nullopt;
        case Op::QshlI: each1(nb, [&](W m, W) { return sat(shl(m, x.imm), bits, !u); }); return finishQc();
        case Op::Qshlu: each1(nb, [&](W m, W) { return sat(shl(m, x.imm), bits, false); }); return finishQc();
        case Op::ShrN: case Op::Qshrn: case Op::Qshrun: {
            const unsigned se = eb * 2;
            const V m = rd(x.m, 16);
            V out;
            for (unsigned i = 0; i < 8 / eb; ++i) {
                const bool src_unsigned = x.op == Op::Qshrn ? u : (x.op == Op::ShrN);
                W v = readLaneS(lane(m, se, i), se, src_unsigned);
                if (x.round) v = v + shl(ws(1), x.imm - 1);
                v = sar(v, x.imm);
                if (x.op == Op::ShrN) setLane(out, eb, i, lowBits(v, bits));
                else setLane(out, eb, i, lowBits(sat(v, bits, x.op == Op::Qshrn ? !u : false), bits));
            }
            wr(x.d, out, 8);
            return x.op == Op::ShrN ? std::nullopt : finishQc();
        }
        case Op::Shll: {
            const unsigned le = eb * 2;
            const V m = rd(x.m, 8);
            V out;
            for (unsigned i = 0; i < 8 / eb; ++i) setLane(out, le, i, lowBits(shl(readLaneS(lane(m, eb, i), eb, u), x.imm), le * 8));
            wr(x.d, out, 16);
            return std::nullopt;
        }
        case Op::CvtFixed:
            fl1([&](u32 a) { return x.acc ? floatToInt(a, !u, x.imm, fx) : intToFloat(a, !u, x.imm, fx); });
            return std::nullopt;
        default: break;
    }

    // ---- two registers, miscellaneous
    switch (x.op) {
        case Op::Rev: {
            const V m = rd(x.m, nb);
            V out;
            const unsigned per = x.imm / eb;
            for (unsigned g = 0; g < nb / x.imm; ++g)
                for (unsigned j = 0; j < per; ++j) setLane(out, eb, g * per + j, lane(m, eb, g * per + per - 1 - j));
            wr(x.d, out, nb);
            return std::nullopt;
        }
        case Op::Paddl: case Op::Padal: {
            const unsigned le = eb * 2;
            const V m = rd(x.m, nb), d = rd(x.d, nb);
            V out;
            for (unsigned i = 0; i < nb / le; ++i) {
                W v = readLaneS(lane(m, eb, 2 * i), eb, u) + readLaneS(lane(m, eb, 2 * i + 1), eb, u);
                if (x.op == Op::Padal) v = v + wu(lane(d, le, i));
                setLane(out, le, i, lowBits(v, le * 8));
            }
            wr(x.d, out, nb);
            return std::nullopt;
        }
        case Op::Cls: case Op::Clz: {
            const V m = rd(x.m, nb);
            V out;
            for (unsigned i = 0; i < nb / eb; ++i) {
                const u64 v = lane(m, eb, i);
                unsigned n = 0;
                if (x.op == Op::Clz) {
                    while (n < bits && !((v >> (bits - 1 - n)) & 1)) ++n;
                } else {
                    const u64 sign = v >> (bits - 1) & 1;
                    while (n + 1 < bits && ((v >> (bits - 2 - n)) & 1) == sign) ++n;
                }
                setLane(out, eb, i, n);
            }
            wr(x.d, out, nb);
            return std::nullopt;
        }
        case Op::Cnt: {
            const V m = rd(x.m, nb);
            V out;
            for (unsigned k = 0; k < nb; ++k) out.b[k] = static_cast<u8>(std::popcount(static_cast<unsigned>(m.b[k])));
            wr(x.d, out, nb);
            return std::nullopt;
        }
        case Op::Mvn: {
            const V m = rd(x.m, nb);
            V out;
            for (unsigned k = 0; k < nb; ++k) out.b[k] = static_cast<u8>(~m.b[k]);
            wr(x.d, out, nb);
            return std::nullopt;
        }
        case Op::Qabs: each1(nb, [&](W m, W) { return sat(m < ws(0) ? -m : m, bits, true); }); return finishQc();
        case Op::Qneg: each1(nb, [&](W m, W) { return sat(-m, bits, true); }); return finishQc();
        case Op::CmpZero: {
            if (x.f) {
                fl1([&](u32 a) -> u32 {
                    a = flushIn(a, fx);
                    if (isNan(a)) {
                        if (x.imm != 2 || isSnan(a)) fx.f |= kIOC;   // (an equality test raises it for a signaling NaN only)
                        return 0;
                    }
                    const double v = dbl(a);
                    const bool holds = x.imm == 0 ? v > 0 : x.imm == 1 ? v >= 0 : x.imm == 2 ? v == 0 : x.imm == 3 ? v <= 0 : v < 0;
                    return holds ? 0xFFFFFFFFu : 0;
                });
                return std::nullopt;
            }
            each1(nb, [&](W m, W) {
                const bool r = x.imm == 0 ? m > ws(0) : x.imm == 1 ? m >= ws(0) : x.imm == 2 ? m == ws(0) : x.imm == 3 ? m <= ws(0) : m < ws(0);
                return ones(r);
            });
            return std::nullopt;
        }
        case Op::Abs:
            if (x.f) { fl1([&](u32 a) { return a & 0x7FFFFFFFu; }); return std::nullopt; }
            each1(nb, [&](W m, W) { return m < ws(0) ? -m : m; });
            return std::nullopt;
        case Op::Neg:
            if (x.f) { fl1([&](u32 a) { return a ^ 0x80000000u; }); return std::nullopt; }
            each1(nb, [&](W m, W) { return -m; });
            return std::nullopt;
        case Op::Swp: {
            const V a = rd(x.d, nb), b = rd(x.m, nb);
            wr(x.d, b, nb);
            wr(x.m, a, nb);
            return std::nullopt;
        }
        case Op::Trn: {
            V a = rd(x.d, nb), b = rd(x.m, nb);
            for (unsigned i = 0; i + 1 < nb / eb; i += 2) {
                const u64 p = lane(a, eb, i + 1), q = lane(b, eb, i);
                setLane(a, eb, i + 1, q);
                setLane(b, eb, i, p);
            }
            wr(x.d, a, nb);
            wr(x.m, b, nb);
            return std::nullopt;
        }
        case Op::Uzp: case Op::Zip: {
            const V a = rd(x.d, nb), b = rd(x.m, nb);
            const unsigned n = nb / eb;
            V oa, ob;
            if (x.op == Op::Uzp) {
                for (unsigned i = 0; i < n / 2; ++i) {
                    setLane(oa, eb, i, lane(a, eb, 2 * i));
                    setLane(oa, eb, n / 2 + i, lane(b, eb, 2 * i));
                    setLane(ob, eb, i, lane(a, eb, 2 * i + 1));
                    setLane(ob, eb, n / 2 + i, lane(b, eb, 2 * i + 1));
                }
            } else {
                for (unsigned i = 0; i < n / 2; ++i) {
                    setLane(oa, eb, 2 * i, lane(a, eb, i));
                    setLane(oa, eb, 2 * i + 1, lane(b, eb, i));
                    setLane(ob, eb, 2 * i, lane(a, eb, n / 2 + i));
                    setLane(ob, eb, 2 * i + 1, lane(b, eb, n / 2 + i));
                }
            }
            wr(x.d, oa, nb);
            wr(x.m, ob, nb);
            return std::nullopt;
        }
        case Op::Movn: case Op::Qmovn: case Op::Qmovun: {
            const unsigned se = eb * 2;
            const V m = rd(x.m, 16);
            V out;
            for (unsigned i = 0; i < 8 / eb; ++i) {
                const bool uns_src = x.op == Op::Qmovn && u;
                const W v = readLaneS(lane(m, se, i), se, uns_src || x.op == Op::Movn);
                setLane(out, eb, i, x.op == Op::Movn ? lowBits(v, bits) : lowBits(sat(v, bits, x.op == Op::Qmovn ? !u : false), bits));
            }
            wr(x.d, out, 8);
            return x.op == Op::Movn ? std::nullopt : finishQc();
        }
        case Op::Cvt16: {
            if (x.acc) {   // half to single: D source, Q destination
                const V m = rd(x.m, 8);
                V out;
                for (unsigned i = 0; i < 4; ++i) setLane(out, 4, i, halfToSingle(static_cast<u32>(lane(m, 2, i)), fx));
                wr(x.d, out, 16);
            } else {
                const V m = rd(x.m, 16);
                V out;
                for (unsigned i = 0; i < 4; ++i) setLane(out, 2, i, singleToHalf(static_cast<u32>(lane(m, 4, i)), fx));
                wr(x.d, out, 8);
            }
            fpscr_ |= fx.f;
            return std::nullopt;
        }
        case Op::Recpe:
            fl1([&](u32 a) -> u32 {
                if (x.f) return fpRecipEstimate(a, fx);
                return (a & 0x80000000u) == 0 ? 0xFFFFFFFFu : recipEstimate9(a >> 23) << 23;
            });
            return std::nullopt;
        case Op::Rsqrte:
            fl1([&](u32 a) -> u32 {
                if (x.f) return fpRsqrtEstimate(a, fx);
                if ((a >> 30) == 0) return 0xFFFFFFFFu;
                return rsqrtEstimate9((a & 0x80000000u) ? a >> 23 : (a >> 23) & 0xFF) << 23;
            });
            return std::nullopt;
        case Op::CvtIntFloat:
            fl1([&](u32 a) -> u32 {
                switch (x.imm) {
                    case 0: return intToFloat(a, true, 0, fx);
                    case 1: return intToFloat(a, false, 0, fx);
                    case 2: return floatToInt(a, true, 0, fx);
                    default: return floatToInt(a, false, 0, fx);
                }
            });
            return std::nullopt;
        case Op::Ext: {
            const V a = rd(x.n, nb), b = rd(x.m, nb);
            V out;
            for (unsigned k = 0; k < nb; ++k) out.b[k] = x.imm + k < nb ? a.b[x.imm + k] : b.b[x.imm + k - nb];
            wr(x.d, out, nb);
            return std::nullopt;
        }
        case Op::Tbl: {
            const V idx = rd(x.m, 8), d = rd(x.d, 8);
            V table[4];
            for (unsigned k = 0; k <= x.imm; ++k) table[k] = rd(x.n + k, 8);
            V out;
            for (unsigned k = 0; k < 8; ++k) {
                const unsigned i = idx.b[k];
                out.b[k] = i < 8 * (x.imm + 1) ? table[i / 8].b[i % 8] : x.tbx ? d.b[k] : 0;
            }
            wr(x.d, out, 8);
            return std::nullopt;
        }
        case Op::DupScalar: {
            const V m = rd(x.m, 8);
            V out;
            for (unsigned i = 0; i < nb / eb; ++i) setLane(out, eb, i, lane(m, eb, x.idx));
            wr(x.d, out, nb);
            return std::nullopt;
        }
        case Op::MovToScalar: {
            V v = rd(x.d, 8);
            setLane(v, eb, x.idx, r[x.rt]);
            wr(x.d, v, 8);
            return std::nullopt;
        }
        case Op::MovFromScalar: {
            const V v = rd(x.m, 8);
            const u64 raw = lane(v, eb, x.idx);
            r[x.rt] = eb == 4 ? static_cast<u32>(raw) : u ? static_cast<u32>(raw) : static_cast<u32>(static_cast<std::int32_t>(readLane(raw, eb, false).lo));
            return std::nullopt;
        }
        case Op::DupCore: {
            V out;
            for (unsigned i = 0; i < nb / eb; ++i) setLane(out, eb, i, r[x.rt]);
            wr(x.d, out, nb);
            return std::nullopt;
        }
        default: break;
    }
    (void)lanesOf;
    return Fault{Fault::Kind::NotRun, at, 0, "neon", 0};
}

}  // namespace cb::emulate::arm
