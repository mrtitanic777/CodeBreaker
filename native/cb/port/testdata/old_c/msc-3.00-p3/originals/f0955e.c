#include "regc.h"
extern char *g_1f7e;
extern f017cb();

/* c taken out of what the register r is known to hold, and given back */
f0955e(c, r)
register struct regc *c;
register r;
{
    struct regc *prev;
    struct regc *p;

    prev = 0;
    for (p = g_1fa8[r]; p && p != c; p = p->next)
        prev = p;
    if (p != c)
        f017cb(g_1f7e, 2555);
    if (prev)
        prev->next = p->next;
    else
        g_1fa8[r] = p->next;
    p->next = g_1fe4;
    g_1fe4 = p;
}
