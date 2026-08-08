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

/* --- MMX / SSE / AVX register storage surfaced by the faithful lift ------------------- */
u64 mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7;
u64 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
u128 ymm0, ymm1, ymm2, ymm3, ymm4, ymm5, ymm6, ymm7;

/* --- division hi/lo halves + the xchg scratch (bare objects in the lift) --------------- */
u32 __dvl, __dvh, __xch;

/* --- x86 flags surfaced as bare values ------------------------------------------------ */
u32 CF, ZF, SF, OF, PF, AF;
