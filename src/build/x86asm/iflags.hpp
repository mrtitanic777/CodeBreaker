// build/x86asm/iflags.hpp -- the instruction flag set: NASM's include/iflag.h over the generated bit numbers.
//
// An instruction template carries a set of IF_* flags (operand-size matching, CPU level, features, prefixes). The
// set is seven 32-bit words; the CPU level lives in the last word, one bit per level, "at least this level" being
// "the highest set bit is not below it". The bit numbers and masks below are generated from insns.dat (NASM's
// iflaggen.h); regenerate rather than edit.
#pragma once

#include <array>
#include <cstdint>

namespace cb::build::x86asm {

// Bit numbers (IF_*): field = bit >> 5, bit within the field = bit & 31.
inline constexpr unsigned IF_SM0 = 0;
inline constexpr unsigned IF_SM1 = 1;
inline constexpr unsigned IF_SM2 = 2;
inline constexpr unsigned IF_SM3 = 3;
inline constexpr unsigned IF_SM4 = 4;
inline constexpr unsigned IF_AR0 = 5;
inline constexpr unsigned IF_AR1 = 6;
inline constexpr unsigned IF_AR2 = 7;
inline constexpr unsigned IF_AR3 = 8;
inline constexpr unsigned IF_AR4 = 9;
inline constexpr unsigned IF_SB = 10;
inline constexpr unsigned IF_SW = 11;
inline constexpr unsigned IF_SD = 12;
inline constexpr unsigned IF_SQ = 13;
inline constexpr unsigned IF_ST = 14;
inline constexpr unsigned IF_SO = 15;
inline constexpr unsigned IF_SY = 16;
inline constexpr unsigned IF_SZ = 17;
inline constexpr unsigned IF_NWSIZE = 18;
inline constexpr unsigned IF_OSIZE = 19;
inline constexpr unsigned IF_ASIZE = 20;
inline constexpr unsigned IF_ANYSIZE = 21;
inline constexpr unsigned IF_SX = 22;
inline constexpr unsigned IF_SDWORD = 23;
inline constexpr unsigned IF_PSEUDO = 24;
inline constexpr unsigned IF_JMP_RELAX = 25;
inline constexpr unsigned IF_JCC_HINT = 26;
inline constexpr unsigned IF_OPT = 27;
inline constexpr unsigned IF_LATEVEX = 28;
inline constexpr unsigned IF_NOREX = 29;
inline constexpr unsigned IF_NOAPX = 30;
inline constexpr unsigned IF_NF = 31;
inline constexpr unsigned IF_NF_R = 32;
inline constexpr unsigned IF_NF_N = 33;
inline constexpr unsigned IF_NF_E = 34;
inline constexpr unsigned IF_ZU = 35;
inline constexpr unsigned IF_ZU_R = 36;
inline constexpr unsigned IF_ZU_E = 37;
inline constexpr unsigned IF_LIG = 38;
inline constexpr unsigned IF_WIG = 39;
inline constexpr unsigned IF_WW = 40;
inline constexpr unsigned IF_SIB = 41;
inline constexpr unsigned IF_LOCK = 42;
inline constexpr unsigned IF_LOCK1 = 43;
inline constexpr unsigned IF_NOLONG = 44;
inline constexpr unsigned IF_LONG = 45;
inline constexpr unsigned IF_NOHLE = 46;
inline constexpr unsigned IF_MIB = 47;
inline constexpr unsigned IF_BND = 48;
inline constexpr unsigned IF_REX2 = 49;
inline constexpr unsigned IF_HLE = 50;
inline constexpr unsigned IF_FL = 51;
inline constexpr unsigned IF_MOPVEC = 52;
inline constexpr unsigned IF_SCC = 53;
inline constexpr unsigned IF_BESTDIS = 54;
inline constexpr unsigned IF_DFV = 55;
inline constexpr unsigned IF_VEX = 64;
inline constexpr unsigned IF_EVEX = 65;
inline constexpr unsigned IF_PRIV = 66;
inline constexpr unsigned IF_SMM = 67;
inline constexpr unsigned IF_PROT = 68;
inline constexpr unsigned IF_UNDOC = 69;
inline constexpr unsigned IF_FPU = 70;
inline constexpr unsigned IF_MMX = 71;
inline constexpr unsigned IF_3DNOW = 72;
inline constexpr unsigned IF_SSE = 73;
inline constexpr unsigned IF_SSE2 = 74;
inline constexpr unsigned IF_SSE3 = 75;
inline constexpr unsigned IF_VMX = 76;
inline constexpr unsigned IF_SSSE3 = 77;
inline constexpr unsigned IF_SSE4A = 78;
inline constexpr unsigned IF_SSE41 = 79;
inline constexpr unsigned IF_SSE42 = 80;
inline constexpr unsigned IF_SSE5 = 81;
inline constexpr unsigned IF_AVX = 82;
inline constexpr unsigned IF_AVX2 = 83;
inline constexpr unsigned IF_FMA = 84;
inline constexpr unsigned IF_BMI1 = 85;
inline constexpr unsigned IF_BMI2 = 86;
inline constexpr unsigned IF_TBM = 87;
inline constexpr unsigned IF_RTM = 88;
inline constexpr unsigned IF_AVX512 = 89;
inline constexpr unsigned IF_AVX512F = 90;
inline constexpr unsigned IF_AVX512CD = 91;
inline constexpr unsigned IF_AVX512ER = 92;
inline constexpr unsigned IF_AVX512PF = 93;
inline constexpr unsigned IF_MPX = 94;
inline constexpr unsigned IF_SHA = 95;
inline constexpr unsigned IF_AVX512VL = 96;
inline constexpr unsigned IF_AVX512DQ = 97;
inline constexpr unsigned IF_AVX512BW = 98;
inline constexpr unsigned IF_AVX512IFMA = 99;
inline constexpr unsigned IF_AVX512VBMI = 100;
inline constexpr unsigned IF_AES = 101;
inline constexpr unsigned IF_VAES = 102;
inline constexpr unsigned IF_VPCLMULQDQ = 103;
inline constexpr unsigned IF_GFNI = 104;
inline constexpr unsigned IF_AVX512VBMI2 = 105;
inline constexpr unsigned IF_AVX512VNNI = 106;
inline constexpr unsigned IF_AVX512BITALG = 107;
inline constexpr unsigned IF_AVX512VPOPCNTDQ = 108;
inline constexpr unsigned IF_AVX5124FMAPS = 109;
inline constexpr unsigned IF_AVX5124VNNIW = 110;
inline constexpr unsigned IF_AVX512FP16 = 111;
inline constexpr unsigned IF_AVX512BMM = 112;
inline constexpr unsigned IF_F16C = 113;
inline constexpr unsigned IF_SGX = 114;
inline constexpr unsigned IF_CET = 115;
inline constexpr unsigned IF_ENQCMD = 116;
inline constexpr unsigned IF_TSXLDTRK = 117;
inline constexpr unsigned IF_AVX512BF16 = 118;
inline constexpr unsigned IF_AVX512VP2INTERSECT = 119;
inline constexpr unsigned IF_AMXTILE = 120;
inline constexpr unsigned IF_AMXBF16 = 121;
inline constexpr unsigned IF_AMXFP16 = 122;
inline constexpr unsigned IF_AMXFP8 = 123;
inline constexpr unsigned IF_AMXTF32 = 124;
inline constexpr unsigned IF_AMXINT8 = 125;
inline constexpr unsigned IF_AMXCOMPLEX = 126;
inline constexpr unsigned IF_AMXAVX512 = 127;
inline constexpr unsigned IF_AMXMOVRS = 128;
inline constexpr unsigned IF_AMXTRANSPOSE = 129;
inline constexpr unsigned IF_FRED = 130;
inline constexpr unsigned IF_RAOINT = 131;
inline constexpr unsigned IF_UINTR = 132;
inline constexpr unsigned IF_CMPCCXADD = 133;
inline constexpr unsigned IF_PREFETCHI = 134;
inline constexpr unsigned IF_MSRLIST = 135;
inline constexpr unsigned IF_AVXNECONVERT = 136;
inline constexpr unsigned IF_AVXVNNI = 137;
inline constexpr unsigned IF_AVXVNNIINT8 = 138;
inline constexpr unsigned IF_AVXVNNIINT16 = 139;
inline constexpr unsigned IF_AVXIFMA = 140;
inline constexpr unsigned IF_HRESET = 141;
inline constexpr unsigned IF_SMAP = 142;
inline constexpr unsigned IF_SHA512 = 143;
inline constexpr unsigned IF_HSM3 = 144;
inline constexpr unsigned IF_HSM4 = 145;
inline constexpr unsigned IF_APX = 146;
inline constexpr unsigned IF_AVX10_1 = 147;
inline constexpr unsigned IF_AVX10_2 = 148;
inline constexpr unsigned IF_AVX10_VNNIINT = 149;
inline constexpr unsigned IF_ADX = 150;
inline constexpr unsigned IF_PKU = 151;
inline constexpr unsigned IF_MONITOR = 152;
inline constexpr unsigned IF_MONITORX = 153;
inline constexpr unsigned IF_WAITPKG = 154;
inline constexpr unsigned IF_MSR_IMM = 155;
inline constexpr unsigned IF_AESKLE = 156;
inline constexpr unsigned IF_AESKLEWIDE_KL = 157;
inline constexpr unsigned IF_INVPCID = 158;
inline constexpr unsigned IF_PREFETCHWT1 = 159;
inline constexpr unsigned IF_PBNDKB = 160;
inline constexpr unsigned IF_PCONFIG = 161;
inline constexpr unsigned IF_WBNOINVD = 162;
inline constexpr unsigned IF_SERIALIZE = 163;
inline constexpr unsigned IF_LKGS = 164;
inline constexpr unsigned IF_WRMSRNS = 165;
inline constexpr unsigned IF_CLFLUSHOPT = 166;
inline constexpr unsigned IF_CLWB = 167;
inline constexpr unsigned IF_RDRAND = 168;
inline constexpr unsigned IF_RDSEED = 169;
inline constexpr unsigned IF_RDPID = 170;
inline constexpr unsigned IF_LZCNT = 171;
inline constexpr unsigned IF_PTWRITE = 172;
inline constexpr unsigned IF_CLDEMOTE = 173;
inline constexpr unsigned IF_MOVDIRI = 174;
inline constexpr unsigned IF_MOVDIR64B = 175;
inline constexpr unsigned IF_CLZERO = 176;
inline constexpr unsigned IF_MOVBE = 177;
inline constexpr unsigned IF_MOVRS = 178;
inline constexpr unsigned IF_OBSOLETE = 179;
inline constexpr unsigned IF_NEVER = 180;
inline constexpr unsigned IF_NOP = 181;
inline constexpr unsigned IF_8086 = 192;
inline constexpr unsigned IF_186 = 193;
inline constexpr unsigned IF_286 = 194;
inline constexpr unsigned IF_386 = 195;
inline constexpr unsigned IF_486 = 196;
inline constexpr unsigned IF_PENT = 197;
inline constexpr unsigned IF_P6 = 198;
inline constexpr unsigned IF_KATMAI = 199;
inline constexpr unsigned IF_WILLAMETTE = 200;
inline constexpr unsigned IF_PRESCOTT = 201;
inline constexpr unsigned IF_IA64 = 202;
inline constexpr unsigned IF_X86_64 = 203;
inline constexpr unsigned IF_NEHALEM = 204;
inline constexpr unsigned IF_WESTMERE = 205;
inline constexpr unsigned IF_SANDYBRIDGE = 206;
inline constexpr unsigned IF_FUTURE = 207;
inline constexpr unsigned IF_DEFAULT = 208;
inline constexpr unsigned IF_ANY = 209;
inline constexpr unsigned IF_CYRIX = 210;
inline constexpr unsigned IF_AMD = 211;

// Masks within a field (IFM_*).
inline constexpr std::uint32_t IFM_SM0 = 0x00000001u;
inline constexpr std::uint32_t IFM_SM1 = 0x00000002u;
inline constexpr std::uint32_t IFM_SM2 = 0x00000004u;
inline constexpr std::uint32_t IFM_SM3 = 0x00000008u;
inline constexpr std::uint32_t IFM_SM4 = 0x00000010u;
inline constexpr std::uint32_t IFM_AR0 = 0x00000020u;
inline constexpr std::uint32_t IFM_AR1 = 0x00000040u;
inline constexpr std::uint32_t IFM_AR2 = 0x00000080u;
inline constexpr std::uint32_t IFM_AR3 = 0x00000100u;
inline constexpr std::uint32_t IFM_AR4 = 0x00000200u;
inline constexpr std::uint32_t IFM_SB = 0x00000400u;
inline constexpr std::uint32_t IFM_SW = 0x00000800u;
inline constexpr std::uint32_t IFM_SD = 0x00001000u;
inline constexpr std::uint32_t IFM_SQ = 0x00002000u;
inline constexpr std::uint32_t IFM_ST = 0x00004000u;
inline constexpr std::uint32_t IFM_SO = 0x00008000u;
inline constexpr std::uint32_t IFM_SY = 0x00010000u;
inline constexpr std::uint32_t IFM_SZ = 0x00020000u;
inline constexpr std::uint32_t IFM_NWSIZE = 0x00040000u;
inline constexpr std::uint32_t IFM_OSIZE = 0x00080000u;
inline constexpr std::uint32_t IFM_ASIZE = 0x00100000u;
inline constexpr std::uint32_t IFM_ANYSIZE = 0x00200000u;
inline constexpr std::uint32_t IFM_SX = 0x00400000u;
inline constexpr std::uint32_t IFM_SDWORD = 0x00800000u;
inline constexpr std::uint32_t IFM_PSEUDO = 0x01000000u;
inline constexpr std::uint32_t IFM_JMP_RELAX = 0x02000000u;
inline constexpr std::uint32_t IFM_JCC_HINT = 0x04000000u;
inline constexpr std::uint32_t IFM_OPT = 0x08000000u;
inline constexpr std::uint32_t IFM_LATEVEX = 0x10000000u;
inline constexpr std::uint32_t IFM_NOREX = 0x20000000u;
inline constexpr std::uint32_t IFM_NOAPX = 0x40000000u;
inline constexpr std::uint32_t IFM_NF = 0x80000000u;
inline constexpr std::uint32_t IFM_NF_R = 0x00000001u;
inline constexpr std::uint32_t IFM_NF_N = 0x00000002u;
inline constexpr std::uint32_t IFM_NF_E = 0x00000004u;
inline constexpr std::uint32_t IFM_ZU = 0x00000008u;
inline constexpr std::uint32_t IFM_ZU_R = 0x00000010u;
inline constexpr std::uint32_t IFM_ZU_E = 0x00000020u;
inline constexpr std::uint32_t IFM_LIG = 0x00000040u;
inline constexpr std::uint32_t IFM_WIG = 0x00000080u;
inline constexpr std::uint32_t IFM_WW = 0x00000100u;
inline constexpr std::uint32_t IFM_SIB = 0x00000200u;
inline constexpr std::uint32_t IFM_LOCK = 0x00000400u;
inline constexpr std::uint32_t IFM_LOCK1 = 0x00000800u;
inline constexpr std::uint32_t IFM_NOLONG = 0x00001000u;
inline constexpr std::uint32_t IFM_LONG = 0x00002000u;
inline constexpr std::uint32_t IFM_NOHLE = 0x00004000u;
inline constexpr std::uint32_t IFM_MIB = 0x00008000u;
inline constexpr std::uint32_t IFM_BND = 0x00010000u;
inline constexpr std::uint32_t IFM_REX2 = 0x00020000u;
inline constexpr std::uint32_t IFM_HLE = 0x00040000u;
inline constexpr std::uint32_t IFM_FL = 0x00080000u;
inline constexpr std::uint32_t IFM_MOPVEC = 0x00100000u;
inline constexpr std::uint32_t IFM_SCC = 0x00200000u;
inline constexpr std::uint32_t IFM_BESTDIS = 0x00400000u;
inline constexpr std::uint32_t IFM_DFV = 0x00800000u;
inline constexpr std::uint32_t IFM_VEX = 0x00000001u;
inline constexpr std::uint32_t IFM_EVEX = 0x00000002u;
inline constexpr std::uint32_t IFM_PRIV = 0x00000004u;
inline constexpr std::uint32_t IFM_SMM = 0x00000008u;
inline constexpr std::uint32_t IFM_PROT = 0x00000010u;
inline constexpr std::uint32_t IFM_UNDOC = 0x00000020u;
inline constexpr std::uint32_t IFM_FPU = 0x00000040u;
inline constexpr std::uint32_t IFM_MMX = 0x00000080u;
inline constexpr std::uint32_t IFM_3DNOW = 0x00000100u;
inline constexpr std::uint32_t IFM_SSE = 0x00000200u;
inline constexpr std::uint32_t IFM_SSE2 = 0x00000400u;
inline constexpr std::uint32_t IFM_SSE3 = 0x00000800u;
inline constexpr std::uint32_t IFM_VMX = 0x00001000u;
inline constexpr std::uint32_t IFM_SSSE3 = 0x00002000u;
inline constexpr std::uint32_t IFM_SSE4A = 0x00004000u;
inline constexpr std::uint32_t IFM_SSE41 = 0x00008000u;
inline constexpr std::uint32_t IFM_SSE42 = 0x00010000u;
inline constexpr std::uint32_t IFM_SSE5 = 0x00020000u;
inline constexpr std::uint32_t IFM_AVX = 0x00040000u;
inline constexpr std::uint32_t IFM_AVX2 = 0x00080000u;
inline constexpr std::uint32_t IFM_FMA = 0x00100000u;
inline constexpr std::uint32_t IFM_BMI1 = 0x00200000u;
inline constexpr std::uint32_t IFM_BMI2 = 0x00400000u;
inline constexpr std::uint32_t IFM_TBM = 0x00800000u;
inline constexpr std::uint32_t IFM_RTM = 0x01000000u;
inline constexpr std::uint32_t IFM_AVX512 = 0x02000000u;
inline constexpr std::uint32_t IFM_AVX512F = 0x04000000u;
inline constexpr std::uint32_t IFM_AVX512CD = 0x08000000u;
inline constexpr std::uint32_t IFM_AVX512ER = 0x10000000u;
inline constexpr std::uint32_t IFM_AVX512PF = 0x20000000u;
inline constexpr std::uint32_t IFM_MPX = 0x40000000u;
inline constexpr std::uint32_t IFM_SHA = 0x80000000u;
inline constexpr std::uint32_t IFM_AVX512VL = 0x00000001u;
inline constexpr std::uint32_t IFM_AVX512DQ = 0x00000002u;
inline constexpr std::uint32_t IFM_AVX512BW = 0x00000004u;
inline constexpr std::uint32_t IFM_AVX512IFMA = 0x00000008u;
inline constexpr std::uint32_t IFM_AVX512VBMI = 0x00000010u;
inline constexpr std::uint32_t IFM_AES = 0x00000020u;
inline constexpr std::uint32_t IFM_VAES = 0x00000040u;
inline constexpr std::uint32_t IFM_VPCLMULQDQ = 0x00000080u;
inline constexpr std::uint32_t IFM_GFNI = 0x00000100u;
inline constexpr std::uint32_t IFM_AVX512VBMI2 = 0x00000200u;
inline constexpr std::uint32_t IFM_AVX512VNNI = 0x00000400u;
inline constexpr std::uint32_t IFM_AVX512BITALG = 0x00000800u;
inline constexpr std::uint32_t IFM_AVX512VPOPCNTDQ = 0x00001000u;
inline constexpr std::uint32_t IFM_AVX5124FMAPS = 0x00002000u;
inline constexpr std::uint32_t IFM_AVX5124VNNIW = 0x00004000u;
inline constexpr std::uint32_t IFM_AVX512FP16 = 0x00008000u;
inline constexpr std::uint32_t IFM_AVX512BMM = 0x00010000u;
inline constexpr std::uint32_t IFM_F16C = 0x00020000u;
inline constexpr std::uint32_t IFM_SGX = 0x00040000u;
inline constexpr std::uint32_t IFM_CET = 0x00080000u;
inline constexpr std::uint32_t IFM_ENQCMD = 0x00100000u;
inline constexpr std::uint32_t IFM_TSXLDTRK = 0x00200000u;
inline constexpr std::uint32_t IFM_AVX512BF16 = 0x00400000u;
inline constexpr std::uint32_t IFM_AVX512VP2INTERSECT = 0x00800000u;
inline constexpr std::uint32_t IFM_AMXTILE = 0x01000000u;
inline constexpr std::uint32_t IFM_AMXBF16 = 0x02000000u;
inline constexpr std::uint32_t IFM_AMXFP16 = 0x04000000u;
inline constexpr std::uint32_t IFM_AMXFP8 = 0x08000000u;
inline constexpr std::uint32_t IFM_AMXTF32 = 0x10000000u;
inline constexpr std::uint32_t IFM_AMXINT8 = 0x20000000u;
inline constexpr std::uint32_t IFM_AMXCOMPLEX = 0x40000000u;
inline constexpr std::uint32_t IFM_AMXAVX512 = 0x80000000u;
inline constexpr std::uint32_t IFM_AMXMOVRS = 0x00000001u;
inline constexpr std::uint32_t IFM_AMXTRANSPOSE = 0x00000002u;
inline constexpr std::uint32_t IFM_FRED = 0x00000004u;
inline constexpr std::uint32_t IFM_RAOINT = 0x00000008u;
inline constexpr std::uint32_t IFM_UINTR = 0x00000010u;
inline constexpr std::uint32_t IFM_CMPCCXADD = 0x00000020u;
inline constexpr std::uint32_t IFM_PREFETCHI = 0x00000040u;
inline constexpr std::uint32_t IFM_MSRLIST = 0x00000080u;
inline constexpr std::uint32_t IFM_AVXNECONVERT = 0x00000100u;
inline constexpr std::uint32_t IFM_AVXVNNI = 0x00000200u;
inline constexpr std::uint32_t IFM_AVXVNNIINT8 = 0x00000400u;
inline constexpr std::uint32_t IFM_AVXVNNIINT16 = 0x00000800u;
inline constexpr std::uint32_t IFM_AVXIFMA = 0x00001000u;
inline constexpr std::uint32_t IFM_HRESET = 0x00002000u;
inline constexpr std::uint32_t IFM_SMAP = 0x00004000u;
inline constexpr std::uint32_t IFM_SHA512 = 0x00008000u;
inline constexpr std::uint32_t IFM_HSM3 = 0x00010000u;
inline constexpr std::uint32_t IFM_HSM4 = 0x00020000u;
inline constexpr std::uint32_t IFM_APX = 0x00040000u;
inline constexpr std::uint32_t IFM_AVX10_1 = 0x00080000u;
inline constexpr std::uint32_t IFM_AVX10_2 = 0x00100000u;
inline constexpr std::uint32_t IFM_AVX10_VNNIINT = 0x00200000u;
inline constexpr std::uint32_t IFM_ADX = 0x00400000u;
inline constexpr std::uint32_t IFM_PKU = 0x00800000u;
inline constexpr std::uint32_t IFM_MONITOR = 0x01000000u;
inline constexpr std::uint32_t IFM_MONITORX = 0x02000000u;
inline constexpr std::uint32_t IFM_WAITPKG = 0x04000000u;
inline constexpr std::uint32_t IFM_MSR_IMM = 0x08000000u;
inline constexpr std::uint32_t IFM_AESKLE = 0x10000000u;
inline constexpr std::uint32_t IFM_AESKLEWIDE_KL = 0x20000000u;
inline constexpr std::uint32_t IFM_INVPCID = 0x40000000u;
inline constexpr std::uint32_t IFM_PREFETCHWT1 = 0x80000000u;
inline constexpr std::uint32_t IFM_PBNDKB = 0x00000001u;
inline constexpr std::uint32_t IFM_PCONFIG = 0x00000002u;
inline constexpr std::uint32_t IFM_WBNOINVD = 0x00000004u;
inline constexpr std::uint32_t IFM_SERIALIZE = 0x00000008u;
inline constexpr std::uint32_t IFM_LKGS = 0x00000010u;
inline constexpr std::uint32_t IFM_WRMSRNS = 0x00000020u;
inline constexpr std::uint32_t IFM_CLFLUSHOPT = 0x00000040u;
inline constexpr std::uint32_t IFM_CLWB = 0x00000080u;
inline constexpr std::uint32_t IFM_RDRAND = 0x00000100u;
inline constexpr std::uint32_t IFM_RDSEED = 0x00000200u;
inline constexpr std::uint32_t IFM_RDPID = 0x00000400u;
inline constexpr std::uint32_t IFM_LZCNT = 0x00000800u;
inline constexpr std::uint32_t IFM_PTWRITE = 0x00001000u;
inline constexpr std::uint32_t IFM_CLDEMOTE = 0x00002000u;
inline constexpr std::uint32_t IFM_MOVDIRI = 0x00004000u;
inline constexpr std::uint32_t IFM_MOVDIR64B = 0x00008000u;
inline constexpr std::uint32_t IFM_CLZERO = 0x00010000u;
inline constexpr std::uint32_t IFM_MOVBE = 0x00020000u;
inline constexpr std::uint32_t IFM_MOVRS = 0x00040000u;
inline constexpr std::uint32_t IFM_OBSOLETE = 0x00080000u;
inline constexpr std::uint32_t IFM_NEVER = 0x00100000u;
inline constexpr std::uint32_t IFM_NOP = 0x00200000u;
inline constexpr std::uint32_t IFM_8086 = 0x00000001u;
inline constexpr std::uint32_t IFM_186 = 0x00000002u;
inline constexpr std::uint32_t IFM_286 = 0x00000004u;
inline constexpr std::uint32_t IFM_386 = 0x00000008u;
inline constexpr std::uint32_t IFM_486 = 0x00000010u;
inline constexpr std::uint32_t IFM_PENT = 0x00000020u;
inline constexpr std::uint32_t IFM_P6 = 0x00000040u;
inline constexpr std::uint32_t IFM_KATMAI = 0x00000080u;
inline constexpr std::uint32_t IFM_WILLAMETTE = 0x00000100u;
inline constexpr std::uint32_t IFM_PRESCOTT = 0x00000200u;
inline constexpr std::uint32_t IFM_IA64 = 0x00000400u;
inline constexpr std::uint32_t IFM_X86_64 = 0x00000800u;
inline constexpr std::uint32_t IFM_NEHALEM = 0x00001000u;
inline constexpr std::uint32_t IFM_WESTMERE = 0x00002000u;
inline constexpr std::uint32_t IFM_SANDYBRIDGE = 0x00004000u;
inline constexpr std::uint32_t IFM_FUTURE = 0x00008000u;
inline constexpr std::uint32_t IFM_DEFAULT = 0x00010000u;
inline constexpr std::uint32_t IFM_ANY = 0x00020000u;
inline constexpr std::uint32_t IFM_CYRIX = 0x00040000u;
inline constexpr std::uint32_t IFM_AMD = 0x00080000u;

inline constexpr unsigned IF_IGEN_FIRST = 0;
inline constexpr unsigned IF_IGEN_COUNT = 56;
inline constexpr unsigned IF_IGEN_FIELD = 0;
inline constexpr unsigned IF_IGEN_NFIELDS = 2;
inline constexpr unsigned IF_FEATURE_FIRST = 64;
inline constexpr unsigned IF_FEATURE_COUNT = 118;
inline constexpr unsigned IF_FEATURE_FIELD = 2;
inline constexpr unsigned IF_FEATURE_NFIELDS = 4;
inline constexpr unsigned IF_CPU_FIRST = 192;
inline constexpr unsigned IF_CPU_COUNT = 20;
inline constexpr unsigned IF_CPU_FIELD = 6;
inline constexpr unsigned IF_CPU_NFIELDS = 1;
inline constexpr unsigned IF_FIELD_COUNT = 7;

// The mask of bit `bit` within its 32-bit field.
constexpr std::uint32_t if_genbit(unsigned bit) { return std::uint32_t{1} << (bit & 31); }

// A set of instruction flags (NASM's iflag_t).
struct IFlags {
    std::array<std::uint32_t, IF_FIELD_COUNT> field{};

