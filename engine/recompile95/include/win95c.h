/* win95c.h -- the RECOMPILE PRELUDE.
 *
 * il.py lifts each byte-exact .asm function into a near-1:1 "register machine" C:
 * CPU registers appear as free variables (eax, ecx, ...), byte/word lanes as al/ah/ax,
 * x86-only operations as __intrinsics, and raw jumps as computed `goto *`. That reads
 * well but does not compile on its own -- every one of those names is undeclared.
 *
 * This header declares that shared vocabulary ONCE, so the existing lift output compiles
 * unchanged (RUNG 1: "it compiles"). It targets GCC (gnu89): computed goto and implicit
 * old-style calls are accepted there; MSVC 2.0 -- the byte-exact oracle -- is a later,
 * separate concern (RUNG 3). Nothing here changes il.py; it is a compile shim on top.
 *
 * Lanes are memory VIEWS of the parent register (little-endian), so writing a lane
 * leaves the rest of the parent intact -- exactly the hardware semantics il.py assumes.
 */
#ifndef WIN95C_H
#define WIN95C_H

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef signed char        i8;
typedef short              i16;
typedef int                i32;
typedef long long          i64;
#ifdef __SIZEOF_INT128__
typedef unsigned __int128  u128;             /* SSE/xmm 128-bit views (64-bit host) */
#else
typedef u64 u128;                            /* i386 (-m32): no __int128. A SCALAR (not a struct) so
                                                SSE-view assignments type-check BOTH ways -- `u128 =
                                                __movups(...)` AND `u32 = __sse(u128arg)` -- which a
                                                struct cannot (structs don't convert to/from scalars,
                                                and the lift emits no {lo,hi} inits). Width isn't
                                                preserved at -m32, but rung-2d is compile+link only;
                                                real 128-bit semantics ride the native/MSVC path. */
#endif
typedef long double        u80;              /* x87 80-bit FPU register views (native on i386) */

/* indirect-call target type the lift casts through: (*(fnptr)addr)(...) */
typedef u32 (*fnptr)();

/* --- the x86 register file, as globals (the lift's implicit machine state) --------- */
extern u32 eax, ebx, ecx, edx, esi, edi, ebp, esp;
extern u32 cs, ds, es, fs, gs, ss;              /* segment selectors, used as bases */

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

/* --- MMX / SSE registers, surfaced by the faithful lift as globals ----------------- */
extern u64 mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7;
extern u64 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
extern u128 ymm0, ymm1, ymm2, ymm3, ymm4, ymm5, ymm6, ymm7;   /* AVX (256-bit; 128 view ok at rung 1) */

/* division hi/lo halves and the exchange temp, used as BARE objects by the lift
 * (edx:eax pair for div, xchg scratch). The like-named __dvl(...)/__xch(...) macros
 * below only fire when followed by '(' , so these globals cover the bare uses. */
extern u32 __dvl, __dvh, __xch;

/* --- x86 operations C has no operator for -----------------------------------------
 * RUNG 1 is "it compiles", not "it's byte-exact", so every intrinsic is a VARIADIC
 * function-like macro: it accepts however many operands the lift passes and yields a
 * value. il.py varies the operand count per instruction, so fixed arity fails corpus-
 * wide; variadic absorbs that. Real semantics/byte-tuning belong to the MSVC-2.0 path. */
#define __rol(...)      __rc_(__VA_ARGS__)
#define __ror(...)      __rc_(__VA_ARGS__)
#define __rcl(...)      __rc_(__VA_ARGS__)
#define __rcr(...)      __rc_(__VA_ARGS__)
#define __mulhi(...)    __rc_(__VA_ARGS__)
#define __xch(...)      __rc_(__VA_ARGS__)
#define __bsr(...)      __rc_(__VA_ARGS__)
#define __bsf(...)      __rc_(__VA_ARGS__)
#define __parity(...)   __rc_(__VA_ARGS__)
#define __div(...)      __rc_(__VA_ARGS__)
#define __div_rem(...)  __rc_(__VA_ARGS__)
#define __idiv(...)     __rc_(__VA_ARGS__)
#define __idiv_rem(...) __rc_(__VA_ARGS__)
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
static u32 __rc_(u32 a, ...){ return a; }    /* takes 1+ operands, yields the first */

/* the lift emits un-lifted instructions as __asm('text') with SINGLE quotes, which hits
 * GCC's __asm keyword (wants a real string). Neutralize it at rung 1 (macro beats keyword,
 * since the preprocessor runs first). Byte-exactness of these belongs to the .asm path. */
#define __asm(...) ((void)0)

/* --- x86 flags surfaced by the faithful lift (RUNG-1 stubs) ------------------------
 * The lift both READS flags as bare values (CF, __cf0, __ovf_...) and CALLS flag
 * helpers (__carry(a,b), __cf(...)). Provide both: object globals for the reads, and
 * variadic macros for the calls. Numbered flag temps are declared by the adapter. */
extern u32 CF, ZF, SF, OF, PF, AF;
#define __cf(...)     0u
#define __carry(...)  0u
#define __borrow(...) 0u

#endif /* WIN95C_H */
