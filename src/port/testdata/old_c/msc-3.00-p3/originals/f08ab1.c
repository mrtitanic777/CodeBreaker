#include "regc.h"

/* what the register r is known to hold forgotten (its list given back) */
f08ab1(r)
register r;
{
    register struct regc *c;

    for (c = g_1fa8[r]; c; c = c->next)
        if (!c->next) {
            c->next = g_1fe4;
            g_1fe4 = g_1fa8[r];
            break;
        }
    g_1fa8[r] = 0;
}
