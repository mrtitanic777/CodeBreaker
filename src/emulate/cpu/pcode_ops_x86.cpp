// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/cpu/pcode_ops_x86.cpp -- see pcode_ops_x86.hpp. The behaviour of each named operation is the Intel manual's, written over the lane library (engine/pcode_vector.hpp); where Unicorn
// (QEMU's x86) is the oracle and the manual leaves a choice, the choice is Unicorn's and the comment says so.
//
// The description gives an operation its operands as the instruction writes them: `paddsb(dest, src)` is (the destination's old value, the source), so a handler reads in[0] and
// in[1]; a scalar or a vector instruction of the VEX forms gives the same shape (the first source is vvvv). A value of 8 bytes is an MMX register, of 16 an XMM, of 32 a YMM: the
// operations that work on each 128-bit lane of a YMM register apart are called for each 16 bytes by `groups`.
#include "emulate/cpu/pcode_ops_x86.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <cstring>

#include "emulate/cpu/pcode_vector.hpp"

namespace cb::emulate::pcode_ops_x86 {

namespace vec = cb::emulate::vec;

namespace {

using Emu = pcode_ops::Emu;
using Call = Emu::UserOpCall;
using Byte = vec::Byte;
using Sign = vec::Sign;
using u64 = std::uint64_t;
using i64 = std::int64_t;

// ---- MXCSR: the rounding mode and the denormal rules in, the exception flags out
vec::FpEnv fpEnv(Emu& emu, void* context) {
    const Context& c = *static_cast<const Context*>(context);
    return environment(c.has_mxcsr ? emu.reg(c.mxcsr) : 0);
}
void raise(Emu& emu, void* context, unsigned flags) {
    const Context& c = *static_cast<const Context*>(context);
    if (!c.has_mxcsr || flags == 0) return;
    emu.setReg(c.mxcsr, emu.reg(c.mxcsr) | exception_bits(flags));
}

// ---- shapes
bool binary(const Call& c) { return c.nIn >= 2 && c.outSize != 0 && c.inSize[0] == c.outSize && c.inSize[1] == c.outSize; }
bool unaryLast(const Call& c) { return c.nIn >= 1 && c.outSize != 0 && c.inSize[c.nIn - 1] == c.outSize; }
std::size_t groupOf(const Call& c) { return std::min<std::size_t>(16, c.outSize); }
template <class F>
void groups(const Call& c, F f) {
    const std::size_t g = groupOf(c);
    for (std::size_t at = 0; at < c.outSize; at += g) f(at, g);
}
// The last input as a small number (an immediate: one byte).
unsigned imm(const Call& c) { return c.in[c.nIn - 1][0]; }

// ---- integer lanes, element by element
#define BINARY(NAME, EXPR)                         \
    bool NAME(Emu&, Call& c, void*) {              \
        if (!binary(c)) return false;              \
        Byte* o = c.out;                           \
        const Byte* a = c.in[0];                   \
        const Byte* b = c.in[1];                   \
        const std::size_t n = c.outSize;           \
        (void)a, (void)b, (void)n;                 \
        EXPR;                                      \
        return true;                               \
    }
#define UNARY(NAME, EXPR)                          \
    bool NAME(Emu&, Call& c, void*) {              \
        if (!unaryLast(c)) return false;           \
        Byte* o = c.out;                           \
        const Byte* a = c.in[c.nIn - 1];           \
        const std::size_t n = c.outSize;           \
        EXPR;                                      \
        return true;                               \
    }

BINARY(paddsb, vec::addSat(o, a, b, n, 1, Sign::Signed))
BINARY(paddsw, vec::addSat(o, a, b, n, 2, Sign::Signed))
BINARY(paddusb, vec::addSat(o, a, b, n, 1, Sign::Unsigned))
BINARY(paddusw, vec::addSat(o, a, b, n, 2, Sign::Unsigned))
BINARY(psubsb, vec::subSat(o, a, b, n, 1, Sign::Signed))
BINARY(psubsw, vec::subSat(o, a, b, n, 2, Sign::Signed))
BINARY(psubusb, vec::subSat(o, a, b, n, 1, Sign::Unsigned))
BINARY(psubusw, vec::subSat(o, a, b, n, 2, Sign::Unsigned))
BINARY(pavgb, vec::halvingAdd(o, a, b, n, 1, Sign::Unsigned, true))
BINARY(pavgw, vec::halvingAdd(o, a, b, n, 2, Sign::Unsigned, true))
BINARY(pmulhw, vec::mulHigh(o, a, b, n, 2, Sign::Signed))
BINARY(pmulhuw, vec::mulHigh(o, a, b, n, 2, Sign::Unsigned))
BINARY(pmulld, vec::mul(o, a, b, n, 4))
BINARY(pmullw, vec::mul(o, a, b, n, 2))
BINARY(pmullq, vec::mul(o, a, b, n, 8))
BINARY(pmulhrsw, vec::mulHighRoundHalfUp(o, a, b, n, 2))
BINARY(pmuldq, vec::mulEvenOdd(o, a, b, n, 4, Sign::Signed, false))
BINARY(pmuludq, vec::mulEvenOdd(o, a, b, n, 4, Sign::Unsigned, false))
BINARY(pmaddwd, vec::multiplyAdd(o, nullptr, a, b, n, 2, 4, Sign::Signed, Sign::Signed, false))
BINARY(pmaddubsw, vec::multiplyAdd(o, nullptr, a, b, n, 1, 2, Sign::Unsigned, Sign::Signed, true))
BINARY(psadbw, vec::sumAbsDiff(o, a, b, n))
BINARY(pmaxsb, vec::max(o, a, b, n, 1, Sign::Signed))
BINARY(pmaxsw, vec::max(o, a, b, n, 2, Sign::Signed))
BINARY(pmaxsd, vec::max(o, a, b, n, 4, Sign::Signed))
BINARY(pmaxsq, vec::max(o, a, b, n, 8, Sign::Signed))
BINARY(pmaxub, vec::max(o, a, b, n, 1, Sign::Unsigned))
BINARY(pmaxuw, vec::max(o, a, b, n, 2, Sign::Unsigned))
BINARY(pmaxud, vec::max(o, a, b, n, 4, Sign::Unsigned))
BINARY(pmaxuq, vec::max(o, a, b, n, 8, Sign::Unsigned))
BINARY(pminsb, vec::min(o, a, b, n, 1, Sign::Signed))
BINARY(pminsw, vec::min(o, a, b, n, 2, Sign::Signed))
BINARY(pminsd, vec::min(o, a, b, n, 4, Sign::Signed))
BINARY(pminsq, vec::min(o, a, b, n, 8, Sign::Signed))
BINARY(pminub, vec::min(o, a, b, n, 1, Sign::Unsigned))
BINARY(pminuw, vec::min(o, a, b, n, 2, Sign::Unsigned))
BINARY(pminud, vec::min(o, a, b, n, 4, Sign::Unsigned))
BINARY(pminuq, vec::min(o, a, b, n, 8, Sign::Unsigned))
BINARY(paddb, vec::add(o, a, b, n, 1))
BINARY(paddw, vec::add(o, a, b, n, 2))
BINARY(paddd, vec::add(o, a, b, n, 4))
BINARY(paddq, vec::add(o, a, b, n, 8))
BINARY(psubb, vec::sub(o, a, b, n, 1))
BINARY(psubw, vec::sub(o, a, b, n, 2))
BINARY(psubd, vec::sub(o, a, b, n, 4))
BINARY(psubq, vec::sub(o, a, b, n, 8))
BINARY(pcmpeqb, vec::compare(o, a, b, n, 1, vec::Cmp::Eq, Sign::Signed))
BINARY(pcmpeqw, vec::compare(o, a, b, n, 2, vec::Cmp::Eq, Sign::Signed))
BINARY(pcmpeqd, vec::compare(o, a, b, n, 4, vec::Cmp::Eq, Sign::Signed))
BINARY(pcmpeqq, vec::compare(o, a, b, n, 8, vec::Cmp::Eq, Sign::Signed))
BINARY(pcmpgtb, vec::compare(o, a, b, n, 1, vec::Cmp::Gt, Sign::Signed))
BINARY(pcmpgtw, vec::compare(o, a, b, n, 2, vec::Cmp::Gt, Sign::Signed))
BINARY(pcmpgtd, vec::compare(o, a, b, n, 4, vec::Cmp::Gt, Sign::Signed))
BINARY(pcmpgtq, vec::compare(o, a, b, n, 8, vec::Cmp::Gt, Sign::Signed))
BINARY(pand, vec::bitAnd(o, a, b, n))
BINARY(pandn, vec::bitAndNot(o, b, a, n))   // (PANDN: NOT the destination AND the source)
BINARY(por, vec::bitOr(o, a, b, n))
BINARY(pxor, vec::bitXor(o, a, b, n))
UNARY(pabsb, vec::abs(o, a, n, 1, false))
UNARY(pabsw, vec::abs(o, a, n, 2, false))
UNARY(pabsd, vec::abs(o, a, n, 4, false))
UNARY(pabsq, vec::abs(o, a, n, 8, false))

// PSIGNB/W/D: the source's sign decides: negative negates the destination, zero clears it, positive keeps it.
template <unsigned EW>
bool psign(Emu&, Call& c, void*) {
    if (!binary(c)) return false;
    for (std::size_t i = 0; i < c.outSize; i += EW) {
        const i64 x = vec::signExtend(vec::load(c.in[0] + i, EW), EW), s = vec::signExtend(vec::load(c.in[1] + i, EW), EW);
        vec::store(c.out + i, EW, s < 0 ? static_cast<u64>(-x) : s == 0 ? 0 : static_cast<u64>(x));
    }
    return true;
}

// Horizontal add and subtract: the pairs of the first operand fill the low half of each 128-bit group, the pairs of the second the high half.
enum class Horizontal { Add, Sub, AddSat, SubSat };
template <unsigned EW, Horizontal H>
bool horizontal(Emu&, Call& c, void*) {
    if (!binary(c)) return false;
    const Byte* const first = c.in[c.nIn - 2];   // (the VEX forms of PHSUBW and PHSUBD also give the destination's old value in front: the last two are the sources)
    const Byte* const second = c.in[c.nIn - 1];
    const i64 hi = (i64{1} << (8 * EW - 1)) - 1, lo = -hi - 1;
    groups(c, [&](std::size_t at, std::size_t g) {
        const std::size_t half = g / EW / 2;
        for (std::size_t k = 0; k < g / EW; ++k) {
            const Byte* src = k < half ? first + at : second + at;
            const std::size_t pair = (k < half ? k : k - half) * 2;
            const i64 x = vec::signExtend(vec::load(src + pair * EW, EW), EW), y = vec::signExtend(vec::load(src + (pair + 1) * EW, EW), EW);
            i64 r = (H == Horizontal::Add || H == Horizontal::AddSat) ? x + y : x - y;
            if (H == Horizontal::AddSat || H == Horizontal::SubSat) r = std::clamp(r, lo, hi);
            vec::store(c.out + at + k * EW, EW, static_cast<u64>(r));
        }
    });
    return true;
}

// Packing: the first operand's lanes narrowed fill the low half of each 128-bit group, the second's the high half.
template <unsigned EW, vec::Narrow M>
bool pack(Emu&, Call& c, void*) {
    if (!binary(c)) return false;
    groups(c, [&](std::size_t at, std::size_t g) {
        vec::narrow(c.out + at, c.in[0] + at, g, EW, M);
        vec::narrow(c.out + at + g / 2, c.in[1] + at, g, EW, M);
    });
    return true;
}

// The unpack of 64-bit lanes, per 128-bit group.
template <unsigned EW, bool High>
bool unpack(Emu&, Call& c, void*) {
    if (!binary(c)) return false;
    groups(c, [&](std::size_t at, std::size_t g) { vec::zip(c.out + at, c.in[0] + at, c.in[1] + at, g, EW, High); });
    return true;
}

// PSHUFB: the first operand shuffled by the second's bytes (bit 7 clears; the low four bits, three for an MMX register, pick within the 128-bit group).
bool pshufb(Emu&, Call& c, void*) {
    if (!binary(c)) return false;
    vec::Lookup mode;
    mode.indexMask = c.outSize == 8 ? 0x07 : 0x0F;
    mode.bit7Zero = true;
    mode.groupBytes = groupOf(c);
    vec::tableLookup(c.out, c.in[0], c.outSize, c.in[1], c.outSize, mode);
    return true;
}

// PSHUFD, PSHUFHW, PSHUFLW, PSHUFW: four lanes picked by an immediate's two-bit fields. `first`/`count` say which lanes of the group are shuffled.
template <unsigned EW, int Part>   // Part: 0 all lanes of the group of four; 1 the low four 16-bit lanes (pshuflw); 2 the high four (pshufhw)
bool pshuf(Emu&, Call& c, void*) {
    if (c.nIn < 2 || c.outSize == 0 || c.inSize[c.nIn - 2] != c.outSize) return false;
    const Byte* a = c.in[c.nIn - 2];
    const unsigned k = imm(c);
    std::memcpy(c.out, a, c.outSize);
    const std::size_t g = EW == 4 ? 16 : groupOf(c);
    for (std::size_t at = 0; at < c.outSize; at += (EW == 4 ? 16 : groupOf(c))) {
        const std::size_t base = at + (Part == 2 ? 8 : 0);
        for (unsigned i = 0; i < 4; ++i) std::memcpy(c.out + base + i * EW, a + base + ((k >> (2 * i)) & 3) * EW, EW);
    }
    (void)g;
    return true;
}

// Variable and immediate shifts of a whole lane by a count (PSLLW, PSRAW...): a 64-bit count read from the low half of an XMM or MMX operand, or an immediate byte.
template <unsigned EW, vec::Shift S>
bool shiftBy(Emu&, Call& c, void*) {
    if (c.nIn < 2 || c.outSize == 0 || c.inSize[0] != c.outSize) return false;
    u64 count = 0;
    if (c.inSize[1] == 1) {
        count = c.in[1][0];
    } else {
        const std::size_t have = std::min<std::size_t>(c.inSize[1], 16);
        for (std::size_t i = 0; i < have && i < 8; ++i) count |= static_cast<u64>(c.in[1][i]) << (8 * i);
        for (std::size_t i = 8; i < have; ++i)
            if (c.in[1][i] != 0) count = ~u64{0};   // (a count of 2^64 or more)
    }
    vec::shiftImm(c.out, c.in[0], c.outSize, EW, S, count, vec::Count::Saturate);
    return true;
}
// A shift of every lane by the same lane of the count vector (VPSLLVD ...).
template <unsigned EW, vec::Shift S>
bool shiftVariable(Emu&, Call& c, void*) {
    if (!binary(c)) return false;
    vec::shiftVar(c.out, c.in[0], c.in[1], c.outSize, EW, S, vec::Count::Saturate);
    return true;
}
// PSLLDQ and PSRLDQ: a shift of each 128-bit group by an immediate number of bytes.
template <bool Left>
bool shiftBytes(Emu&, Call& c, void*) {
    if (c.nIn < 2 || c.outSize == 0 || c.inSize[c.nIn - 2] != c.outSize) return false;
    const unsigned count = std::min(imm(c), 16u);
    vec::shiftBytes(c.out, c.in[c.nIn - 2], c.outSize, groupOf(c), count, Left);
    return true;
}
// PALIGNR: the pair of 128-bit values (the first operand above) shifted right by an immediate number of bytes.
bool palignr(Emu&, Call& c, void*) {
    if (c.nIn < 3 || !binary(c)) return false;
    const unsigned count = imm(c);
    groups(c, [&](std::size_t at, std::size_t g) {
        Byte pair[32] = {};
        std::memcpy(pair, c.in[1] + at, g);
        std::memcpy(pair + g, c.in[0] + at, g);
        for (std::size_t i = 0; i < g; ++i) c.out[at + i] = count + i < 2 * g ? pair[count + i] : 0;
    });
    return true;
}

// ---- widening of the low lanes (PMOVSX*, PMOVZX*): `FromEw` bytes of each lane become `ToEw`; the source is the low part of the operand.
template <unsigned FromEw, unsigned ToEw, Sign S>
bool extend(Emu&, Call& c, void*) {
    if (c.nIn < 1 || c.outSize == 0) return false;
    const Byte* a = c.in[c.nIn - 1];
    const std::size_t lanes = c.outSize / ToEw;
    if (lanes * FromEw > c.inSize[c.nIn - 1]) return false;
    for (std::size_t i = 0; i < lanes; ++i) {
        const u64 v = vec::load(a + i * FromEw, FromEw);
        vec::store(c.out + i * ToEw, ToEw, S == Sign::Signed ? static_cast<u64>(vec::signExtend(v, FromEw)) : v);
    }
    return true;
}

// ---- blends
template <unsigned EW>   // by the immediate's bits (PBLENDW, BLENDPS, BLENDPD, VPBLENDD): a lane of the second operand where the bit is set
bool blendImm(Emu&, Call& c, void*) {
    if (c.nIn < 3 || !binary(c)) return false;
    const unsigned k = imm(c);
    const std::size_t lanes = EW == 2 ? 8 : 64;   // (PBLENDW reuses the immediate's eight bits for each 128-bit group; BLENDPS, BLENDPD and VPBLENDD go on to the next bits)
    for (std::size_t i = 0; i < c.outSize / EW; ++i) std::memcpy(c.out + i * EW, (((k >> (i % lanes)) & 1) != 0 ? c.in[1] : c.in[0]) + i * EW, EW);
    return true;
}
template <unsigned EW>   // by the top bit of each lane of a third operand (PBLENDVB, BLENDVPS, BLENDVPD)
bool blendMask(Emu&, Call& c, void*) {
    if (c.nIn < 3 || !binary(c) || c.inSize[2] != c.outSize) return false;
    vec::blend(c.out, c.in[0], c.in[1], c.in[2], c.outSize, EW);
    return true;
}

// ---- moving bits out
template <unsigned EW>
bool moveMask(Emu&, Call& c, void*) {   // MOVMSKPS, MOVMSKPD, PMOVMSKB: the top bit of each lane
    if (c.nIn < 1 || c.outSize == 0) return false;
    const std::size_t n = c.inSize[c.nIn - 1];
    vec::store(c.out, std::min<unsigned>(c.outSize, 8), vec::moveMask(c.in[c.nIn - 1], n, EW));
    return true;
}
bool crc32c(Emu&, Call& c, void*) {   // CRC32 r32, r/m: the Castagnoli polynomial folded over 1, 2, 4 or 8 bytes of the second operand
    if (c.nIn < 2) return false;
    u64 data = 0;
    for (std::size_t i = 0; i < c.inSize[1] && i < 8; ++i) data |= static_cast<u64>(c.in[1][i]) << (8 * i);
    const u64 acc = vec::load(c.in[0], std::min<unsigned>(c.inSize[0], 4));
    vec::store(c.out, std::min<unsigned>(c.outSize, 8), vec::crc32Update(static_cast<std::uint32_t>(acc), data, c.inSize[1], true));
    return true;
}

// MOVBE: the bytes of the operand in the other order.
bool swapBytes(Emu&, Call& c, void*) {
    if (c.nIn < 1 || c.outSize == 0 || c.outSize != c.inSize[c.nIn - 1]) return false;
    for (std::size_t i = 0; i < c.outSize; ++i) c.out[i] = c.in[c.nIn - 1][c.outSize - 1 - i];
    return true;
}

// ---- floating point
template <unsigned EW, vec::Fp2 Op>
bool fpBinary(Emu& emu, Call& c, void* ctx) {
    if (!binary(c)) return false;
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    vec::fp2(c.out, c.in[0], c.in[1], c.outSize, EW, Op, env, flags);
    raise(emu, ctx, flags);
    return true;
}
template <unsigned EW, vec::Fp1 Op>
bool fpUnary(Emu& emu, Call& c, void* ctx) {
    if (!unaryLast(c)) return false;
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    vec::fp1(c.out, c.in[c.nIn - 1], c.outSize, EW, Op, env, flags);
    raise(emu, ctx, flags);
    return true;
}
// A scalar instruction (SQRTSS, ROUNDSD ...) changes the lowest lane and leaves the rest of the destination: its operands are the whole destination and a source that may be
// a lane of memory.
template <unsigned EW, vec::Fp1 Op>
bool fpUnaryScalar(Emu& emu, Call& c, void* ctx) {
    if (c.nIn < 2 || c.outSize < EW || c.inSize[0] != c.outSize || c.inSize[c.nIn - 1] < EW) return false;
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    std::memcpy(c.out, c.in[0], c.outSize);
    vec::fp1(c.out, c.in[c.nIn - 1], EW, EW, Op, env, flags);
    raise(emu, ctx, flags);
    return true;
}
template <unsigned EW, vec::Fp2 Op>
bool fpBinaryScalar(Emu& emu, Call& c, void* ctx) {
    if (c.nIn < 2 || c.outSize < EW || c.inSize[0] != c.outSize || c.inSize[1] < EW) return false;
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    std::memcpy(c.out, c.in[0], c.outSize);
    vec::fp2(c.out, c.in[0], c.in[1], EW, EW, Op, env, flags);
    raise(emu, ctx, flags);
    return true;
}
// ROUNDPS, ROUNDPD, ROUNDSS, ROUNDSD: the immediate's bit 2 says to use MXCSR's mode, bits 0 and 1 give one, bit 3 suppresses the inexact exception.
vec::FpEnv roundEnv(Emu& emu, void* ctx, unsigned k) {
    vec::FpEnv env = fpEnv(emu, ctx);
    if ((k & 4) == 0) {
        static constexpr vec::Round kRound[4] = {vec::Round::NearestEven, vec::Round::TowardNegative, vec::Round::TowardPositive, vec::Round::TowardZero};
        env.round = kRound[k & 3];
    }
    return env;
}
template <unsigned EW, bool Scalar>
bool roundOp(Emu& emu, Call& c, void* ctx) {
    if (Scalar ? (c.nIn < 3 || c.outSize < EW || c.inSize[0] != c.outSize || c.inSize[1] < EW) : (c.nIn < 2 || c.outSize < EW || c.inSize[c.nIn - 2] != c.outSize)) return false;
    const unsigned k = imm(c);
    const vec::FpEnv env = roundEnv(emu, ctx, k);
    unsigned flags = 0;
    const vec::Fp1 op = (k & 8) != 0 ? vec::Fp1::RoundToInt : vec::Fp1::RoundToIntExact;
    if (Scalar) {   // (ROUNDSS xmm1, xmm2/m32, imm: the destination's other lanes stay; the description names the destination and the source)
        if (c.nIn < 3) return false;
        std::memcpy(c.out, c.in[0], c.outSize);
        vec::fp1(c.out, c.in[1], EW, EW, op, env, flags);
    } else {
        vec::fp1(c.out, c.in[c.nIn - 2], c.outSize, EW, op, env, flags);
    }
    const std::size_t n = Scalar ? EW : c.outSize;
    const Byte* source = Scalar ? c.in[1] : c.in[c.nIn - 2];
    for (std::size_t i = 0; i < n; i += EW) {   // (a negative value rounded to zero is a negative zero: the library gives a positive one)
        const bool negative = (source[i + EW - 1] & 0x80) != 0;
        bool zero = true;
        for (std::size_t b = 0; b + 1 < EW; ++b) zero = zero && c.out[i + b] == 0;
        if (zero && (c.out[i + EW - 1] & 0x7F) == 0 && negative) c.out[i + EW - 1] |= 0x80;
    }
    if ((k & 8) != 0) flags &= ~vec::kInexact;   // (bit 3 of the immediate: the precision exception is not raised)
    raise(emu, ctx, flags);
    return true;
}

// CMPPS, CMPPD, CMPSS, CMPSD: the predicate is the immediate's low three bits (VEX forms take five: the low three and the quiet or signaling variants).
struct Predicate {
    vec::FpCmp cmp;
    bool negate;
    bool signaling;
};
Predicate predicateOf(unsigned k) {
    static constexpr Predicate kPredicates[8] = {{vec::FpCmp::Eq, false, false}, {vec::FpCmp::Lt, false, true},  {vec::FpCmp::Le, false, true},  {vec::FpCmp::Unordered, false, false},
                                                 {vec::FpCmp::Ne, false, false}, {vec::FpCmp::Lt, true, true}, {vec::FpCmp::Le, true, true}, {vec::FpCmp::Ordered, false, false}};
    return kPredicates[k & 7];
}
template <unsigned EW, bool Scalar>
bool cmpFloat(Emu& emu, Call& c, void* ctx) {
    if (c.nIn < 3 || c.outSize < EW || c.inSize[0] != c.outSize || c.inSize[1] < (Scalar ? EW : c.outSize)) return false;
    const unsigned k = imm(c);
    if (k >= 8) {   // (outside VEX the predicate is three bits: the processor and Unicorn refuse the instruction)
        static_cast<Context*>(ctx)->invalid_instruction = true;
        return true;
    }
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    const std::size_t n = Scalar ? EW : c.outSize;
    std::memcpy(c.out, c.in[0], c.outSize);
    const Predicate p = predicateOf(k);
    vec::fpCompare(c.out, c.in[0], c.in[1], n, EW, p.cmp, p.signaling, env, flags);
    if (p.negate)
        for (std::size_t i = 0; i < n; ++i) c.out[i] = static_cast<Byte>(~c.out[i]);
    raise(emu, ctx, flags);
    return true;
}

// VCMPPS, VCMPPD, VCMPSS, VCMPSD: the immediate's five bits pick one of 32 predicates (the low three of them are the legacy ones; the others are the same tests with the other
// choice of signaling, and the unordered-true or ordered-false variants).
template <unsigned EW, bool Scalar>
bool cmpFloatVex(Emu& emu, Call& c, void* ctx) {
    if (c.nIn < 3 || c.outSize < EW || c.inSize[0] != c.outSize || c.inSize[1] < (Scalar ? EW : c.outSize)) return false;
    const unsigned p = imm(c) & 31;
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    const std::size_t n = Scalar ? EW : c.outSize;
    std::memcpy(c.out, c.in[0], c.outSize);
    static constexpr bool kSignaling[16] = {false, true, true, false, false, true, true, false, false, true, true, false, false, true, true, false};
    const bool signaling = kSignaling[p & 15] != (p >= 16);
    const auto compare = [&](Byte* out, vec::FpCmp cmp) { vec::fpCompare(out, c.in[0], c.in[1], n, EW, cmp, signaling, env, flags); };
    Byte other[32];
    switch (p & 15) {
        case 0: compare(c.out, vec::FpCmp::Eq); break;
        case 1: compare(c.out, vec::FpCmp::Lt); break;
        case 2: compare(c.out, vec::FpCmp::Le); break;
        case 3: compare(c.out, vec::FpCmp::Unordered); break;
        case 4: compare(c.out, vec::FpCmp::Ne); break;
        case 5: compare(c.out, vec::FpCmp::Lt); vec::bitNot(c.out, c.out, n); break;
        case 6: compare(c.out, vec::FpCmp::Le); vec::bitNot(c.out, c.out, n); break;
        case 7: compare(c.out, vec::FpCmp::Ordered); break;
        case 8: compare(c.out, vec::FpCmp::Eq); compare(other, vec::FpCmp::Unordered); vec::bitOr(c.out, c.out, other, n); break;
        case 9: compare(c.out, vec::FpCmp::Ge); vec::bitNot(c.out, c.out, n); break;
        case 10: compare(c.out, vec::FpCmp::Gt); vec::bitNot(c.out, c.out, n); break;
        case 11: std::memset(c.out, 0, n); break;
        case 12: compare(c.out, vec::FpCmp::Ne); compare(other, vec::FpCmp::Ordered); vec::bitAnd(c.out, c.out, other, n); break;
        case 13: compare(c.out, vec::FpCmp::Ge); break;
        case 14: compare(c.out, vec::FpCmp::Gt); break;
        default: std::memset(c.out, 0xFF, n); break;
    }
    raise(emu, ctx, flags);
    return true;
}

// RCPPS, RSQRTPS and their scalar forms: QEMU (Unicorn) computes the exact quotient and square root; the processors' own are approximations good to 12 bits.
template <bool Sqrt, bool Scalar>
bool reciprocal(Emu& emu, Call& c, void* ctx) {
    if (c.nIn < 1 || c.outSize < 4) return false;
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    const std::size_t n = Scalar ? 4 : c.outSize;
    const Byte* src = c.in[c.nIn - 1];
    if (c.inSize[c.nIn - 1] < n) return false;
    if (Scalar) {
        if (c.nIn < 2 || c.inSize[0] != c.outSize) return false;
        std::memcpy(c.out, c.in[0], c.outSize);
    }
    Byte root[32], one[32];
    std::memcpy(root, src, n);
    if (Sqrt) vec::fp1(root, src, n, 4, vec::Fp1::Sqrt, env, flags);
    for (std::size_t i = 0; i < n; i += 4) vec::store(one + i, 4, 0x3F800000);
    vec::fp2(c.out, one, root, n, 4, vec::Fp2::Div, env, flags);
    raise(emu, ctx, flags);
    return true;
}

// ---- conversions
template <unsigned From, unsigned To>
bool convertFloat(Emu& emu, Call& c, void* ctx) {
    if (c.nIn < 1 || c.outSize == 0) return false;
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    vec::floatConvert(c.out, c.in[c.nIn - 1], std::min<std::size_t>(c.inSize[c.nIn - 1], c.outSize * From / To), From, To, env, flags);
    raise(emu, ctx, flags);
    return true;
}

// ---- moves the description leaves to a named operation
UNARY(copyLast, std::memcpy(o, a, n))   // LDDQU, MOVNTDQA
bool lowQuadword(Emu&, Call& c, void*) {   // MOVDQ2Q: the low 64 bits of the XMM register
    if (c.nIn < 1 || c.outSize != 8 || c.inSize[c.nIn - 1] < 8) return false;
    std::memcpy(c.out, c.in[c.nIn - 1], 8);
    return true;
}
// MASKMOVDQU and MASKMOVQ: the bytes of the first operand whose mask byte (the second) has its top bit set are stored at the address (the third).
bool maskedMove(Emu& emu, Call& c, void*) {
    if (c.nIn < 3 || c.inSize[0] != c.inSize[1] || c.inSize[2] > 8) return false;
    u64 address = 0;
    for (unsigned i = 0; i < c.inSize[2]; ++i) address |= static_cast<u64>(c.in[2][i]) << (8 * i);
    for (unsigned i = 0; i < c.inSize[0]; ++i)
        if ((c.in[1][i] & 0x80) != 0) emu.busWrite(address + i, 1, c.in[0][i]);
    return true;
}
bool readBase(Emu& emu, Call& c, void* ctx) {
    if (c.outSize == 0 || c.outSize > 8) return false;
    const Context& x = *static_cast<const Context*>(ctx);
    vec::store(c.out, c.outSize, emu.reg(x.fs_base));
    return true;
}
bool readGsBase(Emu& emu, Call& c, void* ctx) {
    if (c.outSize == 0 || c.outSize > 8) return false;
    vec::store(c.out, c.outSize, emu.reg(static_cast<const Context*>(ctx)->gs_base));
    return true;
}
bool writeFsBase(Emu& emu, Call& c, void* ctx) {
    if (c.nIn < 1 || c.inSize[0] > 8) return false;
    emu.setReg(static_cast<const Context*>(ctx)->fs_base, vec::load(c.in[0], c.inSize[0]));
    return true;
}
bool writeGsBase(Emu& emu, Call& c, void* ctx) {
    if (c.nIn < 1 || c.inSize[0] > 8) return false;
    emu.setReg(static_cast<const Context*>(ctx)->gs_base, vec::load(c.in[0], c.inSize[0]));
    return true;
}

// ---- dot products, sums of absolute differences, minimum positions, inserts and extracts
// DPPS / DPPD: the products picked by the high nibble of the immediate, summed (the pairs first), the sum put in the lanes the low nibble picks and zero in the rest.
template <unsigned EW>
bool dotProduct(Emu& emu, Call& c, void* ctx) {
    if (c.nIn < 3 || !binary(c)) return false;
    const unsigned k = imm(c);
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    groups(c, [&](std::size_t at, std::size_t g) {
        const std::size_t lanes = g / EW;
        Byte product[16] = {}, sum[16] = {};
        vec::fp2(product, c.in[0] + at, c.in[1] + at, g, EW, vec::Fp2::Mul, env, flags);
        for (std::size_t i = 0; i < lanes; ++i)
            if (((k >> (4 + i)) & 1) == 0) std::memset(product + i * EW, 0, EW);
        // the sum of two lanes at a time, then of the two sums (Intel's order for DPPS; DPPD adds its two)
        if (EW == 4) {
            Byte pairs[8];
            vec::fp2(pairs, product, product + 4, 4, 4, vec::Fp2::Add, env, flags);       // lane 0 + lane 1
            vec::fp2(pairs + 4, product + 8, product + 12, 4, 4, vec::Fp2::Add, env, flags);   // lane 2 + lane 3
            vec::fp2(sum, pairs, pairs + 4, 4, 4, vec::Fp2::Add, env, flags);
        } else {
            vec::fp2(sum, product, product + 8, 8, 8, vec::Fp2::Add, env, flags);
        }
        for (std::size_t i = 0; i < lanes; ++i) {
            if (((k >> i) & 1) != 0) std::memcpy(c.out + at + i * EW, sum, EW);
            else std::memset(c.out + at + i * EW, 0, EW);
        }
    });
    raise(emu, ctx, flags);
    return true;
}
// MPSADBW: eight sums of the absolute differences of four bytes of the destination (from the offset the immediate's bit 2 gives) and four bytes of the source (the bits 1 and 0).
bool mpsadbw(Emu&, Call& c, void*) {
    if (c.nIn < 3 || !binary(c)) return false;
    const unsigned k = imm(c);
    groups(c, [&](std::size_t at, std::size_t) {
        const unsigned bits = at == 0 ? k : k >> 3;   // (the upper group of a 256-bit operand: bits 4 and 3 pick the source's offset and bit 5 the destination's)
        const Byte* a = c.in[0] + at;
        const Byte* b = c.in[1] + at + (bits & 3) * 4;
        const unsigned offset = ((bits >> 2) & 1) * 4;
        for (unsigned i = 0; i < 8; ++i) {
            unsigned total = 0;
            for (unsigned j = 0; j < 4; ++j) total += static_cast<unsigned>(std::abs(static_cast<int>(a[offset + i + j]) - static_cast<int>(b[j])));
            vec::store(c.out + at + 2 * i, 2, total);
        }
    });
    return true;
}
// PHMINPOSUW: the smallest unsigned word of the source (the first of equals) in the lowest word, its index in the next three bits, the rest zero.
bool phminposuw(Emu&, Call& c, void*) {
    if (c.nIn < 1 || c.outSize != 16 || c.inSize[c.nIn - 1] != 16) return false;
    const Byte* a = c.in[c.nIn - 1];
    unsigned best = 0;
    for (unsigned i = 1; i < 8; ++i)
        if (vec::load(a + 2 * i, 2) < vec::load(a + 2 * best, 2)) best = i;
    vec::store(c.out, 2, vec::load(a + 2 * best, 2));
    vec::store(c.out + 2, 2, best);
    return true;
}
// INSERTPS: a lane of the source (the immediate's bits 7 and 6; the register form only: the memory form is the lane itself) into a lane of the destination (bits 5 and 4), then
// the lanes bits 3 to 0 name cleared.
bool insertps(Emu&, Call& c, void*) {
    if (c.nIn < 3 || c.outSize != 16 || c.inSize[0] != 16) return false;
    const unsigned k = imm(c);
    std::memcpy(c.out, c.in[0], 16);
    const unsigned from = c.inSize[1] >= 16 ? (k >> 6) & 3 : 0;
    std::memcpy(c.out + ((k >> 4) & 3) * 4, c.in[1] + from * 4, 4);
    for (unsigned i = 0; i < 4; ++i)
        if ((k >> i) & 1) std::memset(c.out + i * 4, 0, 4);
    return true;
}
bool extractps(Emu&, Call& c, void*) {
    if (c.nIn < 2 || c.inSize[0] != 16 || c.outSize == 0 || c.outSize > 8) return false;
    vec::store(c.out, c.outSize, vec::load(c.in[0] + (imm(c) & 3) * 4, 4));
    return true;
}

// ---- SSE4.2 string compares (QEMU's algorithm, which is Intel's as far as the processor tests have shown)
struct StringCompare {
    std::uint32_t result;   // the bit mask IntRes2, before the output selection
    bool carry, zero, sign, overflow;
};
int stringValue(const Byte* r, unsigned ctrl, int i) {
    switch (ctrl & 3) {
        case 0: return r[i];
        case 1: return static_cast<int>(vec::load(r + 2 * i, 2));
        case 2: return static_cast<std::int8_t>(r[i]);
        default: return static_cast<std::int16_t>(vec::load(r + 2 * i, 2));
    }
}
// Intel's definition (SDM, "Intel SSE4.2 string and text processing"): A is the first operand (xmm1: `d`, `validd` elements valid), B the second (`s`, `valids`). IntRes1 has a bit for each
// position of B, by the aggregation; the polarity makes IntRes2; the flags and the index or the mask follow from that.
StringCompare stringCompare(const Byte* d, const Byte* s, unsigned ctrl, int valids, int validd) {
    StringCompare out{};
    const int n = (ctrl & 1) != 0 ? 8 : 16;
    unsigned res = 0;
    for (int j = 0; j < n; ++j) {
        bool bit = false;
        switch ((ctrl >> 2) & 3) {
            case 0:   // equal any
                for (int i = 0; i < validd && j < valids; ++i) bit = bit || stringValue(s, ctrl, j) == stringValue(d, ctrl, i);
                break;
            case 1:   // ranges: pairs of bounds from the first element; a lone last bound pairs with nothing (QEMU's helper reads outside the register for it)
                for (int i = 0; i + 1 < validd && j < valids; i += 2) bit = bit || (stringValue(s, ctrl, j) >= stringValue(d, ctrl, i) && stringValue(s, ctrl, j) <= stringValue(d, ctrl, i + 1));
                break;
            case 2:   // equal each
                if (j < validd && j < valids) bit = stringValue(s, ctrl, j) == stringValue(d, ctrl, j);
                else bit = j >= validd && j >= valids;
                break;
            default: {   // equal ordered: A found at B's position j
                bit = true;
                for (int k = 0; k < n; ++k) {
                    if (k >= validd) break;   // (A is used up: found)
                    if (j + k >= valids || j + k >= n || stringValue(s, ctrl, j + k) != stringValue(d, ctrl, k)) {
                        bit = false;
                        break;
                    }
                }
                break;
            }
        }
        if (bit) res |= 1u << j;
    }
    const unsigned all = (1u << n) - 1;
    switch ((ctrl >> 4) & 3) {
        case 1: res ^= all; break;
        case 3: res ^= ((valids >= n ? all : (1u << valids) - 1)); break;   // (masked negative: only the valid positions of B)
        default: break;
    }
    out.result = res;
    out.zero = valids < n;
    out.sign = validd < n;
    out.carry = res != 0;
    out.overflow = (res & 1) != 0;
    return out;
}
int implicitLength(const Byte* r, unsigned ctrl) {
    int n = 0;
    if ((ctrl & 1) != 0) {
        while (n < 8 && vec::load(r + 2 * n, 2) != 0) ++n;
    } else {
        while (n < 16 && r[n] != 0) ++n;
    }
    return n;
}
int explicitLength(u64 value, unsigned ctrl, bool rexw) {
    i64 v = rexw ? static_cast<i64>(value) : static_cast<std::int32_t>(value);
    if (v < 0) v = -v;
    return static_cast<int>(std::min<i64>(v, (ctrl & 1) != 0 ? 8 : 16));
}
void setStringFlags(Emu& emu, void* ctx, const StringCompare& r) {
    const Context& x = *static_cast<const Context*>(ctx);
    emu.setReg(x.cf, r.carry);
    emu.setReg(x.pf, 0);
    emu.setReg(x.af, 0);
    emu.setReg(x.zf, r.zero);
    emu.setReg(x.sf, r.sign);
    emu.setReg(x.of, r.overflow);
}
// PCMPISTRI, PCMPISTRM (the lengths are where the first zero element is) and PCMPESTRI, PCMPESTRM (EAX and EDX give them): the arguments are the two operands, the immediate
// and, for the explicit forms, EAX and EDX; the result is ECX (the index) or XMM0 (the mask).
template <bool Explicit, bool Mask>
bool stringCompareOp(Emu& emu, Call& c, void* ctx) {
    if (c.nIn < (Explicit ? 5u : 3u) || c.inSize[0] != 16 || c.inSize[1] != 16) return false;
    const unsigned ctrl = c.in[2][0];
    int validd, valids;
    if (Explicit) {
        validd = explicitLength(vec::load(c.in[3], std::min<unsigned>(c.inSize[3], 8)), ctrl, c.inSize[3] == 8);
        valids = explicitLength(vec::load(c.in[4], std::min<unsigned>(c.inSize[4], 8)), ctrl, c.inSize[4] == 8);
    } else {
        validd = implicitLength(c.in[0], ctrl);
        valids = implicitLength(c.in[1], ctrl);
    }
    const StringCompare r = stringCompare(c.in[0], c.in[1], ctrl, valids, validd);
    setStringFlags(emu, ctx, r);
    if (!Mask) {
        const unsigned none = (ctrl & 1) != 0 ? 8 : 16;
        unsigned index = none;
        if (r.result != 0) {
            if ((ctrl & 0x40) != 0) {
                index = 31;
                while (((r.result >> index) & 1) == 0) --index;
            } else {
                index = 0;
                while (((r.result >> index) & 1) == 0) ++index;
            }
        }
        vec::store(c.out, std::min<unsigned>(c.outSize, 8), index);
    } else if ((ctrl & 0x40) != 0) {   // a mask of whole elements
        const unsigned ew = (ctrl & 1) != 0 ? 2 : 1;
        for (unsigned i = 0; i < 16 / ew; ++i) vec::store(c.out + i * ew, ew, ((r.result >> i) & 1) != 0 ? vec::laneMask(ew) : 0);
    } else {
        vec::store(c.out, 4, r.result);
    }
    return true;
}

// ---- AES-NI (FIPS 197): the state is 16 bytes in column order
struct AesTables {
    std::array<Byte, 256> sbox{}, inverse{};
    AesTables() {
        // the multiplicative inverse in GF(2^8) and the affine map
        std::array<Byte, 256> inv{};
        for (unsigned a = 1; a < 256; ++a)
            for (unsigned b = 1; b < 256; ++b)
                if (multiply(static_cast<Byte>(a), static_cast<Byte>(b)) == 1) inv[a] = static_cast<Byte>(b);
        for (unsigned a = 0; a < 256; ++a) {
            const unsigned x = inv[a];
            unsigned y = x;
            for (int k = 1; k <= 4; ++k) y ^= ((x << k) | (x >> (8 - k))) & 0xFF;
            sbox[a] = static_cast<Byte>(y ^ 0x63);
            inverse[sbox[a]] = static_cast<Byte>(a);
        }
    }
    static Byte multiply(Byte a, Byte b) {
        unsigned r = 0, x = a;
        for (unsigned y = b; y != 0; y >>= 1) {
            if (y & 1) r ^= x;
            x <<= 1;
            if (x & 0x100) x ^= 0x11B;
        }
        return static_cast<Byte>(r);
    }
};
const AesTables& aes() {
    static const AesTables t;
    return t;
}
void subBytes(Byte* s, bool inverse) {
    for (int i = 0; i < 16; ++i) s[i] = inverse ? aes().inverse[s[i]] : aes().sbox[s[i]];
}
void shiftRows(Byte* s, bool inverse) {
    Byte t[16];
    for (int col = 0; col < 4; ++col)
        for (int row = 0; row < 4; ++row) t[4 * col + row] = s[4 * ((inverse ? col - row + 4 : col + row) & 3) + row];
    std::memcpy(s, t, 16);
}
void mixColumns(Byte* s, bool inverse) {
    static constexpr Byte kForward[4] = {2, 3, 1, 1}, kInverse[4] = {14, 11, 13, 9};
    const Byte* m = inverse ? kInverse : kForward;
    for (int col = 0; col < 4; ++col) {
        Byte in[4], out[4];
        std::memcpy(in, s + 4 * col, 4);
        for (int row = 0; row < 4; ++row) {
            Byte v = 0;
            for (int k = 0; k < 4; ++k) v ^= AesTables::multiply(m[(k - row + 4) & 3], in[k]);
            out[row] = v;
        }
        std::memcpy(s + 4 * col, out, 4);
    }
}
template <bool Inverse, bool Last>
bool aesRound(Emu&, Call& c, void*) {
    if (c.nIn < 2 || c.outSize % 16 != 0 || !binary(c)) return false;
    for (std::size_t at = 0; at < c.outSize; at += 16) {
        Byte s[16];
        std::memcpy(s, c.in[0] + at, 16);
        if (Inverse) {   // (InvShiftRows and InvSubBytes commute)
            shiftRows(s, true);
            subBytes(s, true);
            if (!Last) mixColumns(s, true);
        } else {
            subBytes(s, false);
            shiftRows(s, false);
            if (!Last) mixColumns(s, false);
        }
        for (int i = 0; i < 16; ++i) c.out[at + i] = static_cast<Byte>(s[i] ^ c.in[1][at + i]);
    }
    return true;
}
bool aesimc(Emu&, Call& c, void*) {
    if (c.nIn < 1 || c.outSize != 16 || c.inSize[c.nIn - 1] != 16) return false;
    std::memcpy(c.out, c.in[c.nIn - 1], 16);
    mixColumns(c.out, true);
    return true;
}
bool aeskeygenassist(Emu&, Call& c, void*) {
    if (c.nIn < 2 || c.outSize != 16 || c.inSize[c.nIn - 2] != 16) return false;
    const Byte* a = c.in[c.nIn - 2];
    const unsigned rcon = imm(c);
    for (int half = 0; half < 2; ++half) {
        Byte word[4];
        for (int k = 0; k < 4; ++k) word[k] = aes().sbox[a[4 * (half == 0 ? 1 : 3) + k]];
        std::memcpy(c.out + 8 * half, word, 4);
        c.out[8 * half + 4] = static_cast<Byte>(word[1] ^ rcon);
        c.out[8 * half + 5] = word[2];
        c.out[8 * half + 6] = word[3];
        c.out[8 * half + 7] = word[0];
    }
    return true;
}
// PCLMULQDQ: the immediate's bit 0 picks the low or high quadword of the first operand, bit 4 of the second; the carry-less product is 128 bits.
bool pclmulqdq(Emu&, Call& c, void*) {
    if (c.nIn < 3 || !binary(c)) return false;
    const unsigned k = imm(c);
    for (std::size_t at = 0; at < c.outSize; at += 16)
        vec::polyMul64(c.out + at, vec::load(c.in[0] + at + 8 * (k & 1), 8), vec::load(c.in[1] + at + 8 * ((k >> 4) & 1), 8));
    return true;
}

// ---- shuffles and permutes (candidates for the library)
// SHUFPS, SHUFPD: per 128-bit group, the lanes of the first operand picked by the low fields of the immediate, then of the second.
template <unsigned EW>
bool shufflePacked(Emu&, Call& c, void*) {
    if (c.nIn < 3 || !binary(c)) return false;
    const unsigned k = imm(c);
    groups(c, [&](std::size_t at, std::size_t) {
        const unsigned kk = EW == 4 ? k : k >> (at / 8);   // (SHUFPD uses two bits for each group)
        if (EW == 4) {
            for (unsigned i = 0; i < 4; ++i) std::memcpy(c.out + at + 4 * i, (i < 2 ? c.in[0] : c.in[1]) + at + 4 * ((kk >> (2 * i)) & 3), 4);
        } else {
            std::memcpy(c.out + at, c.in[0] + at + 8 * (kk & 1), 8);
            std::memcpy(c.out + at + 8, c.in[1] + at + 8 * ((kk >> 1) & 1), 8);
        }
    });
    return true;
}
// VPERMILPS and VPERMILPD: lanes of each 128-bit group of the source picked within the group, by the control vector's lanes or by the immediate.
template <unsigned EW>
bool permuteWithin(Emu&, Call& c, void*) {
    if (c.nIn < 2 || c.outSize == 0) return false;
    const bool immediate = c.inSize[c.nIn - 1] == 1;
    const Byte* a = immediate ? c.in[c.nIn - 2] : c.in[c.nIn - 2];
    if (c.inSize[c.nIn - 2] != c.outSize || (!immediate && c.inSize[c.nIn - 1] != c.outSize)) return false;
    const unsigned k = immediate ? imm(c) : 0;
    groups(c, [&](std::size_t at, std::size_t g) {
        for (std::size_t i = 0; i < g / EW; ++i) {
            unsigned pick;
            if (immediate) pick = EW == 4 ? (k >> (2 * i)) & 3 : (k >> (i + (at / 8))) & 1;
            else pick = EW == 4 ? static_cast<unsigned>(vec::load(c.in[c.nIn - 1] + at + 4 * i, 4) & 3) : static_cast<unsigned>((vec::load(c.in[c.nIn - 1] + at + 8 * i, 8) >> 1) & 1);
            std::memcpy(c.out + at + EW * i, a + at + EW * pick, EW);
        }
    });
    return true;
}
// VPERMD, VPERMPS: each lane of the second operand's... the first operand holds the indices (the low three bits), the second the table.
template <unsigned EW>
bool permuteAcross(Emu&, Call& c, void*) {
    if (!binary(c)) return false;
    Byte tmp[32];
    const std::size_t lanes = c.outSize / EW;
    for (std::size_t i = 0; i < lanes; ++i) std::memcpy(tmp + EW * i, c.in[1] + EW * (vec::load(c.in[0] + EW * i, EW) & (lanes - 1)), EW);
    std::memcpy(c.out, tmp, c.outSize);
    return true;
}
// VPERMQ, VPERMPD: four quadwords picked by the immediate's fields.
bool permuteQuadwords(Emu&, Call& c, void*) {
    if (c.nIn < 2 || c.outSize != 32 || c.inSize[c.nIn - 2] != 32) return false;
    const unsigned k = imm(c);
    Byte tmp[32];
    for (unsigned i = 0; i < 4; ++i) std::memcpy(tmp + 8 * i, c.in[c.nIn - 2] + 8 * ((k >> (2 * i)) & 3), 8);
    std::memcpy(c.out, tmp, 32);
    return true;
}
// VPERM2F128, VPERM2I128: each half of the result from a half of either operand (the immediate's low nibble for the low half, the high nibble for the high; bit 3 clears it).
bool permute128(Emu&, Call& c, void*) {
    if (c.nIn < 3 || c.outSize != 32 || !binary(c)) return false;
    const unsigned k = imm(c);
    Byte tmp[32];
    for (unsigned half = 0; half < 2; ++half) {
        const unsigned f = (k >> (4 * half)) & 0xF;
        if ((f & 8) != 0) std::memset(tmp + 16 * half, 0, 16);
        else std::memcpy(tmp + 16 * half, (f & 2 ? c.in[1] : c.in[0]) + 16 * (f & 1), 16);
    }
    std::memcpy(c.out, tmp, 32);
    return true;
}
// VADDSUBPS, VADDSUBPD: the even lanes subtract, the odd lanes add.
template <unsigned EW>
bool addSubtract(Emu& emu, Call& c, void* ctx) {
    if (!binary(c)) return false;
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    Byte sum[32], difference[32];
    vec::fp2(sum, c.in[0], c.in[1], c.outSize, EW, vec::Fp2::Add, env, flags);
    vec::fp2(difference, c.in[0], c.in[1], c.outSize, EW, vec::Fp2::Sub, env, flags);
    for (std::size_t i = 0; i < c.outSize; i += EW) std::memcpy(c.out + i, ((i / EW) % 2 == 0 ? difference : sum) + i, EW);
    raise(emu, ctx, flags);
    return true;
}
// HADDPS, HADDPD, HSUBPS, HSUBPD: per 128-bit group the sums (differences) of the pairs of the first operand, then of the second.
template <unsigned EW, vec::Fp2 Op>
bool horizontalFloat(Emu& emu, Call& c, void* ctx) {
    if (!binary(c)) return false;
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    Byte left[32], right[32], tmp[32];
    const Byte* const first = c.in[c.nIn - 2];
    const Byte* const second = c.in[c.nIn - 1];
    groups(c, [&](std::size_t at, std::size_t g) {
        const std::size_t pairs = g / EW / 2;
        // the even lanes and the odd lanes of the two operands as two vectors, then one operation on them
        for (std::size_t k = 0; k < pairs; ++k) {
            std::memcpy(left + EW * k, first + at + EW * 2 * k, EW);
            std::memcpy(right + EW * k, first + at + EW * (2 * k + 1), EW);
            std::memcpy(left + EW * (pairs + k), second + at + EW * 2 * k, EW);
            std::memcpy(right + EW * (pairs + k), second + at + EW * (2 * k + 1), EW);
        }
        vec::fp2(tmp, left, right, g, EW, Op, env, flags);
        std::memcpy(c.out + at, tmp, g);
    });
    raise(emu, ctx, flags);
    return true;
}
// VCVTDQ2PS and VCVTPS2DQ with the rounding of MXCSR.
bool intToSingle(Emu& emu, Call& c, void* ctx) {
    if (!unaryLast(c)) return false;
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    vec::intToFloat(c.out, c.in[c.nIn - 1], c.outSize, 4, 4, Sign::Signed, 0, env, flags);
    raise(emu, ctx, flags);
    return true;
}
bool singleToInt(Emu& emu, Call& c, void* ctx) {
    if (!unaryLast(c)) return false;
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    vec::floatToInt(c.out, c.in[c.nIn - 1], c.outSize, 4, 4, Sign::Signed, 0, vec::IntOverflow::X86Indefinite, env, flags);
    raise(emu, ctx, flags);
    return true;
}

// VMOVSHDUP, VMOVSLDUP: the odd (even) lanes of the source doubled.
template <bool Odd>
bool duplicateLanes(Emu&, Call& c, void*) {
    if (!unaryLast(c)) return false;
    for (std::size_t i = 0; i + 8 <= c.outSize; i += 8) {
        const Byte* lane = c.in[c.nIn - 1] + i + (Odd ? 4 : 0);
        std::memcpy(c.out + i, lane, 4);
        std::memcpy(c.out + i + 4, lane, 4);
    }
    return true;
}
bool intToDouble(Emu& emu, Call& c, void* ctx) {   // VCVTDQ2PD: the low lanes of the source (half as many bytes as the result has)
    if (c.nIn < 1 || c.outSize == 0 || c.inSize[c.nIn - 1] < c.outSize / 2) return false;
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    vec::intToFloat(c.out, c.in[c.nIn - 1], c.outSize / 2, 4, 8, Sign::Signed, 0, env, flags);
    raise(emu, ctx, flags);
    return true;
}
bool doubleToInt(Emu& emu, Call& c, void* ctx) {   // VCVTPD2DQ: two or four doubles to as many dwords (the rest of the destination zero)
    if (c.nIn < 1 || c.outSize == 0 || c.inSize[c.nIn - 1] < c.outSize * 2 - (c.outSize == 16 ? 16 : 0)) return false;
    const vec::FpEnv env = fpEnv(emu, ctx);
    unsigned flags = 0;
    const std::size_t bytes = std::min<std::size_t>(c.inSize[c.nIn - 1], 32);
    vec::floatToInt(c.out, c.in[c.nIn - 1], bytes, 8, 4, Sign::Signed, 0, vec::IntOverflow::X86Indefinite, env, flags);
    raise(emu, ctx, flags);
    return true;
}
// VCVTPS2PH: the single lanes of the source rounded to halves (the immediate: bit 2 takes MXCSR's mode, else bits 1 and 0).
bool singleToHalf(Emu& emu, Call& c, void* ctx) {
    if (c.nIn < 2 || c.outSize == 0) return false;
    const unsigned k = imm(c);
    const vec::FpEnv env = roundEnv(emu, ctx, k);
    unsigned flags = 0;
    vec::floatConvert(c.out, c.in[c.nIn - 2], c.inSize[c.nIn - 2], 4, 2, env, flags);
    raise(emu, ctx, flags);
    return true;
}

// VLDMXCSR, VSTMXCSR: MXCSR from or to a doubleword (reserved bits are not checked: Unicorn's own is the oracle for the legacy forms, which the description does itself).
bool loadMxcsr(Emu& emu, Call& c, void* ctx) {
    const Context& x = *static_cast<const Context*>(ctx);
    if (c.nIn < 1 || c.inSize[0] < 4 || !x.has_mxcsr) return false;
    emu.setReg(x.mxcsr, vec::load(c.in[0], 4));
    return true;
}
bool storeMxcsr(Emu& emu, Call& c, void* ctx) {
    const Context& x = *static_cast<const Context*>(ctx);
    if (c.outSize < 4 || !x.has_mxcsr) return false;
    vec::store(c.out, 4, emu.reg(x.mxcsr));
    return true;
}
// VPINSRB, VPINSRW: the low byte (word) of the second operand into the lane the immediate names of the first.
template <unsigned EW>
bool insertLane(Emu&, Call& c, void*) {
    if (c.nIn < 3 || c.outSize != 16 || c.inSize[0] != 16 || c.inSize[1] < EW) return false;
    std::memcpy(c.out, c.in[0], 16);
    std::memcpy(c.out + EW * (imm(c) & (16 / EW - 1)), c.in[1], EW);
    return true;
}

}  // namespace

// One name for the legacy form and the two of the VEX forms (`v<name>_avx` on XMM, `v<name>_avx2` on YMM, which the description keeps apart).
#define V3(NAME, H) {NAME, H}, {"v" NAME "_avx", H}, {"v" NAME "_avx2", H}

const pcode_ops::Table& table() {
    static const pcode_ops::Table t{
        V3("paddsb", paddsb), V3("paddsw", paddsw), V3("paddusb", paddusb), V3("paddusw", paddusw),
        V3("psubsb", psubsb), V3("psubsw", psubsw), V3("psubusb", psubusb), V3("psubusw", psubusw),
        V3("pavgb", pavgb), V3("pavgw", pavgw), V3("pmulhw", pmulhw), V3("pmulhuw", pmulhuw), V3("pmulld", pmulld), V3("pmullw", pmullw),
        V3("pmulhrsw", pmulhrsw), V3("pmuldq", pmuldq), V3("pmuludq", pmuludq), V3("pmaddwd", pmaddwd), V3("pmaddubsw", pmaddubsw), V3("psadbw", psadbw),
        V3("pmaxsb", pmaxsb), V3("pmaxsw", pmaxsw), V3("pmaxsd", pmaxsd), V3("pmaxub", pmaxub), V3("pmaxuw", pmaxuw), V3("pmaxud", pmaxud),
        V3("pminsb", pminsb), V3("pminsw", pminsw), V3("pminsd", pminsd), V3("pminub", pminub), V3("pminuw", pminuw), V3("pminud", pminud),
        V3("pabsb", pabsb), V3("pabsw", pabsw), V3("pabsd", pabsd), V3("pabsq", pabsq), V3("pmullq", pmullq), V3("pmaxsq", pmaxsq), V3("pmaxuq", pmaxuq), V3("pminsq", pminsq), V3("pminuq", pminuq),
        V3("paddb", paddb), V3("paddw", paddw), V3("paddd", paddd), V3("paddq", paddq), V3("psubb", psubb), V3("psubw", psubw), V3("psubd", psubd), V3("psubq", psubq),
        V3("pcmpeqb", pcmpeqb), V3("pcmpeqw", pcmpeqw), V3("pcmpeqd", pcmpeqd), V3("pcmpeqq", pcmpeqq), V3("pcmpgtb", pcmpgtb), V3("pcmpgtw", pcmpgtw), V3("pcmpgtd", pcmpgtd), V3("pcmpgtq", pcmpgtq),
        V3("pand", pand), V3("pandn", pandn), V3("por", por), V3("pxor", pxor),
        V3("psignb", psign<1>), V3("psignw", psign<2>), V3("psignd", psign<4>),
        V3("phaddw", (horizontal<2, Horizontal::Add>)), V3("phaddd", (horizontal<4, Horizontal::Add>)), V3("phaddsw", (horizontal<2, Horizontal::AddSat>)),
        V3("phsubw", (horizontal<2, Horizontal::Sub>)), V3("phsubd", (horizontal<4, Horizontal::Sub>)), V3("phsubsw", (horizontal<2, Horizontal::SubSat>)),
        V3("packsswb", (pack<2, vec::Narrow::SignedToSigned>)), V3("packssdw", (pack<4, vec::Narrow::SignedToSigned>)),
        V3("packuswb", (pack<2, vec::Narrow::SignedToUnsigned>)), V3("packusdw", (pack<4, vec::Narrow::SignedToUnsigned>)),
        V3("punpcklqdq", (unpack<8, false>)), V3("punpckhqdq", (unpack<8, true>)),
        V3("pshufb", pshufb), V3("pshufd", (pshuf<4, 0>)), V3("pshuflw", (pshuf<2, 1>)), V3("pshufhw", (pshuf<2, 2>)), {"pshufw", (pshuf<2, 1>)},
        V3("psllw", (shiftBy<2, vec::Shift::Left>)), V3("psraw", (shiftBy<2, vec::Shift::RightArithmetic>)), V3("psrlw", (shiftBy<2, vec::Shift::RightLogical>)),
        V3("pslld", (shiftBy<4, vec::Shift::Left>)), V3("psrad", (shiftBy<4, vec::Shift::RightArithmetic>)), V3("psrld", (shiftBy<4, vec::Shift::RightLogical>)),
        V3("psllq", (shiftBy<8, vec::Shift::Left>)), V3("psrlq", (shiftBy<8, vec::Shift::RightLogical>)),
        V3("pslldq", shiftBytes<true>), V3("psrldq", shiftBytes<false>), V3("palignr", palignr),
        V3("pmovsxbw", (extend<1, 2, Sign::Signed>)), V3("pmovsxbd", (extend<1, 4, Sign::Signed>)), V3("pmovsxbq", (extend<1, 8, Sign::Signed>)),
        V3("pmovsxwd", (extend<2, 4, Sign::Signed>)), V3("pmovsxwq", (extend<2, 8, Sign::Signed>)), V3("pmovsxdq", (extend<4, 8, Sign::Signed>)),
        V3("pmovzxbw", (extend<1, 2, Sign::Unsigned>)), V3("pmovzxbd", (extend<1, 4, Sign::Unsigned>)), V3("pmovzxbq", (extend<1, 8, Sign::Unsigned>)),
        V3("pmovzxwd", (extend<2, 4, Sign::Unsigned>)), V3("pmovzxwq", (extend<2, 8, Sign::Unsigned>)), V3("pmovzxdq", (extend<4, 8, Sign::Unsigned>)),
        V3("pblendw", blendImm<2>), V3("blendps", blendImm<4>), V3("blendpd", blendImm<8>), V3("pblendvb", blendMask<1>), V3("blendvps", blendMask<4>), V3("blendvpd", blendMask<8>),
        V3("movmskps", moveMask<4>), V3("movmskpd", moveMask<8>),
        {"crc32", crc32c}, {"swap_bytes", swapBytes},
        V3("punpcklbw", (unpack<1, false>)), V3("punpcklwd", (unpack<2, false>)), V3("punpckldq", (unpack<4, false>)), V3("punpckhbw", (unpack<1, true>)), V3("punpckhwd", (unpack<2, true>)), V3("punpckhdq", (unpack<4, true>)),
        {"vunpcklps_avx", (unpack<4, false>)}, {"vunpckhps_avx", (unpack<4, true>)}, {"vunpcklpd_avx", (unpack<8, false>)}, {"vunpckhpd_avx", (unpack<8, true>)},
        {"vsubps_avx", (fpBinary<4, vec::Fp2::Sub>)}, {"vsubpd_avx", (fpBinary<8, vec::Fp2::Sub>)},
        {"vshufps_avx", shufflePacked<4>}, {"vshufpd_avx", shufflePacked<8>}, {"vpermilps_avx", permuteWithin<4>}, {"vpermilpd_avx", permuteWithin<8>},
        {"vpermd_avx2", permuteAcross<4>}, {"vpermps_avx2", permuteAcross<4>}, {"vpermq_avx2", permuteQuadwords}, {"vpermpd_avx2", permuteQuadwords},
        {"vperm2f128_avx", permute128}, {"vperm2i128_avx2", permute128}, {"vpblendd_avx2", blendImm<4>},
        {"vpsllvd_avx2", (shiftVariable<4, vec::Shift::Left>)}, {"vpsllvq_avx2", (shiftVariable<8, vec::Shift::Left>)}, {"vpsrlvd_avx2", (shiftVariable<4, vec::Shift::RightLogical>)},
        {"vpsrlvq_avx2", (shiftVariable<8, vec::Shift::RightLogical>)}, {"vpsravd_avx2", (shiftVariable<4, vec::Shift::RightArithmetic>)},
        {"vaddsubps_avx", addSubtract<4>}, {"vaddsubpd_avx", addSubtract<8>},
        {"vhaddps_avx", (horizontalFloat<4, vec::Fp2::Add>)}, {"vhaddpd_avx", (horizontalFloat<8, vec::Fp2::Add>)}, {"vhsubps_avx", (horizontalFloat<4, vec::Fp2::Sub>)}, {"vhsubpd_avx", (horizontalFloat<8, vec::Fp2::Sub>)},
        {"vcvtdq2ps_avx", intToSingle}, {"vcvtps2dq_avx", singleToInt}, {"vcvtdq2pd_avx", intToDouble}, {"vcvtpd2dq_avx", doubleToInt},
        {"vcvtps2pd_avx", (convertFloat<4, 8>)}, {"vcvtpd2ps_avx", (convertFloat<8, 4>)}, {"vcvtph2ps_f16c", (convertFloat<2, 4>)}, {"vcvtps2ph_f16c", singleToHalf},
        {"vldmxcsr_avx", loadMxcsr}, {"vstmxcsr_avx", storeMxcsr}, {"vpinsrb_avx", insertLane<1>}, {"vpinsrw_avx", insertLane<2>},
        {"vmovshdup_avx", duplicateLanes<true>}, {"vmovsldup_avx", duplicateLanes<false>},
        {"vcmpps_avx", (cmpFloatVex<4, false>)}, {"vcmppd_avx", (cmpFloatVex<8, false>)}, {"vcmpss_avx", (cmpFloatVex<4, true>)}, {"vcmpsd_avx", (cmpFloatVex<8, true>)},
        V3("dpps", dotProduct<4>), V3("dppd", dotProduct<8>), V3("mpsadbw", mpsadbw), V3("phminposuw", phminposuw), V3("insertps", insertps), V3("extractps", extractps),
        {"pcmpistri", (stringCompareOp<false, false>)}, {"pcmpistrm", (stringCompareOp<false, true>)}, {"pcmpestri", (stringCompareOp<true, false>)}, {"pcmpestrm", (stringCompareOp<true, true>)},
        {"vpcmpistri_avx", (stringCompareOp<false, false>)}, {"vpcmpistrm_avx", (stringCompareOp<false, true>)}, {"vpcmpestri_avx", (stringCompareOp<true, false>)}, {"vpcmpestrm_avx", (stringCompareOp<true, true>)},
        {"aesenc", (aesRound<false, false>)}, {"aesenclast", (aesRound<false, true>)}, {"aesdec", (aesRound<true, false>)}, {"aesdeclast", (aesRound<true, true>)},
        {"vaesenc_vaes", (aesRound<false, false>)}, {"vaesenclast_vaes", (aesRound<false, true>)}, {"vaesdec_vaes", (aesRound<true, false>)}, {"vaesdeclast_vaes", (aesRound<true, true>)},
        {"aesimc", aesimc}, {"aeskeygenassist", aeskeygenassist}, {"pclmulqdq", pclmulqdq}, {"vpclmulqdq_vpclmulqdq", pclmulqdq},
        {"lddqu", copyLast}, {"movntdqa", copyLast}, {"vlddqu_avx", copyLast}, {"vmovntdqa_avx", copyLast}, {"vmovntdqa_avx2", copyLast}, {"movdq2q", lowQuadword},
        {"maskmovdqu", maskedMove}, {"vmaskmovdqu_avx", maskedMove}, {"MaskedMoveQWord", maskedMove},
        {"readfsbase", readBase}, {"readgsbase", readGsBase}, {"writefsbase", writeFsBase}, {"writegsbase", writeGsBase},
        V3("divps", (fpBinary<4, vec::Fp2::Div>)), V3("divpd", (fpBinary<8, vec::Fp2::Div>)),
        V3("maxps", (fpBinary<4, vec::Fp2::Max>)), V3("maxpd", (fpBinary<8, vec::Fp2::Max>)), V3("minps", (fpBinary<4, vec::Fp2::Min>)), V3("minpd", (fpBinary<8, vec::Fp2::Min>)),
        V3("sqrtps", (fpUnary<4, vec::Fp1::Sqrt>)), V3("sqrtpd", (fpUnary<8, vec::Fp1::Sqrt>)),
        V3("roundps", (roundOp<4, false>)), V3("roundpd", (roundOp<8, false>)), V3("roundss", (roundOp<4, true>)), V3("roundsd", (roundOp<8, true>)),
        {"cmpps", (cmpFloat<4, false>)}, {"cmppd", (cmpFloat<8, false>)}, {"cmpss", (cmpFloat<4, true>)}, {"cmpsd", (cmpFloat<8, true>)},
        V3("rcpps", (reciprocal<false, false>)), V3("rcpss", (reciprocal<false, true>)), V3("rsqrtps", (reciprocal<true, false>)), V3("rsqrtss", (reciprocal<true, true>)),
    };
    return t;
}
#undef V3

vec::FpEnv environment(std::uint64_t m) {
    vec::FpEnv env;   // (NanRule::X86 and MinMax::X86 are the defaults)
    static constexpr vec::Round kRound[4] = {vec::Round::NearestEven, vec::Round::TowardNegative, vec::Round::TowardPositive, vec::Round::TowardZero};
    env.round = kRound[(m >> 13) & 3];
    env.flushInputs = (m & 0x40) != 0;
    env.flushResults = (m & 0x8000) != 0;
    return env;
}

// The library's flags are invalid, divide by zero, overflow, underflow, inexact, denormal input; MXCSR's are IE, DE, ZE, OE, UE, PE.
std::uint64_t exception_bits(unsigned flags) {
    std::uint64_t bits = 0;
    if (flags & vec::kInvalid) bits |= 1;
    if (flags & vec::kInputDenormal) bits |= 2;
    if (flags & vec::kDivideByZero) bits |= 4;
    if (flags & vec::kOverflow) bits |= 8;
    if (flags & vec::kUnderflow) bits |= 16;
    if (flags & vec::kInexact) bits |= 32;
    return bits;
}

Context make_context(const Emu& emu) {
    Context c;
    try {
        c.mxcsr = emu.regRef("MXCSR");
        c.has_mxcsr = true;
        c.cf = emu.regRef("CF");
        c.pf = emu.regRef("PF");
        c.af = emu.regRef("AF");
        c.zf = emu.regRef("ZF");
        c.sf = emu.regRef("SF");
        c.of = emu.regRef("OF");
        c.fs_base = emu.regRef("FS_OFFSET");
        c.gs_base = emu.regRef("GS_OFFSET");
    } catch (const std::exception&) {
        c.has_mxcsr = false;
    }
    return c;
}

pcode_ops::Binding bind(Emu& emu, Context& context) { return pcode_ops::bind(emu, table(), &context); }

std::vector<std::string> served_names() {
    std::vector<std::string> names;
    for (const pcode_ops::Entry& e : table().entries()) names.emplace_back(e.name);
    return names;
}

}  // namespace cb::emulate::pcode_ops_x86
