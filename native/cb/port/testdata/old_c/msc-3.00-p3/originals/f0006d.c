#include "opnd.h"
#include "psym.h"
extern char *g_00ae;
extern f017cb();

/* the operand p given the value of the symbol s for the kind k (0eh, 0bh), else s itself */
f0006d(s, p, k)
struct psym *s;
struct opnd *p;
{
    if (k == 0xe) {
        if (p->kind != 4)
            f017cb(g_00ae, 130);
        p->val = s->l1a;
    } else if (k == 0xb) {
        if (p->kind != 0xb)
            f017cb(g_00ae, 134);
        p->val = s->l16;
    } else
        p->sym = (int)s;
}
