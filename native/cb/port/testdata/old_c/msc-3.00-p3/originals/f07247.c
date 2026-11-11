#include "opnd.h"
#include "ins.h"
#include "jt.h"
extern char *g_1e46;
extern f07042(), f06ff5(), f017cb();

/* the jump p made a jump to lab */
f07247(p, lab)
struct ins *p;
{
    if (p->kind != 0 && p->kind != 7)
        f017cb(g_1e46, 1250);
    f07042(p->kind == 0 ? p->a->sym : *(int *)&p->size, p);
    f06ff5(p, JT(p)->lab = lab);
}
