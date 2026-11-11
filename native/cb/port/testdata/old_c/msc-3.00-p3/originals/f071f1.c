#include "opnd.h"
#include "ins.h"
extern struct ins *g_3570;
extern int g_3aa0;
extern char *g_1e46;
extern f07120(), f017cb();

/* the items from p up to (not) q taken out */
f071f1(p, q)
register struct ins *p, *q;
{
    struct ins *save;

    if (p == q)
        f017cb(g_1e46, 1208);
    save = g_3570;
    for (; p != q; p = p->next)
        f07120(p);
    if (!g_3aa0)
        f017cb(g_1e46, 1214);
    g_3570 = save;
}
