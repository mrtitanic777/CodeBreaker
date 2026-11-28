// emulate/cpu/pcode_vector.hpp -- the lane operations of every vector instruction set, written once (SLEIGH names them and gives them no behavior: AArch64 NEON `NEON_sqadd`, x86
// `paddsb`, AltiVec `vectorAddSignedByteSaturate`, ARM `NEON_vqadd`, ...). The per-set tables (engine/pcode_ops_<set>.cpp) call these; none writes a saturating add of its own.
//
// A vector is `n` bytes (at most kMaxBytes), least significant byte first: the order the one emulator hands a wide value in, whatever the byte order of the machine's memory.
// Lane 0 is the lowest `ew` bytes; a lane is little endian. A big-endian machine's element numbering (AltiVec: element 0 is the most significant) is the reverse of lane
// order: its table reverses the lanes it passes to an operation that depends on order (zip, unzip, pack, extract, table lookup) with reverseLanes().
//
// Every function takes the width of a lane in bytes (`ew`: 1, 2, 4 or 8 for an integer, 2, 4 or 8 for a float) and the byte length `n` (a multiple of `ew`), and throws
// std::invalid_argument for a width or length it does not take. `out` may be the same array as an input (a lane is read before it is written; an operation that moves lanes
// across the vector copies what it needs first), but not partly overlap one.
//
// Meanings are the architecture manuals' (ARM A64 SIMD, Intel SSE/AVX, PowerPC AltiVec). Where the machines differ the function says which choices it takes as arguments (a
// float's NaN rule, its minimum's rule, the rounding mode, a shift count that reaches past the lane), and the test checks every function at every width it takes against a
// separate model: exact integers (a small big-integer type) for the integer functions, exactly rounded rationals for the float ones.
//
// An x86 instruction that works on each 128 bit lane of a 256 or 512 bit register apart (vpunpcklbw, vpshufb, vpalignr, vpslldq on ymm) is called once for each 16 bytes.
//
// Not here: the reciprocal and reciprocal square root estimates (frecpe, frsqrte, vrefp, vrsqrtefp, rcpps, rsqrtps, vexptefp, vlogefp: ARM defines its table exactly, x86 and
// AltiVec leave the low bits to the implementation, so only an instruction by instruction comparison with a real core can check them), and the AES, SHA, SM3 and SM4 rounds.
// FRECPS and FRSQRTS are fp3(NegMulAdd) with 2.0 and 3.0 (and a halving of the second).
#pragma once

#include <cstddef>
#include <cstdint>

