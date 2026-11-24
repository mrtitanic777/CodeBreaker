// decompile/arm32_neon_dp.hpp -- the Advanced SIMD data-processing instructions and the transfers between the core registers and the NEON scalars, read from the ARM ARM (A7.4
// the encoding tables, A8.8 each instruction) into the fields an executor needs. One decoder is read by the own core (emulate/arm32_neon_dp.cpp, per-lane scalar code) and by the
// one emulator's adapter (engine/pcode_neon_arm32.cpp, over the lane library); neither shares a line of arithmetic with the other, and Unicorn's cortex-a15 checks both.
//
// Input is the ARM word: a Thumb instruction has its first halfword's 111U 1111 turned to ARM's 1111 001U (t2::neon_arm_word). A word that is no instruction of these classes, or
// one the manual leaves UNDEFINED (an odd register number of a quadword form, a reserved size), decodes to nothing.
#pragma once

#include <cstdint>
#include <optional>

namespace cb::decompile::arm::t2 {

enum class NeonOp : std::uint8_t {
    // three registers of the same length (integer)
    Hadd, Rhadd, Qadd, Hsub, Qsub, Cgt, Cge, ShiftReg, Max, Min, Abd, Aba, Add, Sub, Tst, Ceq, Mla, Mls, Mul, MulP, Pmax, Pmin, Qdmulh, Padd, And, Bic, Orr, Orn, Eor, Bsl, Bit, Bif,
    // three registers of the same length (single precision float)
    Fma, Fms, FAdd, FSub, FPadd, FAbd, FMla, FMls, FMul, FCeq, FCge, FCgt, FAcge, FAcgt, FMax, FMin, FPmax, FPmin, Recps, Rsqrts,
    // three registers of different lengths
    Addl, Addw, Subl, Subw, AddHn, SubHn, Abal, Abdl, Mlal, Mlsl, Mull, MullP, Qdmlal, Qdmlsl, Qdmull,
    // two registers and a shift amount
    Shr, Sri, ShlI, Sli, Qshlu, QshlI, ShrN, Qshrn, Qshrun, Shll, CvtFixed,
    // two registers, miscellaneous
    Rev, Paddl, Padal, Cls, Clz, Cnt, Mvn, Qabs, Qneg, CmpZero, Abs, Neg, Swp, Trn, Uzp, Zip, Movn, Qmovn, Qmovun, Cvt16, Recpe, Rsqrte, CvtIntFloat,
    // one register and a modified immediate is the own core's (arm32_neon.cpp); these are the rest of the family
    Ext, Tbl, DupScalar,
    // the core registers and the scalars
    MovToScalar, MovFromScalar, DupCore,
    // the VFP of VFPv3 and VFPv4 that the table of arm_ext does not have: fused multiply-add, the conversions with fraction bits, half precision, two core registers and two singles
    VFma, VCvtFx, VCvtHalf, VMovPair,
};

struct NeonDp {
    NeonOp op = NeonOp::Add;
    bool q = false;          // the operands are quadword registers (the destination of a long operation and the source of a narrowing one are quadwords whatever this says)
    bool u = false;          // unsigned (the U bit); for the misc group the bit that picks unsigned
    bool f = false;          // a floating point form
    bool round = false;      // rounding (a rounding shift, a rounding narrowing add, the rounding doubling multiply)
    bool sat = false;        // saturating
    bool acc = false;        // accumulating (VSRA, VRSRA); CvtFixed: float to fixed point; Cvt16: half to single
    bool scalar = false;     // the second operand is a scalar (`idx` of register m)
    unsigned d = 0, n = 0, m = 0;   // D register numbers (the first of the pair for a quadword)
    unsigned size = 0;       // the element of the operation as log2 bytes (the elements of the source for a long or narrowing form)
    unsigned imm = 0;        // the shift count, the fraction bits, the byte index of VEXT, the length of VTBL, the group of VREV in bytes, the form of a comparison with zero
    unsigned idx = 0;        // a scalar's or a core transfer's element number
    unsigned rt = 0;         // the core register of a transfer
    bool tbx = false;        // VTBX (VTBL keeps zero)
    unsigned cond = 14;      // a core transfer's condition (the data-processing forms are unconditional)
    // The VFP forms: `size` is 2 for a single (d, n, m are single register numbers 0-31) and 3 for a double (D register numbers); VFma: imm 0 VFMA, 1 VFMS, 2 VFNMA, 3 VFNMS; VCvtFx:
    // acc to fixed point, u unsigned, imm the fraction bits, idx the width of the fixed point number (16 or 32); VCvtHalf: acc single to half, imm 1 for the top half (VCVTT);
    // VMovPair: u to the core registers (rt, rt2 in `rt` and `idx`), m the first single.
};

// The instruction an ARM word is, or nullopt. `thumb`: the word is a Thumb instruction (a transfer between core registers is then conditional on the IT block, not on bits 31:28).
std::optional<NeonDp> neon_dp(std::uint32_t arm_word, bool thumb);

}  // namespace cb::decompile::arm::t2
