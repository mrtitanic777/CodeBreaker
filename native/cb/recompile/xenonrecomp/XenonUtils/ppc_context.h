#ifndef PPC_CONTEXT_H_INCLUDED
#define PPC_CONTEXT_H_INCLUDED

#ifndef PPC_CONFIG_H_INCLUDED
#error "ppc_config.h must be included before ppc_context.h"
#endif

#include <climits>
#include <cmath>
#include <csetjmp>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <x86/avx.h>
#include <x86/sse.h>
#include <x86/sse4.1.h>

// SSE3 constants are missing from simde
#ifndef _MM_DENORMALS_ZERO_MASK
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif

#define PPC_JOIN(x, y) x##y
#define PPC_XSTRINGIFY(x) #x
#define PPC_STRINGIFY(x) PPC_XSTRINGIFY(x)
#define PPC_FUNC(x) void x(PPCContext& __restrict ctx, uint8_t* base)
#define PPC_FUNC_IMPL(x) extern "C" PPC_FUNC(x)
#define PPC_EXTERN_FUNC(x) extern PPC_FUNC(x)
#define PPC_WEAK_FUNC(x) __attribute__((weak,noinline)) PPC_FUNC(x)

#define PPC_FUNC_PROLOGUE() __builtin_assume(((size_t)base & 0x1F) == 0)

// Host address of a guest effective address. On the Xbox 360 the 0xE0000000-0xFFFFFFFF window (4 KB physical
// pages) maps physical memory with a 0x1000 offset: virtual 0xE0000000 + X is physical X + 0x1000. Hosts map that
// window's view without the offset (views start on 64 KB boundaries), so CPU accesses there add 0x1000, which keeps
// CPU and GPU views of the same physical memory consistent. A runtime may define its own.
#ifndef PPC_HOST
#define PPC_HOST(x) ppc_host_address(base, (uint32_t)(x))
inline uint8_t* ppc_host_address(uint8_t* base, uint32_t address)
{
    return base + address + (address >= 0xE0000000u ? 0x1000u : 0u);
}
#endif

#ifndef PPC_LOAD_U8
#define PPC_LOAD_U8(x) *(volatile uint8_t*)PPC_HOST(x)
#endif

#ifndef PPC_LOAD_U16
#define PPC_LOAD_U16(x) __builtin_bswap16(*(volatile uint16_t*)PPC_HOST(x))
#endif

#ifndef PPC_LOAD_U32
#define PPC_LOAD_U32(x) __builtin_bswap32(*(volatile uint32_t*)PPC_HOST(x))
#endif

#ifndef PPC_LOAD_U64
#define PPC_LOAD_U64(x) __builtin_bswap64(*(volatile uint64_t*)PPC_HOST(x))
#endif

// TODO: Implement.
// These are currently unused. However, MMIO loads could possibly be handled statically with some profiling and a fallback.
// The fallback would be a runtime exception handler which will intercept reads from MMIO regions 
// and log the PC for compiling to static code later.
#ifndef PPC_MM_LOAD_U8
#define PPC_MM_LOAD_U8(x)  PPC_LOAD_U8 (x)
#endif

#ifndef PPC_MM_LOAD_U16
#define PPC_MM_LOAD_U16(x) PPC_LOAD_U16(x)
#endif

#ifndef PPC_MM_LOAD_U32
#define PPC_MM_LOAD_U32(x) PPC_LOAD_U32(x)
#endif

#ifndef PPC_MM_LOAD_U64
#define PPC_MM_LOAD_U64(x) PPC_LOAD_U64(x)
#endif

#ifndef PPC_STORE_U8
#define PPC_STORE_U8(x, y) *(volatile uint8_t*)PPC_HOST(x) = (y)
#endif

#ifndef PPC_STORE_U16
#define PPC_STORE_U16(x, y) *(volatile uint16_t*)PPC_HOST(x) = __builtin_bswap16(y)
#endif

#ifndef PPC_STORE_U32
#define PPC_STORE_U32(x, y) *(volatile uint32_t*)PPC_HOST(x) = __builtin_bswap32(y)
#endif

#ifndef PPC_STORE_U64
#define PPC_STORE_U64(x, y) *(volatile uint64_t*)PPC_HOST(x) = __builtin_bswap64(y)
#endif

// MMIO Store handling is completely reliant on being preeceded by eieio.
// TODO: Verify if that's always the case.
#ifndef PPC_MM_STORE_U8
#define PPC_MM_STORE_U8(x, y)   PPC_STORE_U8 (x, y)
#endif

#ifndef PPC_MM_STORE_U16
#define PPC_MM_STORE_U16(x, y)  PPC_STORE_U16(x, y)
#endif

#ifndef PPC_MM_STORE_U32
#define PPC_MM_STORE_U32(x, y)  PPC_STORE_U32(x, y)
#endif

#ifndef PPC_MM_STORE_U64
#define PPC_MM_STORE_U64(x, y)  PPC_STORE_U64(x, y)
#endif

// Xbox 360 time base (mftb): 50 MHz ticks since the process started. A runtime may define its own.
#ifndef PPC_QUERY_TIMEBASE
#include <chrono>
inline uint64_t ppc_query_timebase()
{
    static const auto start = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::steady_clock::now() - start;
    return uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count()) / 20;
}
#define PPC_QUERY_TIMEBASE() ppc_query_timebase()
#endif

#ifndef PPC_CALL_FUNC
#define PPC_CALL_FUNC(x) x(ctx, base)
#endif

#define PPC_MEMORY_SIZE 0x100000000ull

// A runtime that loads several recompiled modules defines its own (one table for every module).
#ifndef PPC_SPIN_HINT
#define PPC_SPIN_HINT()
#endif

