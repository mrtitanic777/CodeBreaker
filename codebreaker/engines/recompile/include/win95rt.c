/* win95rt.c -- the RECOMPILE RUNTIME: the one definition of the x86 machine state.
 *
 * win95c.h DECLARES the register file, flags, and SIMD registers as `extern` so every
 * lifted module compiles against a shared vocabulary. Exactly ONE translation unit must
 * DEFINE them -- this one. Link a module's object with win95rt.o (plus stubs for its
 * cross-module callees, or the other modules themselves) and the machine-state symbols
 * resolve, taking the lift from "compiles" (rung 1/2) toward "links into a loadable
 * module" (rung 2d). Values are rung-3 semantics; here they just need storage + linkage.
 */
#include "win95c.h"

/* --- the x86 register file (globals the lift reads/writes as implicit machine state) --- */
u32 eax, ebx, ecx, edx, esi, edi, ebp, esp;
u32 cs, ds, es, fs, gs, ss;                       /* segment selectors */
/* address aliases for the register-file globals -- used by the MSVC register-sync wrapper AND the
   clean->byte-exact boundary sync (both compilers), so define unconditionally (see win95c.h). */
u32 *const g_eax=&eax, *const g_ebx=&ebx, *const g_ecx=&ecx,
    *const g_edx=&edx, *const g_esi=&esi, *const g_edi=&edi;

/* --- MMX / SSE / AVX register storage surfaced by the faithful lift ------------------- */
u64 mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7;
u64 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
u128 ymm0, ymm1, ymm2, ymm3, ymm4, ymm5, ymm6, ymm7;

/* --- division hi/lo halves + the xchg scratch (bare objects in the lift) --------------- */
u32 __dvl, __dvh, __xch;

/* --- x86 flags surfaced as bare values ------------------------------------------------ */
u32 CF, ZF, SF, OF, PF, AF;
u32 __cf0;      /* adc/sbb incoming-CF snapshot temp */

/* --- x87 FPU stack + misc single-instruction intrinsics the lift emits as calls ------- */
/* Functional (rung-2) storage/linkage; not byte-exact x87 semantics. */
static u64 __fpu_stk[8]; static int __fpu_sp;
void __fpush(u64 v) { if (__fpu_sp < 8) __fpu_stk[__fpu_sp++] = v; }
u64  __fpop(void)   { return __fpu_sp > 0 ? __fpu_stk[--__fpu_sp] : 0; }
u64  __ftop(void)   { return __fpu_sp > 0 ? __fpu_stk[__fpu_sp - 1] : 0; }
u32  __fnstcw(void) { return 0x037f; }             /* default x87 control word */
u32  __bswap(u32 x) { return (x >> 24) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | (x << 24); }
u32  __sldt(void)   { return 0; }                  /* store LDT selector (no LDT here) */
u32  __das(u32 x)   { return x & 0xff; }           /* decimal-adjust after subtract (functional) */
u32  __daa(u32 x)   { return x & 0xff; }           /* decimal-adjust after add (functional) */

/* Unresolved unconditional transfer (see win95c.h): a no-return trap. Falling through an
 * unknown jump would silently corrupt; trapping keeps every function that never reaches the
 * edge correct, and makes one that does fail loudly. __builtin_trap emits ud2 -- no libc. */
#ifdef _MSC_VER
void __lifted_unresolved_jump(void) { __asm { int 3 } }   /* MSVC: no __builtin_trap; int3 traps */
#else
void __lifted_unresolved_jump(void) { __builtin_trap(); } /* GCC: ud2 -- no libc */
#endif
