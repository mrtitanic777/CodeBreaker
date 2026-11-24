// cb-unity: alone (file-local names that other files in a batch also use)
// The VEX-encoded instructions of the x86 engine (AVX, AVX2, F16C, FMA, AES-NI and CLMUL in their VEX forms) against the host's own instructions through the compiler's intrinsics:
// Unicorn has no AVX (it takes the VEX prefix for none, and refuses 256 bits), so it is no judge of them. One instruction at a time, on random vector registers, in its register form
// and its memory form, 128 and 256 bits, the result compared bit for bit. Guarded: it runs where the compiler is MSVC (or builds for AVX2) and the processor has AVX2, FMA, F16C,
// AES and PCLMULQDQ; elsewhere it says so and passes.
#include <cstdio>
#include <cstring>
#include <format>
#include <map>
#include <random>
#include <string>

#include "core/testing.hpp"
#include "emulate/unicorn_uc.hpp"
#include "engine/pcode_cpu_x86.hpp"

#if defined(_MSC_VER) || defined(__AVX2__)
#include <immintrin.h>
#if defined(_MSC_VER)
#include <intrin.h>
#endif
#define CB_HOST_AVX 1
#endif

namespace cb {
namespace {

namespace pc = cb::engine::pcode_cpu;
namespace uu = cb::emulate::unicorn_uc;
using u64 = std::uint64_t;

#ifdef CB_HOST_AVX

bool host_has_avx() {
#if defined(_MSC_VER)
    int r[4];
    __cpuid(r, 1);
    const bool avx = (r[2] & (1 << 28)) != 0 && (r[2] & (1 << 27)) != 0 && (r[2] & (1 << 12)) != 0 && (r[2] & (1 << 29)) != 0 && (r[2] & (1 << 25)) != 0 && (r[2] & (1 << 1)) != 0;   // AVX, OSXSAVE, FMA, F16C, AES, PCLMUL
    if (!avx) return false;
    if ((_xgetbv(0) & 6) != 6) return false;
    __cpuidex(r, 7, 0);
    return (r[1] & (1 << 5)) != 0;   // AVX2
#else
    return __builtin_cpu_supports("avx2") && __builtin_cpu_supports("fma") && __builtin_cpu_supports("f16c") && __builtin_cpu_supports("aes") && __builtin_cpu_supports("pclmul");
#endif
}

using V = __m256i;
V ps(__m256 v) { return _mm256_castps_si256(v); }
V pd(__m256d v) { return _mm256_castpd_si256(v); }
__m256 fs(V v) { return _mm256_castsi256_ps(v); }
__m256d fd(V v) { return _mm256_castsi256_pd(v); }

// A host function: the registers as the instruction has them (x: the destination before, y: the first source (VEX.vvvv), z: the second source (ModRM), m: the mask of the 4-operand forms).
using Host = V (*)(V x, V y, V z, V m);

struct Case {
    const char* name;
    unsigned map, pp, opcode;
    bool has_imm;
    unsigned imm;
    unsigned form;   // 0: dst, vvvv, rm  1: dst, rm (no vvvv)  2: with a fourth register (is4)  3: FMA (dst is also a source)
    bool l128, l256;
    bool lane_local; // the result of the low 128 bits depends on nothing above
    bool wide;       // VEX.W
    Host host;
};

#define H(NAME, EXPR) V NAME(V x, V y, V z, V m) { (void)x; (void)y; (void)z; (void)m; return EXPR; }

// integer
H(h_paddb, _mm256_add_epi8(y, z)) H(h_paddw, _mm256_add_epi16(y, z)) H(h_paddd, _mm256_add_epi32(y, z)) H(h_paddq, _mm256_add_epi64(y, z))
H(h_psubb, _mm256_sub_epi8(y, z)) H(h_psubw, _mm256_sub_epi16(y, z)) H(h_psubd, _mm256_sub_epi32(y, z)) H(h_psubq, _mm256_sub_epi64(y, z))
H(h_paddsb, _mm256_adds_epi8(y, z)) H(h_paddsw, _mm256_adds_epi16(y, z)) H(h_paddusb, _mm256_adds_epu8(y, z)) H(h_paddusw, _mm256_adds_epu16(y, z))
H(h_psubsb, _mm256_subs_epi8(y, z)) H(h_psubsw, _mm256_subs_epi16(y, z)) H(h_psubusb, _mm256_subs_epu8(y, z)) H(h_psubusw, _mm256_subs_epu16(y, z))
H(h_pavgb, _mm256_avg_epu8(y, z)) H(h_pavgw, _mm256_avg_epu16(y, z))
H(h_pmulhw, _mm256_mulhi_epi16(y, z)) H(h_pmulhuw, _mm256_mulhi_epu16(y, z)) H(h_pmullw, _mm256_mullo_epi16(y, z)) H(h_pmulld, _mm256_mullo_epi32(y, z))
H(h_pmuldq, _mm256_mul_epi32(y, z)) H(h_pmuludq, _mm256_mul_epu32(y, z)) H(h_pmaddwd, _mm256_madd_epi16(y, z)) H(h_pmaddubsw, _mm256_maddubs_epi16(y, z))
H(h_psadbw, _mm256_sad_epu8(y, z)) H(h_pmulhrsw, _mm256_mulhrs_epi16(y, z))
H(h_pmaxsb, _mm256_max_epi8(y, z)) H(h_pmaxsw, _mm256_max_epi16(y, z)) H(h_pmaxsd, _mm256_max_epi32(y, z)) H(h_pmaxub, _mm256_max_epu8(y, z)) H(h_pmaxuw, _mm256_max_epu16(y, z)) H(h_pmaxud, _mm256_max_epu32(y, z))
H(h_pminsb, _mm256_min_epi8(y, z)) H(h_pminsw, _mm256_min_epi16(y, z)) H(h_pminsd, _mm256_min_epi32(y, z)) H(h_pminub, _mm256_min_epu8(y, z)) H(h_pminuw, _mm256_min_epu16(y, z)) H(h_pminud, _mm256_min_epu32(y, z))
H(h_pcmpeqb, _mm256_cmpeq_epi8(y, z)) H(h_pcmpeqw, _mm256_cmpeq_epi16(y, z)) H(h_pcmpeqd, _mm256_cmpeq_epi32(y, z)) H(h_pcmpeqq, _mm256_cmpeq_epi64(y, z))
H(h_pcmpgtb, _mm256_cmpgt_epi8(y, z)) H(h_pcmpgtw, _mm256_cmpgt_epi16(y, z)) H(h_pcmpgtd, _mm256_cmpgt_epi32(y, z)) H(h_pcmpgtq, _mm256_cmpgt_epi64(y, z))
H(h_pand, _mm256_and_si256(y, z)) H(h_pandn, _mm256_andnot_si256(y, z)) H(h_por, _mm256_or_si256(y, z)) H(h_pxor, _mm256_xor_si256(y, z))
H(h_pshufb, _mm256_shuffle_epi8(y, z))
H(h_packsswb, _mm256_packs_epi16(y, z)) H(h_packssdw, _mm256_packs_epi32(y, z)) H(h_packuswb, _mm256_packus_epi16(y, z)) H(h_packusdw, _mm256_packus_epi32(y, z))
H(h_punpcklbw, _mm256_unpacklo_epi8(y, z)) H(h_punpcklwd, _mm256_unpacklo_epi16(y, z)) H(h_punpckldq, _mm256_unpacklo_epi32(y, z)) H(h_punpcklqdq, _mm256_unpacklo_epi64(y, z))
H(h_punpckhbw, _mm256_unpackhi_epi8(y, z)) H(h_punpckhwd, _mm256_unpackhi_epi16(y, z)) H(h_punpckhdq, _mm256_unpackhi_epi32(y, z)) H(h_punpckhqdq, _mm256_unpackhi_epi64(y, z))
H(h_phaddw, _mm256_hadd_epi16(y, z)) H(h_phaddd, _mm256_hadd_epi32(y, z)) H(h_phaddsw, _mm256_hadds_epi16(y, z))
H(h_phsubw, _mm256_hsub_epi16(y, z)) H(h_phsubd, _mm256_hsub_epi32(y, z)) H(h_phsubsw, _mm256_hsubs_epi16(y, z))
H(h_psignb, _mm256_sign_epi8(y, z)) H(h_psignw, _mm256_sign_epi16(y, z)) H(h_psignd, _mm256_sign_epi32(y, z))
H(h_pabsb, _mm256_abs_epi8(z)) H(h_pabsw, _mm256_abs_epi16(z)) H(h_pabsd, _mm256_abs_epi32(z))
H(h_psllw, _mm256_sll_epi16(y, _mm256_castsi256_si128(z))) H(h_pslld, _mm256_sll_epi32(y, _mm256_castsi256_si128(z))) H(h_psllq, _mm256_sll_epi64(y, _mm256_castsi256_si128(z)))
H(h_psrlw, _mm256_srl_epi16(y, _mm256_castsi256_si128(z))) H(h_psrld, _mm256_srl_epi32(y, _mm256_castsi256_si128(z))) H(h_psrlq, _mm256_srl_epi64(y, _mm256_castsi256_si128(z)))
H(h_psraw, _mm256_sra_epi16(y, _mm256_castsi256_si128(z))) H(h_psrad, _mm256_sra_epi32(y, _mm256_castsi256_si128(z)))
H(h_psllvd, _mm256_sllv_epi32(y, z)) H(h_psrlvd, _mm256_srlv_epi32(y, z)) H(h_psravd, _mm256_srav_epi32(y, z)) H(h_psllvq, _mm256_sllv_epi64(y, z)) H(h_psrlvq, _mm256_srlv_epi64(y, z))
H(h_pmovsxbw, _mm256_cvtepi8_epi16(_mm256_castsi256_si128(z))) H(h_pmovsxbd, _mm256_cvtepi8_epi32(_mm256_castsi256_si128(z))) H(h_pmovsxbq, _mm256_cvtepi8_epi64(_mm256_castsi256_si128(z)))
H(h_pmovsxwd, _mm256_cvtepi16_epi32(_mm256_castsi256_si128(z))) H(h_pmovsxwq, _mm256_cvtepi16_epi64(_mm256_castsi256_si128(z))) H(h_pmovsxdq, _mm256_cvtepi32_epi64(_mm256_castsi256_si128(z)))
H(h_pmovzxbw, _mm256_cvtepu8_epi16(_mm256_castsi256_si128(z))) H(h_pmovzxbd, _mm256_cvtepu8_epi32(_mm256_castsi256_si128(z))) H(h_pmovzxbq, _mm256_cvtepu8_epi64(_mm256_castsi256_si128(z)))
H(h_pmovzxwd, _mm256_cvtepu16_epi32(_mm256_castsi256_si128(z))) H(h_pmovzxwq, _mm256_cvtepu16_epi64(_mm256_castsi256_si128(z))) H(h_pmovzxdq, _mm256_cvtepu32_epi64(_mm256_castsi256_si128(z)))
H(h_vpermd, _mm256_permutevar8x32_epi32(z, y)) H(h_vpermps, ps(_mm256_permutevar8x32_ps(fs(z), y)))
H(h_pblendvb, _mm256_blendv_epi8(y, z, m))
H(h_blendvps, ps(_mm256_blendv_ps(fs(y), fs(z), fs(m)))) H(h_blendvpd, pd(_mm256_blendv_pd(fd(y), fd(z), fd(m))))
H(h_vpermilps_var, ps(_mm256_permutevar_ps(fs(y), z))) H(h_vpermilpd_var, pd(_mm256_permutevar_pd(fd(y), z)))
// float
H(h_addps, ps(_mm256_add_ps(fs(y), fs(z)))) H(h_addpd, pd(_mm256_add_pd(fd(y), fd(z)))) H(h_subps, ps(_mm256_sub_ps(fs(y), fs(z)))) H(h_subpd, pd(_mm256_sub_pd(fd(y), fd(z))))
H(h_mulps, ps(_mm256_mul_ps(fs(y), fs(z)))) H(h_mulpd, pd(_mm256_mul_pd(fd(y), fd(z)))) H(h_divps, ps(_mm256_div_ps(fs(y), fs(z)))) H(h_divpd, pd(_mm256_div_pd(fd(y), fd(z))))
H(h_minps, ps(_mm256_min_ps(fs(y), fs(z)))) H(h_minpd, pd(_mm256_min_pd(fd(y), fd(z)))) H(h_maxps, ps(_mm256_max_ps(fs(y), fs(z)))) H(h_maxpd, pd(_mm256_max_pd(fd(y), fd(z))))
H(h_sqrtps, ps(_mm256_sqrt_ps(fs(z)))) H(h_sqrtpd, pd(_mm256_sqrt_pd(fd(z))))
H(h_andps, ps(_mm256_and_ps(fs(y), fs(z)))) H(h_andnps, ps(_mm256_andnot_ps(fs(y), fs(z)))) H(h_orps, ps(_mm256_or_ps(fs(y), fs(z)))) H(h_xorps, ps(_mm256_xor_ps(fs(y), fs(z))))
H(h_unpcklps, ps(_mm256_unpacklo_ps(fs(y), fs(z)))) H(h_unpckhps, ps(_mm256_unpackhi_ps(fs(y), fs(z)))) H(h_unpcklpd, pd(_mm256_unpacklo_pd(fd(y), fd(z)))) H(h_unpckhpd, pd(_mm256_unpackhi_pd(fd(y), fd(z))))
H(h_haddps, ps(_mm256_hadd_ps(fs(y), fs(z)))) H(h_haddpd, pd(_mm256_hadd_pd(fd(y), fd(z)))) H(h_hsubps, ps(_mm256_hsub_ps(fs(y), fs(z)))) H(h_hsubpd, pd(_mm256_hsub_pd(fd(y), fd(z))))
H(h_addsubps, ps(_mm256_addsub_ps(fs(y), fs(z)))) H(h_addsubpd, pd(_mm256_addsub_pd(fd(y), fd(z))))
H(h_cvtdq2ps, ps(_mm256_cvtepi32_ps(z))) H(h_cvtps2dq, _mm256_cvtps_epi32(fs(z))) H(h_cvttps2dq, _mm256_cvttps_epi32(fs(z)))
H(h_cvtps2pd, pd(_mm256_cvtps_pd(_mm256_castps256_ps128(fs(z))))) H(h_cvtdq2pd, pd(_mm256_cvtepi32_pd(_mm256_castsi256_si128(z))))
H(h_fmadd132ps, ps(_mm256_fmadd_ps(fs(x), fs(z), fs(y)))) H(h_fmadd213ps, ps(_mm256_fmadd_ps(fs(y), fs(x), fs(z)))) H(h_fmadd231ps, ps(_mm256_fmadd_ps(fs(y), fs(z), fs(x))))
H(h_fmsub231ps, ps(_mm256_fmsub_ps(fs(y), fs(z), fs(x)))) H(h_fnmadd231ps, ps(_mm256_fnmadd_ps(fs(y), fs(z), fs(x)))) H(h_fnmsub231ps, ps(_mm256_fnmsub_ps(fs(y), fs(z), fs(x))))
H(h_fmadd231pd, pd(_mm256_fmadd_pd(fd(y), fd(z), fd(x)))) H(h_fmaddsub231ps, ps(_mm256_fmaddsub_ps(fs(y), fs(z), fs(x)))) H(h_fmsubadd231ps, ps(_mm256_fmsubadd_ps(fs(y), fs(z), fs(x))))
H(h_aesenc, _mm256_aesenc_epi128(y, z))
H(h_movshdup, ps(_mm256_movehdup_ps(fs(z)))) H(h_movsldup, ps(_mm256_moveldup_ps(fs(z))))
H(h_cvtpd2ps, ps(_mm256_insertf128_ps(_mm256_setzero_ps(), _mm256_cvtpd_ps(fd(z)), 0)))
H(h_cvtpd2dq, _mm256_insertf128_si256(_mm256_setzero_si256(), _mm256_cvtpd_epi32(fd(z)), 0))
H(h_cvttpd2dq, _mm256_insertf128_si256(_mm256_setzero_si256(), _mm256_cvttpd_epi32(fd(z)), 0))
H(h_cvtph2ps, ps(_mm256_cvtph_ps(_mm256_castsi256_si128(z))))
// with an immediate (the immediate is a template argument of the intrinsic: one function for each value tested)
#define HI(NAME, EXPR) H(NAME, EXPR)
HI(h_palignr3, _mm256_alignr_epi8(y, z, 3)) HI(h_palignr12, _mm256_alignr_epi8(y, z, 12)) HI(h_palignr20, _mm256_alignr_epi8(y, z, 20))
HI(h_pblendw_a5, _mm256_blend_epi16(y, z, 0xA5)) HI(h_pblendw_3c, _mm256_blend_epi16(y, z, 0x3C)) HI(h_blendps_9, ps(_mm256_blend_ps(fs(y), fs(z), 0x9C))) HI(h_blendpd_6, pd(_mm256_blend_pd(fd(y), fd(z), 0x6)))
HI(h_pblendd_5a, _mm256_blend_epi32(y, z, 0x5A))
HI(h_pshufd_1b, _mm256_shuffle_epi32(z, 0x1B)) HI(h_pshufd_e4, _mm256_shuffle_epi32(z, 0xE4)) HI(h_pshuflw_4e, _mm256_shufflelo_epi16(z, 0x4E)) HI(h_pshufhw_b1, _mm256_shufflehi_epi16(z, 0xB1))
HI(h_shufps_44, ps(_mm256_shuffle_ps(fs(y), fs(z), 0x44))) HI(h_shufps_b1, ps(_mm256_shuffle_ps(fs(y), fs(z), 0xB1))) HI(h_shufpd_a, pd(_mm256_shuffle_pd(fd(y), fd(z), 0xA)))
HI(h_pslldq5, _mm256_slli_si256(z, 5)) HI(h_psrldq9, _mm256_srli_si256(z, 9))
HI(h_psllwi, _mm256_slli_epi16(z, 3)) HI(h_psrldi, _mm256_srli_epi32(z, 7)) HI(h_psradi, _mm256_srai_epi32(z, 31)) HI(h_psrlqi, _mm256_srli_epi64(z, 40)) HI(h_psrawi, _mm256_srai_epi16(z, 15))
HI(h_vpermq_1b, _mm256_permute4x64_epi64(z, 0x1B)) HI(h_vpermpd_c6, pd(_mm256_permute4x64_pd(fd(z), 0xC6))) HI(h_vperm2f128_20, ps(_mm256_permute2f128_ps(fs(y), fs(z), 0x20)))
HI(h_vperm2i128_31, _mm256_permute2x128_si256(y, z, 0x31)) HI(h_vperm2i128_8_2, _mm256_permute2x128_si256(y, z, 0x82))
HI(h_vpermilps_1b, ps(_mm256_permute_ps(fs(z), 0x1B))) HI(h_vpermilpd_5, pd(_mm256_permute_pd(fd(z), 0x5)))
HI(h_roundps_0, ps(_mm256_round_ps(fs(z), _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC))) HI(h_roundps_1, ps(_mm256_round_ps(fs(z), _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)))
HI(h_roundps_2, ps(_mm256_round_ps(fs(z), _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC))) HI(h_roundps_3, ps(_mm256_round_ps(fs(z), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)))
HI(h_roundpd_1, pd(_mm256_round_pd(fd(z), _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC))) HI(h_roundpd_3, pd(_mm256_round_pd(fd(z), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)))
HI(h_dpps_ff, ps(_mm256_dp_ps(fs(y), fs(z), 0xFF))) HI(h_dpps_71, ps(_mm256_dp_ps(fs(y), fs(z), 0x71))) HI(h_dpps_3a, ps(_mm256_dp_ps(fs(y), fs(z), 0x3A)))
HI(h_mpsadbw_0, _mm256_mpsadbw_epu8(y, z, 0)) HI(h_mpsadbw_5, _mm256_mpsadbw_epu8(y, z, 5)) HI(h_mpsadbw_7, _mm256_mpsadbw_epu8(y, z, 7))
HI(h_pclmul_00, _mm256_clmulepi64_epi128(y, z, 0x00)) HI(h_pclmul_11, _mm256_clmulepi64_epi128(y, z, 0x11)) HI(h_pclmul_10, _mm256_clmulepi64_epi128(y, z, 0x10))
#define CMP(N) HI(h_cmpps_##N, ps(_mm256_cmp_ps(fs(y), fs(z), N))) HI(h_cmppd_##N, pd(_mm256_cmp_pd(fd(y), fd(z), N)))
CMP(0) CMP(1) CMP(2) CMP(3) CMP(4) CMP(5) CMP(6) CMP(7) CMP(8) CMP(9) CMP(10) CMP(11) CMP(12) CMP(13) CMP(14) CMP(15)
CMP(16) CMP(17) CMP(18) CMP(19) CMP(20) CMP(21) CMP(22) CMP(23) CMP(24) CMP(25) CMP(26) CMP(27) CMP(28) CMP(29) CMP(30) CMP(31)

// l128 l256 lane_local wide
#define C(NAME, MAP, PP, OP, FORM, HOST) {NAME, MAP, PP, OP, false, 0, FORM, true, true, true, false, HOST}
#define CI(NAME, MAP, PP, OP, FORM, IMM, HOST) {NAME, MAP, PP, OP, true, IMM, FORM, true, true, true, false, HOST}
#define CX(NAME, MAP, PP, OP, FORM, IMM, HASIMM, L128, LANE, WIDE, HOST) {NAME, MAP, PP, OP, HASIMM, IMM, FORM, L128, true, LANE, WIDE, HOST}

const Case kCases[] = {
    C("vpaddb", 1, 1, 0xFC, 0, h_paddb), C("vpaddw", 1, 1, 0xFD, 0, h_paddw), C("vpaddd", 1, 1, 0xFE, 0, h_paddd), C("vpaddq", 1, 1, 0xD4, 0, h_paddq),
    C("vpsubb", 1, 1, 0xF8, 0, h_psubb), C("vpsubw", 1, 1, 0xF9, 0, h_psubw), C("vpsubd", 1, 1, 0xFA, 0, h_psubd), C("vpsubq", 1, 1, 0xFB, 0, h_psubq),
    C("vpaddsb", 1, 1, 0xEC, 0, h_paddsb), C("vpaddsw", 1, 1, 0xED, 0, h_paddsw), C("vpaddusb", 1, 1, 0xDC, 0, h_paddusb), C("vpaddusw", 1, 1, 0xDD, 0, h_paddusw),
    C("vpsubsb", 1, 1, 0xE8, 0, h_psubsb), C("vpsubsw", 1, 1, 0xE9, 0, h_psubsw), C("vpsubusb", 1, 1, 0xD8, 0, h_psubusb), C("vpsubusw", 1, 1, 0xD9, 0, h_psubusw),
    C("vpavgb", 1, 1, 0xE0, 0, h_pavgb), C("vpavgw", 1, 1, 0xE3, 0, h_pavgw),
    C("vpmulhw", 1, 1, 0xE5, 0, h_pmulhw), C("vpmulhuw", 1, 1, 0xE4, 0, h_pmulhuw), C("vpmullw", 1, 1, 0xD5, 0, h_pmullw), C("vpmulld", 2, 1, 0x40, 0, h_pmulld),
    C("vpmuldq", 2, 1, 0x28, 0, h_pmuldq), C("vpmuludq", 1, 1, 0xF4, 0, h_pmuludq), C("vpmaddwd", 1, 1, 0xF5, 0, h_pmaddwd), C("vpmaddubsw", 2, 1, 0x04, 0, h_pmaddubsw),
    C("vpsadbw", 1, 1, 0xF6, 0, h_psadbw), C("vpmulhrsw", 2, 1, 0x0B, 0, h_pmulhrsw),
    C("vpmaxsb", 2, 1, 0x3C, 0, h_pmaxsb), C("vpmaxsw", 1, 1, 0xEE, 0, h_pmaxsw), C("vpmaxsd", 2, 1, 0x3D, 0, h_pmaxsd), C("vpmaxub", 1, 1, 0xDE, 0, h_pmaxub), C("vpmaxuw", 2, 1, 0x3E, 0, h_pmaxuw), C("vpmaxud", 2, 1, 0x3F, 0, h_pmaxud),
    C("vpminsb", 2, 1, 0x38, 0, h_pminsb), C("vpminsw", 1, 1, 0xEA, 0, h_pminsw), C("vpminsd", 2, 1, 0x39, 0, h_pminsd), C("vpminub", 1, 1, 0xDA, 0, h_pminub), C("vpminuw", 2, 1, 0x3A, 0, h_pminuw), C("vpminud", 2, 1, 0x3B, 0, h_pminud),
    C("vpcmpeqb", 1, 1, 0x74, 0, h_pcmpeqb), C("vpcmpeqw", 1, 1, 0x75, 0, h_pcmpeqw), C("vpcmpeqd", 1, 1, 0x76, 0, h_pcmpeqd), C("vpcmpeqq", 2, 1, 0x29, 0, h_pcmpeqq),
    C("vpcmpgtb", 1, 1, 0x64, 0, h_pcmpgtb), C("vpcmpgtw", 1, 1, 0x65, 0, h_pcmpgtw), C("vpcmpgtd", 1, 1, 0x66, 0, h_pcmpgtd), C("vpcmpgtq", 2, 1, 0x37, 0, h_pcmpgtq),
    C("vpand", 1, 1, 0xDB, 0, h_pand), C("vpandn", 1, 1, 0xDF, 0, h_pandn), C("vpor", 1, 1, 0xEB, 0, h_por), C("vpxor", 1, 1, 0xEF, 0, h_pxor),
    C("vpshufb", 2, 1, 0x00, 0, h_pshufb),
    C("vpacksswb", 1, 1, 0x63, 0, h_packsswb), C("vpackssdw", 1, 1, 0x6B, 0, h_packssdw), C("vpackuswb", 1, 1, 0x67, 0, h_packuswb), C("vpackusdw", 2, 1, 0x2B, 0, h_packusdw),
    C("vpunpcklbw", 1, 1, 0x60, 0, h_punpcklbw), C("vpunpcklwd", 1, 1, 0x61, 0, h_punpcklwd), C("vpunpckldq", 1, 1, 0x62, 0, h_punpckldq), C("vpunpcklqdq", 1, 1, 0x6C, 0, h_punpcklqdq),
    C("vpunpckhbw", 1, 1, 0x68, 0, h_punpckhbw), C("vpunpckhwd", 1, 1, 0x69, 0, h_punpckhwd), C("vpunpckhdq", 1, 1, 0x6A, 0, h_punpckhdq), C("vpunpckhqdq", 1, 1, 0x6D, 0, h_punpckhqdq),
    C("vphaddw", 2, 1, 0x01, 0, h_phaddw), C("vphaddd", 2, 1, 0x02, 0, h_phaddd), C("vphaddsw", 2, 1, 0x03, 0, h_phaddsw),
    C("vphsubw", 2, 1, 0x05, 0, h_phsubw), C("vphsubd", 2, 1, 0x06, 0, h_phsubd), C("vphsubsw", 2, 1, 0x07, 0, h_phsubsw),
    C("vpsignb", 2, 1, 0x08, 0, h_psignb), C("vpsignw", 2, 1, 0x09, 0, h_psignw), C("vpsignd", 2, 1, 0x0A, 0, h_psignd),
    C("vpabsb", 2, 1, 0x1C, 1, h_pabsb), C("vpabsw", 2, 1, 0x1D, 1, h_pabsw), C("vpabsd", 2, 1, 0x1E, 1, h_pabsd),
    C("vpsllw", 1, 1, 0xF1, 0, h_psllw), C("vpslld", 1, 1, 0xF2, 0, h_pslld), C("vpsllq", 1, 1, 0xF3, 0, h_psllq),
    C("vpsrlw", 1, 1, 0xD1, 0, h_psrlw), C("vpsrld", 1, 1, 0xD2, 0, h_psrld), C("vpsrlq", 1, 1, 0xD3, 0, h_psrlq), C("vpsraw", 1, 1, 0xE1, 0, h_psraw), C("vpsrad", 1, 1, 0xE2, 0, h_psrad),
    C("vpsllvd", 2, 1, 0x47, 0, h_psllvd), C("vpsrlvd", 2, 1, 0x45, 0, h_psrlvd), C("vpsravd", 2, 1, 0x46, 0, h_psravd),
    CX("vpsllvq", 2, 1, 0x47, 0, 0, false, true, true, true, h_psllvq), CX("vpsrlvq", 2, 1, 0x45, 0, 0, false, true, true, true, h_psrlvq),
    C("vpmovsxbw", 2, 1, 0x20, 1, h_pmovsxbw), C("vpmovsxbd", 2, 1, 0x21, 1, h_pmovsxbd), C("vpmovsxbq", 2, 1, 0x22, 1, h_pmovsxbq), C("vpmovsxwd", 2, 1, 0x23, 1, h_pmovsxwd), C("vpmovsxwq", 2, 1, 0x24, 1, h_pmovsxwq), C("vpmovsxdq", 2, 1, 0x25, 1, h_pmovsxdq),
    C("vpmovzxbw", 2, 1, 0x30, 1, h_pmovzxbw), C("vpmovzxbd", 2, 1, 0x31, 1, h_pmovzxbd), C("vpmovzxbq", 2, 1, 0x32, 1, h_pmovzxbq), C("vpmovzxwd", 2, 1, 0x33, 1, h_pmovzxwd), C("vpmovzxwq", 2, 1, 0x34, 1, h_pmovzxwq), C("vpmovzxdq", 2, 1, 0x35, 1, h_pmovzxdq),
    CX("vpermd", 2, 1, 0x36, 0, 0, false, false, false, false, h_vpermd), CX("vpermps", 2, 1, 0x16, 0, 0, false, false, false, false, h_vpermps),
    CX("vpblendvb", 3, 1, 0x4C, 2, 0, false, true, true, false, h_pblendvb), CX("vblendvps", 3, 1, 0x4A, 2, 0, false, true, true, false, h_blendvps), CX("vblendvpd", 3, 1, 0x4B, 2, 0, false, true, true, false, h_blendvpd),
    C("vpermilps", 2, 1, 0x0C, 0, h_vpermilps_var), C("vpermilpd", 2, 1, 0x0D, 0, h_vpermilpd_var),
    // float
    C("vaddps", 1, 0, 0x58, 0, h_addps), C("vaddpd", 1, 1, 0x58, 0, h_addpd), C("vsubps", 1, 0, 0x5C, 0, h_subps), C("vsubpd", 1, 1, 0x5C, 0, h_subpd),
    C("vmulps", 1, 0, 0x59, 0, h_mulps), C("vmulpd", 1, 1, 0x59, 0, h_mulpd), C("vdivps", 1, 0, 0x5E, 0, h_divps), C("vdivpd", 1, 1, 0x5E, 0, h_divpd),
    C("vminps", 1, 0, 0x5D, 0, h_minps), C("vminpd", 1, 1, 0x5D, 0, h_minpd), C("vmaxps", 1, 0, 0x5F, 0, h_maxps), C("vmaxpd", 1, 1, 0x5F, 0, h_maxpd),
    C("vsqrtps", 1, 0, 0x51, 1, h_sqrtps), C("vsqrtpd", 1, 1, 0x51, 1, h_sqrtpd),
    C("vandps", 1, 0, 0x54, 0, h_andps), C("vandnps", 1, 0, 0x55, 0, h_andnps), C("vorps", 1, 0, 0x56, 0, h_orps), C("vxorps", 1, 0, 0x57, 0, h_xorps),
    C("vunpcklps", 1, 0, 0x14, 0, h_unpcklps), C("vunpckhps", 1, 0, 0x15, 0, h_unpckhps), C("vunpcklpd", 1, 1, 0x14, 0, h_unpcklpd), C("vunpckhpd", 1, 1, 0x15, 0, h_unpckhpd),
    C("vhaddps", 1, 3, 0x7C, 0, h_haddps), C("vhaddpd", 1, 1, 0x7C, 0, h_haddpd), C("vhsubps", 1, 3, 0x7D, 0, h_hsubps), C("vhsubpd", 1, 1, 0x7D, 0, h_hsubpd),
    C("vaddsubps", 1, 3, 0xD0, 0, h_addsubps), C("vaddsubpd", 1, 1, 0xD0, 0, h_addsubpd),
    C("vcvtdq2ps", 1, 0, 0x5B, 1, h_cvtdq2ps), C("vcvtps2dq", 1, 1, 0x5B, 1, h_cvtps2dq), C("vcvttps2dq", 1, 2, 0x5B, 1, h_cvttps2dq),
    CX("vfmadd132ps", 2, 1, 0x98, 3, 0, false, true, true, false, h_fmadd132ps), CX("vfmadd213ps", 2, 1, 0xA8, 3, 0, false, true, true, false, h_fmadd213ps), CX("vfmadd231ps", 2, 1, 0xB8, 3, 0, false, true, true, false, h_fmadd231ps),
    CX("vfmsub231ps", 2, 1, 0xBA, 3, 0, false, true, true, false, h_fmsub231ps), CX("vfnmadd231ps", 2, 1, 0xBC, 3, 0, false, true, true, false, h_fnmadd231ps), CX("vfnmsub231ps", 2, 1, 0xBE, 3, 0, false, true, true, false, h_fnmsub231ps),
    CX("vfmadd231pd", 2, 1, 0xB8, 3, 0, false, true, true, true, h_fmadd231pd), CX("vfmaddsub231ps", 2, 1, 0xB6, 3, 0, false, true, true, false, h_fmaddsub231ps), CX("vfmsubadd231ps", 2, 1, 0xB7, 3, 0, false, true, true, false, h_fmsubadd231ps),
    C("vaesenc", 2, 1, 0xDC, 0, h_aesenc),
    C("vmovshdup", 1, 2, 0x16, 1, h_movshdup), C("vmovsldup", 1, 2, 0x12, 1, h_movsldup), C("vcvtps2pd", 1, 0, 0x5A, 1, h_cvtps2pd), C("vcvtdq2pd", 1, 2, 0xE6, 1, h_cvtdq2pd),
    C("vcvtpd2ps", 1, 1, 0x5A, 1, h_cvtpd2ps), C("vcvtpd2dq", 1, 3, 0xE6, 1, h_cvtpd2dq), C("vcvttpd2dq", 1, 1, 0xE6, 1, h_cvttpd2dq), C("vcvtph2ps", 2, 1, 0x13, 1, h_cvtph2ps),
    // immediates
    CI("vpalignr3", 3, 1, 0x0F, 0, 3, h_palignr3), CI("vpalignr12", 3, 1, 0x0F, 0, 12, h_palignr12), CI("vpalignr20", 3, 1, 0x0F, 0, 20, h_palignr20),
    CI("vpblendwa5", 3, 1, 0x0E, 0, 0xA5, h_pblendw_a5), CI("vpblendw3c", 3, 1, 0x0E, 0, 0x3C, h_pblendw_3c), CI("vblendps", 3, 1, 0x0C, 0, 0x9C, h_blendps_9), CI("vblendpd", 3, 1, 0x0D, 0, 0x6, h_blendpd_6),
    CX("vpblendd", 3, 1, 0x02, 0, 0x5A, true, true, true, false, h_pblendd_5a),
    CI("vpshufd1b", 1, 1, 0x70, 1, 0x1B, h_pshufd_1b), CI("vpshufde4", 1, 1, 0x70, 1, 0xE4, h_pshufd_e4), CI("vpshuflw", 1, 3, 0x70, 1, 0x4E, h_pshuflw_4e), CI("vpshufhw", 1, 2, 0x70, 1, 0xB1, h_pshufhw_b1),
    CI("vshufps44", 1, 0, 0xC6, 0, 0x44, h_shufps_44), CI("vshufpsb1", 1, 0, 0xC6, 0, 0xB1, h_shufps_b1), CI("vshufpd", 1, 1, 0xC6, 0, 0xA, h_shufpd_a),
    CI("vroundps0", 3, 1, 0x08, 1, 0x08, h_roundps_0), CI("vroundps1", 3, 1, 0x08, 1, 0x09, h_roundps_1), CI("vroundps2", 3, 1, 0x08, 1, 0x0A, h_roundps_2), CI("vroundps3", 3, 1, 0x08, 1, 0x0B, h_roundps_3),
    CI("vroundpd1", 3, 1, 0x09, 1, 0x09, h_roundpd_1), CI("vroundpd3", 3, 1, 0x09, 1, 0x0B, h_roundpd_3),
    CI("vdpps-ff", 3, 1, 0x40, 0, 0xFF, h_dpps_ff), CI("vdpps71", 3, 1, 0x40, 0, 0x71, h_dpps_71), CI("vdpps3a", 3, 1, 0x40, 0, 0x3A, h_dpps_3a),
    CI("vmpsadbw0", 3, 1, 0x42, 0, 0, h_mpsadbw_0), CI("vmpsadbw5", 3, 1, 0x42, 0, 5, h_mpsadbw_5), CI("vmpsadbw7", 3, 1, 0x42, 0, 7, h_mpsadbw_7),
    CI("vpclmulqdq00", 3, 1, 0x44, 0, 0x00, h_pclmul_00), CI("vpclmulqdq11", 3, 1, 0x44, 0, 0x11, h_pclmul_11), CI("vpclmulqdq10", 3, 1, 0x44, 0, 0x10, h_pclmul_10),
    CX("vpermq", 3, 1, 0x00, 1, 0x1B, true, false, false, true, h_vpermq_1b), CX("vpermpd", 3, 1, 0x01, 1, 0xC6, true, false, false, true, h_vpermpd_c6),
    CX("vperm2f128", 3, 1, 0x06, 0, 0x20, true, false, false, false, h_vperm2f128_20), CX("vperm2i128-31", 3, 1, 0x46, 0, 0x31, true, false, false, false, h_vperm2i128_31), CX("vperm2i128-82", 3, 1, 0x46, 0, 0x82, true, false, false, false, h_vperm2i128_8_2),
    CI("vpermilps-imm", 3, 1, 0x04, 1, 0x1B, h_vpermilps_1b), CI("vpermilpd-imm", 3, 1, 0x05, 1, 0x5, h_vpermilpd_5),
#define CMPC(N) CI("vcmpps" #N, 1, 0, 0xC2, 0, N, h_cmpps_##N), CI("vcmppd" #N, 1, 1, 0xC2, 0, N, h_cmppd_##N)
    CMPC(0), CMPC(1), CMPC(2), CMPC(3), CMPC(4), CMPC(5), CMPC(6), CMPC(7), CMPC(8), CMPC(9), CMPC(10), CMPC(11), CMPC(12), CMPC(13), CMPC(14), CMPC(15),
    CMPC(16), CMPC(17), CMPC(18), CMPC(19), CMPC(20), CMPC(21), CMPC(22), CMPC(23), CMPC(24), CMPC(25), CMPC(26), CMPC(27), CMPC(28), CMPC(29), CMPC(30), CMPC(31),
};

constexpr u64 kCode = 0x400000, kData = 0x500000, kStack = 0x600000, kSize = 0x10000;
int reg(const char* name) { return *uu::x86_register(name); }

// The instruction's bytes: a three-byte VEX prefix, the opcode, ModRM (dst 0, vvvv 1, rm 2 or memory at RAX) and the extra bytes.
Bytes encode(const Case& c, bool l256, bool memory) {
    Bytes code;
    const unsigned vvvv = c.form == 1 ? 0 : 1;   // (an instruction without a first source has the field all ones, which is register 15 inverted: 1111b -> ~ = 0)
    code.push_back(0xC4);
    code.push_back(static_cast<std::uint8_t>(0xE0 | c.map));
    code.push_back(static_cast<std::uint8_t>((c.wide ? 0x80 : 0) | ((~vvvv & 0xF) << 3) | (l256 ? 4 : 0) | c.pp));
    code.push_back(static_cast<std::uint8_t>(c.opcode));
    code.push_back(static_cast<std::uint8_t>(memory ? 0x00 : 0xC2));   // reg 0; rm = RAX (memory) or register 2
    if (c.has_imm) code.push_back(static_cast<std::uint8_t>(c.imm));
    if (c.form == 2) code.push_back(0x30);   // is4: the mask register is ymm3
    return code;
}

}  // namespace

CB_TEST("engine.pcode_cpu.x86_64_avx_instructions_equal_the_hosts", [] {
    if (!host_has_avx()) {
        std::printf("pcode_cpu_x86 avx: this machine or compiler has no AVX2, FMA, F16C, AES and PCLMULQDQ to compare with: skipped\n");
        return;
    }
    pc::x86_options().vex_as_unicorn = false;
    std::mt19937 rng(256);
    std::map<std::string, std::string> bad;
    unsigned runs = 0, rounds = 6;
    pc::x86_stats().clear();
    for (const Case& c : kCases)
        for (const bool l256 : {false, true})
            for (const bool memory : {false, true}) {
                if (!l256 && !c.l128) continue;
                // (the 256-bit forms of VAESENC and VPCLMULQDQ are VAES and VPCLMULQDQ's: the description has no encoding for them)
                if (l256 && (std::strncmp(c.name, "vaesenc", 7) == 0 || std::strncmp(c.name, "vpclmulqdq", 10) == 0)) continue;
                for (unsigned round = 0; round < rounds; ++round) {
                    // the registers
                    alignas(32) std::uint8_t reg_x[32], reg_y[32], reg_z[32], reg_m[32];
                    for (auto* r : {reg_x, reg_y, reg_z, reg_m})
                        for (int i = 0; i < 32; ++i) r[i] = static_cast<std::uint8_t>(rng());
                    if (round % 3 == 1)   // (floats of ordinary size: random bits are mostly huge or NaN)
                        for (auto* r : {reg_x, reg_y, reg_z, reg_m})
                            for (int i = 0; i < 32; i += 4) {
                                const float f = static_cast<float>(static_cast<int>(rng() % 2000) - 1000) / 8.0f;
                                std::memcpy(r + i, &f, 4);
                            }
                    if (round % 3 == 2)
                        for (auto* r : {reg_x, reg_y, reg_z, reg_m})
                            for (int i = 0; i < 32; i += 8) {
                                const double f = static_cast<double>(static_cast<int>(rng() % 2000) - 1000) / 8.0;
                                std::memcpy(r + i, &f, 8);
                            }
                    if (!l256) {   // (a 128-bit instruction reads the low halves, and the result's upper half is zero)
                        std::memset(reg_y + 16, 0, 16);
                        std::memset(reg_z + 16, 0, 16);
                        std::memset(reg_m + 16, 0, 16);
                    }
                    if (c.form == 3 && !l256) std::memset(reg_x + 16, 0, 16);
                    // the engine
                    std::unique_ptr<core16::Uc> uc = pc::open_x86_emulator(64);
                    uc->mem_map(kCode, kSize);
                    uc->mem_map(kData, kSize);
                    uc->mem_map(kStack, kSize);
                    uc->reg_write(reg("RAX"), kData + 0x100);
                    uc->reg_write(reg("RSP"), kStack + 0x8000);
                    const Bytes code = encode(c, l256, memory);
                    uc->mem_write(kCode, code);
                    pc::write_ymm(*uc, 0, ByteView(reg_x, 32));
                    pc::write_ymm(*uc, 1, ByteView(reg_y, 32));
                    pc::write_ymm(*uc, 3, ByteView(reg_m, 32));
                    if (memory) uc->mem_write(kData + 0x100, ByteView(reg_z, 32));
                    else pc::write_ymm(*uc, 2, ByteView(reg_z, 32));
                    bool faulted = false;
                    try {
                        uc->emu_start(kCode, kCode + code.size(), 0, 1);
                    } catch (const uu::UcFault&) {
                        faulted = true;
                    }
                    const Bytes got = pc::read_ymm(*uc, 0);
                    // the host
                    alignas(32) std::uint8_t want[32];
                    const V result = c.host(_mm256_load_si256(reinterpret_cast<const __m256i*>(reg_x)), _mm256_load_si256(reinterpret_cast<const __m256i*>(reg_y)),
                                            _mm256_load_si256(reinterpret_cast<const __m256i*>(reg_z)), _mm256_load_si256(reinterpret_cast<const __m256i*>(reg_m)));
                    _mm256_store_si256(reinterpret_cast<__m256i*>(want), result);
                    if (!l256) std::memset(want + 16, 0, 16);
                    ++runs;
                    if (faulted || std::memcmp(want, got.data(), 32) != 0) {
                        const std::string key = std::format("{}{}{}", c.name, l256 ? " ymm" : " xmm", memory ? " mem" : "");
                        if (!bad.count(key)) {
                            std::string text = faulted ? "faulted " : "";
                            for (int i = 0; i < 32; ++i)
                                if (want[i] != got[static_cast<std::size_t>(i)]) {
                                    text += std::format("byte {} host {:02X} engine {:02X}", i, want[i], got[static_cast<std::size_t>(i)]);
                                    break;
                                }
                            bad[key] = text;
                        }
                    }
                }
            }
    std::printf("pcode_cpu_x86 avx: %u runs of %zu instructions, %zu of the forms differ from the host\n", runs, sizeof(kCases) / sizeof(kCases[0]), bad.size());
    for (const auto& [k, v] : bad) std::printf("    %s: %s\n", k.c_str(), v.c_str());
    for (const auto& [k, c] : pc::x86_stats().unserved) std::printf("    unserved user op: %s x%llu\n", k.c_str(), static_cast<unsigned long long>(c));
    for (const auto& [k, c] : pc::x86_stats().handoffs) std::printf("    to Unicorn: %s x%llu\n", k.c_str(), static_cast<unsigned long long>(c));
    CB_CHECK_EQ(bad.size(), 0u);
});

#else

CB_TEST("engine.pcode_cpu.x86_64_avx_instructions_equal_the_hosts", [] { std::printf("pcode_cpu_x86 avx: no host intrinsics with this compiler: skipped\n"); });

#endif

}  // namespace cb
