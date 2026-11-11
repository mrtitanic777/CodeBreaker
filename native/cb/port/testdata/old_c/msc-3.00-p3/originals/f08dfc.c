#include "opnd.h"
#include "regc.h"
extern char *g_1f7e;
extern f0854c(), f017cb();

/* the register (of the width w) known to hold the operand o: -1 if none */
f08dfc(o, w)
struct opnd *o;
{
    register r;
    int end;
    register struct regc *c;

    if (o->kind == 1)
        return -1;
    if (w == 1) {
        r = 12;
        end = 20;
    } else {
        if (w != 2)
            f017cb(g_1f7e, 2108);
        r = 0;
        end = 12;
    }
    for (; r < end; r++)
        for (c = g_1fa8[r]; c; c = c->next)
            if (f0854c(c->o, o))
                return r;
    return -1;
}
