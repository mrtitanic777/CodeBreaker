// c_text_consts.inc -- the C text constants of decompile/c_text.py (_TYPES, _HELPERS, HEADER, _POWERPC, SEG_PTR), as raw
// string literals, char for char (generated once from the Python; included by c_text.cpp).
static const char* const TYPES_TEXT =
R"CBR(#pragma pack(1)
typedef signed char int1; typedef short int2; typedef int int4; typedef long long int8;
typedef unsigned char uint1; typedef unsigned short uint2; typedef unsigned int uint4;
typedef unsigned long long uint8;
typedef unsigned char xunknown1; typedef unsigned short xunknown2; typedef unsigned int xunknown4;
typedef unsigned long long xunknown8;
typedef unsigned char undefined; typedef unsigned char undefined1; typedef unsigned short undefined2;
typedef unsigned int undefined4; typedef unsigned long long undefined8;
typedef unsigned char bool; typedef float float4; typedef double float8; typedef long double float10;
typedef int code();
typedef unsigned int unkbyte3; typedef unsigned int unkint3; typedef unsigned int unkuint3;
#define true 1
#define false 0
)CBR";

static const char* const HELPERS_TEXT =
R"CBR(
#define CONCAT11(a,b) ((uint2)(((uint2)(uint1)(a) << 8) | (uint1)(b)))
#define CONCAT12(a,b) ((uint4)(((uint4)(uint1)(a) << 16) | (uint2)(b)))
#define CONCAT13(a,b) ((uint4)(((uint4)(uint1)(a) << 24) | ((uint4)(b) & 0xFFFFFF)))
#define CONCAT21(a,b) ((uint4)(((uint4)(uint2)(a) << 8) | (uint1)(b)))
#define CONCAT22(a,b) ((uint4)(((uint4)(uint2)(a) << 16) | (uint2)(b)))
#define CONCAT31(a,b) ((uint4)(((uint4)(a) << 8) | (uint1)(b)))
#define CONCAT44(a,b) ((uint8)(((uint8)(uint4)(a) << 32) | (uint4)(b)))
#define SUB21(x,n) ((uint1)((uint2)(x) >> (8 * (n))))
#define SUB41(x,n) ((uint1)((uint4)(x) >> (8 * (n))))
#define SUB42(x,n) ((uint2)((uint4)(x) >> (8 * (n))))
#define SUB43(x,n) ((uint4)((uint4)(x) >> (8 * (n))) & 0xFFFFFF)
#define SUB81(x,n) ((uint1)((uint8)(x) >> (8 * (n))))
#define SUB82(x,n) ((uint2)((uint8)(x) >> (8 * (n))))
#define SUB84(x,n) ((uint4)((uint8)(x) >> (8 * (n))))
#define ZEXT12(x) ((uint2)(uint1)(x))
#define ZEXT14(x) ((uint4)(uint1)(x))
#define ZEXT24(x) ((uint4)(uint2)(x))
#define ZEXT48(x) ((uint8)(uint4)(x))
#define SEXT12(x) ((int2)(int1)(x))
#define SEXT14(x) ((int4)(int1)(x))
#define SEXT24(x) ((int4)(int2)(x))
#define SEXT48(x) ((int8)(int4)(x))
#define CARRY1(a,b) ((uint1)(a) > (uint1)(0xFF - (uint1)(b)))
#define CARRY2(a,b) ((uint2)(a) > (uint2)(0xFFFF - (uint2)(b)))
#define CARRY4(a,b) ((uint4)(a) > 0xFFFFFFFFu - (uint4)(b))
#define SCARRY4(a,b) ((((int4)(a) ^ (int4)((uint4)(a) + (uint4)(b))) & ((int4)(b) ^ (int4)((uint4)(a) + (uint4)(b)))) < 0)
#define SBORROW4(a,b) ((((int4)(a) ^ (int4)(b)) & ((int4)(a) ^ (int4)((uint4)(a) - (uint4)(b)))) < 0)
#define SCARRY2(a,b) ((int2)(((int2)(a) ^ (int2)((a) + (b))) & ((int2)(b) ^ (int2)((a) + (b)))) < 0)
#define SBORROW2(a,b) ((int2)(((int2)(a) ^ (int2)(b)) & ((int2)(a) ^ (int2)((a) - (b)))) < 0)
#define SCARRY1(a,b) ((int1)(((int1)(a) ^ (int1)((a) + (b))) & ((int1)(b) ^ (int1)((a) + (b)))) < 0)
#define SBORROW1(a,b) ((int1)(((int1)(a) ^ (int1)(b)) & ((int1)(a) ^ (int1)((a) - (b)))) < 0)
)CBR";

