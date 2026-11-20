#include "targ.h"
extern char *g_0306;
extern char *f0475f();
extern f01840(), f017cb(), printf();

/* (NOROOM 1 takes it from 85 differing bytes to 79: the compiler of 3.00 was short of room here too) */
/* (open: block motion - the original moves the "else if" test after the code that follows it) */

/* the label of the initialized variable s in the listing, after room of off bytes */
f027e1(s, off)
register struct targ *s;
long off;
{
    if (s->kind)
        f017cb(g_0306, 1006);
    if ((s->flags & 0xf0) != 0x20 && (s->flags & 0xf0) != 0x40)
        f017cb(g_0306, 1007);
    f01840(s->t12);
    if ((s->flags & 0xf00) == 0x100)
        printf("\tPUBLIC\t%s\n", f0475f(s));
    else if ((s->flags & 0xf00) != 0x400)
        f017cb(g_0306, 1012);
    if (off) {
        if (off <= 0)
            f017cb(g_0306, 1014);
        printf("\tORG\t$+%ld\n", off);
    }
    printf("%s", f0475f(s));
}
