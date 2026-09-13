// RT360 Runtime Header
// Defines the ABI for statically recompiled Xbox 360 PPC code.
// Binary-compatible with Nexia360's PPCContext_s struct.
//
// When building inside Nexia360: typedef PPCContext_s RT360Context
// When building standalone: uses the mirrored struct below.

#pragma once

#include <stdint.h>
#include <string.h>

#if defined(_MSC_VER) && !defined(__clang__)
// Pure MSVC
#include <stdlib.h>
#include <intrin.h>
#define RT360_EXPORT __declspec(dllexport)
#define RT360_IMPORT __declspec(dllimport)
#define RT360_INLINE __forceinline
#define RT360_BSWAP16(x) _byteswap_ushort(x)
#define RT360_BSWAP32(x) _byteswap_ulong(x)
#define RT360_BSWAP64(x) _byteswap_uint64(x)
#elif defined(_WIN32)
// Clang/MinGW targeting Windows
#define RT360_EXPORT __declspec(dllexport)
#define RT360_IMPORT __declspec(dllimport)
#define RT360_INLINE __attribute__((always_inline)) inline
#define RT360_BSWAP16(x) __builtin_bswap16(x)
#define RT360_BSWAP32(x) __builtin_bswap32(x)
#define RT360_BSWAP64(x) __builtin_bswap64(x)
#else
// Linux / other
#define RT360_EXPORT __attribute__((visibility("default")))
#define RT360_IMPORT
#define RT360_INLINE __attribute__((always_inline)) inline
#define RT360_BSWAP16(x) __builtin_bswap16(x)
#define RT360_BSWAP32(x) __builtin_bswap32(x)
#define RT360_BSWAP64(x) __builtin_bswap64(x)
#endif

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
// RT360 Context — mirrors Nexia360 PPCContext_s field-for-field
// ---------------------------------------------------------------------------

#ifdef RT360_USE_NEXIA360_CONTEXT
// When building inside Nexia360, use the real PPCContext
#include "xenia/cpu/ppc/ppc_context.h"
typedef xe::cpu::ppc::PPCContext_s RT360Context;
#else

// 128-bit vector type (matches xe::vec128_t — MUST be 16-byte aligned)
#ifdef _MSC_VER
__declspec(align(16))
#endif
typedef union
#ifdef __GNUC__
__attribute__((aligned(16)))
#endif
{
    float    f32[4];
    double   f64[2];
    uint8_t  u8[16];
    uint16_t u16[8];
    uint32_t u32[4];
    uint64_t u64[2];
    int32_t  i32[4];
} RT360Vec128;

// Condition register field
typedef union {
    uint32_t value;
    struct {
        uint8_t lt;  // bit 0: less than / negative
        uint8_t gt;  // bit 1: greater than / positive
        uint8_t eq;  // bit 2: equal / zero
        uint8_t so;  // bit 3: summary overflow
    };
} RT360CRField;

// FPSCR
typedef union {
    uint32_t value;
    struct {
        uint32_t rn      : 2;
        uint32_t ni      : 1;
        uint32_t xe      : 1;
        uint32_t ze      : 1;
        uint32_t ue      : 1;
        uint32_t oe      : 1;
        uint32_t ve      : 1;
        uint32_t vxcvi   : 1;
        uint32_t vxsqrt  : 1;
        uint32_t vxsoft  : 1;
        uint32_t _rsv    : 1;
        uint32_t fprf_un : 1;
        uint32_t fprf_eq : 1;
        uint32_t fprf_gt : 1;
        uint32_t fprf_lt : 1;
        uint32_t fprf_c  : 1;
        uint32_t fi      : 1;
        uint32_t fr      : 1;
        uint32_t vxvc    : 1;
        uint32_t vximz   : 1;
        uint32_t vxzdz   : 1;
        uint32_t vxidi   : 1;
        uint32_t vxisi   : 1;
        uint32_t vxsnan  : 1;
        uint32_t xx      : 1;
        uint32_t zx      : 1;
        uint32_t ux      : 1;
        uint32_t ox      : 1;
        uint32_t vx      : 1;
        uint32_t fex     : 1;
        uint32_t fx      : 1;
    } bits;
} RT360FPSCR;

