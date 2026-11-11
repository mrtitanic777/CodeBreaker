#include "opnd.h"
#include "ins.h"
extern f0854c();

/* whether the instructions p and q are the same */
f072a7(p, q)
register struct ins *p, *q;
{
    if (p->op == q->op && p->size == q->size && f0854c(p->a, q->a) && f0854c(p->b, q->b))
        return 1;
    return 0;
}
