#include "targ.h"
extern char *g_0306;
extern char *f0475f();
extern f01840(), f017cb(), printf();

/* (NOROOM 2 takes it from 56 differing bytes to 39: the compiler of 3.00 was short of room here too) */
/* (open: of the assertions' shared call the original keeps the last copy (the outer default's), release 3.00 the
   first) */

/* the room of the uninitialized variable s in the listing */
f021ff(s)
register struct targ *s;
{
    register sz;

    if ((s->flags & 0xf0) == 0x40 || s->kind || s->flags & 0x1000)
        return;
    sz = s->t10;
    f01840(s->t12);
    switch (s->flags >> 4 & 0xf) {
    case 2:
        switch (s->flags >> 8 & 0xf) {
        case 2:
            break;
        case 4:
            printf("%s\tDB 0%xH DUP (?)\n", f0475f(s), sz);
            break;
        case 3:
            printf(";\t.comm %s,0%xH\n", f0475f(s), sz);
            break;
        default:
            f017cb(g_0306, 681);
            break;
        }
        break;
    case 6:
        if ((s->flags & 0xf00) != 0x100)
            f017cb(g_0306, 686);
        printf("%s\tDB 0%xH DUP (?)\n", f0475f(s), sz);
        break;
    default:
        f017cb(g_0306, 691);
        break;
    }
    if (sz & 1)
        printf("\tEVEN\n");
}
