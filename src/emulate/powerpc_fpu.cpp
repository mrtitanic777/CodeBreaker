// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/powerpc_fpu.cpp -- see powerpc_fpu.hpp.
//
// Every finite result is made exactly (a normalized 128-bit significand, its exponent, and a flag for anything nonzero below it) and rounded once by round_big(), which is the
// manual's rounding with the exponent range unbounded and then bounded: it keeps p bits (fewer in the denormal range), chooses the increment by RN, and from the unrounded
// value and the rounded one gives FR (the magnitude was incremented), FI (the result is inexact), tininess (before rounding), overflow (after rounding) and the wrapped
// exponent of an enabled overflow or underflow.
#include "emulate/powerpc_fpu.hpp"

#include <bit>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace cb::emulate::powerpc::fpu {

namespace {

constexpr std::uint64_t kSignBit = std::uint64_t{1} << 63, kQuietBit = std::uint64_t{1} << 51, kFracMask = (std::uint64_t{1} << 52) - 1, kExpMask = 0x7FF0000000000000ull;
constexpr std::uint64_t kDefaultNan = 0x7FF8000000000000ull;

// ---- 128-bit integers ------------------------------------------------------------------------------------------------------------------------------------------------------

struct U128 {
    std::uint64_t hi = 0, lo = 0;
};

U128 shl(U128 a, unsigned n) {
    if (n == 0) return a;
    if (n >= 128) return {0, 0};
    if (n >= 64) return {a.lo << (n - 64), 0};
    return {(a.hi << n) | (a.lo >> (64 - n)), a.lo << n};
}

// a >> n; `sticky` is raised when a set bit was shifted out.
U128 shr(U128 a, unsigned n, bool& sticky) {
    if (n == 0) return a;
    if (n >= 128) {
        sticky = sticky || (a.hi | a.lo) != 0;
        return {0, 0};
    }
    if (n >= 64) {
        const std::uint64_t kept_part = n == 64 ? 0 : (a.hi & ((std::uint64_t{1} << (n - 64)) - 1));
        sticky = sticky || (a.lo | kept_part) != 0;
        return {0, a.hi >> (n - 64)};
    }
    sticky = sticky || (a.lo & ((std::uint64_t{1} << n) - 1)) != 0;
    return {a.hi >> n, (a.lo >> n) | (a.hi << (64 - n))};
}

U128 add(U128 a, U128 b) {
    U128 r;
    r.lo = a.lo + b.lo;
    r.hi = a.hi + b.hi + (r.lo < a.lo ? 1 : 0);
    return r;
}
U128 sub(U128 a, U128 b) {
    U128 r;
    r.lo = a.lo - b.lo;
    r.hi = a.hi - b.hi - (a.lo < b.lo ? 1 : 0);
    return r;
}
int compare(U128 a, U128 b) { return a.hi != b.hi ? (a.hi < b.hi ? -1 : 1) : a.lo != b.lo ? (a.lo < b.lo ? -1 : 1) : 0; }
bool is_zero(U128 a) { return (a.hi | a.lo) == 0; }
unsigned clz(U128 a) { return a.hi != 0 ? static_cast<unsigned>(std::countl_zero(a.hi)) : 64 + static_cast<unsigned>(std::countl_zero(a.lo)); }
bool bit_at(U128 a, unsigned k) { return k >= 64 ? ((a.hi >> (k - 64)) & 1) != 0 : ((a.lo >> k) & 1) != 0; }
bool any_below(U128 a, unsigned k) {   // a bit of 0..k-1 is set
    if (k == 0) return false;
    if (k < 64) return (a.lo & ((std::uint64_t{1} << k) - 1)) != 0;
    if (k == 64) return a.lo != 0;
    if (k >= 128) return (a.hi | a.lo) != 0;
    return a.lo != 0 || (a.hi & ((std::uint64_t{1} << (k - 64)) - 1)) != 0;
}

U128 multiply(std::uint64_t a, std::uint64_t b) {
#if defined(__SIZEOF_INT128__)
    const unsigned __int128 p = static_cast<unsigned __int128>(a) * b;
    return {static_cast<std::uint64_t>(p >> 64), static_cast<std::uint64_t>(p)};
#else
    const std::uint64_t a0 = a & 0xFFFFFFFFull, a1 = a >> 32, b0 = b & 0xFFFFFFFFull, b1 = b >> 32;
    const std::uint64_t p00 = a0 * b0, p01 = a0 * b1, p10 = a1 * b0, p11 = a1 * b1;
    const std::uint64_t mid = (p00 >> 32) + (p01 & 0xFFFFFFFFull) + (p10 & 0xFFFFFFFFull);
    return {p11 + (p01 >> 32) + (p10 >> 32) + (mid >> 32), (mid << 32) | (p00 & 0xFFFFFFFFull)};
#endif
}

// ---- numbers -----------------------------------------------------------------------------------------------------------------------------------------------------------

enum class Cls : std::uint8_t { Zero, Finite, Inf, Nan };

// A double taken apart. For a finite nonzero number: m has bit 63 set and the value is m * 2^(exp - 63) (exp is the exponent of the leading bit).
struct Num {
    bool neg = false;
    Cls cls = Cls::Zero;
    int exp = 0;
    std::uint64_t m = 0;
};

Num unpack(std::uint64_t b) {
    Num n;
    n.neg = (b & kSignBit) != 0;
    const unsigned e = static_cast<unsigned>((b >> 52) & 0x7FF);
    const std::uint64_t f = b & kFracMask;
    if (e == 0x7FF) {
        n.cls = f ? Cls::Nan : Cls::Inf;
    } else if (e == 0) {
        if (f == 0) return n;
        n.cls = Cls::Finite;
        const unsigned z = static_cast<unsigned>(std::countl_zero(f));
        n.m = f << z;
        n.exp = 63 - static_cast<int>(z) - 1074;
    } else {
        n.cls = Cls::Finite;
        n.m = (f | (std::uint64_t{1} << 52)) << 11;
        n.exp = static_cast<int>(e) - 1023;
    }
    return n;
}

bool is_nan(std::uint64_t b) { return (b & kExpMask) == kExpMask && (b & kFracMask) != 0; }
bool is_snan(std::uint64_t b) { return is_nan(b) && (b & kQuietBit) == 0; }

// A finite value, exact: m has bit 127 set; the value is m * 2^(exp - 127); `sticky`: something nonzero lies below the last bit of m.
struct Big {
    bool neg = false;
    int exp = 0;
    U128 m;
    bool sticky = false;
};

Big big_of(const Num& n) { return {n.neg, n.exp, {n.m, 0}, false}; }

struct Fmt {
    int p, emin, emax, wrap;   // precision, the least and greatest exponent of a normal number, and what an enabled overflow or underflow adds to the exponent
};
constexpr Fmt kDouble{53, -1022, 1023, 1536};
constexpr Fmt kSingle{24, -126, 127, 192};

struct Env {
    unsigned rn = 0;
    bool ve = false, oe = false, ue = false, ze = false;
};
Env env_of(std::uint32_t fpscr) { return {fpscr & kRN, (fpscr & kVE) != 0, (fpscr & kOE) != 0, (fpscr & kUE) != 0, (fpscr & kZE) != 0}; }

// What an operation leaves: the result, and the status changes it makes.
struct Out {
    std::uint64_t bits = 0;
    std::uint32_t raise = 0;      // exception bits to set
    bool fr = false, fi = false;
    bool set_fprf = true;
    bool estimate = false;        // FR, FI and XX are left clear (the hardware's estimate is not the correctly rounded value)
};

std::uint64_t make_double(bool neg, std::uint64_t q, int qe) {
    const std::uint64_t sign = neg ? kSignBit : 0;
    if (q == 0) return sign;
    const int n = 64 - std::countl_zero(q);
    const int e = n - 1 + qe;
    if (e > 1023) return sign | kExpMask;
    if (e >= -1022) return sign | (static_cast<std::uint64_t>(e + 1023) << 52) | ((q << (53 - n)) & kFracMask);
    const int shift = qe + 1074;
    if (shift >= 0) return sign | (q << shift);
    return -shift >= 64 ? sign : sign | (q >> -shift);
}

std::uint64_t infinity(bool neg) { return (neg ? kSignBit : 0) | kExpMask; }
std::uint64_t max_finite(bool neg, const Fmt& f) { return (neg ? kSignBit : 0) | (f.p == 53 ? 0x7FEFFFFFFFFFFFFFull : 0x47EFFFFFE0000000ull); }

// The manual's rounding of an exact value to the format under RN (see the top of the file); the result as raw double bits.
std::uint64_t round_big(const Big& x, const Fmt& f, const Env& cx, Out& o) {
    const bool tiny = x.exp < f.emin;
    const bool denormal = tiny && !cx.ue;   // (with UE set the result is rounded as if the exponent range had no bottom, and wrapped)
    const int kept = denormal ? f.p - (f.emin - x.exp) : f.p;
    std::uint64_t q = 0;
    bool round_bit, rest;
    if (kept >= 1) {
        q = x.m.hi >> (64 - kept);
        round_bit = bit_at(x.m, static_cast<unsigned>(127 - kept));
        rest = x.sticky || any_below(x.m, static_cast<unsigned>(127 - kept));
    } else if (kept == 0) {
        round_bit = true;
        rest = x.sticky || any_below(x.m, 127);
    } else {
        round_bit = false;
        rest = true;
    }
    const bool inexact = round_bit || rest;
    bool increment = false;
    switch (cx.rn) {
        case 0: increment = round_bit && (rest || (q & 1)); break;
        case 1: break;
        case 2: increment = inexact && !x.neg; break;
        default: increment = inexact && x.neg; break;
    }
    q += increment ? 1 : 0;
    int qe = denormal ? f.emin - f.p + 1 : x.exp - f.p + 1;
    if (!denormal && q == (std::uint64_t{1} << f.p)) {
        q >>= 1;
        ++qe;
    }
    const bool overflow = !denormal && !tiny && q != 0 && (64 - std::countl_zero(q) - 1 + qe) > f.emax;
    if (tiny && (cx.ue || inexact)) o.raise |= kUX;
    if (overflow) {
        o.raise |= kOX;
        if (cx.oe) {
            o.fr = increment;
            o.fi = inexact;
            if (inexact) o.raise |= kXX;
            return make_double(x.neg, q, qe - f.wrap);
        }
        o.fi = true;
        o.raise |= kXX;
        const bool to_infinity = cx.rn == 0 || (cx.rn == 2 && !x.neg) || (cx.rn == 3 && x.neg);
        o.fr = to_infinity;
        return to_infinity ? infinity(x.neg) : max_finite(x.neg, f);
    }
    o.fr = inexact && increment;
    o.fi = inexact;
    if (inexact) o.raise |= kXX;
    if (tiny && cx.ue) qe += f.wrap;
    return make_double(x.neg, q, qe);
}

// ---- the exact results ---------------------------------------------------------------------------------------------------------------------------------------------------

// a + b exactly (the larger by magnitude first). False when they cancel exactly.
bool add_bigs(Big a, Big b, Big& out) {
    if (a.exp < b.exp || (a.exp == b.exp && compare(a.m, b.m) < 0)) std::swap(a, b);
    bool sticky = false;
    const U128 big = shr(a.m, 2, sticky);   // (the low bits of both are zero, so nothing is lost)
    U128 small = shr(b.m, static_cast<unsigned>(a.exp - b.exp) + 2, sticky);
    if (sticky) small.lo |= 1;
    U128 sum = a.neg == b.neg ? add(big, small) : sub(big, small);
    if (is_zero(sum)) return false;
    const unsigned z = clz(sum);
    sum = shl(sum, z);
    out = {a.neg, a.exp + 2 - static_cast<int>(z), sum, false};
    return true;
}

Big product(const Num& a, const Num& c) {
    U128 p = multiply(a.m, c.m);
    int e = a.exp + c.exp;
    if (p.hi >> 63) ++e;
    else p = shl(p, 1);
    return {a.neg != c.neg, e, p, false};
}

Big quotient(const Num& a, const Num& b) {
    const bool whole = a.m >= b.m;
    std::uint64_t r = whole ? a.m - b.m : a.m, frac = 0;
    for (unsigned i = 0; i < 64; ++i) {
        const bool carry = (r >> 63) != 0;
        r <<= 1;
        frac <<= 1;
        if (carry || r >= b.m) {
            r -= b.m;
            frac |= 1;
        }
    }
    Big big;
    big.neg = a.neg != b.neg;
    big.sticky = r != 0;
    if (whole) {
        big.exp = a.exp - b.exp;
        big.m = {(std::uint64_t{1} << 63) | (frac >> 1), frac << 63};
    } else {
        big.exp = a.exp - b.exp - 1;
        big.m = {frac, 0};
    }
    return big;
}

Big square_root(const Num& a) {
    const int t = a.exp - 63;
    U128 radicand;
    int k;
    if (((t - 64) & 1) == 0) radicand = {a.m, 0}, k = (t - 64) / 2;
    else radicand = {a.m >> 1, a.m << 63}, k = (t - 63) / 2;
    U128 rem = radicand, result{0, 0}, bit{std::uint64_t{1} << 62, 0};
    bool unused = false;
    for (unsigned i = 0; i < 64; ++i) {
        const U128 trial = add(result, bit);
        if (compare(rem, trial) >= 0) {
            rem = sub(rem, trial);
            result = add(shr(result, 1, unused), bit);
        } else {
            result = shr(result, 1, unused);
        }
        bit = shr(bit, 2, unused);
    }
    Big big;
    big.neg = false;
    big.exp = 63 + k;
    big.m = {result.lo, 0};
    big.sticky = !is_zero(rem);
    return big;
}

// ---- classification, NaNs ----------------------------------------------------------------------------------------------------------------------------------------------

unsigned fprf_of(std::uint64_t bits, bool single) {
    const bool neg = (bits & kSignBit) != 0;
    const unsigned e = static_cast<unsigned>((bits >> 52) & 0x7FF);
    const std::uint64_t f = bits & kFracMask;
    if (e == 0x7FF) return f ? 0x11 : neg ? 0x09 : 0x05;
    if (e == 0 && f == 0) return neg ? 0x12 : 0x02;
    const bool denormal = e == 0 || (single && e < 1023 - 126);
    return denormal ? (neg ? 0x18 : 0x14) : (neg ? 0x08 : 0x04);
}

std::uint64_t quieted(std::uint64_t nan, bool single) {
    nan |= kQuietBit;
    if (single) nan &= ~((std::uint64_t{1} << 29) - 1);
    return nan;
}

// The first NaN among the operands (in the order the instruction looks) is the result, quieted; any signaling NaN among them is an invalid operation.
bool pick_nan(const std::uint64_t* ops, unsigned n, Out& o, bool single) {
    bool found = false;
    for (unsigned i = 0; i < n; ++i) {
        if (!is_nan(ops[i])) continue;
        if (!found) o.bits = quieted(ops[i], single), found = true;
        if (is_snan(ops[i])) o.raise |= kVXSNAN;
    }
    return found;
}

void invalid(Out& o, std::uint32_t bit) {
    o.raise |= bit;
    o.bits = kDefaultNan;
}

std::uint64_t zero_of(bool neg) { return neg ? kSignBit : 0; }

// a + b of two numbers that are not NaNs (the sum of the signs as given).
void add_nums(Out& o, const Env& cx, const Fmt& f, const Num& a, const Num& b) {
    if (a.cls == Cls::Inf || b.cls == Cls::Inf) {
        if (a.cls == Cls::Inf && b.cls == Cls::Inf && a.neg != b.neg) invalid(o, kVXISI);
        else o.bits = infinity(a.cls == Cls::Inf ? a.neg : b.neg);
        return;
    }
    if (a.cls == Cls::Zero && b.cls == Cls::Zero) {
        o.bits = zero_of(a.neg == b.neg ? a.neg : cx.rn == 3);
        return;
    }
    if (a.cls == Cls::Zero || b.cls == Cls::Zero) {
        o.bits = round_big(big_of(a.cls == Cls::Zero ? b : a), f, cx, o);
        return;
    }
    Big sum;
    if (!add_bigs(big_of(a), big_of(b), sum)) o.bits = zero_of(cx.rn == 3);
    else o.bits = round_big(sum, f, cx, o);
}

// a * c + b (b with its sign as the operation wants it), none a NaN.
void fused_nums(Out& o, const Env& cx, const Fmt& f, const Num& a, const Num& c, const Num& b) {
    const bool product_neg = a.neg != c.neg;
    if (a.cls == Cls::Inf || c.cls == Cls::Inf) {
        if (a.cls == Cls::Zero || c.cls == Cls::Zero) invalid(o, kVXIMZ);
        else if (b.cls == Cls::Inf && b.neg != product_neg) invalid(o, kVXISI);
        else o.bits = infinity(product_neg);
        return;
    }
    if (b.cls == Cls::Inf) {
        o.bits = infinity(b.neg);
        return;
    }
    const bool product_zero = a.cls == Cls::Zero || c.cls == Cls::Zero;
    if (product_zero && b.cls == Cls::Zero) {
        o.bits = zero_of(product_neg == b.neg ? b.neg : cx.rn == 3);
        return;
    }
    if (product_zero) {
        o.bits = round_big(big_of(b), f, cx, o);
        return;
    }
    const Big p = product(a, c);
    if (b.cls == Cls::Zero) {
        o.bits = round_big(p, f, cx, o);
        return;
    }
    Big sum;
    if (!add_bigs(p, big_of(b), sum)) o.bits = zero_of(cx.rn == 3);
    else o.bits = round_big(sum, f, cx, o);
}

void divide_nums(Out& o, const Env& cx, const Fmt& f, const Num& a, const Num& b) {
    const bool neg = a.neg != b.neg;
    if (a.cls == Cls::Inf && b.cls == Cls::Inf) return invalid(o, kVXIDI);
    if (a.cls == Cls::Zero && b.cls == Cls::Zero) return invalid(o, kVXZDZ);
    if (b.cls == Cls::Zero) {
        if (a.cls != Cls::Inf) o.raise |= kZX;
        o.bits = infinity(neg);
        return;
    }
    if (a.cls == Cls::Inf) {
        o.bits = infinity(neg);
        return;
    }
    if (b.cls == Cls::Inf || a.cls == Cls::Zero) {
        o.bits = zero_of(neg);
        return;
    }
    o.bits = round_big(quotient(a, b), f, cx, o);
}

}  // namespace

