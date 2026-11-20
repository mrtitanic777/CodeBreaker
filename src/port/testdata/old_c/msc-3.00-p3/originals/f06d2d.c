#include "opnd.h"
#include "ins.h"
#include "psym.h"
extern long g_0cb4;
extern char f010f8();

/* the item p placed at at: the place after it */
long
f06d2d(p, at)
register struct ins *p;
long at;
{
    switch (p->kind) {
    case 0:
        at += p->i9 = f010f8(p);
        break;
    case 7:
        at += 2;
        break;
    case 1:
    case 3:
        ((struct psym *)p->op)->place = at;
        break;
    case 6:
        g_0cb4 = at;
        break;
    }
    return at;
}
