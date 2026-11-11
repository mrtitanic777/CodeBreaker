#include "opnd.h"
#include "ins.h"
extern struct ins *g_0c60;
extern long g_1e4a;
extern long f06d2d();

/* the function's items placed from its beginning */
f06d9a()
{
    long at;
    register struct ins *p;

    p = g_0c60;
    for (at = g_1e4a; p; p = p->next)
        at = f06d2d(p, at);
}
