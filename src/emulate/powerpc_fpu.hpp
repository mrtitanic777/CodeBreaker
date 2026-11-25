// emulate/powerpc_fpu.hpp -- the PowerPC floating-point unit: the arithmetic, the conversions and the status and control register (FPSCR), as the architecture manual gives them.
//
// One implementation, used by the interpreter (powerpc.cpp) and by the adapter that runs the same programs on the one p-code emulator (engine/pcode_cpu_ppc.cpp, pcode_ops_ppc.cpp),
// so a program sees the same floating point on both. Every operation is computed exactly (a significand of 128 bits and a sticky bit) and rounded once, to double or to single
// precision, under the rounding mode FPSCR[RN] says; no host floating point decides a result or a flag.
//
//   fpu::Result r = fpu::execute(fpscr, fpu::Op::Madd, single, a, b, c);   // raw bits in, raw bits out; fpscr updated
//   if (r.written) f[rt] = r.bits;                                           // not written: an enabled exception (VE, ZE) stops the result reaching the register
//
// Modelled: the rounding modes (RN: nearest even, toward zero, +infinity, -infinity), FR and FI, FPRF, the sticky exception bits (FX, OX, UX, ZX, XX and the invalid-operation bits
// VXSNAN, VXISI, VXIDI, VXZDZ, VXIMZ, VXVC, VXSOFT, VXSQRT, VXCVI) with their summaries VX and FEX, the enables (VE, OE, UE, ZE, XE: with OE or UE set an overflow or underflow leaves the
// result with the exponent wrapped by 192 for single, 1536 for double precision, as the manual says; with VE or ZE set an invalid operation or a division by zero leaves the target
// alone), tininess detected before rounding, and the record form's CR1.
// Not modelled, and said so: FPSCR[NI] (non-IEEE mode) is kept and does nothing, and a trap is never taken (FEX only says the program could have been told).
// The estimates fres and frsqrte are the correctly rounded reciprocal and reciprocal square root (the hardware's tables are not available), with FR and FI left clear.
#pragma once

#include <cstdint>

namespace cb::emulate::powerpc::fpu {

// FPSCR bits as masks (the manual numbers bit 0 as the most significant).
inline constexpr std::uint32_t kFX = 1u << 31, kFEX = 1u << 30, kVX = 1u << 29, kOX = 1u << 28, kUX = 1u << 27, kZX = 1u << 26, kXX = 1u << 25;
inline constexpr std::uint32_t kVXSNAN = 1u << 24, kVXISI = 1u << 23, kVXIDI = 1u << 22, kVXZDZ = 1u << 21, kVXIMZ = 1u << 20, kVXVC = 1u << 19;
inline constexpr std::uint32_t kFR = 1u << 18, kFI = 1u << 17;
inline constexpr std::uint32_t kFPRF = 0x1Fu << 12, kFPCC = 0xFu << 12;
inline constexpr std::uint32_t kVXSOFT = 1u << 10, kVXSQRT = 1u << 9, kVXCVI = 1u << 8;
inline constexpr std::uint32_t kVE = 1u << 7, kOE = 1u << 6, kUE = 1u << 5, kZE = 1u << 4, kXE = 1u << 3, kNI = 1u << 2, kRN = 3;
inline constexpr std::uint32_t kInvalidBits = kVXSNAN | kVXISI | kVXIDI | kVXZDZ | kVXIMZ | kVXVC | kVXSOFT | kVXSQRT | kVXCVI;   // what VX summarizes
inline constexpr std::uint32_t kSticky = kOX | kUX | kZX | kXX | kInvalidBits;                                                    // the bits whose rise sets FX
inline constexpr std::uint32_t kWritable = ~(1u << 11);                                                                           // (bit 20 is reserved and reads zero)

enum class Op : std::uint8_t {
    Add, Sub, Mul, Div,                // a+b, a-b, a*c, a/b
    Madd, Msub, Nmadd, Nmsub,          // a*c+b, a*c-b, -(a*c+b), -(a*c-b): fused, one rounding (the negation after it)
    Sqrt, Res, Rsqrte, Frsp,           // of b
    Fctiw, Fctiwz, Fctid, Fctidz,      // b to a 32 or 64-bit integer (Fctiw, Fctid by RN; the z forms toward zero)
    Fcfid,                             // the 64-bit integer in b to double
};

struct Result {
    std::uint64_t bits = 0;
    bool written = true;   // false: the target register keeps its value
};

// Run one operation: the operands are raw 64-bit register contents (a, b, c are FRA, FRB, FRC). `single`: the single-precision form (opcode 59, the paired singles): the result is rounded
// to single precision (and held as a double). Updates `fpscr`.
Result execute(std::uint32_t& fpscr, Op op, bool single, std::uint64_t a, std::uint64_t b, std::uint64_t c);

// A paired-single operation: the same operation on both halves. FPRF, FR and FI say what the first half did; the exception bits are the two halves' together.
struct Pair {
    Result first, second;
};
Pair execute_paired(std::uint32_t& fpscr, Op op, std::uint64_t a0, std::uint64_t b0, std::uint64_t c0, std::uint64_t a1, std::uint64_t b1, std::uint64_t c1);

// fcmpu, fcmpo (ordered: a NaN raises VXVC as well). Sets FPSCR[FPCC] and returns the condition field (LT 8, GT 4, EQ 2, UN 1).
unsigned compare(std::uint32_t& fpscr, std::uint64_t a, std::uint64_t b, bool ordered);

// FPSCR moves.
std::uint64_t move_from(std::uint32_t fpscr);                                         // mffs: the register's value, as 64 bits
void move_to_fields(std::uint32_t& fpscr, unsigned field_mask, std::uint64_t frb);    // mtfsf: FM (bit 0x80 is field 0, FPSCR bits 0-3), the low word of FRB
void move_to_field_immediate(std::uint32_t& fpscr, unsigned field, unsigned value);   // mtfsfi: field 0..7, a 4-bit value
void set_bit(std::uint32_t& fpscr, unsigned bit, bool value);                         // mtfsb1, mtfsb0 (bit 0 is the most significant)
unsigned move_to_cr_from_field(std::uint32_t& fpscr, unsigned field);                 // mcrfs: the field, then its exception bits cleared
inline unsigned cr1_of(std::uint32_t fpscr) { return fpscr >> 28; }                   // a record form's CR1: FX, FEX, VX, OX
void refresh(std::uint32_t& fpscr);                                                   // VX and FEX from the bits and the enables

}  // namespace cb::emulate::powerpc::fpu
