#include "tokval.h"
extern int g_05d4;
extern f00ea0();

f01c68(s)
char *s;
{
    union tokval v;

    if (!g_05d4) {
        v.s = s;
        f00ea0(0x16c, &v);
    }
}
