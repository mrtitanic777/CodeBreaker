#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern long g_2fce, g_2fd2;
extern char *g_1f7e;
extern f017cb();

/* how far the jump p goes (its label from the end of it, made short) */
f08378(p)
register struct ins *p;
{
    long place;

    if (p->a->o10 != 2)
        f017cb(g_1f7e, 984);
    if ((place = ((struct lab *)p->a->sym)->place) > g_2fd2)
        return abs((int)place - (p->op == 0x3d ? 3 : 5) + (int)g_2fce - (int)g_2fd2);
    return abs((int)place - (int)g_2fd2 - 2);
}
