#include "opnd.h"
#include "ins.h"
#include "psym.h"
extern struct ins *g_0c60;

/* the place of the item p: its label's, else the items before it counted from the label before them */
long
f06ccc(p)
register struct ins *p;
{
    int n;

    n = 0;
    if (p->kind == 1)
        return ((struct psym *)p->op)->place;
    while (p != g_0c60) {
        p = p->prev;
        switch (p->kind) {
        case 0:
        case 7:
            n++;
            break;
        case 1:
            return n + ((struct psym *)p->op)->place;
        }
    }
    return n;
}
