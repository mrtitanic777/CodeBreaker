// Generated from native/nasm/x86/regs.dat (regs.h, regs.c, regflags.c, regvals.c): NASM's register numbers, names,
// operand-flag words and x86 encodings, exactly. Do not edit by hand; regenerate from regs.dat.

#pragma once

#include <array>

#include "cb/build/x86asm/opflags.hpp"

namespace cb::build::x86asm {

// The first register number.
inline constexpr int EXPR_REG_START = 1;

// The register numbers. A register's token value is its number here; R_zero and R_none are not registers.
enum Reg : int {
    R_zero = 0,
    R_none = -1,
    R_AH = EXPR_REG_START,
    R_AL,
    R_AX,
    R_BH,
    R_BL,
    R_BND0,
    R_BND1,
    R_BND2,
    R_BND3,
    R_BP,
    R_BPL,
    R_BX,
    R_CH,
    R_CL,
    R_CR0,
    R_CR1,
    R_CR10,
    R_CR11,
    R_CR12,
    R_CR13,
    R_CR14,
    R_CR15,
    R_CR16,
    R_CR17,
    R_CR18,
    R_CR19,
    R_CR2,
    R_CR20,
    R_CR21,
    R_CR22,
    R_CR23,
    R_CR24,
    R_CR25,
    R_CR26,
    R_CR27,
    R_CR28,
    R_CR29,
    R_CR3,
    R_CR30,
    R_CR31,
    R_CR4,
    R_CR5,
    R_CR6,
    R_CR7,
    R_CR8,
    R_CR9,
    R_CS,
    R_CX,
    R_DH,
    R_DI,
    R_DIL,
    R_DL,
    R_DR0,
    R_DR1,
    R_DR10,
    R_DR11,
    R_DR12,
    R_DR13,
    R_DR14,
    R_DR15,
    R_DR16,
    R_DR17,
    R_DR18,
    R_DR19,
    R_DR2,
    R_DR20,
    R_DR21,
    R_DR22,
    R_DR23,
    R_DR24,
    R_DR25,
    R_DR26,
    R_DR27,
    R_DR28,
    R_DR29,
    R_DR3,
    R_DR30,
    R_DR31,
    R_DR4,
    R_DR5,
    R_DR6,
    R_DR7,
    R_DR8,
    R_DR9,
    R_DS,
    R_DX,
    R_EAX,
    R_EBP,
    R_EBX,
    R_ECX,
    R_EDI,
    R_EDX,
    R_ES,
    R_ESI,
    R_ESP,
    R_FS,
    R_GS,
    R_K0,
    R_K1,
    R_K2,
    R_K3,
    R_K4,
    R_K5,
    R_K6,
    R_K7,
    R_MM0,
    R_MM1,
    R_MM2,
    R_MM3,
    R_MM4,
    R_MM5,
    R_MM6,
    R_MM7,
    R_R10,
    R_R10B,
    R_R10D,
    R_R10W,
    R_R11,
    R_R11B,
    R_R11D,
    R_R11W,
    R_R12,
    R_R12B,
    R_R12D,
    R_R12W,
    R_R13,
    R_R13B,
    R_R13D,
    R_R13W,
    R_R14,
    R_R14B,
    R_R14D,
    R_R14W,
    R_R15,
    R_R15B,
    R_R15D,
    R_R15W,
    R_R16,
    R_R16B,
    R_R16D,
    R_R16W,
    R_R17,
    R_R17B,
    R_R17D,
    R_R17W,
    R_R18,
    R_R18B,
    R_R18D,
    R_R18W,
    R_R19,
    R_R19B,
    R_R19D,
    R_R19W,
    R_R20,
    R_R20B,
    R_R20D,
    R_R20W,
    R_R21,
    R_R21B,
    R_R21D,
    R_R21W,
    R_R22,
    R_R22B,
    R_R22D,
    R_R22W,
    R_R23,
    R_R23B,
    R_R23D,
    R_R23W,
    R_R24,
    R_R24B,
    R_R24D,
    R_R24W,
    R_R25,
    R_R25B,
    R_R25D,
    R_R25W,
    R_R26,
    R_R26B,
    R_R26D,
    R_R26W,
    R_R27,
    R_R27B,
    R_R27D,
    R_R27W,
    R_R28,
    R_R28B,
    R_R28D,
    R_R28W,
    R_R29,
    R_R29B,
    R_R29D,
    R_R29W,
    R_R30,
    R_R30B,
    R_R30D,
    R_R30W,
    R_R31,
    R_R31B,
    R_R31D,
    R_R31W,
    R_R8,
    R_R8B,
    R_R8D,
    R_R8W,
    R_R9,
    R_R9B,
    R_R9D,
    R_R9W,
    R_RAX,
    R_RBP,
    R_RBX,
    R_RCX,
    R_RDI,
    R_RDX,
    R_RSI,
    R_RSP,
    R_SEGR6,
    R_SEGR7,
    R_SI,
    R_SIL,
    R_SP,
    R_SPL,
    R_SS,
    R_ST0,
    R_ST1,
    R_ST2,
    R_ST3,
    R_ST4,
    R_ST5,
    R_ST6,
    R_ST7,
    R_TMM0,
    R_TMM1,
    R_TMM2,
    R_TMM3,
    R_TMM4,
    R_TMM5,
    R_TMM6,
    R_TMM7,
    R_TR0,
    R_TR1,
    R_TR2,
    R_TR3,
    R_TR4,
    R_TR5,
    R_TR6,
    R_TR7,
    R_XMM0,
    R_XMM1,
    R_XMM10,
    R_XMM11,
    R_XMM12,
    R_XMM13,
    R_XMM14,
    R_XMM15,
    R_XMM16,
    R_XMM17,
    R_XMM18,
    R_XMM19,
    R_XMM2,
    R_XMM20,
    R_XMM21,
    R_XMM22,
    R_XMM23,
    R_XMM24,
    R_XMM25,
    R_XMM26,
    R_XMM27,
    R_XMM28,
    R_XMM29,
    R_XMM3,
    R_XMM30,
    R_XMM31,
    R_XMM4,
    R_XMM5,
    R_XMM6,
    R_XMM7,
    R_XMM8,
    R_XMM9,
    R_YMM0,
    R_YMM1,
    R_YMM10,
    R_YMM11,
    R_YMM12,
    R_YMM13,
    R_YMM14,
    R_YMM15,
    R_YMM16,
    R_YMM17,
    R_YMM18,
    R_YMM19,
    R_YMM2,
    R_YMM20,
    R_YMM21,
    R_YMM22,
    R_YMM23,
    R_YMM24,
    R_YMM25,
    R_YMM26,
    R_YMM27,
    R_YMM28,
    R_YMM29,
    R_YMM3,
    R_YMM30,
    R_YMM31,
    R_YMM4,
    R_YMM5,
    R_YMM6,
    R_YMM7,
    R_YMM8,
    R_YMM9,
    R_ZMM0,
    R_ZMM1,
    R_ZMM10,
    R_ZMM11,
    R_ZMM12,
    R_ZMM13,
    R_ZMM14,
    R_ZMM15,
    R_ZMM16,
    R_ZMM17,
    R_ZMM18,
    R_ZMM19,
    R_ZMM2,
    R_ZMM20,
    R_ZMM21,
    R_ZMM22,
    R_ZMM23,
    R_ZMM24,
    R_ZMM25,
    R_ZMM26,
    R_ZMM27,
    R_ZMM28,
    R_ZMM29,
    R_ZMM3,
    R_ZMM30,
    R_ZMM31,
    R_ZMM4,
    R_ZMM5,
    R_ZMM6,
    R_ZMM7,
    R_ZMM8,
    R_ZMM9,
    REG_ENUM_LIMIT
};

// The last register an expression may name.
inline constexpr int EXPR_REG_END = 344;

// The names, indexed by register number - EXPR_REG_START.
inline constexpr std::array<const char*, REG_ENUM_LIMIT - EXPR_REG_START> kRegNames = {{
    "ah",
    "al",
    "ax",
    "bh",
    "bl",
    "bnd0",
    "bnd1",
    "bnd2",
    "bnd3",
    "bp",
    "bpl",
    "bx",
    "ch",
    "cl",
    "cr0",
    "cr1",
    "cr10",
    "cr11",
    "cr12",
    "cr13",
    "cr14",
    "cr15",
    "cr16",
    "cr17",
    "cr18",
    "cr19",
    "cr2",
    "cr20",
    "cr21",
    "cr22",
    "cr23",
    "cr24",
    "cr25",
    "cr26",
    "cr27",
    "cr28",
    "cr29",
    "cr3",
    "cr30",
    "cr31",
    "cr4",
    "cr5",
    "cr6",
    "cr7",
    "cr8",
    "cr9",
    "cs",
    "cx",
    "dh",
    "di",
    "dil",
    "dl",
    "dr0",
    "dr1",
    "dr10",
    "dr11",
    "dr12",
    "dr13",
    "dr14",
    "dr15",
    "dr16",
    "dr17",
    "dr18",
    "dr19",
    "dr2",
    "dr20",
    "dr21",
    "dr22",
    "dr23",
    "dr24",
    "dr25",
    "dr26",
    "dr27",
    "dr28",
    "dr29",
    "dr3",
    "dr30",
    "dr31",
    "dr4",
    "dr5",
    "dr6",
    "dr7",
    "dr8",
    "dr9",
    "ds",
    "dx",
    "eax",
    "ebp",
    "ebx",
    "ecx",
    "edi",
    "edx",
    "es",
    "esi",
    "esp",
    "fs",
    "gs",
    "k0",
    "k1",
    "k2",
    "k3",
    "k4",
    "k5",
    "k6",
    "k7",
    "mm0",
    "mm1",
    "mm2",
    "mm3",
    "mm4",
    "mm5",
    "mm6",
    "mm7",
    "r10",
    "r10b",
    "r10d",
    "r10w",
    "r11",
    "r11b",
    "r11d",
    "r11w",
    "r12",
    "r12b",
    "r12d",
    "r12w",
    "r13",
    "r13b",
    "r13d",
    "r13w",
    "r14",
    "r14b",
    "r14d",
    "r14w",
    "r15",
    "r15b",
    "r15d",
    "r15w",
    "r16",
    "r16b",
    "r16d",
    "r16w",
    "r17",
    "r17b",
    "r17d",
    "r17w",
    "r18",
    "r18b",
    "r18d",
    "r18w",
    "r19",
    "r19b",
    "r19d",
    "r19w",
    "r20",
    "r20b",
    "r20d",
    "r20w",
    "r21",
    "r21b",
    "r21d",
    "r21w",
    "r22",
    "r22b",
    "r22d",
    "r22w",
    "r23",
    "r23b",
    "r23d",
    "r23w",
    "r24",
    "r24b",
    "r24d",
    "r24w",
    "r25",
    "r25b",
    "r25d",
    "r25w",
    "r26",
    "r26b",
    "r26d",
    "r26w",
    "r27",
    "r27b",
    "r27d",
    "r27w",
    "r28",
    "r28b",
    "r28d",
    "r28w",
    "r29",
    "r29b",
    "r29d",
    "r29w",
    "r30",
    "r30b",
    "r30d",
    "r30w",
    "r31",
    "r31b",
    "r31d",
    "r31w",
    "r8",
    "r8b",
    "r8d",
    "r8w",
    "r9",
    "r9b",
    "r9d",
    "r9w",
    "rax",
    "rbp",
    "rbx",
    "rcx",
    "rdi",
    "rdx",
    "rsi",
    "rsp",
    "segr6",
    "segr7",
    "si",
    "sil",
    "sp",
    "spl",
    "ss",
    "st0",
    "st1",
    "st2",
    "st3",
    "st4",
    "st5",
    "st6",
    "st7",
    "tmm0",
    "tmm1",
    "tmm2",
    "tmm3",
    "tmm4",
    "tmm5",
    "tmm6",
    "tmm7",
    "tr0",
    "tr1",
    "tr2",
    "tr3",
    "tr4",
    "tr5",
    "tr6",
    "tr7",
    "xmm0",
    "xmm1",
    "xmm10",
    "xmm11",
    "xmm12",
    "xmm13",
    "xmm14",
    "xmm15",
    "xmm16",
    "xmm17",
    "xmm18",
    "xmm19",
    "xmm2",
    "xmm20",
    "xmm21",
    "xmm22",
    "xmm23",
    "xmm24",
    "xmm25",
    "xmm26",
    "xmm27",
    "xmm28",
    "xmm29",
    "xmm3",
    "xmm30",
    "xmm31",
    "xmm4",
    "xmm5",
    "xmm6",
    "xmm7",
    "xmm8",
    "xmm9",
    "ymm0",
    "ymm1",
    "ymm10",
    "ymm11",
    "ymm12",
    "ymm13",
    "ymm14",
    "ymm15",
    "ymm16",
    "ymm17",
    "ymm18",
    "ymm19",
    "ymm2",
    "ymm20",
    "ymm21",
    "ymm22",
    "ymm23",
    "ymm24",
    "ymm25",
    "ymm26",
    "ymm27",
    "ymm28",
    "ymm29",
    "ymm3",
    "ymm30",
    "ymm31",
    "ymm4",
    "ymm5",
    "ymm6",
    "ymm7",
    "ymm8",
    "ymm9",
    "zmm0",
    "zmm1",
    "zmm10",
    "zmm11",
    "zmm12",
    "zmm13",
    "zmm14",
    "zmm15",
    "zmm16",
    "zmm17",
    "zmm18",
    "zmm19",
    "zmm2",
    "zmm20",
    "zmm21",
    "zmm22",
    "zmm23",
    "zmm24",
    "zmm25",
    "zmm26",
    "zmm27",
    "zmm28",
    "zmm29",
    "zmm3",
    "zmm30",
    "zmm31",
    "zmm4",
    "zmm5",
    "zmm6",
    "zmm7",
    "zmm8",
    "zmm9"
}};

// Each register's operand-flag word, indexed by register number (entry 0 is not a register).
inline constexpr std::array<OpFlags, REG_ENUM_LIMIT> kRegFlags = {{
    0,
    /* ah    */    rn_flags(4) | opf::REG_HIGH,
    /* al    */    rn_flags(0) | opf::REG_AL,
    /* ax    */    rn_flags(0) | opf::REG_AX,
    /* bh    */    rn_flags(7) | opf::REG_HIGH,
    /* bl    */    rn_flags(3) | opf::REG_BL,
    /* bnd0  */    rn_flags(0) | opf::BNDREG,
    /* bnd1  */    rn_flags(1) | opf::BNDREG,
    /* bnd2  */    rn_flags(2) | opf::BNDREG,
    /* bnd3  */    rn_flags(3) | opf::BNDREG,
    /* bp    */    rn_flags(5) | opf::REG16,
    /* bpl   */    rn_flags(5) | opf::REG8,
    /* bx    */    rn_flags(3) | opf::REG_BX,
    /* ch    */    rn_flags(5) | opf::REG_HIGH,
    /* cl    */    rn_flags(1) | opf::REG_CL,
    /* cr0   */    rn_flags(0) | opf::REG_CREG,
    /* cr1   */    rn_flags(1) | opf::REG_CREG,
    /* cr10  */    rn_flags(10) | opf::REG_CREG,
    /* cr11  */    rn_flags(11) | opf::REG_CREG,
    /* cr12  */    rn_flags(12) | opf::REG_CREG,
    /* cr13  */    rn_flags(13) | opf::REG_CREG,
    /* cr14  */    rn_flags(14) | opf::REG_CREG,
    /* cr15  */    rn_flags(15) | opf::REG_CREG,
    /* cr16  */    rn_flags(16) | opf::REG_CREG,
    /* cr17  */    rn_flags(17) | opf::REG_CREG,
    /* cr18  */    rn_flags(18) | opf::REG_CREG,
    /* cr19  */    rn_flags(19) | opf::REG_CREG,
    /* cr2   */    rn_flags(2) | opf::REG_CREG,
    /* cr20  */    rn_flags(20) | opf::REG_CREG,
    /* cr21  */    rn_flags(21) | opf::REG_CREG,
    /* cr22  */    rn_flags(22) | opf::REG_CREG,
    /* cr23  */    rn_flags(23) | opf::REG_CREG,
    /* cr24  */    rn_flags(24) | opf::REG_CREG,
    /* cr25  */    rn_flags(25) | opf::REG_CREG,
    /* cr26  */    rn_flags(26) | opf::REG_CREG,
    /* cr27  */    rn_flags(27) | opf::REG_CREG,
    /* cr28  */    rn_flags(28) | opf::REG_CREG,
    /* cr29  */    rn_flags(29) | opf::REG_CREG,
    /* cr3   */    rn_flags(3) | opf::REG_CREG,
    /* cr30  */    rn_flags(30) | opf::REG_CREG,
    /* cr31  */    rn_flags(31) | opf::REG_CREG,
    /* cr4   */    rn_flags(4) | opf::REG_CREG,
    /* cr5   */    rn_flags(5) | opf::REG_CREG,
    /* cr6   */    rn_flags(6) | opf::REG_CREG,
    /* cr7   */    rn_flags(7) | opf::REG_CREG,
    /* cr8   */    rn_flags(8) | opf::REG_CREG,
    /* cr9   */    rn_flags(9) | opf::REG_CREG,
    /* cs    */    rn_flags(1) | opf::REG_CS,
    /* cx    */    rn_flags(1) | opf::REG_CX,
    /* dh    */    rn_flags(6) | opf::REG_HIGH,
    /* di    */    rn_flags(7) | opf::REG16,
    /* dil   */    rn_flags(7) | opf::REG8,
    /* dl    */    rn_flags(2) | opf::REG_DL,
    /* dr0   */    rn_flags(0) | opf::REG_DREG,
    /* dr1   */    rn_flags(1) | opf::REG_DREG,
    /* dr10  */    rn_flags(10) | opf::REG_DREG,
    /* dr11  */    rn_flags(11) | opf::REG_DREG,
    /* dr12  */    rn_flags(12) | opf::REG_DREG,
    /* dr13  */    rn_flags(13) | opf::REG_DREG,
    /* dr14  */    rn_flags(14) | opf::REG_DREG,
    /* dr15  */    rn_flags(15) | opf::REG_DREG,
    /* dr16  */    rn_flags(16) | opf::REG_DREG,
    /* dr17  */    rn_flags(17) | opf::REG_DREG,
    /* dr18  */    rn_flags(18) | opf::REG_DREG,
    /* dr19  */    rn_flags(19) | opf::REG_DREG,
    /* dr2   */    rn_flags(2) | opf::REG_DREG,
    /* dr20  */    rn_flags(20) | opf::REG_DREG,
    /* dr21  */    rn_flags(21) | opf::REG_DREG,
    /* dr22  */    rn_flags(22) | opf::REG_DREG,
    /* dr23  */    rn_flags(23) | opf::REG_DREG,
    /* dr24  */    rn_flags(24) | opf::REG_DREG,
    /* dr25  */    rn_flags(25) | opf::REG_DREG,
    /* dr26  */    rn_flags(26) | opf::REG_DREG,
    /* dr27  */    rn_flags(27) | opf::REG_DREG,
    /* dr28  */    rn_flags(28) | opf::REG_DREG,
    /* dr29  */    rn_flags(29) | opf::REG_DREG,
    /* dr3   */    rn_flags(3) | opf::REG_DREG,
    /* dr30  */    rn_flags(30) | opf::REG_DREG,
    /* dr31  */    rn_flags(31) | opf::REG_DREG,
    /* dr4   */    rn_flags(4) | opf::REG_DREG,
    /* dr5   */    rn_flags(5) | opf::REG_DREG,
    /* dr6   */    rn_flags(6) | opf::REG_DREG,
    /* dr7   */    rn_flags(7) | opf::REG_DREG,
    /* dr8   */    rn_flags(8) | opf::REG_DREG,
    /* dr9   */    rn_flags(9) | opf::REG_DREG,
    /* ds    */    rn_flags(3) | opf::REG_DS,
    /* dx    */    rn_flags(2) | opf::REG_DX,
    /* eax   */    rn_flags(0) | opf::REG_EAX,
    /* ebp   */    rn_flags(5) | opf::REG32,
    /* ebx   */    rn_flags(3) | opf::REG_EBX,
    /* ecx   */    rn_flags(1) | opf::REG_ECX,
    /* edi   */    rn_flags(7) | opf::REG32,
    /* edx   */    rn_flags(2) | opf::REG_EDX,
    /* es    */    rn_flags(0) | opf::REG_ES,
    /* esi   */    rn_flags(6) | opf::REG32,
    /* esp   */    rn_flags(4) | opf::REG32,
    /* fs    */    rn_flags(4) | opf::REG_FS,
    /* gs    */    rn_flags(5) | opf::REG_GS,
    /* k0    */    rn_flags(0) | opf::OPMASKREG,
    /* k1    */    rn_flags(1) | opf::OPMASKREG,
    /* k2    */    rn_flags(2) | opf::OPMASKREG,
    /* k3    */    rn_flags(3) | opf::OPMASKREG,
    /* k4    */    rn_flags(4) | opf::OPMASKREG,
    /* k5    */    rn_flags(5) | opf::OPMASKREG,
    /* k6    */    rn_flags(6) | opf::OPMASKREG,
    /* k7    */    rn_flags(7) | opf::OPMASKREG,
    /* mm0   */    rn_flags(0) | opf::MMXREG,
    /* mm1   */    rn_flags(1) | opf::MMXREG,
    /* mm2   */    rn_flags(2) | opf::MMXREG,
    /* mm3   */    rn_flags(3) | opf::MMXREG,
    /* mm4   */    rn_flags(4) | opf::MMXREG,
    /* mm5   */    rn_flags(5) | opf::MMXREG,
    /* mm6   */    rn_flags(6) | opf::MMXREG,
    /* mm7   */    rn_flags(7) | opf::MMXREG,
    /* r10   */    rn_flags(10) | opf::REG64,
    /* r10b  */    rn_flags(10) | opf::REG8,
    /* r10d  */    rn_flags(10) | opf::REG32,
    /* r10w  */    rn_flags(10) | opf::REG16,
    /* r11   */    rn_flags(11) | opf::REG64,
    /* r11b  */    rn_flags(11) | opf::REG8,
    /* r11d  */    rn_flags(11) | opf::REG32,
    /* r11w  */    rn_flags(11) | opf::REG16,
    /* r12   */    rn_flags(12) | opf::REG64,
    /* r12b  */    rn_flags(12) | opf::REG8,
    /* r12d  */    rn_flags(12) | opf::REG32,
    /* r12w  */    rn_flags(12) | opf::REG16,
    /* r13   */    rn_flags(13) | opf::REG64,
    /* r13b  */    rn_flags(13) | opf::REG8,
    /* r13d  */    rn_flags(13) | opf::REG32,
    /* r13w  */    rn_flags(13) | opf::REG16,
    /* r14   */    rn_flags(14) | opf::REG64,
    /* r14b  */    rn_flags(14) | opf::REG8,
    /* r14d  */    rn_flags(14) | opf::REG32,
    /* r14w  */    rn_flags(14) | opf::REG16,
    /* r15   */    rn_flags(15) | opf::REG64,
    /* r15b  */    rn_flags(15) | opf::REG8,
    /* r15d  */    rn_flags(15) | opf::REG32,
    /* r15w  */    rn_flags(15) | opf::REG16,
    /* r16   */    rn_flags(16) | opf::REG64,
    /* r16b  */    rn_flags(16) | opf::REG8,
    /* r16d  */    rn_flags(16) | opf::REG32,
    /* r16w  */    rn_flags(16) | opf::REG16,
    /* r17   */    rn_flags(17) | opf::REG64,
    /* r17b  */    rn_flags(17) | opf::REG8,
    /* r17d  */    rn_flags(17) | opf::REG32,
    /* r17w  */    rn_flags(17) | opf::REG16,
    /* r18   */    rn_flags(18) | opf::REG64,
    /* r18b  */    rn_flags(18) | opf::REG8,
    /* r18d  */    rn_flags(18) | opf::REG32,
    /* r18w  */    rn_flags(18) | opf::REG16,
    /* r19   */    rn_flags(19) | opf::REG64,
    /* r19b  */    rn_flags(19) | opf::REG8,
    /* r19d  */    rn_flags(19) | opf::REG32,
    /* r19w  */    rn_flags(19) | opf::REG16,
    /* r20   */    rn_flags(20) | opf::REG64,
    /* r20b  */    rn_flags(20) | opf::REG8,
    /* r20d  */    rn_flags(20) | opf::REG32,
    /* r20w  */    rn_flags(20) | opf::REG16,
    /* r21   */    rn_flags(21) | opf::REG64,
    /* r21b  */    rn_flags(21) | opf::REG8,
    /* r21d  */    rn_flags(21) | opf::REG32,
    /* r21w  */    rn_flags(21) | opf::REG16,
    /* r22   */    rn_flags(22) | opf::REG64,
    /* r22b  */    rn_flags(22) | opf::REG8,
    /* r22d  */    rn_flags(22) | opf::REG32,
    /* r22w  */    rn_flags(22) | opf::REG16,
    /* r23   */    rn_flags(23) | opf::REG64,
    /* r23b  */    rn_flags(23) | opf::REG8,
    /* r23d  */    rn_flags(23) | opf::REG32,
    /* r23w  */    rn_flags(23) | opf::REG16,
    /* r24   */    rn_flags(24) | opf::REG64,
    /* r24b  */    rn_flags(24) | opf::REG8,
    /* r24d  */    rn_flags(24) | opf::REG32,
    /* r24w  */    rn_flags(24) | opf::REG16,
    /* r25   */    rn_flags(25) | opf::REG64,
    /* r25b  */    rn_flags(25) | opf::REG8,
    /* r25d  */    rn_flags(25) | opf::REG32,
    /* r25w  */    rn_flags(25) | opf::REG16,
    /* r26   */    rn_flags(26) | opf::REG64,
    /* r26b  */    rn_flags(26) | opf::REG8,
    /* r26d  */    rn_flags(26) | opf::REG32,
    /* r26w  */    rn_flags(26) | opf::REG16,
    /* r27   */    rn_flags(27) | opf::REG64,
    /* r27b  */    rn_flags(27) | opf::REG8,
    /* r27d  */    rn_flags(27) | opf::REG32,
    /* r27w  */    rn_flags(27) | opf::REG16,
    /* r28   */    rn_flags(28) | opf::REG64,
    /* r28b  */    rn_flags(28) | opf::REG8,
    /* r28d  */    rn_flags(28) | opf::REG32,
    /* r28w  */    rn_flags(28) | opf::REG16,
    /* r29   */    rn_flags(29) | opf::REG64,
    /* r29b  */    rn_flags(29) | opf::REG8,
    /* r29d  */    rn_flags(29) | opf::REG32,
    /* r29w  */    rn_flags(29) | opf::REG16,
    /* r30   */    rn_flags(30) | opf::REG64,
    /* r30b  */    rn_flags(30) | opf::REG8,
    /* r30d  */    rn_flags(30) | opf::REG32,
    /* r30w  */    rn_flags(30) | opf::REG16,
    /* r31   */    rn_flags(31) | opf::REG64,
    /* r31b  */    rn_flags(31) | opf::REG8,
    /* r31d  */    rn_flags(31) | opf::REG32,
    /* r31w  */    rn_flags(31) | opf::REG16,
    /* r8    */    rn_flags(8) | opf::REG64,
    /* r8b   */    rn_flags(8) | opf::REG8,
    /* r8d   */    rn_flags(8) | opf::REG32,
    /* r8w   */    rn_flags(8) | opf::REG16,
    /* r9    */    rn_flags(9) | opf::REG64,
    /* r9b   */    rn_flags(9) | opf::REG8,
    /* r9d   */    rn_flags(9) | opf::REG32,
    /* r9w   */    rn_flags(9) | opf::REG16,
    /* rax   */    rn_flags(0) | opf::REG_RAX,
    /* rbp   */    rn_flags(5) | opf::REG64,
    /* rbx   */    rn_flags(3) | opf::REG_RBX,
    /* rcx   */    rn_flags(1) | opf::REG_RCX,
    /* rdi   */    rn_flags(7) | opf::REG64,
    /* rdx   */    rn_flags(2) | opf::REG_RDX,
    /* rsi   */    rn_flags(6) | opf::REG64,
    /* rsp   */    rn_flags(4) | opf::REG64,
    /* segr6 */    rn_flags(6) | opf::REG_SEG67,
    /* segr7 */    rn_flags(7) | opf::REG_SEG67,
    /* si    */    rn_flags(6) | opf::REG16,
    /* sil   */    rn_flags(6) | opf::REG8,
    /* sp    */    rn_flags(4) | opf::REG16,
    /* spl   */    rn_flags(4) | opf::REG8,
    /* ss    */    rn_flags(2) | opf::REG_SS,
    /* st0   */    rn_flags(0) | opf::FPUREG,
    /* st1   */    rn_flags(1) | opf::FPUREG,
    /* st2   */    rn_flags(2) | opf::FPUREG,
    /* st3   */    rn_flags(3) | opf::FPUREG,
    /* st4   */    rn_flags(4) | opf::FPUREG,
    /* st5   */    rn_flags(5) | opf::FPUREG,
    /* st6   */    rn_flags(6) | opf::FPUREG,
    /* st7   */    rn_flags(7) | opf::FPUREG,
    /* tmm0  */    rn_flags(0) | opf::TMMREG,
    /* tmm1  */    rn_flags(1) | opf::TMMREG,
    /* tmm2  */    rn_flags(2) | opf::TMMREG,
    /* tmm3  */    rn_flags(3) | opf::TMMREG,
    /* tmm4  */    rn_flags(4) | opf::TMMREG,
    /* tmm5  */    rn_flags(5) | opf::TMMREG,
    /* tmm6  */    rn_flags(6) | opf::TMMREG,
    /* tmm7  */    rn_flags(7) | opf::TMMREG,
    /* tr0   */    rn_flags(0) | opf::REG_TREG,
    /* tr1   */    rn_flags(1) | opf::REG_TREG,
    /* tr2   */    rn_flags(2) | opf::REG_TREG,
    /* tr3   */    rn_flags(3) | opf::REG_TREG,
    /* tr4   */    rn_flags(4) | opf::REG_TREG,
    /* tr5   */    rn_flags(5) | opf::REG_TREG,
    /* tr6   */    rn_flags(6) | opf::REG_TREG,
    /* tr7   */    rn_flags(7) | opf::REG_TREG,
    /* xmm0  */    rn_flags(0) | opf::XMMREG,
    /* xmm1  */    rn_flags(1) | opf::XMMREG,
    /* xmm10 */    rn_flags(10) | opf::XMMREG,
    /* xmm11 */    rn_flags(11) | opf::XMMREG,
    /* xmm12 */    rn_flags(12) | opf::XMMREG,
    /* xmm13 */    rn_flags(13) | opf::XMMREG,
    /* xmm14 */    rn_flags(14) | opf::XMMREG,
    /* xmm15 */    rn_flags(15) | opf::XMMREG,
    /* xmm16 */    rn_flags(16) | opf::XMMREG,
    /* xmm17 */    rn_flags(17) | opf::XMMREG,
    /* xmm18 */    rn_flags(18) | opf::XMMREG,
    /* xmm19 */    rn_flags(19) | opf::XMMREG,
    /* xmm2  */    rn_flags(2) | opf::XMMREG,
    /* xmm20 */    rn_flags(20) | opf::XMMREG,
    /* xmm21 */    rn_flags(21) | opf::XMMREG,
    /* xmm22 */    rn_flags(22) | opf::XMMREG,
    /* xmm23 */    rn_flags(23) | opf::XMMREG,
    /* xmm24 */    rn_flags(24) | opf::XMMREG,
    /* xmm25 */    rn_flags(25) | opf::XMMREG,
    /* xmm26 */    rn_flags(26) | opf::XMMREG,
    /* xmm27 */    rn_flags(27) | opf::XMMREG,
    /* xmm28 */    rn_flags(28) | opf::XMMREG,
    /* xmm29 */    rn_flags(29) | opf::XMMREG,
    /* xmm3  */    rn_flags(3) | opf::XMMREG,
    /* xmm30 */    rn_flags(30) | opf::XMMREG,
    /* xmm31 */    rn_flags(31) | opf::XMMREG,
    /* xmm4  */    rn_flags(4) | opf::XMMREG,
    /* xmm5  */    rn_flags(5) | opf::XMMREG,
    /* xmm6  */    rn_flags(6) | opf::XMMREG,
    /* xmm7  */    rn_flags(7) | opf::XMMREG,
    /* xmm8  */    rn_flags(8) | opf::XMMREG,
    /* xmm9  */    rn_flags(9) | opf::XMMREG,
    /* ymm0  */    rn_flags(0) | opf::YMMREG,
    /* ymm1  */    rn_flags(1) | opf::YMMREG,
    /* ymm10 */    rn_flags(10) | opf::YMMREG,
    /* ymm11 */    rn_flags(11) | opf::YMMREG,
    /* ymm12 */    rn_flags(12) | opf::YMMREG,
    /* ymm13 */    rn_flags(13) | opf::YMMREG,
    /* ymm14 */    rn_flags(14) | opf::YMMREG,
    /* ymm15 */    rn_flags(15) | opf::YMMREG,
    /* ymm16 */    rn_flags(16) | opf::YMMREG,
    /* ymm17 */    rn_flags(17) | opf::YMMREG,
    /* ymm18 */    rn_flags(18) | opf::YMMREG,
    /* ymm19 */    rn_flags(19) | opf::YMMREG,
    /* ymm2  */    rn_flags(2) | opf::YMMREG,
    /* ymm20 */    rn_flags(20) | opf::YMMREG,
    /* ymm21 */    rn_flags(21) | opf::YMMREG,
    /* ymm22 */    rn_flags(22) | opf::YMMREG,
    /* ymm23 */    rn_flags(23) | opf::YMMREG,
    /* ymm24 */    rn_flags(24) | opf::YMMREG,
    /* ymm25 */    rn_flags(25) | opf::YMMREG,
    /* ymm26 */    rn_flags(26) | opf::YMMREG,
    /* ymm27 */    rn_flags(27) | opf::YMMREG,
    /* ymm28 */    rn_flags(28) | opf::YMMREG,
    /* ymm29 */    rn_flags(29) | opf::YMMREG,
    /* ymm3  */    rn_flags(3) | opf::YMMREG,
    /* ymm30 */    rn_flags(30) | opf::YMMREG,
    /* ymm31 */    rn_flags(31) | opf::YMMREG,
    /* ymm4  */    rn_flags(4) | opf::YMMREG,
    /* ymm5  */    rn_flags(5) | opf::YMMREG,
    /* ymm6  */    rn_flags(6) | opf::YMMREG,
    /* ymm7  */    rn_flags(7) | opf::YMMREG,
    /* ymm8  */    rn_flags(8) | opf::YMMREG,
    /* ymm9  */    rn_flags(9) | opf::YMMREG,
    /* zmm0  */    rn_flags(0) | opf::ZMMREG,
    /* zmm1  */    rn_flags(1) | opf::ZMMREG,
    /* zmm10 */    rn_flags(10) | opf::ZMMREG,
    /* zmm11 */    rn_flags(11) | opf::ZMMREG,
    /* zmm12 */    rn_flags(12) | opf::ZMMREG,
    /* zmm13 */    rn_flags(13) | opf::ZMMREG,
    /* zmm14 */    rn_flags(14) | opf::ZMMREG,
    /* zmm15 */    rn_flags(15) | opf::ZMMREG,
    /* zmm16 */    rn_flags(16) | opf::ZMMREG,
    /* zmm17 */    rn_flags(17) | opf::ZMMREG,
    /* zmm18 */    rn_flags(18) | opf::ZMMREG,
    /* zmm19 */    rn_flags(19) | opf::ZMMREG,
    /* zmm2  */    rn_flags(2) | opf::ZMMREG,
    /* zmm20 */    rn_flags(20) | opf::ZMMREG,
    /* zmm21 */    rn_flags(21) | opf::ZMMREG,
    /* zmm22 */    rn_flags(22) | opf::ZMMREG,
    /* zmm23 */    rn_flags(23) | opf::ZMMREG,
    /* zmm24 */    rn_flags(24) | opf::ZMMREG,
    /* zmm25 */    rn_flags(25) | opf::ZMMREG,
    /* zmm26 */    rn_flags(26) | opf::ZMMREG,
    /* zmm27 */    rn_flags(27) | opf::ZMMREG,
    /* zmm28 */    rn_flags(28) | opf::ZMMREG,
    /* zmm29 */    rn_flags(29) | opf::ZMMREG,
    /* zmm3  */    rn_flags(3) | opf::ZMMREG,
    /* zmm30 */    rn_flags(30) | opf::ZMMREG,
    /* zmm31 */    rn_flags(31) | opf::ZMMREG,
    /* zmm4  */    rn_flags(4) | opf::ZMMREG,
    /* zmm5  */    rn_flags(5) | opf::ZMMREG,
    /* zmm6  */    rn_flags(6) | opf::ZMMREG,
    /* zmm7  */    rn_flags(7) | opf::ZMMREG,
    /* zmm8  */    rn_flags(8) | opf::ZMMREG,
    /* zmm9  */    rn_flags(9) | opf::ZMMREG,
}};

// Each register's x86 register number (its encoding in ModRM/SIB/opcode), indexed by register number (entry 0 is -1).
inline constexpr std::array<int, REG_ENUM_LIMIT> kRegVals = {{
    -1,
     4,  /* ah    */
     0,  /* al    */
     0,  /* ax    */
     7,  /* bh    */
     3,  /* bl    */
     0,  /* bnd0  */
     1,  /* bnd1  */
     2,  /* bnd2  */
     3,  /* bnd3  */
     5,  /* bp    */
     5,  /* bpl   */
     3,  /* bx    */
     5,  /* ch    */
     1,  /* cl    */
     0,  /* cr0   */
     1,  /* cr1   */
    10,  /* cr10  */
    11,  /* cr11  */
    12,  /* cr12  */
    13,  /* cr13  */
    14,  /* cr14  */
    15,  /* cr15  */
    16,  /* cr16  */
    17,  /* cr17  */
    18,  /* cr18  */
    19,  /* cr19  */
     2,  /* cr2   */
    20,  /* cr20  */
    21,  /* cr21  */
    22,  /* cr22  */
    23,  /* cr23  */
    24,  /* cr24  */
    25,  /* cr25  */
    26,  /* cr26  */
    27,  /* cr27  */
    28,  /* cr28  */
    29,  /* cr29  */
     3,  /* cr3   */
    30,  /* cr30  */
    31,  /* cr31  */
     4,  /* cr4   */
     5,  /* cr5   */
     6,  /* cr6   */
     7,  /* cr7   */
     8,  /* cr8   */
     9,  /* cr9   */
     1,  /* cs    */
     1,  /* cx    */
     6,  /* dh    */
     7,  /* di    */
     7,  /* dil   */
     2,  /* dl    */
     0,  /* dr0   */
     1,  /* dr1   */
    10,  /* dr10  */
    11,  /* dr11  */
    12,  /* dr12  */
    13,  /* dr13  */
    14,  /* dr14  */
    15,  /* dr15  */
    16,  /* dr16  */
    17,  /* dr17  */
    18,  /* dr18  */
    19,  /* dr19  */
     2,  /* dr2   */
    20,  /* dr20  */
    21,  /* dr21  */
    22,  /* dr22  */
    23,  /* dr23  */
    24,  /* dr24  */
    25,  /* dr25  */
    26,  /* dr26  */
    27,  /* dr27  */
    28,  /* dr28  */
    29,  /* dr29  */
     3,  /* dr3   */
    30,  /* dr30  */
    31,  /* dr31  */
     4,  /* dr4   */
     5,  /* dr5   */
     6,  /* dr6   */
     7,  /* dr7   */
     8,  /* dr8   */
     9,  /* dr9   */
     3,  /* ds    */
     2,  /* dx    */
     0,  /* eax   */
     5,  /* ebp   */
     3,  /* ebx   */
     1,  /* ecx   */
     7,  /* edi   */
     2,  /* edx   */
     0,  /* es    */
     6,  /* esi   */
     4,  /* esp   */
     4,  /* fs    */
     5,  /* gs    */
     0,  /* k0    */
     1,  /* k1    */
     2,  /* k2    */
     3,  /* k3    */
     4,  /* k4    */
     5,  /* k5    */
     6,  /* k6    */
     7,  /* k7    */
     0,  /* mm0   */
     1,  /* mm1   */
     2,  /* mm2   */
     3,  /* mm3   */
     4,  /* mm4   */
     5,  /* mm5   */
     6,  /* mm6   */
     7,  /* mm7   */
    10,  /* r10   */
    10,  /* r10b  */
    10,  /* r10d  */
    10,  /* r10w  */
    11,  /* r11   */
    11,  /* r11b  */
    11,  /* r11d  */
    11,  /* r11w  */
    12,  /* r12   */
    12,  /* r12b  */
    12,  /* r12d  */
    12,  /* r12w  */
    13,  /* r13   */
    13,  /* r13b  */
    13,  /* r13d  */
    13,  /* r13w  */
    14,  /* r14   */
    14,  /* r14b  */
    14,  /* r14d  */
    14,  /* r14w  */
    15,  /* r15   */
    15,  /* r15b  */
    15,  /* r15d  */
    15,  /* r15w  */
    16,  /* r16   */
    16,  /* r16b  */
    16,  /* r16d  */
    16,  /* r16w  */
    17,  /* r17   */
    17,  /* r17b  */
    17,  /* r17d  */
    17,  /* r17w  */
    18,  /* r18   */
    18,  /* r18b  */
    18,  /* r18d  */
    18,  /* r18w  */
    19,  /* r19   */
    19,  /* r19b  */
    19,  /* r19d  */
    19,  /* r19w  */
    20,  /* r20   */
    20,  /* r20b  */
    20,  /* r20d  */
    20,  /* r20w  */
    21,  /* r21   */
    21,  /* r21b  */
    21,  /* r21d  */
    21,  /* r21w  */
    22,  /* r22   */
    22,  /* r22b  */
    22,  /* r22d  */
    22,  /* r22w  */
    23,  /* r23   */
    23,  /* r23b  */
    23,  /* r23d  */
    23,  /* r23w  */
    24,  /* r24   */
    24,  /* r24b  */
    24,  /* r24d  */
    24,  /* r24w  */
    25,  /* r25   */
    25,  /* r25b  */
    25,  /* r25d  */
    25,  /* r25w  */
    26,  /* r26   */
    26,  /* r26b  */
    26,  /* r26d  */
    26,  /* r26w  */
    27,  /* r27   */
    27,  /* r27b  */
    27,  /* r27d  */
    27,  /* r27w  */
    28,  /* r28   */
    28,  /* r28b  */
    28,  /* r28d  */
    28,  /* r28w  */
    29,  /* r29   */
    29,  /* r29b  */
    29,  /* r29d  */
    29,  /* r29w  */
    30,  /* r30   */
    30,  /* r30b  */
    30,  /* r30d  */
    30,  /* r30w  */
    31,  /* r31   */
    31,  /* r31b  */
    31,  /* r31d  */
    31,  /* r31w  */
     8,  /* r8    */
     8,  /* r8b   */
     8,  /* r8d   */
     8,  /* r8w   */
     9,  /* r9    */
     9,  /* r9b   */
     9,  /* r9d   */
     9,  /* r9w   */
     0,  /* rax   */
     5,  /* rbp   */
     3,  /* rbx   */
     1,  /* rcx   */
     7,  /* rdi   */
     2,  /* rdx   */
     6,  /* rsi   */
     4,  /* rsp   */
     6,  /* segr6 */
     7,  /* segr7 */
     6,  /* si    */
     6,  /* sil   */
     4,  /* sp    */
     4,  /* spl   */
     2,  /* ss    */
     0,  /* st0   */
     1,  /* st1   */
     2,  /* st2   */
     3,  /* st3   */
     4,  /* st4   */
     5,  /* st5   */
     6,  /* st6   */
     7,  /* st7   */
     0,  /* tmm0  */
     1,  /* tmm1  */
     2,  /* tmm2  */
     3,  /* tmm3  */
     4,  /* tmm4  */
     5,  /* tmm5  */
     6,  /* tmm6  */
     7,  /* tmm7  */
     0,  /* tr0   */
     1,  /* tr1   */
     2,  /* tr2   */
     3,  /* tr3   */
     4,  /* tr4   */
     5,  /* tr5   */
     6,  /* tr6   */
     7,  /* tr7   */
     0,  /* xmm0  */
     1,  /* xmm1  */
    10,  /* xmm10 */
    11,  /* xmm11 */
    12,  /* xmm12 */
    13,  /* xmm13 */
    14,  /* xmm14 */
    15,  /* xmm15 */
    16,  /* xmm16 */
    17,  /* xmm17 */
    18,  /* xmm18 */
    19,  /* xmm19 */
     2,  /* xmm2  */
    20,  /* xmm20 */
    21,  /* xmm21 */
    22,  /* xmm22 */
    23,  /* xmm23 */
    24,  /* xmm24 */
    25,  /* xmm25 */
    26,  /* xmm26 */
    27,  /* xmm27 */
    28,  /* xmm28 */
    29,  /* xmm29 */
     3,  /* xmm3  */
    30,  /* xmm30 */
    31,  /* xmm31 */
     4,  /* xmm4  */
     5,  /* xmm5  */
     6,  /* xmm6  */
     7,  /* xmm7  */
     8,  /* xmm8  */
     9,  /* xmm9  */
     0,  /* ymm0  */
     1,  /* ymm1  */
    10,  /* ymm10 */
    11,  /* ymm11 */
    12,  /* ymm12 */
    13,  /* ymm13 */
    14,  /* ymm14 */
    15,  /* ymm15 */
    16,  /* ymm16 */
    17,  /* ymm17 */
    18,  /* ymm18 */
    19,  /* ymm19 */
     2,  /* ymm2  */
    20,  /* ymm20 */
    21,  /* ymm21 */
    22,  /* ymm22 */
    23,  /* ymm23 */
    24,  /* ymm24 */
    25,  /* ymm25 */
    26,  /* ymm26 */
    27,  /* ymm27 */
    28,  /* ymm28 */
    29,  /* ymm29 */
     3,  /* ymm3  */
    30,  /* ymm30 */
    31,  /* ymm31 */
     4,  /* ymm4  */
     5,  /* ymm5  */
     6,  /* ymm6  */
     7,  /* ymm7  */
     8,  /* ymm8  */
     9,  /* ymm9  */
     0,  /* zmm0  */
     1,  /* zmm1  */
    10,  /* zmm10 */
    11,  /* zmm11 */
    12,  /* zmm12 */
    13,  /* zmm13 */
    14,  /* zmm14 */
    15,  /* zmm15 */
    16,  /* zmm16 */
    17,  /* zmm17 */
    18,  /* zmm18 */
    19,  /* zmm19 */
     2,  /* zmm2  */
    20,  /* zmm20 */
    21,  /* zmm21 */
    22,  /* zmm22 */
    23,  /* zmm23 */
    24,  /* zmm24 */
    25,  /* zmm25 */
    26,  /* zmm26 */
    27,  /* zmm27 */
    28,  /* zmm28 */
    29,  /* zmm29 */
     3,  /* zmm3  */
    30,  /* zmm30 */
    31,  /* zmm31 */
     4,  /* zmm4  */
     5,  /* zmm5  */
     6,  /* zmm6  */
     7,  /* zmm7  */
     8,  /* zmm8  */
     9,  /* zmm9  */
}};

// The register numbers as the hardware encodes them (REG_NUM_ESP and so on, regs.h).
inline constexpr int REG_NUM_AH = 4;
inline constexpr int REG_NUM_AL = 0;
inline constexpr int REG_NUM_AX = 0;
inline constexpr int REG_NUM_BH = 7;
inline constexpr int REG_NUM_BL = 3;
inline constexpr int REG_NUM_BND0 = 0;
inline constexpr int REG_NUM_BND1 = 1;
inline constexpr int REG_NUM_BND2 = 2;
inline constexpr int REG_NUM_BND3 = 3;
inline constexpr int REG_NUM_BP = 5;
inline constexpr int REG_NUM_BPL = 5;
inline constexpr int REG_NUM_BX = 3;
inline constexpr int REG_NUM_CH = 5;
inline constexpr int REG_NUM_CL = 1;
inline constexpr int REG_NUM_CR0 = 0;
inline constexpr int REG_NUM_CR1 = 1;
inline constexpr int REG_NUM_CR10 = 10;
inline constexpr int REG_NUM_CR11 = 11;
inline constexpr int REG_NUM_CR12 = 12;
inline constexpr int REG_NUM_CR13 = 13;
inline constexpr int REG_NUM_CR14 = 14;
inline constexpr int REG_NUM_CR15 = 15;
inline constexpr int REG_NUM_CR16 = 16;
inline constexpr int REG_NUM_CR17 = 17;
inline constexpr int REG_NUM_CR18 = 18;
inline constexpr int REG_NUM_CR19 = 19;
inline constexpr int REG_NUM_CR2 = 2;
inline constexpr int REG_NUM_CR20 = 20;
inline constexpr int REG_NUM_CR21 = 21;
inline constexpr int REG_NUM_CR22 = 22;
inline constexpr int REG_NUM_CR23 = 23;
inline constexpr int REG_NUM_CR24 = 24;
inline constexpr int REG_NUM_CR25 = 25;
inline constexpr int REG_NUM_CR26 = 26;
inline constexpr int REG_NUM_CR27 = 27;
inline constexpr int REG_NUM_CR28 = 28;
inline constexpr int REG_NUM_CR29 = 29;
inline constexpr int REG_NUM_CR3 = 3;
inline constexpr int REG_NUM_CR30 = 30;
inline constexpr int REG_NUM_CR31 = 31;
inline constexpr int REG_NUM_CR4 = 4;
inline constexpr int REG_NUM_CR5 = 5;
inline constexpr int REG_NUM_CR6 = 6;
inline constexpr int REG_NUM_CR7 = 7;
inline constexpr int REG_NUM_CR8 = 8;
inline constexpr int REG_NUM_CR9 = 9;
inline constexpr int REG_NUM_CS = 1;
inline constexpr int REG_NUM_CX = 1;
inline constexpr int REG_NUM_DH = 6;
inline constexpr int REG_NUM_DI = 7;
inline constexpr int REG_NUM_DIL = 7;
inline constexpr int REG_NUM_DL = 2;
inline constexpr int REG_NUM_DR0 = 0;
inline constexpr int REG_NUM_DR1 = 1;
inline constexpr int REG_NUM_DR10 = 10;
inline constexpr int REG_NUM_DR11 = 11;
inline constexpr int REG_NUM_DR12 = 12;
inline constexpr int REG_NUM_DR13 = 13;
inline constexpr int REG_NUM_DR14 = 14;
inline constexpr int REG_NUM_DR15 = 15;
inline constexpr int REG_NUM_DR16 = 16;
inline constexpr int REG_NUM_DR17 = 17;
inline constexpr int REG_NUM_DR18 = 18;
inline constexpr int REG_NUM_DR19 = 19;
inline constexpr int REG_NUM_DR2 = 2;
inline constexpr int REG_NUM_DR20 = 20;
inline constexpr int REG_NUM_DR21 = 21;
inline constexpr int REG_NUM_DR22 = 22;
inline constexpr int REG_NUM_DR23 = 23;
inline constexpr int REG_NUM_DR24 = 24;
inline constexpr int REG_NUM_DR25 = 25;
inline constexpr int REG_NUM_DR26 = 26;
inline constexpr int REG_NUM_DR27 = 27;
inline constexpr int REG_NUM_DR28 = 28;
inline constexpr int REG_NUM_DR29 = 29;
inline constexpr int REG_NUM_DR3 = 3;
inline constexpr int REG_NUM_DR30 = 30;
inline constexpr int REG_NUM_DR31 = 31;
inline constexpr int REG_NUM_DR4 = 4;
inline constexpr int REG_NUM_DR5 = 5;
inline constexpr int REG_NUM_DR6 = 6;
inline constexpr int REG_NUM_DR7 = 7;
inline constexpr int REG_NUM_DR8 = 8;
inline constexpr int REG_NUM_DR9 = 9;
inline constexpr int REG_NUM_DS = 3;
inline constexpr int REG_NUM_DX = 2;
inline constexpr int REG_NUM_EAX = 0;
inline constexpr int REG_NUM_EBP = 5;
inline constexpr int REG_NUM_EBX = 3;
inline constexpr int REG_NUM_ECX = 1;
inline constexpr int REG_NUM_EDI = 7;
inline constexpr int REG_NUM_EDX = 2;
inline constexpr int REG_NUM_ES = 0;
inline constexpr int REG_NUM_ESI = 6;
inline constexpr int REG_NUM_ESP = 4;
inline constexpr int REG_NUM_FS = 4;
inline constexpr int REG_NUM_GS = 5;
inline constexpr int REG_NUM_K0 = 0;
inline constexpr int REG_NUM_K1 = 1;
inline constexpr int REG_NUM_K2 = 2;
inline constexpr int REG_NUM_K3 = 3;
inline constexpr int REG_NUM_K4 = 4;
inline constexpr int REG_NUM_K5 = 5;
inline constexpr int REG_NUM_K6 = 6;
inline constexpr int REG_NUM_K7 = 7;
inline constexpr int REG_NUM_MM0 = 0;
inline constexpr int REG_NUM_MM1 = 1;
inline constexpr int REG_NUM_MM2 = 2;
inline constexpr int REG_NUM_MM3 = 3;
inline constexpr int REG_NUM_MM4 = 4;
inline constexpr int REG_NUM_MM5 = 5;
inline constexpr int REG_NUM_MM6 = 6;
inline constexpr int REG_NUM_MM7 = 7;
inline constexpr int REG_NUM_R10 = 10;
inline constexpr int REG_NUM_R10B = 10;
inline constexpr int REG_NUM_R10D = 10;
inline constexpr int REG_NUM_R10W = 10;
inline constexpr int REG_NUM_R11 = 11;
inline constexpr int REG_NUM_R11B = 11;
inline constexpr int REG_NUM_R11D = 11;
inline constexpr int REG_NUM_R11W = 11;
inline constexpr int REG_NUM_R12 = 12;
inline constexpr int REG_NUM_R12B = 12;
inline constexpr int REG_NUM_R12D = 12;
inline constexpr int REG_NUM_R12W = 12;
inline constexpr int REG_NUM_R13 = 13;
inline constexpr int REG_NUM_R13B = 13;
inline constexpr int REG_NUM_R13D = 13;
inline constexpr int REG_NUM_R13W = 13;
inline constexpr int REG_NUM_R14 = 14;
inline constexpr int REG_NUM_R14B = 14;
inline constexpr int REG_NUM_R14D = 14;
inline constexpr int REG_NUM_R14W = 14;
inline constexpr int REG_NUM_R15 = 15;
inline constexpr int REG_NUM_R15B = 15;
inline constexpr int REG_NUM_R15D = 15;
inline constexpr int REG_NUM_R15W = 15;
inline constexpr int REG_NUM_R16 = 16;
inline constexpr int REG_NUM_R16B = 16;
inline constexpr int REG_NUM_R16D = 16;
inline constexpr int REG_NUM_R16W = 16;
inline constexpr int REG_NUM_R17 = 17;
inline constexpr int REG_NUM_R17B = 17;
inline constexpr int REG_NUM_R17D = 17;
inline constexpr int REG_NUM_R17W = 17;
inline constexpr int REG_NUM_R18 = 18;
inline constexpr int REG_NUM_R18B = 18;
inline constexpr int REG_NUM_R18D = 18;
inline constexpr int REG_NUM_R18W = 18;
inline constexpr int REG_NUM_R19 = 19;
inline constexpr int REG_NUM_R19B = 19;
inline constexpr int REG_NUM_R19D = 19;
inline constexpr int REG_NUM_R19W = 19;
inline constexpr int REG_NUM_R20 = 20;
inline constexpr int REG_NUM_R20B = 20;
inline constexpr int REG_NUM_R20D = 20;
inline constexpr int REG_NUM_R20W = 20;
inline constexpr int REG_NUM_R21 = 21;
inline constexpr int REG_NUM_R21B = 21;
inline constexpr int REG_NUM_R21D = 21;
inline constexpr int REG_NUM_R21W = 21;
inline constexpr int REG_NUM_R22 = 22;
inline constexpr int REG_NUM_R22B = 22;
inline constexpr int REG_NUM_R22D = 22;
inline constexpr int REG_NUM_R22W = 22;
inline constexpr int REG_NUM_R23 = 23;
inline constexpr int REG_NUM_R23B = 23;
inline constexpr int REG_NUM_R23D = 23;
inline constexpr int REG_NUM_R23W = 23;
inline constexpr int REG_NUM_R24 = 24;
inline constexpr int REG_NUM_R24B = 24;
inline constexpr int REG_NUM_R24D = 24;
inline constexpr int REG_NUM_R24W = 24;
inline constexpr int REG_NUM_R25 = 25;
inline constexpr int REG_NUM_R25B = 25;
inline constexpr int REG_NUM_R25D = 25;
inline constexpr int REG_NUM_R25W = 25;
inline constexpr int REG_NUM_R26 = 26;
inline constexpr int REG_NUM_R26B = 26;
inline constexpr int REG_NUM_R26D = 26;
inline constexpr int REG_NUM_R26W = 26;
inline constexpr int REG_NUM_R27 = 27;
inline constexpr int REG_NUM_R27B = 27;
inline constexpr int REG_NUM_R27D = 27;
inline constexpr int REG_NUM_R27W = 27;
inline constexpr int REG_NUM_R28 = 28;
inline constexpr int REG_NUM_R28B = 28;
inline constexpr int REG_NUM_R28D = 28;
inline constexpr int REG_NUM_R28W = 28;
inline constexpr int REG_NUM_R29 = 29;
inline constexpr int REG_NUM_R29B = 29;
inline constexpr int REG_NUM_R29D = 29;
inline constexpr int REG_NUM_R29W = 29;
inline constexpr int REG_NUM_R30 = 30;
inline constexpr int REG_NUM_R30B = 30;
inline constexpr int REG_NUM_R30D = 30;
inline constexpr int REG_NUM_R30W = 30;
inline constexpr int REG_NUM_R31 = 31;
inline constexpr int REG_NUM_R31B = 31;
inline constexpr int REG_NUM_R31D = 31;
inline constexpr int REG_NUM_R31W = 31;
inline constexpr int REG_NUM_R8 = 8;
inline constexpr int REG_NUM_R8B = 8;
inline constexpr int REG_NUM_R8D = 8;
inline constexpr int REG_NUM_R8W = 8;
inline constexpr int REG_NUM_R9 = 9;
inline constexpr int REG_NUM_R9B = 9;
inline constexpr int REG_NUM_R9D = 9;
inline constexpr int REG_NUM_R9W = 9;
inline constexpr int REG_NUM_RAX = 0;
inline constexpr int REG_NUM_RBP = 5;
inline constexpr int REG_NUM_RBX = 3;
inline constexpr int REG_NUM_RCX = 1;
inline constexpr int REG_NUM_RDI = 7;
inline constexpr int REG_NUM_RDX = 2;
inline constexpr int REG_NUM_RSI = 6;
inline constexpr int REG_NUM_RSP = 4;
inline constexpr int REG_NUM_SEGR6 = 6;
inline constexpr int REG_NUM_SEGR7 = 7;
inline constexpr int REG_NUM_SI = 6;
inline constexpr int REG_NUM_SIL = 6;
inline constexpr int REG_NUM_SP = 4;
inline constexpr int REG_NUM_SPL = 4;
inline constexpr int REG_NUM_SS = 2;
inline constexpr int REG_NUM_ST0 = 0;
inline constexpr int REG_NUM_ST1 = 1;
inline constexpr int REG_NUM_ST2 = 2;
inline constexpr int REG_NUM_ST3 = 3;
inline constexpr int REG_NUM_ST4 = 4;
inline constexpr int REG_NUM_ST5 = 5;
inline constexpr int REG_NUM_ST6 = 6;
inline constexpr int REG_NUM_ST7 = 7;
inline constexpr int REG_NUM_TMM0 = 0;
inline constexpr int REG_NUM_TMM1 = 1;
inline constexpr int REG_NUM_TMM2 = 2;
inline constexpr int REG_NUM_TMM3 = 3;
inline constexpr int REG_NUM_TMM4 = 4;
inline constexpr int REG_NUM_TMM5 = 5;
inline constexpr int REG_NUM_TMM6 = 6;
inline constexpr int REG_NUM_TMM7 = 7;
inline constexpr int REG_NUM_TR0 = 0;
inline constexpr int REG_NUM_TR1 = 1;
inline constexpr int REG_NUM_TR2 = 2;
inline constexpr int REG_NUM_TR3 = 3;
inline constexpr int REG_NUM_TR4 = 4;
inline constexpr int REG_NUM_TR5 = 5;
inline constexpr int REG_NUM_TR6 = 6;
inline constexpr int REG_NUM_TR7 = 7;
inline constexpr int REG_NUM_XMM0 = 0;
inline constexpr int REG_NUM_XMM1 = 1;
inline constexpr int REG_NUM_XMM10 = 10;
inline constexpr int REG_NUM_XMM11 = 11;
inline constexpr int REG_NUM_XMM12 = 12;
inline constexpr int REG_NUM_XMM13 = 13;
inline constexpr int REG_NUM_XMM14 = 14;
inline constexpr int REG_NUM_XMM15 = 15;
inline constexpr int REG_NUM_XMM16 = 16;
inline constexpr int REG_NUM_XMM17 = 17;
inline constexpr int REG_NUM_XMM18 = 18;
inline constexpr int REG_NUM_XMM19 = 19;
inline constexpr int REG_NUM_XMM2 = 2;
inline constexpr int REG_NUM_XMM20 = 20;
inline constexpr int REG_NUM_XMM21 = 21;
inline constexpr int REG_NUM_XMM22 = 22;
inline constexpr int REG_NUM_XMM23 = 23;
inline constexpr int REG_NUM_XMM24 = 24;
inline constexpr int REG_NUM_XMM25 = 25;
inline constexpr int REG_NUM_XMM26 = 26;
inline constexpr int REG_NUM_XMM27 = 27;
inline constexpr int REG_NUM_XMM28 = 28;
inline constexpr int REG_NUM_XMM29 = 29;
inline constexpr int REG_NUM_XMM3 = 3;
inline constexpr int REG_NUM_XMM30 = 30;
inline constexpr int REG_NUM_XMM31 = 31;
inline constexpr int REG_NUM_XMM4 = 4;
inline constexpr int REG_NUM_XMM5 = 5;
inline constexpr int REG_NUM_XMM6 = 6;
inline constexpr int REG_NUM_XMM7 = 7;
inline constexpr int REG_NUM_XMM8 = 8;
inline constexpr int REG_NUM_XMM9 = 9;
inline constexpr int REG_NUM_YMM0 = 0;
inline constexpr int REG_NUM_YMM1 = 1;
inline constexpr int REG_NUM_YMM10 = 10;
inline constexpr int REG_NUM_YMM11 = 11;
inline constexpr int REG_NUM_YMM12 = 12;
inline constexpr int REG_NUM_YMM13 = 13;
inline constexpr int REG_NUM_YMM14 = 14;
inline constexpr int REG_NUM_YMM15 = 15;
inline constexpr int REG_NUM_YMM16 = 16;
inline constexpr int REG_NUM_YMM17 = 17;
inline constexpr int REG_NUM_YMM18 = 18;
inline constexpr int REG_NUM_YMM19 = 19;
inline constexpr int REG_NUM_YMM2 = 2;
inline constexpr int REG_NUM_YMM20 = 20;
inline constexpr int REG_NUM_YMM21 = 21;
inline constexpr int REG_NUM_YMM22 = 22;
inline constexpr int REG_NUM_YMM23 = 23;
inline constexpr int REG_NUM_YMM24 = 24;
inline constexpr int REG_NUM_YMM25 = 25;
inline constexpr int REG_NUM_YMM26 = 26;
inline constexpr int REG_NUM_YMM27 = 27;
inline constexpr int REG_NUM_YMM28 = 28;
inline constexpr int REG_NUM_YMM29 = 29;
inline constexpr int REG_NUM_YMM3 = 3;
inline constexpr int REG_NUM_YMM30 = 30;
inline constexpr int REG_NUM_YMM31 = 31;
inline constexpr int REG_NUM_YMM4 = 4;
inline constexpr int REG_NUM_YMM5 = 5;
inline constexpr int REG_NUM_YMM6 = 6;
inline constexpr int REG_NUM_YMM7 = 7;
inline constexpr int REG_NUM_YMM8 = 8;
inline constexpr int REG_NUM_YMM9 = 9;
inline constexpr int REG_NUM_ZMM0 = 0;
inline constexpr int REG_NUM_ZMM1 = 1;
inline constexpr int REG_NUM_ZMM10 = 10;
inline constexpr int REG_NUM_ZMM11 = 11;
inline constexpr int REG_NUM_ZMM12 = 12;
inline constexpr int REG_NUM_ZMM13 = 13;
inline constexpr int REG_NUM_ZMM14 = 14;
inline constexpr int REG_NUM_ZMM15 = 15;
inline constexpr int REG_NUM_ZMM16 = 16;
inline constexpr int REG_NUM_ZMM17 = 17;
inline constexpr int REG_NUM_ZMM18 = 18;
inline constexpr int REG_NUM_ZMM19 = 19;
inline constexpr int REG_NUM_ZMM2 = 2;
inline constexpr int REG_NUM_ZMM20 = 20;
inline constexpr int REG_NUM_ZMM21 = 21;
inline constexpr int REG_NUM_ZMM22 = 22;
inline constexpr int REG_NUM_ZMM23 = 23;
inline constexpr int REG_NUM_ZMM24 = 24;
inline constexpr int REG_NUM_ZMM25 = 25;
inline constexpr int REG_NUM_ZMM26 = 26;
inline constexpr int REG_NUM_ZMM27 = 27;
inline constexpr int REG_NUM_ZMM28 = 28;
inline constexpr int REG_NUM_ZMM29 = 29;
inline constexpr int REG_NUM_ZMM3 = 3;
inline constexpr int REG_NUM_ZMM30 = 30;
inline constexpr int REG_NUM_ZMM31 = 31;
inline constexpr int REG_NUM_ZMM4 = 4;
inline constexpr int REG_NUM_ZMM5 = 5;
inline constexpr int REG_NUM_ZMM6 = 6;
inline constexpr int REG_NUM_ZMM7 = 7;
inline constexpr int REG_NUM_ZMM8 = 8;
inline constexpr int REG_NUM_ZMM9 = 9;

}  // namespace cb::build::x86asm
