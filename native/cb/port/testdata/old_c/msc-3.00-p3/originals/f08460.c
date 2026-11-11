#include "opnd.h"
#include "ins.h"
extern char *g_1f7e;
extern struct ins *f06e4d();
extern struct opnd *f06e7b();
extern f06dd2(), f06ff5(), f017cb();

/* a new jump to the label lab put before the item p: the jump */
struct ins *
f08460(p, lab)
register struct ins *p;
register lab;
{
    struct ins *j;
    struct opnd *o;

    j = f06e4d(0x3d, 2);
    o = j->a = f06e7b(10);
    if (!j || !o)
        f017cb(g_1f7e, 1097);
    o->sym = lab;
    o->o10 = 2;
    f06dd2(j, p);
    f06ff5(j, lab);
    return j;
}
