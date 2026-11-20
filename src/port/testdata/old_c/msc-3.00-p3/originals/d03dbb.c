/* MD.c: the machine's description - the operands' kinds, the registers' and the conditions' names */
#include "okind.h"
static char *sccsid = "@(#)MD.c:1.3";
struct okind g_1010[] = {
    { "", 0 },
    { "mr", 4 },
    { "mv", 8 },
    { "msog", 12 },
    { "mdbg", 8 },
    { "msobg", 14 },
    { "mdibg", 14 },
    { "msoibg", 14 },
    { "mso", 12 },
    { "mso", 12 },
    { "ms", 12 },
    { "ms", 8 },
    { "md", 8 },
    { "m#", 14 },
    { "ms", 8 },
    { "m", 12 },
    { "mdg", 8 },
};
char *g_1054[] = {             /* the word registers */
    "ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "es", "cs", "ss", "ds",
};
char *g_106c[] = {             /* the byte registers, then the conditions (of the jumps) */
    "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh",
    "o", "no", "b", "ae", "e", "ne", "be", "a", "s", "ns", "p", "np", "l", "ge", "le", "g",
};
int g_109c[] = {
    0x1, 0x2, 0x64, 0x8, 0x6058, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x5000, 0x2000, 0x4000, 0xa000,
};
