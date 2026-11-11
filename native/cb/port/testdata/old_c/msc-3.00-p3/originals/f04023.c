#include "cell.h"
extern struct cell *g_11b2, *g_11b0;

/* the cells of the list p given back */
f04023(p)
struct cell *p;
{
    register struct cell *q;

    if (p == g_11b0)
        g_11b0 = 0;
    if (p) {
        for (q = p; q->next; q = q->next)
            ;
        q->next = g_11b2;
        g_11b2 = p;
    }
}
