#include "opnd.h"
#include "ins.h"
#include "lab.h"
extern struct ins *g_35f2[];
extern struct ins *g_3570;      /* (the item looked at now) */
extern int g_3aa0;              /* (something was changed) */
extern char *g_1e46;
extern f07042(), f017cb();

/* the item p taken out of the function (given back to the free ones of its kind) */
f07120(p)
register struct ins *p;
{
    char k;
    register struct ins *l;

    switch ((unsigned char)(k = p->kind)) {
    default:
        f017cb(g_1e46, 1155);
        break;
    case 0:
        if ((p->op == 0x3d || p->op == 0x3f) && p->a->kind == 0xa)
            f07042(p->a->sym, p);
        g_3aa0 = 1;
        p->kind = -1;
        break;
    case 1:
        f07042(p->op, 0);
        if (g_3160 == p)
            g_3160 = LNEXT(p);
        else {
            for (l = g_3160; LNEXT(l) != p; l = LNEXT(l))
                ;
            LNEXT(l) = LNEXT(p);
        }
    case 2:
        g_3aa0 = 1;
        break;
    case 8:
        return;
    }
    p->prev->next = p->next;
    p->next->prev = p->prev;
    if (g_3570 == p)
        g_3570 = p->next;
    p->prev = g_35f2[(unsigned char)k];
    g_35f2[(unsigned char)k] = p;
}
