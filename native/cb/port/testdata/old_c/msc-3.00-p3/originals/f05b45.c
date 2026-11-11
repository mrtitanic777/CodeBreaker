#include <stdio.h>
#include "seg.h"
#include "outf.h"
extern int g_0c9a, g_0c82;
extern FILE *g_0c5a;
extern long g_1338, g_0cb4;
extern struct seg *g_11bc;
extern f05bac(), f06988(), f04dd6(), f04f6b(), f04a37(), fseek(), f04b73();

/* the object's end: the PUBDEFs, the last line numbers, MODEND; the code's SEGDEF made again with its true size
   when the post-optimizer changed it */
f05b45()
{
    f06988(f05bac, 0x3a20);
    if (g_0c9a)
        f04dd6(0, 0, 1);
    f04f6b();
    f04a37();
    if (g_0c82) {
        fseek(g_0c5a, g_1338, 0);
        g_11bc->len = g_0cb4;
        f04b73(g_11bc);
        f04a37();
    }
}