static const char* const HEADER_TEXT =
R"CBR(typedef long long int8; typedef unsigned long long uint8; typedef unsigned long long xunknown8;
typedef double float8; typedef long double float10;
typedef unsigned long unkbyte3; typedef long unkint3; typedef unsigned long unkuint3; typedef long int3;
typedef unsigned long uint3; typedef unsigned long long unkbyte6; typedef long double unkbyte10;
typedef signed char int1; typedef short int2; typedef long int4;
typedef unsigned char uint1; typedef unsigned short uint2; typedef unsigned long uint4;
typedef unsigned char xunknown1; typedef unsigned short xunknown2; typedef unsigned long xunknown4;
typedef unsigned char undefined; typedef unsigned char undefined1; typedef unsigned short undefined2;
typedef unsigned long undefined4; typedef unsigned char bool; typedef unsigned long float4;
typedef int2 __cdecl code();                    /* (a convention named: Watcom's own would pass the arguments in registers) */
typedef int2 __stdcall code_callee_pops();      /* a call through a pointer whose callee removes its arguments */
#define true 1
uint2 codebreaker_ss(void);
#pragma aux codebreaker_ss = "mov ax,ss" value [ax] modify exact [ax];
uint2 codebreaker_ds(void);
#pragma aux codebreaker_ds = "mov ax,ds" value [ax] modify exact [ax];
uint2 codebreaker_cs(void);
#pragma aux codebreaker_cs = "mov ax,cs" value [ax] modify exact [ax];
/* the stack pointer as the function was entered (where its return address is): with the
   function's frame (BP pushed, then BP = SP) that is BP+2 */
uint2 codebreaker_entry_sp(void);
#pragma aux codebreaker_entry_sp = "lea ax,[bp+2]" value [ax] modify exact [ax];
/* a word of the caller's stack: the stack segment, at the entry SP plus `off` */
#define codebreaker_stack(T, off) (*(T __based(__segname("_STACK")) *)(codebreaker_entry_sp() + (off)))
#define false 0
#define __stdcall16far __far __stdcall
#define __stdcall16near __near __stdcall
#define __cdecl16far __far __cdecl
#define __cdecl16near __near __cdecl
#ifdef __WATCOMC__
/* Microsoft's _fastcall (describe: a near function reading AX, DX or BX before it writes them):
   its arguments in AX, DX, BX, its result where Watcom puts one too (AL, AX, DX:AX: a `value
   [ax]` here made every long-returning one "invalid return register"), and the registers
   Microsoft C lets a function overwrite. Watcom has no keyword for it: each such function is
   named in a pragma after its declaration. */
#pragma aux codebreaker_fastcall "_*" parm caller [ax] [dx] [bx] modify [ax bx cx dx es];
#define __fastcall16near __near
#define __fastcall16far __far
#endif
#define CONCAT11(a,b) ((uint2)(((uint2)(uint1)(a) << 8) | (uint1)(b)))
#define CONCAT22(a,b) ((uint4)(((uint4)(uint2)(a) << 16) | (uint2)(b)))
#define CONCAT13(a,b) ((uint4)(((uint4)(uint1)(a) << 24) | ((uint4)(b) & 0xFFFFFFUL)))
#define CONCAT31(a,b) ((uint4)(((uint4)(a) << 8) | (uint1)(b)))
#define CONCAT12(a,b) ((uint4)(((uint4)(uint1)(a) << 16) | (uint2)(b)))
#define CONCAT21(a,b) ((uint4)(((uint4)(uint2)(a) << 8) | (uint1)(b)))
#define SUB21(x,n) ((uint1)((uint2)(x) >> ((n) * 8)))
#define SUB41(x,n) ((uint1)((uint4)(x) >> ((n) * 8)))
#define SUB42(x,n) ((uint2)((uint4)(x) >> ((n) * 8)))
#define ZEXT12(x) ((uint2)(uint1)(x))
#define ZEXT14(x) ((uint4)(uint1)(x))
#define ZEXT24(x) ((uint4)(uint2)(x))
#define SEXT12(x) ((int2)(int1)(x))
#define SEXT14(x) ((int4)(int1)(x))
#define SEXT24(x) ((int4)(int2)(x))
#define CARRY1(a,b) ((uint1)((uint1)(a) + (uint1)(b)) < (uint1)(a))
#define CARRY2(a,b) ((uint2)((uint2)(a) + (uint2)(b)) < (uint2)(a))
#define CARRY4(a,b) ((uint4)((uint4)(a) + (uint4)(b)) < (uint4)(a))
#define SBORROW1(a,b) ((int1)(((int1)(a) ^ (int1)(b)) & ((int1)(a) ^ (int1)((a) - (b)))) < 0)
#define SBORROW2(a,b) ((int2)(((int2)(a) ^ (int2)(b)) & ((int2)(a) ^ (int2)((a) - (b)))) < 0)
#define SBORROW4(a,b) ((int4)(((int4)(a) ^ (int4)(b)) & ((int4)(a) ^ (int4)((a) - (b)))) < 0)
#define SCARRY1(a,b) ((int1)(((int1)(a) ^ (int1)((a) + (b))) & ((int1)(b) ^ (int1)((a) + (b)))) < 0)
#define SCARRY2(a,b) ((int2)(((int2)(a) ^ (int2)((a) + (b))) & ((int2)(b) ^ (int2)((a) + (b)))) < 0)
#define SCARRY4(a,b) ((int4)(((int4)(a) ^ (int4)((a) + (b))) & ((int4)(b) ^ (int4)((a) + (b)))) < 0)
#define CODEBREAKER_FAR(seg, off) ((void __far *)(((uint4)(uint2)(seg) << 16) | (uint2)(off)))
)CBR";

