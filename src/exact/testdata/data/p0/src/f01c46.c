#include "tokval.h"
extern int g_09d8;
extern f00ea0();

f01c46()
{
    union tokval v;

    v.l = g_09d8;
    f00ea0(0x16e, &v);
}