// Forward declarations for opaque Nexia360 pointers
typedef void RT360Processor;
typedef void RT360KernelState;
typedef void RT360ThreadState;
typedef void RT360GlobalMutex;

#pragma pack(push, 8)
typedef struct RT360Context {
    // Condition registers (at offset matching PPCContext_s)
    RT360CRField cr0;
    RT360CRField cr1;
    RT360CRField cr2;
    RT360CRField cr3;
    RT360CRField cr4;
    RT360CRField cr5;
    RT360CRField cr6;
    RT360CRField cr7;

    RT360FPSCR fpscr;

    // General purpose registers
    uint64_t r[32];
    uint64_t ctr;
    uint64_t lr;
    uint64_t msr;

    // Floating point registers
    double f[32];

    // Vector registers (VMX128)
    RT360Vec128 v[128];
    RT360Vec128 vscr_vec;

    // XER split
    uint8_t xer_ca;
    uint8_t xer_ov;
    uint8_t xer_so;

    uint8_t vscr_sat;
    uint32_t vrsave;

    // Thread context
    uint32_t thread_id;
    RT360GlobalMutex* global_mutex;
    uint64_t scratch;
    RT360Processor* processor;
    RT360KernelState* kernel_state;

    // Memory bases
    uint8_t* physical_membase;
    uint64_t reserved_val;
    RT360ThreadState* thread_state;
    uint8_t* virtual_membase;
} RT360Context;
#pragma pack(pop)

#endif  // RT360_USE_NEXIA360_CONTEXT

// ---------------------------------------------------------------------------
// Function table types
// ---------------------------------------------------------------------------

typedef void (*RT360FuncPtr)(RT360Context* ctx);

typedef struct {
    uint32_t guest_addr;
    uint32_t guest_end;  // exclusive end address of this function
    RT360FuncPtr func;
    const char* name;
} RT360FuncEntry;

typedef struct {
    const char* module_name;
    uint32_t base_addr;
    uint32_t size;
    uint32_t entry_point;
    uint32_t func_count;
    RT360FuncEntry* functions;
} RT360ModuleInfo;

// ---------------------------------------------------------------------------
// Kernel dispatch table types
// ---------------------------------------------------------------------------

// Matches Nexia's ExternHandler: void (*)(PPCContext*, KernelState*)
typedef void (*RT360KernelHandler)(RT360Context* ctx, void* kernel_state);

typedef struct {
    const char*         module_name;  // "xboxkrnl.exe" or "xam.xex"
    uint32_t            ordinal;
    RT360KernelHandler  handler;
} RT360KernelExport;

// ---------------------------------------------------------------------------
// External dispatch (implemented by Nexia360 or a test harness)
// ---------------------------------------------------------------------------

extern void rt360_call(RT360Context* ctx, uint32_t target_addr);

// ---------------------------------------------------------------------------
// Memory access helpers (big-endian byte-swap for Xbox 360 guest memory)
// ---------------------------------------------------------------------------

// Host pointer from guest virtual address.
// Matches Nexia360's Memory::TranslateVirtual: addresses >= 0xE0000000
// have a 0x1000 byte offset on Windows to avoid page-zero issues.
static RT360_INLINE uint8_t* rt360_memptr(RT360Context* ctx, uint32_t addr) {
    uint8_t* host = ctx->virtual_membase + addr;
#if defined(_WIN32)
    if (addr >= 0xE0000000u) {
        host += 0x1000;
    }
#endif
    return host;
}

// Host pointer from guest physical address (mask to 512MB range).
// Matches Nexia360's Memory::TranslatePhysical.
static RT360_INLINE uint8_t* rt360_physptr(RT360Context* ctx, uint32_t addr) {
    return ctx->physical_membase + (addr & 0x1FFFFFFFu);
}

