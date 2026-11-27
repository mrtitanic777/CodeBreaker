// dotnet/decompiler/util/decimal_arith.cpp -- .NET's arithmetic on util::Decimal (work in progress, built with CB_DOTNET_WIP; the
// declarations are in decimal.hpp).
#include <algorithm>
#include <bit>
#include <cmath>
#include <cstdint>

#include "dotnet/decompiler/util/arithmetic_error.hpp"
#include "dotnet/decompiler/util/decimal.hpp"

namespace cb::dotnet::decompiler::util {

// ---- .NET's arithmetic ---------------------------------------------------------------------------------------------------------------
//
// Ported from System.Private.CoreLib's Decimal.DecCalc.cs and Decimal.cs (.NET 9; MIT, .NET Foundation; see NOTICE in
// dotnet/decompiler). Upstream works on a DecCalc (flags, hi, lo, mid) and on little buffers (Buf12, Buf16, Buf24, Buf28: the words of a
// 96 to 224 bit integer, least significant first); here the buffers are arrays of 32-bit words and the functions that take a "Buf12 at
// word i" take a pointer to the word. The 64-bit (x64) code paths are the ones ported, as .NET 9 runs them on x64 and arm64.

namespace {

using u32 = std::uint32_t;
using u64 = std::uint64_t;

constexpr u32 kSignMask = 0x80000000u;
constexpr u32 kScaleMask = 0x00FF0000u;
constexpr int kScaleShift = 16;
constexpr int kDecScaleMax = 28;
constexpr u32 kTenToPowerNine = 1000000000u;
constexpr u64 kTenToPowerEighteen = 1000000000000000000ull;
constexpr int kMaxInt32Scale = 9;
constexpr int kMaxInt64Scale = 19;

constexpr u32 kUInt32Powers10[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

// 10^1 .. 10^19
constexpr u64 kUInt64Powers10[] = {10ull,
                                   100ull,
                                   1000ull,
                                   10000ull,
                                   100000ull,
                                   1000000ull,
                                   10000000ull,
                                   100000000ull,
                                   1000000000ull,
                                   10000000000ull,
                                   100000000000ull,
                                   1000000000000ull,
                                   10000000000000ull,
                                   100000000000000ull,
                                   1000000000000000ull,
                                   10000000000000000ull,
                                   100000000000000000ull,
                                   1000000000000000000ull,
                                   10000000000000000000ull};

constexpr double kDoublePowers10[] = {1,    1e1,  1e2,  1e3,  1e4,  1e5,  1e6,  1e7,  1e8,  1e9,  1e10, 1e11, 1e12, 1e13, 1e14,
                                      1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22, 1e23, 1e24, 1e25, 1e26, 1e27, 1e28, 1e29,
                                      1e30, 1e31, 1e32, 1e33, 1e34, 1e35, 1e36, 1e37, 1e38, 1e39, 1e40};

[[noreturn]] void throw_overflow() { throw OverflowError("Value was either too large or too small for a Decimal."); }

// A decimal as DecCalc holds it: the flags word (sign bit 31, scale bits 16 to 23), the high word, the low and the middle word.
struct Calc {
    u32 flags = 0, hi = 0, lo = 0, mid = 0;

    u64 low64() const { return (static_cast<u64>(mid) << 32) | lo; }
    void set_low64(u64 value) {
        lo = static_cast<u32>(value);
        mid = static_cast<u32>(value >> 32);
    }
    bool is_negative() const { return static_cast<std::int32_t>(flags) < 0; }
    int scale() const { return static_cast<std::uint8_t>(flags >> kScaleShift); }
};

Calc to_calc(const Decimal& d) {
    Calc c;
    c.flags = (d.negative ? kSignMask : 0u) | (static_cast<u32>(d.scale) << kScaleShift);
    c.hi = d.hi;
    c.lo = d.lo;
    c.mid = d.mid;
    return c;
}

Decimal from_calc(const Calc& c) {
    return Decimal(c.lo, c.mid, c.hi, (c.flags & kSignMask) != 0, static_cast<std::uint8_t>((c.flags & kScaleMask) >> kScaleShift));
}

// The 64-bit word at words p[0] (low) and p[1] (high).
u64 get64(const u32* p) { return (static_cast<u64>(p[1]) << 32) | p[0]; }
void set64(u32* p, u64 value) {
    p[0] = static_cast<u32>(value);
    p[1] = static_cast<u32>(value >> 32);
}

// Math.BigMul(uint, uint)
u64 big_mul(u32 a, u32 b) { return static_cast<u64>(a) * b; }

// UInt64x64To128 into a decimal's 96 bits: the product must fit.
void uint64x64_to_128(u64 a, u64 b, Calc& result) {
    const u64 a_lo = a & 0xFFFFFFFFu, a_hi = a >> 32, b_lo = b & 0xFFFFFFFFu, b_hi = b >> 32;
    const u64 ll = a_lo * b_lo, lh = a_lo * b_hi, hl = a_hi * b_lo, hh = a_hi * b_hi;
    const u64 mid_sum = (ll >> 32) + (lh & 0xFFFFFFFFu) + (hl & 0xFFFFFFFFu);
    const u64 low = (mid_sum << 32) | (ll & 0xFFFFFFFFu);
    const u64 high = hh + (lh >> 32) + (hl >> 32) + (mid_sum >> 32);
    if (high > 0xFFFFFFFFu) throw_overflow();
    result.set_low64(low);
    result.hi = static_cast<u32>(high);
}

// Buf12 (b[0..2]) / 32-bit divisor: the quotient replaces the dividend, the remainder is returned.
u32 div96_by_32(u32* b, u32 den) {
    u64 tmp, div;
    if (b[2] != 0) {
        tmp = get64(b + 1);
        div = tmp / den;
        set64(b + 1, div);
        tmp = ((tmp - static_cast<u32>(div) * den) << 32) | b[0];
        if (tmp == 0) return 0;
        const u32 div32 = static_cast<u32>(tmp / den);
        b[0] = div32;
        return static_cast<u32>(tmp) - div32 * den;
    }
    tmp = get64(b);
    if (tmp == 0) return 0;
    div = tmp / den;
    set64(b, div);
    return static_cast<u32>(tmp - div * den);
}

bool div96_by_const(u64& high64, u32& low, u32 pow) {
    const u64 div64 = high64 / pow;
    const u32 div = static_cast<u32>((((high64 - div64 * pow) << 32) + low) / pow);
    if (low == div * pow) {
        high64 = div64;
        low = div;
        return true;
    }
    return false;
}

// Removes the trailing zeros of the 96-bit value (low, high64) as far as the scale allows.
void unscale(u32& low, u64& high64, int& scale) {
    while (static_cast<std::uint8_t>(low) == 0 && scale >= 8 && div96_by_const(high64, low, 100000000)) scale -= 8;
    if ((low & 0xF) == 0 && scale >= 4 && div96_by_const(high64, low, 10000)) scale -= 4;
    if ((low & 3) == 0 && scale >= 2 && div96_by_const(high64, low, 100)) scale -= 2;
    if ((low & 1) == 0 && scale >= 1 && div96_by_const(high64, low, 10)) scale--;
}

// Buf12 b[0..2] / 64-bit divisor (den > high 64 bits of the dividend): the remainder replaces the low 64 bits, the quotient is returned.
u32 div96_by_64(u32* b, u64 den) {
    u64 num;
    const u32 num2 = b[2];
    if (num2 == 0) {
        num = get64(b);
        if (num < den) return 0;  // result is zero, the dividend is the remainder
        const u64 quo64 = num / den;
        set64(b, num % den);
        return static_cast<u32>(quo64);
    }

    u32 quo;
    const u32 den_high32 = static_cast<u32>(den >> 32);
    if (num2 >= den_high32) {
        // the divide would overflow: assume a quotient of 2^32 and set up the remainder accordingly
        num = get64(b);
        num -= den << 32;
        quo = 0;
        // the remainder went negative: add the divisor back in until it is positive, at most twice
        do {
            quo--;
            num += den;
        } while (num >= den);
        set64(b, num);
        return quo;
    }

    // the hardware divide does not overflow
    const u64 num64 = get64(b + 1);
    if (num64 < den_high32) return 0;  // result is zero

    quo = static_cast<u32>(num64 / den_high32);
    num = b[0] | ((num64 - quo * den_high32) << 32);  // the remainder

    // the full remainder: dividend - quo * divisor
    const u64 prod = big_mul(quo, static_cast<u32>(den));
    num -= prod;

    if (num > ~prod) {
        do {
            quo--;
            num += den;
        } while (num >= den);
    }
    set64(b, num);
    return quo;
}

// Buf16 b[0..3] / Buf12 divisor (den[2] > b[3]): the remainder replaces b[0..2], the quotient is returned.
u32 div128_by_96(u32* b, const u32* den_words) {
    u64 dividend = get64(b + 2);
    const u32 den = den_words[2];
    if (dividend < den) return 0;  // result is zero, the dividend is the remainder

    u32 quo = static_cast<u32>(dividend / den);
    u32 remainder = static_cast<u32>(dividend) - quo * den;

    // the full remainder: dividend - quo * divisor
    u64 prod1 = big_mul(quo, den_words[0]);
    u64 prod2 = big_mul(quo, den_words[1]);
    prod2 += prod1 >> 32;
    prod1 = static_cast<u32>(prod1) | (prod2 << 32);
    prod2 >>= 32;

    u64 num = get64(b);
    num -= prod1;
    remainder -= static_cast<u32>(prod2);

    // propagate the carries
    bool positive;
    if (num > ~prod1) {
        remainder--;
        positive = remainder < ~static_cast<u32>(prod2);
    } else {
        positive = remainder <= ~static_cast<u32>(prod2);
    }
    if (!positive) {
        // the remainder went negative: add the divisor back in until it is positive, at most twice
        prod1 = get64(den_words);
        while (true) {
            quo--;
            num += prod1;
            remainder += den;
            if (num < prod1) {
                // a carry: check for a carry out of the top before adding it in
                if (remainder++ < den) break;
            }
            if (remainder < den) break;  // a carry
        }
    }
    set64(b, num);
    b[2] = remainder;
    return quo;
}

// Buf12 *= power; returns the carry out of the top word.
u32 increase_scale(u32* b, u32 power) {
    u64 tmp = big_mul(b[0], power);
    b[0] = static_cast<u32>(tmp);
    tmp >>= 32;
    tmp += big_mul(b[1], power);
    b[1] = static_cast<u32>(tmp);
    tmp >>= 32;
    tmp += big_mul(b[2], power);
    b[2] = static_cast<u32>(tmp);
    return static_cast<u32>(tmp >> 32);
}

// the two low words times power into b[0..2] (the top word of the Buf12 becomes the carry)
void increase_scale64(u32* b, u32 power) {
    u64 tmp = big_mul(b[0], power);
    b[0] = static_cast<u32>(tmp);
    tmp >>= 32;
    tmp += big_mul(b[1], power);
    set64(b + 1, tmp);
}

// result[0..hi_res] / power in place; the quotient of the top word and the remainder are returned through the references.
u32 div_by_const(u32* result, u32 hi_res, u32& quotient, u32& remainder, u32 power) {
    const u32 high = result[hi_res];
    quotient = high / power;
    remainder = high % power;
    for (u32 i = hi_res - 1; static_cast<std::int32_t>(i) >= 0; i--) {
        const u64 num = result[i] + (static_cast<u64>(remainder) << 32);
        result[i] = static_cast<u32>(num / power);
        remainder = static_cast<u32>(num) - result[i] * power;
    }
    return power;
}

// Scales a product (result[0..hi_res], hi_res the index of the highest non-zero word, 0 to 5) so that it fits 96 bits and the scale is at
// most 28, rounding to even; returns the new scale. OverflowError if it cannot be done.
int scale_result(u32* result, u32 hi_res, int scale) {
    // a lower bound on the scaling needed to make the upper 96 bits zero
    int new_scale = 0;
    if (hi_res > 2) {
        new_scale = static_cast<int>(hi_res) * 32 - 64 - 1;
        new_scale -= std::countl_zero(result[hi_res]);
        // times log10(2), scaled by 256 (77/256), plus one
        new_scale = ((new_scale * 77) >> 8) + 1;
        if (new_scale > scale) throw_overflow();
    }

    // scale by enough to bring the scale factor into the valid range
    if (new_scale < scale - kDecScaleMax) new_scale = scale - kDecScaleMax;

    if (new_scale != 0) {
        // not guaranteed to bring the number within 96 bits: it could be one power of 10 short
        scale -= new_scale;
        u32 sticky = 0;
        u32 quotient = 0, remainder = 0;

        while (true) {
            sticky |= remainder;  // record the remainder as a sticky bit

            u32 power;
            switch (new_scale) {
                case 1: power = div_by_const(result, hi_res, quotient, remainder, 10); break;
                case 2: power = div_by_const(result, hi_res, quotient, remainder, 100); break;
                case 3: power = div_by_const(result, hi_res, quotient, remainder, 1000); break;
                case 4: power = div_by_const(result, hi_res, quotient, remainder, 10000); break;
                case 5: power = div_by_const(result, hi_res, quotient, remainder, 100000); break;
                case 6: power = div_by_const(result, hi_res, quotient, remainder, 1000000); break;
                case 7: power = div_by_const(result, hi_res, quotient, remainder, 10000000); break;
                case 8: power = div_by_const(result, hi_res, quotient, remainder, 100000000); break;
                default: power = div_by_const(result, hi_res, quotient, remainder, kTenToPowerNine); break;
            }
            result[hi_res] = quotient;
            // if the first quotient was 0, update hi_res
            if (quotient == 0 && hi_res != 0) hi_res--;

            new_scale -= kMaxInt32Scale;
            if (new_scale > 0) continue;  // scale some more

            // if it was scaled enough, hi_res is 2 or less; if not, divide by 10 more
            if (hi_res > 2) {
                if (scale == 0) throw_overflow();
                new_scale = 1;
                scale--;
                continue;
            }

            // round the final result: the remainder against half the divisor; at exactly half round up if odd or if the sticky bit is set
            power >>= 1;  // a power of 10 is always even
            if (power <= remainder && (power < remainder || ((result[0] & 1) | sticky) != 0) && ++result[0] == 0) {
                u32 cur = 0;
                do {
                } while (++result[++cur] == 0);

                if (cur > 2) {
                    // the rounding carried beyond 96 bits: scale by 10 more
                    if (scale == 0) throw_overflow();
                    hi_res = cur;
                    sticky = 0;     // no sticky bit
                    remainder = 0;  // or remainder
                    new_scale = 1;
                    scale--;
                    continue;
                }
            }
            break;
        }
    }
    return scale;
}

// Buf12 q = q / 10 after the carry out of 96 bits (the high bit loaded with a one), rounding by the last digit; returns scale - 1.
int overflow_unscale(u32* q, int scale, bool sticky) {
    if (--scale < 0) throw_overflow();

    // we have overflown: load the high bit with a one
    constexpr u64 highbit = 1ull << 32;
    q[2] = static_cast<u32>(highbit / 10);
    u64 tmp = ((highbit % 10) << 32) + q[1];
    u32 div = static_cast<u32>(tmp / 10);
    q[1] = div;
    tmp = ((tmp - div * 10) << 32) + q[0];
    div = static_cast<u32>(tmp / 10);
    q[0] = div;
    const u32 remainder = static_cast<u32>(tmp - div * 10);
    // the remainder is the last digit that does not fit: it decides the rounding
    if (remainder > 5 || (remainder == 5 && (sticky || (q[0] & 1) != 0))) {
        // Add32To96(ref bufQuo, 1)
        const u64 sum = get64(q) + 1;
        set64(q, sum);
        if (sum < 1) ++q[2];
    }
    return scale;
}

struct PowerOvfl {
    u32 hi;
    u64 mid_lo;
};

constexpr PowerOvfl kPowerOvflValues[] = {
    {429496729, (2576980377ull << 32) + 2576980377ull},  // 10^1
    {42949672, (4123168604ull << 32) + 687194767ull},    // 10^2
    {4294967, (1271310319ull << 32) + 2645699854ull},    // 10^3
    {429496, (3133608139ull << 32) + 694066715ull},      // 10^4
    {42949, (2890341191ull << 32) + 2216890319ull},      // 10^5
    {4294, (4154504685ull << 32) + 2369172679ull},       // 10^6
    {429, (2133437386ull << 32) + 4102387834ull},        // 10^7
    {42, (4078814305ull << 32) + 410238783ull},          // 10^8
};

// The power of 10 (0 to 9) the quotient in q (a Buf12) can be scaled by without overflowing 96 bits, within the scale limit.
int search_scale(const u32* q, int scale) {
    constexpr u32 ovfl_max_9_hi = 4;
    constexpr u32 ovfl_max_8_hi = 42;
    constexpr u32 ovfl_max_7_hi = 429;
    constexpr u32 ovfl_max_6_hi = 4294;
    constexpr u32 ovfl_max_5_hi = 42949;
    constexpr u32 ovfl_max_4_hi = 429496;
    constexpr u32 ovfl_max_3_hi = 4294967;
    constexpr u32 ovfl_max_2_hi = 42949672;
    constexpr u32 ovfl_max_1_hi = 429496729;
    constexpr u64 ovfl_max_9_midlo = 5441186219426131129ull;

    const u32 res_hi = q[2];
    const u64 res_mid_lo = get64(q);
    int cur_scale = 0;

    // a quick check to stop us from trying to scale any more
    if (res_hi > ovfl_max_1_hi) {
        // HaveScale
    } else {
        bool have_scale = false;
        if (scale > kDecScaleMax - 9) {
            // we cannot scale by 10^9 without exceeding the max scale factor: see if we can scale to the max
            cur_scale = kDecScaleMax - scale;
            if (res_hi < kPowerOvflValues[cur_scale - 1].hi) have_scale = true;
        } else if (res_hi < ovfl_max_9_hi || (res_hi == ovfl_max_9_hi && res_mid_lo <= ovfl_max_9_midlo)) {
            return 9;
        }

        if (!have_scale) {
            // a power to scale by below 9: a binary search
            if (res_hi > ovfl_max_5_hi) {
                if (res_hi > ovfl_max_3_hi) {
                    cur_scale = 2;
                    if (res_hi > ovfl_max_2_hi) cur_scale--;
                } else {
                    cur_scale = 4;
                    if (res_hi > ovfl_max_4_hi) cur_scale--;
                }
            } else {
                if (res_hi > ovfl_max_7_hi) {
                    cur_scale = 6;
                    if (res_hi > ovfl_max_6_hi) cur_scale--;
                } else {
                    cur_scale = 8;
                    if (res_hi > ovfl_max_8_hi) cur_scale--;
                }
            }

            // we already found we could not use the power one larger: if we can use this power it is the biggest; if not, the one below it
            // is correct unless it is 10^1 (we might have to go to 10^0)
            if (res_hi == kPowerOvflValues[cur_scale - 1].hi && res_mid_lo > kPowerOvflValues[cur_scale - 1].mid_lo) cur_scale--;
        }
    }

    // HaveScale: cur_scale is the largest power of 10 we can scale by without overflow (below 9); is it enough to make the scale positive?
    if (cur_scale + scale < 0) throw_overflow();
    return cur_scale;
}

// Buf12 += value; false on a carry out of 96 bits.
bool add32_to96(u32* b, u32 value) {
    const u64 sum = get64(b) + value;
    set64(b, sum);
    if (sum < value) {
        if (++b[2] == 0) return false;
    }
    return true;
}

// d1 = d1 + d2, or d1 - d2 when sign is set (DecAddSub).
void dec_add_sub(Calc& d1, Calc d2, bool sign) {
    u64 low64 = d1.low64();
    u32 high = d1.hi, flags = d1.flags;
    const u32 d2flags = d2.flags;

    const u32 xorflags = d2flags ^ flags;
    sign ^= (xorflags & kSignMask) != 0;

    // the variables the goto-structured code of upstream shares between its blocks
    int scale = 0;
    u32 power = 0;
    u64 tmp64 = 0, tmp_low = 0;
    u32 buf_num[6] = {0, 0, 0, 0, 0, 0};
    u32 hi_prod = 0;

    if ((xorflags & kScaleMask) == 0) {
        // the scale factors are equal: no alignment necessary
        goto AlignedAdd;
    } else {
        // the scale factors are not equal: assume that the larger scale factor (more decimal places) means that number is smaller; start by
        // guessing that the right operand has the larger scale factor; the result has the larger scale factor
        const u32 d1flags = flags;
        flags = (d2flags & kScaleMask) | (flags & kSignMask);  // the scale factor of the "smaller", but the sign of the "larger"
        scale = static_cast<std::int32_t>(flags - d1flags) >> kScaleShift;

        if (scale < 0) {
            // guessed the scale factor wrong: swap the operands
            scale = -scale;
            flags = d1flags;
            if (sign) flags ^= kSignMask;
            low64 = d2.low64();
            high = d2.hi;
            d2 = d1;
        }

        // d1 will need to be multiplied by 10^scale so it has the same scale as d2; that could extend it to up to 192 bits

        // scan for zeros in the upper words
        if (high == 0) {
            if (low64 <= 0xFFFFFFFFu) {
                if (static_cast<u32>(low64) == 0) {
                    // the left argument is zero: return the right
                    u32 sign_flags = flags & kSignMask;
                    if (sign) sign_flags ^= kSignMask;
                    d1 = d2;
                    d1.flags = (d2.flags & kScaleMask) | sign_flags;
                    return;
                }

                do {
                    if (scale <= kMaxInt32Scale) {
                        low64 = big_mul(static_cast<u32>(low64), kUInt32Powers10[scale]);
                        goto AlignedAdd;
                    }
                    scale -= kMaxInt32Scale;
                    low64 = big_mul(static_cast<u32>(low64), kTenToPowerNine);
                } while (low64 <= 0xFFFFFFFFu);
            }

            do {
                power = kTenToPowerNine;
                if (scale < kMaxInt32Scale) power = kUInt32Powers10[scale];
                tmp_low = big_mul(static_cast<u32>(low64), power);
                tmp64 = big_mul(static_cast<u32>(low64 >> 32), power) + (tmp_low >> 32);
                low64 = static_cast<u32>(tmp_low) + (tmp64 << 32);
                high = static_cast<u32>(tmp64 >> 32);
                if ((scale -= kMaxInt32Scale) <= 0) goto AlignedAdd;
            } while (high == 0);
        }

        while (true) {
            // scaling will not make it larger than 4 words
            power = kTenToPowerNine;
            if (scale < kMaxInt32Scale) power = kUInt32Powers10[scale];
            tmp_low = big_mul(static_cast<u32>(low64), power);
            tmp64 = big_mul(static_cast<u32>(low64 >> 32), power) + (tmp_low >> 32);
            low64 = static_cast<u32>(tmp_low) + (tmp64 << 32);
            tmp64 >>= 32;
            tmp64 += big_mul(high, power);

            scale -= kMaxInt32Scale;
            if (tmp64 > 0xFFFFFFFFu) break;

            high = static_cast<u32>(tmp64);
            // the result fits 96 bits: use the standard aligned add
            if (scale <= 0) goto AlignedAdd;
        }

        // we have to scale by a bunch: move the number to a buffer where it has room to grow as it is scaled
        set64(buf_num, low64);
        set64(buf_num + 2, tmp64);
        hi_prod = 3;

        // the scaling loop, up to 10^9 at a time; hi_prod stays updated with the index of the highest non-zero word
        for (; scale > 0; scale -= kMaxInt32Scale) {
            power = kTenToPowerNine;
            if (scale < kMaxInt32Scale) power = kUInt32Powers10[scale];
            tmp64 = 0;
            for (u32 cur = 0;;) {
                tmp64 += big_mul(buf_num[cur], power);
                buf_num[cur] = static_cast<u32>(tmp64);
                cur++;
                tmp64 >>= 32;
                if (cur > hi_prod) break;
            }

            if (static_cast<u32>(tmp64) != 0) {
                // we are extending the result by another word
                buf_num[++hi_prod] = static_cast<u32>(tmp64);
            }
        }

        // the scaling is complete: do the add (a subtract if the signs differ)
        tmp64 = get64(buf_num);
        low64 = d2.low64();
        const u32 tmp_high = buf_num[2];
        high = d2.hi;

        if (sign) {
            // the signs differ: subtract
            low64 = tmp64 - low64;
            high = tmp_high - high;

            // propagate the carry
            bool no_carry = false;
            if (low64 > tmp64) {
                high--;
                if (high < tmp_high) no_carry = true;
            } else if (high <= tmp_high) {
                no_carry = true;
            }

            if (!no_carry) {
                // carry the subtraction into the higher bits
                u32 cur = 3;
                do {
                } while (buf_num[cur++]-- == 0);
                if (buf_num[hi_prod] == 0 && --hi_prod <= 2) goto ReturnResult;
            }
        } else {
            // the signs are the same: add
            low64 += tmp64;
            high += tmp_high;

            // propagate the carry
            bool no_carry = false;
            if (low64 < tmp64) {
                high++;
                if (high > tmp_high) no_carry = true;
            } else if (high >= tmp_high) {
                no_carry = true;
            }

            if (!no_carry) {
                for (u32 cur = 3; ++buf_num[cur++] == 0;) {
                    if (hi_prod < cur) {
                        buf_num[cur] = 1;
                        hi_prod = cur;
                        break;
                    }
                }
            }
        }

        // NoCarry:
        set64(buf_num, low64);
        buf_num[2] = high;
        scale = scale_result(buf_num, hi_prod, static_cast<std::uint8_t>(flags >> kScaleShift));
        flags = (flags & ~kScaleMask) | (static_cast<u32>(scale) << kScaleShift);
        low64 = get64(buf_num);
        high = buf_num[2];
        goto ReturnResult;
    }

SignFlip : {
    // got a negative result: flip its sign
    flags ^= kSignMask;
    high = ~high;
    low64 = 0ull - low64;
    if (low64 == 0) high++;
    goto ReturnResult;
}

AlignedScale : {
    // the addition carried above 96 bits: divide the value by 10, dropping the scale factor
    if ((flags & kScaleMask) == 0) throw_overflow();
    flags -= 1u << kScaleShift;

    constexpr u32 den = 10;
    u64 num = high + (1ull << 32);
    high = static_cast<u32>(num / den);
    num = ((num - high * den) << 32) + (low64 >> 32);
    u32 div = static_cast<u32>(num / den);
    num = ((num - div * den) << 32) + static_cast<u32>(low64);
    low64 = div;
    low64 <<= 32;
    div = static_cast<u32>(num / den);
    low64 += div;
    div = static_cast<u32>(num) - div * den;

    // see if we need to round up
    if (div >= 5 && (div > 5 || (low64 & 1) != 0)) {
        if (++low64 == 0) high++;
    }
    goto ReturnResult;
}

AlignedAdd : {
    const u64 d1_low64 = low64;
    const u32 d1_high = high;
    if (sign) {
        // the signs differ: subtract
        low64 = d1_low64 - d2.low64();
        high = d1_high - d2.hi;

        // propagate the carry
        if (low64 > d1_low64) {
            high--;
            if (high >= d1_high) goto SignFlip;
        } else if (high > d1_high) {
            goto SignFlip;
        }
    } else {
        // the signs are the same: add
        low64 = d1_low64 + d2.low64();
        high = d1_high + d2.hi;

        // propagate the carry
        if (low64 < d1_low64) {
            high++;
            if (high <= d1_high) goto AlignedScale;
        } else if (high < d1_high) {
            goto AlignedScale;
        }
    }
    goto ReturnResult;
}

ReturnResult:
    d1.flags = flags;
    d1.hi = high;
    d1.set_low64(low64);
}

// Decimal.Compare of the magnitudes' relation when both are non-zero and have the same sign (VarDecCmpSub): the result is relative to the sign.
int var_dec_cmp_sub(const Calc& d1, const Calc& d2) {
    const std::int32_t flags = static_cast<std::int32_t>(d2.flags);
    int sign = (flags >> 31) | 1;
    std::int32_t scale = static_cast<std::int32_t>(d2.flags - d1.flags);

    u64 low64 = d1.low64();
    u32 high = d1.hi;

    u64 d2_low64 = d2.low64();
    u32 d2_high = d2.hi;

    if (scale != 0) {
        scale >>= kScaleShift;

        // the scale factors are not equal: assume the larger scale factor means the smaller number; guess the right operand has it
        if (scale < 0) {
            // guessed wrong: swap the operands
            scale = -scale;
            sign = -sign;

            const u64 tmp64 = low64;
            low64 = d2_low64;
            d2_low64 = tmp64;

            const u32 tmp = high;
            high = d2_high;
            d2_high = tmp;
        }

        // d1 will need to be multiplied by 10^scale so it has the same scale as d2: the scaling loop, up to 10^9 at a time
        do {
            const u32 power = scale >= kMaxInt32Scale ? kTenToPowerNine : kUInt32Powers10[scale];
            const u64 tmp_low = big_mul(static_cast<u32>(low64), power);
            u64 tmp = big_mul(static_cast<u32>(low64 >> 32), power) + (tmp_low >> 32);
            low64 = static_cast<u32>(tmp_low) + (tmp << 32);
            tmp >>= 32;
            tmp += big_mul(high, power);
            // if the scaled value has more than 96 significant bits it is greater than d2
            if (tmp > 0xFFFFFFFFu) return sign;
            high = static_cast<u32>(tmp);
        } while ((scale -= kMaxInt32Scale) > 0);
    }

    const u32 cmp_high = high - d2_high;
    if (cmp_high != 0) {
        // check for overflow
        if (cmp_high > high) sign = -sign;
        return sign;
    }

    const u64 cmp_low64 = low64 - d2_low64;
    if (cmp_low64 == 0) {
        sign = 0;
    } else if (cmp_low64 > low64) {
        // check for overflow
        sign = -sign;
    }
    return sign;
}

// VarDecCmp
int var_dec_cmp(const Calc& d1, const Calc& d2) {
    const std::int32_t f1 = static_cast<std::int32_t>(d1.flags), f2 = static_cast<std::int32_t>(d2.flags);
    if ((d2.low64() | d2.hi) == 0) {
        if ((d1.low64() | d1.hi) == 0) return 0;
        return (f1 >> 31) | 1;
    }
    if ((d1.low64() | d1.hi) == 0) return -((f2 >> 31) | 1);

    const int sign = (f1 >> 31) - (f2 >> 31);
    if (sign != 0) return sign;
    return var_dec_cmp_sub(d1, d2);
}

// d1 = d1 * d2 (VarDecMul)
void var_dec_mul(Calc& d1, const Calc& d2) {
    int scale = static_cast<std::uint8_t>((d1.flags + d2.flags) >> kScaleShift);

    u64 tmp;
    u32 hi_prod;
    u32 prod[6] = {0, 0, 0, 0, 0, 0};

    if ((d1.hi | d1.mid) == 0) {
        if ((d2.hi | d2.mid) == 0) {
            // the upper 64 bits are zero
            u64 low64 = big_mul(d1.lo, d2.lo);
            if (scale > kDecScaleMax) {
                // the result scale is too big: divide the result by a power of 10 to reduce it; if the amount to divide by is above 19 the
                // result is guaranteed less than 1/2 (the maximum value in 64 bits is 1.84E19)
                if (scale > kDecScaleMax + kMaxInt64Scale) {
                    d1 = Calc();
                    return;
                }

                scale -= kDecScaleMax + 1;
                u64 power = kUInt64Powers10[scale];

                const u64 remainder = low64 % power;
                low64 /= power;

                // round the result: the remainder against half the divisor (a power of 10 is always even)
                power >>= 1;
                if (remainder >= power && (remainder > power || (static_cast<u32>(low64) & 1) > 0)) low64++;

                scale = kDecScaleMax;
            }
            d1.set_low64(low64);
            d1.flags = ((d2.flags ^ d1.flags) & kSignMask) | (static_cast<u32>(scale) << kScaleShift);
            return;
        } else {
            // the left value is 32-bit: the result fits 4 words
            tmp = big_mul(d1.lo, d2.lo);
            prod[0] = static_cast<u32>(tmp);

            tmp = big_mul(d1.lo, d2.mid) + (tmp >> 32);
            prod[1] = static_cast<u32>(tmp);
            tmp >>= 32;

            if (d2.hi != 0) {
                tmp += big_mul(d1.lo, d2.hi);
                if (tmp > 0xFFFFFFFFu) {
                    set64(prod + 2, tmp);
                    hi_prod = 3;
                    goto SkipScan;
                }
            }
            prod[2] = static_cast<u32>(tmp);
            hi_prod = 2;
        }
    } else if ((d2.hi | d2.mid) == 0) {
        // the right value is 32-bit: the result fits 4 words
        tmp = big_mul(d2.lo, d1.lo);
        prod[0] = static_cast<u32>(tmp);

        tmp = big_mul(d2.lo, d1.mid) + (tmp >> 32);
        prod[1] = static_cast<u32>(tmp);
        tmp >>= 32;

        if (d1.hi != 0) {
            tmp += big_mul(d2.lo, d1.hi);
            if (tmp > 0xFFFFFFFFu) {
                set64(prod + 2, tmp);
                hi_prod = 3;
                goto SkipScan;
            }
        }
        prod[2] = static_cast<u32>(tmp);
        hi_prod = 2;
    } else {
        // both operands have bits set in the upper 64 bits: the 9 partial products accumulate into a 192-bit result
        tmp = big_mul(d1.lo, d2.lo);
        prod[0] = static_cast<u32>(tmp);

        u64 tmp2 = big_mul(d1.lo, d2.mid) + (tmp >> 32);

        tmp = big_mul(d1.mid, d2.lo);
        tmp += tmp2;  // this could generate a carry
        prod[1] = static_cast<u32>(tmp);
        if (tmp < tmp2) {  // detect the carry
            tmp2 = (tmp >> 32) | (1ull << 32);
        } else {
            tmp2 = tmp >> 32;
        }

        tmp = big_mul(d1.mid, d2.mid) + tmp2;

        if ((d1.hi | d2.hi) > 0) {
            // the highest 32 bits are non-zero: 5 more partial products
            tmp2 = big_mul(d1.lo, d2.hi);
            tmp += tmp2;  // this could generate a carry
            u32 tmp3 = 0;
            if (tmp < tmp2) tmp3 = 1;  // detect the carry

            tmp2 = big_mul(d1.hi, d2.lo);
            tmp += tmp2;  // this could generate a carry
            prod[2] = static_cast<u32>(tmp);
            if (tmp < tmp2) tmp3++;  // detect the carry
            tmp2 = (static_cast<u64>(tmp3) << 32) | (tmp >> 32);

            tmp = big_mul(d1.mid, d2.hi);
            tmp += tmp2;  // this could generate a carry
            tmp3 = 0;
            if (tmp < tmp2) tmp3 = 1;  // detect the carry

            tmp2 = big_mul(d1.hi, d2.mid);
            tmp += tmp2;  // this could generate a carry
            prod[3] = static_cast<u32>(tmp);
            if (tmp < tmp2) tmp3++;  // detect the carry
            tmp = (static_cast<u64>(tmp3) << 32) | (tmp >> 32);

            set64(prod + 4, big_mul(d1.hi, d2.hi) + tmp);

            hi_prod = 5;
        } else {
            set64(prod + 2, tmp);
            hi_prod = 3;
        }
    }

    // check for leading zero words on the product
    while (prod[hi_prod] == 0) {
        if (hi_prod == 0) {
            d1 = Calc();
            return;
        }
        hi_prod--;
    }

SkipScan:
    if (hi_prod > 2 || scale > kDecScaleMax) scale = scale_result(prod, hi_prod, scale);

    d1.set_low64(get64(prod));
    d1.hi = prod[2];
    d1.flags = ((d2.flags ^ d1.flags) & kSignMask) | (static_cast<u32>(scale) << kScaleShift);
}

// d1 = d1 / d2 (VarDecDiv)
void var_dec_div(Calc& d1, const Calc& d2) {
    u32 buf_quo[3] = {0, 0, 0};

    u32 power;
    int cur_scale;

    int scale = static_cast<std::int8_t>((d1.flags - d2.flags) >> kScaleShift);
    bool do_unscale = false;
    u32 tmp;

    if ((d2.hi | d2.mid) == 0) {
        // the divisor is only 32 bits: an easy divide
        const u32 den = d2.lo;
        if (den == 0) throw DivideByZeroError();

        set64(buf_quo, d1.low64());
        buf_quo[2] = d1.hi;
        u32 remainder = div96_by_32(buf_quo, den);

        while (true) {
            if (remainder == 0) {
                if (scale < 0) {
                    cur_scale = std::min(9, -scale);
                    goto HaveScale;
                }
                break;
            }

            // we need to unscale if and only if we have a non-zero remainder
            do_unscale = true;

            // we computed a quotient based on the natural scale (dividend scale - divisor scale); the remainder is non-zero, so increase the
            // scale if possible to include more quotient bits (scaling by 10^9 overflows if bufQuo[2].bufQuo[1] >= 2^32 / 10^9)
            if (scale == kDecScaleMax || (cur_scale = search_scale(buf_quo, scale)) == 0) {
                // no more scaling to be done, but the remainder is non-zero: round the quotient
                tmp = remainder << 1;
                if (tmp < remainder || (tmp >= den && (tmp > den || (buf_quo[0] & 1) != 0))) goto RoundUp;
                break;
            }

        HaveScale:
            power = kUInt32Powers10[cur_scale];
            scale += cur_scale;

            if (increase_scale(buf_quo, power) != 0) throw_overflow();

            {
                const u64 num = big_mul(remainder, power);
                const u32 div = static_cast<u32>(num / den);
                remainder = static_cast<u32>(num) - div * den;

                if (!add32_to96(buf_quo, div)) {
                    scale = overflow_unscale(buf_quo, scale, remainder != 0);
                    break;
                }
            }
        }
    } else {
        // the divisor has bits set in the upper 64 bits: it must be fully normalized (shifted so bit 31 of the most significant word is 1);
        // the dividend is shifted by the same amount so the quotient is not changed
        tmp = d2.hi;
        if (tmp == 0) tmp = d2.mid;

        cur_scale = std::countl_zero(tmp);

        // shift both the dividend and the divisor left by cur_scale
        u32 buf_rem[4] = {0, 0, 0, 0};

        set64(buf_rem, d1.low64() << cur_scale);
        set64(buf_rem + 2, (d1.mid + (static_cast<u64>(d1.hi) << 32)) >> (32 - cur_scale));

        const u64 divisor = d2.low64() << cur_scale;

        if (d2.hi == 0) {
            // a 64-bit divisor: the remainder (96 bits spread over 4 words) is below the divisor
            buf_quo[2] = 0;
            buf_quo[1] = div96_by_64(buf_rem + 1, divisor);
            buf_quo[0] = div96_by_64(buf_rem, divisor);

            while (true) {
                if (get64(buf_rem) == 0) {
                    if (scale < 0) {
                        cur_scale = std::min(9, -scale);
                        goto HaveScale64;
                    }
                    break;
                }

                // we need to unscale if and only if we have a non-zero remainder
                do_unscale = true;

                // the remainder is non-zero: scale up the quotient and the remainder by powers of 10 to compute more significant bits
                if (scale == kDecScaleMax || (cur_scale = search_scale(buf_quo, scale)) == 0) {
                    // no more scaling to be done, but the remainder is non-zero: round the quotient
                    u64 tmp64 = get64(buf_rem);
                    if (static_cast<std::int64_t>(tmp64) < 0 || (tmp64 <<= 1) > divisor || (tmp64 == divisor && (buf_quo[0] & 1) != 0)) goto RoundUp;
                    break;
                }

            HaveScale64:
                power = kUInt32Powers10[cur_scale];
                scale += cur_scale;

                if (increase_scale(buf_quo, power) != 0) throw_overflow();

                increase_scale64(buf_rem, power);
                tmp = div96_by_64(buf_rem, divisor);
                if (!add32_to96(buf_quo, tmp)) {
                    scale = overflow_unscale(buf_quo, scale, get64(buf_rem) != 0);
                    break;
                }
            }
        } else {
            // a 96-bit divisor: finish the shift left by cur_scale
            u32 buf_divisor[3] = {0, 0, 0};

            set64(buf_divisor, divisor);
            buf_divisor[2] = static_cast<u32>((d2.mid + (static_cast<u64>(d2.hi) << 32)) >> (32 - cur_scale));

            // the remainder (96 bits spread over 4 words) is below the divisor
            set64(buf_quo, div128_by_96(buf_rem, buf_divisor));
            buf_quo[2] = 0;

            while (true) {
                if ((get64(buf_rem) | buf_rem[2]) == 0) {
                    if (scale < 0) {
                        cur_scale = std::min(9, -scale);
                        goto HaveScale96;
                    }
                    break;
                }

                // we need to unscale if and only if we have a non-zero remainder
                do_unscale = true;

                // the remainder is non-zero: scale up the quotient and the remainder by powers of 10 to compute more significant bits
                if (scale == kDecScaleMax || (cur_scale = search_scale(buf_quo, scale)) == 0) {
                    // no more scaling to be done, but the remainder is non-zero: round the quotient
                    if (static_cast<std::int32_t>(buf_rem[2]) < 0) goto RoundUp;

                    tmp = buf_rem[1] >> 31;
                    set64(buf_rem, get64(buf_rem) << 1);
                    buf_rem[2] = (buf_rem[2] << 1) + tmp;

                    if (buf_rem[2] > buf_divisor[2] ||
                        (buf_rem[2] == buf_divisor[2] && (get64(buf_rem) > get64(buf_divisor) || (get64(buf_rem) == get64(buf_divisor) && (buf_quo[0] & 1) != 0))))
                        goto RoundUp;
                    break;
                }

            HaveScale96:
                power = kUInt32Powers10[cur_scale];
                scale += cur_scale;

                if (increase_scale(buf_quo, power) != 0) throw_overflow();

                buf_rem[3] = increase_scale(buf_rem, power);
                tmp = div128_by_96(buf_rem, buf_divisor);
                if (!add32_to96(buf_quo, tmp)) {
                    scale = overflow_unscale(buf_quo, scale, (get64(buf_rem) | get64(buf_rem + 2)) != 0);
                    break;
                }
            }
        }
    }

Unscale:
    if (do_unscale) {
        u32 low = buf_quo[0];
        u64 high64 = get64(buf_quo + 1);
        unscale(low, high64, scale);
        d1.lo = low;
        d1.mid = static_cast<u32>(high64);
        d1.hi = static_cast<u32>(high64 >> 32);
    } else {
        d1.set_low64(get64(buf_quo));
        d1.hi = buf_quo[2];
    }

    d1.flags = ((d1.flags ^ d2.flags) & kSignMask) | (static_cast<u32>(scale) << kScaleShift);
    return;

RoundUp : {
    const u64 rounded = get64(buf_quo) + 1;
    set64(buf_quo, rounded);
    if (rounded == 0 && ++buf_quo[2] == 0) scale = overflow_unscale(buf_quo, scale, true);
    goto Unscale;
}
}

// The remainder of a division with the divisor's scale and the dividend's scale not equal and the divisor above 64 bits (VarDecModFull).
void var_dec_mod_full(Calc& d1, const Calc& d2, int scale) {
    // the divisor has bits set in the upper 64 bits: it must be fully normalized (the top bit of its top word set); the dividend is shifted
    // by the same amount so the quotient is not changed
    u32 tmp = d2.hi;
    if (tmp == 0) tmp = d2.mid;
    const int shift = std::countl_zero(tmp);

    u32 b[7] = {0, 0, 0, 0, 0, 0, 0};

    set64(b, d1.low64() << shift);
    set64(b + 2, (d1.mid + (static_cast<u64>(d1.hi) << 32)) >> (32 - shift));

    // the dividend might need to be scaled up to 221 significant bits
    u32 high = 3;
    while (scale < 0) {
        const u32 power = scale <= -kMaxInt32Scale ? kTenToPowerNine : kUInt32Powers10[-scale];
        u64 tmp64 = big_mul(b[0], power);
        b[0] = static_cast<u32>(tmp64);
        for (u32 i = 1; i <= high; i++) {
            tmp64 >>= 32;
            tmp64 += big_mul(b[i], power);
            b[i] = static_cast<u32>(tmp64);
        }
        // the high bit of the dividend must not be set
        if (tmp64 > 0x7FFFFFFFu) b[++high] = static_cast<u32>(tmp64 >> 32);

        scale += kMaxInt32Scale;
    }

    if (d2.hi == 0) {
        const u64 divisor = d2.low64() << shift;
        switch (high) {
            case 6: div96_by_64(b + 4, divisor); [[fallthrough]];
            case 5: div96_by_64(b + 3, divisor); [[fallthrough]];
            case 4: div96_by_64(b + 2, divisor); break;
            default: break;
        }
        div96_by_64(b + 1, divisor);
        div96_by_64(b, divisor);

        d1.set_low64(get64(b) >> shift);
        d1.hi = 0;
    } else {
        u32 buf_divisor[3] = {0, 0, 0};

        set64(buf_divisor, d2.low64() << shift);
        buf_divisor[2] = static_cast<u32>((d2.mid + (static_cast<u64>(d2.hi) << 32)) >> (32 - shift));

        switch (high) {
            case 6: div128_by_96(b + 3, buf_divisor); [[fallthrough]];
            case 5: div128_by_96(b + 2, buf_divisor); [[fallthrough]];
            case 4: div128_by_96(b + 1, buf_divisor); break;
            default: break;
        }
        div128_by_96(b, buf_divisor);

        d1.set_low64((get64(b) >> shift) + ((static_cast<u64>(b[2]) << (32 - shift)) << 32));
        d1.hi = b[2] >> shift;
    }
}

// d1 = d1 % d2 (VarDecMod)
void var_dec_mod(Calc& d1, Calc d2) {
    if ((d2.lo | d2.mid | d2.hi) == 0) throw DivideByZeroError();

    if ((d1.lo | d1.mid | d1.hi) == 0) return;

    // in x % y the sign of y does not matter: the result has the sign of x
    d2.flags = (d2.flags & ~kSignMask) | (d1.flags & kSignMask);

    const int cmp = var_dec_cmp_sub(d1, d2);
    if (cmp == 0) {
        d1.lo = 0;
        d1.mid = 0;
        d1.hi = 0;
        if (d2.flags > d1.flags) d1.flags = d2.flags;
        return;
    }
    if ((cmp ^ static_cast<std::int32_t>(d1.flags & kSignMask)) < 0) return;

    // the divisor is smaller than the dividend and both are non-zero: calculate the integer remainder using the larger scaling factor
    int scale = static_cast<std::int8_t>((d1.flags - d2.flags) >> kScaleShift);
    if (scale > 0) {
        // the divisor's scale can always be increased to the dividend's scale for the remainder calculation
        do {
            const u32 power = scale >= kMaxInt32Scale ? kTenToPowerNine : kUInt32Powers10[scale];
            u64 tmp = big_mul(d2.lo, power);
            d2.lo = static_cast<u32>(tmp);
            tmp >>= 32;
            tmp += (d2.mid + (static_cast<u64>(d2.hi) << 32)) * power;
            d2.mid = static_cast<u32>(tmp);
            d2.hi = static_cast<u32>(tmp >> 32);
        } while ((scale -= kMaxInt32Scale) > 0);
        scale = 0;
    }

    do {
        if (scale < 0) {
            d1.flags = d2.flags;
            // try to scale up the dividend to match the divisor
            u32 buf_quo[3] = {0, 0, 0};

            set64(buf_quo, d1.low64());
            buf_quo[2] = d1.hi;
            do {
                const int i_cur_scale = search_scale(buf_quo, kDecScaleMax + scale);
                if (i_cur_scale == 0) break;
                const u32 power = i_cur_scale >= kMaxInt32Scale ? kTenToPowerNine : kUInt32Powers10[i_cur_scale];
                scale += i_cur_scale;
                u64 tmp = big_mul(buf_quo[0], power);
                buf_quo[0] = static_cast<u32>(tmp);
                tmp >>= 32;
                set64(buf_quo + 1, tmp + get64(buf_quo + 1) * power);
                if (power != kTenToPowerNine) break;
            } while (scale < 0);
            d1.set_low64(get64(buf_quo));
            d1.hi = buf_quo[2];
        }

        if (d1.hi == 0) {
            d1.set_low64(d1.low64() % d2.low64());
            return;
        } else if ((d2.hi | d2.mid) == 0) {
            const u32 den = d2.lo;
            u64 tmp = (static_cast<u64>(d1.hi) << 32) | d1.mid;
            tmp = ((tmp % den) << 32) | d1.lo;
            d1.set_low64(tmp % den);
            d1.hi = 0;
        } else {
            var_dec_mod_full(d1, d2, scale);
            return;
        }
    } while (scale < 0);
}

// How InternalRound treats the digits it cuts off (System.MidpointRounding); only ToZero is used here (Truncate).
enum class Midpoint { ToEven, AwayFromZero, ToZero, ToNegativeInfinity, ToPositiveInfinity };

// Decimal.DecCalc.InternalRound: rounds in place, removing `scale` digits.
void internal_round(Calc& d, u32 scale, Midpoint mode) {
    // the scale becomes the desired decimal count
    d.flags -= scale << kScaleShift;

    u32 remainder = 0, sticky = 0, power = 0;
    // first divide the value by the constant 10^9 up to three times
    while (scale >= static_cast<u32>(kMaxInt32Scale)) {
        scale -= kMaxInt32Scale;

        constexpr u32 divisor = kTenToPowerNine;
        u32 n = d.hi;
        if (n == 0) {
            const u64 tmp = d.low64();
            const u64 div = tmp / divisor;
            d.set_low64(div);
            remainder = static_cast<u32>(tmp - div * divisor);
        } else {
            u32 q;
            d.hi = n / divisor;
            remainder = n % divisor;
            n = d.mid;
            if ((n | remainder) != 0) {
                d.mid = q = static_cast<u32>(((static_cast<u64>(remainder) << 32) | n) / divisor);
                remainder = n - q * divisor;
            }
            n = d.lo;
            if ((n | remainder) != 0) {
                d.lo = q = static_cast<u32>(((static_cast<u64>(remainder) << 32) | n) / divisor);
                remainder = n - q * divisor;
            }
        }
        power = divisor;
        if (scale == 0) goto checkRemainder;
        sticky |= remainder;
    }

    {
        power = kUInt32Powers10[scale];
        u32 n = d.hi;
        if (n == 0) {
            const u64 tmp = d.low64();
            if (tmp == 0) {
                if (mode <= Midpoint::ToZero) goto done;
                remainder = 0;
                goto checkRemainder;
            }
            const u64 div = tmp / power;
            d.set_low64(div);
            remainder = static_cast<u32>(tmp - div * power);
        } else {
            u32 q;
            d.hi = n / power;
            remainder = n % power;
            n = d.mid;
            if ((n | remainder) != 0) {
                d.mid = q = static_cast<u32>(((static_cast<u64>(remainder) << 32) | n) / power);
                remainder = n - q * power;
            }
            n = d.lo;
            if ((n | remainder) != 0) {
                d.lo = q = static_cast<u32>(((static_cast<u64>(remainder) << 32) | n) / power);
                remainder = n - q * power;
            }
        }
    }

checkRemainder:
    if (mode == Midpoint::ToZero) {
        goto done;
    } else if (mode == Midpoint::ToEven) {
        // IEEE rounding: add the LSB of the result to the sticky bits so either causes a round up if remainder * 2 == the last divisor
        remainder <<= 1;
        if ((sticky | (d.lo & 1)) != 0) remainder++;
        if (power >= remainder) goto done;
    } else if (mode == Midpoint::AwayFromZero) {
        // round away from zero at the midpoint
        remainder <<= 1;
        if (power > remainder) goto done;
    } else if (mode == Midpoint::ToNegativeInfinity) {
        // round toward -infinity if a non-zero amount was chopped off a negative value
        if ((remainder | sticky) == 0 || !d.is_negative()) goto done;
    } else {
        // round toward +infinity if a non-zero amount was chopped off a positive value
        if ((remainder | sticky) == 0 || d.is_negative()) goto done;
    }
    {
        const u64 rounded = d.low64() + 1;
        d.set_low64(rounded);
        if (rounded == 0) d.hi++;
    }
done:
    return;
}

// Decimal.Truncate
void truncate_calc(Calc& d) {
    if ((d.flags & kScaleMask) != 0) internal_round(d, static_cast<std::uint8_t>(d.flags >> kScaleShift), Midpoint::ToZero);
}

// VarDecFromR4: (decimal)float.
Calc var_dec_from_r4(float input) {
    Calc result;

    // the most we can scale by is 10^28, just slightly more than 2^93: a float with an exponent of -94 could just barely reach 0.5; smaller
    // exponents always round to zero
    constexpr u32 sngbias = 126;
    const u32 bits = std::bit_cast<u32>(input);
    const int exp = static_cast<std::int32_t>(static_cast<std::uint8_t>(bits >> 23) - sngbias);
    if (exp < -94) return result;  // zero

    if (exp > 96) throw_overflow();

    u32 flags = 0;
    if (input < 0) {
        input = -input;
        flags = kSignMask;
    }

    // round the input to a 7-digit integer: R4 has only 7 digits of precision, and garbage digits stay out of the decimal; the max power of 10
    // is the exponent times log10(2) (scaled by 2^16: 19728)
    double dbl = input;
    int power = 6 - ((exp * 19728) >> 16);
    // power is between -22 and 35

    if (power >= 0) {
        // fewer than 7 digits: scale the input up
        if (power > kDecScaleMax) power = kDecScaleMax;

        dbl *= kDoublePowers10[power];
    } else {
        if (power != -1 || dbl >= 1E7) {
            dbl /= kDoublePowers10[-power];
        } else {
            power = 0;  // not scaled
        }
    }

    if (dbl < 1E6 && power < kDecScaleMax) {
        dbl *= 10;
        power++;
    }

    // round to an integer (to even)
    u32 mant = static_cast<u32>(static_cast<std::int32_t>(std::nearbyint(dbl)));

    if (mant == 0) return result;  // zero

    if (power < 0) {
        // add -power factors of 10, -power <= (29 - 7) = 22
        power = -power;
        if (power < 10) {
            result.set_low64(big_mul(mant, kUInt32Powers10[power]));
        } else {
            // a big power of 10
            if (power > 18) {
                const u64 low64 = big_mul(mant, kUInt32Powers10[power - 18]);
                uint64x64_to_128(low64, kTenToPowerEighteen, result);
            } else {
                u64 low64 = big_mul(mant, kUInt32Powers10[power - 9]);
                u64 hi64 = big_mul(kTenToPowerNine, static_cast<u32>(low64 >> 32));
                low64 = big_mul(kTenToPowerNine, static_cast<u32>(low64));
                result.lo = static_cast<u32>(low64);
                hi64 += low64 >> 32;
                result.mid = static_cast<u32>(hi64);
                hi64 >>= 32;
                result.hi = static_cast<u32>(hi64);
            }
        }
    } else {
        // factor out powers of 10 to reduce the scale if possible: at most 6 (a 7-digit number with a non-zero most significant digit), and the
        // scale is never negative so no more than the power used to get the integer
        int lmax = power;
        if (lmax > 6) lmax = 6;

        if ((mant & 0xF) == 0 && lmax >= 4) {
            const u32 div = mant / 10000, rem = mant % 10000;
            if (rem == 0) {
                mant = div;
                power -= 4;
                lmax -= 4;
            }
        }

        if ((mant & 3) == 0 && lmax >= 2) {
            const u32 div = mant / 100, rem = mant % 100;
            if (rem == 0) {
                mant = div;
                power -= 2;
                lmax -= 2;
            }
        }

        if ((mant & 1) == 0 && lmax >= 1) {
            const u32 div = mant / 10, rem = mant % 10;
            if (rem == 0) {
                mant = div;
                power--;
            }
        }

        flags |= static_cast<u32>(power) << kScaleShift;
        result.lo = mant;
    }

    result.flags = flags;
    return result;
}

// VarDecFromR8: (decimal)double.
Calc var_dec_from_r8(double input) {
    Calc result;

    constexpr u32 dblbias = 1022;
    const u64 bits = std::bit_cast<u64>(input);
    const int exp = static_cast<std::int32_t>(static_cast<u32>((bits >> 52) & 0x7FFu) - dblbias);
    if (exp < -94) return result;  // zero

    if (exp > 96) throw_overflow();

    u32 flags = 0;
    if (input < 0) {
        input = -input;
        flags = kSignMask;
    }

    // round the input to a 15-digit integer: R8 has only 15 digits of precision
    double dbl = input;
    int power = 14 - ((exp * 19728) >> 16);
    // power is between -14 and 43

    if (power >= 0) {
        // fewer than 15 digits: scale the input up
        if (power > kDecScaleMax) power = kDecScaleMax;

        dbl *= kDoublePowers10[power];
    } else {
        if (power != -1 || dbl >= 1E15) {
            dbl /= kDoublePowers10[-power];
        } else {
            power = 0;  // not scaled
        }
    }

    if (dbl < 1E14 && power < kDecScaleMax) {
        dbl *= 10;
        power++;
    }

    // round to int64 (to even)
    u64 mant = static_cast<u64>(static_cast<std::int64_t>(std::nearbyint(dbl)));

    if (mant == 0) return result;  // zero

    if (power < 0) {
        // add -power factors of 10, -power <= (29 - 15) = 14
        power = -power;
        if (power < 10) {
            const u32 pow10 = kUInt32Powers10[power];
            const u64 low64 = big_mul(static_cast<u32>(mant), pow10);
            u64 hi64 = big_mul(static_cast<u32>(mant >> 32), pow10);
            result.lo = static_cast<u32>(low64);
            hi64 += low64 >> 32;
            result.mid = static_cast<u32>(hi64);
            hi64 >>= 32;
            result.hi = static_cast<u32>(hi64);
        } else {
            // a big power of 10
            uint64x64_to_128(mant, kUInt64Powers10[power - 1], result);
        }
    } else {
        // factor out powers of 10 to reduce the scale if possible: at most 14 (a 15-digit number with a non-zero most significant digit)
        int lmax = power;
        if (lmax > 14) lmax = 14;

        if (static_cast<std::uint8_t>(mant) == 0 && lmax >= 8) {
            constexpr u32 den = 100000000;
            const u64 div = mant / den;
            if (static_cast<u32>(mant) == static_cast<u32>(div * den)) {
                mant = div;
                power -= 8;
                lmax -= 8;
            }
        }

        if ((static_cast<u32>(mant) & 0xF) == 0 && lmax >= 4) {
            constexpr u32 den = 10000;
            const u64 div = mant / den;
            if (static_cast<u32>(mant) == static_cast<u32>(div * den)) {
                mant = div;
                power -= 4;
                lmax -= 4;
            }
        }

        if ((static_cast<u32>(mant) & 3) == 0 && lmax >= 2) {
            constexpr u32 den = 100;
            const u64 div = mant / den;
            if (static_cast<u32>(mant) == static_cast<u32>(div * den)) {
                mant = div;
                power -= 2;
                lmax -= 2;
            }
        }

        if ((static_cast<u32>(mant) & 1) == 0 && lmax >= 1) {
            constexpr u32 den = 10;
            const u64 div = mant / den;
            if (static_cast<u32>(mant) == static_cast<u32>(div * den)) {
                mant = div;
                power--;
            }
        }

        flags |= static_cast<u32>(power) << kScaleShift;
        result.set_low64(mant);
    }

    result.flags = flags;
    return result;
}

// VarR8FromDec
double var_r8_from_dec(const Calc& value) {
    // taken via reverse engineering the double that corresponds to 2^64 (oleaut32 has ds2to64)
    constexpr double ds2to64 = 1.8446744073709552e+019;

    double dbl = (static_cast<double>(value.low64()) + static_cast<double>(value.hi) * ds2to64) / kDoublePowers10[value.scale()];

    if (value.is_negative()) dbl = -dbl;

    return dbl;
}

}  // namespace

Decimal Decimal::add(const Decimal& a, const Decimal& b) {
    Calc d1 = to_calc(a);
    dec_add_sub(d1, to_calc(b), false);
    return from_calc(d1);
}

Decimal Decimal::subtract(const Decimal& a, const Decimal& b) {
    Calc d1 = to_calc(a);
    dec_add_sub(d1, to_calc(b), true);
    return from_calc(d1);
}

Decimal Decimal::multiply(const Decimal& a, const Decimal& b) {
    Calc d1 = to_calc(a);
    var_dec_mul(d1, to_calc(b));
    return from_calc(d1);
}

Decimal Decimal::divide(const Decimal& a, const Decimal& b) {
    Calc d1 = to_calc(a);
    var_dec_div(d1, to_calc(b));
    return from_calc(d1);
}

Decimal Decimal::remainder(const Decimal& a, const Decimal& b) {
    Calc d1 = to_calc(a);
    var_dec_mod(d1, to_calc(b));
    return from_calc(d1);
}

Decimal Decimal::negate(const Decimal& d) {
    Decimal result = d;
    result.negative = !d.negative;
    return result;
}

int Decimal::compare(const Decimal& a, const Decimal& b) { return var_dec_cmp(to_calc(a), to_calc(b)); }

Decimal Decimal::truncate(const Decimal& d) {
    Calc c = to_calc(d);
    truncate_calc(c);
    return from_calc(c);
}

Decimal Decimal::from_int64(std::int64_t value) {
    const bool negative = value < 0;
    const u64 magnitude = negative ? 0ull - static_cast<u64>(value) : static_cast<u64>(value);
    return Decimal(static_cast<u32>(magnitude), static_cast<u32>(magnitude >> 32), 0, negative, 0);
}

Decimal Decimal::from_uint64(std::uint64_t value) { return Decimal(static_cast<u32>(value), static_cast<u32>(value >> 32), 0, false, 0); }

Decimal Decimal::from_float(float value) { return from_calc(var_dec_from_r4(value)); }

Decimal Decimal::from_double(double value) { return from_calc(var_dec_from_r8(value)); }

std::int32_t Decimal::to_int32() const {
    Calc d = to_calc(*this);
    truncate_calc(d);
    if ((d.hi | d.mid) == 0) {
        std::int32_t i = static_cast<std::int32_t>(d.lo);
        if (!d.is_negative()) {
            if (i >= 0) return i;
        } else {
            i = static_cast<std::int32_t>(0u - static_cast<u32>(i));
            if (i <= 0) return i;
        }
    }
    throw OverflowError("Value was either too large or too small for an Int32.");
}

std::int64_t Decimal::to_int64() const {
    Calc d = to_calc(*this);
    truncate_calc(d);
    if (d.hi == 0) {
        std::int64_t l = static_cast<std::int64_t>(d.low64());
        if (!d.is_negative()) {
            if (l >= 0) return l;
        } else {
            l = static_cast<std::int64_t>(0ull - static_cast<u64>(l));
            if (l <= 0) return l;
        }
    }
    throw OverflowError("Value was either too large or too small for an Int64.");
}

std::uint32_t Decimal::to_uint32() const {
    Calc d = to_calc(*this);
    truncate_calc(d);
    if ((d.hi | d.mid) == 0) {
        const u32 i = d.lo;
        if (!d.is_negative() || i == 0) return i;
    }
    throw OverflowError("Value was either too large or too small for a UInt32.");
}

std::uint64_t Decimal::to_uint64() const {
    Calc d = to_calc(*this);
    truncate_calc(d);
    if (d.hi == 0) {
        const u64 l = d.low64();
        if (!d.is_negative() || l == 0) return l;
    }
    throw OverflowError("Value was either too large or too small for a UInt64.");
}

std::uint8_t Decimal::to_byte() const {
    const u32 temp = to_uint32();
    if (temp != static_cast<std::uint8_t>(temp)) throw OverflowError("Value was either too large or too small for an unsigned byte.");
    return static_cast<std::uint8_t>(temp);
}

std::int8_t Decimal::to_sbyte() const {
    const std::int32_t temp = to_int32();
    if (temp != static_cast<std::int8_t>(temp)) throw OverflowError("Value was either too large or too small for a signed byte.");
    return static_cast<std::int8_t>(temp);
}

std::int16_t Decimal::to_int16() const {
    const std::int32_t temp = to_int32();
    if (temp != static_cast<std::int16_t>(temp)) throw OverflowError("Value was either too large or too small for an Int16.");
    return static_cast<std::int16_t>(temp);
}

std::uint16_t Decimal::to_uint16() const {
    const u32 temp = to_uint32();
    if (temp != static_cast<std::uint16_t>(temp)) throw OverflowError("Value was either too large or too small for a UInt16.");
    return static_cast<std::uint16_t>(temp);
}

char16_t Decimal::to_char() const { return static_cast<char16_t>(to_uint16()); }

double Decimal::to_double() const { return var_r8_from_dec(to_calc(*this)); }

float Decimal::to_float() const { return static_cast<float>(var_r8_from_dec(to_calc(*this))); }

}  // namespace cb::dotnet::decompiler::util