// ---- the status register ---------------------------------------------------------------------------------------------------------------------------------------------------

void refresh(std::uint32_t& fpscr) {
    fpscr &= kWritable;
    fpscr &= ~(kVX | kFEX);
    const bool invalid_bits = (fpscr & kInvalidBits) != 0;
    if (invalid_bits) fpscr |= kVX;
    if ((invalid_bits && (fpscr & kVE)) || ((fpscr & kOX) && (fpscr & kOE)) || ((fpscr & kUX) && (fpscr & kUE)) || ((fpscr & kZX) && (fpscr & kZE)) || ((fpscr & kXX) && (fpscr & kXE)))
        fpscr |= kFEX;
}

namespace {

// Apply what an operation raised and left. Returns whether the target register is written.
bool commit(std::uint32_t& fpscr, const Env& cx, Out o, std::uint64_t bits_for_class, bool single) {
    if (o.estimate) {
        o.fr = o.fi = false;
        o.raise &= ~kXX;
    }
    const bool stopped = ((o.raise & kInvalidBits) && cx.ve) || ((o.raise & kZX) && cx.ze);
    fpscr &= ~(kFR | kFI);
    if (!stopped) {
        if (o.set_fprf) fpscr = (fpscr & ~kFPRF) | (fprf_of(bits_for_class, single) << 12);
        if (o.fr) fpscr |= kFR;
        if (o.fi) fpscr |= kFI;
    }
    if ((o.raise & kSticky) & ~fpscr) fpscr |= kFX;
    fpscr |= o.raise;
    refresh(fpscr);
    return !stopped;
}

// fctiw, fctiwz, fctid, fctidz: b rounded to an integer (by RN, or toward zero) and saturated; a NaN, an infinity or a value out of range is an invalid conversion (VXCVI) and leaves
// the most negative or positive integer (a NaN the most negative). FPRF is undefined for it and left alone.
Result convert_to_integer(std::uint32_t& fpscr, const Env& cx, std::uint64_t b, unsigned bits, bool toward_zero) {
    Out o;
    o.set_fprf = false;
    const std::int64_t lo = bits == 64 ? INT64_MIN : -(std::int64_t{1} << 31), hi = bits == 64 ? INT64_MAX : (std::int64_t{1} << 31) - 1;
    const Num n = unpack(b);
    std::int64_t value = 0;
    bool bad = false;
    if (n.cls == Cls::Nan) {
        bad = true;
        value = lo;
        if (is_snan(b)) o.raise |= kVXSNAN;
    } else if (n.cls == Cls::Inf) {
        bad = true;
        value = n.neg ? lo : hi;
    } else if (n.cls == Cls::Finite) {
        const double x = std::bit_cast<double>(b);
        double t = std::trunc(x);
        const double frac = x - t;
        const unsigned mode = toward_zero ? 1 : cx.rn;
        bool away = false;
        if (mode == 0) away = std::fabs(frac) > 0.5 || (std::fabs(frac) == 0.5 && std::fmod(t, 2.0) != 0);
        else if (mode == 2) away = frac > 0;
        else if (mode == 3) away = frac < 0;
        if (away) t += x < 0 ? -1.0 : 1.0;
        const double limit = bits == 64 ? 9223372036854775808.0 : 2147483648.0;
        if (t >= limit || t < -limit) {
            bad = true;
            value = x < 0 ? lo : hi;
        } else {
            value = static_cast<std::int64_t>(t);
            o.fi = frac != 0;
            o.fr = away;
            if (o.fi) o.raise |= kXX;
        }
    }
    if (bad) {
        o.raise |= kVXCVI;
        o.fr = o.fi = false;
    }
    o.bits = bits == 64 ? static_cast<std::uint64_t>(value) : (static_cast<std::uint64_t>(value) & 0xFFFFFFFFull);
    const bool written = commit(fpscr, cx, o, o.bits, false);
    return {o.bits, written};
}

}  // namespace

