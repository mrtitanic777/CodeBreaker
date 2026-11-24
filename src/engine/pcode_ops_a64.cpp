// cb-unity: alone (file-local names that other files in a batch also use)
// engine/pcode_ops_a64.cpp -- the AArch64 description's named operations (NEON_*, SIMD_PIECE, a64_TBL, MP_INT_ABS, crc32*), from the Arm architecture manual, over the lane library
// (engine/pcode_vector.hpp).
//
// How the description calls them (read from AARCH64neon.sinc and checked against Unicorn by pcode_ops_a64_test.cpp):
//   - a vector form ends with the lane width in bytes as a constant of one byte (`NEON_sqadd(Rn, Rm, 4:1)`: 4 byte lanes); a scalar form has no such constant and its lane is the
//     whole operand (`NEON_sqadd(Rn_FPR16, Rm_FPR16)`);
//   - an operation that changes the width of the lanes takes the width of the source lanes, the narrowing ones the old Rd first (`NEON_sqxtn(Rd, Rn, 2:1)`; the "2" forms keep
//     the low half of Rd), the widening ones the source vector (the "2" forms are given the 16 byte register and use its upper half);
//   - an immediate is a constant of one to four bytes.
#include "engine/pcode_ops_a64.hpp"

#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <cmath>
#include <limits>
#include <cstring>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "engine/pcode_vector.hpp"