#ifndef PPC_LOOKUP_FUNC
#define PPC_LOOKUP_FUNC(x, y) *(PPCFunc**)(x + PPC_IMAGE_BASE + PPC_IMAGE_SIZE + (uint64_t(uint32_t(y) - PPC_CODE_BASE) * 2))
#endif

#ifndef PPC_CALL_INDIRECT_FUNC
#define PPC_CALL_INDIRECT_FUNC(x) (PPC_LOOKUP_FUNC(base, x))(ctx, base)
#endif

typedef void PPCFunc(struct PPCContext& __restrict__ ctx, uint8_t* base);

struct PPCFuncMapping
{
    size_t guest;
    PPCFunc* host;
};

extern PPCFuncMapping PPCFuncMappings[];

union PPCRegister
{
    int8_t s8;
    uint8_t u8;
    int16_t s16;
    uint16_t u16;
    int32_t s32;
    uint32_t u32;
    int64_t s64;
    uint64_t u64;
    float f32;
    double f64;
};

struct PPCXERRegister
{
    uint8_t so;
    uint8_t ov;
    uint8_t ca;
};

struct PPCCRRegister
{
    uint8_t lt;
    uint8_t gt;
    uint8_t eq;
    union
    {
        uint8_t so;
        uint8_t un;
    };

    template<typename T>
    inline void compare(T left, T right, const PPCXERRegister& xer) noexcept
    {
        lt = left < right;
        gt = left > right;
        eq = left == right;
        so = xer.so;
    }

    inline void compare(double left, double right) noexcept
    {
        un = __builtin_isnan(left) || __builtin_isnan(right);
        lt = !un && (left < right);
        gt = !un && (left > right);
        eq = !un && (left == right);
    }

    inline void setFromMask(simde__m128 mask, int imm) noexcept
    {
        int m = simde_mm_movemask_ps(mask);
        lt = m == imm; // all equal
        gt = 0;
        eq = m == 0; // none equal
        so = 0;
    }

    inline void setFromMask(simde__m128i mask, int imm) noexcept
    {
        int m = simde_mm_movemask_epi8(mask);
        lt = m == imm; // all equal
        gt = 0;
        eq = m == 0; // none equal
        so = 0;
    }
};

union alignas(0x10) PPCVRegister
{
    int8_t s8[16];
    uint8_t u8[16];
    int16_t s16[8];
    uint16_t u16[8];
    int32_t s32[4];
    uint32_t u32[4];
    int64_t s64[2];
    uint64_t u64[2];
    float f32[4];
    double f64[2];
};

#define PPC_ROUND_NEAREST 0x00
#define PPC_ROUND_TOWARD_ZERO 0x01
#define PPC_ROUND_UP 0x02
#define PPC_ROUND_DOWN 0x03
#define PPC_ROUND_MASK 0x03

struct PPCFPSCRRegister
{
    uint32_t csr;

    static constexpr size_t HostToGuest[] = { PPC_ROUND_NEAREST, PPC_ROUND_DOWN, PPC_ROUND_UP, PPC_ROUND_TOWARD_ZERO };

    // simde does not handle denormal flags, so we need to implement per-arch.
#if defined(__x86_64__) || defined(_M_X64)
    static constexpr size_t RoundShift = 13;
    static constexpr size_t RoundMask = SIMDE_MM_ROUND_MASK;
    static constexpr size_t FlushMask = SIMDE_MM_FLUSH_ZERO_MASK | _MM_DENORMALS_ZERO_MASK;
    static constexpr size_t GuestToHost[] = { SIMDE_MM_ROUND_NEAREST, SIMDE_MM_ROUND_TOWARD_ZERO, SIMDE_MM_ROUND_UP, SIMDE_MM_ROUND_DOWN };

    inline uint32_t getcsr() noexcept
    {
        return simde_mm_getcsr();
    }

    inline void setcsr(uint32_t csr) noexcept
    {
        simde_mm_setcsr(csr);
    }
#elif defined(__aarch64__) || defined(_M_ARM64)
    // RMode
    static constexpr size_t RoundShift = 22;
    static constexpr size_t RoundMask = 3 << RoundShift;
    // FZ and FZ16
    static constexpr size_t FlushMask = (1 << 19) | (1 << 24);
    // Nearest, Zero, -Infinity, -Infinity
    static constexpr size_t GuestToHost[] = { 0 << RoundShift, 3 << RoundShift, 1 << RoundShift, 2 << RoundShift };

    inline uint32_t getcsr() noexcept
    {
        uint64_t csr;
        __asm__ __volatile__("mrs %0, fpcr" : "=r"(csr));
        return csr;
    }

    inline void setcsr(uint32_t csr) noexcept
    {
        __asm__ __volatile__("msr fpcr, %0" : : "r"(csr));
    }
#else
#   error "Missing implementation for FPSCR."
#endif

    inline uint32_t loadFromHost() noexcept
    {
        csr = getcsr();
        return HostToGuest[(csr & RoundMask) >> RoundShift];
    }
        
    inline void storeFromGuest(uint32_t value) noexcept
    {
        csr &= ~RoundMask;
        csr |= GuestToHost[value & PPC_ROUND_MASK];
        setcsr(csr);
    }

    inline void enableFlushModeUnconditional() noexcept
    {
        csr |= FlushMask;
        setcsr(csr);
    }

    inline void disableFlushModeUnconditional() noexcept
    {
        csr &= ~FlushMask;
        setcsr(csr);
    }

    inline void enableFlushMode() noexcept
    {
        if ((csr & FlushMask) != FlushMask) [[unlikely]]
        {
            csr |= FlushMask;
            setcsr(csr);
        }
    }

