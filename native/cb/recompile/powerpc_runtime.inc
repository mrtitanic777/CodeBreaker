// cb/recompile/powerpc_runtime.inc -- the text of the runtime header (`runtime_header`) and of the AltiVec helpers it holds.
// Included by powerpc.cpp only. The header's text has @regs@, @fprs@, @vrs@, @crs@, @VEC_HELPERS@, @loads@, @stores@ and
// @port_helpers@ where the context's registers, the memory macros and the builders' runtime helpers go.
inline constexpr std::string_view kVectorHelpers = R"cbvec(
static inline int16_t _rex_sath(int32_t v){ return v>32767?32767:(v<-32768?-32768:(int16_t)v); }
#define REX_VBIN8(NAME,OP)  static inline void NAME(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<16;i++)d.u8[i]=(uint8_t)(OP);}
#define REX_VBIN16(NAME,OP) static inline void NAME(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<8;i++)d.u16[i]=(uint16_t)(OP);}
#define REX_VBIN32(NAME,OP) static inline void NAME(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<4;i++)d.u32[i]=(uint32_t)(OP);}
REX_VBIN8(ppc_vaddubm, a.u8[i]+b.u8[i])
REX_VBIN16(ppc_vadduhm, a.u16[i]+b.u16[i])
REX_VBIN32(ppc_vadduwm, a.u32[i]+b.u32[i])
REX_VBIN8(ppc_vsububm, a.u8[i]-b.u8[i])
REX_VBIN16(ppc_vsubuhm, a.u16[i]-b.u16[i])
REX_VBIN32(ppc_vsubuwm, a.u32[i]-b.u32[i])
REX_VBIN16(ppc_vslh, a.u16[i]<<(b.u16[i]&15))
REX_VBIN16(ppc_vsrh, a.u16[i]>>(b.u16[i]&15))
REX_VBIN32(ppc_vslw, a.u32[i]<<(b.u32[i]&31))
REX_VBIN32(ppc_vsrw, a.u32[i]>>(b.u32[i]&31))
static inline void ppc_vsrah(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<8;i++)d.s16[i]=(int16_t)(a.s16[i]>>(b.u16[i]&15));}
static inline void ppc_vsraw(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<4;i++)d.s32[i]=(int32_t)(a.s32[i]>>(b.u32[i]&31));}
static inline void ppc_vaddshs(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<8;i++)d.s16[i]=_rex_sath((int32_t)a.s16[i]+b.s16[i]);}
static inline void ppc_vsubshs(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){for(int i=0;i<8;i++)d.s16[i]=_rex_sath((int32_t)a.s16[i]-b.s16[i]);}
static inline void ppc_vand(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=a.u64[0]&b.u64[0];d.u64[1]=a.u64[1]&b.u64[1];}
static inline void ppc_vor (PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=a.u64[0]|b.u64[0];d.u64[1]=a.u64[1]|b.u64[1];}
static inline void ppc_vxor(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=a.u64[0]^b.u64[0];d.u64[1]=a.u64[1]^b.u64[1];}
static inline void ppc_vandc(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=a.u64[0]&~b.u64[0];d.u64[1]=a.u64[1]&~b.u64[1];}
static inline void ppc_vnor(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){d.u64[0]=~(a.u64[0]|b.u64[0]);d.u64[1]=~(a.u64[1]|b.u64[1]);}
static inline void ppc_vmrghb(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<8;i++){t.u8[2*i]=a.u8[i];t.u8[2*i+1]=b.u8[i];}d=t;}
static inline void ppc_vmrglb(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<8;i++){t.u8[2*i]=a.u8[8+i];t.u8[2*i+1]=b.u8[8+i];}d=t;}
static inline void ppc_vmrghh(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<4;i++){t.u16[2*i]=a.u16[i];t.u16[2*i+1]=b.u16[i];}d=t;}
static inline void ppc_vmrglh(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<4;i++){t.u16[2*i]=a.u16[4+i];t.u16[2*i+1]=b.u16[4+i];}d=t;}
static inline void ppc_vmrghw(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<2;i++){t.u32[2*i]=a.u32[i];t.u32[2*i+1]=b.u32[i];}d=t;}
static inline void ppc_vmrglw(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b){PPCVRegister t;for(int i=0;i<2;i++){t.u32[2*i]=a.u32[2+i];t.u32[2*i+1]=b.u32[2+i];}d=t;}
static inline void ppc_vperm(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b,const PPCVRegister&c){PPCVRegister t;for(int i=0;i<16;i++){int s=c.u8[i]&0x1F;t.u8[i]=s<16?a.u8[s]:b.u8[s-16];}d=t;}
static inline void ppc_vsel(PPCVRegister&d,const PPCVRegister&a,const PPCVRegister&b,const PPCVRegister&c){d.u64[0]=(a.u64[0]&~c.u64[0])|(b.u64[0]&c.u64[0]);d.u64[1]=(a.u64[1]&~c.u64[1])|(b.u64[1]&c.u64[1]);}
static inline void ppc_vspltw(PPCVRegister&d,const PPCVRegister&a,int e){uint32_t v=a.u32[e&3];for(int i=0;i<4;i++)d.u32[i]=v;}
static inline void ppc_vsplth(PPCVRegister&d,const PPCVRegister&a,int e){uint16_t v=a.u16[e&7];for(int i=0;i<8;i++)d.u16[i]=v;}
static inline void ppc_vspltb(PPCVRegister&d,const PPCVRegister&a,int e){uint8_t v=a.u8[e&15];for(int i=0;i<16;i++)d.u8[i]=v;}
)cbvec";

