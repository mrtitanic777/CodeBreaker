#include "opnd.h"
#include "ins.h"

/* the instruction after p */
struct ins *
f06e12(p)
register struct ins *p;
{
    while ((p = p->next) && p->kind)
        ;
    return p;
}
