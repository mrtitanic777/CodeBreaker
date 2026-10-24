"""The behaviours of Microsoft C/C++ 8.00c (Visual C++ 1.52c) that the 16-bit Windows 95 Setup
files were built with -- what an OpenWatcom made to generate their code has to do. Each case is a
small source, the switches it was compiled with, and the function(s) whose code is compared.

What the cases hold was learned taking SUWIN.EXE and SETUPX.DLL apart (docs: codebreaker/verify/
match16.py): switches per source file (/G2s /Ox, /G3s /Oxs, /Gs /Od ...), no /Gw; Pascal
arguments pushed left to right; `push cs; call` to a function of the same file; locals at /Od laid
out by a hash of their names, at /Ox by declaration; /G3 pushing a 32-bit constant whole; /Os
removing two bytes of arguments with `pop bx`; Microsoft's _fastcall in AX, DX, BX.
"""

# Both compilers read the same source: what each calls a thing.
HEADER = r"""
#ifdef __WATCOMC__
#define FAR __far
#define NEAR __near
#define PASCAL __pascal
#define CDECL __cdecl
#define LOADDS __loadds
#pragma aux ms_fastcall "@*" parm caller [ax] [dx] [bx] value [ax] modify [ax bx cx dx es];
#define FASTCALL __near
#else
#define FAR __far
#define NEAR __near
#define PASCAL __pascal
#define CDECL __cdecl
#define LOADDS _loadds
#define FASTCALL __near _fastcall
#endif
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef char FAR *LPSTR;
"""