inline constexpr std::string_view kRuntimeTemplate = R"cbrt(// rex_ppc_min.h - the runtime of CodeBreaker's recompiled PowerPC code.
// When the rexglue kernel (native/rexglue/kernel) is on the include path (<rex/ppc/context.h>),
// this BRIDGES onto its PPCContext so the recompiled code and the kernel share
// one runtime ABI (the register types are byte-compatible - ours were modeled on
// the kernel's). Otherwise it falls back to a standalone context of its own that compiles
// alone. Define REX_PPC_STANDALONE to force the standalone path.
#pragma once
#include <cstdint>
#include <cmath>     // the floating-point builders (fabs/sqrt/frsp/fsel ...)
#include <climits>   // the conversion builders (saturation bounds)

#if __has_include(<rex/ppc/context.h>) && !defined(REX_PPC_STANDALONE)
// ---- BRIDGE: the rexglue kernel's PPCContext --------------------------------
#define REX_PPC_BRIDGE 1
#include <rex/ppc/context.h>       // PPCContext(+kernel_state), reg types, PPC_FUNC, PPCFuncMapping
using PPCVRegister = rex::VRegister;
#else
// ---- STANDALONE: a minimal context of its own -------------------------------
union PPCRegister {
    int8_t s8; uint8_t u8; int16_t s16; uint16_t u16;
    int32_t s32; uint32_t u32; int64_t s64; uint64_t u64; float f32; double f64;
};
struct PPCXERRegister { uint8_t so, ov, ca; };
struct PPCCRRegister {
    uint8_t lt, gt, eq, so;
    template <class T> void compare(T a, T b, const PPCXERRegister& xer) {
        lt = a < b; gt = a > b; eq = a == b; so = xer.so;
    }
};
union alignas(16) PPCVRegister {
    int8_t s8[16]; uint8_t u8[16]; int16_t s16[8]; uint16_t u16[8];
    int32_t s32[4]; uint32_t u32[4]; int64_t s64[2]; uint64_t u64[2];
    float f32[4]; double f64[2];
};
struct alignas(64) PPCContext {
@regs@
    uint64_t lr;
    PPCRegister ctr;
    PPCXERRegister xer;
    PPCRegister reserved;   // lwarx/stwcx reservation value (atomics)
@crs@
@fprs@
@vrs@
};
using PPCFunc = void(PPCContext& ctx, uint8_t* base);
#define PPC_FUNC(x)        void x(PPCContext& __restrict ctx, uint8_t* base)
#define PPC_FUNC_IMPL(x)   PPC_FUNC(x)
#define PPC_WEAK_FUNC(x)   PPC_FUNC(x)
#define PPC_FUNC_PROLOGUE()  do { (void)ctx; (void)base; } while (0)
#define PPC_UNIMPLEMENTED(s) do { /* trap: {s} not yet translated */ } while (0)
#endif

// ---- shared helpers (PPCVRegister is defined in both branches) --------------
static inline uint32_t ppc_rotl32(uint32_t v, uint32_t n) {
    n &= 31; return n ? ((v << n) | (v >> (32 - n))) : v;
}
static inline uint64_t ppc_rotl64(uint64_t v, uint32_t n) {
    n &= 63; return n ? ((v << n) | (v >> (64 - n))) : v;
}
// PowerPC big-endian bit numbering (bit 0 = MSB): mask bits [mb, me] inclusive,
// wrapping when mb > me.
static inline uint32_t ppc_mask32(int mb, int me) {
    uint32_t begin = 0xFFFFFFFFu >> mb;
    uint32_t end = (me >= 31) ? 0xFFFFFFFFu : (0xFFFFFFFFu << (31 - me));
    return (mb <= me) ? (begin & end) : (begin | end);
}
static inline uint32_t ppc_rlwinm(uint32_t v, int sh, int mb, int me) {
    return ppc_rotl32(v, sh) & ppc_mask32(mb, me);
}
static inline uint32_t ppc_rlwimi(uint32_t cur, uint32_t v, int sh, int mb, int me) {
    uint32_t m = ppc_mask32(mb, me);
    return (cur & ~m) | (ppc_rotl32(v, sh) & m);
}
// AltiVec (128-bit) helpers. NOTE: element order follows the register's native
// lanes; full big-endian vector-load correctness is validated on real runs.
static inline void ppc_vmove(PPCVRegister& d, const PPCVRegister& a) {
    d.u64[0] = a.u64[0]; d.u64[1] = a.u64[1];
}
@VEC_HELPERS@

// ---- indirect calls + memory access ----------------------------------------
// In bridge mode, the kernel's macros are replaced by the forms the generated code
// uses (1-argument PPC_UNIMPLEMENTED; non-extern-C PPC_FUNC_IMPL; its indirect call).
#if defined(REX_PPC_BRIDGE)
#undef PPC_UNIMPLEMENTED
#define PPC_UNIMPLEMENTED(s) do { /* {s} */ } while (0)
#undef PPC_FUNC_IMPL
#define PPC_FUNC_IMPL(x) PPC_FUNC(x)
#undef PPC_CALL_INDIRECT_FUNC
#endif
extern "C" void ppc_call_indirect(PPCContext& ctx, uint8_t* base, uint32_t addr);
#define PPC_CALL_INDIRECT_FUNC(x)  ppc_call_indirect(ctx, base, (x))

@loads@
@stores@

// ---- runtime symbols the builders reference --------------------------------
// (bridge mode: real definitions come from <rex/ppc/context.h> + <memory.h>)
@port_helpers@
)cbrt";