    inline void disableFlushMode() noexcept
    {
        if ((csr & FlushMask) != 0) [[unlikely]]
        {
            csr &= ~FlushMask;
            setcsr(csr);
        }
    }
};

// The non-volatile registers stay members even with PPC_CONFIG_NON_VOLATILE_AS_LOCAL: the functions that keep them in
// the context (SEH parents, funclets and their callers: recompiler.cpp FindNonVolatileInContext) use them.
struct alignas(0x40) PPCContext
{
    PPCRegister r3;
#ifndef PPC_CONFIG_NON_ARGUMENT_AS_LOCAL
    PPCRegister r0;
#endif
    PPCRegister r1;
#ifndef PPC_CONFIG_NON_ARGUMENT_AS_LOCAL
    PPCRegister r2;
#endif
    PPCRegister r4;
    PPCRegister r5;
    PPCRegister r6;
    PPCRegister r7;
    PPCRegister r8;
    PPCRegister r9;
    PPCRegister r10;
#ifndef PPC_CONFIG_NON_ARGUMENT_AS_LOCAL
    PPCRegister r11;
    PPCRegister r12;
#endif
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

#ifndef PPC_CONFIG_SKIP_LR
    uint64_t lr;
#endif
#ifndef PPC_CONFIG_CTR_AS_LOCAL
    PPCRegister ctr;
#endif
#ifndef PPC_CONFIG_XER_AS_LOCAL
    PPCXERRegister xer;
#endif
#ifndef PPC_CONFIG_RESERVED_AS_LOCAL
    PPCRegister reserved;
#endif
#ifndef PPC_CONFIG_SKIP_MSR
    uint32_t msr = 0x200A000;
#endif
#ifndef PPC_CONFIG_CR_AS_LOCAL
    PPCCRRegister cr0;
    PPCCRRegister cr1;
    PPCCRRegister cr2;
    PPCCRRegister cr3;
    PPCCRRegister cr4;
    PPCCRRegister cr5;
    PPCCRRegister cr6;
    PPCCRRegister cr7;
#endif
    PPCFPSCRRegister fpscr;

#ifndef PPC_CONFIG_NON_ARGUMENT_AS_LOCAL
    PPCRegister f0;
#endif
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
#ifndef PPC_CONFIG_NON_ARGUMENT_AS_LOCAL
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
#endif
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

inline uint8_t VectorMaskL[] =
{
    0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00,
    0xFF, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
    0xFF, 0xFF, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02,
    0xFF, 0xFF, 0xFF, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03,
    0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0E, 0x0D, 0x0C,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0E, 0x0D,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0E,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F,
};

inline uint8_t VectorMaskR[] =
{
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x02, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x03, 0x02, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x04, 0x03, 0x02, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
    0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0xFF, 0xFF, 0xFF,
    0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0xFF, 0xFF,
    0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00, 0xFF,
};

inline uint8_t VectorShiftTableL[] =
{
    0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00,
    0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
    0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02,
    0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03,
    0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04,
    0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05,
    0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06,
    0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07,
    0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08,
    0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09,
    0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A,
    0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B,
    0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C,
    0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D,
    0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E,
    0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F,
};

inline uint8_t VectorShiftTableR[] =
{
    0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10,
    0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F,
    0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E,
    0x1C, 0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D,
    0x1B, 0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C,
    0x1A, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B,
    0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A,
    0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09,
    0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08,
    0x16, 0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07,
    0x15, 0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06,
    0x14, 0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05,
    0x13, 0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04,
    0x12, 0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03,
    0x11, 0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02,
    0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01,
};

// ---- Fused multiply-add (fmadd, fmsub, fnmadd, fnmsub and their single forms) ----
// The PowerPC FPU computes frA * frC + frB exactly and rounds once (to double, or to single for the s forms);
// doing the multiply and the add as two operations rounds twice and can differ in the last bit. NaN rules (PowerPC
// architecture, Book I, floating-point chapter): a NaN operand propagates quieted in the order frA, frB, frC; an
// invalid operation (inf * 0, inf - inf) gives the default QNaN 0x7FF8000000000000; fnmadd/fnmsub negate every
// result except a NaN. (The VMX unit's vmaddfp/vnmsubfp are not fused on the console: see the recompiler.)
// ppc_fma / ppc_fmaf must behave as the hardware instruction: the single-precision path relies on it honouring MXCSR
// rounding and setting MXCSR.PE. Built for x86-64-v3 (__FMA__) or ARM64 that is std::fma inline; built for older x86
// CPUs, the hardware instruction where the CPU has it and an exact software FMA otherwise (ppc_soft_fma below,
// checked against the instruction by tools/fma_test).
#if defined(__FMA__) || !(defined(__x86_64__) || defined(_M_X64))
inline double ppc_fma(double a, double c, double b) noexcept { return std::fma(a, c, b); }
inline float ppc_fmaf(float a, float c, float b) noexcept { return std::fmaf(a, c, b); }
#else
// ---- Software FMA (x86 builds below x86-64-v3) ----
// SoftFma computes a * c + b exactly and rounds once to the operands' format, as the hardware instruction does under
// the current MXCSR: its rounding mode, DAZ (denormal operands read as zero) and FTZ (tiny results flushed to zero),
// and it sets MXCSR.PE / UE / OE as the instruction does. NaN payloads follow plain arithmetic (the callers handle
// NaN operands and NaN results themselves). tools/fma_test compares it with vfmadd bit for bit (64 million cases:
// every rounding mode and DAZ / FTZ setting; random operands, exponent edges, cancellations).
#include <bit>
#include <cstdlib>
#include <intrin.h>

namespace ppc_soft_fma
{
    template <typename T> struct Format;
    template <> struct Format<double>
    {
        using Bits = uint64_t;
        static constexpr int kMant = 52, kExpBits = 11, kBias = 1023;
    };
    template <> struct Format<float>
    {
        using Bits = uint32_t;
        static constexpr int kMant = 23, kExpBits = 8, kBias = 127;
    };

