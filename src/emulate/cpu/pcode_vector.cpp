// emulate/cpu/pcode_vector.cpp -- see pcode_vector.hpp. Portable C++17: no __int128 and no compiler extension (a 128 bit value is two 64 bit words), no host float arithmetic (the
// float lanes are rounded with integers, in every rounding mode), so MSVC, clang-cl and MinGW GCC give the same bits.
#include "emulate/cpu/pcode_vector.hpp"

#include <cstring>
#include <stdexcept>

namespace cb::emulate::vec {

namespace {

using U64 = std::uint64_t;
using I64 = std::int64_t;

[[noreturn]] void bad(const char* what) { throw std::invalid_argument(what); }

inline void check(std::size_t n, unsigned ew) {
    if ((ew != 1 && ew != 2 && ew != 4 && ew != 8) || n % ew != 0 || n > kMaxBytes) bad("pcode_vector: lane width or vector length not taken");
}
inline void checkMax(unsigned ew, unsigned most) {
    if (ew > most) bad("pcode_vector: lane too wide for this operation");
}
inline unsigned bitsOf(unsigned ew) { return ew * 8; }

// ---- 128 bit integers (two's complement unless said): enough for the exact result of a 64 bit product or sum
struct W128 {
    U64 lo = 0, hi = 0;
};
inline W128 fromU(U64 v) { return {v, 0}; }
inline W128 fromS(I64 v) { return {static_cast<U64>(v), v < 0 ? ~U64{0} : 0}; }
inline bool isNeg(const W128& v) { return (v.hi >> 63) != 0; }
inline W128 add(const W128& a, const W128& b) {
    W128 r;
    r.lo = a.lo + b.lo;
    r.hi = a.hi + b.hi + (r.lo < a.lo ? 1 : 0);
    return r;
}
inline W128 neg(const W128& a) {
    W128 r;
    r.lo = ~a.lo + 1;
    r.hi = ~a.hi + (r.lo == 0 ? 1 : 0);
    return r;
}
inline W128 sub(const W128& a, const W128& b) { return add(a, neg(b)); }
inline W128 shl(const W128& a, unsigned k) {
    if (k == 0) return a;
    if (k >= 128) return {0, 0};
    if (k >= 64) return {0, a.lo << (k - 64)};
    return {a.lo << k, (a.hi << k) | (a.lo >> (64 - k))};
}
inline W128 shrLogical(const W128& a, unsigned k) {
    if (k == 0) return a;
    if (k >= 128) return {0, 0};
    if (k >= 64) return {a.hi >> (k - 64), 0};
    return {(a.lo >> k) | (a.hi << (64 - k)), a.hi >> k};
}
inline W128 shrArith(const W128& a, unsigned k) {
    if (k == 0) return a;
    const U64 fill = isNeg(a) ? ~U64{0} : 0;
    if (k >= 128) return {fill, fill};
    if (k >= 64) return {static_cast<U64>(static_cast<I64>(a.hi) >> (k - 64)), fill};
    return {(a.lo >> k) | (a.hi << (64 - k)), static_cast<U64>(static_cast<I64>(a.hi) >> k)};
}
inline bool lessSigned(const W128& a, const W128& b) {
    if (a.hi != b.hi) return static_cast<I64>(a.hi) < static_cast<I64>(b.hi);
    return a.lo < b.lo;
}
inline bool lessUnsigned(const W128& a, const W128& b) { return a.hi != b.hi ? a.hi < b.hi : a.lo < b.lo; }
inline bool isZero(const W128& a) { return a.lo == 0 && a.hi == 0; }
// 64 x 64 -> 128 unsigned.
inline W128 mulUU(U64 a, U64 b) {
    const U64 a0 = a & 0xffffffffu, a1 = a >> 32, b0 = b & 0xffffffffu, b1 = b >> 32;
    const U64 p00 = a0 * b0, p01 = a0 * b1, p10 = a1 * b0, p11 = a1 * b1;
    const U64 mid = (p00 >> 32) + (p01 & 0xffffffffu) + (p10 & 0xffffffffu);
    W128 r;
    r.lo = (p00 & 0xffffffffu) | (mid << 32);
    r.hi = p11 + (p01 >> 32) + (p10 >> 32) + (mid >> 32);
    return r;
}
inline W128 mulSS(I64 a, I64 b) {
    const bool negative = (a < 0) != (b < 0);
    const U64 ua = a < 0 ? ~static_cast<U64>(a) + 1 : static_cast<U64>(a), ub = b < 0 ? ~static_cast<U64>(b) + 1 : static_cast<U64>(b);
    const W128 p = mulUU(ua, ub);
    return negative ? neg(p) : p;
}
inline W128 mulSU(I64 a, U64 b) {
    const U64 ua = a < 0 ? ~static_cast<U64>(a) + 1 : static_cast<U64>(a);
    const W128 p = mulUU(ua, b);
    return a < 0 ? neg(p) : p;
}

// A lane as a number: signed or unsigned, held in 128 bits.
inline W128 laneValue(U64 raw, unsigned ew, Sign sign) { return sign == Sign::Signed ? fromS(signExtend(raw, ew)) : fromU(raw); }
inline W128 maxOf(unsigned w, bool isSigned) {   // the largest value of a w bit lane
    return isSigned ? W128{(U64{1} << (w - 1)) - 1, 0} : W128{w == 64 ? ~U64{0} : (U64{1} << w) - 1, 0};
}
inline W128 minOf(unsigned w, bool isSigned) { return isSigned ? neg(shl(fromU(1), w - 1)) : W128{0, 0}; }
// `v` clamped to the lane's range; the bits as a lane.
inline U64 saturate(const W128& v, unsigned w, bool isSigned) {
    const W128 hi = isSigned ? sub(shl(fromU(1), w - 1), fromU(1)) : sub(shl(fromU(1), w), fromU(1));
    const W128 lo = minOf(w, isSigned);
    if (lessSigned(hi, v)) return hi.lo;
    if (lessSigned(v, lo)) return lo.lo;
    return v.lo;
}
inline U64 wrapTo(const W128& v, unsigned w) { return w >= 64 ? v.lo : (v.lo & ((U64{1} << w) - 1)); }

template <class F>
void lanes1(Byte* out, const Byte* a, std::size_t n, unsigned ew, F f) {
    check(n, ew);
    for (std::size_t i = 0; i < n; i += ew) store(out + i, ew, f(load(a + i, ew)));
}
template <class F>
void lanes2(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, F f) {
    check(n, ew);
    for (std::size_t i = 0; i < n; i += ew) store(out + i, ew, f(load(a + i, ew), load(b + i, ew)));
}

}  // namespace

// ---- lanes as numbers
std::uint64_t load(const Byte* p, unsigned ew) {
    U64 v = 0;
    for (unsigned i = 0; i < ew; ++i) v |= U64{p[i]} << (8 * i);
    return v;
}
void store(Byte* p, unsigned ew, std::uint64_t value) {
    for (unsigned i = 0; i < ew; ++i) p[i] = static_cast<Byte>(value >> (8 * i));
}
std::int64_t signExtend(std::uint64_t value, unsigned ew) {
    if (ew >= 8) return static_cast<I64>(value);
    const unsigned w = 8 * ew;
    const U64 m = U64{1} << (w - 1);
    value &= (U64{1} << w) - 1;
    return static_cast<I64>((value ^ m) - m);
}
std::uint64_t laneMask(unsigned ew) { return ew >= 8 ? ~U64{0} : (U64{1} << (8 * ew)) - 1; }
std::uint64_t getLane(const Byte* a, unsigned ew, std::size_t index) { return load(a + index * ew, ew); }
void setLane(Byte* a, unsigned ew, std::size_t index, std::uint64_t value) { store(a + index * ew, ew, value); }

// ---- integer arithmetic
void add(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { lanes2(out, a, b, n, ew, [](U64 x, U64 y) { return x + y; }); }
void sub(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { lanes2(out, a, b, n, ew, [](U64 x, U64 y) { return x - y; }); }
void mul(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew) { lanes2(out, a, b, n, ew, [](U64 x, U64 y) { return x * y; }); }

void mulHigh(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const U64 x = load(a + i, ew), y = load(b + i, ew);
        W128 p = sign == Sign::Signed ? mulSS(signExtend(x, ew), signExtend(y, ew)) : mulUU(x, y);
        p = sign == Sign::Signed ? shrArith(p, w) : shrLogical(p, w);
        store(out + i, ew, p.lo);
    }
}

void addSat(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const W128 s = add(laneValue(load(a + i, ew), ew, sign), laneValue(load(b + i, ew), ew, sign));
        store(out + i, ew, saturate(s, w, sign == Sign::Signed));
    }
}
void subSat(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const W128 s = sub(laneValue(load(a + i, ew), ew, sign), laneValue(load(b + i, ew), ew, sign));
        store(out + i, ew, saturate(s, w, sign == Sign::Signed));
    }
}
void addSatMixed(Byte* out, const Byte* acc, const Byte* x, std::size_t n, unsigned ew, Sign accSign) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    const Sign other = accSign == Sign::Signed ? Sign::Unsigned : Sign::Signed;
    for (std::size_t i = 0; i < n; i += ew) {
        const W128 s = add(laneValue(load(acc + i, ew), ew, accSign), laneValue(load(x + i, ew), ew, other));
        store(out + i, ew, saturate(s, w, accSign == Sign::Signed));
    }
}
void neg(Byte* out, const Byte* a, std::size_t n, unsigned ew, bool saturateMin) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const W128 v = neg(fromS(signExtend(load(a + i, ew), ew)));
        store(out + i, ew, saturateMin ? saturate(v, w, true) : wrapTo(v, w));
    }
}
void abs(Byte* out, const Byte* a, std::size_t n, unsigned ew, bool saturateMin) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        W128 v = fromS(signExtend(load(a + i, ew), ew));
        if (isNeg(v)) v = neg(v);
        store(out + i, ew, saturateMin ? saturate(v, w, true) : wrapTo(v, w));
    }
}
void absDiff(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        W128 d = sub(laneValue(load(a + i, ew), ew, sign), laneValue(load(b + i, ew), ew, sign));
        if (isNeg(d)) d = neg(d);
        store(out + i, ew, wrapTo(d, w));
    }
}
void min(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign) {
    check(n, ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const U64 x = load(a + i, ew), y = load(b + i, ew);
        const bool less = sign == Sign::Signed ? signExtend(x, ew) < signExtend(y, ew) : x < y;
        store(out + i, ew, less ? x : y);
    }
}
void max(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign) {
    check(n, ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const U64 x = load(a + i, ew), y = load(b + i, ew);
        const bool less = sign == Sign::Signed ? signExtend(x, ew) < signExtend(y, ew) : x < y;
        store(out + i, ew, less ? y : x);
    }
}
void halvingAdd(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign, bool round) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        W128 s = add(laneValue(load(a + i, ew), ew, sign), laneValue(load(b + i, ew), ew, sign));
        if (round) s = add(s, fromU(1));
        store(out + i, ew, wrapTo(shrArith(s, 1), w));
    }
}
void halvingSub(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const W128 d = sub(laneValue(load(a + i, ew), ew, sign), laneValue(load(b + i, ew), ew, sign));
        store(out + i, ew, wrapTo(shrArith(d, 1), w));
    }
}
void mulDoublingHigh(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, bool round) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        W128 p = mulSS(signExtend(load(a + i, ew), ew), signExtend(load(b + i, ew), ew));   // (2 a b [+ 2^(w-1)]) >> w == (a b [+ 2^(w-2)]) >> (w-1)
        if (round) p = add(p, shl(fromU(1), w - 2));
        store(out + i, ew, saturate(shrArith(p, w - 1), w, true));
    }
}
void mulDoublingHighAcc(Byte* out, const Byte* acc, const Byte* a, const Byte* b, std::size_t n, unsigned ew, bool subtract) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        W128 p = mulSS(signExtend(load(a + i, ew), ew), signExtend(load(b + i, ew), ew));   // (acc 2^w +/- 2 a b + 2^(w-1)) >> w == (acc 2^(w-1) +/- a b + 2^(w-2)) >> (w-1)
        if (subtract) p = neg(p);
        p = add(add(p, shl(fromS(signExtend(load(acc + i, ew), ew)), w - 1)), shl(fromU(1), w - 2));
        store(out + i, ew, saturate(shrArith(p, w - 1), w, true));
    }
}
void mulHighRoundHalfUp(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        W128 p = shrArith(mulSS(signExtend(load(a + i, ew), ew), signExtend(load(b + i, ew), ew)), w - 2);
        p = shrArith(add(p, fromU(1)), 1);
        store(out + i, ew, wrapTo(p, w));
    }
}
void mulHighAddSat(Byte* out, const Byte* a, const Byte* b, const Byte* c, std::size_t n, unsigned ew, bool round) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        W128 p = mulSS(signExtend(load(a + i, ew), ew), signExtend(load(b + i, ew), ew));
        if (round) p = add(p, shl(fromU(1), w - 2));
        p = add(shrArith(p, w - 1), fromS(signExtend(load(c + i, ew), ew)));
        store(out + i, ew, saturate(p, w, true));
    }
}