Result execute(std::uint32_t& fpscr, Op op, bool single, std::uint64_t a, std::uint64_t b, std::uint64_t c) {
    const Env cx = env_of(fpscr);
    Out o;
    bool negate = false;
    bool single_result = single;
    const Fmt* fmt = single ? &kSingle : &kDouble;
    switch (op) {
        case Op::Add:
        case Op::Sub: {
            const std::uint64_t ops[2] = {a, b};
            if (pick_nan(ops, 2, o, single)) break;
            Num x = unpack(a), y = unpack(b);
            if (op == Op::Sub) y.neg = !y.neg;
            add_nums(o, cx, *fmt, x, y);
            break;
        }
        case Op::Mul: {
            const std::uint64_t ops[2] = {a, c};
            if (pick_nan(ops, 2, o, single)) break;
            const Num x = unpack(a), y = unpack(c);
            const Num zero;
            fused_nums(o, cx, *fmt, x, y, zero);   // (a zero addend leaves the product's own sign: a product that is zero keeps the operands' sign)
            if (x.cls != Cls::Inf && y.cls != Cls::Inf && (x.cls == Cls::Zero || y.cls == Cls::Zero)) o.bits = zero_of(x.neg != y.neg);
            break;
        }
        case Op::Div:
        case Op::Res: {
            const std::uint64_t one = 0x3FF0000000000000ull;
            const std::uint64_t ops[2] = {op == Op::Div ? a : b, b};
            if (pick_nan(ops + (op == Op::Div ? 0 : 1), op == Op::Div ? 2 : 1, o, single)) break;
            divide_nums(o, cx, *fmt, unpack(op == Op::Div ? a : one), unpack(b));
            o.estimate = op == Op::Res;
            break;
        }
        case Op::Madd:
        case Op::Msub:
        case Op::Nmadd:
        case Op::Nmsub: {
            const std::uint64_t ops[3] = {a, b, c};
            if (pick_nan(ops, 3, o, single)) break;
            Num addend = unpack(b);
            if (op == Op::Msub || op == Op::Nmsub) addend.neg = !addend.neg;
            fused_nums(o, cx, *fmt, unpack(a), unpack(c), addend);
            negate = op == Op::Nmadd || op == Op::Nmsub;
            break;
        }
        case Op::Sqrt: {
            if (pick_nan(&b, 1, o, single)) break;
            const Num n = unpack(b);
            if (n.cls == Cls::Zero || (n.cls == Cls::Inf && !n.neg)) o.bits = b;
            else if (n.neg) invalid(o, kVXSQRT);
            else o.bits = round_big(square_root(n), *fmt, cx, o);
            break;
        }
        case Op::Rsqrte: {
            if (pick_nan(&b, 1, o, single)) break;
            const Num n = unpack(b);
            o.estimate = true;
            if (n.cls == Cls::Zero) {
                o.raise |= kZX;
                o.bits = infinity(n.neg);
            } else if (n.neg) {
                invalid(o, kVXSQRT);
            } else if (n.cls == Cls::Inf) {
                o.bits = 0;
            } else {
                const double r = 1.0 / std::sqrt(std::bit_cast<double>(b));
                o.bits = std::bit_cast<std::uint64_t>(single ? static_cast<double>(static_cast<float>(r)) : r);
            }
            break;
        }
        case Op::Frsp: {
            single_result = true;
            if (pick_nan(&b, 1, o, true)) break;
            const Num n = unpack(b);
            if (n.cls == Cls::Finite) o.bits = round_big(big_of(n), kSingle, cx, o);
            else o.bits = b;
            break;
        }
        case Op::Fctiw:
        case Op::Fctiwz:
        case Op::Fctid:
        case Op::Fctidz:
            return convert_to_integer(fpscr, cx, b, (op == Op::Fctiw || op == Op::Fctiwz) ? 32 : 64, op == Op::Fctiwz || op == Op::Fctidz);
        case Op::Fcfid: {
            const std::int64_t v = static_cast<std::int64_t>(b);
            if (v == 0) {
                o.bits = 0;
                break;
            }
            const std::uint64_t magnitude = v < 0 ? 0 - static_cast<std::uint64_t>(v) : static_cast<std::uint64_t>(v);
            const unsigned z = static_cast<unsigned>(std::countl_zero(magnitude));
            Big big{v < 0, 63 - static_cast<int>(z), {magnitude << z, 0}, false};
            o.bits = round_big(big, kDouble, cx, o);
            break;
        }
    }
    if (negate && !is_nan(o.bits)) o.bits ^= kSignBit;
    const bool written = commit(fpscr, cx, o, o.bits, single_result);
    return {o.bits, written};
}

