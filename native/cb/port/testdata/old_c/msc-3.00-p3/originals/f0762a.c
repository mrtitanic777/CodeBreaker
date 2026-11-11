#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern int g_1ff0;              /* (jumps to the next instruction removed) */
extern struct ins *f06df2();
extern f07120();

/* the jump p taken out if it goes to the label just after it: whether it was */
f0762a(p)
register struct ins *p;
{
    register struct ins *q;

    q = f06df2(p);
    if (q->kind == 1 && ((struct lab *)p->a->sym)->def == q) {
        f07120(p);
        g_1ff0++;
        return 1;
    }
    return 0;
}
