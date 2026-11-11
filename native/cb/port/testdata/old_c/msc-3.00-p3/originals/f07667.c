#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern int g_1ff2;              /* (conditional jumps over jumps turned) */
extern struct ins *f06e2c(), *f06df2();
extern f07247(), f07120();

/* the jump p after a conditional jump q over it to the label after p: q turned and made a jump to p's label, p
   taken out (and q taken out when it goes where p goes): whether it was */
f07667(p)
register struct ins *p;
{
    struct ins *nx;
    register struct ins *q;

    q = f06e2c(p);
    if (q->kind == 0 && q->op == 0x3f) {
        nx = f06df2(p);
        if (nx->kind == 1 && ((struct lab *)q->a->sym)->def == nx) {
            q->size ^= 1;
            f07247(q, p->a->sym);
            f07120(p);
            g_1ff2++;
            return 1;
        }
        if (q->a->sym == p->a->sym) {
            f07120(q);
            g_1ff2++;
            return 1;
        }
    }
    return 0;
}
