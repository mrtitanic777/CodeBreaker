#include "targ.h"
#include "fix.h"
extern int g_2f9a, g_2f8c, g_2f98, g_2f8a;     /* (the threads of DGROUP, ...) */
extern char *g_0c6c;
extern char *g_123c;
extern f017cb();

/* (open: NOROOM 5 takes it from 175 differing bytes to 116: the compiler of 3.00 was short of room here too) */

/* the frame and target of the fixup f to the variable s */
f04f85(s, f)
register struct targ *s;
register struct fix *f;
{
    switch (s->flags >> 4 & 0xf) {
    case 4:
        f->fthread = 1;
        f->frame = g_2f9a;
        f->tthread = 1;
        f->target = g_2f8c;
        break;
    case 6:
        if (s->t12 == 4) {
            f->fthread = 1;
            f->frame = g_2f9a;
            f->tthread = 1;
            f->target = g_2f98;
        } else {
            f->fthread = 0;
            f->frame = 5;
            f->tthread = 0;
            f->target = 0;
            f->idx = s->t12;
        }
        break;
    case 2:
        switch (s->flags >> 8 & 0xf) {
        case 2:
        case 3:
            f->fthread = 0;
            f->frame = 5;
            f->tthread = 0;
            f->target = 2;
            f->idx = s->t8;
            break;
        case 1:
        case 4:
            if (s->t12 == 2) {
                if (g_0c6c) {
                    f->fthread = 0;
                    f->frame = 5;
                } else {
                    f->fthread = 1;
                    f->frame = g_2f9a;
                }
                f->tthread = 1;
                f->target = g_2f8a;
            } else {
                f->fthread = 0;
                f->frame = 5;
                f->tthread = 0;
                f->target = 0;
                f->idx = s->t12;
            }
            break;
        default:
            f017cb(g_123c, 872);
            break;
        }
        break;
    default:
        f017cb(g_123c, 877);
        break;
    }
}