// ---- compare, select, bits
void compare(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Cmp cmp, Sign sign) {
    check(n, ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const U64 x = load(a + i, ew), y = load(b + i, ew);
        const bool lt = sign == Sign::Signed ? signExtend(x, ew) < signExtend(y, ew) : x < y;
        bool r = false;
        switch (cmp) {
            case Cmp::Eq: r = x == y; break;
            case Cmp::Ne: r = x != y; break;
            case Cmp::Lt: r = lt; break;
            case Cmp::Le: r = lt || x == y; break;
            case Cmp::Gt: r = !(lt || x == y); break;
            case Cmp::Ge: r = !lt; break;
        }
        store(out + i, ew, r ? ~U64{0} : 0);
    }
}
void bitAnd(Byte* out, const Byte* a, const Byte* b, std::size_t n) { for (std::size_t i = 0; i < n; ++i) out[i] = static_cast<Byte>(a[i] & b[i]); }
void bitOr(Byte* out, const Byte* a, const Byte* b, std::size_t n) { for (std::size_t i = 0; i < n; ++i) out[i] = static_cast<Byte>(a[i] | b[i]); }
void bitXor(Byte* out, const Byte* a, const Byte* b, std::size_t n) { for (std::size_t i = 0; i < n; ++i) out[i] = static_cast<Byte>(a[i] ^ b[i]); }
void bitAndNot(Byte* out, const Byte* a, const Byte* b, std::size_t n) { for (std::size_t i = 0; i < n; ++i) out[i] = static_cast<Byte>(a[i] & ~b[i]); }
void bitOrNot(Byte* out, const Byte* a, const Byte* b, std::size_t n) { for (std::size_t i = 0; i < n; ++i) out[i] = static_cast<Byte>(a[i] | ~b[i]); }
void bitNot(Byte* out, const Byte* a, std::size_t n) { for (std::size_t i = 0; i < n; ++i) out[i] = static_cast<Byte>(~a[i]); }
void bitSelect(Byte* out, const Byte* mask, const Byte* a, const Byte* b, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) out[i] = static_cast<Byte>((mask[i] & a[i]) | (~mask[i] & b[i]));
}
void blend(Byte* out, const Byte* a, const Byte* b, const Byte* mask, std::size_t n, unsigned ew) {
    check(n, ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const bool pick = (mask[i + ew - 1] & 0x80) != 0;
        const U64 v = load(pick ? b + i : a + i, ew);
        store(out + i, ew, v);
    }
}
void bitOp(Byte* out, const Byte* a, std::size_t n, unsigned ew, BitOp op) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const U64 v = load(a + i, ew);
        U64 r = 0;
        switch (op) {
            case BitOp::Popcount:
                for (unsigned k = 0; k < w; ++k) r += (v >> k) & 1;
                break;
            case BitOp::CountLeadingZeros: {
                unsigned k = 0;
                while (k < w && ((v >> (w - 1 - k)) & 1) == 0) ++k;
                r = k;
                break;
            }
            case BitOp::CountLeadingSigns: {
                const U64 sign = (v >> (w - 1)) & 1;
                unsigned k = 0;
                while (k + 1 < w && ((v >> (w - 2 - k)) & 1) == sign) ++k;
                r = k;
                break;
            }
            case BitOp::ReverseBits:
                for (unsigned k = 0; k < w; ++k) r |= ((v >> k) & 1) << (w - 1 - k);
                break;
            case BitOp::ReverseBytes:
                for (unsigned k = 0; k < ew; ++k) r |= ((v >> (8 * k)) & 0xff) << (8 * (ew - 1 - k));
                break;
        }
        store(out + i, ew, r);
    }
}

// ---- shifts
namespace {
U64 shiftLane(U64 v, unsigned ew, Shift shift, U64 count, Count rule) {
    const unsigned w = bitsOf(ew);
    const U64 mask = laneMask(ew);
    v &= mask;
    if (shift == Shift::RotateLeft) {
        const unsigned c = static_cast<unsigned>(count % w);
        return c == 0 ? v : ((v << c) | (v >> (w - c))) & mask;
    }
    if (rule == Count::Mask) count %= w;
    const bool all = count >= w;
    switch (shift) {
        case Shift::Left: return all ? 0 : (v << count) & mask;
        case Shift::RightLogical: return all ? 0 : v >> count;
        case Shift::RightArithmetic: {
            const I64 s = signExtend(v, ew);
            return static_cast<U64>(all ? (s < 0 ? I64{-1} : I64{0}) : (s >> count)) & mask;
        }
        default: return v;
    }
}
}  // namespace

