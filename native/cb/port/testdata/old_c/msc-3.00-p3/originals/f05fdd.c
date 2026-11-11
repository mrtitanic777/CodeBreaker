#include "opnd.h"
#include "ins.h"
#include "optab.h"
extern struct opnd *f05e92();

/* the operands of the instruction p read from the file f */
f05fdd(p, f)
register struct ins *p;
register f;
{
    int n;

    n = g_17b2[p->op].nops;
    if (n--) {
        p->a = f05e92(f, p->a);
        if (n)
            p->b = f05e92(f, p->b);
    }
}
