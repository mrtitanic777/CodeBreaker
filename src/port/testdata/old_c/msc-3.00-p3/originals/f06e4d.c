#include "opnd.h"
#include "ins.h"
extern struct ins *f06a04();

/* a new instruction op of the width size */
struct ins *
f06e4d(op, size)
char size;
{
    register struct ins *p;

    if (p = f06a04(0)) {
        p->kind = 0;
        p->op = op;
        p->size = size;
    }
    return p;
}