Pair execute_paired(std::uint32_t& fpscr, Op op, std::uint64_t a0, std::uint64_t b0, std::uint64_t c0, std::uint64_t a1, std::uint64_t b1, std::uint64_t c1) {
    Pair p;
    p.first = execute(fpscr, op, true, a0, b0, c0);
    std::uint32_t second = fpscr;
    p.second = execute(second, op, true, a1, b1, c1);
    fpscr |= second & (kSticky | kFX);
    refresh(fpscr);
    return p;
}

unsigned compare(std::uint32_t& fpscr, std::uint64_t a, std::uint64_t b, bool ordered) {
    const bool unordered = is_nan(a) || is_nan(b);
    const double x = std::bit_cast<double>(a), y = std::bit_cast<double>(b);
    const unsigned field = unordered ? 1 : x < y ? 8 : x > y ? 4 : 2;
    std::uint32_t raise = 0;
    if (is_snan(a) || is_snan(b)) {
        raise |= kVXSNAN;
        if (ordered && !(fpscr & kVE)) raise |= kVXVC;
    } else if (ordered && unordered) {
        raise |= kVXVC;
    }
    fpscr = (fpscr & ~kFPRF) | (field << 12);
    if ((raise & kSticky) & ~fpscr) fpscr |= kFX;
    fpscr |= raise;
    refresh(fpscr);
    return field;
}

