#include "opnd.h"
#include "ins.h"

/* the item before p, past lines and comments */
struct ins *
f06e2c(p)
register struct ins *p;
{
    while ((p = p->prev) && (p->kind == 2 || p->kind == 8))
        ;
    return p;
}
