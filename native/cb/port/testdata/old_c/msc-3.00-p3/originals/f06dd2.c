#include "opnd.h"
#include "ins.h"

/* (open: the original moves q->prev through AX into BX; release 3.00 loads BX) */

/* the item p put before q */
f06dd2(p, q)
register struct ins *p, *q;
{
    p->next = q;
    (p->prev = q->prev)->next = p;
    q->prev = p;
}