// Load unsigned
static RT360_INLINE uint8_t rt360_load8(RT360Context* ctx, uint32_t addr) {
    return *(uint8_t*)rt360_memptr(ctx, addr);
}

static RT360_INLINE uint16_t rt360_load16(RT360Context* ctx, uint32_t addr) {
    return RT360_BSWAP16(*(uint16_t*)rt360_memptr(ctx, addr));
}

static RT360_INLINE uint32_t rt360_load32(RT360Context* ctx, uint32_t addr) {
    return RT360_BSWAP32(*(uint32_t*)rt360_memptr(ctx, addr));
}

static RT360_INLINE uint64_t rt360_load64(RT360Context* ctx, uint32_t addr) {
    return RT360_BSWAP64(*(uint64_t*)rt360_memptr(ctx, addr));
}

// Load signed
static RT360_INLINE int8_t rt360_load_s8(RT360Context* ctx, uint32_t addr) {
    return (int8_t)rt360_load8(ctx, addr);
}

static RT360_INLINE int16_t rt360_load_s16(RT360Context* ctx, uint32_t addr) {
    return (int16_t)rt360_load16(ctx, addr);
}

// Load float
static RT360_INLINE float rt360_loadf32(RT360Context* ctx, uint32_t addr) {
    uint32_t raw = rt360_load32(ctx, addr);
    float result;
    memcpy(&result, &raw, sizeof(float));
    return result;
}

static RT360_INLINE double rt360_loadf64(RT360Context* ctx, uint32_t addr) {
    uint64_t raw = rt360_load64(ctx, addr);
    double result;
    memcpy(&result, &raw, sizeof(double));
    return result;
}

// Store unsigned
static RT360_INLINE void rt360_store8(RT360Context* ctx, uint32_t addr, uint8_t val) {
    *(uint8_t*)rt360_memptr(ctx, addr) = val;
}

static RT360_INLINE void rt360_store16(RT360Context* ctx, uint32_t addr, uint16_t val) {
    *(uint16_t*)rt360_memptr(ctx, addr) = RT360_BSWAP16(val);
}

static RT360_INLINE void rt360_store32(RT360Context* ctx, uint32_t addr, uint32_t val) {
    *(uint32_t*)rt360_memptr(ctx, addr) = RT360_BSWAP32(val);
}

static RT360_INLINE void rt360_store64(RT360Context* ctx, uint32_t addr, uint64_t val) {
    *(uint64_t*)rt360_memptr(ctx, addr) = RT360_BSWAP64(val);
}

// Store float
static RT360_INLINE void rt360_storef32(RT360Context* ctx, uint32_t addr, float val) {
    uint32_t raw;
    memcpy(&raw, &val, sizeof(uint32_t));
    rt360_store32(ctx, addr, raw);
}

static RT360_INLINE void rt360_storef64(RT360Context* ctx, uint32_t addr, double val) {
    uint64_t raw;
    memcpy(&raw, &val, sizeof(uint64_t));
    rt360_store64(ctx, addr, raw);
}

// ---------------------------------------------------------------------------
// Byte-reverse load/store (lwbrx, stwbrx — load little-endian from BE memory)
// ---------------------------------------------------------------------------

static RT360_INLINE uint16_t rt360_load16_reverse(RT360Context* ctx, uint32_t addr) {
    // lwbrx loads in reverse byte order relative to guest (big-endian)
    // Since guest mem is BE on host, a non-swapped load gives LE = reversed
    return *(uint16_t*)rt360_memptr(ctx, addr);
}

static RT360_INLINE uint32_t rt360_load32_reverse(RT360Context* ctx, uint32_t addr) {
    return *(uint32_t*)rt360_memptr(ctx, addr);
}

static RT360_INLINE void rt360_store16_reverse(RT360Context* ctx, uint32_t addr, uint16_t val) {
    *(uint16_t*)rt360_memptr(ctx, addr) = val;
}

