// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/cpu/pcode_ops_ppc_vector.cpp -- the AltiVec and VMX128 operations the PowerPC descriptions name and give no p-code, by the names of the descriptions (see pcode_ops_ppc.hpp).
//
// Every handler is given the operation's inputs and output as the one emulator hands wide values over: 16 bytes, least significant byte first. AltiVec numbers a vector's elements the
// other way (element 0 is the most significant), so an operation whose result depends on the order of the elements (merge, pack, unpack, permute, splat, the even and odd products, the
// sums across) works on the lanes reversed (vec::reverseLanes), and the lane library's lane 0 is then AltiVec's element 0. An operation that treats each element alone needs no reversal.
// The meanings are the manuals' (AltiVec PEM; the Xbox 360's VMX128 as the Xenon's description names it); the floating point is the library's with the PowerPC's NaN and minimum rules,
// in Java mode (denormals as IEEE says) or, when VSCR[NJ] is set, with denormal inputs and results flushed to zero.
#include <algorithm>
#include <bit>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>

#include "emulate/cpu/pcode_ops_ppc.hpp"
#include "emulate/cpu/pcode_vector.hpp"

namespace cb::emulate::pcode_ops_ppc {

namespace {

namespace vec = cb::emulate::vec;
using Byte = std::uint8_t;
using Call = Emu::UserOpCall;
using Ctx = VectorContext;
constexpr std::size_t kN = 16;

Ctx& ctx(void* c) { return *static_cast<Ctx*>(c); }
bool shaped(const Call& c, unsigned inputs) { return c.nIn >= inputs && c.outSize == kN; }

// ---- the vector as AltiVec numbers it ------------------------------------------------------------------------------------------------------------------------------------

// `in` with its lanes of `ew` bytes in the other order (the same call undoes it).
void reversed(Byte* out, const Byte* in, unsigned ew) { vec::reverseLanes(out, in, kN, ew); }

// Element `i` (AltiVec's numbering: 0 is the most significant) of `ew` bytes as a number.
std::uint64_t element(const Byte* v, unsigned ew, unsigned i) { return vec::load(v + (kN / ew - 1 - i) * ew, ew); }
void set_element(Byte* v, unsigned ew, unsigned i, std::uint64_t x) { vec::store(v + (kN / ew - 1 - i) * ew, ew, x); }

vec::FpEnv fp_env(const Ctx& c, vec::Round round = vec::Round::NearestEven) {
    vec::FpEnv env;
    env.round = round;
    env.nan = vec::NanRule::Ppc;
    env.minmax = vec::MinMax::Ppc;
    env.flushInputs = env.flushResults = ((c.vscr >> 16) & 1) != 0;   // VSCR[NJ]
    return env;
}

void note_saturation(Ctx& c, bool saturated) {
    if (saturated) c.vscr |= 1;   // VSCR[SAT]
}

// ---- integer lanes ---------------------------------------------------------------------------------------------------------------------------------------------------------

template <unsigned EW, vec::Sign S>
bool add_saturate(Emu&, Call& c, void* x) {
    if (!shaped(c, 2)) return false;
    Byte wrapped[kN];
    vec::add(wrapped, c.in[0], c.in[1], kN, EW);
    vec::addSat(c.out, c.in[0], c.in[1], kN, EW, S);
    note_saturation(ctx(x), std::memcmp(wrapped, c.out, kN) != 0);   // (a lane that saturates differs from its wrapped sum)
    return true;
}

template <unsigned EW, vec::Sign S>
bool sub_saturate(Emu&, Call& c, void* x) {
    if (!shaped(c, 2)) return false;
    Byte wrapped[kN];
    vec::sub(wrapped, c.in[0], c.in[1], kN, EW);
    vec::subSat(c.out, c.in[0], c.in[1], kN, EW, S);
    note_saturation(ctx(x), std::memcmp(wrapped, c.out, kN) != 0);
    return true;
}

template <unsigned EW>
bool sub_wrap(Emu&, Call& c, void*) {
    if (!shaped(c, 2)) return false;
    vec::sub(c.out, c.in[0], c.in[1], kN, EW);
    return true;
}

template <unsigned EW, vec::Sign S>
bool average(Emu&, Call& c, void*) {
    if (!shaped(c, 2)) return false;
    vec::halvingAdd(c.out, c.in[0], c.in[1], kN, EW, S, true);
    return true;
}

template <unsigned EW, vec::Sign S>
bool maximum(Emu&, Call& c, void*) {
    if (!shaped(c, 2)) return false;
    vec::max(c.out, c.in[0], c.in[1], kN, EW, S);
    return true;
}

template <unsigned EW, vec::Sign S>
bool minimum(Emu&, Call& c, void*) {
    if (!shaped(c, 2)) return false;
    vec::min(c.out, c.in[0], c.in[1], kN, EW, S);
    return true;
}

template <unsigned EW>
bool compare_equal(Emu&, Call& c, void*) {
    if (!shaped(c, 2)) return false;
    vec::compare(c.out, c.in[0], c.in[1], kN, EW, vec::Cmp::Eq, vec::Sign::Unsigned);
    return true;
}

template <unsigned EW, vec::Sign S>
bool compare_greater(Emu&, Call& c, void*) {
    if (!shaped(c, 2)) return false;
    vec::compare(c.out, c.in[0], c.in[1], kN, EW, vec::Cmp::Gt, S);
    return true;
}

template <unsigned EW, vec::Shift K>
bool shift_each(Emu&, Call& c, void*) {   // each element by the count in the same element of vB (its low bits: the count is taken modulo the element's width)
    if (!shaped(c, 2)) return false;
    vec::shiftVar(c.out, c.in[0], c.in[1], kN, EW, K, vec::Count::Mask);
    return true;
}

template <unsigned EW, vec::Sign S, bool Even>
bool multiply_half(Emu&, Call& c, void*) {   // vmule*, vmulo*: the products of the even (odd) elements, which are the odd (even) lanes of the library's numbering
    if (!shaped(c, 2)) return false;
    vec::mulEvenOdd(c.out, c.in[0], c.in[1], kN, EW, S, Even);
    return true;
}

// vmsum*: each word is the sum of the products of the elements that make it, plus the word of vC; modulo, or saturated (and then VSCR[SAT] says so).
template <unsigned EW, vec::Sign SA, vec::Sign SB, bool Saturate>
bool multiply_sum(Emu&, Call& c, void* x) {
    if (!shaped(c, 3)) return false;
    vec::multiplyAdd(c.out, c.in[2], c.in[0], c.in[1], kN, EW, 4, SA, SB, Saturate);
    if (Saturate) {
        Byte wrapped[kN];
        vec::multiplyAdd(wrapped, c.in[2], c.in[0], c.in[1], kN, EW, 4, SA, SB, false);
        note_saturation(ctx(x), std::memcmp(wrapped, c.out, kN) != 0);
    }
    return true;
}

template <bool Round>
bool multiply_high_add(Emu&, Call& c, void* x) {   // vmhaddshs, vmhraddshs
    if (!shaped(c, 3)) return false;
    vec::mulHighAddSat(c.out, c.in[0], c.in[1], c.in[2], kN, 2, Round);
    // saturated where the result is an end of the range and the sum was not: recomputed in 64 bits
    for (unsigned i = 0; i < 8; ++i) {
        const std::int64_t a = vec::signExtend(vec::getLane(c.in[0], 2, i), 2), b = vec::signExtend(vec::getLane(c.in[1], 2, i), 2), s = vec::signExtend(vec::getLane(c.in[2], 2, i), 2);
        const std::int64_t exact = ((a * b + (Round ? 0x4000 : 0)) >> 15) + s;
        if (exact > 32767 || exact < -32768) note_saturation(ctx(x), true);
    }
    return true;
}

bool multiply_low_add(Emu&, Call& c, void*) {   // vmladduhm
    if (!shaped(c, 3)) return false;
    Byte product[kN];
    vec::mul(product, c.in[0], c.in[1], kN, 2);
    vec::add(c.out, product, c.in[2], kN, 2);
    return true;
}

bool vand(Emu&, Call& c, void*) { return shaped(c, 2) && (vec::bitAnd(c.out, c.in[0], c.in[1], kN), true); }
bool vandc(Emu&, Call& c, void*) { return shaped(c, 2) && (vec::bitAndNot(c.out, c.in[0], c.in[1], kN), true); }
bool vor(Emu&, Call& c, void*) { return shaped(c, 2) && (vec::bitOr(c.out, c.in[0], c.in[1], kN), true); }
bool vnor(Emu&, Call& c, void*) {
    if (!shaped(c, 2)) return false;
    Byte t[kN];
    vec::bitOr(t, c.in[0], c.in[1], kN);
    vec::bitNot(c.out, t, kN);
    return true;
}
bool vsel(Emu&, Call& c, void*) {   // vsel(vA, vB, vC): a bit of vB where vC has a one, else of vA
    if (!shaped(c, 3)) return false;
    vec::bitSelect(c.out, c.in[2], c.in[1], c.in[0], kN);
    return true;
}

bool add_carry_word(Emu&, Call& c, void*) {   // vaddcuw: the carry out of each word's unsigned sum
    if (!shaped(c, 2)) return false;
    for (unsigned i = 0; i < 4; ++i) vec::setLane(c.out, 4, i, ((vec::getLane(c.in[0], 4, i) + vec::getLane(c.in[1], 4, i)) >> 32) & 1);
    return true;
}

bool sub_carry_word(Emu&, Call& c, void*) {   // vsubcuw: 1 where the unsigned subtraction needs no borrow
    if (!shaped(c, 2)) return false;
    for (unsigned i = 0; i < 4; ++i) vec::setLane(c.out, 4, i, vec::getLane(c.in[0], 4, i) >= vec::getLane(c.in[1], 4, i) ? 1 : 0);
    return true;
}

// ---- sums across -------------------------------------------------------------------------------------------------------------------------------------------------------------

std::int64_t clamp_signed(std::int64_t v, std::int64_t lo, std::int64_t hi, bool& sat) {
    if (v < lo) return sat = true, lo;
    if (v > hi) return sat = true, hi;
    return v;
}

template <unsigned EW, bool Signed>
bool sum_into_words(Emu&, Call& c, void* x) {   // vsum4ubs, vsum4sbs, vsum4shs: each word of vB plus the elements of vA that make the same word, saturated
    if (!shaped(c, 2)) return false;
    bool sat = false;
    for (unsigned w = 0; w < 4; ++w) {
        std::int64_t sum = Signed ? vec::signExtend(vec::getLane(c.in[1], 4, w), 4) : static_cast<std::int64_t>(vec::getLane(c.in[1], 4, w));
        for (unsigned k = 0; k < 4 / EW; ++k) {
            const std::uint64_t e = vec::getLane(c.in[0], EW, w * (4 / EW) + k);
            sum += Signed ? vec::signExtend(e, EW) : static_cast<std::int64_t>(e);
        }
        const std::int64_t r = Signed ? clamp_signed(sum, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max(), sat) : clamp_signed(sum, 0, 0xFFFFFFFFll, sat);
        vec::setLane(c.out, 4, w, static_cast<std::uint64_t>(r));
    }
    note_saturation(ctx(x), sat);
    return true;
}

bool sum_pairs(Emu&, Call& c, void* x) {   // vsum2sws: words 0 and 2 (AltiVec's numbering) of the result are zero, words 1 and 3 hold the sum of the two words of vA before them plus the word of vB
    if (!shaped(c, 2)) return false;
    bool sat = false;
    Byte a[kN], b[kN], r[kN] = {};
    reversed(a, c.in[0], 4);
    reversed(b, c.in[1], 4);
    for (unsigned half = 0; half < 2; ++half) {
        const std::int64_t s = vec::signExtend(vec::getLane(a, 4, 2 * half), 4) + vec::signExtend(vec::getLane(a, 4, 2 * half + 1), 4) + vec::signExtend(vec::getLane(b, 4, 2 * half + 1), 4);
        vec::setLane(r, 4, 2 * half + 1, static_cast<std::uint64_t>(clamp_signed(s, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max(), sat)));
    }
    reversed(c.out, r, 4);
    note_saturation(ctx(x), sat);
    return true;
}

bool sum_all(Emu&, Call& c, void* x) {   // vsumsws: word 3 is the sum of the four words of vA and word 3 of vB, saturated; the other words are zero
    if (!shaped(c, 2)) return false;
    bool sat = false;
    std::int64_t s = vec::signExtend(vec::getLane(c.in[1], 4, 0), 4);   // (vB's word 3 is the library's lane 0)
    for (unsigned i = 0; i < 4; ++i) s += vec::signExtend(vec::getLane(c.in[0], 4, i), 4);
    std::memset(c.out, 0, kN);
    vec::setLane(c.out, 4, 0, static_cast<std::uint64_t>(clamp_signed(s, std::numeric_limits<std::int32_t>::min(), std::numeric_limits<std::int32_t>::max(), sat)));
    note_saturation(ctx(x), sat);
    return true;
}

// ---- moving elements ---------------------------------------------------------------------------------------------------------------------------------------------------------

template <unsigned EW, bool High>
bool merge(Emu&, Call& c, void*) {   // vmrgh*, vmrgl*: the elements of the first (last) halves of vA and vB, alternating, vA's first
    if (!shaped(c, 2)) return false;
    Byte a[kN], b[kN], t[kN];
    reversed(a, c.in[0], EW);
    reversed(b, c.in[1], EW);
    vec::zip(t, a, b, kN, EW, !High);   // (in the reversed order the first half is the library's low half)
    reversed(c.out, t, EW);
    return true;
}

// vpk*: the elements of vA then those of vB, each narrowed to half its width (truncated or saturated), vA's in the first half of the result.
template <unsigned EW, vec::Narrow M>
bool pack(Emu&, Call& c, void* x) {
    if (!shaped(c, 2)) return false;
    Byte joined[2 * kN], narrowed[kN];
    reversed(joined, c.in[0], EW);
    reversed(joined + kN, c.in[1], EW);
    vec::narrow(narrowed, joined, 2 * kN, EW, M);
    reversed(c.out, narrowed, EW / 2);
    if (M != vec::Narrow::Truncate) {   // saturated where an element does not fit
        bool sat = false;
        for (unsigned v = 0; v < 2; ++v)
            for (unsigned i = 0; i < kN / EW; ++i) {
                const std::uint64_t raw = vec::getLane(c.in[v], EW, i);
                const bool is_signed = M == vec::Narrow::SignedToSigned || M == vec::Narrow::SignedToUnsigned;
                const std::int64_t value = is_signed ? vec::signExtend(raw, EW) : static_cast<std::int64_t>(raw);
                const std::int64_t lo = M == vec::Narrow::SignedToSigned ? -(std::int64_t{1} << (4 * EW - 1)) : 0;
                const std::int64_t hi = M == vec::Narrow::SignedToSigned ? (std::int64_t{1} << (4 * EW - 1)) - 1 : (std::int64_t{1} << (4 * EW)) - 1;
                if (value < lo || value > hi) sat = true;
            }
        note_saturation(ctx(x), sat);
    }
    return true;
}

template <unsigned EW, bool High>
bool unpack_signed(Emu&, Call& c, void*) {   // vupkhs*, vupkls*: the first (last) half of vB's elements sign extended to twice the width
    if (!shaped(c, 1)) return false;
    Byte b[kN], wide[kN];
    reversed(b, c.in[0], EW);
    vec::widen(wide, b + (High ? 0 : kN / 2), kN / 2, EW, vec::Sign::Signed);
    reversed(c.out, wide, 2 * EW);
    return true;
}

bool pack_pixel(Emu&, Call& c, void*) {   // vpkpx
    if (!shaped(c, 2)) return false;
    Byte a[kN], b[kN], t[kN];
    reversed(a, c.in[0], 4);
    reversed(b, c.in[1], 4);
    vec::packPixel32(t, a, b, kN);
    reversed(c.out, t, 2);
    return true;
}

template <bool High>
bool unpack_pixel(Emu&, Call& c, void*) {   // vupkhpx, vupklpx
    if (!shaped(c, 1)) return false;
    Byte b[kN], t[kN];
    reversed(b, c.in[0], 2);
    vec::unpackPixel16(t, b + (High ? 0 : kN / 2), kN / 2);
    reversed(c.out, t, 4);
    return true;
}

template <unsigned EW>
bool splat(Emu&, Call& c, void*) {   // vspltb, vsplth, vspltw: vB's element number UIMM (AltiVec's numbering)
    if (!shaped(c, 2)) return false;
    vec::splat(c.out, kN, EW, element(c.in[0], EW, c.in[1][0] & (kN / EW - 1)));
    return true;
}

template <unsigned EW>
bool splat_immediate(Emu&, Call& c, void*) {   // vspltisb, vspltish, vspltisw: the 5-bit signed immediate
    if (!shaped(c, 1)) return false;
    const std::int64_t imm = static_cast<std::int64_t>((c.in[0][0] & 31) ^ 16) - 16;
    vec::splat(c.out, kN, EW, static_cast<std::uint64_t>(imm));
    return true;
}

bool permute(Emu&, Call& c, void*) {   // vperm: byte i of the result is byte (vC[i] & 31) of vA followed by vB
    if (!shaped(c, 3)) return false;
    Byte table[2 * kN], indices[kN], t[kN];
    reversed(table, c.in[0], 1);
    reversed(table + kN, c.in[1], 1);
    reversed(indices, c.in[2], 1);
    vec::Lookup mode;
    mode.indexMask = 0x1F;
    vec::tableLookup(t, table, 2 * kN, indices, kN, mode);
    reversed(c.out, t, 1);
    return true;
}

bool shift_bits(Call& c, bool left) {   // vsl, vsr: the whole vector by the 0 to 7 bits of the low byte of vB
    if (!shaped(c, 2)) return false;
    const unsigned n = c.in[1][0] & 7;
    std::uint64_t lo = vec::load(c.in[0], 8), hi = vec::load(c.in[0] + 8, 8);
    if (n != 0) {
        if (left) {
            hi = (hi << n) | (lo >> (64 - n));
            lo <<= n;
        } else {
            lo = (lo >> n) | (hi << (64 - n));
            hi >>= n;
        }
    }
    vec::store(c.out, 8, lo);
    vec::store(c.out + 8, 8, hi);
    return true;
}
bool shift_left_bits(Emu&, Call& c, void*) { return shift_bits(c, true); }
bool shift_right_bits(Emu&, Call& c, void*) { return shift_bits(c, false); }

template <bool Left>
bool shift_octets(Emu&, Call& c, void*) {   // vslo, vsro: the whole vector by the bytes in bits 121-124 of vB
    if (!shaped(c, 2)) return false;
    vec::shiftBytes(c.out, c.in[0], kN, kN, (c.in[1][0] >> 3) & 0xF, Left);
    return true;
}

// ---- floating point -----------------------------------------------------------------------------------------------------------------------------------------------------------

bool float_add(Emu&, Call& c, void* x) {
    if (!shaped(c, 2)) return false;
    unsigned flags = 0;
    vec::fp2(c.out, c.in[0], c.in[1], kN, 4, vec::Fp2::Add, fp_env(ctx(x)), flags);
    return true;
}
bool float_sub(Emu&, Call& c, void* x) {
    if (!shaped(c, 2)) return false;
    unsigned flags = 0;
    vec::fp2(c.out, c.in[0], c.in[1], kN, 4, vec::Fp2::Sub, fp_env(ctx(x)), flags);
    return true;
}
bool float_mul(Emu&, Call& c, void* x) {   // VMX128's vmulfp128 (AltiVec has none: vmaddfp with a zero addend of the right sign)
    if (!shaped(c, 2)) return false;
    unsigned flags = 0;
    vec::fp2(c.out, c.in[0], c.in[1], kN, 4, vec::Fp2::Mul, fp_env(ctx(x)), flags);
    return true;
}
bool float_max(Emu&, Call& c, void* x) {
    if (!shaped(c, 2)) return false;
    unsigned flags = 0;
    vec::fp2(c.out, c.in[0], c.in[1], kN, 4, vec::Fp2::Max, fp_env(ctx(x)), flags);
    return true;
}
bool float_min(Emu&, Call& c, void* x) {
    if (!shaped(c, 2)) return false;
    unsigned flags = 0;
    vec::fp2(c.out, c.in[0], c.in[1], kN, 4, vec::Fp2::Min, fp_env(ctx(x)), flags);
    return true;
}
bool float_multiply_add(Emu&, Call& c, void* x) {   // vmaddfp(vA, vC, vB): vA * vC + vB, one rounding
    if (!shaped(c, 3)) return false;
    unsigned flags = 0;
    if ((ctx(x).word >> 26) == 5)   // VMX128's vmaddfp128 vD,vA,vB: vA * vB + vD (the description passes vA, vD, vB: the old vD is the second input)
        vec::fp3(c.out, c.in[0], c.in[2], c.in[1], kN, 4, vec::Fp3::MulAdd, fp_env(ctx(x)), flags);
    else
        vec::fp3(c.out, c.in[0], c.in[1], c.in[2], kN, 4, vec::Fp3::MulAdd, fp_env(ctx(x)), flags);
    return true;
}
bool float_multiply_add_to_d(Emu&, Call& c, void* x) {   // vmaddcfp128 vD,vA,vB: vA * vD + vB (the old vD is the second input)
    if (!shaped(c, 3)) return false;
    unsigned flags = 0;
    vec::fp3(c.out, c.in[0], c.in[1], c.in[2], kN, 4, vec::Fp3::MulAdd, fp_env(ctx(x)), flags);
    return true;
}
template <unsigned Terms>
bool float_dot(Emu&, Call& c, void* x) {   // vmsum3fp128, vmsum4fp128: the dot product of the first three (four) elements, in every element of the result (each product and sum rounded to single, in non-Java mode with denormals flushed)
    if (!shaped(c, 2)) return false;
    unsigned flags = 0;
    const vec::FpEnv env = fp_env(ctx(x));
    Byte sum[4] = {0, 0, 0, 0};
    for (unsigned i = 0; i < Terms; ++i) {   // (elements 0, 1, 2 (3) in AltiVec's numbering: the high lanes of the library's)
        Byte a[4], b[4], product[4];
        std::memcpy(a, c.in[0] + 4 * (3 - i), 4);
        std::memcpy(b, c.in[1] + 4 * (3 - i), 4);
        vec::fp2(product, a, b, 4, 4, vec::Fp2::Mul, env, flags);
        if (i == 0) std::memcpy(sum, product, 4);
        else vec::fp2(sum, sum, product, 4, 4, vec::Fp2::Add, env, flags);
    }
    vec::splat(c.out, kN, 4, vec::load(sum, 4));
    return true;
}
bool float_negative_multiply_subtract(Emu&, Call& c, void* x) {   // vnmsubfp(vA, vC, vB): -(vA * vC - vB)
    if (!shaped(c, 3)) return false;
    unsigned flags = 0;
    vec::fp3(c.out, c.in[0], c.in[1], c.in[2], kN, 4, vec::Fp3::NegMulAdd, fp_env(ctx(x)), flags);
    return true;
}

template <vec::FpCmp K>
bool float_compare(Emu&, Call& c, void* x) {
    if (!shaped(c, 2)) return false;
    unsigned flags = 0;
    vec::fpCompare(c.out, c.in[0], c.in[1], kN, 4, K, true, fp_env(ctx(x)), flags);
    return true;
}

// A float with a denormal read as a zero of its sign when VSCR[NJ] says so (non-Java mode).
float flushed_if(const Ctx& c, float x) { return (((c.vscr >> 16) & 1) != 0 && std::fpclassify(x) == FP_SUBNORMAL) ? std::copysign(0.0f, x) : x; }

bool float_compare_bounds(Emu&, Call& c, void* x) {   // vcmpbfp: bit 0 set where vA > vB, bit 1 where vA < -vB; a NaN sets both; in non-Java mode a denormal is a zero
    if (!shaped(c, 2)) return false;
    for (unsigned i = 0; i < 4; ++i) {
        const float a = flushed_if(ctx(x), std::bit_cast<float>(static_cast<std::uint32_t>(vec::getLane(c.in[0], 4, i)))), b = flushed_if(ctx(x), std::bit_cast<float>(static_cast<std::uint32_t>(vec::getLane(c.in[1], 4, i))));
        std::uint32_t r = 0;
        if (std::isnan(a) || std::isnan(b)) r = 0xC0000000u;
        else r = (a > b ? 0x80000000u : 0) | (a < -b ? 0x40000000u : 0);
        vec::setLane(c.out, 4, i, r);
    }
    return true;
}

template <vec::Round R>
bool float_round(Emu&, Call& c, void* x) {   // vrfin, vrfiz, vrfip, vrfim
    if (!shaped(c, 1)) return false;
    unsigned flags = 0;
    vec::fp1(c.out, c.in[0], kN, 4, vec::Fp1::RoundToInt, fp_env(ctx(x), R), flags);
    return true;
}

bool convert_from_signed(Emu&, Call& c, void* x) {   // vcfsx(vB, UIMM): the integer divided by 2^UIMM
    if (!shaped(c, 2)) return false;
    unsigned flags = 0;
    vec::intToFloat(c.out, c.in[0], kN, 4, 4, vec::Sign::Signed, c.in[1][0] & 31, fp_env(ctx(x)), flags);
    return true;
}
bool convert_from_unsigned(Emu&, Call& c, void* x) {
    if (!shaped(c, 2)) return false;
    unsigned flags = 0;
    vec::intToFloat(c.out, c.in[0], kN, 4, 4, vec::Sign::Unsigned, c.in[1][0] & 31, fp_env(ctx(x)), flags);
    return true;
}
template <vec::Sign S>
bool convert_to_integer(Emu&, Call& c, void* x) {   // vctsxs, vctuxs(vB, UIMM): truncated toward zero after multiplying by 2^UIMM, saturated (a NaN gives zero); VSCR[SAT] says it saturated
    if (!shaped(c, 2)) return false;
    unsigned flags = 0;
    vec::floatToInt(c.out, c.in[0], kN, 4, 4, S, c.in[1][0] & 31, vec::IntOverflow::Saturate, fp_env(ctx(x), vec::Round::TowardZero), flags);
    note_saturation(ctx(x), (flags & vec::kInvalid) != 0);
    return true;
}

// The estimates (vrefp, vrsqrtefp, vexptefp, vlogefp): the processor's are good to 12 bits and leave the rest to the implementation; these are the host's single-precision results, which
// are within what the manual allows. A NaN gives itself quieted, and an invalid operation the default NaN.
template <float (*F)(float)>
bool estimate(Emu&, Call& c, void* x) {   // (in non-Java mode, VSCR[NJ]: a denormal input is a zero of its sign and a denormal result is flushed to a zero of its sign)
    if (!shaped(c, 1)) return false;
    for (unsigned i = 0; i < 4; ++i) {
        const float in = std::bit_cast<float>(static_cast<std::uint32_t>(vec::getLane(c.in[0], 4, i)));
        const float arg = flushed_if(ctx(x), in);
        float r = flushed_if(ctx(x), F(arg));
        std::uint32_t bits = std::bit_cast<std::uint32_t>(r);
        if (std::isnan(in)) bits = std::bit_cast<std::uint32_t>(in) | 0x00400000u;
        else if (std::isnan(r)) bits = 0x7FC00000u;
        vec::setLane(c.out, 4, i, bits);
    }
    return true;
}
float reciprocal(float x) { return 1.0f / x; }
float reciprocal_sqrt(float x) { return x < 0 ? std::numeric_limits<float>::quiet_NaN() : 1.0f / std::sqrt(x); }
float exp2_of(float x) { return std::exp2(x); }
float log2_of(float x) { return x < 0 ? std::numeric_limits<float>::quiet_NaN() : std::log2(x); }

// ---- the vector status register and the memory instructions that are not p-code ----------------------------------------------------------------------------------------------

bool move_from_vscr(Emu&, Call& c, void* x) {
    if (c.outSize != kN) return false;
    vec::store(c.out, 4, ctx(x).vscr);
    return true;
}
bool move_to_vscr(Emu&, Call& c, void* x) {
    if (c.nIn < 1) return false;
    ctx(x).vscr = static_cast<std::uint32_t>(vec::load(c.in[0], 4)) & 0x00010001u;   // (NJ and SAT: the other bits are reserved)
    return true;
}
bool nothing(Emu&, Call&, void*) { return true; }   // dst, dstst, dss: hints

// The effective address of an indexed instruction whose description passes RA and RB as registers (a zero RA field is the number zero, not r0).
std::uint64_t indexed_address(const Call& c, const Ctx& x) {
    const std::uint64_t a = ((x.word >> 16) & 31) == 0 ? 0 : vec::load(c.in[0], 8);
    return a + vec::load(c.in[1], 8);
}

template <bool Left>
bool shift_permute(Emu&, Call& c, void* x) {   // lvsl, lvsr
    if (!shaped(c, 2)) return false;
    const unsigned sh = static_cast<unsigned>(indexed_address(c, ctx(x))) & 15;
    Byte be[kN];
    for (unsigned i = 0; i < kN; ++i) be[i] = static_cast<Byte>((Left ? sh : 16 - sh) + i);
    reversed(c.out, be, 1);
    return true;
}

template <unsigned EW>
bool load_element(Emu& emu, Call& c, void* x) {   // lvehx, lvewx: the aligned element at the address goes to the element of vD the address says; the others are zero
    if (!shaped(c, 2)) return false;
    const std::uint64_t address = indexed_address(c, ctx(x)) & ~std::uint64_t{EW - 1};
    const unsigned index = static_cast<unsigned>(address & 15) / EW;
    std::memset(c.out, 0, kN);
    set_element(c.out, EW, index, emu.busRead(static_cast<std::uint32_t>(address), EW));
    return true;
}

bool load_vector_lru(Emu& emu, Call& c, void* x) {   // lvxl
    if (!shaped(c, 2)) return false;
    const std::uint32_t address = static_cast<std::uint32_t>(indexed_address(c, ctx(x))) & ~15u;
    vec::store(c.out + 8, 8, emu.busRead(address, 8));
    vec::store(c.out, 8, emu.busRead(address + 8, 8));
    return true;
}

template <unsigned EW>
bool store_element(Emu&, Call& c, void* x) {   // stvebx, stvehx, stvewx: the element of vS the address says is what is stored (the description stores the result)
    if (c.nIn < 3 || c.outSize != EW) return false;
    const std::uint64_t base = ((ctx(x).word >> 16) & 31) == 0 ? 0 : vec::load(c.in[1], 8);
    const std::uint64_t address = (base + vec::load(c.in[2], 8)) & ~std::uint64_t{EW - 1};
    vec::store(c.out, EW, element(c.in[0], EW, static_cast<unsigned>(address & 15) / EW));
    return true;
}


// ---- VMX128 -------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool compare_set_cr6(Emu&, Call& c, void* x) {   // cr6 of a compare with the record bit: LT where every element is true, EQ where none is; vcmpbfp128 sets EQ where every element is within bounds (zero)
    if (c.nIn < 1 || c.inSize[0] != kN || c.outSize != 1) return false;
    bool ones = true, zeros = true;
    for (unsigned i = 0; i < kN; ++i) {
        ones = ones && c.in[0][i] == 0xFF;
        zeros = zeros && c.in[0][i] == 0;
    }
    const bool bounds = (ctx(x).word >> 26) == 6 && ((ctx(x).word >> 7) & 7) == 3;
    c.out[0] = bounds ? (zeros ? 2 : 0) : ones ? 8 : zeros ? 2 : 0;
    return true;
}

template <bool Right>
bool load_left_right(Emu& emu, Call& c, void*) {   // lvlx, lvrx (and their LRU forms): the bytes from the address to the end of its quadword go to the left of vD (to the start of the quadword and to the right of vD), the rest zero
    if (c.nIn < 1 || c.outSize != kN) return false;
    const std::uint32_t address = static_cast<std::uint32_t>(vec::load(c.in[0], 8));
    const unsigned eb = address & 15;
    Byte quad[kN], be[kN] = {};   // (big-endian byte order)
    for (unsigned i = 0; i < kN; ++i) quad[i] = static_cast<Byte>(emu.busRead((address & ~15u) + i, 1));
    if (!Right) {
        for (unsigned i = 0; i + eb < kN; ++i) be[i] = quad[eb + i];
    } else {
        for (unsigned i = 0; i < eb; ++i) be[kN - eb + i] = quad[i];
    }
    reversed(c.out, be, 1);
    return true;
}

template <bool Right>
bool store_left_right(Emu& emu, Call& c, void*) {   // stvlx, stvrx (and their LRU forms): the left bytes of vS to the address and on to the end of its quadword (the right bytes to the start of the quadword up to the address)
    if (c.nIn < 2 || c.inSize[1] != kN) return false;
    const std::uint32_t address = static_cast<std::uint32_t>(vec::load(c.in[0], 8));
    const unsigned eb = address & 15;
    Byte be[kN];
    reversed(be, c.in[1], 1);
    if (!Right) {
        for (unsigned i = 0; i + eb < kN; ++i) emu.busWrite(address + i, 1, be[i]);
    } else {
        for (unsigned i = 0; i < eb; ++i) emu.busWrite((address & ~15u) + i, 1, be[kN - eb + i]);
    }
    return true;
}

bool permute_word_immediate(Emu&, Call& c, void*) {   // vpermwi128 vD,vB,PERM: word i (AltiVec's numbering) of the result is word ((PERM >> (6 - 2 i)) & 3) of vB
    if (!shaped(c, 2)) return false;
    Byte b[kN], t[kN];
    reversed(b, c.in[0], 4);
    for (unsigned i = 0; i < 4; ++i) vec::setLane(t, 4, i, vec::getLane(b, 4, (c.in[1][0] >> (6 - 2 * i)) & 3));
    reversed(c.out, t, 4);
    return true;
}

// ---- VMX128's D3D packing and the rotate-and-insert (the Xbox 360 XDK's "New Instructions in VMX128" white paper) ------------------------------------------------------------

// The register vD of a VMX128 word (the five bits and the two that pick the bank), as the emulator names it.
Emu::RegRef destination_register(const Emu& emu, std::uint32_t word) {
    const unsigned index = ((word >> 21) & 31) | (((word >> 2) & 3) << 5);
    return emu.regRef(index < 32 ? "vs" + std::to_string(32 + index) : "v" + std::to_string(index));
}

// vrlimi128 vD,vB,mask,shift: vB's words rotated left by `shift` words (x moves to w for one), and the words the mask names (8 is x, 1 is w) put into vD; the others stay.
bool rotate_insert(Emu&, Call& c, void*) {
    if (c.nIn < 4 || c.outSize != kN) return false;
    Byte old[kN], b[kN], r[kN];
    reversed(old, c.in[0], 4);
    reversed(b, c.in[1], 4);
    const unsigned mask = c.in[2][0] & 15, shift = c.in[3][0] & 3;
    for (unsigned i = 0; i < 4; ++i) vec::setLane(r, 4, i, ((mask >> (3 - i)) & 1) ? vec::getLane(b, 4, (i + shift) & 3) : vec::getLane(old, 4, i));
    reversed(c.out, r, 4);
    return true;
}

// The packed formats: how many fields, their widths, and whether each is signed (1), unsigned (0) or a half float (2), most significant field first (the order of the white paper's table).
struct D3dFormat {
    unsigned fields;
    unsigned bits[4];
    int kind[4];
    unsigned source[4];   // the element (0 x, 1 y, 2 z, 3 w) each field is made from, in the same order
    bool wide;            // 64 bits (z and w) rather than 32 (w)
};
const D3dFormat* d3d_format(unsigned type) {
    static const D3dFormat formats[7] = {
        {4, {8, 8, 8, 8}, {0, 0, 0, 0}, {3, 0, 1, 2}, false},         // VPACK_D3DCOLOR: W X Y Z
        {2, {16, 16, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, false},       // VPACK_NORMSHORT2: X Y
        {4, {2, 10, 10, 10}, {0, 1, 1, 1}, {3, 2, 1, 0}, false},      // VPACK_NORMPACKED32: W Z Y X
        {2, {16, 16, 0, 0}, {2, 2, 0, 0}, {0, 1, 0, 0}, false},       // VPACK_FLOAT16_2: X Y
        {4, {16, 16, 16, 16}, {1, 1, 1, 1}, {0, 1, 2, 3}, true},      // VPACK_NORMSHORT4: X Y Z W
        {4, {16, 16, 16, 16}, {2, 2, 2, 2}, {0, 1, 2, 3}, true},      // VPACK_FLOAT16_4
        {4, {4, 20, 20, 20}, {0, 1, 1, 1}, {3, 2, 1, 0}, true},       // VPACK_NORMPACKED64: W Z Y X
    };
    return type < 7 ? &formats[type] : nullptr;
}

// A float as a field: the signed and unsigned fields hold 3.0 + x 2^-22 (the integer in the low mantissa bits), clamped to the field's range; a half float rounds toward zero, keeps
// no infinity, no NaN and no denormal (131008 is its largest, a value too small for a normal one gives zero).
std::uint64_t pack_field(float value, unsigned bits, int kind) {
    if (kind == 2) {
        const std::uint32_t f = std::bit_cast<std::uint32_t>(value);
        const std::uint32_t sign = (f >> 16) & 0x8000u;
        if (std::isnan(value)) return 0;
        if (std::fabs(value) >= 131008.0f) return sign | 0x7FFFu;
        const int exponent = static_cast<int>((f >> 23) & 0xFF) - 127;
        if (exponent < -14) return sign;
        return sign | (static_cast<std::uint32_t>(exponent + 15) << 10) | ((f >> 13) & 0x3FFu);
    }
    const double unit = std::ldexp(1.0, -22);
    const std::int64_t lo = kind == 1 ? -(std::int64_t{1} << (bits - 1)) : 0, hi = kind == 1 ? (std::int64_t{1} << (bits - 1)) - 1 : (std::int64_t{1} << bits) - 1;
    double x = std::isnan(value) ? 0.0 : std::floor((static_cast<double>(value) - 3.0) / unit + 0.5);
    x = std::clamp(x, static_cast<double>(lo), static_cast<double>(hi));
    return static_cast<std::uint64_t>(static_cast<std::int64_t>(x)) & ((std::uint64_t{1} << bits) - 1);
}

float unpack_field(std::uint64_t field, unsigned bits, int kind) {
    if (kind == 2) {
        const std::uint32_t sign = (static_cast<std::uint32_t>(field) & 0x8000u) << 16;
        const unsigned exponent = (static_cast<unsigned>(field) >> 10) & 31, mantissa = static_cast<unsigned>(field) & 0x3FFu;
        if (exponent == 0) return std::bit_cast<float>(sign);   // (a denormal is a zero)
        return std::bit_cast<float>(sign | ((exponent - 15 + 127) << 23) | (mantissa << 13));
    }
    if (kind == 1) {
        const std::int64_t s = static_cast<std::int64_t>(field << (64 - bits)) >> (64 - bits);
        if (s == -(std::int64_t{1} << (bits - 1))) return std::bit_cast<float>(0x7FC00000u);   // (the most negative value is a NaN)
        return std::bit_cast<float>(static_cast<std::uint32_t>(0x40400000 + s));
    }
    return std::bit_cast<float>(static_cast<std::uint32_t>(0x3F800000u + field));
}

bool unpack_d3d(Emu&, Call& c, void*) {   // vupkd3d128 vD,vB,type: the packed data in vB's w (and z) element
    if (c.nIn < 2 || c.outSize != kN) return false;
    const D3dFormat* f = d3d_format((c.in[1][0] >> 2) & 7);   // (the data type is the high three bits of the five-bit field)
    if (f == nullptr) return false;
    Byte b[kN], r[kN] = {};
    reversed(b, c.in[0], 4);
    const std::uint64_t packed = f->wide ? (static_cast<std::uint64_t>(vec::getLane(b, 4, 2)) << 32) | vec::getLane(b, 4, 3) : vec::getLane(b, 4, 3);
    unsigned total = 0;
    for (unsigned i = 0; i < f->fields; ++i) total += f->bits[i];
    unsigned shift = total;
    for (unsigned i = 0; i < f->fields; ++i) {
        shift -= f->bits[i];
        const std::uint64_t field = (packed >> shift) & ((std::uint64_t{1} << f->bits[i]) - 1);
        vec::setLane(r, 4, f->source[i], std::bit_cast<std::uint32_t>(unpack_field(field, f->bits[i], f->kind[i])));
    }
    if (f->fields == 2) {   // the two-field formats fill z and w: 0.0 and 3.0 (1.0 for the half floats)
        vec::setLane(r, 4, 2, 0);
        vec::setLane(r, 4, 3, std::bit_cast<std::uint32_t>(f->kind[0] == 2 ? 1.0f : 3.0f));
    }
    reversed(c.out, r, 4);
    return true;
}

// vpkd3d128 vD,vB,type,mask select,shift: vB's floats packed and mask-inserted into vD (the packed word or words, moved left by `shift` words).
bool pack_d3d(Emu& emu, Call& c, void* x) {
    if (c.nIn < 4 || c.outSize != kN) return false;
    const D3dFormat* f = d3d_format(c.in[1][0] & 7);
    if (f == nullptr) return false;
    const unsigned select = c.in[2][0] & 3, shift = c.in[3][0] & 3;
    Byte b[kN], r[kN];
    reversed(b, c.in[0], 4);
    std::uint64_t packed = 0;
    unsigned total = 0;
    for (unsigned i = 0; i < f->fields; ++i) total += f->bits[i];
    unsigned at = total;
    for (unsigned i = 0; i < f->fields; ++i) {
        at -= f->bits[i];
        packed |= pack_field(std::bit_cast<float>(static_cast<std::uint32_t>(vec::getLane(b, 4, f->source[i]))), f->bits[i], f->kind[i]) << at;
    }
    // the destination as it was (the description's operation is not given it: it is read from the register the word names)
    Byte old[kN];
    const Ctx& context = ctx(x);
    if (context.regs == nullptr) return false;
    emu.readRegWide(destination_register(emu, context.word), old);
    reversed(r, old, 4);
    const std::uint32_t high = static_cast<std::uint32_t>(packed >> 32), low = static_cast<std::uint32_t>(packed);
    if (!f->wide || select == 1) {   // VPACK_32: the low word, at the word `shift` from the right
        vec::setLane(r, 4, 3 - shift, low);
    } else {   // VPACK_64LO, VPACK_64HI: the high word at 2 - shift, the low at 3 - shift (past the left end: the low word alone for LO, the high word alone, at w, for HI)
        if (shift < 3) {
            vec::setLane(r, 4, 2 - shift, high);
            vec::setLane(r, 4, 3 - shift, low);
        } else if (select == 2) {
            vec::setLane(r, 4, 0, low);
        } else {
            vec::setLane(r, 4, 3, high);
        }
    }
    reversed(c.out, r, 4);
    return true;
}

}  // namespace

void add_vector_entries(std::vector<pcode_ops::Entry>& out, Flavor flavor) {   // (declared in pcode_ops_ppc.hpp)
    using vec::Sign;
    constexpr Sign U = Sign::Unsigned, S = Sign::Signed;
    const auto add = [&](std::string_view name, pcode_ops::Handler h) { out.push_back({name, h}); };
    add("vectorAddCarryoutUnsignedWord", add_carry_word);
    add("vectorAddFloatingPoint", float_add);
    add("vectorAddSignedByteSaturate", add_saturate<1, S>);
    add("vectorAddSignedHalfWordSaturate", add_saturate<2, S>);
    add("vectorAddSignedWordSaturate", add_saturate<4, S>);
    add("vectorAddUnsignedByteSaturate", add_saturate<1, U>);
    add("vectorAddUnsignedHalfWordSaturate", add_saturate<2, U>);
    add("vectorAddUnsignedWordSaturate", add_saturate<4, U>);
    add("vectorLogicalAnd", vand);
    add("vectorLogicalAndWithComplement", vandc);
    add("vectorLogicalNOR", vnor);
    add("vectorLogicalOR", vor);
    add("vectorAverageSignedByte", average<1, S>);
    add("vectorAverageSignedHalfWord", average<2, S>);
    add("vectorAverageSignedWord", average<4, S>);
    add("vectorAverageUnsignedByte", average<1, U>);
    add("vectorAverageUnsignedHalfWord", average<2, U>);
    add("vectorAverageUnsignedWord", average<4, U>);
    add("vectorConvertFromSignedFixedPointWord", convert_from_signed);
    add("vectorConvertFromUnsignedFixedPointWord", convert_from_unsigned);
    add("vectorConvertToSignedFixedPointWordSaturate", convert_to_integer<S>);
    add("vectorConvertToUnsignedFixedPointWordSaturate", convert_to_integer<U>);
    add("vectorCompareBoundsFloatingPoint", float_compare_bounds);
    add("vectorCompareEqualToFloatingPoint", float_compare<vec::FpCmp::Eq>);
    add("vectorCompareEqualToUnsignedByte", compare_equal<1>);
    add("vectorCompareEqualToUnsignedHalfWord", compare_equal<2>);
    add("vectorCompareEqualToUnsignedWord", compare_equal<4>);
    add("vectorCompareGreaterThanOrEqualToFloatingPoint", float_compare<vec::FpCmp::Ge>);
    add("vectorCompareGreaterThanFloatingPoint", float_compare<vec::FpCmp::Gt>);
    add("vectorCompareGreaterThanSignedByte", compare_greater<1, S>);
    add("vectorCompareGreaterThanConditionRegisterSignedHalfWord", compare_greater<2, S>);   // (the description's odd name for vcmpgtsh)
    add("vectorCompareGreaterThanSignedWord", compare_greater<4, S>);
    add("vectorCompareGreaterThanUnsignedByte", compare_greater<1, U>);
    add("vectorCompareGreaterThanUnsignedHalfWord", compare_greater<2, U>);
    add("vectorCompareGreaterThanUnsignedWord", compare_greater<4, U>);
    add("vector2RaisedToTheExponentEstimateFloatingPoint", estimate<exp2_of>);
    add("vectorLog2EstimateFloatingPoint", estimate<log2_of>);
    add("vectorReciprocalEstimateFloatingPoint", estimate<reciprocal>);
    add("vectorReciprocalSquareRootEstimateFloatingPoint", estimate<reciprocal_sqrt>);
    add("vectorMultiplyAddFloatingPoint", float_multiply_add);
    add("vectorNegativeMultiplySubtractFloatingPoint", float_negative_multiply_subtract);
    add("vectorMaximumFloatingPoint", float_max);
    add("vectorMinimumFloatingPoint", float_min);
    add("vectorMaximumSignedByte", maximum<1, S>);
    add("vectorMaximumSignedHalfWord", maximum<2, S>);
    add("vectorMaximumSignedWord", maximum<4, S>);
    add("vectorMaximumUnsignedByte", maximum<1, U>);
    add("vectorMaximumUnsignedHalfWord", maximum<2, U>);
    add("vectorMaximumUnsignedWord", maximum<4, U>);
    add("vectorMinimumSignedByte", minimum<1, S>);
    add("vectorMinimumSignedHalfWord", minimum<2, S>);
    add("vectorMinimumSignedWord", minimum<4, S>);
    add("vectorMinimumUnsignedByte", minimum<1, U>);
    add("vectorMinimumUnsignedHalfWord", minimum<2, U>);
    add("vectorMinimumUnsignedWord", minimum<4, U>);
    add("vectorMultiplyHighAndAddSignedHalfWordSaturate", multiply_high_add<false>);
    add("vectorMultiplyHighRoundAndAddSignedHalfWordSaturate", multiply_high_add<true>);
    add("vectorMultiplyLowAndAddUnsignedHalfWordModulo", multiply_low_add);
    add("vectorMergeHighByte", merge<1, true>);
    add("vectorMergeHighHalfWord", merge<2, true>);
    add("vectorMergeHighWord", merge<4, true>);
    add("vectorMergeLowByte", merge<1, false>);
    add("vectorMergeLowHalfWord", merge<2, false>);
    add("vectorMergeLowWord", merge<4, false>);
    add("vectorMultiplySumMixedSignByteModulo", multiply_sum<1, S, U, false>);
    add("vectorMultiplySumSignedHalfWordModulo", multiply_sum<2, S, S, false>);
    add("vectorMultiplySumSignedHalfWordSaturate", multiply_sum<2, S, S, true>);
    add("vectorMultiplySumUnsignedByteModulo", multiply_sum<1, U, U, false>);
    add("vectorMultiplySumUnsignedHalfWordModulo", multiply_sum<2, U, U, false>);
    add("vectorMultiplySumUnsignedHalfWordSaturate", multiply_sum<2, U, U, true>);
    add("vectorMultiplyEvenSignedByte", multiply_half<1, S, true>);
    add("vectorMultiplyEvenSignedHalfWord", multiply_half<2, S, true>);
    add("vectorMultiplyEvenUnsignedByte", multiply_half<1, U, true>);
    add("vectorMultiplyEvenUnsignedHalfWord", multiply_half<2, U, true>);
    add("vectorMultiplyOddSignedByte", multiply_half<1, S, false>);
    add("vectorMultiplyOddSignedHalfWord", multiply_half<2, S, false>);
    add("vectorMultiplyOddUnsignedByte", multiply_half<1, U, false>);
    add("vectorMultiplyOddUnsignedHalfWord", multiply_half<2, U, false>);
    add("vectorPermute", permute);
    add("vectorPackPixel32", pack_pixel);
    add("vectorPackSignedHalfWordSignedSaturate", pack<2, vec::Narrow::SignedToSigned>);
    add("vectorPackSignedHalfWordUnsignedSaturate", pack<2, vec::Narrow::SignedToUnsigned>);
    add("vectorPackSignedWordSignedSaturate", pack<4, vec::Narrow::SignedToSigned>);
    add("vectorPackSignedWordUnsignedSaturate", pack<4, vec::Narrow::SignedToUnsigned>);
    add("vectorPackUnsignedHalfWordUnsignedModulo", pack<2, vec::Narrow::Truncate>);
    add("vectorPackUnsignedHalfWordUnsignedSaturate", pack<2, vec::Narrow::UnsignedToUnsigned>);
    add("vectorPackUnsignedWordUnsignedModulo", pack<4, vec::Narrow::Truncate>);
    add("vectorPackUnsignedWordUnsignedSaturate", pack<4, vec::Narrow::UnsignedToUnsigned>);
    add("vectorRoundToFloatingPointIntegerTowardMinusInfinity", float_round<vec::Round::TowardNegative>);
    add("vectorRoundToFloatingPointIntegerNearest", float_round<vec::Round::NearestEven>);
    add("vectorRoundToFloatingPointIntegerTowardPluInfinity", float_round<vec::Round::TowardPositive>);
    add("vectorRoundToFloatingPointIntegerTowardZero", float_round<vec::Round::TowardZero>);
    add("vectorRotateLeftIntegerByte", shift_each<1, vec::Shift::RotateLeft>);
    add("vectorRotateLeftIntegerHalfWord", shift_each<2, vec::Shift::RotateLeft>);
    add("vectorRotateLeftIntegerWord", shift_each<4, vec::Shift::RotateLeft>);
    add("vectorConditionalSelect", vsel);
    add("vectorShiftLeft", shift_left_bits);
    add("vectorShiftLeftIntegerByte", shift_each<1, vec::Shift::Left>);
    add("vectorShiftLeftIntegerHalfWord", shift_each<2, vec::Shift::Left>);
    add("vectorShiftLeftIntegerWord", shift_each<4, vec::Shift::Left>);
    add("vectorShiftLeftByOctet", shift_octets<true>);
    add("vectorShiftRight", shift_right_bits);
    add("vectorShiftRightAlgebraicByte", shift_each<1, vec::Shift::RightArithmetic>);
    add("vectorShiftRightAlgebraicHalfWord", shift_each<2, vec::Shift::RightArithmetic>);
    add("vectorShiftRightAlgebraicWord", shift_each<4, vec::Shift::RightArithmetic>);
    add("vectorShiftRightByte", shift_each<1, vec::Shift::RightLogical>);
    add("vectorShiftRightHalfWord", shift_each<2, vec::Shift::RightLogical>);
    add("vectorShiftRightWord", shift_each<4, vec::Shift::RightLogical>);
    add("vectorShiftRightByOctet", shift_octets<false>);
    add("vectorSplatByte", splat<1>);
    add("vectorSplatHalfWord", splat<2>);
    add("vectorSplatWord", splat<4>);
    add("vectorSplatImmediateSignedByte", splat_immediate<1>);
    add("vectorSplatImmediateSignedHalfWord", splat_immediate<2>);
    add("vectorSplatImmediateSignedWord", splat_immediate<4>);
    add("vectorSubtractCarryoutUnsignedWord", sub_carry_word);
    add("vectorSubtractFloatingPoint", float_sub);
    add("vectorSubtractSignedByteSaturate", sub_saturate<1, S>);
    add("vectorSubtractSignedHalfWordSaturate", sub_saturate<2, S>);
    add("vectorSubtractSignedWordSaturate", sub_saturate<4, S>);
    add("vectorSubtractUnsignedByteModulo", sub_wrap<1>);
    add("vectorSubtractUnsignedByteSaturate", sub_saturate<1, U>);
    add("vectorSubtractUnsignedHalfWordSaturate", sub_saturate<2, U>);
    add("vectorSubtractUnsignedWordModulo", sub_wrap<4>);
    add("vectorSubtractUnsignedWordSaturate", sub_saturate<4, U>);
    add("vectorSumAcrossSignedWordSaturate", sum_all);
    add("vectorSumAcrossPartialSignedWordSaturate", sum_pairs);
    add("vectorSumAcrossPartialSignedByteSaturate", sum_into_words<1, true>);
    add("vectorSumAcrossPartialSignedHalfWordSaturate", sum_into_words<2, true>);
    add("vectorSumAcrossPartialUnsignedByteSaturate", sum_into_words<1, false>);
    add("vectorUnpackHighPixel16", unpack_pixel<true>);
    add("vectorUnpackHighSignedByte", unpack_signed<1, true>);
    add("vectorUnpackHighSignedHalfWord", unpack_signed<2, true>);
    add("vectorUnpackLowPixel16", unpack_pixel<false>);
    add("vectorUnpackLowSignedByte", unpack_signed<1, false>);
    add("vectorUnpackLowSignedHalfWord", unpack_signed<2, false>);
    add("moveFromVectorStatusAndControlRegister", move_from_vscr);
    add("moveToVectorStatusAndControlRegister", move_to_vscr);
    add("loadVectorForShiftLeft", shift_permute<true>);
    add("loadVectorElementHalfWordIndexed", load_element<2>);
    add("loadVectorElementWordIndexed", load_element<4>);
    add("loadVectorIndexedLRU", load_vector_lru);
    add("storeVectorElementByteIndexed", store_element<1>);
    add("storeVectorElementHalfWordIndexed", store_element<2>);
    add("storeVectorElementWordIndexed", store_element<4>);
    add("dataStreamStop", nothing);
    add("dataStreamStopAll", nothing);
    add("dataStreamTouch", nothing);
    add("dataStreamTouchSoon", nothing);
    add("dataStreamTouchForStore", nothing);
    add("dataStreamTouchForStoreTransient", nothing);
    if (flavor == Flavor::Xenon) {
        add("vectorMultiplyFloatingPoint", float_mul);
        add("vectorMultiplyAddFloatingPointToD", float_multiply_add_to_d);
        add("vectorMultiplySum3FloatingPoint", float_dot<3>);
        add("vectorMultiplySum4FloatingPoint", float_dot<4>);
        add("vectorCompareSetCR6", compare_set_cr6);
        add("loadVectorLeftIndexed", load_left_right<false>);
        add("loadVectorLeftIndexedLRU", load_left_right<false>);
        add("loadVectorRightIndexed", load_left_right<true>);
        add("loadVectorRightIndexedLRU", load_left_right<true>);
        add("storeVectorLeftIndexed", store_left_right<false>);
        add("storeVectorLeftIndexedLRU", store_left_right<false>);
        add("storeVectorRightIndexed", store_left_right<true>);
        add("storeVectorRightIndexedLRU", store_left_right<true>);
        add("vectorPermuteWordImmediate", permute_word_immediate);
        add("vectorRotateLeftImmediateAndMaskInsert", rotate_insert);
        add("vectorPackD3D", pack_d3d);
        add("vectorUnpackD3D", unpack_d3d);
    }
}

// The AltiVec instructions the compiled descriptions have no instruction for (vaddubm, vadduhm and vsubuhm: their p-code is spread over sub-constructors the description's compiler
// does not take): the adapter's bad-instruction hook runs them here, from the word the adapter put in the context.
bool serve_undecoded(Emu& emu, const Regs& regs, const VectorContext& context, std::uint64_t pc) {
    const std::uint32_t w = context.word;
    if ((w >> 26) != 4) return false;
    Byte at[4];
    emu.read(pc, at, 4);
    if ((static_cast<std::uint32_t>(at[0]) << 24 | at[1] << 16 | at[2] << 8 | at[3]) != w) return false;   // (the word in the context is the one the emulator is at)
    const unsigned xo = w & 0x7FF, d = (w >> 21) & 31, a = (w >> 16) & 31, b = (w >> 11) & 31;
    if (xo != 0 && xo != 64 && xo != 1088) return false;
    Byte x[kN], y[kN], r[kN];
    emu.readRegWide(regs.vr[a], x);
    emu.readRegWide(regs.vr[b], y);
    if (xo == 0) vec::add(r, x, y, kN, 1);
    else if (xo == 64) vec::add(r, x, y, kN, 2);
    else vec::sub(r, x, y, kN, 2);
    emu.writeRegWide(regs.vr[d], r);
    emu.setPc(pc + 4);
    return true;
}

}  // namespace cb::emulate::pcode_ops_ppc
