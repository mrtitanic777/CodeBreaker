#include "opnd.h"
#include "ins.h"
#include "psym.h"
extern struct psym *g_0cc4;     /* (the procedure) */
extern int g_0c8a;              /* (the line) */
extern long g_0ca8;
extern int g_0c9e, g_0c9c, g_0c58, g_0c80;
extern char *g_0c4c;
extern f006d7(), f03b81(), f01906(), f0596d(), f017cb(), printf();

/* the item p done: its bytes made, listed, and put in the object */
f03ab2(p)
register struct ins *p;
{
    char buf[10];
    register nb;

    if (p->kind == 3)
        g_0cc4 = (struct psym *)p->op;
    else if (p->kind == 2)
        g_0c8a = p->op;
    else if (p->kind == 1 && ((struct psym *)p->op)->place != g_0ca8) {
        printf("codesize inconsistancy, p2 %lx, p3 %x\n", ((struct psym *)p->op)->place, g_0ca8);
        f017cb(g_0c4c, 355);
    }
    nb = f006d7(p, buf);
    if (g_0c9e || g_0c9c) {
        if (g_0c58 && p->kind == 2)
            f03b81(p->op);
        f01906(p, nb, buf, 1);
    }
    if (g_0c80)
        f0596d(p, nb, buf);
    g_0ca8 += nb;
}