void shiftImm(Byte* out, const Byte* a, std::size_t n, unsigned ew, Shift shift, std::uint64_t count, Count rule) {
    lanes1(out, a, n, ew, [&](U64 v) { return shiftLane(v, ew, shift, count, rule); });
}
void shiftVar(Byte* out, const Byte* a, const Byte* counts, std::size_t n, unsigned ew, Shift shift, Count rule) {
    check(n, ew);
    for (std::size_t i = 0; i < n; i += ew) store(out + i, ew, shiftLane(load(a + i, ew), ew, shift, load(counts + i, ew), rule));
}
void shiftNeon(Byte* out, const Byte* a, const Byte* counts, std::size_t n, unsigned ew, Sign sign, bool round, bool saturateLeft) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const int c = static_cast<std::int8_t>(counts[i]);
        const W128 x = laneValue(load(a + i, ew), ew, sign);
        W128 r;
        if (c >= 0) {
            if (static_cast<unsigned>(c) >= w) {   // everything shifted out: zero, or saturated for a non-zero lane
                store(out + i, ew, (saturateLeft && !isZero(x)) ? (isNeg(x) ? minOf(w, true).lo : maxOf(w, sign == Sign::Signed).lo) : 0);
                continue;
            }
            r = shl(x, static_cast<unsigned>(c));
            store(out + i, ew, saturateLeft ? saturate(r, w, sign == Sign::Signed) : wrapTo(r, w));
        } else {
            unsigned s = static_cast<unsigned>(-c);
            if (s > 127) s = 127;
            if (round) r = shrArith(add(x, shl(fromU(1), s - 1)), s);
            else r = shrArith(x, s);
            store(out + i, ew, wrapTo(r, w));
        }
    }
}
void shiftRightImm(Byte* out, const Byte* a, std::size_t n, unsigned ew, Sign sign, unsigned count, bool round) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    if (count > w) bad("pcode_vector: shift count past the lane");
    for (std::size_t i = 0; i < n; i += ew) {
        W128 x = laneValue(load(a + i, ew), ew, sign);
        if (round && count > 0) x = add(x, shl(fromU(1), count - 1));
        store(out + i, ew, wrapTo(shrArith(x, count), w));
    }
}
void shiftLeftSatImm(Byte* out, const Byte* a, std::size_t n, unsigned ew, Sign sign, unsigned count, bool toUnsigned) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    if (count >= w) bad("pcode_vector: shift count past the lane");
    for (std::size_t i = 0; i < n; i += ew) {
        const W128 x = shl(laneValue(load(a + i, ew), ew, sign), count);
        store(out + i, ew, saturate(x, w, sign == Sign::Signed && !toUnsigned));
    }
}
void shiftInsert(Byte* out, const Byte* dst, const Byte* src, std::size_t n, unsigned ew, bool left, unsigned count) {
    check(n, ew);
    const unsigned w = bitsOf(ew);
    if (count > w) bad("pcode_vector: shift count past the lane");
    const U64 full = laneMask(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const U64 d = load(dst + i, ew), s = load(src + i, ew);
        U64 mask, moved;
        if (left) {
            mask = count >= w ? 0 : (full << count) & full;
            moved = count >= w ? 0 : (s << count) & full;
        } else {
            mask = count >= w ? 0 : full >> count;
            moved = count >= w ? 0 : s >> count;
        }
        store(out + i, ew, (d & ~mask & full) | (moved & mask));
    }
}

// ---- width changes
namespace {
U64 narrowValue(const W128& v, unsigned wOut, Narrow mode) {
    switch (mode) {
        case Narrow::Truncate: return wrapTo(v, wOut);
        case Narrow::SignedToSigned: return saturate(v, wOut, true);
        case Narrow::SignedToUnsigned: return saturate(v, wOut, false);
        case Narrow::UnsignedToUnsigned: return saturate(v, wOut, false);
    }
    return 0;
}
}  // namespace

void narrow(Byte* out, const Byte* a, std::size_t nIn, unsigned ewIn, Narrow mode) {
    check(nIn, ewIn);
    if (ewIn < 2) bad("pcode_vector: nothing narrower than a byte");
    const unsigned ewOut = ewIn / 2;
    const Sign sign = (mode == Narrow::SignedToSigned || mode == Narrow::SignedToUnsigned) ? Sign::Signed : Sign::Unsigned;
    for (std::size_t i = 0, j = 0; i < nIn; i += ewIn, j += ewOut) store(out + j, ewOut, narrowValue(laneValue(load(a + i, ewIn), ewIn, sign), bitsOf(ewOut), mode));
}
void narrowShiftRight(Byte* out, const Byte* a, std::size_t nIn, unsigned ewIn, unsigned count, bool round, Narrow mode) {
    check(nIn, ewIn);
    if (ewIn < 2) bad("pcode_vector: nothing narrower than a byte");
    if (count < 1 || count > bitsOf(ewIn)) bad("pcode_vector: shift count past the lane");
    const unsigned ewOut = ewIn / 2;
    const Sign sign = (mode == Narrow::SignedToSigned || mode == Narrow::SignedToUnsigned) ? Sign::Signed : Sign::Unsigned;
    for (std::size_t i = 0, j = 0; i < nIn; i += ewIn, j += ewOut) {
        W128 x = laneValue(load(a + i, ewIn), ewIn, sign);
        if (round) x = add(x, shl(fromU(1), count - 1));
        store(out + j, ewOut, narrowValue(shrArith(x, count), bitsOf(ewOut), mode));
    }
}
void widen(Byte* out, const Byte* a, std::size_t nIn, unsigned ewIn, Sign sign) {
    check(nIn, ewIn);
    checkMax(ewIn, 4);
    const unsigned ewOut = ewIn * 2;
    for (std::size_t i = 0, j = 0; i < nIn; i += ewIn, j += ewOut) store(out + j, ewOut, laneValue(load(a + i, ewIn), ewIn, sign).lo);
}
namespace {
W128 product(U64 x, Sign sx, U64 y, Sign sy, unsigned ew) {
    if (sx == Sign::Signed && sy == Sign::Signed) return mulSS(signExtend(x, ew), signExtend(y, ew));
    if (sx == Sign::Unsigned && sy == Sign::Unsigned) return mulUU(x, y);
    if (sx == Sign::Signed) return mulSU(signExtend(x, ew), y);
    return mulSU(signExtend(y, ew), x);
}
}  // namespace
void mulWiden(Byte* out, const Byte* a, const Byte* b, std::size_t nIn, unsigned ewIn, Sign signA, Sign signB) {
    check(nIn, ewIn);
    checkMax(ewIn, 4);
    const unsigned ewOut = ewIn * 2;
    for (std::size_t i = 0, j = 0; i < nIn; i += ewIn, j += ewOut) store(out + j, ewOut, product(load(a + i, ewIn), signA, load(b + i, ewIn), signB, ewIn).lo);
}
void mulEvenOdd(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign, bool odd) {
    check(n, ew);
    checkMax(ew, 4);
    if (n % (2 * ew) != 0) bad("pcode_vector: an odd number of lanes");
    const unsigned ewOut = ew * 2;
    for (std::size_t i = odd ? ew : 0, j = 0; i < n; i += 2 * ew, j += ewOut) store(out + j, ewOut, product(load(a + i, ew), sign, load(b + i, ew), sign, ew).lo);
}
void multiplyAdd(Byte* out, const Byte* acc, const Byte* a, const Byte* b, std::size_t n, unsigned ewIn, unsigned ewOut, Sign signA, Sign signB, bool saturateSum) {
    check(n, ewIn);
    checkMax(ewIn, 4);
    if (ewOut < ewIn || ewOut > 8 || (ewOut != 2 && ewOut != 4 && ewOut != 8 && ewOut != 1) || n % ewOut != 0) bad("pcode_vector: bad accumulator lane");
    const unsigned group = ewOut / ewIn;
    const bool isSigned = signA == Sign::Signed || signB == Sign::Signed;
    const unsigned w = bitsOf(ewOut);
    for (std::size_t o = 0; o < n; o += ewOut) {
        W128 sum = acc ? laneValue(load(acc + o, ewOut), ewOut, isSigned ? Sign::Signed : Sign::Unsigned) : W128{};
        for (unsigned k = 0; k < group; ++k) {
            const std::size_t at = o / ewOut * group * ewIn + k * ewIn;
            sum = add(sum, product(load(a + at, ewIn), signA, load(b + at, ewIn), signB, ewIn));
        }
        store(out + o, ewOut, saturateSum ? saturate(sum, w, isSigned) : wrapTo(sum, w));
    }
}
void sumAbsDiff(Byte* out, const Byte* a, const Byte* b, std::size_t n) {
    if (n % 8 != 0 || n > kMaxBytes) bad("pcode_vector: psadbw takes groups of 8 bytes");
    for (std::size_t g = 0; g < n; g += 8) {
        U64 sum = 0;
        for (unsigned k = 0; k < 8; ++k) sum += a[g + k] > b[g + k] ? a[g + k] - b[g + k] : b[g + k] - a[g + k];
        store(out + g, 8, sum);
    }
}

