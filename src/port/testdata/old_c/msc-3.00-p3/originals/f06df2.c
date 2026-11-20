#include "opnd.h"
#include "ins.h"

/* the item after p, past lines and comments */
struct ins *
f06df2(p)
register struct ins *p;
{
    while ((p = p->next) && (p->kind == 2 || p->kind == 8))
        ;
    return p;
}
