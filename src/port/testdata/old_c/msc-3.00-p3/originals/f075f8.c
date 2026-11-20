#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern struct ins *f06e2c();

/* whether the jump p goes to the label just before it */
f075f8(p)
register struct ins *p;
{
    register struct ins *q;

    q = f06e2c(p);
    if (q->kind == 1 && ((struct lab *)p->a->sym)->def == q)
        return 1;
    return 0;
}