namespace cb::emulate::vec {

using Byte = std::uint8_t;
constexpr std::size_t kMaxBytes = 256;   // the widest value the emulator takes

enum class Sign : std::uint8_t { Unsigned, Signed };
enum class Cmp : std::uint8_t { Eq, Ne, Lt, Le, Gt, Ge };

// ---- lanes as numbers
std::uint64_t load(const Byte* p, unsigned ew);                 // the lane at p (little endian), zero extended
void store(Byte* p, unsigned ew, std::uint64_t value);          // its low `ew` bytes
std::int64_t signExtend(std::uint64_t value, unsigned ew);
std::uint64_t laneMask(unsigned ew);                            // all ones in `ew` bytes
std::uint64_t getLane(const Byte* a, unsigned ew, std::size_t index);
void setLane(Byte* a, unsigned ew, std::size_t index, std::uint64_t value);

// ---- integer arithmetic, lane by lane. out[i] = a[i] op b[i].
void add(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew);            // wraps
void sub(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew);            // wraps
void mul(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew);            // the low half of the product
void mulHigh(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign);   // the high half (pmulhw, pmulhuw, vmulhuw ...)
void addSat(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign);    // saturates to the lane's range (paddsb, sqadd, uqadd, vaddubs)
void subSat(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign);
// USQADD / SUQADD: `acc` lanes of signedness `accSign` plus `x` lanes of the other signedness, saturated to acc's range.
void addSatMixed(Byte* out, const Byte* acc, const Byte* x, std::size_t n, unsigned ew, Sign accSign);
void neg(Byte* out, const Byte* a, std::size_t n, unsigned ew, bool saturate);            // saturate: the minimum negates to the maximum (sqneg)
void abs(Byte* out, const Byte* a, std::size_t n, unsigned ew, bool saturate);            // a signed lane's magnitude (pabsb; sqabs saturates the minimum)
void absDiff(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign);   // |a - b|, exact in the lane's width (sabd, uabd)
void min(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign);
void max(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign);
// (a + b [+ 1]) >> 1 on the exact sum: shadd/uhadd (round false), srhadd/urhadd, pavgb/pavgw and vavgub (round true, unsigned).
void halvingAdd(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign, bool round);
void halvingSub(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign);   // (a - b) >> 1 on the exact difference (shsub, uhsub)
// Signed multiply returning the high half doubled, saturating: SQDMULH (round false) and SQRDMULH (round true): (2 a b [+ 2^(w-1)]) >> w, w = 8 ew.
void mulDoublingHigh(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, bool round);
// SQRDMLAH (subtract false) and SQRDMLSH (true): signed saturate(((acc << w) +/- 2 a b + 2^(w-1)) >> w), w = 8 ew (A64 RDM; ARM VQRDMLAH).
void mulDoublingHighAcc(Byte* out, const Byte* acc, const Byte* a, const Byte* b, std::size_t n, unsigned ew, bool subtract);
// PMULHRSW: ((a b >> (w-2)) + 1) >> 1 on signed lanes, truncated to the lane (x86; 0x8000 * 0x8000 gives 0x8000).
void mulHighRoundHalfUp(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew);
// AltiVec VMHADDSHS / VMHRADDSHS: signed ((a b [+ 2^(w-2)]) >> (w-1)) + c, saturated.
void mulHighAddSat(Byte* out, const Byte* a, const Byte* b, const Byte* c, std::size_t n, unsigned ew, bool round);

// ---- compare, select and bit operations
void compare(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Cmp cmp, Sign sign);   // a lane of ones where it holds, else zero
void bitAnd(Byte* out, const Byte* a, const Byte* b, std::size_t n);
void bitOr(Byte* out, const Byte* a, const Byte* b, std::size_t n);
void bitXor(Byte* out, const Byte* a, const Byte* b, std::size_t n);
void bitAndNot(Byte* out, const Byte* a, const Byte* b, std::size_t n);    // a & ~b
void bitOrNot(Byte* out, const Byte* a, const Byte* b, std::size_t n);     // a | ~b
void bitNot(Byte* out, const Byte* a, std::size_t n);
void bitSelect(Byte* out, const Byte* mask, const Byte* a, const Byte* b, std::size_t n);   // (mask & a) | (~mask & b): bsl, vsel
void blend(Byte* out, const Byte* a, const Byte* b, const Byte* mask, std::size_t n, unsigned ew);   // a lane of b where the mask lane's top bit is set, else a (pblendvb)
enum class BitOp : std::uint8_t { Popcount, CountLeadingZeros, CountLeadingSigns, ReverseBits, ReverseBytes };
void bitOp(Byte* out, const Byte* a, std::size_t n, unsigned ew, BitOp op);   // per lane: cnt, clz, cls (the leading bits equal to the sign bit, not counting it), rbit, rev (bswap)

// ---- shifts. A count is a number; what it does past the lane is the Count rule: Saturate (x86, ARM immediates in range): a shift by w or more clears the lane (an arithmetic
// right shift fills it with the sign, a rotate is taken modulo w); Mask: the count is taken modulo w (AltiVec, a rotate everywhere).
enum class Shift : std::uint8_t { Left, RightLogical, RightArithmetic, RotateLeft };
enum class Count : std::uint8_t { Saturate, Mask };
void shiftImm(Byte* out, const Byte* a, std::size_t n, unsigned ew, Shift shift, std::uint64_t count, Count rule);
void shiftVar(Byte* out, const Byte* a, const Byte* counts, std::size_t n, unsigned ew, Shift shift, Count rule);   // each lane by the same lane of `counts`
// NEON SSHL, USHL, SRSHL, URSHL, SQSHL, UQSHL, SQRSHL, UQRSHL: each lane's shift count is the signed low byte of the same lane of `counts` (positive: left; negative: right,
// rounded when `round`); `saturate`: a left shift saturates to the lane's range (and a rounding right shift cannot overflow).
void shiftNeon(Byte* out, const Byte* a, const Byte* counts, std::size_t n, unsigned ew, Sign sign, bool round, bool saturate);
// SRSHR / URSHR: a right shift by count (1 to w) of the exact value plus 2^(count-1), and the same without the rounding (round false: SSHR, USHR).
void shiftRightImm(Byte* out, const Byte* a, std::size_t n, unsigned ew, Sign sign, unsigned count, bool round);
// SQSHL / UQSHL / SQSHLU by an immediate (0 to w-1): left shift saturating to the signed (Sign::Signed) or unsigned range; `toUnsigned` (SQSHLU): a signed lane to the unsigned range.
void shiftLeftSatImm(Byte* out, const Byte* a, std::size_t n, unsigned ew, Sign sign, unsigned count, bool toUnsigned);
// SLI / SRI: `dst` with the shifted `src` inserted (the bits the shift vacates keep dst's).
void shiftInsert(Byte* out, const Byte* dst, const Byte* src, std::size_t n, unsigned ew, bool left, unsigned count);

// ---- changing the width of lanes
enum class Narrow : std::uint8_t { Truncate, SignedToSigned, SignedToUnsigned, UnsignedToUnsigned };
// `nIn` bytes of `ewIn` lanes become nIn/2 bytes of lanes half as wide (xtn, sqxtn, sqxtun, uqxtn, packsswb, packuswb, vpkuhum, vpkshss ...).
void narrow(Byte* out, const Byte* a, std::size_t nIn, unsigned ewIn, Narrow mode);
// The same after a right shift of the exact value by `count` (rounded when `round`): shrn, rshrn, sqshrn, sqshrun, uqshrn, sqrshrn, sqrshrun, uqrshrn. The lane is signed
// for SignedToSigned and SignedToUnsigned, unsigned for UnsignedToUnsigned; Truncate takes the shifted value's low half (the lane's own signedness does not matter).
void narrowShiftRight(Byte* out, const Byte* a, std::size_t nIn, unsigned ewIn, unsigned count, bool round, Narrow mode);
void widen(Byte* out, const Byte* a, std::size_t nIn, unsigned ewIn, Sign sign);                    // nIn bytes become 2 nIn (sxtl, uxtl, vupkhsb, pmovsxbw, pmovzxbw)
void mulWiden(Byte* out, const Byte* a, const Byte* b, std::size_t nIn, unsigned ewIn, Sign signA, Sign signB);   // the full products of nIn bytes of lanes (smull, umull, pmuldq)
void mulEvenOdd(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Sign sign, bool odd);   // the full products of lanes 0, 2, 4.. (or 1, 3, 5..) (pmuludq, vmuleub, vmulosh)
// out[i] = acc[i] + the sum of a[k] b[k] over the `ewOut/ewIn` input lanes k that make output lane i (SDOT, UDOT, USDOT, SUDOT, PMADDWD, PMADDUBSW, VMSUMMBM, VMSUMUHM,
// VMSUMSHM, VMSUMSHS, VMSUMUHS). `acc` may be null (zero). The sum is exact and then wraps, or saturates to the output lane's range (signed if either side is signed).
void multiplyAdd(Byte* out, const Byte* acc, const Byte* a, const Byte* b, std::size_t n, unsigned ewIn, unsigned ewOut, Sign signA, Sign signB, bool saturate);
// PSADBW: for each group of 8 bytes the sum of |a - b| of unsigned bytes, as a 64-bit lane.
void sumAbsDiff(Byte* out, const Byte* a, const Byte* b, std::size_t n);

// ---- across lanes
enum class Pair : std::uint8_t { Add, MinSigned, MinUnsigned, MaxSigned, MaxUnsigned };
// The pairs of a, then the pairs of b: out = [a0 op a1, a2 op a3, ..., b0 op b1, ...] (addp, smaxp, uminp; phaddw's order within a 128 bit lane).
void pairwise(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Pair op);
// Lanes twice as wide: out[i] = acc[i] + a[2i] + a[2i+1] with each lane read as `sign` (saddlp, uaddlp, sadalp, uadalp). `acc` may be null.
void addLongPairwise(Byte* out, const Byte* acc, const Byte* a, std::size_t n, unsigned ewIn, Sign sign);
enum class Reduce : std::uint8_t { Add, Min, Max, AddLong };
// ADDV, SMAXV, UMINV, SADDLV ...: the lanes combined into one number (Add: wraps in the lane; AddLong: the exact sum, at most 64 bits for lanes of 4 bytes or less).
std::uint64_t reduce(const Byte* a, std::size_t n, unsigned ew, Reduce op, Sign sign);

// ---- moving lanes (lane numbering is value order: lane 0 is the least significant)
void splat(Byte* out, std::size_t n, unsigned ew, std::uint64_t value);
void reverseLanes(Byte* out, const Byte* a, std::size_t n, unsigned ew);                           // the lane order reversed (to and from a big-endian element numbering)
void reverseInGroups(Byte* out, const Byte* a, std::size_t n, unsigned ew, unsigned groupBytes);   // the lanes inside each group reversed (rev16, rev32, rev64; bswap: ew 1)
void zip(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, bool high);          // a0 b0 a1 b1 .. from the low (or high) halves (zip1/zip2, punpckl*, vmrgl*)
void unzip(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, bool odd);         // the even (or odd) lanes of a, then of b (uzp1/uzp2)
void transpose(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, bool odd);     // a0 b0 a2 b2 .. (or a1 b1 a3 b3 ..) (trn1/trn2)
void extract(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned byteIndex);          // n bytes of the 2n bytes (b above a) from byte `byteIndex` (ext, palignr, vsldoi)
// A shift of each group of `groupBytes` bytes (the whole vector: n) by `count` bytes toward the high (left) or low end, zero filled (pslldq, psrldq, vslo, vsro).
void shiftBytes(Byte* out, const Byte* a, std::size_t n, std::size_t groupBytes, unsigned count, bool left);
void permuteLanes(Byte* out, const Byte* a, std::size_t n, unsigned ew, const Byte* indices, std::size_t count);   // out[i] = a[indices[i]] (an index past the vector gives 0)
struct Lookup {
    unsigned indexMask = 0xff;       // the index byte is ANDed with this first (pshufb 0x0f, vperm 0x1f)
    bool bit7Zero = false;           // an index with its top bit set gives 0 (pshufb)
    bool keepOutOfRange = false;     // an index past the table leaves the output byte as it was (TBX); else it gives 0 (TBL)
    std::size_t groupBytes = 0;      // 0: the index picks from the whole table; else from the same-numbered group of the table (pshufb on 256 bits works per 128 bit lane)
};
void tableLookup(Byte* out, const Byte* table, std::size_t tableBytes, const Byte* indices, std::size_t n, const Lookup& mode);

// ---- bit fields and checks
std::uint64_t moveMask(const Byte* a, std::size_t n, unsigned ew);   // bit i is the top bit of lane i (pmovmskb, movmskps, movmskpd; at most 64 lanes)
// PMUL (8 bit lanes) and the low half of a carry-less (polynomial over GF(2)) product of each pair of lanes; PMULL and PCLMULQDQ: the whole 128 bit product of two 64 bit values.
void polyMul(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew);
void polyMul64(Byte* out16, std::uint64_t a, std::uint64_t b);                                  // the 128 bit product, least significant byte first
void polyMulWiden(Byte* out, const Byte* a, const Byte* b, std::size_t nIn, unsigned ewIn);     // PMULL: the full 2 ewIn byte products of nIn bytes of lanes (ewIn 1, 2 or 4)
// CRC32 and CRC32C instructions (A64 crc32b..crc32x, crc32cb..crc32cx; x86 crc32): the reflected CRC of the low `size` (1, 2, 4 or 8) bytes of `data` folded into `acc`, with
// no inversion before or after (the standard CRC-32 of a byte string is ~crc32Update(0xFFFFFFFF, ...), for "123456789" 0xCBF43926; the Castagnoli one 0xE3069283).
std::uint32_t crc32Update(std::uint32_t acc, std::uint64_t data, unsigned size, bool castagnoli);
// AltiVec VPKPX and VUPKHPX / VUPKLPX: four 8 bit channels (the high one one bit) packed to 1:5:5:5, and the 16 bit pixels widened (the one bit channel sign extended to 8 bits,
// the five bit ones zero extended). `lanes` words (VPKPX takes `a` then `b` in the order given: the first `lanes` words of a then of b).
void packPixel32(Byte* out, const Byte* a, const Byte* b, std::size_t n);                       // n bytes of words from each of a and b become n bytes of halfwords
void unpackPixel16(Byte* out, const Byte* a, std::size_t nIn);                                   // nIn bytes of halfwords become 2 nIn bytes of words

// ---- floating point lanes (ew 2: IEEE half, 4: single, 8: double; the arithmetic is exact rounding, done with integers: no host float, no host rounding mode)
enum class Round : std::uint8_t { NearestEven, TowardZero, TowardPositive, TowardNegative, NearestAway };
// What a machine does with a NaN: X86 (SSE/AVX): the first source operand that is a NaN is the result, quieted (an operation with no NaN input that is invalid gives the
// negative quiet NaN "indefinite", 0xFFC00000 for a single); Arm: a signaling NaN input wins over a quiet one, the first of each kind, quieted, and an invalid operation
// gives the positive default NaN (0x7FC00000), which `defaultNan` makes the result of every NaN input too (FPCR.DN); Ppc: the first NaN input, quieted, an invalid
// operation gives 0x7FC00000. A fused multiply-add looks at its operands in the order the machine does: Arm: the addend, then the two product operands (FPMulAdd); Ppc: the first product operand, the
// addend, the second product operand (FRA, FRB, FRC of fmadd; the manuals' order, not checked on hardware here); X86: as given to fp3 (a, b, c), so a table passes the operands of
// its 132, 213 and 231 forms in the order the manual looks for a NaN.
enum class NanRule : std::uint8_t { X86, Arm, Ppc };
// How a minimum or a maximum treats a NaN and signed zeros: X86 (minps, maxps: `a < b ? a : b`, so a NaN or two zeros give b); ArmPropagate (fmin, fmax: NaN rule of the
// machine, -0 < +0); ArmNumber (fminnm, fmaxnm: a quiet NaN against a number gives the number); Ppc (vminfp, vmaxfp: a NaN input is the result, -0 < +0).
enum class MinMax : std::uint8_t { X86, ArmPropagate, ArmNumber, Ppc };
struct FpEnv {
    Round round = Round::NearestEven;
    NanRule nan = NanRule::X86;
    MinMax minmax = MinMax::X86;
    bool flushInputs = false;       // a denormal input is read as a zero of its sign (DAZ, FPCR.FZ)
    bool flushResults = false;      // a result that is tiny (before rounding) and not zero is a zero of its sign (FTZ, FPCR.FZ)
    bool defaultNan = false;        // Arm: FPCR.DN
};
// The exceptions an operation raised, ORed into the caller's word. A denormal input is reported whether it was flushed or not; underflow is a tiny (before rounding)
// inexact result, or a flushed one.
constexpr unsigned kInvalid = 1, kDivideByZero = 2, kOverflow = 4, kUnderflow = 8, kInexact = 16, kInputDenormal = 32;
enum class Fp2 : std::uint8_t { Add, Sub, Mul, Div, Min, Max, AbsDiff, MulExtended };   // AbsDiff: |a - b| (fabd); MulExtended (fmulx): Mul, but 0 times infinity is +-2.0 and not invalid
enum class Fp1 : std::uint8_t { Sqrt, Abs, Neg, RoundToInt, RoundToIntExact };   // RoundToInt: to an integral value in env.round (frint*; Exact raises inexact too)
enum class Fp3 : std::uint8_t { MulAdd, MulSub, NegMulAdd, NegMulSub };   // a*b + c, a*b - c, -(a*b) + c, -(a*b) - c, with one rounding (fmla, fmls, vfmadd*, vfnmadd*, vmaddfp)
void fp2(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, Fp2 op, const FpEnv& env, unsigned& flags);
void fp1(Byte* out, const Byte* a, std::size_t n, unsigned ew, Fp1 op, const FpEnv& env, unsigned& flags);
void fp3(Byte* out, const Byte* a, const Byte* b, const Byte* c, std::size_t n, unsigned ew, Fp3 op, const FpEnv& env, unsigned& flags);
enum class FpCmp : std::uint8_t { Eq, Ne, Lt, Le, Gt, Ge, Unordered, Ordered };   // Ne is true for a NaN (x86 cmpneqps); Gt and Ge are Lt and Le with the operands exchanged
// A lane of ones where the comparison holds. `signaling`: any NaN raises invalid (cmpltps, fcmgt); else only a signaling NaN does (cmpeqps, fcmeq).
void fpCompare(Byte* out, const Byte* a, const Byte* b, std::size_t n, unsigned ew, FpCmp cmp, bool signaling, const FpEnv& env, unsigned& flags);
// Float lanes to integers of `iEw` bytes (the lane count is n / fEw): rounded in env.round, then `outOfRange` decides what a NaN or an out of range value gives: Saturate (A64
// fcvtz*, AltiVec vcts*): the nearest representable value, NaN 0; X86Indefinite (cvtps2dq, cvttps2dq): the lane's minimum signed value ("integer indefinite"). `fracBits`: the
// float is multiplied by 2^fracBits first (fcvtzs #fbits, vctsxs).
enum class IntOverflow : std::uint8_t { Saturate, X86Indefinite };
void floatToInt(Byte* out, const Byte* a, std::size_t n, unsigned fEw, unsigned iEw, Sign sign, unsigned fracBits, IntOverflow outOfRange, const FpEnv& env, unsigned& flags);
void intToFloat(Byte* out, const Byte* a, std::size_t n, unsigned iEw, unsigned fEw, Sign sign, unsigned fracBits, const FpEnv& env, unsigned& flags);   // divided by 2^fracBits (scvtf #fbits, vcfsx)
void floatConvert(Byte* out, const Byte* a, std::size_t n, unsigned fromEw, unsigned toEw, const FpEnv& env, unsigned& flags);   // half, single and double to each other (fcvt, fcvtl, fcvtn, cvtps2pd, cvtpd2ps)
void bfloat16ToSingle(Byte* out, const Byte* a, std::size_t n);                                                                   // 2 byte lanes to 4 byte lanes (exact: the high half of a single)
void singleToBfloat16(Byte* out, const Byte* a, std::size_t n, const FpEnv& env, unsigned& flags);                               // 4 byte lanes to 2 byte lanes, rounded (bfcvt)

}  // namespace cb::emulate::vec