namespace cb::engine::pcode_ops {
namespace {

namespace vec = cb::engine::vec;
using Bytes = std::vector<std::uint8_t>;
using u64 = std::uint64_t;
using vec::Sign;

thread_local std::vector<std::string_view>* t_trace = nullptr;
thread_local Emu::RegRef t_fpcr{};
thread_local std::uint32_t t_word = 0;   // the instruction being run, for the few operations whose arguments do not tell a scalar form from a vector one
bool g_laneCompat = true;                // the maximum and minimum with a NaN as the lane's own core gives them (the host's), not as the manual says

struct OpContext {
    std::string_view name;
};

std::atomic<bool> g_counting{false};   // count how often each operation is called, on every thread
std::mutex g_count_lock;
std::map<std::string, std::uint64_t> g_counts;

void note(void* context) {
    if (t_trace != nullptr) t_trace->push_back(static_cast<const OpContext*>(context)->name);
    if (g_counting.load(std::memory_order_relaxed)) {
        const std::lock_guard guard(g_count_lock);
        ++g_counts[std::string(static_cast<const OpContext*>(context)->name)];
    }
}

// the call's lanes: the length of the result in bytes, and the lane width (the constant at `k`, or the whole operand for a scalar form)
struct Shape {
    std::size_t n;
    unsigned ew;
};

Shape shapeK(const Call& c, unsigned k) { return {c.outSize, c.nIn > k ? static_cast<unsigned>(c.in[k][0]) : c.outSize}; }

// the lane width of the three-same-width integer forms: the constant the description gives, except that the 2S form of URHADD gives 2 (a lane of 2 bytes: a description error),
// so the instruction's own size field is taken where the instruction is one of that group
Shape shapeThreeSame(const Call& c) {
    Shape s = shapeK(c, 2);
    if (c.nIn >= 3 && (t_word & 0x9F200400u) == 0x0E200400u) s.ew = 1u << ((t_word >> 22) & 3);
    return s;
}

u64 imm(const Call& c, unsigned i) { return vec::load(c.in[i], std::min(c.inSize[i], 8u)); }

vec::FpEnv fpEnv(Emu& self) {
    const u64 fpcr = t_fpcr.size != 0 ? self.reg(t_fpcr) : 0;
    vec::FpEnv env;
    env.nan = vec::NanRule::Arm;
    switch ((fpcr >> 22) & 3) {
        case 0: env.round = vec::Round::NearestEven; break;
        case 1: env.round = vec::Round::TowardPositive; break;
        case 2: env.round = vec::Round::TowardNegative; break;
        default: env.round = vec::Round::TowardZero; break;
    }
    env.flushInputs = env.flushResults = ((fpcr >> 24) & 1) != 0;
    env.defaultNan = ((fpcr >> 25) & 1) != 0;
    return env;
}

// ---- integer lanes ----------------------------------------------------------------------------------------------------------------------------------------------------

template <Sign S>
bool opAddSat(Emu&, Call& c, void* ctx) {
    note(ctx);
    const Shape s = shapeK(c, 2);
    vec::addSat(c.out, c.in[0], c.in[1], s.n, s.ew, S);
    return true;
}
template <Sign S>
bool opSubSat(Emu&, Call& c, void* ctx) {
    note(ctx);
    const Shape s = shapeK(c, 2);
    vec::subSat(c.out, c.in[0], c.in[1], s.n, s.ew, S);
    return true;
}
bool opUsqadd(Emu&, Call& c, void* ctx) {   // USQADD Rd, Rn: Rd unsigned plus Rn signed
    note(ctx);
    const Shape s = shapeK(c, 2);
    vec::addSatMixed(c.out, c.in[0], c.in[1], s.n, s.ew, Sign::Unsigned);
    return true;
}
template <Sign S, bool Round>
bool opHalvingAdd(Emu&, Call& c, void* ctx) {
    note(ctx);
    const Shape s = shapeThreeSame(c);
    vec::halvingAdd(c.out, c.in[0], c.in[1], s.n, s.ew, S, Round);
    return true;
}
template <Sign S>
bool opHalvingSub(Emu&, Call& c, void* ctx) {
    note(ctx);
    const Shape s = shapeThreeSame(c);
    vec::halvingSub(c.out, c.in[0], c.in[1], s.n, s.ew, S);
    return true;
}
template <Sign S>
bool opMax(Emu&, Call& c, void* ctx) {
    note(ctx);
    const Shape s = shapeK(c, 2);
    vec::max(c.out, c.in[0], c.in[1], s.n, s.ew, S);
    return true;
}
template <Sign S>
bool opMin(Emu&, Call& c, void* ctx) {
    note(ctx);
    const Shape s = shapeK(c, 2);
    vec::min(c.out, c.in[0], c.in[1], s.n, s.ew, S);
    return true;
}
template <Sign S>
bool opAbsDiff(Emu&, Call& c, void* ctx) {
    note(ctx);
    const Shape s = shapeK(c, 2);
    vec::absDiff(c.out, c.in[0], c.in[1], s.n, s.ew, S);
    return true;
}
template <Sign S>
bool opAbsDiffAcc(Emu&, Call& c, void* ctx) {   // SABA, UABA: Rd + |Rn - Rm|
    note(ctx);
    const Shape s = shapeK(c, 3);
    Bytes d(s.n);
    vec::absDiff(d.data(), c.in[1], c.in[2], s.n, s.ew, S);
    vec::add(c.out, c.in[0], d.data(), s.n, s.ew);
    return true;
}
template <vec::Pair P>
bool opPairwise(Emu&, Call& c, void* ctx) {
    note(ctx);
    const Shape s = shapeK(c, 2);
    vec::pairwise(c.out, c.in[0], c.in[1], s.n, s.ew, P);
    return true;
}
template <vec::Reduce R, Sign S>
bool opReduce(Emu&, Call& c, void* ctx) {   // SMAXV ... SADDLV: the result is a scalar of c.outSize bytes
    note(ctx);
    const unsigned ew = c.nIn > 1 ? static_cast<unsigned>(c.in[1][0]) : c.inSize[0];
    vec::store(c.out, c.outSize, vec::reduce(c.in[0], c.inSize[0], ew, R, S));
    return true;
}
// The left shift count of the immediate forms of SQSHL, UQSHL and SQSHLU: a vector form is given the count, a scalar form the right shift count of its encoding (the description
// names the field Imm_shr_imm for both), so it is the lane's width less that.
unsigned leftCountOfImmediate(const Call& c, const Shape& s) {
    const unsigned value = static_cast<unsigned>(imm(c, 1));
    return c.nIn >= 3 ? value : 8 * s.ew - value;
}
// whether the second operand is an immediate: a vector form's is one byte, a register's the width of the vector; a scalar form's is smaller than the register, or for a byte
// scalar (where the sizes are the same) told by the instruction's bit 21, which is set in the register form
bool immediateForm(const Call& c, const Shape& s) {
    if (c.nIn >= 3) return c.inSize[1] == 1 && s.n > 1;
    return c.inSize[1] < s.n || (s.n == 1 && ((t_word >> 21) & 1) == 0);
}
template <Sign S, bool Round, bool Sat>
bool opShiftReg(Emu&, Call& c, void* ctx) {   // SSHL ... UQRSHL by the register's lanes; an immediate form (SQSHL, UQSHL by #imm) has a one byte second operand
    note(ctx);
    const Shape s = shapeK(c, 2);
    if (!immediateForm(c, s)) {
        vec::shiftNeon(c.out, c.in[0], c.in[1], s.n, s.ew, S, Round, Sat);
    } else {
        vec::shiftLeftSatImm(c.out, c.in[0], s.n, s.ew, S, leftCountOfImmediate(c, s), false);
    }
    return true;
}
bool opSqshlu(Emu&, Call& c, void* ctx) {
    note(ctx);
    const Shape s = shapeK(c, 2);
    vec::shiftLeftSatImm(c.out, c.in[0], s.n, s.ew, Sign::Signed, leftCountOfImmediate(c, s), true);
    return true;
}
template <Sign S, bool Round>
bool opShiftRightImm(Emu&, Call& c, void* ctx) {   // SSHR, USHR, SRSHR, URSHR
    note(ctx);
    const Shape s = shapeK(c, 2);
    vec::shiftRightImm(c.out, c.in[0], s.n, s.ew, S, static_cast<unsigned>(imm(c, 1)), Round);
    return true;
}
bool opShl(Emu&, Call& c, void* ctx) {   // SHL (scalar)
    note(ctx);
    const Shape s = shapeK(c, 2);
    vec::shiftImm(c.out, c.in[0], s.n, s.ew, vec::Shift::Left, imm(c, 1), vec::Count::Saturate);
    return true;
}
template <bool Left>
bool opShiftInsert(Emu&, Call& c, void* ctx) {   // SLI, SRI: Rd, Rn, #shift
    note(ctx);
    const Shape s{c.outSize, c.nIn > 3 ? static_cast<unsigned>(c.in[3][0]) : c.outSize};   // (a scalar form has no lane width: the third operand is the count)
    vec::shiftInsert(c.out, c.in[0], c.in[1], s.n, s.ew, Left, static_cast<unsigned>(imm(c, 2)));
    return true;
}
bool opSqdmulh(Emu&, Call& c, void* ctx) {
    note(ctx);
    const Shape s = shapeK(c, 2);
    vec::mulDoublingHigh(c.out, c.in[0], c.in[1], s.n, s.ew, false);
    return true;
}
bool opSqrdmulh(Emu&, Call& c, void* ctx) {
    note(ctx);
    const Shape s = shapeK(c, 2);
    vec::mulDoublingHigh(c.out, c.in[0], c.in[1], s.n, s.ew, true);
    return true;
}

// narrowing: the source is in[1]; the result is half as wide, in the low half of the result (the "2" forms: in the high half, the low half being Rd's)
void placeNarrow(Call& c, const Bytes& narrowed) {
    if (c.outSize == narrowed.size()) {
        std::memcpy(c.out, narrowed.data(), narrowed.size());
    } else {   // the "2" form
        std::memcpy(c.out, c.in[0], c.outSize / 2);
        std::memcpy(c.out + c.outSize / 2, narrowed.data(), narrowed.size());
    }
}
template <vec::Narrow M>
bool opNarrow(Emu&, Call& c, void* ctx) {
    note(ctx);
    const unsigned ew = c.nIn > 2 ? static_cast<unsigned>(c.in[2][0]) : c.inSize[1];
    Bytes r(c.inSize[1] / 2);
    vec::narrow(r.data(), c.in[1], c.inSize[1], ew, M);
    placeNarrow(c, r);
    return true;
}
template <vec::Narrow M, bool Round>
bool opNarrowShift(Emu&, Call& c, void* ctx) {   // (Rd, Rn, #shift, esize)
    note(ctx);
    const unsigned ew = c.nIn > 3 ? static_cast<unsigned>(c.in[3][0]) : c.inSize[1];
    Bytes r(c.inSize[1] / 2);
    vec::narrowShiftRight(r.data(), c.in[1], c.inSize[1], ew, static_cast<unsigned>(imm(c, 2)), Round, M);
    placeNarrow(c, r);
    return true;
}
template <bool Sub>
bool opRoundingHighNarrow(Emu&, Call& c, void* ctx) {   // RADDHN, RSUBHN (+2): (Rn +/- Rm + 2^(w/2-1)) >> w/2, truncated
    note(ctx);
    const unsigned ew = static_cast<unsigned>(c.in[3][0]);
    const std::size_t n = c.inSize[1];
    Bytes t(n), half(n, 0);
    if (Sub) vec::sub(t.data(), c.in[1], c.in[2], n, ew);
    else vec::add(t.data(), c.in[1], c.in[2], n, ew);
    for (std::size_t i = 0; i < n; i += ew) vec::store(half.data() + i, ew, u64{1} << (8 * ew / 2 - 1));
    vec::add(t.data(), t.data(), half.data(), n, ew);
    Bytes r(n / 2);
    vec::narrowShiftRight(r.data(), t.data(), n, ew, 4 * ew, false, vec::Narrow::Truncate);
    placeNarrow(c, r);
    return true;
}

// widening multiplies: the sources are 8 bytes ("1" forms) or the 16 byte registers whose upper half is used ("2" forms)
const std::uint8_t* upperIf(const Call& c, unsigned i, std::size_t outSize, std::size_t& n) {
    n = c.inSize[i];
    if (n * 2 == outSize) return c.in[i];   // the lower half's own 64 bit register
    n /= 2;
    return c.in[i] + n;
}
bool opUmull(Emu&, Call& c, void* ctx) {   // UMULL, UMULL2
    note(ctx);
    std::size_t n;
    const std::uint8_t* a = upperIf(c, 0, c.outSize, n);
    const std::uint8_t* b = upperIf(c, 1, c.outSize, n);
    vec::mulWiden(c.out, a, b, n, static_cast<unsigned>(c.in[2][0]), Sign::Unsigned, Sign::Unsigned);
    return true;
}
bool opSqdmull(Emu&, Call& c, void* ctx) {   // SQDMULL, SQDMULL2: saturate(2 a b)
    note(ctx);
    std::size_t n;
    const std::uint8_t* a = upperIf(c, 0, c.outSize, n);
    const std::uint8_t* b = upperIf(c, 1, c.outSize, n);
    vec::mulWiden(c.out, a, b, n, static_cast<unsigned>(c.in[2][0]), Sign::Signed, Sign::Signed);
    vec::addSat(c.out, c.out, c.out, c.outSize, 2 * static_cast<unsigned>(c.in[2][0]), Sign::Signed);
    return true;
}
bool opPmul(Emu&, Call& c, void* ctx) {
    note(ctx);
    const Shape s = shapeK(c, 2);
    vec::polyMul(c.out, c.in[0], c.in[1], s.n, s.ew);
    return true;
}
bool opPmull(Emu&, Call& c, void* ctx) {   // PMULL, PMULL2: the lower 8 bytes of the 8 byte registers, or the upper half of the 16 byte ones
    note(ctx);
    const unsigned ew = static_cast<unsigned>(c.in[2][0]);
    const std::size_t off = c.inSize[0] == 16 ? 8 : 0;
    if (ew == 8) vec::polyMul64(c.out, vec::load(c.in[0] + off, 8), vec::load(c.in[1] + off, 8));
    else vec::polyMulWiden(c.out, c.in[0] + off, c.in[1] + off, 8, ew);
    return true;
}

// ---- moving lanes ------------------------------------------------------------------------------------------------------------------------------------------------------

template <unsigned Group>
bool opRev(Emu&, Call& c, void* ctx) {   // REV64, REV32, REV16
    note(ctx);
    const Shape s = shapeK(c, 1);
    vec::reverseInGroups(c.out, c.in[0], s.n, s.ew, Group);
    return true;
}
bool opRbit(Emu&, Call& c, void* ctx) {
    note(ctx);
    vec::bitOp(c.out, c.in[0], c.outSize, 1, vec::BitOp::ReverseBits);
    return true;
}
bool opCls(Emu&, Call& c, void* ctx) {
    note(ctx);
    const Shape s = shapeK(c, 1);
    vec::bitOp(c.out, c.in[0], s.n, s.ew, vec::BitOp::CountLeadingSigns);
    return true;
}
bool opExt(Emu&, Call& c, void* ctx) {   // EXT (Rn, Rm, #index)
    note(ctx);
    vec::extract(c.out, c.in[0], c.in[1], c.outSize, static_cast<unsigned>(imm(c, 2)));
    return true;
}
bool opTbl(Emu&, Call& c, void* ctx) {   // a64_TBL(tblx, table registers..., indices): tblx is zero for TBL and the old Rd for TBX
    note(ctx);
    const unsigned tables = c.nIn - 2;
    std::uint8_t table[64];
    for (unsigned i = 0; i < tables; ++i) std::memcpy(table + 16 * i, c.in[1 + i], 16);
    vec::Lookup mode;
    mode.keepOutOfRange = true;   // (TBX keeps the old register where an index is out of range; for TBL tblx is zero, so the byte is zero)
    std::memcpy(c.out, c.in[0], c.outSize);
    vec::tableLookup(c.out, table, 16 * tables, c.in[c.nIn - 1], c.outSize, mode);
    return true;
}
bool opSimdPiece(Emu&, Call& c, void* ctx) {   // SIMD_PIECE(register, index): the lane of c.outSize bytes
    note(ctx);
    std::memcpy(c.out, c.in[0] + imm(c, 1) * c.outSize, c.outSize);
    return true;
}
bool opMpAbs(Emu&, Call& c, void* ctx) {   // MP_INT_ABS(x): the absolute value of a signed integer, in the same width
    note(ctx);
    const std::int64_t v = vec::signExtend(vec::load(c.in[0], c.inSize[0]), c.inSize[0]);
    vec::store(c.out, c.outSize, static_cast<u64>(v < 0 ? 0 - static_cast<u64>(v) : static_cast<u64>(v)));
    return true;
}

// ---- floating point lanes ----------------------------------------------------------------------------------------------------------------------------------------------

// The lane's maximum and minimum (the host's, which is what the own core computes): FMAX and FMIN give the default NaN when either operand is a NaN; FMAXNM and FMINNM the other
// operand; the pairwise FMAXNMP and FMINNMP the host's fmax and fmin. Singles and doubles only (the half precision forms go through the library).
void reduceFp(std::uint8_t* out, const std::uint8_t* a, std::size_t lanes, unsigned ew, vec::Fp2 op, const vec::FpEnv& env, unsigned& flags);
enum class HostMinMax : std::uint8_t { Max, Min, MaxNum, MinNum, PairMaxNum, PairMinNum };
template <class T>
T hostMinMax(T x, T y, HostMinMax mode) {
    switch (mode) {
        case HostMinMax::Max: return std::isnan(x) || std::isnan(y) ? std::numeric_limits<T>::quiet_NaN() : std::fmax(x, y);
        case HostMinMax::Min: return std::isnan(x) || std::isnan(y) ? std::numeric_limits<T>::quiet_NaN() : std::fmin(x, y);
        case HostMinMax::MaxNum: return std::isnan(x) ? y : std::isnan(y) ? x : std::fmax(x, y);
        case HostMinMax::MinNum: return std::isnan(x) ? y : std::isnan(y) ? x : std::fmin(x, y);
        case HostMinMax::PairMaxNum: return std::fmax(x, y);
        default: return std::fmin(x, y);
    }
}
template <class T>
void hostLanes(std::uint8_t* out, const std::uint8_t* a, const std::uint8_t* b, std::size_t n, HostMinMax mode) {
    for (std::size_t i = 0; i + sizeof(T) <= n; i += sizeof(T)) {
        T x, y;
        std::memcpy(&x, a + i, sizeof(T));
        std::memcpy(&y, b + i, sizeof(T));
        const T r = hostMinMax(x, y, mode);
        std::memcpy(out + i, &r, sizeof(T));
    }
}
template <HostMinMax M>
bool opFpMinMax(Emu& self, Call& c, void* ctx) {   // FMAX, FMIN, FMAXNM, FMINNM
    note(ctx);
    const Shape s = shapeK(c, 2);
    if (s.ew == 4 && g_laneCompat) {
        hostLanes<float>(c.out, c.in[0], c.in[1], s.n, M);
    } else if (s.ew == 8 && g_laneCompat) {
        hostLanes<double>(c.out, c.in[0], c.in[1], s.n, M);
    } else {
        vec::FpEnv env = fpEnv(self);
        env.minmax = (M == HostMinMax::Max || M == HostMinMax::Min) ? vec::MinMax::ArmPropagate : vec::MinMax::ArmNumber;
        unsigned flags = 0;
        vec::fp2(c.out, c.in[0], c.in[1], s.n, s.ew, (M == HostMinMax::Max || M == HostMinMax::MaxNum || M == HostMinMax::PairMaxNum) ? vec::Fp2::Max : vec::Fp2::Min, env, flags);
    }
    return true;
}
template <HostMinMax M>
bool opFpMinMaxPair(Emu& self, Call& c, void* ctx) {   // FMAXP, FMINP, FMAXNMP, FMINNMP
    note(ctx);
    const bool vector = c.nIn == 3;
    const unsigned ew = static_cast<unsigned>(c.in[vector ? 2 : 1][0]);
    const std::size_t n = vector ? c.outSize : ew;
    Bytes even(n), odd(n);
    if (vector) {
        vec::unzip(even.data(), c.in[0], c.in[1], n, ew, false);
        vec::unzip(odd.data(), c.in[0], c.in[1], n, ew, true);
    } else {
        std::memcpy(even.data(), c.in[0], ew);
        std::memcpy(odd.data(), c.in[0] + ew, ew);
    }
    if (ew == 4 && g_laneCompat) {
        hostLanes<float>(c.out, even.data(), odd.data(), n, M);
    } else if (ew == 8 && g_laneCompat) {
        hostLanes<double>(c.out, even.data(), odd.data(), n, M);
    } else {
        vec::FpEnv env = fpEnv(self);
        env.minmax = (M == HostMinMax::Max || M == HostMinMax::Min) ? vec::MinMax::ArmPropagate : vec::MinMax::ArmNumber;
        unsigned flags = 0;
        vec::fp2(c.out, even.data(), odd.data(), n, ew, (M == HostMinMax::Max || M == HostMinMax::MaxNum || M == HostMinMax::PairMaxNum) ? vec::Fp2::Max : vec::Fp2::Min, env, flags);
    }
    return true;
}
template <bool Minimum>
bool opFpAcross(Emu& self, Call& c, void* ctx) {   // FMAXV, FMINV, FMAXNMV, FMINNMV: the own core folds four singles with the host's fmax or fmin
    note(ctx);
    const unsigned ew = static_cast<unsigned>(c.in[1][0]);
    if (ew == 4 && c.inSize[0] == 16 && g_laneCompat) {
        float r;
        std::memcpy(&r, c.in[0], 4);
        for (unsigned i = 1; i < 4; ++i) {
            float x;
            std::memcpy(&x, c.in[0] + 4 * i, 4);
            r = Minimum ? std::fmin(r, x) : std::fmax(r, x);
        }
        std::memcpy(c.out, &r, 4);
        return true;
    }
    vec::FpEnv env = fpEnv(self);
    env.minmax = vec::MinMax::ArmNumber;
    unsigned flags = 0;
    reduceFp(c.out, c.in[0], c.inSize[0] / ew, ew, Minimum ? vec::Fp2::Min : vec::Fp2::Max, env, flags);
    return true;
}

template <vec::Fp2 Op, vec::MinMax MM>
bool opFp2(Emu& self, Call& c, void* ctx) {   // FMULX: the lane width is the constant, or the whole operand of a scalar form
    note(ctx);
    const Shape s = shapeK(c, 2);
    vec::FpEnv env = fpEnv(self);
    env.minmax = MM;
    unsigned flags = 0;
    vec::fp2(c.out, c.in[0], c.in[1], s.n, s.ew, Op, env, flags);
    return true;
}

// the pairs of a vector form (Rn, Rm, esize): [n0 op n1, n2 op n3 ... m0 op m1 ...]; a scalar form (Rn, esize) is the one pair of the register

void reduceFp(std::uint8_t* out, const std::uint8_t* a, std::size_t lanes, unsigned ew, vec::Fp2 op, const vec::FpEnv& env, unsigned& flags) {   // the manual's tree: low half, high half, combine
    if (lanes == 1) {
        std::memcpy(out, a, ew);
        return;
    }
    std::uint8_t lo[8], hi[8];
    reduceFp(lo, a, lanes / 2, ew, op, env, flags);
    reduceFp(hi, a + (lanes / 2) * ew, lanes / 2, ew, op, env, flags);
    vec::fp2(out, lo, hi, ew, ew, op, env, flags);
}
bool opFmov(Emu&, Call& c, void* ctx) {   // FMOV (vector, immediate): the expanded immediate in every lane
    note(ctx);
    const unsigned ew = static_cast<unsigned>(c.in[1][0]);
    vec::splat(c.out, c.outSize, ew, imm(c, 0));
    return true;
}

// the scalar form of a float conversion has the fractional bits as its second operand, a vector form has them (or not) before the lane width; the one case the arguments do not tell
// apart is a 64 bit scalar with its immediate in one byte from a 64 bit vector's lane width, which the instruction's bit 28 does
struct ConvertShape {
    std::size_t n;
    unsigned ew;
    unsigned fbits;
};
ConvertShape convertShape(const Call& c) {
    const bool scalar = c.nIn == 2 && (c.outSize < 8 || (c.outSize == 8 && ((t_word >> 28) & 1) != 0));
    if (scalar) return {c.outSize, c.outSize, static_cast<unsigned>(imm(c, 1))};
    if (c.nIn == 1) return {c.outSize, c.outSize, 0};   // (a scalar without fractional bits)
    if (c.nIn == 3) return {c.outSize, static_cast<unsigned>(c.in[2][0]), static_cast<unsigned>(imm(c, 1))};
    return {c.outSize, static_cast<unsigned>(c.in[1][0]), 0};
}
template <Sign S>
bool opFloatToInt(Emu& self, Call& c, void* ctx) {   // FCVTZS, FCVTZU: toward zero, saturating
    note(ctx);
    const ConvertShape s = convertShape(c);
    vec::FpEnv env = fpEnv(self);
    env.round = vec::Round::TowardZero;
    unsigned flags = 0;
    vec::floatToInt(c.out, c.in[0], s.n, s.ew, s.ew, S, s.fbits, vec::IntOverflow::Saturate, env, flags);
    return true;
}
template <Sign S>
bool opIntToFloat(Emu& self, Call& c, void* ctx) {   // SCVTF, UCVTF
    note(ctx);
    const ConvertShape s = convertShape(c);
    unsigned flags = 0;
    vec::intToFloat(c.out, c.in[0], s.n, s.ew, s.ew, S, s.fbits, fpEnv(self), flags);
    return true;
}

// An operation nothing serves yet records the shape of each call (the sizes of its result and arguments, and the value of the small ones) so that its form can be read off from what
// the description really passes; it declines the call, which is counted as before.
thread_local std::map<std::string, std::set<std::string>> t_shapes;
bool recordShape(Emu&, Call& c, void* ctx) {
    std::string shape = std::to_string(c.outSize) + ":";
    for (unsigned i = 0; i < c.nIn; ++i) {
        shape += (i ? "," : "") + std::to_string(c.inSize[i]);
        if (c.inSize[i] <= 4) shape += "=" + std::to_string(vec::load(c.in[i], c.inSize[i]));
    }
    auto& seen = t_shapes[std::string(static_cast<const OpContext*>(ctx)->name)];
    if (seen.size() < 6) seen.insert(shape);
    return false;
}

// ---- instructions whose description leaves the accumulator out of the arguments: the old Rd is read from the register the instruction names ----------------------------------

void oldRd(Emu& self, std::uint8_t* out16) {
    thread_local std::array<Emu::RegRef, 32> refs;
    thread_local const Emu* owner = nullptr;
    if (owner != &self) {
        for (unsigned i = 0; i < 32; ++i) refs[i] = self.regRef("q" + std::to_string(i));
        owner = &self;
    }
    self.readRegWide(refs[t_word & 31], out16);
}

// FCADD (Rn, Rm, #rotate, esize): complex add of the pairs (real, imaginary) of the lanes, with Rm turned a quarter or three quarters of a turn
bool opFcadd(Emu& self, Call& c, void* ctx) {
    note(ctx);
    const unsigned ew = static_cast<unsigned>(c.in[3][0]);
    const bool rot270 = imm(c, 2) == 270;
    vec::FpEnv env = fpEnv(self);
    unsigned flags = 0;
    for (std::size_t i = 0; i + 2 * ew <= c.outSize; i += 2 * ew) {
        std::uint8_t* re = c.out + i;
        std::uint8_t* im = c.out + i + ew;
        // 90: re = a_re - b_im, im = a_im + b_re;  270: re = a_re + b_im, im = a_im - b_re
        vec::fp2(re, c.in[0] + i, c.in[1] + i + ew, ew, ew, rot270 ? vec::Fp2::Add : vec::Fp2::Sub, env, flags);
        vec::fp2(im, c.in[0] + i + ew, c.in[1] + i, ew, ew, rot270 ? vec::Fp2::Sub : vec::Fp2::Add, env, flags);
    }
    return true;
}

// FCMLA (Rn, Rm, #rotate, esize), accumulating into the old Rd; by element, Rm is one complex pair
bool opFcmla(Emu& self, Call& c, void* ctx) {
    note(ctx);
    const unsigned ew = static_cast<unsigned>(c.in[3][0]);
    const unsigned rotate = static_cast<unsigned>(imm(c, 2));
    std::uint8_t acc[16];
    oldRd(self, acc);
    vec::FpEnv env = fpEnv(self);
    unsigned flags = 0;
    const bool element = c.inSize[1] < c.outSize;
    // The description passes the half precision form by element one 2 byte piece of the register at the index H:L, where the complex pair is the 4 bytes at that index of 4 byte
    // pairs: the pair is taken from the register the word names.
    std::uint8_t pair[16];
    const std::uint8_t* m = c.in[1];
    if (element && ew == 2 && c.inSize[1] == 2) {
        thread_local std::array<Emu::RegRef, 32> refs;
        thread_local const Emu* owner = nullptr;
        if (owner != &self) {
            for (unsigned i = 0; i < 32; ++i) refs[i] = self.regRef("q" + std::to_string(i));
            owner = &self;
        }
        std::uint8_t reg[16];
        self.readRegWide(refs[(t_word >> 16) & 31], reg);
        const unsigned index = ((t_word >> 11) & 1) << 1 | ((t_word >> 21) & 1);
        std::memcpy(pair, reg + 4 * index, 4);
        m = pair;
    }
    for (std::size_t i = 0; i + 2 * ew <= c.outSize; i += 2 * ew) {
        const std::uint8_t* nr = c.in[0] + i;
        const std::uint8_t* ni = c.in[0] + i + ew;
        const std::uint8_t* mr = element ? m : m + i;
        const std::uint8_t* mi = element ? m + ew : m + i + ew;
        // the manual's (and QEMU's) form: the multiplicand is Rn's real or imaginary part (the imaginary one for the quarter and three-quarter turns), and what is negated is Rm's
        const bool flip = rotate == 90 || rotate == 270, negate_real = rotate == 90 || rotate == 180, negate_imag = rotate >= 180;
        const std::uint8_t* multiplicand = flip ? ni : nr;
        std::uint8_t neg_a[8], neg_b[8];
        const auto negated = [&](const std::uint8_t* p, std::uint8_t* store, bool negate) {
            if (!negate) return p;
            std::memcpy(store, p, ew);
            store[ew - 1] ^= 0x80;
            return static_cast<const std::uint8_t*>(store);
        };
        const std::uint8_t* e1 = negated(flip ? mi : mr, neg_a, negate_real);
        const std::uint8_t* e2 = multiplicand;
        const std::uint8_t* e3 = negated(flip ? mr : mi, neg_b, negate_imag);
        const std::uint8_t* e4 = multiplicand;
        vec::fp3(c.out + i, e2, e1, acc + i, ew, ew, vec::Fp3::MulAdd, env, flags);
        vec::fp3(c.out + i + ew, e4, e3, acc + i + ew, ew, ew, vec::Fp3::MulAdd, env, flags);
    }
    return true;
}

// the sum of four byte products into each 32 bit lane: SDOT, UDOT take the accumulator from the old Rd (the description does not pass it), USDOT and SUDOT are given it
template <Sign A, Sign B, bool Acc>
bool opDot(Emu& self, Call& c, void* ctx) {
    note(ctx);
    const unsigned first = Acc ? 1 : 0;   // (the arguments are Rd?, Rn, Rm or the 4 byte element)
    std::uint8_t acc[16], element[16];
    if (!Acc) oldRd(self, acc);
    const std::uint8_t* a = c.in[first];
    const std::uint8_t* b = c.in[first + 1];
    if (c.inSize[first + 1] == 4 && c.outSize > 4) {   // (by element: the same four bytes for every lane)
        for (std::size_t i = 0; i < c.outSize; i += 4) std::memcpy(element + i, b, 4);
        b = element;
    }
    vec::multiplyAdd(c.out, Acc ? c.in[0] : acc, a, b, c.outSize, 1, 4, A, B, false);
    return true;
}

// SMMLA, UMMLA, USMMLA (Rd, Rn, Rm): a 2 by 8 matrix of bytes times the transpose of another into a 2 by 2 matrix of 32 bit lanes
template <Sign A, Sign B>
bool opMatMul(Emu&, Call& c, void* ctx) {
    note(ctx);
    for (unsigned i = 0; i < 2; ++i)
        for (unsigned j = 0; j < 2; ++j) {
            std::int64_t sum = static_cast<std::int32_t>(vec::load(c.in[0] + 4 * (2 * i + j), 4));
            for (unsigned k = 0; k < 8; ++k) {
                const std::int64_t x = A == Sign::Signed ? static_cast<std::int8_t>(c.in[1][8 * i + k]) : c.in[1][8 * i + k];
                const std::int64_t y = B == Sign::Signed ? static_cast<std::int8_t>(c.in[2][8 * j + k]) : c.in[2][8 * j + k];
                sum += x * y;
            }
            vec::store(c.out + 4 * (2 * i + j), 4, static_cast<u64>(sum));
        }
    return true;
}

// ---- AES: one round of the cipher on a 16 byte state (the manual's AESE, AESD, AESMC, AESIMC) ----------------------------------------------------------------------------

struct AesTables {
    std::uint8_t sbox[256], inverse[256];
    AesTables() {   // the S-box is the multiplicative inverse in GF(2^8) (modulus x^8 + x^4 + x^3 + x + 1) followed by the affine map
        const auto multiply = [](std::uint8_t a, std::uint8_t b) {
            std::uint8_t product = 0;
            for (int i = 0; i < 8; ++i) {
                if ((b & 1) != 0) product ^= a;
                const bool carry = (a & 0x80) != 0;
                a = static_cast<std::uint8_t>(a << 1);
                if (carry) a ^= 0x1B;
                b >>= 1;
            }
            return product;
        };
        for (unsigned x = 0; x < 256; ++x) {
            std::uint8_t inv = 0;
            for (unsigned y = 1; y < 256 && x != 0; ++y)
                if (multiply(static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(y)) == 1) {
                    inv = static_cast<std::uint8_t>(y);
                    break;
                }
            std::uint8_t s = inv;
            for (int k = 1; k <= 4; ++k) s ^= static_cast<std::uint8_t>((inv << k) | (inv >> (8 - k)));
            s ^= 0x63;
            sbox[x] = s;
            inverse[s] = static_cast<std::uint8_t>(x);
        }
    }
};
const AesTables& aesTables() {
    static const AesTables tables;
    return tables;
}
std::uint8_t gfMul(std::uint8_t a, std::uint8_t b) {
    std::uint8_t product = 0;
    for (int i = 0; i < 8; ++i) {
        if ((b & 1) != 0) product ^= a;
        const bool carry = (a & 0x80) != 0;
        a = static_cast<std::uint8_t>(a << 1);
        if (carry) a ^= 0x1B;
        b >>= 1;
    }
    return product;
}
template <bool Decrypt>
bool opAesRound(Emu&, Call& c, void*ctx) {   // AESE, AESD (Rd, Rn): Rd xor Rn, then the S-box and the row shift
    note(ctx);
    static const unsigned shift[16] = {0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 1, 6, 11};
    static const unsigned inverseShift[16] = {0, 13, 10, 7, 4, 1, 14, 11, 8, 5, 2, 15, 12, 9, 6, 3};
    std::uint8_t state[16];
    for (unsigned i = 0; i < 16; ++i) state[i] = c.in[0][i] ^ c.in[1][i];
    const AesTables& t = aesTables();
    for (unsigned i = 0; i < 16; ++i) c.out[i] = Decrypt ? t.inverse[state[inverseShift[i]]] : t.sbox[state[shift[i]]];
    return true;
}
template <bool Inverse>
bool opAesMix(Emu&, Call& c, void* ctx) {   // AESMC, AESIMC (Rd, Rn): MixColumns of Rn
    note(ctx);
    static const std::uint8_t forward[4] = {2, 3, 1, 1}, backward[4] = {14, 11, 13, 9};
    const std::uint8_t* m = Inverse ? backward : forward;
    for (unsigned col = 0; col < 4; ++col)
        for (unsigned row = 0; row < 4; ++row) {
            std::uint8_t v = 0;
            for (unsigned k = 0; k < 4; ++k) v ^= gfMul(c.in[1][4 * col + k], m[(k + 4 - row) % 4]);
            c.out[4 * col + row] = v;
        }
    return true;
}

// ---- SHA-1 and SHA-256 (the manual's SHA1C, SHA1P, SHA1M, SHA1SU0, SHA1SU1, SHA256H, SHA256H2, SHA256SU0, SHA256SU1) --------------------------------------------------------

std::uint32_t rol32(std::uint32_t x, unsigned n) { return (x << n) | (x >> (32 - n)); }
std::uint32_t ror32(std::uint32_t x, unsigned n) { return (x >> n) | (x << (32 - n)); }
std::uint32_t word32(const std::uint8_t* p, unsigned i) { return static_cast<std::uint32_t>(vec::load(p + 4 * i, 4)); }
void setWord32(std::uint8_t* p, unsigned i, std::uint32_t v) { vec::store(p + 4 * i, 4, v); }

template <int Kind>   // 0: choose (SHA1C), 1: parity (SHA1P), 2: majority (SHA1M)
bool opSha1Hash(Emu&, Call& c, void* ctx) {
    note(ctx);
    std::uint32_t x[4], y = word32(c.in[1], 0);
    for (unsigned i = 0; i < 4; ++i) x[i] = word32(c.in[0], i);
    for (unsigned e = 0; e < 4; ++e) {
        const std::uint32_t a = x[1], b = x[2], d = x[3];
        const std::uint32_t t = Kind == 0 ? (a & b) | (~a & d) : Kind == 1 ? a ^ b ^ d : (a & b) | (a & d) | (b & d);
        y = y + rol32(x[0], 5) + t + word32(c.in[2], e);
        x[1] = rol32(x[1], 30);
        const std::uint32_t top = x[3];   // <Y, X> = ROL(Y : X, 32)
        x[3] = x[2];
        x[2] = x[1];
        x[1] = x[0];
        x[0] = y;
        y = top;
    }
    for (unsigned i = 0; i < 4; ++i) setWord32(c.out, i, x[i]);
    return true;
}
bool opSha1Su0(Emu&, Call& c, void* ctx) {   // (Rd, Rn, Rm): (Rn<63:0> : Rd<127:64>) xor Rd xor Rm
    note(ctx);
    const std::uint32_t t[4] = {word32(c.in[0], 2), word32(c.in[0], 3), word32(c.in[1], 0), word32(c.in[1], 1)};
    for (unsigned i = 0; i < 4; ++i) setWord32(c.out, i, t[i] ^ word32(c.in[0], i) ^ word32(c.in[2], i));
    return true;
}
bool opSha1Su1(Emu&, Call& c, void* ctx) {   // (Rd, Rn)
    note(ctx);
    const std::uint32_t t[4] = {word32(c.in[0], 0) ^ word32(c.in[1], 1), word32(c.in[0], 1) ^ word32(c.in[1], 2), word32(c.in[0], 2) ^ word32(c.in[1], 3), word32(c.in[0], 3)};
    setWord32(c.out, 0, rol32(t[0], 1));
    setWord32(c.out, 1, rol32(t[1], 1));
    setWord32(c.out, 2, rol32(t[2], 1));
    setWord32(c.out, 3, rol32(t[3], 1) ^ rol32(t[0], 2));
    return true;
}
template <bool Part1>
bool opSha256Hash(Emu&, Call& c, void* ctx) {   // SHA256H (Rd, Rn, Rm): X = Rd, Y = Rn; SHA256H2: X = Rn, Y = Rd
    note(ctx);
    std::uint32_t x[4], y[4];
    for (unsigned i = 0; i < 4; ++i) {
        x[i] = word32(c.in[Part1 ? 0 : 1], i);
        y[i] = word32(c.in[Part1 ? 1 : 0], i);
    }
    for (unsigned e = 0; e < 4; ++e) {
        const std::uint32_t chs = (y[0] & y[1]) | (~y[0] & y[2]), maj = (x[0] & x[1]) | (x[0] & x[2]) | (x[1] & x[2]);
        const std::uint32_t t1 = y[3] + (ror32(y[0], 6) ^ ror32(y[0], 11) ^ ror32(y[0], 25)) + chs + word32(c.in[2], e);
        x[3] = t1 + x[3];
        y[3] = t1 + (ror32(x[0], 2) ^ ror32(x[0], 13) ^ ror32(x[0], 22)) + maj;
        const std::uint32_t xtop = x[3], ytop = y[3];   // the 256 bit value Y : X rotated left by 32
        x[3] = x[2];
        x[2] = x[1];
        x[1] = x[0];
        x[0] = ytop;
        y[3] = y[2];
        y[2] = y[1];
        y[1] = y[0];
        y[0] = xtop;
    }
    for (unsigned i = 0; i < 4; ++i) setWord32(c.out, i, Part1 ? x[i] : y[i]);
    return true;
}
bool opSha256Su0(Emu&, Call& c, void* ctx) {   // (Rd, Rn)
    note(ctx);
    const std::uint32_t t[4] = {word32(c.in[0], 1), word32(c.in[0], 2), word32(c.in[0], 3), word32(c.in[1], 0)};
    for (unsigned i = 0; i < 4; ++i) setWord32(c.out, i, (ror32(t[i], 7) ^ ror32(t[i], 18) ^ (t[i] >> 3)) + word32(c.in[0], i));
    return true;
}
bool opSha256Su1(Emu&, Call& c, void* ctx) {   // (Rd, Rn, Rm)
    note(ctx);
    const std::uint32_t t0[4] = {word32(c.in[1], 1), word32(c.in[1], 2), word32(c.in[1], 3), word32(c.in[2], 0)};   // Rm<31:0> : Rn<127:32>
    std::uint32_t t1[4] = {word32(c.in[2], 2), word32(c.in[2], 3), 0, 0};
    std::uint32_t r[4];
    for (unsigned e = 0; e < 4; ++e) {
        const std::uint32_t elt = t1[e];
        r[e] = (ror32(elt, 17) ^ ror32(elt, 19) ^ (elt >> 10)) + word32(c.in[0], e) + t0[e];
        if (e == 1) {
            t1[2] = r[0];
            t1[3] = r[1];
        }
    }
    for (unsigned i = 0; i < 4; ++i) setWord32(c.out, i, r[i]);
    return true;
}

// FRECPX (scalar): the sign, the exponent field complemented (the largest finite one for a zero or denormal), no fraction; a NaN comes back quieted
bool opFrecpx(Emu&, Call& c, void* ctx) {
    note(ctx);
    const unsigned ew = c.outSize;
    const u64 v = vec::load(c.in[0], ew);
    const unsigned shift = ew == 2 ? 10 : ew == 4 ? 23 : 52, width = ew == 2 ? 5 : ew == 4 ? 8 : 11;
    const u64 field = (v >> shift) & ((u64{1} << width) - 1), fraction = v & ((u64{1} << shift) - 1), sign = v & (u64{1} << (8 * ew - 1));
    u64 result;
    if (field == (u64{1} << width) - 1 && fraction != 0) result = v | (u64{1} << (shift - 1));
    else result = sign | ((field == 0 ? (u64{1} << width) - 2 : ~field & ((u64{1} << width) - 1)) << shift);
    vec::store(c.out, ew, result);
    return true;
}

// SQRDMLAH, SQRDMLSH (Rd, Rn, Rm, esize in bits, elements, subtract[, index]): the rounding doubling multiply high accumulating into Rd; with an index, Rm is one lane of the register
bool opSqrdml(Emu&, Call& c, void* ctx) {
    note(ctx);
    const unsigned ew = static_cast<unsigned>(imm(c, 3)) / 8;
    const std::size_t n = static_cast<std::size_t>(imm(c, 4)) * ew;
    const bool subtract = imm(c, 5) != 0;
    std::uint8_t splat[16];
    const std::uint8_t* b = c.in[2];
    if (c.nIn > 6) {
        vec::splat(splat, 16, ew, vec::load(c.in[2] + imm(c, 6) * ew, ew));
        b = splat;
    }
    std::memcpy(c.out, c.in[0], c.outSize);
    vec::mulDoublingHighAcc(c.out, c.in[0], c.in[1], b, n, ew, subtract);
    return true;
}

bool isNanBits(u64 v, unsigned ew) { return ew == 2 ? (v & 0x7C00) == 0x7C00 && (v & 0x3FF) != 0 : ew == 4 ? (v & 0x7F800000u) == 0x7F800000u && (v & 0x7FFFFF) != 0 : (v & 0x7FF0000000000000ull) == 0x7FF0000000000000ull && (v & 0xFFFFFFFFFFFFFull) != 0; }
bool isInfBits(u64 v, unsigned ew) { return ew == 2 ? (v & 0x7FFF) == 0x7C00 : ew == 4 ? (v & 0x7FFFFFFFu) == 0x7F800000u : (v & 0x7FFFFFFFFFFFFFFFull) == 0x7FF0000000000000ull; }
bool isZeroBits(u64 v, unsigned ew) { return (v & (vec::laneMask(ew) >> 1)) == 0; }
u64 floatBits(double v, unsigned ew) {   // 1.5, 2.0, 3.0 and 0.5 are exact in every width
    if (ew == 8) return std::bit_cast<u64>(v);
    if (ew == 4) return std::bit_cast<std::uint32_t>(static_cast<float>(v));
    return v == 2.0 ? 0x4000 : v == 3.0 ? 0x4200 : v == 1.5 ? 0x3E00 : 0x3800;
}

// FRECPS, FRSQRTS: 2 - a b and (3 - a b) / 2 with one rounding, an infinity times a zero giving 2.0 and 1.5; the half precision scalar form of FRSQRTS is passed one argument
template <bool Sqrt>
bool opRecipStep(Emu& self, Call& c, void* ctx) {
    note(ctx);
    const Shape s = shapeK(c, 2);
    if (c.nIn < 2) return false;
    vec::FpEnv env = fpEnv(self);
    unsigned flags = 0;
    for (std::size_t i = 0; i < s.n; i += s.ew) {
        const u64 a = vec::load(c.in[0] + i, s.ew), b = vec::load(c.in[1] + i, s.ew);
        if (!isNanBits(a, s.ew) && !isNanBits(b, s.ew) && ((isInfBits(a, s.ew) && isZeroBits(b, s.ew)) || (isZeroBits(a, s.ew) && isInfBits(b, s.ew)))) {
            vec::store(c.out + i, s.ew, floatBits(Sqrt ? 1.5 : 2.0, s.ew));
            continue;
        }
        std::uint8_t three[8], half[8], negated[8];
        vec::store(three, s.ew, floatBits(Sqrt ? 3.0 : 2.0, s.ew));
        std::memcpy(negated, c.in[0] + i, s.ew);   // (the first operand is negated before anything else, a NaN included: the NaN that comes back has the opposite sign)
        negated[s.ew - 1] ^= 0x80;
        vec::fp3(c.out + i, negated, c.in[1] + i, three, s.ew, s.ew, vec::Fp3::MulAdd, env, flags);
        if (Sqrt) {   // the halving of the fused result
            vec::store(half, s.ew, floatBits(0.5, s.ew));
            vec::fp2(c.out + i, c.out + i, half, s.ew, s.ew, vec::Fp2::Mul, env, flags);
        }
    }
    return true;
}

}  // namespace

const Table& a64_table() {
    static const Table table{
        {"NEON_sqadd", opAddSat<Sign::Signed>},
        {"NEON_uqadd", opAddSat<Sign::Unsigned>},
        {"NEON_sqsub", opSubSat<Sign::Signed>},
        {"NEON_uqsub", opSubSat<Sign::Unsigned>},
        {"NEON_usqadd", opUsqadd},
        {"NEON_shadd", opHalvingAdd<Sign::Signed, false>},
        {"NEON_uhadd", opHalvingAdd<Sign::Unsigned, false>},
        {"NEON_srhadd", opHalvingAdd<Sign::Signed, true>},
        {"NEON_urhadd", opHalvingAdd<Sign::Unsigned, true>},
        {"NEON_shsub", opHalvingSub<Sign::Signed>},
        {"NEON_uhsub", opHalvingSub<Sign::Unsigned>},
        {"NEON_smax", opMax<Sign::Signed>},
        {"NEON_umax", opMax<Sign::Unsigned>},
        {"NEON_smin", opMin<Sign::Signed>},
        {"NEON_umin", opMin<Sign::Unsigned>},
        {"NEON_sabd", opAbsDiff<Sign::Signed>},
        {"NEON_uabd", opAbsDiff<Sign::Unsigned>},
        {"NEON_saba", opAbsDiffAcc<Sign::Signed>},
        {"NEON_uaba", opAbsDiffAcc<Sign::Unsigned>},
        {"NEON_smaxp", opPairwise<vec::Pair::MaxSigned>},
        {"NEON_umaxp", opPairwise<vec::Pair::MaxUnsigned>},
        {"NEON_sminp", opPairwise<vec::Pair::MinSigned>},
        {"NEON_uminp", opPairwise<vec::Pair::MinUnsigned>},
        {"NEON_smaxv", opReduce<vec::Reduce::Max, Sign::Signed>},
        {"NEON_umaxv", opReduce<vec::Reduce::Max, Sign::Unsigned>},
        {"NEON_sminv", opReduce<vec::Reduce::Min, Sign::Signed>},
        {"NEON_uminv", opReduce<vec::Reduce::Min, Sign::Unsigned>},
        {"NEON_saddlv", opReduce<vec::Reduce::AddLong, Sign::Signed>},
        {"NEON_uaddlv", opReduce<vec::Reduce::AddLong, Sign::Unsigned>},
        {"NEON_sshl", opShiftReg<Sign::Signed, false, false>},
        {"NEON_ushl", opShiftReg<Sign::Unsigned, false, false>},
        {"NEON_srshl", opShiftReg<Sign::Signed, true, false>},
        {"NEON_urshl", opShiftReg<Sign::Unsigned, true, false>},
        {"NEON_sqshl", opShiftReg<Sign::Signed, false, true>},
        {"NEON_uqshl", opShiftReg<Sign::Unsigned, false, true>},
        {"NEON_sqrshl", opShiftReg<Sign::Signed, true, true>},
        {"NEON_uqrshl", opShiftReg<Sign::Unsigned, true, true>},
        {"NEON_sqshlu", opSqshlu},
        {"NEON_sshr", opShiftRightImm<Sign::Signed, false>},
        {"NEON_srshr", opShiftRightImm<Sign::Signed, true>},
        {"NEON_urshr", opShiftRightImm<Sign::Unsigned, true>},
        {"NEON_shl", opShl},
        {"NEON_sli", opShiftInsert<true>},
        {"NEON_sri", opShiftInsert<false>},
        {"NEON_sqdmulh", opSqdmulh},
        {"NEON_sqrdmulh", opSqrdmulh},
        {"NEON_sqxtn", opNarrow<vec::Narrow::SignedToSigned>},
        {"NEON_sqxtn2", opNarrow<vec::Narrow::SignedToSigned>},
        {"NEON_uqxtn", opNarrow<vec::Narrow::UnsignedToUnsigned>},
        {"NEON_uqxtn2", opNarrow<vec::Narrow::UnsignedToUnsigned>},
        {"NEON_sqxtun", opNarrow<vec::Narrow::SignedToUnsigned>},
        {"NEON_sqxtun2", opNarrow<vec::Narrow::SignedToUnsigned>},
        {"NEON_sqshrn", opNarrowShift<vec::Narrow::SignedToSigned, false>},
        {"NEON_sqshrn2", opNarrowShift<vec::Narrow::SignedToSigned, false>},
        {"NEON_sqrshrn", opNarrowShift<vec::Narrow::SignedToSigned, true>},
        {"NEON_sqrshrn2", opNarrowShift<vec::Narrow::SignedToSigned, true>},
        {"NEON_uqshrn", opNarrowShift<vec::Narrow::UnsignedToUnsigned, false>},
        {"NEON_uqshrn2", opNarrowShift<vec::Narrow::UnsignedToUnsigned, false>},
        {"NEON_uqrshrn", opNarrowShift<vec::Narrow::UnsignedToUnsigned, true>},
        {"NEON_uqrshrn2", opNarrowShift<vec::Narrow::UnsignedToUnsigned, true>},
        {"NEON_sqshrun", opNarrowShift<vec::Narrow::SignedToUnsigned, false>},
        {"NEON_sqshrun2", opNarrowShift<vec::Narrow::SignedToUnsigned, false>},
        {"NEON_sqrshrun", opNarrowShift<vec::Narrow::SignedToUnsigned, true>},
        {"NEON_sqrshrun2", opNarrowShift<vec::Narrow::SignedToUnsigned, true>},
        {"NEON_rshrn", opNarrowShift<vec::Narrow::Truncate, true>},
        {"NEON_rshrn2", opNarrowShift<vec::Narrow::Truncate, true>},
        {"NEON_raddhn", opRoundingHighNarrow<false>},
        {"NEON_raddhn2", opRoundingHighNarrow<false>},
        {"NEON_rsubhn", opRoundingHighNarrow<true>},
        {"NEON_rsubhn2", opRoundingHighNarrow<true>},
        {"NEON_umull", opUmull},
        {"NEON_sqdmull", opSqdmull},
        {"NEON_pmul", opPmul},
        {"NEON_pmull", opPmull},
        {"NEON_pmull2", opPmull},
        {"NEON_fmax", opFpMinMax<HostMinMax::Max>},
        {"NEON_fmin", opFpMinMax<HostMinMax::Min>},
        {"NEON_fmaxnm", opFpMinMax<HostMinMax::MaxNum>},
        {"NEON_fminnm", opFpMinMax<HostMinMax::MinNum>},
        {"NEON_fmulx", opFp2<vec::Fp2::MulExtended, vec::MinMax::ArmPropagate>},
        {"NEON_fmaxp", opFpMinMaxPair<HostMinMax::Max>},
        {"NEON_fminp", opFpMinMaxPair<HostMinMax::Min>},
        {"NEON_fmaxnmp", opFpMinMaxPair<HostMinMax::PairMaxNum>},
        {"NEON_fminnmp", opFpMinMaxPair<HostMinMax::PairMinNum>},
        {"NEON_fmaxv", opFpAcross<false>},
        {"NEON_fminv", opFpAcross<true>},
        {"NEON_fmaxnmv", opFpAcross<false>},
        {"NEON_fminnmv", opFpAcross<true>},
        {"NEON_fmov", opFmov},
        {"NEON_fcvtzs", opFloatToInt<Sign::Signed>},
        {"NEON_fcvtzu", opFloatToInt<Sign::Unsigned>},
        {"NEON_scvtf", opIntToFloat<Sign::Signed>},
        {"NEON_ucvtf", opIntToFloat<Sign::Unsigned>},
        {"NEON_sqrdml_as_h", opSqrdml},
        {"NEON_frecpx", opFrecpx},
        {"NEON_sha1c", opSha1Hash<0>},
        {"NEON_sha1p", opSha1Hash<1>},
        {"NEON_sha1m", opSha1Hash<2>},
        {"NEON_sha1su0", opSha1Su0},
        {"NEON_sha1su1", opSha1Su1},
        {"NEON_sha256h", opSha256Hash<true>},
        {"NEON_sha256h2", opSha256Hash<false>},
        {"NEON_sha256su0", opSha256Su0},
        {"NEON_sha256su1", opSha256Su1},
        {"NEON_aese", opAesRound<false>},
        {"NEON_aesd", opAesRound<true>},
        {"NEON_aesmc", opAesMix<false>},
        {"NEON_aesimc", opAesMix<true>},
        {"NEON_fcadd", opFcadd},
        {"NEON_fcmla", opFcmla},
        {"NEON_sdot", opDot<Sign::Signed, Sign::Signed, false>},
        {"NEON_udot", opDot<Sign::Unsigned, Sign::Unsigned, false>},
        {"NEON_usdot", opDot<Sign::Unsigned, Sign::Signed, true>},
        {"NEON_sudot", opDot<Sign::Signed, Sign::Unsigned, true>},
        {"NEON_smmla", opMatMul<Sign::Signed, Sign::Signed>},
        {"NEON_ummla", opMatMul<Sign::Unsigned, Sign::Unsigned>},
        {"NEON_usmmla", opMatMul<Sign::Unsigned, Sign::Signed>},
        {"NEON_frecps", opRecipStep<false>},
        {"NEON_frsqrts", opRecipStep<true>},
        {"NEON_rev64", opRev<8>},
        {"NEON_rev32", opRev<4>},
        {"NEON_rev16", opRev<2>},
        {"NEON_rbit", opRbit},
        {"NEON_cls", opCls},
        {"NEON_ext", opExt},
        {"a64_TBL", opTbl},
        {"SIMD_PIECE", opSimdPiece},
        {"MP_INT_ABS", opMpAbs},
    };
    return table;
}

Binding bind_a64(Emu& emu) {
    static std::vector<std::unique_ptr<OpContext>> contexts;   // (one per operation name, never freed: the emulators of every thread point at them)
    static std::mutex lock;
    if (emu.hasReg("fpcr")) t_fpcr = emu.regRef("fpcr");
    Binding result;
    const Table& table = a64_table();
    const std::lock_guard guard(lock);
    for (std::size_t i = 0; i < emu.userOpCount(); ++i) {
        const std::string& name = emu.userOpName(i);
        if (const Handler* h = table.find(name)) {
            OpContext* ctx = nullptr;
            for (const auto& known : contexts)
                if (known->name == name) ctx = known.get();
            if (ctx == nullptr) {
                const Entry* entry = nullptr;
                for (const Entry& e : table.entries())
                    if (e.name == name) entry = &e;
                contexts.push_back(std::make_unique<OpContext>(OpContext{entry->name}));
                ctx = contexts.back().get();
            }
            emu.bindUserOp(i, *h, ctx);
            ++result.bound;
        } else {
            result.unbound.push_back(name);
            if (name.rfind("NEON_", 0) == 0 || name.rfind("SVE_", 0) == 0) {   // (the shape of every call it gets is recorded)
                OpContext* ctx = nullptr;
                for (const auto& known : contexts)
                    if (known->name == name) ctx = known.get();
                if (ctx == nullptr) {
                    static std::vector<std::unique_ptr<std::string>> names;
                    names.push_back(std::make_unique<std::string>(name));
                    contexts.push_back(std::make_unique<OpContext>(OpContext{*names.back()}));
                    ctx = contexts.back().get();
                }
                emu.bindUserOp(i, recordShape, ctx);
            }
        }
    }
    return result;
}

std::map<std::string, std::set<std::string>> a64_recorded_shapes() { return t_shapes; }

void trace_a64(std::vector<std::string_view>* into) { t_trace = into; }

void count_a64_operations(bool on) {
    if (on) {
        const std::lock_guard guard(g_count_lock);
        g_counts.clear();
    }
    g_counting = on;
}

std::map<std::string, std::uint64_t> a64_operation_counts() {
    const std::lock_guard guard(g_count_lock);
    return g_counts;
}

void set_a64_word(std::uint32_t word) { t_word = word; }

void set_a64_lane_compat(bool on) { g_laneCompat = on; }

std::vector<std::string> a64_unserved(Emu& emu) {
    std::vector<std::string> out;
    const Table& table = a64_table();
    for (std::size_t i = 0; i < emu.userOpCount(); ++i) {
        const std::string& name = emu.userOpName(i);
        if ((name.rfind("NEON_", 0) == 0 || name.rfind("SVE_", 0) == 0) && table.find(name) == nullptr) out.push_back(name);
    }
    return out;
}

}  // namespace cb::engine::pcode_ops
