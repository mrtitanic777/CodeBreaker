/* NOROOM: 2 */
#include "targ.h"
#include "fix.h"
extern int g_3368;
extern int g_123e;
extern int g_2f84, g_2f92;
extern char *g_123c;
extern struct cell *f03fec();
extern f04f85(), f017cb();

/* (NOROOM 2: the compiler of 3.00 had no room for that merging of equal code; it waited and came out the
   other way round) */

/* a fixup of the kind kind to the symbol s at the end of the record */
f05069(s, kind)
register struct targ *s;
{
    register struct fix *f;

    f = f03fec();
    f->loc5 = 0;
    f->loc = kind;
    f->seg = 1;
    switch (kind) {
    case 3:
        f->off = g_3368 - 0xa;
        break;
    case 1:
    case 2:
        f->off = g_3368 - 8;
        break;
    default:
        f017cb(g_123c, 913);
        break;
    }
    f->p = 1;
    f->idx = 0;
    switch (s->kind) {
    case 0:
        f04f85(s, f);
        break;
    case 1:
    case 2:
        f->fthread = 0;
        f->frame = 5;
        if (s->kind == 1 && s->flags != 4) {
            if (g_123e) {
                f->fthread = 1;
                f->frame = g_2f84;
            }
            f->tthread = 1;
            f->target = g_2f92;
        } else {
            f->tthread = 0;
            f->target = 2;
            f->idx = s->t8;
        }
        break;
    default:
        f017cb(g_123c, 946);
        break;
    }
}
