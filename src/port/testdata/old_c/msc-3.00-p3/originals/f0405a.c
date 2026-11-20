#include "targ.h"
#include "cell.h"
extern struct cell *g_1192, *g_1194;    /* (the communal variables' sizes: near, far) */

/* the index of the communal variable s's size, 0 if s is not one */
f0405a(s)
struct targ *s;
{
    int n;
    register struct cell *p;
    int x;                      /* (not used) */

    if ((s->flags & 0xf00) == 0x300) {
        p = s->t12 == 2 ? g_1192 : g_1194;
        n = s->t10;
        while (p) {
            if (n == p->size)
                return p->idx;
            p = p->next;
        }
    }
    return 0;
}