    bool test(unsigned bit) const { return (field[bit >> 5] & if_genbit(bit)) != 0; }
    void set(unsigned bit) { field[bit >> 5] |= if_genbit(bit); }
    void clear(unsigned bit) { field[bit >> 5] &= ~if_genbit(bit); }
    void clear_all() { field.fill(0); }
    void set_all() { field.fill(~std::uint32_t{0}); }

    friend bool operator==(const IFlags&, const IFlags&) = default;

    // Order as NASM's iflag_cmp: the highest word first.
    friend int compare(const IFlags& a, const IFlags& b) {
        for (int i = static_cast<int>(IF_FIELD_COUNT) - 1; i >= 0; --i) {
            if (a.field[i] == b.field[i]) continue;
            return a.field[i] > b.field[i] ? 1 : -1;
        }
        return 0;
    }

    // Bitwise exclusive or, word by word (iflag_xor).
    friend IFlags operator^(const IFlags& a, const IFlags& b) {
        IFlags r;
        for (unsigned i = 0; i < IF_FIELD_COUNT; ++i) r.field[i] = a.field[i] ^ b.field[i];
        return r;
    }
};

// Helper masks over the size and operand-matching flags (TSMASK is the "true size" mask).
inline constexpr std::uint32_t IF_TSMASK = IFM_SB | IFM_SW | IFM_SD | IFM_SQ | IFM_ST | IFM_SO | IFM_SY | IFM_SZ;
inline constexpr std::uint32_t IF_SMASK = IF_TSMASK | IFM_NWSIZE | IFM_OSIZE | IFM_ASIZE | IFM_ANYSIZE | IFM_SX;
inline constexpr std::uint32_t IF_ARMASK = IFM_AR0 | IFM_AR1 | IFM_AR2 | IFM_AR3 | IFM_AR4;
inline constexpr std::uint32_t IF_SMMASK = IFM_SM0 | IFM_SM1 | IFM_SM2 | IFM_SM3 | IFM_SM4;

// IF_ANY is the highest CPU level by definition.
inline constexpr std::uint32_t IF_CPU_LEVEL_MASK = (IFM_ANY << 1) - 1;

// The CPU level part of a flag set: the CPU word, and its level bits.
inline std::uint32_t iflag_cpu_level(const IFlags& a) { return a.field[IF_CPU_FIELD] & IF_CPU_LEVEL_MASK; }

// Compare two sets' CPU words (iflag_cmp_cpu), and their CPU levels only (iflag_cmp_cpu_level).
inline int iflag_cmp_cpu(const IFlags& a, const IFlags& b) {
    return (a.field[IF_CPU_FIELD] > b.field[IF_CPU_FIELD]) - (a.field[IF_CPU_FIELD] < b.field[IF_CPU_FIELD]);
}
inline int iflag_cmp_cpu_level(const IFlags& a, const IFlags& b) {
    return (iflag_cpu_level(a) > iflag_cpu_level(b)) - (iflag_cpu_level(a) < iflag_cpu_level(b));
}

// True if the CPU level is at least level `bit` (an IF_8086..IF_ANY number).
inline bool iflag_cpu_level_ok(const IFlags& a, unsigned bit) { return iflag_cpu_level(a) >= if_genbit(bit); }

// Every feature flag set (iflag_set_all_features).
inline void iflag_set_all_features(IFlags& a) {
    for (unsigned i = 0; i < IF_FEATURE_NFIELDS; ++i) a.field[IF_FEATURE_FIELD + i] = ~std::uint32_t{0};
}

// The vendor-specific flags (CYRIX, AMD) of `a`, as a set of their own (iflag_pfmask).
inline IFlags iflag_pfmask(const IFlags& a) {
    IFlags r;
    if (a.test(IF_CYRIX)) r.set(IF_CYRIX);
    if (a.test(IF_AMD)) r.set(IF_AMD);
    return r;
}

}  // namespace cb::build::x86asm
