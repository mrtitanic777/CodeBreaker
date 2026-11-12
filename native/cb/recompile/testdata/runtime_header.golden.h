// rex_ppc_min.h - the runtime of CodeBreaker's recompiled PowerPC code.
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
    PPCRegister r0;
    PPCRegister r1;
    PPCRegister r2;
    PPCRegister r3;
    PPCRegister r4;
    PPCRegister r5;
    PPCRegister r6;
    PPCRegister r7;
    PPCRegister r8;
    PPCRegister r9;
    PPCRegister r10;
    PPCRegister r11;
    PPCRegister r12;
    PPCRegister r13;
    PPCRegister r14;
    PPCRegister r15;
    PPCRegister r16;
    PPCRegister r17;
    PPCRegister r18;
    PPCRegister r19;
    PPCRegister r20;
    PPCRegister r21;
    PPCRegister r22;
    PPCRegister r23;
    PPCRegister r24;
    PPCRegister r25;
    PPCRegister r26;
    PPCRegister r27;
    PPCRegister r28;
    PPCRegister r29;
    PPCRegister r30;
    PPCRegister r31;
    uint64_t lr;
    PPCRegister ctr;
    PPCXERRegister xer;
    PPCRegister reserved;   // lwarx/stwcx reservation value (atomics)
    PPCCRRegister cr0;
    PPCCRRegister cr1;
    PPCCRRegister cr2;
    PPCCRRegister cr3;
    PPCCRRegister cr4;
    PPCCRRegister cr5;
    PPCCRRegister cr6;
    PPCCRRegister cr7;
    PPCRegister f0;
    PPCRegister f1;
    PPCRegister f2;
    PPCRegister f3;
    PPCRegister f4;
    PPCRegister f5;
    PPCRegister f6;
    PPCRegister f7;
    PPCRegister f8;
    PPCRegister f9;
    PPCRegister f10;
    PPCRegister f11;
    PPCRegister f12;
    PPCRegister f13;
    PPCRegister f14;
    PPCRegister f15;
    PPCRegister f16;
    PPCRegister f17;
    PPCRegister f18;
    PPCRegister f19;
    PPCRegister f20;
    PPCRegister f21;
    PPCRegister f22;
    PPCRegister f23;
    PPCRegister f24;
    PPCRegister f25;
    PPCRegister f26;
    PPCRegister f27;
    PPCRegister f28;
    PPCRegister f29;
    PPCRegister f30;
    PPCRegister f31;
    PPCVRegister v0;
    PPCVRegister v1;
    PPCVRegister v2;
    PPCVRegister v3;
    PPCVRegister v4;
    PPCVRegister v5;
    PPCVRegister v6;
    PPCVRegister v7;
    PPCVRegister v8;
    PPCVRegister v9;
    PPCVRegister v10;
    PPCVRegister v11;
    PPCVRegister v12;
    PPCVRegister v13;
    PPCVRegister v14;
    PPCVRegister v15;
    PPCVRegister v16;
    PPCVRegister v17;
    PPCVRegister v18;
    PPCVRegister v19;
    PPCVRegister v20;
    PPCVRegister v21;
    PPCVRegister v22;
    PPCVRegister v23;
    PPCVRegister v24;
    PPCVRegister v25;
    PPCVRegister v26;
    PPCVRegister v27;
    PPCVRegister v28;
    PPCVRegister v29;
    PPCVRegister v30;
    PPCVRegister v31;
    PPCVRegister v32;
    PPCVRegister v33;
    PPCVRegister v34;
    PPCVRegister v35;
    PPCVRegister v36;
    PPCVRegister v37;
    PPCVRegister v38;
    PPCVRegister v39;
    PPCVRegister v40;
    PPCVRegister v41;
    PPCVRegister v42;
    PPCVRegister v43;
    PPCVRegister v44;
    PPCVRegister v45;
    PPCVRegister v46;
    PPCVRegister v47;
    PPCVRegister v48;
    PPCVRegister v49;
    PPCVRegister v50;
    PPCVRegister v51;
    PPCVRegister v52;
    PPCVRegister v53;
    PPCVRegister v54;
    PPCVRegister v55;
    PPCVRegister v56;
    PPCVRegister v57;
    PPCVRegister v58;
    PPCVRegister v59;
    PPCVRegister v60;
    PPCVRegister v61;
    PPCVRegister v62;
    PPCVRegister v63;
    PPCVRegister v64;
    PPCVRegister v65;
    PPCVRegister v66;
    PPCVRegister v67;
    PPCVRegister v68;
    PPCVRegister v69;
    PPCVRegister v70;
    PPCVRegister v71;
    PPCVRegister v72;
    PPCVRegister v73;
    PPCVRegister v74;
    PPCVRegister v75;
    PPCVRegister v76;
    PPCVRegister v77;
    PPCVRegister v78;
    PPCVRegister v79;
    PPCVRegister v80;
    PPCVRegister v81;
    PPCVRegister v82;
    PPCVRegister v83;
    PPCVRegister v84;
    PPCVRegister v85;
    PPCVRegister v86;
    PPCVRegister v87;
    PPCVRegister v88;
    PPCVRegister v89;
    PPCVRegister v90;
    PPCVRegister v91;
    PPCVRegister v92;
    PPCVRegister v93;
    PPCVRegister v94;
    PPCVRegister v95;
    PPCVRegister v96;
    PPCVRegister v97;
    PPCVRegister v98;
    PPCVRegister v99;
    PPCVRegister v100;
    PPCVRegister v101;
    PPCVRegister v102;
    PPCVRegister v103;
    PPCVRegister v104;
    PPCVRegister v105;
    PPCVRegister v106;
    PPCVRegister v107;
    PPCVRegister v108;
    PPCVRegister v109;
    PPCVRegister v110;
    PPCVRegister v111;
    PPCVRegister v112;
    PPCVRegister v113;
    PPCVRegister v114;
    PPCVRegister v115;
    PPCVRegister v116;
    PPCVRegister v117;
    PPCVRegister v118;
    PPCVRegister v119;
    PPCVRegister v120;
    PPCVRegister v121;
    PPCVRegister v122;
    PPCVRegister v123;
    PPCVRegister v124;
    PPCVRegister v125;
    PPCVRegister v126;
    PPCVRegister v127;
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

