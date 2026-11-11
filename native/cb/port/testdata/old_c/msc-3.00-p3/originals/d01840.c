#include "seg.h"
#include "opnd.h"
#include "ins.h"
char *g_0306 = "@(#)disasm.c:1.32";
int g_0308 = 36;
struct ins g_030a = { 0, 0, 0, 61, 2 };     /* (a jump, for a long conditional jump's second half) */
char g_0318[16] = { 1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14 };   /* (each condition's opposite) */
char *g_0328[] = {             /* the 8087 instructions' names, from 69h */
    "fld", "fld", "fild", "fild", "fst", "fst", "fist", "fist",
    "fstp", "fstp", "fistp", "fistp", "fchs", "fdup", "fadd", "fadd",
    "fiadd", "fiadd", "fadd", "fsub", "fsub", "fisub", "fisub", "fsub",
    "fsubr", "fsubr", "fisubr", "fisubr", "fsubr", "fmul", "fmul", "fimul",
    "fimul", "fmul", "fdiv", "fdiv", "fidiv", "fidiv", "fdiv", "fdivr",
    "fdivr", "fidivr", "fidivr", "fdivr",
};
int g_0380[] = {               /* (their widths) */
    8, 4, 4, 2, 8, 4, 4, 2, 8, 4, 4, 2, 0, 0, 8, 4,
    4, 2, 0, 8, 4, 4, 2, 0, 0, 8, 4, 4, 2, 8, 4, 4,
    2, 0, 8, 4, 4, 2, 0, 0, 8, 4, 4, 2,
};
struct seg *g_03d8 = 0;
