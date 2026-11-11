#include "opnd.h"
#include "ins.h"
#define LNEXT(p)    (*(struct ins **)&(p)->size)        /* (a label item's next label) */
extern struct ins *g_0c60;
extern struct ins *g_3160;      /* (the function's first label) */

/* the function's labels chained, in order */
f06c80()
{
    struct ins *last;
    register struct ins *ip;

    last = 0;
    g_3160 = 0;
    for (ip = g_0c60; ip; ip = ip->next)
        if (ip->kind == 1) {
            if (last)
                LNEXT(last) = ip;
            else
                g_3160 = ip;
            last = ip;
        }
    if (last)
        LNEXT(last) = 0;
}
