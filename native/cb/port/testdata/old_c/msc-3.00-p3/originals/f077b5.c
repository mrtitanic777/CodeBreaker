/* NOROOM: 4 */
#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern int g_3a1e, g_3aa0, g_2000;
extern char *g_1e46;
extern struct ins *f06df2(), *f06e2c(), *f06f79();
/* (NOROOM 4: the compiler of 3.00 had no room for that merging of equal code; it waited and came out the
   other way round) */
extern f072ef(), f08986(), f06ea9(), f06edb(), f02c53(), f07247(), f07667(), f017cb();
extern struct ins *f08460();

/* the loop the jump p closes, entered at its top by a conditional jump to the label after p, turned: its test
   moved to its end (loop inversion); whether it was */
f077b5(p)
struct ins *p;
{
    int how;
    struct lab *tl;
    int nl2;
    struct ref *r;
    struct ins *newl;
    struct ins *jp;
    register struct ins *top;
    register struct ins *test;

    if (!f072ef(p))
        return 0;
    tl = (struct lab *)p->a->sym;
    nl2 = f06df2(p)->op;
    top = f06e2c(tl->def);
    for (test = f06df2(tl->def); p != test; test = f06df2(test)) {
        if (test->kind == 1)
            return 0;
        if (test->kind == 0 && test->op == 0x3f) {
            if (test->a->sym != nl2)
                return 0;
            break;
        }
    }
    if (p == test)
        return 0;
    if ((how = f08986(p, top, test)) == -1)
        return 0;
    if (how != 1 && how != 2)
        return 1;
    top = f06df2(tl->def);
    if (!f06ea9() || !f06edb()) {
        if (g_3a1e++ > 2)
            f02c53(0, "procedure too large, skipping loop inversion optimization and continuing.");
        return 0;
    }
    newl = f06f79(p);
    for (r = tl->refs; r; ) {
        jp = r->ins;
        r = r->next;
        if (jp != p)
            f07247(jp, newl->op);
    }
    if (how == 2)
        jp = (struct ins *)f08460(tl->def, newl->op);
    top->prev->next = test->next;
    test->next->prev = top->prev;
    top->prev = p->prev;
    p->prev->next = top;
    test->next = p;
    p->prev = test;
    if (!f07667(p))
        f017cb(g_1e46, 2030);
    g_3aa0 = 1;
    g_2000++;
    return 1;
}
