/* win95c.h -- the RECOMPILE PRELUDE (dual-compiler: MinGW/GCC and MSVC 2.0).
 *
 * il.py lifts each byte-exact .asm function into a near-1:1 "register machine" C:
 * CPU registers appear as free variables (eax, ecx, ...), byte/word lanes as al/ah/ax,
 * x86-only operations as __intrinsics, and raw jumps as computed `goto *`.
 *
 * The GCC path (#else below) is preserved byte-for-byte from the original so the working
 * MinGW corpus is unchanged. The MSVC path (#ifdef _MSC_VER) is C89: no `long long`, no
 * variadic macros, no __attribute__, no computed goto -- so those constructs are provided
 * in MSVC-legal form, and package_ff --msvc rewrites computed goto + the naked-asm templates.
 */
#ifndef WIN95C_H
#define WIN95C_H

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef signed char        i8;
typedef short              i16;
typedef int                i32;

#ifdef _MSC_VER
/* ---- MSVC 2.0 (C89) path ---- */
typedef unsigned __int64   u64;
typedef __int64            i64;
typedef u64                u128;             /* no __int128 in MSVC2.0; scalar alias */
typedef double             u80;              /* no 80-bit type in MSVC C */
typedef u32 (__stdcall *fnptr)();            /* Win95 APIs are __stdcall (callee cleans) */
#else
/* ---- MinGW/GCC path (unchanged) ---- */
typedef unsigned long long u64;
typedef long long          i64;
#ifdef __SIZEOF_INT128__
typedef unsigned __int128  u128;
#else
typedef u64 u128;
#endif
typedef long double        u80;
typedef u32 (__attribute__((stdcall)) *fnptr)();
#endif

extern void __lifted_unresolved_jump(void);

/* --- the x86 register file, as globals (the lift's implicit machine state) --------- */
extern u32 eax, ebx, ecx, edx, esi, edi, ebp, esp;
extern u32 cs, ds, es, fs, gs, ss;
/* Address aliases for the register-file globals (defined in win95rt.c). MSVC inline asm can't NAME
   register-keyword globals (eax..); and the clean lift's LOCAL eax.. shadow the globals, so writing
   the global from a clean fn needs `*g_eax` too. Used by the register-sync wrapper AND the clean->
   byte-exact boundary sync -- so declare unconditionally (both MSVC and gcc). */
extern u32 *const g_eax, *const g_ebx, *const g_ecx, *const g_edx, *const g_esi, *const g_edi;

/* --- byte/word LANES: memory views of the parent register (little-endian) ---------- */
#define al (*((u8 *)&eax + 0))
#define ah (*((u8 *)&eax + 1))
#define ax (*((u16 *)&eax))
#define bl (*((u8 *)&ebx + 0))
#define bh (*((u8 *)&ebx + 1))
#define bx (*((u16 *)&ebx))
#define cl (*((u8 *)&ecx + 0))
#define ch (*((u8 *)&ecx + 1))
#define cx (*((u16 *)&ecx))
#define dl (*((u8 *)&edx + 0))
#define dh (*((u8 *)&edx + 1))
#define dx (*((u16 *)&edx))
#define si (*((u16 *)&esi))
#define di (*((u16 *)&edi))
#define bp (*((u16 *)&ebp))
#define sp (*((u16 *)&esp))

extern u64 mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7;
extern u64 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
extern u128 ymm0, ymm1, ymm2, ymm3, ymm4, ymm5, ymm6, ymm7;
extern u32 __dvl, __dvh, __xch;

static u32 __mulhi(u32 a, u32 b){ return (u32)(((u64)a * (u64)b) >> 32); }
static u32 __imulhi(u32 a, u32 b){ return (u32)(((i64)(i32)a * (i64)(i32)b) >> 32); }
static u32 __div(u32 hi, u32 lo, u32 d){ u64 n=((u64)hi<<32)|lo; return d?(u32)(n/d):0u; }
static u32 __div_rem(u32 hi, u32 lo, u32 d){ u64 n=((u64)hi<<32)|lo; return d?(u32)(n%d):0u; }
static i32 __idiv(u32 hi, u32 lo, u32 d){ i64 n=(i64)(((u64)hi<<32)|lo); return d?(i32)(n/(i32)d):0; }
static i32 __idiv_rem(u32 hi, u32 lo, u32 d){ i64 n=(i64)(((u64)hi<<32)|lo); return d?(i32)(n%(i32)d):0; }

