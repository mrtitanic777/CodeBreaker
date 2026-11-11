#include "regc.h"
extern f0854c();

/* the register r known to hold the operand o: 1 if so */
f09657(o, r)
register struct opnd *o;
{
    register struct regc *c;

    for (c = g_1fa8[r]; c; c = c->next)
        if (f0854c(o, c->o))
            return 1;
    return 0;
}