static RT360_INLINE void rt360_store32_reverse(RT360Context* ctx, uint32_t addr, uint32_t val) {
    *(uint32_t*)rt360_memptr(ctx, addr) = val;
}

// ---------------------------------------------------------------------------
// Condition register helpers
// ---------------------------------------------------------------------------

// Access CR field by index (0-7)
static RT360_INLINE RT360CRField* rt360_cr_field(RT360Context* ctx, int field) {
    return &ctx->cr0 + field;
}

// Set a CR field from signed comparison
static RT360_INLINE void rt360_compare_signed(RT360Context* ctx, int cr_field,
                                               int32_t a, int32_t b) {
    RT360CRField* cr = rt360_cr_field(ctx, cr_field);
    cr->lt = (a < b) ? 1 : 0;
    cr->gt = (a > b) ? 1 : 0;
    cr->eq = (a == b) ? 1 : 0;
    cr->so = ctx->xer_so;
}

// Set a CR field from unsigned comparison
static RT360_INLINE void rt360_compare_unsigned(RT360Context* ctx, int cr_field,
                                                 uint32_t a, uint32_t b) {
    RT360CRField* cr = rt360_cr_field(ctx, cr_field);
    cr->lt = (a < b) ? 1 : 0;
    cr->gt = (a > b) ? 1 : 0;
    cr->eq = (a == b) ? 1 : 0;
    cr->so = ctx->xer_so;
}

// Set CR0 from arithmetic result (common after instructions with Rc=1)
static RT360_INLINE void rt360_update_cr0(RT360Context* ctx, int32_t result) {
    ctx->cr0.lt = (result < 0) ? 1 : 0;
    ctx->cr0.gt = (result > 0) ? 1 : 0;
    ctx->cr0.eq = (result == 0) ? 1 : 0;
    ctx->cr0.so = ctx->xer_so;
}

// ---------------------------------------------------------------------------
// XER helpers
// ---------------------------------------------------------------------------

static RT360_INLINE void rt360_set_xer_ov(RT360Context* ctx, uint8_t ov) {
    ctx->xer_ov = ov;
    if (ov) ctx->xer_so = 1;  // SO is sticky
}

// Detect add overflow
static RT360_INLINE uint8_t rt360_add_overflow(int32_t a, int32_t b, int32_t result) {
    return ((a ^ result) & (b ^ result)) >> 31;
}

// Detect sub overflow (a - b = result)
static RT360_INLINE uint8_t rt360_sub_overflow(int32_t a, int32_t b, int32_t result) {
    return ((a ^ b) & (a ^ result)) >> 31;
}

// ---------------------------------------------------------------------------
// Atomic operations (lwarx / stwcx.)
// Uses real atomic CAS on unified shared guest memory so that cross-thread
// and cross-fiber contention works correctly.
// ---------------------------------------------------------------------------

#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedCompareExchange64)
#endif

static RT360_INLINE uint32_t rt360_lwarx(RT360Context* ctx, uint32_t addr) {
    // Load the current value from guest memory (big-endian, so byte-swapped).
    // Store both the guest address and the raw (host-byte-order) value
    // so stwcx can do an atomic CAS against it.
    volatile uint32_t* host_ptr = (volatile uint32_t*)rt360_memptr(ctx, addr);
    uint32_t raw_host = *host_ptr;  // Read raw host-order value
    ctx->reserved_val = (uint64_t)addr | ((uint64_t)raw_host << 32);
    return RT360_BSWAP32(raw_host);  // Return guest-order (big-endian) value
}

