#include "opnd.h"
#include "ins.h"
extern int g_2004, g_3aa0;
extern struct ins *f06e2c(), *f06df2();
extern f0854c(), f07120();

/* the compare p of a register with 0 (after an item that set the flags from it) taken out, and the jump
   after it made sure of where it goes: 1 if so */
f086c5(p)
struct ins *p;
{
    register struct ins *q;
    register struct ins *j;
    int k;
    int x;                      /* (not used) */

    if (!(q = f06e2c(p))->kind) {
        k = q->op;
        if (p->b->kind == 2 && !p->b->val && f0854c(p->a, q->a) && (j = f06df2(p))->op == 0x3f)
            switch (j->size) {
            default:
                if (k == 0x2d || k == 0x2f || k == 0x30) {
                    f07120(p);
                    g_2004++;
                    return 1;
                }
                return 0;
            case 2:
                f07120(p);
                f07120(j);
                g_2004++;
                return g_3aa0 = 1;
            case 3:
                f07120(p);
                j->op = 0x3d;
                j->size = 2;
                j->a->o10 = 2;
                g_2004++;
                return g_3aa0 = 1;
            }
    }
    return 0;
}
