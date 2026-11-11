#include "targ.h"
#include "seg.h"
extern char *g_116e[];
extern f017cb();

/* the segment of the symbol s */
struct seg *
f040a4(s)
struct targ *s;
{
    register struct seg *p;

    for (p = g_11b8; p; p = p->next)
        if (s->t12 == p->num)
            break;
    if (!p)
        f017cb(g_116e[0], 203);
    return p;
}