static RT360_INLINE int rt360_stwcx(RT360Context* ctx, uint32_t addr, uint32_t val) {
    uint32_t reserved_addr = (uint32_t)(ctx->reserved_val & 0xFFFFFFFF);
    uint32_t reserved_raw  = (uint32_t)(ctx->reserved_val >> 32);

    if (reserved_addr != addr) {
        // Reservation address mismatch — always fails
        ctx->cr0.lt = 0;
        ctx->cr0.gt = 0;
        ctx->cr0.eq = 0;
        ctx->cr0.so = ctx->xer_so;
        ctx->reserved_val = 0;
        return 0;
    }

    // Atomic compare-and-swap on the actual shared host memory.
    // val is in guest (BE) order, convert to host (LE) for the CAS.
    uint32_t new_raw = RT360_BSWAP32(val);
    volatile long* host_ptr = (volatile long*)rt360_memptr(ctx, addr);

#if defined(_MSC_VER) && !defined(__clang__)
    long prev = _InterlockedCompareExchange(host_ptr, (long)new_raw, (long)reserved_raw);
#else
    uint32_t expected = reserved_raw;
    int success = __atomic_compare_exchange_n(
        (volatile uint32_t*)host_ptr, &expected, new_raw,
        0 /* strong */, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    long prev = success ? (long)reserved_raw : (long)expected;
#endif

    ctx->reserved_val = 0;  // Clear reservation regardless

    if ((uint32_t)prev == reserved_raw) {
        // CAS succeeded
        ctx->cr0.lt = 0;
        ctx->cr0.gt = 0;
        ctx->cr0.eq = 1;
        ctx->cr0.so = ctx->xer_so;
        return 1;
    } else {
        // CAS failed — another thread/fiber modified the value
        ctx->cr0.lt = 0;
        ctx->cr0.gt = 0;
        ctx->cr0.eq = 0;
        ctx->cr0.so = ctx->xer_so;
        return 0;
    }
}

// ---------------------------------------------------------------------------
// Synchronization / Cache (mostly no-ops on x86)
// ---------------------------------------------------------------------------

#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
#define RT360_COMPILER_BARRIER() _ReadWriteBarrier()
#define RT360_MEMORY_FENCE()     _mm_mfence()
#else
#define RT360_COMPILER_BARRIER() __asm__ volatile("" ::: "memory")
#define RT360_MEMORY_FENCE()     __atomic_thread_fence(__ATOMIC_SEQ_CST)
#endif

static RT360_INLINE void rt360_sync(void)    { RT360_MEMORY_FENCE(); }
static RT360_INLINE void rt360_lwsync(void)  { RT360_COMPILER_BARRIER(); }
static RT360_INLINE void rt360_ptesync(void) { RT360_MEMORY_FENCE(); }
static RT360_INLINE void rt360_isync(void)   { RT360_COMPILER_BARRIER(); }
static RT360_INLINE void rt360_eieio(void)   { RT360_COMPILER_BARRIER(); }

// dcbz: zero a 32-byte cache line
static RT360_INLINE void rt360_dcbz(RT360Context* ctx, uint32_t addr) {
    memset(rt360_memptr(ctx, addr & ~0x1Fu), 0, 32);
}

// dcbt / dcbst / dcbf / icbi: prefetch / cache management hints (no-op)
static RT360_INLINE void rt360_dcbt(RT360Context* ctx, uint32_t addr)  { (void)ctx; (void)addr; }
static RT360_INLINE void rt360_dcbst(RT360Context* ctx, uint32_t addr) { (void)ctx; (void)addr; }
static RT360_INLINE void rt360_dcbf(RT360Context* ctx, uint32_t addr)  { (void)ctx; (void)addr; }
static RT360_INLINE void rt360_icbi(RT360Context* ctx, uint32_t addr)  { (void)ctx; (void)addr; }

// ---------------------------------------------------------------------------
// Time Base Register (mftb)
// ---------------------------------------------------------------------------

static RT360_INLINE uint64_t rt360_mftb(void) {
#ifdef _XBOX360
    // On real Xbox 360 hardware, use the actual mftb instruction
    uint32_t lo, hi;
    __asm {
        mftb lo
        mftbu hi
    }
    return ((uint64_t)hi << 32) | lo;
#elif defined(_MSC_VER)
    return __rdtsc();  // x86 approximation
#else
    uint32_t lo, hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
#endif
}

// ---------------------------------------------------------------------------
// Rotate / mask helpers (for rlwinm, rlwimi, rlwnm)
// ---------------------------------------------------------------------------

static RT360_INLINE uint32_t rt360_rotl32(uint32_t val, uint32_t n) {
    n &= 31;
    return (val << n) | (val >> (32 - n));
}

static RT360_INLINE uint64_t rt360_rotl64(uint64_t val, uint32_t n) {
    n &= 63;
    return (val << n) | (val >> (64 - n));
}

// Compute mask from MB..ME (PPC bit numbering: 0=MSB, 31=LSB)
static RT360_INLINE uint32_t rt360_mask(uint32_t mb, uint32_t me) {
    uint32_t mask_begin = 0xFFFFFFFF >> mb;
    uint32_t mask_end   = 0xFFFFFFFF << (31 - me);
    if (mb <= me) {
        return mask_begin & mask_end;
    } else {
        return mask_begin | mask_end;  // Wrapping mask
    }
}

// rlwinm: rotate left then AND with mask
static RT360_INLINE uint32_t rt360_rlwinm(uint32_t src, uint32_t sh,
                                           uint32_t mb, uint32_t me) {
    return rt360_rotl32(src, sh) & rt360_mask(mb, me);
}

// rlwimi: rotate left, insert into target under mask
static RT360_INLINE uint32_t rt360_rlwimi(uint32_t target, uint32_t src,
                                           uint32_t sh, uint32_t mb, uint32_t me) {
    uint32_t mask = rt360_mask(mb, me);
    uint32_t rotated = rt360_rotl32(src, sh);
    return (rotated & mask) | (target & ~mask);
}

// rlwnm: rotate left by register amount, AND with mask
static RT360_INLINE uint32_t rt360_rlwnm(uint32_t src, uint32_t rb,
                                          uint32_t mb, uint32_t me) {
    return rt360_rotl32(src, rb & 31) & rt360_mask(mb, me);
}

// ---------------------------------------------------------------------------
// Count leading zeros
// ---------------------------------------------------------------------------

static RT360_INLINE uint32_t rt360_cntlzw(uint32_t val) {
    if (val == 0) return 32;
#if defined(_MSC_VER) && !defined(__clang__)
    unsigned long idx;
    _BitScanReverse(&idx, val);
    return 31 - idx;
#else
    return (uint32_t)__builtin_clz(val);
#endif
}

// ---------------------------------------------------------------------------
// Trap (tw, twi) — for debug, just call dispatch with trap flag
// ---------------------------------------------------------------------------

static RT360_INLINE void rt360_trap(RT360Context* ctx, uint32_t to,
                                     int32_t a, int32_t b) {
    int trap = 0;
    if ((to & 0x10) && (a < b))  trap = 1;
    if ((to & 0x08) && (a > b))  trap = 1;
    if ((to & 0x04) && (a == b)) trap = 1;
    if ((to & 0x02) && ((uint32_t)a < (uint32_t)b)) trap = 1;
    if ((to & 0x01) && ((uint32_t)a > (uint32_t)b)) trap = 1;
    if (trap) {
        // In a real implementation this would raise a program exception.
        // For RT360, we just set a flag or abort.
        (void)ctx;
    }
}

// ---------------------------------------------------------------------------
// Kernel API call tracing (for crash handler)
// ---------------------------------------------------------------------------

#ifndef RT360_MAX_CALL_TRACE
#define RT360_MAX_CALL_TRACE 32
#endif

typedef struct {
    uint32_t target_addr;
    uint64_t r3, r4, r5;  // first 3 args
    uint64_t ret_r3;       // return value
    const char* name;      // resolved name (NULL if unknown)
} RT360CallTraceEntry;

// Per-thread call trace ring buffer (stored externally, indexed by thread_id)
// The crash handler reads this to show "LAST KERNEL CALLS"

#ifdef __cplusplus
}  // extern "C"
#endif
