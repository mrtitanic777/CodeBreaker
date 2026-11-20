// build/x86asm/opflags.hpp -- the operand flag word: NASM's include/opflags.h, bit for bit.
//
// An operand (or an instruction template's operand slot) is described by one 64-bit word: its kind (register,
// immediate, memory), modifiers, register class, subclass, size and register-set count each own a field of it.
// The encoder's matching is "class & ~operand == 0", so every bit position here is load-bearing: they are NASM's.
// NASM spells them as macros; here they are constants in namespace `opf` (the names are NASM's; FAR, NEAR and ABS
// carry a trailing underscore because <windows.h> defines FAR and NEAR as macros).
#pragma once

#include <cstdint>

namespace cb::build::x86asm {

// The operand flag word, and the AVX-512 decorator flag word (nasm tables.h).
using OpFlags = std::uint64_t;
using DecoFlags = std::uint16_t;

// A field of `bits` bits at `shift`, and one bit of a field.
constexpr OpFlags op_genmask(unsigned bits, unsigned shift) { return ((OpFlags{1} << bits) - 1) << shift; }
constexpr OpFlags op_genbit(unsigned bit, unsigned shift) { return OpFlags{1} << (shift + bit); }
constexpr OpFlags op_genval(OpFlags val, unsigned bits, unsigned shift) { return (val & ((OpFlags{1} << bits) - 1)) << shift; }

// Type of operand: memory reference, register, etc. Bits 0-3.
inline constexpr unsigned OPTYPE_SHIFT = 0;
inline constexpr unsigned OPTYPE_BITS = 4;
inline constexpr OpFlags OPTYPE_MASK = op_genmask(OPTYPE_BITS, OPTYPE_SHIFT);
constexpr OpFlags gen_optype(unsigned bit) { return op_genbit(bit, OPTYPE_SHIFT); }

// Modifiers. Bits 4-6.
inline constexpr unsigned MODIFIER_SHIFT = OPTYPE_SHIFT + OPTYPE_BITS;
inline constexpr unsigned MODIFIER_BITS = 3;
inline constexpr OpFlags MODIFIER_MASK = op_genmask(MODIFIER_BITS, MODIFIER_SHIFT);
constexpr OpFlags gen_modifier(unsigned bit) { return op_genbit(bit, MODIFIER_SHIFT); }

// Register classes. Bits 7-17.
inline constexpr unsigned REG_CLASS_SHIFT = MODIFIER_SHIFT + MODIFIER_BITS;
inline constexpr unsigned REG_CLASS_BITS = 11;
inline constexpr OpFlags REG_CLASS_MASK = op_genmask(REG_CLASS_BITS, REG_CLASS_SHIFT);
constexpr OpFlags gen_reg_class(unsigned bit) { return op_genbit(bit, REG_CLASS_SHIFT); }

// Subclasses (their meaning depends on the type of operand). Bits 18-31.
inline constexpr unsigned SUBCLASS_SHIFT = REG_CLASS_SHIFT + REG_CLASS_BITS;
inline constexpr unsigned SUBCLASS_BITS = 14;
inline constexpr OpFlags SUBCLASS_MASK = op_genmask(SUBCLASS_BITS, SUBCLASS_SHIFT);
constexpr OpFlags gen_subclass(unsigned bit) { return op_genbit(bit, SUBCLASS_SHIFT); }

// Sizes of the operands and attributes. Bits 32-47.
inline constexpr unsigned SIZE_SHIFT = SUBCLASS_SHIFT + SUBCLASS_BITS;
inline constexpr unsigned SIZE_BITS = 16;
inline constexpr OpFlags SIZE_MASK = op_genmask(SIZE_BITS, SIZE_SHIFT);
constexpr OpFlags gen_size(unsigned bit) { return op_genbit(bit, SIZE_SHIFT); }

// Register set count. Bits 48-52.
inline constexpr unsigned REGSET_SHIFT = SIZE_SHIFT + SIZE_BITS;
inline constexpr unsigned REGSET_BITS = 5;
inline constexpr OpFlags REGSET_MASK = op_genmask(REGSET_BITS, REGSET_SHIFT);
constexpr OpFlags gen_regset(unsigned bit) { return op_genbit(bit, REGSET_SHIFT); }

// Remaining bits.
inline constexpr unsigned OPFLAGS_USED_BITS = REGSET_SHIFT + REGSET_BITS;
inline constexpr unsigned OPFLAGS_UNUSED_BITS = 64 - OPFLAGS_USED_BITS;

// The flag word of a register number (the x86 register number, 0-31): zero or non-zero, and below 16 or not.
// Appended to every register's word by regs.dat's generator (RN_FLAGS in opflags.h).
namespace opf {

inline constexpr OpFlags REGISTER = gen_optype(0);   // register number in 'basereg'
inline constexpr OpFlags IMMEDIATE = gen_optype(1);
inline constexpr OpFlags REGMEM = gen_optype(2);   // for r/m, ie EA, operands
inline constexpr OpFlags MEMORY = gen_optype(3) | REGMEM;

// First, the actual sizes.
inline constexpr OpFlags BITS8 = gen_size(0);     // 8 bits (BYTE)
inline constexpr OpFlags BITS16 = gen_size(1);    // 16 bits (WORD)
inline constexpr OpFlags BITS32 = gen_size(2);    // 32 bits (DWORD)
inline constexpr OpFlags BITS64 = gen_size(3);    // 64 bits (QWORD), x64 and FPU only
inline constexpr OpFlags BITS80 = gen_size(4);    // 80 bits (TWORD), FPU only
inline constexpr OpFlags BITS128 = gen_size(5);   // 128 bits (OWORD)
inline constexpr OpFlags BITS256 = gen_size(6);   // 256 bits (YWORD)
inline constexpr OpFlags BITS512 = gen_size(7);   // 512 bits (ZWORD)

// Then, size modifiers.
inline constexpr OpFlags FAR_ = gen_size(8);     // this means 16:16 or 16:32, like in CALL/JMP
inline constexpr OpFlags NEAR_ = gen_size(9);
inline constexpr OpFlags SHORT = gen_size(10);   // and this means what it says
inline constexpr OpFlags ABS_ = gen_size(11);    // JMP ABS, MOV ABS

// Special modifiers. (STRICT carries a trailing underscore too: <windows.h> defines it as a macro.)
inline constexpr OpFlags TO = gen_modifier(0);       // reverse effect in FADD, FSUB &c
inline constexpr OpFlags COLON = gen_modifier(1);    // operand is followed by a colon
inline constexpr OpFlags STRICT_ = gen_modifier(2);   // do not optimize this operand

inline constexpr OpFlags REG_CLASS_CDT = gen_reg_class(0);
inline constexpr OpFlags REG_CLASS_GPR = gen_reg_class(1);
inline constexpr OpFlags REG_CLASS_SREG = gen_reg_class(2);
inline constexpr OpFlags REG_CLASS_FPUREG = gen_reg_class(3);
inline constexpr OpFlags REG_CLASS_RM_MMX = gen_reg_class(4);
inline constexpr OpFlags REG_CLASS_RM_XMM = gen_reg_class(5);
inline constexpr OpFlags REG_CLASS_RM_YMM = gen_reg_class(6);
inline constexpr OpFlags REG_CLASS_RM_ZMM = gen_reg_class(7);
inline constexpr OpFlags REG_CLASS_OPMASK = gen_reg_class(8);
inline constexpr OpFlags REG_CLASS_BND = gen_reg_class(9);
inline constexpr OpFlags REG_CLASS_RM_TMM = gen_reg_class(10);

// Register classes treated as vectors for EVEX/REX2 encoding purposes.
inline constexpr OpFlags REG_CLASS_VECTOR = REG_CLASS_RM_XMM | REG_CLASS_RM_YMM | REG_CLASS_RM_ZMM | REG_CLASS_RM_TMM;

// These are used across the RM classes so cannot conflict with neither EA nor REG subtypes.
inline constexpr OpFlags RN_L16 = gen_subclass(10);             // Register number 0-15
inline constexpr OpFlags RN_ZERO = gen_subclass(11) | RN_L16;   // Register number 0
inline constexpr OpFlags RN_NZERO = gen_subclass(12);           // Register number 1+
inline constexpr OpFlags RN_1_15 = RN_NZERO | RN_L16;           // Register number 1-15

inline constexpr OpFlags RM_L16 = RN_L16 | REGMEM;
inline constexpr OpFlags RM_ZERO = RN_ZERO | REGMEM;
inline constexpr OpFlags RM_NZERO = RN_NZERO | REGMEM;
inline constexpr OpFlags RM_1_15 = RN_1_15 | REGMEM;   // (NASM's own definition of this macro refers to itself, so it is unusable there)
inline constexpr OpFlags RM_SEL = gen_subclass(13) | REGMEM;   // valid segment selector

// Register classes.
inline constexpr OpFlags REG_L16 = RN_L16 | REGISTER;
inline constexpr OpFlags REG_ZERO = RN_ZERO | REGISTER;
inline constexpr OpFlags REG_NZERO = RN_NZERO | REGISTER;
inline constexpr OpFlags REG_1_15 = RN_1_15 | REGISTER;

inline constexpr OpFlags REG_SMASK = SUBCLASS_MASK;

inline constexpr OpFlags REG_EA = REGMEM | REGISTER;                    // 'normal' reg, qualifies as EA
inline constexpr OpFlags RM_GPR = REG_CLASS_GPR | REGMEM;               // integer operand
inline constexpr OpFlags REG_GPR = REG_CLASS_GPR | REG_EA;              // integer register
inline constexpr OpFlags FPUREG = REG_CLASS_FPUREG | REGISTER;          // fp stack registers
inline constexpr OpFlags FPU0 = REG_CLASS_FPUREG | REG_ZERO;            // fp stack register zero
inline constexpr OpFlags RM_MMX = REG_CLASS_RM_MMX | REGMEM;            // MMX operand
inline constexpr OpFlags MMXREG = REG_CLASS_RM_MMX | REG_EA;            // MMX register
inline constexpr OpFlags RM_XMM = REG_CLASS_RM_XMM | REGMEM;            // XMM operand
inline constexpr OpFlags XMMREG = REG_CLASS_RM_XMM | REG_EA;            // XMM register
inline constexpr OpFlags RM_XMM_L16 = REG_CLASS_RM_XMM | REGMEM | RN_L16;   // XMM operand reg 0-15
inline constexpr OpFlags XMM_L16 = REG_CLASS_RM_XMM | REG_EA | RN_L16;      // XMM register 0-15
inline constexpr OpFlags XMM0 = REG_CLASS_RM_XMM | REGMEM | REG_ZERO;       // XMM register 0
inline constexpr OpFlags RM_YMM = REG_CLASS_RM_YMM | REGMEM;            // YMM operand
inline constexpr OpFlags YMMREG = REG_CLASS_RM_YMM | REG_EA;            // YMM register
inline constexpr OpFlags RM_YMM_L16 = REG_CLASS_RM_YMM | REGMEM | RN_L16;   // YMM operand reg 0-15
inline constexpr OpFlags YMM_L16 = REG_CLASS_RM_YMM | REG_EA | RN_L16;      // YMM register 0-15
inline constexpr OpFlags YMM0 = REG_CLASS_RM_YMM | REGMEM | REG_ZERO;       // YMM register 0
inline constexpr OpFlags RM_ZMM = REG_CLASS_RM_ZMM | REGMEM;            // ZMM operand
inline constexpr OpFlags ZMMREG = REG_CLASS_RM_ZMM | REG_EA;            // ZMM register
inline constexpr OpFlags RM_ZMM_L16 = REG_CLASS_RM_ZMM | REGMEM | RN_L16;   // ZMM operand reg 0-15
inline constexpr OpFlags ZMM_L16 = REG_CLASS_RM_ZMM | REG_EA | RN_L16;      // ZMM register 0-15
inline constexpr OpFlags ZMM0 = REG_CLASS_RM_ZMM | REGMEM | REG_ZERO;       // ZMM register 0
inline constexpr OpFlags RM_OPMASK = REG_CLASS_OPMASK | REGMEM;         // Opmask operand
inline constexpr OpFlags OPMASKREG = REG_CLASS_OPMASK | REG_EA;         // Opmask register
inline constexpr OpFlags OPMASK0 = REG_CLASS_OPMASK | REG_EA;           // Opmask register zero (k0)
inline constexpr OpFlags RM_K = RM_OPMASK;
inline constexpr OpFlags KREG = OPMASKREG;
inline constexpr OpFlags RM_BND = REG_CLASS_BND | REGMEM;               // Bounds operand
inline constexpr OpFlags BNDREG = REG_CLASS_BND | REG_EA;               // Bounds register
inline constexpr OpFlags TMMREG = REG_CLASS_RM_TMM | REG_EA;            // TMM (AMX) register

inline constexpr OpFlags REG_CDT = REG_CLASS_CDT | REGISTER;   // CRn, DRn and TRn
inline constexpr OpFlags REG_CREG = gen_subclass(0) | REG_CDT;   // CRn
inline constexpr OpFlags REG_DREG = gen_subclass(1) | REG_CDT;   // DRn
inline constexpr OpFlags REG_TREG = gen_subclass(2) | REG_CDT;   // TRn

inline constexpr OpFlags REG_SREG = REG_CLASS_SREG | REG_L16 | BITS16;   // segment register

// Segment registers.
inline constexpr OpFlags REG_ES = gen_subclass(0) | gen_subclass(2) | REG_SREG;
inline constexpr OpFlags REG_CS = gen_subclass(1) | gen_subclass(2) | REG_SREG;
inline constexpr OpFlags REG_SS = gen_subclass(0) | gen_subclass(3) | REG_SREG;
inline constexpr OpFlags REG_DS = gen_subclass(1) | gen_subclass(3) | REG_SREG;
inline constexpr OpFlags REG_FS = gen_subclass(0) | gen_subclass(4) | REG_SREG;
inline constexpr OpFlags REG_GS = gen_subclass(1) | gen_subclass(4) | REG_SREG;
inline constexpr OpFlags REG_FSGS = gen_subclass(4) | REG_SREG;   // FS or GS
inline constexpr OpFlags REG_SEG6 = gen_subclass(0) | gen_subclass(5) | REG_SREG;
inline constexpr OpFlags REG_SEG7 = gen_subclass(1) | gen_subclass(5) | REG_SREG;
inline constexpr OpFlags REG_SEG67 = gen_subclass(5) | REG_SREG;

// GPRs.
inline constexpr OpFlags REG8 = REG_GPR | BITS8;
inline constexpr OpFlags REG16 = REG_GPR | BITS16 | RM_SEL;
inline constexpr OpFlags REG32 = REG_GPR | BITS32 | RM_SEL;
inline constexpr OpFlags REG64 = REG_GPR | BITS64 | RM_SEL;

// Accumulator: AL, AX, EAX, RAX.
inline constexpr OpFlags REG_ACCUM = REG_GPR | REG_ZERO;
inline constexpr OpFlags REG_AL = REG_ACCUM | REG8;
inline constexpr OpFlags REG_AX = REG_ACCUM | REG16;
inline constexpr OpFlags REG_EAX = REG_ACCUM | REG32;
inline constexpr OpFlags REG_RAX = REG_ACCUM | REG64;

// Counter: CL, CX, ECX, RCX.
inline constexpr OpFlags REG_COUNT = REG_GPR | REG_1_15 | gen_subclass(0);
inline constexpr OpFlags REG_CL = REG_COUNT | REG8;
inline constexpr OpFlags REG_CX = REG_COUNT | REG16;
inline constexpr OpFlags REG_ECX = REG_COUNT | REG32;
inline constexpr OpFlags REG_RCX = REG_COUNT | REG64;

// Data: DL, DX, EDX, RDX.
inline constexpr OpFlags REG_DATA = REG_GPR | REG_1_15 | gen_subclass(1);
inline constexpr OpFlags REG_DL = REG_DATA | REG8;
inline constexpr OpFlags REG_DX = REG_DATA | REG16;
inline constexpr OpFlags REG_EDX = REG_DATA | REG32;
inline constexpr OpFlags REG_RDX = REG_DATA | REG64;

// Base: BL, BX, EBX, RBX.
inline constexpr OpFlags REG_BASE = REG_GPR | REG_1_15 | gen_subclass(2);
inline constexpr OpFlags REG_BL = REG_BASE | REG8;
inline constexpr OpFlags REG_BX = REG_BASE | REG16;
inline constexpr OpFlags REG_EBX = REG_BASE | REG32;
inline constexpr OpFlags REG_RBX = REG_BASE | REG64;

// High 8-bit regs: AH, CH, DH, BH.
inline constexpr OpFlags REG_HIGH = REG8 | REG_1_15 | gen_subclass(3);

// Non-accumulator registers.
inline constexpr OpFlags REG_NA = REG_GPR | REG_NZERO;
inline constexpr OpFlags REG8NA = REG_NA | REG8;
inline constexpr OpFlags REG16NA = REG_NA | REG16;
inline constexpr OpFlags REG32NA = REG_NA | REG32;
inline constexpr OpFlags REG64NA = REG_NA | REG64;

// Special types of EAs.
inline constexpr OpFlags MEM_OFFS = gen_subclass(0) | MEMORY;   // simple [address] offset - absolute!
inline constexpr OpFlags IP_REL = gen_subclass(1) | MEMORY;     // IP-relative offset
inline constexpr OpFlags XMEM = gen_subclass(2) | MEMORY;       // 128-bit vector SIB
inline constexpr OpFlags YMEM = gen_subclass(3) | MEMORY;       // 256-bit vector SIB
inline constexpr OpFlags ZMEM = gen_subclass(4) | MEMORY;       // 512-bit vector SIB

// Operand bits to match an instruction carrying any type of memory r/m operand.
inline constexpr OpFlags MEMORY_ANY = MEMORY | RM_GPR | RM_MMX | RM_XMM | RM_YMM | RM_ZMM | RM_L16 | RM_ZERO | RM_NZERO | RM_OPMASK | RM_BND;

// Special immediate values.
inline constexpr OpFlags IMM_NORMAL = gen_subclass(0) | IMMEDIATE;   // operand is NOT a brcconst
inline constexpr OpFlags UNITY = gen_subclass(1) | IMMEDIATE;        // operand equals 1
inline constexpr OpFlags SBYTEWORD = gen_subclass(2) | IMMEDIATE;    // operand is in the range -128..127 mod 2^16
inline constexpr OpFlags SBYTEDWORD = gen_subclass(3) | IMMEDIATE;   // operand is in the range -128..127 mod 2^32
inline constexpr OpFlags SDWORD = gen_subclass(4) | IMMEDIATE;       // operand is in the range -0x80000000..0x7FFFFFFF
inline constexpr OpFlags UDWORD = gen_subclass(5) | IMMEDIATE;       // operand is in the range 0..0xFFFFFFFF
inline constexpr OpFlags FOURBITS = gen_subclass(6) | IMMEDIATE;     // operand is in the range 0-15
inline constexpr OpFlags IMM_KNOWN = gen_subclass(7) | IMMEDIATE;    // operand value is known at compile time

inline constexpr OpFlags BYTEEXTMASK = gen_subclass(2) | gen_subclass(3);
inline constexpr OpFlags DWORDEXTMASK = gen_subclass(4) | gen_subclass(5);

// Register set sizes.
inline constexpr OpFlags RS2 = gen_regset(0);
inline constexpr OpFlags RS4 = gen_regset(1);
inline constexpr OpFlags RS8 = gen_regset(2);
inline constexpr OpFlags RS16 = gen_regset(3);
inline constexpr OpFlags RS32 = gen_regset(4);

}  // namespace opf

// A register's flag word carries its number: zero or non-zero, below 16 or not (RN_FLAGS).
constexpr OpFlags rn_flags(unsigned n) { return (n == 0 ? opf::RN_ZERO : opf::RN_NZERO) | (n < 16 ? opf::RN_L16 : OpFlags{0}); }

// True when `op` belongs to class `cls`: (class & ~operand) == 0.
constexpr bool is_class(OpFlags cls, OpFlags op) { return (cls & ~op) == 0; }

// ---- AVX-512 decorator flags (nasm.h: decoflags_t) ----
// Bits: 0-3 opmask register number (identical to EVEX.aaa), 4 zeroing/merging (EVEX.z), 5 broadcast, 6 static rounding
// (it goes with the last simd operand), 7 SAE, 8-11 broadcast element size, 12-14 number of broadcast elements.
namespace deco {

inline constexpr unsigned OPMASK_SHIFT = 0;
inline constexpr unsigned OPMASK_BITS = 4;
inline constexpr OpFlags OPMASK_MASK = op_genmask(OPMASK_BITS, OPMASK_SHIFT);
constexpr OpFlags gen_opmask(unsigned bit) { return op_genbit(bit, OPMASK_SHIFT); }
constexpr OpFlags val_opmask(OpFlags val) { return op_genval(val, OPMASK_BITS, OPMASK_SHIFT); }

inline constexpr unsigned Z_SHIFT = 4;
inline constexpr unsigned Z_BITS = 1;
inline constexpr OpFlags Z_MASK = op_genmask(Z_BITS, Z_SHIFT);
constexpr OpFlags gen_z(unsigned bit) { return op_genbit(bit, Z_SHIFT); }

inline constexpr unsigned BRDCAST_SHIFT = 5;
inline constexpr unsigned BRDCAST_BITS = 1;
inline constexpr OpFlags BRDCAST_MASK = op_genmask(BRDCAST_BITS, BRDCAST_SHIFT);
constexpr OpFlags gen_brdcast(unsigned bit) { return op_genbit(bit, BRDCAST_SHIFT); }

inline constexpr unsigned STATICRND_SHIFT = 6;
inline constexpr unsigned STATICRND_BITS = 1;
inline constexpr OpFlags STATICRND_MASK = op_genmask(STATICRND_BITS, STATICRND_SHIFT);
constexpr OpFlags gen_staticrnd(unsigned bit) { return op_genbit(bit, STATICRND_SHIFT); }

inline constexpr unsigned SAE_SHIFT = 7;
inline constexpr unsigned SAE_BITS = 1;
inline constexpr OpFlags SAE_MASK = op_genmask(SAE_BITS, SAE_SHIFT);
constexpr OpFlags gen_sae(unsigned bit) { return op_genbit(bit, SAE_SHIFT); }

inline constexpr unsigned BRSIZE_SHIFT = 8;
inline constexpr unsigned BRSIZE_BITS = 4;
inline constexpr OpFlags BRSIZE_MASK = op_genmask(BRSIZE_BITS, BRSIZE_SHIFT);
constexpr OpFlags gen_brsize(unsigned bit) { return op_genbit(bit, BRSIZE_SHIFT); }

inline constexpr OpFlags BR_BITS8 = gen_brsize(0);   // for potential future use
inline constexpr OpFlags BR_BITS16 = gen_brsize(1);
inline constexpr OpFlags BR_BITS32 = gen_brsize(2);
inline constexpr OpFlags BR_BITS64 = gen_brsize(3);

inline constexpr unsigned BRNUM_SHIFT = 12;
inline constexpr unsigned BRNUM_BITS = 3;
inline constexpr OpFlags BRNUM_MASK = op_genmask(BRNUM_BITS, BRNUM_SHIFT);
constexpr OpFlags val_brnum(OpFlags val) { return op_genval(val, BRNUM_BITS, BRNUM_SHIFT); }

inline constexpr OpFlags BR_1TO2 = val_brnum(0);
inline constexpr OpFlags BR_1TO4 = val_brnum(1);
inline constexpr OpFlags BR_1TO8 = val_brnum(2);
inline constexpr OpFlags BR_1TO16 = val_brnum(3);
inline constexpr OpFlags BR_1TO32 = val_brnum(4);
inline constexpr OpFlags BR_1TO64 = val_brnum(5);   // for potential future use

inline constexpr OpFlags MASK = OPMASK_MASK;                   // Opmask (k1 ~ 7) can be used
inline constexpr OpFlags Z = Z_MASK;
inline constexpr OpFlags B16 = BRDCAST_MASK | BR_BITS16;       // {1to32}: broadcast 16b * 32 to zmm (512b)
inline constexpr OpFlags B32 = BRDCAST_MASK | BR_BITS32;       // {1to16}: broadcast 32b * 16 to zmm (512b)
inline constexpr OpFlags B64 = BRDCAST_MASK | BR_BITS64;       // {1to8}: broadcast 64b * 8 to zmm (512b)
inline constexpr OpFlags ER = STATICRND_MASK;                  // embedded rounding == static rounding mode
inline constexpr OpFlags SAE = SAE_MASK;                       // suppress all exceptions

}  // namespace deco

// Broadcast flags (BR_BITS*) to sizes (BITS*).
constexpr OpFlags brsize_to_size(OpFlags brbits) { return (brbits & deco::BRSIZE_MASK) << (SIZE_SHIFT - deco::BRSIZE_SHIFT); }

}  // namespace cb::build::x86asm
