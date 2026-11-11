#include "regc.h"
extern int g_3a1e;
extern struct fcon *f03e86();
extern f0950c(), f02c53();

/* the register r known to hold the operand o (if it can be) */
f095d6(r, o)
struct opnd *o;
{
    register i;                 /* (not used) */
    struct regc *c;

    if (!f0950c(r, o))
        return;
    if (g_1fe4) {
        c = g_1fe4;
        g_1fe4 = c->next;
    } else
        c = f03e86(4, 4);
    if (c) {
        c->o = o;
        c->next = g_1fa8[r];
        g_1fa8[r] = c;
    } else if (g_3a1e++ < 2)
        f02c53(0, "recoverable heap overflow in post optimizer - some optimizations may be missed");
}
