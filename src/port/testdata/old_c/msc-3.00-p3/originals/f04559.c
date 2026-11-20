#include "targ.h"
#include "seg.h"
extern struct seg *g_11b4;      /* (the segment now) */
extern char *g_116e[];
extern struct seg *f040a4();
extern f017cb();

/* the segment of the variable s made the one now, its size the end of s's room (n bytes) */
f04559(s, n)
register struct targ *s;
{
    int x;                      /* (not used) */

    if (s->kind)
        f017cb(g_116e[0], 553);
    g_11b4 = f040a4(s);
    g_11b4->size = s->off + n;
}
