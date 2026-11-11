#include "targ.h"
#include "cell.h"
extern int g_356e;              /* (the index of the far communal variables' element size, 1) */
extern int g_1188;              /* (the COMDEF names so far) */
extern struct cell *g_1192, *g_1194;
extern char *f03f02();
extern f0405a(), f048bd(), f040fb(), f042a3(), f03dad();

/* the COMDEF record of the communal variable s (near or far), its size kept */
f04c3e(s)
struct targ *s;
{
    struct cell *c;
    register unsigned n;
    long len;
    struct targ t;
    register struct targ *tp;

    if (s->t12 != 2 && !g_356e) {
        tp = &t;
        tp->t10 = 1;
        tp->t12 = 2;
        tp->flags = tp->flags & 0xf0ff | 0x300;
        f04c3e(tp);
        g_356e = f0405a(tp);
    }
    n = s->t10;
    if (f0405a(s))
        return;
    f048bd(0x8e, 0);
    f040fb(0);
    f040fb(0);
    if (!(c = f03f02(6, 0, 0)))
        f03dad();
    c->idx = g_1188++;
    c->size = n;
    if (s->t12 == 2) {
        f040fb(0x62);
        f040fb(0x7b);
        f042a3(len = (long)n << 3);
        c->next = g_1192;
        g_1192 = c;
    } else {
        f040fb(0x61);
        f040fb(0x77);
        f042a3((long)n);
        f042a3((long)g_356e);
        c->next = g_1194;
        g_1194 = c;
    }
}