// ---- across lanes
void pairwise(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Pair op) {
    check(n, ew);
    if (n % (2 * ew) != 0) bad("pcode_vector: an odd number of lanes");
    Byte tmp[kMaxBytes];
    const std::size_t half = n / 2;
    for (int part = 0; part < 2; ++part) {
        const Byte* src = part == 0 ? a : b;
        for (std::size_t i = 0, j = static_cast<std::size_t>(part) * half; i < n; i += 2 * ew, j += ew) {
            const U64 x = load(src + i, ew), y = load(src + i + ew, ew);
            U64 r = 0;
            switch (op) {
                case Pair::Add: r = x + y; break;
                case Pair::MinSigned: r = signExtend(x, ew) < signExtend(y, ew) ? x : y; break;
                case Pair::MinUnsigned: r = x < y ? x : y; break;
                case Pair::MaxSigned: r = signExtend(x, ew) < signExtend(y, ew) ? y : x; break;
                case Pair::MaxUnsigned: r = x < y ? y : x; break;
            }
            store(tmp + j, ew, r);
        }
    }
    std::memcpy(out, tmp, n);
}
void addLongPairwise(Byte* out, const Byte* acc, const Byte* a, std::size_t n, unsigned ewIn, Sign sign) {
    check(n, ewIn);
    checkMax(ewIn, 4);
    if (n % (2 * ewIn) != 0) bad("pcode_vector: an odd number of lanes");
    const unsigned ewOut = ewIn * 2;
    for (std::size_t i = 0, j = 0; i < n; i += 2 * ewIn, j += ewOut) {
        W128 s = add(laneValue(load(a + i, ewIn), ewIn, sign), laneValue(load(a + i + ewIn, ewIn), ewIn, sign));
        if (acc) s = add(s, fromU(load(acc + j, ewOut)));
        store(out + j, ewOut, wrapTo(s, bitsOf(ewOut)));
    }
}
std::uint64_t reduce(const Byte* a, std::size_t n, unsigned ew, Reduce op, Sign sign) {
    check(n, ew);
    if (n == 0) bad("pcode_vector: nothing to reduce");
    if (op == Reduce::AddLong) {
        checkMax(ew, 4);
        W128 s;
        for (std::size_t i = 0; i < n; i += ew) s = add(s, laneValue(load(a + i, ew), ew, sign));
        return s.lo;
    }
    U64 r = load(a, ew);
    for (std::size_t i = ew; i < n; i += ew) {
        const U64 v = load(a + i, ew);
        if (op == Reduce::Add) r = (r + v) & laneMask(ew);
        else {
            const bool less = sign == Sign::Signed ? signExtend(v, ew) < signExtend(r, ew) : v < r;
            if (op == Reduce::Min ? less : (!less && v != r)) r = v;
        }
    }
    return r;
}

// ---- moving lanes
void splat(Byte* out, std::size_t n, unsigned ew, std::uint64_t value) {
    check(n, ew);
    for (std::size_t i = 0; i < n; i += ew) store(out + i, ew, value);
}
void reverseLanes(Byte* out, const Byte* a, std::size_t n, unsigned ew) {
    check(n, ew);
    Byte tmp[kMaxBytes];
    for (std::size_t i = 0; i < n; i += ew) std::memcpy(tmp + (n - ew - i), a + i, ew);
    std::memcpy(out, tmp, n);
}
void reverseInGroups(Byte* out, const Byte* a, std::size_t n, unsigned ew, unsigned groupBytes) {
    check(n, ew);
    if (groupBytes == 0 || groupBytes % ew != 0 || n % groupBytes != 0) bad("pcode_vector: bad group");
    Byte tmp[kMaxBytes];
    for (std::size_t g = 0; g < n; g += groupBytes)
        for (std::size_t i = 0; i < groupBytes; i += ew) std::memcpy(tmp + g + (groupBytes - ew - i), a + g + i, ew);
    std::memcpy(out, tmp, n);
}
void zip(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, bool high) {
    check(n, ew);
    if (n % (2 * ew) != 0) bad("pcode_vector: an odd number of lanes");
    Byte tmp[kMaxBytes];
    const std::size_t base = high ? n / 2 : 0;
    for (std::size_t i = 0, j = 0; i < n / 2; i += ew, j += 2 * ew) {
        std::memcpy(tmp + j, a + base + i, ew);
        std::memcpy(tmp + j + ew, b + base + i, ew);
    }
    std::memcpy(out, tmp, n);
}
void unzip(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, bool odd) {
    check(n, ew);
    if (n % (2 * ew) != 0) bad("pcode_vector: an odd number of lanes");
    Byte tmp[kMaxBytes];
    const std::size_t half = n / 2;
    for (std::size_t i = odd ? ew : 0, j = 0; i < n; i += 2 * ew, j += ew) {
        std::memcpy(tmp + j, a + i, ew);
        std::memcpy(tmp + half + j, b + i, ew);
    }
    std::memcpy(out, tmp, n);
}
void transpose(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, bool odd) {
    check(n, ew);
    if (n % (2 * ew) != 0) bad("pcode_vector: an odd number of lanes");
    Byte tmp[kMaxBytes];
    for (std::size_t i = 0; i < n; i += 2 * ew) {
        const std::size_t from = odd ? i + ew : i;
        std::memcpy(tmp + i, a + from, ew);
        std::memcpy(tmp + i + ew, b + from, ew);
    }
    std::memcpy(out, tmp, n);
}
void extract(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned byteIndex) {
    if (n > kMaxBytes || byteIndex > n) bad("pcode_vector: bad extract");
    Byte tmp[2 * kMaxBytes];
    std::memcpy(tmp, a, n);
    std::memcpy(tmp + n, b, n);
    std::memcpy(out, tmp + byteIndex, n);
}
void shiftBytes(Byte* out, const Byte* a, std::size_t n, std::size_t groupBytes, unsigned count, bool left) {
    if (n > kMaxBytes || groupBytes == 0 || n % groupBytes != 0) bad("pcode_vector: bad byte shift");
    Byte tmp[kMaxBytes];
    for (std::size_t g = 0; g < n; g += groupBytes)
        for (std::size_t i = 0; i < groupBytes; ++i) {
            if (left) tmp[g + i] = i >= count ? a[g + i - count] : 0;
            else tmp[g + i] = i + count < groupBytes ? a[g + i + count] : 0;
        }
    std::memcpy(out, tmp, n);
}
void permuteLanes(Byte* out, const Byte* a, std::size_t n, unsigned ew, const Byte* indices, std::size_t count) {
    check(n, ew);
    Byte tmp[kMaxBytes];
    const std::size_t lanes = n / ew;
    for (std::size_t i = 0; i < count && (i + 1) * ew <= kMaxBytes; ++i) {
        if (indices[i] < lanes) std::memcpy(tmp + i * ew, a + indices[i] * ew, ew);
        else std::memset(tmp + i * ew, 0, ew);
    }
    std::memcpy(out, tmp, count * ew);
}
void tableLookup(Byte* out, const Byte* table, std::size_t tableBytes, const Byte* indices, std::size_t n, const Lookup& mode) {
    if (n > kMaxBytes || tableBytes > 1024) bad("pcode_vector: bad table lookup");
    Byte t[1024], idx[kMaxBytes], res[kMaxBytes];
    std::memcpy(t, table, tableBytes);
    std::memcpy(idx, indices, n);
    std::memcpy(res, out, n);
    for (std::size_t i = 0; i < n; ++i) {
        if (mode.bit7Zero && (idx[i] & 0x80)) {
            res[i] = 0;
            continue;
        }
        const unsigned k = idx[i] & mode.indexMask;
        std::size_t at;
        bool in;
        if (mode.groupBytes) {
            at = (i / mode.groupBytes) * mode.groupBytes + k;
            in = k < mode.groupBytes && at < tableBytes;
        } else {
            at = k;
            in = at < tableBytes;
        }
        if (in) res[i] = t[at];
        else if (!mode.keepOutOfRange) res[i] = 0;
    }
    std::memcpy(out, res, n);
}

// ---- bit fields and checks
std::uint64_t moveMask(const Byte* a, std::size_t n, unsigned ew) {
    check(n, ew);
    if (n / ew > 64) bad("pcode_vector: more than 64 lanes");
    U64 r = 0;
    for (std::size_t i = 0; i < n / ew; ++i) r |= static_cast<U64>((a[i * ew + ew - 1] >> 7) & 1) << i;
    return r;
}
namespace {
// the carry-less product of two 64 bit values: (hi, lo)
W128 clmul(U64 a, U64 b) {
    W128 r;
    for (unsigned i = 0; i < 64; ++i)
        if ((b >> i) & 1) {
            const W128 s = shl(fromU(a), i);
            r.lo ^= s.lo;
            r.hi ^= s.hi;
        }
    return r;
}
}  // namespace
void polyMul(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew) {
    lanes2(out, a, b, n, ew, [](U64 x, U64 y) { return clmul(x, y).lo; });
}
void polyMul64(Byte* out16, std::uint64_t a, std::uint64_t b) {
    const W128 r = clmul(a, b);
    store(out16, 8, r.lo);
    store(out16 + 8, 8, r.hi);
}
void polyMulWiden(Byte* out, const Byte* a, const Byte* b, std::size_t nIn, unsigned ewIn) {
    check(nIn, ewIn);
    checkMax(ewIn, 4);
    for (std::size_t i = 0, j = 0; i < nIn; i += ewIn, j += 2 * ewIn) store(out + j, 2 * ewIn, clmul(load(a + i, ewIn), load(b + i, ewIn)).lo);
}
std::uint32_t crc32Update(std::uint32_t acc, std::uint64_t data, unsigned size, bool castagnoli) {
    if (size != 1 && size != 2 && size != 4 && size != 8) bad("pcode_vector: crc32 takes 1, 2, 4 or 8 bytes");
    const std::uint32_t poly = castagnoli ? 0x82f63b78u : 0xedb88320u;   // the reflected polynomials
    for (unsigned i = 0; i < 8 * size; ++i) {
        const std::uint32_t bit = (acc ^ static_cast<std::uint32_t>(data >> i)) & 1u;
        acc = (acc >> 1) ^ (bit ? poly : 0u);
    }
    return acc;
}
void packPixel32(Byte* out, const Byte* a, const Byte* b, std::size_t n) {
    check(n, 4);
    if (2 * n > kMaxBytes) bad("pcode_vector: result too wide");
    Byte tmp[kMaxBytes];
    const std::size_t words = n / 4;
    for (std::size_t i = 0; i < 2 * words; ++i) {
        const U64 w = i < words ? load(a + 4 * i, 4) : load(b + 4 * (i - words), 4);
        const U64 px = (((w >> 24) & 1) << 15) | (((w >> 19) & 0x1f) << 10) | (((w >> 11) & 0x1f) << 5) | ((w >> 3) & 0x1f);
        store(tmp + 2 * i, 2, px);
    }
    std::memcpy(out, tmp, n);
}
void unpackPixel16(Byte* out, const Byte* a, std::size_t nIn) {
    check(nIn, 2);
    if (2 * nIn > kMaxBytes) bad("pcode_vector: result too wide");
    Byte tmp[kMaxBytes];
    for (std::size_t i = 0; i < nIn / 2; ++i) {
        const U64 h = load(a + 2 * i, 2);
        const U64 w = (((h >> 15) & 1) ? 0xff000000u : 0u) | (((h >> 10) & 0x1f) << 16) | (((h >> 5) & 0x1f) << 8) | (h & 0x1f);
        store(tmp + 4 * i, 4, w);
    }
    std::memcpy(out, tmp, 2 * nIn);
}

