/* lzexpand.dll -- types, declarations and globals used by the functions in lzexpand.c.
 * Globals are bound to their addresses in the original until the module's data is
 * reconstructed. */

typedef long long int8; typedef unsigned long long uint8; typedef unsigned long long xunknown8;
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

struct codebreaker_registers { uint2 ax, bx, cx, dx, si, di, ds, es; uint1 cf, zf; uint2 set; };
extern struct codebreaker_registers codebreaker_int;   /* as the last interrupt left them */
#define int_AX codebreaker_int.ax
#define int_BX codebreaker_int.bx
#define int_CX codebreaker_int.cx
#define int_DX codebreaker_int.dx
#define int_SI codebreaker_int.si
#define int_DI codebreaker_int.di
#define int_DS codebreaker_int.ds
#define int_ES codebreaker_int.es
#define int_CF codebreaker_int.cf
#define int_ZF codebreaker_int.zf
#define int_AL (*(uint1 *)&codebreaker_int.ax)
#define int_AH (*((uint1 *)&codebreaker_int.ax + 1))
#define int_BL (*(uint1 *)&codebreaker_int.bx)
#define int_BH (*((uint1 *)&codebreaker_int.bx + 1))
#define int_CL (*(uint1 *)&codebreaker_int.cx)
#define int_CH (*((uint1 *)&codebreaker_int.cx + 1))
#define int_DL (*(uint1 *)&codebreaker_int.dx)
#define int_DH (*((uint1 *)&codebreaker_int.dx + 1))
void __far codebreaker_int21(void);              /* INT 21h */
static uint4 int21_ax(uint2 ax) { codebreaker_int.ax = ax; codebreaker_int.set = 0x01; codebreaker_int21(); return (uint4)codebreaker_int.dx << 16 | codebreaker_int.ax; }
static uint4 int21_ax_bx(uint2 ax, uint2 bx) { codebreaker_int.ax = ax; codebreaker_int.bx = bx; codebreaker_int.set = 0x03; codebreaker_int21(); return (uint4)codebreaker_int.dx << 16 | codebreaker_int.ax; }
static uint4 int21_ax_bx_cx_dx(uint2 ax, uint2 bx, uint2 cx, uint2 dx) { codebreaker_int.ax = ax; codebreaker_int.bx = bx; codebreaker_int.cx = cx; codebreaker_int.dx = dx; codebreaker_int.set = 0x0f; codebreaker_int21(); return (uint4)codebreaker_int.dx << 16 | codebreaker_int.ax; }
static uint4 int21_ax_cx_dx(uint2 ax, uint2 cx, uint2 dx) { codebreaker_int.ax = ax; codebreaker_int.cx = cx; codebreaker_int.dx = dx; codebreaker_int.set = 0x0d; codebreaker_int21(); return (uint4)codebreaker_int.dx << 16 | codebreaker_int.ax; }
static uint4 int21_ax_cx_dx_ds(uint2 ax, uint2 cx, uint2 dx, uint2 ds) { codebreaker_int.ax = ax; codebreaker_int.cx = cx; codebreaker_int.dx = dx; codebreaker_int.ds = ds; codebreaker_int.set = 0x4d; codebreaker_int21(); return (uint4)codebreaker_int.dx << 16 | codebreaker_int.ax; }
static uint4 int21_ax_cx_si_di(uint2 ax, uint2 cx, uint2 si, uint2 di) { codebreaker_int.ax = ax; codebreaker_int.cx = cx; codebreaker_int.si = si; codebreaker_int.di = di; codebreaker_int.set = 0x35; codebreaker_int21(); return (uint4)codebreaker_int.dx << 16 | codebreaker_int.ax; }
static uint4 int21_ax_dx_ds(uint2 ax, uint2 dx, uint2 ds) { codebreaker_int.ax = ax; codebreaker_int.dx = dx; codebreaker_int.ds = ds; codebreaker_int.set = 0x49; codebreaker_int21(); return (uint4)codebreaker_int.dx << 16 | codebreaker_int.ax; }
#define MK_FP(s, o) ((void __far *)(((uint4)(uint2)(s) << 16) | (uint2)(o)))
#define SEG_PTR(T, s) ((T __far *)((uint4)(uint2)(s) << 16))   /* a structure at the start of a segment */
#define iRam1050035f (*(int2 __far *)(((uint4)(uint2)((uint4)(void __far *)&___EXPORTEDSTUB >> 16) << 16) | 0x35f))
#define uRam10500071 (*(uint2 __far *)(((uint4)(uint2)((uint4)(void __far *)&___EXPORTEDSTUB >> 16) << 16) | 0x71))
typedef struct s_LZSEEK_iVar5 s_LZSEEK_iVar5;
_Packed struct s_LZSEEK_iVar5 { uint1 _pad_0[5]; uint2 f_5; int2 f_7; uint1 _pad_9[4]; uint2 f_d; int2 f_f; };
typedef _Packed struct seg_sub_11_00D8_unaff_DS { uint1 _pad_0[164]; xunknown2 f_a4; xunknown2 f_a6; } seg_sub_11_00D8_unaff_DS;
extern uint4 __far __stdcall FatalExit();
extern uint4 __far __stdcall DOS3CALL();
extern uint4 __far __stdcall OutputDebugString();
extern uint4 __far __stdcall FatalAppExit();
extern uint4 __far __stdcall GlobalAlloc();
extern uint4 __far __stdcall GlobalFree();
extern uint4 __far __stdcall GlobalLock();
extern uint4 __far __stdcall GlobalUnlock();
extern uint4 __far __stdcall OpenFile();
extern uint4 __far __stdcall _lclose();
extern uint4 __far __stdcall _lread();
extern uint4 __far __stdcall _lcreat();
extern uint4 __far __stdcall _llseek();
extern uint4 __far __stdcall _lopen();
extern uint4 __far __stdcall _lwrite();
extern uint4 __far __stdcall lstrcpy();
extern uint4 __far __stdcall lstrcat();
extern uint4 __far __stdcall lstrlen();
extern uint4 __far __stdcall lstrcmp();
extern uint4 __far __stdcall IsCharUpper();
extern uint4 __far __stdcall IsCharLower();
int2 __cdecl16far __export sub_1_0000(void);
int2 __cdecl16far __export sub_1_00B2(void);
int2 __cdecl16far __export sub_1_0116();
int2 __stdcall16far __export LZINIT(int2 a1);
int2 __stdcall16far __export GETEXPANDEDNAME(int2 a1, int2 a2, int2 a3, int2 a4);
int2 __stdcall16far __export LZOPENFILE(int2 a1, void __far * a2, int2 a4, int2 a5);
int2 __stdcall16far __export LZSEEK(int2 a1, int2 a2, int2 a3, int2 a4);
int2 __stdcall16far __export LZREAD(int2 a1, int2 a2, int2 a3, int2 a4);
xunknown2 __stdcall16far __export LZCLOSE(int2 a1);
xunknown2 __stdcall16far __export WEP(int2 a1);
int2 __cdecl16far __export LZSTART(void);
xunknown2 __cdecl16far __export LZDONE(void);
int4 __stdcall16far __export COPYLZFILE(int2 a1, int2 a2);
int4 __stdcall16far __export LZCOPY(int2 a1, int2 a2);
int2 __cdecl16far __export sub_5_0000();
int2 __cdecl16far __export sub_5_0052();
int2 __cdecl16far __export sub_5_00D0();
int2 __cdecl16far __export sub_6_0000();
int2 __cdecl16far __export sub_6_00A4();
int2 __cdecl16far __export sub_7_0000(int2 param_1, int2 param_2);
int2 __cdecl16far __export sub_7_00F4();
int2 __cdecl16far __export sub_7_01E0(code * param_1, xunknown2 param_2, int2 param_3, int2 param_4, int2 param_5, int2 param_6, int2 param_7);
int2 __cdecl16far __export sub_8_00AA();
int2 __cdecl16far __export sub_8_01A2(int2 param_1, xunknown2 param_2);
int2 __cdecl16far __export sub_8_01EC(void __far * param_1, xunknown1 param_3, xunknown1 param_4);
int2 __cdecl16far __export sub_9_0000();
int2 __cdecl16far __export sub_10_0000();
int4 __cdecl16far __export sub_10_004E();
int2 __cdecl16far __export sub_10_00A2(int2 param_1, int2 param_2, int2 param_3, int2 param_4);
int2 __cdecl16far __export sub_10_0124(int2 param_1, int2 param_2);
int4 __cdecl16far __export sub_10_01B6();
int2 __cdecl16far __export sub_10_02A8(int2 param_1, int2 param_2);
uint2 __cdecl16far __export _dos_getftime(xunknown2 param_1, xunknown2 * param_2, xunknown2 * param_3);
uint2 __cdecl16far __export _dos_setftime();
int2 __fastcall16near sub_11_00AB();
int2 __cdecl16far __export sub_11_00D8(void);
int2 __cdecl16far __export sub_11_01B7(xunknown1 param_1);
int2 __cdecl16far __export sub_11_01C5(xunknown1 param_1);
int2 __cdecl16far __export sub_11_01D4(void);
int2 __cdecl16far __export sub_11_01E5(void);
int2 __cdecl16near sub_11_0253(void);
int2 __cdecl16near sub_11_02A0(void);
int2 __stdcall16far __export _NMSG_TEXT(int2 a1);
int2 __stdcall16far __export sub_11_0319(int2 a1);
int2 __cdecl16far _GetDGROUP(void);
int2 __cdecl16far __export ___EXPORTEDSTUB(void);
#ifdef __WATCOMC__
#pragma aux (codebreaker_fastcall) sub_11_00AB;
#endif