    using u128 = unsigned __int128;

    inline int HighBit(u128 x) noexcept // x != 0
    {
        const uint64_t hi = uint64_t(x >> 64);
        return hi ? 127 - std::countl_zero(hi) : 63 - std::countl_zero(uint64_t(x));
    }

    // x >> d with every shifted-out bit ORed into bit 0 ("jamming"): the result is x / 2^d rounded to odd, which keeps
    // the later rounding exact because the data never reaches bit 0 unless it was shifted (ppc_soft_fma notes).
    inline u128 ShiftRightJam(u128 x, int d) noexcept
    {
        if (d <= 0)
            return x;
        if (d >= 128)
            return x != 0;
        return (x >> d) | u128((x & ((u128(1) << d) - 1)) != 0);
    }

    template <typename T>
    inline T SoftFma(T a, T c, T b) noexcept
    {
        using F = Format<T>;
        using Bits = typename F::Bits;
        constexpr int kExpMax = (1 << F::kExpBits) - 1;
        constexpr int kMinUlp = 1 - F::kBias - F::kMant; // exponent of the smallest denormal: -1074 / -149
        constexpr int kMinNormal = 1 - F::kBias;          // -1022 / -126

        const uint32_t csr = _mm_getcsr();
        const bool daz = (csr & 0x40) != 0, ftz = (csr & 0x8000) != 0;
        const uint32_t mode = (csr >> 13) & 3; // 0 nearest, 1 down, 2 up, 3 toward zero

        // value = (-1)^neg * m * 2^e; returns true for infinities and NaNs.
        auto parts = [&](T x, bool& neg, uint64_t& m, int& e) {
            const Bits bits = std::bit_cast<Bits>(x);
            neg = (bits >> (sizeof(Bits) * 8 - 1)) != 0;
            const int exp = int((bits >> F::kMant) & kExpMax);
            const uint64_t frac = uint64_t(bits & ((Bits(1) << F::kMant) - 1));
            if (exp == 0)
            {
                m = daz ? 0 : frac;
                e = kMinUlp;
            }
            else
            {
                m = frac | (uint64_t(1) << F::kMant);
                e = exp - F::kBias - F::kMant;
            }
            return exp == kExpMax;
        };
        bool na, nc, nb;
        uint64_t ma, mc, mb;
        int ea, ec, eb;
        const bool specialA = parts(a, na, ma, ea), specialC = parts(c, nc, mc, ec), specialB = parts(b, nb, mb, eb);
        if (specialA || specialC || specialB)
        {
            if (!specialA && !specialC)
                return b; // a finite product plus an infinity or NaN
            return a * c + b; // an infinite or NaN factor: the product is exact (inf, or NaN), as is the sum
        }

        auto setFlags = [](uint32_t flags) { _mm_setcsr(_mm_getcsr() | flags); };
        const bool productNeg = na != nc;
        bool neg;
        u128 sum;
        int sumExp;
        // Both values with their top bit at bit 124 (3 bits of headroom), then the smaller aligned to the larger.
        u128 product = 0;
        int productExp = 0;
        if (ma != 0 && mc != 0)
        {
            product = u128(ma) * mc;
            const int shift = 124 - HighBit(product);
            product <<= shift;
            productExp = ea + ec - shift;
        }
        u128 addend = 0;
        int addendExp = 0;
        if (mb != 0)
        {
            addend = mb;
            const int shift = 124 - HighBit(addend);
            addend <<= shift;
            addendExp = eb - shift;
        }
        if (product == 0 && addend == 0)
        {
            // Zero plus zero: the sign rules of an ordinary addition.
            const T zp = productNeg ? -T(0) : T(0), zb = nb ? -T(0) : T(0);
            return zp + zb;
        }
        if (addend == 0)
        {
            sum = product;
            sumExp = productExp;
            neg = productNeg;
        }
        else if (product == 0)
        {
            sum = addend;
            sumExp = addendExp;
            neg = nb;
        }
        else
        {
            if (productExp >= addendExp)
            {
                addend = ShiftRightJam(addend, productExp - addendExp);
                sumExp = productExp;
            }
            else
            {
                product = ShiftRightJam(product, addendExp - productExp);
                sumExp = addendExp;
            }
            if (productNeg == nb)
            {
                sum = product + addend;
                neg = nb;
            }
            else if (product >= addend)
            {
                sum = product - addend;
                neg = productNeg;
            }
            else
            {
                sum = addend - product;
                neg = nb;
            }
            if (sum == 0)
                return mode == 1 ? -T(0) : T(0); // exact cancellation: +0, -0 when rounding down
        }

        // Round sum * 2^sumExp to a quantum of 2^ulpExp.
        auto roundAt = [&](int ulpExp, bool& inexact) -> uint64_t {
            const int r = ulpExp - sumExp;
            if (r <= 0)
            {
                inexact = false;
                return uint64_t(sum << -r);
            }
            uint64_t q;
            bool above, half;
            if (r >= 128)
            {
                q = 0;
                inexact = true;
                above = false;
                half = false;
            }
            else
            {
                q = uint64_t(sum >> r);
                const u128 rem = sum & ((u128(1) << r) - 1), halfway = u128(1) << (r - 1);
                inexact = rem != 0;
                above = rem > halfway;
                half = rem == halfway;
            }
            bool up = false;
            switch (mode)
            {
            case 0: up = above || (half && (q & 1)); break;
            case 1: up = inexact && neg; break;
            case 2: up = inexact && !neg; break;
            default: break;
            }
            return q + up;
        };

        const int exponent = HighBit(sum) + sumExp; // value in [2^exponent, 2^(exponent + 1))
        const int ulpExp = exponent - F::kMant > kMinUlp ? exponent - F::kMant : kMinUlp;
        bool inexact;
        const uint64_t q = roundAt(ulpExp, inexact);
        const Bits sign = neg ? Bits(1) << (sizeof(Bits) * 8 - 1) : 0;
        // The quantum count over the field: an implicit bit or a carry to 2^(kMant + 1) moves the exponent field.
        const uint64_t magnitude = (uint64_t(ulpExp - kMinUlp) << F::kMant) + q;
        if (magnitude >= (uint64_t(kExpMax) << F::kMant))
        {
            setFlags(0x28); // overflow + inexact
            const Bits largest = (Bits(kExpMax - 1) << F::kMant) | ((Bits(1) << F::kMant) - 1);
            const Bits infinity = Bits(kExpMax) << F::kMant;
            const bool toInfinity = mode == 0 || (mode == 1 && neg) || (mode == 2 && !neg);
            return std::bit_cast<T>(Bits(sign | (toInfinity ? infinity : largest)));
        }
        if (exponent < kMinNormal)
        {
            // Tiny (x86 decides after rounding, with an unbounded exponent): only a value just below the smallest
            // normal can round up out of it.
            bool tiny = true;
            if (exponent == kMinNormal - 1)
            {
                bool unused;
                tiny = roundAt(exponent - F::kMant, unused) < (uint64_t(1) << (F::kMant + 1));
            }
            if (tiny && ftz)
            {
                setFlags(0x30); // underflow + inexact
                return std::bit_cast<T>(sign);
            }
            if (tiny && inexact)
                setFlags(0x10);
        }
        if (inexact)
            setFlags(0x20);
        return std::bit_cast<T>(Bits(sign | Bits(magnitude)));
    }

