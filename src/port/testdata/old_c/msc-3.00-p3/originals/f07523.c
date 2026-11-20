#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern int g_3a1e, g_3aa0, g_1fea;
extern char *g_1e46;
extern struct ins *f06e2c(), *f06df2(), *f06f79();
extern f072a7(), f06ea9(), f02c53(), f071f1(), f07247(), f017cb();

/* (open: block motion - the original moves the "the same instruction?" test after the code that follows) */

/* the code before the jump p that is the same as the code before its label taken out, the jump made one to a
   label put before that code (cross jumping) */
f07523(p)
register struct ins *p;
{
    struct ins *lab;
    char ok;
    register struct ins *a;
    struct ins *b;

    lab = ((struct lab *)p->a->sym)->def;
    ok = 0;
    a = p;
    b = lab;
    for (;;) {
        a = f06e2c(a);
        b = f06e2c(b);
        if (a->kind || b->kind || !f072a7(a, b))
            break;
        ok = 1;
    }
    if (!ok || f06e2c(lab) == p)
        return;
    if (!f06ea9()) {
        if (g_3a1e++ > 2)
            f02c53(0, "procedure too large, skipping cross jump optimization and continuing.");
        return;
    }
    f071f1(a->next, p);
    a = f06f79(f06df2(b));
    f07247(p, a->op);
    if (!g_3aa0)
        f017cb(g_1e46, 1632);
    g_1fea++;
}