#define PPC_LOAD_U8(x)  ((*(volatile uint8_t*)(base + (uint32_t)(x))))
#define PPC_LOAD_U16(x)  (__builtin_bswap16(*(volatile uint16_t*)(base + (uint32_t)(x))))
#define PPC_LOAD_U32(x)  (__builtin_bswap32(*(volatile uint32_t*)(base + (uint32_t)(x))))
#define PPC_LOAD_U64(x)  (__builtin_bswap64(*(volatile uint64_t*)(base + (uint32_t)(x))))
#define PPC_STORE_U8(x,y)  (*(volatile uint8_t*)(base + (uint32_t)(x)) = ((((uint8_t)(y)))))
#define PPC_STORE_U16(x,y)  (*(volatile uint16_t*)(base + (uint32_t)(x)) = (__builtin_bswap16(((uint16_t)(y)))))
#define PPC_STORE_U32(x,y)  (*(volatile uint32_t*)(base + (uint32_t)(x)) = (__builtin_bswap32(((uint32_t)(y)))))
#define PPC_STORE_U64(x,y)  (*(volatile uint64_t*)(base + (uint32_t)(x)) = (__builtin_bswap64(((uint64_t)(y)))))

// ---- runtime symbols the builders reference --------------------------------
// (bridge mode: real definitions come from <rex/ppc/context.h> + <memory.h>)

// Provided by the runtime (native/rexglue/kernel; not redefined when it is included):
//   #include <simde/x86/sse4.1.h>   // simde_mm_shuffle_epi8, etc.
// extern const uint8_t VectorMaskL[];        // whole-vector byte-reverse mask
// extern const uint8_t VectorMaskR[];
// extern const uint8_t VectorShiftTableL[];  // lvsl table
// extern const uint8_t VectorShiftTableR[];  // lvsr table
// PPC_LOAD_U*/PPC_STORE_U*/PPC_RAW_ADDR come from the recompiler runtime header.


// ---------------------------------------------------------------------------
// VMX runtime helpers the vector statements call. They are the runtime's (the rex namespace
// of native/rexglue/kernel: ppc.h and its SIMD extension header), declared here, not defined.
//
//   namespace rex {
//     simde__m128i simde_mm_adds_epu32(simde__m128i a, simde__m128i b);   // saturating u32 add
//     simde__m128i simde_mm_avg_epi8 (simde__m128i a, simde__m128i b);    // signed byte average
//     simde__m128i simde_mm_avg_epi16(simde__m128i a, simde__m128i b);    // signed hword average
//     simde__m128i simde_mm_avg_epi32(simde__m128i a, simde__m128i b);    // signed word average
//     simde__m128i simde_mm_cmpgt_epu8 (simde__m128i a, simde__m128i b);  // unsigned byte  >
//     simde__m128i simde_mm_cmpgt_epu16(simde__m128i a, simde__m128i b);  // unsigned hword >
//     simde__m128i simde_mm_vctsxs(simde__m128 v);      // float->signed word saturate
//     simde__m128i simde_mm_vctuxs(simde__m128 v);      // float->unsigned word saturate
//     simde__m128  simde_mm_cvtepu32_ps_(simde__m128i v); // unsigned word -> float
//     simde__m128i simde_mm_perm_epi8_(simde__m128i a, simde__m128i b, simde__m128i c); // vperm
//     simde__m128i simde_mm_vsl (simde__m128i a, simde__m128i b);  // 128-bit shift-left  (bits)
//     simde__m128i simde_mm_vsr (simde__m128i a, simde__m128i b);  // 128-bit shift-right (bits)
//     simde__m128i simde_mm_vslo(simde__m128i a, simde__m128i b);  // 128-bit shift-left  (octets)
//     simde__m128i simde_mm_vsro(simde__m128i a, simde__m128i b);  // 128-bit shift-right (octets)
//   }
//
// Constants referenced by vpkd3d128 (NORMPACKED32) come from ppc/context.h:
//   kPack2101010_Min10 / kPack2101010_Max10 / kPack2101010_Min2 / kPack2101010_Max2
// ---------------------------------------------------------------------------

