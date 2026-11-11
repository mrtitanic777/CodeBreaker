#include "opnd.h"
#include "ins.h"
extern int g_1fee;              /* (dead code removed: how many times) */
extern f07120();

/* the code after the jump p, up to a label, taken out (it is never reached) */
f073d4(p)
register struct ins *p;
{
    char removed;

    removed = 0;
    while ((p = p->next) && p->kind != 1 && p->kind != 6) {
        f07120(p);
        removed = 1;
    }
    if (removed)
        g_1fee++;
}
