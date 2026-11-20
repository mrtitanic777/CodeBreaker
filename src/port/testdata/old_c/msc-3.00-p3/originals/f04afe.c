#include "rec.h"
extern f041e1();

/* the THEADR record of the module name */
f04afe(name)
char *name;
{
    g_360e[0] = 0x80;
    g_3368 = 3;
    f041e1(name);
}
