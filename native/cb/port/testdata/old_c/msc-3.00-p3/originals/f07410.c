#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern int g_3a1e, g_3aa0, g_1fec;
extern char *g_1e46;
extern struct ins *f06e2c(), *f06df2(), *f06f79();
extern f072a7(), f06ea9(), f02c53(), f071f1(), f07247(), f017cb();

/* (open: block motion - the original moves the "the same instruction?" test after the code that follows) */

/* the jumps to p's label whose code before them is the same as p's: that code taken out, the jump made one to
   a label put before p's copy (branch tail merging) */
f07410(p)
register struct ins *p;
{
    struct ins *q;
    struct ref *refs;
    char same;
    int x1;                     /* (not used) */
    struct ins *t;
    register struct ins *a;
    int w;                      /* (not used) */

    refs = ((struct lab *)p->a->sym)->refs;
    while (refs) {
        if ((q = refs->ins) != p && q->op == 0x3d) {
            same = 0;
            a = p;
            t = q;
            for (;;) {
                a = f06e2c(a);
                t = f06e2c(t);
                if (a->kind || t->kind || !f072a7(a, t))
                    break;
                same = 1;
            }
            if (same && f06e2c(p) != q) {
                if (!f06ea9()) {
                    if (g_3a1e++ > 2)
                        f02c53(0, "procedure too large, skipping branch sequence optimization and continuing.");
                    return;
                }
                f071f1(t->next, q);
                refs = refs->next;
                t = f06f79(f06df2(a));
                f07247(q, t->op);
                if (!g_3aa0)
                    f017cb(g_1e46, 1531);
                g_1fec++;
                continue;
            }
        }
        refs = refs->next;
    }
}
