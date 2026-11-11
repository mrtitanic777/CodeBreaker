#include "regc.h"
extern f095d6();

/* the register r known to hold all that the register s is */
f0968e(r, s)
register r;
{
    register struct regc *c;

    for (c = g_1fa8[s]; c; c = c->next)
        f095d6(r, c->o);
}