    // FMA3 present and enabled by the OS (AVX state saved). PPC_SOFT_FMA=1 forces the software path (tests).
    __attribute__((target("xsave"))) inline bool DetectFma() noexcept
    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        if (const char* force = std::getenv("PPC_SOFT_FMA"); force && force[0] == '1')
            return false;
#pragma clang diagnostic pop
        int r[4];
        __cpuid(r, 1);
        const bool fma = (r[2] >> 12) & 1, osxsave = (r[2] >> 27) & 1, avx = (r[2] >> 28) & 1;
        return fma && osxsave && avx && (_xgetbv(0) & 6) == 6;
    }
}

inline const bool ppc_cpu_has_fma = ppc_soft_fma::DetectFma();

__attribute__((target("fma"))) inline double ppc_fma_hardware(double a, double c, double b) noexcept { return __builtin_fma(a, c, b); }
__attribute__((target("fma"))) inline float ppc_fmaf_hardware(float a, float c, float b) noexcept { return __builtin_fmaf(a, c, b); }
inline double ppc_fma(double a, double c, double b) noexcept
{
    return ppc_cpu_has_fma ? ppc_fma_hardware(a, c, b) : ppc_soft_fma::SoftFma(a, c, b);
}
inline float ppc_fmaf(float a, float c, float b) noexcept
{
    return ppc_cpu_has_fma ? ppc_fmaf_hardware(a, c, b) : ppc_soft_fma::SoftFma(a, c, b);
}
#endif

inline double ppc_bits_to_double(uint64_t bits) noexcept
{
    double d;
    std::memcpy(&d, &bits, sizeof(d));
    return d;
}

inline uint64_t ppc_double_to_bits(double d) noexcept
{
    uint64_t bits;
    std::memcpy(&bits, &d, sizeof(bits));
    return bits;
}

inline bool ppc_fma_nan_operand(double a, double c, double b, double& result) noexcept
{
    const double* nan = a != a ? &a : b != b ? &b : c != c ? &c : nullptr;
    if (nan == nullptr)
        return false;
    result = ppc_bits_to_double(ppc_double_to_bits(*nan) | 0x0008000000000000ull);
    return true;
}

// a * c + b, one rounding to double; negate: fnmadd form.
inline double ppc_fmadd(double a, double c, double b, bool negate = false) noexcept
{
    double r;
    if (ppc_fma_nan_operand(a, c, b, r))
        return r;
    r = ppc_fma(a, c, b);
    if (r != r)
        return ppc_bits_to_double(0x7FF8000000000000ull);
    return negate ? -r : r;
}

// a * c - b; negate: fnmsub form (-(a * c - b), so an exact zero difference gives -0).
inline double ppc_fmsub(double a, double c, double b, bool negate = false) noexcept
{
    double r;
    if (ppc_fma_nan_operand(a, c, b, r))
        return r;
    r = ppc_fma(a, c, -b);
    if (r != r)
        return ppc_bits_to_double(0x7FF8000000000000ull);
    return negate ? -r : r;
}

// a * c + b rounded once to single (the operands are doubles; usually they are exact singles).
inline double ppc_fma_single(double a, double c, double b) noexcept
{
    const float fa = float(a), fc = float(c), fb = float(b);
    if (double(fa) == a && double(fc) == c && double(fb) == b)
        return double(ppc_fmaf(fa, fc, fb)); // the single FMA rounds the exact result once
    // Other doubles: round to odd in double (truncate, then set the last bit if inexact), then round to single
    // in the current mode. 53 >= 2 * 24 + 2 bits, so this equals one rounding of the exact value.
    const uint32_t csr = _mm_getcsr();
    _mm_setcsr((csr & ~0x603Fu) | 0x6000u); // round toward zero, clear the sticky flags
    double t = ppc_fma(a, c, b);
    const bool inexact = (_mm_getcsr() & 0x20) != 0;
    _mm_setcsr(csr);
    if (inexact)
        t = ppc_bits_to_double(ppc_double_to_bits(t) | 1);
    return double(float(t));
}