// ======================================================================================================================================
// Floating point: IEEE 754 binary16, binary32, binary64 (and bfloat16 for the conversions) rounded with integers.
// ======================================================================================================================================
namespace {

struct Fmt {
    unsigned ew;
    unsigned expBits, fracBits;
    int bias;
    U64 fracMask() const { return (U64{1} << fracBits) - 1; }
    U64 expMax() const { return (U64{1} << expBits) - 1; }
    U64 quietBit() const { return U64{1} << (fracBits - 1); }
    U64 signBit() const { return U64{1} << (expBits + fracBits); }
    int emin() const { return 1 - bias; }
};
const Fmt kHalf{2, 5, 10, 15}, kSingle{4, 8, 23, 127}, kDouble{8, 11, 52, 1023}, kBfloat{2, 8, 7, 127};
const Fmt& fmtOf(unsigned ew) {
    if (ew == 2) return kHalf;
    if (ew == 4) return kSingle;
    if (ew == 8) return kDouble;
    bad("pcode_vector: a float lane is 2, 4 or 8 bytes");
}
inline void checkFloat(std::size_t n, unsigned ew) {
    if ((ew != 2 && ew != 4 && ew != 8) || n % ew != 0 || n > kMaxBytes) bad("pcode_vector: float lane width or vector length not taken");
}

enum Cls { kZero, kFinite, kInf, kQNaN, kSNaN };
struct Un {
    Cls cls = kZero;
    bool sign = false;
    int e = 0;           // finite: the exponent of the leading bit
    U64 sig = 0;         // finite: the significand, its leading bit at bit 63
    U64 raw = 0;         // NaN: the bits as given
};

inline unsigned clz64(U64 v) {
    unsigned n = 0;
    if (v == 0) return 64;
    while ((v & (U64{1} << 63)) == 0) {
        v <<= 1;
        ++n;
    }
    return n;
}

Un unpack(const Fmt& f, U64 bits, const FpEnv& env, unsigned& flags) {
    Un u;
    u.sign = (bits & f.signBit()) != 0;
    const U64 exp = (bits >> f.fracBits) & f.expMax();
    const U64 frac = bits & f.fracMask();
    if (exp == f.expMax()) {
        if (frac == 0) u.cls = kInf;
        else {
            u.cls = (frac & f.quietBit()) ? kQNaN : kSNaN;
            u.raw = bits;
        }
        return u;
    }
    if (exp == 0) {
        if (frac == 0) {
            u.cls = kZero;
            return u;
        }
        flags |= kInputDenormal;
        if (env.flushInputs) {
            u.cls = kZero;
            return u;
        }
        const unsigned lz = clz64(frac);
        u.cls = kFinite;
        u.sig = frac << lz;
        u.e = static_cast<int>(63 - lz) + f.emin() - static_cast<int>(f.fracBits);
        return u;
    }
    u.cls = kFinite;
    u.sig = ((U64{1} << f.fracBits) | frac) << (63 - f.fracBits);
    u.e = static_cast<int>(exp) - f.bias;
    return u;
}

inline U64 packInf(const Fmt& f, bool sign) { return (sign ? f.signBit() : 0) | (f.expMax() << f.fracBits); }
inline U64 packZero(const Fmt& f, bool sign) { return sign ? f.signBit() : 0; }
inline U64 packMax(const Fmt& f, bool sign) { return (sign ? f.signBit() : 0) | ((f.expMax() - 1) << f.fracBits) | f.fracMask(); }
U64 defaultNan(const Fmt& f, const FpEnv& env) { return ((env.nan == NanRule::X86) ? f.signBit() : 0) | (f.expMax() << f.fracBits) | f.quietBit(); }

// The first NaN the machine's rule picks among the operands (a NaN is `kQNaN` or `kSNaN`); false when none is a NaN.
bool pickNan(const Fmt& f, const FpEnv& env, const Un* const* ops, int count, unsigned& flags, U64& result) {
    const Un* pick = nullptr;
    bool anySignaling = false;
    for (int i = 0; i < count; ++i)
        if (ops[i]->cls == kSNaN) anySignaling = true;
    bool any = false;
    for (int i = 0; i < count; ++i) any = any || ops[i]->cls == kQNaN || ops[i]->cls == kSNaN;
    if (!any) return false;
    if (anySignaling) flags |= kInvalid;
    if (env.nan == NanRule::Arm) {
        for (int i = 0; i < count && !pick; ++i)
            if (ops[i]->cls == kSNaN) pick = ops[i];
        for (int i = 0; i < count && !pick; ++i)
            if (ops[i]->cls == kQNaN) pick = ops[i];
        if (env.defaultNan) {
            result = defaultNan(f, env);
            return true;
        }
    } else {
        for (int i = 0; i < count && !pick; ++i)
            if (ops[i]->cls == kQNaN || ops[i]->cls == kSNaN) pick = ops[i];
    }
    result = pick->raw | f.quietBit();
    return true;
}
U64 invalidResult(const Fmt& f, const FpEnv& env, unsigned& flags) {
    flags |= kInvalid;
    return defaultNan(f, env);
}

// ---- rounding a value held as a 128 bit significand (leading bit at bit 127) and an exponent to the format
inline W128 jamRight(const W128& v, unsigned k) {   // v >> k with the bits lost ORed into the lowest bit
    if (k == 0) return v;
    if (k >= 128) return {isZero(v) ? 0 : U64{1}, 0};
    W128 r = shrLogical(v, k);
    const W128 back = shl(r, k);
    if (back.lo != v.lo || back.hi != v.hi) r.lo |= 1;
    return r;
}

U64 roundPack(const Fmt& f, const FpEnv& env, bool sign, int e, W128 sig, bool sticky, unsigned& flags) {
    if (sticky) sig.lo |= 1;
    const unsigned p = f.fracBits + 1;
    const int biased = e + f.bias;
    const bool tiny = biased < 1;
    if (tiny) {
        if (env.flushResults) {
            flags |= kUnderflow | (env.nan == NanRule::X86 ? kInexact : 0u);
            return packZero(f, sign);
        }
        sig = jamRight(sig, static_cast<unsigned>(1 - biased > 130 ? 130 : 1 - biased));
    }
    U64 mant = sig.hi >> (64 - p);
    const bool guard = ((sig.hi >> (63 - p)) & 1) != 0;
    const bool rest = (sig.hi & ((U64{1} << (63 - p)) - 1)) != 0 || sig.lo != 0;
    const bool inexact = guard || rest;
    bool inc = false;
    switch (env.round) {
        case Round::NearestEven: inc = guard && (rest || (mant & 1)); break;
        case Round::NearestAway: inc = guard; break;
        case Round::TowardZero: inc = false; break;
        case Round::TowardPositive: inc = !sign && inexact; break;
        case Round::TowardNegative: inc = sign && inexact; break;
    }
    if (inc) ++mant;
    if (inexact) flags |= kInexact;
    if (tiny && inexact) flags |= kUnderflow;
    U64 field;
    if (tiny) {
        return (sign ? f.signBit() : 0) | mant;   // a carry into the exponent field makes the smallest normal number
    }
    field = static_cast<U64>(biased);
    if (mant >> p) {   // rounding carried out of the significand
        mant >>= 1;
        ++field;
    }
    if (field >= f.expMax()) {
        flags |= kOverflow | kInexact;
        const bool toInf = env.round == Round::NearestEven || env.round == Round::NearestAway || (env.round == Round::TowardPositive && !sign) ||
                           (env.round == Round::TowardNegative && sign);
        return toInf ? packInf(f, sign) : packMax(f, sign);
    }
    return (sign ? f.signBit() : 0) | (field << f.fracBits) | (mant & f.fracMask());
}

// ---- multiplication
U64 mulBits(const Fmt& f, const FpEnv& env, U64 a, U64 b, unsigned& flags, bool extended = false) {
    const Un x = unpack(f, a, env, flags), y = unpack(f, b, env, flags);
    const Un* ops[2] = {&x, &y};
    U64 r;
    if (pickNan(f, env, ops, 2, flags, r)) return r;
    const bool sign = x.sign != y.sign;
    if ((x.cls == kInf && y.cls == kZero) || (x.cls == kZero && y.cls == kInf)) {
        if (extended) return (sign ? f.signBit() : 0) | (static_cast<U64>(f.bias + 1) << f.fracBits);   // +-2.0
        return invalidResult(f, env, flags);
    }
    if (x.cls == kInf || y.cls == kInf) return packInf(f, sign);
    if (x.cls == kZero || y.cls == kZero) return packZero(f, sign);
    W128 p = mulUU(x.sig, y.sig);
    int e = x.e + y.e;
    if (p.hi >> 63) ++e;
    else p = shl(p, 1);
    return roundPack(f, env, sign, e, p, false, flags);
}

// ---- 192 bit sums for add and fused multiply-add: three 64 bit words, w[2] the top
struct W192 {
    U64 w[3] = {0, 0, 0};
};
inline bool zero192(const W192& a) { return a.w[0] == 0 && a.w[1] == 0 && a.w[2] == 0; }
inline bool less192(const W192& a, const W192& b) {
    for (int i = 2; i >= 0; --i)
        if (a.w[i] != b.w[i]) return a.w[i] < b.w[i];
    return false;
}
inline W192 add192(const W192& a, const W192& b) {
    W192 r;
    U64 carry = 0;
    for (int i = 0; i < 3; ++i) {
        const U64 s = a.w[i] + b.w[i];
        const U64 c1 = s < a.w[i] ? 1 : 0;
        r.w[i] = s + carry;
        carry = c1 + (r.w[i] < s ? 1 : 0);
    }
    return r;
}
inline W192 sub192(const W192& a, const W192& b) {   // a >= b
    W192 r;
    U64 borrow = 0;
    for (int i = 0; i < 3; ++i) {
        const U64 d = a.w[i] - b.w[i];
        const U64 b1 = a.w[i] < b.w[i] ? 1 : 0;
        r.w[i] = d - borrow;
        borrow = b1 + (d < borrow ? 1 : 0);
    }
    return r;
}
inline W192 shl192(const W192& a, unsigned k) {   // k < 192
    W192 r;
    const unsigned ws = k / 64, bs = k % 64;
    for (int i = 2; i >= 0; --i) {
        const int src = i - static_cast<int>(ws);
        U64 v = 0;
        if (src >= 0) {
            v = a.w[src] << bs;
            if (bs && src >= 1) v |= a.w[src - 1] >> (64 - bs);
        }
        r.w[i] = v;
    }
    return r;
}
inline W192 jamShr192(const W192& a, unsigned k) {   // a >> k, the bits lost ORed into the lowest bit
    if (k == 0) return a;
    W192 r;
    if (k >= 192) {
        r.w[0] = zero192(a) ? 0 : 1;
        return r;
    }
    const unsigned ws = k / 64, bs = k % 64;
    bool lost = false;
    for (unsigned i = 0; i < ws; ++i) lost = lost || a.w[i] != 0;
    if (bs && (a.w[ws] & ((U64{1} << bs) - 1))) lost = true;
    for (unsigned i = 0; i < 3; ++i) {
        const unsigned src = i + ws;
        U64 v = 0;
        if (src < 3) {
            v = a.w[src] >> bs;
            if (bs && src + 1 < 3) v |= a.w[src + 1] << (64 - bs);
        }
        r.w[i] = v;
    }
    if (lost) r.w[0] |= 1;
    return r;
}
inline int msb192(const W192& a) {   // -1 for zero
    for (int i = 2; i >= 0; --i)
        if (a.w[i]) return i * 64 + 63 - static_cast<int>(clz64(a.w[i]));
    return -1;
}

// The sum of two values (each a 128 bit significand with its leading bit at bit 127, an exponent and a sign), rounded once: the core of add, subtract and fused multiply-add.
// value = sig / 2^127 * 2^e. Both must be non zero. `exactZeroNegative`: the sign of an exact zero sum of opposite signs.
U64 sumRound(const Fmt& f, const FpEnv& env, bool sx, int ex, const W128& x, bool sy, int ey, const W128& y, unsigned& flags) {
    W192 a, b;
    a.w[1] = x.lo, a.w[2] = x.hi;   // x << 64 ...
    b.w[1] = y.lo, b.w[2] = y.hi;
    a = jamShr192(a, 1);            // ... then one bit of headroom: the leading bit is bit 190
    b = jamShr192(b, 1);
    // a is the one with the larger exponent (or the larger magnitude at equal exponents)
    bool swap = ey > ex;
    if (ey == ex && less192(a, b)) swap = true;
    if (swap) {
        W192 t = a;
        a = b;
        b = t;
        const bool ts = sx;
        sx = sy;
        sy = ts;
        const int te = ex;
        ex = ey;
        ey = te;
    }
    const int d = ex - ey;
    b = jamShr192(b, static_cast<unsigned>(d > 200 ? 200 : d));
    W192 r;
    bool sign = sx;
    if (sx == sy) r = add192(a, b);
    else r = sub192(a, b);
    if (zero192(r)) return packZero(f, env.round == Round::TowardNegative);
    const int m = msb192(r);
    const int e = ex + (m - 190);
    W128 sig;
    if (m >= 127) {
        const W192 s = jamShr192(r, static_cast<unsigned>(m - 127));
        sig = {s.w[0], s.w[1]};
    } else {
        const W192 s = shl192(r, static_cast<unsigned>(127 - m));
        sig = {s.w[0], s.w[1]};
    }
    return roundPack(f, env, sign, e, sig, false, flags);
}

// a * b + c with one rounding. `negProduct` and `negAddend` flip those signs first (they do not change a NaN).
U64 fmaBits(const Fmt& f, const FpEnv& env, U64 abits, U64 bbits, U64 cbits, bool negProduct, bool negAddend, unsigned& flags) {
    const Un x = unpack(f, abits, env, flags), y = unpack(f, bbits, env, flags), z = unpack(f, cbits, env, flags);
    // the order a machine looks for a NaN in: ARM's FPMulAdd, the addend first; PowerPC, FRA then FRB (the addend) then FRC; x86 as passed (the table orders its 132, 213, 231 forms)
    const Un* ops[3] = {&x, &y, &z};
    if (env.nan == NanRule::Arm) ops[0] = &z, ops[1] = &x, ops[2] = &y;
    else if (env.nan == NanRule::Ppc) ops[1] = &z, ops[2] = &y;
    const bool productInvalid = (x.cls == kInf && y.cls == kZero) || (x.cls == kZero && y.cls == kInf);
    U64 r;
    if (env.nan == NanRule::Arm && productInvalid && z.cls == kQNaN && !(x.cls == kQNaN || x.cls == kSNaN || y.cls == kQNaN || y.cls == kSNaN)) {
        flags |= kInvalid;
        return defaultNan(f, env);
    }
    if (pickNan(f, env, ops, 3, flags, r)) return r;
    if (productInvalid) return invalidResult(f, env, flags);
    const bool sp = (x.sign != y.sign) != negProduct, sc = z.sign != negAddend;
    const bool productInf = x.cls == kInf || y.cls == kInf, productZero = x.cls == kZero || y.cls == kZero;
    if (productInf) {
        if (z.cls == kInf && sc != sp) return invalidResult(f, env, flags);
        return packInf(f, sp);
    }
    if (z.cls == kInf) return packInf(f, sc);
    if (productZero && z.cls == kZero) {
        if (sp == sc) return packZero(f, sp);
        return packZero(f, env.round == Round::TowardNegative);
    }
    if (productZero) {   // the addend, exactly (a denormal one is kept unless flushed on the way in)
        return roundPack(f, env, sc, z.e, {0, z.sig}, false, flags);
    }
    W128 p = mulUU(x.sig, y.sig);
    int ep = x.e + y.e;
    if (p.hi >> 63) ++ep;
    else p = shl(p, 1);
    if (z.cls == kZero) return roundPack(f, env, sp, ep, p, false, flags);
    return sumRound(f, env, sp, ep, p, sc, z.e, {0, z.sig}, flags);
}

U64 addBits(const Fmt& f, const FpEnv& env, U64 abits, U64 bbits, bool negateB, unsigned& flags) {
    const Un x = unpack(f, abits, env, flags);
    Un y = unpack(f, bbits, env, flags);
    const Un* ops[2] = {&x, &y};
    U64 r;
    if (pickNan(f, env, ops, 2, flags, r)) return r;
    if (negateB) y.sign = !y.sign;
    if (x.cls == kInf) {
        if (y.cls == kInf && y.sign != x.sign) return invalidResult(f, env, flags);
        return packInf(f, x.sign);
    }
    if (y.cls == kInf) return packInf(f, y.sign);
    if (x.cls == kZero && y.cls == kZero) return x.sign == y.sign ? packZero(f, x.sign) : packZero(f, env.round == Round::TowardNegative);
    if (x.cls == kZero) return roundPack(f, env, y.sign, y.e, {0, y.sig}, false, flags);
    if (y.cls == kZero) return roundPack(f, env, x.sign, x.e, {0, x.sig}, false, flags);
    return sumRound(f, env, x.sign, x.e, {0, x.sig}, y.sign, y.e, {0, y.sig}, flags);
}

// ---- division and square root
U64 divBits(const Fmt& f, const FpEnv& env, U64 abits, U64 bbits, unsigned& flags) {
    const Un x = unpack(f, abits, env, flags), y = unpack(f, bbits, env, flags);
    const Un* ops[2] = {&x, &y};
    U64 r;
    if (pickNan(f, env, ops, 2, flags, r)) return r;
    const bool sign = x.sign != y.sign;
    if ((x.cls == kInf && y.cls == kInf) || (x.cls == kZero && y.cls == kZero)) return invalidResult(f, env, flags);
    if (x.cls == kInf) return packInf(f, sign);
    if (y.cls == kInf || x.cls == kZero) return packZero(f, sign);
    if (y.cls == kZero) {
        flags |= kDivideByZero;
        return packInf(f, sign);
    }
    // q = floor(x.sig * 2^63 / y.sig) in (2^62, 2^64), by restoring division of the 128 bit dividend (its high word is below the divisor)
    U64 rem = x.sig >> 1, q = 0;
    const U64 lo = x.sig << 63;
    for (int i = 63; i >= 0; --i) {
        const bool carry = (rem >> 63) != 0;
        rem = (rem << 1) | ((lo >> i) & 1);
        q <<= 1;
        if (carry || rem >= y.sig) {
            rem -= y.sig;
            q |= 1;
        }
    }
    int e = x.e - y.e;   // q / 2^63 is the significand ratio, in (0.5, 2)
    W128 sig;
    if (q >> 63) sig = {0, q};
    else {
        sig = {0, q << 1};
        --e;
    }
    return roundPack(f, env, sign, e, sig, rem != 0, flags);
}

U64 sqrtBits(const Fmt& f, const FpEnv& env, U64 abits, unsigned& flags) {
    const Un x = unpack(f, abits, env, flags);
    const Un* ops[1] = {&x};
    U64 r;
    if (pickNan(f, env, ops, 1, flags, r)) return r;
    if (x.cls == kZero) return packZero(f, x.sign);
    if (x.sign) return invalidResult(f, env, flags);
    if (x.cls == kInf) return packInf(f, false);
    // value = S * 2^t with t even; N = S * 2^sh in [2^126, 2^128); root = isqrt(N) in [2^63, 2^64)
    int t = x.e - 63;
    W128 s = {x.sig, 0};
    if (t & 1) {
        s = shl(s, 1);
        --t;
    }
    unsigned sh;
    if (s.hi == 0) sh = 64;       // S < 2^64: N = S << 64
    else sh = 62;                 // S in [2^64, 2^65): N = S << 62
    W128 n = shl(s, sh);
    // bit by bit integer square root of the 128 bit n
    U64 root = 0;
    W128 rem = n;
    W128 bit = {0, U64{1} << 62};   // the highest power of 4 not above n
    while (lessUnsigned(n, bit)) bit = shrLogical(bit, 2);
    rem = n;
    W128 res = {0, 0};
    while (!isZero(bit)) {
        const W128 t2 = add(res, bit);
        if (!lessUnsigned(rem, t2)) {
            rem = sub(rem, t2);
            res = add(shrLogical(res, 1), bit);
        } else {
            res = shrLogical(res, 1);
        }
        bit = shrLogical(bit, 2);
    }
    root = res.lo;
    // root has its leading bit at 63 (n >= 2^126 gives root >= 2^63)
    const int e = (t - static_cast<int>(sh)) / 2 + 63;
    return roundPack(f, env, false, e, {0, root}, !isZero(rem), flags);
}

// ---- integral rounding of a finite value held as significand and exponent: the integer part after rounding by `mode` (as a 128 bit magnitude), and whether anything was lost
struct IntRound {
    W128 magnitude;
    bool inexact = false;
};
IntRound roundToIntegral(bool sign, int e, U64 sig, Round mode) {   // value = sig * 2^(e - 63), e <= 64
    IntRound r;
    const int fb = 63 - e;   // fraction bits of `sig`
    W128 whole;
    bool guard = false, rest = false;
    if (fb <= 0) {
        whole = shl(fromU(sig), static_cast<unsigned>(-fb));
    } else if (fb > 64) {
        whole = {0, 0};
        guard = false;
        rest = true;   // below one half: sig < 2^64 <= 2^(fb-1)
    } else {
        const W128 t = fromU(sig);
        whole = shrLogical(t, static_cast<unsigned>(fb));
        guard = ((sig >> (fb - 1)) & 1) != 0;
        rest = fb > 1 && (sig & ((U64{1} << (fb - 1)) - 1)) != 0;
        if (fb == 64) {   // sig >> 63 is the guard bit; whole is 0
            guard = (sig >> 63) != 0;
            rest = (sig & ((U64{1} << 63) - 1)) != 0;
        }
    }
    r.inexact = guard || rest;
    bool inc = false;
    switch (mode) {
        case Round::NearestEven: inc = guard && (rest || (whole.lo & 1)); break;
        case Round::NearestAway: inc = guard; break;
        case Round::TowardZero: inc = false; break;
        case Round::TowardPositive: inc = !sign && r.inexact; break;
        case Round::TowardNegative: inc = sign && r.inexact; break;
    }
    r.magnitude = inc ? add(whole, fromU(1)) : whole;
    return r;
}

// The float holding the integer magnitude `m` (non zero, below 2^128) with `sign`, then scaled by 2^-scale: rounded in env.round.
U64 magnitudeToFloat(const Fmt& f, const FpEnv& env, bool sign, const W128& m, int scale, unsigned& flags) {
    if (isZero(m)) return packZero(f, sign);
    unsigned top = 127;
    {
        W128 t = m;
        top = 0;
        while (!isZero(t)) {
            t = shrLogical(t, 1);
            ++top;
        }
        top -= 1;
    }
    const W128 sig = shl(m, 127 - top);
    return roundPack(f, env, sign, static_cast<int>(top) - scale, sig, false, flags);
}

U64 roundToIntBits(const Fmt& f, const FpEnv& env, U64 abits, bool exact, unsigned& flags) {
    const Un x = unpack(f, abits, env, flags);
    const Un* ops[1] = {&x};
    U64 r;
    if (pickNan(f, env, ops, 1, flags, r)) return r;
    if (x.cls == kZero) return packZero(f, x.sign);
    if (x.cls == kInf) return packInf(f, x.sign);
    if (x.e >= static_cast<int>(f.fracBits)) return abits;   // already integral
    const IntRound ir = roundToIntegral(x.sign, x.e, x.sig, env.round);
    if (ir.inexact && exact) flags |= kInexact;
    FpEnv exactEnv = env;
    exactEnv.round = Round::NearestEven;
    exactEnv.flushResults = false;
    unsigned scratch = 0;
    return magnitudeToFloat(f, exactEnv, x.sign, ir.magnitude, 0, scratch);
}

// ---- compare
// -1, 0, 1, 2 (unordered)
int ordering(const Un& x, const Un& y) {
    if (x.cls == kQNaN || x.cls == kSNaN || y.cls == kQNaN || y.cls == kSNaN) return 2;
    if (x.cls == kZero && y.cls == kZero) return 0;
    const auto key = [](const Un& u) {   // a signed comparison key of the finite and infinite values
        if (u.cls == kZero) return 0;
        return u.sign ? -1 : 1;
    };
    const int kx = key(x), ky = key(y);
    if (kx != ky) return kx < ky ? -1 : 1;
    // same sign (or one zero handled above): compare magnitudes
    int mag;
    if (x.cls == kInf || y.cls == kInf) mag = (x.cls == kInf ? 1 : 0) - (y.cls == kInf ? 1 : 0);
    else if (x.e != y.e) mag = x.e < y.e ? -1 : 1;
    else mag = x.sig == y.sig ? 0 : (x.sig < y.sig ? -1 : 1);
    return kx < 0 ? -mag : mag;
}

U64 minMaxBits(const Fmt& f, const FpEnv& env, U64 abits, U64 bbits, bool isMax, unsigned& flags) {
    const Un x = unpack(f, abits, env, flags), y = unpack(f, bbits, env, flags);
    const bool xNan = x.cls == kQNaN || x.cls == kSNaN, yNan = y.cls == kQNaN || y.cls == kSNaN;
    const auto asFlushed = [&](const Un& u, U64 bits) { return (u.cls == kZero) ? packZero(f, u.sign) : bits; };
    const Un* ops[2] = {&x, &y};
    U64 r;
    switch (env.minmax) {
        case MinMax::X86: {
            if (xNan || yNan) {
                flags |= kInvalid;
                return asFlushed(y, bbits);
            }
            const int o = ordering(x, y);
            if (o == 0) return asFlushed(y, bbits);   // equal (two zeros of either sign too): the second operand
            const bool pickA = isMax ? o > 0 : o < 0;
            return pickA ? asFlushed(x, abits) : asFlushed(y, bbits);
        }
        case MinMax::ArmNumber:
            if (x.cls == kQNaN && !yNan) return asFlushed(y, bbits);
            if (y.cls == kQNaN && !xNan) return asFlushed(x, abits);
            [[fallthrough]];
        case MinMax::ArmPropagate: {
            if (pickNan(f, env, ops, 2, flags, r)) return r;
            if (x.cls == kZero && y.cls == kZero && x.sign != y.sign) return packZero(f, isMax ? false : true);
            const int o = ordering(x, y);
            const bool pickA = isMax ? o >= 0 : o <= 0;
            return pickA ? asFlushed(x, abits) : asFlushed(y, bbits);
        }
        case MinMax::Ppc: {
            if (pickNan(f, env, ops, 2, flags, r)) return r;
            if (x.cls == kZero && y.cls == kZero && x.sign != y.sign) return packZero(f, isMax ? false : true);
            const int o = ordering(x, y);
            const bool pickA = isMax ? o >= 0 : o <= 0;
            return pickA ? asFlushed(x, abits) : asFlushed(y, bbits);
        }
    }
    return 0;
}

bool compareHolds(FpCmp cmp, int o) {
    switch (cmp) {
        case FpCmp::Eq: return o == 0;
        case FpCmp::Ne: return o != 0;
        case FpCmp::Lt: return o == -1;
        case FpCmp::Le: return o == -1 || o == 0;
        case FpCmp::Gt: return o == 1;
        case FpCmp::Ge: return o == 1 || o == 0;
        case FpCmp::Unordered: return o == 2;
        case FpCmp::Ordered: return o != 2;
    }
    return false;
}

U64 convertBits(const Fmt& from, const Fmt& to, const FpEnv& env, U64 abits, unsigned& flags) {
    const Un x = unpack(from, abits, env, flags);
    if (x.cls == kQNaN || x.cls == kSNaN) {
        if (x.cls == kSNaN) flags |= kInvalid;
        if (env.nan == NanRule::Arm && env.defaultNan) return defaultNan(to, env);
        const U64 frac = x.raw & from.fracMask();
        const U64 widened = to.fracBits >= from.fracBits ? frac << (to.fracBits - from.fracBits) : frac >> (from.fracBits - to.fracBits);
        return (x.sign ? to.signBit() : 0) | (to.expMax() << to.fracBits) | widened | to.quietBit();
    }
    if (x.cls == kZero) return packZero(to, x.sign);
    if (x.cls == kInf) return packInf(to, x.sign);
    return roundPack(to, env, x.sign, x.e, {0, x.sig}, false, flags);
}

}  // namespace

