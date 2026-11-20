#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern struct ins *f06a04();
extern struct psym *f06f2d();
extern f06dd2();

/* a new label put before the item p (in the chain of labels after the one before it): 0 if no room */
struct ins *
f06f79(p)
register struct ins *p;
{
    struct ins *b;
    register struct ins *l;

    if (!(l = f06a04(1)))
        return 0;
    l->kind = 1;
    if (!(l->op = (int)f06f2d()))
        return 0;
    ((struct lab *)l->op)->def = l;
    for (b = p; b; b = b->prev)
        if (b->kind == 1) {
            LNEXT(l) = LNEXT(b);
            LNEXT(b) = l;
            break;
        }
    if (!b) {
        LNEXT(l) = g_3160;
        g_3160 = l;
    }
    f06dd2(l, p);
    return l;
}