static const char* const POWERPC_TEXT =
R"CBR(
#define PIECE(x, off, T) (*(T *)((char *)&(x) + sizeof(x) - (off) - sizeof(T)))
/* Instructions the decompiler writes by name rather than as C. */
static uint4 codebreaker_lzcount32(uint4 x) { uint4 n = 0; if (!x) return 32; while (!(x & 0x80000000)) { x <<= 1; n++; } return n; }
static uint4 codebreaker_lzcount64(uint8 x) { uint4 n = 0; if (!x) return 64; while (!(x >> 63)) { x <<= 1; n++; } return n; }
#define LZCOUNT(x) (sizeof(x) == 8 ? codebreaker_lzcount64((uint8)(x)) : codebreaker_lzcount32((uint4)(x)))
#define ABS(x) ((x) < 0 ? -(x) : (x))
double sqrt(double);
#define SQRT(x) sqrt((double)(x))
/* a store that succeeds when nothing else wrote there since the matching load: here nothing else runs */
static uint4 codebreaker_store_conditional(uint4 v, uint4 a, uint4 b) { *(uint4 *)(a + b) = v; return 1; }
#define storeWordConditionalIndexed(v, a, b) codebreaker_store_conditional((uint4)(v), (uint4)(a), (uint4)(b))
static void codebreaker_zero_block(uint4 a, uint4 b) { uint1 *p = (uint1 *)((a + b) & ~31u); int i; for (i = 0; i < 32; i++) p[i] = 0; }
#define dataCacheBlockSetToZero(a, b) codebreaker_zero_block((uint4)(a), (uint4)(b))
/* hints to the processor and waits for it, which change nothing a program can see; and a trap,
   which is taken only when a check the compiler put in fails (a division by zero) */
#define sync(...) ((void)0)
#define instructionSynchronize(...) ((void)0)
#define enforceInOrderExecutionIO(...) ((void)0)
#define dataCacheBlockTouch(...) ((void)0)
#define dataCacheBlockTouchForStore(...) ((void)0)
#define dataCacheBlockStore(...) ((void)0)
#define dataCacheBlockFlush(...) ((void)0)
/* a trap: taken when the comparison its first operand selects holds (16 less, 8 greater, 4 equal,
   2 and 1 the same unsigned) -- the check the compiler put before a division. Taken, the program
   stops there; here by a call to nowhere. */
static void codebreaker_trap(uint4 to, int8 a, int8 b, uint8 ua, uint8 ub) {
  if (((to & 16) && a < b) || ((to & 8) && a > b) || ((to & 4) && a == b) || ((to & 2) && ua < ub) || ((to & 1) && ua > ub))
    ((void (*)())0)();
}
#define trapWord(to, a, b) codebreaker_trap((uint4)(to), (int8)(int4)(a), (int8)(int4)(b), (uint8)(uint4)(a), (uint8)(uint4)(b))
#define trapDoubleWord(to, a, b) codebreaker_trap((uint4)(to), (int8)(a), (int8)(b), (uint8)(a), (uint8)(b))
)CBR";

static const char* const SEG_PTR_TEXT =
R"CBR(#define SEG_PTR(T, s) ((T __far *)((uint4)(uint2)(s) << 16))   /* a structure at the start of a segment */)CBR";
