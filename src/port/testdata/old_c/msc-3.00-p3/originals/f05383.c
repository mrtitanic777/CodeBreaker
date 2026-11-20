#include "fix.h"
extern struct cell *f03fec();

/* (open: the original loads f again for its first two stores, release 3.00 keeps it in BX) */

/* a fixup of the offset at off to the external idx */
f05383(off, idx)
{
    struct fix *f;

    f = f03fec();
    f->seg = 1;
    f->loc5 = 0;
    f->loc = 1;
    f->off = off;
    f->p = 1;
    f->fthread = 0;
    f->frame = 5;
    f->tthread = 0;
    f->target = 2;
    f->idx = idx;
}