inline double ppc_fmadds(double a, double c, double b, bool negate = false) noexcept
{
    double r;
    if (ppc_fma_nan_operand(a, c, b, r))
        return double(float(r));
    r = ppc_fma_single(a, c, b);
    if (r != r)
        return ppc_bits_to_double(0x7FF8000000000000ull);
    return negate ? -r : r;
}

inline double ppc_fmsubs(double a, double c, double b, bool negate = false) noexcept
{
    double r;
    if (ppc_fma_nan_operand(a, c, b, r))
        return double(float(r));
    r = ppc_fma_single(a, c, -b);
    if (r != r)
        return ppc_bits_to_double(0x7FF8000000000000ull);
    return negate ? -r : r;
}

inline simde__m128i simde_mm_adds_epu32(simde__m128i a, simde__m128i b)
{
    return simde_mm_add_epi32(a, simde_mm_min_epu32(simde_mm_xor_si128(a, simde_mm_cmpeq_epi32(a, a)), b));
}

inline simde__m128i simde_mm_avg_epi8(simde__m128i a, simde__m128i b)
{
    simde__m128i c = simde_mm_set1_epi8(char(128));
    return simde_mm_xor_si128(c, simde_mm_avg_epu8(simde_mm_xor_si128(c, a), simde_mm_xor_si128(c, b)));
}

inline simde__m128i simde_mm_avg_epi16(simde__m128i a, simde__m128i b)
{
    simde__m128i c = simde_mm_set1_epi16(short(32768));
    return simde_mm_xor_si128(c, simde_mm_avg_epu16(simde_mm_xor_si128(c, a), simde_mm_xor_si128(c, b)));
}

inline simde__m128 simde_mm_cvtepu32_ps_(simde__m128i src1)
{
    simde__m128i xmm1 = simde_mm_add_epi32(src1, simde_mm_set1_epi32(127));
    simde__m128i xmm0 = simde_mm_slli_epi32(src1, 31 - 8);
    xmm0 = simde_mm_srli_epi32(xmm0, 31);
    xmm0 = simde_mm_add_epi32(xmm0, xmm1);
    xmm0 = simde_mm_srai_epi32(xmm0, 8);
    xmm0 = simde_mm_add_epi32(xmm0, simde_mm_set1_epi32(0x4F800000));
    simde__m128 xmm2 = simde_mm_cvtepi32_ps(src1);
    return simde_mm_blendv_ps(xmm2, simde_mm_castsi128_ps(xmm0), simde_mm_castsi128_ps(src1));
}

inline simde__m128i simde_mm_perm_epi8_(simde__m128i a, simde__m128i b, simde__m128i c)
{
    simde__m128i d = simde_mm_set1_epi8(0xF);
    simde__m128i e = simde_mm_sub_epi8(d, simde_mm_and_si128(c, d));
    return simde_mm_blendv_epi8(simde_mm_shuffle_epi8(a, e), simde_mm_shuffle_epi8(b, e), simde_mm_slli_epi32(c, 3));
}

inline simde__m128i simde_mm_cmpgt_epu8(simde__m128i a, simde__m128i b)
{
    simde__m128i c = simde_mm_set1_epi8(char(128));
    return simde_mm_cmpgt_epi8(simde_mm_xor_si128(a, c), simde_mm_xor_si128(b, c));
}

inline simde__m128i simde_mm_cmpgt_epu16(simde__m128i a, simde__m128i b)
{
    simde__m128i c = simde_mm_set1_epi16(short(32768));
    return simde_mm_cmpgt_epi16(simde_mm_xor_si128(a, c), simde_mm_xor_si128(b, c));
}

inline simde__m128i simde_mm_vctsxs(simde__m128 src1)
{
    simde__m128 xmm2 = simde_mm_cmpunord_ps(src1, src1);
    simde__m128i xmm0 = simde_mm_cvttps_epi32(src1);
    simde__m128i xmm1 = simde_mm_cmpeq_epi32(xmm0, simde_mm_set1_epi32(INT_MIN));
    xmm1 = simde_mm_andnot_si128(simde_mm_castps_si128(src1), xmm1);
    simde__m128 dest = simde_mm_blendv_ps(simde_mm_castsi128_ps(xmm0), simde_mm_castsi128_ps(simde_mm_set1_epi32(INT_MAX)), simde_mm_castsi128_ps(xmm1));
    return simde_mm_andnot_si128(simde_mm_castps_si128(xmm2), simde_mm_castps_si128(dest));
}

// vctuxs / vcfpuxws128: float -> uint32 with truncation, saturating to [0, UINT32_MAX]; NaN becomes 0.
inline simde__m128i simde_mm_vctuxs(simde__m128 src1)
{
    alignas(16) float in[4];
    alignas(16) uint32_t out[4];
    simde_mm_store_ps(in, src1);
    for (size_t i = 0; i < 4; i++)
    {
        if (__builtin_isnan(in[i]) || in[i] <= 0.0f)
            out[i] = 0;
        else if (in[i] >= 4294967296.0f)
            out[i] = UINT32_MAX;
        else
            out[i] = uint32_t(in[i]);
    }
    return simde_mm_load_si128((simde__m128i*)out);
}