#ifdef _MSC_VER
/* ---- MSVC: intrinsics as C89 variadic FUNCTIONS aliased by object-like macros
 *      (`#define __rol __rol_` -> `__rol(x,n)` = `__rol_(x,n)`, a variadic call; legal in C89,
 *      unlike variadic MACROS). Real semantics where cheap: rotates default to 32-bit width
 *      (the lift doesn't carry width -- a future refinement); carry/borrow computed for real. */
/* BISECT: match mingw's known-working stubs (passthrough) to isolate whether real
   intrinsic semantics are what breaks the MSVC build. Real rol/ror can come back once
   proven safe; rcl/rcr need through-carry (33-bit) not plain rotate anyway. */
static u32 __rol_(u32 x, u32 n, ...){ return x; }
static u32 __ror_(u32 x, u32 n, ...){ return x; }
#define __rol __rol_
#define __ror __ror_
#define __rcl __rol_
#define __rcr __ror_
static u32 __bsr_(u32 a, ...){ u32 i; if(!a) return 0; for(i=31;;i--){ if(a&(1u<<i)) return i; if(!i) break; } return 0; }
static u32 __bsf_(u32 a, ...){ u32 i; if(!a) return 0; for(i=0;i<32;i++) if(a&(1u<<i)) return i; return 0; }
#define __bsr __bsr_
#define __bsf __bsf_
static u32 __parity_(u32 a, ...){ u32 p=a&0xffu; p^=p>>4; p^=p>>2; p^=p>>1; return (~p)&1u; }
#define __parity __parity_
static u32 __gen1_(u32 a, ...){ return a; }
/* __dvl/__dvh/__xch stay bare extern globals (defined in win95rt.c) -- NOT macro'd. */
#define __inp __gen1_
#define __outp __gen1_
#define __lsl __gen1_
#define __lar __gen1_
#define __arpl __gen1_
#define __aam __gen1_
#define __aad __gen1_
#define __aas __gen1_
#define __psubb __gen1_
static u32 __rc_(u32 a, ...){ return a; }
#define __asm(x)
extern u32 CF, ZF, SF, OF, PF, AF;
extern u32 __cf0;      /* adc/sbb incoming-CF snapshot temp (il emits `__cf0 = CF` before each use) */
static u32 __cf_(u32 a, ...){ return 0u; }
static u32 __carry_(u32 a, u32 b, ...){ return ((a + b) < a) ? 1u : 0u; }
static u32 __borrow_(u32 a, u32 b, ...){ return (a < b) ? 1u : 0u; }
#define __cf __cf_
#define __carry __carry_
#define __borrow __borrow_
#else
/* ---- MinGW/GCC: original variadic-macro stubs (unchanged) ---- */
#define __rol(...)      __rc_(__VA_ARGS__)
#define __ror(...)      __rc_(__VA_ARGS__)
#define __rcl(...)      __rc_(__VA_ARGS__)
#define __rcr(...)      __rc_(__VA_ARGS__)
#define __xch(...)      __rc_(__VA_ARGS__)
#define __bsr(...)      __rc_(__VA_ARGS__)
#define __bsf(...)      __rc_(__VA_ARGS__)
#define __parity(...)   __rc_(__VA_ARGS__)
#define __dvl(...)      __rc_(__VA_ARGS__)
#define __dvh(...)      __rc_(__VA_ARGS__)
#define __inp(...)      __rc_(__VA_ARGS__)
#define __outp(...)     __rc_(__VA_ARGS__)
#define __lsl(...)      __rc_(__VA_ARGS__)
#define __lar(...)      __rc_(__VA_ARGS__)
#define __arpl(...)     __rc_(__VA_ARGS__)
#define __aam(...)      __rc_(__VA_ARGS__)
#define __aad(...)      __rc_(__VA_ARGS__)
#define __aas(...)      __rc_(__VA_ARGS__)
#define __psubb(...)    __rc_(__VA_ARGS__)
static u32 __rc_(u32 a, ...){ return a; }
#define __asm(...) ((void)0)
extern u32 CF, ZF, SF, OF, PF, AF;
extern u32 __cf0;      /* adc/sbb incoming-CF snapshot temp (il emits `__cf0 = CF` before each use) */
#define __cf(...)     0u
#define __carry(...)  0u
#define __borrow(...) 0u
#endif

#endif /* WIN95C_H */