std::uint64_t move_from(std::uint32_t fpscr) { return 0xFFF8000000000000ull | fpscr; }

void move_to_fields(std::uint32_t& fpscr, unsigned field_mask, std::uint64_t frb) {
    std::uint32_t mask = 0;
    for (unsigned i = 0; i < 8; ++i)
        if (field_mask & (0x80u >> i)) mask |= 0xF0000000u >> (4 * i);
    mask &= ~(kFEX | kVX);
    fpscr = (fpscr & ~mask) | (static_cast<std::uint32_t>(frb) & mask);
    refresh(fpscr);
}

void move_to_field_immediate(std::uint32_t& fpscr, unsigned field, unsigned value) {
    const unsigned shift = 28 - 4 * (field & 7);
    std::uint32_t mask = 0xFu << shift;
    mask &= ~(kFEX | kVX);
    fpscr = (fpscr & ~mask) | ((static_cast<std::uint32_t>(value & 0xF) << shift) & mask);
    refresh(fpscr);
}

void set_bit(std::uint32_t& fpscr, unsigned bit, bool value) {
    const std::uint32_t mask = 1u << (31 - (bit & 31));
    if (mask & (kFEX | kVX)) {   // (computed, not written)
        refresh(fpscr);
        return;
    }
    if (value) {
        if ((mask & kSticky) && !(fpscr & mask)) fpscr |= kFX;
        fpscr |= mask;
    } else {
        fpscr &= ~mask;
    }
    refresh(fpscr);
}

unsigned move_to_cr_from_field(std::uint32_t& fpscr, unsigned field) {
    const unsigned shift = 28 - 4 * (field & 7);
    const unsigned value = (fpscr >> shift) & 0xF;
    fpscr &= ~((0xFu << shift) & (kFX | kSticky));
    refresh(fpscr);
    return value;
}

}  // namespace cb::emulate::powerpc::fpu