// ---- Xbox 360 VMX128 D3D pack/unpack and vcmpbfp -------------------------------------------------------------
// Vectors are stored element-reversed: PPC element e (x=0 .. w=3) lives in u32[3 - e].
// Semantics checked against Xenia's hardware-derived tests (src/xenia/cpu/ppc/testing). The 16-bit float format
// is the Xbox 360 one used by the XDK's XMConvertFloatToHalf: no infinities or NaNs (exponent 31 is an ordinary
// range up to 131008), values out of range saturate to 0x7FFF, rounding to nearest even.

inline uint32_t ppc_velem(const PPCVRegister& v, size_t e) { return v.u32[3 - e]; }
inline void ppc_velem_set(PPCVRegister& v, size_t e, uint32_t value) { v.u32[3 - e] = value; }

inline float ppc_bits_to_float(uint32_t bits) { float f; memcpy(&f, &bits, 4); return f; }
inline uint32_t ppc_float_to_bits(float f) { uint32_t bits; memcpy(&bits, &f, 4); return bits; }

// Clamp like (v)maxps then (v)minps: a NaN input yields the lower bound.
inline uint32_t ppc_clamp_float_bits(uint32_t bits, uint32_t lo, uint32_t hi)
{
    float v = ppc_bits_to_float(bits);
    float l = ppc_bits_to_float(lo), h = ppc_bits_to_float(hi);
    v = v > l ? v : l;
    v = v < h ? v : h;
    return ppc_float_to_bits(v);
}

inline uint16_t ppc_float_to_half360(uint32_t bits)
{
    uint32_t sign = (bits >> 16) & 0x8000;
    uint32_t value = bits & 0x7FFFFFFF;
    if (value > 0x47FFEFFF) // too large (includes infinities and NaNs): saturate
        return uint16_t(sign | 0x7FFF);
    if (value < 0x38800000) // below the smallest normal half: produce a denormal (or zero)
    {
        uint32_t shift = 113 - (value >> 23);
        value = shift < 32 ? (0x800000 | (value & 0x7FFFFF)) >> shift : 0;
    }
    else
    {
        value -= 112u << 23; // rebias the exponent from 127 to 15
    }
    return uint16_t(sign | (((value + 0x0FFF + ((value >> 13) & 1)) >> 13) & 0x7FFF));
}

inline uint32_t ppc_half360_to_float(uint16_t half)
{
    uint32_t sign = uint32_t(half & 0x8000) << 16;
    int32_t exponent = (half >> 10) & 0x1F;
    uint32_t mantissa = half & 0x3FF;
    if (exponent == 0)
    {
        if (mantissa == 0)
            return sign;
        exponent = 1; // normalize the denormal
        while (!(mantissa & 0x400))
        {
            mantissa <<= 1;
            exponent--;
        }
        mantissa &= 0x3FF;
    }
    return sign | (uint32_t(exponent + 112) << 23) | (mantissa << 13);
}

// Returns false for a pack type / mode the helper does not implement.
inline bool ppc_vpkd3d128(PPCVRegister& vd, const PPCVRegister& vb, uint32_t type, uint32_t pack, uint32_t shift)
{
    uint32_t packed[4] = {};
    uint32_t x = ppc_velem(vb, 0), y = ppc_velem(vb, 1), z = ppc_velem(vb, 2), w = ppc_velem(vb, 3);
    switch (type)
    {
    case 0: // D3DCOLOR: 3.0 + n * 2^-22 per channel -> ARGB in the last word
    {
        auto c = [](uint32_t v) { return ppc_clamp_float_bits(v, 0x40400000, 0x404000FF) & 0xFF; };
        packed[3] = (c(w) << 24) | (c(x) << 16) | (c(y) << 8) | c(z);
        break;
    }
    case 1: // NORMSHORT2
    case 4: // NORMSHORT4
    {
        auto s = [](uint32_t v) { return ppc_clamp_float_bits(v, 0x403F8001, 0x40407FFF) & 0xFFFF; };
        if (type == 1)
            packed[3] = (s(x) << 16) | s(y);
        else
        {
            packed[2] = (s(x) << 16) | s(y);
            packed[3] = (s(z) << 16) | s(w);
        }
        break;
    }
    case 2: // NORMPACKED32 2_10_10_10 (w_z_y_x)
    {
        uint32_t px = ppc_clamp_float_bits(x, 0x403FFE01, 0x404001FF) & 0x3FF;
        uint32_t py = ppc_clamp_float_bits(y, 0x403FFE01, 0x404001FF) & 0x3FF;
        uint32_t pz = ppc_clamp_float_bits(z, 0x403FFE01, 0x404001FF) & 0x3FF;
        uint32_t pw = ppc_clamp_float_bits(w, 0x40400000, 0x40400003) & 0x3;
        uint32_t value = px | (py << 10) | (pz << 20) | (pw << 30);
        packed[0] = packed[1] = packed[2] = packed[3] = value;
        break;
    }
    case 3: // FLOAT16_2
        packed[3] = (uint32_t(ppc_float_to_half360(x)) << 16) | ppc_float_to_half360(y);
        break;
    case 5: // FLOAT16_4
        packed[2] = (uint32_t(ppc_float_to_half360(x)) << 16) | ppc_float_to_half360(y);
        packed[3] = (uint32_t(ppc_float_to_half360(z)) << 16) | ppc_float_to_half360(w);
        break;
    default:
        return false;
    }

    // Merge into vD. Each entry selects (source, element) for result elements x..w: 0 = previous vD, 1 = packed.
    static constexpr uint8_t kMerge[4][4][4][2] =
    {
        {}, // pack 0 is not a valid mode
        { // VPACK_32
            { {0,0}, {0,1}, {0,2}, {1,3} }, { {0,0}, {0,1}, {1,3}, {0,3} },
            { {0,0}, {1,3}, {0,2}, {0,3} }, { {1,3}, {0,1}, {0,2}, {0,3} },
        },
        { // VPACK_64LO
            { {0,0}, {0,1}, {1,2}, {1,3} }, { {0,0}, {1,2}, {1,3}, {0,3} },
            { {1,2}, {1,3}, {0,2}, {0,3} }, { {1,3}, {0,1}, {0,2}, {0,3} },
        },
        { // VPACK_64HI
            { {0,0}, {0,1}, {1,2}, {1,3} }, { {0,0}, {1,2}, {1,3}, {0,3} },
            { {1,2}, {1,3}, {0,2}, {0,3} }, { {0,0}, {0,1}, {0,2}, {1,2} },
        },
    };
    if (pack == 0 || pack > 3 || shift > 3)
        return false;
    PPCVRegister result;
    for (size_t e = 0; e < 4; e++)
    {
        auto [src, index] = kMerge[pack][shift][e];
        ppc_velem_set(result, e, src ? packed[index] : ppc_velem(vd, index));
    }
    vd = result;
    return true;
}

