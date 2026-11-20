#include "fix.h"
extern int g_2f92, g_2f86;
extern struct cell *f03fec();

/* a fixup of the offset at off to the code (by its threads) */
f05347(off)
{
    struct fix *f;

    f = f03fec();
    f->seg = 1;
    f->loc5 = 0;
    f->loc = 1;
    f->off = off;
    f->p = 1;
    f->tthread = 1;
    f->target = g_2f92;
    f->fthread = 1;
    f->frame = g_2f86;
}
