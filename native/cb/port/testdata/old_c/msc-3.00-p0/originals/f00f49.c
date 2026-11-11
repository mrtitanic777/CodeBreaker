#include "tokval.h"
extern int g_05d4, g_09d8;
extern char *g_09d4;
extern f00ea0();

f00f49()
{
    union tokval v;

    if (!g_05d4) {
        v.s = g_09d4;
        f00ea0(0x16d, &v);
        v.l = g_09d8;
        f00ea0(0x16e, &v);
    }
}