// Returns false for an unpack type the helper does not implement.
inline bool ppc_vupkd3d128(PPCVRegister& vd, const PPCVRegister& vb, uint32_t type)
{
    constexpr uint32_t kOne = 0x3F800000, kThree = 0x40400000, kQNaN = 0x7FC00000;
    uint32_t lo = ppc_velem(vb, 2), hi = ppc_velem(vb, 3);
    uint32_t out[4];
    auto sext = [](uint32_t value, int bits) { return uint32_t(int32_t(value << (32 - bits)) >> (32 - bits)); };
    // Integer formats are biased onto 3.0; the most negative value (one below the range) becomes a quiet NaN.
    auto biased = [&](uint32_t value, int bits, uint32_t overflow)
        {
            uint32_t r = kThree + sext(value, bits);
            return r == overflow ? kQNaN : r;
        };
    switch (type)
    {
    case 0: // D3DCOLOR: ARGB -> 1.0 + n * 2^-23 per channel, in xyzw = RGBA order
        out[0] = kOne | ((hi >> 16) & 0xFF);
        out[1] = kOne | ((hi >> 8) & 0xFF);
        out[2] = kOne | (hi & 0xFF);
        out[3] = kOne | (hi >> 24);
        break;
    case 1: // NORMSHORT2
        out[0] = biased(hi >> 16, 16, 0x403F8000);
        out[1] = biased(hi & 0xFFFF, 16, 0x403F8000);
        out[2] = 0;
        out[3] = kOne;
        break;
    case 4: // NORMSHORT4
        out[0] = biased(lo >> 16, 16, 0x403F8000);
        out[1] = biased(lo & 0xFFFF, 16, 0x403F8000);
        out[2] = biased(hi >> 16, 16, 0x403F8000);
        out[3] = biased(hi & 0xFFFF, 16, 0x403F8000);
        break;
    case 2: // NORMPACKED32 2_10_10_10: xyz signed 10-bit, w unsigned 2-bit
        out[0] = biased(hi & 0x3FF, 10, 0x403FFE00);
        out[1] = biased((hi >> 10) & 0x3FF, 10, 0x403FFE00);
        out[2] = biased((hi >> 20) & 0x3FF, 10, 0x403FFE00);
        out[3] = kOne + (hi >> 30);
        break;
    case 3: // FLOAT16_2
        out[0] = ppc_half360_to_float(uint16_t(hi >> 16));
        out[1] = ppc_half360_to_float(uint16_t(hi));
        out[2] = 0;
        out[3] = kOne;
        break;
    case 5: // FLOAT16_4
        out[0] = ppc_half360_to_float(uint16_t(lo >> 16));
        out[1] = ppc_half360_to_float(uint16_t(lo));
        out[2] = ppc_half360_to_float(uint16_t(hi >> 16));
        out[3] = ppc_half360_to_float(uint16_t(hi));
        break;
    default:
        return false;
    }
    for (size_t e = 0; e < 4; e++)
        ppc_velem_set(vd, e, out[e]);
    return true;
}

// vcmpbfp: bit 31 set when !(a <= b), bit 30 set when !(a >= -b); NaN sets both. Returns true when every
// element is within bounds (all zero), which is what the record form puts in CR6[eq].
inline bool ppc_vcmpbfp(PPCVRegister& vd, const PPCVRegister& va, const PPCVRegister& vb)
{
    PPCVRegister result;
    for (size_t i = 0; i < 4; i++)
    {
        float a = va.f32[i], b = vb.f32[i];
        result.u32[i] = (!(a <= b) ? 0x80000000u : 0) | (!(a >= -b) ? 0x40000000u : 0);
    }
    vd = result;
    return (result.u64[0] | result.u64[1]) == 0;
}

inline simde__m128i simde_mm_vsr(simde__m128i a, simde__m128i b)
{
    b = simde_mm_srli_epi64(simde_mm_slli_epi64(b, 61), 61);
    return simde_mm_castps_si128(simde_mm_insert_ps(simde_mm_castsi128_ps(simde_mm_srl_epi64(a, b)), simde_mm_castsi128_ps(simde_mm_srl_epi64(simde_mm_srli_si128(a, 4), b)), 0x10));
}

#if defined(__aarch64__) || defined(_M_ARM64)
inline uint64_t __rdtsc()
{
    uint64_t ret;
    asm volatile("mrs %0, cntvct_el0\n\t"
                 : "=r"(ret)::"memory");
    return ret;
}
#elif !defined(__x86_64__) && !defined(_M_X64)
#   error "Missing implementation for __rdtsc()"
#endif

#endif