// ---- float lanes
void fp2(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Fp2 op, const FpEnv& env, unsigned& flags) {
    checkFloat(n, ew);
    const Fmt& f = fmtOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const U64 x = load(a + i, ew), y = load(b + i, ew);
        U64 r = 0;
        switch (op) {
            case Fp2::Add: r = addBits(f, env, x, y, false, flags); break;
            case Fp2::Sub: r = addBits(f, env, x, y, true, flags); break;
            case Fp2::Mul: r = mulBits(f, env, x, y, flags); break;
            case Fp2::MulExtended: r = mulBits(f, env, x, y, flags, true); break;
            case Fp2::Div: r = divBits(f, env, x, y, flags); break;
            case Fp2::Min: r = minMaxBits(f, env, x, y, false, flags); break;
            case Fp2::Max: r = minMaxBits(f, env, x, y, true, flags); break;
            case Fp2::AbsDiff: r = addBits(f, env, x, y, true, flags) & ~f.signBit(); break;
        }
        store(out + i, ew, r);
    }
}
void fp1(Byte* out, const Byte* a, std::size_t n, unsigned ew, Fp1 op, const FpEnv& env, unsigned& flags) {
    checkFloat(n, ew);
    const Fmt& f = fmtOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const U64 x = load(a + i, ew);
        U64 r = 0;
        switch (op) {
            case Fp1::Sqrt: r = sqrtBits(f, env, x, flags); break;
            case Fp1::Abs: r = x & ~f.signBit(); break;
            case Fp1::Neg: r = x ^ f.signBit(); break;
            case Fp1::RoundToInt: r = roundToIntBits(f, env, x, false, flags); break;
            case Fp1::RoundToIntExact: r = roundToIntBits(f, env, x, true, flags); break;
        }
        store(out + i, ew, r);
    }
}
void fp3(Byte* out, const Byte* a, const Byte* b, const Byte* c, std::size_t n, unsigned ew, Fp3 op, const FpEnv& env, unsigned& flags) {
    checkFloat(n, ew);
    const Fmt& f = fmtOf(ew);
    const bool negProduct = op == Fp3::NegMulAdd || op == Fp3::NegMulSub, negAddend = op == Fp3::MulSub || op == Fp3::NegMulSub;
    for (std::size_t i = 0; i < n; i += ew) store(out + i, ew, fmaBits(f, env, load(a + i, ew), load(b + i, ew), load(c + i, ew), negProduct, negAddend, flags));
}
void fpCompare(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, FpCmp cmp, bool signaling, const FpEnv& env, unsigned& flags) {
    checkFloat(n, ew);
    const Fmt& f = fmtOf(ew);
    for (std::size_t i = 0; i < n; i += ew) {
        const Un x = unpack(f, load(a + i, ew), env, flags), y = unpack(f, load(b + i, ew), env, flags);
        const int o = ordering(x, y);
        if (o == 2) {
            if (signaling || x.cls == kSNaN || y.cls == kSNaN) flags |= kInvalid;
        }
        store(out + i, ew, compareHolds(cmp, o) ? ~U64{0} : 0);
    }
}
void floatToInt(Byte* out, const Byte* a, std::size_t n, unsigned fEw, unsigned iEw, Sign sign, unsigned fracBits, IntOverflow outOfRange, const FpEnv& env, unsigned& flags) {
    checkFloat(n, fEw);
    if (iEw != 1 && iEw != 2 && iEw != 4 && iEw != 8) bad("pcode_vector: integer lane width not taken");
    const Fmt& f = fmtOf(fEw);
    const unsigned w = bitsOf(iEw);
    const bool isSigned = sign == Sign::Signed;
    const std::size_t lanes = n / fEw;
    for (std::size_t k = 0; k < lanes; ++k) {
        const Un x = unpack(f, load(a + k * fEw, fEw), env, flags);
        const U64 indefinite = isSigned ? (U64{1} << (w - 1)) : laneMask(iEw);
        U64 result = 0;
        bool invalid = false;
        if (x.cls == kQNaN || x.cls == kSNaN) {
            invalid = true;
            result = outOfRange == IntOverflow::Saturate ? 0 : indefinite;
        } else if (x.cls == kZero) {
            result = 0;
        } else {
            bool overflow = false;
            W128 mag;
            bool inexact = false;
            if (x.cls == kInf) overflow = true;
            else {
                const int e = x.e + static_cast<int>(fracBits);
                if (e > 64) overflow = true;
                else {
                    const IntRound ir = roundToIntegral(x.sign, e, x.sig, env.round);
                    mag = ir.magnitude;
                    inexact = ir.inexact;
                }
            }
            if (!overflow) {
                // range check of the magnitude
                const W128 limitPos = isSigned ? sub(shl(fromU(1), w - 1), fromU(1)) : sub(shl(fromU(1), w), fromU(1));
                const W128 limitNeg = isSigned ? shl(fromU(1), w - 1) : W128{0, 0};
                const W128 limit = x.sign ? limitNeg : limitPos;
                if (lessUnsigned(limit, mag)) overflow = true;
            }
            if (overflow) {
                invalid = true;
                if (outOfRange == IntOverflow::X86Indefinite) result = indefinite;
                else if (x.sign) result = isSigned ? (U64{1} << (w - 1)) : 0;
                else result = isSigned ? ((U64{1} << (w - 1)) - 1) : laneMask(iEw);
            } else {
                result = x.sign ? wrapTo(neg(mag), w) : mag.lo;
                if (inexact) flags |= kInexact;
            }
        }
        if (invalid) flags |= kInvalid;
        store(out + k * iEw, iEw, result & laneMask(iEw));
    }
}
void intToFloat(Byte* out, const Byte* a, std::size_t n, unsigned iEw, unsigned fEw, Sign sign, unsigned fracBits, const FpEnv& env, unsigned& flags) {
    check(n, iEw);
    if (fEw != 2 && fEw != 4 && fEw != 8) bad("pcode_vector: float lane width not taken");
    const Fmt& f = fmtOf(fEw);
    const std::size_t lanes = n / iEw;
    for (std::size_t k = 0; k < lanes; ++k) {
        const W128 v = laneValue(load(a + k * iEw, iEw), iEw, sign);
        const bool negative = isNeg(v);
        const W128 m = negative ? neg(v) : v;
        store(out + k * fEw, fEw, magnitudeToFloat(f, env, negative, m, static_cast<int>(fracBits), flags));
    }
}
void floatConvert(Byte* out, const Byte* a, std::size_t n, unsigned fromEw, unsigned toEw, const FpEnv& env, unsigned& flags) {
    checkFloat(n, fromEw);
    const Fmt& from = fmtOf(fromEw);
    const Fmt& to = fmtOf(toEw);
    const std::size_t lanes = n / fromEw;
    if (lanes * toEw > kMaxBytes) bad("pcode_vector: result too wide");
    Byte tmp[kMaxBytes];
    for (std::size_t k = 0; k < lanes; ++k) store(tmp + k * toEw, toEw, convertBits(from, to, env, load(a + k * fromEw, fromEw), flags));
    std::memcpy(out, tmp, lanes * toEw);
}
void bfloat16ToSingle(Byte* out, const Byte* a, std::size_t n) {
    check(n, 2);
    if (n * 2 > kMaxBytes) bad("pcode_vector: result too wide");
    Byte tmp[kMaxBytes];
    for (std::size_t k = 0; k * 2 < n; ++k) store(tmp + k * 4, 4, load(a + k * 2, 2) << 16);
    std::memcpy(out, tmp, n * 2);
}
void singleToBfloat16(Byte* out, const Byte* a, std::size_t n, const FpEnv& env, unsigned& flags) {
    checkFloat(n, 4);
    Byte tmp[kMaxBytes];
    for (std::size_t k = 0; k * 4 < n; ++k) store(tmp + k * 2, 2, convertBits(kSingle, kBfloat, env, load(a + k * 4, 4), flags));
    std::memcpy(out, tmp, n / 2);
}

}  // namespace cb::emulate::vec