CASES = [
    dict(name="od_frame", flags="/AM /Gs /Od", funcs=["od_frame"],
         what="/Od: a frame (push bp / mov bp,sp / sub sp,N), every local in memory",
         src="int FAR CDECL od_frame(int a, int b) { int x; int y; x = a + 1; y = b * 2; return x + y; }"),
    dict(name="od_local_order", flags="/AM /Gs /Od", funcs=["od_local_order"],
         what="/Od: locals laid out by a hash of their names, not their order",
         src="int FAR CDECL od_local_order(void) { int zeta; int alpha; int mid; zeta = 1; alpha = 2; mid = 3;"
             " return zeta * 100 + alpha * 10 + mid; }"),
    dict(name="ox_local_order", flags="/AM /G2s /Ox", funcs=["ox_local_order"],
         what="/Ox: what is left in memory laid out by declaration",
         src="extern void FAR CDECL take(int NEAR *p);\n"
             "int FAR CDECL ox_local_order(void) { int zeta; int alpha; zeta = 1; alpha = 2; take(&zeta); take(&alpha);"
             " return zeta + alpha; }"),
    dict(name="ox_leaf", flags="/AM /G2s /Ox", funcs=["ox_leaf"],
         what="/Ox: a leaf with its arguments read through a frame (far functions keep push bp)",
         src="int FAR CDECL ox_leaf(int a, int b) { return a < b ? b : a; }"),
    dict(name="g3_push_long", flags="/AM /G3s /Ox", funcs=["g3_push_long"],
         what="/G3: a 32-bit constant argument pushed whole (66 68 imm32)",
         src="extern void FAR CDECL takel(long v);\nvoid FAR CDECL g3_push_long(void) { takel(0x12345678L); }"),
    dict(name="g2_push_long", flags="/AM /G2s /Ox", funcs=["g2_push_long"],
         what="/G2: the same constant pushed as two words, high first",
         src="extern void FAR CDECL takel(long v);\nvoid FAR CDECL g2_push_long(void) { takel(0x12345678L); }"),
    dict(name="os_pop_cleanup", flags="/AM /G2s /Oxs", funcs=["os_pop_cleanup"],
         what="/Os: two bytes of a cdecl call's arguments taken off with pop bx, more with add sp",
         src="extern void FAR CDECL one(int a);\nextern void FAR CDECL three(int a, int b, int c);\n"
             "void FAR CDECL os_pop_cleanup(void) { one(1); three(1, 2, 3); }"),
    dict(name="pascal_order", flags="/AM /G2s /Ox", funcs=["pascal_order"],
         what="FAR PASCAL: arguments pushed left to right, the callee removes them",
         src="extern int FAR PASCAL Callee(int a, int b, int c);\nint FAR CDECL pascal_order(void) { return Callee(1, 2, 3); }"),
    dict(name="pascal_callee", flags="/AM /G2s /Ox", funcs=["PASCALCALLEE"],
         what="FAR PASCAL function: retf N, its arguments read from the far end",
         src="int FAR PASCAL PascalCallee(int a, int b) { return a - b; }"),
    dict(name="push_cs_call", flags="/AM /G2s /Ox", funcs=["push_cs_call"],
         what="a far call to a far function of the same file: push cs; call (near)",
         src="static int FAR CDECL helper(int a) { return a * 3; }\n"
             "int FAR CDECL push_cs_call(int a) { return helper(a) + 1; }"),
    dict(name="far_call_extern", flags="/AM /G2s /Ox", funcs=["far_call_extern"],
         what="a far call to a function elsewhere: lcall with a fix-up",
         src="extern int FAR CDECL elsewhere(int a);\nint FAR CDECL far_call_extern(int a) { return elsewhere(a) + 1; }"),
    dict(name="fastcall", flags="/AM /G2s /Ox", funcs=["fastcall_add", "fastcall_caller"],
         what="Microsoft's _fastcall: a near function's arguments in AX, DX, BX",
         src="#ifdef __WATCOMC__\nint FASTCALL fastcall_add(int a, int b, int c);\n#pragma aux (ms_fastcall) fastcall_add;\n#endif\n"
             "int FASTCALL fastcall_add(int a, int b, int c) { return a + b * c; }\n"
             "int FAR CDECL fastcall_caller(void) { return fastcall_add(4, 5, 6); }"),
    dict(name="return_long", flags="/AM /G2s /Ox", funcs=["return_long"],
         what="a long returned in DX:AX",
         src="long FAR CDECL return_long(int hi, int lo) { return ((long)hi << 16) | (unsigned)lo; }"),
    dict(name="long_mul", flags="/AM /G2s /Ox", funcs=["long_mul"],
         what="long multiply: the run-time helper (__aFlmul) and how its arguments go",
         src="long FAR CDECL long_mul(long a, long b) { return a * b; }"),
    dict(name="far_pointer_param", flags="/AM /G2s /Ox", funcs=["far_pointer_param"],
         what="a far pointer parameter loaded with les",
         src="int FAR CDECL far_pointer_param(LPSTR s) { int n = 0; while (s[n]) n++; return n; }"),
    dict(name="switch_dense", flags="/AM /G2s /Ox", funcs=["switch_dense"],
         what="a dense switch: a jump table in the code segment",
         src="int FAR CDECL switch_dense(int k) { switch (k) { case 0: return 10; case 1: return 21;"
             " case 2: return 32; case 3: return 43; case 4: return 54; case 5: return 65; } return -1; }"),
    dict(name="switch_sparse", flags="/AM /G2s /Ox", funcs=["switch_sparse"],
         what="a sparse switch: compares",
         src="int FAR CDECL switch_sparse(int k) { switch (k) { case 3: return 1; case 100: return 2;"
             " case 1000: return 3; } return 0; }"),
    dict(name="struct_copy", flags="/AM /G2s /Ox", funcs=["struct_copy"],
         what="a structure assigned: rep movsw",
         src="typedef struct { int a[8]; } BIG;\nvoid FAR CDECL struct_copy(BIG FAR *d, BIG FAR *s) { *d = *s; }"),
    dict(name="register_vars", flags="/AM /G2s /Ox", funcs=["register_vars"],
         what="SI and DI kept for variables, saved after the frame",
         src="int FAR CDECL register_vars(int NEAR *p, int n) { int i, t = 0; for (i = 0; i < n; i++) t += p[i]; return t; }"),
    dict(name="char_extend", flags="/AM /G2s /Ox", funcs=["char_extend"],
         what="a signed char widened: cbw",
         src="int FAR CDECL char_extend(char c, unsigned char u) { return c + u; }"),
    dict(name="if_else", flags="/AM /G2s /Ox", funcs=["if_else"],
         what="if/else: which arm comes first, how the branch is turned",
         src="int FAR CDECL if_else(int a) { if (a > 5) return a - 5; else return 5 - a; }"),
    dict(name="global_ds", flags="/AM /G2s /Ox", funcs=["global_ds"],
         what="a global read through DS (a fix-up for its offset)",
         src="int counter;\nint FAR CDECL global_ds(void) { return ++counter; }"),
    dict(name="loadds_export", flags="/AM /G2s /Ox", funcs=["loadds_export"],
         what="_loadds: the function loads its own data segment",
         src="int counter2;\nint FAR CDECL LOADDS loadds_export(void) { return counter2; }"),
    dict(name="od_call_args", flags="/AM /Gs /Od", funcs=["od_call_args"],
         what="/Od: a call's arguments evaluated and pushed one by one, add sp after",
         src="extern int FAR CDECL three(int a, int b, int c);\n"
             "int FAR CDECL od_call_args(int x) { return three(x, x + 1, x * 2); }"),
]
