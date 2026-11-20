#include "psym.h"
#include "targ.h"
#include "sflags.h"
#include "recfmt.h"
#define FL(s)   ((struct sflags *)&(s)->se)
extern struct recfmt g_2096[];
extern int g_3572[];
extern char *g_1e1e;
extern struct hsym *f067b5();
extern char *f03f02();
extern f03dad(), f03fc9(), f017cb();

/* (open: the original evaluates the frames' compare left to right; release 3.00 the bit field first) */

/* the symbol s entered in the table tab (its earlier entry, if any, made it): the entry */
struct psym *
f06824(s, tab)
register struct psym *s;
struct psym **tab;
{
    int n;
    register struct psym *p;
    int pool;
    struct psym **slot;

    s->s8 = -1;
    pool = tab == (struct psym **)g_3572 ? 1 : 0;
    if (!(p = f067b5(s->s4, tab))) {
        n = g_2096[s->kind].n;
        slot = &tab[s->s4 % 64];
        if (!(p = f03f02(n, 0, pool)))
            f03dad();
        f03fc9(p, s, n);
        *(struct psym **)p = *slot;
        *slot = p;
    } else {
        if (p->kind != s->kind)
            f017cb(g_1e1e, 297);
        if (p->kind != 2 && p->kind != 0)
            f017cb(g_1e1e, 298);
        *(int *)s = *(int *)p;
        if (p->kind == 2) {
            if (p->cls != s->cls && !(p->cls == 4 && s->cls == 5) && !(p->cls == 4 && s->cls == 3)
                && !(p->cls == 2 && s->cls == 3))
                f017cb(g_1e1e, 307);
            *p = *s;
        } else {
            if ((p->se & 0xf00) != FL(s)->frame << 8
                && !(((p->se & 0xf00) == 0x200 || (p->se & 0xf00) == 0x300)
                    && ((s->se & 0xf00) == 0x100 || (s->se & 0xf00) == 0x300)))
                f017cb(g_1e1e, 313);
            *(struct targ *)p = *(struct targ *)s;
        }
    }
    return p;
}