/* imports by number:
 *   DOS3CALL                     KERNEL.102
 *   FatalAppExit                 KERNEL.137
 *   FatalExit                    KERNEL.1
 *   GlobalAlloc                  KERNEL.15
 *   GlobalFree                   KERNEL.17
 *   GlobalLock                   KERNEL.18
 *   GlobalUnlock                 KERNEL.19
 *   IsCharLower                  USER.436
 *   IsCharUpper                  USER.435
 *   OpenFile                     KERNEL.74
 *   OutputDebugString            KERNEL.115
 *   __WINFLAGS                   KERNEL.178
 *   _lclose                      KERNEL.81
 *   _lcreat                      KERNEL.83
 *   _llseek                      KERNEL.84
 *   _lopen                       KERNEL.85
 *   _lread                       KERNEL.82
 *   _lwrite                      KERNEL.86
 *   lstrcat                      KERNEL.89
 *   lstrcmp                      USER.430
 *   lstrcpy                      KERNEL.88
 *   lstrlen                      KERNEL.90
 */

/* SDK constants written into lzexpand.c, with the values the SDK headers give them */
#ifndef GMEM_MODIFY
#define GMEM_MODIFY 0x80
#endif
#ifndef GMEM_NOT_BANKED
#define GMEM_NOT_BANKED 0x1000
#endif
